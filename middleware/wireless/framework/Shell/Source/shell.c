/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2020 NXP
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
#include "SerialManager.h"
#include "MemManager.h"
#include "board.h"

#if SHELL_ENABLED
/************************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
************************************************************************************/
#define CTL_CH(c)               ((c) - 'a' + (char)1)
#define CTL_BACKSPACE           ('\b')
#define DEL                     ((char)255)
#define DEL7                    ((char)127)

/* Move cursor at the beginning of the line */
#define BEGINNING_OF_LINE()             \
while( (bool)mCmdIdx ) {                      \
    shell_putc(CTL_BACKSPACE);          \
    mCmdIdx--;                          \
}

/* Refresh the line starting from the current cursor pozition */
#define REFRESH_TO_EOL()                                \
if( mCmdIdx < mCmdLen )                                 \
{                                                       \
    shell_writeN(mCmdBuf + mCmdIdx, mCmdLen - mCmdIdx); \
    mCmdIdx = mCmdLen;                                  \
}

/* Clear current input */
#define SHELL_RESET() \
{                            \
    mCmdBuf[0] = '\0';       \
    mCmdLen = 0;             \
    mCmdIdx = 0;             \
}

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/
static void shell_main( void *params );
static void shell_ProcessDelete(void);
static void shell_ProcessBackspace(void);
static void shell_AddCharToCommandBuf(char processChar);
static int16_t shell_ProcessReadChar(char processChar);
static void shell_GetHistoryLine(char processChar);
#if SHELL_USE_AUTO_COMPLETE
static void shell_ProcessTab(void);
#endif
static int16_t shell_ReadChr( void );
static void shell_erase_to_eol( void );

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
static const char* pPrompt;
static char     mCmdBuf[SHELL_CB_SIZE + 1];
static uint16_t mCmdLen;
static uint16_t mCmdIdx;
static uint8_t  gShellSerMgrIf;
static uint8_t  mInsert = 1;
static uint8_t  esc_len = 0;

uint8_t  mShellMaxCmdLen = 0;

cmd_tbl_t *gpCmdTable[SHELL_CMD_TBL_SIZE];

#if SHELL_BREAK_KEY
int8_t (*mpfShellBreak)(uint8_t argc, char * argv[]) = NULL;
#endif
#if SHELL_PROCESS_COMMAND
void (*pfShellProcessCommand) (char * pCmd, uint16_t length) = NULL;
#endif

#if SHELL_USE_LOGO
static const char mLogo[] = "\n\n\r\n"
       " ####         ######      ##### ##########    \n\r"
       " ######      # ######    ##### #############  \n\r"
       " #######     ## ######  ##### ############### \n\r"
       " ########    ### ########### ####       ##### \n\r"
       " #### #####  #### ######### #####       ##### \n\r"
       " ####  ##### #### ######### ################# \n\r"
       " ####   ######## ########### ###############  \n\r"
       " ####     ##### ######  ##### ############    \n\r"
       " ####      ### ######    ##### ##             \n\r"
       " ####       # ######      ##### #             \n\r";
#endif

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/
#if SHELL_USE_AUTO_COMPLETE
#include "shell_autocomplete.h"
#endif
/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief  Initializes the SHELL module .
*
* \param[in]  prompt the string which will be used for command prompt
*
* \remarks
*
********************************************************************************** */
void shell_init(const char* prompt)
{
    serialStatus_t ret = gSerial_OsError_c;

    Serial_InitManager();

    /* Register Serial Manager interface */
    ret = Serial_InitInterface(&gShellSerMgrIf, SHELL_IO_TYPE, SHELL_IO_NUMBER);
    assert(gSerial_Success_c == ret);
    NOT_USED(ret);
    /* Set serial baud rate */
    (void)Serial_SetBaudRate(gShellSerMgrIf, SHELL_IO_SPEED);
#if (SHELL_USE_ALT_TASK == 0)
    /* Set RX callback */
    ret = Serial_SetRxCallBack(gShellSerMgrIf, shell_main, NULL);
    assert(gSerial_Success_c == ret);
#endif

    mCmdLen = 0;
    mCmdIdx = 0;
    FLib_MemSet(gpCmdTable, 0, sizeof(gpCmdTable));
    FLib_MemSet(mCmdBuf, 0, sizeof(mCmdBuf));
#if SHELL_USE_HELP
    (void)shell_register_function((cmd_tbl_t *)&CommandFun_Help);
    (void)shell_register_function((cmd_tbl_t *)&CommandFun_Ver);
#endif
#if defined(SHELL_MAX_HIST)&&(SHELL_MAX_HIST > 0u)
    hist_init();
#endif

    if(strlen(prompt) != 0U)
    {
        shell_write_prompt(prompt);
    }
}

