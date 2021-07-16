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

/* GATT Defined Attributes */
#define GATT_PRIMARY_SERVICE            0x2800U
#define GATT_SECONDARY_SERVICE          0x2801U
#define GATT_INCLUDE                    0x2802U
#define GATT_CHARACTERISTIC             0x2803U
#define GATT_EXTENDED_PROPERTIES        0x2900U
#define GATT_USER_DESCRIPTION           0x2901U
#define GATT_CLIENT_CONFIG              0x2902U
#define GATT_SERVER_CONFIG              0x2903U
#define GATT_FORMAT                     0x2904U
#define GATT_AGGREGATE_FORMAT           0x2905U
#define GATT_VALID_RANGE                0x2906U
#define GATT_EXTERNAL_REPORT_REF        0x2907U
#define GATT_IP_OP_FEATURE_REPORT_REF   0x2908U

/** GATT Characteristic Properties Bit Field */

/** Broadcast */
#define GATT_CH_PROP_BIT_BROADCAST         0x01U

/** Read */
#define GATT_CH_PROP_BIT_READ              0x02U

/** Write Without Response */
#define GATT_CH_PROP_BIT_WRITE_WO_RSP      0x04U

/** Write */
#define GATT_CH_PROP_BIT_WRITE             0x08U

/** Notify */
#define GATT_CH_PROP_BIT_NOTIFY            0x10U

/** Indicate */
#define GATT_CH_PROP_BIT_INDICATE          0x20U

/** Authenticated Signed Write */
#define GATT_CH_PROP_BIT_SIGN_WRITE        0x40U

/** Extended Properties */
#define GATT_CH_PROP_BIT_EXT_PROPERTY      0x80U


/* GATT Client Configuration values */
#define GATT_CLI_CNFG_NOTIFICATION         0x0001U
#define GATT_CLI_CNFG_INDICATION           0x0002U
#define GATT_CLI_CNFG_DEFAULT              0x0000U

/* GATT Server Configuration values */
#define GATT_SER_CNFG_BROADCAST            0x0001U
#define GATT_SER_CNFG_DEFAULT              0x0000U

/* GATT Services */
#define GATT_GAP_SERVICE                       0x1800U
#define GATT_GATT_SERVICE                      0x1801U
#define GATT_IMMEDIATE_ALERT_SERVICE           0x1802U
#define GATT_LINK_LOSS_SERVICE                 0x1803U
#define GATT_TX_POWER_SERVICE                  0x1804U
#define GATT_CURRENT_TIME_SERVICE              0x1805U
#define GATT_REF_TIME_UPDATE_SERVICE           0x1806U
#define GATT_NEXT_DST_CHANGE_SERVICE           0x1807U
#define GATT_GLUCOSE_SERVICE                   0x1808U
#define GATT_HEALTH_THERMOMETER_SERVICE        0x1809U
#define GATT_DEVICE_INFO_SERVICE               0x180AU
#define GATT_NWA_SERVICE                       0x180BU
#define GATT_WATCH_DOG_SERVICE                 0x180CU
#define GATT_HEART_RATE_SERVICE                0x180DU
#define GATT_PHONE_ALERT_STATUS_SERVICE        0x180EU
#define GATT_BATTERY_SERVICE                   0x180FU
#define GATT_BLOOD_PRESSURE_SERVICE            0x1810U
#define GATT_ALERT_NOTIFICATION_SERVICE        0x1811U
#define GATT_HID_SERVICE                       0x1812U
#define GATT_SCAN_PARAM_SERVICE                0x1813U
#define GATT_RUNNING_SPEED_AND_CADENCE_SERVICE 0x1814U
#define GATT_AIOS_SERVICE                      0x1815U
#define GATT_CYCLING_SPEED_AND_CADENCE_SERVICE 0x1816U
#define GATT_CPM_SERVICE                       0x1818U
#define GATT_LOCATION_AND_NAVIGATION_SERVICE   0x1819U
#define GATT_ENVIRONMENTAL_SENSING_SERVICE     0x181AU
#define GATT_BODY_COMPOSITION_SERVICE          0x181BU
#define GATT_USER_DATA_SERVICE                 0x181CU
#define GATT_WEIGHT_SCALE_SERVICE              0x181DU
#define GATT_BM_SERVICE                        0x181EU
#define GATT_CGM_SERVICE                       0x181FU
#define GATT_INT_PS_SERVICE                    0x1820U
#define GATT_IPS_SERVICE                       0x1821U
#define GATT_PULSE_OXIMETER_SERVICE            0x1822U
#define GATT_HPS_SERVICE                       0x1823U
#define GATT_TDS_SERVICE                       0x1824U
#define GATT_OBJECT_TRANSFER_SERVICE           0x1825U
#define GATT_FTMS_SERVICE                      0x1826U
#define GATT_RCS_SERVICE                       0x1829U
#define GATT_INSULIN_DELIVERY_SERVICE          0x183AU

