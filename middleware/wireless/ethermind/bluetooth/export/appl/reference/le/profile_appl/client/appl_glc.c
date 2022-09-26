
/**
 *  \file appl_glc.c
 *
 *  This is a Glucose Meter Collector application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_gatt_client.h"


#if (defined ATT && defined GLC)
/* ----------------------------------------- Macro Defines */
/** RACP Commands */
#define REPORT_STORED_RECORDS_OPCODE             1U
#define DELETE_STORED_RECORDS_OPCODE             2U
#define ABORT_OPERATION_OPCODE                   3U
#define REPORT_NUM_OF_STORED_RECORDS_OPCODE      4U
#define NUM_OF_STORED_RECORDS_RESPONSE_OPCODE    5U
#define RESPONSE_CODE_OPCODE                     6U

/** RACP Operators */
#define NULL_OPERATOR                            0U
#define ALL_RECORDS_OPERATOR                     1U
#define LESS_THAN_OR_EQUAL_TO_OPERATOR           2U
#define GREATER_THAN_OR_EQUAL_TO_OPERATOR        3U
#define WITHIN_RANGE_OF_OPERATOR                 4U
#define FIRST_RECORD_OPERATOR                    5U
#define LAST_RECORD_OPERATOR                     6U

/** RACP Operand Filter Types */
#define SEQUENCE_NUMBER_FILTER_TYPE              1U
#define USER_FACING_TIME_FILTER_TYPE             2U

/** RACP Command Length */
#define RACP_NO_OPERAND_LEN                      2U /* (2) */
#define RACP_OPERAND_SEQUENCE_NUMBER_LEN         5U /* (2 + 1 + 2) */
#define RACP_OPERAND_SEQUENCE_NUMBER_PAIR_LEN    7U /* (2 + 1 + 2 + 2) */
#define RACP_OPERAND_USER_FACING_TIME_LEN       10U /* (2 + 1 + 7) */
#define RACP_OPERAND_USER_FACING_TIME_PAIR_LEN  17U /* (2 + 1 + 7 + 7) */

#define RACP_COMMAND_PKT_MAX_SIZE               RACP_OPERAND_USER_FACING_TIME_PAIR_LEN

#define SEQUENCE_NUMBER_LEN                      2U
#define USER_FACING_TIME_LEN                     7U

/* ----------------------------------------- External Global Variables */
extern ATT_HANDLE appl_gatt_client_handle;

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */
static ATT_ATTR_HANDLE gls_msrmnt_ccc_hdl;
static ATT_ATTR_HANDLE gls_msrmnt_cntxt_ccc_hdl;
static ATT_ATTR_HANDLE gls_racp_ccc_hdl;
static ATT_ATTR_HANDLE gls_racp_hdl;
static ATT_ATTR_HANDLE gls_glucose_feature_hdl;
static ATT_ATTR_HANDLE gls_msrmnt_cntxt_hdl;
static ATT_ATTR_HANDLE gls_msrmnt_hdl;

static const UCHAR glc_client_menu[] ="\n\
======== C L I E N T   M E N U ========\n\
    0.  Exit\n\
    1.  Refresh\n\
\n\
       === Glucose-meter Service ===\n\
   10.  Discover Glucose-meter Service\n\
\n\
        -- Glucose Measurement --\n\
   11.  Configure Glucose Measurement\n\
   12.  Stop Glucose Measurement CCC\n\
\n\
        -- Glucose Measurement Context --\n\
   13.  Configure Glucose Measurement Context CCC\n\
   14.  Stop Glucose Measurement Context CCC\n\
\n\
        -- Glucose Feature --\n\
   15.  Read Glucose Feature\n\
\n\
        -- RACP --\n\
   16.  Configure RACP CCC\n\
   17.  Stop RACP CCC\n\
\n\
        -- Remote Access Control Point --\n\
   18.  Report Stored Records through Control Point\n\
   19.  Delete Stored Records through Control Point\n\
   20.  Abort Operation through Control Point\n\
   21.  Report Number of Stored Records through Control Point\n\
   22.  DIS Operations\n\
   Your Option?\n\
";

