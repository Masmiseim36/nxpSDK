
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
    UCHAR bt_snoop_file_name[48U];

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

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    nbytes = 0U;

    (BT_IGNORE_RETURN_VALUE) BT_fops_file_write
    (
        buffer,
        size,
        bt_snoop_fd,
        &nbytes
    );

    return nbytes;
}

/*
 * This function returns elapsed time in microsecond from 01-Jan-0000
 */
UINT64 btsnoop_get_us_timestamp_pl(void)
{
    return BT_get_us_timestamp();
}

#endif /* BT_SNOOP */

