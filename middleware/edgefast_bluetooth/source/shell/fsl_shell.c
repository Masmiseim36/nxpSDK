/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * POSIX getopt for Windows
 * Code given out at the 1985 UNIFORUM conference in Dallas.
 *
 * From std-unix@ut-sally.UUCP (Moderator, John Quarterman) Sun Nov  3 14:34:15 1985
 * Relay-Version: version B 2.10.3 4.3bsd-beta 6/6/85; site gatech.CSNET
 * Posting-Version: version B 2.10.2 9/18/84; site ut-sally.UUCP
 * Path: gatech!akgua!mhuxv!mhuxt!mhuxr!ulysses!allegra!mit-eddie!genrad!panda!talcott!harvard!seismo!ut-sally!std-unix
 * From: std-unix@ut-sally.UUCP (Moderator, John Quarterman)
 * Newsgroups: mod.std.unix
 * Subject: public domain AT&T getopt source
 * Message-ID: <3352@ut-sally.UUCP>
 * Date: 3 Nov 85 19:34:15 GMT
 * Date-Received: 4 Nov 85 12:25:09 GMT
 * Organization: IEEE/P1003 Portable Operating System Environment Committee
 * Lines: 91
 * Approved: jsq@ut-sally.UUC
 * Here's something you've all been waiting for:  the AT&T public domain
 * source for getopt(3).  It is the code which was given out at the 1985
 * UNIFORUM conference in Dallas.  I obtained it by electronic mail
 * directly from AT&T.  The people there assure me that it is indeed
 * in the public domain
 * There is no manual page.  That is because the one they gave out at
 * UNIFORUM was slightly different from the current System V Release 2
 * manual page.  The difference apparently involved a note about the
 * famous rules 5 and 6, recommending using white space between an option
 * and its first argument, and not grouping options that have arguments.
 * Getopt itself is currently lenient about both of these things White
 * space is allowed, but not mandatory, and the last option in a group can
 * have an argument.  That particular version of the man page evidently
 * has no official existence, and my source at AT&T did not send a copy.
 * The current SVR2 man page reflects the actual behavor of this getopt.
 * However, I am not about to post a copy of anything licensed by AT&T.
 */

#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include "fsl_common.h"
#include "fsl_str.h"

#include "fsl_component_generic_list.h"
#include "fsl_component_serial_manager.h"

#include "fsl_shell.h"

/*
 * The OSA_USED macro can only be defined when the OSA component is used.
 * If the source code of the OSA component does not exist, the OSA_USED cannot be defined.
 * OR, If OSA component is not added into project event the OSA source code exists, the OSA_USED
 * also cannot be defined.
 * The source code path of the OSA component is <MCUXpresso_SDK>/components/osa.
 *
 */
#if defined(OSA_USED)

#if (defined(SHELL_USE_COMMON_TASK) && (SHELL_USE_COMMON_TASK > 0U))
#include "fsl_component_common_task.h"
#else
#include "fsl_os_abstraction.h"
#endif

#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define KEY_ESC (0x1BU)
#define KET_DEL (0x7FU)

#define SHELL_EVENT_DATA_ARRIVED (1U << 0)
#define SHELL_EVENT_DATA_SENT    (1U << 1)

#define SHELL_SPRINTF_BUFFER_SIZE (64U)

/*! @brief A type for the handle special key. */
typedef enum _fun_key_status
{
    kSHELL_Normal   = 0U, /*!< Normal key */
    kSHELL_Special  = 1U, /*!< Special key */
    kSHELL_Function = 2U, /*!< Function key */
} fun_key_status_t;

/*! @brief Data structure for Shell environment. */
typedef struct _shell_context_handle
{
    list_label_t commandContextListHead; /*!< Command shellContextHandle list queue head */
    serial_handle_t serialHandle;        /*!< Serial manager handle */
    uint8_t
        serialWriteHandleBuffer[SERIAL_MANAGER_WRITE_HANDLE_SIZE];   /*!< The buffer for serial manager write handle */
    serial_write_handle_t serialWriteHandle;                         /*!< The serial manager write handle */
    uint8_t serialReadHandleBuffer[SERIAL_MANAGER_READ_HANDLE_SIZE]; /*!< The buffer for serial manager read handle */
    serial_read_handle_t serialReadHandle;                           /*!< The serial manager read handle */
    char *prompt;                                                    /*!< Prompt string */
#if (defined(SHELL_NON_BLOCKING_MODE) && (SHELL_NON_BLOCKING_MODE > 0U))

#if defined(OSA_USED)

#if (defined(SHELL_USE_COMMON_TASK) && (SHELL_USE_COMMON_TASK > 0U))
    common_task_message_t commontaskMsg; /*!< Message for common task */
#else
    uint8_t semphore[OSA_SEM_HANDLE_SIZE]; /*!< Event instance */
    uint8_t taskId[OSA_TASK_HANDLE_SIZE]; /*!< Task handle */
#endif

#endif

#endif
    char line[SHELL_BUFFER_SIZE];                          /*!< Consult buffer */
    char hist_buf[SHELL_HISTORY_COUNT][SHELL_BUFFER_SIZE]; /*!< History buffer*/
    char printBuffer[SHELL_SPRINTF_BUFFER_SIZE];           /*!< Buffer for print */
    uint32_t printLength;                                  /*!< All length has been printed */
    uint16_t hist_current;                                 /*!< Current history command in hist buff*/
    uint16_t hist_count;                                   /*!< Total history command in hist buff*/
    uint16_t l_pos;                                         /*!< Total line position */
    uint16_t c_pos;                                         /*!< Current line position */
    enum _fun_key_status stat;                             /*!< Special key status */
    uint8_t cmd_num;                                       /*!< Number of user commands */
    volatile uint8_t notificationPost;                     /*!< The serial manager notification is post */
    struct
    {
        uint8_t exit : 1;                                  /*!< Exit Flag*/
        uint8_t printBusy : 1;                             /*!< Print is busy */
        uint8_t noEcho : 1;                                /*!< No echo flag */
    } state;
} shell_context_handle_t;

#define SHELL_STRUCT_OFFSET(type, field) ((size_t) & (((type *)0)->field))
#define SHEEL_COMMAND_POINTER(node) \
    ((shell_command_t *)(((uint32_t)(node)) - SHELL_STRUCT_OFFSET(shell_command_t, link)))
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static shell_status_t SHELL_HelpCommand(shell_handle_t shellHandle, int32_t argc, char **argv); /*!< help command */

static shell_status_t SHELL_ExitCommand(shell_handle_t shellHandle, int32_t argc, char **argv); /*!< exit command */

#if (defined(SHELL_ADVANCE) && (SHELL_ADVANCE > 0))
#if (SHELL_ECHO > 0)
static shell_status_t SHELL_EchoCommand(shell_handle_t shellHandle, int32_t argc, char **argv); /*!< echo command */
#endif
#endif

static int32_t SHELL_ParseLine(const char *cmd, uint32_t len, char *argv[]); /*!< parse line command */

static int32_t SHELL_StringCompare(const char *str1, const char *str2, int32_t count); /*!< compare string command */

