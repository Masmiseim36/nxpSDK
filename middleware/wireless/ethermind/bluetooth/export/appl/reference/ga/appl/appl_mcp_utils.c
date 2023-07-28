/**
 *  \file appl_mcp_utils.c
 *
 *  Reference application for GA MCP profile utils
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */

#include "appl_mcp_utils.h"
#include "appl_ga_utils.h"
#include "appl_utils.h"

#ifdef GA_MCP

void appl_mcp_utils_parse_and_display_playing_order(UINT8 playing_order)
{
    switch(playing_order)
    {
    case MCP_PLAYING_ORDER_SINGLE_ONCE:
        APPL_TRC("Single once (0x%02X)\n", playing_order);
        break;

    case MCP_PLAYING_ORDER_SINGLE_REPEAT:
        APPL_TRC("Single repeat (0x%02X)\n", playing_order);
        break;

    case MCP_PLAYING_ORDER_SINGLE_IN_ORDER_ONCE:
        APPL_TRC("In order once (0x%02X)\n", playing_order);
        break;

    case MCP_PLAYING_ORDER_SINGLE_IN_ORDER_REPEAT:
        APPL_TRC("In order repeat (0x%02X)\n", playing_order);
        break;

    case MCP_PLAYING_ORDER_SINGLE_OLDEST_ONCE:
        APPL_TRC("Oldest once (0x%02X)\n", playing_order);
        break;

    case MCP_PLAYING_ORDER_SINGLE_OLDEST_REPEAT:
        APPL_TRC("Oldest repeat (0x%02X)\n", playing_order);
        break;

    case MCP_PLAYING_ORDER_SINGLE_NEWEST_ONCE:
        APPL_TRC("Newest once (0x%02X)\n", playing_order);
        break;

    case MCP_PLAYING_ORDER_SINGLE_NEWEST_REPEAT:
        APPL_TRC("Newest repeat (0x%02X)\n", playing_order);
        break;

    case MCP_PLAYING_ORDER_SINGLE_SHUFFLE_ONCE:
        APPL_TRC("Shuffle once (0x%02X)\n", playing_order);
        break;

    case MCP_PLAYING_ORDER_SINGLE_SHUFFLE_REPEAT:
        APPL_TRC("Shuffle repeat (0x%02X)\n", playing_order);
        break;

    default:
        APPL_ERR
        (
            "UnHandled Playing Orders Value (0x%02X) !\n",
            playing_order
        );
    }

    return;
}

void appl_mcp_utils_parse_and_display_playing_orders_supp(UINT16 playing_order_supp)
{
    if (MCP_PLAYING_ORDER_SUPP_SINGLE_ONCE & playing_order_supp)
    {
        APPL_TRC
        (
            "\t-Single once (0x%04X)\n",
            MCP_PLAYING_ORDER_SUPP_SINGLE_ONCE
        );
    }
    if (MCP_PLAYING_ORDER_SUPP_SINGLE_REPEAT & playing_order_supp)
    {
        APPL_TRC
        (
            "\t-Single repeat (0x%04X)\n",
            MCP_PLAYING_ORDER_SUPP_SINGLE_REPEAT
        );
    }
    if (MCP_PLAYING_ORDER_SUPP_IN_ORDER_ONCE & playing_order_supp)
    {
        APPL_TRC
        (
            "\t-In order once (0x%04X)\n",
            MCP_PLAYING_ORDER_SUPP_IN_ORDER_ONCE
        );
    }
    if (MCP_PLAYING_ORDER_SUPP_IN_ORDER_REPEAT & playing_order_supp)
    {
        APPL_TRC
        (
            "\t-In order repeat (0x%04X)\n",
            MCP_PLAYING_ORDER_SUPP_IN_ORDER_REPEAT
        );
    }
    if (MCP_PLAYING_ORDER_SUPP_OLDEST_ONCE & playing_order_supp)
    {
        APPL_TRC
        (
            "\t-Oldest once (0x%04X)\n",
            MCP_PLAYING_ORDER_SUPP_OLDEST_ONCE
        );
    }
    if (MCP_PLAYING_ORDER_SUPP_OLDEST_REPEAT & playing_order_supp)
    {
        APPL_TRC
        (
            "\t-Oldest repeat (0x%04X)\n",
            MCP_PLAYING_ORDER_SUPP_OLDEST_REPEAT
        );
    }
    if (MCP_PLAYING_ORDER_SUPP_NEWEST_ONCE & playing_order_supp)
    {
        APPL_TRC
        (
            "\t-Newest once (0x%04X)\n",
            MCP_PLAYING_ORDER_SUPP_NEWEST_ONCE
        );
    }
    if (MCP_PLAYING_ORDER_SUPP_NEWEST_REPEAT & playing_order_supp)
    {
        APPL_TRC
        (
            "\t-Newest repeat (0x%04X)\n",
            MCP_PLAYING_ORDER_SUPP_NEWEST_REPEAT
        );
    }
    if (MCP_PLAYING_ORDER_SUPP_SHUFFLE_ONCE & playing_order_supp)
    {
        APPL_TRC
        (
            "\t-Shuffle once (0x%04X)\n",
            MCP_PLAYING_ORDER_SUPP_SHUFFLE_ONCE
        );
    }
    if (MCP_PLAYING_ORDER_SUPP_SHUFFLE_REPEAT & playing_order_supp)
    {
        APPL_TRC
        (
            "\t-Shuffle repeat (0x%04X)\n",
            MCP_PLAYING_ORDER_SUPP_SHUFFLE_REPEAT
        );
    }

    return;
}

