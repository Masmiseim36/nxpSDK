
/**
 *  \file appl_hid.c
 *
 *  This file contains the Sample HID Boot Mouse application.
 *  Sample applications detailed below:
 *      a. The Sensor, as defined by the Specification plays the GAP Peripheral
 *         role.
 *      b. The Sensor application has following service records:
 *           - GAP
 *           - GATT
 *           - Device Information
 *           - Battery
 *           - Human Interface Device
 *           - Scan Parameter
 *         [NOTE]: Please see gatt_db.c for more details of the record.
 *      c. The Human Interface Device Service exposes:
 *           - Protocol Mode
 *           - Report Map
 *           - Report
 *           - Report Map
 *           - HID Control Point
 *           - HID Information
 *           - Boot Keyboard Input Report
 *           - Boot Keyboard Output Report
 *           - Boot Mouse Input Report
 *      d. Boot Mouse Input Report can be configured to send
 *         Indications and Notifications respectively.
 *      e. appl_manage_transfer routine takes care of handling peer
 *         configuration. This handling would be needed:
 *           - When Peer Configures Event Transfer by writing to the
 *             Characteristic Client Configuration of Boot Mouse Input Report.
 *           - Subsequent reconnection with bonded device that had already
 *             configured the device for transfer. Please note it is mandatory
 *             for GATT Servers to remember the configurations of bonded GATT
 *             clients.
 *         In order to ensure the above mentioned configurations are correctly
 *         handled, the routine, appl_manage_transfer, is therefore called from:
 *           - gatt_db_hid_boot_mouse_event_handler and
 *           - appl_hid_connect
 *         [NOTE]: If link does not have the needed security for the service,
 *         transfer will not be initiated.
 *      f. This sample application, once configured, send different mouse events
 *         every 10s. This interval can be altered using the define
 *         APPL_HID_EVENT_INTERVAL.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_service.h"

#if (defined ATT && defined HID)

/* --------------------------------------------- Global Definitions */
#define APPL_HID_EVENT_INTERVAL                   1U /* 5 */
#define APPL_HID_BOOT_MOUSE_REPORT_LENGTH         4U
#define APPL_HID_BOOT_MOUSE_EVENT_COUNT           6U
#define APPL_HID_BOOT_KBD_REPORT_LENGTH           9U
#define APPL_HID_BOOT_KBD_EVENT_COUNT             12U
#define APPL_BOOT_EVENT_OFFSET                    1U

#define APPL_HID_BOOT_MODE                        0x00U
#define APPL_HID_REPORT_MODE                      0x01U

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */

static BT_timer_handle timer_handle;
static GATT_DB_HANDLE  appl_proto_mode_db_handle;
static GATT_DB_HANDLE  appl_hid_report_map_db_handle;
static GATT_DB_HANDLE  appl_ip_hid_report_db_handle;
static GATT_DB_HANDLE  appl_op_hid_report_db_handle;
static GATT_DB_HANDLE  appl_feature_hid_report_db_handle;
static GATT_DB_HANDLE  appl_hid_cp_db_handle;
static GATT_DB_HANDLE  appl_hid_info_db_handle;
static GATT_DB_HANDLE  appl_hid_db_handle;
static GATT_DB_HANDLE  appl_kb_ip_report_db_handle;
static GATT_DB_HANDLE  appl_kb_op_db_handle;
static GATT_DB_HANDLE  appl_scan_interval_window_db_handle;
static GATT_DB_HANDLE  appl_scan_refresh_db_handle;

static ATT_ATTR_HANDLE appl_keyboard_hid_report_hndl;
static ATT_ATTR_HANDLE appl_keyboard_ip_report_hndl;
static ATT_ATTR_HANDLE appl_mouse_hid_report_hndl;
static ATT_ATTR_HANDLE appl_mouse_ip_report_hndl;

static UCHAR           mouse_event_index;
static UCHAR           kbd_event_index;

static UCHAR           appl_keyboard_hid_report;
static UCHAR           appl_keyboard_input_report;
static UCHAR           appl_mouse_hid_report;
static UCHAR           appl_mouse_ip_report;

static UCHAR           appl_bat_lvl;

static GATT_DB_HANDLE  appl_hid_mouse_proto_mode_db_handle;
static GATT_DB_HANDLE  appl_hid_mouse_report_map_db_handle;
static GATT_DB_HANDLE  appl_hid_mouse_ip_hid_report_db_handle;
static GATT_DB_HANDLE  appl_hid_mouse_op_hid_report_db_handle;
static GATT_DB_HANDLE  appl_hid_mouse_feature_hid_report_db_handle;
static GATT_DB_HANDLE  appl_hid_mouse_cp_db_handle;
static GATT_DB_HANDLE  appl_hid_mouse_hid_info_db_handle;
static GATT_DB_HANDLE  appl_hid_mouse_in_db_handle;

/* Protocol Mode */
/* TODO: Not maintaining per bonded peer device */
static UCHAR appl_hid_mouse_proto_mode[1U];