static void SHELL_ProcessCommand(shell_context_handle_t *shellContextHandle, const char *cmd); /*!< process a command */

static void SHELL_GetHistoryCommand(shell_context_handle_t *shellContextHandle,
                                    uint16_t hist_pos); /*!< get commands history */

static void SHELL_AutoComplete(shell_context_handle_t *shellContextHandle); /*!< auto complete command */

static shell_status_t SHELL_GetChar(shell_context_handle_t *shellContextHandle,
                                    uint8_t *ch); /*!< get a char from communication interface */

static shell_status_t SHELL_PrintCommandPath(shell_context_handle_t *shellContextHandle, shell_command_t *cmd, char *path, uint32_t pathLength);

#if (defined(SHELL_NON_BLOCKING_MODE) && (SHELL_NON_BLOCKING_MODE > 0U))
static void SHELL_Task(void *param); /*!<  Shell task*/
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/

static SHELL_COMMAND_DEFINE(help, "\"help\": List all the registered commands\r\n", SHELL_HelpCommand, 0);
static SHELL_COMMAND_DEFINE(exit, "\"exit\": Exit program\r\n", SHELL_ExitCommand, 0);
#if (defined(SHELL_ADVANCE) && (SHELL_ADVANCE > 0))
#if (SHELL_ECHO > 0)
static SHELL_COMMAND_DEFINE(echo, "\"echo\": Set echo(0 - disable, 1 - enable)\r\n", SHELL_EchoCommand, 1);
#endif
#endif

static char s_paramBuffer[SHELL_BUFFER_SIZE];

#if (defined(SHELL_ADVANCE) && (SHELL_ADVANCE > 0))
static char s_cmdLineBuffer[SHELL_BUFFER_SIZE];
#endif /* SHELL_ADVANCE */

#if (defined(SHELL_NON_BLOCKING_MODE) && (SHELL_NON_BLOCKING_MODE > 0U))

#if defined(OSA_USED)

#if (defined(SHELL_USE_COMMON_TASK) && (SHELL_USE_COMMON_TASK > 0U))

#else
/*
 * \brief Defines the serial manager task's stack
 */
OSA_TASK_DEFINE(SHELL_Task, SHELL_TASK_PRIORITY, 1, SHELL_TASK_STACK_SIZE, false);
#endif

#endif

#endif
/*******************************************************************************
 * Code
 ******************************************************************************/

#if (defined(SHELL_NON_BLOCKING_MODE) && (SHELL_NON_BLOCKING_MODE > 0U))
static void SHELL_SerialManagerRxCallback(void *callbackParam,
                                          serial_manager_callback_message_t *message,
                                          serial_manager_status_t status)
{
    shell_context_handle_t *shellHandle;

    assert(callbackParam);
    assert(message);

    shellHandle = (shell_context_handle_t *)callbackParam;

    if (!shellHandle->notificationPost)
    {
        shellHandle->notificationPost = 1U;
#if defined(OSA_USED)

#if (defined(SHELL_USE_COMMON_TASK) && (SHELL_USE_COMMON_TASK > 0U))
        shellHandle->commontaskMsg.callback      = SHELL_Task;
        shellHandle->commontaskMsg.callbackParam = shellHandle;
        COMMON_TASK_post_message(&shellHandle->commontaskMsg);
#else
        (void)OSA_SemaphorePost((osa_semaphore_handle_t)shellHandle->semphore);
#endif

#else
        SHELL_Task(shellHandle);
#endif
    }
}
#endif

static void SHELL_WriteBuffer(char *buffer, int32_t *indicator, char val, int len)
{
    shell_context_handle_t *shellContextHandle;
    int i              = 0;
    shellContextHandle = (shell_context_handle_t *)buffer;

    for (i = 0; i < len; i++)
    {
        if ((uint32_t)(*indicator + 1) >= SHELL_SPRINTF_BUFFER_SIZE)
        {
#if (!defined(SDK_DEBUGCONSOLE_UART) && (defined(SDK_DEBUGCONSOLE) && (SDK_DEBUGCONSOLE != 1)))
            if (NULL == shellContextHandle->serialHandle)
            {
                for (uint32_t index = 0; index < ((uint32_t)*indicator); index++)
                {
                    (void)putchar(shellContextHandle->printBuffer[index]);
                }
            }
            else
#endif
            {
                (void)SerialManager_WriteBlocking(shellContextHandle->serialWriteHandle,
                                                  (uint8_t *)shellContextHandle->printBuffer, (uint32_t)*indicator);
            }

            shellContextHandle->printLength += (uint32_t)*indicator;
            *indicator = 0;
        }

        shellContextHandle->printBuffer[*indicator] = val;
        (*indicator)++;
    }
}

static int SHELL_Sprintf(void *buffer, const char *formatString, va_list ap)
{
    shell_context_handle_t *shellContextHandle;
    uint32_t length;
    shellContextHandle = (shell_context_handle_t *)buffer;

    length = (uint32_t)StrFormatPrintf(formatString, ap, (char *)buffer, SHELL_WriteBuffer);
    shellContextHandle->printLength += length;
    return (int32_t)length;
}

