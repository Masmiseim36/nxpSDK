
/**
 *  \file appl_gatt_client.c
 *
 *  This file contains the ATT client application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_gatt_client.h"
#include "appl_gatt_server.h"
#include "appl_smp.h"

#include "gatt.h"
#include "BT_gatt_db_api.h"

/* Configure logs for the functions in this file. */
#ifdef IOT_LOG_LEVEL_GLOBAL
    #define LIBRARY_LOG_LEVEL    IOT_LOG_LEVEL_GLOBAL
#else
    #define LIBRARY_LOG_LEVEL    IOT_LOG_NONE
#endif

#define LIBRARY_LOG_NAME         ( "APPL_GATT_CLIENT" )
#include "iot_logging_setup.h"

#ifdef ATT

/* #define APPL_AUTO_DISCOVERY_ON_CONNECTION */

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */
ATT_HANDLE appl_gatt_client_handle;

/* --------------------------------------------- Static Global Variables */
API_RESULT appl_gatt_cb
           (
               ATT_HANDLE    * handle,
               UCHAR         att_event,
               API_RESULT    event_result,
               void          * event_data,
               UINT16        event_datalen
           );

void appl_gatt_client_operations(void);
UCHAR * appl_display_uuid_label(UINT16 uuid);

#ifdef ATT_SUPPORT_128_BIT_UUID
void appl_print_128_bit_uuid (ATT_UUID128 * uuid);
#endif /* ATT_SUPPORT_128_BIT_UUID */

#ifdef SMP_DATA_SIGNING
static ATT_SIGNED_WRITE_CMD_PARAM sign_write_req_param;
static UCHAR sign_value[23];
static UCHAR * s_data;
static UINT16 s_datalen;
#endif /* SMP_DATA_SIGNING */

#ifdef APPL_AUTO_DISCOVERY_ON_CONNECTION
static UCHAR auto_discovery;
#endif /* APPL_AUTO_DISCOVERY_ON_CONNECTION */

/**
 * Default: Set False
 * On General Service Discovery: Set True
 */
static UCHAR general_service_discovery;

/* Peer device bluetooth address */
static BT_DEVICE_ADDR gatt_peer_address;

static UCHAR *profile_client_menu = (UCHAR *) \
 "\n--------------------------------------------\n"\
   "            PROFILE CLIENT MENU\n"\
   "--------------------------------------------\n"\
   "0 - Exit\n\
    1 - Refresh\n\n\
    --- Device Information Service ---\n\
   10 - Device Information Operations\n\n\
   --- Battery Service ---\n\
   20 - Battery Operations\n\n\
   --- Health Thermometer Service ---\n\
   30 - HTC Profile Operations\n\n\
   --- Heart Rate Service ---\n\
   40 - HRC Profile Operations\n\n\
   --- Running Speed and Cadence Service ---\n\
   50 - RSCC Profile Operations\n\n\
   --- Cycling Speed and Cadence Service ---\n\
   60 - CSCC Profile Operations\n\n\
   --- Location and Navigation Service --\n\
   70 - LNC Profile Operations\n\n\
   --- Cycling Power Service --\n\
   80 - CPC Profile Operations\n\n\
   --- Weight Scale Service --\n\
   90 - WSC Profile Operations\n\n\
   --- Environmental Sensing Sevice --\n\
  100 - ESC Profile Operations\n\n\
   --- Bond Management Service ---\n\
  110 - BMS Profile Operation\n\n\
   --- Automation IO Service ---\n\
  120 - AIOS Profile Operation\n\n\
   --- Indoor Positioning Service ---\n\
  130 - IPS Profile Operation\n\n\
   --- Proximity Monitor Operation ---\n\
  140 - PX Profile Operation\n\n\
   --- Find Me Locator Operation ---\n\
  150 - FM Profile Operation\n\n\
  --- Human Interface Device (Host) Profile ---\n\
  160 - HOG Profile Operation\n\n\
  --- Blood Pressure Profile ---\n\
  170 - Blood Pressure Operation\n\n\
  --- Phone Alert Status Profile ---\n\
  180 - Phone Alert Status Operation\n\n\
  --- Pulse Oximeter Profile ---\n\
  190 - Pulse Oximeter Operation\n\n\
  --- Cycling Power Meter Profile ---\n\
  200 - Cycling Power Meter Operation\n\n\
  --- Location and Navigation Profile ---\n\
  210 - Location and Navigation Operation\n\n\
  --- Scan Parameters Profile ---\n\
  220 - Scan Parameters Operation\n\n\
  --- Glucose Meter Profile ---\n\
  230 - Glucose Meter Operation\n\n\
  --- Time Profile ---\n\
  240 - Time Operation\n\n\
  --- Alert Notification Profile ---\n\
  260 - Alert Notification Operation\n\n\
  --- Fitness Machine Profile ---\n\
  270 - Fitness Machine Operation\n\n\
  --- Apple Media Service Profile ---\n\
  280 - Apple Media Service Operation\n\n\
  --- Apple Notification Center Profile ---\n\
  290 - ANCC Operation\n\n\
  --- Reconnection Configuration Profile ---\n\
  300 - RCC Operation\n\n\
  --- Object Transfer Profile ---\n\
  310 - OTC Operation\n\n\
  --- Fitness Machin Profile ---\n\
  320 - FTMC Operation\n\n\
\nYour Option ? \0";

static UCHAR *gatt_client_menu = (UCHAR *) \
 "\n--------------------------------------------\n"\
   "            GATT CLIENT MENU\n"\
   "--------------------------------------------\n"\
   "0 - Exit\n"\
   "1 - Refresh\n"\
   "2 - Register GATT Handle\n\n"\
   "5 - Connect ATT over BR/EDR\n\n"\
   "6 - Disconnect ATT over BR/EDR\n\n"\
   "7 - Connect ATT over LE\n"\
   "8 - Disconnect ATT over LE\n\n"\
   "Primary Service Discovery\n"\
   "10 - Discover All Primary Services \n"\
   "11 - Discover Primary Services By Service UUID\n\n"\
   "Relationship Discovery\n"\
   "20 - Find Included Services \n\n"\
   "Characteristic Discovery\n"\
   "30 - Discover All Characteristic of a Service\n"\
   "31 - Discover Characteristic by UUID\n\n"\
   "Characteristic Descriptor Discovery\n"\
   "40 - Discover All Characteristic Descriptors\n\n"\
   "Characteristic Value Read\n"\
   "50 - Read Characteristic Value\n"\
   "51 - Read Using Characteristic UUID\n"\
   "52 - Read Long Characteristic Values\n"\
   "53 - Read Multiple Characteristic Values\n\n"\
   "Characteristic Value Write\n"\
   "60.  Write Without Response\n"\
   "61.  Signed Write Without Response\n"\
   "62.  Write Characteristic Value\n"\
   "63.  Write Long Characteristic Values\n"\
   "64.  Characteristic Value Reliable Writes\n\n"\
   "Characteristic Descriptor Value Read\n"\
   "70 - Read Characteristic Descriptors\n"\
   "71 - Read Long Characteristic Descriptors\n\n"\
   "Characteristic Descriptor Value Write\n"\
   "80 - Write Characteristic Descriptors\n"\
   "81 - Write Long Characteristic Descriptors\n\n"\
   "Server Configuration\n"\
   "90 - Exchange MTU Request\n\n"\
   "100 - Profile Client Operations\n\n\
Your Option ? \0";

typedef struct
{
    UINT16 uuid;

    UCHAR  * desc_str;
}APPL_UUID_DESC;

