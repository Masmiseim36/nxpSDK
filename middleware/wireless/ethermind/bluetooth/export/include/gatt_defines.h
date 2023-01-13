/**
 *  \file gatt_defines.h
 *
 *  \brief Contains GATT defined Constants
 */

/*
 *  Copyright (C) 2014. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GATT_DEFINES_
#define _H_GATT_DEFINES_

/**
 * \defgroup gatt_defines Defines
 * \{
 * \brief This section describes the various GATT (Generic Attribute) Defines
 * from the Bluetooth Spec.
 */

/**
 * \defgroup gatt_constants Constants
 * \{
 * \brief This section describes the various GATT (Generic Attribute) Constants.
 */

/**
 * \name GATT Declaration related defines
 * \{
 * \brief This section describes the various GATT (Generic Attribute)
 * Declaration related 16-bit UUIDs as defined in the Bluetooth Assigned
 * Numbers. These defines are used for GATT Service or Characteristic
 * Declarations.
 */

/** Primary Service */
#define GATT_PRIMARY_SERVICE                                         0x2800U
/** Secondary Service */
#define GATT_SECONDARY_SERVICE                                       0x2801U
/** Include */
#define GATT_INCLUDE                                                 0x2802U
/** Characteristic */
#define GATT_CHARACTERISTIC                                          0x2803U

/** \} */

/**
 * \name GATT Descriptor related defines
 * \{
 * \brief This section describes the various GATT (Generic Attribute)
 * Descriptor related 16-bit UUIDs as defined in the Bluetooth Assigned
 * Numbers. These defines are used for GATT Characteristic Descriptors.
 */
/** Characteristic Extended Properties */
#define GATT_EXTENDED_PROPERTIES                                     0x2900U
/** Characteristic User Description */
#define GATT_USER_DESCRIPTION                                        0x2901U
/** Client Characteristic Configuration */
#define GATT_CLIENT_CONFIG                                           0x2902U
/** Server Characteristic Configuration */
#define GATT_SERVER_CONFIG                                           0x2903U
/** Characteristic Presentation Format */
#define GATT_FORMAT                                                  0x2904U
/** Characteristic Aggregate Format */
#define GATT_AGGREGATE_FORMAT                                        0x2905U
/** Valid Range */
#define GATT_VALID_RANGE                                             0x2906U
/** External Report Reference */
#define GATT_EXTERNAL_REPORT_REF                                     0x2907U
/** Report Reference */
#define GATT_IP_OP_FEATURE_REPORT_REF                                0x2908U
/** Number of Digitals */
#define GATT_NUMBER_OF_DIGITALS                                      0x2909U
/** Value Trigger Setting */
#define GATT_VALUE_TRIGGER_SETTING                                   0x290AU
/** Environmental Sensing Configuration */
#define GATT_ES_CONFIGURATION                                        0x290BU
/** Environmental Sensing Measurement */
#define GATT_ES_MEASUREMENT                                          0x290CU
/** Environmental Sensing Trigger Setting */
#define GATT_ES_TRIGGER_SETTING                                      0x290DU
/** Timer Trigger Setting */
#define GATT_TIME_TRIGGER_SETTING                                    0x290EU
/** Complete BREDR Transport Block Data */
#define GATT_COMPLETE_BREDR_TRANSPORT_BLOCK_DATA                     0x290FU

/** \} */

/**
 * \name GATT Characteristic Properties Bit Field
 * \{
 * \brief This section describes the various GATT (Generic Attribute)
 * Characteristic Properties bit field as defined in the Bluetooth Spec.
 */

/** Broadcast Property */
#define GATT_CH_PROP_BIT_BROADCAST                                   0x01U

/** Read Property */
#define GATT_CH_PROP_BIT_READ                                        0x02U

/** Write Without Response Property */
#define GATT_CH_PROP_BIT_WRITE_WO_RSP                                0x04U

/** Write Property */
#define GATT_CH_PROP_BIT_WRITE                                       0x08U

/** Notify Property */
#define GATT_CH_PROP_BIT_NOTIFY                                      0x10U

/** Indicate Property */
#define GATT_CH_PROP_BIT_INDICATE                                    0x20U

/** Authenticated Signed Writes Property */
#define GATT_CH_PROP_BIT_SIGN_WRITE                                  0x40U

/** Extended Properties Property */
#define GATT_CH_PROP_BIT_EXT_PROPERTY                                0x80U

/** \} */

/**
 * \name GATT Client Configuration values
 * \{
 * \brief This section describes the GATT Client Configuration Values.
 */

/** Configure GATT Client for Notification */
#define GATT_CLI_CNFG_NOTIFICATION                                   0x0001U
/** Configure GATT Client for Indication */
#define GATT_CLI_CNFG_INDICATION                                     0x0002U
/** Configure GATT Client for Default */
#define GATT_CLI_CNFG_DEFAULT                                        0x0000U

/** \} */

/**
 * \name GATT Server Configuration values
 * \{
 * \brief This section describes the GATT Server Configuration Values.
 */

/** Configure GATT Server for Broadcast */
#define GATT_SER_CNFG_BROADCAST                                      0x0001U
/** Configure GATT Server for Default */
#define GATT_SER_CNFG_DEFAULT                                        0x0000U

/** \} */

/**
 * \name GATT Services
 * \{
 * \brief This section describes the various GATT Services 16-bit UUIDs as
 * defined in the Bluetooth Assigned Numbers.
 */