/** Service UUID defines of GA profiles/Services Under Development */
/**
 * Volume Control Service                       0x8FD1
 * Volume Offset Control Service                0x8FD2
 * Auido Input Control Service                  0x8FD3
 * Microphone Control Service                   0x8FD4
 * Telephone Bearer Service                     0x8FD5
 * Routing Active Audio Service                 0x8FD6
 * Media Control Service                        0x8FD7
 * Coordinated Set Identification               0x8FD8
 * Published Audio Capability Service           0x8FD9
 * Audio Stream Control Service                 0x8FDA
 * Broadcast Audio Scan Service                 0x8FDB
 * Basic Audio Announcement Service             0x8FDC
 * Broadcast Audio Announcement Service         0x9FDD
 * Generic Media Control Service                0x8FDE
 */
#define GATT_VCS_SERVICE                       0x8FD1U
#define GATT_VOCS_SERVICE                      0x8FD2U
#define GATT_AICS_SERVICE                      0x8FD3U
#define GATT_MICS_SERVICE                      0x8FD4U
#define GATT_TBS_SERVICE                       0x8FD5U
#define GATT_RAAS_SERVICE                      0x8FD6U
#define GATT_MCS_SERVICE                       0x8FD7U
#define GATT_CSIS_SERVICE                      0x8FD8U
#define GATT_ACS_SERVICE                       0x8FD9U
#define GATT_ASCS_SERVICE                      0x8FDAU
#define GATT_BASS_SERVICE                      0x8FDBU
#define GATT_BAAS_SERVICE                      0x8FDCU
#define GATT_BRAAS_SERVICE                     0x8FDDU
#define GATT_GMCS_SERVICE                      0x8FDEU

/* MCS/GMCS OTS Service */
/**
 * Track Segments                               0x8F90
 * Track                                        0x8F91
 * Group                                        0x8F92
 */
#define GATT_MCS_OTS_OBJ_T_TS                   0x8F90U
#define GATT_MCS_OTS_OBJ_T_T                    0x8F91U
#define GATT_MCS_OTS_OBJ_T_G                    0x8F92U

/* Dont find this on .org */
#define GATT_FIND_ME_SERVICE                   0x18A3U

/* Still not adopted, may need updation */
#define GATT_TPMS_SERVICE                      0x7FD0U