static UCHAR glc_racp_operator_menu[] =
"\n\
       === RACP Operator ===\n\
   1.  ALL_RECORDS\n\
   2.  LESS_THAN_OR_EQUAL_TO\n\
   3.  GREATER_THAN_OR_EQUAL_TO\n\
   4.  WITHIN_RANGE_OF\n\
   5   FIRST_RECORD\n\
   6.  LAST_RECORD\n\
   Your Option?\n\
";

/* ------------------------------- Functions */

void glc_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size)
{
    UINT16 i;
    UINT16 j;

    for (i = 0U; i < size; i++)
    {
        switch (GATT_GET_CHAR_UUID(characteristic))
        {
            case GATT_GLUCOSE_MSRMNT_CHARACTERISTIC:
                gls_msrmnt_hdl = characteristic->value_handle;
                break;

            case GATT_GL_MSRMT_CTX_CHARACTERISTIC:
                gls_msrmnt_cntxt_hdl = characteristic->value_handle;
                break;

            case GATT_GL_FEATURE_CHARACTERISTIC:
                gls_glucose_feature_hdl = characteristic->value_handle;
                break;

            case GATT_RACP_CHARACTERISTIC:
                gls_racp_hdl = characteristic->value_handle;
                break;

            default:
                /* CONSOLE_OUT("Invalid...\n"); */
                break;
        }

        if (0U != characteristic->desc_index)
        {
            for (j = 0U; j < characteristic->desc_index; j++)
            {
                if (GATT_CLIENT_CONFIG ==
                    GATT_GET_CLI_CHAR_DESC_UUID(characteristic,j))
                {
                    switch (GATT_GET_CHAR_UUID(characteristic))
                    {
                        case GATT_GLUCOSE_MSRMNT_CHARACTERISTIC:
                            gls_msrmnt_ccc_hdl = characteristic->descriptor[j].handle;
                            break;

                        case GATT_GL_MSRMT_CTX_CHARACTERISTIC:
                            gls_msrmnt_cntxt_ccc_hdl = characteristic->descriptor[j].handle;
                            break;

                        case GATT_RACP_CHARACTERISTIC:
                            gls_racp_ccc_hdl = characteristic->descriptor[j].handle;
                            break;

                        default:
                            /* CONSOLE_OUT("Invalid...\n"); */
                            break;
                    }

                    break;
                }
            }
        }

        characteristic ++;
    }

    /**
     * The below stored characteristic value handles are currently
     * not used, but these variables are retained for reference.
     */
    BT_IGNORE_UNUSED_PARAM(gls_msrmnt_hdl);
    BT_IGNORE_UNUSED_PARAM(gls_msrmnt_cntxt_hdl);
}

UCHAR appl_read_racp_command_operand
      (
         UCHAR    is_pair,
         UCHAR *  filter_type,
         UCHAR *  operand
      )
{
    UINT32 i;
    int read_val;

    LOG_DEBUG ("Enter Filter Type :\n");
    LOG_DEBUG (" 1. Sequence Number [UINT16)\n");
    LOG_DEBUG (" 2. User Facing Time (Base Time)\n");
    CONSOLE_IN ("%d", &read_val);

    if ((1U > read_val) || (2U < read_val))
    {
        LOG_DEBUG ("Invalid Filter Type..\n");
        return 0U;
    }

    (*filter_type) = (UCHAR) read_val;

    if (SEQUENCE_NUMBER_FILTER_TYPE == (*filter_type))
    {
        LOG_DEBUG ("Enter Sequence Number[In Decimal] %s:\n",(is_pair)?"Pair":"");
        for (i = 0U; i < ((is_pair + 1U) * SEQUENCE_NUMBER_LEN); i+=SEQUENCE_NUMBER_LEN)
        {
            CONSOLE_IN ("%d", &read_val);
            BT_PACK_LE_2_BYTE_VAL(&operand[i],(UINT16)read_val);
        }
        return 1U;
    }
    else if (USER_FACING_TIME_FILTER_TYPE == (*filter_type))
    {
        LOG_DEBUG ("Enter User Facing Time %s:\n",(is_pair)?"Pair":"");
        for (i = 0U; i < ((is_pair + 1U) * USER_FACING_TIME_LEN); i+=USER_FACING_TIME_LEN)
        {
            LOG_DEBUG ("Year[In Decimal] : ");
            CONSOLE_IN ("%d", &read_val);
            BT_PACK_LE_2_BYTE_VAL(&operand[i],(UINT16)read_val);
            LOG_DEBUG ("Month[In Decimal] : ");
            CONSOLE_IN ("%d", &read_val);
            operand[i+2U] = (UCHAR) read_val;
            LOG_DEBUG ("Day[In Decimal] : ");
            CONSOLE_IN ("%d", &read_val);
            operand[i+3U] = (UCHAR) read_val;
            LOG_DEBUG ("Hour[In Decimal] : ");
            CONSOLE_IN ("%d", &read_val);
            operand[i+4U] = (UCHAR) read_val;
            LOG_DEBUG ("Minute[In Decimal] : ");
            CONSOLE_IN("%d", &read_val);
            operand[i+5U] = (UCHAR) read_val;
            LOG_DEBUG ("Second[In Decimal] : ");
            CONSOLE_IN("%d", &read_val);
            operand[i+6U] = (UCHAR) read_val;
        }
        return 1U;
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }

    return 0U;
}