void appl_mcp_utils_parse_and_display_media_state(UINT8 media_state)
{
    switch(media_state)
    {
    case MCP_MEDIA_STATE_INACTIVE:
        APPL_TRC("Inactive (0x%02X)\n", media_state);
        break;

    case MCP_MEDIA_STATE_PLAYING:
        APPL_TRC("Playing (0x%02X)\n", media_state);
        break;

    case MCP_MEDIA_STATE_PAUSED:
        APPL_TRC("Paused (0x%02X)\n", media_state);
        break;

    case MCP_MEDIA_STATE_SEEKING:
        APPL_TRC("Seeking (0x%02X)\n", media_state);
        break;

    default:
        APPL_ERR("UnHandled Media State Value (0x%04X) !\n", media_state);
    }

    return;
}

void appl_mcp_utils_parse_and_display_mcp_opc_supp(UINT32 mcp_opc_supp)
{
    if (MCP_CP_OPC_SUPP_PLAY & mcp_opc_supp)
    {
        APPL_TRC("\t-Play (0x%08X)\n", MCP_CP_OPC_SUPP_PLAY);
    }
    if (MCP_CP_OPC_SUPP_PAUSE & mcp_opc_supp)
    {
        APPL_TRC("\t-Pause (0x%08X)\n", MCP_CP_OPC_SUPP_PAUSE);
    }
    if (MCP_CP_OPC_SUPP_FAST_REW & mcp_opc_supp)
    {
        APPL_TRC("\t-Fast Rewind (0x%08X)\n", MCP_CP_OPC_SUPP_FAST_REW);
    }
    if (MCP_CP_OPC_SUPP_FAST_FWD & mcp_opc_supp)
    {
        APPL_TRC("\t-Fast Forward (0x%08X)\n", MCP_CP_OPC_SUPP_FAST_FWD);
    }
    if (MCP_CP_OPC_SUPP_STOP & mcp_opc_supp)
    {
        APPL_TRC("\t-Stop (0x%08X)\n", MCP_CP_OPC_SUPP_STOP);
    }
    if (MCP_CP_OPC_SUPP_MOVE_RELATIVE & mcp_opc_supp)
    {
        APPL_TRC("\t-Move Relative (0x%08X)\n", MCP_CP_OPC_SUPP_MOVE_RELATIVE);
    }
    if (MCP_CP_OPC_SUPP_PREV_SEG & mcp_opc_supp)
    {
        APPL_TRC("\t-Previous Segment (0x%08X)\n", MCP_CP_OPC_SUPP_PREV_SEG);
    }
    if (MCP_CP_OPC_SUPP_NEXT_SEG & mcp_opc_supp)
    {
        APPL_TRC("\t-Next Segment (0x%08X)\n", MCP_CP_OPC_SUPP_NEXT_SEG);
    }
    if (MCP_CP_OPC_SUPP_FIRST_SEG & mcp_opc_supp)
    {
        APPL_TRC("\t-First Segment (0x%08X)\n", MCP_CP_OPC_SUPP_FIRST_SEG);
    }
    if (MCP_CP_OPC_SUPP_LAST_SEG & mcp_opc_supp)
    {
        APPL_TRC("\t-Last Segment (0x%08X)\n", MCP_CP_OPC_SUPP_LAST_SEG);
    }
    if (MCP_CP_OPC_SUPP_GOTO_SEG & mcp_opc_supp)
    {
        APPL_TRC("\t-Goto Segment (0x%08X)\n", MCP_CP_OPC_SUPP_GOTO_SEG);
    }
    if (MCP_CP_OPC_SUPP_PREV_TRACK & mcp_opc_supp)
    {
        APPL_TRC("\t-Previous Track (0x%08X)\n", MCP_CP_OPC_SUPP_PREV_TRACK);
    }
    if (MCP_CP_OPC_SUPP_NEXT_TRACK & mcp_opc_supp)
    {
        APPL_TRC("\t-Next Track (0x%08X)\n", MCP_CP_OPC_SUPP_NEXT_TRACK);
    }
    if (MCP_CP_OPC_SUPP_FIRST_TRACK & mcp_opc_supp)
    {
        APPL_TRC("\t-First Track (0x%08X)\n", MCP_CP_OPC_SUPP_FIRST_TRACK);
    }
    if (MCP_CP_OPC_SUPP_LAST_TRACK & mcp_opc_supp)
    {
        APPL_TRC("\t-Last Track (0x%08X)\n", MCP_CP_OPC_SUPP_LAST_TRACK);
    }
    if (MCP_CP_OPC_SUPP_GOTO_TRACK & mcp_opc_supp)
    {
        APPL_TRC("\t-Goto Track (0x%08X)\n", MCP_CP_OPC_SUPP_GOTO_TRACK);
    }
    if (MCP_CP_OPC_SUPP_PREV_GRP & mcp_opc_supp)
    {
        APPL_TRC("\t-Previous Group (0x%08X)\n", MCP_CP_OPC_SUPP_PREV_GRP);
    }
    if (MCP_CP_OPC_SUPP_NEXT_GRP & mcp_opc_supp)
    {
        APPL_TRC("\t-Next Group (0x%08X)\n", MCP_CP_OPC_SUPP_NEXT_GRP);
    }
    if (MCP_CP_OPC_SUPP_FIRST_GRP & mcp_opc_supp)
    {
        APPL_TRC("\t-First Group (0x%08X)\n", MCP_CP_OPC_SUPP_FIRST_GRP);
    }
    if (MCP_CP_OPC_SUPP_LAST_GRP & mcp_opc_supp)
    {
        APPL_TRC("\t-Last Group (0x%08X)\n", MCP_CP_OPC_SUPP_LAST_GRP);
    }
    if (MCP_CP_OPC_SUPP_GOTO_GRP & mcp_opc_supp)
    {
        APPL_TRC("\t-Goto Group (0x%08X)\n", MCP_CP_OPC_SUPP_GOTO_GRP);
    }

    return;
}

