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
#include <string.h>
#include "FunctionLib.h"
#include "MemManager.h"
#include "ModuleInfo.h"
#include "fsl_common.h"

#if (SHELL_ENABLED && SHELL_USE_HELP)
/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/
static int8_t DoHelp(uint8_t argc, char * argv[]);
static int8_t DoVer(uint8_t argc, char * argv[]);
#if SHELL_USE_AUTO_COMPLETE
static int8_t DoHelpComplete(uint8_t argc, char * argv[], char last_char, uint8_t maxv, char * cmdv[]);
#endif

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

cmd_tbl_t CommandFun_Help =
{
    .name = "help",
    .maxargs = 2,
    .repeatable = 1,
    .cmd = DoHelp,
    .usage = "Print command description/usage",
#if SHELL_USE_AUTO_COMPLETE
    .complete = DoHelpComplete,
#endif
    .help = "print brief description of all commands\r\n"
            "help <command> - "
            "print detailed usage of 'command'\r\n"
};

cmd_tbl_t CommandFun_Ver =
{
    .name = "version",
    .maxargs = 2,
    .repeatable = 1,
    .cmd = DoVer,
    .usage = "Print version of all the registered modules",
#if SHELL_USE_AUTO_COMPLETE
    .complete = NULL,
#endif
    .help = NULL
};

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief  This function will display a list of all registered SW module information.
*
* \param[in]  argc  The number of arguments
* \param[in]  argv  table with command argumens
*
* \return  int8_t  command status (command_ret_t)
*
********************************************************************************** */
static int8_t DoVer(uint8_t argc, char * argv[])
{
    moduleInfo_t *pInfo = gVERSION_TAGS_startAddr_d;

#if defined(CPU_K32W042S1M2VPJ_cm4) && (CPU_K32W042S1M2VPJ_cm4 == 1)
    uint8_t* pString = MEM_BufferAlloc(MAX_REGISTERED_MODULES_STRLEN);
    uint8_t iCount = 0, noOfModules = 0;
    bool_t hasModules = FALSE;

    shell_write("CM4:");
    SHELL_NEWLINE();
#endif

    assert( 0U != gVERSION_TAGS_entries_d );

    while( pInfo < gVERSION_TAGS_endAddr_d )
    {
        shell_write((const char*)*pInfo->moduleString);
        SHELL_NEWLINE();
        pInfo++;
    }
#if defined(CPU_K32W042S1M2VPJ_cm4) && (CPU_K32W042S1M2VPJ_cm4 == 1)
    if(NULL != pString)
    {
        hasModules = ModVer_GetInfoShellFormat_Multicore(pString);

        shell_write("CM0P:");
        SHELL_NEWLINE();
        if(FALSE == hasModules)
        {
            shell_write("There are no registered modules!");
            SHELL_NEWLINE();
            (void)MEM_BufferFree(pString);
            return CMD_RET_FAILURE;
        }
        else
        {
            shell_write((char*)pString);
            (void)MEM_BufferFree(pString);
        }
    }
#endif

    return CMD_RET_SUCCESS;
}
 
/*! *********************************************************************************
* \brief  This function will display a list of all commands with a short usage.
*         If called with argument, it will displat a detailed help for a command
*
* \param[in]  argc  The number of arguments
* \param[in]  argv  table with command argumens
*
* \return  int8_t  command status (command_ret_t)
*
********************************************************************************** */
static int8_t DoHelp(uint8_t argc, char * argv[])
{
    uint16_t i;

    if (argc == 1U)
    {
        char * pStr = MEM_BufferAlloc( mShellMaxCmdLen + 2UL );
        
        if (pStr != NULL)
        {
            for (i = 0; i < SHELL_CMD_TBL_SIZE; i++)
            {
                if (gpCmdTable[i] != NULL)
                {
                    uint16_t len = (uint16_t)strlen(gpCmdTable[i]->name);
                    
                    FLib_MemCpy(pStr, gpCmdTable[i]->name, len);
                    FLib_MemSet(&pStr[len], (uint8_t)' ', (uint32_t)mShellMaxCmdLen - len + 2UL);
                    shell_writeN(pStr, (uint16_t)mShellMaxCmdLen + 2u);
                    shell_write(gpCmdTable[i]->usage);
                    SHELL_NEWLINE();
                }
            }

            (void)MEM_BufferFree(pStr);
        }
        else
        {
            for (i = 0; i < SHELL_CMD_TBL_SIZE; i++)
            {
                if (gpCmdTable[i] != NULL)
                {
                    shell_write(gpCmdTable[i]->name);
                    shell_writeN(" ", 1);
                    shell_write(gpCmdTable[i]->usage);
                    SHELL_NEWLINE();
                }
            }
        }
    }
    else
    {
        for (i = 0; i < SHELL_CMD_TBL_SIZE; i++)
        {
            if ((gpCmdTable[i] != NULL) && !(bool)strcmp(argv[1], gpCmdTable[i]->name))
            {
                if (gpCmdTable[i]->help != NULL)
                {
                    shell_write(gpCmdTable[i]->name);
                    shell_writeN(" - ", 3);
                    shell_write(gpCmdTable[i]->help);
                    SHELL_NEWLINE();
                    break;
                }
                else
                {
                    shell_write ("- No additional help available.\r\n");
                    break;
                }
            }
        }
        
        if(SHELL_CMD_TBL_SIZE == i)
        {
            shell_write( "- No command available.\r\n" );
        }
    }
    return CMD_RET_SUCCESS;
}

#if SHELL_USE_AUTO_COMPLETE
/*! *********************************************************************************
* \brief  This is the autocomplete function for Help. 
*         It will return possible arguments
*
* \param[in]  argc       The number of arguments
* \param[in]  argv       Table with command argumens
* \param[in]  last_char  The Last character received
* \param[in]  maxv       The maximun number of matchs
* \param[in]  cmdv       Table with possible matchs
*
* \return  int8_t  command status (command_ret_t)
*
********************************************************************************** */
static int8_t DoHelpComplete(uint8_t argc, char * argv[], char last_char, uint8_t maxv, char * cmdv[])
{
    uint8_t str_len;
    uint8_t found = 0;
    uint8_t i;
    str_len = (uint8_t)strlen(argv[argc-1u]);

    switch(argc)
    {
        case 2:
            for (i = 0; i < SHELL_CMD_TBL_SIZE; i++)
            {
                if ((gpCmdTable[i] != NULL) && !(bool)strncmp(argv[argc-1u], gpCmdTable[i]->name, str_len))
                {
                    cmdv[found] = gpCmdTable[i]->name;
                    found++;
                }  
            }
            break;
        default:
        	/*MISRA rule 16.4*/
            break;
    }
    return (int8_t)found;
}
#endif /* SHELL_USE_AUTO_COMPLETE */
#endif /* (SHELL_ENABLED && SHELL_USE_HELP) */
/*******************************************************************************
 * EOF
 ******************************************************************************/