const APPL_UUID_DESC appl_uuid_desc_table[] =
{
    {
        GATT_GAP_SERVICE,
        (UCHAR *)"GAP Service"
    },
    {
        GATT_GATT_SERVICE,
        (UCHAR *)"GATT Service"
    },
    {
        GATT_IMMEDIATE_ALERT_SERVICE,
        (UCHAR *)"Immediate Alert Service"
    },
    {
        GATT_LINK_LOSS_SERVICE,
        (UCHAR *)"Link Loss Service"
    },
    {
        GATT_TX_POWER_SERVICE,
        (UCHAR *)"Tx Power Service"
    },
    {
        GATT_CURRENT_TIME_SERVICE,
        (UCHAR *)"Current Time Service"
    },
    {
        GATT_REF_TIME_UPDATE_SERVICE,
        (UCHAR *)"Reference Time Update Service"
    },
    {
        GATT_NEXT_DST_CHANGE_SERVICE,
        (UCHAR *)"Next DST Change Service"
    },
    {
        GATT_BATTERY_SERVICE,
        (UCHAR *)"Battery Service"
    },
    {
        GATT_HEALTH_THERMOMETER_SERVICE,
        (UCHAR *)"Health Thermometer"
    },
    {
        GATT_GLUCOSE_SERVICE,
        (UCHAR *)"Glucose"
    },
    {
        GATT_DEVICE_INFO_SERVICE,
        (UCHAR *)"Device Information"
    },
    {
        GATT_NWA_SERVICE,
        (UCHAR *)"Network Availability"
    },
    {
        GATT_WATCH_DOG_SERVICE,
        (UCHAR *)"Watchdog Service"
    },
    {
        GATT_PHONE_ALERT_STATUS_SERVICE,
        (UCHAR *)"Phone Alert Status Service"
    },
    {
        GATT_HEART_RATE_SERVICE,
        (UCHAR *)"Heart Rate Service"
    },
    {
        GATT_RUNNING_SPEED_AND_CADENCE_SERVICE,
        (UCHAR *)"Running Speed and Cadence Service"
    },
    {
        GATT_CYCLING_SPEED_AND_CADENCE_SERVICE,
        (UCHAR *)"Cycling Speed and Cadence Service"
    },
    {
        GATT_PULSE_OXIMETER_SERVICE,
        (UCHAR *)"Pulse Oximeter Service"
    },
    {
        GATT_BM_SERVICE,
        (UCHAR *)"Bond Management Service"
    },
    {
        GATT_CPM_SERVICE,
        (UCHAR *)"Cycling Power Service"
    },
    {
        GATT_LOCATION_AND_NAVIGATION_SERVICE,
        (UCHAR *)"Location and Navigation Service"
    },
    {
        GATT_AIOS_SERVICE,
        (UCHAR *)"Automation IO Service"
    },
    {
        GATT_DEVICE_NAME_CHARACTERISTIC,
        (UCHAR *)"Device Name"
    },
    {
        GATT_APPEARANCE_CHARACTERISTIC,
        (UCHAR *)"Appearance"
    },
    {
        GATT_PERIPHRL_PRIVCY_FLG_CHARACTERISTIC,
        (UCHAR *)"Peripheral Privacy Flag"
    },
    {
        GATT_RECONNECT_ADDR_CHARACTERISTIC,
        (UCHAR *)"Reconnection Address"
    },
    {
        GATT_PRFRRD_CNXN_PARAM_CHARACTERISTIC,
        (UCHAR *)"Peripheral Preferred Connection Parameters"
    },
    {
        GATT_SERVICE_CHANGED_CHARACTERISTIC,
        (UCHAR *)"Service Changed"
    },
    {
        GATT_ALERT_LEVEL_CHARACTERISTIC,
        (UCHAR *)"Alert Level"
    },
    {
        GATT_TX_POWER_LEVEL_CHARACTERISTIC,
        (UCHAR *)"Tx Power Level"
    },
    {
        GATT_DATE_TIME_CHARACTERISTIC,
        (UCHAR *)"Date Time"
    },
    {
        GATT_DAY_OF_WEEK_CHARACTERISTIC,
        (UCHAR *)"Day of Week"
    },
    {
        GATT_DAY_DATE_TIME_CHARACTERISTIC,
        (UCHAR *)"Time"
    },
    {
        GATT_EXACT_TIME_100_CHARACTERISTIC,
        (UCHAR *)"Exact Time 100"
    },
    {
        GATT_EXACT_TIME_256_CHARACTERISTIC,
        (UCHAR *)"Exact Time 256"
    },
    {
        GATT_DST_OFFSET_CHARACTERISTIC,
        (UCHAR *)"Daylight Saving Time"
    },
    {
        GATT_TIME_ZONE_CHARACTERISTIC,
        (UCHAR *)"Time Zone"
    },
    {
        GATT_LOCAL_TIME_INFO_CHARACTERISTIC,
        (UCHAR *)"Local Time Information"
    },
    {
        GATT_SEC_TIME_ZONE_CHARACTERISTIC,
        (UCHAR *)"Secondary Time Zone"
    },
    {
        GATT_TIME_WITH_DST_CHARACTERISTIC,
        (UCHAR *)"Time with DST"
    },
    {
        GATT_TIME_ACCURACY_CHARACTERISTIC,
        (UCHAR *)"Time Accuracy"
    },
    {
        GATT_TIME_SOURCE_CHARACTERISTIC,
        (UCHAR *)"Time Source"
    },
    {
        GATT_REF_TIME_INFO_CHARACTERISTIC,
        (UCHAR *)"Reference Time Information"
    },
    {
        GATT_TIME_BROADCAST_CHARACTERISTIC,
        (UCHAR *)"Time Broadcast"
    },
    {
        GATT_TIME_UPDATE_CONTROL_POINT,
        (UCHAR *)"Time Update Control Point"
    },
    {
        GATT_TIME_UPDATE_STATE_CHARACTERISTIC,
        (UCHAR *)"Time Update State"
    },
    {
        GATT_GLUCOSE_MSRMNT_CHARACTERISTIC,
        /* "Boolean" */
        (UCHAR *)"Glucose Measurement"
    },
    {
        GATT_BATTERY_LEVEL_CHARACTERISTIC,
        (UCHAR *)"Battery Level"
    },
    {
        0x2A1A,
        (UCHAR *)"Battery Power State"
    },
    {
        0x2A1B,
        (UCHAR *)"Battery Level State"
    },
    {
        GATT_TEMPERATURE_MSMNT_CHARACTERISTIC,
        (UCHAR *)"Temperature Measurement"
    },
    {
        GATT_TEMPERATURE_TYPE_CHARACTERISTIC,
        (UCHAR *)"Temperature Type"
    },
    {
        GATT_INTERMEDIATE_TEMP_CHARACTERISTIC,
        (UCHAR *)"Intermediate Temperature"
    },
    {
        0x2A1F,
        (UCHAR *)"Temperature Celsius"
    },
    {
        0x2A20,
        (UCHAR *)"Temperature Fahrenheit"
    },
    {
        GATT_MSMNT_INTERVAL_CHARATACTERISTIC,
        (UCHAR *)"Measurement Interval"
    },
    {
        GATT_BOOT_KYBRD_IN_RPT_CHARACTERISTIC,
        (UCHAR *)"HID Boot Keyboard Input Report"
    },
    {
        GATT_SYSTEM_ID_CHARACTERISTIC,
        (UCHAR *)"System ID"
    },
    {
        GATT_MODEL_NUMBER_CHARACTERISTIC,
        (UCHAR *)"Model Number String"
    },
    {
        GATT_SERIAL_NUMBER_CHARACTERISTIC,
        (UCHAR *)"Serial Number String"
    },
    {
        GATT_FIRMWARE_REV_CHARACTERISTIC,
        (UCHAR *)"Firmware Revision String"
    },
    {
        GATT_HARDWARE_REV_CHARACTERISTIC,
        (UCHAR *)"Hardware Revision String"
    },
    {
        GATT_SOFTWARE_REV_CHARACTERISTIC,
        (UCHAR *)"Software Revision String"
    },
    {
        GATT_MANUFACTURER_NAME_CHARACTERISTIC,
        (UCHAR *)"Manufacturer Name String"
    },
    {
        GATT_REG_CERT_DATA_CHARACTERISTIC,
        (UCHAR *)"IEEE 11073-20601 Regulatory Certification Data List"
    },
    {
        GATT_CURRENT_TIME_CHARACTERISTIC,
        (UCHAR *)"Current Time"
    },
    {
        GATT_POX_SPOT_CHK_MSRMT_CHARACTERISTIC,
        (UCHAR *)"POX Spot Check Measurement"
    },
    {
        GATT_POX_CONTINUOUS_MSRMT_CHARACTERISTIC,
        (UCHAR *)" POX Continuous Meaurement"
    },
    {
        GATT_POX_FEATURES_CHARACTERISTIC,
        (UCHAR *)"POX Features"
    },
    {
        GATT_CPM_SENSOR_LOCATION_CHARACTERISTIC ,
        (UCHAR *)"CPM Sensor Location"
    },
    {
        GATT_CPM_MSRMT_CHARACTERISTIC,
        (UCHAR *)"CPM Measurement"
    },
    {
        GATT_CPM_VECTOR_CHARACTERISTIC,
        (UCHAR *)"CPM Vector"
    },
    {
        GATT_CPM_FEATURE_CHARACTERISTIC,
        (UCHAR *)"CPM Feature"
    },
    {
        GATT_CPM_CNTRL_PNT_CHARACTERISTIC,
        (UCHAR *)"CPM Control Point"
    },
    {
        0x2A2C,
        (UCHAR *)"Elevation"
    },
    {
        0x2A2D,
        (UCHAR *)"Latitude"
    },
    {
        0x2A2E,
        (UCHAR *)"Longitude"
    },
    {
        0x2A2F,
        (UCHAR *)"Position 2D"
    },
    {
        0x2A30,
        (UCHAR *)"Position 3D"
    },
    {
        GATT_SCAN_REFRESH_CHARACTERISTIC,
        (UCHAR *)"Scan Refresh"
    },
    {
        GATT_BOOT_KYBRD_OP_CHARACTERISTIC,
        /* "Product ID" */
        (UCHAR *)"Boot Keyboard Output Report"
    },
    {
        GATT_MOUSE_IP_CHARACTERISTIC,
        /* "HID Version" */
        (UCHAR *)"Boot Mouse Input Report"
    },
    {
        GATT_GL_MSRMT_CTX_CHARACTERISTIC,
        /* "Vendor ID Source" */
        (UCHAR *)"Glucose Measurement Context"
    },
    {
        GATT_BP_MSRMT_CHARACTERISTIC,
        (UCHAR *)"Blood Pressure Measurement"
    },
    {
        GATT_INTRMDT_CUFF_PRSR_CHARACTERISTIC,
        (UCHAR *)"Intermediate Cuff Pressure"
    },
    {
        GATT_HR_MSRMT_CHARACTERISTIC,
        (UCHAR *)"Heart Rate Measurement"
    },
    {
        GATT_BODY_SENSOR_LOC_CHARACTERISTIC,
        (UCHAR *)"Heart Rate Sensor Location"
    },
    {
        GATT_HR_CNTRL_POINT,
        (UCHAR *)"Heart Rate Control Point"
    },
    {
        GATT_BLOOD_PRESSURE_SERVICE,
        (UCHAR *)"Blood Pressure Monitor"
    },
    {
        GATT_ALERT_NOTIFICATION_SERVICE,
        (UCHAR *)"Alert Notification"
    },
    {
        GATT_BP_MSRMT_CHARACTERISTIC,
        (UCHAR *)"Blood Pressure Measurement"
    },
    {
        GATT_INTRMDT_CUFF_PRSR_CHARACTERISTIC,
        (UCHAR *)"Intermediate Cuff Measurement"
    },
    {
        0x2A3A,
        (UCHAR *)"Removable"
    },
    {
        0x2A3B,
        (UCHAR *)"Service Required"
    },
    {
        GATT_NWA_CHARACTERISTIC,
        (UCHAR *)"Network Availability"
    },
    {
        GATT_ALERT_STATUS_CHARACTERISTIC,
        (UCHAR *)"Alert Status"
    },
    {
        GATT_RINGER_CONTROL_POINT_CHARACTERISTIC,
        (UCHAR *)"Ringer Control Point"
    },
    {
        GATT_RINGER_SETTING_CHARACTERISTIC,
        (UCHAR *)"Ringer Setting"
    },
    {
        GATT_BP_FEATURE_CHARACTERISTIC,
        (UCHAR *)"Blood Pressure Feature"
    },
    {
        GATT_HID_SERVICE,
        (UCHAR *)"Human Interface Device Service"
    },
    {
        GATT_ALERT_NTF_CONTROL_POINT,
        (UCHAR *)"Alert Notification Control Point"
    },
    {
        GATT_UNREAD_ALRT_STATUS_CHARACTERISTIC,
        (UCHAR *)"Unread Alert Status"
    },
    {
        GATT_NEW_ALERT_CHARACTERISTIC,
        (UCHAR *)"New Alert"
    },
    {
        GATT_SPRTD_NEW_ALRT_CTGRY_CHARACTERISTIC,
        (UCHAR *)"Supported New Alert Category"
    },
    {
        GATT_SPRT_UNRD_ALRT_CTGRY_CHARACTERISTIC,
        (UCHAR *)"Supported Unread Alert Category"
    },
    {
        GATT_HID_INFO_CHARACTERISTIC,
        (UCHAR *)"HID Information"
    },
    {
        GATT_HID_RPT_MAP_CHARACTERISTIC,
        (UCHAR *)"HID Report Mapping"
    },
    {
        GATT_HID_CP_CHARACTERISTIC,
        (UCHAR *)"HID Control Point"
    },
    {
        GATT_HID_RPT_CHARACTERISTIC,
        (UCHAR *)"HID Report"
    },
    {
        GATT_HID_PROTO_MODE_CHARACTERISTIC,
        (UCHAR *)"HID Protocol Mode"
    },
    {
        GATT_SCAN_PARAM_SERVICE,
        (UCHAR *)"Scan Parameter Service"
    },
    {
        GATT_SCAN_WINDOW_CHARACTERISTIC,
        (UCHAR *)"Scan Interval Window"
    },
    {
        GATT_PNP_ID_CHARACTERISTIC,
        (UCHAR *)"PnP ID"
    },
    {
        GATT_GL_FEATURE_CHARACTERISTIC,
        (UCHAR *)"Glucose Feature"
    },
    {
        GATT_RACP_CHARACTERISTIC,
        (UCHAR *)"RACP"
    },
    {
        GATT_RSC_MSRMT_CHARACTERISTIC,
        (UCHAR *)"RSC Measurment"
    },
    {
        GATT_RSC_FEATURE_CHARACTERISTIC,
        (UCHAR *)"RSC Feature"
    },
    {
        GATT_CSC_MSRMT_CHARACTERISTIC,
        (UCHAR *)"CSC Measurment"
    },
    {
        GATT_CSC_FEATURE_CHARACTERISTIC,
        (UCHAR *)"CSC Feature"
    },
    {
        GATT_SENSOR_LOCATION_CHARACTERISTIC,
        (UCHAR *)"Sensor Location"
    },
    {
        GATT_SC_CONTROL_POINT_CHARACTERISTIC,
        (UCHAR *)"SC Control Point"
    },
    {
        GATT_BM_FEATURES_CHARACTERISTIC ,
        (UCHAR *)"BM Features"
    },
    {
        GATT_BM_CNTRL_PNT_CHARACTERISTIC,
        (UCHAR *)"BM Control Point"
    },
    {
        GATT_LOCATION_AND_SPEED_CHARACTERISTIC,
        (UCHAR *)"LN Location and Speed "
    },
    {
        GATT_NAVIGATION_CHARACTERISTIC,
        (UCHAR *)"LN Navigaton "
    },
    {
        GATT_LN_POSITION_QUALITY_CHARACTERISTIC,
        (UCHAR *)"LN Position Quality"
    },
    {
        GATT_LN_FEATURE_CHARACTERISTIC,
        (UCHAR *)"LN Feature"
    },
    {
        GATT_LN_CNTRL_PNT_CHARACTERISTIC,
        (UCHAR *)"LN Control Point"
    },
    {
        GATT_AIO_DIGITAL_CHARACTERISTIC ,
        (UCHAR *)"AIO Digital"
    },
    {
        GATT_AIO_ANALOG_CHARACTERISTIC,
        (UCHAR *)"AIO Analog"
    },
    {
        GATT_AIO_AGGREGATE_CHARACTERISTIC ,
        (UCHAR *)"AIO Aggregate"
    },
    {
        0x0000,
        NULL
    }
};

