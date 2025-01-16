
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

#ifndef CONFIG_BT_SNOOP
#define CONFIG_BT_SNOOP 0
#endif /* CONFIG_BT_SNOOP */

/*
*READ_BTSNOOP_FILENAME can be defined if user want to enter the btsnoop file name manually
*/

/*#define READ_BTSNOOP_FILENAME */
#ifdef BT_SNOOP

#if (defined(CONFIG_BT_SNOOP) && (CONFIG_BT_SNOOP > 0))

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

    /* Reset */
    BT_mem_set(bt_snoop_file_name, 0, sizeof(bt_snoop_file_name));

#ifdef READ_BTSNOOP_FILENAME
    UCHAR bt_snoop_name_location ;
	bt_snoop_name_location = BT_str_len(EM_FOPS_BASE) + BT_str_len(EM_FOPS_PATH_SEP);
	BT_str_n_copy(bt_snoop_file_name, EM_FOPS_BASE, BT_str_len(EM_FOPS_BASE));
	BT_str_n_copy(bt_snoop_file_name + BT_str_len(EM_FOPS_BASE), EM_FOPS_PATH_SEP, BT_str_len(EM_FOPS_PATH_SEP));
	printf("Enter the btsnoop filename\n");
	scanf("%44s",&bt_snoop_file_name[bt_snoop_name_location]);
	printf("btsnoop filename = %s\n",bt_snoop_file_name);
#else
    /* Add Prefix */
    BT_str_n_copy(bt_snoop_file_name, BT_SNOOP_FILE_NAME, BT_str_len(BT_SNOOP_FILE_NAME));
#endif /*READ_BTSNOOP_FILENAME*/
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

#else

void btsnoop_init_pl (void)
{
}

API_RESULT btsnoop_open_pl (void)
{
    return API_FAILURE;
}

API_RESULT btsnoop_close_pl (void)
{
    return API_FAILURE;
}

UINT16 btsnoop_write_pl (void * buffer, UINT16 size)
{
    return 0;
}

UINT64 btsnoop_get_us_timestamp_pl(void)
{
    return 0;
}

#endif /* CONFIG_BT_SNOOP */

#endif /* BT_SNOOP */
