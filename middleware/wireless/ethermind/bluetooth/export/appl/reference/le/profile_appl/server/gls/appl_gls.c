
/**
 *  \file appl_gls.c
 *
 *  This file contains the Glucose Sensor application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_service.h"

#if (defined ATT && defined GLS)

/* --------------------------------------------- Global Definitions */
/* GL Response Codes */
#define APPL_GL_RACP_SUCCESS                 0x01U
#define APPL_GL_RACP_OP_CODE_NOT_SUPPORTED   0x02U
#define APPL_GL_RACP_INVALID_OPERATOR        0x03U
#define APPL_GL_RACP_OPRTR_NOT_SUPPORTED     0x04U
#define APPL_GL_RACP_INVALID_OPERAND         0x05U
#define APPL_GL_RACP_NO_RECORDS_FOUND        0x06U
#define APPL_GL_RACP_ABRT_UNSUCCESSFUL       0x07U
#define APPL_GL_RACP_PROC_INCOMPLETE         0x08U
#define APPL_GL_RACP_OPERAND_NOT_SUPPORTED   0x09U

/* Application RACP States */
#define APPL_RACP_INIT_STATE                 0x00U
#define APPL_RACP_TRANSFR_ONGOING_STATE      0x01U
#define APPL_RACP_CLR_STATE                  0x02U

/* Glucose Meter Defined RACP Operations */
#define APPL_GL_RACP_STRD_RECORD_REQ_OPCODE       0x01U
#define APPL_GL_RACP_CLR_RECORD_REQ_OPCODE        0x02U
#define APPL_GL_RACP_ABRT_RPRTING_REQ_OPCODE      0x03U
#define APPL_GL_RACP_STRD_RECORD_CNT_REQ_OPCODE   0x04U
#define APPL_GL_RACP_STRD_RECORD_CNT_RSP_OPCODE   0x05U
#define APPL_GL_RACP_RSP_OPCODE                   0x06U

/* Glucose Meter Defined RACP Operators */
#define APPL_GL_RACP_NULL_OPRTR                   0x00U
#define APPL_GL_RACP_ALL_RCRD_OPRTR               0x01U
#define APPL_GL_RACP_LSS_THN_EQL_TO_OPRTR         0x02U
#define APPL_GL_RACP_GRTR_HNN_EQL_TO_OPRTR        0x03U
#define APPL_GL_RACP_WINTHIN_RNG_OPRTR            0x04U
#define APPL_GL_RACP_FRST_RCRD_OPRTR              0x05U
#define APPL_GL_RACP_LST_RCRD_OPRTR               0x06U

/* Glucose Meter Defined RACP Filters */
#define APPL_GL_RACP_SEQ_NO_FILTER                0x01U
#define APPL_GL_RACP_USR_FACING_TIME_FILTER       0x02U

#define APPL_GLS_ADV_DATA_LEN                     21U
#define APPL_RECORD_TRNSFER_INTERVAL               3U

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
/* Timer related Globals */
static BT_timer_handle timer_handle;

/* GATT DB Handle related Globals */
static GATT_DB_HANDLE  appl_gl_msrmt_db_handle;
static GATT_DB_HANDLE  appl_gl_cntxt_msrmt_db_handle;
static GATT_DB_HANDLE  appl_gl_racp_db_handle;

/* Attribute Handle related Globals */
static ATT_ATTR_HANDLE appl_gl_msrmt_hndl;
static ATT_ATTR_HANDLE appl_gl_cntxt_msrmt_hndl;
static ATT_ATTR_HANDLE appl_gl_racp_hndl;
static ATT_ATTR_HANDLE appl_gl_feature_hndl;

/* Service data and states related Globals */
static UCHAR  appl_gl_operating_state;
static UCHAR  appl_record_count;
UCHAR         appl_stored_record_count;
UINT16        appl_seg_index;
static UCHAR  msrmnt_to_send;
UCHAR         last_record_update;
UCHAR         gls_racp_id;
APPL_HANDLE   appl_hndl;

DECL_STATIC DECL_CONST RACP_MODULE_T gls_racp =
{
    appl_gls_report_stored_records,

    appl_gls_report_stored_number_of_records,

    appl_gls_delete_stored_record,

    appl_gls_abort_operation
};
/**
 *  Glucose Measurement Observed Value:
 *  1st Octet - Flag
 *  2nd & 3rd Octet  - Sequence Number
 *  4th - 10th Octet - TimeStamp (In YYYY-MM-DD:HR-MIN-SEC)
 */
UCHAR gls_msrmt_obs_data
             [APPL_GLUCOSE_MEASUREMENT_LENGTH * APPL_GLUCOSE_MEASUREMENT_COUNT] =
{
    /* Record 1 */
    0x00U,
    /* Flag */
    0x0BU,
    /* sequence number */
    0x00U, 0x00U,
    /* TimeStamp (In YYYY-MM-DD:HR-MIN-SEC) */
    0xDCU, 0x07U, 0x06U, 0x07U, 0x00U, 0x00U, 0x00U,
    /* Time Offset Field */
    0x00U, 0x00U,
    /* Glucose Concentration Field and Type-Sample Location Field */
    0x16U, 0xD1U, 0x11U,
    /* Sensor Status Annunciation Field */
    0x00U, 0x00U,

    /* Record 2 */
    0x00U, 0x1FU, 0x01U, 0x00U, 0xDCU, 0x07U, 0x06U, 0x08U, 0x00U, 0x00U, 0x00U, 0x01U, 0x00U,
    0x37U, 0xC0U, 0x11U, 0x00U, 0x00U,

    /* Record 3 */
    0x00U, 0x0FU, 0x02U, 0x00U, 0xDCU, 0x07U, 0x06U, 0x09U, 0x00U, 0x00U, 0x00U, 0x02U, 0x00U,
    0x37U, 0xC0U, 0x11U, 0x00U, 0x00U,

    /* Record 4 */
    0x00U, 0x0FU, 0x03U, 0x00U, 0xDCU, 0x07U, 0x06U, 0x09U, 0x00U, 0x00U, 0x00U, 0x3CU, 0x00U,
    0x37U, 0xC0U, 0x11U, 0x00U, 0x00U,

    /* Record 5 */
    0x00U, 0x0FU, 0x07U, 0x00U, 0xDCU, 0x07U, 0x06U, 0x09U, 0x00U, 0x00U, 0x00U, 0x3DU, 0x00U,
    0x37U, 0xC0U, 0x11U, 0x00U, 0x00U,

    /* Record 6 */
    0x00U, 0x0FU, 0x08U, 0x00U, 0xDCU, 0x07U, 0x06U, 0x09U, 0x00U, 0x00U, 0x00U, 0x3EU, 0x00U,
    0x37U, 0xC0U, 0x11U, 0x00U, 0x00U,

    /* Record 7 */
    0x00U, 0x0FU, 0x09U, 0x00U, 0xDCU, 0x07U, 0x06U, 0x09U, 0x00U, 0x00U, 0x00U, 0xC4U, 0xFFU,
    0x37U, 0xC0U, 0x11U, 0x00U, 0x00U,

    /* Record 8 */
    0x00U, 0x0FU, 0x0AU, 0x00U, 0xDCU, 0x07U, 0x06U, 0x09U, 0x00U, 0x00U, 0x00U, 0xC3U, 0xFFU,
    0x37U, 0xC0U, 0x11U, 0x00U, 0x00U,

    /* Record 9 */
    0x00U, 0x0FU, 0x0BU, 0x00U, 0xDCU, 0x07U, 0x06U, 0x09U, 0x00U, 0x00U, 0x00U, 0xC2U, 0xFFU,
    0x37U, 0xC0U, 0x11U, 0x00U, 0x00U
};

