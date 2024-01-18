/*
 * Copyright (c) 2007-2015 Freescale Semiconductor, Inc.
 * Copyright 2018-2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * FreeMASTER Communication Driver - Example Application Code
 */

#include <string.h>
#include <stdio.h>

#include "freemaster.h"
#include "freemaster_example.h"

/****************************************************************************
 *
 * Default configuration of FreeMASTER demo
 *
 */

/* Demo application configuration. Enough ROM resources by default,
 * but not really large ROM by default (storing images and other active content) */
#ifndef FMSTR_DEMO_ENOUGH_ROM
#define FMSTR_DEMO_ENOUGH_ROM 1
#endif
#ifndef FMSTR_DEMO_LARGE_ROM
#define FMSTR_DEMO_LARGE_ROM 0
#endif

/* in LARGE_ROM is defined, then also enable ENOUGH_ROM option */
#if FMSTR_DEMO_LARGE_ROM && !(FMSTR_DEMO_ENOUGH_ROM)
#warning Probably wrong demo application settings
#undef FMSTR_DEMO_ENOUGH_ROM
#define FMSTR_DEMO_ENOUGH_ROM 1
#endif

/* Platform configuration for demo application */
#ifndef FMSTR_DEMO_SUPPORT_I64
#define FMSTR_DEMO_SUPPORT_I64 1 /* support for long long type */
#endif
#ifndef FMSTR_DEMO_SUPPORT_FLT
#define FMSTR_DEMO_SUPPORT_FLT 1 /* support for float type */
#endif
#ifndef FMSTR_DEMO_SUPPORT_DBL
#define FMSTR_DEMO_SUPPORT_DBL 1 /* support for double type */
#endif

/* User may re-declare this to put the TSA Active Content to a specific section. */
#ifndef FMSTR_DEMO_CONTENT 
#define FMSTR_DEMO_CONTENT const  /* By default, it is just const. */
#endif

/****************************************************************************
 *
 * Test variables, will be displayed in the FreeMASTER application
 *
 */

#define ARR_SIZE     10
#define ARR_SIZE_FLT 10
#define ARR_SIZE_DBL 10

volatile unsigned char var8;
volatile unsigned char var8rw;
volatile unsigned char var8inc = 1;
volatile unsigned short var16;
volatile unsigned short var16rw;
volatile unsigned short var16inc = 1;
volatile unsigned short var16prv;
volatile unsigned long var32;
volatile unsigned long var32rw;
volatile unsigned long var32inc = 100;

volatile unsigned char frac8;
volatile unsigned short frac16;
volatile unsigned long frac32;

volatile unsigned char ufrac8;
volatile unsigned short ufrac16;
volatile unsigned long ufrac32;

volatile unsigned char *var8ptr   = &var8;
volatile unsigned short *var16ptr = &var16;
volatile unsigned long *var32ptr  = &var32;

volatile unsigned short arr_size = ARR_SIZE;
volatile unsigned char arr8[ARR_SIZE];
volatile unsigned short arr16[ARR_SIZE];
volatile unsigned long arr32[ARR_SIZE];

#if FMSTR_DEMO_SUPPORT_I64
volatile unsigned long long var64;
volatile unsigned long long var64rw;
volatile unsigned long long var64inc = 10000;
volatile unsigned long long arr64[ARR_SIZE];
volatile unsigned long long frac64;
volatile unsigned long long ufrac64;
#endif

#if FMSTR_DEMO_SUPPORT_FLT
volatile float varFLT;
volatile float varFLTrw;
volatile float varFLTinc = 1.0;
volatile float arrFLT[ARR_SIZE_FLT];
volatile unsigned int arrFLT_size = ARR_SIZE_FLT;
#endif

#if FMSTR_DEMO_SUPPORT_DBL
volatile double varDBL;
volatile double varDBLrw;
volatile double varDBLinc = 1.0;
volatile double arrDBL[ARR_SIZE_DBL];
volatile unsigned int arrDBL_size = ARR_SIZE_DBL;
#endif

/* count received application commands, just for test purposes */
volatile unsigned char nAppCmdCounter;

/****************************************************************************
 *
 * Test structure types - demonstrates the "TSA" feature thanks to which the
 * FreeMASTER is able to load a variable and type information directly from
 * the embedded application.
 *
 */

