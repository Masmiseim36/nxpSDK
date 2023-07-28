/**
 *  \file ga_gatt_db.h
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GATT_DB_GA_
#define _H_GATT_DB_GA_

#ifndef GATT_DB_DYNAMIC
/**
 * addgroup gatt_db_module
 */

/**
 * defgroup gatt_db_tuneable_param Tuneable Parameters
 * {
 * This section defines the Tuneable Constants of Data Base Module.
 */

/** Number of Characteristics in the data base */
#define GATT_CHARACTERISTIC_COUNT            148

/** Number of Services in the data base */
#define GATT_SERVICE_COUNT                   19

/** Number of Characteristics that are configurable by the client */
#define GATT_DB_MAX_CONFIGUREABLE_CHAR       102

/** Maximum Length of any Characteristic Value/Descriptor */
#define GATT_DB_MAX_VAL_LENGTH                32

#define GATT_VALUE_ARRAY_SIZE                 79

#define GATT_CONST_VALUE_ARRAY_SIZE           804

#define GATT_DB_PEER_VALUE_ARRAY_SIZE         203

#define GATT_DB_MAX_ATTRIBUTES                421

#define GATT_UUID_ARRAY_SIZE                  216

#define GATT_DB_MAX_TYPE_COUNT                104

#define GATT_DB_MAX_PEER_CONFIGURATION        \
        (GATT_DB_PEER_VALUE_ARRAY_SIZE * BT_MAX_DEVICE_QUEUE_SIZE)

/** \} */

/** Service Instance Reference */

/** GAP Service */
#define GATT_SER_GAP_INST                                                  0

/** GATT Service */
#define GATT_SER_GATT_INST                                                 1

/** Battery Service */
#define GATT_SER_BATTERY_INST                                              2

/** DeviceInformation Service */
#define GATT_SER_DEV_INFO_INST                                             3

/** Volume Control Service */
#define GATT_SER_VOLUME_CNTL_INST                                          4

/** VolumeOffsetControl Service */
#define GATT_SER_VOLUME_OFFSET_CNTL_INST                                   5

/** AudioInputControl Service */
#define GATT_SER_AUDIO_IP_CONTROL_INST                                     6

/** MicrophoneControlService Service */
#define GATT_SER_MICS_INST                                                 7

/** AudioInputControl Service */
#define GATT_SER_AUDIO_IP_CONTROL_MICS_INST                                8

/** CoordinatedSetIdentificationService Service */
#define GATT_SER_CSI_INST                                                  9

/** CommonAudioService Service */
#define GATT_SER_CAS_INST                                                  10

/** PublishedAudioCapabilityService Service */
#define GATT_SER_PACS_INST                                                 11

/** GenericTelephoneBearerService Service */
#define GATT_SER_GENERIC_TELEPHONE_BRR_INST                                12

/** TelephonyAndMediaAudioService Service */
#define GATT_SER_TMAS_INST                                                 13

/** AudioStreamControlService Service */
#define GATT_SER_ASCS_INST                                                 14

/** GenericMediaControlService Service */
#define GATT_SER_GMCS_INST                                                 15

/** MediaControlService Service */
#define GATT_SER_MCS1_INST                                                 16

/** MediaControlService Service */
#define GATT_SER_MCS2_INST                                                 17

/** TelephoneBearerService Service */
#define GATT_SER_TELEPHONE_BRR_INST1                                       18

/** Characteristic Instance Reference */

/** DeviceName */
#define GATT_CHAR_DEV_NAME_INST                                            0

/** Appearance */
#define GATT_CHAR_APPEARANCE_INST                                          1

/** Service Changed */
#define GATT_CHAR_SER_CHNGD_INST                                           2

/** Client Supported Features */
#define GATT_CHAR_CLIENT_SUPPORTED_INST                                    3

/** Server Supported Features */
#define GATT_CHAR_SERVER_SUPPORTED_INST                                    4

/** BatteryLevel */
#define GATT_CHAR_BATTERY_LVL_INST                                         5

/** ManufacturerName */
#define GATT_CHAR_MAN_NAME_INST                                            6

/** ModelNumber */
#define GATT_CHAR_MODEL_NO_INST                                            7

