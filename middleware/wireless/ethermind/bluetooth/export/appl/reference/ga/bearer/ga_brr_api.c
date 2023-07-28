
/**
 *  \file ga_brr_api.c
 *
 *  \brief This file defines the Generic Audio Bearer APIs - includes
 *  Data Structures and Methods.
 */

/**
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */


/* --------------------------------------------- Header File Inclusion */
#include "ga_brr_internal.h"
#include "ga_brr_extern.h"

#ifdef BT_GAM

/* --------------------------------------------- Global Definitions */
static const GA_BRR_INTERFACE brr =
{
    ga_brr_init,
    ga_brr_shutdown,
    ga_brr_add_service,
    ga_brr_include_service,
    ga_brr_remove_service,
    ga_brr_add_characteristic,
    ga_brr_manage_service,
    ga_brr_manage_characteristic,
    NULL, /* ga_brr_update_characteristic_value, */
    ga_brr_write_characteristic_rsp,
    ga_brr_read_characteristic_rsp,
    ga_brr_notify_characteristic,
    ga_brr_register_profile,
    ga_brr_unregister_profile,
    ga_brr_setup_transport,
    ga_brr_discover_service,
    ga_brr_get_service_composition,
    ga_brr_get_characteristic_composition,
    ga_brr_write_characteristic,
    ga_brr_read_characteristic,
    ga_brr_config_notification,
    ga_brr_config_adv,
    ga_brr_config_scan,
    ga_brr_config_sync,
    ga_brr_config_broadcast,
    ga_brr_config_broadcast_sync,
    ga_brr_dispatch_event
};

/* --------------------------------------------- Extern Definitions */

/* --------------------------------------------- Data types /Structures */

/* --------------------------------------------- Macros */

/* --------------------------------------------- APIs */
void ga_brr_register (void)
{
    GA_BRR_TRC (
    "[GA_BRR]: Registering Bearer interface\n");

    GA_brr_register((GA_BRR_INTERFACE *)&brr);
}

