
/**
 *  \file gatt.c
 *
 *  This file contains the GATT Client interface implementations.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "gatt.h"

#ifdef BT_4_0
#ifdef GATT_CLIENT

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
/* GATT procedure context */
DECL_STATIC GATT_CONTEXT gatt_ctx[GATT_NUM_CONTEXTS];

/* GATT application callback */
DECL_STATIC GATT_APP_CB gatt_app_cb;

UCHAR   * val_ptr;

UINT16  reliable_write_count;

GATT_PREPARE_WRITE_REQ_PARAM prep_write_req_param;

UCHAR    char_desc_start;

/* --------------------------------------------- Functions */

API_RESULT gatt_search_context (ATT_HANDLE * att_handle, UCHAR * id)
{
    UCHAR i;

    for (i= 0; i < GATT_NUM_CONTEXTS; i++)
    {
        if (0 == BT_mem_cmp(&gatt_ctx[i].att_handle, att_handle, sizeof(ATT_HANDLE)))
        {
            *id = i;
            return API_SUCCESS;
        }
    }

    *id = GATT_INVALID_CONTEXT;
    return API_FAILURE;
}

API_RESULT gatt_alloc_context (ATT_HANDLE * att_handle, UCHAR * id)
{
    UCHAR i;

    for (i= 0; i < GATT_NUM_CONTEXTS; i++)
    {
        if (DEVICE_HANDLE_INIT_VAL == gatt_ctx[i].att_handle.device_id)
        {
            BT_mem_set
            (
                &gatt_ctx,
                0x00,
                (sizeof (GATT_CONTEXT) * GATT_NUM_CONTEXTS)
            );

            gatt_ctx[i].att_handle = *att_handle;
            *id = i;
            return API_SUCCESS;
        }
    }

    *id = i;
    return API_FAILURE;
}

API_RESULT gatt_free_context (ATT_HANDLE * att_handle)
{
    UCHAR id;

    if (API_SUCCESS == gatt_search_context(att_handle, &id))
    {
        gatt_ctx[id].att_handle.device_id = DEVICE_HANDLE_INIT_VAL;
        gatt_ctx[id].att_handle.att_id = 0xFF;
        gatt_ctx[id].proc = 0x00;

        return API_SUCCESS;
    }

    return API_FAILURE;
}

void gatt_handle_service_search_complete
     (
         /* IN */ GATT_CONTEXT * context
     )
{
    ATT_HANDLE handle;
    UINT16 proc;

    /* Get the ATT handle and procedure locally */
    handle = context->att_handle;
    proc = context->proc;

    if (GATT_PS_DISCOVERY == proc)
    {
        GATT_TRC ("No of Primary Services - %d\n", context->attr_index);
    }
    else
    {
        GATT_TRC ("No of Primary Services - %d\n", context->attr_index);
    }

    /* Free the context */
    gatt_free_context (&context->att_handle);

    GATT_APP_CB_NTF
    (
        &handle,
        (GATT_PS_DISCOVERY == proc)?
        GATT_PS_DISCOVERY_RSP: GATT_SS_DISCOVERY_RSP,
        API_SUCCESS,
        context->service,
        context->attr_index
    );
}

void gatt_handle_included_service_search_complete
     (
         /* IN */ GATT_CONTEXT * context
     )
{
    ATT_HANDLE handle;

    GATT_TRC ("No of Included Services - %d\n", context->attr_index);

    /* Get the ATT handle locally */
    handle = context->att_handle;

    /* Free the context */
    gatt_free_context (&context->att_handle);

    GATT_APP_CB_NTF
    (
        &handle,
        GATT_IS_DISCOVERY_RSP,
        API_SUCCESS,
        context->inc_service,
        context->attr_index
    );
}

void gatt_handle_characteristics_search_complete
     (
          /* IN */ GATT_CONTEXT * context
     )
{
    API_RESULT retval;
    ATT_FIND_INFO_REQ_PARAM param;
    ATT_HANDLE handle;

    GATT_TRC ("No of Characteristics - %d\n", context->attr_index);

    /* Notify application if Descriptor Discovery is not requested */
    if (!(GATT_CHAR_DESC_DISCOVERY & context->proc) ||
         (0 == context->attr_index) ||
         (++context->index > context->attr_index))
    {
        /* Get the ATT handle locally */
        handle = context->att_handle;

        /* Free the context */
        gatt_free_context (&context->att_handle);

        GATT_APP_CB_NTF
        (
            &handle,
            GATT_CHAR_DISCOVERY_RSP,
            API_SUCCESS,
            context->characteristic,
            context->attr_index
        );
    }
    else
    {
        /* Initialize context parse index */
        context->index = 0;

        BT_LOOP_FOREVER()
        {
            /* Store operation end handle */
            if ((context->index + 1) < (context->attr_index))
            {
                context->handle = context->characteristic[context->index + 1].
                                  range.start_handle - 1;
            }
            else if ((context->index + 1) == (context->attr_index))
            {
                context->handle = context->se_handle;
            }
            else
            {
                gatt_handle_characteristics_search_complete(context);
                break;
            }

            /* Prepare request parameters */
            param.start_handle = context->characteristic[context->index].
                                 value_handle + 1;
            param.end_handle = context->handle;

            if (param.start_handle > param.end_handle)
            {
                context->index ++;
                continue;
            }

            /* Send Request */
            retval = BT_att_send_find_info_req
                     (
                         &context->att_handle,
                         &param
                     );

            if (API_SUCCESS != retval)
            {
                GATT_ERR ("ATT Request failed\n");

                /* Free the context */
                gatt_free_context (&context->att_handle);
            }

            break;
        }
    }
}

void gatt_handle_char_desc_search_complete
     (
          /* IN */ GATT_CONTEXT * context
     )
{
    API_RESULT retval;
    ATT_FIND_INFO_REQ_PARAM param;
    ATT_HANDLE handle;

    /* To keep some compilers happy */
    param.start_handle = param.end_handle = 0;

    if (GATT_CHAR_DISCOVERY & context->proc)
    {
        BT_LOOP_FOREVER()
        {
            /* Update context temp index */
            if (++context->index <= context->attr_index)
            {
                /* Store operation end handle */
                if ((context->index + 1) < (context->attr_index))
                {
                    context->handle = context->characteristic[context->index + 1].
                                      range.start_handle - 1;
                }
                else if ((context->index + 1) == (context->attr_index))
                {
                    context->handle = context->se_handle;
                }
                else
                {
                    gatt_handle_characteristics_search_complete(context);
                    break;
                }

                /* Prepare request parameters */
                param.start_handle = context->characteristic[context->index].
                                     value_handle + 1;
                param.end_handle = context->handle;

                if (param.start_handle > param.end_handle)
                {
                    continue;
                }

                /* Send Request */
                retval = BT_att_send_find_info_req
                         (
                             &context->att_handle,
                             &param
                         );
                if (API_SUCCESS != retval)
                {
                    GATT_ERR ("ATT Request failed\n");

                    /* Free the context */
                    gatt_free_context (&context->att_handle);
                }

                break;
            }
        }
        return;
    }

    /* Get the ATT handle locally */
    handle = context->att_handle;

    /* Free the context */
    gatt_free_context (&context->att_handle);

    GATT_APP_CB_NTF
    (
        &handle,
        GATT_CHAR_DESC_DISCOVERY_RSP,
        API_SUCCESS,
        context->characteristic[context->index].descriptor,
        context->characteristic[context->index].desc_index
    );
}


