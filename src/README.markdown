## i8krellm-2650 ##

i8krellm-2650 is a fork of i8krellm, the [gkrellm](http://gkrellm.net)
plugin by [Eric Johnson](mailto:eric@coding-zone.com) (et al).  
Original i8k kernel module and i8k utilities written by
[Massimo Dal Zotto](mailto:dz@debian.org).

This project is **specifically** intended for the Dell Inspiron 2650,
to work under Debian stable (Squeeze) and later.  
The code herein may work with other Dell Inspiron/Latitude laptops, in
other Linux distributions.  
Your mileage may vary.

### Functionality ###
i8krellm-2650 reports the system's current CPU temperature. Fan states
are identified by two animated fans and numerical RPM.
Your Dell Service Tag appears as a tooltip.

The GKrellM plugin periodically polls, at a user-adjustable polling
interval, the /proc/i8k file for the CPU temperature and the fan speeds.
It controls the fan state by calling the i8kfan (i8kctl) utility to con-
trol the fan states.

### GUI configuration ###
All GUI options can be enabled/disabled under the plugin's configuration
window.  You can display your CPU temperature in Celsius or Fahrenheit,
enable/disable the one of the fans, toggle the CPU temperature chart,
display the fan revolution-per-minutes speed, and toggle the mouseover
display of your Dell Service Tag.  Most of these GUI widgets are at the
request of various users of the original i8krellm plugin. If you prefer,
they may be disabled to the point where the original GUI layout can be
displayed.

### Power Status ###
Removed from i8krellm-2650.  
Gkrellm2's built-in Battery plugin does this well already.

### Automatic fan control (hysteresis) ###
Currently removed from i8krellm-2650.

### Temperature Units ###
You may monitor your CPU temperature in either Celsius or Fahrenheit.

### Troubleshooting ###
If you are having problems installing the kernel module, please consult
the various on-line resources for adding kernel modules or rebuilding
your kernel.

*Special thanks to the following,*
	Massimo Dal Zotto for writing the Dell I8Kutils.
	Bill Wilson for GKrellM and the gkrellm-demos and all his help!
	Marcel Mol for /proc/apm, hysteresis, and other improvements.
	Dagfinn Ilmari Mannsåker for bugfixes and Debian package.
	Cole Tuininga for UI suggestions.
	Richard Kroonen and Mike Woods for pointing out that some laptops
		have only one fan and the plugin should allow for that.
	Alan Murphy for suggesting the temperature graphing.
	Ted Yu for finding a bug or two.
	Marc for requesting the RPM display.
	Frank Solensky and Keith Perkins for submitting patches for GKrellM 2.
	John D'Ausilio for submitting the patch for ACPI support.