/** GAP (Generic Access) */
#define GATT_GAP_SERVICE                                             0x1800U
/** GATT (Generic Attribute) Service */
#define GATT_GATT_SERVICE                                            0x1801U
/** IAS (Immediate Alert) Service */
#define GATT_IMMEDIATE_ALERT_SERVICE                                 0x1802U
/** LLS (Link Loss) Service */
#define GATT_LINK_LOSS_SERVICE                                       0x1803U
/** TPS (Tx Power) Service */
#define GATT_TX_POWER_SERVICE                                        0x1804U
/** CTS (Current Time) Service */
#define GATT_CURRENT_TIME_SERVICE                                    0x1805U
/** RTUS (Reference Time Update) Service */
#define GATT_REF_TIME_UPDATE_SERVICE                                 0x1806U
/** NDCS (Next DST Change) Service */
#define GATT_NEXT_DST_CHANGE_SERVICE                                 0x1807U
/** GLS (Glucose) Service */
#define GATT_GLUCOSE_SERVICE                                         0x1808U
/** HTS (Health Thermometer) Service */
#define GATT_HEALTH_THERMOMETER_SERVICE                              0x1809U
/** DIS (Device Information) Service */
#define GATT_DEVICE_INFO_SERVICE                                     0x180AU
/** HRS (Heart Rate) Service */
#define GATT_HEART_RATE_SERVICE                                      0x180DU
/** PASS (Phone Alert Status) Service */
#define GATT_PHONE_ALERT_STATUS_SERVICE                              0x180EU
/** BAS (Battery) Service */
#define GATT_BATTERY_SERVICE                                         0x180FU
/** BLS (Blood Pressure) Service */
#define GATT_BLOOD_PRESSURE_SERVICE                                  0x1810U
/** ANS (Alert Notification) Service */
#define GATT_ALERT_NOTIFICATION_SERVICE                              0x1811U
/** HIDS (Human Interface Device) Service */
#define GATT_HID_SERVICE                                             0x1812U
/** ScPS (Scan Parameters) Service */
#define GATT_SCAN_PARAM_SERVICE                                      0x1813U
/** RSCS (Running Speed and Cadence) Service */
#define GATT_RUNNING_SPEED_AND_CADENCE_SERVICE                       0x1814U
/** AIOS (Automation IO) Service */
#define GATT_AIOS_SERVICE                                            0x1815U
/** CSCS (Cycling Speed and Cadence) Service */
#define GATT_CYCLING_SPEED_AND_CADENCE_SERVICE                       0x1816U
/** CPS (Cycling Power) Service */
#define GATT_CPM_SERVICE                                             0x1818U
/** LNS (Location and Navigation) Service */
#define GATT_LOCATION_AND_NAVIGATION_SERVICE                         0x1819U
/** ESS (Environmental Sensing) Service */
#define GATT_ENVIRONMENTAL_SENSING_SERVICE                           0x181AU
/** BCS (Body Composition) Service */
#define GATT_BODY_COMPOSITION_SERVICE                                0x181BU
/** UDS (User Data) Service */
#define GATT_USER_DATA_SERVICE                                       0x181CU
/** WSS (Weight Scale) Service */
#define GATT_WEIGHT_SCALE_SERVICE                                    0x181DU
/** BMS (Bond Management) Service */
#define GATT_BM_SERVICE                                              0x181EU
/** CGMS (Continuous Glucose Monitoring) Service */
#define GATT_CGM_SERVICE                                             0x181FU
/** IPSP (Internet Protocol Support) Service */
#define GATT_INT_PS_SERVICE                                          0x1820U
/** IPS (Indoor Positioning) Service */
#define GATT_IPS_SERVICE                                             0x1821U
/** PLXS (Pulse Oximeter) Service */
#define GATT_PULSE_OXIMETER_SERVICE                                  0x1822U
/** HPS (HTTP Proxy) Service */
#define GATT_HPS_SERVICE                                             0x1823U
/** TDS (Transport Discovery) Service */
#define GATT_TDS_SERVICE                                             0x1824U
/** OTS (Object Transfer) Service */
#define GATT_OBJECT_TRANSFER_SERVICE                                 0x1825U
/** FTMS (Fitness Machine) Service */
#define GATT_FTMS_SERVICE                                            0x1826U
/** RCS (Reconnection Configuration) Service */
#define GATT_RCS_SERVICE                                             0x1829U
/** IDS (Insulin Delivery) Service */
#define GATT_INSULIN_DELIVERY_SERVICE                                0x183AU
/** BSS (Binary Sensor) Service */
#define GATT_BINARY_SENSOR_SERVICE                                   0x183BU
/** EMCS (Emergency Configuration) Service */
#define GATT_EMERGENCY_CONFIGURATION_SERVICE                         0x183CU
/** PAMS (Physical Activity Monitor) Service */
#define GATT_PAMS_SERVICE                                            0x183EU
/** AICS (Audio Input Control) Service */
#define GATT_AICS_SERVICE                                            0x1843U
/** VCS (Volume Control) Service */
#define GATT_VCS_SERVICE                                             0x1844U
/** VOCS (Volume Offset Control) Service */
#define GATT_VOCS_SERVICE                                            0x1845U
/** CSIS (Coordinated Set Identification) Service */
#define GATT_CSIS_SERVICE                                            0x1846U
/** DTS (Device Time) Service */
#define GATT_DEVICE_TIME_SERVICE                                     0x1847U
/** MCS (Media Control) Service */
#define GATT_MCS_SERVICE                                             0x1848U
/** GMCS (Generic Media Control) Service */
#define GATT_GMCS_SERVICE                                            0x1849U
/** TBS (Telephone Bearer) Service */
#define GATT_TBS_SERVICE                                             0x184BU
/** GTBS (Generic Telephone Bearer) Service */
#define GATT_GTBS_SERVICE                                            0x184CU
/** MICS (Microphone Control) Service */
#define GATT_MICS_SERVICE                                            0x184DU
/** ASCS (Audio Stream Control) Service */
#define GATT_ASCS_SERVICE                                            0x184EU
/** BASS (Broadcast Audio Scan) Service */
#define GATT_BASS_SERVICE                                            0x184FU
/** PACS (Published Audio Capabilities) Service */
#define GATT_PACS_SERVICE                                            0x1850U
/** BAAS (Basic Audio Announcement) Service */
#define GATT_BAAS_SERVICE                                            0x1851U
/** BCAS (Broadcast Audio Announcement) Service */
#define GATT_BCAS_SERVICE                                            0x1852U
/** \} */

/**
 * \name GATT Unused Characteristics Defines
 * \{
 * \brief This section describes the various GATT Characteristics 16-bit UUIDs
 * as defined in the Bluetooth Assigned Numbers.
 * \note
 * These characteristics are Obsolete in current Specification versions. These
 * will be removed later when their corresponding specifications are
 * deprecated.
 */

/** Peripheral Privacy Flag */
#define GATT_PERIPHRL_PRIVCY_FLG_CHARACTERISTIC                      0x2A02U
/** Reconnection Address */
#define GATT_RECONNECT_ADDR_CHARACTERISTIC                           0x2A03U
/** Exact Time 256 */
#define GATT_EXACT_TIME_256_CHARACTERISTIC                           0x2A0CU

/** \} */

/**
 * \name GAP Service Characteristics Defines
 * \{
 * \brief This section describes the various GAP Service Characteristics
 * 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** Device Name */
#define GATT_DEVICE_NAME_CHARACTERISTIC                              0x2A00U
/** Appearance */
#define GATT_APPEARANCE_CHARACTERISTIC                               0x2A01U
/** Peripheral Preferred Connection Parameters */
#define GATT_PRFRRD_CNXN_PARAM_CHARACTERISTIC                        0x2A04U
/** Central Address Resolution */
#define GATT_CENTRAL_ADDRESS_RESOLUTION                              0x2AA6U
/** Resolvable Private Address Only */
#define GATT_RESOLVABLE_PRIVATE_ADDRESS_ONLY                         0x2AC9U

/** \} */

/**
 * \name GATT Service Characteristics Defines
 * \{
 * \brief This section describes the various GATT Service Characteristics
 * 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** Service Changed */
#define GATT_SERVICE_CHANGED_CHARACTERISTIC                          0x2A05U
/** Client Supported Features */
#define GATT_CLIENT_SUPP_FEATURES_CHARACTERISTIC                     0x2B29U
/** Database Hash */
#define GATT_DATABASE_HASH_CHARACTERISTIC                            0x2B2AU
/** Server Supported Features */
#define GATT_SERVER_SUPP_FEATURES_CHARACTERISTIC                     0x2B3AU

/** \} */

/**
 * \name Common Characteristics Defines
 * \{
 * \brief This section describes the 16-bit UUIDs of Common Characteristic,
 * that belong to two or more Services, as defined in the Bluetooth Assigned Numbers.
 */

/** Record Access Control Point */
#define GATT_RACP_CHARACTERISTIC                                     0x2A52U
/** Speed and Cadence Control Point */
#define GATT_SC_CONTROL_POINT_CHARACTERISTIC                         0x2A55U
/** Sensor Location */
#define GATT_SENSOR_LOCATION_CHARACTERISTIC                          0x2A5DU

/** \} */

/**
 * \name Immediate Alert Service and Link Loss Service Characteristics Defines
 * \{
 * \brief This section describes the Record Access Control Point Characteristic
 * 16-bit UUID as defined in the Bluetooth Assigned Numbers.
 * The characteristic is common for Immediate Alert and Link Loss Services.
 */

/** Alert Level */
#define GATT_ALERT_LEVEL_CHARACTERISTIC                              0x2A06U

/** \} */

/**
 * \name Glucose Service Characteristics Defines
 * \{
 * \brief This section describes the various Glucose Service Characteristics
 * 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** Glucose Measurement */