API_RESULT gatt_handle_find_by_type_value_response
           (
               ATT_HANDLE     * att_handle,
               UCHAR          * data,
               UINT16         datalen
           )
{
    ATT_HANDLE_RANGE range;
    API_RESULT retval;
    UCHAR id, i, count;
    GATT_CONTEXT * ctx;
    ATT_FIND_BY_TYPE_VAL_REQ_PARAM    param;
    ATT_VALUE attval;
    UCHAR value[ATT_128_BIT_UUID_SIZE];

    /* Get the context */
    retval = gatt_search_context (att_handle, &id);
    if (API_SUCCESS != retval)
    {
        GATT_ERR ("Failed to get context - 0x%04X\n", retval);
        return retval;
    }

    ctx = &gatt_ctx[id];

    if ((GATT_PS_DISCOVERY == ctx->proc) || (GATT_SS_DISCOVERY == ctx->proc))
    {
        /* Extract count based on Start Handle End Handle pairs received */
        count = (datalen >> (sizeof(ATT_ATTR_HANDLE)));

        for (i = 0; i < count; i++)
        {
            /* Get the start handle */
            BT_UNPACK_LE_2_BYTE
            (
                &range.start_handle,
                data
            );
            data += 2;

            /* Get the end handle */
            BT_UNPACK_LE_2_BYTE
            (
                &range.end_handle,
                data
            );
            data += 2;

            /* Store Service Related information */
            ctx->service[ctx->attr_index].range = range;
            /* Type in FBTV is of 16 bit */
            ctx->service[ctx->attr_index].uuid = ctx->uuid;
            /* Update UUID type */
            ctx->service[ctx->attr_index].uuid_type = ctx->frmt;

            /* Increment next index */
            ctx->attr_index ++;
        }

        if ((GATT_MAX_SERVICES == ctx->attr_index) ||
            (ATT_ATTR_HANDLE_END_RANGE == range.end_handle))
        {
            /* Cannot process any further Services */
            GATT_TRC ("[*** Alert***]: Cannot process further requests\n");

            gatt_handle_service_search_complete (ctx);
        }
        else
        {
            range.start_handle = range.end_handle + 1;
            range.end_handle = ATT_ATTR_HANDLE_END_RANGE;

            /* There could be more Primary Services, Initiate Discovery again */
#ifdef ATT_SUPPORT_128_BIT_UUID
            if (ATT_16_BIT_UUID_FORMAT == ctx->frmt)
            {
                BT_PACK_LE_2_BYTE (value,&(ctx->uuid.uuid_16));
                attval.len = ATT_16_BIT_UUID_SIZE;
            }
            else
            {
                BT_PACK_LE_16_BYTE (value,&(ctx->uuid.uuid_128.value));
                attval.len = ATT_128_BIT_UUID_SIZE;
            }
#else /* ATT_SUPPORT_128_BIT_UUID */
            BT_PACK_LE_2_BYTE (value,&(ctx->uuid));
            attval.len = ATT_16_BIT_UUID_SIZE;
#endif /* ATT_SUPPORT_128_BIT_UUID */
            attval.val = value;
            attval.actual_len = 0U;

            param.range = range;
            param.uuid = GATT_PRIMARY_SERVICE;
            param.value = attval;

            retval = BT_att_send_find_by_type_val_req
                     (
                         att_handle,
                         &param
                     );

            if (API_SUCCESS != retval)
            {
                GATT_ERR ("ATT Request failed\n");

                /* Free the context */
                gatt_free_context (att_handle);
            }
        }

        return API_SUCCESS;
    }

    return API_FAILURE;
}

API_RESULT gatt_handle_read_blob_response
           (
               ATT_HANDLE   * att_handle,
               UCHAR          * data,
               UINT16         datalen
           )
{
    API_RESULT    retval;
    GATT_CONTEXT  * ctx;
    UCHAR         id;
    ATT_READ_BLOB_REQ_PARAM param;

    /* Get the context */
    retval = gatt_search_context (att_handle, &id);
    if (API_SUCCESS != retval)
    {
        GATT_ERR ("Failed to get context - 0x%04X\n", retval);
        return retval;
    }

    GATT_APP_CB_NTF
    (
        att_handle,
        ATT_READ_BLOB_RSP,
        API_SUCCESS,
        data,
        datalen
    );

    ctx = &gatt_ctx[id];

    if ((datalen + 1) < ATT_DEFAULT_MTU)
    {
        /* Free Context */
        gatt_free_context(att_handle);
        /* Data read complete */
        GATT_TRC ("Characteristic Long Value Read Complete\n");
    }
    else
    {
        /* Send Read blob req with the update offset */
        ctx->characteristic[id].val_offset += datalen;
        param.handle = ctx->handle;
        param.offset = ctx->characteristic[id].val_offset;
        retval = BT_att_send_read_blob_req
                 (
                     &ctx->att_handle,
                     &param
                 );
    }

    return retval;
}

API_RESULT gatt_handle_read_by_group_response
           (
               ATT_HANDLE   * att_handle,
               UINT16         data_elem_size,
               UCHAR          * data,
               UINT16         data_len
           )
{
    ATT_HANDLE_RANGE range;
    UINT32 index;
    UCHAR * dptr;
    UCHAR  search_again;
    API_RESULT retval;
    UCHAR id;
    GATT_CONTEXT * ctx;
    ATT_READ_BY_GROUP_TYPE_REQ_PARAM param;
    UCHAR uuid_type;

    /* Get the context */
    retval = gatt_search_context (att_handle, &id);
    if (API_SUCCESS != retval)
    {
        GATT_ERR ("Failed to get context - 0x%04X\n", retval);
        return retval;
    }

    ctx = &gatt_ctx[id];

    search_again = 1;

    for (index = 0; index < (unsigned)(data_len/(data_elem_size)); index++)
    {
        /* Refer to the data element start */
        dptr = (data + (index * data_elem_size));

        /* Get the start handle */
        BT_UNPACK_LE_2_BYTE
        (
            &range.start_handle,
            dptr
        );
        dptr += 2;

        /* Get the end handle */
        BT_UNPACK_LE_2_BYTE
        (
            &range.end_handle,
            dptr
        );
        dptr += 2;

        GATT_TRC (
        "Handle Range 0x%04X - 0x%04X\n",range.start_handle,range.end_handle);

        GATT_TRC ("Handle Value Received - \n");
        GATT_debug_dump_bytes(dptr, (data_elem_size - 4));

        if ((GATT_PS_DISCOVERY == ctx->proc) || (GATT_SS_DISCOVERY == ctx->proc))
        {
            if ((ATT_16_BIT_UUID_SIZE != (data_elem_size - 4)) &&
                (ATT_128_BIT_UUID_SIZE != (data_elem_size - 4)))
            {
                GATT_TRC ("[*** ERR ***]: Cannot Handle Service Values of Size"
                "0x%04X\n",data_elem_size-4);

                continue;
            }

            /* Store Service Related information */
            ctx->service[ctx->attr_index].range = range;

            if (ATT_16_BIT_UUID_SIZE == (data_elem_size - 4))
            {
                uuid_type = ATT_16_BIT_UUID_FORMAT;
            }
            else
            {
                uuid_type = ATT_128_BIT_UUID_FORMAT;
            }

            GATT_EXTRACT_UUID
            (
                &ctx->service[ctx->attr_index].uuid,
                dptr,
                uuid_type
            );

#ifdef ATT_SUPPORT_128_BIT_UUID
            ctx->service[ctx->attr_index].uuid_type = uuid_type;
#endif /* ATT_SUPPORT_128_BIT_UUID */

            if (GATT_MAX_SERVICES == ++(ctx->attr_index))
            {
                /* Cannot process any further Services */
                GATT_TRC ("[*** Alert***]: Cannot process further requests\n");
                break;
            }
        }
    }

    /**
     * If the last handle is ATT_ATTR_HANDLE_END_RANGE, or MAX Service count
     * is reached locally, there is no need to search again,
     * go to the next logical step!
     */
    if ((ATT_ATTR_HANDLE_END_RANGE == range.end_handle) ||
        (GATT_MAX_SERVICES == ctx->attr_index))
    {
        search_again = 0;
    }

    if ((GATT_PS_DISCOVERY == ctx->proc) || (GATT_SS_DISCOVERY == ctx->proc))
    {
        if (0 == search_again)
        {
            gatt_handle_service_search_complete (ctx);
        }
        else
        {
            range.start_handle = range.end_handle + 1;
            range.end_handle = ATT_ATTR_HANDLE_END_RANGE;

            /* There could be more Primary Services, Initiate Discovery again */
            param.range = range;
#ifdef ATT_SUPPORT_128_BIT_UUID
            param.group_type.uuid_16 = (GATT_PS_DISCOVERY == ctx->proc)?
            GATT_PRIMARY_SERVICE: GATT_SECONDARY_SERVICE;
#else /* ATT_SUPPORT_128_BIT_UUID */
            param.group_type = (GATT_PS_DISCOVERY == ctx->proc)?
            GATT_PRIMARY_SERVICE: GATT_SECONDARY_SERVICE;
#endif /* ATT_SUPPORT_128_BIT_UUID */
            param.uuid_format = ATT_16_BIT_UUID_FORMAT;

            retval = BT_att_send_read_by_group_req
                     (
                         att_handle,
                         &param
                     );

            if (API_SUCCESS != retval)
            {
                GATT_ERR ("ATT Request failed\n");

                /* Free the context */
                gatt_free_context (att_handle);
            }
        }

        return API_SUCCESS;
    }

    return API_FAILURE;
}