void appl_mcp_utils_display_ga_result(GA_RESULT result, UINT16 svc_type)
{
    GA_RESULT  retval;

    retval = GA_FAILURE;

    /* Checking for common GA_RESULT */
    retval = appl_ga_utils_display_ga_result(result);

    if (GA_FAILURE == retval)
    {
        if ((GA_SERVICE_GMCS == svc_type) ||
            (GA_SERVICE_MCS == svc_type))
        {
            appl_mcp_utils_parse_and_display_mcs_err_codes(result);
        }
        else if (GA_SERVICE_UUID_OTS == svc_type)
        {
            appl_mcp_utils_parse_and_display_ots_err_codes(result);
        }
        else
        {
            APPL_ERR("Invalid Service: 0x%04X !\n", svc_type);
        }
    }

    return;
}

void appl_mcp_utils_parse_and_display_ots_err_codes(GA_RESULT result)
{
    switch(result)
    {
    /* Application error code */
    case OTP_ERR_CODE_WRITE_REQUEST_REJECTED:
        APPL_ERR("Write Request Rejected !");
        break;

    case OTP_ERR_CODE_OBJECT_NOT_SELECTED:
        APPL_ERR("Object Not Selected !");
        break;

    case OTP_ERR_CODE_CONCURRENCY_LIMIT_EXCEEDED:
        APPL_ERR("Concurrency Limit Exceeded !");
        break;

    case OTP_ERR_CODE_OBJECT_NAME_ALREADY_EXISTS:
        APPL_ERR("Object Name Already Exists !");
        break;

    /* Implementation related error codes */
    case GA_OTP_INVALID_STATE:
        APPL_ERR("Invalid State !");
        break;

    case GA_OTP_INVALID_PARAMS:
        APPL_ERR("Invalid Params !");
        break;

    case GA_OTP_CNTX_ALLOC_FAILED:
        APPL_ERR("Context allocation failed !");
        break;

    case GA_OTP_IN_BUSY_STATE:
        APPL_ERR("In Busy State !");
        break;

    case GA_OTP_CNTX_NOT_FOUND:
        APPL_ERR("Context not found !");
        break;

    case GA_OTP_SESSION_EXISTS:
        APPL_ERR("Session Exists !");
        break;

    default:
        APPL_ERR("Invalid Code !");
    }

    APPL_TRC(" (0x%04X)\n", result);
    return;
}

void appl_mcp_utils_parse_and_display_mcs_err_codes(GA_RESULT result)
{
    switch(result)
    {
    /* Application error code */
    case MCP_ERR_CODE_VALUE_CHANGED_DURING_READ_LONG:
        APPL_ERR("Value Changed During Read Long !");
        break;

    /* Implementation related error codes */
    case GA_MCP_INVALID_STATE:
        APPL_ERR("Invalid State !");
        break;

    case GA_MCP_INVALID_PARAMS:
        APPL_ERR("Invalid Params !");
        break;

    case GA_MCP_CNTX_ALLOC_FAILED:
        APPL_ERR("Context allocation failed !");
        break;

    case GA_MCP_IN_BUSY_STATE:
        APPL_ERR("In Busy State !");
        break;

    case GA_MCP_CNTX_NOT_FOUND:
        APPL_ERR("Context not found !");
        break;

    case GA_MCP_INVALID_HANDLE:
        APPL_ERR("Invalid Handle !");
        break;

    default:
        APPL_ERR("Invalid Code !");
    }

    APPL_TRC(" (0x%04X)\n", result);
    return;
}

