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
#define GATT_PRIMARY_SERVICE            0x2800
#define GATT_SECONDARY_SERVICE          0x2801
#define GATT_INCLUDE                    0x2802
#define GATT_CHARACTERISTIC             0x2803
#define GATT_EXTENDED_PROPERTIES        0x2900
#define GATT_USER_DESCRIPTION           0x2901
#define GATT_CLIENT_CONFIG              0x2902
#define GATT_SERVER_CONFIG              0x2903
#define GATT_FORMAT                     0x2904
#define GATT_AGGREGATE_FORMAT           0x2905
#define GATT_VALID_RANGE                0x2906
#define GATT_EXTERNAL_REPORT_REF        0x2907
#define GATT_IP_OP_FEATURE_REPORT_REF   0x2908

/** GATT Characteristic Properties Bit Field */

/** Broadcast */
#define GATT_CH_PROP_BIT_BROADCAST         0x01

/** Read */
#define GATT_CH_PROP_BIT_READ              0x02

/** Write Without Response */
#define GATT_CH_PROP_BIT_WRITE_WO_RSP      0x04

/** Write */
#define GATT_CH_PROP_BIT_WRITE             0x08

/** Notify */
#define GATT_CH_PROP_BIT_NOTIFY            0x10

/** Indicate */
#define GATT_CH_PROP_BIT_INDICATE          0x20

/** Authenticated Signed Write */
#define GATT_CH_PROP_BIT_SIGN_WRITE        0x40

/** Extended Properties */
#define GATT_CH_PROP_BIT_EXT_PROPERTY      0x80


/* GATT Client Configuration values */
#define GATT_CLI_CNFG_NOTIFICATION         0x0001
#define GATT_CLI_CNFG_INDICATION           0x0002
#define GATT_CLI_CNFG_DEFAULT              0x0000

/* GATT Server Configuration values */
#define GATT_SER_CNFG_BROADCAST            0x0001
#define GATT_SER_CNFG_DEFAULT              0x0000

/* GATT Services */
#define GATT_GAP_SERVICE                       0x1800
#define GATT_GATT_SERVICE                      0x1801
#define GATT_IMMEDIATE_ALERT_SERVICE           0x1802
#define GATT_LINK_LOSS_SERVICE                 0x1803
#define GATT_TX_POWER_SERVICE                  0x1804
#define GATT_CURRENT_TIME_SERVICE              0x1805
#define GATT_REF_TIME_UPDATE_SERVICE           0x1806
#define GATT_NEXT_DST_CHANGE_SERVICE           0x1807
#define GATT_GLUCOSE_SERVICE                   0x1808
#define GATT_HEALTH_THERMOMETER_SERVICE        0x1809
#define GATT_DEVICE_INFO_SERVICE               0x180A
#define GATT_NWA_SERVICE                       0x180B
#define GATT_WATCH_DOG_SERVICE                 0x180C
#define GATT_HEART_RATE_SERVICE                0x180D
#define GATT_PHONE_ALERT_STATUS_SERVICE        0x180E
#define GATT_BATTERY_SERVICE                   0x180F
#define GATT_BLOOD_PRESSURE_SERVICE            0x1810
#define GATT_ALERT_NOTIFICATION_SERVICE        0x1811
#define GATT_HID_SERVICE                       0x1812
#define GATT_SCAN_PARAM_SERVICE                0x1813
#define GATT_RUNNING_SPEED_AND_CADENCE_SERVICE 0x1814
#define GATT_AIOS_SERVICE                      0x1815
#define GATT_CYCLING_SPEED_AND_CADENCE_SERVICE 0x1816
#define GATT_CPM_SERVICE                       0x1818
#define GATT_LOCATION_AND_NAVIGATION_SERVICE   0x1819
#define GATT_ENVIRONMENTAL_SENSING_SERVICE     0x181A
#define GATT_BODY_COMPOSITION_SERVICE          0x181B
#define GATT_USER_DATA_SERVICE                 0x181C
#define GATT_WEIGHT_SCALE_SERVICE              0x181D
#define GATT_BM_SERVICE                        0x181E
#define GATT_CGM_SERVICE                       0x181F
#define GATT_INT_PS_SERVICE                    0x1820
#define GATT_IPS_SERVICE                       0x1821
#define GATT_PULSE_OXIMETER_SERVICE            0x1822
#define GATT_HPS_SERVICE                       0x1823
#define GATT_TDS_SERVICE                       0x1824
#define GATT_OBJECT_TRANSFER_SERVICE           0x1825
#define GATT_FTMS_SERVICE                      0x1826
#define GATT_RCS_SERVICE                       0x1829
#define GATT_INSULIN_DELIVERY_SERVICE          0x183A

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
#define GATT_VCS_SERVICE                       0x8FD1
#define GATT_VOCS_SERVICE                      0x8FD2
#define GATT_AICS_SERVICE                      0x8FD3
#define GATT_MICS_SERVICE                      0x8FD4
#define GATT_TBS_SERVICE                       0x8FD5
#define GATT_RAAS_SERVICE                      0x8FD6
#define GATT_MCS_SERVICE                       0x8FD7
#define GATT_CSIS_SERVICE                      0x8FD8
#define GATT_ACS_SERVICE                       0x8FD9
#define GATT_ASCS_SERVICE                      0x8FDA
#define GATT_BASS_SERVICE                      0x8FDB
#define GATT_BAAS_SERVICE                      0x8FDC
#define GATT_BRAAS_SERVICE                     0x8FDD
#define GATT_GMCS_SERVICE                      0x8FDE