/* GATT Characteristic */
#define GATT_DEVICE_NAME_CHARACTERISTIC          0x2A00U
#define GATT_APPEARANCE_CHARACTERISTIC           0x2A01U
#define GATT_PERIPHRL_PRIVCY_FLG_CHARACTERISTIC  0x2A02U
#define GATT_RECONNECT_ADDR_CHARACTERISTIC       0x2A03U
#define GATT_PRFRRD_CNXN_PARAM_CHARACTERISTIC    0x2A04U
#define GATT_SERVICE_CHANGED_CHARACTERISTIC      0x2A05U
#define GATT_ALERT_LEVEL_CHARACTERISTIC          0x2A06U
#define GATT_TX_POWER_LEVEL_CHARACTERISTIC       0x2A07U
#define GATT_DATE_TIME_CHARACTERISTIC            0x2A08U
#define GATT_DAY_OF_WEEK_CHARACTERISTIC          0x2A09U
#define GATT_DAY_DATE_TIME_CHARACTERISTIC        0x2A0AU
#define GATT_EXACT_TIME_100_CHARACTERISTIC       0x2A0BU
#define GATT_EXACT_TIME_256_CHARACTERISTIC       0x2A0CU
#define GATT_DST_OFFSET_CHARACTERISTIC           0x2A0DU
#define GATT_TIME_ZONE_CHARACTERISTIC            0x2A0EU
#define GATT_LOCAL_TIME_INFO_CHARACTERISTIC      0x2A0FU
#define GATT_SEC_TIME_ZONE_CHARACTERISTIC        0x2A10U
#define GATT_TIME_WITH_DST_CHARACTERISTIC        0x2A11U
#define GATT_TIME_ACCURACY_CHARACTERISTIC        0x2A12U
#define GATT_TIME_SOURCE_CHARACTERISTIC          0x2A13U
#define GATT_REF_TIME_INFO_CHARACTERISTIC        0x2A14U
#define GATT_TIME_BROADCAST_CHARACTERISTIC       0x2A15U
#define GATT_TIME_UPDATE_CONTROL_POINT           0x2A16U
#define GATT_TIME_UPDATE_STATE_CHARACTERISTIC    0x2A17U
#define GATT_GLUCOSE_MSRMNT_CHARACTERISTIC       0x2A18U
#define GATT_BATTERY_LEVEL_CHARACTERISTIC        0x2A19U
#define GATT_BATTERY_PWR_ST_CHARCTERISTIC        0x2A1AU
#define GATT_BATTERY_LEVEL_ST_CHARACTERISTIC     0x2A1BU
#define GATT_TEMPERATURE_MSMNT_CHARACTERISTIC    0x2A1CU
#define GATT_TEMPERATURE_TYPE_CHARACTERISTIC     0x2A1DU
#define GATT_INTERMEDIATE_TEMP_CHARACTERISTIC    0x2A1EU
#define GATT_MSMNT_INTERVAL_CHARATACTERISTIC     0x2A21U
#define GATT_BOOT_KYBRD_IN_RPT_CHARACTERISTIC    0x2A22U
#define GATT_SYSTEM_ID_CHARACTERISTIC            0x2A23U
#define GATT_MODEL_NUMBER_CHARACTERISTIC         0x2A24U
#define GATT_SERIAL_NUMBER_CHARACTERISTIC        0x2A25U
#define GATT_FIRMWARE_REV_CHARACTERISTIC         0x2A26U
#define GATT_HARDWARE_REV_CHARACTERISTIC         0x2A27U
#define GATT_SOFTWARE_REV_CHARACTERISTIC         0x2A28U
#define GATT_MANUFACTURER_NAME_CHARACTERISTIC    0x2A29U
#define GATT_REG_CERT_DATA_CHARACTERISTIC        0x2A2AU
#define GATT_CURRENT_TIME_CHARACTERISTIC         0x2A2BU
#define GATT_SCAN_REFRESH_CHARACTERISTIC         0x2A31U
#define GATT_BOOT_KYBRD_OP_CHARACTERISTIC        0x2A32U
#define GATT_MOUSE_IP_CHARACTERISTIC             0x2A33U
#define GATT_GL_MSRMT_CTX_CHARACTERISTIC         0x2A34U
#define GATT_BP_MSRMT_CHARACTERISTIC             0x2A35U
#define GATT_INTRMDT_CUFF_PRSR_CHARACTERISTIC    0x2A36U
#define GATT_HR_MSRMT_CHARACTERISTIC             0x2A37U
#define GATT_BODY_SENSOR_LOC_CHARACTERISTIC      0x2A38U
#define GATT_HR_CNTRL_POINT                      0x2A39U
#define GATT_NWA_CHARACTERISTIC                  0x2A3EU
#define GATT_ALERT_STATUS_CHARACTERISTIC         0x2A3FU
#define GATT_RINGER_CONTROL_POINT_CHARACTERISTIC 0x2A40U
#define GATT_RINGER_SETTING_CHARACTERISTIC       0x2A41U
#define GATT_ALERT_ID_BIT_MASK_CHARACTERISTIC    0x2A42U
#define GATT_ALERT_ID_CHARACTERISTIC             0x2A43U
#define GATT_ALERT_NTF_CONTROL_POINT             0x2A44U
#define GATT_UNREAD_ALRT_STATUS_CHARACTERISTIC   0x2A45U
#define GATT_NEW_ALERT_CHARACTERISTIC            0x2A46U
#define GATT_SPRTD_NEW_ALRT_CTGRY_CHARACTERISTIC 0x2A47U
#define GATT_SPRT_UNRD_ALRT_CTGRY_CHARACTERISTIC 0x2A48U
#define GATT_BP_FEATURE_CHARACTERISTIC           0x2A49U
#define GATT_HID_INFO_CHARACTERISTIC             0x2A4AU
#define GATT_HID_RPT_MAP_CHARACTERISTIC          0x2A4BU
#define GATT_HID_CP_CHARACTERISTIC               0x2A4CU
#define GATT_HID_RPT_CHARACTERISTIC              0x2A4DU
#define GATT_HID_PROTO_MODE_CHARACTERISTIC       0x2A4EU
#define GATT_SCAN_WINDOW_CHARACTERISTIC          0x2A4FU
#define GATT_PNP_ID_CHARACTERISTIC               0x2A50U
#define GATT_GL_FEATURE_CHARACTERISTIC           0x2A51U
#define GATT_RACP_CHARACTERISTIC                 0x2A52U
#define GATT_RSC_MSRMT_CHARACTERISTIC            0x2A53U
#define GATT_RSC_FEATURE_CHARACTERISTIC          0x2A54U
#define GATT_SC_CONTROL_POINT_CHARACTERISTIC     0x2A55U
#define GATT_CSC_MSRMT_CHARACTERISTIC            0x2A5BU
#define GATT_CSC_FEATURE_CHARACTERISTIC          0x2A5CU
#define GATT_SENSOR_LOCATION_CHARACTERISTIC      0x2A5DU

/* Still Not Adopted, may need updation */
/* Pulse Oximeter Characteristics defines */
#define GATT_POX_SPOT_CHK_MSRMT_CHARACTERISTIC   0x2A5EU
#define GATT_POX_CONTINUOUS_MSRMT_CHARACTERISTIC 0x2A5FU
#define GATT_POX_PULSATILE_EVENT_CHARACTERISTIC  0x2A61U
#define GATT_POX_FEATURES_CHARACTERISTIC         0x2A60U
#define GATT_POX_CONTROL_POINT_CHARACTERISTIC    0x2A62U

