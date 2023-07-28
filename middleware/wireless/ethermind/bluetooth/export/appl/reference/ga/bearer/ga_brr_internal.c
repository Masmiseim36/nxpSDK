
/**
*  \file ga_brr_internal.c
*
*  \brief This file defines the Generic Audio Bearer Internal routines,
*  Data Structures and Methods.
*/

/*
*  Copyright (C) 2020. Mindtree Ltd.
*  All rights reserved.
*/


/* --------------------------------------------- Header File Inclusion */
/* GA Bearer Related Includes */
#include "ga_brr_internal.h"
#include "ga_brr_extern.h"

#ifdef BT_GAM

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */

/* --------------------------------------------- Functions */
/** Get the profile ID for the module */
GA_RESULT ga_brr_get_module_pid(UINT32 id, GA_BRR_PRF_HANDLE * pid)
{
    GA_RESULT retval;
    UINT8 i;

    /* Init */
    retval = GA_FAILURE;
    *pid = GA_BRR_PROFILE_ID_INVALID;

    for (i = 0; i < GA_BRR_MAX_PROFILE_CNTX; i++)
    {
        if ((id == ga_brr_profile_cntx[i].pid) &&
            (NULL != ga_brr_profile_cntx[i].pid_cb))
        {
            *pid = i;
            retval = GA_SUCCESS;
            break;
        }
    }

    return retval;
}

/** GA Bearer Internal Initialization */
GA_RESULT ga_brr_int_init(void)
{
    GA_RESULT retval;

    retval = GA_SUCCESS;

    GA_BRR_TRC
    ("[GA_BRR]: >> ga_brr_register_int_cb\n");

    ga_brr_register_pl_cb(ga_brr_int_cb);

    GA_BRR_TRC (
    "[GA_BRR]: << ga_brr_register_int_cb, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_int_cb
          (
              /* IN */ GA_BRR_DEVICE * device,
              /* IN */ void          * context,
              /* IN */ UCHAR         brr_role,
              /* IN */ void          * inst_handle,
              /* IN */ UINT8         event_type,
              /* IN */ UINT16        event_status,
              /* IN */ void          * event_data,
              /* IN */ UINT16        event_datalen
          )
{
    GA_RESULT         retval;

    retval = GA_SUCCESS;

    if (NULL != device)
    {
        GA_BRR_TRC(
            "[GA_BRR]: >> ga_brr_int_cb, for Role 0x%02X with BRR DEV "
            GA_BRR_DEVICE_FRMT_SPECIFIER "\n", GA_BRR_DEVICE_PRINT_STR(device),
            brr_role);
    }

    /* Param Check */
    if (NULL == inst_handle)
    {
        GA_BRR_ERR (
        "[GA_BRR]: Invalid Parameters! Dropping the event!\n");

        return GA_BRR_INVALID_PARAMS;
    }

    /**
     * Dereference the Instance handle based on the Role.
     */
    switch(brr_role)
    {
        case GA_BRR_PROFILE_ROLE:
            {
                GA_BRR_PRF_HANDLE pid;
                GA_RESULT         ret;
                UCHAR             i;

                pid = *((GA_BRR_PRF_HANDLE *)inst_handle);

                GA_BRR_TRC (
                "[GA_BRR]: PID:0x%02X Event_Type:0x%02X Event_Status 0x%04X\n",
                pid, event_type, event_status);

                /* Check if the received PID is within Range */
                if (pid < GA_BRR_PROFILE_ID_INVALID)
                {
                    if (NULL != ga_brr_profile_cntx[pid].pid_cb)
                    {
                        /* Notify Upper Layer */
                        ret = ga_brr_profile_cntx[pid].pid_cb
                              (
                                  device,
                                  context,
                                  event_type,
                                  event_status,
                                  event_data,
                                  event_datalen
                              );
                    }
                }
                else
                {
                    /**
                     * Since the profile ID is unknown.
                     * Pass this event to all registred profiles
                     */
                    for (i = 0; i < GA_BRR_MAX_PROFILE_CNTX; i++)
                    {
                        if (NULL != ga_brr_profile_cntx[i].pid_cb)
                        {
                            /* Notify Upper Layer */
                            ret = ga_brr_profile_cntx[i].pid_cb
                                  (
                                      device,
                                      context,
                                      event_type,
                                      event_status,
                                      event_data,
                                      event_datalen
                                  );

                            if (GA_EVT_CONSUMED == ret)
                            {
                                GA_BRR_TRC (
                                "[GA_BRR]: Event_Type:0x%02X consumed by PID:0x%02X\n",
                                event_type, i);

                                /* Break the Look as this event in consumed */
                                break;
                            }
                        }
                    }
                }
            }
            break;

        case GA_BRR_SERVICE_ROLE:
            {
                GA_BRR_SVC_INST sid;
                UCHAR           id;

                sid = *((GA_BRR_SVC_INST *)inst_handle);

                GA_BRR_TRC (
                "[GA_BRR]: SID:0x%02X Event_Type:0x%02X Event_Status 0x%04X\n",
                sid, event_type, event_status);

                if ((sid != GA_BRR_INST_DONTCARE) &&
                    (GA_SUCCESS == ga_brr_search_service_cntx(sid, GA_FALSE, &id)))
                {
                    if (NULL != ga_brr_srvc_cntx[id].sid_cb)
                    {
                        /* Notify Upper Layer */
                        ga_brr_srvc_cntx[id].sid_cb
                        (
                            device,
                            context,
                            event_type,
                            event_status,
                            event_data,
                            event_datalen
                        );
                    }
                }
                else
                {
                    /*
                     * Service not registered, hence no context found for this
                     * service, Send Error Response for any request on this
                     * service.
                     */
                    retval = GA_FAILURE;
                }
            }
            break;

        case GA_BRR_GENERIC_ROLE:
            {
                GA_BRR_TRC (
                "[GA_BRR]: Event_Type:0x%02X Event_Status 0x%04X for Generic role\n",
                event_type, event_status);

                if (NULL != ga_brr_cb)
                {
                    ga_brr_cb
                    (
                        device,
                        event_type,
                        event_status,
                        event_data,
                        event_datalen
                    );
                }
            }
            break;
        default:
            GA_BRR_ERR (
            "[GA_BRR]: Event received for Unknown Role 0x%02X\n", brr_role);
            break;
    }

    GA_BRR_TRC (
    "[GA_BRR]: << ga_brr_int_cb, retval 0x%04X\n", retval);

    return retval;
}

void ga_brr_dispatch_event_handler
     (
         /* IN */ GA_BRR_PRF_HANDLE        pid,
         /* IN */ GA_BRR_DEVICE           *device,
         /* IN */ GA_BRR_EVENT_PARAMS     *event_params,
         /* IN */ void                    *context
     )
{
    ga_brr_profile_cntx[pid].pid_cb
    (
        device,
        context,
        event_params->event_type,
        event_params->event_status,
        event_params->event_data,
        event_params->event_datalen
    );
}

#endif /* BT_GAM */