#if (defined(SHELL_NON_BLOCKING_MODE) && (SHELL_NON_BLOCKING_MODE > 0U))
static void SHELL_Task(void *param)
#else
void SHELL_Task(shell_handle_t shellHandle)
#endif
{
#if (defined(SHELL_NON_BLOCKING_MODE) && (SHELL_NON_BLOCKING_MODE > 0U))
    shell_context_handle_t *shellContextHandle = (shell_context_handle_t *)param;
#else
    shell_context_handle_t *shellContextHandle = (shell_context_handle_t *)shellHandle;
#endif
    uint8_t ch;

    if (NULL != shellContextHandle)
    {
#if (defined(SHELL_NON_BLOCKING_MODE) && (SHELL_NON_BLOCKING_MODE > 0U))

#if defined(OSA_USED)

#if (defined(SHELL_USE_COMMON_TASK) && (SHELL_USE_COMMON_TASK > 0U))
#else
        do
        {
            if (KOSA_StatusSuccess == OSA_SemaphoreWait((osa_semaphore_handle_t)shellContextHandle->semphore, osaWaitForever_c))
            {
#endif

#endif

#endif
        {
            shellContextHandle->notificationPost = 0;
            do
            {
                if (1 == shellContextHandle->state.exit)
                {
                    if (shellContextHandle->serialReadHandle != NULL)
                    {
                        (void)SerialManager_CloseReadHandle(shellContextHandle->serialReadHandle);
                        shellContextHandle->serialReadHandle = NULL;
                    }
                    if (shellContextHandle->serialWriteHandle != NULL)
                    {
                        (void)SerialManager_CloseWriteHandle(shellContextHandle->serialWriteHandle);
                        shellContextHandle->serialWriteHandle = NULL;
                    }
                    break;
                }
                if (kStatus_SHELL_Success != (shell_status_t)SHELL_GetChar(shellContextHandle, &ch))
                {
                    /* If error occurred when getting a char, exit the task and waiting the new data arriving. */
                    break;
                }

                /* Special key */
                if (ch == KEY_ESC)
                {
                    shellContextHandle->stat = kSHELL_Special;
                    continue;
                }
                else if (shellContextHandle->stat == kSHELL_Special)
                {
                    /* Function key */
                    if ((char)ch == '[')
                    {
                        shellContextHandle->stat = kSHELL_Function;
                        continue;
                    }
                    shellContextHandle->stat = kSHELL_Normal;
                }
                else if (shellContextHandle->stat == kSHELL_Function)
                {
                    shellContextHandle->stat = kSHELL_Normal;

                    switch ((uint8_t)ch)
                    {
                        /* History operation here */
                        case 'A': /* Up key */
                            SHELL_GetHistoryCommand(shellContextHandle, (uint16_t)shellContextHandle->hist_current);
                            if (shellContextHandle->hist_current < (shellContextHandle->hist_count - 1U))
                            {
                                shellContextHandle->hist_current++;
                            }
                            break;
                        case 'B': /* Down key */
                            SHELL_GetHistoryCommand(shellContextHandle, (uint16_t)shellContextHandle->hist_current);
                            if (shellContextHandle->hist_current > 0U)
                            {
                                shellContextHandle->hist_current--;
                            }
                            break;
                        case 'D': /* Left key */
                            if ((bool)shellContextHandle->c_pos)
                            {
#if (SHELL_ECHO > 0)
                                if (0 == shellContextHandle->state.noEcho)
                                {
                                    (void)SHELL_Write(shellContextHandle, (char *)"\b", 1);
                                }
#endif
                                shellContextHandle->c_pos--;
                            }
                            break;
                        case 'C': /* Right key */
                            if (shellContextHandle->c_pos < shellContextHandle->l_pos)
                            {
#if (SHELL_ECHO > 0)
                                if (0 == shellContextHandle->state.noEcho)
                                {
                                    (void)SHELL_Write(shellContextHandle,
                                                      &shellContextHandle->line[shellContextHandle->c_pos], 1);
                                }
#endif
                                shellContextHandle->c_pos++;
                            }
                            break;
                        default:
                            break;
                    }
                    continue;
                }
                /* Handle tab key */
                else if ((char)ch == '\t')
                {
#if SHELL_AUTO_COMPLETE
                    /* Move the cursor to the beginning of line */
                    int i;
                    for (i = 0; i < shellContextHandle->c_pos; i++)
                    {
#if (SHELL_ECHO > 0)
                        if (0 == shellContextHandle->state.noEcho)
                        {
                            (void)SHELL_Write(shellContextHandle, (char *)"\b", 1);
                        }
#endif
                    }
                    /* Do auto complete */
                    SHELL_AutoComplete(shellContextHandle);
                    /* Move position to end */
                    shellContextHandle->c_pos = shellContextHandle->l_pos = (uint16_t)strlen(shellContextHandle->line);
#endif
                    continue;
                }
                /* Handle backspace key */
                else if ((ch == KET_DEL) || ((char)ch == '\b'))
                {
                    uint8_t l_pos_sub =0;

                    if (shellContextHandle->l_pos > 0)
                    {
                        shellContextHandle->l_pos--;
                        l_pos_sub = 1;
                    }

                    if (shellContextHandle->c_pos > 0)
                    {
                        shellContextHandle->c_pos--;
                    }
                    else
                    {
                        if (l_pos_sub > 0)
                        {
                            shellContextHandle->l_pos ++;
                        }
                        continue;
                    }

                    if (shellContextHandle->l_pos > shellContextHandle->c_pos)
                    {
                        (void)memmove(&shellContextHandle->line[shellContextHandle->c_pos],
                                      &shellContextHandle->line[shellContextHandle->c_pos + 1U],
                                      shellContextHandle->l_pos - shellContextHandle->c_pos);
                        shellContextHandle->line[shellContextHandle->l_pos] = 0U;
#if (SHELL_ECHO > 0)
                        if (0 == shellContextHandle->state.noEcho)
                        {
                            (void)SHELL_Write(shellContextHandle, (char *)"\b", 1);
                            (void)SHELL_Write(shellContextHandle, &shellContextHandle->line[shellContextHandle->c_pos],
                                              strlen(&shellContextHandle->line[shellContextHandle->c_pos]));
                            (void)SHELL_Write(shellContextHandle, (char *)"  \b", 3);

                            /* Reset position */
                            int i;
                            for (i = shellContextHandle->c_pos; i <= shellContextHandle->l_pos; i++)
                            {
                                (void)SHELL_Write(shellContextHandle, (char *)"\b", 1);
                            }
                        }
#endif
                    }
                    else /* Normal backspace operation */
                    {
#if (SHELL_ECHO > 0)
                        if (0 == shellContextHandle->state.noEcho)
                        {
                            (void)SHELL_Write(shellContextHandle, (char *)"\b \b", 3);
                        }
#endif
                        shellContextHandle->line[shellContextHandle->l_pos] = 0U;
                    }
                    continue;
                }
                else
                {
                }

                /* Input too long */
                if (shellContextHandle->l_pos >= (SHELL_BUFFER_SIZE - 1U))
                {
                    shellContextHandle->l_pos = 0;
                }

                /* Handle end of line, break */
                if (((char)ch == '\r') || ((char)ch == '\n'))
                {
                    static char endoflinechar = 0U;

                    if (((uint8_t)endoflinechar != 0U) && ((uint8_t)endoflinechar != ch))
                    {
                        continue;
                    }
                    else
                    {
                        endoflinechar = (char)ch;
                        (void)SHELL_Write(shellContextHandle, (char *)"\r\n", 2);
                        /* If command line is or start with NULL, will start a new transfer */
                        if (0U == strlen(shellContextHandle->line))
                        {
                            /* Reset all params */
                            shellContextHandle->c_pos = shellContextHandle->l_pos = 0;
                            shellContextHandle->hist_current                      = 0;
                            (void)SHELL_Write(shellContextHandle, shellContextHandle->prompt,
                                              strlen(shellContextHandle->prompt));
                            (void)memset(shellContextHandle->line, 0, sizeof(shellContextHandle->line));
                            continue;
                        }
                        SHELL_ProcessCommand(shellContextHandle, shellContextHandle->line);
                        /* Reset all params */
                        shellContextHandle->c_pos = shellContextHandle->l_pos = 0;
                        shellContextHandle->hist_current                      = 0;
                        (void)SHELL_Write(shellContextHandle, shellContextHandle->prompt,
                                          strlen(shellContextHandle->prompt));
                        memset(shellContextHandle->line, 0, sizeof(shellContextHandle->line));
                        continue;
                    }
                }

                /* Normal character */
                if (shellContextHandle->c_pos < shellContextHandle->l_pos)
                {
                    (void)memmove(&shellContextHandle->line[shellContextHandle->c_pos + 1U],
                                  &shellContextHandle->line[shellContextHandle->c_pos],
                                  shellContextHandle->l_pos - shellContextHandle->c_pos);
                    shellContextHandle->line[shellContextHandle->c_pos] = (char)ch;
#if (SHELL_ECHO > 0)
                    if (0 == shellContextHandle->state.noEcho)
                    {
                        (void)SHELL_Write(shellContextHandle, &shellContextHandle->line[shellContextHandle->c_pos],
                                          strlen(&shellContextHandle->line[shellContextHandle->c_pos]));
                        /* Move the cursor to new position */
                        int i;
                        for (i = shellContextHandle->c_pos; i < shellContextHandle->l_pos; i++)
                        {
                            (void)SHELL_Write(shellContextHandle, (char *)"\b", 1);
                        }
                    }
#endif
                }
                else
                {
                    shellContextHandle->line[shellContextHandle->l_pos] = (char)ch;
#if (SHELL_ECHO > 0)
                    if (0 == shellContextHandle->state.noEcho)
                    {
                        (void)SHELL_Write(shellContextHandle, &shellContextHandle->line[shellContextHandle->l_pos], 1);
                    }
#endif
                }

                ch = 0;
                shellContextHandle->l_pos++;
                shellContextHandle->c_pos++;
            } while (0 == shellContextHandle->state.exit);
        }
#if (defined(SHELL_NON_BLOCKING_MODE) && (SHELL_NON_BLOCKING_MODE > 0U))

#if defined(OSA_USED)

#if (defined(SHELL_USE_COMMON_TASK) && (SHELL_USE_COMMON_TASK > 0U))
#else
            }
        } while (gUseRtos_c);
#endif

#endif

#endif
    }
}