UCHAR appl_handle_racp_opcode (UCHAR opcode, UCHAR **value)
{
    UCHAR length;
    UCHAR oprtr;
    UCHAR operand[20U];
    UCHAR filter_type;
    int read_val;

    length = 0U;
    oprtr = 0xFFU;

    if (ABORT_OPERATION_OPCODE != opcode)
    {
        LOG_DEBUG ("Enter the Operator :\n");
        LOG_DEBUG ("%s", glc_racp_operator_menu);
        CONSOLE_IN ("%d", &read_val);
        oprtr = (UCHAR) read_val;
    }

    switch (opcode)
    {
        case REPORT_STORED_RECORDS_OPCODE: /* Fall Through */
        case DELETE_STORED_RECORDS_OPCODE: /* Fall Through */
        case REPORT_NUM_OF_STORED_RECORDS_OPCODE:
            switch (oprtr)
            {
                case ALL_RECORDS_OPERATOR: /* Fall Through */
                case FIRST_RECORD_OPERATOR: /* Fall Through */
                case LAST_RECORD_OPERATOR: /* Fall Through */
                    (*value)[0U] = opcode;
                    (*value)[1U] = oprtr;
                    length = RACP_NO_OPERAND_LEN;
                    LOG_DEBUG("After decoding value");
                    break;

                case GREATER_THAN_OR_EQUAL_TO_OPERATOR: /* Fall Through */
                case LESS_THAN_OR_EQUAL_TO_OPERATOR: /* Fall Through */
                    (*value)[0U] = opcode;
                    (*value)[1U] = oprtr;

                    if (0U != appl_read_racp_command_operand
                         (0U,
                           &filter_type,
                           operand
                         ))
                    {
                        if (SEQUENCE_NUMBER_FILTER_TYPE == filter_type)
                        {
                            (*value)[2U] = SEQUENCE_NUMBER_FILTER_TYPE;
                            BT_mem_copy
                            (
                               value[3U],
                               operand,
                               SEQUENCE_NUMBER_LEN
                            );
                            length = RACP_OPERAND_SEQUENCE_NUMBER_LEN;
                        }
                        else if (USER_FACING_TIME_FILTER_TYPE == filter_type)
                        {
                            (*value)[2U] = USER_FACING_TIME_FILTER_TYPE;
                            BT_mem_copy
                            (
                               value[3U],
                               operand,
                               USER_FACING_TIME_LEN
                            );
                            length = RACP_OPERAND_USER_FACING_TIME_LEN;
                        }
                    }
                    break;

                case WITHIN_RANGE_OF_OPERATOR:
                    (*value)[0U] = opcode;
                    (*value)[1U] = oprtr;

                    if (0U != appl_read_racp_command_operand
                         (1U,
                           &filter_type,
                           operand
                         ))
                    {
                        if (SEQUENCE_NUMBER_FILTER_TYPE == filter_type)
                        {
                            (*value)[2U] = SEQUENCE_NUMBER_FILTER_TYPE;
                            BT_mem_copy
                            (
                               value[3U],
                               operand,
                               (2U*SEQUENCE_NUMBER_LEN)
                            );
                            length = RACP_OPERAND_SEQUENCE_NUMBER_PAIR_LEN;
                        }
                        else if (USER_FACING_TIME_FILTER_TYPE == filter_type)
                        {
                            (*value)[2U] = USER_FACING_TIME_FILTER_TYPE;
                            BT_mem_copy
                            (
                               value[3U],
                               operand,
                               (2U*USER_FACING_TIME_LEN)
                            );
                            length = RACP_OPERAND_USER_FACING_TIME_PAIR_LEN;
                        }
                    }
                    break;

                default:
                    LOG_DEBUG("Invalid Operator: 0x%02X\n", oprtr);
                    break;
            }
            break;

        case ABORT_OPERATION_OPCODE:
            (*value)[0U] = opcode;
            (*value)[1U] = NULL_OPERATOR;
            length = RACP_NO_OPERAND_LEN;
            break;

        default:
            LOG_DEBUG("Invalid Opcode: 0x%02X\n", opcode);
            break;
    }

    return length;
}