static UCHAR appl_hid_boot_mouse_event_make[APPL_HID_BOOT_MOUSE_REPORT_LENGTH * APPL_HID_BOOT_MOUSE_EVENT_COUNT] =
{
   /* Mouse Left Button */
   0x01U, 0x00U, 0xF5U, 0x00U,

   /* Mouse Right Button*/
   0x01U, 0x02U, 0x00U, 0x00U,

   /* Mouse Move Left */
   0x01U,0x01U, 0x10U, 0x00U,

   /* Mouse Move Right */
   0x01U, 0x00U, 0x0FU, 0x00U,

   /* Mouse Move Up */
   0x01U, 0x00U, 0x00U, 0xF5U,

   /* Mouse Move Down */
   0x01U, 0x00U, 0x00U, 0x0FU

};
static UCHAR appl_hid_boot_kbd_event_make[APPL_HID_BOOT_KBD_REPORT_LENGTH * APPL_HID_BOOT_KBD_EVENT_COUNT] =
{
    /* small letters */
    0x02U, 0x00U, 0x00U, 0x04U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
    0x02U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,

    0x02U, 0x00U, 0x00U, 0x05U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
    0x02U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,

    0x02U, 0x00U, 0x00U, 0x06U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
    0x02U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,

    0x02U, 0x00U, 0x00U, 0x07U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
    0x02U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,

    0x02U, 0x00U, 0x00U, 0x08U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
    0x02U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,

    0x02U, 0x01U, 0x00U, 0x04U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
    0x02U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
};

/* --------------------------------------------- Functions */