#if (defined(SHELL_ADVANCE) && (SHELL_ADVANCE > 0))
static void SHELL_PrintPrefixBlank(shell_context_handle_t *shellContextHandle, char *c, uint32_t count)
{
    uint32_t i;

    for (i = 0;i < count;i++)
    {
        (void)SHELL_Write(shellContextHandle, c, strlen(c));
    }
}
#endif /* SHELL_ADVANCE */

static shell_status_t SHELL_PrintCmdHelp(shell_context_handle_t *shellContextHandle, shell_command_t *cmd, uint32_t level)
{
#if (defined(SHELL_ADVANCE) && (SHELL_ADVANCE > 0))
    uint32_t i = 0;
    shell_command_t *cmdP;
#endif /* SHELL_ADVANCE */

    if (NULL == cmd)
    {
        return kStatus_SHELL_Success;
    }

    if ((cmd->pcHelpString != NULL) && (bool)strlen(cmd->pcHelpString))
    {
#if (defined(SHELL_ADVANCE) && (SHELL_ADVANCE > 0))
        SHELL_PrintPrefixBlank(shellContextHandle, (char *)"    ", level);
        SHELL_Write(shellContextHandle, (char *)"+---", 4);
#endif /* SHELL_ADVANCE */
        (void)SHELL_Write(shellContextHandle, cmd->pcHelpString, strlen(cmd->pcHelpString));
    }

#if (defined(SHELL_ADVANCE) && (SHELL_ADVANCE > 0))
    if (NULL != cmd->commandList)
    {
        while (NULL != cmd->commandList[i])
        {
            cmdP = cmd->commandList[i];
            (void)SHELL_PrintCmdHelp(shellContextHandle, cmdP, level + 1);
            i++;
        }
    }
#endif /* SHELL_ADVANCE */

    return kStatus_SHELL_Success;
}

static shell_status_t SHELL_PrintAllCmdHelp(shell_context_handle_t *shellContextHandle)
{
    shell_command_t *shellCommandContextHandle;
    list_element_handle_t p = LIST_GetHead(&shellContextHandle->commandContextListHead);

    SHELL_Printf(shellContextHandle, "\r\n");
    while (p != NULL)
    {
        shellCommandContextHandle = SHEEL_COMMAND_POINTER(p);

        (void)SHELL_PrintCmdHelp(shellContextHandle, shellCommandContextHandle, 0);

        p = LIST_GetNext(p);
    }
#if 0
    (void)SHELL_Write(shellContextHandle, shellContextHandle->prompt, strlen(shellContextHandle->prompt));
#endif
    return kStatus_SHELL_Success;
}

#if (defined(SHELL_ADVANCE) && (SHELL_ADVANCE > 0))
static shell_status_t SHELL_FindCommand(shell_context_handle_t *shellContextHandle, shell_command_t *cmd, char *cmdLine, shell_command_t **cmdOutput)
{
    shell_command_t *cmdP;
    shell_command_t *cmdBest = NULL;
    shell_command_t *cmdTemp;
    shell_status_t status;
    uint32_t i = 0;

    if ((NULL == cmd) || (NULL == cmdOutput))
    {
        return kStatus_SHELL_Error;
    }

    if (0 != SHELL_StringCompare(cmd->pcCommand, cmdLine, (uint32_t)strlen(cmd->pcCommand)))
    {
        return kStatus_SHELL_Error;
    }

    if ((NULL != cmd->commandList) && (strlen(cmdLine) > ((uint32_t)strlen(cmd->pcCommand))))
    {
        if (SHELL_COMMAND_CONNECTOR != cmdLine[((uint32_t)strlen(cmd->pcCommand))])
        {
            return kStatus_SHELL_Error;
        }

        while (NULL != cmd->commandList[i])
        {
            cmdP = cmd->commandList[i];
            status = SHELL_FindCommand(shellContextHandle, cmdP, &cmdLine[((uint32_t)strlen(cmd->pcCommand)) + 1], &cmdTemp);

            if (kStatus_SHELL_Success == status)
            {
                if (NULL == cmdBest)
                {
                    cmdBest = cmdTemp;
                }
                else
                {
                    uint32_t deltaBest = 0xFF;
                    uint32_t deltaTemp = 0xFF;

                    memset(s_cmdLineBuffer, 0U, sizeof(s_cmdLineBuffer));
                    if (kStatus_SHELL_Success == SHELL_PrintCommandPath(shellContextHandle, cmdBest, s_cmdLineBuffer, sizeof(s_cmdLineBuffer)))
                    {
                        if (strlen(cmdLine) > strlen(s_cmdLineBuffer))
                        {
                            deltaBest = strlen(cmdLine) - strlen(s_cmdLineBuffer);
                        }
                        else
                        {
                            deltaBest = strlen(s_cmdLineBuffer) - strlen(cmdLine);
                        }
                    }

                    memset(s_cmdLineBuffer, 0U, sizeof(s_cmdLineBuffer));
                    if (kStatus_SHELL_Success == SHELL_PrintCommandPath(shellContextHandle, cmdTemp, s_cmdLineBuffer, sizeof(s_cmdLineBuffer)))
                    {
                        if (strlen(cmdLine) > strlen(s_cmdLineBuffer))
                        {
                            deltaTemp = strlen(cmdLine) - strlen(s_cmdLineBuffer);
                        }
                        else
                        {
                            deltaTemp = strlen(s_cmdLineBuffer) - strlen(cmdLine);
                        }
                    }

                    if (deltaTemp < deltaBest)
                    {
                        cmdBest = cmdTemp;
                    }
                }
            }
            i++;
        }
        if (NULL != cmdBest)
        {
            *cmdOutput = cmdBest;
            return kStatus_SHELL_Success;
        }
    }
    else
    {
        *cmdOutput = cmd;

        return kStatus_SHELL_Success;
    }

    return kStatus_SHELL_Error;
}
#endif /* SHELL_ADVANCE */

