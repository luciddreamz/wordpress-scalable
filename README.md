# Scalable WordPress (Experimental) #
WordPress is an open source, semantic, blogging and content management 
platform written in PHP with a MySQL backend focusing on aesthetics, web 
standards, and usability.

The easiest way to install this application is to use the [OpenShift
Instant Application][template]. If you'd like to install it
manually, follow [these directions](#manual-installation).

## OpenShift Considerations ##
These are some special considerations you may need to keep in mind when
running your application on OpenShift.

### Who is this QuickStart for? ###
This is an 'experimental' QuickStart intended for development use only. You should have a strong understanding of WordPress and the implications of horizontal scaling WordPress without a network file system. More on that below.

### Preface ###
The WordPress platform itself is pretty awkward when it comes to horizontal scaling, which is what OpenShift was designed for. Basically, what that means is as web traffic to your application increases, OpenShift would typically scale by adding additional instances of your application to new gears to handle the load. Many modern web frameworks handle this architecture well, but WordPress expects a single (or networked) file system when managing system updates, themes, and plugins. The WordPress system and many plugins also directly modify system files in production and store a corresponding record in the database noting that the change has been made in order to prevent making redundant changes.

Basically that means we have two options when deploying WordPress on OpenShift:

#### Option 1 ####
Deploy WordPress into OpenShift's persistent storage directory (like the original WordPress 4 QuickStart does). This allows end-users to use WordPress in the expected way enabling automatic system updates along with theme/plugin management in the admin console. The original WordPress 4 QuickStart also allows site developers to check-in themes/plugins through version control.

#### Option 2 ####
That's where this QuickStart comes in. 

The second option is to check ALL WordPress files in through version control (the base system, themes, plug-ins, updates, etc). With this option you get the benefit of a normal looking WordPress filesystem and can easily transition between local/remote development. The downside is your entire remote application will live in ephemeral storage. In order to enable scaling, we have to outright disable automatic updates and file modifications on the remote site (setting DISALLOW_FILE_MODS=true and WP_AUTO_UPDATE_CORE=false in the wp-config). Without making this change, the scaled instances of your application would become out of sync with one another as new gears come online.

![Architecture Overview]()

### Likely Issues ###
This is this initial release of this QuickStart, and based on the outline architecture we'll likely run into issues:

#### Images ####
The Amazon S3 images plugin does not publish images directly to Amazon S3. They hit the local file system first. That means that the image files will only exist on one of the several scaled gears. My guess is that when WordPress goes to sync the image to S3 there will be a good chance the gear running the sync process is different than the gear with the image stored locally. This process will likely fail fairly often. Further investigation is needed here.

#### Plugins ####
Some plugins, like W3 Total Cache, try to modify files (like .htaccess) in production. In the W3 Total Cache example, I'm guessing the .htaccess file will only be modified on one gear and a databse entry marking that the modification has been made will prevent .htaccess from being updated on other gears.

### System Updates ###
WP_AUTO_UPDATE_CORE=false

### Installing and Updating Plugins ###
DISALLOW_FILE_MODS=true

### Installing and Updating Themes ###
DISALLOW_FILE_MODS=true

### .htaccess ###
A basic .htaccess configuration file has been included. For more information about htaccess settings for WordPress, please visit [htaccess](https://codex.wordpress.org/htaccess).

### Backup and Restore ###
Use the [OpenShift Backup Server](https://hub.openshift.com/quickstarts/126-openshift-backup-server) to create one-time or scheduled backups for your WordPress site. It's the easiest, most complete way to backup your OpenShift applications using OpenShift's built-in `snapshot` system for backup/restore.

### Development Mode ###
When you develop your WordPress application on OpenShift, you can also enable 
the 'development' environment by setting the `APPLICATION_ENV` environment 
variable using the `rhc` client, like:

```
$ rhc env set APPLICATION_ENV=development -a <app-name>
```

Then, restart your application:

```
$ rhc app restart -a <app-name>
```

If you do so, OpenShift will run your application under 'development' mode.
In development mode, your application will:

* Enable WordPress debugging (sets `WP_DEBUG` to TRUE)
* Show more detailed errors in browser
* Display startup errors
* Enable the [Xdebug PECL extension](http://xdebug.org/)
* Enable [APC stat check](http://php.net/manual/en/apc.configuration.php#ini.apc.stat)
* Ignore your composer.lock file

Set the variable to 'production' and restart your app to deactivate error reporting 
and resume production PHP settings.

Using the development environment can help you debug problems in your application
in the same way as you do when developing on your local machine. However, we 
strongly advise you not to run your application in this mode in production.

## Manual Installation ##

Create a php-5.4 application (you can call your application whatever you want)

    $ rhc app create wordpress php-5.4 mysql-5.5 --from-code=https://github.com/luciddreamz/wordpress -s

That's it, you can now checkout your application at:

    https://wordpress-$yournamespace.rhcloud.com

You'll be prompted to set an admin password and name your WordPress site the first time you visit this
page.

[template]: https://hub.openshift.com/quickstarts/1-wordpress-4