UCHAR gls_msrmt_seq_id_index[APPL_GLUCOSE_MEASUREMENT_COUNT] =
{
    2U,

    20U,

    38U,

    56U,

    74U,

    92U,

    110U,

    128U,

    146U
};

/* Glucose Measurement Context Observed Value */
UCHAR gls_msrmt_cntxt_obs_data
             [APPL_GLUCOSE_MEASUREMENT_CONTEXT_LENGTH * APPL_GLUCOSE_MEASUREMENT_CONTEXT_COUNT] =
{
    /* Record 1 */

    /* Flag */
    0xFFU,
    /* sequence number */
    0x01U, 0x00U,
    /* Extended Flags  */
    0x00U,
    /* Carbohydrate ID */
    0x01U,
    /* Carbohydrate - units of kilograms */
    0x32U, 0xD0U,
    /* Meal */
    0x01U,
    /* Tester + Health */
    0x51U,
    /* Exercise Duration */
    0xFFU, 0x00U,
    /* Exercise Intensity */
    0x89U,
    /* Medication ID */
    0x01U,
    /**
     * Medication - units of kilograms(if bit4 is set, bit 5 is not set)
     * Medication - units of litres(if bit4 is set, bit 5 is set)
     *
     * The intended value to be sent is
     * 96mg = (96 * 10^(-3))Kg
     * In SFLOAT,
     * exponent = -3 = 0xD(2's Complement of -3: with 4bits Value as Exp)
     * mantissa = 96 = 0x060(in Hexadecimal with 24bits Value of Mantissa)
     * Combined Value = 0xD060
     * i.e. 0x60, 0xD0 in Byte Stream (LSB to MSB)
     */
    0x60U, 0xD0U,

    /* HbA1c */
    0x07U, 0x00U,

    /* Record 2 */
    0xDFU, 0x02U, 0x00U, 0x00U, 0x01U, 0x32U, 0xD0U, 0x01U, 0x51U, 0xFFU, 0x00U, 0x89U,
    0x01U, 0x60U, 0xA0U, 0x07U, 0x00U,
};

UCHAR gls_cntxt_msrmt_seq_id_index[APPL_GLUCOSE_MEASUREMENT_CONTEXT_COUNT] =
{
    1U,

    18U
};

UCHAR appl_gl_feature_value[] = { 0xFFU, 0x03U };
/* --------------------------------------------- Functions */
void appl_gls_init (void)
{
    gls_racp_id = RACP_MAX_MODULES;
#ifdef GATT_DB_DYNAMIC
#ifdef GATT_DB_HAVE_DB_SIGNATURE
    GATT_DB_SIGNATURE appl_gls_db_sign;
#endif /* GATT_DB_HAVE_DB_SIGNATURE */
    API_RESULT      retval = API_FAILURE;
#endif /* GATT_DB_DYNAMIC */

#ifdef GATT_DB_DYNAMIC
    (BT_IGNORE_RETURN_VALUE) appl_gls_add_gaps();
    (BT_IGNORE_RETURN_VALUE) appl_gls_add_gatts();
    (BT_IGNORE_RETURN_VALUE) appl_gls_add_bas();
    (BT_IGNORE_RETURN_VALUE) appl_gls_add_dis();
    (BT_IGNORE_RETURN_VALUE) appl_gls_add_gls();

#ifdef GATT_DB_HAVE_DB_SIGNATURE
    /* Set the GLS GATT DB Signature */
    appl_gls_db_sign = APPL_GLS_GATT_DB_SIGN;
    retval = BT_gatt_db_dyn_set_db_signature(appl_gls_db_sign);
#endif /* GATT_DB_HAVE_DB_SIGNATURE */

    retval = BT_gatt_db_dyn_register();

    APPL_TRC(
    "[GLS]: GATT Database Registration Status: 0x%04X\n", retval);
    GATT_DB_MAX_ATTRIBUTES         = BT_gatt_db_get_attribute_count();
#endif /* GATT_DB_DYNAMIC */
    /* Populate the GATT DB HANDLE for GLS Measurement */
    appl_gl_msrmt_db_handle.device_id        = DEVICE_HANDLE_INIT_VAL;
    appl_gl_msrmt_db_handle.service_id       = (UCHAR)GATT_SER_GLS_GLUCOSE_METER_INST;
    appl_gl_msrmt_db_handle.char_id          = (UCHAR)GATT_CHAR_GLS_GL_MSRMNT_INST;

    appl_gl_cntxt_msrmt_db_handle.device_id  = DEVICE_HANDLE_INIT_VAL;
    appl_gl_cntxt_msrmt_db_handle.service_id = (UCHAR)GATT_SER_GLS_GLUCOSE_METER_INST;
    appl_gl_cntxt_msrmt_db_handle.char_id    = (UCHAR)GATT_CHAR_GLS_GL_CNTXT_INST;

    appl_gl_racp_db_handle.device_id         = DEVICE_HANDLE_INIT_VAL;
    appl_gl_racp_db_handle.service_id        = (UCHAR)GATT_SER_GLS_GLUCOSE_METER_INST;
    appl_gl_racp_db_handle.char_id           = (UCHAR)GATT_CHAR_GLS_GL_RACP_INST;

#ifndef GATT_DB_DYNAMIC
    /* Register for GLS services if Dynamic GATT Database is not Used */
    appl_gls_gatt_db_register();
#endif /* GATT_DB_DYNAMIC */

    /* Register GLS GATT DB Handler with PL Extension */
    gatt_db_init_pl(gatt_db_gls_gatt_char_handler);
}