static shell_status_t SHELL_HelpCommand(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    (void)argc;
    (void)argv;

    return SHELL_PrintAllCmdHelp((shell_context_handle_t *)shellHandle);
}

static shell_status_t SHELL_ExitCommand(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    shell_context_handle_t *shellContextHandle = (shell_context_handle_t *)shellHandle;
    /* Skip warning */
    (void)SHELL_Write(shellContextHandle, (char *)"\r\nSHELL exited\r\n", strlen("\r\nSHELL exited\r\n"));
    shellContextHandle->state.exit = (uint8_t) 1;
    return kStatus_SHELL_Success;
}

#if (defined(SHELL_ADVANCE) && (SHELL_ADVANCE > 0))
#if (SHELL_ECHO > 0)
static shell_status_t SHELL_EchoCommand(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    shell_context_handle_t *shellContextHandle = (shell_context_handle_t *)shellHandle;
    /* Skip warning */
    (void)SHELL_Printf(shellContextHandle, "\r\nSHELL echo %s\r\n", ('0' == argv[1][0]) ? "disabled" : "enabled");
    shellContextHandle->state.noEcho = (uint8_t) (('0' == argv[1][0]) ? 1 : 0);
    return kStatus_SHELL_Success;
}
#endif
#endif

static void SHELL_ProcessCommand(shell_context_handle_t *shellContextHandle, const char *cmd)
{
    shell_command_t *tmpCommand = NULL;
    const char *tmpCommandString;
    int32_t argc;
    char *argv[SHELL_MAX_ARGS];
    list_element_handle_t p;
    uint8_t flag = 1;
    uint16_t tmpCommandLen;
    uint16_t tmpLen;
    uint16_t i = 0;

    tmpLen = (uint16_t)strlen(cmd);
    argc   = SHELL_ParseLine(cmd, tmpLen, argv);

    if ((argc > 0))
    {
        p = LIST_GetHead(&shellContextHandle->commandContextListHead);
        while (p != NULL)
        {
            tmpCommand       = SHEEL_COMMAND_POINTER(p);
            tmpCommandString = tmpCommand->pcCommand;
            tmpCommandLen    = (uint16_t)strlen(tmpCommandString);
            /* Compare with space or end of string */
            if ((cmd[tmpCommandLen] == ' ') || (cmd[tmpCommandLen] == (char)0x00)
#if (defined(SHELL_ADVANCE) && (SHELL_ADVANCE > 0))
                || (SHELL_COMMAND_CONNECTOR == cmd[tmpCommandLen])
#endif /* SHELL_ADVANCE */
                  )
            {
                if (SHELL_StringCompare(tmpCommandString, argv[0], (uint32_t)tmpCommandLen) == 0)
                {
#if (defined(SHELL_ADVANCE) && (SHELL_ADVANCE > 0))
                    if (kStatus_SHELL_Success != SHELL_FindCommand(shellContextHandle, tmpCommand, argv[0], &tmpCommand))
                    {
                        p = LIST_GetNext(p);
                        continue;
                    }
                    /* support commands with optional number of parameters */
                    if (tmpCommand->parameter.cExpectedNumberOfParameters >=
                        (uint32_t)(((uint32_t)SHELL_IGNORE_PARAMETER_COUNT) | (((uint32_t)SHELL_IGNORE_PARAMETER_COUNT) << 16)))
                    {
                        flag = 0;
                    }
                    else if (((tmpCommand->parameter.range.min + 1) <= argc) && ((tmpCommand->parameter.range.max + 1) >= argc))
                    {
                        flag = 0;
                    }
                    else
#else
                    /* support commands with optional number of parameters */
                    if (tmpCommand->cExpectedNumberOfParameters == (uint8_t)SHELL_IGNORE_PARAMETER_COUNT)
                    {
                        flag = 0;
                    }
                    else if ((tmpCommand->cExpectedNumberOfParameters == 0U) && (argc == 1))
                    {
                        flag = 0;
                    }
                    else if (tmpCommand->cExpectedNumberOfParameters > 0U)
                    {
                        if ((argc - 1U) == tmpCommand->cExpectedNumberOfParameters)
                        {
                            flag = 0;
                        }
                    }
                    else
#endif /* SHELL_ADVANCE */
                    {
                        flag = 1;
                    }
                    break;
                }
            }
            p = LIST_GetNext(p);
        }
        if (NULL == p)
        {
            tmpCommand = NULL;
        }
    }

    if ((tmpCommand != NULL) && (flag == 1U))
    {
        (void)SHELL_Write(
            shellContextHandle,
            (char *)"\r\nIncorrect command parameter(s).  Enter \"help\" to view a list of available commands.\r\n\r\n",
            strlen(
                "\r\nIncorrect command parameter(s).  Enter \"help\" to view a list of available commands.\r\n\r\n"));
        tmpCommand = NULL;
    }
    else if (tmpCommand != NULL)
    {
        shell_status_t ret;
        tmpLen = (uint16_t)strlen(cmd);
        /* Compare with last command. Push back to history buffer if different */
        if (tmpLen != (uint16_t)SHELL_StringCompare(cmd, shellContextHandle->hist_buf[0], (int32_t)strlen(cmd)))
        {
            for (i = SHELL_HISTORY_COUNT - 1U; i > 0U; i--)
            {
                (void)memset(shellContextHandle->hist_buf[i], '\0', SHELL_BUFFER_SIZE);
                tmpLen = (uint16_t)strlen(shellContextHandle->hist_buf[i - 1U]);
                (void)memcpy(shellContextHandle->hist_buf[i], shellContextHandle->hist_buf[i - 1U], tmpLen);
            }
            (void)memset(shellContextHandle->hist_buf[0], '\0', SHELL_BUFFER_SIZE);
            tmpLen = (uint16_t)strlen(cmd);
            (void)memcpy(shellContextHandle->hist_buf[0], cmd, tmpLen);
            if (shellContextHandle->hist_count < SHELL_HISTORY_COUNT)
            {
                shellContextHandle->hist_count++;
            }
        }
#if (defined(SHELL_ADVANCE) && (SHELL_ADVANCE > 0))
        memcpy(argv[0], tmpCommand->pcCommand, strlen(tmpCommand->pcCommand) + 1);
#endif /* SHELL_ADVANCE */
        if (NULL !=  tmpCommand->pFuncCallBack)
        {
            ret = tmpCommand->pFuncCallBack(shellContextHandle, argc, argv);
        }
        else
        {
            ret = kStatus_SHELL_PrintCmdHelp;
        }
        if (kStatus_SHELL_Success == ret)
        {
            /* Command executed successfully */
        }
        else
        {
            /* error is occurred */
#if (defined(SHELL_ADVANCE) && (SHELL_ADVANCE > 0))
            if (kStatus_SHELL_PrintCmdHelp == ret)
            {
                SHELL_PrintCmdHelp(shellContextHandle, tmpCommand, 0);
            }
            else if (kStatus_SHELL_PrintAllHelp == ret)
            {
                SHELL_PrintAllCmdHelp(shellContextHandle);
            }
            else
            {
                /* Other errors */
            }
            (void)SHELL_Printf(shellContextHandle, "Command executed failure\r\n");
#endif /* SHELL_ADVANCE */
        }
        tmpCommand = NULL;
    }
    else
    {
        (void)SHELL_Write(
            shellContextHandle,
            (char *)"\r\nCommand not recognized.  Enter 'help' to view a list of available commands.\r\n\r\n",
            strlen("\r\nCommand not recognized.  Enter 'help' to view a list of available commands.\r\n\r\n"));
        tmpCommand = NULL;
    }
}

