/* i8krellm.c GKrellM plugin for Dell Inspiron notebook fan control
 * Copyright (c) 2011-2012 Matthew Blissett <matthew.r.blissett@gmail.com>
 * Copyright (c) 2001-2003 Eric Johnson <eric@coding-zone.com>
 *
 * This plugin is coded for GKrellm v1.2 (for now), but uses Bill's
 * 'porting' suggestions to support both v1.2 and v2.0.
 * At some point in the future, the v1.2 stuff can be removed.
 *
 * In order to use this plugin, you must be using the SMM kernel
 * BIOS driver and utilities by Massimo Dal Zotto <dz@debian.org>
 * http://www.debian.org/~dz/i8k/
 *
 ***********************************************************************
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 * 
 * On Debian GNU/Linux systems, the complete text of the GNU General
 * Public License can be found in `/usr/share/common-licenses/GPL'.
 *
 */
#include <sys/types.h>   /* fstat() */
#include <sys/stat.h>    /* fstat() */
#include <unistd.h>      /* fstat() */
#include <string.h>      /* strcmp(),etc */
#include <stdlib.h>      /* ? */
#include "prop-anim.xpm"


/* support for both versions */
#ifdef GKRELLM2
	/* use the new GTK2 API */
#	include <gkrellm2/gkrellm.h>
#	define VERSION_MAJOR       GKRELLM_VERSION_MAJOR
#	define VERSION_MINOR       GKRELLM_VERSION_MINOR
#	define VERSION_REV         GKRELLM_VERSION_REV
#	define DEFAULT_STYLE       DEFAULT_STYLE_ID
#	define LABEL_MAX           GKRELLM_LABEL_MAX
#	define init_plugin         gkrellm_init_plugin
#	define Monitor             GkrellmMonitor
#	define TextStyle           GkrellmTextstyle
#	define Decal               GkrellmDecal
#	define Label               GkrellmLabel
#	define Margin              GkrellmMargin
#	define Style               GkrellmStyle
#	define Krell               GkrellmKrell
#	define Panel               GkrellmPanel
#	define ChartConfig         GkrellmChartconfig
#	define ChartLayer          GkrellmChartlayer
#	define Chart               GkrellmChart
#	define ChartData           GkrellmChartdata
#	define DecalButton         GkrellmDecalbutton
#	define Launcher            GkrellmLauncher
#	define AlertDecal          GkrellmAlertdecal
#	define Alert               GkrellmAlert
#	define gkrellm_system      system              /* need output redirection */
#	define gkrellm_configure_panel gkrellm_panel_configure
#	define gkrellm_destroy_panel   gkrellm_panel_destroy
#	define gkrellm_draw_layers     gkrellm_draw_panel_layers
#	define gkrellm_draw_layers_force gkrellm_draw_panel_layers_force
#	define gkrellm_render_to_pixmap    gkrellm_scale_piximage_to_pixmap
#	define gdk_imlib_set_image_border  gkrellm_set_piximage_border
#	define gdk_imlib_create_image_from_xpm_data gkrellm_piximage_new_from_xpm_data
#	define gdk_imlib_load_image    gkrellm_piximage_new_from_file
#	define gdk_imlib_kill_image    gkrellm_destroy_piximage
#	define gdk_imlib_destroy_image gkrellm_destroy_piximage
#	define gdk_imlib_paste_image   gkrellm_paste_piximage
#	define GdkImlibBorder          GkrellmBorder
#	define GdkImlibImage           GkrellmPiximage
#	define gkrellm_chart_bg_image_override gkrellm_chart_bg_piximage_override
#	define gkrellm_panel_bg_image_override gkrellm_panel_bg_piximage_override
#	define gkrellm_load_image              gkrellm_load_piximage
#	define gkrellm_set_image_border        gkrellm_set_gkrellmrc_piximage_border
#	define gkrellm_get_gkrellmrc_image_border gkrellm_get_gkrellmrc_piximage_border
#	define gkrellm_bg_chart_image          gkrellm_bg_chart_piximage
#	define gkrellm_bg_grid_image           gkrellm_bg_grid_piximage
#	define gkrellm_bg_panel_image          gkrellm_bg_panel_piximage
#	define gkrellm_bg_meter_image          gkrellm_bg_meter_piximage
#	define gkrellm_krell_panel_image       gkrellm_krell_panel_piximage
#	define gkrellm_krell_meter_image       gkrellm_krell_meter_piximage
#	define gkrellm_krell_slider_image      gkrellm_krell_slider_piximage
#	define gkrellm_krell_mini_image        gkrellm_krell_mini_piximage
#	define gkrellm_get_decal_alarm_image   gkrellm_get_decal_alarm_piximage
#	define gkrellm_get_decal_warn_image    gkrellm_get_decal_warn_piximage
#	define gkrellm_create_tab          gkrellm_gtk_notebook_page
#	define gkrellm_create_framed_tab   gkrellm_gtk_framed_notebook_page
#	define gkrellm_launcher_table_new  gkrellm_gtk_launcher_table_new
#	define gkrellm_entry_get_text      gkrellm_gtk_entry_get_text
#	define gkrellm_spin_button         gkrellm_gtk_spin_button
#	define gkrellm_check_button        gkrellm_gtk_check_button
#	define gkrellm_check_button_connected gkrellm_gtk_check_button_connected
#	define gkrellm_button_connected    gkrellm_gtk_button_connected
#	define gkrellm_scrolled_vbox       gkrellm_gtk_scrolled_vbox
#	define gkrellm_framed_vbox         gkrellm_gtk_framed_vbox
#	define gkrellm_framed_vbox_end     gkrellm_gtk_framed_vbox_end
#	define gkrellm_scrolled_text       gkrellm_gtk_scrolled_text_view
#	define gkrellm_add_info_text       gkrellm_gtk_text_view_append_strings
#	define gkrellm_add_info_text_string gkrellm_gtk_text_view_append
#	define gkrellm_config_launcher     gkrellm_gtk_config_launcher
#	define gkrellm_remove_and_destroy_decal(p,d) gkrellm_destroy_decal(d)
#	define gkrellm_remove_and_destroy_krell(p,d) gkrellm_destroy_krell(d)
#	define gkrellm_message_window(title, message, widget) gkrellm_message_dialog(title, message)
#	define gkrellm_config_message_window(title, message, widget) gkrellm_config_message_dialog(title, message)

#else
	/* use the original API */
#	include <gkrellm/gkrellm.h>
#endif	/* end ifdef GKRELLM2 */

#define I8K_VERSION_MAJOR     "2"
#define I8K_VERSION_MINOR     "5"

#define PLUGIN_PLACEMENT      (MON_CPU | MON_INSERT_AFTER)
#define PLUGIN_CONFIG_KEYWORD "i8krellm"
#define STYLE_NAME            "i8krellm"
#define IMG_ARRAY_NAME        prop_anim_xpm
#define DFRAMES_COUNT         2
#define SFRAMES_COUNT         4
#define FAN_IMG_WIDTH         18 /* pixels wide */
#define ANIMSPEED             1

/* /proc/i8k vars - see __i8k_get_proc_info() for file format */
#define PROC_I8K              "/proc/i8k"
#define PROC_APM              "/proc/apm"
#define PROC_ACPI             "/proc/acpi/ac_adapter/AC/state"

struct _i8krellm_proc_info {
	gchar version[16];
	gchar bios[16];
	gchar svctag[32];
	gchar cputemp[8];
	gchar lfan_stat[4];
	gchar rfan_stat[4];
	gchar lfan_rpm[16];
	gchar rfan_rpm[16];
	gchar ac_stat[8];
	gchar b_stat[8];
};

static struct      _i8krellm_proc_info _i8k_procinfo;
static gchar       _i8k_procbuf[128];        /* memory we read proc into */
static const gchar *_i8k_delim       = " ";  /* proc field delimiter */