/* MCS/GMCS OTS Service */
/**
 * Track Segments                               0x8F90
 * Track                                        0x8F91
 * Group                                        0x8F92
 */
#define GATT_MCS_OTS_OBJ_T_TS                   0x8F90
#define GATT_MCS_OTS_OBJ_T_T                    0x8F91
#define GATT_MCS_OTS_OBJ_T_G                    0x8F92

/* Dont find this on .org */
#define GATT_FIND_ME_SERVICE                   0x18A3

/* Still not adopted, may need updation */
#define GATT_TPMS_SERVICE                      0x7FD0

/* GATT Characteristic */
#define GATT_DEVICE_NAME_CHARACTERISTIC          0x2A00
#define GATT_APPEARANCE_CHARACTERISTIC           0x2A01
#define GATT_PERIPHRL_PRIVCY_FLG_CHARACTERISTIC  0x2A02
#define GATT_RECONNECT_ADDR_CHARACTERISTIC       0x2A03
#define GATT_PRFRRD_CNXN_PARAM_CHARACTERISTIC    0x2A04
#define GATT_SERVICE_CHANGED_CHARACTERISTIC      0x2A05
#define GATT_ALERT_LEVEL_CHARACTERISTIC          0x2A06
#define GATT_TX_POWER_LEVEL_CHARACTERISTIC       0x2A07
#define GATT_DATE_TIME_CHARACTERISTIC            0x2A08
#define GATT_DAY_OF_WEEK_CHARACTERISTIC          0x2A09
#define GATT_DAY_DATE_TIME_CHARACTERISTIC        0x2A0A
#define GATT_EXACT_TIME_100_CHARACTERISTIC       0x2A0B
#define GATT_EXACT_TIME_256_CHARACTERISTIC       0x2A0C
#define GATT_DST_OFFSET_CHARACTERISTIC           0x2A0D
#define GATT_TIME_ZONE_CHARACTERISTIC            0x2A0E
#define GATT_LOCAL_TIME_INFO_CHARACTERISTIC      0x2A0F
#define GATT_SEC_TIME_ZONE_CHARACTERISTIC        0x2A10
#define GATT_TIME_WITH_DST_CHARACTERISTIC        0x2A11
#define GATT_TIME_ACCURACY_CHARACTERISTIC        0x2A12
#define GATT_TIME_SOURCE_CHARACTERISTIC          0x2A13
#define GATT_REF_TIME_INFO_CHARACTERISTIC        0x2A14
#define GATT_TIME_BROADCAST_CHARACTERISTIC       0x2A15
#define GATT_TIME_UPDATE_CONTROL_POINT           0x2A16
#define GATT_TIME_UPDATE_STATE_CHARACTERISTIC    0x2A17
#define GATT_GLUCOSE_MSRMNT_CHARACTERISTIC       0x2A18
#define GATT_BATTERY_LEVEL_CHARACTERISTIC        0x2A19
#define GATT_BATTERY_PWR_ST_CHARCTERISTIC        0x2A1A
#define GATT_BATTERY_LEVEL_ST_CHARACTERISTIC     0x2A1B
#define GATT_TEMPERATURE_MSMNT_CHARACTERISTIC    0x2A1C
#define GATT_TEMPERATURE_TYPE_CHARACTERISTIC     0x2A1D
#define GATT_INTERMEDIATE_TEMP_CHARACTERISTIC    0x2A1E
#define GATT_MSMNT_INTERVAL_CHARATACTERISTIC     0x2A21
#define GATT_BOOT_KYBRD_IN_RPT_CHARACTERISTIC    0x2A22
#define GATT_SYSTEM_ID_CHARACTERISTIC            0x2A23
#define GATT_MODEL_NUMBER_CHARACTERISTIC         0x2A24
#define GATT_SERIAL_NUMBER_CHARACTERISTIC        0x2A25
#define GATT_FIRMWARE_REV_CHARACTERISTIC         0x2A26
#define GATT_HARDWARE_REV_CHARACTERISTIC         0x2A27
#define GATT_SOFTWARE_REV_CHARACTERISTIC         0x2A28
#define GATT_MANUFACTURER_NAME_CHARACTERISTIC    0x2A29
#define GATT_REG_CERT_DATA_CHARACTERISTIC        0x2A2A
#define GATT_CURRENT_TIME_CHARACTERISTIC         0x2A2B
#define GATT_SCAN_REFRESH_CHARACTERISTIC         0x2A31
#define GATT_BOOT_KYBRD_OP_CHARACTERISTIC        0x2A32
#define GATT_MOUSE_IP_CHARACTERISTIC             0x2A33
#define GATT_GL_MSRMT_CTX_CHARACTERISTIC         0x2A34
#define GATT_BP_MSRMT_CHARACTERISTIC             0x2A35
#define GATT_INTRMDT_CUFF_PRSR_CHARACTERISTIC    0x2A36
#define GATT_HR_MSRMT_CHARACTERISTIC             0x2A37
#define GATT_BODY_SENSOR_LOC_CHARACTERISTIC      0x2A38
#define GATT_HR_CNTRL_POINT                      0x2A39
#define GATT_NWA_CHARACTERISTIC                  0x2A3E
#define GATT_ALERT_STATUS_CHARACTERISTIC         0x2A3F
#define GATT_RINGER_CONTROL_POINT_CHARACTERISTIC 0x2A40
#define GATT_RINGER_SETTING_CHARACTERISTIC       0x2A41
#define GATT_ALERT_ID_BIT_MASK_CHARACTERISTIC    0x2A42
#define GATT_ALERT_ID_CHARACTERISTIC             0x2A43
#define GATT_ALERT_NTF_CONTROL_POINT             0x2A44
#define GATT_UNREAD_ALRT_STATUS_CHARACTERISTIC   0x2A45
#define GATT_NEW_ALERT_CHARACTERISTIC            0x2A46
#define GATT_SPRTD_NEW_ALRT_CTGRY_CHARACTERISTIC 0x2A47
#define GATT_SPRT_UNRD_ALRT_CTGRY_CHARACTERISTIC 0x2A48
#define GATT_BP_FEATURE_CHARACTERISTIC           0x2A49
#define GATT_HID_INFO_CHARACTERISTIC             0x2A4A
#define GATT_HID_RPT_MAP_CHARACTERISTIC          0x2A4B
#define GATT_HID_CP_CHARACTERISTIC               0x2A4C
#define GATT_HID_RPT_CHARACTERISTIC              0x2A4D
#define GATT_HID_PROTO_MODE_CHARACTERISTIC       0x2A4E
#define GATT_SCAN_WINDOW_CHARACTERISTIC          0x2A4F
#define GATT_PNP_ID_CHARACTERISTIC               0x2A50
#define GATT_GL_FEATURE_CHARACTERISTIC           0x2A51
#define GATT_RACP_CHARACTERISTIC                 0x2A52
#define GATT_RSC_MSRMT_CHARACTERISTIC            0x2A53
#define GATT_RSC_FEATURE_CHARACTERISTIC          0x2A54
#define GATT_SC_CONTROL_POINT_CHARACTERISTIC     0x2A55
#define GATT_CSC_MSRMT_CHARACTERISTIC            0x2A5B
#define GATT_CSC_FEATURE_CHARACTERISTIC          0x2A5C
#define GATT_SENSOR_LOCATION_CHARACTERISTIC      0x2A5D