void appl_profile_operations (void);

/* --------------------------------------------- Functions */

API_RESULT appl_gatt_cb
           (
               ATT_HANDLE    * handle,
               UCHAR         att_event,
               API_RESULT    event_result,
               void          * eventdata,
               UINT16        event_datalen
           )
{
    int            i, j;
    API_RESULT     retval;
    UINT16         attr_handle;
    UINT16         offset;
    UINT16         mtu;
    UCHAR          op_code;
    UCHAR          rsp_code;
    ATT_VALUE      uuid128;
#ifdef ATT_SUPPORT_128_BIT_UUID
    ATT_VALUE      uuid16;
    ATT_UUID16     uuid_16;
#endif /* ATT_SUPPORT_128_BIT_UUID */

    ATT_ATTR_HANDLE csh, ceh;

    GATT_SERVICE_PARAM * service;
    GATT_INC_SERVICE_PARAM * inc_service;
    GATT_CHARACTERISTIC_PARAM * characteristic;
    GATT_CHAR_DESC_PARAM * descriptor;

    UCHAR * event_data;
    UCHAR att_scb;

    event_data = (UCHAR *) eventdata;

    IotLogInfo
    ("[ATT]:[0x%02X: 0x%02X]: Received ATT Event 0x%02X with result 0x%04X\n",
    handle->device_id, handle->att_id, att_event, event_result);

    if (NULL != event_data)
    {
        appl_dump_bytes(event_data, event_datalen);
    }

    /* Set no server application callback */
    att_scb = 0x00;
    uuid_16 = 0x0000;
    (void)uuid_16;    /*fix build warning: set but never used.*/

    /* Initialize the Characterist Start and End Handles */
    csh = ceh = 0x0000;

    switch(att_event)
    {
    case ATT_CONNECTION_IND:
        IotLogInfo (
        "[0x%02X: 0x%02X]:Received Connection Indication, Result 0x%04X!\n",
        handle->device_id, handle->att_id, event_result);

        appl_gatt_client_handle = *handle;

        att_scb = 0x01;

        appl_notify_gatt_conn();

#ifdef APPL_AUTO_DISCOVERY_ON_CONNECTION
        auto_discovery = 0x01;
        gatt_discover_ps (handle, 0x0000);
#endif /* APPL_AUTO_DISCOVERY_ON_CONNECTION */
        break;

    case ATT_DISCONNECTION_IND:
        IotLogInfo (
        "[0x%02X: 0x%02X]:Received Disconnection Indication, Result 0x%04X!\n",
        handle->device_id, handle->att_id, event_result);

        appl_notify_gatt_disconn();

        att_scb = 0x01;

        break;

    case ATT_ERROR_RSP:
        if (event_data != NULL)
        {
            BT_UNPACK_LE_1_BYTE(&op_code,event_data);
            BT_UNPACK_LE_2_BYTE(&attr_handle, event_data+1);
            BT_UNPACK_LE_1_BYTE(&rsp_code,event_data+3);
        }
        IotLogInfo (
        "Received Error Response, for Op-Code 0x%02X for Handle 0x%04X, Rsp "
        "Code 0x%02X!\n", op_code, attr_handle,rsp_code);
        (void)op_code;    /*fix build warning: set but never used.*/
        (void)rsp_code;   /*fix build warning: set but never used.*/
        break;

    case ATT_XCHNG_MTU_REQ:
        if (event_data != NULL)
        {
            BT_UNPACK_LE_2_BYTE(&mtu, event_data);
        }
        IotLogInfo (
        "Received Exchange MTU Request with Result 0x%04X. MTU Size "
        "= 0x%04X!\n", event_result, mtu);

        att_scb = 0x01;

        break;

    case ATT_XCHNG_MTU_RSP:
        if (NULL == event_data)
        {
            break;
        }
        BT_UNPACK_LE_2_BYTE(&mtu, event_data);
        IotLogInfo (
        "Received Exchange MTU Response with Result 0x%04X. MTU Size "
        "= 0x%04X!\n",event_result,mtu);
        break;

    case ATT_FIND_INFO_RSP:
        IotLogInfo ("Received Find Information Response Opcode!\n");

        if (NULL == event_data)
        {
            break;
        }
        break;
    case ATT_READ_BY_TYPE_RSP:
        IotLogInfo ("Received Read Type Response Opcode 0x%04X!\n",
                event_result);
        if (NULL == event_data)
        {
            break;
        }
        appl_dump_bytes(event_data, event_datalen);
        break;
    case ATT_READ_BY_GROUP_RSP:
        IotLogInfo ("Received Read Group Type Response Opcode 0x%04X!\n",
                event_result);
        if (NULL == event_data)
        {
            break;
        }
        break;
    case ATT_FIND_BY_TYPE_VAL_RSP:
        IotLogInfo ("Received Find by Type Value Response Opcode!\n");
        if (NULL == event_data)
        {
            break;
        }

        IotLogInfo ("Found Handle        End Found Handle\n");
        for (i = 0; i < event_datalen; i+=4)
        {
            BT_UNPACK_LE_2_BYTE(&attr_handle, (event_data+i));
            IotLogInfo ("%04X",attr_handle);
            BT_UNPACK_LE_2_BYTE(&attr_handle, (event_data+i+2));
            IotLogInfo ("                  %04X\n", attr_handle);
        }
        break;

    case ATT_READ_RSP:
        IotLogInfo ("Received Read Response Opcode!\n");
        IotLogInfo ("Handle Value Received - \n");
        if (event_data != NULL)
        {
            appl_dump_bytes(event_data, event_datalen);
        }
        break;

    case ATT_READ_BLOB_RSP:
        IotLogInfo ("Received Read Response Opcode!\n");
        IotLogInfo ("Handle Value Received - \n");
        if (event_data != NULL)
        {
            appl_dump_bytes(event_data, event_datalen);
        }
        break;

    case ATT_READ_MULTIPLE_RSP:
        IotLogInfo ("Received read multiple response Opcode!\n");
        IotLogInfo ("Byte stream of values received [not seperated by handle]");
        if (event_data != NULL)
        {
            appl_dump_bytes(event_data, event_datalen);
        }
        break;

    case ATT_WRITE_RSP:
        IotLogInfo ("Received Write Response Opcode!\n");
        break;

    case ATT_WRITE_CMD_TX_COMPLETE:
        IotLogInfo("Received Write Command Tx Complete (Locally generated)\n");
        if (event_data != NULL)
        {
            appl_dump_bytes(event_data, event_datalen);
        }
        break;

    case ATT_SIGNED_WRITE_CMD_TX_COMPLETE:
        IotLogInfo("Received Signed Write Command Tx Complete (Locally generated)\n");
        if (event_data != NULL)
        {
            appl_dump_bytes(event_data, event_datalen);
        }
        break;

    case ATT_PREPARE_WRITE_RSP:
        IotLogInfo ("Prepare Write Response, Event Result 0x%04X\n",event_result);
        if (NULL != event_data)
        {
            BT_UNPACK_LE_2_BYTE(&attr_handle, event_data);
            IotLogInfo ("Handle - 0x%04X\n", attr_handle);
            BT_UNPACK_LE_2_BYTE(&offset, event_data+2);
            IotLogInfo ("Offset - 0x%04X\n", offset);
            IotLogInfo ("Handle Value Received - \n");
            appl_dump_bytes(event_data + 4, event_datalen - 4);
        }
        IotLogInfo ("\n");
        break;

    case ATT_EXECUTE_WRITE_RSP:
        IotLogInfo ("Received Execute Write Response\n");
        break;

    case ATT_HANDLE_VALUE_NTF:
        if (event_data != NULL)
        {
            IotLogInfo ("Received HVN\n");
            BT_UNPACK_LE_2_BYTE(&attr_handle, event_data);
            IotLogInfo ("Handle - 0x%04X\n", attr_handle);
            IotLogInfo ("Handle Value Received - \n");
#ifndef AMC
            appl_dump_bytes(event_data + 2, (event_datalen - 2));
#endif /* AMC */

            /* Notify upper layers about the Notifications received */
            appl_parse_notification_data(attr_handle, (event_data + 2), (event_datalen - 2));
        }
        break;

    case ATT_HANDLE_VALUE_IND:
        if (event_data != NULL)
        {
            IotLogInfo ("Received HVI\n");
            BT_UNPACK_LE_2_BYTE(&attr_handle, event_data);
            IotLogInfo ("Handle - 0x%04X\n", attr_handle);
            IotLogInfo ("Handle Value Received - \n");
            appl_dump_bytes(event_data + 2, (event_datalen-2));
            retval = BT_att_send_hndl_val_cnf (handle);
            appl_parse_indication_data(attr_handle, (event_data + 2), (event_datalen - 2));
        }
        break;

    case GATT_PS_DISCOVERY_RSP:
        IotLogInfo ("Received GATT_PS_DISCOVERY_RSP\n");
        IotLogInfo ("No. Primary Services - %d\n\n", event_datalen);

        service = (GATT_SERVICE_PARAM *) eventdata;

        for (i = 0; i < event_datalen; i++)
        {
#ifdef ATT_SUPPORT_128_BIT_UUID
            if (ATT_16_BIT_UUID_FORMAT == service->uuid_type)
            {
                IotLogInfo (
                "UUID: 0x%04X (%s)\nStart Hdl: 0x%04X, End Hdl: 0x%04X\n\n",
                service->uuid.uuid_16, appl_display_uuid_label(service->uuid.uuid_16),
                service->range.start_handle, service->range.end_handle);
            }
            else
            {
                uuid128.val = service->uuid.uuid_128.value;
                uuid128.len = ATT_128_BIT_UUID_SIZE;
                retval = BT_gatt_db_get_16_bit_uuid(&uuid128,&uuid16);
                if (API_SUCCESS == retval)
                {
                    BT_UNPACK_LE_2_BYTE(&uuid_16, uuid16.val);
                }

                IotLogInfo ("UUID: ");
                appl_print_128_bit_uuid(&service->uuid.uuid_128);
                IotLogInfo (
                " (%s)\nStart Hdl: 0x%04X, End Hdl: 0x%04X\n\n",
                appl_display_uuid_label(uuid_16),
                service->range.start_handle, service->range.end_handle);
            }
#else /* ATT_SUPPORT_128_BIT_UUID */
            IotLogInfo ("UUID: 0x%04X (%s)\nStart Hdl: 0x%04X, End Hdl: 0x%04X\n\n",
                service->uuid, appl_display_uuid_label(service->uuid),
                service->range.start_handle, service->range.end_handle);
#endif /* ATT_SUPPORT_128_BIT_UUID */

            if (0 == i)
            {
                /* Copy the start handle */
                csh = service->range.start_handle;

                /* Copy End Handle from the first Iteration */
                ceh = service->range.end_handle;
            }
            else if ((event_datalen - 1) == i)
            {
                /* Copy the end handle */
                ceh = service->range.end_handle;
            }

            /* Call Primary Service Discovery Completion Handler */
            appl_notify_gatt_servdata(service, sizeof(GATT_SERVICE_PARAM));

            /* Increment Service Pointer */
            service ++;
        }

        if (0 == general_service_discovery)
        {
            gatt_discover_char
            (
                &appl_gatt_client_handle,
                csh,
                ceh,
                0x0000,
                1
            );
        }

#ifdef APPL_AUTO_DISCOVERY_ON_CONNECTION
        if (0x01 == auto_discovery)
        {
            gatt_discover_ss (handle, 0x0000);
        }
#endif /* APPL_AUTO_DISCOVERY_ON_CONNECTION */
        break;

    case GATT_SS_DISCOVERY_RSP:
        IotLogInfo ("Received GATT_SS_DISCOVERY_RSP\n");
        IotLogInfo ("No. Secondary Services - %d\n\n", event_datalen);

        service = (GATT_SERVICE_PARAM *) eventdata;

        for (i = 0; i < event_datalen; i++)
        {
#ifdef ATT_SUPPORT_128_BIT_UUID
            if (ATT_16_BIT_UUID_FORMAT == service->uuid_type)
            {
                IotLogInfo (
                "UUID: 0x%04X (%s)\nStart Hdl: 0x%04X, End Hdl: 0x%04X\n\n",
                service->uuid.uuid_16, appl_display_uuid_label(service->uuid.uuid_16),
                service->range.start_handle, service->range.end_handle);
            }
            else
            {
                uuid128.val = service->uuid.uuid_128.value;
                uuid128.len = ATT_128_BIT_UUID_SIZE;
                retval = BT_gatt_db_get_16_bit_uuid(&uuid128,&uuid16);
                if (API_SUCCESS == retval)
                {
                    BT_UNPACK_LE_2_BYTE(&uuid_16, uuid16.val);
                }

                IotLogInfo ("UUID: ");
                appl_print_128_bit_uuid(&service->uuid.uuid_128);
                IotLogInfo (
                " (%s)\nStart Hdl: 0x%04X, End Hdl: 0x%04X\n\n",
                appl_display_uuid_label(uuid_16),
                service->range.start_handle, service->range.end_handle);
            }
#else /* ATT_SUPPORT_128_BIT_UUID */
            IotLogInfo ("UUID: 0x%04X (%s)\nStart Hdl: 0x%04X, End Hdl: 0x%04X\n\n",
                service->uuid, appl_display_uuid_label(service->uuid),
                service->range.start_handle, service->range.end_handle);
#endif /* ATT_SUPPORT_128_BIT_UUID */

            service ++;
        }

#ifdef APPL_AUTO_DISCOVERY_ON_CONNECTION
        if (0x01 == auto_discovery)
        {
            gatt_discover_is (handle, 0x0001, 0xFFFF);
        }
#endif /* APPL_AUTO_DISCOVERY_ON_CONNECTION */
        break;

    case GATT_IS_DISCOVERY_RSP:
        IotLogInfo ("Received GATT_IS_DISCOVERY_RSP\n");
        IotLogInfo ("No. Included Services - %d\n\n", event_datalen);

        inc_service = (GATT_INC_SERVICE_PARAM *) eventdata;

        for (i = 0; i < event_datalen; i++)
        {
            service = (GATT_SERVICE_PARAM *) &(inc_service->service);

#ifdef ATT_SUPPORT_128_BIT_UUID
            if (ATT_16_BIT_UUID_FORMAT == service->uuid_type)
            {
                IotLogInfo (
                "(%s)\nHandle: 0x%04X, UUID: 0x%04X\nStart Hdl: 0x%04X, End Hdl: 0x%04X\n\n",
                appl_display_uuid_label(service->uuid.uuid_16),
                inc_service->handle, service->uuid.uuid_16,
                service->range.start_handle, service->range.end_handle);
            }
            else
            {
                uuid128.val = service->uuid.uuid_128.value;
                uuid128.len = ATT_128_BIT_UUID_SIZE;
                retval = BT_gatt_db_get_16_bit_uuid(&uuid128,&uuid16);
                if (API_SUCCESS == retval)
                {
                    BT_UNPACK_LE_2_BYTE(&uuid_16, uuid16.val);
                }

                IotLogInfo ("(%s)\nHandle: 0x%04X,",
                    appl_display_uuid_label(uuid_16), inc_service->handle);
                IotLogInfo ("UUID: ");
                appl_print_128_bit_uuid(&service->uuid.uuid_128);
                IotLogInfo (
                "\nStart Hdl: 0x%04X, End Hdl: 0x%04X\n\n",
                service->range.start_handle, service->range.end_handle);
            }
#else /* ATT_SUPPORT_128_BIT_UUID */
            IotLogInfo ("(%s)\nHandle: 0x%04X, UUID: 0x%04X\nStart Hdl: 0x%04X, End Hdl: 0x%04X\n\n",
                appl_display_uuid_label(service->uuid),
                inc_service->handle, service->uuid, service->range.start_handle,
                service->range.end_handle);
#endif /* ATT_SUPPORT_128_BIT_UUID */

            if (0 == general_service_discovery)
            {
                gatt_discover_char
                (
                    &appl_gatt_client_handle,
                    service->range.start_handle,
                    service->range.end_handle,
                    0x0000,
                    1
                );
            }
            inc_service ++;
        }

#ifdef APPL_AUTO_DISCOVERY_ON_CONNECTION
        if (0x01 == auto_discovery)
        {
            gatt_discover_char (handle, 0x0001, 0xFFFF, 0x0000, 1);
        }
#endif /* APPL_AUTO_DISCOVERY_ON_CONNECTION */
        break;

    case GATT_CHAR_DISCOVERY_RSP:
        IotLogInfo ("Received GATT_CHAR_DISCOVERY_RSP\n");
        IotLogInfo ("No. Characteristics - %d\n\n", event_datalen);

        characteristic = (GATT_CHARACTERISTIC_PARAM *) eventdata;

        for (i = 0; i < event_datalen; i++)
        {
#ifdef ATT_SUPPORT_128_BIT_UUID
            if (ATT_16_BIT_UUID_FORMAT == characteristic->uuid_type)
            {
                IotLogInfo (
                "(%s)\nChar Handle: 0x%04X, UUID: 0x%04X\n"
                "Property: 0x%02X, Value Handle: 0x%04X\n",
                appl_display_uuid_label(characteristic->uuid.uuid_16),
                characteristic->range.start_handle, characteristic->uuid.uuid_16,
                characteristic->cproperty, characteristic->value_handle);
            }
            else
            {
                uuid128.val = characteristic->uuid.uuid_128.value;
                uuid128.len = ATT_128_BIT_UUID_SIZE;
                retval = BT_gatt_db_get_16_bit_uuid(&uuid128,&uuid16);
                if (API_SUCCESS == retval)
                {
                    BT_UNPACK_LE_2_BYTE(&uuid_16, uuid16.val);
                }

                IotLogInfo ("(%s)\nChar Handle: 0x%04X,",
                    appl_display_uuid_label(uuid_16),
                    characteristic->range.start_handle);
                IotLogInfo ("UUID: ");
                appl_print_128_bit_uuid(&characteristic->uuid.uuid_128);
                IotLogInfo (
                "\nProperty: 0x%02X, Value Handle: 0x%04X\n",
                characteristic->cproperty, characteristic->value_handle);
            }
#else
            IotLogInfo ("(%s)\nChar Handle: 0x%04X, UUID: 0x%04X\n"
                      "Property: 0x%02X, Value Handle: 0x%04X\n",
                appl_display_uuid_label(characteristic->uuid),
                characteristic->range.start_handle, characteristic->uuid,
                characteristic->cproperty, characteristic->value_handle);
#endif /* ATT_SUPPORT_128_BIT_UUID */

            if (0 != characteristic->desc_index)
            {
                IotLogInfo ("No. Characteristic Descriptors: %d\n",
                    characteristic->desc_index);

                for (j = 0; j < characteristic->desc_index; j++)
                {
#ifdef ATT_SUPPORT_128_BIT_UUID
                    if (ATT_16_BIT_UUID_FORMAT == characteristic->descriptor[j].uuid_type)
                    {
                        IotLogInfo (
                        "Desc Handle: 0x%04X, Desc UUID: 0x%04X (%s)\n",
                        characteristic->descriptor[j].handle,
                        characteristic->descriptor[j].uuid.uuid_16,
                        appl_display_uuid_label(characteristic->descriptor[j].uuid.uuid_16));
                    }
                    else
                    {
                        uuid128.val = characteristic->uuid.uuid_128.value;
                        uuid128.len = ATT_128_BIT_UUID_SIZE;
                        retval = BT_gatt_db_get_16_bit_uuid(&uuid128,&uuid16);
                        if (API_SUCCESS == retval)
                        {
                            BT_UNPACK_LE_2_BYTE(&uuid_16, uuid16.val);
                        }

                        IotLogInfo (
                        "Desc Handle: 0x%04X, ",
                        characteristic->descriptor[j].handle);
                        IotLogInfo ("Desc UUID: ");
                        appl_print_128_bit_uuid(&characteristic->descriptor[j].uuid.uuid_128);
                        IotLogInfo (" (%s)\n",appl_display_uuid_label(uuid_16));
                    }
#else /* ATT_SUPPORT_128_BIT_UUID */
                    IotLogInfo ("Desc Handle: 0x%04X, Desc UUID: 0x%04X (%s)\n",
                              characteristic->descriptor[j].handle,
                              characteristic->descriptor[j].uuid,
                              appl_display_uuid_label(characteristic->descriptor[j].uuid));
#endif /* ATT_SUPPORT_128_BIT_UUID */
                }
            }
            IotLogInfo ("\n");
            characteristic ++;
        }

        appl_notify_gatt_chardata ((GATT_CHARACTERISTIC_PARAM *) eventdata, event_datalen);

#ifdef APPL_AUTO_DISCOVERY_ON_CONNECTION
        if (0x01 == auto_discovery)
        {
            auto_discovery = 0x00;
        }
#endif /* APPL_AUTO_DISCOVERY_ON_CONNECTION */
        break;

    case GATT_CHAR_DESC_DISCOVERY_RSP:
        IotLogInfo ("Received GATT_CHAR_DESC_DISCOVERY_RSP\n");
        IotLogInfo ("No. Descriptors - %d\n\n", event_datalen);

        descriptor = (GATT_CHAR_DESC_PARAM *) eventdata;

        for (i = 0; i < event_datalen; i++)
        {
#ifdef ATT_SUPPORT_128_BIT_UUID
            if (ATT_16_BIT_UUID_FORMAT == descriptor->uuid_type)
            {
                IotLogInfo ("Desc Handle: 0x%04X, Desc UUID: 0x%04X\n",
                      descriptor->handle,
                      descriptor->uuid.uuid_16);
            }
            else
            {
                IotLogInfo (
                        "Desc Handle: 0x%04X, ",
                        descriptor->handle);
                        IotLogInfo ("Desc UUID: ");
                        appl_print_128_bit_uuid(&descriptor->uuid.uuid_128);
                        IotLogInfo ("\n");
            }
#else /* ATT_SUPPORT_128_BIT_UUID */
            IotLogInfo ("Desc Handle: 0x%04X, Desc UUID: 0x%04X\n",
                      descriptor->handle,
                      descriptor->uuid);
#endif /* ATT_SUPPORT_128_BIT_UUID */


            descriptor ++;
        }
        break;

    default:
        att_scb = 0x01;
        break;
    }

    if ((0x01 == att_scb) && (event_data != NULL))
    {
        appl_att_cb
        (
            handle,
            att_event,
            event_result,
            event_data,
            event_datalen
        );
    }

    return API_SUCCESS;
}