/* graphics plugin vars */
static Monitor     *_i8k_mon         = NULL;
static Panel       *_i8k_panel       = NULL;
static Chart       *_i8k_chart       = NULL;
static ChartData   *_i8k_chartdata   = NULL;
static ChartConfig *_i8k_chartconfig = NULL;
static GdkPixmap   *_i8k_miscimage   = NULL; /* decal_misc.xpm - led */
static GdkBitmap   *_i8k_miscmask    = NULL; /* decal_misc.xpm - led */
static GdkPixmap   *_i8k_propimage   = NULL; /* propellar animation */
static GdkBitmap   *_i8k_propmask    = NULL; /* propellar animation */
static Decal       *_i8k_noprocdecal = NULL;
static Decal       *_i8k_leftfan     = NULL;
static Decal       *_i8k_rightfan    = NULL;
static Decal       *_i8k_cputemp     = NULL;
static Decal       *_i8k_autoled     = NULL;
static Decal       *_i8k_leftrpm     = NULL;
static Decal       *_i8k_rightrpm    = NULL;
static GtkWidget   *_i8k_vbox;
static GtkWidget   *_i8k_unitbutton[2];
static GtkWidget   *_i8k_fanbuttons[3];
static GtkWidget   *_i8k_lflow       = NULL;
static GtkWidget   *_i8k_hyst        = NULL;
static GtkWidget   *_i8k_lfhigh      = NULL;
static GtkWidget   *_i8k_rflow       = NULL;
static GtkWidget   *_i8k_rfhigh      = NULL;
static GtkWidget   *_i8k_bhyst       = NULL;
static GtkWidget   *_i8k_blflow      = NULL;
static GtkWidget   *_i8k_blfhigh     = NULL;
static GtkWidget   *_i8k_brflow      = NULL;
static GtkWidget   *_i8k_brfhigh     = NULL;
static GtkWidget   *_i8k_i8kfan      = NULL;
static GtkWidget   *_i8k_polling     = NULL;
static GtkWidget   *_i8k_disprpm     = NULL;
static GtkWidget   *_i8k_dispchart   = NULL;
static GtkWidget   *_i8k_svctag      = NULL;
static GtkTooltips *_i8k_svctt       = NULL;
static gint        _i8k_styleid;
static gchar *i8krellm_info_text[] = {

N_("<b>i8krellm\n"),
N_("i8krellm is a GKrellM interface to the Dell Inspiron 8000 fan control utilities\n"),
N_("and Linux kernel module written by Massimo Dal Zotto <dz@debian.org>.\n\n"),

N_("i8krellm reports the system's current CPU temperature, AC/BAT status, 'mode'\n"),
N_("LED, and two animated fans and their RPMs.  Your Dell Service Tag appears as\n"),
N_("a tooltip.  There are two modes of operation which can be toggled by clicking\n"),
N_("the temperature button.\n"),
N_("	auto: fan speeds cycle off, low, or high based on current CPU temp.\n"),
N_("	manual: fan states are cycled only when a fan image is clicked.\n\n"),

N_("The GKrellM plugin periodically polls (user adjustable polling interval) the\n"),
N_("/proc/i8k file for the CPU temperature, AC/Battery state (if enabled in\n"),
N_("your i8k kernel module), and the fan speeds.  It controls the fan state by\n"),
N_("calling the i8kfan (i8kctl) utility to control the fan states.\n\n"),

N_("<b>i8krellm GUI configuration:\n"),
N_("All GUI options can be enabled/disabled under the plugin's configuration\n"),
N_("window.  You can display your CPU temperature in Celsius or Fahrenheit,\n"),
N_("enable/disable the one of the fans, toggle the CPU temperature chart,\n"),
N_("display the fan revolution-per-minutes speed, and toggle the mouseover\n"),
N_("display of your Dell Service Tag.  Most of these GUI widgets are at the\n"),
N_("request of various users of the original i8krellm plugin.  If you prefer,\n"),
N_("they may be disabled to the point where the original GUI layout can be\n"),
N_("displayed.\n\n"),

N_("<b>Power Status:\n"),
N_("The AC/BAT state is determined first by using the i8k kernel module (try\n"),
N_("\"insmod i8k power_status=1\"), second by looking at /proc/apm, and lastly\n"),
N_("by looking at /proc/acpi/ac_adapter/AC/state. If no power status can be\n"),
N_("determined, you should use the battery temperature triggers as the default\n"),
N_("triggers.\n\n"),

N_("<b>Temperature Triggers:\n"),
N_("i8krellm supports a hysteresis approach to cycling the fan states.  Under\n"),
N_("the Temps tag you can set the hysteresis value for both AC and BAT states.\n"),
N_("As the CPU temperature exceeds the low value plus the hysteresis value,\n"),
N_("it will enter low speed.  If the CPU temperature falls below the low value\n"),
N_("minus the hysteresis value, it will shut off.  The same approach applies\n"),
N_("to the high temperature values also.\n\n"),

N_("<b>Temperature Units:\n"),
N_("You may monitor your CPU temperature in either Celsius or Fahrenheit, but\n"),
N_("you will need to change the fan temperature triggers accordingly.  They\n"),
N_("do not rescale if you toggle between the two temperature scales.  Also, if\n"),
N_("you change temperature units, the chart will not rescale accordingly either.\n\n"),

N_("<b>Troubleshooting:\n"),
N_("If you are having problems installing the kernel module, please consult the\n"),
N_("various on-line resources for adding kernel modules or rebuilding your kernel.\n"),
N_("If you have successfully built the kernel module (i.e. /proc/i8k exists) and\n"),
N_("you cannot get this module to work, you may contact me for help.\n\n"),

N_("Special thanks to the following,\n"),
N_("	Massimo Dal Zotto for writing the Dell I8Kutils.\n"),
N_("	Bill Wilson for GKrellM and the gkrellm-demos and all his help!\n"),
N_("	Marcel Mol for /proc/apm and hysteresis.\n"),
N_("	Dagfinn Ilmari Mannsåker for bugfixes and Debian package.\n"),
N_("	Cole Tuininga for UI suggestions.\n"),
N_("	Richard Kroonen and Mike Woods for pointing out that some laptops\n"),
N_("		have only one fan and the plugin should allow for that.\n"),
N_("	Alan Murphy for suggesting the temperature graphing.\n"),
N_("	Ted Yu for finding a bug or two.\n"),
N_("	Marc for requesting the RPM display.\n"),
N_("	Frank Solensky and Keith Perkins for submitting patches for GKrellM 2.\n"),
N_("	John D'Ausilio for submitting the patch for ACPI support.\n\n"),

};

/* non-graphics plugin vars */
static gchar       *_i8k_noprocmsg   =
"i8krellm error: missing proc file, see Info tag";
static gint        _i8k_hasprocfile  = 0;
#define PWR_I8K  0 /* get power state from /proc/i8k */
#define PWR_APM  1 /* get power state from PROC_APM */
#define PWR_ACPI 2 /* get power state from PROC_ACPI */
static gint        _i8k_powerref = PWR_I8K;

struct _i8krellm_fan {
	gint  frame;
	gint  state;
	gint  prev_state;
	gint  ac_low_temp;
	gint  ac_high_temp;
	gint  bat_low_temp;
	gint  bat_high_temp;
};
static struct _i8krellm_fan _i8k_lf = {
	0,0,0,40,45,50,55
};

static struct _i8krellm_fan _i8k_rf = {
	0,0,0,45,50,55,65
};


#define I8K_RESOLUTION 2  /* how may degrees for grid resolution */
struct _i8krellm_chart_settings {
	gint  grids;
	gint  lower_bound;
};
static struct _i8krellm_chart_settings _i8k_chart_settings;

struct _i8krellm_guiopts {
	gchar fan_cmd[128];
	gchar i8kfan_loc[128];
	gint  enabled_fans; /* 0->L, 1->R, 2->Both */
	gint  cpuchart_enabled;
	gint  fanspeed_enabled;
	gint  svctag_enabled;
	gint  temp_units; /* 0->C, 1->F */
	gint  my_ticks;
	gint  poll_interval;
	gint  ac_hysteresis;
	gint  bat_hysteresis;
};
static struct _i8krellm_guiopts _i8k_guiopts = {
	"","/usr/bin/i8kfan",2,1,1,1,0,10,5,5,5
};

struct _i8krellm_state {
	gchar temp_disp[12];
	gint  update;
	gint  ac_power;
	gint  auto_mode;
	gint  panel_height_rpm;
	gint  panel_height_norpm;
};

static struct _i8krellm_state _i8k_state = {
	"",1,0,1
};


static void __i8k_draw_chart(void) {
	static gchar temp[12];

	memset(temp,0,12);
	strcpy(temp,"\\t");
	strcat(temp,&_i8k_state.temp_disp[0]);
	gkrellm_draw_chartdata(_i8k_chart);
	gkrellm_draw_chart_text(_i8k_chart,_i8k_styleid,temp);
	gkrellm_draw_chart_to_screen(_i8k_chart);
}

static gint __i8k_chart_expose_event(GtkWidget *widget, GdkEventExpose *ev) {
	gdk_draw_pixmap(widget->window,
	  widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
	  _i8k_chart->pixmap, ev->area.x, ev->area.y, ev->area.x, ev->area.y,
	  ev->area.width, ev->area.height);
	return FALSE;
}

static gint __i8k_panel_expose_event(GtkWidget *widget, GdkEventExpose *ev) {
	gdk_draw_pixmap(widget->window,
	  widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
	  _i8k_panel->pixmap, ev->area.x, ev->area.y, ev->area.x, ev->area.y,
	  ev->area.width, ev->area.height);
	return FALSE;
}

/* shut fans off when gkrellm closes */
static void __i8k_fan_off() {
	if (_i8k_hasprocfile) {
		memset(&_i8k_guiopts.fan_cmd[0],0,128);
		snprintf(&_i8k_guiopts.fan_cmd[0],sizeof(_i8k_guiopts.fan_cmd)-1,"%s 0 0 >/dev/null &",
		  &_i8k_guiopts.i8kfan_loc[0]);
#ifndef FAKE
		gkrellm_system(&_i8k_guiopts.fan_cmd[0]);
#endif
	}
	return;
}

/* callback for mousebutton-3 */
static void __i8k_cb_chart_press(GtkWidget *w, GdkEventButton *e, gpointer d) {
	if (e->button == 3)
		gkrellm_chartconfig_window_create(_i8k_chart);
	return;
}

/* callback for mousebutton-3 */
static gint __i8k_cb_panel_press(GtkWidget *w, GdkEventButton *ev) {
	if (ev->button == 3)
		gkrellm_open_config_window(_i8k_mon);
	return TRUE;
}

static gint __i8k_get_proc_apminfo() {
	FILE *fp;
	char *tok = NULL;
	char apm_buf[128];

	if ((fp = fopen(PROC_APM,"r")) == NULL) {
		/* assume battery */
		_i8k_state.ac_power = 0;
		return 0;
	}

	memset(&apm_buf[0],0,128);
	if (fread(&apm_buf[0],sizeof(gchar),128,fp) == 0) {
		/* assume battery */
		_i8k_state.ac_power = 0;
		return 0;
	}
	fclose(fp);

	/* format of /proc/apm at the time of writing was:
   * 1.15 1.2 0x03 0x01 0x03 0x09 100% 345 min
   * |    |   |    |    |    |    |    |   
   * |    |   |    |    |    |    |    +------ 8.  Remaining time on battery
   * |    |   |    |    |    |    +----------- 7.  Battery charge percentage
   * |    |   |    |    |    +---------------- 6.  ?
   * |    |   |    |    +--------------------- 5.  ?
   * |    |   |    +-------------------------- 4.  1 = AC , 0 = BATT?
   * |    |   +------------------------------- 3.  ?
   * |    +----------------------------------- 2.  ?
   * +---------------------------------------- 1.  /proc/apm format version
   */
	tok = strtok(&apm_buf[0],_i8k_delim);
	tok = strtok(NULL,_i8k_delim);
	tok = strtok(NULL,_i8k_delim);
	tok = strtok(NULL,_i8k_delim);
	if (strcmp(tok,"0x01") == 0) {
		strcpy(&_i8k_procinfo.ac_stat[0], "1");
		_i8k_state.ac_power = 1;
	} else {
		strcpy(&_i8k_procinfo.ac_stat[0], "0");
		_i8k_state.ac_power = 0;
	}

	return 1;
}

