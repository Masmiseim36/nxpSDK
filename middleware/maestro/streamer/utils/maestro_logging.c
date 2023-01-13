/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>

#include "audio_cfg.h"
#include "maestro_logging.h"
#include "error.h"

#define COMMON_ENTER LOG_ENTER(LOGMDL_COMMON)
#define COMMON_EXIT  LOG_EXIT(LOGMDL_COMMON)

#ifdef CASCFG_PLATFORM_FREERTOS
#include "FreeRTOSConfig.h"
#ifdef DbgConsole_Printf
#define printf DbgConsole_Printf
#endif
#endif

/* Maximum string sizes */
#define LOG_MAX_TXT_LINE_SIZE  128
#define LOG_MAX_FULL_LINE_SIZE 256

/* This sets the level of output of the non-active modules. */
/*! @cond */
/* These defines are private and should not be used outside of this file. */
#if CASCFG_LOGLVL_DEFAULT_CATASTROPHIC
#define LOGLVL_DEFAULT_CATA LOGLVL_CATASTROPHIC
#else
#define LOGLVL_DEFAULT_CATA 0
#endif

#if CASCFG_LOGLVL_DEFAULT_ERROR
#define LOGLVL_DEFAULT_ERR LOGLVL_ERROR
#else
#define LOGLVL_DEFAULT_ERR 0
#endif

#if CASCFG_LOGLVL_DEFAULT_WARNING
#define LOGLVL_DEFAULT_WARN LOGLVL_WARNING
#else
#define LOGLVL_DEFAULT_WARN 0
#endif

#if CASCFG_LOGLVL_DEFAULT_INFO
#define LOGLVL_DEFAULT_INFO LOGLVL_INFO
#else
#define LOGLVL_DEFAULT_INFO 0
#endif

#if CASCFG_LOGLVL_DEFAULT_FUNCTIONS
#define LOGLVL_DEFAULT_FUNC LOGLVL_FUNCTION_INFO
#else
#define LOGLVL_DEFAULT_FUNC 0
#endif

#define LOGLVL_DEFAULTLEVEL \
    (LOGLVL_DEFAULT_CATA | LOGLVL_DEFAULT_ERR | LOGLVL_DEFAULT_WARN | LOGLVL_DEFAULT_INFO | LOGLVL_DEFAULT_FUNC)
/*! @endcond */

#define LOGMDL_DEFAULT LOGMDL_ALL

/* Set the default log output location. */
#if CASCFG_LOG_DEFAULT_OUTPUT_CONSOLE
#define LOGPUT_DEFAULT LOGPUT_CONSOLE
#elif CASCFG_LOG_DEFAULT_OUTPUT_FILE
#define LOGPUT_DEFAULT LOGPUT_FILE
#elif CASCFG_LOG_DEFAULT_OUTPUT_BOTH
#define LOGPUT_DEFAULT LOGPUT_ALL
#else
#define LOGPUT_DEFAULT LOGPUT_NONE
#endif

#ifdef LOG_ENABLED
/*!
 * A structure to hold the string names and ids of the modules.
 */
typedef struct name_id_link
{
    uint32_t id;
    char name[LOG_NAME_LENGTH + 1];
} name_id_link_t;

/* Local static variables */
static uint32_t ActiveModules;
static uint8_t ActiveLevel;
static uint8_t ActiveOutput;

/* Array to contain the id and text name of all modules */
static name_id_link_t module_names[LOGMDL_MAX_MODULES];

/* Contains the id and text string for all errors. */
static name_id_link_t error_names[LOGMDL_MAX_ERRORS];

#ifdef VECTOR_ENABLED
static Vector *error_names_vector;
/* Contains the text for an error name, temporarily. */
static char error_text[LOG_NAME_LENGTH + 1];
#endif
#endif

/* ------------------ Local Functions ----------------- */

#ifdef LOG_ENABLED
/*!
 * @brief  Returns a name for a module code.
 * This is intended to be used with the lprintf module specified, not the
 * currently active modules, which can combine more than one.
 *
 * @param[in]  module: id of a module
 */