API_RESULT gatt_handle_find_info_response
           (
               ATT_HANDLE  * att_handle,
               UINT16        uuid_type,
               UCHAR         * data,
               UINT16        data_len
           )
{
    UINT32 index;
    UINT16 handle;
    UINT16 data_elem_size;
    UCHAR  search_again;
    UCHAR * dptr;
    API_RESULT retval;
    UCHAR id, did;
    ATT_UUID uuid;
    GATT_CONTEXT * ctx;
    ATT_FIND_INFO_REQ_PARAM param;
    UINT16 pri_serv_uuid;

    did = 0;
    pri_serv_uuid = GATT_PRIMARY_SERVICE;
    /* TODO: Check why handle to be set here */
    handle = 0;
    data_elem_size = 0U;

    /* Get the context */
    retval = gatt_search_context (att_handle, &id);
    if (API_SUCCESS != retval)
    {
        GATT_ERR ("Failed to get context - 0x%04X\n", retval);
        return retval;
    }

    ctx = &gatt_ctx[id];

    if (GATT_CHAR_DESC_DISCOVERY & ctx->proc)
    {
        /* Set Char Descriptor Discovery Start flag */
        char_desc_start = BT_TRUE;
    }

    search_again = 1;

    if ((ATT_16_BIT_UUID_FORMAT == uuid_type) || (ATT_128_BIT_UUID_FORMAT == uuid_type))
    {
        switch (uuid_type)
        {
        case ATT_16_BIT_UUID_FORMAT:
            data_elem_size = ATT_16_BIT_UUID_SIZE + 2;
            break;

        case ATT_128_BIT_UUID_FORMAT:
            data_elem_size = ATT_128_BIT_UUID_SIZE + 2;
            break;
        }

        for (index = 0; index < (unsigned)(data_len/data_elem_size); index++ )
        {
            if (GATT_CHAR_DESC_DISCOVERY & ctx->proc)
            {
                /* Refer to the data element start */
                dptr = (data + (index * data_elem_size));

                /* Get the descriptor handl e */
                BT_UNPACK_LE_2_BYTE(&handle, dptr);
                dptr += 2;

                /* Get the descriptor UUID */
                GATT_EXTRACT_UUID(&uuid,dptr,uuid_type);

                /**
                 *  Exclude the primary services in the descriptor discovery.
                 *  As the end handle for the characteristics descriptor discover is
                 *  derived from the next characteristics start handle.
                 *  Need to add a proper logic!!
                 */
                if (!(GATT_CHECK_UUID(&uuid,&pri_serv_uuid,ATT_16_BIT_UUID_FORMAT)))
                {
#ifdef ATT_SUPPORT_128_BIT_UUID
                    if (ATT_16_BIT_UUID_FORMAT == uuid_type)
                    {
                        GATT_TRC (
                        "Handle - 0x%04X, UUID - 0x%04X\n", handle, uuid.uuid_16);
                    }
                    else
                    {
                        /* Add trace for 128 bit UUID */
                    }
#else /* ATT_SUPPORT_128_BIT_UUID */
                    GATT_TRC ("Handle - 0x%04X, UUID - 0x%04X\n", handle, uuid);
#endif /* ATT_SUPPORT_128_BIT_UUID */


                    /* Store descriptor related information */
                    did = ctx->characteristic[ctx->index].desc_index;
                    ctx->characteristic[ctx->index].descriptor[did].handle = handle;
                    ctx->characteristic[ctx->index].descriptor[did].uuid = uuid;
#ifdef ATT_SUPPORT_128_BIT_UUID
                    ctx->characteristic[ctx->index].descriptor[did].uuid_type = (UCHAR)uuid_type;
#endif /* ATT_SUPPORT_128_BIT_UUID */

                    /* Update descriptor index */
                    ctx->characteristic[ctx->index].desc_index = ++did;
                }

                if ((GATT_MAX_CHAR_DESCRIPTORS == did) ||
                    (handle == ctx->handle))
                {
                    search_again = 0;

                    /* Cannot process any further Services */
                    GATT_TRC ("[*** Alert***]: Cannot process further requests\n");
                    break;
                }
            }
        }
    }

    if (0 == search_again)
    {
        gatt_handle_char_desc_search_complete (ctx);
    }
    else
    {
        /* Prepare request parameters */
        param.start_handle = handle + 1;
        param.end_handle = ctx->handle;

        /* Send Request */
        retval = BT_att_send_find_info_req
                 (
                     att_handle,
                     &param
                 );

        if (API_SUCCESS != retval)
        {
            GATT_ERR ("ATT Request failed\n");

            /* Free the context */
            gatt_free_context (&ctx->att_handle);
        }
    }
    return API_SUCCESS;
}


