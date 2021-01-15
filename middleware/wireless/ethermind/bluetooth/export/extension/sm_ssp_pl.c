
/**
 *  \file sm_ssp_pl.c
 *
 *  This file contains the implementation for all platform specific
 *  extensions for Secure Simple Pairing feature for Security Manager
 *  module.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "sm_internal.h"
#include "sm_extern.h"

#ifdef BT_SSP

/* ----------------------------------------- External Global Variables */


/* ----------------------------------------- Exported Global Variables */


/* ----------------------------------------- Static Global Variables */
#ifdef SM_IO_CAP_DYNAMIC
/** Local IO Capability */
DECL_STATIC UCHAR sm_local_io_cap = SM_IO_CAPABILITY_DEFAULT;
#endif /* SM_IO_CAP_DYNAMIC */

#ifdef BT_SSP_OOB
/**
 *  Remote OOB Data
 *
 *  OOB exchange is expected to happen just before Simple Pairing
 *  (or, Bonding) is triggered. Hence, only one device's OOB
 *  information need to be stored at any point of time.
 */
DECL_STATIC SM_OOB_INFO sm_oob_info;
#endif /* BT_SSP_OOB */


/* ----------------------------------------- API Functions */
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
           )
{
    /* Check parameters */
    if (io_cap > SM_IO_CAPABILITY_NO_INPUT_NO_OUTPUT)
    {
        SM_ERR(
        "[SM] Invalid Device IO Capability = 0x%02X\n",
        io_cap);

        return SM_INVALID_PARAMETERS;
    }

    /* Lock SM */
    sm_lock();

    /* Store IO Cap */
    sm_local_io_cap = io_cap;

    SM_TRC(
    "[SM] Updated Local IO Capability = 0x%02X\n",
    sm_local_io_cap);

    /* Unlock SM */
    sm_unlock();

    return API_SUCCESS;
}
#endif /* SM_IO_CAP_DYNAMIC */


#ifdef BT_SSP_OOB
/**
 *  API to register remote OOB data.
 */
API_RESULT BT_sm_set_device_oob_data
           (
               /* IN */  SM_OOB_INFO *    oob_info
           )
{
    /* Lock SM */
    sm_lock();

    /* Store Remote OOB Information */
    sm_oob_info = *oob_info;

    SM_TRC(
    "[SM] Updated OOB Info. "BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER "\n",
    BT_DEVICE_ADDR_ONLY_PRINT_STR(sm_oob_info.bd_addr));

#ifdef SEC_DEBUG
    SM_TRC(
    "[SM] Device Simple Pairing Hash 'C':\n");
    SM_debug_dump_bytes (sm_oob_info.ssp_c, 16);

    SM_TRC(
    "[SM] Device Simple Pairing Randomizer 'R':\n");
    SM_debug_dump_bytes (sm_oob_info.ssp_r, 16);
#endif /* SEC_DEBUG */

    /* Unlock SM */
    sm_unlock();

    return API_SUCCESS;
}
#endif /* BT_SSP_OOB */