#define GATT_GLUCOSE_MSRMNT_CHARACTERISTIC                           0x2A18U
/** Glucose Measurement Context */
#define GATT_GL_MSRMT_CTX_CHARACTERISTIC                             0x2A34U
/** Glucose Feature */
#define GATT_GL_FEATURE_CHARACTERISTIC                               0x2A51U

/** \} */

/**
 * \name Running Speed And Cadence Service Characteristics Defines
 * \{
 * \brief This section describes the various Running Speed And Cadence Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** RSC Measurement */
#define GATT_RSC_MSRMT_CHARACTERISTIC                                0x2A53U
/** RSC Feature */
#define GATT_RSC_FEATURE_CHARACTERISTIC                              0x2A54U

/** \} */

/**
 * \name Cycling Speed And Cadence Service Characteristics Defines
 * \{
 * \brief This section describes the various Cycling Speed And Cadence Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** CSC Measurement */
#define GATT_CSC_MSRMT_CHARACTERISTIC                                0x2A5BU
/** CSC Feature */
#define GATT_CSC_FEATURE_CHARACTERISTIC                              0x2A5CU

/** \} */

/**
 * \name Device Information Service Characteristics Defines
 * \{
 * \brief This section describes the various Device Information Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** Manufacturer Name String */
#define GATT_MANUFACTURER_NAME_CHARACTERISTIC                        0x2A29U
/** Model Number String */
#define GATT_MODEL_NUMBER_CHARACTERISTIC                             0x2A24U
/** Serial Number String */
#define GATT_SERIAL_NUMBER_CHARACTERISTIC                            0x2A25U
/** Hardware Revision String */
#define GATT_HARDWARE_REV_CHARACTERISTIC                             0x2A27U
/** Firmware Revision String */
#define GATT_FIRMWARE_REV_CHARACTERISTIC                             0x2A26U
/** Software Revision String */
#define GATT_SOFTWARE_REV_CHARACTERISTIC                             0x2A28U
/** System ID */
#define GATT_SYSTEM_ID_CHARACTERISTIC                                0x2A23U
/** IEEE 11073-20601 Regulatory Certification Data List */
#define GATT_REG_CERT_DATA_CHARACTERISTIC                            0x2A2AU
/** PnP ID */
#define GATT_PNP_ID_CHARACTERISTIC                                   0x2A50U

/** \} */

/**
 * \name Phone Alert Status Service Characteristics Defines
 * \{
 * \brief This section describes the various Phone Alert Status Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** Alert Status */
#define GATT_ALERT_STATUS_CHARACTERISTIC                             0x2A3FU
/** Ringer Setting */
#define GATT_RINGER_SETTING_CHARACTERISTIC                           0x2A41U
/** Ringer Control Point */
#define GATT_RINGER_CONTROL_POINT_CHARACTERISTIC                     0x2A40U

/** \} */

/**
 * \name Alert Notification Service Characteristics Defines
 * \{
 * \brief This section describes the various Alert Notification Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** Supported New Alert Category */
#define GATT_SPRTD_NEW_ALRT_CTGRY_CHARACTERISTIC                     0x2A47U
/** New Alert */
#define GATT_NEW_ALERT_CHARACTERISTIC                                0x2A46U
/** Supported Unread Alert Category */
#define GATT_SPRT_UNRD_ALRT_CTGRY_CHARACTERISTIC                     0x2A48U
/** Unread Alert Status */
#define GATT_UNREAD_ALRT_STATUS_CHARACTERISTIC                       0x2A45U
/** Alert Notification Control Point */
#define GATT_ALERT_NTF_CONTROL_POINT                                 0x2A44U
/** Alert Category ID Bit Mask */
#define GATT_ALERT_ID_BIT_MASK_CHARACTERISTIC                        0x2A42U
/** Alert Category ID */
#define GATT_ALERT_ID_CHARACTERISTIC                                 0x2A43U

/** \} */

/**
 * \name Battery Service Characteristics Defines
 * \{
 * \brief This section describes the various Battery Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** Battery Level */
#define GATT_BATTERY_LEVEL_CHARACTERISTIC                            0x2A19U

/** \} */

/**
 * \name Blood Pressure Service Characteristics Defines
 * \{
 * \brief This section describes the various Blood Pressure Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** Blood Pressure Measurement */
#define GATT_BP_MSRMT_CHARACTERISTIC                                 0x2A35U
/** Intermediate Cuff Pressure */
#define GATT_INTRMDT_CUFF_PRSR_CHARACTERISTIC                        0x2A36U
/** Blood Pressure Feature */
#define GATT_BP_FEATURE_CHARACTERISTIC                               0x2A49U
/** Enhanced Blood Pressure Measurement */
#define GATT_BP_ENHANCED_BP_MSRMT_CHARACTERISTIC                     0x2B34U
/** Enhanced Intermediate Cuff Pressure */
#define GATT_BP_ENHANCED_INTERM_CUFF_PRESSURE_CHARACTERISTIC         0x2B35U
/** Blood Pressure Record */
#define GATT_BP_BLOOD_PRESSURE_RECORD_CHARACTERISTIC                 0x2B36U

/** \} */

/**
 * \name Health Thermometer Service Characteristics Defines
 * \{
 * \brief This section describes the various Health Thermometer Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** Temperature Measurement */
#define GATT_TEMPERATURE_MSMNT_CHARACTERISTIC                        0x2A1CU
/** Temperature Type */
#define GATT_TEMPERATURE_TYPE_CHARACTERISTIC                         0x2A1DU
/** Intermediate Temperature */
#define GATT_INTERMEDIATE_TEMP_CHARACTERISTIC                        0x2A1EU
/** Measurement Interval */
#define GATT_MSMNT_INTERVAL_CHARATACTERISTIC                         0x2A21U

/** \} */

/**
 * \name Heart Rate Service Characteristics Defines
 * \{
 * \brief This section describes the various Heart Rate Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** Heart Rate Measurement */
#define GATT_HR_MSRMT_CHARACTERISTIC                                 0x2A37U
/** Body Sensor Location */
#define GATT_BODY_SENSOR_LOC_CHARACTERISTIC                          0x2A38U
/** Heart Rate Control Point */
#define GATT_HR_CNTRL_POINT                                          0x2A39U

/** \} */

/**
 * \name Next DST Change Service Characteristics Defines
 * \{
 * \brief This section describes the various Next DST Change Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** Time with DST */
#define GATT_TIME_WITH_DST_CHARACTERISTIC                            0x2A11U

/** \} */

/**
 * \name Current Time Service Characteristics Defines
 * \{
 * \brief This section describes the various Current Time Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** Current Time */
#define GATT_CURRENT_TIME_CHARACTERISTIC                             0x2A2BU
/** Local Time Information */
#define GATT_LOCAL_TIME_INFO_CHARACTERISTIC                          0x2A0FU
/** Reference Time Information */
#define GATT_REF_TIME_INFO_CHARACTERISTIC                            0x2A14U
/** DST Offset */
#define GATT_DST_OFFSET_CHARACTERISTIC                               0x2A0DU
/** Time Zone */
#define GATT_TIME_ZONE_CHARACTERISTIC                                0x2A0EU
/** Time Accuracy */
#define GATT_TIME_ACCURACY_CHARACTERISTIC                            0x2A12U
/** Time Source */
#define GATT_TIME_SOURCE_CHARACTERISTIC                              0x2A13U
/** Date Time */
#define GATT_DATE_TIME_CHARACTERISTIC                                0x2A08U
/** Day of Week */
#define GATT_DAY_OF_WEEK_CHARACTERISTIC                              0x2A09U
/** Day Date Time */
#define GATT_DAY_DATE_TIME_CHARACTERISTIC                            0x2A0AU

