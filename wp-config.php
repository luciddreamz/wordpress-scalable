<?php
/*
|--------------------------------------------------------------------------
| WordPress Configuration
|--------------------------------------------------------------------------
|
| This file has been configured for use with OpenShift.
|
*/


/*
|--------------------------------------------------------------------------
| WordPress Database Table Prefix
|--------------------------------------------------------------------------
|
| You can have multiple installations in one database if you give each a unique
| prefix. Only numbers, letters, and underscores please!
|
*/

$table_prefix  = 'wp_';

/*
|--------------------------------------------------------------------------
| WordPress Administration Panel
|--------------------------------------------------------------------------
|
| Determine whether the administration panel should be viewed over SSL. We
| prefer to be secure by default.
|
*/

define('FORCE_SSL_ADMIN', true);

/*
|--------------------------------------------------------------------------
| WordPress File Modifications - DO NOT MODIFY
|--------------------------------------------------------------------------
|
| Disallow theme or plugin changes.
|
*/

define('DISALLOW_FILE_MODS', true);

/*
|--------------------------------------------------------------------------
| WordPress Automatice Updates - DO NOT MODIFY
|--------------------------------------------------------------------------
|
| Disable automatic system updates.
|
*/

define('WP_AUTO_UPDATE_CORE', false);

/*
|--------------------------------------------------------------------------
| WordPress Debugging Mode - MODIFICATION NOT RECOMMENDED (see below)
|--------------------------------------------------------------------------
| 
| Set OpenShift's APPLICATION_ENV environment variable in order to enable 
| detailed PHP and WordPress error messaging during development.
|
| Set the variable, then restart your app. Using the `rhc` client:
|
|   $ rhc env set APPLICATION_ENV=development -a <app-name>
|   $ rhc app restart -a <app-name>
|
| Set the variable to 'production' and restart your app to deactivate error 
| reporting.
|
| For more information about the APPLICATION_ENV variable, see:
| https://developers.openshift.com/en/php-getting-started.html#development-mode
|
| WARNING: We strongly advise you NOT to run your application in this mode 
|          in production.
|
*/

define('WP_DEBUG', getenv('APPLICATION_ENV') == 'development' ? true : false);

/*
|--------------------------------------------------------------------------
| MySQL Settings - DO NOT MODIFY
|--------------------------------------------------------------------------
|
| WordPress will automatically connect to your OpenShift MySQL database
| by making use of OpenShift environment variables configured below.
|
| For more information on using environment variables on OpenShift, see:
| https://developers.openshift.com/en/managing-environment-variables.html
|
*/

define('DB_HOST', getenv('OPENSHIFT_MYSQL_DB_HOST') . ':' . getenv('OPENSHIFT_MYSQL_DB_PORT'));
define('DB_NAME', getenv('OPENSHIFT_APP_NAME'));
define('DB_USER', getenv('OPENSHIFT_MYSQL_DB_USERNAME'));
define('DB_PASSWORD', getenv('OPENSHIFT_MYSQL_DB_PASSWORD'));
define('DB_CHARSET', 'utf8');
define('DB_COLLATE', '');

/*
|--------------------------------------------------------------------------
| Authentication Unique Keys and Salts - DO NOT MODIFY
|--------------------------------------------------------------------------
|
| Keys and Salts are automatically configured below.
|
*/

require_once(getenv('OPENSHIFT_REPO_DIR') . '.openshift/openshift.inc');


/*
|--------------------------------------------------------------------------
| That's all, stop editing! Happy blogging.
|--------------------------------------------------------------------------
*/

// Absolute path to the WordPress directory.
if ( !defined('ABSPATH') )
	define('ABSPATH', dirname(__FILE__) . '/');

// Sets up WordPress vars and included files.
require_once(ABSPATH . 'wp-settings.php');