static void SHELL_GetHistoryCommand(shell_context_handle_t *shellContextHandle, uint16_t hist_pos)
{
    uint32_t i;
    uint32_t tmp;

    if (shellContextHandle->hist_buf[0][0] == '\0')
    {
        shellContextHandle->hist_current = 0;
        return;
    }

#if 0 /*hist_pos is passed from hist_current. And hist_current is only changed in case 'A'/'B',as hist_count is 3 \
         most, it can't be more than 3  */
    if (hist_pos >= SHELL_HISTORY_COUNT)
    {
        hist_pos = SHELL_HISTORY_COUNT - 1U;
    }
#endif

    tmp = strlen(shellContextHandle->line);
    /* Clear current if have */
    if (tmp > 0U)
    {
        (void)memset(shellContextHandle->line, '\0', tmp);
        for (i = 0; i < tmp; i++)
        {
            (void)SHELL_Write(shellContextHandle, (char *)"\b \b", 3);
        }
    }

    shellContextHandle->l_pos = (uint16_t)strlen(shellContextHandle->hist_buf[hist_pos]);
    shellContextHandle->c_pos = shellContextHandle->l_pos;
    (void)memcpy(shellContextHandle->line, shellContextHandle->hist_buf[hist_pos], shellContextHandle->l_pos);
    (void)SHELL_Write(shellContextHandle, shellContextHandle->hist_buf[hist_pos],
                      strlen(shellContextHandle->hist_buf[hist_pos]));
}

#if (defined(SHELL_ADVANCE) && (SHELL_ADVANCE > 0))

static shell_status_t SHELL_PrintCommandPathRecursion(shell_context_handle_t *shellContextHandle, shell_command_t *cmdStart, shell_command_t *cmdTarget, char *path, uint32_t pathLength)
{
    shell_command_t *cmdP;
    uint32_t i = 0;

    if ((NULL == cmdStart) || (NULL == cmdTarget) ||(NULL == path))
    {
        return kStatus_SHELL_Error;
    }

    if ((NULL != cmdStart->commandList))
    {
        while (NULL != cmdStart->commandList[i])
        {
            cmdP = cmdStart->commandList[i];
            if (cmdP == cmdTarget)
            {
                memset(path, 0, pathLength);
                memcpy(path, cmdP->pcCommand, strlen(cmdP->pcCommand));
                return kStatus_SHELL_Success;
            }
            else
            {
                if (kStatus_SHELL_Success == SHELL_PrintCommandPathRecursion(shellContextHandle, cmdP, cmdTarget, path, pathLength))
                {
                    uint32_t length = strlen(path);
                    if((length + strlen(cmdP->pcCommand)) < pathLength)
                    {
                        for (uint32_t index = 0;index < length;index++)
                        {
                            path[(length - index) + strlen(cmdP->pcCommand)] = path[(length - index) - 1];
                        }
                        memcpy(path, cmdP->pcCommand, strlen(cmdP->pcCommand));
                        path[strlen(cmdP->pcCommand)]     = SHELL_COMMAND_CONNECTOR;
                        path[pathLength-1] = '\0';
                        return kStatus_SHELL_Success;
                    }

                    return kStatus_SHELL_Error;
                }
            }
            i++;
        }
    }
    return kStatus_SHELL_Error;
}

static shell_status_t SHELL_PrintCommandPath(shell_context_handle_t *shellContextHandle, shell_command_t *cmd, char *path, uint32_t pathLength)
{
    list_element_handle_t cmdP;
    shell_command_t *cmdTemp = NULL;

        /* Do auto complete */
    cmdP = LIST_GetHead(&shellContextHandle->commandContextListHead);
    while (cmdP != NULL)
    {
        cmdTemp = SHEEL_COMMAND_POINTER(cmdP);
        if (kStatus_SHELL_Success == SHELL_PrintCommandPathRecursion(shellContextHandle, cmdTemp, cmd, path, pathLength))
        {
            uint32_t length = strlen(path);
            if((length + strlen(cmdTemp->pcCommand)) < pathLength)
            {
                for (uint32_t index = 0;index < length;index++)
                {
                    path[(length - index) + strlen(cmdTemp->pcCommand)] = path[(length - index) - 1];
                }
                memcpy(path, cmdTemp->pcCommand, strlen(cmdTemp->pcCommand));
                path[strlen(cmdTemp->pcCommand)]     = SHELL_COMMAND_CONNECTOR;
                path[pathLength-1] = '\0';
                return kStatus_SHELL_Success;
            }

            return kStatus_SHELL_Error;
        }
        if (cmdTemp == cmd)
        {
            memset(path, 0, pathLength);
            memcpy(path, cmdTemp->pcCommand, strlen(cmdTemp->pcCommand));
            return kStatus_SHELL_Success;
        }
        cmdP = LIST_GetNext(cmdP);
    }

    return kStatus_SHELL_Error;
}

static shell_status_t SHELL_AutoCompleteCommand(shell_context_handle_t *shellContextHandle, shell_command_t *cmd, char *cmdLine, shell_command_t **cmdOut)
{
    shell_command_t *cmdP;
    shell_command_t *cmdTemp = NULL;
    uint32_t i = 0;

    if (NULL == cmd)
    {
        return kStatus_SHELL_Error;
    }

    if (strlen(cmdLine) <= strlen(cmd->pcCommand))
    {
        if (0 != SHELL_StringCompare(cmdLine, cmd->pcCommand, (uint32_t)strlen(cmdLine)))
        {
            return kStatus_SHELL_Error;
        }
        else
        {
            memset(s_cmdLineBuffer, 0U, sizeof(s_cmdLineBuffer));
            /* Show possible matches */
            if (kStatus_SHELL_Success == SHELL_PrintCommandPath(shellContextHandle, cmd, s_cmdLineBuffer, sizeof(s_cmdLineBuffer)))
            {
                SHELL_Printf(shellContextHandle, "%s\r\n", (char *)s_cmdLineBuffer);
            }
            *cmdOut = cmd;
            return kStatus_SHELL_Success;
        }
    }

    if (0 != SHELL_StringCompare(cmd->pcCommand, cmdLine, (uint32_t)strlen(cmd->pcCommand)))
    {
        return kStatus_SHELL_Error;
    }

    if ((NULL != cmd->commandList) && (strlen(cmdLine) > ((uint32_t)strlen(cmd->pcCommand))))
    {
        if (SHELL_COMMAND_CONNECTOR != cmdLine[((uint32_t)strlen(cmd->pcCommand))])
        {
            return kStatus_SHELL_Error;
        }

        while (NULL != cmd->commandList[i])
        {
            cmdP = cmd->commandList[i];
            (void)SHELL_AutoCompleteCommand(shellContextHandle, cmdP, &cmdLine[((uint32_t)strlen(cmd->pcCommand)) + 1], &cmdTemp);
            if (NULL != cmdTemp)
            {
                *cmdOut = cmdTemp;
            }
            i++;
        }
    }

    if (NULL != *cmdOut)
    {
        return kStatus_SHELL_Success;
    }

    return kStatus_SHELL_Error;
}
#endif /* SHELL_ADVANCE */