static char *module_name(uint32_t module)
{
    /* Printing enter and exit for this function is too messy. */
    int i;

    /* Look for this module in the table. */
    for (i = 0; i < LOGMDL_MAX_MODULES; i++)
    {
        if (module_names[i].id == module)
            return module_names[i].name;
    }

    return "UNKNOWN MODULE";
}

/*!
 * @brief  Returns a simple name for a level code.
 * This is intended to be used with the lprintf level specified, not the
 * currently active levels, which can combine more than one.
 *
 * @param[in]  level: id for a level
 */
static const char *level_name(uint8_t level)
{
    /* Printing enter and exit for this function is too messy. */
    switch (level)
    {
        case LOGLVL_CATASTROPHIC:
            return "CATASTROPHIC";
        case LOGLVL_ERROR:
            return "ERROR";
        case LOGLVL_WARNING:
            return "WARNING";
        case LOGLVL_INFO:
            return "INFO";
        case LOGLVL_DEBUG:
            return "DEBUG";
        case LOGLVL_FUNCTION_INFO:
            return "FUNCTION";
        default:
            return "UNKNOWN LEVEL";
    }
}
#endif

/*!
 *  @brief  Local function that handles printing of the message to the proper
 *          location.
 *          This function also handles pre-pending the time.
 *
 * @param[in]  message: the text of the message.
 */
void print_message(char *message)
{
#ifdef LOG_ENABLED
    /* Printing enter and exit for this function is too messy. */

    char logoutput[LOG_MAX_FULL_LINE_SIZE + 1];

    /* Add trailing '\r' for FreeRTOS for proper newline with MCUXpresso SDK. */
    snprintf(logoutput, LOG_MAX_FULL_LINE_SIZE, "%s\r", message);
    logoutput[LOG_MAX_FULL_LINE_SIZE] = '\0';

    /* print to console */
    if (ActiveOutput & LOGPUT_CONSOLE)
    {
        printf(logoutput);
    }
#endif
}

/* ------------------ External Functions ----------------- */

void lprintf(const uint32_t module, const uint8_t level, const uint32_t error, const char *message, ...)
{
    /* Printing enter and exit for this function is too messy. */
#ifdef LOG_ENABLED
    va_list args;
    char buffer[LOG_MAX_TXT_LINE_SIZE + 1];
    char logoutput[LOG_MAX_FULL_LINE_SIZE + 1];

    if ((level & (LOGLVL_DEFAULTLEVEL)) || ((module & ActiveModules) && (level & ActiveLevel)))
    {
        va_start(args, message);
        vsnprintf(buffer, sizeof(buffer), message, args);
        va_end(args);
        buffer[LOG_MAX_TXT_LINE_SIZE] = '\0';

        if (level < LOGLVL_INFO)
        {
            snprintf(logoutput, LOG_MAX_FULL_LINE_SIZE, "%s %s 0x%016lx %s %s", module_name(module), level_name(level),
                     error, error_name(error), buffer);
            logoutput[LOG_MAX_FULL_LINE_SIZE] = '\0';
        }
        else
        {
            snprintf(logoutput, LOG_MAX_FULL_LINE_SIZE, "%s %s %s", module_name(module), level_name(level), buffer);
            logoutput[LOG_MAX_FULL_LINE_SIZE] = '\0';
        }

        print_message(logoutput);
    }
#endif
}

void dlprintf(const uint32_t module, const char *message, ...)
{
    /* Printing enter and exit for this function is too messy. */
#ifdef LOG_ENABLED
    va_list args;
    char buffer[LOG_MAX_TXT_LINE_SIZE + 1];
    char logoutput[LOG_MAX_FULL_LINE_SIZE + 1];

    if ((LOGLVL_DEBUG & (LOGLVL_DEFAULTLEVEL)) || ((module & ActiveModules) && (LOGLVL_DEBUG & ActiveLevel)))
    {
        va_start(args, message);
        vsnprintf(buffer, sizeof(buffer), message, args);
        va_end(args);
        buffer[LOG_MAX_TXT_LINE_SIZE] = '\0';

        snprintf(logoutput, LOG_MAX_FULL_LINE_SIZE, "%s %s %s", module_name(module), "DEBUG", buffer);
        logoutput[LOG_MAX_FULL_LINE_SIZE] = '\0';

        print_message(logoutput);
    }
#endif
}