/** \} */

/**
 * \name Reference Time Update Service Characteristics Defines
 * \{
 * \brief This section describes the various Reference Time Update Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** Time Update Control Point */
#define GATT_TIME_UPDATE_CONTROL_POINT                               0x2A16U
/** Time Update State */
#define GATT_TIME_UPDATE_STATE_CHARACTERISTIC                        0x2A17U

/** \} */

/**
 * \name Scan Parameters Service Characteristics Defines
 * \{
 * \brief This section describes the various Scan Parameters Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** Scan Interval Window */
#define GATT_SCAN_WINDOW_CHARACTERISTIC                              0x2A4FU
/** Scan Refresh */
#define GATT_SCAN_REFRESH_CHARACTERISTIC                             0x2A31U

/** \} */

/**
 * \name TX Power Service Characteristics Defines
 * \{
 * \brief This section describes the various TX Power Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** TX Power Level */
#define GATT_TX_POWER_LEVEL_CHARACTERISTIC                           0x2A07U

/** \} */

/**
 * \name Human Interface Device Service Characteristics Defines
 * \{
 * \brief This section describes the various Human Interface Device Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** Protocol Mode */
#define GATT_HID_PROTO_MODE_CHARACTERISTIC                           0x2A4EU
/** Report */
#define GATT_HID_RPT_CHARACTERISTIC                                  0x2A4DU
/* Report: Input Report Type - missing (in spec, but not in assigned numbers) */
/* Report: Output Report Type - missing (in spec, but not in assigned numbers) */
/** Report Map */
#define GATT_HID_RPT_MAP_CHARACTERISTIC                              0x2A4BU
/** Boot Keyboard Input Report */
#define GATT_BOOT_KYBRD_IN_RPT_CHARACTERISTIC                        0x2A22U
/** Boot Keyboard Output Report */
#define GATT_BOOT_KYBRD_OP_CHARACTERISTIC                            0x2A32U
/** Boot Mouse Input Report */
#define GATT_MOUSE_IP_CHARACTERISTIC                                 0x2A33U
/** HID Information */
#define GATT_HID_INFO_CHARACTERISTIC                                 0x2A4AU
/** HID Control Point */
#define GATT_HID_CP_CHARACTERISTIC                                   0x2A4CU

/** \} */

/**
 * \name Pulse Oximeter Service Characteristics Defines
 * \{
 * \brief This section describes the various Pulse Oximeter Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** PLX Spot-Check Measurement */
#define GATT_POX_SPOT_CHK_MSRMT_CHARACTERISTIC                       0x2A5EU
/** PLX Continuous Measurement */
#define GATT_POX_CONTINUOUS_MSRMT_CHARACTERISTIC                     0x2A5FU
/** PLX Features */
#define GATT_POX_FEATURES_CHARACTERISTIC                             0x2A60U

/** \} */

/**
 * \name Cycling Power Service Characteristics Defines
 * \{
 * \brief This section describes the various Cycling Power Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** Cycling Power Measurement */
#define GATT_CPM_MSRMT_CHARACTERISTIC                                0x2A63U
/** Cycling Power Vector */
#define GATT_CPM_VECTOR_CHARACTERISTIC                               0x2A64U
/** Cycling Power Feature */
#define GATT_CPM_FEATURE_CHARACTERISTIC                              0x2A65U
/** Cycling Power Control Point */
#define GATT_CPM_CNTRL_PNT_CHARACTERISTIC                            0x2A66U
/** Sensor Location */
#define GATT_CPM_SENSOR_LOCATION_CHARACTERISTIC                      0x2A5DU

/** \} */

/**
 * \name Location and Navigation Service Characteristics Defines
 * \{
 * \brief This section describes the various Location and Navigation Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** Location and Speed */
#define GATT_LOCATION_AND_SPEED_CHARACTERISTIC                       0x2A67U
/** Navigation */
#define GATT_NAVIGATION_CHARACTERISTIC                               0x2A68U
/** Position Quality */
#define GATT_LN_POSITION_QUALITY_CHARACTERISTIC                      0x2A69U
/** LN Feature */
#define GATT_LN_FEATURE_CHARACTERISTIC                               0x2A6AU
/** LN Control Point */
#define GATT_LN_CNTRL_PNT_CHARACTERISTIC                             0x2A6BU

/** \} */

/**
 * \name Continuous Glucose Monitoring Service Characteristics Defines
 * \{
 * \brief This section describes the various Continuous Glucose Monitoring
 * Service Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned
 * Numbers.
 */

/** CGM Measurement */
#define GATT_CGM_MSRMT_CHARACTERISTIC                                0x2AA7U
/** CGM Feature */
#define GATT_CGM_FEATURES_CHARACTERISTIC                             0x2AA8U
/** CGM Status */
#define GATT_CGM_STATUS_CHARACTERISTIC                               0x2AA9U
/** CGM Session Start Time */
#define GATT_CGM_SSN_STRT_TIME_CHARACTERISTIC                        0x2AAAU
/** CGM Session Run Time */
#define GATT_CGM_SSN_RUN_TIME_CHARACTERISTIC                         0x2AABU
/** CGM Specific Ops Control Point */
#define GATT_CGM_SPECIFIC_OPS_CP_CHARACTERISTIC                      0x2AACU

/** \} */

/**
 * \name Body Composition Service Characteristics Defines
 * \{
 * \brief This section describes the various Body Composition Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** Body Composition Feature */
#define GATT_BC_FEATURE_CHARACTERISTIC                               0x2A9BU
/** Body Composition Measurement */
#define GATT_BC_MSRMT_CHARACTERISTIC                                 0x2A9CU

/** \} */

/**
 * \name Weight Scale Service Characteristics Defines
 * \{
 * \brief This section describes the various Weight Scale Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** Weight Measurement */
#define GATT_WS_MSRMT_CHARACTERISTIC                                 0x2A9DU
/** Weight Scale Feature */
#define GATT_WS_FEATURE_CHARACTERISTIC                               0x2A9EU

/** \} */

/**
 * \name User Data Service Characteristics Defines
 * \{
 * \brief This section describes the various User Data Service Characteristics
 * 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 * Check 0x2A8AU - 0x2AA2U
 */