static void SHELL_AutoComplete(shell_context_handle_t *shellContextHandle)
{
    int32_t minLen;
    list_element_handle_t p;
    shell_command_t *tmpCommand = NULL;
    const char *namePtr;
    const char *cmdName;

    minLen  = SHELL_BUFFER_SIZE;
    namePtr = NULL;

    /* Empty tab, list all commands */
    if (shellContextHandle->line[0] == '\0')
    {
        (void)SHELL_HelpCommand(shellContextHandle, 0, NULL);
        return;
    }

    SHELL_Write(shellContextHandle, (char *)"\r\n", 2);

    /* Do auto complete */
    p = LIST_GetHead(&shellContextHandle->commandContextListHead);
    while (p != NULL)
    {
        tmpCommand = SHEEL_COMMAND_POINTER(p);
        cmdName    = tmpCommand->pcCommand;
#if (defined(SHELL_ADVANCE) && (SHELL_ADVANCE > 0))
        if ((NULL != tmpCommand->commandList))
        {
            shell_command_t *cmdTemp = NULL;
            if (kStatus_SHELL_Success == SHELL_AutoCompleteCommand(shellContextHandle, tmpCommand, shellContextHandle->line, &cmdTemp))
            {
                memset(s_cmdLineBuffer, 0U, sizeof(s_cmdLineBuffer));
                if (kStatus_SHELL_Success == SHELL_PrintCommandPath(shellContextHandle, cmdTemp, s_cmdLineBuffer, sizeof(s_cmdLineBuffer)))
                {
                    namePtr = s_cmdLineBuffer;
                    minLen  = (int32_t)strlen(s_cmdLineBuffer);
                }
            }
        }
        else
#endif /* SHELL_ADVANCE */
        {
            if (SHELL_StringCompare(shellContextHandle->line, cmdName, (int32_t)strlen(shellContextHandle->line)) == 0)
            {
                /* Show possible matches */
                SHELL_Printf(shellContextHandle, "%s\r\n", (char *)cmdName);
                if (minLen > ((int32_t)strlen(cmdName)))
                {
                    namePtr = cmdName;
                    minLen  = (int32_t)strlen(namePtr);
                }
            }
        }
        p = LIST_GetNext(p);
    }
    /* Auto complete string */
    if (namePtr != NULL)
    {
        (void)memcpy(shellContextHandle->line, namePtr, (uint32_t)minLen);
    }
    (void)SHELL_Write(shellContextHandle, shellContextHandle->prompt, strlen(shellContextHandle->prompt));
    (void)SHELL_Write(shellContextHandle, shellContextHandle->line, strlen(shellContextHandle->line));
    return;
}

static int32_t SHELL_StringCompare(const char *str1, const char *str2, int32_t count)
{
    while ((bool)(count--))
    {
        if (*str1++ != *str2++)
        {
            return (int32_t) * (const unsigned char *)(str1 - 1) - *(const unsigned char *)(str2 - 1);
        }
    }
    return kStatus_SHELL_Success;
}

static int32_t SHELL_ParseLine(const char *cmd, uint32_t len, char *argv[])
{
    uint32_t argc;
    char *p;
    uint32_t position;

    /* Init params */
    (void)memset(s_paramBuffer, '\0', len + 1U);
    (void)memcpy(s_paramBuffer, cmd, len);

    p        = s_paramBuffer;
    position = 0;
    argc     = 0;

    while (position < len)
    {
        /* Skip all blanks */
        while ((position < len) && ((char)(*p) == ' '))
        {
            *p = '\0';
            p++;
            position++;
        }

        if (position >= len)
        {
            break;
        }

        /* Process begin of a string */
        if (*p == '"')
        {
            p++;
            position++;
            argv[argc] = p;
            argc++;
            /* Skip this string */
            while ((*p != '"') && (position < len))
            {
                p++;
                position++;
            }
            /* Skip '"' */
            *p = '\0';
            p++;
            position++;
        }
        else /* Normal char */
        {
            argv[argc] = p;
            argc++;
            while (((char)*p != ' ') && (position < len))
            {
                p++;
                position++;
            }
        }

        if (argc >= SHELL_MAX_ARGS)
        {
            break;
        }
    }
    return (int32_t)argc;
}

static shell_status_t SHELL_GetChar(shell_context_handle_t *shellContextHandle, uint8_t *ch)
{
#if (!defined(SDK_DEBUGCONSOLE_UART) && (defined(SDK_DEBUGCONSOLE) && (SDK_DEBUGCONSOLE != 1)))
    if (NULL == shellContextHandle->serialHandle)
    {
        int ret;
        ret = getchar();
        if (ret > 0)
        {
            *ch = (uint8_t)ret;
            return kStatus_SHELL_Success;
        }
        else
        {
            return kStatus_SHELL_Error;
        }
    }
    else
#endif
    {
#if (defined(SHELL_NON_BLOCKING_MODE) && (SHELL_NON_BLOCKING_MODE > 0U))
        uint32_t length = 0;

        (void)SerialManager_TryRead(shellContextHandle->serialReadHandle, ch, 1, &length);

        if (length > 0)
        {
            return kStatus_SHELL_Success;
        }
        else
        {
            return kStatus_SHELL_Error;
        }
#else
        return (shell_status_t)SerialManager_ReadBlocking(shellContextHandle->serialReadHandle, ch, 1);
#endif
    }
}

