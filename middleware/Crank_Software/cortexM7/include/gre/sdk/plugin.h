/*
 * Copyright 2007, Crank Software Inc. All Rights Reserved.
 *
 * For more information email info@cranksoftware.com.
 */

/**
* \file plugin.h
*
*/

#ifndef GR_PLUGIN_H
#define GR_PLUGIN_H

#include <gre/gre.h>

#if defined(__cplusplus)
extern "C"{
#endif

/**
 * Convenience macro for setting the plugin major/minor/service/build versions
 * @param v A pointer t a gr_version_t structure
 * @param mj Major version number (uint16_t)
 * @param mi Major version number (uint16_t)
 * @param s Major version number (uint16_t)
 * @param b Major version number (uint16_t)
 */
#define SET_PLUGIN_VERSION(v, mj, mi, s, b) (v)->major = (mj), (v)->minor = (mi), (v)->service = (s), (v)->build = (b)

/**
 * Define service number boundaries
 */
typedef enum {
	SERVICE_NUMBER_EARLIEST = -100,
	SERVICE_NUMBER_EARLY = -50,
	SERVICE_NUMBER_NORMAL = 0,
	SERVICE_NUMBER_LATE = 50,
	SERVICE_NUMBER_LATEST = 100
} gr_service_number_t;

/**
 * This structure is used to manage the plugin initialization and state.
 * It is used by both manager plugins and standard plugins.
 */
struct gr_plugin_state {
	/**
     * Name of the plugin.  This must be non-null.  Used to
     * match user options and for tracing and debugging.
     */
    const char * name;

    /**
     * Plugin specific data pointer that can be assigned during creation and
     * is provided back to the plugin during init and fini calls.
     */
    void *      plugin_data;

    /**
     * Plugin initialization function.
     *
     * The option string provided may be modified by the plugin, but the 
     * string may be free'ed immediately upon return of the init() call so
     * plugins must copy any values out of this string that they require.  
     *
     * @param app The application structure pointer
     * @param options Options as a key[=value] string suitable for getsubopt() use or NULL 
	 * @param pdata User defined data 
     * @return Zero (0) on successful initialization, non-zero for failure
     */
    int         (*init)(gr_application_t *app, char *options, void *pdata);

    /**
     * Plugin destruction function. This may be NULL if no cleanup
     * is required by the plugin.  Model must be destroyed before calling this or
     * instances of the plugin will reference invalid data when freed
     *
     * @param app The application structure pointer
	 * @param pdata User defined data 
     */
    void        (*fini)(gr_application_t *app, void *pdata);

    /**
     * Plugin version number
     */
    gr_version_t version;

    /**
     * Service priority for this plugin, represents the order in which plugins are initialized.
     * This should be in between the boundaries set by the gr_service_number_t enumeration, where lower
     * numbered plugins will load first and higher numbered plugins will load later.
     */
    int service_number;

};
typedef struct gr_plugin_state gr_plugin_state_t;

/**
 * Plugins should export a function with this signature and with the
 * name specified by GRE_PLUGIN_CREATE_FUNC.
 *
 * The function should initialize the plugin state structure that is
 * passed in with the appropriate initializer and finalizer calls to
 * be called when the plugin starts and stops.
 * 
 * @param state Plugin State structure
 * @return Zero (0) on success, non-zero on failure
 */
typedef int (*gr_plugin_create_func_t)(gr_plugin_state_t *state);

#if defined(GRE_ENABLE_STATIC_PLUGINS)
/**
 * For static plugin deployments, define a single entry point for the plugin list to be provided.
 *
 * @param app The application structure pointer
 * @return Plugin create function pointer or NULL on error
 */
const gr_plugin_create_func_t * gr_application_get_plugins(gr_application_t *app);

#else
/**
 * For dynamic plugin deployments, define the name of the entry point function in the
 * dynamically located libraries.
 */
#define GRE_PLUGIN_CREATE_FUNC_STR  "gre_plugin_create"

#if defined(_WIN32)
#define GRE_PLUGIN_CREATE_FUNC      DLLExport gre_plugin_create
#else
#define GRE_PLUGIN_CREATE_FUNC      gre_plugin_create
#endif /* _WIN32 */

#endif /* GRE_ENABLE_STATIC_PLUGINS */

#if defined(__cplusplus)
}
#endif

#endif /* GR_PLUGIN_H */
