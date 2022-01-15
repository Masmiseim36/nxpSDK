
/**
 *  \file sm_pl.c
 *
 *  This file contains the implementation for all platform specific
 *  extensions for Security Manager module.
 *
 *  The code here manages the Rank awarded to Device Database entries
 *  whenever one is "accessed" - either on completion of ACL establishment
 *  or Pairing.
 *
 *  The most recently used/accessed device is the one with Rank = 1.
 *  The least recently used/accessed device is the one with Rank = Max.
 *
 *  The API functions defined here can be modified to any extent, including
 *  the name & prototype of the functions, as these functions are called
 *  by applications only.
 *
 *  On the other hand, the internal functions (functions not starting with
 *  "BT_") are used by SM core. Content of these functions can be modified
 *  to implement a different logic for storing ranks (or, for completely
 *  different use all together), however the function names and prototypes
 *  cannot be changed.
 *
 *  Important points to note:
 *  SM core calls the internal functions for a Device Database entry which
 *  is guaranteed to be marked "USED". Hence, Ranks should be awarded and/or
 *  managed only for Device Database entries which are marked "USED" only.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "sm_internal.h"
#include "sm_extern.h"

#ifdef CLASSIC_SEC_MANAGER
/* ----------------------------------------- External Global Variables */


/* ----------------------------------------- Exported Global Variables */


/* ----------------------------------------- Static Global Variables */
/** Highest Rank offered to devices so far */
DECL_STATIC UINT32 sm_rank_count;


/* ----------------------------------------- API Functions */
/** To return a device with specified Device Attribute */
API_RESULT BT_sm_get_device_attr_pl
           (
               /* IN */  SM_DEVICE_ATTR_PL    attr,
               /* OUT */ UCHAR *              bd_addr
           )
{
    UINT32 di;
    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;

    if (NULL == bd_addr)
    {
        SM_ERR(
        "[SM-PL] NULL Unacceptable for BD_ADDR\n");

        retval = SM_INVALID_PARAMETERS; /* return SM_INVALID_PARAMETERS; */
    }
    else
    {
        /* Lock SM */
        sm_lock();

        SM_INF(
        "[SM-PL] Searching for Device with Rank = %u\n",
        (unsigned int) attr);

        for (di = 0U; di < SM_MAX_DEVICES; di ++)
        {
            if ((SM_DEVICE_USED == sm_devices[di].valid) &&
                (attr == sm_devices[di].device_attr_pl))
            {
                SM_INF(
                "[SM-PL] Rank %u -> Loc %u, Device = %02X\n",
                (unsigned int) attr, (unsigned int) di,
                sm_devices[di].device_handle);

                /* Found match. Copy BD_ADDR */
                sm_get_device_address (di, bd_addr);

                break;
            }
        }

        if (SM_MAX_DEVICES == di)
        {
            SM_ERR(
            "[SM-PL] FAILED to Match Rank %u in Device Database\n",
            (unsigned int) attr);

            retval = SM_NO_DEVICE_ENTRY;
        }
        else
        {
            retval = API_SUCCESS;
        }

        /* Unlock SM */
        sm_unlock();
    }

    return retval;
}


/** To return the Rank Count offered so far */
API_RESULT BT_sm_get_rank_count_pl
           (
               /* OUT */ SM_DEVICE_ATTR_PL *    rank_count
           )
{
    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;

    if (NULL == rank_count)
    {
        SM_ERR(
        "[SM-PL] NULL Unacceptable for Rank Count\n");

        retval = SM_INVALID_PARAMETERS; /* return SM_INVALID_PARAMETERS; */
    }
    else
    {
        /* Lock SM */
        sm_lock();

        /* Copy Rank Count offered so far */
        *rank_count = sm_rank_count;

        /* Unlock SM */
        sm_unlock();
    }

    return retval;
}


/* ----------------------------------------- Internal Functions */
/**
 *  \fn sm_init_device_attr_pl
 *
 *  \par Description:
 *  This function initializes platform specific device attributes. This
 *  function is called from sm_bt_init() during Bluetooth-ON initialization
 *  of SM module.
 *
 *  \param None
 *
 *  \return None
 */