void appl_hid_init(void)
{
#ifdef GATT_DB_DYNAMIC
    #ifdef GATT_DB_HAVE_DB_SIGNATURE
    GATT_DB_SIGNATURE appl_hids_db_sign;
#endif /* GATT_DB_HAVE_DB_SIGNATURE */
    API_RESULT retval = API_FAILURE;
#endif /* GATT_DB_DYNAMIC */

#ifdef GATT_DB_DYNAMIC
    /**
     * APPLICATION NOTE:
     * =================
     * Total number of attributes added by each of the mentioned services
     * listed below are as follows:
     * GAP Service :  6 Attributes
     * GATT Service:  4 Attributes
     * DIS Service :  5 Attributes
     * BAS Service :  5 Attributes
     * HID Keyboard: 25 Attributes
     * HID Mouse   : 23 Attributes
     * SCPS Service:  6 Attributes
     * Totally 74 Attributes get added.
     *
     * The current DIS service used in constructing the HID database has
     * only 2 Characteristics in it, placed as sample reference.
     */
    (BT_IGNORE_RETURN_VALUE) appl_hid_add_gaps();
    (BT_IGNORE_RETURN_VALUE) appl_hid_add_gatts();
    (BT_IGNORE_RETURN_VALUE) appl_hid_add_dis();
    (BT_IGNORE_RETURN_VALUE) appl_hid_add_bas();
    (BT_IGNORE_RETURN_VALUE) appl_hid_add_hid_keyboard();
    (BT_IGNORE_RETURN_VALUE) appl_hid_add_hid_mouse();
    (BT_IGNORE_RETURN_VALUE) appl_add_scps();

#ifdef GATT_DB_HAVE_DB_SIGNATURE
    /* Set the HIDS GATT DB Signature */
    appl_hids_db_sign = APPL_HIDS_GATT_DB_SIGN;
    retval = BT_gatt_db_dyn_set_db_signature(appl_hids_db_sign);
#endif /* GATT_DB_HAVE_DB_SIGNATURE */

    retval = BT_gatt_db_dyn_register();

    APPL_TRC(
    "[HID]: GATT Database Registration Status: 0x%04X\n",
    retval);
#endif /* GATT_DB_DYNAMIC */

    /* Populate the GATT DB HANDLE for Protocol Mode */
    appl_proto_mode_db_handle.device_id  = DEVICE_HANDLE_INIT_VAL;
    appl_proto_mode_db_handle.service_id = (UCHAR)GATT_SER_HID_KEYBOARD_INST;
    appl_proto_mode_db_handle.char_id    = (UCHAR)GATT_CHAR_HID_KEYBOARD_PROTO_MODE_INST;;

    /* Populate the GATT DB HANDLE for Report Map */
    appl_hid_report_map_db_handle.device_id  = DEVICE_HANDLE_INIT_VAL;
    appl_hid_report_map_db_handle.service_id = (UCHAR)GATT_SER_HID_KEYBOARD_INST;
    appl_hid_report_map_db_handle.char_id    = (UCHAR)GATT_CHAR_HID_KEYBOARD_REPORT_MAP_INST;

    /* Populate the GATT DB HANDLE for Input HID Report */
    appl_ip_hid_report_db_handle.device_id  = DEVICE_HANDLE_INIT_VAL;
    appl_ip_hid_report_db_handle.service_id = (UCHAR)GATT_SER_HID_KEYBOARD_INST;
    appl_ip_hid_report_db_handle.char_id    = (UCHAR)GATT_CHAR_HID_KEYBOARD_REPORT_INST0;

    /* Populate the GATT DB HANDLE for Output HID Report */
    appl_op_hid_report_db_handle.device_id  = DEVICE_HANDLE_INIT_VAL;
    appl_op_hid_report_db_handle.service_id = (UCHAR)GATT_SER_HID_KEYBOARD_INST;
    appl_op_hid_report_db_handle.char_id    = (UCHAR)GATT_CHAR_HID_KEYBOARD_REPORT_INST1;

    /* Populate the GATT DB HANDLE for feature HID Report */
    appl_feature_hid_report_db_handle.device_id  = DEVICE_HANDLE_INIT_VAL;
    appl_feature_hid_report_db_handle.service_id = (UCHAR)GATT_SER_HID_KEYBOARD_INST;
    appl_feature_hid_report_db_handle.char_id    = (UCHAR)GATT_CHAR_HID_KEYBOARD_REPORT_INST2;

    /* Populate the GATT DB HANDLE for HID control point */
    appl_hid_cp_db_handle.device_id  = DEVICE_HANDLE_INIT_VAL;
    appl_hid_cp_db_handle.service_id = (UCHAR)GATT_SER_HID_KEYBOARD_INST;
    appl_hid_cp_db_handle.char_id    = (UCHAR)GATT_CHAR_HID_KEYBOARD_CNTRL_PNT_INST;

    /* Populate the GATT DB HANDLE for HID Information */
    appl_hid_info_db_handle.device_id  = DEVICE_HANDLE_INIT_VAL;
    appl_hid_info_db_handle.service_id = (UCHAR)GATT_SER_HID_KEYBOARD_INST;
    appl_hid_info_db_handle.char_id = (UCHAR)GATT_CHAR_HID_KEYBOARD_HID_INFO_INST;

    /* Populate the GATT DB HANDLE for BootKeyboardInputReport */
    appl_kb_ip_report_db_handle.device_id  = DEVICE_HANDLE_INIT_VAL;
    appl_kb_ip_report_db_handle.service_id = (UCHAR)GATT_SER_HID_KEYBOARD_INST;
    appl_kb_ip_report_db_handle.char_id    = (UCHAR)GATT_CHAR_HID_KEYBOARD_KBD_IN_INST;

    /* Populate the GATT DB HANDLE for BootKeyboardOutputReport */
    appl_kb_op_db_handle.device_id  = DEVICE_HANDLE_INIT_VAL;
    appl_kb_op_db_handle.service_id = (UCHAR)GATT_SER_HID_KEYBOARD_INST;
    appl_kb_op_db_handle.char_id    = (UCHAR)GATT_CHAR_HID_KEYBOARD_KBD_OUT_INST;

    /* GATT DB handle population for 2nd Instance */
    /* Populate the GATT DB HANDLE for Protocol Mode of 2nd instance */
    appl_hid_mouse_proto_mode_db_handle.device_id  = DEVICE_HANDLE_INIT_VAL;
    appl_hid_mouse_proto_mode_db_handle.service_id = (UCHAR)GATT_SER_HID_MOUSE_INST;
    appl_hid_mouse_proto_mode_db_handle.char_id    = (UCHAR)GATT_CHAR_HID_MOUSE_PROTO_MODE_INST;

    /* Populate the GATT DB HANDLE for Report Map of 2nd instance */
    appl_hid_mouse_report_map_db_handle.device_id  = DEVICE_HANDLE_INIT_VAL;
    appl_hid_mouse_report_map_db_handle.service_id = (UCHAR)GATT_SER_HID_MOUSE_INST;
    appl_hid_mouse_report_map_db_handle.char_id    = (UCHAR)GATT_CHAR_HID_MOUSE_REPORT_MAP_INST;

    /* Populate the GATT DB HANDLE for Input HID Report of 2nd instance */
    appl_hid_mouse_ip_hid_report_db_handle.device_id  = DEVICE_HANDLE_INIT_VAL;
    appl_hid_mouse_ip_hid_report_db_handle.service_id = (UCHAR)GATT_SER_HID_MOUSE_INST;
    appl_hid_mouse_ip_hid_report_db_handle.char_id    = (UCHAR)GATT_CHAR_HID_MOUSE_REPORT_INST0;

    /* Populate the GATT DB HANDLE for Output HID Report of 2nd instance */
    appl_hid_mouse_op_hid_report_db_handle.device_id  = DEVICE_HANDLE_INIT_VAL;
    appl_hid_mouse_op_hid_report_db_handle.service_id = (UCHAR)GATT_SER_HID_MOUSE_INST;
    appl_hid_mouse_op_hid_report_db_handle.char_id    = (UCHAR)GATT_CHAR_HID_MOUSE_REPORT_INST1;

    /* Populate the GATT DB HANDLE for feature HID Report of 2nd instance */
    appl_hid_mouse_feature_hid_report_db_handle.device_id  = DEVICE_HANDLE_INIT_VAL;
    appl_hid_mouse_feature_hid_report_db_handle.service_id = (UCHAR)GATT_SER_HID_MOUSE_INST;
    appl_hid_mouse_feature_hid_report_db_handle.char_id    = (UCHAR)GATT_CHAR_HID_MOUSE_REPORT_INST2;

    /* Populate the GATT DB HANDLE for HID control point of 2nd instance */
    appl_hid_mouse_cp_db_handle.device_id  = DEVICE_HANDLE_INIT_VAL;
    appl_hid_mouse_cp_db_handle.service_id = (UCHAR)GATT_SER_HID_MOUSE_INST;
    appl_hid_mouse_cp_db_handle.char_id    = (UCHAR)GATT_CHAR_HID_MOUSE_CNTRL_PNT_INST;

    /* Populate the GATT DB HANDLE for HID Information of 2nd instance */
    appl_hid_mouse_hid_info_db_handle.device_id  = DEVICE_HANDLE_INIT_VAL;
    appl_hid_mouse_hid_info_db_handle.service_id = (UCHAR)GATT_SER_HID_MOUSE_INST;
    appl_hid_mouse_hid_info_db_handle.char_id    = (UCHAR)GATT_CHAR_HID_MOUSE_HID_INFO_INST;

    /* Populate the GATT DB HANDLE for Boot Mouse Input of 2nd instance */
    appl_hid_mouse_in_db_handle.device_id = DEVICE_HANDLE_INIT_VAL;
    appl_hid_mouse_in_db_handle.service_id = (UCHAR)GATT_SER_HID_MOUSE_INST;
    appl_hid_mouse_in_db_handle.char_id = (UCHAR)GATT_CHAR_HID_MOUSE_IN_INST;

    /* Populate the GATT DB HANDLE for ScanIntervalWindow */
    appl_scan_interval_window_db_handle.device_id  = DEVICE_HANDLE_INIT_VAL;
    appl_scan_interval_window_db_handle.service_id = (UCHAR)GATT_SER_HID_SCAN_PARAM_INST;
    appl_scan_interval_window_db_handle.char_id    = (UCHAR)GATT_CHAR_HID_SCAN_INTRVL_WNDW_INST;

    /* Populate the GATT DB HANDLE for ScanRefresh */
    appl_scan_refresh_db_handle.device_id  = DEVICE_HANDLE_INIT_VAL;
    appl_scan_refresh_db_handle.service_id = (UCHAR)GATT_SER_HID_SCAN_PARAM_INST;
    appl_scan_refresh_db_handle.char_id    = (UCHAR)GATT_CHAR_HID_SCAN_REFRESH_INST;

#ifndef GATT_DB_DYNAMIC
    appl_hid_gatt_db_register();
#endif /* GATT_DB_DYNAMIC */

    /* Register HID GATT DB Handler with PL Extension */
    gatt_db_init_pl(gatt_db_hid_gatt_char_handler);

    timer_handle = BT_TIMER_HANDLE_INIT_VAL;

    /* By default, Report Mode */
    appl_hid_mouse_proto_mode[0U] = APPL_HID_REPORT_MODE;
}