/* Still Not Adopted, may need updation */
/* Pulse Oximeter Characteristics defines */
#define GATT_POX_SPOT_CHK_MSRMT_CHARACTERISTIC   0x2A5E
#define GATT_POX_CONTINUOUS_MSRMT_CHARACTERISTIC 0x2A5F
#define GATT_POX_PULSATILE_EVENT_CHARACTERISTIC  0x2A61
#define GATT_POX_FEATURES_CHARACTERISTIC         0x2A60
#define GATT_POX_CONTROL_POINT_CHARACTERISTIC    0x2A62

/* CPM Characteristics Measurments */
#define GATT_CPM_MSRMT_CHARACTERISTIC            0x2A63
#define GATT_CPM_VECTOR_CHARACTERISTIC           0x2A64
#define GATT_CPM_FEATURE_CHARACTERISTIC          0x2A65
#define GATT_CPM_CNTRL_PNT_CHARACTERISTIC        0x2A66
#define GATT_CPM_SENSOR_LOCATION_CHARACTERISTIC  0x2A5D

/* Location and Navigation Characteristics */
#define GATT_LOCATION_AND_SPEED_CHARACTERISTIC   0x2A67
#define GATT_NAVIGATION_CHARACTERISTIC           0x2A68
#define GATT_LN_POSITION_QUALITY_CHARACTERISTIC  0x2A69
#define GATT_LN_FEATURE_CHARACTERISTIC           0x2A6A
#define GATT_LN_CNTRL_PNT_CHARACTERISTIC         0x2A6B