/*! *********************************************************************************
* \brief  Return Serial Interface used by Shell
*
********************************************************************************** */
uint8_t shell_get_interface(void)
{
    /* return serial interface used by Shell */
    return gShellSerMgrIf;
}

/*! *********************************************************************************
* \brief  Changes the prompt name for the SHELL module .
*
* \param[in]  prompt the string which will be used for command prompt
*
* \remarks
*
********************************************************************************** */
void shell_change_prompt(const char* prompt)
{
    pPrompt = prompt;
    shell_refresh();
}

/*! *********************************************************************************
* \brief  Notify shell that the async command has finished
*
********************************************************************************** */
void shell_cmd_finished(void)
{
#if SHELL_BREAK_KEY
    mpfShellBreak = NULL;
#endif
    SHELL_RESET();
    SHELL_NEWLINE();
    shell_write(pPrompt);
}

/*! *********************************************************************************
* \brief  Empty command buffer and print command prompt
*
********************************************************************************** */
void shell_refresh(void)
{
    SHELL_RESET();
#if (SHELL_USE_ECHO)
    SHELL_NEWLINE();
#if !(defined(gHybridApp_d)&&(gHybridApp_d>0))
    shell_write(pPrompt);
#endif
#endif
}

/*! *********************************************************************************
* \brief  Print Shell Logo and prompt message
*
********************************************************************************** */
void shell_write_prompt(const char* prompt)
{
    /* store prompt */
    pPrompt = prompt;
    /* print logo and prompt */
#if SHELL_USE_LOGO
    shell_write(mLogo);
#endif
    shell_write("\r\nSHELL build: ");
    shell_write(__DATE__);
    shell_write("\n\rCopyright (c) 2020 NXP\r\n");
    shell_write(pPrompt);
}

/*! *********************************************************************************
* \brief  This function will write N bytes  over the serial interface
*
* \param[in]  pBuff pointer to a string
* \param[in]  n number of chars to be written
*
* \remarks
*
********************************************************************************** */
void shell_writeN(const char *pBuff, uint16_t n)
{
    if( (NULL != pBuff) && (0U != n) )
    {
#if (((defined(gShellUseSerialMgrUseIIC_c)) && (gShellUseSerialMgrUseIIC_c > 0U)) \
     || (defined(gShellUseSerialMgrUseSPI_c)) && (gShellUseSerialMgrUseSPI_c > 0U))
        if (SHELL_IO_TYPE == gSerialMgrIICSlave_c ||
            SHELL_IO_TYPE == gSerialMgrSPISlave_c)
        {
            uint8_t *pHdr = MEM_BufferAlloc(n+5UL);

            if (pHdr != NULL)
            {
                pHdr[0] = 0x02;
                pHdr[1] = 0x77;
                pHdr[2] = 0x77;
                pHdr[3] = (uint8_t)n;
                FLib_MemCpy(&pHdr[4], pBuff, n);
                pHdr[4U+n] = 0U;
                (void)Serial_SyncWrite( gShellSerMgrIf, pHdr, n+5U );
                (void)MEM_BufferFree(pHdr);
            }
        }
        else
        {
            (void)Serial_SyncWrite(gShellSerMgrIf, (const uint8_t*)pBuff, n);
        }
#else
        (void)Serial_SyncWrite(gShellSerMgrIf, (const uint8_t*)pBuff, n);
#endif
    }
}

