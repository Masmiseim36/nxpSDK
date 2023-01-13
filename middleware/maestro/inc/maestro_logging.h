/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef __MAESTRO_LOGGING_H_
#define __MAESTRO_LOGGING_H_

/*!
 * @file    maestro_logging.h
 * @brief   Contains maestro_logging prototypes and definitions for the connected radio.
 */

/*
 * CONSTANTS, DEFINES AND MACROS
 */

/*! @{
 * @name The level of the error will be defined as:
 */
#define LOGLVL_CATASTROPHIC 0x01 /*!< @brief  Serious error. System cannot recover. */
#define LOGLVL_ERROR                                                              \
    0x02                    /*!< @brief  An error has occurred. The system should \
                                  be able to continue but an individual           \
                                  module may not be able to. */
#define LOGLVL_WARNING 0x04 /*!< @brief  A warning about a potential problem. */
#define LOGLVL_INFO                                                            \
    0x08                  /*!< @brief  Information that could be useful to the \
                                user. */
#define LOGLVL_DEBUG 0x10 /*!< @brief  For developers for debugging */
#define LOGLVL_FUNCTION_INFO                             \
    0x20 /*!< @brief  For developers debugging. Function \
                Enter/Exit info. */
/*! @} */

/*! @{
 * @name Additional level specifications used by set_debug_level()
 * These levels cannot be used by lprintf()
 */
#define LOGLVL_NONE    0x00 /*!< @brief No error messages reported. */
#define LOGLVL_DEFAULT 0x03 /*!< @brief Default log level */
#define LOGLVL_ALL     0xFF /*!< @brief All error messages reported. */
/*! @} */

/*! @{
 * @name Output location for the error logs
 */
#define LOGPUT_NONE    0x0 /*!< @brief No output */
#define LOGPUT_CONSOLE 0x1 /*!< @brief Output goes to the terminal window */
#define LOGPUT_FILE    0x2 /*!< @brief Output goes to a file */
#define LOGPUT_ALL     0x3 /*!< @brief Output to all possibilities */
/*! @} */

/*! @{
 * @name The module bit field definition.
 * The modules will be defined as bit settings in a 32 bit value so this is
 * limited to 32 modules.
 */
#define LOGMDL_COMMON   0x00000001 /*!< @brief Common module */
#define LOGMDL_OSA      0x00000002 /*!< @brief OSA module */
#define LOGMDL_APP      0x00000004 /*!< @brief Application module */
#define LOGMDL_GUI      0x00000008 /*!< @brief GUI module */
#define LOGMDL_STREAMER 0x00000010 /*!< @brief Streamer module */
/*! @} */

/*! @{
 * @name  Additional level specifications used by set_debug_module()
 * These levels cannot be used by lprintf().
 * The user can also OR several modules together for a custom selection.
 */
#define LOGMDL_NONE 0x00000000 /*!< @brief None module allowed */
#define LOGMDL_ALL  0x3FFFFFFF /*!< @brief All modulew allowed */
/*! @} */

#define LOGMDL_MAX_MODULES 6 /*!< @brief Maximum number of modules supported. */

#define LOGMDL_MAX_ERRORS 17 /*!< @brief Maximum number of modules supported. */

#define LOG_NAME_LENGTH 19 /*!< @brief Maximum string length for module name. */

/*
 * Configuration settings
 */

/**
 * @brief Logging enabled
 *
 */
#define LOG_ENABLED

/*!
 * @{
 * @name LOGGING FUNCTION PROTOTYPES
 */

/*!
 * @ingroup utils
 * @brief  Convenience function that prints a debug level message specifically
 *         for entering a function.
 *
 * @param[in]  module: module id
 */
#define LOG_ENTER(module) lprintf(module, LOGLVL_FUNCTION_INFO, 0, "Entering: %s\n", __FUNCTION__)

/*!
 * @brief  Convenience function that prints a debug level message specifically
 *         for exiting a function.
 *
 * @param[in]  module: module id
 */
#define LOG_EXIT(module) lprintf(module, LOGLVL_FUNCTION_INFO, 0, "Exiting: %s\n\n", __FUNCTION__)

/*!
 * @brief The function lprintf() is used in module code in place of
 *        printf().  Different levels of information can be turned on or off
 *        at runtime for specific modules.
 *
 * @param[in]  module is an enumerated value defining the module this is used
 *             in
 * @param[in]  level is an enumerated value indicating the seriousness of the
 *             error
 * @param[in]  error is a defined error/warning code
 * @param[in]  message is a text message describing the error.
 *             The message can use args just as a standard printf.
 */
void lprintf(const uint32_t module, const uint8_t level, const uint32_t error, const char *message, ...);