void appl_gls_bt_init(void)
{
    APPL_TRC("[GLS]: In appl_gls_bt_init\n");

    /* Set GAP Role */
    APPL_SET_GAP_PERIPHERAL_ROLE();

    appl_gls_server_reinitialize();
}

void appl_gls_bt_deinit(void)
{
    APPL_TRC("[GLS]: In appl_gls_bt_deinit\n");

    /* Stop Timer */
    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
        BT_stop_timer(timer_handle);
        timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }
}

void gls_racp_init(void)
{
    API_RESULT retval;

    if (RACP_MAX_MODULES == gls_racp_id)
    {
        retval = BT_racp_register_module(&gls_racp, &gls_racp_id);

        if (API_SUCCESS != retval)
        {
            APPL_ERR("[GLS]: **ERR** RACP Registration Failed!\n");
        }
    }
    else
    {
        APPL_ERR("[GLS]: **ERR** RACP Re-Registration Attempted!\n");
    }
}
void appl_gls_connect(DEVICE_HANDLE  * dq_handle)
{
    API_RESULT retval;
    ATT_VALUE         value;
    UINT16            cli_cnfg;

    cli_cnfg = 0U;

    appl_gl_msrmt_db_handle.device_id = (*dq_handle);

    /* Calling RACP Registration for GLS from here */
    /* This is done as profile init is invoked ahead of BT_bluetooth_on in SingleMode */
    gls_racp_init();

    /* If GLS Measurement is configured, initiated transfer */
    appl_gl_msrmt_db_handle.device_id = (*dq_handle);

    appl_get_handle_from_device_handle
    (
        appl_gl_msrmt_db_handle.device_id,
        &appl_hndl
    );
    /* Get Glucose Measurement Characteristics Handle */
    retval = BT_gatt_db_get_char_val_hndl
             (
                  &appl_gl_msrmt_db_handle,
                  &appl_gl_msrmt_hndl
             );
    retval = BT_gatt_db_get_char_cli_cnfg(&appl_gl_msrmt_db_handle, &value);
    if (retval != API_SUCCESS)
    {
        cli_cnfg = 0x0000U;
    }
    else
    {
        BT_UNPACK_LE_2_BYTE(&cli_cnfg, value.val);
    }
    APPL_TRC(
    "[GLS]: Fetched Client Configuration (0x%04X) for Device (0x%02X)\n",
    cli_cnfg, (*dq_handle));

    appl_gl_cntxt_msrmt_db_handle.device_id = (*dq_handle);

    /* Get Glucose Context Measurement Characteristics Handle */
    retval = BT_gatt_db_get_char_val_hndl
             (
                  &appl_gl_cntxt_msrmt_db_handle,
                  &appl_gl_cntxt_msrmt_hndl
             );
    retval = BT_gatt_db_get_char_cli_cnfg(&appl_gl_cntxt_msrmt_db_handle, &value);
    if (retval != API_SUCCESS)
    {
        cli_cnfg = 0x0000U;
    }
    else
    {
        BT_UNPACK_LE_2_BYTE(&cli_cnfg, value.val);
    }

    APPL_TRC(
    "[GLS]: Fetched Client Configuration (0x%04X) for Device (0x%02X)\n",
    cli_cnfg, (*dq_handle));

    appl_gl_racp_db_handle.device_id = (*dq_handle);

    /* Get Glucose RACP Characteristics Handle */
    retval = BT_gatt_db_get_char_val_hndl
             (
                  &appl_gl_racp_db_handle,
                  &appl_gl_racp_hndl
             );

    retval = BT_gatt_db_get_char_cli_cnfg(&appl_gl_racp_db_handle, &value);
    if (retval != API_SUCCESS)
    {
        cli_cnfg = 0x0000U;
    }
    else
    {
        BT_UNPACK_LE_2_BYTE(&cli_cnfg, value.val);
    }

    APPL_TRC(
    "[GLS]: Fetched Client Configuration (0x%04X) for Device (0x%02X)\n",
    cli_cnfg, (*dq_handle));
#ifdef APPL_GLS_SINGLE_BOND_SUPPORT
    appl_gl_msrmt_db_handle.char_id = GATT_CHAR_GL_FEATURES_INST;

    value.val = appl_gl_feature_value;
    value.len = sizeof(appl_gl_feature_value);

    /* Set Multi Bond bit in Glucose feature to zero */
    retval = BT_gatt_db_set_char_val
             (
                &appl_gl_msrmt_db_handle,
                &value
             );
#endif /* APPL_GLS_SINGLE_BOND_SUPPORT */
}

void appl_gls_server_reinitialize (void)
{
    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
        BT_stop_timer (timer_handle);
        timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

    appl_gl_operating_state = APPL_RACP_INIT_STATE;
    appl_record_count = 0U;
    appl_stored_record_count = APPL_STORED_RECORD_COUNT;
    appl_seg_index = APPL_STORED_RECORD_COUNT;
    msrmnt_to_send = BT_FALSE;
    last_record_update = 0U;

#if ((defined APPL_GAP_BROACASTER) || defined (APPL_GAP_PERIPHERAL))
    if (BT_TRUE == APPL_IS_GAP_PERIPHERAL_ROLE())
    {
        /* Configure and Enable Advertising */
        appl_service_configure_adv(APPL_GAP_PROC_NORMAL, HCI_ADV_IND, 0x00U, 0x00U, NULL, 0x00U);
        appl_service_enable_adv(0x01U);
    }
#endif /* ((defined APPL_GAP_BROACASTER) || defined (APPL_GAP_PERIPHERAL)) */

#if ((defined APPL_GAP_OBSERVER) || (defined APPL_GAP_CENTRAL))
    if (BT_TRUE == APPL_IS_GAP_CENTRAL_ROLE())
    {
        /* Configure and Enable Scanning */
        appl_service_configure_scan(APPL_GAP_PROC_NORMAL, 0x00U, 0x00U, 0x00U);
        appl_service_enable_scan(0x01U);
    }
#endif /* ((defined APPL_GAP_OBSERVER) || (defined APPL_GAP_CENTRAL)) */
}