void ilprintf(const uint32_t module, const char *message, ...)
{
    /* Printing enter and exit for this function is too messy. */
#ifdef LOG_ENABLED
    va_list args;
    char buffer[LOG_MAX_TXT_LINE_SIZE + 1];
    char logoutput[LOG_MAX_FULL_LINE_SIZE + 1];

    if ((LOGLVL_INFO & (LOGLVL_DEFAULTLEVEL)) || ((module & ActiveModules) && (LOGLVL_INFO & ActiveLevel)))
    {
        va_start(args, message);
        vsnprintf(buffer, sizeof(buffer), message, args);
        va_end(args);
        buffer[LOG_MAX_TXT_LINE_SIZE] = '\0';

        snprintf(logoutput, LOG_MAX_FULL_LINE_SIZE, "%s %s %s", module_name(module), "INFO", buffer);
        logoutput[LOG_MAX_FULL_LINE_SIZE] = '\0';

        print_message(logoutput);
    }
#endif
}

void set_debug_module(const uint32_t module)
{
#ifdef LOG_ENABLED
    COMMON_ENTER;

    ActiveModules = ActiveModules | module;
    dlprintf(LOGMDL_COMMON, "Log modules now set to 0x%x\n", ActiveModules);

    COMMON_EXIT;
#endif
}

void clear_debug_module(const uint32_t module)
{
#ifdef LOG_ENABLED
    COMMON_ENTER;

    ActiveModules = ActiveModules & ~module;
    dlprintf(LOGMDL_COMMON, "Clear log modules. Now set to 0x%x\n", ActiveModules);

    COMMON_EXIT;
#endif
}

void set_debug_level(const uint8_t level)
{
#ifdef LOG_ENABLED
    COMMON_ENTER;

    ActiveLevel = ActiveLevel | level;
    dlprintf(LOGMDL_COMMON, "Log level now set to 0x%x\n", ActiveLevel);

    COMMON_EXIT;
#endif
}

void clear_debug_level(const uint8_t level)
{
#ifdef LOG_ENABLED
    COMMON_ENTER;

    ActiveLevel = ActiveLevel & ~level;
    dlprintf(LOGMDL_COMMON, "Clear log level. Now set to 0x%x\n", ActiveLevel);

    COMMON_EXIT;
#endif
}

void set_debug_out(const uint8_t output)
{
#ifdef LOG_ENABLED
    COMMON_ENTER;

    ActiveOutput = ActiveOutput | output;
    dlprintf(LOGMDL_COMMON, "Log output now set to 0x%x\n", ActiveOutput);

    COMMON_EXIT;
#endif
}

void clear_debug_out(const uint8_t output)
{
#ifdef LOG_ENABLED
    COMMON_ENTER;

    ActiveOutput = ActiveOutput & ~output;
    dlprintf(LOGMDL_COMMON, "Clear log output. Now set to 0x%x\n", ActiveOutput);

    COMMON_EXIT;
#endif
}