shell_status_t SHELL_Init(shell_handle_t shellHandle, serial_handle_t serialHandle, char *prompt)
{
    shell_context_handle_t *shellContextHandle;
    serial_manager_status_t status = kStatus_SerialManager_Error;
    (void)status;

    assert(shellHandle);
#if !(!defined(SDK_DEBUGCONSOLE_UART) && (defined(SDK_DEBUGCONSOLE) && (SDK_DEBUGCONSOLE != 1)))
    assert(serialHandle);
#endif
    assert(prompt);

    assert(SHELL_HANDLE_SIZE >= sizeof(shell_context_handle_t));

    shellContextHandle = (shell_context_handle_t *)shellHandle;

    /* memory set for shellHandle */
    (void)memset(shellHandle, 0, SHELL_HANDLE_SIZE);

#if (!defined(SDK_DEBUGCONSOLE_UART) && (defined(SDK_DEBUGCONSOLE) && (SDK_DEBUGCONSOLE != 1)))
    if (NULL == serialHandle)
    {
    }
    else
#endif
    {
#if (defined(SHELL_NON_BLOCKING_MODE) && (SHELL_NON_BLOCKING_MODE > 0U))

#if defined(OSA_USED)

#if (defined(SHELL_USE_COMMON_TASK) && (SHELL_USE_COMMON_TASK > 0U))
        COMMON_TASK_init();
#else
        if (KOSA_StatusSuccess != OSA_SemaphoreCreate((osa_semaphore_handle_t)shellContextHandle->semphore, 0))
        {
            return kStatus_SHELL_Error;
        }

        if (KOSA_StatusSuccess !=
            OSA_TaskCreate((osa_task_handle_t)shellContextHandle->taskId, OSA_TASK(SHELL_Task), shellContextHandle))
        {
            return kStatus_SHELL_Error;
        }
#endif

#endif

#endif
    }

    shellContextHandle->prompt       = prompt;
    shellContextHandle->serialHandle = serialHandle;

#if (!defined(SDK_DEBUGCONSOLE_UART) && (defined(SDK_DEBUGCONSOLE) && (SDK_DEBUGCONSOLE != 1)))
    if (NULL == serialHandle)
    {
    }
    else
#endif
    {
        shellContextHandle->serialWriteHandle = (serial_write_handle_t)&shellContextHandle->serialWriteHandleBuffer[0];
        status = SerialManager_OpenWriteHandle(shellContextHandle->serialHandle, shellContextHandle->serialWriteHandle);
        assert(kStatus_SerialManager_Success == status);

        shellContextHandle->serialReadHandle = (serial_read_handle_t)&shellContextHandle->serialReadHandleBuffer[0];
        status = SerialManager_OpenReadHandle(shellContextHandle->serialHandle, shellContextHandle->serialReadHandle);
        assert(kStatus_SerialManager_Success == status);

#if (defined(SHELL_NON_BLOCKING_MODE) && (SHELL_NON_BLOCKING_MODE > 0U))
        status = SerialManager_InstallRxCallback(shellContextHandle->serialReadHandle, SHELL_SerialManagerRxCallback,
                                                 shellContextHandle);
        assert(kStatus_SerialManager_Success == status);
#endif
        (void)status;
    }

    (void)SHELL_RegisterCommand(shellContextHandle, SHELL_COMMAND(help));
    (void)SHELL_RegisterCommand(shellContextHandle, SHELL_COMMAND(exit));
#if (defined(SHELL_ADVANCE) && (SHELL_ADVANCE > 0))
#if (SHELL_ECHO > 0)
    (void)SHELL_RegisterCommand(shellContextHandle, SHELL_COMMAND(echo));
#endif
#endif

    (void)SHELL_Write(shellContextHandle, (char *)"\r\nSHELL build: ", strlen("\r\nSHELL build: "));
    (void)SHELL_Write(shellContextHandle, (char *)__DATE__, strlen(__DATE__));
    (void)SHELL_Write(shellContextHandle, (char *)"\r\nCopyright  2020  NXP\r\n",
                      strlen("\r\nCopyright  2020  NXP\r\n"));
    (void)SHELL_Write(shellContextHandle, shellContextHandle->prompt, strlen(shellContextHandle->prompt));

    return kStatus_SHELL_Success;
}

shell_status_t SHELL_RegisterCommand(shell_handle_t shellHandle, shell_command_t *command)
{
    shell_context_handle_t *shellContextHandle = (shell_context_handle_t *)shellHandle;
    assert(shellHandle);
    assert(command);

    /* memory set for shellHandle */
    (void)memset(&command->link, 0, sizeof(command->link));

    (void)LIST_AddTail(&shellContextHandle->commandContextListHead, &command->link);

    return kStatus_SHELL_Success;
}

shell_status_t SHELL_UnregisterCommand(shell_command_t *command)
{
    assert(command);

    (void)LIST_RemoveElement(&command->link);

    /* memory set for shellHandle */
    (void)memset(&command->link, 0, sizeof(command->link));

    return kStatus_SHELL_Success;
}

shell_status_t SHELL_Write(shell_handle_t shellHandle, char *buffer, uint32_t length)
{
    shell_context_handle_t *shellContextHandle;
    uint32_t primask;
    shell_status_t status = kStatus_SHELL_Error;

    assert(shellHandle);
    assert(buffer);

    if (!(bool)length)
    {
        return kStatus_SHELL_Success;
    }

    shellContextHandle = (shell_context_handle_t *)shellHandle;

    primask = DisableGlobalIRQ();
    if ((bool)shellContextHandle->state.printBusy)
    {
        EnableGlobalIRQ(primask);
        return kStatus_SHELL_Error;
    }
    shellContextHandle->state.printBusy = 1U;
    EnableGlobalIRQ(primask);
#if (!defined(SDK_DEBUGCONSOLE_UART) && (defined(SDK_DEBUGCONSOLE) && (SDK_DEBUGCONSOLE != 1)))
    if (NULL == shellContextHandle->serialHandle)
    {
        status = kStatus_SHELL_Success;
        for (uint32_t index = 0; index < length; index++)
        {
            (void)putchar(buffer[index]);
        }
    }
    else
#endif
    {
        status = (shell_status_t)SerialManager_WriteBlocking(shellContextHandle->serialWriteHandle, (uint8_t *)buffer,
                                                             length);
    }

    shellContextHandle->state.printBusy = 0U;

    return status;
}

int SHELL_Printf(shell_handle_t shellHandle, const char *formatString, ...)
{
    shell_context_handle_t *shellContextHandle;
    uint32_t length;
    uint32_t primask;
    va_list ap;

    assert(shellHandle);
    assert(formatString);

    shellContextHandle = (shell_context_handle_t *)shellHandle;

    primask = DisableGlobalIRQ();
    if ((bool)shellContextHandle->state.printBusy)
    {
        EnableGlobalIRQ(primask);
        return -1;
    }
    shellContextHandle->state.printBusy = 1U;
    EnableGlobalIRQ(primask);

    va_start(ap, formatString);

    shellContextHandle->printLength = 0U;
    length                          = (uint32_t)SHELL_Sprintf(shellHandle, formatString, ap);
#if (!defined(SDK_DEBUGCONSOLE_UART) && (defined(SDK_DEBUGCONSOLE) && (SDK_DEBUGCONSOLE != 1)))
    if (NULL == shellContextHandle->serialHandle)
    {
        for (uint32_t index = 0; index < length; index++)
        {
            (void)putchar(shellContextHandle->printBuffer[index]);
        }
    }
    else
#endif
    {
        (void)SerialManager_WriteBlocking(shellContextHandle->serialWriteHandle,
                                          (uint8_t *)shellContextHandle->printBuffer, length);
    }
    va_end(ap);

    shellContextHandle->state.printBusy = 0U;
    return (int32_t)shellContextHandle->printLength;
}

void SHELL_ChangePrompt(shell_handle_t shellHandle, char *prompt)
{
    shell_context_handle_t *shellContextHandle;
    assert(shellHandle);
    assert(prompt);

    shellContextHandle = (shell_context_handle_t *)shellHandle;

    shellContextHandle->prompt = prompt;
    SHELL_Printf(shellHandle, "\r\n");
    SHELL_Write(shellHandle, prompt, strlen((char *)prompt));
}