/** SerialNumber */
#define GATT_CHAR_SL_NO_INST                                               8

/** FirmwareRevision */
#define GATT_CHAR_FW_REV_INST                                              9

/** HardwareRevision */
#define GATT_CHAR_HW_REV_INST                                              10

/** SoftwareRevision */
#define GATT_CHAR_SW_REV_INST                                              11

/** SystemId */
#define GATT_CHAR_SYS_ID_INST                                              12

/** RegCertDataList */
#define GATT_CHAR_REG_CERT_DATA_INST                                       13

/** PnPID */
#define GATT_CHAR_PNP_ID_INST                                              14

/** Volume State */
#define GATT_CHAR_VOLUME_STATE_INST                                        15

/** Volume Control Point */
#define GATT_CHAR_VOLUME_CP_INST                                           16

/** Volume Flag */
#define GATT_CHAR_VOLUME_FLAG_INST                                         17

/** Offset State */
#define GATT_CHAR_OFFSET_STATE_INST                                        18

/** Audio Location */
#define GATT_CHAR_AUDIO_LOC_INST                                           19

/** Volume Offset Control Point */
#define GATT_CHAR_VOL_OFFSET_CP_INST                                       20

/** Audio Output Description */
#define GATT_CHAR_AUDIO_OP_DESPTN_INST                                     21

/** Input State */
#define GATT_CHAR_INPUT_STATE_INST                                         22

/** Gain Setting Properties */
#define GATT_CHAR_GAIN_SETTING_PROTY_INST                                  23

/** Input Type */
#define GATT_CHAR_INPUT_TYPE_INST                                          24

/** Input Status */
#define GATT_CHAR_INPUT_STATUS_INST                                        25

/** Audio Input Control Point */
#define GATT_CHAR_AUDIO_IP_CP_INST                                         26

/** Audio Input Description */
#define GATT_CHAR_AUDIO_IP_DESPTN_INST                                     27

/** Mute */
#define GATT_CHAR_MUTE_INST                                                28

/** Input State */
#define GATT_CHAR_INPUT_STATE_MICS_INST                                    29

/** Gain Setting Properties */
#define GATT_CHAR_GAIN_SETTING_PROTY_MICS_INST                             30

/** Input Type */
#define GATT_CHAR_INPUT_TYPE_MICS_INST                                     31

/** Input Status */
#define GATT_CHAR_INPUT_STATUS_MICS_INST                                   32

/** Audio Input Control Point */
#define GATT_CHAR_AUDIO_IP_CP_MICS_INST                                    33

/** Audio Input Description */
#define GATT_CHAR_AUDIO_IP_DESPTN_MICS_INST                                34

/** Set Identity Resloving Key */
#define GATT_CHAR_SIRK_INST                                                35

/** Coordinated Set Size */
#define GATT_CHAR_SIZE_INST                                                36

/** Set Member Lock */
#define GATT_CHAR_LOCK_INST                                                37

/** Set Member Rank */
#define GATT_CHAR_RANK_INST                                                38

/** Common Audio Profile Role */
#define GATT_CHAR_CAP_ROLE_INST                                            39

/** Sink PAC */
#define GATT_CHAR_SNK_PAC_INST                                             40

/** Sink Audio Locations */
#define GATT_CHAR_SNK_AUDIO_LOCATIONS_INST                                 41

/** Source PAC */
#define GATT_CHAR_SRC_PAC_INST                                             42

/** Source Audio Locations */
#define GATT_CHAR_SRC_AUDIO_LOCATIONS_INST                                 43

/** Available Audio Contexts  */
#define GATT_CHAR_AVAILABLE_AUDIO_CONTEXTS_INST                            44

/** Supported Audio Contexts */
#define GATT_CHAR_SUPP_AUDIO_CONTEXTS_INST                                 45

/** Bearer Provider Name */
#define GATT_CHAR_GTBS_BRR_PROVIDER_NAME_INST                              46

/** Bearer Uniform Caller Identifier (UCI) */
#define GATT_CHAR_GTBS_BRR_UCI_INST                                        47

/** Bearer Technology */
#define GATT_CHAR_GTBS_BRR_TECH_INST                                       48