API_RESULT appl_gl_racp_validate_operands
           (
                /* IN */ UCHAR    oprtr,
                /* IN */ UCHAR  * operand,
                /* IN */ UCHAR    len
           )
{
    API_RESULT    retval;
    UINT16 min_seg_id;
    UINT16 max_seg_id;

    retval = API_SUCCESS;

    if ((oprtr != APPL_GL_RACP_ALL_RCRD_OPRTR) ||
        (oprtr != APPL_GL_RACP_FRST_RCRD_OPRTR) ||
        (oprtr != APPL_GL_RACP_LST_RCRD_OPRTR))
    {
        switch (oprtr)
        {
        case APPL_GL_RACP_LSS_THN_EQL_TO_OPRTR:
        case APPL_GL_RACP_GRTR_HNN_EQL_TO_OPRTR:
            /** checking length of 3 is 1 byte of filter type and 2 bytes of seq no
             *  checking length of 8 is 1 byte of filter type and 7 bytes of user facing time
             */
            if ((APPL_GL_RACP_SEQ_NO_FILTER == operand[0U] && 3U == len) ||
                (APPL_GL_RACP_USR_FACING_TIME_FILTER == operand[0U] && 8U == len))
            {
                retval = API_SUCCESS;
            }
            else
            {
                retval = APPL_GL_RACP_OPERAND_NOT_SUPPORTED | APPL_ERR_ID;
            }

            break;
        case APPL_GL_RACP_WINTHIN_RNG_OPRTR:
            if (APPL_GL_RACP_SEQ_NO_FILTER == operand[0U] && 5U == len)
            {
                BT_UNPACK_LE_2_BYTE
                (
                    &min_seg_id,
                    &operand[1U]
                );

                BT_UNPACK_LE_2_BYTE
                (
                    &max_seg_id,
                    &operand[3U]
                );

                if (min_seg_id > max_seg_id)
                {
                    retval = APPL_GL_RACP_INVALID_OPERAND | APPL_ERR_ID;
                }
                else
                {
                    retval = API_SUCCESS;
                }
            }
            else
            {
                retval = APPL_GL_RACP_OPERAND_NOT_SUPPORTED | APPL_ERR_ID;
            }

            break;
        default:
            CONSOLE_OUT("Invalid Operator\n");
            break;
        }
    }

    return retval;
}

void appl_gls_manage_transfer (APPL_TIMER_PARAM * appl_timer_param)
{
    API_RESULT     retval;

    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
        retval = BT_stop_timer (timer_handle);

        APPL_TRC (
        "[GLS]: Record Transfer Timer %p Stopped\n",
        timer_handle);

        timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

    retval = BT_start_timer
             (
                 &timer_handle,
                 APPL_RECORD_TRNSFER_INTERVAL,
                 appl_timer_expiry_handler,
                 appl_timer_param,
                 sizeof(APPL_TIMER_PARAM)
             );
    APPL_TRC (
    "[GLS]: Record Transfer Timer %p Started, result 0x%04X!\n",
    timer_handle, retval);
}

void appl_timer_expiry_handler (void *data, UINT16 datalen)
{
    BT_IGNORE_UNUSED_PARAM(datalen);
    API_RESULT         retval;
    APPL_TIMER_PARAM * appl_timer_param;

    appl_timer_param = (APPL_TIMER_PARAM *) data;

    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
        APPL_TRC (
        "[GLS]: Record Transfer Timeout Occurred: %p\n",
        timer_handle);

        timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

    retval = BT_start_timer
             (
                 &timer_handle,
                 APPL_RECORD_TRNSFER_INTERVAL,
                 appl_timer_expiry_handler,
                 appl_timer_param,
                 sizeof (APPL_TIMER_PARAM)
             );

    appl_gl_racp_operation_hndlr(appl_timer_param);

    APPL_TRC (
    "[GLS]: Record Transfer Timer %p Started, result 0x%04X!\n",
    timer_handle, retval);
}

API_RESULT appl_gl_racp_operation_hndlr(APPL_TIMER_PARAM * appl_timer_param)
{
    API_RESULT             retval;

    if (0U != appl_stored_record_count)
    {
        /* Report Stored Records - ALL */
        if (APPL_GL_RACP_STRD_RECORD_REQ_OPCODE == appl_timer_param->appl_req_param.req_op_code &&
            APPL_GL_RACP_SUCCESS == appl_timer_param->rsp_code_val)
        {
            /* Update Operating State */
            appl_gl_operating_state = APPL_RACP_TRANSFR_ONGOING_STATE;
            retval = appl_gl_racp_rprt_strd_record (appl_timer_param);
            if (API_SUCCESS != retval)
            {
                APPL_ERR("[GLS]: **ERR** Failed to send RACP Records,"
                " reason 0x%04X", retval);
            }
        }
        else if (APPL_GL_RACP_CLR_RECORD_REQ_OPCODE == appl_timer_param->appl_req_param.req_op_code &&
            APPL_GL_RACP_SUCCESS == appl_timer_param->rsp_code_val)
        {
            /* Update Operating State */
            appl_gl_operating_state = APPL_RACP_CLR_STATE;
            retval = appl_gl_racp_clear_strd_records (appl_timer_param);
        }
        else
        {
            /* Update Operating State */
            appl_gl_operating_state = APPL_RACP_TRANSFR_ONGOING_STATE;
            retval = appl_gl_racp_send_response (appl_timer_param);
            if (API_SUCCESS != retval)
            {
                APPL_ERR("[GLS]: **ERR** Failed to send RACP Rsp,"
                " reason 0x%04X", retval);
            }
        }
    }
    else
    {
        if (APPL_GL_RACP_STRD_RECORD_CNT_REQ_OPCODE !=
            appl_timer_param->appl_req_param.req_op_code)
        {
            appl_timer_param->rsp_code_val = APPL_GL_RACP_NO_RECORDS_FOUND;
        }
        retval = appl_gl_racp_send_response (appl_timer_param);
        if (API_SUCCESS != retval)
        {
            APPL_ERR("[GLS]: **ERR** Failed to send RACP Rsp,"
            " reason 0x%04X", retval);
        }
    }
    return retval;
}

API_RESULT appl_gl_racp_clear_strd_records(APPL_TIMER_PARAM  *appl_timer_param)
{
    API_RESULT retval;
    UINT16 min_seg_id;
    UINT16 max_seg_id;
    UINT16 seg_id;
    UINT16 index;

    if (APPL_GL_RACP_ALL_RCRD_OPRTR == appl_timer_param->appl_req_param.oprtr)
    {
        for (index = 0U; index <= appl_stored_record_count; index++)
        {
            gls_msrmt_obs_data[gls_msrmt_seq_id_index[index] - 2U] = 0x01U;
        }
        appl_stored_record_count = 0U;
    }
    else if (APPL_GL_RACP_WINTHIN_RNG_OPRTR == appl_timer_param->appl_req_param.oprtr)
    {
        BT_UNPACK_LE_2_BYTE
        (
            &min_seg_id,
            appl_timer_param->appl_req_param.operand + 1U
        );

        BT_UNPACK_LE_2_BYTE
        (
            &max_seg_id,
            appl_timer_param->appl_req_param.operand + 3U
        );

        for (index = 0U; index < appl_stored_record_count; index++)
        {
            BT_UNPACK_LE_2_BYTE
            (
                &seg_id,
                &gls_msrmt_obs_data[gls_msrmt_seq_id_index[index]]
            );

            if (seg_id >= min_seg_id && seg_id <= max_seg_id)
            {
                gls_msrmt_obs_data[gls_msrmt_seq_id_index[index] - 2U] = 0x01U;
                /* appl_stored_record_count --; */
            }
        }
    }
    else
    {
        appl_timer_param->rsp_code_val = APPL_GL_RACP_INVALID_OPERATOR;
    }
    retval = appl_gl_racp_send_response (appl_timer_param);
    if (API_SUCCESS != retval)
    {
        APPL_ERR("[GLS]: **ERR** Failed to send RACP Rsp,"
        " reason 0x%04X", retval);
    }
    return API_SUCCESS;
}