#ifdef MCP_SUPPORT_OBJECT_TRANSFER
void appl_mcp_utils_parse_and_display_object_type(void * object_type)
{
    UINT16  obj_type;

    obj_type = *((UINT16*)object_type);

    switch(obj_type)
    {
    case GA_OTS_OBJ_TYPE_MPLAYER_ICON:
        APPL_TRC("Media Player Icon Object Type (0x%04X)\n", obj_type);
        break;
    case GA_OTS_OBJ_TYPE_TRACK_SEGMENTS:
        APPL_TRC("Track Segments Object Type (0x%04X)\n", obj_type);
        break;
    case GA_OTS_OBJ_TYPE_TRACK:
        APPL_TRC("Track Object Type (0x%04X)\n", obj_type);
        break;
    case GA_OTS_OBJ_TYPE_GROUP:
        APPL_TRC("Group Object Type (0x%04X)\n", obj_type);
        break;
    default:
        APPL_ERR("UnHandled Object Type (0x%04X) !\n", obj_type);
        break;
    }

    return;
}

void appl_mcp_utils_parse_and_display_obj_prop(void * data, UINT16 datalen)
{
    UINT32 obj_prop;

    obj_prop = *((UINT32*)data);

    APPL_TRC("Len: 0x%02X,  Object Properties: 0x%08X\n", datalen, obj_prop);
    if (obj_prop & MCP_OTS_OBJ_PROP_DELETE)
    {
        APPL_TRC("\t-Delete: True\n");
    }
    else
    {
        APPL_TRC("\t-Delete: False\n");
    }

    if (obj_prop & MCP_OTS_OBJ_PROP_EXECUTE)
    {
        APPL_TRC("\t-Execute: True\n");
    }
    else
    {
        APPL_TRC("\t-Execute: False\n");
    }

    if (obj_prop & MCP_OTS_OBJ_PROP_READ)
    {
        APPL_TRC("\t-Read: True\n");
    }
    else
    {
        APPL_TRC("\t-Read: False\n");
    }

    if (obj_prop & MCP_OTS_OBJ_PROP_WRITE)
    {
        APPL_TRC("\t-Write: True\n");
    }
    else
    {
        APPL_TRC("\t-Write: False\n");
    }

    if (obj_prop & MCP_OTS_OBJ_PROP_APPEND)
    {
        APPL_TRC("\t-Append: True\n");
    }
    else
    {
        APPL_TRC("\t-Append: False\n");
    }

    if (obj_prop & MCP_OTS_OBJ_PROP_TRUNCATE)
    {
        APPL_TRC("\t-Truncate: True\n");
    }
    else
    {
        APPL_TRC("\t-Truncate: False\n");
    }

    if (obj_prop & MCP_OTS_OBJ_PROP_PATCH)
    {
        APPL_TRC("\t-Patch: True\n");
    }
    else
    {
        APPL_TRC("\t-Patch: False\n");
    }

    if (obj_prop & MCP_OTS_OBJ_PROP_MARK)
    {
        APPL_TRC("\t-Mark: True\n");
    }
    else
    {
        APPL_TRC("\t-Mark: False\n");
    }

    return;
}