void sm_init_device_attr_pl (void)
{
    UINT32 di;

    /* Initialize Rank Count */
    sm_rank_count = 0U;

    /* Initialize Ranks for Devices */
    for (di = 0U; di < SM_MAX_DEVICES; di ++)
    {
        sm_devices[di].device_attr_pl = 0U;
    }
}


/**
 *  \fn sm_set_device_attr_pl
 *
 *  \par Description:
 *  This function is called to set the platform specific attribute upon
 *  reading from persistent storage at the time of Bluetooth-ON
 *  initialization of SM module.
 *
 *  \param [in] di
 *         The SM Device Database Index
 *  \param [in] attr
 *         The platform specific attribute for the device
 *
 *  \return None
 */
void sm_set_device_attr_pl
     (
         /* IN */  UINT32               di,
         /* IN */  SM_DEVICE_ATTR_PL    attr
     )
{
    /* Store Device Attribute */
    sm_devices[di].device_attr_pl = attr;

    /* Update Rank Count */
    if (attr > sm_rank_count)
    {
        sm_rank_count = attr;
    }

    /* Debug: Dump Device Ranks */
    sm_dump_device_rank_pl();
}


/**
 *  \fn sm_update_device_attr_pl
 *
 *  \par Description:
 *  This function is called to update the platform specific attribute for
 *  a device under the following conditions/events:
 *  - ACL connection initiation from local (application) or remote end
 *  - ACL connection completion
 *  - Pairing completion
 *  - Setting a device as Trusted
 *
 *  This function is also called whenever a device is deleted from device
 *  database, or from Paired or Trusted list.
 *
 *  \param [in] event
 *         The event for which updation of device specific attribute might
 *         be required.
 *  \param [in] di
 *         The SM Device Database Index
 *
 *  \return API_RESULT:
 *          - API_SUCCESS: If successful.
 *          - Error Codes: An error code describing the cause of failure.
 */