/*! *********************************************************************************
* \brief  This function will write a NULL terminated string over the serial interface
*
* \param[in]  pBuff pointer to a string
*
* \remarks
*
********************************************************************************** */
void shell_write(const char *pBuff )
{
    shell_writeN(pBuff, (uint16_t)strlen(pBuff));
}

/*! *********************************************************************************
* \brief  This function will write one character over the serial interface
*
* \param[in]  c character to be written
*
* \remarks
*
********************************************************************************** */
void shell_putc(char c)
{
    (void)Serial_SyncWrite(gShellSerMgrIf, (uint8_t*)&c, 1);
}

/*! *********************************************************************************
* \brief  This function will write a decimal number over the serial interface
*
* \param[in]  nb number to be written
*
* \remarks
*
********************************************************************************** */
void shell_writeDec
(
    uint32_t nb
)
{
    (void)Serial_PrintDec(gShellSerMgrIf, nb);
}

/*! *********************************************************************************
* \brief  This function will write a signed decimal number over the serial interface
*
* \param[in]  nb number to be written
*
* \remarks
*
********************************************************************************** */
void shell_writeSignedDec
(
    int8_t nb
)
{
    if (((uint8_t)nb >> 7) != 0u)
    {
        /* Negative Value */
        shell_write("-");
        nb = (int8_t)(uint8_t)~((uint8_t)nb - (uint8_t)1);
    }
    (void)Serial_PrintDec(gShellSerMgrIf, (uint32_t)nb);
}

/*! *********************************************************************************
* \brief  This function will write a decimal number over the serial interface
*
* \param[in]  pHex  pointer to an octet string
* \param[in]  len   the length of the string
*
* \remarks
*
********************************************************************************** */
void shell_writeHex
(
    uint8_t *pHex,
    uint8_t len
)
{
    (void)Serial_PrintHex(gShellSerMgrIf, pHex, len, (uint8_t)gPrtHexBigEndian_c);
}

/*! *********************************************************************************
* \brief  This function will write a decimal number over the serial interface
*
* \param[in]  pHex  pointer to an octet string
* \param[in]  len   the length of the string
*
* \remarks
*
********************************************************************************** */
void shell_writeHexLe
(
    uint8_t *pHex,
    uint8_t len
)
{
    (void)Serial_PrintHex(gShellSerMgrIf, pHex, len, gPrtHexNoFormat_c);
}

/*! *********************************************************************************
* \brief  This function will write a boolean value in pretty format: "TRUE" or "FALSE"
*
* \param[in]  b  Boolean value
*
* \remarks
*
********************************************************************************** */
void shell_writeBool
(
    bool_t boolValue
)
{
    if (boolValue)
    {
        shell_write("TRUE");
    }
    else
    {
        shell_write("FALSE");
    }
}

/*! *********************************************************************************
* \brief  This function will output a formated string onto the serial interface
*
* \param[in]  format string defining the output
* \param[in]  ... variable number of parameters
*
* \remarks
*
********************************************************************************** */
#if SHELL_USE_PRINTF
#include <stdio.h>
#include <stdarg.h>

uint16_t shell_printf(char * format,...)
{
    va_list ap;
    uint16_t n = 0;
    char *pStr = (char*)MEM_BufferAlloc(SHELL_CB_SIZE);

    if (pStr)
    {
        va_start(ap, format);
        n = vsnprintf(pStr, SHELL_CB_SIZE, format, ap);
        //va_end(ap);  to follow MISRA...
        Serial_SyncWrite(gShellSerMgrIf, (uint8_t*)pStr, n);
        (void)MEM_BufferFree(pStr);
    }

    return n;
}
#endif