void appl_hid_bt_init(void)
{
    APPL_TRC("[HIDS]: In appl_hids_bt_init\n");

    /* Set GAP Role */
    APPL_SET_GAP_PERIPHERAL_ROLE();

    appl_hid_server_reinitialize();
}

void appl_hid_bt_deinit(void)
{
    APPL_TRC("[HIDS]: In appl_hids_bt_deinit\n");

    /* Stop Timer */
    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
        BT_stop_timer(timer_handle);
        timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }
}


void appl_hid_connect(DEVICE_HANDLE  * dq_handle)
{
    ATT_VALUE         value;
    API_RESULT        retval;
    UINT16            cli_cnfg;

    cli_cnfg = 0U;

    appl_hid_db_handle.device_id  = (*dq_handle);

    value.len = 1U;
    value.val = &appl_hid_mouse_proto_mode[0U];

    /* Update Protocol Mode Value with Report Mode */
    appl_hid_db_handle.char_id = (UCHAR)GATT_CHAR_HID_KEYBOARD_PROTO_MODE_INST;
    retval = BT_gatt_db_set_char_val
             (
                 &appl_hid_db_handle,
                 &value
             );

    /* If HID Input Report is configured, initiate transfer */
    appl_hid_db_handle.char_id = (UCHAR)GATT_CHAR_HID_KEYBOARD_REPORT_INST0;

    retval = BT_gatt_db_get_char_val_hndl
             (
                  &appl_hid_db_handle,
                  &appl_keyboard_hid_report_hndl
             );
    retval = BT_gatt_db_get_char_cli_cnfg(&appl_hid_db_handle,&value);
    if (API_SUCCESS != retval)
    {
        cli_cnfg = 0x0000U;
    }
    else
    {
        BT_UNPACK_LE_2_BYTE(&cli_cnfg, value.val);
    }
    if (0U != cli_cnfg)
    {
        appl_manage_trasnfer (appl_hid_db_handle,cli_cnfg);
    }

    APPL_TRC(
    "[HID]: Fetched Client Configuration (0x%04X) for Device (0x%02X) with "
    "result 0x%04X\n", cli_cnfg, (*dq_handle), retval);

    /* If BootKeyboardInputReport is configured, initiate transfer */
    appl_hid_db_handle.char_id = (UCHAR)GATT_CHAR_HID_KEYBOARD_KBD_IN_INST;

    retval = BT_gatt_db_get_char_val_hndl
    (
        &appl_hid_db_handle,
        &appl_keyboard_ip_report_hndl
    );
    retval = BT_gatt_db_get_char_cli_cnfg(&appl_hid_db_handle, &value);
    if (API_SUCCESS != retval)
    {
        cli_cnfg = 0x0000U;
    }
    else
    {
        BT_UNPACK_LE_2_BYTE(&cli_cnfg, value.val);
    }
    if (0U != cli_cnfg)
    {
        appl_manage_trasnfer(appl_hid_db_handle, cli_cnfg);
    }

    APPL_TRC(
    "[HID]: Fetched Client Configuration (0x%04X) for Device (0x%02X) with "
    "result 0x%04X\n", cli_cnfg, (*dq_handle), retval);

    /* For 2nd Instance */
    value.len = 1U;
    value.val = &appl_hid_mouse_proto_mode[0U];

    /* Update Protocol Mode Value with Report Mode */
    appl_hid_db_handle.char_id = (UCHAR)GATT_CHAR_HID_MOUSE_PROTO_MODE_INST;
    retval = BT_gatt_db_set_char_val
    (
        &appl_hid_db_handle,
        &value
    );

    /* If HID Input Report is configured, initiate transfer */
    appl_hid_db_handle.char_id = (UCHAR)GATT_CHAR_HID_MOUSE_REPORT_INST0;

    retval = BT_gatt_db_get_char_val_hndl
    (
        &appl_hid_db_handle,
        &appl_mouse_hid_report_hndl
    );
    retval = BT_gatt_db_get_char_cli_cnfg(&appl_hid_db_handle, &value);
    if (API_SUCCESS != retval)
    {
        cli_cnfg = 0x0000U;
    }
    else
    {
        BT_UNPACK_LE_2_BYTE(&cli_cnfg, value.val);
    }

    if (0U != cli_cnfg)
    {
        appl_manage_trasnfer(appl_hid_db_handle, cli_cnfg);
    }

    APPL_TRC(
    "[HID]: Fetched Client Configuration (0x%04X) for Device (0x%02X) with "
    "result 0x%04X\n", cli_cnfg, (*dq_handle), retval);

    /* If BootMouseInputReport is configured, initiate transfer */
    appl_hid_db_handle.char_id = (UCHAR)GATT_CHAR_HID_MOUSE_IN_INST;

    retval = BT_gatt_db_get_char_val_hndl
             (
                 &appl_hid_db_handle,
                 &appl_mouse_ip_report_hndl
             );

    retval = BT_gatt_db_get_char_cli_cnfg(&appl_hid_db_handle, &value);

    if (API_SUCCESS != retval)
    {
        cli_cnfg = 0x0000U;
    }
    else
    {
        BT_UNPACK_LE_2_BYTE(&cli_cnfg, value.val);
    }

    if (0U != cli_cnfg)
    {
        appl_manage_trasnfer(appl_hid_db_handle, cli_cnfg);
    }

    APPL_TRC(
    "[HID]: Fetched Client Configuration (0x%04X) for Device (0x%02X) with "
    "result 0x%04X\n", cli_cnfg, (*dq_handle), retval);
}


