
/**
 *  \file sm_pl.h
 *
 *  Header file for all platform specific implementation & extensions for
 *  Security Manager module.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_SM_PL_
#define _H_SM_PL_

/* ----------------------------------------- Header File Inclusion */
/* EtherMind Common Header Files */
#include "BT_common.h"


/* ----------------------------------------- Global Definitions */
/** Request Types for interaction with SM Core module */
#define SM_DEVICE_ATTR_PL_DELETE                0x01
#define SM_DEVICE_ATTR_PL_ACL_COMPLETE          0x02
#define SM_DEVICE_ATTR_PL_PAIRING_COMPLETE      0x03
#define SM_DEVICE_ATTR_PL_SET_TRUSTED           0x04


/* ----------------------------------------- Structures/Data Types */
/** Platform specific Device Attribute */
typedef UINT32 SM_DEVICE_ATTR_PL;


/* ----------------------------------------- Macros */


/* ----------------------------------------- API Declarations */
/** To return a device with specified Device Attribute */
API_RESULT BT_sm_get_device_attr_pl
           (
               /* IN */  SM_DEVICE_ATTR_PL    attr,
               /* OUT */ UCHAR *              bd_addr
           );

/** To return the Rank Count offered so far */
API_RESULT BT_sm_get_rank_count_pl
           (
               /* OUT */ SM_DEVICE_ATTR_PL *    rank_count
           );


/* ----------------------------------------- Function Declarations */
/* The following functions are defined to interact with SM Core module */

#ifdef SM_STORAGE
/**
 *  Security Manager Persistent Storage Read & Write
 *
 *  These functions are declared here so that Application can call them
 *  whenever required.
 */
void sm_storage_write (void);
void sm_storage_read (void);
void sm_pstorage_cb (UCHAR id, UCHAR type, UCHAR action);
#endif /* SM_STORAGE */

/** To initialize Platform specific Device Attributes */
void sm_init_device_attr_pl (void);

/** To set Platform specific Device Attributes after reading from Storage */
void sm_set_device_attr_pl
     (
         /* IN */  UINT32               di,
         /* IN */  SM_DEVICE_ATTR_PL    attr
     );

/** To update SM Platform specific Device Attribute */
API_RESULT sm_update_device_attr_pl
           (
               /* IN */  UCHAR    event,
               /* IN */  UINT32   di
           );

/** To delete/purge a device based on usage/device attributes */
API_RESULT sm_purge_device_list_pl
           (
               /* OUT */ UINT32 *    di
           );

/** To get bit field representation of valid device queue entries */
#ifdef STORAGE_CHECK_CONSISTENCY_ON_RESTORE
API_RESULT sm_get_valid_dq_entries_pl
           (
               /* OUT */ UINT32 *    dq_entries
           );
#endif /* STORAGE_CHECK_CONSISTENCY_ON_RESTORE */

#ifdef SEC_DEBUG
void sm_dump_device_rank_pl (void);
#else  /* SEC_DEBUG */
#define sm_dump_device_rank_pl()
#endif /* SEC_DEBUG */

#endif /* _H_SM_PL_ */