/*! *********************************************************************************
* \brief  This function registers a command into the SHELL
*
* \param[in]  pAddress pointer to the command structure
*
* \remarks
*
********************************************************************************** */
uint8_t shell_register_function(cmd_tbl_t * pAddress)
{
    uint16_t i;
    uint8_t status = 0U;
    /* check name conflict */
    for (i = 0; i < SHELL_CMD_TBL_SIZE; i++)
    {
        if( (gpCmdTable[i] != NULL) && !(bool)strcmp(pAddress->name, gpCmdTable[i]->name) )
        {
            status = 1U;
            break;
        }
    }
    
    if( 0U == status)
    {
        /* insert */
        for (i = 0; i < SHELL_CMD_TBL_SIZE; i++)
        {
            if (gpCmdTable[i] == NULL)
            {
                gpCmdTable[i] =  pAddress;
                // Update max command length
                if (strlen(pAddress->name) > mShellMaxCmdLen)
                {
                    mShellMaxCmdLen = (uint8_t)strlen(pAddress->name);
                }

                status = 0U;
                break;
            }
        }      
    }
    
    return status;
}

/*! *********************************************************************************
* \brief  This function registers multiple commands into the SHELL
*
* \param[in]  pAddress pointer to an array of commands
* \param[in]  num the number of entries
*
* \remarks
*
********************************************************************************** */
void shell_register_function_array(cmd_tbl_t * pAddress, uint8_t num)
{
    while ((bool)(num--))
    {
        (void)shell_register_function(pAddress++);
    }
}

/*! *********************************************************************************
* \brief  This function remove a command from the SHELL
*
* \param[in]  name a string reprezenting the command to be removed
*
* \remarks
*
********************************************************************************** */
uint8_t shell_unregister_function(char * name)
{
    uint16_t i;
    uint8_t returnValue = 1U;

    for( i=0; i<SHELL_CMD_TBL_SIZE; i++ )
    {
        if( (gpCmdTable[i] != NULL) && !(bool)strcmp(name, gpCmdTable[i]->name) )
        {
            gpCmdTable[i] = NULL;
            returnValue = 0U;
            break;
        }
    }

    return returnValue;
}

/*! *********************************************************************************
* \brief  This function is used to get the pointer to the value of the option
*         specified by pOption parameter.
*
* \param [in]   argc      number of available arguments
* \param [in]   argv      pointer to the list of arguments
* \param [in]   pOption   pointer to the string which describes the option
*
* \return       char*     pointer to the value of the option
*
********************************************************************************** */
char * shell_get_opt
(
    uint8_t argc,
    char *argv[],
    char *pOption
)
{
    uint8_t idx;
    char *pRes = NULL;

    /* Check all arguments */
    for(idx=0;idx<argc;idx++)
    {
        if(pRes == NULL)
        {
            /* The option was found */
            uint32_t pOption_len = strlen(pOption);
            uint32_t argv_len = strlen(argv[idx]);

            if(FLib_MemCmp(argv[idx], pOption, MIN(pOption_len, argv_len)) && (argv[idx + 1u] != NULL))
            {
                /* The value should be in this argument after the pOption text */
                if((bool)*(argv[idx] + strlen(pOption)))
                {
                    pRes = argv[idx] + strlen(pOption);
                }
                /* The value should be in the next argument */
                else
                {
                    pRes = argv[idx + 1u];
                }
            }
        }
    }
    return pRes;
}

/*! *********************************************************************************
* \brief  This function return a pointer to the command data structure
*
* \param [in]   cmd      string reprezenting the command
*
* \return       cmd_tbl_t*     pointer to the command's data
*
********************************************************************************** */
cmd_tbl_t * shell_find_command (char * cmd)
{
    uint16_t i;
    cmd_tbl_t * findResult = NULL;

    assert(NULL != cmd);

    for (i = 0; i < SHELL_CMD_TBL_SIZE; i++)
    {
        if( (gpCmdTable[i] != NULL) && !(bool)strcmp((char*)cmd, gpCmdTable[i]->name) )
        {
            findResult = gpCmdTable[i];
            break;
        }
    }
    
    return findResult;
}