void appl_manage_trasnfer (GATT_DB_HANDLE handle, UINT16 config)
{
    API_RESULT     retval;
    APPL_HANDLE    appl_handle;

    /* Init */
    retval = API_SUCCESS;

    /* Verify if security requirements are available with the link */
    retval = BT_gatt_db_assert_security(&handle);

    /* Security requirements satisfied? */
    if (API_SUCCESS == retval)
    {
        /**
         *  Security requirements satisfied.
         *
         *  Check if CCCD configured for HID Boot/Report Mode Input.
         *  Initiate transfer of simulated reported.
         */

        /**
         *  TODO: If the Input Report Descriptor position changes
         *  in the database, the following piece of code also to
         *  be modified.
         */
        appl_get_handle_from_device_handle (handle.device_id,&appl_handle);

        if (GATT_CLI_CNFG_NOTIFICATION == config)
        {
            if (GATT_CHAR_HID_KEYBOARD_REPORT_INST0 == handle.char_id)
            {
                appl_keyboard_hid_report = BT_TRUE;
            }
            else if (GATT_CHAR_HID_KEYBOARD_KBD_IN_INST == handle.char_id)
            {
                appl_keyboard_input_report = BT_TRUE;
            }
            else if (GATT_CHAR_HID_MOUSE_REPORT_INST0 == handle.char_id)
            {
                appl_mouse_hid_report = BT_TRUE;
            }
            else if (GATT_CHAR_HID_MOUSE_IN_INST == handle.char_id)
            {
                appl_mouse_ip_report = BT_TRUE;
            }
            else
            {
                /* Do Nothing */
                retval = API_FAILURE; /* return; */
            }

            if (API_SUCCESS == retval)
            {
                /* Start timer to simulate HID boot mouse events */
                if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
                {
                    retval = BT_stop_timer (timer_handle);

                    APPL_TRC(
                    "[HID]: Boot Mouse event Timer %p Stopped, result 0x%04X!\n",
                    timer_handle, retval);

                    timer_handle = BT_TIMER_HANDLE_INIT_VAL;
                }

                retval = BT_start_timer
                         (
                             &timer_handle,
                             APPL_HID_EVENT_INTERVAL,
                             appl_timer_expiry_handler,
                             &appl_handle,
                             sizeof (appl_handle)
                         );

                APPL_TRC(
                "[HID]: Boot Mouse event Timer %p Started, result 0x%04X!\n",
                timer_handle, retval);
            }
        }
        else if (GATT_CLI_CNFG_DEFAULT == config)
        {
            if (GATT_CHAR_HID_KEYBOARD_REPORT_INST0 == handle.char_id)
            {
                appl_keyboard_hid_report = BT_FALSE;
            }
            else if (GATT_CHAR_HID_KEYBOARD_KBD_IN_INST == handle.char_id)
            {
                appl_keyboard_input_report = BT_FALSE;
            }
            else if (GATT_CHAR_HID_MOUSE_REPORT_INST0 == handle.char_id)
            {
                appl_mouse_hid_report = BT_FALSE;
            }
            else if (GATT_CHAR_HID_MOUSE_IN_INST == handle.char_id)
            {
                appl_mouse_ip_report = BT_FALSE;
            }
            else
            {
                /* Do Nothing */
                retval = API_FAILURE; /* return; */
            }

            if (API_SUCCESS == retval)
            {
                /* Stop timer to simulate HID boot mouse events */
                if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
                {
                    retval = BT_stop_timer (timer_handle);

                    APPL_TRC(
                    "[HID]: Boot Mouse event Timer %p Stopped, result 0x%04X!\n",
                    timer_handle, retval);

                    timer_handle = BT_TIMER_HANDLE_INIT_VAL;
                }
            }
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }
    }
}

