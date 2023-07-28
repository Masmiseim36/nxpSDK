
/**
 *  \file ga_brr_utils.c
 *
 *  \brief This file defines the Generic Audio Bearer Utility Interface - includes
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

/* --------------------------------------------- Data types /Structures */

/* --------------------------------------------- Macros */

/* --------------------------------------------- APIs */

/** Profile Registration and Callback Context */
GA_RESULT ga_brr_allocate_profile_cntx
          (
              /* OUT */ GA_BRR_PRF_HANDLE * pid
          )
{
    UCHAR i;
    GA_RESULT  retval;

    GA_BRR_TRC (
    "[GA_BRR]: >> ga_brr_allocate_profile_cntx\n");

    /* Param Check */
    if (NULL == pid)
    {
        GA_BRR_ERR (
        "[GA_BRR]: Invalid Parameters!\n");

        return  GA_BRR_INVALID_PARAMS;
    }

    /* Init */
    retval = GA_FAILURE;

    for (i = 0; i < GA_BRR_MAX_PROFILE_CNTX; i++)
    {
        if (NULL ==  ga_brr_profile_cntx[i].pid_cb)
        {
            *pid = i;

            /* ga_pfile_cntx[i].pid = i; */
            retval = GA_SUCCESS;

            break;
        }
    }

    GA_BRR_TRC (
    "[GA_BRR]: << ga_brr_allocate_profile_cntx, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_free_profile_cntx
          (
              /* IN */ GA_BRR_PRF_HANDLE pid
          )
{
    GA_BRR_TRC (
    "[GA_BRR]: >> ga_brr_free_profile_cntx\n");

    /* Param Check */
    if (pid >= GA_BRR_MAX_PROFILE_CNTX)
    {
        GA_BRR_ERR (
        "[GA_BRR]: Invalid Parameters!\n");

        return  GA_FAILURE;
    }

    /* Reset */
    /* ga_pfile_cntx[pid].pid = GA_PROFILE_ID_INVALID; */
    ga_brr_profile_cntx[pid].pid_cb = NULL;

    GA_BRR_TRC (
    "[GA_BRR]: << ga_brr_free_profile_cntx, retval 0x%04X\n", GA_SUCCESS);

    return GA_SUCCESS;
}

GA_RESULT ga_brr_search_service_cntx
          (
              /* IN  */ GA_BRR_SVC_INST sid,
              /* IN  */ UCHAR           only_ps,
              /* OUT */ UCHAR           * id
          )
{
    UINT32          i;
    GA_RESULT       retval;

    retval = GA_FAILURE;

    GA_BRR_TRC(
    "[GA_BRR]: >> ga_brr_search_service_cntx\n");

    /* Search the included service context */
    if (GA_FALSE == only_ps)
    {
        retval = ga_brr_search_include_service_cntx(sid, id);

        /**
         * Found the Included Service. Now find the sid of the Primary Service
         * which this Included Service is associated with
         */
        if (GA_SUCCESS == retval)
        {
            GA_BRR_TRC(
            "[GA_BRR]: Included service with ID %0x02X found @ 0x502X\n",
            sid, *id);

            sid = ga_brr_in_srvc_cntx[*id].isid;

            GA_BRR_INF(
            "[GA_BRR]: Overwriting the incoming sid to 0x%02X\n", sid);

            GA_BRR_TRC(
            "[GA_BRR]: Primary Service ID associated with this Included "
            "service is 0x%02X\n", sid);
        }
    }

    *id = GA_BRR_MAX_SERVICE_CNTX;

    /**
     * First treat the incoming sid as Primary Service ID and look
     * for a corresponding primary service context.
     */
    for (i = 0; i < GA_BRR_MAX_SERVICE_CNTX; i++)
    {
        if (ga_brr_srvc_cntx[i].sid == sid)
        {
            *id = (UCHAR)i;
            retval = GA_SUCCESS;

            break;
        }
    }

    GA_BRR_TRC (
    "[GA_BRR]: << ga_brr_search_service_cntx, retval 0x%04X\n", retval);

    return retval;
}

/** Service Registration and Callback Context */
GA_RESULT ga_brr_allocate_service_cntx
          (
             /* OUT */ UCHAR * id
          )
{
    GA_RESULT  retval;
    UCHAR i;

    GA_BRR_TRC (
    "[GA_BRR]: >> ga_brr_allocate_service_cntx\n");

    /* Param Check */
    if (NULL == id)
    {
        GA_BRR_ERR (
        "[GA_BRR]: Invalid Parameters!\n");

        return  GA_BRR_INVALID_PARAMS;
    }

    /* Init */
    retval = GA_FAILURE;

    for (i = 0; i < GA_BRR_MAX_SERVICE_CNTX; i++)
    {
        if (NULL == ga_brr_srvc_cntx[i].sid_cb)
        {
            *id = i;
            retval = GA_SUCCESS;

            break;
        }
    }

    GA_BRR_TRC (
    "[GA_BRR]: << ga_brr_allocate_service_cntx, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_free_service_cntx
          (
              /* IN */ UCHAR id
          )
{
    GA_BRR_TRC (
    "[GA_BRR]: >> ga_brr_free_service_cntx for id 0x%02X\n", id);

    /* Param Check */
    if (id >= GA_BRR_MAX_SERVICE_CNTX)
    {
        GA_BRR_ERR (
        "[GA_BRR]: Invalid Parameters!\n");

        return  GA_FAILURE;
    }

    ga_brr_srvc_cntx[id].sid    = GA_BRR_INST_DONTCARE;
    ga_brr_srvc_cntx[id].sid_cb = NULL;
    ga_brr_srvc_cntx[id].s_uuid = GA_BRR_UUID_INVALID;
    ga_brr_srvc_cntx[id].s_type = GA_BRR_SERVICE_TYPE_INVALID;

    GA_BRR_TRC (
    "[GA_BRR]: << ga_brr_free_service_cntx, retval 0x%04X\n", GA_SUCCESS);

    return GA_SUCCESS;
}

GA_RESULT ga_brr_fetch_sid_of_cid
          (
              /* IN  */ GA_BRR_CHR_INST  cid,
              /* OUT */ GA_BRR_SVC_INST  * sid
          )
{
    UINT32    i;
    GA_RESULT retval;

    retval    = GA_FAILURE;

    GA_BRR_TRC(
    "[GA_BRR]: >> ga_brr_fetch_sid_of_cid\n");

    *sid = GA_BRR_INST_DONTCARE;

    for (i = 0; i < GA_BRR_MAX_SERVICE_CNTX; i++)
    {
        if ((ga_brr_srvc_cntx[i].s_cid <= cid) &&
            (ga_brr_srvc_cntx[i].e_cid >= cid))
        {
            *sid = ga_brr_srvc_cntx[i].sid;
            retval = GA_SUCCESS;

            break;
        }
    }

    GA_BRR_TRC (
    "[GA_BRR]: << ga_brr_fetch_sid_of_cid, retval 0x%04X\n", retval);

    return retval;
}

/** Service Registration and Callback Context */
GA_RESULT ga_brr_allocate_include_service_cntx
          (
              /* OUT */ UCHAR * id
          )
{
    UCHAR i;
    GA_RESULT  retval;

    GA_BRR_TRC (
    "[GA_BRR]: >> ga_brr_allocate_include_service_cntx\n");

    /* Param Check */
    if (NULL == id)
    {
        GA_BRR_ERR (
        "[GA_BRR]: Invalid Parameters!\n");

        return  GA_BRR_INVALID_PARAMS;
    }

    /* Init */
    retval = GA_FAILURE;

    for (i = 0; i < GA_BRR_MAX_INCLUDE_SERVICE_CNTX; i++)
    {
        if (GA_BRR_INST_DONTCARE == ga_brr_in_srvc_cntx[i].isid)
        {
            *id = i;
            retval = GA_SUCCESS;

            break;
        }
    }

    GA_BRR_TRC (
    "[GA_BRR]: << ga_brr_allocate_include_service_cntx, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_search_include_service_cntx
          (
              /* IN  */ GA_BRR_SVC_INST isid,
              /* OUT */ UCHAR           * id
          )
{
    UINT32 i;
    GA_RESULT retval;

    retval = GA_FAILURE;

    GA_BRR_TRC(
    "[GA_BRR]: >> ga_brr_search_include_service_cntx\n");

    *id = GA_BRR_MAX_INCLUDE_SERVICE_CNTX;

    /**
     * First treat the incoming sid as Primary Service ID and look
     * for a corresponding primary service context.
     */
    for (i = 0; i < GA_BRR_MAX_INCLUDE_SERVICE_CNTX; i++)
    {
        if (ga_brr_in_srvc_cntx[i].isid == isid)
        {
            *id = (UCHAR)i;
            retval = GA_SUCCESS;

            break;
        }
    }

    GA_BRR_TRC (
    "[GA_BRR]: << ga_brr_search_include_service_cntx, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_free_include_service_cntx
          (
              /* IN */ UCHAR id
          )
{
    GA_BRR_TRC (
    "[GA_BRR]: >> ga_brr_free_include_service_cntx for id 0x%02X\n", id);

    /* Param Check */
    if (id >= GA_BRR_MAX_INCLUDE_SERVICE_CNTX)
    {
        GA_BRR_ERR (
        "[GA_BRR]: Invalid Parameters!\n");

        return  GA_FAILURE;
    }

    /* Reset */
    ga_brr_in_srvc_cntx[id].isid    = GA_BRR_INST_DONTCARE;
    ga_brr_in_srvc_cntx[id].sid     = GA_BRR_INST_DONTCARE;
    ga_brr_in_srvc_cntx[id].is_uuid = GA_BRR_UUID_INVALID;

    GA_BRR_TRC (
    "[GA_BRR]: << ga_brr_free_include_service_cntx, retval 0x%04X\n", GA_SUCCESS);

    return GA_SUCCESS;
}

/** Context to track each individual Service UUID */
/** TODO: Check if this is a List to be maintained in the BRR PL */
GA_RESULT ga_brr_search_serv_uuid_cntx
          (
              /* IN  */ GA_BRR_UUID s_uuid,
              /* OUT */ UCHAR       * id
          )
{
    UINT32 i;
    GA_RESULT retval;

    retval = GA_FAILURE;

    GA_BRR_TRC(
    "[GA_BRR]: >> ga_brr_search_serv_uuid_cntx\n");

    *id = GA_BRR_MAX_SERVICE_UUID_CNTX;

    for (i = 0; i < GA_BRR_MAX_SERVICE_UUID_CNTX; i++)
    {
        if (ga_brr_srvc_uuid_cntx[i].s_uuid == s_uuid)
        {
            *id = (UCHAR)i;
            retval = GA_SUCCESS;

            break;
        }
    }

    GA_BRR_TRC (
    "[GA_BRR]: << ga_brr_search_serv_uuid_cntx, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_allocate_serv_uuid_cntx
          (
              /* IN  */ GA_BRR_UUID s_uuid,
              /* OUT */ UCHAR       * id
          )
{
    GA_RESULT retval;
    UCHAR i;

    GA_BRR_TRC(
    "[GA_BRR]: >> ga_brr_allocate_serv_uuid_cntx\n");

    /* Param Check */
    if (NULL == id)
    {
        GA_BRR_ERR (
        "[GA_BRR]: Invalid Parameters!\n");

        return  GA_BRR_INVALID_PARAMS;
    }

    /* Init */
    retval = GA_FAILURE;

    for (i = 0; i < GA_BRR_MAX_SERVICE_UUID_CNTX; i++)
    {
        if (GA_BRR_UUID_INVALID == ga_brr_srvc_uuid_cntx[i].s_uuid)
        {
            *id = i;
            ga_brr_srvc_uuid_cntx[i].s_uuid = s_uuid;
            retval = GA_SUCCESS;

            break;
        }
    }

    GA_BRR_TRC (
    "[GA_BRR]: << ga_brr_allocate_serv_uuid_cntx, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_free_serv_uuid_cntx
          (
              /* IN  */ GA_BRR_UUID s_uuid
          )
{
    GA_RESULT retval;
    UCHAR id;

    GA_BRR_TRC(
    "[GA_BRR]: >> ga_brr_free_serv_uuid_cntx\n");

    retval = GA_FAILURE;

    /* Reset */
    if (GA_SUCCESS == ga_brr_search_serv_uuid_cntx(s_uuid, &id))
    {
        ga_brr_srvc_uuid_cntx[id].s_uuid = GA_BRR_UUID_INVALID;
        ga_brr_srvc_uuid_cntx[id].lkid   = GA_BRR_INST_DONTCARE;
    }

    GA_BRR_TRC (
    "[GA_BRR]: << ga_brr_free_serv_uuid_cntx, retval 0x%04X\n", retval);

    return retval;
}

/** Context to track each individual Characteristic UUID */
/** TODO: Check if this is a List to be maintained in the BRR PL */
GA_RESULT ga_brr_search_char_uuid_cntx
          (
              /* IN  */ GA_BRR_UUID c_uuid,
              /* OUT */ UCHAR       * id
          )
{
    UINT32 i;
    GA_RESULT retval;

    retval = GA_FAILURE;

    GA_BRR_TRC(
    "[GA_BRR]: >> ga_brr_search_char_uuid_cntx\n");

    *id = GA_BRR_MAX_CHAR_UUID_CNTX;

    for (i = 0; i < GA_BRR_MAX_CHAR_UUID_CNTX; i++)
    {
        if (ga_brr_char_uuid_cntx[i].c_uuid == c_uuid)
        {
            *id = (UCHAR)i;
            retval = GA_SUCCESS;

            break;
        }
    }

    GA_BRR_TRC (
    "[GA_BRR]: << ga_brr_search_char_uuid_cntx, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_allocate_char_uuid_cntx
          (
              /* IN  */ GA_BRR_UUID c_uuid,
              /* OUT */ UCHAR       * id
          )
{
    GA_RESULT retval;
    UCHAR i;

    GA_BRR_TRC(
    "[GA_BRR]: >> ga_brr_allocate_char_uuid_cntx\n");

   /* Param Check */
    if (NULL == id)
    {
        GA_BRR_ERR (
        "[GA_BRR]: Invalid Parameters!\n");

        return  GA_BRR_INVALID_PARAMS;
    }

    /* Init */
    retval = GA_FAILURE;

    for (i = 0; i < GA_BRR_MAX_CHAR_UUID_CNTX; i++)
    {
        if (GA_BRR_UUID_INVALID == ga_brr_char_uuid_cntx[i].c_uuid)
        {
            *id = i;
            ga_brr_char_uuid_cntx[i].c_uuid = c_uuid;
            retval = GA_SUCCESS;

            break;
        }
    }

    GA_BRR_TRC (
    "[GA_BRR]: << ga_brr_allocate_char_uuid_cntx, retval 0x%04X\n", retval);

    return retval;
}

GA_RESULT ga_brr_free_char_uuid_cntx
          (
              /* IN  */ GA_BRR_UUID c_uuid
          )
{
    GA_RESULT retval;
    UCHAR id;

    GA_BRR_TRC(
    "[GA_BRR]: >> ga_brr_free_char_uuid_cntx\n");

    retval = GA_FAILURE;

    /* Reset */
    if (GA_SUCCESS == ga_brr_search_char_uuid_cntx(c_uuid, &id))
    {
        ga_brr_char_uuid_cntx[id].c_uuid = GA_BRR_UUID_INVALID;
        ga_brr_char_uuid_cntx[id].lkid   = GA_BRR_INST_DONTCARE;
    }

    GA_BRR_TRC (
    "[GA_BRR]: << ga_brr_free_char_uuid_cntx, retval 0x%04X\n", retval);

    return retval;
}

#endif /* BT_GAM */