/*! *********************************************************************************
* \brief  This function splits a command string into arguments
*
* \param [in]   s       command strin
* \param [in]   argvsz  maximun number of arguments
* \param [in]   argv    pointer to the argument vector
*
* \return       uint8_t     the number of arguments found
*
********************************************************************************** */
uint8_t make_argv(char *s, uint8_t argvsz, char * argv[])
{
    uint8_t argc = 0U;
    /* split into argv */
    while (argc < argvsz - 1U)
    {
        /* skip any white space */
        while( *s == ' ' )
        {
            ++s;
        }
        if (*s == '\0') /* end of s, no more args   */
        {
            argv[argc] = NULL;
            break;
        }
        argv[argc++] = s;   /* begin of argument string */
        /* find end of string */
        while( (bool)(*s) && ( *s != ' ' ) )
        {
            ++s;
        }
        if (*s == '\0')     /* end of s, no more args   */
        {
            argv[argc] = NULL;
            break;
        }
        *s++ = '\0';        /* terminate current arg     */
    }
    return argc;
}

/*! *********************************************************************************
* \brief  This function process the characters received over the serial interface
*
*
********************************************************************************** */
#if SHELL_USE_ALT_TASK
void shell_task(void)
{
    shell_main(NULL);
}
#endif

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief  This function is called every time a character is received.
*         The main SHELL processing is done from here
*
* \param [in]   params       unused
*
********************************************************************************** */
static void shell_main( void *params )
{
    int16_t ret;
    uint8_t argc;
    char * argv[SHELL_MAX_ARGS+1] = {0};    /* NULL terminated  */
    cmd_tbl_t * cmdtp;

    // Process the received char
    ret = shell_ReadChr();

    if (ret == 0)
    {
        if (mCmdLen == 0u)
        {
            SHELL_RESET();
#if SHELL_BREAK_KEY
            if (NULL == mpfShellBreak)
            {
                shell_write(pPrompt);
            }
#endif
        }
#if SHELL_PROCESS_COMMAND
        else if (pfShellProcessCommand)
        {
            pfShellProcessCommand(mCmdBuf, mCmdLen);
        }
#endif
        else
        {
            // Split command into arguments
            argc = make_argv(mCmdBuf, SHELL_MAX_ARGS + 1U, argv);
            if (argc >= SHELL_MAX_ARGS)
            {
                shell_write("** Too many args (max. ");
                shell_writeDec(SHELL_MAX_ARGS);
                shell_write (") **\r\n");
            }
            // Search for the appropriate command
            cmdtp = shell_find_command(argv[0]);
            if ((cmdtp != NULL) && (cmdtp->cmd != NULL))
            {
                if (argc > cmdtp->maxargs)
                {
                    ret = CMD_RET_USAGE;
                }
                else
                {
                    ret = (cmdtp->cmd)(argc, argv);
                }
            }
            else
            {
                shell_write("Unknown command '");
                shell_write(argv[0]);
#if SHELL_USE_HELP
                shell_write("' - try 'help'\r\n");
#else
                shell_write("' ");
#endif
            }
#if SHELL_USE_HELP
            if (ret == CMD_RET_USAGE)
            {
                if (cmdtp->usage != NULL)
                {
                    shell_write("Usage:\r\n");
                    shell_write(cmdtp->name);
                    shell_writeN(" - ", 3);
                    shell_write(cmdtp->usage);
                    SHELL_NEWLINE();
                }
                if (cmdtp->help != NULL)
                {
                    shell_write(cmdtp->name);
                    shell_writeN(" ", 1);
                    shell_write(cmdtp->help);
                    SHELL_NEWLINE();
                }
                else
                {
                    shell_write ("- No additional help available.\r\n");
                }
            }
#endif
            if (ret == CMD_RET_ASYNC)
            {
#if SHELL_BREAK_KEY
                mpfShellBreak = cmdtp->cmd;
#endif
                SHELL_RESET();
            }
            else
            {
#if SHELL_BREAK_KEY
                mpfShellBreak = NULL;
#endif
                shell_refresh();
            }
        }
    }
    else if (ret == -1)
    {
        shell_write("<INTERRUPT>\r\n");
#if SHELL_BREAK_KEY
        if (mpfShellBreak)
        {
            mpfShellBreak(0,0);
            mpfShellBreak = NULL;
            shell_refresh();
        }
#endif
        SHELL_RESET();
#if SHELL_PROCESS_COMMAND
        if (pfShellProcessCommand)
        {
            pfShellProcessCommand(NULL, 0);
        }
#endif
    }
    else
    {
        /*MISRA rule 15.7*/
    }
}