API_RESULT gatt_handle_read_by_type_response
           (
               ATT_HANDLE  * att_handle,
               UINT16        data_elem_size,
               UCHAR         * data,
               UINT16        data_len
           )
{
    ATT_HANDLE_RANGE range;
    UINT32 index;
    UINT16 handle;
    UCHAR  search_again;
    UCHAR * dptr;
    API_RESULT retval;
    UCHAR id;
    GATT_CONTEXT * ctx;
    ATT_READ_BY_TYPE_REQ_PARAM param;
    UCHAR uuid_type;
    UINT16 temp_uuid;

    uuid_type = ATT_16_BIT_UUID_FORMAT;
    temp_uuid = 0x0000;
    /* TODO: Check why handle to be set here */
    handle = 0;

    /* Get the context */
    retval = gatt_search_context (att_handle, &id);
    if (API_SUCCESS != retval)
    {
        GATT_ERR ("Failed to get context - 0x%04X\n", retval);
        return retval;
    }

    ctx = &gatt_ctx[id];

    search_again = 1;

    for (index = 0; index < (unsigned)(data_len/(data_elem_size)); index++ )
    {
        if (GATT_IS_DISCOVERY == ctx->proc)
        {
            /* Refer to the data element start */
            dptr = (data + (index * data_elem_size));

            BT_UNPACK_LE_2_BYTE(&handle, dptr);
            dptr += 2;

            /* Get the start handle */
            BT_UNPACK_LE_2_BYTE
            (
                &range.start_handle,
                dptr
            );
            dptr += 2;

            /* Get the end handle */
            BT_UNPACK_LE_2_BYTE
            (
                &range.end_handle,
                dptr
            );
            dptr += 2;

            GATT_TRC ("Handle - 0x%04X\n", handle);

            GATT_TRC (
            "Handle Range 0x%04X - 0x%04X\n",range.start_handle,range.end_handle);

            GATT_TRC ("Handle Value Received - \n");
            GATT_debug_dump_bytes(dptr, (data_elem_size - 6));

            /**
             * Check if the Value is of size 2, as the application handles only
             * 16 bit UUIDs
             */
            if ((data_elem_size - 6) > ATT_16_BIT_UUID_SIZE)
            {
                GATT_TRC ("[*** ERR ***]: Cannot Handle Service Values of Size"
                "0x%04X\n",data_elem_size-2);
                handle = ATT_ATTR_HANDLE_END_RANGE;
                break;
            }

            /* Store Included Service Related information */
            ctx->inc_service[ctx->attr_index].handle = handle;
            ctx->inc_service[ctx->attr_index].service.range = range;
#ifdef ATT_SUPPORT_128_BIT_UUID
            ctx->inc_service[ctx->attr_index].service.uuid.uuid_16 = 0x0000;
#else /* ATT_SUPPORT_128_BIT_UUID */
            ctx->inc_service[ctx->attr_index].service.uuid = 0x0000;
#endif /* ATT_SUPPORT_128_BIT_UUID */
            ctx->inc_service[ctx->attr_index].service.uuid_type = ATT_16_BIT_UUID_FORMAT;

            /**
             *  Supports only 16 bit UUID. For 128 bit UUID send a read request
             *  if there is no UUID included in the Read By Type Response.
             */
            if (0 != (data_elem_size - 6))
            {
                GATT_EXTRACT_UUID
                (
                    &ctx->inc_service[ctx->attr_index].service.uuid,
                    dptr,
                    ATT_16_BIT_UUID_FORMAT
                );
            }


            if (GATT_MAX_INC_SERVICES == ++ctx->attr_index)
            {
                search_again = 0;

                /* Cannot process any further Services */
                GATT_TRC ("[*** Alert***]: Cannot process further requests\n");
                break;
            }
        }
        else if (GATT_CHAR_DISCOVERY & ctx->proc)
        {
            /* Refer to the data element start */
            dptr = (data + (index * data_elem_size));

            BT_UNPACK_LE_2_BYTE(&handle, dptr);
            dptr += 2;

            /**
             * Check if the Value is of size 2 or 16, as the application handles
             * only 16 and 128 bit UUIDs
             */
            if ((ATT_16_BIT_UUID_SIZE != (data_elem_size - 5)) &&
                (ATT_128_BIT_UUID_SIZE != (data_elem_size - 5)))
            {
                GATT_TRC ("[*** ERR ***]: Cannot Handle Service Values of Size"
                "0x%04X\n",data_elem_size-2);
                handle = ATT_ATTR_HANDLE_END_RANGE;
                break;
            }

            /* Store CHaracteristic Related information */
            ctx->characteristic[ctx->attr_index].range.start_handle = handle;

            /* Get the property */
            BT_UNPACK_LE_1_BYTE
            (
                &ctx->characteristic[ctx->attr_index].cproperty,
                dptr
            );
            dptr += 1;

            /* Get the value handle */
            BT_UNPACK_LE_2_BYTE
            (
                &ctx->characteristic[ctx->attr_index].value_handle,
                dptr
            );
            dptr += 2;

            GATT_TRC ("Handle - 0x%04X\n", handle);

            GATT_TRC (
            "Property - 0x%02X, Value Handle - 0x%04X\n",
            ctx->characteristic[ctx->attr_index].cproperty,
            ctx->characteristic[ctx->attr_index].value_handle);

            GATT_TRC ("Handle Value Received - \n");
            GATT_debug_dump_bytes(dptr, (data_elem_size - 5));

            if (ATT_16_BIT_UUID_SIZE == (data_elem_size - 5))
            {
                uuid_type = ATT_16_BIT_UUID_FORMAT;
            }
            else
            {
                uuid_type = ATT_128_BIT_UUID_FORMAT;
            }

            /* Extract UUID */
            GATT_EXTRACT_UUID
            (
                &ctx->characteristic[ctx->attr_index].uuid,
                dptr,
                uuid_type
            );

#ifdef ATT_SUPPORT_128_BIT_UUID
            ctx->characteristic[ctx->attr_index].uuid_type = uuid_type;
#endif /* ATT_SUPPORT_128_BIT_UUID */

            if (GATT_CHECK_UUID(&ctx->uuid,&temp_uuid,ATT_16_BIT_UUID_FORMAT) ||
                GATT_CHECK_UUID(&ctx->uuid,&ctx->characteristic[ctx->attr_index].uuid,uuid_type))
            {
                if (GATT_MAX_CHARACTERISTICS == ++ctx->attr_index)
                {
                    search_again = 0;

                    /* Cannot process any further Services */
                    GATT_TRC ("[*** Alert***]: Cannot process further requests\n");
                    break;
                }
            }
        }
        else if ((GATT_CHAR_READ == ctx->proc) &&
            (!(GATT_CHECK_UUID(&(ctx->uuid),&temp_uuid,ATT_16_BIT_UUID_FORMAT))))
        {
            return API_FAILURE;
        }
    }

    if (GATT_IS_DISCOVERY == ctx->proc)
    {
        if (0 == search_again)
        {
            gatt_handle_included_service_search_complete (ctx);
        }
        else
        {
            /* There could be more included services, Discover them */
            range.start_handle = handle + 1;
            range.end_handle = ATT_ATTR_HANDLE_END_RANGE;

            param.range = range;
#ifdef ATT_SUPPORT_128_BIT_UUID
            param.uuid.uuid_16 = GATT_INCLUDE;
#else /* ATT_SUPPORT_128_BIT_UUID */
            param.uuid = GATT_INCLUDE;
#endif /* ATT_SUPPORT_128_BIT_UUID */
            param.uuid_format = ATT_16_BIT_UUID_FORMAT;

            retval = BT_att_send_read_by_type_req
                     (
                         att_handle,
                         &param
                     );

            if (API_SUCCESS != retval)
            {
                GATT_ERR ("ATT Request failed\n");

                /* Free the context */
                gatt_free_context (att_handle);
            }
        }

        return API_SUCCESS;
    }
    else if (GATT_CHAR_DISCOVERY & ctx->proc)
    {
        /* Check if further Search is required */
        if ((0 == search_again) ||
            (handle + 2 >= ctx->handle))
        {
            gatt_handle_characteristics_search_complete (ctx);
        }
        else
        {
            /* There are more characteristics, discover them */
            range.start_handle = handle + 1;
            range.end_handle = ctx->handle;

            param.range = range;
#ifdef ATT_SUPPORT_128_BIT_UUID
            param.uuid.uuid_16 = GATT_CHARACTERISTIC;
#else /* ATT_SUPPORT_128_BIT_UUID */
            param.uuid = GATT_CHARACTERISTIC;
#endif /* ATT_SUPPORT_128_BIT_UUID */
            param.uuid_format = ATT_16_BIT_UUID_FORMAT;

            retval = BT_att_send_read_by_type_req
                     (
                         att_handle,
                         &param
                     );

            if (API_SUCCESS != retval)
            {
                GATT_ERR ("ATT Request failed\n");

                /* Free the context */
                gatt_free_context (att_handle);
            }
        }

        return API_SUCCESS;
    }

    return API_FAILURE;
}

API_RESULT gatt_handle_prepare_write_response
           (
               ATT_HANDLE  * att_handle,
               UCHAR         * data,
               UINT16        data_len
           )
{
    UCHAR id;
    GATT_CONTEXT * ctx;
    API_RESULT retval;
    ATT_PREPARE_WRITE_REQ_PARAM param;
    ATT_EXECUTE_WRITE_REQ_PARAM req_param;
    int indx;

    /* Get the context */
    retval = gatt_search_context (att_handle, &id);
    if (API_SUCCESS != retval)
    {
        GATT_ERR ("Failed to get context - 0x%04X\n", retval);
        return retval;
    }

    ctx = &gatt_ctx[id];

    if (GATT_CHAR_WRITE == ctx->proc)
    {
        if (ctx->characteristic[id].val_length > (data_len - 4))
        {
            ctx->characteristic[id].val_offset += (data_len - 4);
            ctx->characteristic[id].val_length = (ctx->characteristic[id].val_length - (data_len - 4));
            param.offset = ctx->characteristic[id].val_offset;
            param.handle_value.handle = ctx->handle;
            param.handle_value.value.val = val_ptr + param.offset;
            param.handle_value.value.len = ctx->characteristic[id].val_length;
            retval = BT_att_send_prepare_write_req
                     (
                         att_handle,
                         &param
                     );
        }
        else
        {
            /* Execute the queued write */
            req_param.flags = ATT_EXECUTE_WRITE_EXEC_FLAG;
            retval = BT_att_send_execute_write_req( att_handle, &req_param);
            BT_free_mem (val_ptr);
        }
    }
    else
    {
        if ((0 != BT_mem_cmp(data, &ctx->handle, 2)) ||
            (0 != BT_mem_cmp(data + 2, &ctx->characteristic[id].val_offset, 2)) ||
            (0 != BT_mem_cmp(data + 4, prep_write_req_param.req_param\
                [(reliable_write_count - 1)].handle_value.value.val,\
                ctx->characteristic[id].val_length)))
        {
            /* Cancel all Prepare write request!! */
            req_param.flags = ATT_EXECUTE_WRITE_CANCEL_FLAG;
            retval = BT_att_send_execute_write_req( att_handle, &req_param);
        }
        else if (reliable_write_count != prep_write_req_param.count)
        {
            ctx->characteristic[id].val_offset = prep_write_req_param.req_param\
                [reliable_write_count].offset;
            ctx->handle = prep_write_req_param.req_param[reliable_write_count]\
                .handle_value.handle;
            ctx->characteristic[id].val_length = prep_write_req_param.req_param\
                [reliable_write_count].handle_value.value.len;

            param.offset = ctx->characteristic[id].val_offset;
            param.handle_value.handle = ctx->handle;
            param.handle_value.value.len = ctx->characteristic[id].val_length;
            param.handle_value.value.val = prep_write_req_param.req_param\
                [reliable_write_count].handle_value.value.val;

            retval = BT_att_send_prepare_write_req
                     (
                         att_handle,
                         &param
                     );
            reliable_write_count++;
        }
        else
        {
            /* Execute the queued write */
            req_param.flags = ATT_EXECUTE_WRITE_EXEC_FLAG;
            retval = BT_att_send_execute_write_req( att_handle, &req_param);
            reliable_write_count = 0;
            for (indx = 0; indx < prep_write_req_param.count; indx ++)
            {
                BT_free_mem(prep_write_req_param.req_param[indx].handle_value.value.val);
            }
        }

    }
    return retval;
}
API_RESULT gatt_handle_error_response
           (
               /* IN */ ATT_HANDLE   * att_handle,
               /* IN */ UCHAR          response_code
           )
{
    UCHAR id;
    GATT_CONTEXT * ctx;
    API_RESULT retval;

    /* Get the context */
    retval = gatt_search_context (att_handle, &id);
    if (API_SUCCESS != retval)
    {
        GATT_ERR ("Failed to get context - 0x%04X\n", retval);
        return retval;
    }

    ctx = &gatt_ctx[id];

    if ((ATT_ATTRIBUTE_NOT_FOUND == response_code) ||
        (ATT_UNSUPPORTED_GROUP_TYPE == response_code))
    {
        if ((GATT_PS_DISCOVERY == ctx->proc) || (GATT_SS_DISCOVERY == ctx->proc))
        {
            GATT_TRC ("Primary Service search complete..\n");
            gatt_handle_service_search_complete (ctx);
            return API_SUCCESS;
        }
        else if (GATT_IS_DISCOVERY == ctx->proc)
        {
            gatt_handle_included_service_search_complete (ctx);
            return API_SUCCESS;
        }
        else if ((GATT_CHAR_DISCOVERY & ctx->proc) && (BT_FALSE == char_desc_start))
        {
            gatt_handle_characteristics_search_complete (ctx);
            return API_SUCCESS;
        }
        else if (GATT_CHAR_DESC_DISCOVERY & ctx->proc)
        {
            gatt_handle_char_desc_search_complete (ctx);
            /* Reset Char Descriptor Discovery Start flag */
            char_desc_start = BT_FALSE;
            return API_SUCCESS;
        }
    }
    else if (ATT_INSUFFICIENT_AUTHORIZATION == response_code)
    {
        /**
         * Add initiation of necessary Security Procedures, Reinitiate Search on
         * successful completion
         */
    }
    else
    {
        /* TODO: Determinde the behavior */
    }

    /* Free the context */
    gatt_free_context (att_handle);

    return API_FAILURE;
}