void main_gatt_client_operations(void)
{
    int                               data;
    int                               index;
    int                               indx;
    int                               choice;
    UCHAR                             value[20];
    ATT_UUID                          uuid;
    API_RESULT                        retval;
    ATT_HANDLE_RANGE                  range;
    ATT_WRITE_REQ_PARAM               write_req_param;
    ATT_READ_BLOB_REQ_PARAM           read_blob_req_param;
    ATT_READ_MULTIPLE_REQ_PARAM       read_multiple_param;
    ATT_PREPARE_WRITE_REQ_PARAM       prepare_write_param;
    GATT_PREPARE_WRITE_REQ_PARAM      appl_req_param;
    UINT16 handle;

    general_service_discovery = 0x01;
    memset(&appl_req_param, 0, sizeof(appl_req_param));

    BT_LOOP_FOREVER()
    {
        IotLogInfo ("%s \n", gatt_client_menu);
        (void)gatt_client_menu;    /*fix build warning: set but never used.*/
        IotLogInfo ("Enter you choice : ");
        CONSOLE_IN ("%d", &choice);

        switch(choice)
        {
        case 0:
            general_service_discovery = 0x00;
            return;

        case 1:
            break;

        case 2:
            IotLogInfo ("Enter Device ID: ");
            CONSOLE_IN ("%x", &choice);
            appl_gatt_client_handle.device_id = choice;
            IotLogInfo ("Enter ATT ID: ");
            CONSOLE_IN ("%x", &choice);
            appl_gatt_client_handle.att_id = choice;
            break;

        case 5:
#ifdef ATT_ON_BR_EDR_SUPPORT
            {
                BT_DEVICE_ADDR    device_addr;
                ATT_HANDLE        att_handle;

                ATT_INIT_HANDLE (att_handle);

                IotLogInfo ("Enter Peer BD Address: ");
                appl_get_bd_addr(device_addr.addr);
                BT_BD_ADDR_TYPE (&device_addr) = 0x00;

                retval = device_queue_search_br_edr_remote_addr
                         (
                             &att_handle.device_id,
                             &device_addr
                         );
                IotLogInfo("Device Queue search result 0x%04X\n",retval);

                if (API_SUCCESS == retval)
                {
                    retval = BT_att_connect_req
                             (
                                  &att_handle
                             );
                    IotLogInfo(
                    "Initiated ATT Connect Request with Result 0x%04X\n",retval);
                }
            }
#endif /* ATT_ON_BR_EDR_SUPPORT */
            break;

        case 6:
#ifdef ATT_ON_BR_EDR_SUPPORT
            {
                retval = BT_att_disconnect_req
                         (
                              &appl_gatt_client_handle
                         );
                IotLogInfo(
                "Initiated ATT Disconnect Request with Result 0x%04X\n",retval);
            }
#endif /* ATT_ON_BR_EDR_SUPPORT */
            break;

        case 7:
            IotLogDebug ("Initiating LE ATT transport connection (HCI LE)...\n");
            IotLogDebug("Enter peer_address_type : \n");
            scanf("%d",&choice);
            gatt_peer_address.type = (UCHAR) choice;
            IotLogDebug("Enter peer_address : \n");
            appl_get_bd_addr(gatt_peer_address.addr);

            retval = BT_hci_le_create_connection
                     (
                         0x0040,
                         0x0040,
                         0x00,
                         gatt_peer_address.type,
                         gatt_peer_address.addr,
                         0x00,
                         0x0040,
                         0x0056,
                         0x00,
                         0x03BB,
                         0x0020,
                         0x0020
                     );

            if(API_SUCCESS != retval)
            {
                IotLogDebug("FAILED !!! Error code = %04X\n", retval);
            }
            else
            {
                IotLogDebug("API returned success...\n");
            }

            break;

        case 8:
            IotLogDebug ("Initiating LE ATT transport disconnection (HCI LE)...\n");
            retval = BT_hci_get_le_connection_handle (&gatt_peer_address, &handle);
            if (API_SUCCESS == retval)
            {
                BT_hci_disconnect (handle, 0x13);
            }
            break;

        case 10: /* Discover All Services */
            uuid.uuid_16 = 0x0000;
            retval = gatt_discover_ps
                     (
                         &appl_gatt_client_handle,
                         uuid,
                         ATT_16_BIT_UUID_FORMAT
                     );
            break;
        case 11: /* Discover Services By Service UUID */
            IotLogInfo ("Enter UUID Format 1 - 16Bit and 2 - 128Bit: ");
            CONSOLE_IN ("%x", &data);
            IotLogInfo ("Enter the Service UUID[HEX] : ");
            if (ATT_16_BIT_UUID_FORMAT == data)
            {
#ifdef ATT_SUPPORT_128_BIT_UUID
                CONSOLE_IN ("%x", &choice);
                uuid.uuid_16 = (ATT_UUID16)choice;
#else /* ATT_SUPPORT_128_BIT_UUID */
                CONSOLE_IN ("%x", &choice);
                uuid = (ATT_UUID16)choice;
#endif /* ATT_SUPPORT_128_BIT_UUID */
            }
            else
            {
#ifdef ATT_SUPPORT_128_BIT_UUID
                for (index = 0; index < ATT_128_BIT_UUID_SIZE; index++)
                {
                    CONSOLE_IN ("%x", &indx);
                    uuid.uuid_128.value[index] = (UCHAR) indx;
                }
#else /* ATT_SUPPORT_128_BIT_UUID */
                IotLogInfo ("Enable ATT_SUPPORT_128_BIT_UUID support\n");
#endif /* ATT_SUPPORT_128_BIT_UUID */
            }

            retval = gatt_discover_ps (&appl_gatt_client_handle, uuid, (UCHAR) data);
            break;
        case 20: /* Find Included Services */
            IotLogInfo ("Enter the Start Handle[HEX] : ");
            CONSOLE_IN ("%x", &data);
            range.start_handle = (UINT16) data;

            IotLogInfo ("Enter the End Handle[HEX] : ");
            CONSOLE_IN ("%x", &data);
            range.end_handle = (UINT16) data;

            retval = gatt_discover_is
                     (
                         &appl_gatt_client_handle,
                         range.start_handle,
                         range.end_handle
                     );
            break;
        case 30: /* Discover all Characteristic of a Service */
            IotLogInfo ("Enter the Start Handle[HEX] : ");
            CONSOLE_IN ("%x", &data);
            range.start_handle = (UINT16) data;

            IotLogInfo ("Enter the End Handle[HEX] : ");
            CONSOLE_IN ("%x", &data);
            range.end_handle = (UINT16) data;

            retval = gatt_discover_char
                     (
                         &appl_gatt_client_handle,
                         range.start_handle,
                         range.end_handle,
                         0x0000,
                         0
                     );
            break;
        case 31: /* Discover Characteristic by UUID */
            IotLogInfo ("Enter the Start Handle[HEX] : ");
            CONSOLE_IN ("%x", &data);
            range.start_handle = (UINT16) data;

            IotLogInfo ("Enter the End Handle[HEX] : ");
            CONSOLE_IN ("%x", &data);
            range.end_handle = (UINT16) data;

            IotLogInfo ("Enter the Characteristic UUID[HEX] : ");
            CONSOLE_IN ("%x", &data);

            retval = gatt_discover_char
                     (
                         &appl_gatt_client_handle,
                         range.start_handle,
                         range.end_handle,
                         (UINT16) data,
                         1
                     );
            break;
        case 40: /* Discover All Characteristic Descriptors */
            IotLogInfo ("Enter the Start Handle[HEX] : ");
            CONSOLE_IN ("%x", &data);
            range.start_handle = (UINT16) data;

            IotLogInfo ("Enter the End Handle[HEX] : ");
            CONSOLE_IN ("%x", &data);
            range.end_handle = (UINT16) data;

            retval = gatt_discover_char_desc
                     (
                         &appl_gatt_client_handle,
                         range.start_handle,
                         range.end_handle
                     );
            break;
        case 50: /* Read Characteristic Value */
        case 70: /* Read Characteristic Descriptors */
            IotLogInfo ("Enter the Handle[HEX] : ");
            CONSOLE_IN ("%x", &data);
#ifdef ATT_SUPPORT_128_BIT_UUID
            uuid.uuid_16 = 0x0000;
#else
            uuid = 0x0000;
#endif /* ATT_SUPPORT_128_BIT_UUID */
            retval = gatt_char_read
                     (
                         &appl_gatt_client_handle,
                         (UINT16) data,
                         0x0000,
                         uuid,
                         0
                     );
            break;
        case 51: /* Read Characteristic Using UUID */
            IotLogInfo ("Enter the Start handle[HEX] : ");
            CONSOLE_IN ("%x", &data);
            range.start_handle = (UINT16) data;

            IotLogInfo ("Enter the End handle[HEX] : ");
            CONSOLE_IN ("%x", &data);
            range.end_handle = (UINT16) data;

            IotLogInfo ("Enter UUID Format 1 - 16Bit and 2 - 128Bit: ");
            CONSOLE_IN ("%x", &data);
            IotLogInfo ("Enter the UUID[HEX] : ");
            if (ATT_16_BIT_UUID_FORMAT == data)
            {
#ifdef ATT_SUPPORT_128_BIT_UUID
                CONSOLE_IN ("%x", &choice);
                uuid.uuid_16 = (ATT_UUID16)choice;
#else /* ATT_SUPPORT_128_BIT_UUID */
                CONSOLE_IN ("%x", &choice);
                uuid = (ATT_UUID16)choice;
#endif /* ATT_SUPPORT_128_BIT_UUID */
            }
            else
            {
#ifdef ATT_SUPPORT_128_BIT_UUID
                for (index = 0; index < ATT_128_BIT_UUID_SIZE; index++)
                {
                    CONSOLE_IN ("%x", &indx);
                    uuid.uuid_128.value[index] = (UCHAR) indx;
                }
#else /* ATT_SUPPORT_128_BIT_UUID */
                IotLogInfo ("Enable ATT_SUPPORT_128_BIT_UUID support\n");
#endif /* ATT_SUPPORT_128_BIT_UUID */
            }

            retval = gatt_char_read
                     (
                         &appl_gatt_client_handle,
                         range.start_handle,
                         range.end_handle,
                         uuid,
                         (UCHAR) data
                     );
            break;
        case 52: /* Read Long Characteristic Values */
        case 71: /* Read Long Characteristic Descriptors */
            IotLogInfo ("Enter the handle[HEX] : ");
            CONSOLE_IN ("%x", &data);
            read_blob_req_param.handle = (UINT16) data;

            IotLogInfo ("Enter the Offset[DEC] : ");
            CONSOLE_IN ("%d", &data);
            read_blob_req_param.offset = (UINT16) data;

            retval = gatt_char_read_long
                     (
                         &appl_gatt_client_handle,
                         read_blob_req_param.handle,
                         read_blob_req_param.offset
                     );
            break;
        case 53: /* Read Multiple Characteristic Values */
            IotLogInfo ("Enter the number of Handles[DEC] : ");
            CONSOLE_IN ("%x",&data);

            if (0 != data)
            {
                read_multiple_param.list_count = data;
                read_multiple_param.handle_list = \
                    BT_alloc_mem ( 2 * read_multiple_param.list_count);
                if (NULL == read_multiple_param.handle_list)
                {
                    break;
                }
                for (index = 0; index < read_multiple_param.list_count;
                    index++)
                {
                    IotLogInfo ("[0x%04X]:Enter the Handle[HEX] : ",index);
                    CONSOLE_IN ("%x",&data);
                    read_multiple_param.handle_list[index] = data;
                }
                retval = gatt_char_read_multiple
                         (
                             &appl_gatt_client_handle,
                             &read_multiple_param
                         );
                BT_free_mem (read_multiple_param.handle_list);
            }
            else
            {
                IotLogInfo (
                "[APPL]: Invalid Entry, try again!");
            }
            break;
        case 60: /* Write Without Response */
            IotLogInfo ("Enter the Handle[HEX] : ");
            CONSOLE_IN("%x", &data);
            write_req_param.handle = (UINT16) data;

            IotLogInfo ("Enter the value len[HEX] : ");
            CONSOLE_IN("%x", &data);
            write_req_param.value.len = (UINT16)data;

            IotLogInfo ("Enter the value[HEX] : ");
            for(index = 0; index < write_req_param.value.len; index++)
            {
                CONSOLE_IN("%x", &data);
                value [index] = (UCHAR)data;
            }
            write_req_param.value.val = value;

            retval = gatt_char_wr
                     (
                         &appl_gatt_client_handle,
                         write_req_param.handle,
                         write_req_param.value.val,
                         write_req_param.value.len,
                         0x00
                     );
            break;
        case 61: /* Signed Write Without Response */
#ifdef SMP_DATA_SIGNING
            IotLogInfo ("Enter the Handle[HEX] : ");
            CONSOLE_IN("%x", &data);
            sign_write_req_param.handle_value.handle = (UINT16) data;

            IotLogInfo ("Enter the value len[HEX] (Max 10 bytes) : ");
            CONSOLE_IN("%x", &data);
            sign_write_req_param.handle_value.value.len = (UINT16)data;

            IotLogInfo ("Enter the value[HEX] : ");
            for(index = 0; index < sign_write_req_param.handle_value.value.len; index++)
            {
                CONSOLE_IN("%x", &data);
                sign_value [3 + index] = (UCHAR)data;
            }

            sign_write_req_param.handle_value.value.val = &sign_value[3];

            s_datalen = 1 + 2 + sign_write_req_param.handle_value.value.len;
            s_data = sign_value;
            s_data[0] = ATT_SIGNED_WRITE_CMD;
            BT_PACK_LE_2_BYTE(&s_data[1], &(sign_write_req_param.handle_value.handle));
            appl_smp_generate_sign_data(s_data, s_datalen);
#else /* SMP_DATA_SIGNING */
            IotLogInfo ("Data Signing NOT enabled\n");
#endif /* SMP_DATA_SIGNING */
            break;
        case 62: /* Write Characteristic Values */
        case 80: /* Write Characteristic Descriptors */
            IotLogInfo ("Enter the Handle[HEX] : ");
            CONSOLE_IN("%x", &data);
            write_req_param.handle = (UINT16) data;

            IotLogInfo ("Enter the value len[HEX] : ");
            CONSOLE_IN("%x", &data);
            write_req_param.value.len = (UINT16)data;

            IotLogInfo ("Enter the value[HEX] : ");
            for(index = 0; index < write_req_param.value.len; index++)
            {
                CONSOLE_IN("%x", &data);
                value [index] = (UCHAR)data;
            }
            write_req_param.value.val = value;

            retval = gatt_char_wr
                     (
                         &appl_gatt_client_handle,
                         write_req_param.handle,
                         write_req_param.value.val,
                         write_req_param.value.len,
                         0x01
                     );
            break;
        case 63: /* Write Long Characteristic Values */
        case 81: /* Write Long Characteristic Descriptors */
            IotLogInfo ("Enter the Handle[Hex] : ");
            CONSOLE_IN ("%x",&data);
            prepare_write_param.handle_value.handle = (UINT16) data;

            IotLogInfo ("Enter the Value Offset[DEC] : ");
            CONSOLE_IN ("%d",&data);
            prepare_write_param.offset = data;

            IotLogInfo ("Enter the Value length[DEC] : ");
            CONSOLE_IN ("%d",&data);
            prepare_write_param.handle_value.value.len = data;

            if (0 == prepare_write_param.handle_value.value.len)
            {
                IotLogInfo (
                "[APPL]: Invalid Zero Length, cannot process request!\n");
                break;
            }

            prepare_write_param.handle_value.value.val = BT_alloc_mem \
                (prepare_write_param.handle_value.value.len);

            if (NULL == prepare_write_param.handle_value.value.val)
            {
                IotLogInfo (
                "[APPL]: Failed to allocate memory of size 0x%04X\n",
                prepare_write_param.handle_value.value.len);

                break;
            }

            IotLogInfo ("Enter the Value[HEX] : ");
            for (index = 0;
                index < prepare_write_param.handle_value.value.len; index ++)
            {
                CONSOLE_IN ("%x",&data);
                prepare_write_param.handle_value.value.val[index] = data;
            }

            retval = gatt_char_wr_long
                     (
                         &appl_gatt_client_handle,
                         &prepare_write_param
                     );

            BT_free_mem (prepare_write_param.handle_value.value.val);
            break;
        case 64: /* Characteristic Value Reliable Writes */
            IotLogInfo ("Enter the number of Reliable Writes[DEC] : ");
            CONSOLE_IN ("%x",&data);

            if (0 != data)
            {
                appl_req_param.count = data;
                for (index = 0; index < appl_req_param.count ; index++)
                {
                    IotLogInfo ("[0x%04X]:Enter the Handle[HEX] : ",index);
                    CONSOLE_IN ("%x",&data);
                    appl_req_param.req_param[index].handle_value.handle = data;

                    IotLogInfo ("[0x%04X]:Enter the Value Offset[DEC] : ",index);
                    CONSOLE_IN ("%d",&data);
                    appl_req_param.req_param[index].offset = data;

                    IotLogInfo ("[0x%04X]:Enter the Value length[DEC] : ",index);
                    CONSOLE_IN ("%d",&data);
                    appl_req_param.req_param[index].handle_value.value.len = data;

                    if (0 == appl_req_param.req_param[index].handle_value.value.len)
                    {
                        IotLogInfo (
                        "[APPL]: Invalid Zero Length, cannot process request!\n");
                        break;
                    }

                    appl_req_param.req_param[index].handle_value.value.val = BT_alloc_mem \
                        (appl_req_param.req_param[index].handle_value.value.len);

                    if (NULL == appl_req_param.req_param[index].handle_value.value.val)
                    {
                        IotLogInfo (
                        "[APPL]: Failed to allocate memory of size 0x%04X\n",
                        appl_req_param.req_param[index].handle_value.value.len);
                        break;
                    }

                    IotLogInfo ("Enter the Value[HEX] : ");
                    for (indx = 0;
                    indx < appl_req_param.req_param[index].handle_value.value.len; indx ++)
                    {
                        CONSOLE_IN ("%x",&data);
                        appl_req_param.req_param[index].handle_value.value.val[indx] = data;
                    }
                }
            }
            else
            {
                IotLogInfo (
                "[APPL]: Invalid Entry, try again!");
            }
            retval = gatt_char_reliable_wr
                     (
                         &appl_gatt_client_handle,
                         &appl_req_param
                     );
            for (indx = 0; indx < appl_req_param.count; indx ++)
            {
                BT_free_mem(appl_req_param.req_param[indx].handle_value.value.val);
            }
            break;
        case 90: /* Exchange MTU Request */
            IotLogInfo ("Enter the MTU for the ATT connection [HEX]\n");
            CONSOLE_IN ("%x", &data);

            retval = gatt_xchg_mtu
                     (
                         &appl_gatt_client_handle,
                         (UINT16) data,
                         0
                     );

            IotLogInfo ("Exchange MTU REQ retval - 0x%04X\n", retval);
            break;

        case 100:
            general_service_discovery = 0x00;
            appl_profile_operations ();
            general_service_discovery = 0x01;
            break;

        default:
            break;
        }
    }
}


