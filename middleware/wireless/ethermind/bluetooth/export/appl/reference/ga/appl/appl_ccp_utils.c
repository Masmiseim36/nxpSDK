/**
 *  \file appl_ccp_utils.c
 *
 *  \brief This file defines the CCP utility Data Structures and Methods.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_ccp_utils.h"

#ifdef GA_CCP

void appl_ccp_display_ga_result(GA_RESULT event_status)
{
    GA_RESULT  retval;

    retval = GA_FAILURE;

    /* checking for common GA_RESULT */
    retval = appl_ga_utils_display_ga_result(event_status);

    if (GA_FAILURE == retval)
    {
        switch(event_status)
        {
            /* Application error code */
            case CCP_ERR_CODE_VALUE_CHANGED_DURING_READ_LONG:
                APPL_ERR("Value changed during read long !");
                break;

            /* Implementation related error codes */
            case GA_CCP_INVALID_STATE:
                APPL_ERR("Invalid State !");
                break;

            case GA_CCP_INVALID_PARAMS:
                APPL_ERR("Invalid Params !");
                break;

            case GA_CCP_CNTX_ALLOC_FAILED:
                APPL_ERR("Context allocation failed !");
                break;

            case GA_CCP_TBS_CNTX_ALLOC_FAILED:
                APPL_ERR("TBS Context allocation failed !");
                break;

            case GA_CCP_IN_BUSY_STATE:
                APPL_ERR("In Busy State !");
                break;

            case GA_CCP_CNTX_NOT_FOUND:
                APPL_ERR("Context not found !");
                break;

            case GA_CCP_INVALID_HANDLE:
                APPL_ERR("Invalid Handle !");
                break;

            default:
                APPL_ERR("Invalid Code:");
        }
        APPL_TRC(" (0x%04X)\n", event_status);
    }

    return;
}

void appl_ccp_utils_parse_and_display_char_uuid(UINT16 char_uuid)
{
    switch(char_uuid)
    {
        case GA_CHAR_TBS_BRR_PROVIDER_NAME:
            APPL_TRC("Char UUID: Bearer Provider Name (0x%04X)\n", char_uuid);
            break;

        case GA_CHAR_TBS_BRR_UCI:
            APPL_TRC("Char UUID: Bearer Uniform Caller Identifier (UCI) (0x%04X)\n", char_uuid);
            break;

        case GA_CHAR_TBS_BRR_TECHNOLOGY:
            APPL_TRC("Char UUID: Bearer Technology (0x%04X)\n", char_uuid);
            break;

        case GA_CHAR_TBS_BRR_URI_SCHMS_SUPPORT_LIST:
            APPL_TRC("Char UUID: Bearer URI Schemes Supported List (0x%04X)\n", char_uuid);
            break;

        case GA_CHAR_TBS_BRR_SIGSTRENGTH:
            APPL_TRC("Char UUID: Bearer Signal Strength (0x%04X)\n", char_uuid);
            break;

        case GA_CHAR_TBS_BRR_SIGSTRENGTH_REPORTING_INTERVAL:
            APPL_TRC("Char UUID: Bearer Signal Strength Reporting Interval (0x%04X)\n", char_uuid);
            break;

        case GA_CHAR_TBS_BRR_LIST_CUR_CALLS:
            APPL_TRC("Char UUID: Bearer List Current Calls (0x%04X)\n", char_uuid);
            break;

        case GA_CHAR_TBS_STATUS_FLAGS:
            APPL_TRC("Char UUID: Status Flags (0x%04X)\n", char_uuid);
            break;

        case GA_CHAR_TBS_INCOMING_CALL_TARGET_BRR_URI:
            APPL_TRC("Char UUID: Incoming Call Target Bearer URI (0x%04X)\n", char_uuid);
            break;

        case GA_CHAR_TBS_CALL_STATE:
            APPL_TRC("Char UUID: Call State (0x%04X)\n", char_uuid);
            break;

        case GA_CHAR_TBS_CALL_CP:
            APPL_TRC("Char UUID: Call Control Point (0x%04X)\n", char_uuid);
            break;

        case GA_CHAR_TBS_CALL_CP_OPTIONAL_OPCODES:
            APPL_TRC("Char UUID: Call Control Point Optional Opcodes (0x%04X)\n", char_uuid);
            break;

        case GA_CHAR_TBS_TERMINATION_REASON:
            APPL_TRC("Char UUID: Termination Reason (0x%04X)\n", char_uuid);
            break;

        case GA_CHAR_TBS_INCOMING_CALL:
            APPL_TRC("Char UUID: Incoming Call (0x%04X)\n", char_uuid);
            break;

        case GA_CHAR_TBS_CALL_FRIENDLY_NAME:
            APPL_TRC("Char UUID: Call Friendly Name (0x%04X)\n", char_uuid);
            break;

        case GA_CHAR_CONTENT_CONTROL_ID:
            APPL_TRC("Char UUID: Content Control ID (0x%04X)\n", char_uuid);
            break;

        default:
            APPL_ERR("UnHandled Characteristic UUID (0x%04X) !\n", char_uuid);
            break;
    }

    return;
}