/* CPM Characteristics Measurments */
#define GATT_CPM_MSRMT_CHARACTERISTIC            0x2A63U
#define GATT_CPM_VECTOR_CHARACTERISTIC           0x2A64U
#define GATT_CPM_FEATURE_CHARACTERISTIC          0x2A65U
#define GATT_CPM_CNTRL_PNT_CHARACTERISTIC        0x2A66U
#define GATT_CPM_SENSOR_LOCATION_CHARACTERISTIC  0x2A5DU

/* Location and Navigation Characteristics */
#define GATT_LOCATION_AND_SPEED_CHARACTERISTIC   0x2A67U
#define GATT_NAVIGATION_CHARACTERISTIC           0x2A68U
#define GATT_LN_POSITION_QUALITY_CHARACTERISTIC  0x2A69U
#define GATT_LN_FEATURE_CHARACTERISTIC           0x2A6AU
#define GATT_LN_CNTRL_PNT_CHARACTERISTIC         0x2A6BU

/* CGM Characteristics defines */
#define GATT_CGM_MSRMT_CHARACTERISTIC            0x2AA7U
#define GATT_CGM_FEATURES_CHARACTERISTIC         0x2AA8U
#define GATT_CGM_STATUS_CHARACTERISTIC           0x2AA9U
#define GATT_CGM_SSN_STRT_TIME_CHARACTERISTIC    0x2AAAU
#define GATT_CGM_SSN_RUN_TIME_CHARACTERISTIC     0x2AABU
#define GATT_CGM_SPECIFIC_OPS_CP_CHARACTERISTIC  0x2AACU

#define GATT_BATTERY_REMOVABLE_CHARACTERISTIC    0x2A3AU
#define GATT_BATTERY_SERV_REQRD_CHARACTERISTIC   0x2A3BU

/* Weight Scale Characteristics define */
#define GATT_WS_MSRMT_CHARACTERISTIC             0x2A9DU
#define GATT_WS_FEATURE_CHARACTERISTIC           0x2A9EU

/* Body Composition Characteristics define */
#define GATT_BC_MSRMT_CHARACTERISTIC             0x2A9CU
#define GATT_BC_FEATURE_CHARACTERISTIC           0x2A9BU

/* User Name Characterictics define */
#define GATT_UD_FIRST_NAME_CHARACTERISTIC        0x2A8AU
#define GATT_UD_LAST_NAME_CHARACTERISTIC         0x2A90U
#define GATT_UD_EMAIL_CHARACTERISTIC             0x2A87U
#define GATT_UD_AGE_CHARACTERISTIC               0x2A80U
#define GATT_UD_GENDER_CHARACTERISTIC            0x2A8CU
#define GATT_UD_DB_CHNG_INC_CHARACTERISTIC       0x2A99U
#define GATT_UD_DOB_CHARACTERISTIC               0x2A85U
#define GATT_UD_WEIGHT_CHARACTERISTIC            0x2A98U
#define GATT_UD_HEIGHT_CHARACTERISTIC            0x2A8EU
#define GATT_UD_VO2MAX_CHARACTERISTIC            0x2A96U
#define GATT_UD_HRMAX_CHARACTERISTIC             0x2A8DU
#define GATT_UD_RHR_CHARACTERISTIC               0x2A92U
#define GATT_UD_MAXRHR_CHARACTERISTIC            0x2A91U
#define GATT_UD_AEROTH_CHARACTERISTIC            0x2A7FU
#define GATT_UD_ANAETH_CHARACTERISTIC            0x2A83U
#define GATT_UD_SPORTTYPE_CHARACTERISTIC         0x2A93U
#define GATT_UD_DATETHASS_CHARACTERISTIC         0x2A86U
#define GATT_UD_WAISTCIR_CHARACTERISTIC          0x2A97U
#define GATT_UD_HIPCIR_CHARACTERISTIC            0x2A8FU
#define GATT_UD_FATBURN_LOW_CHARACTERISTIC       0x2A88U
#define GATT_UD_FATBURN_UPL_CHARACTERISTIC       0x2A89U
#define GATT_UD_AERO_LOW_CHARACTERISTIC          0x2A7EU
#define GATT_UD_AERO_UPL_CHARACTERISTIC          0x2A84U
#define GATT_UD_ANAE_LOW_CHARACTERISTIC          0x2A81U
#define GATT_UD_ANAE_UPL_CHARACTERISTIC          0x2A82U
#define GATT_UD_FIVEZONE_HRL_CHARACTERISTIC      0x2A8BU
#define GATT_UD_THREEZONE_HRL_CHARACTERISTIC     0x2A94U
#define GATT_UD_TWOZONE_HRL_CHARACTERISTIC       0x2A95U
#define GATT_UD_USER_INDEX_CHARACTERISTIC        0x2A9AU
#define GATT_UD_USER_CNTRL_PNT_CHARACTERISTIC    0x2A9FU
#define GATT_UD_LANGUAGE_CHARACTERISTIC          0x2AA2U
#define GATT_UD_REGISTERED_USER_CHARACTERISTIC   0x2B37U