/* ----------------------------------------- Internal Functions */
/** To return IO Capability to SM Core */
API_RESULT sm_get_io_capability_pl
           (
               /* IN */  UINT32          di,
               /* OUT */ SM_IO_CAPS *    io_cap
           )
{
    UINT32 si;
    UCHAR mitm_reqd, bonding;
    BT_DEVICE_ADDR device_addr;

#ifdef BT_BRSC
    UCHAR sc_only_mode;
#endif /* BT_BRSC */

    device_queue_get_remote_addr (&sm_devices[di].device_handle,&device_addr);

#ifdef SM_IO_CAP_DYNAMIC
    /* Set Local IO Cap */
    io_cap->io_cap = sm_local_io_cap;
#else  /* SM_IO_CAP_DYNAMIC */
    /* Set Local IO Cap */
    io_cap->io_cap = SM_IO_CAPABILITY_DEFAULT;
#endif /* SM_IO_CAP_DYNAMIC */

#ifdef BT_SSP_OOB
    /* Set Local OOB */
    if (0 == BT_mem_cmp(sm_oob_info.bd_addr, BT_BD_ADDR(&device_addr), BT_BD_ADDR_SIZE))
    {
        io_cap->oob_present = 0x1;
    }
    else
    {
        io_cap->oob_present = 0x0;
    }
#else  /* BT_SSP_OOB */
    io_cap->oob_present = 0x0;
#endif /* BT_SSP_OOB */

    /*
     *  Is this locally or remotely initiated Authentication?
     *  We know this by looking at whether remote IO Capability Response
     *  has already been received by the local side. If we have remote
     *  IO Capability already with us, then this is remotely initiated
     *  Authentication.
     */
    mitm_reqd = 0x2;

#ifdef BT_BRSC
    /* Unlock SM */
    sm_unlock();

    BT_sm_get_secure_connections_only_mode(&sc_only_mode);

    /* Lock SM */
    sm_lock();
#endif /* BT_BRSC */

    if (SM_TRUE == sm_devices[di].remote_io_cap.valid)
    {
        /* => Remotely Initiated Authentication */
        SM_INF(
        "[SM] Remote IO Cap Present. Remotely Initiated Authentication\n");

        /*
         *  For remotely initiated Authentication, we reply the same for
         *  Bonding requirements, but MITM bit is set according to wishes
         *  of the locally available services.
         *
         *  Authentication_Requirements:
         *    0x00: MITM Protection Not Required - No Bonding.
         *    0x01: MITM Protection Required - No Bonding.
         *    0x02: MITM Protection Not Required - Dedicated Bonding.
         *    0x03: MITM Protection Required - Dedicated Bonding.
         *    0x04: MITM Protection Not Required - General Bonding.
         *    0x05: MITM Protection Required - General Bonding.
         *
         *  => Bit   0: MITM Protection
         *              0 - MITM Protection Not Required
         *              1 - MITM Protection Required
         *  => Bit 1-2: Bonding
         *              00 - No Bonding
         *              01 - Dedicated Bonding
         *              10 - General Bonding
         */
        bonding = (sm_devices[di].remote_io_cap.auth_reqs & 0x06);
    }
    else
    {
        /* => Locally Initiated Authentication */
        SM_INF(
        "[SM] Remote IO Cap NOT Present. Locally Initiated Authentication\n");

        /* Is Authentication initiated for Dedicated or General Bonding? */
        if (SM_MAX_SERVICES == sm_authentication_service)
        {
            SM_INF(
            "[SM] Authentication Initiated for Dedicated Bonding\n");

            /* Set Dedicated Bonding */
            bonding = 0x02;
        }
        else
        {
            SM_INF(
            "[SM] Authentication Initiated for General Bonding\n");

            /* Set General Bonding, or No Bonding */
            if (0x01 == SM_GET_SERVICE_NO_BONDING(sm_authentication_service))
            {
                /* Set No Bonding */
                bonding = 0x0;
            }
            else
            {
                /* Set General Bonding */
                bonding = 0x04;
            }

#ifndef BT_BRSC
            /* Set if MITM Required */
            mitm_reqd =
            (SM_SERVICE_SECURITY_LEVEL_3 == SM_GET_SERVICE_AUTHENTICATION(sm_authentication_service));
#else
            if ((BT_FALSE != sc_only_mode) ||
                (SM_SERVICE_SECURITY_LEVEL_4 == SM_GET_SERVICE_AUTHENTICATION(sm_authentication_service)) ||
                (SM_SERVICE_SECURITY_LEVEL_3 == SM_GET_SERVICE_AUTHENTICATION(sm_authentication_service)))
            {
                mitm_reqd = 0x01;
            }
            else
            {
                mitm_reqd = 0x00;
            }
#endif /* BT_BRSC */
        }
    }

    if (0x02 == mitm_reqd)
    {
        /*
         *  If Locally or Remotely initiated Dedicated Bonding,
         *  decide whether MITM required or not, based upon whether
         *  there exists a service requiring MITM protection.
         */
        mitm_reqd = 0x0;
        for (si = 0; si < SM_MAX_SERVICES; si ++)
        {
#ifndef BT_BRSC
            if (SM_SERVICE_SECURITY_LEVEL_3 == SM_GET_SERVICE_AUTHENTICATION(si))
#else
            if ((BT_FALSE != sc_only_mode) ||
                (SM_SERVICE_SECURITY_LEVEL_4 == SM_GET_SERVICE_AUTHENTICATION(sm_authentication_service)) ||
                (SM_SERVICE_SECURITY_LEVEL_3 == SM_GET_SERVICE_AUTHENTICATION(sm_authentication_service)))
#endif /* BT_BRSC */
            {
                mitm_reqd = 0x1;
                break;
            }
        }

    #ifdef SEC_DEBUG
        if (0x1 == mitm_reqd)
        {
            SM_INF(
            "[SM] Found MITM Protection Required for Service 0x%02X\n",
            si);
        }
    #endif /* SEC_DEBUG */
    }

    /*
     *  Set Local Authentication Requirements.
     */
    io_cap->auth_reqs = mitm_reqd | bonding;

    SM_INF(
    "[SM] Local IO Cap Generated: IO Cap %02X, OOB %02X, AuthReqs %02X\n",
    io_cap->io_cap, io_cap->oob_present, io_cap->auth_reqs);

    return API_SUCCESS;
}


#ifdef BT_SSP_OOB
/** To return Remote OOB Information to SM Core */
API_RESULT sm_get_remote_oob_data_pl
           (
               /* IN */  UINT32           di,
               /* OUT */ SM_OOB_INFO *    oob_info
           )
{
    API_RESULT retval;
    DEVICE_HANDLE handle;

    sm_get_device_handle (sm_oob_info.bd_addr,&handle);

    /*
     *  It is expected that the application would have set
     *  the Remote OOB data by the time this request is received.
     */
    if (handle == sm_devices[di].device_handle)
    {
        *oob_info = sm_oob_info;
        retval = API_SUCCESS;
    }
    else
    {
        retval = API_FAILURE;
    }

    return retval;
}
#endif /* BT_SSP_OOB */

#endif /* BT_SSP */

