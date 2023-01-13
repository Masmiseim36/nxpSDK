
/**
 *  \file sm_ssp_pl.h
 *
 *  Header file for all platform specific implementation & extensions for
 *  Secure Simple Pairing (SSP) feature of Security Manager module.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_SM_SSP_PL_
#define _H_SM_SSP_PL_

/* ----------------------------------------- Header File Inclusion */
/* EtherMind Common Header Files */
#include "BT_common.h"
#include "BT_device_queue.h"

/* ----------------------------------------- Global Definitions */
/** IO Capability Constants */
#define SM_IO_CAPABILITY_DISPLAY_ONLY               0x00
#define SM_IO_CAPABILITY_DISPLAY_YES_NO             0x01
#define SM_IO_CAPABILITY_KEYBOARD_ONLY              0x02
#define SM_IO_CAPABILITY_NO_INPUT_NO_OUTPUT         0x03


/** Default IO Capability of the Device */
#define SM_IO_CAPABILITY_DEFAULT \
        SM_IO_CAPABILITY_DISPLAY_YES_NO


/* ----------------------------------------- Structures/Data Types */
/** Data type to represent IO Capabilities */
typedef struct
{
    /* Flag to decide if IO Cap is valid */
    UCHAR valid;

    /* IO Capability */
    UCHAR io_cap;

    /* OOB Authentication data present */
    UCHAR oob_present;

    /* Authentication Requirements */
    UCHAR auth_reqs;

} SM_IO_CAPS;


#ifdef BT_SSP_OOB
/** Data type to represent remote OOB data */
typedef struct
{
    /* Simple Pairing Hash 'C' */
    UCHAR ssp_c [16];

    /* Simple Pairing Randomizer 'R' */
    UCHAR ssp_r [16];

    /* Bluetooth Device Address */
    UCHAR bd_addr [BT_BD_ADDR_SIZE];

} SM_OOB_INFO;
#endif /* BT_SSP_OOB */


/* ----------------------------------------- Macros */


/* ----------------------------------------- API Declarations */
/** To return IO Capability to caller */
API_RESULT BT_sm_get_remote_io_cap
           (
               /* IN */  DEVICE_HANDLE    * device_handle,
               /* OUT */ SM_IO_CAPS *    io_cap
           );
#ifdef SM_IO_CAP_DYNAMIC
/**
 *  API to set local IO Capability from application.
 *
 *  IO Capability is not expected to change for a device runtime).
 *  Hence, usually local IO Capability will be defined as a constant.
 *  However, for testing SM under various conditions, there is a need
 *  to be able to change the IO Capability of the device at runtime.
 */
API_RESULT BT_sm_set_local_io_cap
           (
               /* IN */  UCHAR    io_cap
           );
#endif /* SM_IO_CAP_DYNAMIC */

#ifdef BT_SSP_OOB
/**
 *  API to register remote OOB data.
 */
API_RESULT BT_sm_set_device_oob_data
           (
               /* IN */  SM_OOB_INFO *    oob_info
           );
#endif /* BT_SSP_OOB */

#ifdef SM_AUTHREQ_DYNAMIC
/**
 *  API to set local Authentication Requirements for IOCAPS.
 */
API_RESULT BT_sm_set_local_authreq
           (
               /* IN */  UCHAR    valid,
               /* IN */  UCHAR    authreq
           );
#endif /* SM_AUTHREQ_DYNAMIC */

/**
 *  API to return IO Capability to caller
 */
API_RESULT BT_sm_get_remote_iocaps_pl
           (
               /* IN */  UCHAR      * bd_addr,
               /* OUT */ SM_IO_CAPS * io_caps
           );

/* ----------------------------------------- Function Declarations */
/** To return IO Capability to SM Core */
API_RESULT sm_get_io_capability_pl
           (
               /* IN */  UINT32          di,
               /* OUT */ SM_IO_CAPS *    io_cap
           );

#ifdef BT_SSP_OOB
/** To return Remote OOB Information to SM Core */
API_RESULT sm_get_remote_oob_data_pl
           (
               /* IN */  UINT32           di,
               /* OUT */ SM_OOB_INFO *    oob_info
           );
#endif /* BT_SSP_OOB */

#endif /* _H_SM_SSP_PL_ */