API_RESULT appl_gl_racp_rprt_strd_record(APPL_TIMER_PARAM  *appl_timer_param)
{
    ATT_HANDLE_VALUE_PAIR  appl_gl_msrmt_param = { 0U };
    ATT_HANDLE_VALUE_PAIR  appl_gl_cntxt_msrmt_param = { 0U };
    API_RESULT retval;
    UINT16     seg_index;
    UINT16     oprnd;
    UCHAR      contxt_flag;
    UINT16     index;

    seg_index = 0U;
    oprnd = 0U;
    retval = API_SUCCESS;
    contxt_flag = 0U;
    appl_gl_msrmt_param.handle = ATT_INVALID_ATTR_HANDLE_VAL;

    /* For All Record Operator */
    if (APPL_GL_RACP_ALL_RCRD_OPRTR == appl_timer_param->appl_req_param.oprtr)
    {
        if (0x01U != gls_msrmt_obs_data[gls_msrmt_seq_id_index[appl_record_count] - 2U])
        {
            appl_gl_msrmt_param.handle = appl_gl_msrmt_hndl;
            appl_gl_msrmt_param.value.len = APPL_GLUCOSE_MEASUREMENT_LENGTH;

            if (1U == last_record_update)
            {
                appl_gl_msrmt_param.value.val = &gls_msrmt_obs_data
                    [(gls_msrmt_seq_id_index[appl_stored_record_count - 1U]) - 2U];
            }
            else
            {
                appl_gl_msrmt_param.value.val = &gls_msrmt_obs_data
                [APPL_GLUCOSE_MEASUREMENT_LENGTH * appl_record_count];
            }

            contxt_flag = appl_check_context_info_flag();
        }
        appl_record_count++;
    }
    else if (APPL_GL_RACP_LSS_THN_EQL_TO_OPRTR == appl_timer_param->appl_req_param.oprtr)
    {
        if (APPL_GL_RACP_SEQ_NO_FILTER == appl_timer_param->appl_req_param.operand[0U])
        {
            BT_UNPACK_LE_2_BYTE
            (
                &oprnd,
                &(appl_timer_param->appl_req_param.operand[1U])
            );
        }
        else
        {
            for (index = 0U; index < appl_stored_record_count; index++)
            {
                if (gls_msrmt_obs_data[gls_msrmt_seq_id_index[index] + 5U] ==
                    appl_timer_param->appl_req_param.operand[4U])
                {
                    BT_UNPACK_LE_2_BYTE
                    (
                        &oprnd,
                        &gls_msrmt_obs_data[gls_msrmt_seq_id_index[index]]
                    );
                    break;
                }
            }
        }
        while (appl_record_count < appl_stored_record_count)
        {
            BT_UNPACK_LE_2_BYTE
            (
                &seg_index,
                &(gls_msrmt_obs_data[gls_msrmt_seq_id_index[appl_record_count]])
            );

            if ((seg_index <= oprnd) &&
                (0x01U != gls_msrmt_obs_data[gls_msrmt_seq_id_index[appl_record_count] - 2U]))
            {
                msrmnt_to_send = BT_TRUE;
                appl_timer_param->rsp_code_val = APPL_GL_RACP_SUCCESS;

                appl_gl_msrmt_param.handle = appl_gl_msrmt_hndl;
                appl_gl_msrmt_param.value.len = APPL_GLUCOSE_MEASUREMENT_LENGTH;
                appl_gl_msrmt_param.value.val = &gls_msrmt_obs_data
                    [APPL_GLUCOSE_MEASUREMENT_LENGTH * appl_record_count];
                contxt_flag = appl_check_context_info_flag();
                appl_record_count++;
                break;
            }

            if (0U == msrmnt_to_send)
            {
                appl_timer_param->rsp_code_val = APPL_GL_RACP_NO_RECORDS_FOUND;
            }
            appl_record_count++;
        }
    }
    /* For Greater than or equal to Operator (Seq Id) */
    else if (APPL_GL_RACP_GRTR_HNN_EQL_TO_OPRTR == appl_timer_param->appl_req_param.oprtr)
    {
        if (APPL_GL_RACP_SEQ_NO_FILTER == appl_timer_param->appl_req_param.operand[0U])
        {
            BT_UNPACK_LE_2_BYTE
            (
                &oprnd,
                &(appl_timer_param->appl_req_param.operand[1U])
            );
        }
        else
        {
            for (index = 0U; index < appl_stored_record_count; index++)
            {
                if (gls_msrmt_obs_data[gls_msrmt_seq_id_index[index] + 5U] ==
                    appl_timer_param->appl_req_param.operand[4U])
                {
                    BT_UNPACK_LE_2_BYTE
                    (
                        &oprnd,
                        &gls_msrmt_obs_data[gls_msrmt_seq_id_index[index]]
                    );
                    break;
                }
            }
        }
        while (appl_record_count < appl_stored_record_count)
        {
            BT_UNPACK_LE_2_BYTE
            (
                &seg_index,
                &(gls_msrmt_obs_data[gls_msrmt_seq_id_index[appl_record_count]])
            );

            if ((seg_index >= oprnd) &&
                (0x01U != gls_msrmt_obs_data[gls_msrmt_seq_id_index[appl_record_count] - 2U]))
            {
                msrmnt_to_send = BT_TRUE;
                appl_timer_param->rsp_code_val = APPL_GL_RACP_SUCCESS;

                appl_gl_msrmt_param.handle = appl_gl_msrmt_hndl;
                appl_gl_msrmt_param.value.len = APPL_GLUCOSE_MEASUREMENT_LENGTH;
                appl_gl_msrmt_param.value.val = &gls_msrmt_obs_data
                    [APPL_GLUCOSE_MEASUREMENT_LENGTH * appl_record_count];
                contxt_flag = appl_check_context_info_flag();
                appl_record_count++;
                break;
            }

            if (0U == msrmnt_to_send)
            {
                appl_timer_param->rsp_code_val = APPL_GL_RACP_NO_RECORDS_FOUND;
            }
            appl_record_count++;
        }
    }
    else if (APPL_GL_RACP_WINTHIN_RNG_OPRTR == appl_timer_param->appl_req_param.oprtr)
    {
        if (APPL_GL_RACP_SEQ_NO_FILTER == appl_timer_param->appl_req_param.operand[0U])
        {
            BT_UNPACK_LE_2_BYTE
            (
                &oprnd,
                &(appl_timer_param->appl_req_param.operand[1U])
            );

            BT_UNPACK_LE_2_BYTE
            (
                &index,
                &(appl_timer_param->appl_req_param.operand[3U])
            );

            while (appl_record_count < appl_stored_record_count)
            {
                BT_UNPACK_LE_2_BYTE
                (
                    &seg_index,
                    &(gls_msrmt_obs_data[gls_msrmt_seq_id_index[appl_record_count]])
                );

                if ((seg_index >= oprnd && seg_index <= index) &&
                    (0x01U != gls_msrmt_obs_data[gls_msrmt_seq_id_index[appl_record_count] - 2U]))
                {
                    msrmnt_to_send = BT_TRUE;
                    appl_timer_param->rsp_code_val = APPL_GL_RACP_SUCCESS;

                    appl_gl_msrmt_param.handle = appl_gl_msrmt_hndl;
                    appl_gl_msrmt_param.value.len = APPL_GLUCOSE_MEASUREMENT_LENGTH;
                    appl_gl_msrmt_param.value.val = &gls_msrmt_obs_data
                        [APPL_GLUCOSE_MEASUREMENT_LENGTH * appl_record_count];
                    contxt_flag = appl_check_context_info_flag();
                    appl_record_count++;
                    break;
                }

                if (0U == msrmnt_to_send)
                {
                    appl_timer_param->rsp_code_val = APPL_GL_RACP_NO_RECORDS_FOUND;
                }
                appl_record_count++;
            }

        }
    }
    else if (APPL_GL_RACP_FRST_RCRD_OPRTR == appl_timer_param->appl_req_param.oprtr)
    {
        appl_gl_msrmt_param.handle = appl_gl_msrmt_hndl;
        appl_gl_msrmt_param.value.len = APPL_GLUCOSE_MEASUREMENT_LENGTH;
        appl_gl_msrmt_param.value.val = &gls_msrmt_obs_data
            [APPL_GLUCOSE_MEASUREMENT_LENGTH * appl_record_count];
        contxt_flag = appl_check_context_info_flag();
        appl_record_count += appl_stored_record_count;
    }
    else if (APPL_GL_RACP_LST_RCRD_OPRTR == appl_timer_param->appl_req_param.oprtr)
    {
        appl_record_count = (appl_stored_record_count - 1U);
        appl_gl_msrmt_param.handle = appl_gl_msrmt_hndl;
        appl_gl_msrmt_param.value.len = APPL_GLUCOSE_MEASUREMENT_LENGTH;
        appl_gl_msrmt_param.value.val = &gls_msrmt_obs_data
            [APPL_GLUCOSE_MEASUREMENT_LENGTH * appl_record_count];
        contxt_flag = appl_check_context_info_flag();
        appl_record_count ++;
    }
    else
    {
        appl_timer_param->rsp_code_val = APPL_GL_RACP_INVALID_OPERATOR;
        appl_record_count = appl_stored_record_count;
    }

    if ((APPL_GL_RACP_SUCCESS == appl_timer_param->rsp_code_val) &&
        (ATT_INVALID_ATTR_HANDLE_VAL != appl_gl_msrmt_param.handle))
    {
        appl_gl_msrmt_param.value.len -= 1U;
        appl_gl_msrmt_param.value.val += 1U;

        retval = BT_att_send_hndl_val_ntf
                 (
                     &APPL_GET_ATT_INSTANCE(appl_timer_param->handle),
                     &appl_gl_msrmt_param
                 );
        if (API_SUCCESS != retval)
        {
            appl_timer_param->rsp_code_val = APPL_GL_RACP_PROC_INCOMPLETE;
            retval = appl_gl_racp_send_response (appl_timer_param);
            APPL_ERR("[GLS]: **ERR** Failed to send Glucose measurement, "
            "reason 0x%04X", retval);
        }
        if (BT_FALSE != contxt_flag)
        {
            contxt_flag = 0U;
            appl_gl_cntxt_msrmt_param.handle = appl_gl_cntxt_msrmt_hndl;
            appl_gl_cntxt_msrmt_param.value.len = APPL_GLUCOSE_MEASUREMENT_CONTEXT_LENGTH;

            if (1U == last_record_update)
            {
                appl_gl_cntxt_msrmt_param.value.val = &gls_msrmt_cntxt_obs_data
                [(gls_cntxt_msrmt_seq_id_index\
                [APPL_GLUCOSE_MEASUREMENT_CONTEXT_COUNT - 1U]) - 1U];
            }
            else
            {
                appl_gl_cntxt_msrmt_param.value.val = &gls_msrmt_cntxt_obs_data
                [APPL_GLUCOSE_MEASUREMENT_CONTEXT_LENGTH * (appl_record_count - 2U)];
            }

            retval = BT_att_send_hndl_val_ntf
                     (
                         &APPL_GET_ATT_INSTANCE(appl_timer_param->handle),
                         &appl_gl_cntxt_msrmt_param
                     );
            if (API_SUCCESS != retval)
            {
                appl_timer_param->rsp_code_val = APPL_GL_RACP_PROC_INCOMPLETE;
                retval = appl_gl_racp_send_response (appl_timer_param);
                APPL_ERR("[GLS]: **ERR** Failed to send Glucose Context Measurement,"
                " reason 0x%04X", retval);
            }
        }
    }

    if (appl_stored_record_count == appl_record_count)
    {
        retval = appl_gl_racp_send_response (appl_timer_param);
        if (API_SUCCESS != retval)
        {
            APPL_ERR("[GLS]: **ERR** Failed to send RACP Rsp,"
            " reason 0x%04X", retval);
        }
    }
    return retval;
}