/* CGM Characteristics defines */
#define GATT_CGM_MSRMT_CHARACTERISTIC            0x2AA7
#define GATT_CGM_FEATURES_CHARACTERISTIC         0x2AA8
#define GATT_CGM_STATUS_CHARACTERISTIC           0x2AA9
#define GATT_CGM_SSN_STRT_TIME_CHARACTERISTIC    0x2AAA
#define GATT_CGM_SSN_RUN_TIME_CHARACTERISTIC     0x2AAB
#define GATT_CGM_SPECIFIC_OPS_CP_CHARACTERISTIC  0x2AAC

#define GATT_BATTERY_REMOVABLE_CHARACTERISTIC    0x2A3A
#define GATT_BATTERY_SERV_REQRD_CHARACTERISTIC   0x2A3B

/* Weight Scale Characteristics define */
#define GATT_WS_MSRMT_CHARACTERISTIC             0x2A9D
#define GATT_WS_FEATURE_CHARACTERISTIC           0x2A9E

/* Body Composition Characteristics define */
#define GATT_BC_MSRMT_CHARACTERISTIC             0x2A9C
#define GATT_BC_FEATURE_CHARACTERISTIC           0x2A9B

/* User Name Characterictics define */
#define GATT_UD_FIRST_NAME_CHARACTERISTIC        0x2A8A
#define GATT_UD_LAST_NAME_CHARACTERISTIC         0x2A90
#define GATT_UD_EMAIL_CHARACTERISTIC             0x2A87
#define GATT_UD_AGE_CHARACTERISTIC               0x2A80
#define GATT_UD_GENDER_CHARACTERISTIC            0x2A8C
#define GATT_UD_DB_CHNG_INC_CHARACTERISTIC       0x2A99
#define GATT_UD_DOB_CHARACTERISTIC               0x2A85
#define GATT_UD_WEIGHT_CHARACTERISTIC            0x2A98
#define GATT_UD_HEIGHT_CHARACTERISTIC            0x2A8E
#define GATT_UD_VO2MAX_CHARACTERISTIC            0x2A96
#define GATT_UD_HRMAX_CHARACTERISTIC             0x2A8D
#define GATT_UD_RHR_CHARACTERISTIC               0x2A92
#define GATT_UD_MAXRHR_CHARACTERISTIC            0x2A91
#define GATT_UD_AEROTH_CHARACTERISTIC            0x2A7F
#define GATT_UD_ANAETH_CHARACTERISTIC            0x2A83
#define GATT_UD_SPORTTYPE_CHARACTERISTIC         0x2A93
#define GATT_UD_DATETHASS_CHARACTERISTIC         0x2A86
#define GATT_UD_WAISTCIR_CHARACTERISTIC          0x2A97
#define GATT_UD_HIPCIR_CHARACTERISTIC            0x2A8F
#define GATT_UD_FATBURN_LOW_CHARACTERISTIC       0x2A88
#define GATT_UD_FATBURN_UPL_CHARACTERISTIC       0x2A89
#define GATT_UD_AERO_LOW_CHARACTERISTIC          0x2A7E
#define GATT_UD_AERO_UPL_CHARACTERISTIC          0x2A84
#define GATT_UD_ANAE_LOW_CHARACTERISTIC          0x2A81
#define GATT_UD_ANAE_UPL_CHARACTERISTIC          0x2A82
#define GATT_UD_FIVEZONE_HRL_CHARACTERISTIC      0x2A8B
#define GATT_UD_THREEZONE_HRL_CHARACTERISTIC     0x2A94
#define GATT_UD_TWOZONE_HRL_CHARACTERISTIC       0x2A95
#define GATT_UD_USER_INDEX_CHARACTERISTIC        0x2A9A
#define GATT_UD_USER_CNTRL_PNT_CHARACTERISTIC    0x2A9F
#define GATT_UD_LANGUAGE_CHARACTERISTIC          0x2AA2
#define GATT_UD_REGISTERED_USER_CHARACTERISTIC   0x2B37

