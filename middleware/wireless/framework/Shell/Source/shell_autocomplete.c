/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017, 2019 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "shell.h"
#include "shell_autocomplete.h"
#include <string.h>
#include "FunctionLib.h"
#include "fsl_common.h"

#if (SHELL_ENABLED && SHELL_USE_AUTO_COMPLETE)
/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief  find the available matchs and return possiable match number.
*
* \param[in]  argc       The number of arguments
* \param[in]  argv       Table with command argumens
* \param[in]  last_char  The Last character received
* \param[in]  maxv       The maximun number of matchs
* \param[in]  cmdv       Table with possible matchs
*
* \return  int8_t  The number of possible matchs
*
********************************************************************************** */
static int8_t complete_cmdv(uint8_t argc, char * argv[], char last_char, uint8_t maxv, char *cmdv[])
{
    cmd_tbl_t *cmdtp;
    uint8_t i;
    uint8_t n_found = 0;
    static char cmdvBuf[4];

    cmdv[0] = NULL;

    /* sanity */
    assert(maxv > 1U);

    /* more than one arg or one but the start of the next */
    if ((argc > 1U) ||
        (((last_char == '\0') || (last_char == ' ')) && (argc == 1U)))
    {
        cmdtp = shell_find_command(argv[0]);
        if((NULL != cmdtp)&&(NULL != cmdtp->complete))
        {
            n_found = (uint8_t)((*cmdtp->complete)(argc, argv, last_char, maxv, cmdv));
        }
    }
    else
    {
        /*
        * one or no arguments
        * If no arguments, output full list of commands.
        * Some commands allow length modifiers (like "cp.b");
        * compare command name only until first dot.
        */
        for (i = 0; i < (uint8_t)SHELL_CMD_TBL_SIZE; i++)
        {
            if ((gpCmdTable[i] != NULL) && (gpCmdTable[i]->name != NULL))
            {
                if ((bool)argc && !FLib_MemCmp(argv[0], gpCmdTable[i]->name, strlen(argv[0])))
                {
                    continue;
                }

                /* too many! */
                if (n_found >= maxv - 2U)
                {
                    (void)strncpy(cmdvBuf, "...", 4);
                    cmdv[n_found++] = cmdvBuf;
                    break;
                }
                cmdv[n_found++] = gpCmdTable[i]->name;
            }
        }
        cmdv[n_found] = NULL;
    }
    return (int8_t)n_found;
}

/*! *********************************************************************************
* \brief  print possible matchs
*
* \param[in]  banner     A string to be displayed before the list
* \param[in]  leader     The line leader string
* \param[in]  sep        The matchs separator string
* \param[in]  linemax    The maximum length of a line
* \param[in]  argv       Table with possible matchs
* \param[in]  num        The number of matchs
*
********************************************************************************** */
static void print_argv(const char * banner, const char * leader, const char * sep, uint8_t linemax, char * argv[], uint8_t num)
{
    uint8_t ll = (uint8_t)strlen(leader);
    uint8_t sl = (uint8_t)strlen(sep);
    uint8_t len, i;

    SHELL_NEWLINE();
    shell_write((const char *)banner);

    i = linemax;    /* force leader and newline */
    while (NULL != *argv)
    {
        len = (uint8_t)strlen(*argv) + sl;
        if (i + len >= linemax)
        {
            SHELL_NEWLINE();

            shell_writeN(leader, ll);
            i = ll - sl;
        }
        else
        {
            shell_writeN(sep, sl);
        }
        shell_write(*argv++);
        i += len;
    }
    SHELL_NEWLINE();
}

