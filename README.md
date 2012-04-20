## i8krellm-2650 ##

i8krellm-2650 is a fork of i8krellm, the [gkrellm](http://gkrellm.net) plugin by [Eric Johnson](mailto:eric@coding-zone.com) (et al).  
It is __specifically__ intended for the Dell Inspiron 2650, to work under Debian stable (Squeeze) and later.
The code herein may work with other Dell Inspiron/Latitude laptops, in other Linux distributions. Your mileage may vary.

### Goals ###
1.	Restore basic functionality - CPU temp, fan speed, & fan control
2.	Improve automatic fan control
3.	Streamline plugin

### Prerequisites ###
_Incomplete list_

*	i8kutils 1.33 ([Debian-squeeze package & source](http://packages.debian.org/squeeze/i8kutils))
*	i8k kernel module installed (from i8kutils source)
*	libc0.1  >=2.3
*	libc6  >=2.3
* 	tk8.4  
The tk toolkit is a dependency for the i8kutils Debian _package_, but is not utilized in this project.
It created a GUI interface for i8kutils, which is essentially what i8krellm-2650 will do.
*	gkrellm version 2

### Background ###
  Together, i8kutils & the i8k module adds Fn key, fan, and CPU temperature functionality to Dell Inspiron/Latitude laptops.
Eric Johnson's i8krellm-2.5 displays CPU temperature, battery/AC status, fan status & RPM, as well as a few other features.

Debian dropped their gkrellm-i8k package after the Lenny release, so this plugin will not be available for Debian in the future.
But there were a few issues with the Debian Lenny plugin working under Debian Squeeze:

#### Original Issues ####
1.	CPU hang
2.	Automatic fan control broken (related to CPU hang)
3.	Battery/AC display inaccurate
4.	Fan speeds inaccurate
5.	Line graph slow and inaccurate

Auto fan control was done internally by i8krellm-2.5, polling and controlling i8kutils every so often.
This is the major source of CPU hang in the plugin. i8kutils has its own method of auto fan control. This should be utilized in the i8krellm-2650.

The line graph displaying temperature is inaccurate and clutters the plugin. It needs to be rewritten if able, but for now it will be disabled.

Battery/AC display will be removed. Other gkrellm plugins better handle this.