/* Environmental Sensing Characterisitcs define */
#define GATT_ESS_DVC_CHARACTERISTIC              0x2A7D
#define GATT_ESS_TEMP_CHARACTERISTIC             0x2A6E
#define GATT_ES_MEASURMENT_DESCRIPTOR            0x290C
#define GATT_ES_TRIGGER_SETTING_DESCRIPTOR       0x290D
#define GATT_ES_CONFIGURATION_DESCRIPTOR         0x290B
#define GATT_ES_MEASUREMENT_DESCRIPTOR           0x290C

/* Bond Management Characteritiscs defines */
#define GATT_BM_FEATURES_CHARACTERISTIC          0x2AA5
#define GATT_BM_CNTRL_PNT_CHARACTERISTIC         0x2AA4

/* Automation IO Service Characteritiscs defines */
#define GATT_AIO_DIGITAL_CHARACTERISTIC          0x2A56
#define GATT_AIO_ANALOG_CHARACTERISTIC           0x2A58
#define GATT_AIO_AGGREGATE_CHARACTERISTIC        0x2A5A

/* Indoor Positioning Characteritiscs defines */
#define GATT_IP_INDOOR_POS_CONFIG_CHARACTERISTIC 0x2AAD
#define GATT_IP_LATITUDE_CHARACTERISTIC          0x2AAE
#define GATT_IP_LONGITUDE_CHARACTERISTIC         0x2AAF
#define GATT_IP_LOCAL_NORTH_CHARACTERISTIC       0x2AB0
#define GATT_IP_LOCAL_EAST_CHARACTERISTIC        0x2AB1
#define GATT_IP_FLOOR_NUM_CHARACTERISTIC         0x2AB2
#define GATT_IP_ALTITUDE_CHARACTERISTIC          0x2AB3
#define GATT_IP_UNCERTAINTY_CHARACTERISTIC       0x2AB4
#define GATT_IP_LOCAL_NAME_CHARACTERISTIC        0x2AB5

/* Object Transfer Characteritics defines */
#define GATT_OTS_FEATURE_CHARACTERISTIC          0x2ABD
#define GATT_OTS_OBJ_NAME_CHARACTERISTIC         0x2ABE
#define GATT_OTS_OBJ_TYPE_CHARACTERISTIC         0x2ABF
#define GATT_OTS_OBJ_SIZE_CHARACTERISTIC         0x2AC0
#define GATT_OTS_OBJ_FIRST_CRTD_CHARACTERISTIC   0x2AC1
#define GATT_OTS_OBJ_LAST_MODFD_CHARACTERISTIC   0x2AC2
#define GATT_OTS_OBJ_ID_CHARACTERISTIC           0x2AC3
#define GATT_OTS_OBJ_PRPTY_CHARACTERISTIC        0x2AC4
#define GATT_OTS_OACP_CHARACTERISTIC             0x2AC5
#define GATT_OTS_OLCP_CHARACTERISTIC             0x2AC6
#define GATT_OTS_OBJ_LIST_FILTER_CHARACTERISTIC  0x2AC7
#define GATT_OTS_OBJ_CHANGED_CHARACTERISTIC      0x2AC8
#define GATT_OTS_LE_PSM_CHARACTERISTIC           0x7FBD
#define GATT_OTS_OBJ_LAST_ACCSD_CHARACTERISTIC   0x7FE3
#define GATT_OTS_OBJ_CHECKSUM_CHARACTERISTIC     0x7FE2
#define GATT_OTS_OCTECT_OFFSET_CHARACTERISTIC    0x7FE1