void appl_ccp_utils_parse_and_display_ccp_call_state(UCHAR call_state)
{
    APPL_TRC("0x%02X ", call_state);
    switch(call_state)
    {
        case CCP_CALL_STATE_INCOMING:
            APPL_TRC("(Incoming)");
            break;

        case CCP_CALL_STATE_DIALING:
            APPL_TRC("(Dialing)");
            break;

        case CCP_CALL_STATE_ALERTING:
            APPL_TRC("(Alerting)");
            break;

        case CCP_CALL_STATE_ACTIVE:
            APPL_TRC("(Active)");
            break;

        case CCP_CALL_STATE_L_HELD:
            APPL_TRC("(Local Held)");
            break;

        case CCP_CALL_STATE_R_HELD:
            APPL_TRC("(Remote Held)");
            break;

        case CCP_CALL_STATE_LR_HELD:
            APPL_TRC("(Locally and Remotely Held)");
            break;

        case CCP_CALL_STATE_INVALID:
            APPL_TRC("(Invalid)");
            break;

        default:
            APPL_ERR("(Unhandled !)");
    }
    APPL_TRC("\n");
    return;
}

void appl_ccp_utils_parse_and_display_ccp_cp_opcode(UCHAR  cp_opcode)
{
    APPL_TRC("0x%02X ", cp_opcode);
    switch(cp_opcode)
    {
        case CCP_CP_OP_ACCEPT:
            APPL_TRC("(Accept)");
            break;

        case CCP_CP_OP_TERMINATE:
            APPL_TRC("(Terminate)");
            break;

        case CCP_CP_OP_L_HLD:
            APPL_TRC("(Local Hold)");
            break;

        case CCP_CP_OP_L_RTRV:
            APPL_TRC("(Local Retrieve)");
            break;

        case CCP_CP_OP_ORIGINATE:
            APPL_TRC("(Originate)");
            break;

        case CCP_CP_OP_JOIN:
            APPL_TRC("(Join)");
            break;

        default:
            APPL_ERR("(Invalid !)");
    }
    APPL_TRC("\n");
    return;
}

void appl_ccp_utils_parse_and_display_ccp_call_flags(UCHAR  call_flag)
{
    APPL_TRC("0x%02X ", call_flag);
    if (CCP_CALL_FLAGS_OUTGOING & call_flag)
    {
        APPL_TRC("(Call is an outgoing call, ");
    }
    else
    {
        APPL_TRC("(Call is an incoming call, ");
    }

    if (CCP_CALL_FLAGS_WITHHELD & call_flag)
    {
        APPL_TRC("Withheld, ");
    }
    else
    {
        APPL_TRC("Not withheld, ");
    }

    if (CCP_CALL_FLAGS_WITHHELD_BT_NTW & call_flag)
    {
        APPL_TRC("Withheld by network)");
    }
    else
    {
        APPL_TRC("Provided by network)");
    }
    APPL_TRC("\n");
    return;
}