API_RESULT appl_manage_wwr (GATT_DB_HANDLE * handle, GATT_DB_PARAMS * params)
{
    APPL_HANDLE    appl_handle;
    API_RESULT     retval;

    retval = API_SUCCESS;

    appl_get_handle_from_device_handle (handle->device_id,&appl_handle);

    if ((GATT_CHAR_HID_KEYBOARD_PROTO_MODE_INST == handle->char_id) ||
        (GATT_CHAR_HID_MOUSE_PROTO_MODE_INST == handle->char_id))
    {
        /* Verify and Save Protocol Mode */
        if ((1U == params->value.len) &&
            (APPL_HID_REPORT_MODE >= params->value.val[0U]))
        {
            /* Update Protocol Mode Value */
            BT_gatt_db_set_char_val
            (
                handle,
                &params->value
            );
            APPL_TRC(
            "[HID]: Successfully updated Protocol Mode Value\n");
        }
        else
        {
            retval = API_FAILURE;
            APPL_TRC(
            "[HID]: Failed to update Protocol Mode Value\n");
        }
    }
    return retval;
}

void appl_timer_expiry_handler (void *data, UINT16 datalen)
{
    APPL_EVENT_PARAM   fsm_param;
    API_RESULT         retval;

    BT_IGNORE_UNUSED_PARAM(datalen);

    fsm_param.handle = (*((APPL_HANDLE *)data));

    /* Restart timer to simulate HID Boot Mouse events */
    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
      /* APPL_TRC(
        "[HID]: Boot Mouse event Timeout Occurred: %p\n", timer_handle); */

        timer_handle = BT_TIMER_HANDLE_INIT_VAL;
    }

    retval = BT_start_timer
             (
                 &timer_handle,
                 APPL_HID_EVENT_INTERVAL,
                 appl_timer_expiry_handler,
                 &fsm_param.handle,
                 sizeof(fsm_param.handle)
             );

#ifdef APPL_FSM_SUPPORT
    /* Post Measurement Indication to APPL FSM */
    fsm_post_event
    (
         APPL_FSM_ID,
         ev_appl_measurement_ind,
         &fsm_param
    );
#else /* APPL_FSM_SUPPORT */
    APPL_SEND_MEASUREMENT(&fsm_param.handle);
#endif /* APPL_FSM_SUPPORT */

  /*APPL_TRC(
    "[HID]: Boot Mouse event Timer %p Started, result 0x%04X!\n",
    timer_handle, retval); */
}