/* Tire Pressure Monitoring Characteritiscs defines */
/* Needs to update the opcode once TPMS spec is adopted  */
#define GATT_TPM_TIRE_PRESSURE_CHARACTERISTIC    0x1111
#define GATT_TPM_TIRE_TEMP_CHARACTERISTIC        0x2222
#define GATT_TPM_PRESSURE_CALIBN_CHARACTERISTIC  0x3333
#define GATT_TPM_TEMP_CALIBN_CHARACTERISTIC      0x4444
#define GATT_TPM_DISRY_CONN_MGMNT_CHARACTERISTIC 0xFFFF
#define GATT_TPM_ADV_PERIOD_CHARACTERISTIC       0xAAA1

/* Transport Discovery Characteristics defines */
#define GATT_TD_CONTROL_POINT_CHARACTERISTIC     0x2ABC

/* HTTP Proxy Characteristics defines */
#define GATT_HPC_URI_CHARACTERISTIC              0x2AB6
#define GATT_HPC_HTTP_HEADERS_CHARACTERISTIC     0x2AB7
#define GATT_HPC_HTTP_ENTITY_BODY_CHARACTERISTIC 0x2AB9
#define GATT_HPC_HTTP_CP_CHARACTERISTIC          0x2ABA
#define GATT_HPC_HTTP_STATUS_CODE_CHARACTERISTIC 0x2AB8
#define GATT_HPC_HTTPS_SECURITY_CHARACTERISTIC   0x2ABB

/* Fitness Machine Characteristics defines */
#define GATT_FTM_FEATURE_CHARACTERISTIC          0x2ACC
#define GATT_FTM_TREADMILL_CHARACTERISTIC        0x2ACD
#define GATT_FTM_CROSS_TRAINER_CHARACTERISTIC    0x2ACE
#define GATT_FTM_STEP_CLIMBER_CHARACTERISTIC     0x2ACF
#define GATT_FTM_STAIR_CLIMBER_CHARACTERISTIC    0x2AD0
#define GATT_FTM_ROWER_DATA_CHARACTERISTIC       0x2AD1
#define GATT_FTM_INDOOR_BIKE_CHARACTERISTIC      0x2AD2
#define GATT_FTM_TRAINING_STATUS_CHARACTERISTIC  0x2AD3
#define GATT_FTM_SUPPD_SPEED_RNG_CHARACTERISTIC  0x2AD4
#define GATT_FTM_SUPPD_INCLN_RNG_CHARACTERISTIC  0x2AD5
#define GATT_FTM_SUPPD_RESIS_RNG_CHARACTERISTIC  0x2AD6
#define GATT_FTM_SUPPD_POWER_RNG_CHARACTERISTIC  0x2AD8
#define GATT_FTM_SUPPD_HRTRE_RNG_CHARACTERISTIC  0x2AD7
#define GATT_FTM_CONTROL_POINT_CHARACTERISTIC    0x2AD9
#define GATT_FTM_MACHINE_STATUS_CHARACTERISTIC   0x2ADA

/* Reconnection Configuration Characteristic defines */
#define GATT_RC_FEATURE_CHARACTERISTIC           0x2B1D
#define GATT_RC_SETTING_CHARACTERISTIC           0x2B1E
#define GATT_RC_CP_CHARACTERISTIC                0x2B1F

/* Insulin Delivery Characteristics defines */
#define GATT_IDD_STATUS_CHANGED_CHARACTERISTIC   0x2B20
#define GATT_IDD_STATUS_CHARACTERISTIC           0x2B21
#define GATT_IDD_ANNUN_STATUS_CHARACTERISTIC     0x2B22
#define GATT_IDD_FEATURE_CHARACTERISTIC          0x2B23
#define GATT_IDD_STATUS_READER_CP_CHARACTERISTIC 0x2B24
#define GATT_IDD_COMMAND_CP_CHARACTERISTIC       0x2B25
#define GATT_IDD_COMMAND_DATA_CHARACTERISTIC     0x2B26
#define GATT_IDD_RACP_CHARACTERISTIC             0x2B27
#define GATT_IDD_HISTORY_DATA_CHARACTERISTIC     0x2B28

/** Characteristic UUID defines of GA profiles/Services Under Development */
/* Volume Control Characteristic defines */
/**
 * Volume State Characteristic                   0x8FB9
 * Volume Control Point Characteristic           0x8FBA
 * Volume Flags Characteristic                   0x8FBB
 */