/** Database Change Increment */
#define GATT_UD_DB_CHNG_INC_CHARACTERISTIC                           0x2A99U
/** User Index */
#define GATT_UD_USER_INDEX_CHARACTERISTIC                            0x2A9AU
/** User Control Point */
#define GATT_UD_USER_CNTRL_PNT_CHARACTERISTIC                        0x2A9FU
/** Registered User */
#define GATT_UD_REGISTERED_USER_CHARACTERISTIC                       0x2B37U
/** First Name */
#define GATT_UD_FIRST_NAME_CHARACTERISTIC                            0x2A8AU
/** Last Name */
#define GATT_UD_LAST_NAME_CHARACTERISTIC                             0x2A90U
/** Email Address */
#define GATT_UD_EMAIL_CHARACTERISTIC                                 0x2A87U
/** Age */
#define GATT_UD_AGE_CHARACTERISTIC                                   0x2A80U
/** Gender */
#define GATT_UD_GENDER_CHARACTERISTIC                                0x2A8CU
/** Date of Birth */
#define GATT_UD_DOB_CHARACTERISTIC                                   0x2A85U
/** Weight */
#define GATT_UD_WEIGHT_CHARACTERISTIC                                0x2A98U
/** Height */
#define GATT_UD_HEIGHT_CHARACTERISTIC                                0x2A8EU
/** VO2 Max */
#define GATT_UD_VO2MAX_CHARACTERISTIC                                0x2A96U
/** Heart Rate Max */
#define GATT_UD_HRMAX_CHARACTERISTIC                                 0x2A8DU
/** Resting Heart Rate */
#define GATT_UD_RHR_CHARACTERISTIC                                   0x2A92U
/** Maximum Recommended Heart Rate */
#define GATT_UD_MAXRHR_CHARACTERISTIC                                0x2A91U
/** Aerobic Threshold */
#define GATT_UD_AEROTH_CHARACTERISTIC                                0x2A7FU
/** Anaerobic Threshold */
#define GATT_UD_ANAETH_CHARACTERISTIC                                0x2A83U
/** Sport Type for Aerobic and Anaerobic Threshold */
#define GATT_UD_SPORTTYPE_CHARACTERISTIC                             0x2A93U
/** Date of Threshold Assessment */
#define GATT_UD_DATETHASS_CHARACTERISTIC                             0x2A86U
/** Waist Circumference */
#define GATT_UD_WAISTCIR_CHARACTERISTIC                              0x2A97U
/** Hip Circumference */
#define GATT_UD_HIPCIR_CHARACTERISTIC                                0x2A8FU
/** Fat Burn Heart Rate Lower Limit */
#define GATT_UD_FATBURN_LOW_CHARACTERISTIC                           0x2A88U
/** Fat Burn Heart Rate Upper Limit */
#define GATT_UD_FATBURN_UPL_CHARACTERISTIC                           0x2A89U
/** Aerobic Heart Rate Lower Limit */
#define GATT_UD_AERO_LOW_CHARACTERISTIC                              0x2A7EU
/** Aerobic Heart Rate Upper Limit */
#define GATT_UD_AERO_UPL_CHARACTERISTIC                              0x2A84U
/** Anaerobic Heart Rate Lower Limit */
#define GATT_UD_ANAE_LOW_CHARACTERISTIC                              0x2A81U
/** Anaerobic Heart Rate Upper Limit */
#define GATT_UD_ANAE_UPL_CHARACTERISTIC                              0x2A82U
/** Five Zone Heart Rate Limits */
#define GATT_UD_FIVEZONE_HRL_CHARACTERISTIC                          0x2A8BU
/** Three Zone Heart Rate Limits */
#define GATT_UD_THREEZONE_HRL_CHARACTERISTIC                         0x2A94U
/** Two Zone Heart Rate Limits */
#define GATT_UD_TWOZONE_HRL_CHARACTERISTIC                           0x2A95U
/** Language */
#define GATT_UD_LANGUAGE_CHARACTERISTIC                              0x2AA2U
/** Middle Name */
#define GATT_UD_MIDDLE_NAME_CHARACTERISTIC                           0x2B48U
/** High Resolution Height */
#define GATT_UD_HIGH_RESOLN_HEIGHT_CHARACTERISTIC                    0x2B47U
/** Stride Length */
#define GATT_UD_STRIDE_LENGTH_CHARACTERISTIC                         0x2B49U
/** Handedness */
#define GATT_UD_HANDEDNESS_CHARACTERISTIC                            0x2B4AU
/** Device Wearing Position */
#define GATT_UD_DEV_WEARING_POS_CHARACTERISTIC                       0x2B4BU
/** Four Zone Heart Rate */
#define GATT_UD_4_ZONE_HR_LIMITS_CHARACTERISTIC                      0x2B4CU
/** High Intensity Exercise Threshold */
#define GATT_UD_HIGH_INTSTY_EX_THRESOLD_CHARACTERISTIC               0x2B4DU
/** Activity Goal */
#define GATT_UD_ACTIVITY_GOAL_CHARACTERISTIC                         0x2B4EU
/** Sedentary Interval Notification */
#define GATT_UD_SEDENTARY_INTVAL_NOTFY_CHARACTERISTIC                0x2B4FU
/** Caloric Intake */
#define GATT_UD_CALORIC_INTAKE_CHARACTERISTIC                        0x2B50U
/** Preferred Units */
#define GATT_UD_PREFERRED_UINTS_CHARACTERISTIC                       0x2B46U
/** \} */

/**
 * \name Physical Activity Monitor Service Characteristics Defines
 * \{
 * \brief This section describes the various Physical Activity Monitor Service
 * 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */
/** Physical Activity Monitor Features */
#define GATT_PAMS_FEATURE_CHARACTERISTIC                             0x2B3BU
/** General Activity Instantaneous Data */
#define GATT_PAMS_GEN_ACTVTY_INSTANOUS_DATA_CHARACTERISTIC           0x2B3CU
/** General Activity Summary Data */
#define GATT_PAMS_GEN_ACTVTY_SUMMARY_DATA_CHARACTERISTIC             0x2B3DU
/** CardioRespiratory Activity Instantaneous Data */
#define GATT_PAMS_CARDIO_RESPTORY_ACTVTY_IDATA_CHARACTERISTIC        0x2B3EU
/** CardioRespiratory Activity Summary Data */
#define GATT_PAMS_CARDIO_RESPTORY_ACTVTY_SDATA_CHARACTERISTIC        0x2B3FU
/** Step Counter Activity Summary Data */
#define GATT_PAMS_STEP_CUNTER_ACTVTY_SUMMARY_DATA_CHARACTERISTIC     0x2B40U
/** Sleep Activity Instantaneous Data */
#define GATT_PAMS_SLEEP_ACTVTY_INSTANOUS_DATA_CHARACTERISTIC         0x2B41U
/** Sleep Activity Summary Data */
#define GATT_PAMS_SLEEP_ACTVTY_SUMMARY_DATA_CHARACTERISTIC           0x2B42U
/** Physical Activity Monitor Control Point */
#define GATT_PAMS_CONTROL_POINT_CHARACTERISTIC                       0x2B43U
/** Current Session */
#define GATT_PAMS_CURRENT_SESSION_CHARACTERISTIC                     0x2B44U
/** Session */
#define GATT_PAMS_SESSION_DESCRIPTOR_CHARACTERISTIC                  0x2B45U

/** \} */

/**
 * \name Environmental Sensing Service Characteristics Defines
 * \{
 * \brief This section describes the various Environmental Sensing Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned.
 */

/** Descriptor Value Changed */
#define GATT_ESS_DVC_CHARACTERISTIC                                  0x2A7DU
/** Temperature */
#define GATT_ESS_TEMP_CHARACTERISTIC                                 0x2A6EU
/** Apparent Wind Direction */
#define GATT_ESS_APPARENT_WIND_DIRECTION_CHARACTERISTIC              0x2A73U
/** Pressure */
#define GATT_ESS_PRESSURE_CHARECTERISTIC                             0x2A6DU
/** Barometric Pressure Trend */
#define GATT_ESS_BIOMETRIC_PRESS_TREND_CHARACTERISTIC                0x2AA3U
/** Elevation */
#define GATT_ESS_ELEVATION_CHARACTERISTIC                            0x2A6CU
/** Humidity */
#define GATT_ESS_HUMIDITY_CHARACTERISTIC                             0x2A6FU
/** True Wind Speed */
#define GATT_ESS_TRUE_WIND_SPEED_CHARACTERISTIC                      0x2A70U
/** True Wind Direction */
#define GATT_ESS_TRUE_WIND_DIRECTION_CHARACTERISTIC                  0x2A71U
/** Apparent Wind Speed */
#define GATT_ESS_APPARENT_WIND_SPEED_CHARACTERISTIC                  0x2A72U
/** Guest Factor */
#define GATT_ESS_GUEST_FACTOR_CHARACTERISTIC                         0x2A74U
/** Pollen Concentration */
#define GATT_ESS_POLLEN_CONCENTRATION_CHARACTERISTIC                 0x2A75U
/** UV Index */
#define GATT_ESS_UV_INDEX_CHARACTERISTIC                             0x2A76U
/** Irradiance */
#define GATT_ESS_IRRADIANCE_CHARACTERISTIC                           0x2A77U
/** Rainfall */
#define GATT_ESS_RAINFALL_CHARACTERISTIC                             0x2A78U
/** Windchill */
#define GATT_ESS_WINDCHILL_CHARACTERISTIC                            0x2A79U
/** Heat Index */
#define GATT_ESS_HEAT_INDEX_CHARACTERISTIC                           0x2A7AU
/** Dew Point */
#define GATT_ESS_DEW_POINT_CHARACTERISTIC                            0x2A7BU
/** Magnetic Declination */
#define GATT_ESS_MAGNATIC_DECLINATION_CHARACTERISTIC                 0x2A2CU
/** Magnetic Flux Density 2D */
#define GATT_ESS_MAGNATIC_FLUX_DENSITY_2D_CHARACTERISTIC             0x2AA0U
/** Magnetic Flux Density 2D */
#define GATT_ESS_MAGNATIC_FLUX_DENSITY_3D_CHARACTERISTIC             0x2AA1U