void appl_mcp_utils_parse_and_display_list_filter(UINT8 * data, UINT16 datalen)
{
    UCHAR   index;
    UINT16  obj_type;
    UINT16  data16;
    UINT32  data32;

    APPL_TRC("Len: 0x%02X,  Filter: ", (UCHAR)sizeof(UINT8));
    switch(data[0U])
    {
    case MCP_OTS_OBJ_LIST_FILTER_NO_FILTER:
        APPL_TRC("No Filter (everything passes) (0x%02X)\n", data[0U]);
        break;

    case MCP_OTS_OBJ_LIST_FILTER_NAME_STARTS_WITH:
        APPL_TRC("Name Starts With (0x%02X)\n", data[0U]);
        APPL_TRC("Len: 0x%02X,  String: ", (datalen-1U));
        for (index = 1U; index < datalen; index++)
        {
            APPL_TRC("%c", data[index]);
        }
        APPL_TRC("\n");
        break;

    case MCP_OTS_OBJ_LIST_FILTER_NAME_ENDS_WITH:
        APPL_TRC("Name Ends With (0x%02X)\n", data[0U]);
        APPL_TRC("Len: 0x%02X,  String: ", (datalen-1U));
        for (index = 1U; index < datalen; index++)
        {
            APPL_TRC("%c", data[index]);
        }
        APPL_TRC("\n");
        break;

    case MCP_OTS_OBJ_LIST_FILTER_NAME_CONTAINS:
        APPL_TRC("Name Contains (0x%02X)\n", data[0U]);
        APPL_TRC("Len: 0x%02X,  String: ", (datalen-1U));
        for (index = 1U; index < datalen; index++)
        {
            APPL_TRC("%c", data[index]);
        }
        APPL_TRC("\n");
        break;

    case MCP_OTS_OBJ_LIST_FILTER_NAME_IS_EXACTLY:
        APPL_TRC("Name is Exactly (0x%02X)\n", data[0U]);
        APPL_TRC("Len: 0x%02X,  String: ", (datalen-1U));
        for (index = 1U; index < datalen; index++)
        {
            APPL_TRC("%c", data[index]);
        }
        APPL_TRC("\n");
        break;

    case MCP_OTS_OBJ_LIST_FILTER_OBJ_TYPE:
        APPL_TRC("Object Type (UUID) (0x%02X)\n", data[0U]);
        APPL_TRC("Len: 0x%02X,  UUID: ", (datalen-1U));
        GA_UNPACK_LE_2_BYTE(&obj_type, &data[1U]);
        appl_mcp_utils_parse_and_display_object_type(&obj_type);
        break;

    case MCP_OTS_OBJ_LIST_FILTER_CREAT_BTWN:
        APPL_TRC
        (
            "Created between (inclusive): timestamp1 <= t <= timestamp2 (0x%02X)\n",
            data[0U]
        );
        GA_UNPACK_LE_2_BYTE(&data16, &data[1U]);
        APPL_TRC("Len: 0x07,  timestamp1:\n");
        APPL_TRC("\t-Year: %d (0x%04X)\n", data16, data16);
        APPL_TRC("\t-Month: %d (0x%02X)\n", data[3U], data[3U]);
        APPL_TRC("\t-Day: %d (0x%02X)\n", data[4U], data[4U]);
        APPL_TRC("\t-Hours: %d (0x%02X)\n", data[5U], data[5U]);
        APPL_TRC("\t-Minutes: %d (0x%02X)\n", data[6U], data[6U]);
        APPL_TRC("\t-Seconds: %d (0x%02X)\n", data[7U], data[7U]);
        GA_UNPACK_LE_2_BYTE(&data16, &data[8U]);
        APPL_TRC("Len: 0x07,  timestamp2:\n");
        APPL_TRC("\t-Year: %d (0x%04X)\n", data16, data16);
        APPL_TRC("\t-Month: %d (0x%02X)\n", data[10U], data[10U]);
        APPL_TRC("\t-Day: %d (0x%02X)\n", data[11U], data[11U]);
        APPL_TRC("\t-Hours: %d (0x%02X)\n", data[12U], data[12U]);
        APPL_TRC("\t-Minutes: %d (0x%02X)\n", data[13U], data[13U]);
        APPL_TRC("\t-Seconds: %d (0x%02X)\n", data[14U], data[14U]);
        break;

    case MCP_OTS_OBJ_LIST_FILTER_MODIFY_BTWN:
        APPL_TRC
        (
            "Modified between (inclusive): timestamp1 <= t <= timestamp2 (0x%02X)\n",
            data[0]
        );
        GA_UNPACK_LE_2_BYTE(&data16, &data[1U]);
        APPL_TRC("Len: 0x07,  timestamp1:\n");
        APPL_TRC("\t-Year: %d (0x%04X)\n", data16, data16);
        APPL_TRC("\t-Month: %d (0x%02X)\n", data[3U], data[3U]);
        APPL_TRC("\t-Day: %d (0x%02X)\n", data[4U], data[4U]);
        APPL_TRC("\t-Hours: %d (0x%02X)\n", data[5U], data[5U]);
        APPL_TRC("\t-Minutes: %d (0x%02X)\n", data[6U], data[6U]);
        APPL_TRC("\t-Seconds: %d (0x%02X)\n", data[7U], data[7U]);
        GA_UNPACK_LE_2_BYTE(&data16, &data[8U]);
        APPL_TRC("Len: 0x07,  timestamp2:\n");
        APPL_TRC("\t-Year: %d (0x%04X)\n", data16, data16);
        APPL_TRC("\t-Month: %d (0x%02X)\n", data[10U], data[10U]);
        APPL_TRC("\t-Day: %d (0x%02X)\n", data[11U], data[11U]);
        APPL_TRC("\t-Hours: %d (0x%02X)\n", data[12U], data[12U]);
        APPL_TRC("\t-Minutes: %d (0x%02X)\n", data[13U], data[13U]);
        APPL_TRC("\t-Seconds: %d (0x%02X)\n", data[14U], data[14U]);
        break;

    case MCP_OTS_OBJ_LIST_FILTER_CURR_SIZE_BTWN:
        APPL_TRC
        (
            "Current Size between (inclusive): size1 <= s <= size2 (0x%02X)\n",
            data[0U]
        );
        GA_UNPACK_LE_4_BYTE(&data32, &data[1U]);
        APPL_TRC
        (
            "Len: 0x%02X,  size1: 0x%08X\n",
            (UCHAR)sizeof(UINT32),
            data32
        );
        GA_UNPACK_LE_4_BYTE(&data32, &data[5U]);
        APPL_TRC
        (
            "Len: 0x%02X,  size2: 0x%08X\n",
            (UCHAR)sizeof(UINT32),
            data32
        );
        break;

    case MCP_OTS_OBJ_LIST_FILTER_ALLOC_SIZE_BTWN:
        APPL_TRC
        (
            "Allocated Size between (inclusive): size1 <= s <= size2 (0x%02X)\n",
            data[0U]
        );
        GA_UNPACK_LE_4_BYTE(&data32, &data[1U]);
        APPL_TRC
        (
            "Len: 0x%02X,  size1: 0x%08X\n",
            (UCHAR)sizeof(UINT32),
            data32
        );
        GA_UNPACK_LE_4_BYTE(&data32, &data[5U]);
        APPL_TRC
        (
            "Len: 0x%02X,  size2: 0x%08X\n",
            (UCHAR)sizeof(UINT32),
            data32
        );
        break;

    case MCP_OTS_OBJ_LIST_FILTER_MARKED_OBJS:
        APPL_TRC("Marked Objects (0x%02X)\n", data[0U]);
        break;
    }

    return;
}