void appl_profile_operations (void)
{
    int choice;

    BT_LOOP_FOREVER()
    {
        IotLogInfo ("%s \n", profile_client_menu);
        (void)profile_client_menu;    /*fix build warning: set but never used.*/
        IotLogInfo ("Enter you choice : ");
        CONSOLE_IN ("%d", &choice);
        switch(choice)
        {
        case 0:
            return;

        case 1:
            break;

        case 10:
#ifdef DIC
            dic_profile_operations();
#endif /* DIC */
            break;

        case 20:
#ifdef BAC
            bac_profile_operations();
#endif /* BAC */
            break;

        case 30:
#ifdef HTC
            htc_profile_operations ();
#endif /* HTC */
            break;

        case 40:
#ifdef HRC
            hrc_profile_operations ();
#endif /* HRC */
            break;

        case 50:
#ifdef RSCC
            rscc_profile_operations ();
#endif /* RSCC */
            break;

        case 60:
#ifdef CSCC
            cscc_profile_operations ();
#endif /* CSCC */
            break;

        case 70:
#ifdef LNC
            lnc_profile_operations ();
#endif /* LNC */
            break;

        case 80:
#ifdef CPMC
            cpmc_profile_operations ();
#endif /* CPMC */
            break;

        case 90:
#ifdef WSC
            wsc_profile_operations ();
#endif /* WSC */
            break;

        case 100:
#ifdef ESC
            esc_profile_operations ();
#endif /* ESC */
            break;

        case 110:
#ifdef BMC
            bmc_profile_operations ();
#endif /* BMC */
            break;

        case 120:
#ifdef AIOC
            aioc_profile_operations ();
#endif /* AIOC */
            break;

        case 130:
#ifdef IPC
            ipc_profile_operations ();
#endif /* IPC */
            break;

        case 140:
#ifdef PXM
            pxm_profile_operations ();
#endif /* PXM */
            break;

        case 150:
#ifdef FML
            fml_profile_operations ();
#endif /* FML */
            break;

        case 160:
#ifdef HIDH
            hidh_profile_operations ();
#endif /* HIDH */
            break;

        case 170:
#ifdef BPC
            bpc_profile_operations ();
#endif /* BPC */
            break;

        case 180:
#ifdef PASP
            pasp_profile_operations ();
#endif /* PASP */
            break;

        case 190:
#ifdef POC
            poc_profile_operations ();
#endif /* POC */
            break;

        case 200:
#ifdef CPMC
            cpmc_profile_operations ();
#endif /* CPMC */
            break;

        case 210:
#ifdef LNC
            lnc_profile_operations ();
#endif /* LNC */
            break;

        case 220:
#ifdef SPC
            spc_profile_operations ();
#endif /* SPC */
            break;

        case 230:
#ifdef GLC
            glc_profile_operations ();
#endif /* GLC */
            break;

        case 240:
#ifdef TIC
            tic_profile_operations ();
#endif /* TIC */
            break;

        case 260:
#ifdef ANP
            anp_profile_operations ();
#endif /* ANP */
            break;
        case 270:
#ifdef FTMC
            ftmc_profile_operations ();
#endif /* FTMC */
            break;

        case 280:
#ifdef AMC
            amc_profile_operations ();
#endif /* AMC */
            break;

        case 290:
#ifdef ANCC
            ancc_profile_operations ();
#endif /* ANCC */
            break;

        case 300:
#ifdef RCC
            rcc_profile_operations();
#endif /* RCC */
            break;

        case 310:
#ifdef OTC
            otc_profile_operations();
#endif /* OTC */
            break;
        case 320:
#ifdef FTMC
            ftmc_profile_operations();
#endif /* FTMC */
            break;

        default:
            break;
        }
    }
}