/* Environmental Sensing Characterisitcs define */
#define GATT_ESS_DVC_CHARACTERISTIC              0x2A7DU
#define GATT_ESS_TEMP_CHARACTERISTIC             0x2A6EU
#define GATT_ES_MEASURMENT_DESCRIPTOR            0x290CU
#define GATT_ES_TRIGGER_SETTING_DESCRIPTOR       0x290DU
#define GATT_ES_CONFIGURATION_DESCRIPTOR         0x290BU
#define GATT_ES_MEASUREMENT_DESCRIPTOR           0x290CU

/* Bond Management Characteritiscs defines */
#define GATT_BM_FEATURES_CHARACTERISTIC          0x2AA5U
#define GATT_BM_CNTRL_PNT_CHARACTERISTIC         0x2AA4U

/* Automation IO Service Characteritiscs defines */
#define GATT_AIO_DIGITAL_CHARACTERISTIC          0x2A56U
#define GATT_AIO_ANALOG_CHARACTERISTIC           0x2A58U
#define GATT_AIO_AGGREGATE_CHARACTERISTIC        0x2A5AU

/* Indoor Positioning Characteritiscs defines */
#define GATT_IP_INDOOR_POS_CONFIG_CHARACTERISTIC 0x2AADU
#define GATT_IP_LATITUDE_CHARACTERISTIC          0x2AAEU
#define GATT_IP_LONGITUDE_CHARACTERISTIC         0x2AAFU
#define GATT_IP_LOCAL_NORTH_CHARACTERISTIC       0x2AB0U
#define GATT_IP_LOCAL_EAST_CHARACTERISTIC        0x2AB1U
#define GATT_IP_FLOOR_NUM_CHARACTERISTIC         0x2AB2U
#define GATT_IP_ALTITUDE_CHARACTERISTIC          0x2AB3U
#define GATT_IP_UNCERTAINTY_CHARACTERISTIC       0x2AB4U
#define GATT_IP_LOCAL_NAME_CHARACTERISTIC        0x2AB5U

/* Object Transfer Characteritics defines */
#define GATT_OTS_FEATURE_CHARACTERISTIC          0x2ABDU
#define GATT_OTS_OBJ_NAME_CHARACTERISTIC         0x2ABEU
#define GATT_OTS_OBJ_TYPE_CHARACTERISTIC         0x2ABFU
#define GATT_OTS_OBJ_SIZE_CHARACTERISTIC         0x2AC0U
#define GATT_OTS_OBJ_FIRST_CRTD_CHARACTERISTIC   0x2AC1U
#define GATT_OTS_OBJ_LAST_MODFD_CHARACTERISTIC   0x2AC2U
#define GATT_OTS_OBJ_ID_CHARACTERISTIC           0x2AC3U
#define GATT_OTS_OBJ_PRPTY_CHARACTERISTIC        0x2AC4U
#define GATT_OTS_OACP_CHARACTERISTIC             0x2AC5U
#define GATT_OTS_OLCP_CHARACTERISTIC             0x2AC6U
#define GATT_OTS_OBJ_LIST_FILTER_CHARACTERISTIC  0x2AC7U
#define GATT_OTS_OBJ_CHANGED_CHARACTERISTIC      0x2AC8U
#define GATT_OTS_LE_PSM_CHARACTERISTIC           0x7FBDU
#define GATT_OTS_OBJ_LAST_ACCSD_CHARACTERISTIC   0x7FE3U
#define GATT_OTS_OBJ_CHECKSUM_CHARACTERISTIC     0x7FE2U
#define GATT_OTS_OCTECT_OFFSET_CHARACTERISTIC    0x7FE1U

/* Tire Pressure Monitoring Characteritiscs defines */
/* Needs to update the opcode once TPMS spec is adopted  */
#define GATT_TPM_TIRE_PRESSURE_CHARACTERISTIC    0x1111U
#define GATT_TPM_TIRE_TEMP_CHARACTERISTIC        0x2222U
#define GATT_TPM_PRESSURE_CALIBN_CHARACTERISTIC  0x3333U
#define GATT_TPM_TEMP_CALIBN_CHARACTERISTIC      0x4444U
#define GATT_TPM_DISRY_CONN_MGMNT_CHARACTERISTIC 0xFFFFU
#define GATT_TPM_ADV_PERIOD_CHARACTERISTIC       0xAAA1U

/* Transport Discovery Characteristics defines */
#define GATT_TD_CONTROL_POINT_CHARACTERISTIC     0x2ABCU

