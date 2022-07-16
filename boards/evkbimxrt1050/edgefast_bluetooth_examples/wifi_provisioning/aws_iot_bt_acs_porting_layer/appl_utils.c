
/**
 *  \file appl_utils.c
 *
 *  Main Application Utility File
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_utils.h"

/* Configure logs for the functions in this file. */
#ifdef IOT_LOG_LEVEL_GLOBAL
    #define LIBRARY_LOG_LEVEL    IOT_LOG_LEVEL_GLOBAL
#else
    #define LIBRARY_LOG_LEVEL    IOT_LOG_NONE
#endif

#define LIBRARY_LOG_NAME         ( "APPL_UTILS" )
#include "iot_logging_setup.h"

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */

/* --------------------------------------------- Functions */
void appl_get_bd_addr(UCHAR *bd_addr)
{
    int i, read;
    UCHAR bd[BT_BD_ADDR_SIZE];

    BT_mem_set(bd, 0x0, BT_BD_ADDR_SIZE);

    fflush(stdout);
    for(i = 0; i < BT_BD_ADDR_SIZE; i ++)
    {
        scanf("%x", &read);
        bd[i] = (UCHAR)read;
    }

    BT_mem_copy(bd_addr, bd, BT_BD_ADDR_SIZE);
}


void appl_dump_bytes (UCHAR *buffer, UINT16 length)
{
    char hex_stream[49];
    char char_stream[17];
    UINT32 i;
    UINT16 offset, count;
    UCHAR c;

    IotLogDebug("\n");
    IotLogDebug("-- Dumping %d Bytes --\n",
    (int)length);

    IotLogDebug(
    "-------------------------------------------------------------------\n");

    count = 0;
    offset = 0;
    for(i = 0; i < length; i ++)
    {
        c =  buffer[i];
        sprintf(hex_stream + offset, "%02X ", c);

        if ( (c >= 0x20) && (c <= 0x7E) )
        {
            char_stream[count] = c;
        }
        else
        {
            char_stream[count] = '.';
        }

        count ++;
        offset += 3;

        if(16 == count)
        {
            char_stream[count] = '\0';
            count = 0;
            offset = 0;

            IotLogDebug("%s   %s\n",
            hex_stream, char_stream);

            BT_mem_set(hex_stream, 0, 49);
            BT_mem_set(char_stream, 0, 17);
        }
    }

    if(offset != 0)
    {
        char_stream[count] = '\0';

        /* Maintain the alignment */
        IotLogDebug("%-48s   %s\n",
        hex_stream, char_stream);
    }

    IotLogDebug(
    "-------------------------------------------------------------------\n");

    IotLogDebug("\n");

    return;
}