UCHAR * appl_display_uuid_label(UINT16 uuid)
{
    int index;

    index=0;

    do
    {
        if (uuid == appl_uuid_desc_table[index].uuid)
        {
            return appl_uuid_desc_table[index].desc_str;
        }
        index++;
    }while (0 != appl_uuid_desc_table[index].uuid);

    return (UCHAR *)"Unknown";
}

#ifdef ATT_SUPPORT_128_BIT_UUID
void appl_print_128_bit_uuid (ATT_UUID128 * uuid)
{
    INT32 index;
    IotLogInfo ("0x");

    index = ATT_128_BIT_UUID_SIZE ;

    do
    {
        index--;
        IotLogInfo ("%X",uuid->value[index]);

    } while (index > 0);
}
#endif /* ATT_SUPPORT_128_BIT_UUID */

void appl_gatt_init (void)
{
    gatt_init (appl_gatt_cb);
}

#ifdef SMP_DATA_SIGNING
void appl_gatt_signing_complete (API_RESULT status, UCHAR * data, UINT16 datalen)
{
    if (API_SUCCESS == status)
    {
        BT_mem_copy (sign_write_req_param.auth_signature, data, datalen);
        BT_att_send_signed_write_cmd (&appl_gatt_client_handle, &sign_write_req_param);
    }
    else
    {
        /* Data signing procedure failed */
    }
}
#endif /* SMP_DATA_SIGNING */