uint32_t add_name_into_array(const uint32_t module, const char *name, name_id_link_t *name_array, uint32_t size)
{
#ifdef LOG_ENABLED
    int i;

    COMMON_ENTER;

    /* This function will not overwrite a module that is already in the table.
     * Check for it. */
    for (i = 0; i < size; i++)
    {
        // removed always true (module_names[i].name != NULL)
        if (name_array[i].id == module && name_array[i].name[0] != '\0')
        {
            lprintf(LOGMDL_COMMON, LOGLVL_ERROR, ERRCODE_PARAMETER_EXISTS,
                    "ERROR: module %x already exists with name %s.\n", module, name_array[i].name);

            COMMON_EXIT;
            return ERRCODE_PARAMETER_EXISTS;
        }
    }

    /* Find an empty spot for this module. */
    for (i = 0; i < size; i++)
    {
        if (0 == name_array[i].id && name_array[i].name[0] == '\0')
        {
            name_array[i].id = module;
            strncpy(name_array[i].name, name, LOG_NAME_LENGTH);
            dlprintf(LOGMDL_COMMON, "   [%d] = %x  %s\n", i, name_array[i].id, name_array[i].name);

            COMMON_EXIT;
            return ERRCODE_NO_ERROR;
        }
    }

    if (size == i)
    {
        lprintf(LOGMDL_COMMON, LOGLVL_ERROR, ERRCODE_SYSTEM_LIMIT,
                "ERROR: All module spaces are filled. %d modules are allowed.\n", size);

        COMMON_EXIT;
        return ERRCODE_SYSTEM_LIMIT;
    }

    COMMON_EXIT;
#endif

    return ERRCODE_NO_ERROR;
}

uint32_t remove_name_from_array(const uint32_t module, name_id_link_t *name_array, uint32_t size)
{
#ifdef LOG_ENABLED
    int i;

    COMMON_ENTER;

    /* This function will not overwrite a module that is already in the table.
     * Check for it. */
    for (i = 0; i < size; i++)
    {
        // removed always true (name_array[i].name != NULL)
        if (name_array[i].id == module)
        {
            dlprintf(LOGMDL_COMMON, "   Removed [%d] = %x  %s\n", i, name_array[i].id, name_array[i].name);
            name_array[i].id      = 0;
            name_array[i].name[0] = '\0';

            COMMON_EXIT;
            return ERRCODE_NO_ERROR;
        }
    }

    if (size == i)
    {
        lprintf(LOGMDL_COMMON, LOGLVL_WARNING, ERRCODE_NOT_FOUND, "WARNING: Module %x was not found.\n", module);

        COMMON_EXIT;
        return ERRCODE_NOT_FOUND;
    }

    COMMON_EXIT;
#endif

    return ERRCODE_NO_ERROR;
}

#ifdef VECTOR_ENABLED

uint32_t add_error_name(const uint32_t error_id, const char *text)
{
#ifdef LOG_ENABLED
    name_id_link_t error_info;

    COMMON_ENTER;

    error_info.id = error_id;
    strncpy(error_info.name, text, LOG_NAME_LENGTH);
    error_info.name[LOG_NAME_LENGTH - 1] = '\0';

    COMMON_EXIT;
    return vector_pushback(error_names_vector, &error_info);
#else

    return ERRCODE_NO_ERROR;
#endif
}

#endif

char *error_name(const uint32_t error_id)
{
#ifdef LOG_ENABLED
#ifdef VECTOR_ENABLED
    /* Printing enter and exit for this function is too messy. */
    VectorIter iter;
    name_id_link_t *node;
    void *n;

    container_foreach(vector, error_names, iter, n)
    {
        node = (name_id_link_t *)n;
        if (node->id == error_id)
            return node->name;
    }

    /* if not found, return the text of the id */
    snprintf(error_text, LOG_NAME_LENGTH, "0x%lx", error_id);
    error_text[LOG_NAME_LENGTH - 1] = '\0';

    return error_text;
#endif

    /* Look for this module in the table. */
    for (int i = 0; i < LOGMDL_MAX_ERRORS; i++)
    {
        if (error_names[i].id == error_id)
            return error_names[i].name;
    }

    return "UNKNOWN MODULE";
#endif
}

void get_debug_state()
{
#ifdef LOG_ENABLED
    printf("\n");
    printf("Active output bit field: 0x%x\r\n", ActiveOutput);
    printf("Active module bit field: 0x%lx\r\n", ActiveModules);
    printf("Active level bit field: 0x%x\r\n", ActiveLevel);
    printf("Level always displayed bit field: 0x%x\r\n", LOGLVL_DEFAULT);
    printf("\n");
#else
    printf("\nLogging not enabled\n\n");
#endif
}