/** \} */

/**
 * \name Bond Management Service Characteristics Defines
 * \{
 * \brief This section describes the various Bond Management Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** Bond Management Feature */
#define GATT_BM_FEATURES_CHARACTERISTIC                              0x2AA5U
/** Bond Management Control Point */
#define GATT_BM_CNTRL_PNT_CHARACTERISTIC                             0x2AA4U

/** \} */

/**
 * \name Automation IO Service Characteristics Defines
 * \{
 * \brief This section describes the various Automation IO Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** Digital - Check - (not in assigned numbers, but adopted in spec) */
#define GATT_AIO_DIGITAL_CHARACTERISTIC                              0x2A56U
/** Analog - Check - (not in assigned numbers, but adopted in spec) */
#define GATT_AIO_ANALOG_CHARACTERISTIC                               0x2A58U
/** Aggregate */
#define GATT_AIO_AGGREGATE_CHARACTERISTIC                            0x2A5AU

/** \} */

/**
 * \name Indoor Positioning Service Characteristics Defines
 * \{
 * \brief This section describes the various Indoor Positioning Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** Indoor Positioning Configuration */
#define GATT_IP_INDOOR_POS_CONFIG_CHARACTERISTIC                     0x2AADU
/** Latitude */
#define GATT_IP_LATITUDE_CHARACTERISTIC                              0x2AAEU
/** Longitude */
#define GATT_IP_LONGITUDE_CHARACTERISTIC                             0x2AAFU
/** Local North Coordinate */
#define GATT_IP_LOCAL_NORTH_CHARACTERISTIC                           0x2AB0U
/** Local East Coordinate */
#define GATT_IP_LOCAL_EAST_CHARACTERISTIC                            0x2AB1U
/** Floor Number */
#define GATT_IP_FLOOR_NUM_CHARACTERISTIC                             0x2AB2U
/** Altitude */
#define GATT_IP_ALTITUDE_CHARACTERISTIC                              0x2AB3U
/** Uncertainty */
#define GATT_IP_UNCERTAINTY_CHARACTERISTIC                           0x2AB4U
/** Location Name */
#define GATT_IP_LOCAL_NAME_CHARACTERISTIC                            0x2AB5U

/** \} */

/**
 * \name Object Transfer Service Characteristics Defines
 * \{
 * \brief This section describes the various Object Transfer Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** OTS Feature */
#define GATT_OTS_FEATURE_CHARACTERISTIC                              0x2ABDU
/** object name */
#define GATT_OTS_OBJ_NAME_CHARACTERISTIC                             0x2ABEU
/** object type */
#define GATT_OTS_OBJ_TYPE_CHARACTERISTIC                             0x2ABFU
/** object size */
#define GATT_OTS_OBJ_SIZE_CHARACTERISTIC                             0x2AC0U
/** object first created */
#define GATT_OTS_OBJ_FIRST_CRTD_CHARACTERISTIC                       0x2AC1U
/** object last modified */
#define GATT_OTS_OBJ_LAST_MODFD_CHARACTERISTIC                       0x2AC2U
/** object ID */
#define GATT_OTS_OBJ_ID_CHARACTERISTIC                               0x2AC3U
/** object properties */
#define GATT_OTS_OBJ_PRPTY_CHARACTERISTIC                            0x2AC4U
/** object action control point */
#define GATT_OTS_OACP_CHARACTERISTIC                                 0x2AC5U
/** object list control point */
#define GATT_OTS_OLCP_CHARACTERISTIC                                 0x2AC6U
/** object list filter */
#define GATT_OTS_OBJ_LIST_FILTER_CHARACTERISTIC                      0x2AC7U
/** object changed */
#define GATT_OTS_OBJ_CHANGED_CHARACTERISTIC                          0x2AC8U
/** Directory Listing */
#define GATT_OTS_DIRECTORY_LISTING_CHARACTERISTIC                    0x2ACBU
/** \} */

/**
 * \name Transport Discovery Service Characteristics Defines
 * \{
 * \brief This section describes the various Transport Discovery Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** TDS Control Point */
#define GATT_TDS_CONTROL_POINT_CHARACTERISTIC                        0x2ABCU
/** TDS BREDR Handover Data */
#define GATT_TDS_BREDR_HANDOVER_DATA_CHARACTERISTIC                  0x2B38U
/** TDS Bluetooth SIG Data */
#define GATT_TDS_BLUETOOTH_SIG_DATA_CHARACTERISTIC                   0x2B39U

/** \} */

/**
 * \name HTTP Proxy Service Characteristics Defines
 * \{
 * \brief This section describes the various HTTP Proxy Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** URI */
#define GATT_HPC_URI_CHARACTERISTIC                                  0x2AB6U
/** HTTP Headers */
#define GATT_HPC_HTTP_HEADERS_CHARACTERISTIC                         0x2AB7U
/** HTTP Status Code */
#define GATT_HPC_HTTP_STATUS_CODE_CHARACTERISTIC                     0x2AB8U
/** HTTP Entity Body */
#define GATT_HPC_HTTP_ENTITY_BODY_CHARACTERISTIC                     0x2AB9U
/** HTTP Control Point */
#define GATT_HPC_HTTP_CP_CHARACTERISTIC                              0x2ABAU
/** HTTPS Security */
#define GATT_HPC_HTTPS_SECURITY_CHARACTERISTIC                       0x2ABBU

/** \} */

/**
 * \name Fitness Machine Service Characteristics Defines
 * \{
 * \brief This section describes the various Fitness Machine Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** Fitness Machine Feature */
#define GATT_FTM_FEATURE_CHARACTERISTIC                              0x2ACCU
/** Treadmill Data */
#define GATT_FTM_TREADMILL_CHARACTERISTIC                            0x2ACDU
/** Cross Trainer Data */
#define GATT_FTM_CROSS_TRAINER_CHARACTERISTIC                        0x2ACEU
/** Step Climber Data */
#define GATT_FTM_STEP_CLIMBER_CHARACTERISTIC                         0x2ACFU
/** Stair Climber Data */
#define GATT_FTM_STAIR_CLIMBER_CHARACTERISTIC                        0x2AD0U
/** Rower Data */
#define GATT_FTM_ROWER_DATA_CHARACTERISTIC                           0x2AD1U
/** Indoor Bike Data */
#define GATT_FTM_INDOOR_BIKE_CHARACTERISTIC                          0x2AD2U
/** Training Status */
#define GATT_FTM_TRAINING_STATUS_CHARACTERISTIC                      0x2AD3U
/** Supported Speed Range */
#define GATT_FTM_SUPPD_SPEED_RNG_CHARACTERISTIC                      0x2AD4U
/** Supported Inclination Range */
#define GATT_FTM_SUPPD_INCLN_RNG_CHARACTERISTIC                      0x2AD5U
/** Supported Resistance Level Range */
#define GATT_FTM_SUPPD_RESIS_RNG_CHARACTERISTIC                      0x2AD6U
/** Supported Power Range */
#define GATT_FTM_SUPPD_POWER_RNG_CHARACTERISTIC                      0x2AD8U
/** Supported Heart Rate Range */
#define GATT_FTM_SUPPD_HRTRE_RNG_CHARACTERISTIC                      0x2AD7U
/** Fitness Machine Control Point */
#define GATT_FTM_CONTROL_POINT_CHARACTERISTIC                        0x2AD9U
/** Fitness Machine Status */
#define GATT_FTM_MACHINE_STATUS_CHARACTERISTIC                       0x2ADAU

