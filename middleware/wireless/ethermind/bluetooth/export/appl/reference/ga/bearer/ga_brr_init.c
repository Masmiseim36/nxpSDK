
/**
 *  \file ga_brr_init.c
 *
 *  \brief This file defines the Generic Audio Bearer Init Interface - includes
 *  Data Structures and Methods.
 */

/**
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */


/* --------------------------------------------- Header File Inclusion */
#include "ga_brr_internal.h"

#ifdef BT_GAM

/* --------------------------------------------- Global Definitions */
/* TODO: Global GA Bearer variable */
GA_DEFINE_MUTEX(ga_brr_mutex)

/* --------------------------------------------- Data types /Structures */

/**
 * Common Bearer Callback to GA Core.
 */
GA_BRR_CB          ga_brr_cb;

/**
 * GA Bearer Profile Contexts
 */
GA_BRR_PROFILE_CNTX    ga_brr_profile_cntx[GA_BRR_MAX_PROFILE_CNTX];

/**
 * GA Bearer Service Contexts
 */
GA_BRR_SERVICE_CNTX    ga_brr_srvc_cntx[GA_BRR_MAX_SERVICE_CNTX];

/**
 * Include Service Contexts
 */
GA_BRR_INCLUDE_SERVICE_CNTX ga_brr_in_srvc_cntx[GA_BRR_MAX_INCLUDE_SERVICE_CNTX];

/**
 * GA Bearer Service UUID Contexts
 */
GA_SERVICE_UUID_CNTX ga_brr_srvc_uuid_cntx[GA_BRR_MAX_SERVICE_UUID_CNTX];

/**
 * GA Bearer Characteristic UUID Contexts
 */
GA_CHAR_UUID_CNTX ga_brr_char_uuid_cntx[GA_BRR_MAX_CHAR_UUID_CNTX];

/* GA Bearer initialization identifier */
DECL_STATIC UCHAR ga_brr_initialized = GA_FALSE;
/* --------------------------------------------- Macros */

/* --------------------------------------------- APIs */

/* GA Bearer Init */
GA_RESULT ga_brr_init(GA_BRR_CB cb)
{
    UCHAR i;

    GA_BRR_TRC
    ("[GA_BRR]: >> ga_brr_init\n");

    if (GA_FALSE != ga_brr_initialized)
    {
        GA_BRR_ERR
        ("[GA_BRR]: GA Bearer already Initialized\n");

        return GA_BRR_INVALID_STATE;
    }

    /* TODO: Initialize global GA Bearer mutex */
    /* BT_MUTEX_INIT(ga_brr_mutex, GA_BRR); */

    if (NULL == cb)
    {
        GA_BRR_ERR
        ("[GA_BRR]: Callback is NULL!\n");

        return GA_BRR_INVALID_PARAMS;
    }

    /**
     * Init all the internal data structures
     */
    GA_mem_set(&ga_brr_profile_cntx[0], 0, sizeof(ga_brr_profile_cntx));

    GA_mem_set(&ga_brr_srvc_cntx[0], 0, sizeof(ga_brr_srvc_cntx));
    for (i = 0; i < GA_BRR_MAX_SERVICE_CNTX; i++)
    {
        /* Set as Invalid Service Id */
        ga_brr_srvc_cntx[i].sid   = GA_BRR_INST_DONTCARE;
        ga_brr_srvc_cntx[i].s_cid = GA_BRR_INST_DONTCARE;
        ga_brr_srvc_cntx[i].e_cid = GA_BRR_INST_DONTCARE;
    }

    GA_mem_set(&ga_brr_in_srvc_cntx[0], 0, sizeof(ga_brr_in_srvc_cntx));
    for (i = 0; i < GA_BRR_MAX_INCLUDE_SERVICE_CNTX; i++)
    {
        /* Set as Invalid Include Service Id */
        ga_brr_in_srvc_cntx[i].sid  = GA_BRR_INST_DONTCARE;
        ga_brr_in_srvc_cntx[i].isid = GA_BRR_INST_DONTCARE;
    }

    GA_mem_set(&ga_brr_srvc_uuid_cntx[0], 0, sizeof(ga_brr_srvc_uuid_cntx));
    for (i = 0; i < GA_BRR_MAX_SERVICE_UUID_CNTX; i++)
    {
        /* Set as Invalid LastKnownInstanceId of Services */
        ga_brr_srvc_uuid_cntx[i].lkid = GA_BRR_INST_DONTCARE;
    }

    GA_mem_set(&ga_brr_char_uuid_cntx[0], 0, sizeof(ga_brr_char_uuid_cntx));
    for (i = 0; i < GA_BRR_MAX_CHAR_UUID_CNTX; i++)
    {
        /* Set as Invalid LastKnownInstanceId of Characteristics */
        ga_brr_char_uuid_cntx[i].lkid = GA_BRR_INST_DONTCARE;
    }

    /* Set the Callback */
    ga_brr_cb = cb;

    /* Set Initialized as TRUE */
    ga_brr_initialized = GA_TRUE;

    /* Call Bearer Internal Initialization */
    ga_brr_int_init();

    /* Call Bearer Platform Initialization */
    ga_brr_init_pl();

    GA_BRR_TRC (
    "[GA_BRR]: << brr_init, retval 0x%04X\n", GA_SUCCESS);

    return GA_SUCCESS;
}

/* GA Bearer Shutdown */
GA_RESULT ga_brr_shutdown(void)
{
    GA_BRR_TRC (
    "[GA_BRR]: >> ga_brr_shutdown\n");

    if (GA_TRUE != ga_brr_initialized)
    {
        return GA_BRR_INVALID_STATE;
    }

    ga_brr_cb = NULL;

    /* Set Initialized as TRUE */
    ga_brr_initialized = GA_FALSE;

    ga_brr_shutdown_pl();

    GA_BRR_TRC (
    "[GA_BRR]: << brr_shutdown, retval 0x%04X\n", GA_SUCCESS);

    return GA_SUCCESS;
}

#endif /* BT_GAM */