void init_logging()
{
#ifdef LOG_ENABLED
    int i;

    ActiveModules = LOGMDL_DEFAULT;
    ActiveOutput  = LOGPUT_DEFAULT;

    COMMON_ENTER;

    /* This function will not overwrite a module that is already in the table.
     * Check for it. */
    for (i = 0; i < LOGMDL_MAX_MODULES; i++)
    {
        module_names[i].id      = 0;
        module_names[i].name[0] = '\0';
    }

    /* Add the modules that must be there. */
    add_name_into_array(LOGMDL_COMMON, "COMMON", module_names, LOGMDL_MAX_MODULES);
    add_name_into_array(LOGMDL_OSA, "OSA", module_names, LOGMDL_MAX_MODULES);
    add_name_into_array(LOGMDL_APP, "APPLICATION", module_names, LOGMDL_MAX_MODULES);
    add_name_into_array(LOGMDL_STREAMER, "STREAMER", module_names, LOGMDL_MAX_MODULES);

    /* Create the vector that will hold this data. */
    for (i = 0; i < LOGMDL_MAX_ERRORS; i++)
    {
        error_names[i].id      = 0;
        error_names[i].name[0] = '\0';
    }

    /* Fill it with the known error messages. */
    add_name_into_array(ERRCODE_NO_ERROR, "NO ERROR", error_names, LOGMDL_MAX_ERRORS);
    add_name_into_array(ERRCODE_TRY_AGAIN, "TRY AGAIN", error_names, LOGMDL_MAX_ERRORS);
    add_name_into_array(ERRCODE_INVALID_ARGUMENT, "INVALID ARGUMENT", error_names, LOGMDL_MAX_ERRORS);
    add_name_into_array(ERRCODE_OUT_OF_MEMORY, "OUT OF MEMORY", error_names, LOGMDL_MAX_ERRORS);
    add_name_into_array(ERRCODE_OUT_OF_SPACE, "OUT OF SPACE", error_names, LOGMDL_MAX_ERRORS);
    add_name_into_array(ERRCODE_GENERAL_ERROR, "GENERAL ERROR", error_names, LOGMDL_MAX_ERRORS);
    add_name_into_array(ERRCODE_DEADLOCK, "DEADLOCK", error_names, LOGMDL_MAX_ERRORS);
    add_name_into_array(ERRCODE_NOT_SUPPORTED, "NOT SUPPORTED", error_names, LOGMDL_MAX_ERRORS);
    add_name_into_array(ERRCODE_BUSY, "BUSY", error_names, LOGMDL_MAX_ERRORS);
    add_name_into_array(ERRCODE_PERMISSION_DENIED, "PERMISSION DENIED", error_names, LOGMDL_MAX_ERRORS);
    add_name_into_array(ERRCODE_TIMED_OUT, "TIMED OUT", error_names, LOGMDL_MAX_ERRORS);
    add_name_into_array(ERRCODE_SYSTEM_LIMIT, "SYSTEM LIMIT REACHED", error_names, LOGMDL_MAX_ERRORS);
    add_name_into_array(ERRCODE_PARAMETER_EXISTS, "PARAMETER ALREADY EXISTS", error_names, LOGMDL_MAX_ERRORS);
    add_name_into_array(ERRCODE_END_OF_DIRECTORY, "END OF DIRECTORY REACHED", error_names, LOGMDL_MAX_ERRORS);
    add_name_into_array(ERRCODE_NOT_FOUND, "NOT FOUND", error_names, LOGMDL_MAX_ERRORS);
    add_name_into_array(ERRCODE_INTERNAL, "INTERNAL ERROR", error_names, LOGMDL_MAX_ERRORS);
    add_name_into_array(ERRCODE_OVERFLOW, "OVERFLOW", error_names, LOGMDL_MAX_ERRORS);

    COMMON_EXIT;
#endif
}

void deinit_logging(void)
{
#ifdef VECTOR_ENABLED
    vector_destroy(error_names);
#endif
}