/** \} */

/**
 * \name Reconnection Configuration Service Characteristics Defines
 * \{
 * \brief This section describes the various Reconnection Configuration Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** RC Feature */
#define GATT_RC_FEATURE_CHARACTERISTIC                               0x2B1DU
/** RC Settings */
#define GATT_RC_SETTING_CHARACTERISTIC                               0x2B1EU
/** Reconnection Configuration Control Point */
#define GATT_RC_CP_CHARACTERISTIC                                    0x2B1FU

/** \} */

/**
 * \name Insulin Delivery Service Characteristics Defines
 * \{
 * \brief This section describes the various Insulin Delivery Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** IDD Status Changed */
#define GATT_IDD_STATUS_CHANGED_CHARACTERISTIC                       0x2B20U
/** IDD Status */
#define GATT_IDD_STATUS_CHARACTERISTIC                               0x2B21U
/** IDD Annunciation Status */
#define GATT_IDD_ANNUN_STATUS_CHARACTERISTIC                         0x2B22U
/** IDD Features */
#define GATT_IDD_FEATURE_CHARACTERISTIC                              0x2B23U
/** IDD Status Reader Control Point */
#define GATT_IDD_STATUS_READER_CP_CHARACTERISTIC                     0x2B24U
/** IDD Command Control Point */
#define GATT_IDD_COMMAND_CP_CHARACTERISTIC                           0x2B25U
/** IDD Command Data */
#define GATT_IDD_COMMAND_DATA_CHARACTERISTIC                         0x2B26U
/** IDD Record Access Control Point */
#define GATT_IDD_RACP_CHARACTERISTIC                                 0x2B27U
/** IDD History Data */
#define GATT_IDD_HISTORY_DATA_CHARACTERISTIC                         0x2B28U

/** \} */

/**
 * \name Binary Sensor Service Characteristics Defines
 * \{
 * \brief This section describes the various Binary Service Characteristics
 * 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** Binary Sensor Control Point */
#define GATT_BSS_CP_CHARACTERISTIC                                   0x2B2BU
/** Binary Sensor Response */
#define GATT_BSS_RESPONSE_CHARACTERISTIC                             0x2B2CU

/** \} */

/**
 * \name Emergency Configuration Service Characteristics Defines
 * \{
 * \brief This section describes the various Emergency Configuration Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** Emergency ID */
#define GATT_EMERGENCY_ID_CHARACTERISTIC                             0x2B2DU
/** Emergency Text */
#define GATT_EMERGENCY_TEXT_CHARACTERISTIC                           0x2B2EU

/** \} */

/**
 * \name Device Time Service Characteristics Defines
 * \{
 * \brief This section describes the various Device Time Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */
/** Device Time Feature */
#define GATT_DTS_DT_FEATURE_CHARACTERISTIC                           0x2B8EU
/** Device Time  Parameters */
#define GATT_DTS_DT_PARAMETERS_CHARACTERISTIC                        0X2B8FU
/** Device Time */
#define GATT_DTS_DT_CHARACTERISTIC                                   0X2B90U
/** Device Time Control Point */
#define GATT_DTS_DT_CONTROL_POINT_CHARACTERISTIC                     0X2B91U
/** Device Time Time Change Log Data */
#define GATT_DTS_TIME_CHANGE_LOG_DATA_CHARACTERISTIC                 0X2B92U

/** \} */

/**
 * \name Volume Control Service Characteristics Defines
 * \{
 * \brief This section describes the various Volume Control Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** Volume State */
#define GATT_VC_VOLUME_STATE_CHARACTERISTIC                          0x2B7DU
/** Volume Control Point */
#define GATT_VC_VOLUME_CP_CHARACTERISTIC                             0x2B7EU
/** Volume Flags */
#define GATT_VC_VOLUME_FLAG_CHARACTERISTIC                           0x2B7FU

/** \} */

/**
 * \name Volume Offset Control Service Characteristics Defines
 * \{
 * \brief This section describes the various Volume Offset Control Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** Offset State */
#define GATT_VOC_OFFSET_STATE_CHARACTERISTIC                         0x2B80U
/** Audio Location */
#define GATT_VOC_AUDIO_LOCATION_CHARACTERISTIC                       0x2B81U
/** Volume Offset Control Point */
#define GATT_VOC_OFFSET_CP_CHARACTERISTIC                            0x2B82U
/** Audio Output Description */
#define GATT_VOC_AUDIO_OP_DESC_CHARACTERISTIC                        0x2B83U

/** \} */

/**
 * \name Audio Input Control Service Characteristics Defines
 * \{
 * \brief This section describes the various Audio Input Control Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** Audio Input State */
#define GATT_AIC_INPUT_STATE_CHARACTERISTIC                          0x2B77U
/** Gain Settings Attribute */
#define GATT_AIC_GAIN_SETTING_CHARACTERISTIC                         0x2B78U
/** Audio Input Type */
#define GATT_AIC_INPUT_TYPE_CHARACTERISTIC                           0x2B79U
/** Audio Input Status */
#define GATT_AIC_INPUT_STATUS_CHARACTERISTIC                         0x2B7AU
/** Audio Input Control Point */
#define GATT_AIC_AUDIO_INPUT_CP_CHARACTERISTIC                       0x2B7BU
/** Audio Input Description */
#define GATT_AIC_AUDIO_INPUT_DESC_CHARACTERISTIC                     0x2B7CU

/** \} */

/**
 * \name Microphone Control Service Characteristics Defines
 * \{
 * \brief This section describes the various Microphone Control Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** Mute */
#define GATT_MIC_MUTE_CHARACTERISTIC                                 0x2BC3U

/** \} */

/**
 * \name Published Audio Capabilities Service Characteristics Defines
 * \{
 * \brief This section describes the various Published Audio Capabilities
 * Service Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned
 * Numbers.
 */

/** Sink PAC */
#define GATT_PAC_SNK_PAC_CHARACTERISTIC                              0x2BC9U
/** Sink Audio Locations */
#define GATT_PAC_SNK_AUDIO_LOC_CHARACTERISTIC                        0x2BCAU
/** Source PAC */
#define GATT_PAC_SRC_PAC_CHARACTERISTIC                              0x2BCBU
/** Source Audio Locations */
#define GATT_PAC_SRC_AUDIO_LOC_CHARACTERISTIC                        0x2BCCU
/** Available Audio Contexts */
#define GATT_PAC_AVBL_AUDIO_CONTEXTS_CHARACTERISITC                  0x2BCDU
/** Supported Audio Contexts */
#define GATT_PAC_SUPP_AUDIO_CONTEXTS_CHARACTERISTIC                  0x2BCEU

/** \} */

/**
 * \name Telephone Bearer Service Characteristics Defines
 * \{
 * \brief This section describes the various Telephone Bearer Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** Bearer Provider Name */