void appl_notify_gatt_conn(void)
{
#ifdef HIDH
    APPL_NOTIFY_GATT_CONN_TO_PROFILE(hidh);
#endif /* HIDH */
}

void appl_notify_gatt_disconn(void)
{
#ifdef HIDH
    APPL_NOTIFY_GATT_DISCONN_TO_PROFILE(hidh);
#endif /* HIDH */
}

void appl_notify_gatt_servdata (GATT_SERVICE_PARAM * service, UINT16 size)
{
#ifdef HIDH
    APPL_NOTIFY_GATT_SERVDATA_TO_PROFILE(hidh, service, size);
#endif /* HIDH */
}

void appl_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size)
{
    /* Notify individual service handlers about peer handles */

#ifdef BAC
    APPL_NOTIFY_GATT_CHARDATA_TO_PROFILE(bac, characteristic, size);
#endif /* BAC */
#ifdef DIC
    APPL_NOTIFY_GATT_CHARDATA_TO_PROFILE(dic, characteristic, size);
#endif /* DIC */
#ifdef HTC
    APPL_NOTIFY_GATT_CHARDATA_TO_PROFILE(htc, characteristic, size);
#endif /* HTC */
#ifdef HRC
    APPL_NOTIFY_GATT_CHARDATA_TO_PROFILE(hrc, characteristic, size);
#endif /* HRC */
#ifdef RSCC
    APPL_NOTIFY_GATT_CHARDATA_TO_PROFILE(rscc, characteristic, size);
#endif /* RSCC */
#ifdef CSCC
    APPL_NOTIFY_GATT_CHARDATA_TO_PROFILE(cscc, characteristic, size);
#endif /* CSCC */
#ifdef LNC
    APPL_NOTIFY_GATT_CHARDATA_TO_PROFILE(lnc, characteristic, size);
#endif /* LNC */
#ifdef CPMC
    APPL_NOTIFY_GATT_CHARDATA_TO_PROFILE(cpmc, characteristic, size);
#endif /* CPMC */
#ifdef WSC
    APPL_NOTIFY_GATT_CHARDATA_TO_PROFILE(wsc, characteristic, size);
#endif /* WSC */
#ifdef ESC
    APPL_NOTIFY_GATT_CHARDATA_TO_PROFILE(esc, characteristic, size);
#endif /* ESC */
#ifdef BMC
    APPL_NOTIFY_GATT_CHARDATA_TO_PROFILE(bmc, characteristic, size);
#endif /* BMC */
#ifdef AIOC
    APPL_NOTIFY_GATT_CHARDATA_TO_PROFILE(aioc, characteristic, size);
#endif /* AIOC */
#ifdef IPC
    APPL_NOTIFY_GATT_CHARDATA_TO_PROFILE(ipc, characteristic, size);
#endif /* IPC */
#ifdef PXM
    APPL_NOTIFY_GATT_CHARDATA_TO_PROFILE(pxm, characteristic, size);
#endif /*PXM */
#ifdef FML
    APPL_NOTIFY_GATT_CHARDATA_TO_PROFILE(fml, characteristic, size);
#endif /*FML */
#ifdef HIDH
    APPL_NOTIFY_GATT_CHARDATA_TO_PROFILE(hidh, characteristic, size);
#endif /*HIDH */
#ifdef ANP
    APPL_NOTIFY_GATT_CHARDATA_TO_PROFILE(anp, characteristic, size);
#endif /* ANP */
#ifdef BPC
    APPL_NOTIFY_GATT_CHARDATA_TO_PROFILE(bpc, characteristic, size);
#endif /*BPC */
#ifdef PASP
    APPL_NOTIFY_GATT_CHARDATA_TO_PROFILE(pasp, characteristic, size);
#endif /*PASP */
#ifdef POC
    APPL_NOTIFY_GATT_CHARDATA_TO_PROFILE(poc, characteristic, size);
#endif /*POC */
#ifdef LNC
    APPL_NOTIFY_GATT_CHARDATA_TO_PROFILE(lnc, characteristic, size);
#endif /*LNC */
#ifdef SPC
    APPL_NOTIFY_GATT_CHARDATA_TO_PROFILE(spc, characteristic, size);
#endif /*SPC */
#ifdef GLC
    APPL_NOTIFY_GATT_CHARDATA_TO_PROFILE(glc, characteristic, size);
#endif /*GLC */
#ifdef TIC
    APPL_NOTIFY_GATT_CHARDATA_TO_PROFILE(tic, characteristic, size);
#endif /* TIC */
#ifdef FTMC
    APPL_NOTIFY_GATT_CHARDATA_TO_PROFILE(ftmc, characteristic, size);
#endif /* FTMC */
#ifdef AMC
    APPL_NOTIFY_GATT_CHARDATA_TO_PROFILE(amc, characteristic, size);
#endif /* AMC */
#ifdef ANCC
    APPL_NOTIFY_GATT_CHARDATA_TO_PROFILE(ancc, characteristic, size);
#endif /* ANCC */
#ifdef RCC
    APPL_NOTIFY_GATT_CHARDATA_TO_PROFILE(rcc, characteristic, size);
#endif /* RCC */
#ifdef OTC
    APPL_NOTIFY_GATT_CHARDATA_TO_PROFILE(otc, characteristic, size);
#endif /* OTC */
}

void appl_parse_notification_data(UINT16 handle, UCHAR * data, UCHAR datalen)
{
#ifdef AMC
    amc_parse_notification_data(handle, data, datalen);
#endif /* AMC */
#ifdef ANCC
    ancc_parse_notification(handle, data, datalen);
#endif /* ANCC */
}

void appl_parse_indication_data(UINT16 handle, UCHAR * data, UCHAR datalen)
{
#ifdef OTC
    otc_parse_indication(handle, data, datalen);
#endif /* OTC */
}
#endif /* ATT */