UCHAR appl_check_context_info_flag()
{
    UCHAR ret_val;

    /* Init */
    ret_val = BT_FALSE;

    if (APPL_CONTEXT_INFORMATION_FLAG_MASK == \
        ((gls_msrmt_obs_data[(APPL_GLUCOSE_MEASUREMENT_LENGTH * appl_record_count) + 1U]) & APPL_CONTEXT_INFORMATION_FLAG_MASK))
    {
        ret_val = BT_TRUE; /* return BT_TRUE; */
    }

    return ret_val;
}

API_RESULT appl_gl_racp_send_response (APPL_TIMER_PARAM * appl_timer_param)
{
    API_RESULT             retval;
    ATT_HANDLE_VALUE_PAIR  hndl_val_param;
    UCHAR                  val[4U];
    UINT16                 count;

    /* Initialize record count */
    appl_record_count = 0U;

    if (APPL_GL_RACP_STRD_RECORD_CNT_REQ_OPCODE == appl_timer_param->appl_req_param.req_op_code
        && APPL_GL_RACP_SUCCESS == appl_timer_param->rsp_code_val)
    {
        val[0U] = APPL_GL_RACP_STRD_RECORD_CNT_RSP_OPCODE;
        val[1U] = APPL_GL_RACP_NULL_OPRTR;

        if (APPL_GL_RACP_ALL_RCRD_OPRTR == appl_timer_param->appl_req_param.oprtr)
        {
            count = appl_stored_record_count;
        }
        else if (APPL_GL_RACP_GRTR_HNN_EQL_TO_OPRTR == appl_timer_param->appl_req_param.oprtr)
        {
            count = appl_get_stored_records_count(appl_timer_param, APPL_GL_RACP_GRTR_HNN_EQL_TO_OPRTR);
        }
        else if (APPL_GL_RACP_LSS_THN_EQL_TO_OPRTR == appl_timer_param->appl_req_param.oprtr)
        {
            count = appl_get_stored_records_count(appl_timer_param,APPL_GL_RACP_LSS_THN_EQL_TO_OPRTR);
        }
        else
        {
            /* Need to update for LAST and FIRST Record!! */
            count = appl_get_stored_records_count(appl_timer_param, 0U);
        }
        val[2U] = (UCHAR)(count);
        val[3U] = (UCHAR)(count >> 8U);
    }
    else
    {
        val[0U] = APPL_GL_RACP_RSP_OPCODE;
        val[1U] = APPL_GL_RACP_NULL_OPRTR;
        val[2U] = appl_timer_param->appl_req_param.req_op_code;
        val[3U] = appl_timer_param->rsp_code_val;
    }
    hndl_val_param.handle = appl_gl_racp_hndl;
    hndl_val_param.value.len = 4U;
    hndl_val_param.value.val = &val[0U];

    retval = BT_att_send_hndl_val_ind
             (
                 &APPL_GET_ATT_INSTANCE(appl_timer_param->handle),
                 &hndl_val_param
             );

    /* Intialize the operating state */
    appl_gl_operating_state = APPL_RACP_INIT_STATE;
    msrmnt_to_send = BT_FALSE;

    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
        retval = BT_stop_timer (timer_handle);
        APPL_TRC (
        "[GLS]: Record Transfer Timer %p Stopped, result 0x%04X\n",
            timer_handle, retval);
        timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }
    return retval;
}