static gint __i8k_get_proc_acpiinfo() {
	FILE *fp;
	char *tok = NULL;
	char acpi_buf[128];

	if ((fp = fopen(PROC_ACPI,"r")) == NULL) {
		/* assume battery */
		_i8k_state.ac_power = 0;
		return 0;
	}

	memset(&acpi_buf[0],0,128);
	if (fread(&acpi_buf[0],sizeof(gchar),128,fp) == 0) {
		/* assume battery */
		_i8k_state.ac_power = 0;
		return 0;
	}
	fclose(fp);

	/* format of /proc/acpi/ac-adapter/AC/state  at the time of writing was:

	*/
	tok = strtok(&acpi_buf[0],_i8k_delim);
	tok = strtok(NULL,_i8k_delim);
	if (strncmp(tok,"on-line",7) == 0) {
		strcpy(&_i8k_procinfo.ac_stat[0], "1");
		_i8k_state.ac_power = 1;
	} else {
		strcpy(&_i8k_procinfo.ac_stat[0], "0");
		_i8k_state.ac_power = 0;
	}

	return 1;
}

/* read and store info in /proc/i8k */
static gint __i8k_get_proc_info() {
	gint temp;
	FILE *fp;
	char *tok = NULL;

#ifdef FAKE
	strcpy(&_i8k_procbuf[0],"1.0 A17 82S5Q01 44 0 1 0 7200 1 0");
	fp = NULL;
#else
	if ((fp = fopen(PROC_I8K,"r")) == NULL) {
		return 0;
	}

	memset(&_i8k_procbuf[0],0,128);
	if (fread(&_i8k_procbuf[0],sizeof(gchar),128,fp) == 0) {
		return 0;
	}
	fclose(fp);
#endif

	/* format of /proc/i8k at the time of writing was:
   * 1.0 A17 2J59L02 52 2 1 8040 6420 1 2
   * |   |   |       |  | | |    |    | |
   * |   |   |       |  | | |    |    | +------- 10. buttons status
   * |   |   |       |  | | |    |    +--------- 9.  ac status
   * |   |   |       |  | | |    +-------------- 8.  right fan rpm
   * |   |   |       |  | | +------------------- 7.  left fan rpm
   * |   |   |       |  | +--------------------- 6.  right fan status
   * |   |   |       |  +----------------------- 5.  left fan status
   * |   |   |       +-------------------------- 4.  cpu temperature
   * |   |   +---------------------------------- 3.  serial number
   * |   +-------------------------------------- 2.  bios version
   * +------------------------------------------ 1.  /proc/i8k format version
   */
	tok = strtok(&_i8k_procbuf[0],_i8k_delim);
	strcpy(&_i8k_procinfo.version[0],tok);

	tok = strtok(NULL,_i8k_delim);
	strcpy(&_i8k_procinfo.bios[0],tok);

	tok = strtok(NULL,_i8k_delim);
	strcpy(&_i8k_procinfo.svctag[0],tok);

	tok = strtok(NULL,_i8k_delim);
	temp = atoi(tok);
	if (_i8k_guiopts.temp_units) {
		temp = (gint)(temp*9/5+32);
	}
	sprintf(&_i8k_procinfo.cputemp[0],"%d",temp);

	tok = strtok(NULL,_i8k_delim);
	strcpy(&_i8k_procinfo.lfan_stat[0],tok);

	tok = strtok(NULL,_i8k_delim);
	strcpy(&_i8k_procinfo.rfan_stat[0],tok);

	tok = strtok(NULL,_i8k_delim);
	strcpy(&_i8k_procinfo.lfan_rpm[0],tok);

	tok = strtok(NULL,_i8k_delim);
	strcpy(&_i8k_procinfo.rfan_rpm[0],tok);

	tok = strtok(NULL,_i8k_delim);
	strcpy(&_i8k_procinfo.ac_stat[0],tok);

	tok = strtok(NULL,_i8k_delim);
	strcpy(&_i8k_procinfo.b_stat[0],tok);

	if (strcmp(&_i8k_procinfo.ac_stat[0],"-1") == 0) {
		/* need to check other /proc files */
		if (_i8k_powerref == PWR_ACPI) {
			(void) __i8k_get_proc_acpiinfo();
		} else if (_i8k_powerref == PWR_APM) {
			(void) __i8k_get_proc_apminfo();
		} else {
			_i8k_state.ac_power = 0;
		}
	} else if (strcmp(&_i8k_procinfo.ac_stat[0],"1") == 0) {
		_i8k_state.ac_power = 1;
	} else {
		_i8k_state.ac_power = 0;
	}

	return 1;
}

static void __i8k_load_animation(gint has_proc) {
	GdkImlibImage *image = NULL;

	if (has_proc) {
		gkrellm_load_image(NULL, IMG_ARRAY_NAME, &image, NULL);
		gkrellm_render_to_pixmap(image, &_i8k_propimage, &_i8k_propmask, 0, 0);
		_i8k_miscimage = gkrellm_decal_misc_pixmap();
		_i8k_miscmask = gkrellm_decal_misc_mask();
	}
	return;
}

static void __i8k_fan_check() {
	gint temp;

	if (_i8k_state.auto_mode) {
		temp = atoi(&_i8k_procinfo.cputemp[0]);
		_i8k_lf.prev_state = atoi(&_i8k_procinfo.lfan_stat[0]);
		_i8k_rf.prev_state = atoi(&_i8k_procinfo.rfan_stat[0]);
		_i8k_lf.state = 0; _i8k_rf.state = 0;
		if (_i8k_state.ac_power) {
			if (temp <= _i8k_lf.ac_low_temp - _i8k_guiopts.ac_hysteresis) _i8k_lf.state = 0;
			else if (temp >= _i8k_lf.ac_high_temp + _i8k_guiopts.ac_hysteresis) _i8k_lf.state = 2;
			else if ((temp >= _i8k_lf.ac_low_temp + _i8k_guiopts.ac_hysteresis && _i8k_lf.prev_state == 0) ||
			  (temp <= _i8k_lf.ac_high_temp - _i8k_guiopts.ac_hysteresis && _i8k_lf.prev_state == 2))
			    _i8k_lf.state = 1;
			else _i8k_lf.state = _i8k_lf.prev_state;

			if (temp <= _i8k_rf.ac_low_temp - _i8k_guiopts.ac_hysteresis) _i8k_rf.state = 0;
			else if (temp >= _i8k_rf.ac_high_temp + _i8k_guiopts.ac_hysteresis) _i8k_rf.state = 2;
			else if ((temp >= _i8k_rf.ac_low_temp + _i8k_guiopts.ac_hysteresis && _i8k_rf.prev_state == 0) ||
			  (temp <= _i8k_rf.ac_high_temp - _i8k_guiopts.ac_hysteresis && _i8k_rf.prev_state == 2))
			    _i8k_rf.state = 1;
			else _i8k_rf.state = _i8k_rf.prev_state;
		} else {
			if (temp <= _i8k_lf.bat_low_temp - _i8k_guiopts.bat_hysteresis) _i8k_lf.state = 0;
			else if (temp >= _i8k_lf.bat_high_temp + _i8k_guiopts.bat_hysteresis) _i8k_lf.state = 2;
			else if ((temp >= _i8k_lf.bat_low_temp + _i8k_guiopts.bat_hysteresis && _i8k_lf.prev_state == 0)
			  || (temp <= _i8k_lf.bat_high_temp - _i8k_guiopts.bat_hysteresis && _i8k_lf.prev_state == 2))
			    _i8k_lf.state = 1;
			else _i8k_lf.state = _i8k_lf.prev_state;

			if (temp <= _i8k_rf.bat_low_temp - _i8k_guiopts.bat_hysteresis) _i8k_rf.state = 0;
			else if (temp >= _i8k_rf.bat_high_temp + _i8k_guiopts.bat_hysteresis) _i8k_rf.state = 2;
			else if ((temp >= _i8k_rf.bat_low_temp + _i8k_guiopts.bat_hysteresis && _i8k_rf.prev_state == 0)
			  || (temp <= _i8k_rf.bat_high_temp - _i8k_guiopts.bat_hysteresis && _i8k_rf.prev_state == 2))
			    _i8k_rf.state = 1;
			else _i8k_rf.state = _i8k_rf.prev_state;
		}
		if (_i8k_lf.prev_state != _i8k_lf.state || _i8k_rf.prev_state != _i8k_rf.state) {
			memset(&_i8k_guiopts.fan_cmd[0],0,128);

			if (_i8k_guiopts.enabled_fans == 0) {        /* left fan only */
				snprintf(&_i8k_guiopts.fan_cmd[0],sizeof(_i8k_guiopts.fan_cmd)-1,"%s %d - >/dev/null &",
				  &_i8k_guiopts.i8kfan_loc[0],_i8k_lf.state);
			} else if (_i8k_guiopts.enabled_fans == 1) { /* right fan only */
				snprintf(&_i8k_guiopts.fan_cmd[0],sizeof(_i8k_guiopts.fan_cmd)-1,"%s - %d >/dev/null &",
				  &_i8k_guiopts.i8kfan_loc[0],_i8k_rf.state);
			} else {                                     /* both fans */
				snprintf(&_i8k_guiopts.fan_cmd[0],sizeof(_i8k_guiopts.fan_cmd)-1,"%s %d %d >/dev/null &",
				  &_i8k_guiopts.i8kfan_loc[0],_i8k_lf.state,_i8k_rf.state);
			}
#ifndef FAKE
			gkrellm_system(&_i8k_guiopts.fan_cmd[0]);
#endif
		}
	} else {
		if (_i8k_lf.prev_state != _i8k_lf.state || _i8k_rf.prev_state != _i8k_rf.state) {
			memset(&_i8k_guiopts.fan_cmd[0],0,128);
			if (_i8k_guiopts.enabled_fans == 0) {        /* left fan only */
				snprintf(&_i8k_guiopts.fan_cmd[0],sizeof(_i8k_guiopts.fan_cmd)-1,"%s %d - >/dev/null &",
				  &_i8k_guiopts.i8kfan_loc[0],_i8k_lf.state);
			} else if (_i8k_guiopts.enabled_fans == 1) { /* right fan only */
				snprintf(&_i8k_guiopts.fan_cmd[0],sizeof(_i8k_guiopts.fan_cmd)-1,"%s - %d >/dev/null &",
				  &_i8k_guiopts.i8kfan_loc[0],_i8k_rf.state);
			} else {                                     /* both fans */
				snprintf(&_i8k_guiopts.fan_cmd[0],sizeof(_i8k_guiopts.fan_cmd)-1,"%s %d %d >/dev/null &",
				  &_i8k_guiopts.i8kfan_loc[0],_i8k_lf.state,_i8k_rf.state);
			}
#ifndef FAKE
			gkrellm_system(&_i8k_guiopts.fan_cmd[0]);
#endif
			_i8k_lf.prev_state = _i8k_lf.state;
			_i8k_rf.prev_state = _i8k_rf.state;
		}
	}
	return;
}