API_RESULT gatt_cb
           (
               ATT_HANDLE    * handle,
               UCHAR         att_event,
               API_RESULT    event_result,
               UCHAR         * event_data,
               UINT16        event_datalen
           )
{
    int            i;
    UINT16         arg_len;
    UINT16         attr_handle;
    UINT16         offset;
    UINT16         mtu;
    UCHAR          op_code;
    UCHAR          rsp_code;
    API_RESULT     retval;

    UCHAR event_type;
    UCHAR id;
    UINT16 proc;

    GATT_TRC
    ("[ATT]:[0x%02X]: Received ATT Event 0x%02X with result 0x%04X",
    *handle,att_event,event_result);

    /* Dump received data */
    GATT_debug_dump_bytes (event_data, event_datalen);

    event_type = GATT_INVALID_EVENT;

    /* Get the context */
    proc = 0;
    (void)proc;    /*fix build warning: set but never used.*/
    retval = gatt_search_context (handle, &id);
    if (API_SUCCESS == retval)
    {
        proc = gatt_ctx[id].proc;
    }

    switch(att_event)
    {
#ifdef ATT_ON_BR_EDR_SUPPORT
    case ATT_CONNECT_REQ:
        BT_att_connect_rsp (handle,ATT_ACCEPT_CONNECTION);
        break;
#endif /* ATT_ON_BR_EDR_SUPPORT */

    case ATT_CONNECTION_IND:
        GATT_TRC (
        "[0x%02X]:Received Connection Indication, Result 0x%04X!\n",
        *handle,event_result);

        event_type = att_event;
        break;

    case ATT_DISCONNECTION_IND:
        GATT_TRC (
        "[0x%02X]:Received Disconnection Indication, Result 0x%04X!\n",
        *handle,event_result);

        event_type = att_event;
        break;

    case ATT_ERROR_RSP:
        BT_UNPACK_LE_1_BYTE(&op_code,event_data);
        BT_UNPACK_LE_2_BYTE(&attr_handle, event_data+1);
        BT_UNPACK_LE_1_BYTE(&rsp_code,event_data+3);
        GATT_TRC (
        "Received Error Response, for Op-Code 0x%02X for Handle 0x%04X, Rsp "
        "Code 0x%02X!\n", op_code, attr_handle,rsp_code);
        (void)op_code;     /*fix build warning: set but never used.*/
        (void)rsp_code;    /*fix build warning: set but never used.*/
        if (API_SUCCESS != gatt_handle_error_response(handle, event_data[3]))
        {
            event_type = att_event;
        }
        break;

    case ATT_XCHNG_MTU_RSP:
        if (NULL == event_data)
        {
            break;
        }
        BT_UNPACK_LE_2_BYTE(&mtu, event_data);
        GATT_TRC (
        "Received Exchange MTU Response with Result 0x%04X. MTU Size "
        "= 0x%04X!\n",event_result,mtu);
        extern void prvGattMqttExchangeRsp(ATT_HANDLE * handle, uint16_t mtu);    /*fix build warning: function declared implicitly.*/
        prvGattMqttExchangeRsp(handle, mtu);
        event_type = att_event;
        break;

    case ATT_FIND_INFO_RSP:
        GATT_TRC ("Received Find Information Response Opcode!\n");
        if (NULL == event_data)
        {
            break;
        }

        arg_len = event_data[0];

        event_datalen --;
        event_data++;

        if (API_SUCCESS != gatt_handle_find_info_response
                           (
                               handle,
                               arg_len,
                               event_data,
                               event_datalen
                           ))
        {
            event_type = att_event;
        }
        break;
    case ATT_READ_BY_TYPE_RSP:
        GATT_TRC ("Received Read Type Response Opcode 0x%04X!\n",event_result);
        if (NULL == event_data)
        {
            break;
        }

        arg_len = event_data[0];

        event_datalen --;
        event_data++;

        if (API_SUCCESS != gatt_handle_read_by_type_response
                           (
                               handle,
                               arg_len,
                               event_data,
                               event_datalen
                           ))
        {
            event_type = att_event;
        }
        break;
    case ATT_READ_BY_GROUP_RSP:
        if (NULL == event_data)
        {
            break;
        }

        GATT_TRC ("Received Read Group Type Response Opcode!\n");

        arg_len = event_data[0];

        event_datalen --;
        event_data++;

        if (API_SUCCESS != gatt_handle_read_by_group_response
                           (
                                handle,
                                arg_len,
                                event_data,
                                event_datalen
                           ))
        {
            event_type = att_event;
        }
        break;
    case ATT_FIND_BY_TYPE_VAL_RSP:
        GATT_TRC ("Received Find by Type Value Response Opcode!\n");

        if (NULL == event_data)
        {
            break;
        }

        GATT_TRC ("Found Handle        End Found Handle\n");
        for (i = 0; i < event_datalen; i+=4)
        {
            BT_UNPACK_LE_2_BYTE(&attr_handle, (event_data+i));
            GATT_TRC ("%04X",attr_handle);
            BT_UNPACK_LE_2_BYTE(&attr_handle, (event_data+i+2));
            GATT_TRC ("                  %04X\n", attr_handle);
        }

        if (API_SUCCESS != gatt_handle_find_by_type_value_response
                           (handle, event_data, event_datalen))
        {
            event_type = att_event;
        }
        break;

    case ATT_READ_RSP:
        GATT_TRC ("Received Read Response Opcode!\n");
        GATT_TRC ("Handle Value Received - \n");
        GATT_debug_dump_bytes(event_data, event_datalen);
        event_type = att_event;
        break;

    case ATT_READ_BLOB_RSP:
        GATT_TRC ("Received Read Response Opcode!\n");
        GATT_TRC ("Handle Value Received - \n");
        GATT_debug_dump_bytes(event_data, event_datalen);
        retval = gatt_handle_read_blob_response
                 (
                     handle,
                     event_data,
                     event_datalen
                  );
        GATT_TRC ("\n");
        break;

    case ATT_READ_MULTIPLE_RSP:
        GATT_TRC ("Received read multiple response Opcode!\n");

        GATT_TRC ("Byte stream of values received [not seperated by handle]");
        GATT_debug_dump_bytes(event_data, event_datalen);
        event_type = att_event;
        GATT_TRC ("\n");
        break;
    case ATT_WRITE_RSP:
        GATT_TRC ("Received Write Response Opcode!\n");

        event_type = att_event;
        break;

    case ATT_PREPARE_WRITE_RSP:
        GATT_TRC ("Prepare Write Response, Event Result 0x%04X\n",event_result);
        if (NULL != event_data)
        {
            BT_UNPACK_LE_2_BYTE(&attr_handle, event_data);
            GATT_TRC ("Handle - 0x%04X\n", attr_handle);
            BT_UNPACK_LE_2_BYTE(&offset, event_data+2);
            GATT_TRC ("Offset - 0x%04X\n", offset);
            GATT_TRC ("Handle Value Received - \n");
            GATT_debug_dump_bytes(event_data + 4, event_datalen - 4);
            gatt_handle_prepare_write_response (handle, event_data, event_datalen);
        }
        GATT_TRC ("\n");
        break;

    case ATT_EXECUTE_WRITE_RSP:
        GATT_TRC ("Received Execute Write Response\n");

        event_type = att_event;
        break;

    case ATT_HANDLE_VALUE_NTF:
        GATT_TRC ("Received HVN\n");
        BT_UNPACK_LE_2_BYTE(&attr_handle, event_data);
        GATT_TRC ("Handle - 0x%04X\n", attr_handle);
        GATT_TRC ("Handle Value Received - \n");
        GATT_debug_dump_bytes(event_data + 2, (event_datalen - 2));
        GATT_TRC ("\n");

        event_type = att_event;
        break;

    case ATT_HANDLE_VALUE_IND:
        GATT_TRC ("Received HVI\n");
        BT_UNPACK_LE_2_BYTE(&attr_handle, event_data);
        GATT_TRC ("Handle - 0x%04X\n", attr_handle);
        GATT_TRC ("Handle Value Received - \n");
        GATT_debug_dump_bytes(event_data + 2, (event_datalen-2));

        event_type = att_event;
        break;

    case ATT_HANDLE_VALUE_CNF:

        event_type = att_event;
        break;

    default:
        event_type = att_event;
        break;
    }

    /* Notify application of a valid event */
    if (GATT_INVALID_EVENT != event_type)
    {
        if (GATT_INVALID_CONTEXT != id)
        {
            /* Free the context */
            gatt_free_context (handle);
        }

        GATT_APP_CB_NTF
        (
            handle,
            event_type,
            event_result,
            event_data,
            event_datalen
        );
    }

    return API_SUCCESS;
}