typedef enum
{
    EXAMPLE_ENUM_FIRST_ENTRY,
    EXAMPLE_ENUM_SECOND_ENTRY,
    EXAMPLE_ENUM_THIRD_ENTRY,
} EXAMPLE_ENUM;

typedef enum
{
    SPEED_ZERO   = 0,
    SPEED_FAST   = 1,
    SPEED_NORMAL = 20,
    SPEED_SLOW   = 200,
} INCREMENT_SPEED;

typedef struct
{
    unsigned short aa;
    unsigned long bb[2];
    unsigned short cc;
    unsigned long dd[3];
    unsigned char ee;
    unsigned char ff[5];
} INNER_STRUCT;

typedef struct
{
    unsigned short a;
    unsigned long b;
    INNER_STRUCT inA[4];
    INNER_STRUCT inB;
} OUTER_STRUCT;

/****************************************************************************
 *
 * Test variables which make use of the user-defined data types. You are able
 * to display the variables including the member items in FreeMASTER.
 *
 */

volatile OUTER_STRUCT so1, so2;
volatile INNER_STRUCT si1, si2;
volatile EXAMPLE_ENUM varENUM;
volatile INCREMENT_SPEED varIncSpeed;

/****************************************************************************
 * Local prototypes
 */
FMSTR_APPCMD_RESULT my_appcmd_handler(FMSTR_APPCMD_CODE nAppcmd, FMSTR_APPCMD_PDATA pData, FMSTR_SIZE nDataLen);
void my_pipe_handler(FMSTR_HPIPE hpipe);
void my_pipe_math(FMSTR_HPIPE hpipe);

// clang-format off

/****************************************************************************
 *
 * With TSA enabled, the user describes the global and static variables using
 * so-called TSA tables. There can be any number of tables defined in
 * the project files. Each table does have the identifier which should be
 * unique across the project.
 *
 * Note that you can declare variables as Read-Only or Read-Write.
 * The FreeMASTER driver denies any write access to the Read-Only variables
 * when TSA_SAFETY is enabled.
 */

FMSTR_TSA_TABLE_BEGIN(first_table)
    FMSTR_TSA_RO_VAR(var8, FMSTR_TSA_UINT8)
    FMSTR_TSA_RW_VAR(arr8, FMSTR_TSA_UINT8)
    FMSTR_TSA_RW_VAR(var8rw, FMSTR_TSA_UINT8)
    FMSTR_TSA_RW_VAR(var8inc, FMSTR_TSA_UINT8)

    FMSTR_TSA_RO_VAR(var16, FMSTR_TSA_UINT16)
    FMSTR_TSA_RW_VAR(arr16, FMSTR_TSA_UINT16)
    FMSTR_TSA_RW_VAR(var16rw, FMSTR_TSA_UINT16)
    FMSTR_TSA_RW_VAR(var16inc, FMSTR_TSA_UINT16)

    FMSTR_TSA_RO_VAR(var32, FMSTR_TSA_UINT32)
    FMSTR_TSA_RW_VAR(arr32, FMSTR_TSA_UINT32)
    FMSTR_TSA_RW_VAR(var32rw, FMSTR_TSA_UINT32)
    FMSTR_TSA_RW_VAR(var32inc, FMSTR_TSA_UINT32)

    FMSTR_TSA_RO_VAR(var8ptr, FMSTR_TSA_POINTER)
    FMSTR_TSA_RO_VAR(var16ptr, FMSTR_TSA_POINTER)
    FMSTR_TSA_RO_VAR(var32ptr, FMSTR_TSA_POINTER)

    FMSTR_TSA_RW_VAR(frac8, FMSTR_TSA_FRAC_Q(0, 7))
    FMSTR_TSA_RW_VAR(frac16, FMSTR_TSA_FRAC_Q(3, 12))
    FMSTR_TSA_RW_VAR(frac32, FMSTR_TSA_FRAC_Q(12, 19))

    FMSTR_TSA_RW_VAR(ufrac8, FMSTR_TSA_UFRAC_UQ(1, 7))
    FMSTR_TSA_RW_VAR(ufrac16, FMSTR_TSA_UFRAC_UQ(4, 12))
    FMSTR_TSA_RW_VAR(ufrac32, FMSTR_TSA_UFRAC_UQ(13, 19))