static void __i8k_fan_update(struct _i8krellm_fan* fan) {
	_i8k_state.auto_mode = 0;
	if (fan->state == 0) {
		fan->state = 1;
		++fan->frame;
		fan->frame %= SFRAMES_COUNT;
	} else if (fan->state == 1) {
		fan->state = 2;
		++fan->frame;
		fan->frame %= DFRAMES_COUNT;
		fan->frame += SFRAMES_COUNT;
	} else {
		fan->state = 0;
		fan->frame %= SFRAMES_COUNT;
	}
	__i8k_get_proc_info();
	__i8k_fan_check();
	gkrellm_draw_decal_pixmap(_i8k_panel, _i8k_leftfan, _i8k_lf.frame);
	gkrellm_draw_decal_pixmap(_i8k_panel, _i8k_rightfan, _i8k_rf.frame);
	/* show/hide the fans */
	if (_i8k_guiopts.enabled_fans == 0 || _i8k_guiopts.enabled_fans == 2) {
		gkrellm_make_decal_visible(_i8k_panel, _i8k_leftfan);
	} else {
		gkrellm_make_decal_invisible(_i8k_panel, _i8k_leftfan);
	}
	if (_i8k_guiopts.enabled_fans == 1 || _i8k_guiopts.enabled_fans == 2) {
		gkrellm_make_decal_visible(_i8k_panel, _i8k_rightfan);
	} else {
		gkrellm_make_decal_invisible(_i8k_panel, _i8k_rightfan);
	}
	
	gkrellm_draw_decal_pixmap(_i8k_panel,_i8k_autoled,D_MISC_LED0); /* off */

	if (_i8k_guiopts.fanspeed_enabled) {
		gkrellm_draw_decal_text(_i8k_panel,_i8k_leftrpm,
		  _i8k_procinfo.lfan_rpm,-1);
		gkrellm_draw_decal_text(_i8k_panel,_i8k_rightrpm,
		  _i8k_procinfo.rfan_rpm,-1);
		if (_i8k_guiopts.enabled_fans == 0 || _i8k_guiopts.enabled_fans == 2) {
			gkrellm_make_decal_visible(_i8k_panel, _i8k_leftrpm);
		} else {
			gkrellm_make_decal_invisible(_i8k_panel, _i8k_leftrpm);
		}
		if (_i8k_guiopts.enabled_fans == 1 || _i8k_guiopts.enabled_fans == 2) {
			gkrellm_make_decal_visible(_i8k_panel, _i8k_rightrpm);
		} else {
			gkrellm_make_decal_invisible(_i8k_panel, _i8k_rightrpm);
		}
	}
	gkrellm_draw_layers(_i8k_panel);
	return;
}    
static void __i8k_lf_callback(DecalButton *button) {
	__i8k_fan_update(&_i8k_lf);
}

static void __i8k_rf_callback(DecalButton *button) {
	__i8k_fan_update(&_i8k_rf);
}

static void __i8k_mode_callback(DecalButton *button) {
	if (_i8k_state.auto_mode) {
		_i8k_state.auto_mode=0;
		gkrellm_draw_decal_pixmap(_i8k_panel,_i8k_autoled,D_MISC_LED0); /* off */
	} else {
		_i8k_state.auto_mode=1;
		gkrellm_draw_decal_pixmap(_i8k_panel,_i8k_autoled,D_MISC_LED1); /* on */
	}
	__i8k_fan_check();
	if (_i8k_lf.state == 2) {
		_i8k_lf.frame %= DFRAMES_COUNT;
		_i8k_lf.frame += SFRAMES_COUNT;
	} else {
		_i8k_lf.frame %= SFRAMES_COUNT;
	}
	if (_i8k_rf.state == 2) {
		_i8k_rf.frame %= DFRAMES_COUNT;
		_i8k_rf.frame += SFRAMES_COUNT;
	} else {
		_i8k_rf.frame %= SFRAMES_COUNT;
	}
	gkrellm_draw_decal_pixmap(_i8k_panel, _i8k_rightfan, _i8k_rf.frame);
	gkrellm_draw_decal_pixmap(_i8k_panel, _i8k_leftfan, _i8k_lf.frame);
	/* show/hide the fans */
	if (_i8k_guiopts.enabled_fans == 0 || _i8k_guiopts.enabled_fans == 2) {
		gkrellm_make_decal_visible(_i8k_panel, _i8k_leftfan);
	} else {
		gkrellm_make_decal_invisible(_i8k_panel, _i8k_leftfan);
	}
	if (_i8k_guiopts.enabled_fans == 1 || _i8k_guiopts.enabled_fans == 2) {
		gkrellm_make_decal_visible(_i8k_panel, _i8k_rightfan);
	} else {
		gkrellm_make_decal_invisible(_i8k_panel, _i8k_rightfan);
	}
	
	gkrellm_draw_layers(_i8k_panel);
	return;
}

static void __i8k_calculate_chart_settings() {
	gint upper=0;
	gint lower=999;

	if (_i8k_lf.ac_high_temp > upper) upper = _i8k_lf.ac_high_temp;
	if (_i8k_lf.bat_high_temp > upper) upper = _i8k_lf.bat_high_temp;
	if (_i8k_rf.ac_high_temp > upper) upper = _i8k_rf.ac_high_temp;
	if (_i8k_rf.bat_high_temp > upper) upper = _i8k_rf.bat_high_temp;

	if (_i8k_lf.ac_low_temp < lower) lower = _i8k_lf.ac_low_temp;
	if (_i8k_lf.bat_low_temp < lower) lower = _i8k_lf.bat_low_temp;
	if (_i8k_rf.ac_low_temp < lower) lower = _i8k_rf.ac_low_temp;
	if (_i8k_rf.bat_low_temp < lower) lower = _i8k_rf.bat_low_temp;

	lower -= I8K_RESOLUTION;
	upper += I8K_RESOLUTION;
	_i8k_chart_settings.lower_bound = lower;
	_i8k_chart_settings.grids = ((upper - lower) / I8K_RESOLUTION);
	return;
}