void appl_mcp_utils_parse_and_display_obj_chgd_flag_bit(UINT8 flag)
{
    if (flag & MCP_OTS_OBJ_CHGD_FLAG_SOURCE_OF_CHNG)
    {
        APPL_TRC("\t-Source of Change: Client\n");
    }
    else
    {
        APPL_TRC("\t-Source of Change: Server\n");
    }

    if (flag & MCP_OTS_OBJ_CHGD_FLAG_OBJ_CONTENT_CHNG)
    {
        APPL_TRC("\t-Change occurred to the object contents: True\n");
    }
    else
    {
        APPL_TRC("\t-Change occurred to the object contents: False\n");
    }

    if (flag & MCP_OTS_OBJ_CHGD_FLAG_OBJ_METADATA_CHNG)
    {
        APPL_TRC("\t-Change occurred to the object metadata: True\n");
    }
    else
    {
        APPL_TRC("\t-Change occurred to the object metadata: False\n");
    }

    if (flag & MCP_OTS_OBJ_CHGD_FLAG_OBJ_CREATION)
    {
        APPL_TRC("\t-Object Creation: True\n");
    }
    else
    {
        APPL_TRC("\t-Object Creation: False\n");
    }

    if (flag & MCP_OTS_OBJ_CHGD_FLAG_OBJ_DELETION)
    {
        APPL_TRC("\t-Object Deletion: True\n");
    }
    else
    {
        APPL_TRC("\t-Object Deletion: False\n");
    }

    return;
}

void appl_mcp_par_and_display_oacp_feature(UINT32 oacp_feature)
{
    if (oacp_feature & MCP_OTS_OACP_CREATE_OPC_SUPPORTED)
    {
        APPL_TRC("\t-OACP Create Op Code Supported: True\n");
    }
    else
    {
        APPL_TRC("\t-OACP Create Op Code Supported: False\n");
    }

    if (oacp_feature & MCP_OTS_OACP_DELETE_OPC_SUPPORTED)
    {
        APPL_TRC("\t-OACP Delete Op Code Supported: True\n");
    }
    else
    {
        APPL_TRC("\t-OACP Delete Op Code Supported: False\n");
    }

    if (oacp_feature & MCP_OTS_OACP_CAL_CHECKSUM_OPC_SUPPORTED)
    {
        APPL_TRC("\t-OACP Calculate Checksum Op Code Supported: True\n");
    }
    else
    {
        APPL_TRC("\t-OACP Calculate Checksum Op Code Supported: False\n");
    }

    if (oacp_feature & MCP_OTS_OACP_EXECUTE_OPC_SUPPORTED)
    {
        APPL_TRC("\t-OACP Execute Op Code Supported: True\n");
    }
    else
    {
        APPL_TRC("\t-OACP Execute Op Code Supported: False\n");
    }

    if (oacp_feature & MCP_OTS_OACP_READ_OPC_SUPPORTED)
    {
        APPL_TRC("\t-OACP Read Op Code Supported: True\n");
    }
    else
    {
        APPL_TRC("\t-OACP Read Op Code Supported: False\n");
    }

    if (oacp_feature & MCP_OTS_OACP_WRITE_OPC_SUPPORTED)
    {
        APPL_TRC("\t-OACP Write Op Code Supported: True\n");
    }
    else
    {
        APPL_TRC("\t-OACP Write Op Code Supported: False\n");
    }

    if (oacp_feature & MCP_OTS_OACP_APPEND_ADDNL_DATA_TO_OBJS_SUPPORTED)
    {
        APPL_TRC("\t-Appending Additional Data to Objects Supported: True\n");
    }
    else
    {
        APPL_TRC("\t-Appending Additional Data to Objects Supported: False\n");
    }

    if (oacp_feature & MCP_OTS_OACP_TRUNC_OF_OBJS_SUPPORTED)
    {
        APPL_TRC("\t-Truncation of Objects Supported: True\n");
    }
    else
    {
        APPL_TRC("\t-Truncation of Objects Supported: False\n");
    }

    if (oacp_feature & MCP_OTS_OACP_PATCH_OF_OBJS_SUPPORTED)
    {
        APPL_TRC("\t-Patching of Objects Supported: True\n");
    }
    else
    {
        APPL_TRC("\t-Patching of Objects Supported: False\n");
    }

    if (oacp_feature & MCP_OTS_OACP_ABORT_OPC_SUPPORTED)
    {
        APPL_TRC("\t-OACP Abort Op Code Supported: True\n");
    }
    else
    {
        APPL_TRC("\t-OACP Abort Op Code Supported: False\n");
    }

    return;
}