/*! *********************************************************************************
* \brief  find the number of common characters of matchs.
*
* \param[in]  argv       Table with possible matchs
* \param[in]  num        The number of matchs
*
* \return  uint32_t  The number of common characters
*
********************************************************************************** */
static uint32_t find_common_prefix(char * argv[], uint8_t num)
{
    uint8_t i;
    uint32_t len;
    char * anchor, *s, *t;

    assert(NULL != *argv);

    /* begin with max */
    anchor = argv[0];
    len = (uint8_t)strlen(anchor);
    while ((bool)(num--))
    {
        t = *argv++;
        s = anchor;
        for (i = 0; i < len; i++)
        {
            t++;s++;
            if (*t != *s)
            {
                break;
            }
        }
        len = (uint32_t)s - (uint32_t)anchor;
    }
    return len;
}

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
/*! *********************************************************************************
* \brief  auto complete interface function.
*
* \param[in]  prompt   The prompt string
* \param[in]  buf      The current command string
* \param[in]  np       Table with possible matchs
* \param[in]  colp     The number of matchs
*
* \return  int8_t      status: 0 - success; 1 - error
*
********************************************************************************** */
 /*!
 * @brief auto complete interface function.
 */
int8_t cmd_auto_complete(const char * prompt, char * buf, uint8_t * np, uint8_t * colp)
{
    char *tmp_buf;
    uint8_t n = *np, col = *colp;
    char * argv[SHELL_MAX_ARGS + 1]= {0};        /* NULL terminated  */
    char * cmdv[10] = {0};
    char * s, *t;
    char sep = '\0';
    int i, j, k, len, seplen, argc = 1;
    uint8_t cnt;
    char last_char;
    int8_t status = 0;

    cnt = (uint8_t)strlen(buf);
    if (cnt >= 1u)
    {
        last_char = buf[cnt - 1u];
    }
    else
    {
        last_char = '\0';
    }
    /* copy to secondary buffer which will be affected */
    tmp_buf = MEM_BufferAlloc(SHELL_CB_SIZE+1u);
    if(tmp_buf == NULL)
    {
    	status = 1;
    }
    else
    {
        (void)strncpy(tmp_buf, buf, SHELL_CB_SIZE);
        tmp_buf[SHELL_CB_SIZE] = '\0';
        /* separate into argv */
        argc = (int32_t)make_argv(tmp_buf, NumberOfElements(argv), argv);
        /* do the completion and return the possible completions */
        i = complete_cmdv((uint8_t)argc, argv, last_char, NumberOfElements(cmdv), cmdv);
        /* no match; bell and out */
        if (i == 0)
        {
            if (argc > 1)   /* allow tab for non command */
            {
                status = 0;
            }
            else
            {
                SHELL_BEEP();
                status = 1;
            }
        }
        else
        {
            s = NULL;
            len = 0;
            seplen = 0;

            if (i == 1)
            {
                /* one match; perfect */
                if(argc > 0)
                {
                    k = (int32_t)strlen(argv[argc - 1]);
                    s = cmdv[0] + k;
                    len = (int32_t)strlen(s);
                    sep = ' ';
                    seplen = 1;
                }
            }
            else
            {
                if (argc > 0)
                {
                    /* more */
                    j = (int32_t)find_common_prefix(cmdv, (uint8_t)i);
                    k = (int32_t)strlen(argv[argc - 1]);
                    j -= k;
                    if (j > 0)
                    {
                        s = cmdv[0] + k;
                        len = j;
                    }
                }
            }

            if (s != NULL)
            {
                k = len + seplen;
                /* make sure it fits */
                if (n + (uint32_t)k >= SHELL_CB_SIZE - 2UL)
                {
                    SHELL_BEEP();
                }
                else
                {
                    t = buf + cnt;
                    for (i = 0; i < len; i++)
                    {
                      *t++ = *s++;
                    }

                    if ( '\0' != sep )
                    {
                        *t++ = sep;
                    }
                    *t = '\0';
                    n += (uint8_t)k;
                    col += (uint8_t)k;
                    shell_write(t - k);
                    if ('\0' == sep)
                    {
                        SHELL_BEEP();
                    }
                    *np = n;
                    *colp = col;
                }
            }
            else
            {
                print_argv("The possible options:" , "  ", " ", 78, cmdv, (uint8_t)i);
                shell_write(prompt);
                shell_write(buf);
            }
            status = 1;
        }
        (void)MEM_BufferFree(tmp_buf);
    }
    return status;
}

#endif
/*******************************************************************************
 * EOF
 ******************************************************************************/