void appl_hid_server_reinitialize (void)
{
    /* Stop Timer for HID boot mouse events */
    if (BT_TIMER_HANDLE_INIT_VAL != timer_handle)
    {
        BT_stop_timer (timer_handle);
    }

    timer_handle = BT_TIMER_HANDLE_INIT_VAL;

#if ((defined APPL_GAP_BROACASTER) || defined (APPL_GAP_PERIPHERAL))
    /* Configure and Enable Advertising */
    if (BT_TRUE == APPL_IS_GAP_PERIPHERAL_ROLE())
    {
        appl_service_configure_adv(APPL_GAP_PROC_NORMAL, HCI_ADV_IND, 0x00U, 0x00U, NULL, 0x00U);
        appl_service_enable_adv(0x01U);
    }
#endif /* ((defined APPL_GAP_BROACASTER) || defined (APPL_GAP_PERIPHERAL)) */

#if ((defined APPL_GAP_OBSERVER) || (defined APPL_GAP_CENTRAL))
    /* Configure and Enable Scanning */
    if (BT_TRUE == APPL_IS_GAP_CENTRAL_ROLE())
    {
        appl_service_configure_scan(APPL_GAP_PROC_NORMAL, 0x00U, 0x00U, 0x00U);
        appl_service_enable_scan(0x01U);
    }
#endif /* ((defined APPL_GAP_OBSERVER) || (defined APPL_GAP_CENTRAL)) */
}

void appl_send_hid_event (APPL_HANDLE    * handle)
{
    ATT_HANDLE_VALUE_PAIR hndl_val_param;
    API_RESULT            retval;

    /*  Simulation to send HID events perodically */
    if (APPL_HID_BOOT_MOUSE_EVENT_COUNT == mouse_event_index)
    {
        mouse_event_index = 0U;
    }

    if (APPL_HID_BOOT_KBD_EVENT_COUNT == kbd_event_index)
    {
        kbd_event_index = 0U;
    }

    if (0U != appl_mouse_ip_report)
    {
        hndl_val_param.handle = appl_mouse_ip_report_hndl;
        hndl_val_param.value.val = (&appl_hid_boot_mouse_event_make[(APPL_HID_BOOT_MOUSE_REPORT_LENGTH * mouse_event_index++) + APPL_BOOT_EVENT_OFFSET]);
        hndl_val_param.value.len = APPL_HID_BOOT_MOUSE_REPORT_LENGTH;

        retval = BT_att_send_hndl_val_ntf
                 (
                     &APPL_GET_ATT_INSTANCE(*handle),
                     &hndl_val_param
                  );
        if (API_SUCCESS != retval)
        {
            APPL_ERR("[HID]: **ERR** Failed to send Mouse make event, reason 0x%04X",
            retval);
        }

        hndl_val_param.value.val = (&appl_hid_boot_mouse_event_make[(APPL_HID_BOOT_MOUSE_REPORT_LENGTH * mouse_event_index++) + APPL_BOOT_EVENT_OFFSET]);

        retval = BT_att_send_hndl_val_ntf
                 (
                     &APPL_GET_ATT_INSTANCE(*handle),
                     &hndl_val_param
                  );
        if (API_SUCCESS != retval)
        {
            APPL_ERR("[HID]: **ERR** Failed to send Mouse make event, reason 0x%04X",
            retval);
        }
    }

    if (0U != appl_keyboard_input_report)
    {
        /* Send Make Event */
        hndl_val_param.handle = appl_keyboard_ip_report_hndl;
        hndl_val_param.value.val = (&appl_hid_boot_kbd_event_make[(APPL_HID_BOOT_KBD_REPORT_LENGTH * kbd_event_index++) + APPL_BOOT_EVENT_OFFSET]);
        hndl_val_param.value.len = APPL_HID_BOOT_KBD_REPORT_LENGTH;

        retval = BT_att_send_hndl_val_ntf
                 (
                     &APPL_GET_ATT_INSTANCE(*handle),
                     &hndl_val_param
                  );
        if (API_SUCCESS != retval)
        {
            APPL_ERR("[HID]: **ERR** Failed to send Mouse make event, reason 0x%04X",
            retval);
        }

        hndl_val_param.value.val = (&appl_hid_boot_kbd_event_make[(APPL_HID_BOOT_KBD_REPORT_LENGTH * kbd_event_index++) + APPL_BOOT_EVENT_OFFSET]);

        retval = BT_att_send_hndl_val_ntf
                 (
                     &APPL_GET_ATT_INSTANCE(*handle),
                     &hndl_val_param
                  );
        if (API_SUCCESS != retval)
        {
            APPL_ERR("[HID]: **ERR** Failed to send Mouse make event, reason 0x%04X",
            retval);
        }

        /* Send Break Event */
        hndl_val_param.value.val = (appl_hid_boot_kbd_event_make+
                        (APPL_HID_BOOT_KBD_REPORT_LENGTH*kbd_event_index++));

        retval = BT_att_send_hndl_val_ntf
                 (
                     &APPL_GET_ATT_INSTANCE(*handle),
                     &hndl_val_param
                  );
        if (API_SUCCESS != retval)
        {
            APPL_ERR("[HID]: **ERR** Failed to send Mouse make event, reason 0x%04X",
            retval);
        }
    }

    /* Both the instances are having Input HIDReport characteritsic */
    if (0U != appl_mouse_hid_report)
    {
        retval = appl_mouse_hid_report_handler(handle);
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }

    if (0U != appl_keyboard_hid_report)
    {
        retval = appl_keyboard_hid_report_handler(handle);
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }
}