/*! *********************************************************************************
* \brief  This function is called to process standard linux xterm esc sequences for 
*  arrow key, etc.
*
* \param [in]   ichar       The char to be processed
*
* \return  Processed char
*
********************************************************************************** */
static char shell_ProcessLinuxXterm(char ichar)
{
    char ret = ichar;
    
    if (esc_len == 1u)
    {
        if (ichar == '[')
        {
//          esc_save[esc_len] = ichar;
            esc_len++;
        }
        else
        {
//          cread_add_str(esc_save, esc_len, mInsert, &mCmdIdx, &mCmdLen, mCmdBuf, mCmdLen);
            esc_len = 0;
        }
    }
    else
    {
        switch (ichar)
        {
            case 'D':   /* <- key */
                ret = CTL_CH('b');
                break;
            case 'C':   /* -> key */
                ret = CTL_CH('f');
                break;  /* pass off to ^F handler */
            case 'H':   /* Home key */
                ret = CTL_CH('a');
                break;  /* pass off to ^A handler */
            case 'A':   /* up arrow */
                ret = CTL_CH('p');
                break;  /* pass off to ^P handler */
            case 'B':   /* down arrow */
                ret = CTL_CH('n');
                break;  /* pass off to ^N handler */
            default:
//              esc_save[esc_len] = ichar;
//              esc_len++;
//              cread_add_str(esc_save, esc_len, mInsert, &mCmdIdx, &mCmdLen, mCmdBuf, mCmdLen);
                break;
        }
        esc_len = 0;
    }
    return ret;
}

/*! *********************************************************************************
* \brief  This function is called to process the received deleate key
*
* \return
*
********************************************************************************** */
static void shell_ProcessDelete(void)
{
    uint16_t wlen;

    if (mCmdIdx < mCmdLen)    /*make sure there is char after cursor*/
    {
        wlen = mCmdLen - mCmdIdx - 1u;
        if ((bool)wlen)
        {
            FLib_MemInPlaceCpy(&mCmdBuf[mCmdIdx],&mCmdBuf[mCmdIdx+1u],wlen);
            shell_writeN(mCmdBuf + mCmdIdx, wlen);
        }
        shell_putc(' ');
        do
        {
            shell_putc(CTL_BACKSPACE);
        } while ((bool)(wlen--));
        mCmdLen--;
    }
}

/*! *********************************************************************************
* \brief  This function is called to process the received backspace key
*
* \return
*
********************************************************************************** */
static void shell_ProcessBackspace(void)
{
    uint16_t wlen;

    if ((bool)mCmdIdx)       /*make sure there is char before cursor*/
    {
        wlen = mCmdLen - mCmdIdx;
        mCmdIdx--;
        FLib_MemInPlaceCpy(&mCmdBuf[mCmdIdx], &mCmdBuf[mCmdIdx+1u], wlen);
        shell_putc(CTL_BACKSPACE);
        shell_writeN(mCmdBuf + mCmdIdx, wlen);
        shell_putc(' ');
        do
        {
            shell_putc(CTL_BACKSPACE);
        } while ((bool)(wlen--));
        mCmdLen--;
    }
}