/* HTTP Proxy Characteristics defines */
#define GATT_HPC_URI_CHARACTERISTIC              0x2AB6U
#define GATT_HPC_HTTP_HEADERS_CHARACTERISTIC     0x2AB7U
#define GATT_HPC_HTTP_ENTITY_BODY_CHARACTERISTIC 0x2AB9U
#define GATT_HPC_HTTP_CP_CHARACTERISTIC          0x2ABAU
#define GATT_HPC_HTTP_STATUS_CODE_CHARACTERISTIC 0x2AB8U
#define GATT_HPC_HTTPS_SECURITY_CHARACTERISTIC   0x2ABBU

/* Fitness Machine Characteristics defines */
#define GATT_FTM_FEATURE_CHARACTERISTIC          0x2ACCU
#define GATT_FTM_TREADMILL_CHARACTERISTIC        0x2ACDU
#define GATT_FTM_CROSS_TRAINER_CHARACTERISTIC    0x2ACEU
#define GATT_FTM_STEP_CLIMBER_CHARACTERISTIC     0x2ACFU
#define GATT_FTM_STAIR_CLIMBER_CHARACTERISTIC    0x2AD0U
#define GATT_FTM_ROWER_DATA_CHARACTERISTIC       0x2AD1U
#define GATT_FTM_INDOOR_BIKE_CHARACTERISTIC      0x2AD2U
#define GATT_FTM_TRAINING_STATUS_CHARACTERISTIC  0x2AD3U
#define GATT_FTM_SUPPD_SPEED_RNG_CHARACTERISTIC  0x2AD4U
#define GATT_FTM_SUPPD_INCLN_RNG_CHARACTERISTIC  0x2AD5U
#define GATT_FTM_SUPPD_RESIS_RNG_CHARACTERISTIC  0x2AD6U
#define GATT_FTM_SUPPD_POWER_RNG_CHARACTERISTIC  0x2AD8U
#define GATT_FTM_SUPPD_HRTRE_RNG_CHARACTERISTIC  0x2AD7U
#define GATT_FTM_CONTROL_POINT_CHARACTERISTIC    0x2AD9U
#define GATT_FTM_MACHINE_STATUS_CHARACTERISTIC   0x2ADAU

/* Reconnection Configuration Characteristic defines */
#define GATT_RC_FEATURE_CHARACTERISTIC           0x2B1DU
#define GATT_RC_SETTING_CHARACTERISTIC           0x2B1EU
#define GATT_RC_CP_CHARACTERISTIC                0x2B1FU

/* Insulin Delivery Characteristics defines */
#define GATT_IDD_STATUS_CHANGED_CHARACTERISTIC   0x2B20U
#define GATT_IDD_STATUS_CHARACTERISTIC           0x2B21U
#define GATT_IDD_ANNUN_STATUS_CHARACTERISTIC     0x2B22U
#define GATT_IDD_FEATURE_CHARACTERISTIC          0x2B23U
#define GATT_IDD_STATUS_READER_CP_CHARACTERISTIC 0x2B24U
#define GATT_IDD_COMMAND_CP_CHARACTERISTIC       0x2B25U
#define GATT_IDD_COMMAND_DATA_CHARACTERISTIC     0x2B26U
#define GATT_IDD_RACP_CHARACTERISTIC             0x2B27U
#define GATT_IDD_HISTORY_DATA_CHARACTERISTIC     0x2B28U

/** Characteristic UUID defines of GA profiles/Services Under Development */
/* Volume Control Characteristic defines */
/**
 * Volume State Characteristic                   0x8FB9
 * Volume Control Point Characteristic           0x8FBA
 * Volume Flags Characteristic                   0x8FBB
 */
#define GATT_VC_VOLUME_STATE_CHARACTERISTIC      0x8FB9U
#define GATT_VC_VOLUME_CP_CHARACTERISTIC         0x8FBAU
#define GATT_VC_VOLUME_FLAG_CHARACTERISTIC       0x8FBBU

/* Volume Offset Control Characteristic defines */
/**
 * Offset State Characteristic                   0x8FBC
 * Audio Location Characteristic                 0x8FBD
 * Volume Offset Control Point Characteristic    0x8FBE
 * Audio Output Description Characteristic       0x8FBF
 */
#define GATT_VOC_OFFSET_STATE_CHARACTERISTIC     0x8FBCU
#define GATT_VOC_AUDIO_LOCATION_CHARACTERISTIC   0x8FBDU
#define GATT_VOC_OFFSET_CP_CHARACTERISTIC        0x8FBEU
#define GATT_VOC_AUDIO_OP_DESC_CHARACTERISTIC    0x8FBFU

/* Mic Input Control Service Characteristic defines */
/**
 * Mute characteristic                           0x8FE1
 */
#define GATT_MICS_MUTE_CHARACTERISTIC           0x8FE1U

/* Audio Input Control Service Characteristic defines */
/**
 * Input State Characteristic                   0x8FE2
 * Gain Settings Attribute Characteristic       0x8FE3
 * Input Type Characteristic                    0x8FE5
 * Input Status Characteristic                  0x8FE6
 * Audio Input Control Point Characteristic     0x8FE7
 * Audio Input Description Characteristic       0x8FE8
 */