#define GATT_VC_VOLUME_STATE_CHARACTERISTIC      0x8FB9
#define GATT_VC_VOLUME_CP_CHARACTERISTIC         0x8FBA
#define GATT_VC_VOLUME_FLAG_CHARACTERISTIC       0x8FBB

/* Volume Offset Control Characteristic defines */
/**
 * Offset State Characteristic                   0x8FBC
 * Audio Location Characteristic                 0x8FBD
 * Volume Offset Control Point Characteristic    0x8FBE
 * Audio Output Description Characteristic       0x8FBF
 */
#define GATT_VOC_OFFSET_STATE_CHARACTERISTIC     0x8FBC
#define GATT_VOC_AUDIO_LOCATION_CHARACTERISTIC   0x8FBD
#define GATT_VOC_OFFSET_CP_CHARACTERISTIC        0x8FBE
#define GATT_VOC_AUDIO_OP_DESC_CHARACTERISTIC    0x8FBF

/* Mic Input Control Service Characteristic defines */
/**
 * Mute characteristic                           0x8FE1
 */
#define GATT_MICS_MUTE_CHARACTERISTIC           0x8FE1

/* Audio Input Control Service Characteristic defines */
/**
 * Input State Characteristic                   0x8FE2
 * Gain Settings Attribute Characteristic       0x8FE3
 * Input Type Characteristic                    0x8FE5
 * Input Status Characteristic                  0x8FE6
 * Audio Input Control Point Characteristic     0x8FE7
 * Audio Input Description Characteristic       0x8FE8
 */
#define GATT_AICS_OFFSET_STATE_CHARACTERISTIC     0x8FE2
#define GATT_AICS_GAIN_SETTING_CHARACTERISTIC     0x8FE3
#define GATT_AICS_INPUT_TYPE_CHARACTERISTIC       0x8FE5
#define GATT_AICS_INPUT_STATUS_CHARACTERISTIC     0x8FE6
#define GATT_AICS_AUDIO_INPUT_CP_CHARACTERISTIC   0x8FE7
#define GATT_AICS_AUDIO_INPUT_DESC_CHARACTERISTIC 0x8FE8

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
#define GATT_AC_SNK_PAC_CHARACTERISTIC              0x8F97
#define GATT_AC_SNK_AUDIO_LOC_CHARACTERISTIC        0x8F98
#define GATT_AC_SRC_PAC_CHARACTERISTIC              0x8F99
#define GATT_AC_SRC_AUDIO_LOC_CHARACTERISTIC        0x8F9A

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
#define GATT_TBS_BRR_PROVIDER_NAME                  0x8FEA
#define GATT_TBS_BRR_UCI                            0x8FEB
#define GATT_TBS_BRR_TECHNOLOGY                     0x8FEC
#define GATT_TBS_BRR_URI_PRFX_SUPPORT_LIST          0x8FED
#define GATT_TBS_BRR_SIGNAL_STRENGTH                0x8FEF
#define GATT_TBS_BRR_SIGNAL_STRENGTH_RPRT_INTVL     0x8FF0
#define GATT_TBS_BRR_LIST_CUR_CALLS                 0x8FF1
#define GATT_TBS_CNTNT_CTRL_ID                      0x8FF2
#define GATT_TBS_FEATURE_AND_STATUS_FLAGS           0x8FF3
#define GATT_TBS_INCALL_TGT_BRR_CALLER_ID           0x8FF4
#define GATT_TBS_CALL_STATE                         0x8FF5
#define GATT_TBS_CALL_CTRL_POINT                    0x8FF6
#define GATT_TBS_CALL_CTRL_POINT_SUPPORT_OP         0x8FF7
#define GATT_TBS_TERMINATION_REASON                 0x8FF8
#define GATT_TBS_INCALL_REMOTE_CALLER_ID            0x8FF9
#define GATT_TBS_OUTCALL_REMOTE_CALLER_ID           0x8FFA

/* Routing Active Audio Service Characteristic defines */
/**
 * Available Member List Characteristic                 0x8FC0
 * Available Member Connection Status Characteristic    0x8FC1
 * Default Audio Routes List Characteristic             0x8FC2
 * Current Active Routes List Characteristic            0x8FC3
 * Modify Audio Routing Control Point Characteristic    0x8FC4
 * Max Supported Characteristic                         0x8FC5
 */