#define GATT_TBS_BRR_PROVIDER_NAME_CHARACTERISTIC                    0x2BB3U
/** Bearer UCI */
#define GATT_TBS_BRR_UCI_CHARACTERISTIC                              0x2BB4U
/** Bearer Technology */
#define GATT_TBS_BRR_TECHNOLOGY_CHARACTERISTIC                       0x2BB5U
/** Bearer URI Schemes Supported List */
#define GATT_TBS_BRR_URI_SCHEMES_SUPPORTED_LIST_CHARACTERISTIC       0x2BB6U
/** Bearer Signal Strength */
#define GATT_TBS_BRR_SIGSTRENGTH_CHARACTERISTIC                      0x2BB7U
/** Bearer Signal Strength Reporting Interval */
#define GATT_TBS_BRR_SIGSTRENGTH_REPORTING_INTERVAL_CHARACTERISTIC   0x2BB8U
/** Bearer List Current Calls */
#define GATT_TBS_BRR_LIST_CUR_CALLS_CHARACTERISTIC                   0x2BB9U
/** Content Control ID */
#define GATT_CONTENT_CONTROL_ID_CHARACTERISTIC                       0x2BBAU
/** Status Flags */
#define GATT_TBS_STATUS_FLAGS_CHARACTERISTIC                         0x2BBBU
/** Incoming Call Target Bearer URI */
#define GATT_TBS_INCOMING_CALL_TARGET_BRR_URI_CHARACTERISTIC         0x2BBCU
/** Call State */
#define GATT_TBS_CALL_STATE_CHARACTERISTIC                           0x2BBDU
/** Call Control Point */
#define GATT_TBS_CALL_CP_CHARACTERISTIC                              0x2BBEU
/** Call Control Point Optional Opcodes */
#define GATT_TBS_CALL_CP_OPTIONAL_OPCODES_CHARACTERISTIC             0x2BBFU
/** Termination Reason */
#define GATT_TBS_TERMINATION_REASON_CHARACTERISTIC                   0x2BC0U
/** Incoming Call */
#define GATT_TBS_INCOMING_CALL_CHARACTERISTIC                        0x2BC1U
/** Call Friendly Name */
#define GATT_TBS_CALL_FRIENDLY_NAME_CHARACTERISTIC                   0x2BC2U

/** \} */

/**
 * \name Coordinated Set Identification Service Characteristics Defines
 * \{
 * \brief This section describes the various Coordinated Set Identification
 * Service Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned
 * Numbers.
 */

/** Set Identity Resolving Key Characteristic */
#define GATT_CSI_SIRK_CHARACTERISTIC                                 0x2B84U
/** Size Characteristic */
#define GATT_CSI_SIZE_CHARACTERISITC                                 0x2B85U
/** Lock Characteristic */
#define GATT_CSI_LOCK_CHARACTERISTIC                                 0x2B86U
/** Rank Characteristic */
#define GATT_CSI_RANK_CHARACTERISTIC                                 0x2B87U

/** \} */

/**
 * \name Media Control Service Characteristics Defines
 * \{
 * \brief This section describes the various Media Control Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** Media Player Name */
#define GATT_MCS_MEDIA_PLAYER_NAME_CHARACTERISTIC                    0x2B93U
/** Media Player Icon Object ID */
#define GATT_MCS_MEDIA_PLAYER_ICON_OBJ_ID_CHARACTERISTIC             0x2B94U
/** Media Player Icon URL */
#define GATT_MCS_MEDIA_PLAYER_ICON_URL_CHARACTERISTIC                0x2B95U
/** Track Changed */
#define GATT_MCS_TRACK_CHGD_CHARACTERISTIC                           0x2B96U
/** Track Title */
#define GATT_MCS_TRACK_TITLE_CHARACTERISTIC                          0x2B97U
/** Track Duration */
#define GATT_MCS_TRACK_DURATION_CHARACTERISTIC                       0x2B98U
/** Track Position */
#define GATT_MCS_TRACK_POS_CHARACTERISTIC                            0x2B99U
/** Playback Speed */
#define GATT_MCS_PLAYBACK_SPEED_CHARACTERISTIC                       0x2B9AU
/** Seeking Speed */
#define GATT_MCS_SEEKING_SPEED_CHARACTERISTIC                        0x2B9BU
/** Current Track Segments Object ID */
#define GATT_MCS_CURR_TRACK_SEG_OBJ_ID_CHARACTERISTIC                0x2B9CU
/** Current Track Object ID */
#define GATT_MCS_CURR_TRACK_OBJ_ID_CHARACTERISTIC                    0x2B9DU
/** Next Track Object ID */
#define GATT_MCS_NEXT_TRACK_OBJ_ID_CHARACTERISTIC                    0x2B9EU
/** Parent Group Object ID */
#define GATT_MCS_PARENT_GRP_OBJ_ID_CHARACTERISTIC                    0x2B9FU
/** Current Group Object ID */
#define GATT_MCS_CURR_GROUP_OBJ_ID_CHARACTERISTIC                    0x2BA0U
/** Playing Order */
#define GATT_MCS_PLAYING_ORDER_CHARACTERISTIC                        0x2BA1U
/** Playing Orders Supported */
#define GATT_MCS_PLAYING_ORDERS_SUPP_CHARACTERISTIC                  0x2BA2U
/** Media State */
#define GATT_MCS_MEDIA_STATE_CHARACTERISTIC                          0x2BA3U
/** Media Control Point */
#define GATT_MCS_MEDIA_CONTROL_POINT_CHARACTERISTIC                  0x2BA4U
/** Media Control Point Opcodes Supported */
#define GATT_MCS_MCP_OPC_SUPP_CHARACTERISTIC                         0x2BA5U
/** Search Results Object ID */
#define GATT_MCS_SEARCH_RES_OBJ_ID_CHARACTERISTIC                    0x2BA6U
/** Search Control Point */
#define GATT_MCS_SEARCH_CONTROL_POINT_CHARACTERISTIC                 0x2BA7U

/** \} */

/**
 * \name Audio Stream Control Service Characteristics Defines
 * \{
 * \brief This section describes the various Audio Stream Control Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** Sink ASE */
#define GATT_ASCS_SINK_ASE_CHARACTERISTIC                            0x2BC4U
/** Source ASE */
#define GATT_ASCS_SOURCE_ASE_CHARACTERISTIC                          0x2BC5U
/** ASE Control Point */
#define GATT_ASCS_ASE_CP_CHARACTERISTIC                              0x2BC6U

/** \} */

/**
 * \name Broadcast Audio Scan Service Characteristics Defines
 * \{
 * \brief This section describes the various Audio Broadcast Audio Scan Service
 * Characteristics 16-bit UUIDs as defined in the Bluetooth Assigned Numbers.
 */

/** Broadcast Audio Scan Control Point */
#define GATT_BASS_BCAST_AUDIO_SCAN_CP_CHARACTERISTIC                 0x2BC7U
/** Broadcast Receive State */
#define GATT_BASS_BCAST_RECEIVE_STATE_CHARACTERISTIC                 0x2BC8U

/** \} */

/**
 * \name Error Code Defines
 * \{
 * \brief This section describes the various Error Codes.
 */

/** Procedure Already in Progress */
#define ATT_APPL_PROCEDURE_ALREADY_IN_PROGRESS                       0x80U
/** CCD Improperly Configured */
#define ATT_APPL_CCD_IMPROPERLY_CONFIGURED                           0x81U

/** \} */

/* GATT Units */

/** \} */

/** \} */

/** \} */

#endif /* _H_GATT_DEFINES_ */

