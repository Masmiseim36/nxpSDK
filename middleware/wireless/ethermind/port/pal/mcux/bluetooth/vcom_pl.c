
/**
 *  \file vcom_pl.c
 *
 *  This File contains the routines to Transmit and Receive Data from
 *  the VCOM Transport Layer (Windows Version).
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "vcom_pl.h"

/* ----------------------------------------- Function Declaraions */

/* ----------------------------------------- External Global Variables */

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */

/* ----------------------------------------- Functions */
void vcom_init_pl (void)
{
    return;
}

VCOM_HANDLE vcom_connect_pl
            (
                CHAR * port,
                UINT32 baud,
                UCHAR flow,
                void (* read_cb)(UCHAR *data, UINT16 datalen)
            )
{
    return VCOM_HANDLE_INVALID;
}


VCOM_HANDLE vcom_disconnect_pl (VCOM_HANDLE handle)
{
    return VCOM_HANDLE_INVALID;
}

INT16 vcom_write_pl (VCOM_HANDLE handle, UCHAR * buf, UINT16 length)
{
    return -1;
}