#define GATT_RAAS_CHAR_AVLBL_MBR_LIST                   0x8FC0
#define GATT_RAAS_CHAR_AVLBL_MBR_CONTN_STATUS           0x8FC1
#define GATT_RAAS_CHAR_DEFAULT_AUDIO_ROUTES_LIST        0x8FC2
#define GATT_RAAS_CHAR_CURR_ACTIVE_ROUTES_LIST          0x8FC3
#define GATT_RAAS_CHAR_MODIFY_AUDIO_ROUTE_CP            0x8FC4
#define GATT_RAAS_CHAR_MAX_SUPPORTED                    0x8FC5

/* Coordinated Set Identification Service Characteristic defines */
/**
 * Rank Characteristic                                  0x8FCC
 * Set Identity Resolving Key Characteristic            0x8FCD
 * Size Characteristic                                  0x8FCE
 * Lock Characteristic                                  0x8FCF
 */
#define GATT_CSI_CHAR_RANK                              0x8FCC
#define GATT_CSI_CHAR_SET_IDNTY_RESOLVE_KEY             0x8FCD
#define GATT_CSI_CHAR_SIZE                              0x8FCE
#define GATT_CSI_CHAR_LOCK                              0x8FCF

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
#define GATT_MCS_CHAR_MEDIA_PLAYER_NAME                 0x8FA0
#define GATT_MCS_CHAR_MEDIA_ICON_OBJ                    0x8FA1
#define GATT_MCS_CHAR_MEDIA_ICON_URI                    0x8FA2
#define GATT_MCS_CHAR_TRACK_CHANGED                     0x8FA3
#define GATT_MCS_CHAR_TRACK_TITLE                       0x8FA4
#define GATT_MCS_CHAR_TRACK_DURATION                    0x8FA5
#define GATT_MCS_CHAR_TRACK_PROSITION                   0x8FA6
#define GATT_MCS_CHAR_PLAYBACK_SPEED                    0x8FA7
#define GATT_MCS_CHAR_PLAYBACK_SPEED_SUPPORTED          0x8FA8
#define GATT_MCS_CHAR_SEEKING_SPEED                     0x8FA9
#define GATT_MCS_CHAR_TRACK_SEGMNTS_OBJECT              0x8FAA
#define GATT_MCS_CHAR_CURRENT_TRACK_OBJECT              0x8FAB
#define GATT_MCS_CHAR_NEXT_TRACK_OBJECT                 0x8FAC
#define GATT_MCS_CHAR_CURRENT_GROUP_OBJECT              0x8FAD
#define GATT_MCS_CHAR_PLAYING_ORDER                     0x8FAE
#define GATT_MCS_CHAR_PLAYING_ORDER_SUPPORTED           0x8FAF
#define GATT_MCS_CHAR_MEDIA_STATE                       0x8FB0
#define GATT_MCS_CHAR_MEDIA_CP                          0x8FB1
#define GATT_MCS_CHAR_MEDIA_CP_SUPPORTED                0x8FB2
#define GATT_MCS_CHAR_SEARCH_RESULTS_OBJECT             0x8FB3
#define GATT_MCS_CHAR_SEARCH_CP                         0x8FB4
#define GATT_MCS_CHAR_SEARCH_CNTNT_CNTRL_ID             0x8FB5

/* Audio Stream Control Service */
/**
 * Audio Content Availabilty Characteristic             0x8F9B
 * ASE  Characteristic                                  0x8F9C
 * ASE Control Point Characteristic                     0x8F9D
 */
#define GATT_ASCS_CHAR_AUDIO_CNTNT_AVAILBLTY            0x8F9B
#define GATT_ASCS_CHAR_ASE                              0x8F9C
#define GATT_ASCS_CHAR_ASE_CP                           0x8F9D

/* Broadcast Audio Scan Service */
/**
 * Remote Scanning                                      0x8F9E
 * Broadcast Receive State                              0x8F9F
 */
#define GATT_BASS_CHAR_REMOTE_SCAN                      0x8F9E
#define GATT_BASS_CHAR_BRCST_RX_STATE                   0x8F9F

#define ATT_APPL_PROCEDURE_ALREADY_IN_PROGRESS   0x80
#define ATT_APPL_CCD_IMPROPERLY_CONFIGURED       0x81

/* GATT Units */

#endif /* _H_GATT_DEFINES_ */

