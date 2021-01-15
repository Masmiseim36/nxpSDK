
/**
 *  \file btsnoop_pl.c
 *
 */

/*
 *  Copyright (C) 2016. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "btsnoop_pl.h"

#ifdef BT_SNOOP

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
/** Snoop File Descriptor */
DECL_STATIC BT_fops_file_handle bt_snoop_fd;


/* --------------------------------------------- Functions */

void btsnoop_init_pl (void)
{
    /* Initialize BT Snoop File Handle */
    bt_snoop_fd = NULL;
}

API_RESULT btsnoop_open_pl (void)
{
    API_RESULT retval;
    UCHAR bt_snoop_file_name[48];

    /* Add Prefix */
    BT_str_copy(bt_snoop_file_name, BT_SNOOP_FILE_NAME);

    retval = BT_fops_file_open
             (
                 bt_snoop_file_name,
                 (UCHAR *)"wb",
                 &bt_snoop_fd
             );

    return retval;
}

API_RESULT btsnoop_close_pl (void)
{
    API_RESULT retval;

    retval = BT_fops_file_close
             (
                 bt_snoop_fd
             );

    return retval;
}

UINT16 btsnoop_write_pl (void * buffer, UINT16 size)
{
    UINT16 nbytes;

    BT_fops_file_write
    (
        buffer,
        size,
        bt_snoop_fd,
        &nbytes
    );

    return nbytes;
}


/* Microseconds from 01-Jan-0000 till epoch(in ISO 8601: 1970 - 01 - 01T00 : 00 : 00Z). */
#define BTSNOOP_uS_TILL_EPOCH           0x00DCDDB30F2F8000ULL

/*
 * This function returns elapsed time in microsecond from 01-Jan-0000
 */
UINT64 btsnoop_get_us_timestamp_pl(void)
{
#ifndef FREERTOS
    UINT64 timestamp;
    SYSTEMTIME  system_time;
    FILETIME    file_time;

    GetSystemTime(&system_time);
    SystemTimeToFileTime(&system_time, &file_time);
    timestamp = ((UINT64)file_time.dwLowDateTime);
    timestamp += ((UINT64)file_time.dwHighDateTime) << 32;

    /* FILETIME till epoch in 100 nano seconds */
#define BTSNOOP_FILETIME_TILL_EPOCH     0x019DB1DED53E8000ULL
#if 0
    /* Calculated as below */
    {
        UINT64 timestamp2;
        system_time.wDay = 1;
        system_time.wDayOfWeek = 4;
        system_time.wHour = 0;
        system_time.wMilliseconds = 0;
        system_time.wMinute = 0;
        system_time.wMonth = 1;
        system_time.wSecond = 0;
        system_time.wYear = 1970;

        SystemTimeToFileTime(&system_time, &file_time);

        timestamp2 = ((UINT64)file_time.dwLowDateTime);
        timestamp2 += ((UINT64)file_time.dwHighDateTime) << 32;
    }
#endif /* 0 */

    timestamp -= BTSNOOP_FILETIME_TILL_EPOCH;

    /* Divide by 10 as the unit is 100 nano second */
    timestamp = timestamp / 10;
    timestamp += BTSNOOP_uS_TILL_EPOCH;

    return timestamp;
#else
    /* midnight, January 1st 2000 AD */
#define BTSNOOP_January_1st_2000_AD 0x00E03AB44A676000ULL
    UINT64 timestamp;
    timestamp = BTSNOOP_January_1st_2000_AD + ((UINT64)EM_get_time_ms() * 1000ULL);
    return timestamp;
#endif /* FREERTOS */
}

#endif /* BT_SNOOP */