UINT16 appl_get_stored_records_count(APPL_TIMER_PARAM * appl_timer_param, UCHAR type)
{
    UINT16  seg_index;
    UINT16  oprnd;
    UINT16  index;
    UINT16  count;

    count = 0U;

    BT_UNPACK_LE_2_BYTE
    (
        &oprnd,
        &(appl_timer_param->appl_req_param.operand[1U])
    );

    for (index = 0U; index < appl_stored_record_count; index++)
    {
        BT_UNPACK_LE_2_BYTE
        (
            &seg_index,
            &(gls_msrmt_obs_data[gls_msrmt_seq_id_index[index]])
        );

        /*For Greater than or equal Operator (Seg ID) */
        if (seg_index >= oprnd && APPL_GL_RACP_GRTR_HNN_EQL_TO_OPRTR == type)
        {
            count++;
        }
        else if (seg_index >= oprnd && APPL_GL_RACP_LSS_THN_EQL_TO_OPRTR == type)
        {
            count++;
        }
    }

    return count;
}

void appl_gls_update_time_offset
     (
         APPL_GLS_USER_FACING_TIME * appl_user_facing_time,
         UCHAR record_no,
         UCHAR total_records
     )
{
    INT16 total_mins;
    INT16 prev_rec_timeoffset;
    unsigned int i;

    if(record_no < 2U)
    {
        APPL_ERR(
        "\n[GLS]: **ERR** Requested to access non existing previous record\n");
        /* return; */
    }
    else
    {
        /* Get the time offset of the previous record */
        BT_UNPACK_LE_2_BYTE
        (
            &prev_rec_timeoffset,
            &gls_msrmt_obs_data[gls_msrmt_seq_id_index[record_no - 2U] + 9U]
        );

        total_mins = appl_gls_convert_from_hrs_to_minutes
                     (
                         appl_user_facing_time,
                         prev_rec_timeoffset
                     );
        /** Total Minutes is equal to
         *  new user facing time + previous records time offset
         */
        CONSOLE_OUT("\n%d hour + %d minutes = %d minutes\n\n", appl_user_facing_time->hours,
        appl_user_facing_time->minutes, total_mins);

        for (i = 0U; i < total_records; i++)
        {
            BT_PACK_LE_2_BYTE
            (
                &gls_msrmt_obs_data[gls_msrmt_seq_id_index[record_no - 1U] + 9U],
                (UINT16 *)&total_mins
            );
            record_no = record_no + 1U;
            total_mins += 1U;
        }
    }
}

INT16 appl_gls_convert_from_hrs_to_minutes
      (
          APPL_GLS_USER_FACING_TIME * appl_user_facing_time,
          INT16                       prev_record_time_offset
      )
{
    INT16 minute;
    INT16 total_minutes;

    minute = appl_user_facing_time->hours * 60U;

    /**
     *  Total minutes can be taken by adding or subtracting the new user facing time
     *  from the previous record time offset
     */
    total_minutes = minute + appl_user_facing_time->minutes + prev_record_time_offset;

    return total_minutes;
}

void appl_activate_gls_records (void)
{
    UINT16 index;

    for (index = 0U; index < appl_stored_record_count; index++)
    {
        gls_msrmt_obs_data[gls_msrmt_seq_id_index[index] - 2U] = 0x00U;
    }
}