/*! *********************************************************************************
* \brief  This function is called to add a normal char to command buffer
*
* \param [in]   processChar       The char added to command buffer
*
* \return
*
********************************************************************************** */
static void shell_AddCharToCommandBuf(char processChar)
{
    char ichar = processChar;

    /* Add a character to the command buffer */
    if( (mCmdIdx < mCmdLen) && (bool)mInsert )
    {
        uint16_t len = mCmdLen - mCmdIdx;
        FLib_MemInPlaceCpy( &mCmdBuf[mCmdIdx+1u],
                           &mCmdBuf[mCmdIdx], len );
        mCmdBuf[mCmdIdx] = ichar;
        shell_writeN(mCmdBuf + mCmdIdx, len+1u);
        mCmdLen++;
        mCmdIdx++;
        while( (bool)len )
        {
            shell_putc(CTL_BACKSPACE);
            len--;
        }
    }else
    {
        if( mCmdLen == mCmdIdx )
        {
            mCmdLen++;
        }
        else
        {
            ;
        }

        mCmdBuf[mCmdIdx++] = ichar;
#if SHELL_USE_ECHO
        shell_putc(ichar);
#endif
    }

    /* Check if the received command exceeds he size of the buffer */
    if( mCmdLen >= SHELL_CB_SIZE )
    {
        SHELL_RESET();
    }
}

/*! *********************************************************************************
* \brief  This function is called to process the received character
*
* \return       uint16_t     0 - comand received complete
*                           -1 - CTRL + C was pressed
*                           -2 - new character received
*
********************************************************************************** */
static int16_t shell_ProcessReadChar(char processChar)
{
    char ichar = processChar;
    int16_t status = 0;
    switch (ichar)
    {
        case (char)0x1b:
            esc_len++;
            break;
        case CTL_CH('a'):
            BEGINNING_OF_LINE();
            break;
        case CTL_CH('c'):   /* ^C - break */
            SHELL_RESET();
            status = -1;
            break;
        case CTL_CH('f'):
            if( mCmdIdx < mCmdLen )
            {
                shell_putc(mCmdBuf[mCmdIdx]);
                mCmdIdx++;
            }
            break;
        case CTL_CH('b'):    /* ^b - backspace */
            if( (bool)mCmdIdx )
            {
                shell_putc(CTL_BACKSPACE);
                mCmdIdx--;
            }
            break;
        case CTL_CH('d'):   /* ^d - delete */
            shell_ProcessDelete();
            break;
        case CTL_CH('k'):
            shell_erase_to_eol();
            break;
        case CTL_CH('e'):
            REFRESH_TO_EOL();
            break;
        case CTL_CH('o'):
            mInsert = (uint8_t)!(bool)mInsert;
            break;
        case CTL_CH('x'):
        case CTL_CH('u'):
            BEGINNING_OF_LINE();
            shell_erase_to_eol();
            break;
        case DEL:
        case DEL7:
        case (char)8:
            shell_ProcessBackspace();
            break;
        default:
#if (0 == SHELL_IS_CASE_SENSITIVE)
            if ((ichar >= 'A' && ichar <= 'Z'))
            {
                /* Convert received char to small caps */
                ichar += ('a' - 'A');
            }
#endif /*(0 == SHELL_IS_CASE_SENSITIVE)*/
            shell_AddCharToCommandBuf(ichar);
            break;
    }
    return status;
}

/*! *********************************************************************************
* \brief  This function is called to get history command
*
* \param [in]   processChar       To get previous or next command
*
********************************************************************************** */
static void shell_GetHistoryLine(char processChar)
{
#if defined(SHELL_MAX_HIST)&&(SHELL_MAX_HIST > 0)
    char ichar = processChar;
    char *hline = NULL;

    esc_len = 0;
    if (CTL_CH('p') == ichar)
    {
        hline = hist_prev();
    }

    if(CTL_CH('n') == ichar)
    {
        hline = hist_next();
    }

    if (NULL != hline)
    {
        /* nuke the current line */
        /* first, go home */
        BEGINNING_OF_LINE();
        shell_erase_to_eol();
        /* copy new line into place and display */
        (void)strncpy(mCmdBuf, hline, SHELL_CB_SIZE);

        mCmdLen = (uint16_t)strnlen(mCmdBuf, SHELL_CB_SIZE);
        shell_writeN(mCmdBuf + mCmdIdx, mCmdLen - mCmdIdx);
        mCmdIdx = mCmdLen;
    }else
    {
        SHELL_BEEP();
    }
#endif /* SHELL_CONFIG_USE_HIST */
}