/** Bearer URI Schemes Supportet List */
#define GATT_CHAR_GTBS_BRR_URI_PRFIX_SUPP_LIST_INST                        49

/** Bearer Signal Strength */
#define GATT_CHAR_GTBS_BRR_SIG_STRENGTH_INST                               50

/** Bearer Signal Strength Reporting Interval */
#define GATT_CHAR_GTBS_BRR_SIG_STRENGTH_RPORTNG_INTVAL_INST                51

/** Bearer List Current Calls */
#define GATT_CHAR_GTBS_BRR_LIST_CURR_CALL_INST                             52

/** Content Control ID (CCID) */
#define GATT_CHAR_GTBS_CONTENT_CNTL_ID_INST                                53

/** Status Flags */
#define GATT_CHAR_GTBS_STATUS_FLAG_INST                                    54

/** Incoming Call Target Bearer URI */
#define GATT_CHAR_GTBS_INCALL_TARG_BRR_CALL_ID_INST                        55

/** Call State */
#define GATT_CHAR_GTBS_CALL_STATE_INST                                     56

/** Call Control Point */
#define GATT_CHAR_GTBS_CALL_CP_INST                                        57

/** Call Control Point Optional Opcodes */
#define GATT_CHAR_GTBS_CALL_CP_OPTIONAL_OPCODES_INST                       58

/** Termination Reason */
#define GATT_CHAR_GTBS_TERMINATION_REASON_INST                             59

/** Incoming Call */
#define GATT_CHAR_GTBS_INCMG_CALL_INST                                     60

/** Call Friendly Name */
#define GATT_CHAR_GTBS_CALL_FRIENDLY_NAME_INST                             61

/** Telephony and Media Audio Profile Role */
#define GATT_CHAR_TMAP_ROLE_INST                                           62

/** Sink ASE */
#define GATT_CHAR_SINK_ASE_INST                                            63

/** Source ASE */
#define GATT_CHAR_SRC_ASE_INST                                             64

/** ASE Control Point */
#define GATT_CHAR_ASE_CP_INST                                              65

/** Media Player Name */
#define GATT_CHAR_GMCS_MP_NAME_INST                                        66

/** Media Player Icon Object */
#define GATT_CHAR_GMCS_MP_ICON_OBJ_INST                                    67

/** Media Player Icon URL */
#define GATT_CHAR_GMCS_MP_ICON_URL_INST                                    68

/** Track Changed */
#define GATT_CHAR_GMCS_TRACK_CHANGED_INST                                  69

/** Track Title */
#define GATT_CHAR_GMCS_TRACK_TITLE_INST                                    70

/** Track Duration */
#define GATT_CHAR_GMCS_TRACK_DURATION_INST                                 71

/** Track Position */
#define GATT_CHAR_GMCS_TRACK_POSITION_INST                                 72

/** Playback Speed */
#define GATT_CHAR_GMCS_PLAYBACK_SPEED_INST                                 73

/** Seeking Speed */
#define GATT_CHAR_GMCS_SEEKING_SPEED_INST                                  74

/** Current Track Segments Object ID */
#define GATT_CHAR_GMCS_CUR_TRACK_SEG_OBJ_ID_INST                           75

/** Current Track Object ID */
#define GATT_CHAR_GMCS_CURR_TRACK_OBJ_ID_INST                              76

/** Next Track Object ID  */
#define GATT_CHAR_GMCS_NEXT_TRACK_OBJ_ID_INST                              77

/** Parent Group Object ID */
#define GATT_CHAR_GMCS_PARENT_GRP_OBJ_ID_INST                              78

/** Current Group Object ID */
#define GATT_CHAR_GMCS_CURR_GROUP_OBJ_ID_INST                              79

/** Playing Order */
#define GATT_CHAR_GMCS_PLAY_ORDER_INST                                     80

/** Playing Order Supported */
#define GATT_CHAR_GMCS_PLAY_ORDER_SUPPORTED_INST                           81

/** Media State */
#define GATT_CHAR_MEDIA_STATE_INST                                         82

/** Media Control Point */
#define GATT_CHAR_GMCS_MEDIA_CP_INST                                       83