#if FMSTR_DEMO_SUPPORT_I64
    FMSTR_TSA_RO_VAR(var64, FMSTR_TSA_UINT64)
    FMSTR_TSA_RW_VAR(arr64, FMSTR_TSA_UINT64)
    FMSTR_TSA_RW_VAR(var64rw, FMSTR_TSA_UINT64)
    FMSTR_TSA_RW_VAR(var64inc, FMSTR_TSA_UINT64)
    FMSTR_TSA_RW_VAR(frac64, FMSTR_TSA_FRAC_Q(8, 55))
    FMSTR_TSA_RW_VAR(ufrac64, FMSTR_TSA_UFRAC_UQ(9, 55))
#endif

#if FMSTR_DEMO_SUPPORT_FLT
    FMSTR_TSA_RO_VAR(varFLT, FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(varFLTrw, FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(arrFLT, FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(varFLTinc, FMSTR_TSA_FLOAT)
    FMSTR_TSA_RO_VAR(arrFLT_size, FMSTR_TSA_UINT32)
#endif

#if FMSTR_DEMO_SUPPORT_DBL
    FMSTR_TSA_RO_VAR(varDBL, FMSTR_TSA_DOUBLE)
    FMSTR_TSA_RW_VAR(arrDBL, FMSTR_TSA_DOUBLE)
    FMSTR_TSA_RW_VAR(varDBLrw, FMSTR_TSA_DOUBLE)
    FMSTR_TSA_RW_VAR(varDBLinc, FMSTR_TSA_DOUBLE)
    FMSTR_TSA_RO_VAR(arrDBL_size, FMSTR_TSA_UINT32)
#endif

    FMSTR_TSA_RO_VAR(arr_size, FMSTR_TSA_UINT16)

    FMSTR_TSA_RO_VAR(nAppCmdCounter, FMSTR_TSA_UINT8)

    FMSTR_TSA_RW_VAR(so1, FMSTR_TSA_USERTYPE(OUTER_STRUCT))
    FMSTR_TSA_RW_VAR(si1, FMSTR_TSA_USERTYPE(INNER_STRUCT))
    FMSTR_TSA_RW_VAR(varENUM, FMSTR_TSA_USERTYPE(EXAMPLE_ENUM))
    FMSTR_TSA_RW_VAR(varIncSpeed, FMSTR_TSA_USERTYPE(INCREMENT_SPEED))

    FMSTR_TSA_STRUCT(OUTER_STRUCT)
    FMSTR_TSA_MEMBER(OUTER_STRUCT, a, FMSTR_TSA_UINT16)
    FMSTR_TSA_MEMBER(OUTER_STRUCT, b, FMSTR_TSA_UINT32)
    FMSTR_TSA_MEMBER(OUTER_STRUCT, inA, FMSTR_TSA_USERTYPE(INNER_STRUCT))
    FMSTR_TSA_MEMBER(OUTER_STRUCT, inB, FMSTR_TSA_USERTYPE(INNER_STRUCT))

    FMSTR_TSA_STRUCT(INNER_STRUCT)
    FMSTR_TSA_MEMBER(INNER_STRUCT, aa, FMSTR_TSA_UINT16)
    FMSTR_TSA_MEMBER(INNER_STRUCT, bb, FMSTR_TSA_UINT32)
    FMSTR_TSA_MEMBER(INNER_STRUCT, cc, FMSTR_TSA_SINT16)
    FMSTR_TSA_MEMBER(INNER_STRUCT, dd, FMSTR_TSA_SINT32)
    FMSTR_TSA_MEMBER(INNER_STRUCT, ee, FMSTR_TSA_UINT8)
    FMSTR_TSA_MEMBER(INNER_STRUCT, ff, FMSTR_TSA_SINT8)

    FMSTR_TSA_ENUM(EXAMPLE_ENUM)
    FMSTR_TSA_CONST(EXAMPLE_ENUM_FIRST_ENTRY)
    FMSTR_TSA_CONST(EXAMPLE_ENUM_SECOND_ENTRY)
    FMSTR_TSA_CONST(EXAMPLE_ENUM_THIRD_ENTRY)

    FMSTR_TSA_ENUM(INCREMENT_SPEED)
    FMSTR_TSA_CONST(SPEED_ZERO)
    FMSTR_TSA_CONST(SPEED_FAST)
    FMSTR_TSA_CONST(SPEED_NORMAL)
    FMSTR_TSA_CONST(SPEED_SLOW)

FMSTR_TSA_TABLE_END()

/****************************************************************************
 *
 * This is an example of another TSA table. Typically, you put one table
 * to each .c file where your global or static variables are instantiated.
 */

FMSTR_TSA_TABLE_BEGIN(next_table)
    FMSTR_TSA_RO_VAR(so2, FMSTR_TSA_USERTYPE(OUTER_STRUCT))
    FMSTR_TSA_RO_VAR(si2, FMSTR_TSA_USERTYPE(INNER_STRUCT))
FMSTR_TSA_TABLE_END()

/****************************************************************************
 *
 * Another TSA table with specific information about in-memory files,
 * project files and related web-links. This feature requires FreeMASTER
 * version 2.0 or later running on the PC Host to make use of this "Active Content"
 */

/* Uncomment this macro to add link to your project file accessible online */
//#define PROJECT_WEB_LINK "http://customwebsite.com/path/file.pmp"

/* In-memory text "files" which are going to be accessible in FreeMASTER browser */
#if FMSTR_USE_TSA && FMSTR_DEMO_ENOUGH_ROM
static const char readme_txt[] =
    "This is a sample text file stored in target device memory and "
    "retrieved over the FreeMASTER protocol.\n"
    "Press the back button to return to previous page.";
static const char one_txt[] =
    "This is sample file named 'one.txt'\n"
    "Press the back button to return to previous page.";
static const char two_txt[] =
    "This is sample file named 'two.txt'\n"
    "Press the back button to return to previous page.";
static const char three_txt[] =
    "This is sample file named 'three.txt'\n"
    "Press the back button to return to previous page.";
#endif

/* Data files generated with bin2h utility are put here as C arrays.
 * Get all files if enough ROM memory is available, otherwise get
 * only the basic index.html file. */
#if FMSTR_DEMO_LARGE_ROM
#include "example_data.h"
#elif FMSTR_DEMO_ENOUGH_ROM
#include "data_html_index.h"
#endif

FMSTR_TSA_TABLE_BEGIN(files_and_links)

#if FMSTR_DEMO_ENOUGH_ROM
    /* text files in the default / root folder */
    FMSTR_TSA_MEMFILE("index.htm", index_htm, sizeof(index_htm))
    FMSTR_TSA_MEMFILE("readme.txt", readme_txt, sizeof(readme_txt))
    /* text files in dedicated folder */
    FMSTR_TSA_DIRECTORY("/text_files")
    FMSTR_TSA_MEMFILE("one.txt", one_txt, sizeof(one_txt))
    FMSTR_TSA_MEMFILE("two.txt", two_txt, sizeof(two_txt))
    /* text file in separate folder which is directly specified */
    FMSTR_TSA_MEMFILE("/other/three.txt", three_txt, sizeof(three_txt))
    /* links to be visible in FreeMASTER GUI */
    FMSTR_TSA_HREF("Board's Built-in Welcome Page", "/index.htm")
#endif

#if FMSTR_DEMO_LARGE_ROM
    /* image files in dedicated folder */
    FMSTR_TSA_DIRECTORY("/images")
    FMSTR_TSA_MEMFILE("file.png", file_png, sizeof(file_png))
    FMSTR_TSA_MEMFILE("logo.png", logo_png, sizeof(logo_png))
    FMSTR_TSA_MEMFILE("fmstrlogo.png", fmstrlogo_png, sizeof(fmstrlogo_png))
    /* project files in root */
    FMSTR_TSA_MEMFILE("/demo.pmp.zip", demo_pmp, sizeof(demo_pmp))
    /* projects to be made available in FreeMASTER */
    FMSTR_TSA_PROJECT("FreeMASTER Demonstration Project (embedded in device)", "/demo.pmp.zip")
#endif /* FMSTR_DEMO_ENOUGH_ROM */

/* project file stored online */
#ifdef PROJECT_WEB_LINK
    FMSTR_TSA_PROJECT("Full FreeMASTER Project (online)", PROJECT_WEB_LINK)
#endif

    /* additional web links to be shown in FreeMASTER GUI */
    FMSTR_TSA_HREF("NXP Web Site", "http://www.nxp.com")
    FMSTR_TSA_HREF("MCUXpresso SDK Home Page","http://www.nxp.com/mcuxpresso/sdk")
    FMSTR_TSA_HREF("FreeMASTER Home Page", "http://www.nxp.com/freemaster")
FMSTR_TSA_TABLE_END()

// clang-format on

/* RAM buffer for runtime definition of the TSA table
 * up to 5 variables will be enabled to register dynamically */
static unsigned char tsa_dyn_table_storage[5 * sizeof(FMSTR_TSA_ENTRY)];

/****************************************************************************
 *
 * This list describes all TSA tables which should be exported to the
 * FreeMASTER application.
 *
 */

FMSTR_TSA_TABLE_LIST_BEGIN()
FMSTR_TSA_TABLE(first_table)
FMSTR_TSA_TABLE(next_table)
FMSTR_TSA_TABLE(files_and_links)
FMSTR_TSA_TABLE_LIST_END()

/****************************************************************************
 *
 * This function is registered as an application command handler (see the
 * initialization code below. It gets automatically invoked when the
 * FreeMASTER PC Host tool sends appropriate application command.
 *
 */

FMSTR_APPCMD_RESULT my_appcmd_handler(FMSTR_APPCMD_CODE nAppcmd, FMSTR_APPCMD_PDATA pData, FMSTR_SIZE nDataLen)
{
    // the return value is used as the application command result code
    return 0x10;
}

/****************************************************************************
 *
 * This function is registered as a pipe communication handler (see the
 * initialization code below. It gets automatically invoked when the
 * FreeMASTER PC Host tool services the open pipe. This handler is called
 * any time the pipe receives the data or when PC probes if pipe has anything
 * to transmit.
 *
 */

void my_pipe_handler(FMSTR_HPIPE hpipe)
{
    static const char *names[10] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

    static char buff[4];
    char *n;
    char c;

    /* This simple handler translates the digits into number names.
     * Other characters are just echoed back.  */
    while (FMSTR_PipeRead(hpipe, (FMSTR_ADDR)buff, 1, 0))
    {
        c = buff[0];

        if (c >= '0' && c <= '9')
        {
            /* echo a number name */
            n = (char *)names[c - '0'];
            FMSTR_PipeWrite(hpipe, (FMSTR_ADDR)n, FMSTR_StrLen(n), 0);
        }
        else
        {
            /* echo non-number character */
            FMSTR_PipeWrite(hpipe, (FMSTR_ADDR)buff, 1, 0);
        }
    }
}

/****************************************************************************
 *
 * Example of more useful pipe handler function which performs multiplication
 * operation on two input long integers, returning one long integer.
 *
 */

void my_pipe_math(FMSTR_HPIPE hpipe)
{
    unsigned long result;
    unsigned long buff[2];

    /* we want to receive both operands at once (we do not want to do any caching here).
     * The 'granularity' argument can be used for this purpose. */
    while (FMSTR_PipeRead(hpipe, (FMSTR_ADDR)&buff, 8, 8) == 8)
    {
        result = buff[0] * buff[1];

        /* Note: Thanks to granularity argument, we can be sure that the
         * data are sent as a whole (if there is a space in the transmit buffer)
         * or not sent at all. */
        FMSTR_PipeWrite(hpipe, (FMSTR_ADDR)&result, 4, 4);
    }
}

/* Pipe memory buffers */
static unsigned char pipe1_rxb[10];
static unsigned char pipe1_txb[128];
static unsigned char pipe2_rxb[10];
static unsigned char pipe2_txb[128];
static unsigned char pipe3_rxb[8];
static unsigned char pipe3_txb[4];

/* Pipe welcome strings put to pipe as soon as application starts. */
static char *sz_pipe1_hello = "Hello from embedded application! Pipe 1.\n";
static char *sz_pipe2_hello = "Kind regards from from embedded application! Pipe 2.\n";
static char *sz_pipe_other  = "Send me numbers 0-9 and I will echo it back to you on the same pipe.\n";

/****************************************************************************
 * General initialization of FreeMASTER example */

void FMSTR_Example_Init(void)
{
    FMSTR_Example_Init_Ex(FMSTR_TRUE);
}

void FMSTR_Example_Init_Ex(FMSTR_BOOL callFmstrInit)
{
    static FMSTR_U8 recBuffer[512]; /* Recorder #1 sampling buffer */
    FMSTR_REC_BUFF recBuffCfg;
    FMSTR_HPIPE hpipe;
    int i;

    var8     = 0;
    var8rw   = 0;
    var8inc  = 1;
    var16    = 0;
    var16rw  = 0;
    var16inc = 1;
    var32    = 0;
    var32rw  = 0;
    var32inc = 100;

    frac8   = 0;
    frac16  = 0;
    frac32  = 0;
    ufrac8  = 0;
    ufrac16 = 0;
    ufrac32 = 0;

    arr_size = ARR_SIZE;
    
#if FMSTR_DEMO_SUPPORT_I64
    var64    = 0;
    var64rw  = 0;
    var64inc = 10000;
    frac64   = 0;
    ufrac64  = 0;
#endif

#if FMSTR_DEMO_SUPPORT_FLT
    varFLT    = 0;
    varFLTrw  = 0;
    varFLTinc = 1.0;
    arrFLT_size = ARR_SIZE_FLT;    
#endif

#if FMSTR_DEMO_SUPPORT_DBL
    varDBL    = 0;
    varDBLrw  = 0;
    varDBLinc = 1.0;
    arrDBL_size = ARR_SIZE_DBL;
#endif

    varENUM     = EXAMPLE_ENUM_SECOND_ENTRY;
    varIncSpeed = SPEED_NORMAL;

    for (i = 0; i < ARR_SIZE; i++)
    {
        arr8[i]  = (unsigned char)i;
        arr16[i] = (unsigned short)i * 10;
        arr32[i] = (unsigned long)i * 100;
#if FMSTR_DEMO_SUPPORT_I64
        arr64[i] = (unsigned long long)i * 1000;
#endif
    }

#if FMSTR_DEMO_SUPPORT_FLT
    for (i = 0; i < ARR_SIZE_FLT; i++)
        arrFLT[i] = ((float)i) / 10;
#endif

#if FMSTR_DEMO_SUPPORT_DBL
    for (i = 0; i < ARR_SIZE_DBL; i++)
        arrDBL[i] = ((double)i) / 10;
#endif

    /* FreeMASTER driver initialization */
    if(callFmstrInit)
        FMSTR_Init();
    
    /* Registering the App.Command handler */
    FMSTR_RegisterAppCmdCall(10, my_appcmd_handler);

    /* Example of dynamic TSA table definition in runtime. Use this approach to add TSA entries
       in runtime for variables whose address is not known in compilation time. This may be used for
       dynamically allocated variables, provided that they remain allocated at the same place throughout
       the whole application runtime. In this case, we just demonstrate it using var16prv symbol. */
    FMSTR_SetUpTsaBuff((FMSTR_ADDR)tsa_dyn_table_storage, sizeof(tsa_dyn_table_storage));
    FMSTR_TsaAddVar("var16prv", FMSTR_TSA_UINT16, (void *)&var16prv, sizeof(var16prv), FMSTR_TSA_INFO_RW_VAR);

    /* Note that Recorder #0 is set up automatically when FMSTR_USE_RECORDER>0 and FMSTR_REC_BUFF_SIZE != 0
       this is to enable backward compatibility with older driver versions. Extra recorder instances
       need to be set up during initialization (see below). */

    /* Setup the buffer for Recorder #1 */
#if FMSTR_USE_RECORDER >= 2
    recBuffCfg.addr          = (FMSTR_ADDR)recBuffer;
    recBuffCfg.size          = sizeof(recBuffer);
    recBuffCfg.basePeriod_ns = 0; /* Unknown period, use FMSTR_RecorderSetTimeBase to set in runtime later */
    recBuffCfg.name          = "Example of additional recorder";
    FMSTR_RecorderCreate(1, &recBuffCfg);
#endif
    
    /* Open pipes. The first pipe at port 1 */
    hpipe = FMSTR_PipeOpen(1, my_pipe_handler, (FMSTR_ADDR)pipe1_rxb, sizeof(pipe1_rxb), (FMSTR_ADDR)pipe1_txb,
                           sizeof(pipe1_txb), FMSTR_PIPE_TYPE_ANSI_TERMINAL, "Pipe 1");

    if (hpipe)
    {
        FMSTR_PipeWrite(hpipe, (FMSTR_ADDR)sz_pipe1_hello, FMSTR_StrLen(sz_pipe1_hello), 0);
        FMSTR_PipeWrite(hpipe, (FMSTR_ADDR)sz_pipe_other, FMSTR_StrLen(sz_pipe_other), 0);
    }

    /* Second pipe at port 2 */
    hpipe = FMSTR_PipeOpen(2, my_pipe_handler, (FMSTR_ADDR)pipe2_rxb, sizeof(pipe2_rxb), (FMSTR_ADDR)pipe2_txb,
                           sizeof(pipe2_txb), FMSTR_PIPE_TYPE_ANSI_TERMINAL, "Pipe 2");

    if (hpipe)
    {
        FMSTR_PipeWrite(hpipe, (FMSTR_ADDR)sz_pipe2_hello, FMSTR_StrLen(sz_pipe2_hello), 0);
        FMSTR_PipeWrite(hpipe, (FMSTR_ADDR)sz_pipe_other, FMSTR_StrLen(sz_pipe_other), 0);
    }

    /* Third pipe is a binary one at port 33 */
    FMSTR_PipeOpen(33, my_pipe_math, (FMSTR_ADDR)pipe3_rxb, sizeof(pipe3_rxb), (FMSTR_ADDR)pipe3_txb, sizeof(pipe3_txb),
                   FMSTR_PIPE_TYPE_UNICODE_TERMINAL, "Test pipe");
}

/****************************************************************************
 *
 * Poll function of the FreeMASTER example, called periodically from the
 * main application. We increment the variables so the change can be monitored
 * in Variable Watch or graphs displayed in the FreeMASTER tool.
 *
 */
void FMSTR_Example_Poll(void)
{
    FMSTR_Example_Poll_Ex(FMSTR_TRUE);
}

void FMSTR_Example_Poll_Ex(FMSTR_BOOL callFmstrPoll)
{
    static unsigned short div;
    unsigned short nAppCmdCode;
    int i;

    /* scope variables, increment once a while */
    if (varIncSpeed && ++div > varIncSpeed)
    {
        var8 += var8inc;
        var16 += var16inc;
        var32 += var32inc;

#if FMSTR_DEMO_SUPPORT_I64
        var64 += var64inc;
#endif
#if FMSTR_DEMO_SUPPORT_FLT
        varFLT = varFLT + varFLTinc;
        for (i = 0; i < arrFLT_size; i++)
            arrFLT[i]++;
#endif
#if FMSTR_DEMO_SUPPORT_DBL
        varDBL = varDBL + varDBLinc;
        for (i = 0; i < arrDBL_size; i++)
            arrDBL[i]++;
#endif

        div = 0;
    }

    /* the application commands not registered with callback handlers
       can be detected and processed using the API calls below */

    /* first, check if a new command has been received */
    nAppCmdCode = FMSTR_GetAppCmd();

    /* when a new command arrives, the nAppCmdCode contains the application
       command code. In other case, the "NOCMD" special value is returned */
    if (nAppCmdCode != FMSTR_APPCMDRESULT_NOCMD)
    {
        nAppCmdCounter++;

        /* each command may have different processing and different
           result code. The command processing is finished by
           calling FMSTR_AppCmdAck() with the result code value */
        switch (nAppCmdCode)
        {
            case 1:
                FMSTR_AppCmdAck(1);
                break;
            case 2:
                FMSTR_AppCmdAck(0xcd);
                break;
            default:
                FMSTR_AppCmdAck(0);
                break;
        }
    }

    /* This call should rather be placed in the timer interrupt or anywhere
       where the recorder sampling should occur. */
    FMSTR_Recorder(0);

    /* In this example we call both recorders here in the main loop. In real project
        each recorder will be called from different place. */
    FMSTR_Recorder(1);

    /* The FreeMASTER poll call must be called in the main application loop
       to handle the communication interface and protocol. */
    if(callFmstrPoll)
        FMSTR_Poll();
}