void appl_mcp_par_and_display_olcp_feature(UINT32 olcp_feature)
{
    if (olcp_feature & MCP_OTS_OLCP_GO_TO_OPC_SUPPORTED)
    {
        APPL_TRC("\t-OLCP Go To Op Code Supported: True\n");
    }
    else
    {
        APPL_TRC("\t-OLCP Go To Op Code Supported: False\n");
    }

    if (olcp_feature & MCP_OTS_OLCP_ORDER_OPC_SUPPORTED)
    {
        APPL_TRC("\t-OLCP Order Op Code Supported: True\n");
    }
    else
    {
        APPL_TRC("\t-OLCP Order Op Code Supported: False\n");
    }

    if (olcp_feature & MCP_OTS_OLCP_REQ_NUM_OF_OBJS_OPC_SUPPORTED)
    {
        APPL_TRC("\t-OLCP Request Number of Objects Op Code Supported: True\n");
    }
    else
    {
        APPL_TRC("\t-OLCP Request Number of Objects Op Code Supported: False\n");
    }

    if (olcp_feature & MCP_OTS_OLCP_CLEAR_MARKING_OPC_SUPPORTED)
    {
        APPL_TRC("\t-OLCP Clear Marking Op Code Supported: True\n");
    }
    else
    {
        APPL_TRC("\t-OLCP Clear Marking Op Code Supported: False\n");
    }

    return;
}

void appl_mcp_utils_parse_and_display_oacp(UINT8 * data)
{
    UINT32 offset, length;

    offset = 0U;
    length = 0U;

    switch(data[0U])
    {
    case MCP_OTS_OACP_CAL_CHECK_SUM_OPCODE:
        APPL_TRC
        (
            "Len: 0x%02X,  Op Code: Calculate Checksum (0x%02X)\n",
            (UCHAR)sizeof(UINT8),
            data[0U]
        );
        GA_UNPACK_LE_4_BYTE(&offset, &data[1U]);
        APPL_TRC
        (
            "Len: 0x%02X,  Offset: 0x%08X\n",
            (UCHAR)sizeof(UINT32),
            offset
        );
        GA_UNPACK_LE_4_BYTE(&length, &data[5U]);
        APPL_TRC
        (
            "Len: 0x%02X,  Length: 0x%08X\n",
            (UCHAR)sizeof(UINT32),
            length
        );
        break;

    case MCP_OTS_OACP_READ_OPCODE:
        APPL_TRC
        (
            "Len: 0x%02X,  Op Code: Read (0x%02X)\n",
            (UCHAR)sizeof(UINT8),
            data[0U]
        );
        GA_UNPACK_LE_4_BYTE(&offset, &data[1U]);
        APPL_TRC
        (
            "Len: 0x%02X,  Offset: 0x%08X\n",
            (UCHAR)sizeof(UINT32),
            offset
        );
        GA_UNPACK_LE_4_BYTE(&length, &data[5U]);
        APPL_TRC
        (
            "Len: 0x%02X,  Length: 0x%08X\n",
            (UCHAR)sizeof(UINT32),
            length
        );
        break;

    case MCP_OTS_OACP_RSP_CODE_OPCODE:
    {
        APPL_TRC
        (
            "Len: 0x%02X,  Op Code: Response Code (0x%02X)\n",
            (UCHAR)sizeof(UINT8),
            data[0U]
        );
        APPL_TRC
        (
            "Len: 0x%02X,  Request Op Code: ",
            (UCHAR)sizeof(UINT8)
        );
        switch(data[1U])
        {
        case MCP_OTS_OACP_CAL_CHECK_SUM_OPCODE:
            APPL_TRC("Calculate Checksum (0x%02X)\n", data[1U]);
            appl_mcp_utils_parse_and_display_oacp_rsp_result(data[2U]);
            GA_UNPACK_LE_4_BYTE(&offset, &data[3U]);
            APPL_TRC
            (
                "Len: 0x%02X,  Checksum: 0x%08X\n",
                (UCHAR)sizeof(UINT32),
                offset
            );
            break;

        case MCP_OTS_OACP_READ_OPCODE:
            APPL_TRC("Read (0x%02X)\n", data[1U]);
            appl_mcp_utils_parse_and_display_oacp_rsp_result(data[2U]);
            break;

        default:
            APPL_ERR("Request Opcode not supported (0x%02X) !\n", data[1U]);
            break;
        }
    }

    default:
        APPL_ERR("Opcode not supported (0x%02X) !\n", data[0U]);
        break;
    }

    return;
}