void glc_profile_operations (void)
{
    UINT32       choice, menu_choice;
    UINT16       cli_cfg;
    UCHAR        cfg_val[GATT_CLI_CFG_VAL_LEN];
    UCHAR        value_arr[RACP_COMMAND_PKT_MAX_SIZE];
    ATT_UUID     uuid;
    UCHAR        length;
    UCHAR        *pointer = value_arr;
    UCHAR        **pointer2 = &pointer;

    BT_LOOP_FOREVER()
    {
        CONSOLE_OUT(
        "%s",glc_client_menu);

        CONSOLE_IN ( "%u",&choice);
        menu_choice = choice;

        switch (choice)
        {
        case 0:
            break; /* return; */

        case 1:
            break;

        case 10:
            uuid.uuid_16 = GATT_GLUCOSE_SERVICE;
            gatt_discover_ps
            (
                &appl_gatt_client_handle,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 11:
            cli_cfg = GATT_CLI_CNFG_NOTIFICATION;
            BT_PACK_LE_2_BYTE(cfg_val, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                gls_msrmnt_ccc_hdl,
                cfg_val,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 12:
            cli_cfg = GATT_CLI_CNFG_DEFAULT;
            BT_PACK_LE_2_BYTE(cfg_val, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                gls_msrmnt_ccc_hdl,
                cfg_val,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 13:
            cli_cfg = GATT_CLI_CNFG_NOTIFICATION;
            BT_PACK_LE_2_BYTE(cfg_val, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                gls_msrmnt_cntxt_ccc_hdl,
                cfg_val,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 14:
            cli_cfg = GATT_CLI_CNFG_DEFAULT;
            BT_PACK_LE_2_BYTE(cfg_val, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                gls_msrmnt_cntxt_ccc_hdl,
                cfg_val,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 15:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                gls_glucose_feature_hdl,
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 16:
            cli_cfg = GATT_CLI_CNFG_INDICATION;
            BT_PACK_LE_2_BYTE(cfg_val, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                gls_racp_ccc_hdl,
                cfg_val,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 17:
            cli_cfg = GATT_CLI_CNFG_DEFAULT;
            BT_PACK_LE_2_BYTE(cfg_val, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                gls_racp_ccc_hdl,
                cfg_val,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 18:
            length = appl_handle_racp_opcode
                     (
                         REPORT_STORED_RECORDS_OPCODE,
                         pointer2
                     );
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                gls_racp_hdl,
                value_arr,
                length,
                0x01U
            );
            break;

        case 19:
            length = appl_handle_racp_opcode
                     (
                         DELETE_STORED_RECORDS_OPCODE,
                         pointer2
                     );
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                gls_racp_hdl,
                value_arr,
                length,
                0x01U
            );
            break;

        case 20:
            length = appl_handle_racp_opcode
                     (
                         ABORT_OPERATION_OPCODE,
                         pointer2
                     );
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                gls_racp_hdl,
                value_arr,
                length,
                0x01U
            );
            break;

        case 21:
            length = appl_handle_racp_opcode
                     (
                         REPORT_NUM_OF_STORED_RECORDS_OPCODE,
                         pointer2
                     );
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                gls_racp_hdl,
                value_arr,
                length,
                0x01U
            );
            break;

        case 22:
#ifdef DIC
        dic_profile_operations ();
#endif /* DIC */
            break;

        default:
            LOG_DEBUG("Invalid Choice\n");
            break;
        }

        if (0 == menu_choice)
        {
            /* return */
            break;
        }
    }

    return;
}

#endif /* (defined ATT && defined GLC) */