API_RESULT sm_update_device_attr_pl
           (
               /* IN */  UCHAR     event,
               /* IN */  UINT32    di
           )
{
    UINT32 i;
    SM_DEVICE_ATTR_PL rank;

    SM_TRC(
    "[SM-PL] Event 0x%02X for Device Index %u, Rank %u\n",
    event, (unsigned int) di, (unsigned int) sm_devices[di].device_attr_pl);

    switch (event)
    {
    case SM_DEVICE_ATTR_PL_DELETE:
        /*
         *  Deletion of Rank is handled only if the Device has
         *  been provided with a valid Rank before.
         */
        if (0U != sm_devices[di].device_attr_pl)
        {
            /* Save current Rank of the Device */
            rank = sm_devices[di].device_attr_pl;

            /* Reset Device's Rank */
            sm_devices[di].device_attr_pl = 0U;

            /* Decrement Ranks of Devices holding Ranks above this Device */
            for (i = 0U; i < SM_MAX_DEVICES; i ++)
            {
                if ((i == di) ||
                    (SM_DEVICE_USED != sm_devices[i].valid))
                {
                    continue;
                }

                if (sm_devices[i].device_attr_pl > rank)
                {
                    sm_devices[i].device_attr_pl --;
                }
            }

            /* Decrement Rank Count offered so far */
            sm_rank_count --;
        }

#ifndef SM_NOSYNC_PSSTORE_ON_DELETION
        /* Unlock */
        sm_unlock();

        /* Update Persistent Storage */
        (BT_IGNORE_RETURN_VALUE) BT_storage_store_db(STORAGE_TYPE_PERSISTENT, STORAGE_EVENT_OTHERS);

        /* Lock */
        sm_lock();
#endif /* SM_NOSYNC_PSSTORE_ON_DELETION */

        break;

    case SM_DEVICE_ATTR_PL_SET_TRUSTED:         /* Fall Through */
    case SM_DEVICE_ATTR_PL_ACL_COMPLETE:        /* Fall Through */
    case SM_DEVICE_ATTR_PL_PAIRING_COMPLETE:
        /*
         *  If there is no Rank awarded to the Device so far, award
         *  it a Rank provisionally so that it is put in the end
         *  of the Rank list.
         */
        if (0U == sm_devices[di].device_attr_pl)
        {
            /* Increment Rank Count offered so far */
            sm_rank_count ++;

            /* The Device is being added in the end of Rank List */
            sm_devices[di].device_attr_pl = sm_rank_count;
        }

        /*
         *  Setting a Device as Trusted is about automatic authorization
         *  for that device to allow use of services on the local device.
         *  It is not same as ACL or Pairing completion, which really means
         *  accessing/using a device for which the rank to be updated.
         *
         *  However, when a device is set as trusted, it must be awarded a
         *  rank because if the device happens to be stored in storage, then
         *  at the time of recovery, it must have a rank.
         *
         *  Ideally, it is expected that the application ensures a device
         *  is set as trusted only when pairing (which requires ACL before or
         *  after anyway) is complete.
         *
         *  If the device already has a rank, it is left unchanged. Otherwise,
         *  by the above logic, the device would be earning a rank at the end
         *  of the Rank list - which is good enough for a device just set
         *  as trusted.
         */

        /*
         *  If the Device already at the top of Rank List (Rank = 1),
         *  then nothing needs to be done - the Device continues to
         *  hold Rank 1.
         */
        rank = 1U;

        if ((1U != sm_devices[di].device_attr_pl) && (SM_DEVICE_ATTR_PL_SET_TRUSTED != event))
        {
            /* Save current Rank of the Device */
            rank = sm_devices[di].device_attr_pl;

            /* Move the Device at the top of Rank List */
            sm_devices[di].device_attr_pl = 1U;

            /* Update Rank of Devices appearing below by 1 */
            for (i = 0U; i < SM_MAX_DEVICES; i ++)
            {
                if ((i == di) ||
                    (SM_DEVICE_USED != sm_devices[i].valid))
                {
                    continue;
                }

                if (sm_devices[i].device_attr_pl < rank)
                {
                    sm_devices[i].device_attr_pl ++;
                }
            }
        }

        /*
         *  At this point Rank for the Devices have been updated. The updated
         *  Ranks now must be stored in the Persistent Storage, to avoid
         *  unwanted loss of information in case of crash/hang.
         *
         *  Same argument holds true for Link Key as well, if this function
         *  is called for Pairing Complete.
         *
         *  Pairing Complete (Link Key Notification) might occur before ACL
         *  Complete in case of a new device in Security Mode 1. In other
         *  cases, ACL Complete is expected to happen before Pairing
         *  Complete, or, Pairing Complete need not happen at all.
         *
         *  Rank & Link Key must be stored in case of Pairing Complete,
         *  irrespective of when it happens w.r.t. ACL Complete.
         *
         *  Rank must be stored in case of ACL Complete, if it has changed
         *  from the previous value.
         */
    #ifdef SM_STORAGE
        if(!((1U == rank) && (SM_DEVICE_ATTR_PL_ACL_COMPLETE == event)))
        {
            UINT16            storage_type;

            /* Unlock */
            sm_unlock();

            if (SM_DEVICE_ATTR_PL_PAIRING_COMPLETE == event)
            {
                storage_type = STORAGE_EVENT_AUTH_UPDATE;
            }
            else
            {
                storage_type = STORAGE_EVENT_OTHERS;
            }

            /* Update Persistent Storage */
            (BT_IGNORE_RETURN_VALUE) BT_storage_store_db(STORAGE_TYPE_PERSISTENT, storage_type);

            /* Lock */
            sm_lock();
        }
    #endif /* SM_STORAGE */

        break;

    default:
        SM_TRC("Invalid Event Type: 0x%02X\n", event);
        break;
    }

    /* Debug: Dump Device Ranks */
    sm_dump_device_rank_pl();

    return API_SUCCESS;
}