static void i8krellm_create(GtkWidget *vbox, gint first_create) {
	Style *style = NULL;
#ifdef GKRELLM2
	GkrellmMargin *margin;
#endif
	gint style_width;
	gint rfan_x=0;
	gint rfan_y=0;
	gint lfan_x=0;
	gint lfan_y=0;
	gint led_x=0;
	gint led_y=0;
	gint cpu_x=0;
	gint cpu_y=0;

	if (first_create) {
		_i8k_panel = gkrellm_panel_new0();
		_i8k_chart = gkrellm_chart_new0();
		_i8k_hasprocfile = __i8k_get_proc_info();
		__i8k_calculate_chart_settings();
	}

	__i8k_load_animation(_i8k_hasprocfile);

	style = gkrellm_meter_style(_i8k_styleid);
#ifdef GKRELLM2
	margin = gkrellm_get_style_margins(style);
	style_width = (int)(margin->left + margin->right + margin->top + margin->bottom)/4;
#else
	style_width = style->margin;
#endif
	_i8k_panel->textstyle = gkrellm_meter_textstyle(_i8k_styleid);

	if (_i8k_hasprocfile) {

		gkrellm_set_chart_height_default(_i8k_chart,40);

		gkrellm_chart_create(vbox,_i8k_mon,_i8k_chart,&_i8k_chartconfig);

		_i8k_chartdata = gkrellm_add_default_chartdata(_i8k_chart,
		  "CPU Temperature");

		gkrellm_monotonic_chartdata(_i8k_chartdata,FALSE);

		gkrellm_set_chartdata_draw_style_default(_i8k_chartdata,CHARTDATA_LINE);

		gkrellm_set_chartdata_flags(_i8k_chartdata,CHARTDATA_NO_CONFIG_INVERTED);

		gkrellm_set_draw_chart_function(_i8k_chart,__i8k_draw_chart,NULL);

		gkrellm_set_chartconfig_flags(_i8k_chartconfig,
		  NO_CONFIG_AUTO_GRID_RESOLUTION | NO_CONFIG_FIXED_GRIDS);
		gkrellm_set_chartconfig_auto_grid_resolution(_i8k_chartconfig, FALSE);

		gkrellm_set_chartconfig_fixed_grids(_i8k_chartconfig,
		  _i8k_chart_settings.grids);
		gkrellm_set_chartconfig_grid_resolution(_i8k_chartconfig, I8K_RESOLUTION);

		gkrellm_alloc_chartdata(_i8k_chart);

		cpu_x = style_width;
		cpu_y = style_width;

		if (_i8k_guiopts.cpuchart_enabled) {
			_i8k_cputemp = gkrellm_create_decal_text(_i8k_panel, "Automatic",
		 	 _i8k_panel->textstyle, style,cpu_x,cpu_y,0);

		} else {
			_i8k_cputemp = gkrellm_create_decal_text(_i8k_panel, "580 F  BAT",
		 	 _i8k_panel->textstyle, style,cpu_x,cpu_y,0);
		}

		led_x = cpu_x + _i8k_cputemp->w + style_width;
		led_y = style_width;
		_i8k_autoled = gkrellm_create_decal_pixmap(_i8k_panel,_i8k_miscimage,
		  _i8k_miscmask,N_MISC_DECALS,style,led_x,led_y);
		lfan_y = cpu_y + _i8k_cputemp->h + style_width;

		lfan_x = (gkrellm_chart_width() / 4) - (FAN_IMG_WIDTH / 2);
		_i8k_leftfan = gkrellm_create_decal_pixmap(_i8k_panel,_i8k_propimage,
		  _i8k_propmask,SFRAMES_COUNT+DFRAMES_COUNT,style,lfan_x,lfan_y);

		rfan_x = (gkrellm_chart_width() * 3 / 4) - (FAN_IMG_WIDTH / 2);
		rfan_y = lfan_y;
		_i8k_rightfan = gkrellm_create_decal_pixmap(_i8k_panel,_i8k_propimage,
		  _i8k_propmask,SFRAMES_COUNT+DFRAMES_COUNT,style,rfan_x,rfan_y);


		lfan_y = lfan_y + _i8k_leftfan->h + style_width;
		rfan_y = lfan_y;

		_i8k_state.panel_height_norpm = lfan_y;

		rfan_x = rfan_x - style_width;
		lfan_x = lfan_x - style_width;
		_i8k_leftrpm = gkrellm_create_decal_text(_i8k_panel,
		  "0000", _i8k_panel->textstyle,
		  style, lfan_x, lfan_y, 0);
		_i8k_rightrpm = gkrellm_create_decal_text(_i8k_panel,
		  "0000", _i8k_panel->textstyle,
		  style, rfan_x, rfan_y, 0);

		_i8k_state.panel_height_rpm = lfan_y + _i8k_leftrpm->h + style_width;

	} else {
		_i8k_noprocdecal = gkrellm_create_decal_text(_i8k_panel,"Ay",
		  _i8k_panel->textstyle,style,-1,-1,-1);
	}

	if (_i8k_hasprocfile) {
#ifdef GKRELLM2
		gkrellm_panel_configure_set_height(_i8k_panel,
				_i8k_guiopts.fanspeed_enabled ?
				_i8k_state.panel_height_rpm : _i8k_state.panel_height_norpm);
#else
		_i8k_panel->label->h_panel =
				_i8k_guiopts.fanspeed_enabled ?
				_i8k_state.panel_height_rpm : _i8k_state.panel_height_norpm;
#endif
	} else {
		gkrellm_panel_configure(_i8k_panel,NULL,style);
	}
	gkrellm_panel_create(vbox,_i8k_mon,_i8k_panel);

	if (_i8k_hasprocfile) {

		gkrellm_make_decal_button(_i8k_panel,_i8k_leftfan,__i8k_lf_callback,
		  NULL, D_MISC_BUTTON_IN, D_MISC_BUTTON_OUT);

		gkrellm_make_decal_button(_i8k_panel,_i8k_rightfan,__i8k_rf_callback,
		  NULL, D_MISC_BUTTON_IN, D_MISC_BUTTON_OUT);

#ifdef GKRELLM2
		gkrellm_put_decal_in_meter_button(_i8k_panel,_i8k_cputemp,
		  __i8k_mode_callback,NULL,margin);
#else
		gkrellm_put_decal_in_meter_button(_i8k_panel,_i8k_cputemp,
		  __i8k_mode_callback,NULL);
#endif

		if (first_create) {
			gtk_signal_connect(GTK_OBJECT(_i8k_panel->drawing_area),
			  "expose_event", (GtkSignalFunc) __i8k_panel_expose_event, NULL);
			gtk_signal_connect(GTK_OBJECT(_i8k_panel->drawing_area),
			  "button_press_event", (GtkSignalFunc) __i8k_cb_panel_press, NULL);

			gtk_signal_connect(GTK_OBJECT(_i8k_chart->drawing_area),
			  "expose_event", (GtkSignalFunc) __i8k_chart_expose_event, NULL);
			gtk_signal_connect(GTK_OBJECT(_i8k_chart->drawing_area),
			  "button_press_event", (GtkSignalFunc) __i8k_cb_chart_press, NULL);

			_i8k_svctt = gtk_tooltips_new();
		}

		gkrellm_draw_decal_pixmap(_i8k_panel,_i8k_leftfan,0);
		gkrellm_draw_decal_pixmap(_i8k_panel,_i8k_rightfan,0);
		if (_i8k_guiopts.fanspeed_enabled) {
			gkrellm_draw_decal_text(_i8k_panel,_i8k_leftrpm,
			  _i8k_procinfo.lfan_rpm,-1);
			gkrellm_draw_decal_text(_i8k_panel,_i8k_rightrpm,
			  _i8k_procinfo.rfan_rpm,-1);
			if (_i8k_guiopts.enabled_fans == 0 || _i8k_guiopts.enabled_fans == 2) {
				gkrellm_make_decal_visible(_i8k_panel, _i8k_leftrpm);
			} else {
				gkrellm_make_decal_invisible(_i8k_panel, _i8k_leftrpm);
			}
			if (_i8k_guiopts.enabled_fans == 1 || _i8k_guiopts.enabled_fans == 2) {
				gkrellm_make_decal_visible(_i8k_panel, _i8k_rightrpm);
			} else {
				gkrellm_make_decal_invisible(_i8k_panel, _i8k_rightrpm);
			}
		}

		if (_i8k_state.auto_mode) {
			gkrellm_draw_decal_pixmap(_i8k_panel,_i8k_autoled,D_MISC_LED1); /* on */
		} else {
			gkrellm_draw_decal_pixmap(_i8k_panel,_i8k_autoled,D_MISC_LED0);
		}

		if (_i8k_guiopts.cpuchart_enabled) {
			gkrellm_draw_decal_text(_i8k_panel,_i8k_cputemp,"Automatic",0);
		} else {
			gkrellm_draw_decal_text(_i8k_panel,_i8k_cputemp,"580 F  BAT",0);
		}

		if (_i8k_guiopts.svctag_enabled) {
			gtk_tooltips_set_tip(_i8k_svctt,_i8k_panel->drawing_area,
			  &_i8k_procinfo.svctag[0],"");
			gtk_tooltips_enable(_i8k_svctt);
		} else {
			gtk_tooltips_disable(_i8k_svctt);
		}
		/* show/hide the fans */
		if (_i8k_guiopts.enabled_fans == 0 || _i8k_guiopts.enabled_fans == 2) {
			gkrellm_make_decal_visible(_i8k_panel, _i8k_leftfan);
		} else {
			gkrellm_make_decal_invisible(_i8k_panel, _i8k_leftfan);
		}
		if (_i8k_guiopts.enabled_fans == 1 || _i8k_guiopts.enabled_fans == 2) {
			gkrellm_make_decal_visible(_i8k_panel, _i8k_rightfan);
		} else {
			gkrellm_make_decal_invisible(_i8k_panel, _i8k_rightfan);
		}
	
	} else {
		if (first_create) {
			gtk_signal_connect(GTK_OBJECT(_i8k_panel->drawing_area),
			  "expose_event", (GtkSignalFunc) __i8k_panel_expose_event,
			  NULL);
		}
		gkrellm_draw_decal_text(_i8k_panel, _i8k_noprocdecal, _i8k_noprocmsg, 0);
	}
	gkrellm_draw_panel_layers(_i8k_panel);
	return;
}

static void i8krellm_create_plugin(GtkWidget *vbox, gint first_create) {

	struct stat fstats;

	if (first_create)
		atexit(__i8k_fan_off);

	/* assume we can get power state from /proc/i8k, but let's decide
	   which fallback to use; PROC_APM or PROC_ACPI */
	if (stat(PROC_APM, &fstats) != -1) {
		_i8k_powerref = PWR_APM;
	} else if (stat(PROC_ACPI, &fstats) != -1) {
		_i8k_powerref = PWR_ACPI;
	} else {
		_i8k_powerref = PWR_I8K;
	}

	_i8k_vbox = vbox;
	i8krellm_create(vbox, first_create);
}

