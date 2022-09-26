
/**
 *  \file vcom_pl.h
 *
 *  This files containes the definitins used by the VCOM driver Layer
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_VCOM_PL_
#define _H_VCOM_PL_

/** --------------------------------- Header File Inclusion */
#include "BT_common.h"

/* ------------------------------------------- VCOM Debugging */
#ifndef VCOM_NO_DEBUG
#define VCOM_ERR(...)         BT_debug_error(BT_MODULE_ID_VCOM, __VA_ARGS__)
#else  /* VCOM_NO_DEBUG */
#define VCOM_ERR              BT_debug_null
#endif /* VCOM_NO_DEBUG */

#ifdef VCOM_DEBUG

#define VCOM_TRC(...)         BT_debug_trace(BT_MODULE_ID_VCOM, __VA_ARGS__)
#define VCOM_INF(...)         BT_debug_info(BT_MODULE_ID_VCOM, __VA_ARGS__)

#define VCOM_debug_dump_bytes(data, datalen) BT_debug_dump_bytes(BT_MODULE_ID_VCOM, (data), (datalen))

#else /* VCOM_DEBUG */

#define VCOM_TRC              BT_debug_null
#define VCOM_INF              BT_debug_null

#define VCOM_debug_dump_bytes(data, datalen)

#endif /* VCOM_DEBUG */

/* ---------------------------------- Global Definitions */
/* Number of VCOM instances */
#define VCOM_NUM_INSTANCES          1U

/* Invalid VCOM handle */
#define VCOM_HANDLE_INVALID        -1

/* ---------------------------------- Data Structures */
typedef INT16 VCOM_HANDLE;

typedef struct _VCOM_INSTANCE
{
    /* COM Handle */
    int handle;

    /* Callback to give read data on instance */
    void (* read_pl)(UCHAR * data, UINT16 datalen);

    /* Task pointer for the instance */
    BT_THREAD_RETURN_TYPE (* read_task) (BT_THREAD_ARGS args);

} VCOM_INSTANCE;

/* ---------------------------------- Function Declarations */
/** VCOM Interfaces */
void vcom_init_pl(void);
VCOM_HANDLE vcom_connect_pl
            (
                CHAR * port,
                UINT32 baud,
                UCHAR flow,
                void(*read_cb)(UCHAR *data, UINT16 datalen)
            );
VCOM_HANDLE vcom_disconnect_pl(VCOM_HANDLE handle);
INT16 vcom_write_pl(VCOM_HANDLE handle, UCHAR * buf, UINT16 length);

#endif /* _H_VCOM_PL_ */

