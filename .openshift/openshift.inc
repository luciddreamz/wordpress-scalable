<?php

// Gets the secret token provided by OpenShift 
// Or generates one (this is slightly less secure, but good enough for now)
function get_openshift_secret_token() {
  $my_token = null;

  $token = getenv('OPENSHIFT_SECRET_TOKEN');
  $name  = getenv('OPENSHIFT_APP_NAME');
  $uuid  = getenv('OPENSHIFT_APP_UUID');

  if ($token)
    $my_token = $token;
  elseif ($name && $uuid)
    $my_token = hash('sha256',"$name-$uuid");

  return $my_token;
}

// Loop through all provided variables and generate secure versions
// If not running on OpenShift, returns defaults and logs an error message
//
// This function calls secure_function and passes an array of:
//  { 
//    'hash'     => generated sha hash,
//    'variable' => name of variable,
//    'original' => original value
//  }
function openshift_secure($default_keys,$secure_function = null) {
  // Attempts to get secret token
  $my_token = get_openshift_secret_token();

  // Only generate random values if on OpenShift
  $array = $default_keys;

  // This is the same function run by includes/install.core.inc#install_settings_form_submit
  if ($my_token){
    // Loop over each default_key and set the new value
    foreach ($default_keys as $key => $value) {
      // Create hash out of token and this key's name
      $sha = hash('sha256',"$my_token-$key");
      // Pass an array so we can add stuff without breaking existing calls
      $vals = array(
        'hash' => $sha,
        'variable'  => $key,
        'original'  => $value
      );
      // Call user specified function or just return hash
      $array[$key] = ($secure_function ? call_user_func($secure_function,$vals) : $sha);
    }
  } else {
    // Get the relative path for a more meaningful error message
    $t = debug_backtrace();
    $calling_file = $t[0]['file'];
    // Remove directory to give relative dir for file
    if (getenv('OPENSHIFT_REPO_DIR')) {
      $base = getenv('OPENSHIFT_REPO_DIR');
      $calling_file = str_replace($base,'',$calling_file);
    }
    error_log("OPENSHIFT WARNING: Using default values for secure variables, please manually modify in $calling_file", 0);
  }
  return $array;
}

// this function gets called by openshift_secure and passes an array
function make_secure_key($args) {
  $hash = $args['hash'];
  $key  = $args['variable'];
  $original = $args['original'];

  $chars = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789';
  $chars .= '!@#$%^&*()';
  $chars .= '-_ []{}<>~`+=,.;:/?|';

  // convert the hash to an int to seed the RNG
  srand(hexdec(substr($hash,0,8)));
  // create a random string the same length as the default
  $val = '';
  for($i = 1; $i <= strlen($original); $i++){
    $val .= substr( $chars, rand(0,strlen($chars))-1, 1);
  }
  // reset the RNG
  srand();
  // Set the value
  return $val;
}

$_default_keys = array(
  'AUTH_KEY'          => ' w*lE&r=t-;!|rhdx5}vlF+b=+D>a)R:nTY1Kdrw[~1,xDQS]L&PA%uyZ2:w6#ec',
  'SECURE_AUTH_KEY'   => '}Sd%ePgS5R[KwDxdBt56(DM:0m1^4)-k6_p8}|C:[-ei:&qA)j!X`:7d-krLZM*5',
  'LOGGED_IN_KEY'     => '$l^J?o)!zhp6s[-x^ckF}|BjU4d+(g1as)n/Q^s+k|,ZZc@E^h%Rx@VTm|0|?]6R',
  'NONCE_KEY'         => '#f^JM8d^!sVsq]~|4flCZHdaTy.-I.f+1tc[!h?%-+]U}|_8qc K=k;]mXePl-4v',
  'AUTH_SALT'         => 'I_wL2t!|mSw_z_ zyIY:q6{IHw:R1yTPAO^%!5,*bF5^VX`5aO4]D=mtu~6]d}K?',
  'SECURE_AUTH_SALT'  => '&%j?6!d<3IR%L[@iz=^OH!oHRXs4W|D,VCD7w%TC.uUa`NpOH_XXpGtL$A]{+pv9',
  'LOGGED_IN_SALT'    => 'N<mft[~OZp0&Sn#t(IK2px0{KloRcjvIJ1+]:,Ye]>tb*_aM8P&2-bU~_Z>L/n(k',
  'NONCE_SALT'        => 'u E-DQw%[k7l8SX=fsAVT@|_U/~_CUZesq{v(=y2}#X&lTRL{uOVzw6b!]`frTQ|'
);

// generate OpenShift secure keys (or return defaults if not on OpenShift)
$array = openshift_secure($_default_keys,'make_secure_key');

// Loop through returned values and define them
foreach ($array as $key => $value) {
  define($key, $value);
}