void appl_ccp_utils_parse_and_display_ccp_brr_tech(UCHAR  brr_tech)
{
    APPL_TRC("0x%02X ", brr_tech);
    switch(brr_tech)
    {
        case CCP_BRR_TECH_3G:
            APPL_TRC("(3G)");
            break;

        case CCP_BRR_TECH_4G:
            APPL_TRC("(4G)");
            break;

        case CCP_BRR_TECH_LTE:
            APPL_TRC("(LTE)");
            break;

        case CCP_BRR_TECH_WIFI:
            APPL_TRC("(WIFI)");
            break;

        case CCP_BRR_TECH_5G:
            APPL_TRC("(5G)");
            break;

        case CCP_BRR_TECH_GSM:
            APPL_TRC("(GSM)");
            break;

        case CCP_BRR_TECH_CDMA:
            APPL_TRC("(CDMA)");
            break;

        case CCP_BRR_TECH_2G:
            APPL_TRC("(2G)");
            break;

        case CCP_BRR_TECH_WCDMA:
            APPL_TRC("(WCDMA)");
            break;

        case CCP_BRR_TECH_IP:
            APPL_TRC("(IP)");
            break;

        default:
            APPL_ERR("(Invalid !)");
    }
    APPL_TRC("\n");
    return;
}

void appl_ccp_utils_parse_and_display_ccp_brr_signal_strength(UCHAR  signal_strength)
{
    APPL_TRC("0x%02X ", signal_strength);
    if (CCP_BRR_SIG_NO_SERVICE == signal_strength)
    {
        APPL_TRC("(No Service)");
    }
    else if (CCP_BRR_SIG_UNAVAILABLE == signal_strength)
    {
        APPL_TRC("(Unavailable)");
    }
    else if (CCP_BRR_SIG_MAX_STRENGTH == signal_strength)
    {
        APPL_TRC("(Maximum Signal Strength)");
    }
    else if ((0x65U <= signal_strength) && (0xFEU >= signal_strength))
    {
        APPL_TRC("(RFU)");
    }
    APPL_TRC("\n");
    return;
}

void appl_ccp_utils_parse_and_display_ccp_status_flags(UINT16  status_flags)
{
    APPL_TRC("0x%04X ", status_flags);
    /* Check if inband ringtone is enabled or disabled */
    if (1U & status_flags)
    {
        APPL_TRC("(Inband ringtone enabled, ");
    }
    else
    {
        APPL_TRC("(Inband ringtone disabled, ");
    }
    /* Check if server is in silent mode or not */
    if (2U & status_flags)
    {
        APPL_TRC("Server is in silent mode)");
    }
    else
    {
        APPL_TRC("Server is not in silent mode)");
    }
    APPL_TRC("\n");
    return;
}

void appl_ccp_utils_parse_and_display_ccp_cp_opt_opcodes(UINT16  opt_cp_opcodes)
{
    APPL_TRC("0x%04X ", opt_cp_opcodes);
    /*
     * Check if local hold and local retrieve call control point
     * opcodes supported or not
     */
    if (1U & opt_cp_opcodes)
    {
        APPL_TRC
        (
            "(Local Hold and Local Retrieve Call Control Point Opcodes supported, "
        );
    }
    else
    {
        APPL_TRC
        (
            "(Local Hold and Local Retrieve Call Control Point Opcodes not supported, "
        );
    }
    /*
     * Check if Join Call Control Point Opcode supported or not
     */
    if (2U & opt_cp_opcodes)
    {
        APPL_TRC("Join Call Control Point Opcode supported)");
    }
    else
    {
        APPL_TRC("Join Call Control Point Opcode supported)");
    }
    APPL_TRC("\n");
    return;
}

#endif /* GA_CCP */