GA_RESULT ga_brr_register_profile
          (
              /* IN  */ UINT32 id,
              /* IN  */ GA_BRR_PROFILE_CB cb,
              /* OUT */ GA_BRR_PRF_HANDLE * pid
          )
{
    GA_RESULT   retval;

    retval = GA_SUCCESS;

    GA_BRR_TRC (
    "[GA_BRR]: >> ga_brr_register_profile\n");

    /* Param Check */
    if ((NULL == cb) ||
        (NULL == pid))
    {
        GA_BRR_ERR (
        "[GA_BRR]: Invalid Parameters!\n");

        return GA_BRR_INVALID_PARAMS;
    }

    /* TODO: Lock */

    retval = ga_brr_allocate_profile_cntx(pid);
    if (GA_SUCCESS == retval)
    {
        ga_brr_profile_cntx[*pid].pid = id;
        ga_brr_profile_cntx[*pid].pid_cb = cb;
    }

    /* TODO: UnLock */

    GA_BRR_TRC (
    "[GA_BRR]: << ga_brr_register_profile, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_unregister_profile
          (
              /* IN */ GA_BRR_PRF_HANDLE pid
          )
{

    GA_RESULT   retval;

    retval = GA_SUCCESS;

    GA_BRR_TRC (
    "[GA_BRR]: >> ga_brr_unregister_profile\n");

    /* Param Check */
    if (pid >= GA_BRR_MAX_PROFILE_CNTX)
    {
        GA_BRR_ERR (
        "[GA_BRR]: Invalid Parameters!\n");

        return GA_BRR_INVALID_PARAMS;
    }

    /* TODO: Lock */

    retval = ga_brr_free_profile_cntx(pid);

    /* TODO: UnLock */

    GA_BRR_TRC (
    "[GA_BRR]: << ga_brr_unregister_profile, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_add_service
          (
              /* IN  */ GA_BRR_UUID service_uuid,
              /* IN  */ UINT8 service_type,
              /* IN  */ GA_BRR_SERVICE_CB cb,
              /* OUT */ GA_BRR_SVC_INST * sid
          )
{
    GA_RESULT   retval;
    UCHAR       lkid, id_1, id_2;

    retval = GA_SUCCESS;

    GA_BRR_TRC (
    "[GA_BRR]: >> ga_brr_add_service\n");

    /* Param Check */
    if ((NULL == cb) ||
        (NULL == sid))
    {
        GA_BRR_ERR (
        "[GA_BRR]: Invalid Parameters!\n");

        return GA_BRR_INVALID_PARAMS;
    }

    /* TODO: Lock */

    /* Fetch the Last Known Service Instance of this UUID*/
    if (GA_SUCCESS != ga_brr_search_serv_uuid_cntx(service_uuid, &id_1))
    {
        lkid = GA_BRR_INST_DONTCARE;
    }
    else
    {
        lkid = ga_brr_srvc_uuid_cntx[id_1].lkid;
    }

    /* Fetch the Service ID from the Platform */
    retval = ga_brr_add_service_pl(service_uuid, service_type, lkid, sid);

    if (GA_SUCCESS != retval)
    {
        GA_BRR_ERR (
        "[GA_BRR]: Platform Service Addition Failed, retval 0x%04X\n", retval);
    }
    else
    {
        /* Allocate a Context for the found Service */
        retval = ga_brr_allocate_service_cntx(&id_2);

        if (GA_SUCCESS == retval)
        {
            ga_brr_srvc_cntx[id_2].sid_cb = cb;

            ga_brr_srvc_cntx[id_2].s_uuid = service_uuid;
            ga_brr_srvc_cntx[id_2].s_type = service_type;
            ga_brr_srvc_cntx[id_2].sid    = *sid;

            /* If previously lkid was unknown, then allocate
             * for a new Service UUID Context.
             */
            if (GA_BRR_INST_DONTCARE == lkid)
            {
                /* Allocate Service UUID Instance */
                retval = ga_brr_allocate_serv_uuid_cntx(service_uuid, &id_1);

                /** TODO: Check if validation to be done? */
            }

            if (GA_BRR_MAX_SERVICE_UUID_CNTX > id_1)
            {
                /* Update the last known Service UUID Instnace */
                ga_brr_srvc_uuid_cntx[id_1].lkid = *sid;
            }
        }
        else
        {
            GA_BRR_ERR (
            "[GA_BRR]: Service Context Allocation Failed, retval 0x%04X\n", retval);
        }
    }

    /* TODO: UnLock */

    GA_BRR_TRC (
    "[GA_BRR]: << ga_brr_add_service, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_remove_service
          (
              /* IN  */ GA_BRR_SVC_INST sid
          )
{
    GA_RESULT   retval;
    UCHAR       id;

    GA_BRR_TRC (
    "[GA_BRR]: >> ga_brr_remove_service\n");

    retval = GA_FAILURE;

    /* Search the Service Context for SID which is Primary Service */
    if (GA_SUCCESS == ga_brr_search_service_cntx(sid, GA_TRUE, &id))
    {
        /* Free Service UUID Context */
        retval = ga_brr_free_serv_uuid_cntx(ga_brr_srvc_cntx[id].s_uuid);

        /** TODO: Check how to free the Characterisitic UUID Context */

        /* Free the Service Context */
        retval = ga_brr_free_service_cntx(id);
    }
    /* Search the Included Service Context */
    else if (GA_SUCCESS == ga_brr_search_include_service_cntx(sid, &id))
    {
        /* Free Included Service UUID Context */
        retval = ga_brr_free_serv_uuid_cntx(ga_brr_in_srvc_cntx[id].is_uuid);

        /** TODO: Check how to free the Characterisitic UUID Context */

        /* Free the Included Service Context */
        retval = ga_brr_free_include_service_cntx(id);
    }

    GA_BRR_TRC (
    "[GA_BRR]: << ga_brr_remove_service, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_include_service
          (
              /* IN  */ GA_BRR_SVC_INST sid,
              /* IN  */ GA_BRR_UUID service_uuid,
              /* IN  */ UINT8 service_state,
              /* IN  */ GA_BRR_SVC_INST * isid
          )
{
    GA_RESULT   retval;
    UCHAR       lkid, id_1, id_2;

    GA_BRR_TRC (
    "[GA_BRR]: >> ga_brr_include_service\n");

    retval = GA_SUCCESS;

    /* Param Check */
    if ((sid >= GA_BRR_MAX_SERVICE_CNTX) ||
        (GA_BRR_UUID_INVALID == service_uuid) ||
        (NULL == isid))
    {
        GA_BRR_ERR (
        "[GA_BRR]: Invalid Parameters!\n");

        return GA_BRR_INVALID_PARAMS;
    }

    /* TODO: Lock */

    /**
     * TODO: Get sid from the GATT DB and pass the sid as in parameters
     *       for the ga_brr_allocate_service_cntx().
     *
     * For now keeping as is.
     */

    /* Fetch the Last Known Service Instance of this UUID*/
    if (GA_SUCCESS != ga_brr_search_serv_uuid_cntx(service_uuid, &id_1))
    {
        lkid = GA_BRR_INST_DONTCARE;
    }
    else
    {
        lkid = ga_brr_srvc_uuid_cntx[id_1].lkid;
    }

    /* Fetch the Service ID from the Platform */
    retval = ga_brr_include_service_add_pl
             (
                 sid,
                 service_uuid,
                 service_state,
                 lkid,
                 isid
             );

    /* Allocate a Context for this Included Service */
    retval = ga_brr_allocate_include_service_cntx(&id_2);
    if (GA_SUCCESS == retval)
    {
        ga_brr_in_srvc_cntx[id_2].is_uuid = service_uuid;
        ga_brr_in_srvc_cntx[id_2].sid     = sid;
        ga_brr_in_srvc_cntx[id_2].isid    = *isid;

       /* If previously lkid was unknown, then allocate
        * for a new Service UUID Context.
        */
        if (GA_BRR_INST_DONTCARE == lkid)
        {
            /* Allocate Service UUID Instance */
            retval = ga_brr_allocate_serv_uuid_cntx(service_uuid, &id_1);

            /** TODO: Check if validation to be done? */
        }

        if (GA_BRR_MAX_SERVICE_UUID_CNTX > id_1)
        {
            /* Update the last known Service UUID Instnace */
            ga_brr_srvc_uuid_cntx[id_1].lkid = *isid;
        }
    }

    /* TODO: UnLock */

    GA_BRR_TRC (
    "[GA_BRR]: << ga_brr_include_service, retval 0x%04X\n", retval);

    return retval;

}

GA_RESULT ga_brr_add_characteristic
          (
              /* IN  */ GA_BRR_SVC_INST sid,
              /* IN  */ GA_BRR_UUID     char_uuid,
              /* IN  */ UINT16          char_property,
              /* IN  */ GA_BRR_CHR_VAL  * value,
              /* OUT */ GA_BRR_CHR_INST * cid
          )
{
    GA_RESULT retval;
    UCHAR     id_1, id_2, lkid, temp_cid;

    retval = GA_SUCCESS;

    GA_BRR_TRC(
    "[GA_BRR]: >> ga_brr_add_characteristic\n");

    /* TODO: Lock */

    /* Fetch the Last Known Characteristic Instance of this UUID */
    if (GA_SUCCESS != ga_brr_search_char_uuid_cntx(char_uuid, &id_1))
    {
        lkid = GA_BRR_INST_DONTCARE;
    }
    else
    {
        lkid = ga_brr_char_uuid_cntx[id_1].lkid;
    }

    /* Fetch the Characteristic ID from the Platform */
    retval = ga_brr_add_characteristic_pl
             (
                 sid,
                 char_uuid,
                 char_property,
                 value,
                 lkid,
                 &temp_cid
             );

    if (GA_SUCCESS != retval)
    {
        GA_BRR_ERR (
        "[GA_BRR]: Platform Characteristic Addition Failed, retval 0x%04X\n", retval);
    }
    else
    {
        /*
         * Search the Service Context for SID which could be either
         * Primary Service or Secondary(included) Service
         */
        if (GA_SUCCESS == ga_brr_search_service_cntx(sid, GA_FALSE, &id_2))
        {
            /*
             * Update the Characteristic Start Instance and End
             * Instance if this is the first Char, else
             * update the End Instance
             */
            if (GA_BRR_INST_DONTCARE == ga_brr_srvc_cntx[id_2].s_cid)
            {
                /* Store the Characterisitc Start Index */
                ga_brr_srvc_cntx[id_2].s_cid = temp_cid;
            }

            /* Store the Characteristic End Index */
            ga_brr_srvc_cntx[id_2].e_cid = temp_cid;

            /* If previously lkid was unknown, then allocate
             * for a new Service UUID Context.
             */
            if (GA_BRR_INST_DONTCARE == lkid)
            {
                /* Allocate Characteristic UUID Instance */
                retval = ga_brr_allocate_char_uuid_cntx(char_uuid, &id_1);

                /** TODO: Check if validation to be done? */
            }

            if (GA_BRR_MAX_SERVICE_UUID_CNTX > id_1)
            {
                /* Update the last knon Characterisitc UUID Instnace */
                ga_brr_char_uuid_cntx[id_1].lkid = temp_cid;
            }

            if (NULL != cid)
            {
                *cid = temp_cid;
            }
        }
        else
        {
            GA_BRR_ERR (
            "[GA_BRR]: Unknown Service ID Failed to add Char, retval 0x%04X\n", retval);
        }
    }

    /* TODO: UnLock */

    GA_BRR_TRC (
    "[GA_BRR]: << ga_brr_add_characteristic, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_manage_service
          (
              /* IN  */ UINT8 sid,
              /* IN  */ UINT8 iid,
              /* IN  */ UINT8 state
          )
{
    GA_RESULT retval;

    retval = GA_SUCCESS;

    GA_BRR_TRC(
    "[GA_BRR]: >> ga_brr_manage_service\n");

    /* TODO: Lock */

    retval = ga_brr_manage_service_pl(sid, iid, state);

    if (GA_SUCCESS != retval)
    {
        GA_BRR_ERR (
        "[GA_BRR]: Platform Service Manage Failed, retval 0x%04X\n", retval);
    }

    /* TODO: UnLock */

    GA_BRR_TRC(
    "[GA_BRR]: << ga_brr_manage_service, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_manage_characteristic
          (
              /* IN  */ UINT8 sid,
              /* IN  */ UINT8 cid,
              /* IN  */ UINT8 state
          )
{
    GA_RESULT retval;

    retval = GA_SUCCESS;

    GA_BRR_TRC(
    "[GA_BRR]: >> ga_brr_manage_characteristic\n");

    /* TODO: Lock */

    retval = ga_brr_manage_characteristic_pl(sid, cid, state);

    if (GA_SUCCESS != retval)
    {
        GA_BRR_ERR (
        "[GA_BRR]: Platform Characteristic Manage Failed, retval 0x%04X\n", retval);
    }

    /* TODO: UnLock */

    GA_BRR_TRC(
    "[GA_BRR]: << ga_brr_manage_characteristic, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_write_characteristic_rsp
          (
              /* IN */ GA_BRR_DEVICE      * device,
              /* IN */ GA_BRR_CHR_CONTEXT * ctx,
              /* IN */ UINT16             status
          )
{
    GA_RESULT retval;

    retval = GA_SUCCESS;

    GA_BRR_TRC (
    "[GA_BRR]: >> ga_brr_write_characteristic_rsp\n");

    /** Call Platform Abstraction here */
    retval = ga_brr_write_characteristic_rsp_pl
             (
                 device,
                 ctx,
                 status
             );

    GA_BRR_TRC (
    "[GA_BRR]: << ga_brr_write_characteristic_rsp, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_read_characteristic_rsp
          (
              /* IN */ GA_BRR_DEVICE      * device,
              /* IN */ GA_BRR_CHR_CONTEXT * ctx,
              /* IN */ UINT16             status,
              /* IN */ GA_BRR_CHR_VAL     * value
          )
{
    GA_RESULT retval;

    retval = GA_SUCCESS;

    GA_BRR_TRC (
    "[GA_BRR]: >> ga_brr_read_characteristic_rsp\n");

    /** Call Platform Abstraction here */
    retval = ga_brr_read_characteristic_rsp_pl
             (
                 device,
                 ctx,
                 status,
                 value
             );

    GA_BRR_TRC (
    "[GA_BRR]: << ga_brr_read_characteristic_rsp, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_notify_characteristic
          (
              /* IN */ GA_BRR_DEVICE   * device,
              /* IN */ GA_BRR_SVC_INST sid,
              /* IN */ GA_BRR_UUID     char_uuid,
              /* IN */ GA_BRR_CHR_INST cid,
              /* IN */ GA_BRR_CHR_VAL  * value,
              /* IN */ UINT8           nwr
          )
{
    GA_RESULT    retval;

    GA_BRR_TRC (
    "[GA_BRR]: >> ga_brr_notify_characteristic\n");

    retval = GA_SUCCESS;

    /** Call Platform Abstraction here */
    retval = ga_brr_notify_characteristic_pl
             (
                 device,
                 sid,
                 char_uuid,
                 cid,
                 value,
                 nwr
             );

    GA_BRR_TRC (
    "[GA_BRR]: << ga_brr_notify_characteristic, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_setup_transport
          (
              /* IN */ GA_BRR_PRF_HANDLE pid,
              /* IN */ UINT8 type,
              /* IN */ UINT8 enable
          )
{
    GA_RESULT  retval;

    GA_IGNORE_UNUSED_PARAM(pid);
    GA_IGNORE_UNUSED_PARAM(type);
    GA_IGNORE_UNUSED_PARAM(enable);

    GA_BRR_TRC (
    "[GA_BRR]: >> ga_brr_setup_transport\n");

    retval = GA_SUCCESS;

    GA_BRR_TRC (
    "[GA_BRR]: << ga_brr_setup_transport, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_discover_service
          (
              /* IN */ GA_BRR_PRF_HANDLE pid,
              /* IN */ GA_BRR_DEVICE     * device,
              /* IN */ GA_BRR_SVC_INFO   * svc_info,
              /* IN */ UCHAR             disc_type,
              /* IN */ void              * context
          )
{
    GA_RESULT    retval;

    GA_BRR_TRC (
    "[GA_BRR]: >> ga_brr_discover_service\n");

    retval = GA_SUCCESS;

    /* Call the Platform Abstraction for Discovery */
    retval = ga_brr_discover_service_pl
             (
                 pid,
                 device,
                 svc_info,
                 disc_type,
                 context
             );

    GA_BRR_TRC (
    "[GA_BRR]: << ga_brr_discover_service, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_get_service_composition
          (
              /* IN */ GA_BRR_PRF_HANDLE   pid,
              /* IN */ GA_BRR_DEVICE     * device,
              /* IN */ GA_BRR_SVC_INFO   * service,
              /* IN */ void              * context
          )
{
    GA_RESULT    retval;

    GA_BRR_TRC (
    "[GA_BRR]: >> ga_brr_get_service_composition\n");

    retval = ga_brr_get_service_composition_pl
             (
                 pid,
                 device,
                 service,
                 context
             );

    GA_BRR_TRC (
    "[GA_BRR]: << ga_brr_get_service_composition, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_get_characteristic_composition
          (
              /* IN */ GA_BRR_PRF_HANDLE   pid,
              /* IN */ GA_BRR_DEVICE     * device,
              /* IN */ GA_BRR_CHAR_RANGE * characteristic,
              /* IN */ void              * context
          )
{
    GA_RESULT    retval;

    GA_BRR_TRC (
    "[GA_BRR]: >> ga_brr_get_characteristic_composition\n");

    retval = ga_brr_get_characteristic_composition_pl
             (
                 pid,
                 device,
                 characteristic,
                 context
             );

    GA_BRR_TRC (
    "[GA_BRR]: << ga_brr_get_characteristic_composition, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_read_characteristic
          (
              /* IN */ GA_BRR_PRF_HANDLE pid,
              /* IN */ GA_BRR_DEVICE     * device,
              /* IN */ GA_BRR_CHR_HANDLE characteristic,
              /* IN */ void              * context
          )
{
    GA_RESULT    retval;

    GA_BRR_TRC (
    "[GA_BRR]: >> ga_brr_read_characteristic\n");

    retval = GA_SUCCESS;

    retval = ga_brr_read_characteristic_pl
             (
                 pid,
                 device,
                 characteristic,
                 context
             );

    GA_BRR_TRC (
    "[GA_BRR]: << ga_brr_read_characteristic, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_write_characteristic
          (
              /* IN */ GA_BRR_PRF_HANDLE pid,
              /* IN */ GA_BRR_DEVICE     * device,
              /* IN */ GA_BRR_CHR_HANDLE characteristic,
              /* IN */ GA_BRR_CHR_VAL    * value,
              /* IN */ UINT8             wwr,
              /* IN */ void              * context
          )
{
    GA_RESULT    retval;

    GA_BRR_TRC (
    "[GA_BRR]: >> ga_brr_write_characteristic\n");

    retval = ga_brr_write_characteristic_pl
             (
                 pid,
                 device,
                 characteristic,
                 value,
                 wwr,
                 context
             );

    GA_BRR_TRC (
    "[GA_BRR]: << ga_brr_write_characteristic, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_config_notification
          (
              /* IN */ GA_BRR_PRF_HANDLE pid,
              /* IN */ GA_BRR_DEVICE     * device,
              /* IN */ GA_BRR_CHR_HANDLE characteristic,
              /* IN */ UINT8             state,
              /* IN */ void              * context
          )
{
    GA_RESULT      retval;

    GA_BRR_TRC (
    "[GA_BRR]: >> ga_brr_config_notification\n");

    retval = ga_brr_config_notification_pl
             (
                 pid,
                 device,
                 characteristic,
                 state,
                 context
             );

    GA_BRR_TRC (
    "[GA_BRR]: << ga_brr_config_notification, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_dispatch_event
          (
              /* IN */ GA_BRR_PRF_HANDLE        pid,
              /* IN */ GA_BRR_DEVICE           *device,
              /* IN */ GA_BRR_EVENT_PARAMS     *event_params,
              /* IN */ void                    *context
          )
{
    GA_RESULT retval;

    GA_BRR_TRC(
    "[GA_BRR]: >> ga_brr_dispatch_event\n");

    retval = GA_FAILURE;

    /* Check if the received PID is within Range */
    if (pid < GA_BRR_PROFILE_ID_INVALID)
    {
        if (NULL != ga_brr_profile_cntx[pid].pid_cb)
        {
            retval = ga_brr_dispatch_event_pl
                     (
                         pid,
                         device,
                         event_params,
                         context
                     );
        }
    }

    GA_BRR_TRC(
    "[GA_BRR]: << ga_brr_dispatch_event\n");

    return retval;
}

#endif /* BT_GAM */