#define GATT_AICS_OFFSET_STATE_CHARACTERISTIC     0x8FE2U
#define GATT_AICS_GAIN_SETTING_CHARACTERISTIC     0x8FE3U
#define GATT_AICS_INPUT_TYPE_CHARACTERISTIC       0x8FE5U
#define GATT_AICS_INPUT_STATUS_CHARACTERISTIC     0x8FE6U
#define GATT_AICS_AUDIO_INPUT_CP_CHARACTERISTIC   0x8FE7U
#define GATT_AICS_AUDIO_INPUT_DESC_CHARACTERISTIC 0x8FE8U

/* Audio Capability Characteristic defines */
/**
 * Sink PAC                                         0x8F97
 * Sink Audio Locations                             0x8F98
 * Source PAC                                       0x8F99
 * Source Audio Locations                           0x8F9A
 *
 * TODO:
 * - Char. values are not correct.
 */
#define GATT_AC_SNK_PAC_CHARACTERISTIC              0x8F97U
#define GATT_AC_SNK_AUDIO_LOC_CHARACTERISTIC        0x8F98U
#define GATT_AC_SRC_PAC_CHARACTERISTIC              0x8F99U
#define GATT_AC_SRC_AUDIO_LOC_CHARACTERISTIC        0x8F9AU

/* Telephone Bearer Service Characteristic defines */
/**
 * Bearer Provider Name                             0x8FEA
 * Bearer UCI                                       0x8FEB
 * Bearer Technology                                0x8FEC
 * Bearer URI Prefixes Supported List               0x8FED
 * Bearer Signal Strength                           0x8FEF
 * Bearer Signal Strength Reporting Interval        0x8FF0
 * Bearer List Current Calls                        0x8FF1
 * Content Control ID                               0x8FF2
 * Features and Status Flags Characteristic         0x8FF3
 * Incoming Call Target Caller ID                   0x8FF4
 * Call State Characteristic                        0x8FF5
 * Call Control Point Characteristic                0x8FF6
 * Call Control Point Supported Characteristic      0x8FF7
 * Termination Reason Characteristic                0x8FF8
 * Incoming Remote Caller-ID Characteristic         0x8FF9
 * Outgoing Remote Caller-ID Characteristic         0x8FFA
 */
#define GATT_TBS_BRR_PROVIDER_NAME                  0x8FEAU
#define GATT_TBS_BRR_UCI                            0x8FEBU
#define GATT_TBS_BRR_TECHNOLOGY                     0x8FECU
#define GATT_TBS_BRR_URI_PRFX_SUPPORT_LIST          0x8FEDU
#define GATT_TBS_BRR_SIGNAL_STRENGTH                0x8FEFU
#define GATT_TBS_BRR_SIGNAL_STRENGTH_RPRT_INTVL     0x8FF0U
#define GATT_TBS_BRR_LIST_CUR_CALLS                 0x8FF1U
#define GATT_TBS_CNTNT_CTRL_ID                      0x8FF2U
#define GATT_TBS_FEATURE_AND_STATUS_FLAGS           0x8FF3U
#define GATT_TBS_INCALL_TGT_BRR_CALLER_ID           0x8FF4U
#define GATT_TBS_CALL_STATE                         0x8FF5U
#define GATT_TBS_CALL_CTRL_POINT                    0x8FF6U
#define GATT_TBS_CALL_CTRL_POINT_SUPPORT_OP         0x8FF7U
#define GATT_TBS_TERMINATION_REASON                 0x8FF8U
#define GATT_TBS_INCALL_REMOTE_CALLER_ID            0x8FF9U
#define GATT_TBS_OUTCALL_REMOTE_CALLER_ID           0x8FFAU

/* Routing Active Audio Service Characteristic defines */
/**
 * Available Member List Characteristic                 0x8FC0
 * Available Member Connection Status Characteristic    0x8FC1
 * Default Audio Routes List Characteristic             0x8FC2
 * Current Active Routes List Characteristic            0x8FC3
 * Modify Audio Routing Control Point Characteristic    0x8FC4
 * Max Supported Characteristic                         0x8FC5
 */
#define GATT_RAAS_CHAR_AVLBL_MBR_LIST                   0x8FC0U
#define GATT_RAAS_CHAR_AVLBL_MBR_CONTN_STATUS           0x8FC1U
#define GATT_RAAS_CHAR_DEFAULT_AUDIO_ROUTES_LIST        0x8FC2U
#define GATT_RAAS_CHAR_CURR_ACTIVE_ROUTES_LIST          0x8FC3U
#define GATT_RAAS_CHAR_MODIFY_AUDIO_ROUTE_CP            0x8FC4U
#define GATT_RAAS_CHAR_MAX_SUPPORTED                    0x8FC5U