/* ----------------------------------------------------- GATT API Definitions */

API_RESULT gatt_init (GATT_APP_CB cb)
{
    ATT_APPLICATION gatt_interface;
    UCHAR index;

    /* Reset Char Descriptor Discovery Start flag */
    char_desc_start = BT_FALSE;

    /* Initialize GATT contexts */
    for (index = 0; index < GATT_NUM_CONTEXTS; index++)
    {
        gatt_ctx[index].att_handle.device_id = DEVICE_HANDLE_INIT_VAL;
        gatt_ctx[index].att_handle.att_id = 0xFF;
    }

    /* Store the callback interface */
    gatt_app_cb = cb;

    /* Register ATT interface */
    gatt_interface.cb = gatt_cb;
    return BT_att_register(&gatt_interface);
}


API_RESULT gatt_xchg_mtu (ATT_HANDLE * handle, UINT16 mtu, UCHAR rsp)
{
    API_RESULT retval;

    /* Is Request or Response? */
    if (0x00 == rsp)
    {
        /* Request */
        retval = BT_att_send_mtu_xcnhg_req
                 (
                     handle,
                     &mtu
                 );
    }
    else /* if (0x01 == rsp) */
    {
        /* Response */
        retval = API_FAILURE;
    }

    return retval;
}

API_RESULT gatt_discover_service (ATT_HANDLE * att_handle, ATT_UUID uuid, UCHAR frmt, UINT16 type)
{
    ATT_HANDLE_RANGE range;
    API_RESULT retval;
    GATT_CONTEXT * ctx;
    UCHAR id;

    GATT_TRC ("Searching Context ...\n");
    retval = gatt_search_context (att_handle, &id);
    if (API_SUCCESS == retval)
    {
        GATT_ERR ("Context exists for device at %02X with operation for %04X\n",
            id, gatt_ctx[id].uuid);

        return API_FAILURE;
    }

    GATT_TRC ("Allocating Context ...\n");
    retval = gatt_alloc_context (att_handle, &id);
    if (API_SUCCESS != retval)
    {
        GATT_ERR ("Context allocation failed for device\n");

        return retval;
    }

    GATT_TRC ("Initiating Service Discovery Procedure..\n");

    /* Get the context */
    ctx = &gatt_ctx[id];

    /* Update context */
    ctx->proc = type;
    ctx->attr_index = 0;
    ctx->handle = ATT_ATTR_HANDLE_END_RANGE;
    ctx->uuid = uuid;
    ctx->frmt = frmt;

    range.start_handle = ATT_ATTR_HANDLE_START_RANGE;
    range.end_handle = ATT_ATTR_HANDLE_END_RANGE;

#ifdef ATT_SUPPORT_128_BIT_UUID
    if (0x0000 == uuid.uuid_16)
#else /* ATT_SUPPORT_128_BIT_UUID */
    if (0x0000 == uuid)
#endif /* ATT_SUPPORT_128_BIT_UUID */
    {
        ATT_READ_BY_GROUP_TYPE_REQ_PARAM param;

        param.range = range;
        param.uuid_format = frmt;
#ifdef ATT_SUPPORT_128_BIT_UUID
        param.group_type.uuid_16 = (GATT_PS_DISCOVERY == type)?
            GATT_PRIMARY_SERVICE: GATT_SECONDARY_SERVICE;
#else /* ATT_SUPPORT_128_BIT_UUID */
        param.group_type = (GATT_PS_DISCOVERY == type)?
            GATT_PRIMARY_SERVICE: GATT_SECONDARY_SERVICE;
#endif /* ATT_SUPPORT_128_BIT_UUID */

        retval = BT_att_send_read_by_group_req
                 (
                     att_handle,
                     &param
                 );
    }
    else
    {
        ATT_FIND_BY_TYPE_VAL_REQ_PARAM    param;
        ATT_VALUE attval;
        UCHAR value[ATT_128_BIT_UUID_SIZE];

        attval.val = value;
        attval.actual_len = 0U;
        if (ATT_16_BIT_UUID_FORMAT == frmt)
        {
#ifdef ATT_SUPPORT_128_BIT_UUID
            BT_PACK_LE_2_BYTE (value,&uuid.uuid_16);
#else /* ATT_SUPPORT_128_BIT_UUID */
            BT_PACK_LE_2_BYTE (value,&uuid);
#endif /* ATT_SUPPORT_128_BIT_UUID */
            attval.len = ATT_16_BIT_UUID_SIZE;
        }
#ifdef ATT_SUPPORT_128_BIT_UUID
        else
        {
            BT_PACK_LE_16_BYTE (value,&uuid.uuid_128.value);
            attval.len = ATT_128_BIT_UUID_SIZE;
        }
#endif /* ATT_SUPPORT_128_BIT_UUID */
        param.range = range;
        param.uuid = (GATT_PS_DISCOVERY == type)?
            GATT_PRIMARY_SERVICE: GATT_SECONDARY_SERVICE;
        param.value = attval;

        retval = BT_att_send_find_by_type_val_req
                 (
                     att_handle,
                     &param
                 );
    }

    if (API_SUCCESS != retval)
    {
        GATT_ERR ("ATT Request failed\n");

        /* Free the context */
        gatt_free_context (att_handle);
    }

    return retval;
}

API_RESULT gatt_discover_is (ATT_HANDLE * att_handle, UINT16 sh, UINT16 eh)
{
    API_RESULT retval;
    GATT_CONTEXT * ctx;
    UCHAR id;
    ATT_READ_BY_TYPE_REQ_PARAM param;

    GATT_TRC ("Searching Context ...\n");
    retval = gatt_search_context (att_handle, &id);
    if (API_SUCCESS == retval)
    {
        GATT_ERR ("Context exists for device at %02X with operation for %04X\n",
            id, gatt_ctx[id].uuid);

        return API_FAILURE;
    }

    GATT_TRC ("Allocating Context ...\n");
    retval = gatt_alloc_context (att_handle, &id);
    if (API_SUCCESS != retval)
    {
        GATT_ERR ("Context allocation failed for device\n");

        return retval;
    }

    GATT_TRC ("Initiating Relationship Discovery Procedure..\n");

    /* Get the context */
    ctx = &gatt_ctx[id];

    /* Update context */
    ctx->proc = GATT_IS_DISCOVERY;
    ctx->attr_index = 0;
    ctx->handle = eh;

    param.range.start_handle = sh;
    param.range.end_handle = eh;
#ifdef ATT_SUPPORT_128_BIT_UUID
    param.uuid.uuid_16 = GATT_INCLUDE;
#else /* ATT_SUPPORT_128_BIT_UUID */
    param.uuid = GATT_INCLUDE;
#endif /* ATT_SUPPORT_128_BIT_UUID */
    param.uuid_format = ATT_16_BIT_UUID_FORMAT;

    retval = BT_att_send_read_by_type_req
             (
                 att_handle,
                 &param
             );

    if (API_SUCCESS != retval)
    {
        GATT_ERR ("ATT Request failed\n");

        /* Free the context */
        gatt_free_context (att_handle);
    }

    return retval;
}