/** Media Control Point Opcodes Supported */
#define GATT_CHAR_GMCS_MEDIA_CP_OP_SUPPORTED_INST                          84

/** Search Results Object ID */
#define GATT_CHAR_GMCS_SEARCH_RESULTS_OBJ_ID_INST                          85

/** Search Control Point */
#define GATT_CHAR_GMCS_SEARCH_CP_INST                                      86

/** Content Control ID */
#define GATT_CHAR_GMCS_CONTENT_CNTL_ID_INST                                87

/** Media Player Name */
#define GATT_CHAR_MCS1_MP_NAME_INST                                        88

/** Media Player Icon Object */
#define GATT_CHAR_MCS1_MP_ICON_OBJ_INST                                    89

/** Media Player Icon URL */
#define GATT_CHAR_MCS1_MP_ICON_URL_INST                                    90

/** Track Changed */
#define GATT_CHAR_MCS1_TRACK_CHANGED_INST                                  91

/** Track Title */
#define GATT_CHAR_MCS1_TRACK_TITLE_INST                                    92

/** Track Duration */
#define GATT_CHAR_MCS1_TRACK_DURATION_INST                                 93

/** Track Position */
#define GATT_CHAR_MCS1_TRACK_POSITION_INST                                 94

/** Playback Speed */
#define GATT_CHAR_MCS1_PLAYBACK_SPEED_INST                                 95

/** Seeking Speed */
#define GATT_CHAR_MCS1_SEEKING_SPEED_INST                                  96

/** Current Track Segments Object ID */
#define GATT_CHAR_MCS1_CUR_TRACK_SEG_OBJ_ID_INST                           97

/** Current Track Object ID */
#define GATT_CHAR_MCS1_CURR_TRACK_OBJ_ID_INST                              98

/** Next Track Object ID  */
#define GATT_CHAR_MCS1_NEXT_TRACK_OBJ_ID_INST                              99

/** Parent Group Object ID */
#define GATT_CHAR_MCS1_PARENT_GRP_OBJ_ID_INST                              100

/** Current Group Object ID */
#define GATT_CHAR_MCS1_CURR_GROUP_OBJ_ID_INST                              101

/** Playing Order */
#define GATT_CHAR_MCS1_PLAY_ORDER_INST                                     102

/** Playing Order Supported */
#define GATT_CHAR_MCS1_PLAY_ORDER_SUPPORTED_INST                           103

/** Media State */
#define GATT_CHAR_MCS1_MEDIA_STATE_INST                                    104

/** Media Control Point */
#define GATT_CHAR_MCS1_MEDIA_CP_INST                                       105

/** Media Control Point Opcodes Supported */
#define GATT_CHAR_MCS1_MEDIA_CP_OP_SUPPORTED_INST                          106

/** Search Results Object ID */
#define GATT_CHAR_MCS1_SEARCH_RESULTS_OBJ_ID_INST                          107

/** Search Control Point */
#define GATT_CHAR_MCS1_SEARCH_CP_INST                                      108

/** Content Control ID */
#define GATT_CHAR_MCS1_CONTENT_CNTL_ID_INST                                109

/** Media Player Name */
#define GATT_CHAR_MCS2_MP_NAME_INST                                        110

/** Media Player Icon Object */
#define GATT_CHAR_MCS2_MP_ICON_OBJ_INST                                    111

/** Media Player Icon URL */
#define GATT_CHAR_MCS2_MP_ICON_URL_INST                                    112

/** Track Changed */
#define GATT_CHAR_MCS2_TRACK_CHANGED_INST                                  113

/** Track Title */
#define GATT_CHAR_MCS2_TRACK_TITLE_INST                                    114

/** Track Duration */
#define GATT_CHAR_MCS2_TRACK_DURATION_INST                                 115

/** Track Position */
#define GATT_CHAR_MCS2_TRACK_POSITION_INST                                 116

/** Playback Speed */
#define GATT_CHAR_MCS2_PLAYBACK_SPEED_INST                                 117

/** Seeking Speed */
#define GATT_CHAR_MCS2_SEEKING_SPEED_INST                                  118

/** Current Track Segments Object ID */
#define GATT_CHAR_MCS2_CUR_TRACK_SEG_OBJ_ID_INST                           119