/* Coordinated Set Identification Service Characteristic defines */
/**
 * Rank Characteristic                                  0x8FCC
 * Set Identity Resolving Key Characteristic            0x8FCD
 * Size Characteristic                                  0x8FCE
 * Lock Characteristic                                  0x8FCF
 */
#define GATT_CSI_CHAR_RANK                              0x8FCCU
#define GATT_CSI_CHAR_SET_IDNTY_RESOLVE_KEY             0x8FCDU
#define GATT_CSI_CHAR_SIZE                              0x8FCEU
#define GATT_CSI_CHAR_LOCK                              0x8FCFU

/* Media Control Service Characteristic defines */
/**
 * Media Player Name Characteristic                     0x8FA0
 * Media Icon Object Characteristic                     0x8FA1
 * Media Icon URI Characteristic                        0x8FA2
 * Track Changed Characteristic                         0x8FA3
 * Track Title Characteristic                           0x8FA4
 * Track Duration Characteristic                        0x8FA5
 * Track Position Characteristic                        0x8FA6
 * Playback Speed Characteristic                        0x8FA7
 * Playback Speeds Supported Characteristic             0x8FA8
 * Seeking Speed Characteristic                         0x8FA9
 * Track Segments Object Characteristic                 0x8FAA
 * Current Track Object Characteristic                  0x8FAB
 * Next Track Object Characteristic                     0x8FAC
 * Current Group Object Characteristic                  0x8FAD
 * Playing Order Characteristic                         0x8FAE
 * Playing Order Supported Characteristic               0x8FAF
 * Media State Characteristic                           0x8FB0
 * Media Control Point  Characteristic                  0x8FB1
 * Media Control Opcodes Supported  Characteristic      0x8FB2
 * Search Results Object Characteristic                 0x8FB3
 * Search Control Point Characteristic                  0x8FB4
 * Content Control ID Characteristic                    0x8FB5
 */
#define GATT_MCS_CHAR_MEDIA_PLAYER_NAME                 0x8FA0U
#define GATT_MCS_CHAR_MEDIA_ICON_OBJ                    0x8FA1U
#define GATT_MCS_CHAR_MEDIA_ICON_URI                    0x8FA2U
#define GATT_MCS_CHAR_TRACK_CHANGED                     0x8FA3U
#define GATT_MCS_CHAR_TRACK_TITLE                       0x8FA4U
#define GATT_MCS_CHAR_TRACK_DURATION                    0x8FA5U
#define GATT_MCS_CHAR_TRACK_PROSITION                   0x8FA6U
#define GATT_MCS_CHAR_PLAYBACK_SPEED                    0x8FA7U
#define GATT_MCS_CHAR_PLAYBACK_SPEED_SUPPORTED          0x8FA8U
#define GATT_MCS_CHAR_SEEKING_SPEED                     0x8FA9U
#define GATT_MCS_CHAR_TRACK_SEGMNTS_OBJECT              0x8FAAU
#define GATT_MCS_CHAR_CURRENT_TRACK_OBJECT              0x8FABU
#define GATT_MCS_CHAR_NEXT_TRACK_OBJECT                 0x8FACU
#define GATT_MCS_CHAR_CURRENT_GROUP_OBJECT              0x8FADU
#define GATT_MCS_CHAR_PLAYING_ORDER                     0x8FAEU
#define GATT_MCS_CHAR_PLAYING_ORDER_SUPPORTED           0x8FAFU
#define GATT_MCS_CHAR_MEDIA_STATE                       0x8FB0U
#define GATT_MCS_CHAR_MEDIA_CP                          0x8FB1U
#define GATT_MCS_CHAR_MEDIA_CP_SUPPORTED                0x8FB2U
#define GATT_MCS_CHAR_SEARCH_RESULTS_OBJECT             0x8FB3U
#define GATT_MCS_CHAR_SEARCH_CP                         0x8FB4U
#define GATT_MCS_CHAR_SEARCH_CNTNT_CNTRL_ID             0x8FB5U

/* Audio Stream Control Service */
/**
 * Audio Content Availabilty Characteristic             0x8F9B
 * ASE  Characteristic                                  0x8F9C
 * ASE Control Point Characteristic                     0x8F9D
 */
#define GATT_ASCS_CHAR_AUDIO_CNTNT_AVAILBLTY            0x8F9BU
#define GATT_ASCS_CHAR_ASE                              0x8F9CU
#define GATT_ASCS_CHAR_ASE_CP                           0x8F9DU

/* Broadcast Audio Scan Service */
/**
 * Remote Scanning                                      0x8F9E
 * Broadcast Receive State                              0x8F9F
 */
#define GATT_BASS_CHAR_REMOTE_SCAN                      0x8F9EU
#define GATT_BASS_CHAR_BRCST_RX_STATE                   0x8F9FU

#define ATT_APPL_PROCEDURE_ALREADY_IN_PROGRESS   0x80U
#define ATT_APPL_CCD_IMPROPERLY_CONFIGURED       0x81U

/* GATT Units */

#endif /* _H_GATT_DEFINES_ */