API_RESULT appl_gls_racp_write_handler
           (
                ATT_VALUE         * value
           )
{
    API_RESULT          retval;
    API_RESULT          retv;
    RACP_REQ_INFO       gls_racp_req_info;

    retval = API_SUCCESS;

    if (APPL_RACP_INIT_STATE == appl_gl_operating_state ||
        APPL_GL_RACP_ABRT_RPRTING_REQ_OPCODE == value->val[0U])
    {
        if (1U < value->len)
        {
            retv = BT_racp_req_handler
                   (
                       gls_racp_id,
                       value->val,
                       value->len,
                       &gls_racp_req_info
                   );
            if (API_SUCCESS != retv)
            {
                /*
                 * Translating the RACP related error to
                 * GLS related specific error.
                 */
                if ((RACP_ERR_ID | RACP_OPERAND_NOT_SUPPORTED) == retv)
                {
                    retv = (RACP_ERR_ID | RACP_INVALID_OPERAND);
                }

                appl_send_rsp_handler(&gls_racp_req_info, value->len, retv);
            }
        }
        else
        {
            retval = ATT_INVALID_ATTRIBUTE_LEN | APPL_ERR_ID;
        }
    }
    else
    {
        retval = ATT_APPL_PROCEDURE_ALREADY_IN_PROGRESS | APPL_ERR_ID;
    }

    return retval;
}

void appl_send_rsp_handler
    (
        RACP_REQ_INFO   * gls_racp_req_info,
        UINT16            len,
        UINT16            result
    )
{
    API_RESULT retval;
    APPL_TIMER_PARAM    appl_timer_param;

    appl_timer_param.handle = appl_hndl;
    appl_timer_param.rsp_code_val = (UCHAR)result;
    appl_timer_param.appl_req_param.req_op_code = gls_racp_req_info->racp_opcode;
    appl_timer_param.appl_req_param.oprtr = gls_racp_req_info->racp_operator;

    BT_mem_copy
    (
        appl_timer_param.appl_req_param.operand,
        gls_racp_req_info->racp_operand,
        len
    );

    retval = BT_start_timer
             (
                 &timer_handle,
                 APPL_RECORD_TRNSFER_INTERVAL,
                 appl_racp_timer_expiry_handler,
                 &appl_timer_param,
                 sizeof(APPL_TIMER_PARAM)
             );
}

void appl_racp_timer_expiry_handler(void *data, UINT16 datalen)
{
    BT_IGNORE_UNUSED_PARAM(datalen);
    API_RESULT         retval;
    APPL_TIMER_PARAM * appl_timer_param;

    appl_timer_param = (APPL_TIMER_PARAM *)data;

    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
        APPL_TRC(
        "[GLS]: Record Transfer Timeout Occurred %p\n",
        timer_handle);

        timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

    retval = appl_gl_racp_send_response(appl_timer_param);

    APPL_ERR("[GLS]: **ERR** Failed to send Glucose Measurement,"
    " reason 0x%04X", retval);
}

API_RESULT appl_gls_access_record
           (
               RACP_REQ_INFO * req_info,
               UINT16          length
           )
{
    API_RESULT retval;
    APPL_TIMER_PARAM appl_timer_param;

    retval = API_SUCCESS;

    retval = appl_gl_racp_validate_operands
             (
                 req_info->racp_operator,
                 req_info->racp_operand,
                 (UCHAR)(length - 2U)
             );

    if (API_SUCCESS != retval)
    {
        appl_send_rsp_handler(req_info, length, retval);
        /* return retval; */
    }
    else
    {
        appl_timer_param.handle = appl_hndl;
        appl_timer_param.rsp_code_val = APPL_GL_RACP_SUCCESS;
        appl_timer_param.appl_req_param.req_op_code = req_info->racp_opcode;
        appl_timer_param.appl_req_param.oprtr = req_info->racp_operator;

        BT_mem_copy
        (
            appl_timer_param.appl_req_param.operand,
            req_info->racp_operand,
            length
        );

        appl_gls_manage_transfer(&appl_timer_param);
    }

    return retval;
}

API_RESULT appl_gls_report_stored_records
           (
               RACP_REQ_INFO * req_info,
               UINT16          length
           )
{
    API_RESULT retval;

    retval = appl_gls_access_record(req_info, length);

    if (API_SUCCESS != retval)
    {
        APPL_TRC(
        "[GLS]: Accessing the records failed with result 0x%04X\n" ,retval);
    }
    return retval;
}

API_RESULT appl_gls_report_stored_number_of_records
            (
                RACP_REQ_INFO * req_info,
                UINT16          length
            )
{
    API_RESULT retval;

    retval = appl_gls_access_record(req_info, length);

    if (API_SUCCESS != retval)
    {
        APPL_TRC(
        "[GLS]: Accessing the records failed with result 0x%04X\n", retval);
    }
    return retval;
}

API_RESULT appl_gls_delete_stored_record
           (
               RACP_REQ_INFO * req_info,
               UINT16          length
           )
{
    API_RESULT retval;

    retval = appl_gls_access_record(req_info, length);

    if (API_SUCCESS != retval)
    {
        APPL_TRC(
        "[GLS]: Accessing the records failed with result 0x%04X\n", retval);
    }
    return retval;
}

API_RESULT appl_gls_abort_operation
           (
               RACP_REQ_INFO * req_info,
               UINT16          length
           )
{
    API_RESULT retval;

    if ((APPL_RACP_TRANSFR_ONGOING_STATE == appl_gl_operating_state)
        || (APPL_RACP_CLR_STATE == appl_gl_operating_state))
    {
        /* Stop Ongoing transfer */
        if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
        {
            retval = BT_stop_timer(timer_handle);

            APPL_TRC(
            "[GLS]: Record Transfer Timer with result 0x%04X, timer handle %p\n",
            retval, timer_handle);

            timer_handle = BT_TIMER_HANDLE_INIT_VAL;
        }
    }
    appl_gl_operating_state = APPL_RACP_INIT_STATE;

    retval = appl_gls_access_record(req_info, length);

    if (API_SUCCESS != retval)
    {
        APPL_TRC(
        "[GLS]: Accessing the records failed with result 0x%04X\n", retval);
    }

    return retval;
}

void appl_gls_handle_ind_complete
(
    APPL_HANDLE* handle,
    UINT16      evt_result
)
{
    CONSOLE_OUT("\n [GLS]: IND Completed for Appl Handle 0x%02X with result 0x%04X\n",
    *handle, evt_result);
}

void appl_gls_handle_ntf_complete
(
    APPL_HANDLE* handle,
    UCHAR* event_data,
    UINT16      datalen
)
{
    CONSOLE_OUT("\n [GLS]: NTF Sent for Appl Handle 0x%02X\n",
    *handle);
    appl_dump_bytes(event_data, datalen);
}

void appl_gls_handle_mtu_update_complete
(
    APPL_HANDLE* handle,
    UINT16      mtu
)
{
    CONSOLE_OUT("\n [GLS]: Updated MTU is %d for Appl Handle 0x%02X\n",
    mtu, *handle);
}

#endif /* (defined ATT && defined GLS) */