/*!
 * @brief   This function is the information-specific shortcut variant of
 *          lprintf(). It is the same as specifying lprintf(module,
 *          LOGLVL_DEBUG, ERRCODE_NO_ERROR, ...).
 *
 * @param[in]  module is an enumerated value defining the module this is used
 *             in
 * @param[in]  message is a text message describing the error.
 *             The message can use args just as a standard printf.
 */
void dlprintf(const uint32_t module, const char *message, ...);

/*!
 * @brief   This function is the information-specific shortcut variant of
 *          lprintf(). It is the same as specifying lprintf(module, LOGLVL_INFO,
 *          ERRCODE_NO_ERROR, ...).
 *
 * @param[in]  module is an enumerated value defining the module that this is
 *             used in
 * @param[in]  message is a text message describing the error.
 *             The message can use args just as a standard printf.
 */
void ilprintf(const uint32_t module, const char *message, ...);

/*!
 * @brief  Sets the module of interest based on bits set in <i>module</i>.
 *         Adds to any already set.
 *
 * @param[in]  module bit(s) to be added to active modules. This value is
 *             generated by or'ing the LOGMDL_* defines.
 */
void set_debug_module(const uint32_t module);

/*!
 * @brief  Sets the levels to display based on bits set in <i>level</i>. Adds to
 *         those already set.
 *
 * @param[in]  level bit(s) to be added to active levels. This value is
 *             generated by or'ing the LOGLVL_* defines.
 */
void set_debug_level(const uint8_t level);

/*!
 * @brief  Indicates where the output should go: to the console, a file, etc.
 *
 * @param[in]  output bit(s) to be added to active output. This should be one
 *             of the LOGPUT_* defines.
 */
void set_debug_out(const uint8_t output);

/*!
 * @brief  Clears the modules set in <i>module</i>.
 *
 * @param[in]  module bit(s) to be cleared in active modules. This value is
 *             generated by or'ing the LOGMDL_* defines.
 */
void clear_debug_module(const uint32_t module);

/*!
 * @brief  Clears the levels set in <i>level</i>.
 *
 * @param[in]  level bit(s) to be removed from active levels. This value is
 *             generated by or'ing the LOGLVL_* defines.
 */
void clear_debug_level(const uint8_t level);

/*!
 * @brief  Clears the output bits set in <i>output</i>.
 *
 * @param[in]  output bit(s) to be removed from active output. This should be
 *             one of the LOGPUT_* defines.
 */
void clear_debug_out(const uint8_t output);

/*!
 * @brief  Adds a name for a module to the system.
 * The module name will be printed with the output of lprintf().
 *
 * @param[in]  module id of a module
 * @param[in]  name   name for the module
 *
 * @returns  an error code.  The error code can be: <BR>
 * @retval  ERRCODE_NO_ERROR worked as expected
 * @retval  ERRCODE_PARAMETER_EXISTS the module id already has a name
 * @retval  ERRCODE_SYSTEM_LIMIT the system has run out of places to store
 *          module information
 */
uint32_t add_module_name(const uint32_t module, const char *name);

/*!
 * @brief  Removes the name for a module.
 *
 * @param[in]  module id of a module
 *
 * @returns  an error code.  The error code can be: <BR>
 * @retval  ERRCODE_NO_ERROR worked as expected
 * @retval  ERRCODE_NOT_FOUND no text was found for this module
 */
uint32_t remove_module_name(const uint32_t module);

/*!
 * @brief  Adds a text string for an error of 25 char or less to the system.
 * The error text will be printed with the output of lprintf().
 *
 * @param[in]  error_id error the text will be added to
 * @param[in]  text     error text to be printed for error_id
 *
 * @returns  an error code.  The error code can be: <BR>
 * @retval  ERRCODE_NO_ERROR worked as expected
 * @retval  ERRCODE_PARAMETER_EXISTS the error id already has identifying text
 * @retval  ERRCODE_SYSTEM_LIMIT the system has run out of places to store
 *          error information
 */
uint32_t add_error_name(const uint32_t error_id, const char *text);

/*!
 * @brief  Returns a simple name for an error code, if it was pre-set with
 *         add_error_name().
 *
 * @param[in]  error_id an error code indicating a particular error
 *
 * @returns a descriptive text identification will be returned for this error id
 */
char *error_name(const uint32_t error_id);

/*!
 * @brief  Initializes the module and error tables.
 *
 */
void init_logging(void);

/*!
 * @brief  Deinitializes the module.
 *
 */
void deinit_logging(void);

/*!
 * @brief  Prints the settings for active modules, level and output.
 *
 */
void get_debug_state(void);

/*! @} */
#endif