static void i8krellm_update_plugin() {
	static gint x_scroll, w;
	gint t;
#ifdef GKRELLM2
	GkrellmTicks *GKp = gkrellm_ticks();
#endif

	if (_i8k_hasprocfile) {
		/* the ui as intended */
		if (_i8k_guiopts.cpuchart_enabled) {
			gkrellm_chart_show(_i8k_chart,TRUE);
		} else {
			gkrellm_chart_hide(_i8k_chart,TRUE);
		}

		t = atoi(_i8k_procinfo.cputemp) - _i8k_chart_settings.lower_bound;

#ifdef GKRELLM2
		if (GKp->second_tick) {
#else
		if (GK.second_tick) {
#endif
			gkrellm_store_chartdata(_i8k_chart,0,t);
			__i8k_draw_chart();
			if ( ++_i8k_guiopts.my_ticks >= _i8k_guiopts.poll_interval) {
			    _i8k_guiopts.my_ticks = 0;
			    __i8k_get_proc_info();
			    _i8k_state.update=1;
			}
		}

#ifdef GKRELLM2
		if ((_i8k_lf.state || _i8k_rf.state) && GKp->timer_ticks % ANIMSPEED == 0) {
#else
		if ((_i8k_lf.state || _i8k_rf.state) && GK.timer_ticks % ANIMSPEED == 0) {
#endif
			_i8k_state.update=1;
			if (_i8k_lf.state) {
				++_i8k_lf.frame;
			}
			if (_i8k_rf.state) {
				++_i8k_rf.frame;
			}
		}
		__i8k_fan_check();
		if (_i8k_lf.state == 2) {
			_i8k_lf.frame %= DFRAMES_COUNT;
			_i8k_lf.frame += SFRAMES_COUNT;
		} else {
			_i8k_lf.frame %= SFRAMES_COUNT;
		}
		if (_i8k_rf.state == 2) {
			_i8k_rf.frame %= DFRAMES_COUNT;
			_i8k_rf.frame += SFRAMES_COUNT;
		} else {
			_i8k_rf.frame %= SFRAMES_COUNT;
		}
		if (_i8k_state.update) {
			/* update temp-display strings */
			memset(&_i8k_state.temp_disp[0],0,12);
			strcpy(&_i8k_state.temp_disp[0],&_i8k_procinfo.cputemp[0]);
			if (_i8k_guiopts.temp_units) {
				strcat(&_i8k_state.temp_disp[0]," F");
			} else {
				strcat(&_i8k_state.temp_disp[0]," C");
			}
			if (_i8k_state.ac_power) {
				strcat(&_i8k_state.temp_disp[0],"  AC");
			} else {
				strcat(&_i8k_state.temp_disp[0],"  BAT");
			}

			if (_i8k_guiopts.cpuchart_enabled) {
				/* if chart, use auto/manual button */
				if (_i8k_state.auto_mode) {
					gkrellm_draw_decal_text(_i8k_panel,_i8k_cputemp,"Automatic",-1);
				} else {
					gkrellm_draw_decal_text(_i8k_panel,_i8k_cputemp,"Manual",-1);
				}
			} else {
				/* if no chart, use old gui temp-display-button-mode-led */
				gkrellm_draw_decal_text(_i8k_panel,_i8k_cputemp,&_i8k_state.temp_disp[0],-1);
				if (_i8k_state.auto_mode) {
					gkrellm_draw_decal_pixmap(_i8k_panel,_i8k_autoled,D_MISC_LED1);/*on*/
				} else {
					gkrellm_draw_decal_pixmap(_i8k_panel,_i8k_autoled,D_MISC_LED0);/*off*/
				}
			}
			gkrellm_draw_decal_pixmap(_i8k_panel,_i8k_leftfan,_i8k_lf.frame);
			gkrellm_draw_decal_pixmap(_i8k_panel,_i8k_rightfan,_i8k_rf.frame);
			if (_i8k_guiopts.fanspeed_enabled) {
				gkrellm_draw_decal_text(_i8k_panel,_i8k_leftrpm,
				  _i8k_procinfo.lfan_rpm,-1);
				gkrellm_draw_decal_text(_i8k_panel,_i8k_rightrpm,
				  _i8k_procinfo.rfan_rpm,-1);
				if (_i8k_guiopts.enabled_fans == 0 || _i8k_guiopts.enabled_fans == 2) {
					gkrellm_make_decal_visible(_i8k_panel, _i8k_leftrpm);
				} else {
					gkrellm_make_decal_invisible(_i8k_panel, _i8k_leftrpm);
				}
				if (_i8k_guiopts.enabled_fans == 1 || _i8k_guiopts.enabled_fans == 2) {
					gkrellm_make_decal_visible(_i8k_panel, _i8k_rightrpm);
				} else {
					gkrellm_make_decal_invisible(_i8k_panel, _i8k_rightrpm);
				}
			}

			/* show/hide the fans */
			if (_i8k_guiopts.enabled_fans == 0 || _i8k_guiopts.enabled_fans == 2) {
				gkrellm_make_decal_visible(_i8k_panel, _i8k_leftfan);
			} else {
				gkrellm_make_decal_invisible(_i8k_panel, _i8k_leftfan);
			}
			if (_i8k_guiopts.enabled_fans == 1 || _i8k_guiopts.enabled_fans == 2) {
				gkrellm_make_decal_visible(_i8k_panel, _i8k_rightfan);
			} else {
				gkrellm_make_decal_invisible(_i8k_panel, _i8k_rightfan);
			}
	
			gkrellm_draw_layers(_i8k_panel);
			_i8k_state.update=0;
		}
	} else {
		/* the ui if no support for our kernel mod */
		if (w == 0) {
			w = gkrellm_chart_width();
		}
		x_scroll = (x_scroll + 1) % (4 * w);
		_i8k_noprocdecal->x_off = w - x_scroll;
		gkrellm_draw_decal_text(_i8k_panel,_i8k_noprocdecal,_i8k_noprocmsg,
		  w - x_scroll);
		gkrellm_draw_layers(_i8k_panel);
		_i8k_state.update=0;
	}
	return;
}

static void i8krellm_create_tab(GtkWidget *tab_vbox) {
	GtkWidget *tabs;
	GtkWidget *table, *vbox, *vbox1, *hbox;
	GtkWidget *button;
	GtkWidget *label;
	GtkWidget *text;
	GSList *group;
	gchar *about_text;
	gint i;

	tabs = gtk_notebook_new();
	gtk_notebook_set_tab_pos(GTK_NOTEBOOK(tabs), GTK_POS_TOP);
	gtk_box_pack_start(GTK_BOX(tab_vbox), tabs, TRUE, TRUE, 0);

	/* -- setup -- */
	vbox = gkrellm_create_framed_tab(tabs, _("Setup"));
	vbox1 = gkrellm_framed_vbox(vbox,_("Temperature Units"), 4, FALSE, 0, 2);
	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox1), hbox, FALSE, FALSE, 0);
	button = gtk_radio_button_new_with_label(NULL, _("Celsius"));
	gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);
	group = gtk_radio_button_group(GTK_RADIO_BUTTON (button));
	_i8k_unitbutton[0] = button;
	button = gtk_radio_button_new_with_label(group, _("Fahrenheit"));
	gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);
	group = gtk_radio_button_group(GTK_RADIO_BUTTON (button));
	_i8k_unitbutton[1] = button;
	button = _i8k_unitbutton[_i8k_guiopts.temp_units];
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), TRUE);

	vbox1 = gkrellm_framed_vbox(vbox,_("Fan Animation"), 4, FALSE, 0, 2);
	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox1), hbox, FALSE, FALSE, 0);

	button = gtk_radio_button_new_with_label(NULL, _("Left Only"));
	gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);
	group = gtk_radio_button_group(GTK_RADIO_BUTTON (button));
	_i8k_fanbuttons[0] = button;

	button = gtk_radio_button_new_with_label(group, _("Right Only"));
	gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);
	group = gtk_radio_button_group(GTK_RADIO_BUTTON (button));
	_i8k_fanbuttons[1] = button;

	button = gtk_radio_button_new_with_label(group, _("Both Fans"));
	gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);
	group = gtk_radio_button_group(GTK_RADIO_BUTTON (button));
	_i8k_fanbuttons[2] = button;

	button = _i8k_fanbuttons[_i8k_guiopts.enabled_fans];
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), TRUE);

	vbox1 = gkrellm_framed_vbox_end(vbox,_("GUI Options"),4,FALSE,0,2);
	table = gtk_table_new(4,2,TRUE);
	gtk_table_set_col_spacings(GTK_TABLE(table), 2);
	gtk_box_pack_start(GTK_BOX(vbox1),table,FALSE,FALSE,2);
	hbox = gtk_hbox_new(TRUE,0);
	gtk_table_attach_defaults(GTK_TABLE(table), hbox, 0,1,0,1);
	label = gtk_label_new(_("i8kfan utility:"));
	gtk_misc_set_alignment(GTK_MISC(label),1.0,0.5);
	gtk_box_pack_start(GTK_BOX(hbox),label,TRUE,TRUE,4);
	_i8k_i8kfan = gtk_entry_new_with_max_length(128);
	gtk_table_attach_defaults(GTK_TABLE(table),_i8k_i8kfan,1,2,0,1);
	gtk_entry_set_text(GTK_ENTRY(_i8k_i8kfan),&_i8k_guiopts.i8kfan_loc[0]);
	hbox = gtk_hbox_new(TRUE,0);
	gtk_table_attach_defaults(GTK_TABLE(table), hbox, 0, 1, 1, 2);
	label = gtk_label_new(_("Polling interval:"));
	gtk_misc_set_alignment(GTK_MISC(label), 1.0, 0.5);
	gtk_box_pack_start(GTK_BOX(hbox), label, TRUE, TRUE, 4);
	hbox = gtk_hbox_new(FALSE,0);
	gtk_table_attach_defaults(GTK_TABLE(table), hbox, 1, 2, 1, 2);
	gkrellm_spin_button(hbox, &_i8k_polling, (gfloat) _i8k_guiopts.poll_interval,
	  1.0, 120.0, 1.0, 5.0, 0, 60, NULL, NULL, FALSE, _(" seconds"));

	hbox = gtk_hbox_new(TRUE,0);
	gtk_table_attach_defaults(GTK_TABLE(table), hbox, 0, 1, 2, 3);
	gkrellm_check_button(hbox, &_i8k_disprpm, _i8k_guiopts.fanspeed_enabled, TRUE, 0,
	  _("Display fan speeds"));
	hbox = gtk_hbox_new(TRUE,0);
	gtk_table_attach_defaults(GTK_TABLE(table), hbox, 1, 2, 2, 3);
	gkrellm_check_button(hbox, &_i8k_dispchart, _i8k_guiopts.cpuchart_enabled, TRUE, 0,
	  _("Enable temperature chart"));
	hbox = gtk_hbox_new(TRUE,0);
	gtk_table_attach_defaults(GTK_TABLE(table), hbox, 0, 2, 3, 4);
	gkrellm_check_button(hbox, &_i8k_svctag, _i8k_guiopts.svctag_enabled, TRUE, 0,
	  _("Show Dell Service Tag as a tooltip on mouseover on panel"));
	
	/* -- temperature settings -- */
	vbox = gkrellm_create_framed_tab(tabs, _("Temps"));
	vbox1 = gkrellm_framed_vbox(vbox, _("AC Power Temperature Triggers"),
	  4, FALSE, 0, 2);
	table = gtk_table_new(3,3,TRUE);
	gtk_table_set_col_spacings(GTK_TABLE(table),2);
	gtk_box_pack_start(GTK_BOX(vbox1), table, FALSE, FALSE, 2);
	hbox = gtk_hbox_new(TRUE,0);
	gtk_table_attach_defaults(GTK_TABLE(table), hbox, 0, 1, 0, 1);
	label = gtk_label_new(_("Left Fan:"));
	gtk_misc_set_alignment(GTK_MISC(label), 1.0, 0.5);
	gtk_box_pack_start(GTK_BOX(hbox), label, TRUE, TRUE, 4);
	hbox = gtk_hbox_new(FALSE,0);
	gtk_table_attach_defaults(GTK_TABLE(table), hbox, 1, 2, 0, 1);
	gkrellm_spin_button(hbox, &_i8k_lflow, (gfloat) _i8k_lf.ac_low_temp,
	  10.0, 200.0, 1.0, 5.0, 0, 60, NULL, NULL, FALSE, _(" low"));
	hbox = gtk_hbox_new(FALSE,0);
	gtk_table_attach_defaults(GTK_TABLE(table), hbox, 2, 3, 0, 1);
	gkrellm_spin_button(hbox, &_i8k_lfhigh, (gfloat) _i8k_lf.ac_high_temp,
	  10.0, 200.0, 1.0, 5.0, 0, 60, NULL, NULL, FALSE, _(" high"));
	hbox = gtk_hbox_new(TRUE,0);
	gtk_table_attach_defaults(GTK_TABLE(table), hbox, 0, 1, 1, 2);
	label = gtk_label_new(_("Right Fan:"));
	gtk_misc_set_alignment(GTK_MISC(label), 1.0, 0.5);
	gtk_box_pack_start(GTK_BOX(hbox), label, TRUE, TRUE, 4);
	hbox = gtk_hbox_new(FALSE,0);
	gtk_table_attach_defaults(GTK_TABLE(table), hbox, 1, 2, 1, 2);
	gkrellm_spin_button(hbox, &_i8k_rflow, (gfloat) _i8k_rf.ac_low_temp,
	  10.0, 200.0, 1.0, 5.0, 0, 60, NULL, NULL, FALSE, _(" low"));
	hbox = gtk_hbox_new(FALSE,0);
	gtk_table_attach_defaults(GTK_TABLE(table), hbox, 2, 3, 1, 2);
	gkrellm_spin_button(hbox, &_i8k_rfhigh, (gfloat) _i8k_rf.ac_high_temp,
	  10.0, 200.0, 1.0, 5.0, 0, 60, NULL, NULL, FALSE, _(" high"));
	hbox = gtk_hbox_new(TRUE,0);
	gtk_table_attach_defaults(GTK_TABLE(table), hbox, 0, 1, 2, 3);
	label = gtk_label_new(_("Hysteresis:"));
	gtk_misc_set_alignment(GTK_MISC(label), 1.0, 0.5);
	gtk_box_pack_start(GTK_BOX(hbox), label, TRUE, TRUE, 4);
	hbox = gtk_hbox_new(FALSE,0);
	gtk_table_attach_defaults(GTK_TABLE(table), hbox, 1, 2, 2, 3);
	gkrellm_spin_button(hbox, &_i8k_hyst, (gfloat) _i8k_guiopts.ac_hysteresis,
	  00.0, 20.0, 1.0, 5.0, 0, 60, NULL, NULL, FALSE, NULL);

	vbox1 = gkrellm_framed_vbox(vbox, _("Battery Power Temperature Triggers"),
	  4, FALSE, 0, 2);
	table = gtk_table_new(3,3,TRUE);
	gtk_table_set_col_spacings(GTK_TABLE(table),2);
	gtk_box_pack_start(GTK_BOX(vbox1), table, FALSE, FALSE, 2);
	hbox = gtk_hbox_new(TRUE,0);
	gtk_table_attach_defaults(GTK_TABLE(table), hbox, 0, 1, 0, 1);
	label = gtk_label_new(_("Left Fan:"));
	gtk_misc_set_alignment(GTK_MISC(label), 1.0, 0.5);
	gtk_box_pack_start(GTK_BOX(hbox), label, TRUE, TRUE, 4);
	hbox = gtk_hbox_new(FALSE,0);
	gtk_table_attach_defaults(GTK_TABLE(table), hbox, 1, 2, 0, 1);
	gkrellm_spin_button(hbox, &_i8k_blflow, (gfloat) _i8k_lf.bat_low_temp,
	  10.0, 200.0, 1.0, 5.0, 0, 60, NULL, NULL, FALSE, _(" low"));
	hbox = gtk_hbox_new(FALSE,0);
	gtk_table_attach_defaults(GTK_TABLE(table), hbox, 2, 3, 0, 1);
	gkrellm_spin_button(hbox, &_i8k_blfhigh, (gfloat) _i8k_lf.bat_high_temp,
	  10.0, 200.0, 1.0, 5.0, 0, 60, NULL, NULL, FALSE, _(" high"));
	hbox = gtk_hbox_new(TRUE,0);
	gtk_table_attach_defaults(GTK_TABLE(table), hbox, 0, 1, 1, 2);
	label = gtk_label_new(_("Right Fan:"));
	gtk_misc_set_alignment(GTK_MISC(label), 1.0, 0.5);
	gtk_box_pack_start(GTK_BOX(hbox), label, TRUE, TRUE, 4);
	hbox = gtk_hbox_new(FALSE,0);
	gtk_table_attach_defaults(GTK_TABLE(table), hbox, 1, 2, 1, 2);
	gkrellm_spin_button(hbox, &_i8k_brflow, (gfloat) _i8k_rf.bat_low_temp,
	  10.0, 200.0, 1.0, 5.0, 0, 60, NULL, NULL, FALSE, _(" low"));
	hbox = gtk_hbox_new(FALSE,0);
	gtk_table_attach_defaults(GTK_TABLE(table), hbox, 2, 3, 1, 2);
	gkrellm_spin_button(hbox, &_i8k_brfhigh, (gfloat) _i8k_rf.bat_high_temp,
	  10.0, 200.0, 1.0, 5.0, 0, 60, NULL, NULL, FALSE, _(" high"));
	hbox = gtk_hbox_new(TRUE,0);
	gtk_table_attach_defaults(GTK_TABLE(table), hbox, 0, 1, 2, 3);
	label = gtk_label_new(_("Hysteresis:"));
	gtk_misc_set_alignment(GTK_MISC(label), 1.0, 0.5);
	gtk_box_pack_start(GTK_BOX(hbox), label, TRUE, TRUE, 4);
	hbox = gtk_hbox_new(FALSE,0);
	gtk_table_attach_defaults(GTK_TABLE(table), hbox, 1, 2, 2, 3);
	gkrellm_spin_button(hbox, &_i8k_bhyst, (gfloat) _i8k_guiopts.bat_hysteresis,
	  00.0, 20.0, 1.0, 5.0, 0, 60, NULL, NULL, FALSE, NULL);

	/* -- Info -- */
	vbox = gkrellm_create_framed_tab(tabs, _("Info"));