/**
 *  \fn sm_purge_device_attr_pl
 *
 *  \par Description:
 *  This function is called to purge/delete one or more number of device
 *  from the device database, based on logic implemented in this function,
 *  in the event of an incoming or outgoing connection or pairing attempt
 *  while the device database is full.
 *
 *  \param [out] di
 *         A freed SM Device Database Index
 *
 *  \return API_RESULT:
 *          - API_SUCCESS: If successful.
 *          - Error Codes: An error code describing the cause of failure.
 */
API_RESULT sm_purge_device_list_pl
           (
               /* OUT */ UINT32 *    di
           )
{
    UINT32 i, rank_di;
    SM_DEVICE_ATTR_PL rank;
    API_RESULT retval;

    rank = 0U;
    rank_di = SM_MAX_DEVICES;

    /*
     *  Scan the Device Database to find out the Index that contains
     *  the highest Rank not having an active ACL or ACL in progress.
     *
     *  No need to check Device is 'USED' or not because this function
     *  is called at the time of adding a new device entity, and the
     *  list is ascertained to have no free entry (either INVALID or
     *  CAN_BE_FREED).
     */
    for (i = 0U; i < SM_MAX_DEVICES; i ++)
    {
        if ((sm_devices[i].device_attr_pl > rank) &&
            (SM_FALSE == SM_IS_DEVICE_ACL_INITIATED(i)) &&
            (HCI_INVALID_CONNECTION_HANDLE == sm_devices[i].acl_handle))
        {
            rank = sm_devices[i].device_attr_pl;
            rank_di = i;
        }
    }

    if (SM_MAX_DEVICES != rank_di)
    {
        SM_TRC(
        "[SM-PL] Purging/Deleting Device Index %u, Current Rank = %u\n",
        (unsigned int) rank_di,
        (unsigned int) sm_devices[rank_di].device_attr_pl);

        /* Delete SM Device Entity */
        sm_delete_device_entity (rank_di);

        /* Save Index */
        *di = rank_di;

        retval = API_SUCCESS;
    }
    else
    {
        SM_ERR(
        "[SM-PL] FAILED to Purge Device List for Highest Rank!\n");

        retval = SM_DEVICE_ENTRY_PURGE_FAILED;
    }

    /* Debug: Dump Device Ranks */
    sm_dump_device_rank_pl();

    return retval;
}

#ifdef STORAGE_CHECK_CONSISTENCY_ON_RESTORE
/**
 *  \fn sm_get_valid_dq_entries_pl
 *
 *  \par Description:
 *  This function is called to get bit field representing valid
 *  device queue entries.
 *
 *  \param [out] dq_entries
 *         Bit field to represent device queue entries.
 *         1 : Valid device queue entry.
 *         0 : Unknown device queue entry (it could be associated with BLE).
 *
 *  \return API_RESULT:
 *          - API_SUCCESS: If successful.
 *          - Error Codes: An error code describing the cause of failure.
 */
API_RESULT sm_get_valid_dq_entries_pl
           (
               /* OUT */ UINT32 *    dq_entries
           )
{
    *dq_entries = sm_dq_valid_entries;

    SM_TRC(
    "[SM] Valid DQ entries bit field 0x%08X.\n", sm_dq_valid_entries);

    return API_SUCCESS;
}
#endif /* STORAGE_CHECK_CONSISTENCY_ON_RESTORE */

#ifdef SEC_DEBUG
/** Debug: To dump/write Rank information of Devices in Debug Log */
void sm_dump_device_rank_pl (void)
{
    UINT32 di;

    SM_TRC(
    "[SM-PL] Ranking of Device Database Entries. Rank Count = %lu\n",
    sm_rank_count);

    SM_TRC(
    "[SM-PL] ------------------------------------------------\n");

    for (di = 0U; di < SM_MAX_DEVICES; di ++)
    {
        if (SM_DEVICE_USED == sm_devices[di].valid)
        {
            SM_TRC(
            "[SM-PL] Loc %3lu -> Device %02X -> Rank %lu\n",
            di, sm_devices[di].device_handle,
            sm_devices[di].device_attr_pl);
        }
    }

    SM_TRC(
    "[SM-PL] ------------------------------------------------\n");
}
#endif /* SEC_DEBUG */
#endif /* CLASSIC_SEC_MANAGER */

