
/**
 *  \file device_queue_pl.c
 *
 *  This file contains source code for the platform specific portions of
 *  Device Queue.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* -------------------------------------- Header File Inclusion */
#include "device_queue_pl.h"
#include "smp_pl.h"

#ifdef CLASSIC_SEC_MANAGER
#include "sm_pl.h"
#endif /* CLASSIC_SEC_MANAGER */

/* -------------------------------------- External Global Variables */


/* -------------------------------------- Exported Global Variables */

/* -------------------------------------- Staic Global Variables */

/* -------------------------------------- Functions */

void device_queue_pl_init ( void )
{
    DQ_PL_TRC("[DQ PL]: Init\n");

    return;
}


void device_queue_pl_shutdown ( void )
{
    DQ_PL_TRC("[DQ PL]: Shutdown\n");

    return;
}


void device_queue_free_pl (DEVICE_HANDLE    * hndl)
{
    BT_IGNORE_UNUSED_PARAM(hndl);
}

void device_queue_full_pl(DEVICE_LINK_TYPE    link_type)
{
#if (defined BT_DUAL_MODE || defined CLASSIC_SEC_MANAGER)
    UINT32 sm_index;
#endif
    UCHAR  smp_index;

#ifdef BT_DUAL_MODE
    API_RESULT retval;

    /*
     * NOTE:
     *
     * The default reference handling here tries to purge SM/SMP
     * database to create a free device entity, based on the link type.
     * If it fails to free device entity based on the link type, then
     * try for the other.
     * Platform implementation can modify this as required. If one does
     * not want to make any free space here but act at a later point of
     * time, this function can be left empty. But in that case, all other
     * incoming connections will be disconnected from the stack.
     */

    /* Call to free a device and make space through SM purge  */
    if (DQ_BR_LINK == link_type)
    {
        retval = sm_purge_device_list_pl (&sm_index);

#ifdef BT_LE
        if (API_SUCCESS != retval)
        {
            (BT_IGNORE_RETURN_VALUE) smp_purge_device_list_pl(&smp_index);
        }
#endif /* BT_LE */
    }
#ifdef BT_LE
    else if (DQ_LE_LINK == link_type)
    {
        retval = smp_purge_device_list_pl(&smp_index);

        if (API_SUCCESS != retval)
        {
            (BT_IGNORE_RETURN_VALUE) sm_purge_device_list_pl(&sm_index);
        }
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }
#endif /* BT_LE */

#else
    /* Ignore Parameter */
    BT_IGNORE_UNUSED_PARAM(link_type);
    BT_IGNORE_UNUSED_PARAM(smp_index);

    /*
    * NOTE:
    *
    * The default reference handling here tries to purge SM
    * database to create a free device entity. Platform
    * implementation can modify this as required. If one does
    * not want to make any free space here but act at a later point of
    * time, this function can be left empty. But in that case, all other
    * incoming connections will be disconnected from the stack.
    */

#ifdef CLASSIC_SEC_MANAGER
    /* Call to free a device and make space through SM purge  */
    sm_purge_device_list_pl(&sm_index);
#endif /* CLASSIC_SEC_MANAGER */
#endif /* BT_DUAL_MODE */
}


void device_queue_disconnect_on_full_pl
     (
         /* IN */ BT_DEVICE_ADDR    * bd_addr,
         /* IN */ DEVICE_LINK_TYPE    link_type
     )
{
    BT_IGNORE_UNUSED_PARAM(bd_addr);

    /**
     *  Code shall not reach here.
     *  Confirm with the below print in the log.
     */
    DQ_PL_ERR (
    "[DQ PL]: Disconnect on FULL\n");

    /* Do the same handling as DQ full to try create an empty space */
    device_queue_full_pl(link_type);

    return;
}

#ifdef STORAGE_CHECK_CONSISTENCY_ON_RESTORE
void device_queue_cleanup_pl
     (
         /* IN */ UINT32 dq_valid_entries
     )
{
    DEVICE_HANDLE index;

    DQ_PL_TRC(
    "[DQ PL] Valid DQ entries bit field 0x%08X.\n", dq_valid_entries);

#if (1 != BT_MAX_DEVICE_QUEUE_SIZE)
    for (index = 0U; index < BT_MAX_DEVICE_QUEUE_SIZE; index++)
#else
    index = 0;
#endif /* (1 != BT_MAX_DEVICE_QUEUE_SIZE) */
    {
        /**
         * Check if the associated device queue handle is valid or not.
         * If not valid, free the device queue element.
         */
        if (0U == (dq_valid_entries & (1U << index)))
        {
            device_queue_free(&index);
        }
    }

    return;
}
#endif /* STORAGE_CHECK_CONSISTENCY_ON_RESTORE */