/** Current Track Object ID */
#define GATT_CHAR_MCS2_CURR_TRACK_OBJ_ID_INST                              120

/** Next Track Object ID  */
#define GATT_CHAR_MCS2_NEXT_TRACK_OBJ_ID_INST                              121

/** Parent Group Object ID */
#define GATT_CHAR_MCS2_PARENT_GRP_OBJ_ID_INST                              122

/** Current Group Object ID */
#define GATT_CHAR_MCS2_CURR_GROUP_OBJ_ID_INST                              123

/** Playing Order */
#define GATT_CHAR_MCS2_PLAY_ORDER_INST                                     124

/** Playing Order Supported */
#define GATT_CHAR_MCS2_PLAY_ORDER_SUPPORTED_INST                           125

/** Media State */
#define GATT_CHAR_MCS2_MEDIA_STATE_INST                                    126

/** Media Control Point */
#define GATT_CHAR_MCS2_MEDIA_CP_INST                                       127

/** Media Control Point Opcodes Supported */
#define GATT_CHAR_MCS2_MEDIA_CP_OP_SUPPORTED_INST                          128

/** Search Results Object ID */
#define GATT_CHAR_MCS2_SEARCH_RESULTS_OBJ_ID_INST                          129

/** Search Control Point */
#define GATT_CHAR_MCS2_SEARCH_CP_INST                                      130

/** Content Control ID */
#define GATT_CHAR_MCS2_CONTENT_CNTL_ID_INST                                131

/** Bearer Provider Name */
#define GATT_CHAR_TBS_BRR_PROVIDER_NAME_INST1                              132

/** Bearer Uniform Caller Identifier(UCI) */
#define GATT_CHAR_TBS_BRR_UCI_INST1                                        133

/** Bearer Technology */
#define GATT_CHAR_TBS_BRR_TECH_INST1                                       134

/** Bearer URI Schemes Supported List */
#define GATT_CHAR_TBS_BRR_URI_PRFIX_SUPP_LIST_INST1                        135

/** Bearer Signal Strength */
#define GATT_CHAR_TBS_BRR_SIG_STRENGTH_INST1                               136

/** Bearer Signal Strength Reporting Interval */
#define GATT_CHAR_TBS_BRR_SIG_STRENGTH_RPORTNG_INTVAL_INST1                137

/** Bearer List Current Calls */
#define GATT_CHAR_TBS_BRR_LIST_CURR_CALL_INST1                             138

/** Content Control ID(CCID) */
#define GATT_CHAR_TBS_CONTENT_CNTL_ID_INST1                                139

/** Status Flags */
#define GATT_CHAR_TBS_STATUS_FLAG_INST1                                    140

/** Incoming Call Target Bearer URI */
#define GATT_CHAR_TBS_INCALL_TARG_BRR_CALL_ID_INST1                        141

/** Call State */
#define GATT_CHAR_TBS_CALL_STATE_INST1                                     142

/** Call Control Point */
#define GATT_CHAR_TBS_CALL_CP_INST1                                        143

/** Call Control Point Optional Opcodes */
#define GATT_CHAR_TBS_CALL_CP_OPTIONAL_OPCODES_INST1                       144

/** Termination Reason */
#define GATT_CHAR_TBS_TERMINATION_REASON_INST1                             145

/** Incoming Call */
#define GATT_CHAR_TBS_INCMG_CALL_INST1                                     146

/** Call Friendly Name */
#define GATT_CHAR_TBS_CALL_FRIENDLY_NAME_INST1                             147

#else

/* Service Instances */
extern UINT16 GATT_SER_GAP_INST;
extern UINT16 GATT_SER_GATT_INST;
extern UINT16 GATT_SER_BATTERY_INST;
extern UINT16 GATT_SER_DEV_INFO_INST;

/* Characteristic Instances */
extern UINT16 GATT_CHAR_BATTERY_LVL_INST;

API_RESULT appl_ga_add_gaps(void);
API_RESULT appl_ga_add_gatts(void);
API_RESULT appl_ga_add_bas(void);
API_RESULT appl_ga_add_dis(void);

#endif /* GATT_DB_DYNAMIC */
#endif /* _H_GATT_DB_GA_ */