API_RESULT gatt_discover_char
           (
               ATT_HANDLE * att_handle,
               UINT16 sh,
               UINT16 eh,
               UINT16 uuid,
               UCHAR desc
           )
{
    API_RESULT retval;
    GATT_CONTEXT * ctx;
    UCHAR id;
    ATT_READ_BY_TYPE_REQ_PARAM param;

    GATT_TRC ("Searching Context ...\n");
    retval = gatt_search_context (att_handle, &id);
    if (API_SUCCESS == retval)
    {
        GATT_ERR ("Context exists for device at %02X with operation for %04X\n",
            id, gatt_ctx[id].uuid);

        return API_FAILURE;
    }

    GATT_TRC ("Allocating Context ...\n");
    retval = gatt_alloc_context (att_handle, &id);
    if (API_SUCCESS != retval)
    {
        GATT_ERR ("Context allocation failed for device\n");

        return retval;
    }

    GATT_TRC ("Initiating Characteristic Discovery Procedure..\n");

    /* Get the context */
    ctx = &gatt_ctx[id];

    /* Update context */
    ctx->proc = (desc)? (GATT_CHAR_DISCOVERY | GATT_CHAR_DESC_DISCOVERY):
                GATT_CHAR_DISCOVERY;
    ctx->attr_index = 0;
    ctx->handle = eh;
    ctx->se_handle = eh;

#ifdef ATT_SUPPORT_128_BIT_UUID
    ctx->uuid.uuid_16 = uuid;
#else /* ATT_SUPPORT_128_BIT_UUID */
    ctx->uuid = uuid;
#endif /* ATT_SUPPORT_128_BIT_UUID */

    param.range.start_handle = sh;
    param.range.end_handle = eh;
#ifdef ATT_SUPPORT_128_BIT_UUID
    param.uuid.uuid_16 = GATT_CHARACTERISTIC;
#else /* ATT_SUPPORT_128_BIT_UUID */
    param.uuid = GATT_CHARACTERISTIC;
#endif /* ATT_SUPPORT_128_BIT_UUID */
    param.uuid_format = ATT_16_BIT_UUID_FORMAT;

    retval = BT_att_send_read_by_type_req
             (
                 att_handle,
                 &param
             );

    if (API_SUCCESS != retval)
    {
        GATT_ERR ("ATT Request failed\n");

        /* Free the context */
        gatt_free_context (att_handle);
    }

    return retval;
}


API_RESULT gatt_discover_char_desc (ATT_HANDLE * att_handle, UINT16 sh, UINT16 eh)
{
    API_RESULT retval;
    GATT_CONTEXT * ctx;
    UCHAR id;
    ATT_FIND_INFO_REQ_PARAM param;

    GATT_TRC ("Searching Context ...\n");
    retval = gatt_search_context (att_handle, &id);
    if (API_SUCCESS == retval)
    {
        GATT_ERR ("Context exists for device at %02X with operation for %04X\n",
            id, gatt_ctx[id].uuid);

        return API_FAILURE;
    }

    GATT_TRC ("Allocating Context ...\n");
    retval = gatt_alloc_context (att_handle, &id);
    if (API_SUCCESS != retval)
    {
        GATT_ERR ("Context allocation failed for device\n");

        return retval;
    }

    GATT_TRC ("Initiating Characteristic Descriptor Discovery Procedure..\n");

    /* Get the context */
    ctx = &gatt_ctx[id];

    /* Update context */
    ctx->proc = GATT_CHAR_DESC_DISCOVERY;
    ctx->attr_index = 0;
    ctx->handle = eh;

    param.start_handle = sh;
    param.end_handle = eh;

    /* Set Char Descriptor Start flag */
    char_desc_start = BT_TRUE;

    retval = BT_att_send_find_info_req
             (
                 att_handle,
                 &param
             );

    if (API_SUCCESS != retval)
    {
        GATT_ERR ("ATT Request failed\n");

        /* Free the context */
        gatt_free_context (att_handle);
    }

    return retval;
}


API_RESULT gatt_char_read (ATT_HANDLE * att_handle, UINT16 sh, UINT16 eh, ATT_UUID uuid, UCHAR frmt)
{
    API_RESULT retval;
    GATT_CONTEXT * ctx;
    UCHAR id;
    ATT_READ_BY_TYPE_REQ_PARAM param;

    GATT_TRC ("Searching Context ...\n");
    retval = gatt_search_context (att_handle, &id);
    if (API_SUCCESS == retval)
    {
        GATT_ERR ("Context exists for device at %02X with operation for %04X\n",
            id, gatt_ctx[id].uuid);

        return API_FAILURE;
    }

    GATT_TRC ("Allocating Context ...\n");
    retval = gatt_alloc_context (att_handle, &id);
    if (API_SUCCESS != retval)
    {
        GATT_ERR ("Context allocation failed for device\n");

        return retval;
    }

    GATT_TRC ("Initiating Characteristic Read Procedure..\n");

    /* Get the context */
    ctx = &gatt_ctx[id];
    ctx->proc = GATT_CHAR_READ;

#ifdef ATT_SUPPORT_128_BIT_UUID
    if (0x0000 == uuid.uuid_16)
#else /* ATT_SUPPORT_128_BIT_UUID */
    if (0x0000 == uuid)
#endif /* ATT_SUPPORT_128_BIT_UUID */
    {
        /* Update context */
        ctx->handle = sh;

        retval = BT_att_send_read_req
                 (
                     att_handle,
                     &sh
                 );

        if (API_SUCCESS != retval)
        {
            GATT_ERR ("ATT Request failed\n");

            /* Free the context */
            gatt_free_context (att_handle);
        }
    }
    else
    {
        /* Update context */
        ctx->handle = eh;
        ctx->uuid = uuid;

        param.range.start_handle = sh;
        param.range.end_handle = eh;
#ifdef ATT_SUPPORT_128_BIT_UUID
        param.uuid = uuid;
#else /* ATT_SUPPORT_128_BIT_UUID */
        param.uuid = uuid;
#endif /* ATT_SUPPORT_128_BIT_UUID */
        param.uuid_format = frmt;

        retval = BT_att_send_read_by_type_req
                 (
                     att_handle,
                     &param
                 );

        if (API_SUCCESS != retval)
        {
            GATT_ERR ("ATT Request failed\n");

            /* Free the context */
            gatt_free_context (att_handle);
        }
    }

    return retval;
}

API_RESULT gatt_char_read_long (ATT_HANDLE * att_handle, UINT16 hdl, UINT16 offset)
{
    ATT_READ_BLOB_REQ_PARAM param;
    API_RESULT retval;
    GATT_CONTEXT * ctx;
    UCHAR id;

    GATT_TRC ("Searching Context ...\n");
    retval = gatt_search_context (att_handle, &id);
    if (API_SUCCESS == retval)
    {
        GATT_ERR ("Context exists for device at %02X with operation for %04X\n",
            id, gatt_ctx[id].uuid);

        return API_FAILURE;
    }

    GATT_TRC ("Allocating Context ...\n");
    retval = gatt_alloc_context (att_handle, &id);
    if (API_SUCCESS != retval)
    {
        GATT_ERR ("Context allocation failed for device\n");

        return retval;
    }

    GATT_TRC ("Initiating Read Long Characteristic Procedure..\n");

        /* Get the context */
    ctx = &gatt_ctx[id];
    ctx->proc = GATT_CHAR_READ;
    ctx->handle = hdl;
    ctx->characteristic[id].val_offset = offset;

    param.handle = hdl;
    param.offset = offset;

    retval = BT_att_send_read_blob_req
             (
                 att_handle,
                 &param
             );
    if (API_SUCCESS != retval)
    {
        GATT_ERR ("ATT Request failed\n");

        /* Free the context */
        gatt_free_context (att_handle);
    }
    return retval;
}

API_RESULT gatt_char_read_multiple (ATT_HANDLE * att_handle, ATT_READ_MULTIPLE_REQ_PARAM * param)
{
    API_RESULT retval;
    GATT_CONTEXT * ctx;
    UCHAR id;

    GATT_TRC ("Searching Context ...\n");
    retval = gatt_search_context (att_handle, &id);
    if (API_SUCCESS == retval)
    {
        GATT_ERR ("Context exists for device at %02X with operation for %04X\n",
            id, gatt_ctx[id].uuid);

        return API_FAILURE;
    }

    GATT_TRC ("Allocating Context ...\n");
    retval = gatt_alloc_context (att_handle, &id);
    if (API_SUCCESS != retval)
    {
        GATT_ERR ("Context allocation failed for device\n");

        return retval;
    }

    GATT_TRC ("Initiating Read Multiple Characteristic Procedure..\n");

        /* Get the context */
    ctx = &gatt_ctx[id];
    ctx->proc = GATT_CHAR_READ;

    retval = BT_att_send_read_multiple_req
             (
                 att_handle,
                 param
             );
    if (API_SUCCESS != retval)
    {
        GATT_ERR ("ATT Request failed\n");

        /* Free the context */
        gatt_free_context (att_handle);
    }
    return retval;
}