void appl_mcp_utils_parse_and_display_oacp_rsp_result(UINT8 result_code)
{
    APPL_TRC("Len: 0x%02X,  Result Code: ", (UCHAR)sizeof(UINT8));

    switch(result_code)
    {
    case MCP_OTS_OACP_SUCCESS:
        APPL_TRC("Success (0x%02X)\n", result_code);
        break;
    case MCP_OTS_OACP_OPCODE_NOT_SUPPORTED:
        APPL_ERR("Op Code Not Supported (0x%02X) !\n", result_code);
        break;
    case MCP_OTS_OACP_INVALID_PARAMETER:
        APPL_ERR("Invalid Parameter (0x%02X) !\n", result_code);
        break;
    case MCP_OTS_OACP_INSUFF_RESOURCES:
        APPL_ERR("Insufficient Resources (0x%02X) !\n", result_code);
        break;
    case MCP_OTS_OACP_INVALID_OBJECT:
        APPL_ERR("Invalid Object (0x%02X) !\n", result_code);
        break;
    case MCP_OTS_OACP_CHANNEL_UNAVAILABLE:
        APPL_ERR("Channel Unavailable (0x%02X) !\n", result_code);
        break;
    case MCP_OTS_OACP_UNSUPP_TYPE:
        APPL_ERR("Unsupported Type (0x%02X) !\n", result_code);
        break;
    case MCP_OTS_OACP_PROCEDURE_NOT_PERMITTED:
        APPL_ERR("Procedure Not Permitted (0x%02X) !\n", result_code);
        break;
    case MCP_OTS_OACP_OBJECT_LOCKED:
        APPL_ERR("Object Locked (0x%02X) !\n", result_code);
        break;
    case MCP_OTS_OACP_OPERATION_FAILED:
        APPL_ERR("Operation Failed (0x%02X) !\n", result_code);
        break;
    default:
        APPL_ERR("UnHandled Result Code (0x%02X) !\n", result_code);
        break;
    }

    return;
}

void appl_mcp_utils_parse_and_display_olcp(UINT8 * data, UINT16 datalen)
{
    UCHAR  index;

    switch(data[0U])
    {
    case MCP_OTS_OLCP_GO_TO_OPCODE:
        APPL_TRC
        (
            "Len: 0x%02X,  Op Code: Go To (0x%02X)\n",
            (UCHAR)sizeof(UINT8),
            data[0U]
        );
        APPL_TRC("Len: 0x%02X,  Object ID: 0x", (datalen-1U));
        for (index = 1U; index < datalen; index++)
        {
            APPL_TRC("%02X", data[index]);
        }
        APPL_TRC("\n");
        break;

    case MCP_OTS_OLCP_RSP_CODE_OPCODE:
    {
        APPL_TRC
        (
            "Len: 0x%02X,  Op Code: Response Code (0x%02X)\n",
            (UCHAR)sizeof(UINT8),
            data[0U]
        );
        APPL_TRC
        (
            "Len: 0x%02X,  Request Op Code: ",
            (UCHAR)sizeof(UINT8)
        );
        switch(data[1U])
        {
        case MCP_OTS_OLCP_GO_TO_OPCODE:
            APPL_TRC("Go To (0x%02X)\n", data[1U]);
            appl_mcp_utils_parse_and_display_olcp_rsp_result(data[2U]);
            break;

        default:
            APPL_ERR("Request Opcode not supported (0x%02X) !\n", data[1U]);
            break;
        }
    }

    default:
        APPL_ERR("Opcode not supported (0x%02X) !\n", data[0U]);
        break;
    }

    return;
}

void appl_mcp_utils_parse_and_display_olcp_rsp_result(UINT8 result_code)
{
    APPL_TRC("Len: 0x%02X,  Result Code: ", (UCHAR)sizeof(UINT8));

    switch(result_code)
    {
    case MCP_OTS_OLCP_SUCCESS:
        APPL_TRC("Success (0x%02X)\n", result_code);
        break;
    case MCP_OTS_OLCP_OPCODE_NOT_SUPPORTED:
        APPL_ERR("Op Code Not Supported (0x%02X) !\n", result_code);
        break;
    case MCP_OTS_OLCP_INVALID_PARAMETER:
        APPL_ERR("Invalid Parameter (0x%02X) !\n", result_code);
        break;
    case MCP_OTS_OLCP_OPERATION_FAILED:
        APPL_ERR("Operation Failed (0x%02X) !\n", result_code);
        break;
    case MCP_OTS_OLCP_OUT_OF_BOUNDS:
        APPL_ERR("Out Of Bounds (0x%02X) !\n", result_code);
        break;
    case MCP_OTS_OLCP_TOO_MANY_OBJECTS:
        APPL_ERR("Too Many Objects (0x%02X) !\n", result_code);
        break;
    case MCP_OTS_OLCP_NO_OBJECT:
        APPL_ERR("No Object (0x%02X) !\n", result_code);
        break;
    case MCP_OTS_OLCP_OBJ_ID_NOT_FOUND:
        APPL_ERR("Object ID Not Found (0x%02X) !\n", result_code);
        break;
    default:
        APPL_ERR("UnHandled Result Code (0x%02X) !\n", result_code);
        break;
    }

    return;
}
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */
#endif /* GA_MCP */
