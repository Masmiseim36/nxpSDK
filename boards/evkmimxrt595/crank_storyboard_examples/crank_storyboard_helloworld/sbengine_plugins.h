/*
 * Copyright 2013, Crank Software Inc. All Rights Reserved.
 *
 * For more information email info@cranksoftware.com.
 */

#include <gre/sdk/plugin.h>

/*
 * This is a list of the available plugins.  Add the functions to the plugin list
 * below to make the features available to your application
 */

// gre plugins
extern int gre_plugin_animate(gr_plugin_state_t *);
extern int gre_plugin_greio(gr_plugin_state_t *); 
extern int gre_plugin_logger(gr_plugin_state_t *);
extern int gre_plugin_script_lua(gr_plugin_state_t *);
extern int gre_plugin_timer(gr_plugin_state_t *);
extern int gre_plugin_gesture(gr_plugin_state_t *);

// Render extension plugins
extern int gre_plugin_circle(gr_plugin_state_t *);
extern int gre_plugin_poly(gr_plugin_state_t *);

// Image loading plugins
extern int gre_plugin_sbimage_soil(gr_plugin_state_t *);

const gr_plugin_create_func_t sb_plugins[] = {
		gre_plugin_animate,
		gre_plugin_circle,
		gre_plugin_greio,
		gre_plugin_logger,
		gre_plugin_script_lua,
		gre_plugin_poly,
		gre_plugin_timer,
		gre_plugin_sbimage_soil,
		gre_plugin_gesture,
		NULL,
};