API_RESULT gatt_char_wr (ATT_HANDLE * att_handle, UINT16 hdl, UCHAR * value, UINT16 length, UCHAR rsp)
{
    ATT_WRITE_REQ_PARAM param;
    API_RESULT retval;
    GATT_CONTEXT * ctx;
    UCHAR id;

    GATT_TRC ("Searching Context ...\n");
    retval = gatt_search_context (att_handle, &id);
    if (API_SUCCESS == retval)
    {
        GATT_ERR ("Context exists for device at %02X with operation for %04X\n",
            id, gatt_ctx[id].uuid);

        return API_FAILURE;
    }

    GATT_TRC ("Allocating Context ...\n");
    retval = gatt_alloc_context (att_handle, &id);
    if (API_SUCCESS != retval)
    {
        GATT_ERR ("Context allocation failed for device\n");

        return retval;
    }

    GATT_TRC ("Initiating Characteristic Write Procedure..\n");

    /* Get the context */
    ctx = &gatt_ctx[id];

    /* Update context */
    ctx->handle = hdl;
    ctx->proc = GATT_CHAR_WRITE;

    param.handle = hdl;
    param.value.val = value;
    param.value.len = length;

    if (0x01 == rsp)
    {
        retval = BT_att_send_write_req
                 (
                     att_handle,
                     &param
                 );

        if (API_SUCCESS != retval)
        {
            GATT_ERR ("ATT Request failed\n");

            /* Free the context */
            gatt_free_context (att_handle);
        }
    }
    else
    {
        retval = BT_att_send_write_cmd
                 (
                     att_handle,
                     &param
                 );

        /* Free the context */
        gatt_free_context (att_handle);
    }

    return retval;
}

API_RESULT gatt_char_wr_long (ATT_HANDLE * att_handle, ATT_PREPARE_WRITE_REQ_PARAM * prepare_write_param)
{
    API_RESULT retval;
    GATT_CONTEXT * ctx;
    UCHAR id;

    GATT_TRC ("Searching Context ...\n");
    retval = gatt_search_context (att_handle, &id);
    if (API_SUCCESS == retval)
    {
        GATT_ERR ("Context exists for device at %02X with operation for %04X\n",
            id, gatt_ctx[id].uuid);

        return API_FAILURE;
    }

    GATT_TRC ("Allocating Context ...\n");
    retval = gatt_alloc_context (att_handle, &id);
    if (API_SUCCESS != retval)
    {
        GATT_ERR ("Context allocation failed for device\n");

        return retval;
    }

    GATT_TRC ("Initiating Characteristic Write Procedure..\n");

    /* Get the context */
    ctx = &gatt_ctx[id];

    /* Update context */
    ctx->handle = prepare_write_param->handle_value.handle;
    ctx->proc = GATT_CHAR_WRITE;
    ctx->characteristic[id].val_offset = prepare_write_param->offset;
    ctx->characteristic[id].val_length = prepare_write_param->handle_value.value.len;
    retval = BT_att_send_prepare_write_req
             (
                 att_handle,
                 prepare_write_param
             );
    if (API_SUCCESS == retval)
    {
        val_ptr = BT_alloc_mem(prepare_write_param->handle_value.value.len);
        if (NULL == prepare_write_param->handle_value.value.val)
        {
            GATT_ERR ("Failed to allocate memory of size 0x%04X\n",
            prepare_write_param->handle_value.value.len);
            return API_FAILURE;
        }
        BT_mem_copy
        (
            val_ptr,
            prepare_write_param->handle_value.value.val,
            prepare_write_param->handle_value.value.len
        );
    }
    return retval;
}

API_RESULT gatt_char_reliable_wr (ATT_HANDLE * att_handle, GATT_PREPARE_WRITE_REQ_PARAM * req_param)
{
    API_RESULT retval;
    GATT_CONTEXT * ctx;
    UCHAR id;
    ATT_PREPARE_WRITE_REQ_PARAM param;
    int index;

    GATT_TRC ("Searching Context ...\n");
    retval = gatt_search_context (att_handle, &id);
    if (API_SUCCESS == retval)
    {
        GATT_ERR ("Context exists for device at %02X with operation for %04X\n",
            id, gatt_ctx[id].uuid);

        return API_FAILURE;
    }

    GATT_TRC ("Allocating Context ...\n");
    retval = gatt_alloc_context (att_handle, &id);
    if (API_SUCCESS != retval)
    {
        GATT_ERR ("Context allocation failed for device\n");

        return retval;
    }

    GATT_TRC ("Initiating Characteristic Write Procedure..\n");

    /* Get the context */
    ctx = &gatt_ctx[id];

    /* Update context */
    ctx->proc = GATT_CHAR_RELIABLE_WRITE;
    ctx->handle = req_param->req_param[0].handle_value.handle;
    ctx->characteristic[id].val_offset = req_param->req_param[0].offset;
    ctx->characteristic[id].val_length = req_param->req_param[0].handle_value.value.len;

    param = req_param->req_param[0];

    retval = BT_att_send_prepare_write_req
             (
                 att_handle,
                 &param
             );
    if (API_SUCCESS == retval)
    {
        prep_write_req_param.count = req_param->count;

        for (index = 0; index < prep_write_req_param.count; index++)
        {
            prep_write_req_param.req_param[index].offset =\
                req_param->req_param[index].offset;
            prep_write_req_param.req_param[index].handle_value.handle =\
                req_param->req_param[0].handle_value.handle;
            prep_write_req_param.req_param[index].handle_value.value.len =\
                req_param->req_param[0].handle_value.value.len;

            prep_write_req_param.req_param[index].handle_value.value.val =\
                BT_alloc_mem
                (prep_write_req_param.req_param[index].handle_value.value.len);

            if (NULL == prep_write_req_param.req_param[index].handle_value.value.val)
            {
                GATT_ERR (
                "[APPL]: Failed to allocate memory of size 0x%04X\n",
                prep_write_req_param.req_param[index].handle_value.value.len);
                break;
            }

            BT_mem_copy
            (
                prep_write_req_param.req_param[index].handle_value.value.val,
                req_param->req_param[index].handle_value.value.val,
                prep_write_req_param.req_param[index].handle_value.value.len
            );
        }
    }

    reliable_write_count++;

    return retval;
}

API_RESULT gatt_char_hv_action
           (
               ATT_HANDLE * att_handle,
               UCHAR action,
               UINT16 hdl,
               UCHAR * value,
               UINT16 length
           )
{
    ATT_HNDL_VAL_NTF_PARAM param;
    API_RESULT retval;
    GATT_CONTEXT * ctx;
    UCHAR id;

    GATT_TRC ("Searching Context ...\n");
    retval = gatt_search_context (att_handle, &id);
    if (API_SUCCESS == retval)
    {
        GATT_ERR ("Context exists for device at %02X with operation for %04X\n",
            id, gatt_ctx[id].uuid);

        return API_FAILURE;
    }

    GATT_TRC ("Allocating Context ...\n");
    retval = gatt_alloc_context (att_handle, &id);
    if (API_SUCCESS != retval)
    {
        GATT_ERR ("Context allocation failed for device\n");

        return retval;
    }

    GATT_TRC ("Initiating Characteristic Write Procedure..\n");

    /* Get the context */
    ctx = &gatt_ctx[id];

    /* Update context */
    ctx->handle = hdl;

    param.handle = hdl;
    param.value.val = value;
    param.value.len = length;

    if (GATT_HV_NTF == action)
    {
        GATT_TRC ("Initiating HVN Procedure..\n");

        retval = BT_att_send_hndl_val_ntf
                 (
                     att_handle,
                     &param
                 );
    }
    else if (GATT_HV_IND == action)
    {
        GATT_TRC ("Initiating HVI Procedure..\n");

        retval = BT_att_send_hndl_val_ind
                 (
                     att_handle,
                     &param
                 );
    }
    else if (GATT_HV_CNF == action)
    {
        GATT_TRC ("Initiating HVC Procedure..\n");

        retval = BT_att_send_hndl_val_cnf (att_handle);
    }

    /* Free the context */
    gatt_free_context (att_handle);

    return retval;
}

#endif /* GATT_CLIENT */
#endif /* BT_4_0 */