/*! *********************************************************************************
* \brief  This function is called to process the received tab key
*
* \return
*
********************************************************************************** */
#if SHELL_USE_AUTO_COMPLETE
static void shell_ProcessTab(void)
{
    uint16_t col,wlen;

    /* do not autocomplete when in the middle */
    if (mCmdIdx < mCmdLen)
    {
        SHELL_BEEP();
    }
    else
    {
        mCmdBuf[mCmdIdx] = '\0';
        col = (uint16_t)strlen(pPrompt) + mCmdLen;
        wlen = mCmdIdx;
        if( (bool)cmd_auto_complete(pPrompt, mCmdBuf, (uint8_t*)&wlen, (uint8_t*)&col) )
        {
            col = wlen - mCmdIdx;
            mCmdIdx += col;
            mCmdLen += col;
        }      
    }
}
#endif

/*! *********************************************************************************
* \brief  This function is called to receive character
*
* \return       uint16_t     0 - comand received complete
*                           -1 - CTRL + C was pressed
*                           -2 - new character received
*
********************************************************************************** */
static int16_t shell_ReadChr( void )
{
    uint16_t wlen;
    char ichar,linuxChar;
    int16_t status = 0;
//    static char esc_save[4];

    while( (bool)1 )
    {
         /*read a char*/
        (void)Serial_Read( gShellSerMgrIf, (uint8_t*)&ichar, 1, &wlen );

        /*if Serial_Read() failed, return*/
        if( 0u == wlen )
        {
            status = -2;
            break;
        }

        if( ('\n' == ichar) || ('\r' == ichar) )
        {
            (void)Serial_Read( gShellSerMgrIf, (uint8_t*)&ichar, 1, &wlen );
            SHELL_NEWLINE();
            break;
        }

        /* handle standard linux xterm esc sequences for arrow key, etc.*/
        if (esc_len != 0u)
        {
            linuxChar = shell_ProcessLinuxXterm(ichar);
            if ( ichar == linuxChar )
            {
                continue;
            }
            else
            {
                ichar = linuxChar;
            }
        }

        if ((CTL_CH('p') == ichar)||(CTL_CH('n') == ichar))
        {
            shell_GetHistoryLine(ichar);
            continue;
        }else if('\t' == ichar)
        {
#if SHELL_USE_AUTO_COMPLETE
             shell_ProcessTab();
#endif
             continue;
        }else
        {
            status = shell_ProcessReadChar(ichar);
        }
        if (0 != status)
        {
            break;
        }
    }
    if (0 == status)
    {
        mCmdBuf[mCmdLen] = '\0';    /* lose the newline */
#if defined(SHELL_MAX_HIST)&&(SHELL_MAX_HIST > 0)
        hist_add(mCmdBuf);
#endif
    }
    return status;
}

/*! *********************************************************************************
* \brief  Erase line from the current cursor position
*
********************************************************************************** */
static void shell_erase_to_eol(void)
{
    uint16_t i;

    if (mCmdIdx < mCmdLen)
    {
        for (i = mCmdIdx; i < mCmdLen; i++)
        {
            mCmdBuf[i] = ' ';
        }

        shell_writeN(mCmdBuf + mCmdIdx, mCmdLen - mCmdIdx);

        for (i = mCmdIdx; i < mCmdLen; i++)
        {
            mCmdBuf[i] = CTL_BACKSPACE;
        }

        shell_writeN(mCmdBuf + mCmdIdx, mCmdLen - mCmdIdx);

        mCmdLen = mCmdIdx;
    }
}
#endif /* SHELL_ENABLED */