#ifdef GKRELLM2
	text = gkrellm_gtk_scrolled_text_view(vbox, NULL,
	  GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
#else
	text = gkrellm_scrolled_text(vbox, NULL,
	  GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
#endif
	for (i=0; i < sizeof(i8krellm_info_text)/sizeof(gchar *); ++i) {
		gkrellm_add_info_text_string(text,_(i8krellm_info_text[i]));
	}

	/* -- About -- */
	about_text = g_strdup_printf(
	  "i8krellm %s.%s\n"
	  "GKrellM Dell Inspiron 8000 Fan Control Plugin\n"
	  "(works with other Dell notebooks, see README)\n\n"
	  "Copyright (c) 2001,2002 Eric Johnson\n"
	  "<eric@coding-zone.com>\n"
	  "http://www.coding-zone.com\n\n"
	  "Released under the GNU Public License",
	  I8K_VERSION_MAJOR,I8K_VERSION_MINOR);
	text = gtk_label_new(about_text);
	label = gtk_label_new("About");
	gtk_notebook_append_page(GTK_NOTEBOOK(tabs),text,label);
	g_free(about_text);

	return;
}

static void i8krellm_apply_config() {
	GtkSpinButton *spin;
	gchar *s;
	gboolean old_fanspeed_enabled;
	int i;

	/* Celsius/Fahrenheit */
	if (GTK_TOGGLE_BUTTON(_i8k_unitbutton[0])->active) {
	  _i8k_guiopts.temp_units = 0;
	} else {
	  _i8k_guiopts.temp_units = 1;
	}
	/* enabled fans */
	for (i = 0; i < 3; i++) {
		if (GTK_TOGGLE_BUTTON(_i8k_fanbuttons[i])->active) {
			_i8k_guiopts.enabled_fans = i;
		}
	}
	/* enable rpm display */
	old_fanspeed_enabled = _i8k_guiopts.fanspeed_enabled;
	_i8k_guiopts.fanspeed_enabled = GTK_TOGGLE_BUTTON(_i8k_disprpm)->active;
	/* enable chart display */
	_i8k_guiopts.cpuchart_enabled = GTK_TOGGLE_BUTTON(_i8k_dispchart)->active;
	/* show service tag */
	_i8k_guiopts.svctag_enabled = GTK_TOGGLE_BUTTON(_i8k_svctag)->active;
	/* i8kfan utility */
	s = gkrellm_entry_get_text(&_i8k_i8kfan);
	strncpy(&_i8k_guiopts.i8kfan_loc[0],s,sizeof(_i8k_guiopts.i8kfan_loc)-1);
	/* polling interval */
	spin = GTK_SPIN_BUTTON(_i8k_polling);
	_i8k_guiopts.poll_interval = gtk_spin_button_get_value_as_int(spin);
	/* ac temps */
	spin = GTK_SPIN_BUTTON(_i8k_lflow);
	_i8k_lf.ac_low_temp = gtk_spin_button_get_value_as_int(spin);
	spin = GTK_SPIN_BUTTON(_i8k_lfhigh);
	_i8k_lf.ac_high_temp = gtk_spin_button_get_value_as_int(spin);
	spin = GTK_SPIN_BUTTON(_i8k_rflow);
	_i8k_rf.ac_low_temp = gtk_spin_button_get_value_as_int(spin);
	spin = GTK_SPIN_BUTTON(_i8k_rfhigh);
	_i8k_rf.ac_high_temp = gtk_spin_button_get_value_as_int(spin);
	spin = GTK_SPIN_BUTTON(_i8k_hyst);
	_i8k_guiopts.ac_hysteresis = gtk_spin_button_get_value_as_int(spin);
	/* battery temps */
	spin = GTK_SPIN_BUTTON(_i8k_blflow);
	_i8k_lf.bat_low_temp = gtk_spin_button_get_value_as_int(spin);
	spin = GTK_SPIN_BUTTON(_i8k_blfhigh);
	_i8k_lf.bat_high_temp = gtk_spin_button_get_value_as_int(spin);
	spin = GTK_SPIN_BUTTON(_i8k_brflow);
	_i8k_rf.bat_low_temp = gtk_spin_button_get_value_as_int(spin);
	spin = GTK_SPIN_BUTTON(_i8k_brfhigh);
	_i8k_rf.bat_high_temp = gtk_spin_button_get_value_as_int(spin);
	spin = GTK_SPIN_BUTTON(_i8k_bhyst);
	_i8k_guiopts.bat_hysteresis = gtk_spin_button_get_value_as_int(spin);
	__i8k_get_proc_info();
	if (_i8k_guiopts.svctag_enabled) {
		gtk_tooltips_set_tip(_i8k_svctt,_i8k_panel->drawing_area,
		  &_i8k_procinfo.svctag[0],"");
		gtk_tooltips_enable(_i8k_svctt);
	} else {
		gtk_tooltips_disable(_i8k_svctt);
	}

	if (old_fanspeed_enabled != _i8k_guiopts.fanspeed_enabled) {
#ifdef GKRELLM2
		gkrellm_panel_configure_set_height(_i8k_panel,
				_i8k_guiopts.fanspeed_enabled ?
				_i8k_state.panel_height_rpm : _i8k_state.panel_height_norpm);
#else
		_i8k_panel->label->h_panel =
				_i8k_guiopts.fanspeed_enabled ?
				_i8k_state.panel_height_rpm : _i8k_state.panel_height_norpm;
#endif
		gkrellm_panel_create(_i8k_vbox,_i8k_mon,_i8k_panel);
	}

	__i8k_fan_check();
	_i8k_state.update=1;
	i8krellm_update_plugin();
	return;
}

static void i8krellm_save_config(FILE *f) {
	fprintf(f,"i8krellm i8kfan_utility %s\n",&_i8k_guiopts.i8kfan_loc[0]);
	fprintf(f,"i8krellm temp_units %d\n",_i8k_guiopts.temp_units);
	fprintf(f,"i8krellm poll_interval %d\n",_i8k_guiopts.poll_interval);
	fprintf(f,"i8krellm ac_left_fan_low %d\n",_i8k_lf.ac_low_temp);
	fprintf(f,"i8krellm ac_left_fan_high %d\n",_i8k_lf.ac_high_temp);
	fprintf(f,"i8krellm ac_right_fan_low %d\n",_i8k_rf.ac_low_temp);
	fprintf(f,"i8krellm ac_right_fan_high %d\n",_i8k_rf.ac_high_temp);
	fprintf(f,"i8krellm ac_hysteresis %d\n",_i8k_guiopts.ac_hysteresis);
	fprintf(f,"i8krellm left_fan_low %d\n",_i8k_lf.bat_low_temp);
	fprintf(f,"i8krellm left_fan_high %d\n",_i8k_lf.bat_high_temp);
	fprintf(f,"i8krellm right_fan_low %d\n",_i8k_rf.bat_low_temp);
	fprintf(f,"i8krellm right_fan_high %d\n",_i8k_rf.bat_high_temp);
	fprintf(f,"i8krellm hysteresis %d\n",_i8k_guiopts.bat_hysteresis);
	fprintf(f,"i8krellm show_svctag %d\n",_i8k_guiopts.svctag_enabled);
	fprintf(f,"i8krellm enabled_fans %d\n",_i8k_guiopts.enabled_fans);
	fprintf(f,"i8krellm display_fan_speed %d\n",_i8k_guiopts.fanspeed_enabled);
	fprintf(f,"i8krellm display_cputemp_chart %d\n",_i8k_guiopts.cpuchart_enabled);
	return;
}

static void i8krellm_load_config(gchar *arg) {
	gchar i8krellm_config[32], item[CFG_BUFSIZE];
	gint n;

	n = sscanf(arg, "%31s %[^\n]", i8krellm_config, item);
	if (n == 2) {
		if (strcmp(i8krellm_config,"i8kfan_utility") == 0) {
			strncpy(&_i8k_guiopts.i8kfan_loc[0],item,sizeof(_i8k_guiopts.i8kfan_loc)-1);
		} else if (strcmp(i8krellm_config,"temp_units") == 0) {
			sscanf(item,"%d",&_i8k_guiopts.temp_units);
		} else if (strcmp(i8krellm_config,"poll_interval") == 0) {
			sscanf(item,"%d",&_i8k_guiopts.poll_interval);
		} else if (strcmp(i8krellm_config,"ac_left_fan_low") == 0) {
			sscanf(item,"%d",&_i8k_lf.ac_low_temp);
		} else if (strcmp(i8krellm_config,"ac_left_fan_high") == 0) {
			sscanf(item,"%d",&_i8k_lf.ac_high_temp);
		} else if (strcmp(i8krellm_config,"ac_right_fan_low") == 0) {
			sscanf(item,"%d",&_i8k_rf.ac_low_temp);
		} else if (strcmp(i8krellm_config,"ac_right_fan_high") == 0) {
			sscanf(item,"%d",&_i8k_rf.ac_high_temp);
		} else if (strcmp(i8krellm_config,"ac_hysteresis") == 0) {
			sscanf(item,"%d",&_i8k_guiopts.ac_hysteresis);
		} else if (strcmp(i8krellm_config,"left_fan_low") == 0) {
			sscanf(item,"%d",&_i8k_lf.bat_low_temp);
		} else if (strcmp(i8krellm_config,"left_fan_high") == 0) {
			sscanf(item,"%d",&_i8k_lf.bat_high_temp);
		} else if (strcmp(i8krellm_config,"right_fan_low") == 0) {
			sscanf(item,"%d",&_i8k_rf.bat_low_temp);
		} else if (strcmp(i8krellm_config,"right_fan_high") == 0) {
			sscanf(item,"%d",&_i8k_rf.bat_high_temp);
		} else if (strcmp(i8krellm_config,"hysteresis") == 0) {
			sscanf(item,"%d",&_i8k_guiopts.bat_hysteresis);
		} else if (strcmp(i8krellm_config,"show_svctag") == 0) {
			sscanf(item,"%d",&_i8k_guiopts.svctag_enabled);
		} else if (strcmp(i8krellm_config,"enabled_fans") == 0) {
			sscanf(item,"%d",&_i8k_guiopts.enabled_fans);
		} else if (strcmp(i8krellm_config,"display_fan_speed") == 0) {
			sscanf(item,"%d",&_i8k_guiopts.fanspeed_enabled);
		} else if (strcmp(i8krellm_config,"display_cputemp_chart") == 0) {
			sscanf(item,"%d",&_i8k_guiopts.cpuchart_enabled);
		}
	}
	return;
}

static Monitor plugin_mon = {
	"Dell I8K Plugin",         /* Name, for config tab.  */
	0,                         /* Id,  0 if a plugin           */
	i8krellm_create_plugin,    /* The create_plugin() function */
	i8krellm_update_plugin,    /* The update_plugin() function */
	i8krellm_create_tab,       /* The create_plugin_tab() config function */
	i8krellm_apply_config,     /* The apply_plugin_config() function  */
	i8krellm_save_config,      /* The save_plugin_config() function  */
	i8krellm_load_config,      /* The load_plugin_config() function  */
	PLUGIN_CONFIG_KEYWORD,     /* config keyword  */
	NULL,                      /* Undefined 2  */
	NULL,                      /* Undefined 1  */
	NULL,                      /* Undefined 0  */
	PLUGIN_PLACEMENT,          /* Insert plugin before this monitor.*/
	NULL,                      /* Handle if a plugin, filled in by GKrellM */
	NULL                       /* path if a plugin, filled in by GKrellM   */
};

/* required */
Monitor *init_plugin(void) {
	_i8k_mon = &plugin_mon;
	_i8k_styleid = gkrellm_add_meter_style(_i8k_mon,STYLE_NAME);
	return _i8k_mon;
}

