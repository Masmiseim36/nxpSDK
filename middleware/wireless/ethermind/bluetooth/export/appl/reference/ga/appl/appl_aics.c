/**
 *  \file appl_aics.c
 *
 *  \brief This file defines the Audio Input Control Service:
 *  Application Data Structures and Methods.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_aics.h"

#if (defined GA_VCP || defined GA_MICP)

/* --------------------------------------------- APIs */
void appl_aics_parse_and_display_gain_mode(UINT8 gain_mode)
{
    if(AICS_GM_MANUAL_ONLY == gain_mode)
    {
        APPL_TRC("Manual Only (0x%02X)\n", gain_mode);
    }
    else if(AICS_GM_AUTOMATIC_ONLY == gain_mode)
    {
        APPL_TRC("Automatic Only (0x%02X)\n", gain_mode);
    }
    else if(AICS_GM_MANUAL == gain_mode)
    {
        APPL_TRC("Manual (0x%02X)\n", gain_mode);
    }
    else if(AICS_GM_AUTO == gain_mode)
    {
        APPL_TRC("Automatic (0x%02X)\n", gain_mode);
    }
    else
    {
        APPL_ERR("Invalid AICS Gain Mode Value (0x%02X) !\n", gain_mode);
    }

    return;
}

void appl_aics_parse_and_display_audio_input_type(UINT8 audio_input_type)
{
    if(GA_AUDIO_INPUT_TYPE_UNSPECIFIED == audio_input_type)
    {
        APPL_TRC("Unspecified input (0x%02X)\n", audio_input_type);
    }
    else if(GA_AUDIO_INPUT_TYPE_BLUETOOTH == audio_input_type)
    {
        APPL_TRC("Bluetooth audio stream (0x%02X)\n", audio_input_type);
    }
    else if(GA_AUDIO_INPUT_TYPE_MICROPHONE == audio_input_type)
    {
        APPL_TRC("Microphone (0x%02X)\n", audio_input_type);
    }
    else if(GA_AUDIO_INPUT_TYPE_ANALOG == audio_input_type)
    {
        APPL_TRC("Analog Interface (0x%02X)\n", audio_input_type);
    }
    else if(GA_AUDIO_INPUT_TYPE_DIGITAL == audio_input_type)
    {
        APPL_TRC("Digital Interface (0x%02X)\n", audio_input_type);
    }
    else if(GA_AUDIO_INPUT_TYPE_RADIO == audio_input_type)
    {
        APPL_TRC("AM/FM/XM/etc. (0x%02X)\n", audio_input_type);
    }
    else if(GA_AUDIO_INPUT_TYPE_STREAMING == audio_input_type)
    {
        APPL_TRC("Streaming audio source (0x%02X)\n", audio_input_type);
    }
    else
    {
        APPL_ERR
        (
            "Invalid AICS Audio Input Type Value (0x%02X) !\n",
            audio_input_type
        );
    }

    return;
}

void appl_aics_parse_and_display_audio_input_status(UINT8 audio_input_status)
{
    if(AICS_IN_STATUS_INACTIVE == audio_input_status)
    {
        APPL_TRC("Inactive (0x%02X)\n", audio_input_status);
    }
    else if(AICS_IN_STATUS_ACTIVE == audio_input_status)
    {
        APPL_TRC("Active (0x%02X)\n", audio_input_status);
    }
    else
    {
        APPL_ERR
        (
            "Invalid AICS Audio Input Status Value (0x%02X) !\n",
            audio_input_status
        );
    }

    return;
}

void appl_aics_parse_and_display_char_value
     (
         UINT16   char_uuid,
         UCHAR  * aics_data,
         UINT16   datalen
     )
{
    UCHAR i;

    i = 0U;
    switch(char_uuid)
    {
        case GA_CHAR_AICS_INPUT_STATE:
            APPL_TRC
            (
                "Len: 0x%02X,  Gain_Setting: 0x%02X\n",
                (UCHAR)sizeof(aics_data[0U]), aics_data[0U]
            );
            APPL_TRC("Len: 0x%02X,  Mute: ", (UCHAR)sizeof(aics_data[1U]));
            appl_ga_utils_display_mute(aics_data[1U]);
            APPL_TRC
            (
                "Len: 0x%02X,  Gain_Mode: ",
                (UCHAR)sizeof(aics_data[2U])
            );
            appl_aics_parse_and_display_gain_mode(aics_data[2U]);
            APPL_TRC
            (
                "Len: 0x%02X,  Change_Counter: 0x%02X\n",
                (UCHAR)sizeof(aics_data[3U]), aics_data[3U]
            );
            break;

        case GA_CHAR_AICS_GAIN_SETTING:
            APPL_TRC
            (
                "Len: 0x%02X,  Gain_Setting_Units: 0x%02X\n",
                (UCHAR)sizeof(aics_data[0U]),
                aics_data[0U]
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Gain_Setting_Minimum: 0x%02X (%d)\n",
                (UCHAR)sizeof(aics_data[1U]),
                (CHAR)aics_data[1U],
                (CHAR)aics_data[1U]
            );
            APPL_TRC
            (
                "Len: 0x%02X,  Gain_Setting_Maximum: 0x%02X (%d)\n",
                (UCHAR)sizeof(aics_data[2U]),
                aics_data[2U],
                aics_data[2U]
            );
            break;

        case GA_CHAR_AICS_INPUT_TYPE:
            APPL_TRC
            (
                "Len: 0x%02X,  Audio Input Type: ",
                (UCHAR)sizeof(aics_data[0U])
            );
            appl_aics_parse_and_display_audio_input_type
            (
                aics_data[0U]
            );
            break;

        case GA_CHAR_AICS_INPUT_STATUS:
            APPL_TRC
            (
                "Len: 0x%02X,  Audio Input Status: ",
                (UCHAR)sizeof(aics_data[0U])
            );
            appl_aics_parse_and_display_audio_input_status
            (
                aics_data[0U]
            );
            break;

        case GA_CHAR_AICS_AUDIO_INPUT_DESC:
            APPL_TRC
            (
                "Len: 0x%02X,  Audio Input Description: ",
                (UCHAR)sizeof(aics_data[0U])
            );
            for (i = 0U; i < datalen; i++)
            {
                APPL_TRC("%c", aics_data[i]);
            }
            APPL_TRC("\n");
            break;

        default:
            APPL_ERR
            (
                "UnHandled AICS Characteristic UUID: 0x%04X !\n",
                char_uuid
            );
            break;
    }

    return;
}

void appl_aics_display_appl_err_codes(GA_RESULT status)
{
    switch (status)
    {
    /* Application error code */
    case AICS_ERR_CODE_INVALID_CHNG_CNTR:
        APPL_ERR("Invalid Change Counter !");
        break;

    case AICS_ERR_CODE_OPCODE_NOT_SUPPORTED:
        APPL_ERR("Opcode Not Supported !");
        break;

    case AICS_ERR_CODE_VALUE_OUT_OF_RANGE:
        APPL_ERR("Value Out of Range !");
        break;

    case AICS_ERR_CODE_MUTE_DISABLED:
        APPL_ERR("Mute Disabled !");
        break;

    case AICS_ERR_CODE_GAIN_MODE_CHANG_NOT_ALLOWED:
        APPL_ERR("Gain Mode Change Not Allowed !");
        break;

    default:
        APPL_ERR("Invalid Code !");
        break;
    }

    APPL_TRC(" (0x%04X)\n", status);
    return;
}

#endif /* (defined GA_VCP || defined GA_MICP) */