API_RESULT appl_mouse_hid_report_handler(APPL_HANDLE * appl_handle)
{
    API_RESULT retval;

    retval = appl_notify_hid_report
             (
                 appl_handle,
                 appl_mouse_hid_report_hndl
             );

    APPL_TRC(
    "[HID]: appl_mouse_hid_report_handler Status: 0x%04X\n", retval);

    return retval;
}

API_RESULT appl_keyboard_hid_report_handler(APPL_HANDLE * appl_handle)
{
    API_RESULT retval;

    retval = appl_notify_hid_report
             (
                 appl_handle,
                 appl_keyboard_hid_report_hndl
             );

    APPL_TRC(
    "[HID]: appl_keyboard_hid_report_handler Status: 0x%04X\n", retval);

    return retval;
}

API_RESULT appl_notify_hid_report
           (
               APPL_HANDLE * hndl,
               ATT_ATTR_HANDLE attr_hndl
           )
{
    API_RESULT retval;
    ATT_HANDLE_VALUE_PAIR hndl_val_param;

    retval = API_SUCCESS;

    hndl_val_param.handle = attr_hndl;

    /* Send Key board Event */
    if ((APPL_HID_BOOT_KBD_EVENT_COUNT > kbd_event_index) &&
        (appl_keyboard_hid_report_hndl == attr_hndl))
    {
        /* Send Keyboard Event */
        hndl_val_param.value.val = (appl_hid_boot_kbd_event_make +
            (APPL_HID_BOOT_KBD_REPORT_LENGTH * kbd_event_index++));
        hndl_val_param.value.len = APPL_HID_BOOT_KBD_REPORT_LENGTH;

        retval = BT_att_send_hndl_val_ntf
                 (
                     &APPL_GET_ATT_INSTANCE(*hndl),
                     &hndl_val_param
                 );

        if (API_SUCCESS != retval)
        {
            APPL_ERR("[HID]: **ERR** Failed to send Keyboard event, reason 0x%04X",
            retval);
        }

        /* Send Break Event */
        hndl_val_param.value.val = (appl_hid_boot_kbd_event_make +
            (APPL_HID_BOOT_KBD_REPORT_LENGTH * kbd_event_index++));

        retval = BT_att_send_hndl_val_ntf
                 (
                     &APPL_GET_ATT_INSTANCE(*hndl),
                     &hndl_val_param
                 );

        if (API_SUCCESS != retval)
        {
            APPL_ERR("[HID]: **ERR** Failed to send Keyboard event, reason 0x%04X",
            retval);
        }
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }

    if ((APPL_HID_BOOT_MOUSE_EVENT_COUNT > mouse_event_index) &&
        (appl_mouse_hid_report_hndl == attr_hndl))
    {
        hndl_val_param.value.val = (appl_hid_boot_mouse_event_make +
            (APPL_HID_BOOT_MOUSE_REPORT_LENGTH * mouse_event_index++));
        hndl_val_param.value.len = APPL_HID_BOOT_MOUSE_REPORT_LENGTH;

        retval = BT_att_send_hndl_val_ntf
                 (
                     &APPL_GET_ATT_INSTANCE(*hndl),
                     &hndl_val_param
                 );

        if (API_SUCCESS != retval)
        {
            APPL_ERR("[HID]: **ERR** Failed to send Mouse event, reason 0x%04X",
            retval);
        }

        hndl_val_param.value.val = (appl_hid_boot_mouse_event_make +
            (APPL_HID_BOOT_MOUSE_REPORT_LENGTH * mouse_event_index++));

        retval = BT_att_send_hndl_val_ntf
                 (
                     &APPL_GET_ATT_INSTANCE(*hndl),
                     &hndl_val_param
                 );

        if (API_SUCCESS != retval)
        {
            APPL_ERR("[HID]: **ERR** Failed to send Mouse event, reason 0x%04X",
            retval);
        }
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }

    return retval;
}

void appl_hid_handle_ind_complete
     (
         APPL_HANDLE * handle,
         UINT16      evt_result
     )
{
    APPL_TRC("\n[HID]: IND Completed for Appl Handle 0x%02X with result 0x%04X\n",
    *handle, evt_result);
}

void appl_hid_handle_ntf_complete
     (
         APPL_HANDLE * handle,
         UCHAR       * event_data,
         UINT16      datalen
     )
{
    BT_IGNORE_UNUSED_PARAM(handle);
    BT_IGNORE_UNUSED_PARAM(event_data);
    BT_IGNORE_UNUSED_PARAM(datalen);

#ifdef APPL_HID_HAVE_DATA_DUMP
    APPL_TRC("\n[HID]: NTF Sent for Appl Handle 0x%02X\n", *handle);
    appl_dump_bytes(event_data, datalen);
#endif /* APPL_HID_HAVE_DATA_DUMP */
}

void appl_hid_handle_mtu_update_complete
     (
         APPL_HANDLE * handle,
         UINT16      mtu
     )
{
    APPL_TRC("\n[HID]: Updated MTU is %d for Appl Handle 0x%02X\n",
    mtu, *handle);
}
#endif /* (defined ATT && defined HID) */

