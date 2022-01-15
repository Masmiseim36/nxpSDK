
/**
 *  \file appl_hci.c
 *
 *  Source File for HCI Command Line Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_utils.h"
#include "appl_hci.h"

#ifdef BT_LE
#include "appl_hci_le.h"
#endif /* BT_LE */

#ifdef BT_VENDOR_SPECIFIC_INIT
#include "vendor_specific_init.h"
#endif /* BT_VENDOR_SPECIFIC_INIT */

#ifdef HCI_SCO
/* For eSCO audio with mSBC codec */
#include "sbc_api.h"

/* For MIC recording & SPEAKER playback */
#include "hfp_sco_audio_pl.h"
#endif /* HCI_SCO */

/* Configure logs for the functions in this file. */
#ifdef IOT_LOG_LEVEL_GLOBAL
    #define LIBRARY_LOG_LEVEL    IOT_LOG_LEVEL_GLOBAL
#else
    #define LIBRARY_LOG_LEVEL    IOT_LOG_NONE
#endif

#define LIBRARY_LOG_NAME         ( "APPL_HCI" )
#include "iot_logging_setup.h"

/* ----------------------------------------- External Global Variables */

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */
#ifndef APPL_HCI_NO_PROFILE_EVENT_IND
/** Array of HCI Profile Event Indication Callbacks */
DECL_STATIC APPL_HCI_CB_PTR appl_hci_cb_list [ APPL_MAX_NUM_HCI_CB ];

/** Running Counter of Number of HCI Profile Event Indication CB Registered */
UCHAR appl_num_hci_cb;
#endif /* APPL_HCI_NO_PROFILE_EVENT_IND */

#ifndef HCI_NO_ESCO_AUTO_ACCEPT
#ifdef BT_HCI_1_2
/*
 * If this flag is set as BT_TRUE,
 * application will set eSCO parameters
 */
DECL_STATIC UCHAR appl_hci_set_esco_connect_param;

/*
 *  To store eSCO Channel Parameters to be used while responding to
 *  eSCO Connection Request from Host Controller with Synchronous
 *  Connection Accept Command
 */
DECL_STATIC HCI_SCO_IN_PARAMS appl_esco_params;

#ifdef HCI_ENH_SCO
/* Enhanced setup synchronous connection parameters */
DECL_STATIC HCI_ENH_SCO_PARAMS appl_enh_sco_params;
#endif /* HCI_ENH_SCO */

/* Synchronous connection auto accept option */
DECL_STATIC UCHAR appl_hci_sync_conn_accept_auto_rsp;

/* Synchronous connection reject reason */
DECL_STATIC UCHAR appl_hci_sync_conn_reject_reason;
#endif /* BT_HCI_1_2 */
#endif /* HCI_NO_ESCO_AUTO_ACCEPT */

#ifdef HCI_SCO
DECL_STATIC UINT16                 sco_connection_handle;

DECL_STATIC UCHAR                  sco_tx_pcm_data[256];
DECL_STATIC UINT16                 sco_tx_pcm_data_offset;
DECL_STATIC UCHAR                  sco_tx_msbc_frame_seq_num;

DECL_STATIC SBC_ENCODER_IO_STRUCT  msbc_enc_io;
DECL_STATIC SBC_DECODER_IO_STRUCT  msbc_dec_io;

DECL_STATIC UINT16                 msbc_enc_output_frame_len;
DECL_STATIC HFP_SCO_AUDIO_EP_INFO  sco_audio_ep_mic_info;
DECL_STATIC HFP_SCO_AUDIO_EP_INFO  sco_audio_ep_spkr_info;

DECL_STATIC UCHAR                  sco_rx_msbc_data[64];
DECL_STATIC UCHAR                  sco_rx_msbc_data_offset;
DECL_STATIC UCHAR                  expected_sco_rx_msbc_frame_seq_num;

DECL_STATIC UINT16 actual;
#ifdef HAVE_SCO_MIC_DATA_DUMP
extern BT_fops_file_handle  mic_fptr;
#endif /* HAVE_SCO_MIC_DATA_DUMP */

#ifdef HAVE_SCO_SPKR_DATA_DUMP
extern BT_fops_file_handle  spkr_fptr;
#endif /* HAVE_SCO_SPKR_DATA_DUMP */

#ifdef HAVE_SCO_TX_DATA_DUMP
extern BT_fops_file_handle  sco_tx_fptr;
#endif /* HAVE_SCO_TX_DATA_DUMP */

#ifdef HAVE_SCO_RX_DATA_DUMP
extern BT_fops_file_handle  sco_rx_fptr;
#endif /* HAVE_SCO_RX_DATA_DUMP */

#endif /* HCI_SCO */

DECL_STATIC const APPL_HCI_OPCODE_NAME opcode_name_table[] =
{
    /* No Operation Opcode */
    {0x0000, "HCI_NO_OPERATION_OPCODE"                                 },

    /* Link Control Commands - OGF : 0x01 */
    {0x0401, "HCI_INQUIRY_OPCODE"                                      },
    {0x0402, "HCI_INQUIRY_CANCEL_OPCODE"                               },
    {0x0403, "HCI_PERIODIC_INQUIRY_MODE_OPCODE"                        },
    {0x0404, "HCI_EXIT_PERIODIC_INQUIRY_MODE_OPCODE"                   },
    {0x0405, "HCI_CREATE_CONNECTION_OPCODE"                            },
    {0x0406, "HCI_DISCONNECT_OPCODE"                                   },
    {0x0407, "HCI_ADD_SCO_CONNECTION_OPCODE"                           },
    {0x0409, "HCI_ACCEPT_CONNECTION_REQUEST_OPCODE"                    },
    {0x040A, "HCI_REJECT_CONNECTION_REQUEST_OPCODE"                    },
    {0x040B, "HCI_LINK_KEY_REQUEST_REPLY_OPCODE"                       },
    {0x040C, "HCI_LINK_KEY_REQUEST_NEGATIVE_REPLY_OPCODE"              },
    {0x040D, "HCI_PIN_CODE_REQUEST_REPLY_OPCODE"                       },
    {0x040E, "HCI_PIN_CODE_REQUEST_NEGATIVE_REPLY_OPCODE"              },
    {0x040F, "HCI_CHANGE_CONNECTION_PACKET_TYPE_OPCODE"                },
    {0x0411, "HCI_AUTHENTICATION_REQUESTED_OPCODE"                     },
    {0x0413, "HCI_SET_CONNECTION_ENCRYPTION_OPCODE"                    },
    {0x0415, "HCI_CHANGE_CONNECTION_LINK_KEY_OPCODE"                   },
    {0x0417, "HCI_MASTER_LINK_KEY_OPCODE"                              },
    {0x0419, "HCI_REMOTE_NAME_REQUEST_OPCODE"                          },
    {0x041B, "HCI_READ_REMOTE_SUPPORTED_FEATURES_OPCODE"               },
    {0x041D, "HCI_READ_REMOTE_VERSION_INFORMATION_OPCODE"              },
    {0x041F, "HCI_READ_CLOCK_OFFSET_OPCODE"                            },

#ifdef BT_HCI_1_2
    {0x0408, "HCI_CREATE_CONNECTION_CANCEL_OPCODE"                     },
    {0x041A, "HCI_REMOTE_NAME_REQUEST_CANCEL_OPCODE"                   },
    {0x041C, "HCI_READ_REMOTE_EXTENDED_FEATURES_OPCODE"                },
    {0x0428, "HCI_SETUP_SYNCHRONOUS_CONNECTION_OPCODE"                 },
    {0x0429, "HCI_ACCEPT_SYNCHRONOUS_CONNECTION_REQUEST_OPCODE"        },
    {0x042A, "HCI_REJECT_SYNCHRONOUS_CONNECTION_REQUEST_OPCODE"        },
#endif /* BT_HCI_1_2 */

    /* Link Policy Commands - OGF : 0x02 */
    {0x0801, "HCI_HOLD_MODE_OPCODE"                                    },
    {0x0803, "HCI_SNIFF_MODE_OPCODE"                                   },
    {0x0804, "HCI_EXIT_SNIFF_MODE_OPCODE"                              },
    {0x0805, "HCI_PARK_MODE_OPCODE"                                    },
    {0x0806, "HCI_EXIT_PARK_MODE_OPCODE"                               },
    {0x0807, "HCI_QOS_SETUP_OPCODE"                                    },
    {0x0809, "HCI_ROLE_DISCOVERY_OPCODE"                               },
    {0x080B, "HCI_SWITCH_ROLE_OPCODE"                                  },
    {0x080C, "HCI_READ_LINK_POLICY_SETTINGS_OPCODE"                    },
    {0x080D, "HCI_WRITE_LINK_POLICY_SETTINGS_OPCODE"                   },

#ifdef BT_HCI_1_2
    {0x080E, "HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_OPCODE"            },
    {0x080F, "HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_OPCODE"           },
    {0x0810, "HCI_FLOW_SPECIFICATION_OPCODE"                           },
#endif /* BT_HCI_1_2 */

    /* Host Controller & BaseBand Commands - OGF : 0x03 */
    {0x0C01, "HCI_SET_EVENT_MASK_OPCODE"                               },
    {0x0C03, "HCI_RESET_OPCODE"                                        },
    {0x0C05, "HCI_SET_EVENT_FILTER_OPCODE"                             },
    {0x0C08, "HCI_FLUSH_OPCODE"                                        },
    {0x0C09, "HCI_READ_PIN_TYPE_OPCODE"                                },
    {0x0C0A, "HCI_WRITE_PIN_TYPE_OPCODE"                               },
    {0x0C0B, "HCI_CREATE_NEW_UNIT_KEY_OPCODE"                          },
    {0x0C13, "HCI_CHANGE_LOCAL_NAME_OPCODE"                            },
    {0x0C14, "HCI_READ_LOCAL_NAME_OPCODE"                              },
    {0x0C15, "HCI_READ_CONNECTION_ACCEPT_TIMEOUT_OPCODE"               },
    {0x0C16, "HCI_WRITE_CONNECTION_ACCEPT_TIMEOUT_OPCODE"              },
    {0x0C17, "HCI_READ_PAGE_TIMEOUT_OPCODE"                            },
    {0x0C18, "HCI_WRITE_PAGE_TIMEOUT_OPCODE"                           },
    {0x0C19, "HCI_READ_SCAN_ENABLE_OPCODE"                             },
    {0x0C1A, "HCI_WRITE_SCAN_ENABLE_OPCODE"                            },
    {0x0C1B, "HCI_READ_PAGE_SCAN_ACTIVITY_OPCODE"                      },
    {0x0C1C, "HCI_WRITE_PAGE_SCAN_ACTIVITY_OPCODE"                     },
    {0x0C1D, "HCI_READ_INQUIRY_SCAN_ACTIVITY_OPCODE"                   },
    {0x0C1E, "HCI_WRITE_INQUIRY_SCAN_ACTIVITY_OPCODE"                  },
    {0x0C1F, "HCI_READ_AUTHENTICATION_ENABLE_OPCODE"                   },
    {0x0C20, "HCI_WRITE_AUTHENTICATION_ENABLE_OPCODE"                  },
    {0x0C21, "HCI_READ_ENCRYPTION_MODE_OPCODE"                         },
    {0x0C22, "HCI_WRITE_ENCRYPTION_MODE_OPCODE"                        },
    {0x0C23, "HCI_READ_CLASS_OF_DEVICE_OPCODE"                         },
    {0x0C24, "HCI_WRITE_CLASS_OF_DEVICE_OPCODE"                        },
    {0x0C25, "HCI_READ_VOICE_SETTING_OPCODE"                           },
    {0x0C26, "HCI_WRITE_VOICE_SETTING_OPCODE"                          },
    {0x0C27, "HCI_READ_AUTOMATIC_FLUSH_TIMEOUT_OPCODE"                 },
    {0x0C28, "HCI_WRITE_AUTOMATIC_FLUSH_TIMEOUT_OPCODE"                },
    {0x0C29, "HCI_READ_NUM_BROADCAST_RETRANSMISSIONS_OPCODE"           },
    {0x0C2A, "HCI_WRITE_NUM_BROADCAST_RETRANSMISSIONS_OPCODE"          },
    {0x0C2B, "HCI_READ_HOLD_MODE_ACTIVITY_OPCODE"                      },
    {0x0C2C, "HCI_WRITE_HOLD_MODE_ACTIVITY_OPCODE"                     },
    {0x0C2D, "HCI_READ_TRANSMIT_POWER_LEVEL_OPCODE"                    },
    {0x0C2E, "HCI_READ_SCO_FLOW_CONTROL_ENABLE_OPCODE"                 },
    {0x0C2F, "HCI_WRITE_SCO_FLOW_CONTROL_ENABLE_OPCODE"                },
    {0x0C31, "HCI_SET_HOST_CONTROLLER_TO_HOST_FLOW_CONTROL_OPCODE"     },
    {0x0C33, "HCI_HOST_BUFFER_SIZE_OPCODE"                             },
    {0x0C35, "HCI_HOST_NUMBER_OF_COMPLETED_PACKETS_OPCODE"             },
    {0x0C36, "HCI_READ_LINK_SUPERVISION_TIMEOUT_OPCODE"                },
    {0x0C37, "HCI_WRITE_LINK_SUPERVISION_TIMEOUT_OPCODE"               },
    {0x0C38, "HCI_READ_NUMBER_OF_SUPPORTED_IAC_OPCODE"                 },
    {0x0C39, "HCI_READ_CURRENT_IAC_LAP_OPCODE"                         },
    {0x0C3A, "HCI_WRITE_CURRENT_IAC_LAP_OPCODE"                        },
    {0x0C3B, "HCI_READ_PAGE_SCAN_PERIOD_MODE_OPCODE"                   },
    {0x0C3C, "HCI_WRITE_PAGE_SCAN_PERIOD_MODE_OPCODE"                  },
    {0x0C3D, "HCI_READ_PAGE_SCAN_MODE_OPCODE"                          },
    {0x0C3E, "HCI_WRITE_PAGE_SCAN_MODE_OPCODE"                         },

#ifdef BT_HCI_1_2
    {0x0C3F, "HCI_SET_AFH_HOST_CHANNEL_CLASSIFICATION_OPCODE"          },
    {0x0C42, "HCI_READ_INQUIRY_SCAN_TYPE_OPCODE"                       },
    {0x0C43, "HCI_WRITE_INQUIRY_SCAN_TYPE_OPCODE"                      },
    {0x0C44, "HCI_READ_INQUIRY_MODE_OPCODE"                            },
    {0x0C45, "HCI_WRITE_INQUIRY_MODE_OPCODE"                           },
    {0x0C46, "HCI_READ_PAGE_SCAN_TYPE_OPCODE"                          },
    {0x0C47, "HCI_WRITE_PAGE_SCAN_TYPE_OPCODE"                         },
    {0x0C48, "HCI_READ_AFH_CHANNEL_ASSESSMENT_MODE_OPCODE"             },
    {0x0C49, "HCI_WRITE_AFH_CHANNEL_ASSESSMENT_MODE_OPCODE"            },
#endif /* BT_HCI_1_2 */

#ifdef BT_2_1_EDR
    {0x0C51, "HCI_READ_EXTENDED_INQUIRY_RESPONSE_OPCODE"               },
    {0x0C52, "HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_OPCODE"              },
    {0x0C53, "HCI_REFRESH_ENCRYPTION_KEY_OPCODE"                       },
    {0x0C55, "HCI_READ_SIMPLE_PAIRING_MODE_OPCODE"                     },
    {0x0C56, "HCI_WRITE_SIMPLE_PAIRING_MODE_OPCODE"                    },
    {0x0C57, "HCI_READ_LOCAL_OOB_DATA_OPCODE"                          },
    {0x0C58, "HCI_READ_INQUIRY_RESP_TX_POWER_LEVEL_OPCODE"             },
    {0x0C59, "HCI_WRITE_INQUIRY_RESP_TX_POWER_LEVEL_OPCODE"            },
    {0x0C5F, "HCI_ENHANCED_FLUSH_OPCODE"                               },
    {0x0C5A, "HCI_READ_DEFAULT_ERR_DATA_REPORTING_OPCODE"              },
    {0x0C5B, "HCI_WRITE_DEFAULT_ERR_DATA_REPORTING_OPCODE"             },
    {0x0C60, "HCI_SEND_KEY_PRESS_NOTIFICATION_OPCODE"                  },
#endif /* BT_2_1_EDR */

#ifdef BT_3_0
#ifdef HCI_FLOW_CONTROL_COMMANDS
    {0x0C66, "HCI_READ_FLOW_CONTROL_MODE_OPCODE"                       },
    {0x0C67, "HCI_WRITE_FLOW_CONTROL_MODE_OPCODE"                      },
#endif /* HCI_FLOW_CONTROL_COMMANDS */

    {0x0C68, "HCI_READ_ENHANCE_TRANSMIT_POWER_LEVEL_OPCODE"            },
#endif /* BT_3_0 */

#ifdef BT_BRSC
    {0x0C79, "HCI_READ_SECURE_CONNECTIONS_HOST_SUPPORT_OPCODE"         },
    {0x0C7A, "HCI_WRITE_SECURE_CONNECTIONS_HOST_SUPPORT_OPCODE"        },
#endif /* BT_BRSC */

    /* Informational Parameters - OGF : 0x04 */
    {0x1001, "HCI_READ_LOCAL_VERSION_INFORMATION_OPCODE"               },
    {0x1002, "HCI_READ_LOCAL_SUPPORTED_COMMANDS_OPCODE"                },
    {0x1003, "HCI_READ_LOCAL_SUPPORTED_FEATURES_OPCODE"                },
    {0x1005, "HCI_READ_BUFFER_SIZE_OPCODE"                             },
    {0x1007, "HCI_READ_COUNTRY_CODE_OPCODE"                            },
    {0x1009, "HCI_READ_BD_ADDR_OPCODE"                                 },

#ifdef BT_HCI_1_2
    {0x1002, "HCI_READ_LOCAL_SUPPORTED_COMMANDS_OPCODE"                },
    {0x1004, "HCI_READ_LOCAL_EXTENDED_FEATURES_OPCODE"                 },
#endif /* BT_HCI_1_2 */

    /* Status Parameters - OGF : 0x05 */
    {0x1401, "HCI_READ_FAILED_CONTACT_COUNTER_OPCODE"                  },
    {0x1402, "HCI_RESET_FAILED_CONTACT_COUNTER_OPCODE"                 },
    {0x1403, "HCI_GET_LINK_QUALITY_OPCODE"                             },
    {0x1405, "HCI_READ_RSSI_OPCODE"                                    },

#ifdef BT_HCI_1_2
    {0x1406, "HCI_READ_AFH_CHANNEL_MAP_OPCODE"                         },
    {0x1407, "HCI_READ_CLOCK_OPCODE"                                   },
#endif /* BT_HCI_1_2 */

#ifdef BT_3_0
#ifndef HCI_LITE
    {0x1408, "HCI_READ_ENCRYPTION_KEY_SIZE_OPCODE"                     },
#endif /* HCI_LITE */
#endif /* BT_3_0 */

    /* Bluetooth Low Energy Commands */
#ifdef BT_4_0
    {0x2001, "HCI_LE_SET_EVENT_MASK_OPCODE"                            },
    {0x2002, "HCI_LE_READ_BUFFER_SIZE_OPCODE"                          },
    {0x2003, "HCI_LE_READ_LOCAL_SUPPORTED_FEATURES_OPCODE"             },
    {0x2005, "HCI_LE_SET_RANDOM_ADDRESS_OPCODE"                        },
    {0x2006, "HCI_LE_SET_ADVERTISING_PARAMETERS_OPCODE"                },
    {0x2007, "HCI_LE_READ_ADVERTISING_CHANNEL_TX_POWER_OPCODE"         },
    {0x2008, "HCI_LE_SET_ADVERTISING_DATA_OPCODE"                      },
    {0x2009, "HCI_LE_SET_SCAN_RESPONSE_DATA_OPCODE"                    },
    {0x200A, "HCI_LE_SET_ADVERTISING_ENABLE_OPCODE"                    },
    {0x200B, "HCI_LE_SET_SCAN_PARAMETERS_OPCODE"                       },
    {0x200C, "HCI_LE_SET_SCAN_ENABLE_OPCODE"                           },
    {0x200D, "HCI_LE_CREATE_CONNECTION_OPCODE"                         },
    {0x200E, "HCI_LE_CREATE_CONNECTION_CANCEL_OPCODE"                  },
    {0x200F, "HCI_LE_READ_WHITE_LIST_SIZE_OPCODE"                      },
    {0x2010, "HCI_LE_CLEAR_WHITE_LIST_OPCODE"                          },
    {0x2011, "HCI_LE_ADD_DEVICE_TO_WHITE_LIST_OPCODE"                  },
    {0x2012, "HCI_LE_REMOVE_DEVICE_FROM_WHITE_LIST_OPCODE"             },
    {0x2013, "HCI_LE_CONNECTION_UPDATE_OPCODE"                         },
    {0x2014, "HCI_LE_SET_HOST_CHANNEL_CLASSIFICATION_OPCODE"           },
    {0x2015, "HCI_LE_READ_CHANNEL_CLASSIFICATION_OPCODE"               },
    {0x2016, "HCI_LE_READ_REMOTE_USED_FEATURES_OPCODE"                 },
    {0x2017, "HCI_LE_ENCRYPT_OPCODE"                                   },
    {0x2018, "HCI_LE_RAND_OPCODE"                                      },
    {0x2019, "HCI_LE_START_ENCRYPTION_OPCODE"                          },
    {0x201A, "HCI_LE_LONG_TERM_KEY_REQUESTED_REPLY_OPCODE"             },
    {0x201B, "HCI_LE_LONG_TERM_KEY_REQUESTED_NEGATIVE_REPLY_OPCODE"    },
    {0x201C, "HCI_LE_READ_SUPPORTED_STATES_OPCODE"                     },
    {0x201D, "HCI_LE_RECEIVER_TEST_COMMAND_OPCODE"                     },
    {0x201E, "HCI_LE_TRANSMITTER_TEST_COMMAND_OPCODE"                  },
    {0x201F, "HCI_LE_TEST_END_OPCODE"                                  },
#endif /* BT_4_0 */

#ifdef HCI_ENH_SCO
    /* HCI opcode for enhanced SCO  */
    {0x100B, "HCI_READ_LOCAL_SUPPORTED_CODECS_OPCODE"                 },
    {0x043D, "HCI_SETUP_ENHANCED_SYNCHRONOUS_CONNECTION_OPCODE"       },
    {0x043E, "HCI_ACCEPT_ENHANCED_SYNCHRONOUS_CONNECTION_OPCODE"      },
#endif /* HCI_ENH_SCO */

    /* Testing Commands - OGF : 0x06 */
    {0X1801, "HCI_READ_LOOPBACK_MODE_OPCODE"                           },
    {0X1802, "HCI_WRITE_LOOPBACK_MODE_OPCODE"                          },
    {0X1803, "HCI_ENABLE_DEVICE_UNDER_TEST_MODE_OPCODE"                },

#ifdef BT_BRSC_TEST
    {0x180A, "HCI_WRITE_SECURE_CONNECTIONS_TEST_MODE_OPCODE"           }
#endif /* BT_BRSC_TEST */
};


DECL_STATIC const char hci_options[] = "\
========= HCI Menu ================== \n\
     0. Exit. \n\
     1. Refresh \n\
 \n\
     2. Read Local BD_ADDR. \n\
 \n\
     3. Inquiry. \n\
     4. Write Scan Enable.\n\
 \n\
    10. Create Connection (ACL). \n\
    11. Add SCO Connection. \n\
    12. Disconnection. \n\
    13. Remote Name Request. \n\
 \n\
    15. Setup Synchronous Connection. \n\
    16. Accept/Reject Synchronous Connection Request. \n\
    17. Enhanced Setup Synchronous Connection. \n\
    18. Accept/Reject Enhanced Setup Synchronous Connection Request. \n\
    19. Set Synchronous Connection Request Auto Response. \n\
 \n\
    20. Get HCI Connection Details. \n\
 \n\
    25. Read Local Supported Codecs. \n\
    26. Read Local Supported Features. \n\
    27. Read Local Version Information. \n\
    28. Read Remote Version and Supported Features. \n\
 \n\
    30. Role Discovery. \n\
    31. Switch Role. \n\
    32. Sniff Mode. \n\
    33. Park Mode. \n\
    34. Hold Mode. \n\
    35. Exit Sniff Mode. \n\
    36. Exit Park Mode. \n\
    37. Write Link Policy. \n\
 \n\
    40. Write Connection Accept Timeout.\n\
    41. Write Page Timeout.\n\
    42. Write Page Scan Activity.\n\
    43. Write Inquiry Scan Activity.\n\
    44. Write Class of Device.\n\
    45. Write Local Name.\n\
 \n\
    50. Write Inquiry Mode. \n\
    51. Write Inquiry Scan Type. \n\
    52. Write Page Scan Type. \n\
    53. Write Flow Control Mode. \n\
    54. Read current IAC LAP. \n\
    55. Write current IAC LAP. \n\
 \n\
    60. Read Voice Settings. \n\
    61. Read Enhanced Transmit Power Level. \n\
    62. Read Encryption Key Size. \n\
 \n\
    65. Read Secure Connections Host Support. \n\
    66. Write Secure Connections Host Support. \n\
    67. Write Secure Connections Test Mode. \n\
 \n\
    70. Write Extended Inquiry Response. \n\
 \n\
    80. LE Operations. \n\
 \n\
Your Option ? \0";


/* ----------------------------------------- Functions */

API_RESULT appl_hci_event_indication_callback
           (
               UCHAR  event_type,
               UCHAR  * event_data,
               UCHAR  event_datalen
           )
{
    UINT32 count, value_4, value_3;
    UINT16 connection_handle, value_2;
    UCHAR  status, num_responses, value_1, link_type;
    UCHAR * bd_addr;

    UCHAR * revent_data;
    UCHAR revent_datalen;

    /* Store event data and datalen */
    revent_data = event_data;
    revent_datalen = event_datalen;

    /* Switch on the Event Code */
    switch (event_type)
    {
    case HCI_INQUIRY_COMPLETE_EVENT:
        IotLogDebug("Received HCI_INQUIRY_COMPLETE_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        IotLogDebug("\tStatus: 0x%02X\n", status);
        (void)status;    /*fix build warning: set but never used.*/

        break;

    case HCI_INQUIRY_RESULT_EVENT:
        IotLogDebug("Received HCI_INQUIRY_RESULT_EVENT.\n");

        /* Number of Responses */
        hci_unpack_1_byte_param(&num_responses, event_data);
        IotLogDebug("\tNum Response: %d (0x%02X)\n", num_responses, num_responses);
        event_data += 1;

        /* For each Response, Print the Inquiry Result */
        for (count = 0; count < num_responses; count ++)
        {
            /* BD_ADDR of the Remote Device */
            IotLogDebug("\tBD_ADDR: ");
            appl_hci_print_bd_addr(event_data);
            event_data += 6;

            /* Page Scan Repetition Mode */
            hci_unpack_1_byte_param(&value_1, event_data);
            IotLogDebug("\tPage Scan Repetition Mode: 0x%02X\n", value_1);
            event_data += 1;

            /* Reserved 2 octets */
            event_data += 2;

            /* Class of Device */
            hci_unpack_3_byte_param(&value_3, event_data)
            IotLogDebug("\tClass of Device: 0x%06X\n", (unsigned int)value_3);
            event_data += 3;

            /* Clock Offset */
            hci_unpack_2_byte_param(&value_2, event_data);
            IotLogDebug("\tClock Offset: 0x%04X\n", value_2);
            event_data += 2;
        }

        break;

#ifdef BT_EIR
    case HCI_EXTENDED_INQUIRY_RESULT_EVENT:
        IotLogDebug("Received HCI_EXTENDED_INQUIRY_RESULT_EVENT.\n");

        /* Number of Responses */
        hci_unpack_1_byte_param(&num_responses, event_data);
        IotLogDebug("\tNum Response: %d (0x%02X)\n", num_responses, num_responses);
        event_data += 1;

        /* For each Response, Print the Inquiry Result */
        for (count = 0; count < num_responses; count ++)
        {
            /* BD_ADDR of the Remote Device */
            IotLogDebug("\tBD_ADDR: ");
            appl_hci_print_bd_addr(event_data);
            event_data += 6;

            /* Page Scan Repetition Mode */
            hci_unpack_1_byte_param(&value_1, event_data);
            IotLogDebug("\tPage Scan Repetition Mode: 0x%02X\n", value_1);
            event_data += 1;

            /* Reserved 1 octet */
            event_data += 1;

            /* Class of Device */
            hci_unpack_3_byte_param(&value_3, event_data)
            IotLogDebug("\tClass of Device: 0x%06X\n", (unsigned int)value_3);
            event_data += 3;

            /* Clock Offset */
            hci_unpack_2_byte_param(&value_2, event_data);
            IotLogDebug("\tClock Offset: 0x%04X\n", value_2);
            event_data += 2;

            /* RSSI */
            hci_unpack_1_byte_param(&value_1, event_data);
            IotLogDebug("\tRSSI: 0x%02X -> %d dBm\n", value_1, (char) value_1);
            event_data += 1;

            appl_print_extended_inquiry_response
            (
                event_data,
                (UINT16)(event_datalen - 15)
            );
        }

        break;
#endif /* BT_EIR */

    case HCI_CONNECTION_COMPLETE_EVENT:
        IotLogDebug("Received HCI_CONNECTION_COMPLETE_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        IotLogDebug("\tStatus: 0x%02X\n", status);
        event_data += 1;

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        IotLogDebug("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2;

        /* BD_ADDR */
        IotLogDebug("\tBD_ADDR: ");
        bd_addr = event_data;
        appl_hci_print_bd_addr(bd_addr);
        event_data += 6;

        /* Link Type */
        hci_unpack_1_byte_param(&link_type, event_data);
        IotLogDebug("\tLink Type: 0x%02X", link_type);
        switch (link_type)
        {
        case HCI_SCO_LINK:
            IotLogDebug(" -> SCO\n");
            break;
        case HCI_ACL_LINK:
            IotLogDebug(" -> ACL\n");
            break;
    #ifdef BT_HCI_1_2
        case HCI_ESCO_LINK:
            IotLogDebug(" -> eSCO\n");
            break;
    #endif /* BT_HCI_1_2 */
        default:
            IotLogDebug(" -> ???\n");
            break;
        }
        event_data += 1;

        /* Encryption Mode */
        hci_unpack_1_byte_param(&value_1, event_data);
        IotLogDebug("\tEcnryption Mode: 0x%02X", value_1);
        switch (value_1)
        {
        case 0x00:
            IotLogDebug(" -> Encryption OFF\n");
            break;
        case 0x01:
            IotLogDebug(" -> Encryption ON\n");
            break;
        default:
            IotLogDebug(" -> ???\n");
            break;
        }
        event_data += 1;

    #ifdef HCI_SCO
        /* Setup HCI-USB for SCO Data Reception */
        if ((0x0 == status) &&
            ((HCI_SCO_LINK == link_type) ||
             (HCI_ESCO_LINK == link_type)))
        {
            /* Save SCO/eSCO Connection Handle */
            sco_connection_handle = connection_handle;
            IotLogDebug("SCO Connection Complete.\n");
        }
    #endif /* HCI_SCO */

        break;

    case HCI_CONNECTION_REQUEST_EVENT:
        IotLogDebug("Received (** Unexpected **) HCI_CONNECTION_REQUEST_EVENT.\n");

        /* BD_ADDR */
        IotLogDebug("\tBD_ADDR: ");
        appl_hci_print_bd_addr(event_data);
        event_data += 6;

        /* Class of Device */
        hci_unpack_3_byte_param(&value_3, event_data);
        IotLogDebug("\tClass of Device: 0x%06X\n", (unsigned int)value_3);
        event_data += 3;

        /* Link Type */
        hci_unpack_1_byte_param(&value_1, event_data);
        IotLogDebug("\tLink Type: 0x%02X", value_1);
        switch (value_1)
        {
        case HCI_SCO_LINK:
            IotLogDebug(" -> SCO\n");
            break;
        case HCI_ACL_LINK:
            IotLogDebug(" -> ACL\n");
            break;
    #ifdef BT_HCI_1_2
        case HCI_ESCO_LINK:
            IotLogDebug(" -> eSCO\n");

        #ifndef HCI_NO_ESCO_AUTO_ACCEPT
            if (0 == appl_hci_sync_conn_accept_auto_rsp)
            {
                if (BT_TRUE == appl_hci_set_esco_connect_param)
                {
                    BT_hci_set_esco_channel_parameters (&appl_esco_params);
                }
            }
            else if (1 == appl_hci_sync_conn_accept_auto_rsp)
            {
    #ifdef HCI_ENH_SCO
                /*
                 * The parameters will get updated with the stack, but will be
                 * used only when HCI_RETURN_ENHANCED_SYNCHRONOUS_ACCEPT is
                 * returned from the callback after this setting
                 */
                BT_hci_set_enh_sco_channel_parameters (&appl_enh_sco_params);

                return HCI_ESCO_REQ_RETURN_ENH_SYNCHRONOUS_ACCEPT;
    #else /* HCI_ENH_SCO */
                IotLogDebug ("HCI_ENH_SCO is not defined!\n");
    #endif /* HCI_ENH_SCO */
            }
            else if (2 == appl_hci_sync_conn_accept_auto_rsp)
            {
                /* Return no auto accept */
                return HCI_ESCO_REQ_RETURN_NO_AUTO_RESPONSE;
            }
            else if (3 == appl_hci_sync_conn_accept_auto_rsp)
            {
                /* Return Reject reason */
                return appl_hci_sync_conn_reject_reason;
            }
        #endif /* HCI_NO_ESCO_AUTO_ACCEPT */
            break;
    #endif /* BT_HCI_1_2 */
        default:
            IotLogDebug(" -> ???\n");
            break;
        }
        event_data += 1;

        break;

    case HCI_DISCONNECTION_COMPLETE_EVENT:
        IotLogDebug("Received HCI_DISCONNECTION_COMPLETE_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        IotLogDebug("\tStatus: 0x%02X\n", status);
        event_data += 1;

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        IotLogDebug("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2;

        /* Reason */
        hci_unpack_1_byte_param(&value_1, event_data);
        IotLogDebug("\tReason: 0x%02X\n", value_1);
        event_data += 1;

    #ifdef HCI_SCO
        if ((connection_handle == sco_connection_handle) &&
            (LMP_VOICE_AIR_CODING_TRANSPARENT ==
            (appl_esco_params.voice_setting & LMP_VOICE_AIR_CODING_TRANSPARENT)))
        {

            /**
             * Stop Voice Record(MIC)
             */
            hfp_sco_audio_stop_pl (&sco_audio_ep_mic_info);
            IotLogDebug ("MIC Recording stoped\n");

            /**
             * Stop Voice Play(Speaker)
             */
            hfp_sco_audio_stop_pl (&sco_audio_ep_spkr_info);
            IotLogDebug ("Speaker Playback stoped\n");
        }

        /* Reset SCO connection handle */
        sco_connection_handle = HCI_INVALID_CONNECTION_HANDLE;
    #endif /* HCI_SCO */

        break;

    case HCI_AUTHENTICATION_COMPLETE_EVENT:
        IotLogDebug("Received HCI_AUTHENTICATION_COMPLETE_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        IotLogDebug("\tStatus: 0x%02X\n", status);
        event_data += 1;

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        IotLogDebug("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2;

        break;

    case HCI_REMOTE_NAME_REQUEST_COMPLETE_EVENT:
        IotLogDebug("Received HCI_REMOTE_NAME_REQUEST_COMPLETE_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        IotLogDebug("\tStatus: 0x%02X\n", status);
        event_data += 1;

        /* BD_ADDR of Remote Device */
        IotLogDebug("\tBD_ADDR: ");
        appl_hci_print_bd_addr(event_data);
        event_data += 6;

        /* Remote Name */
        IotLogDebug("\tRemote Name: ");
        if (event_datalen > 7)
        {
            for (count = 7; count < event_datalen; count ++)
            {
                if ('\0' == (*event_data))
                {
                    break;
                }

                IotLogDebug("%c", *event_data);
                event_data += 1;
            }
        }
        IotLogDebug(".\n");

        break;


    case HCI_ENCRYPTION_CHANGE_EVENT:
        IotLogDebug("Received HCI_ENCRYPTION_CHANGE_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        IotLogDebug("\tStatus: 0x%02X\n", status);
        event_data += 1;

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        IotLogDebug("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2;

        /* Encryption Enable */
        hci_unpack_1_byte_param(&value_1, event_data);
        IotLogDebug("\tEcnryption Enable: 0x%02X", value_1);
        switch (value_1)
        {
        case 0x00:
            IotLogDebug(" -> Encryption OFF\n");
            break;
        case 0x01:
            IotLogDebug(" -> Encryption ON\n");
            break;
        default:
            IotLogDebug(" -> ???\n");
            break;
        }
        event_data += 1;

        break;

    case HCI_CHANGE_CONNECTION_LINK_KEY_COMPLETE_EVENT:
        IotLogDebug("Received HCI_CHANGE_CONNECTION_LINK_KEY_COMPLETE_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        IotLogDebug("\tStatus: 0x%02X\n", status);
        event_data += 1;

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        IotLogDebug("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2;

        break;

    case HCI_MASTER_LINK_KEY_COMPLETE_EVENT:
        IotLogDebug("Received HCI_MASTER_LINK_KEY_COMPLETE_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        IotLogDebug("\tStatus: 0x%02X\n", status);
        event_data += 1;

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        IotLogDebug("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2;

        /* Key Flag */
        hci_unpack_1_byte_param(&value_1, event_data);
        IotLogDebug("\tKey Flag: 0x%02X", value_1);
        switch (value_1)
        {
        case 0x00:
            IotLogDebug(" -> Using Semi-parmanent Link Key\n");
            break;
        case 0x01:
            IotLogDebug(" -> Using Temporary Link Key\n");
            break;
        default:
            IotLogDebug(" -> ???\n");
            break;
        }
        event_data += 1;

        break;

    case HCI_READ_REMOTE_SUPPORTED_FEATURES_COMPLETE_EVENT:
        IotLogDebug("Received HCI_READ_REMOTE_SUPPORTED_FEATURES_COMPLETE_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        IotLogDebug("\tStatus: 0x%02X\n", status);
        event_data += 1;

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        IotLogDebug("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2;

        /* LMP Features */
        IotLogDebug("\tLMP Features: %02X %02X %02X %02X %02X %02X %02X %02X\n",
        event_data[0], event_data[1], event_data[2], event_data[3],
        event_data[4], event_data[5], event_data[6], event_data[7]);
        event_data += 8;

        break;

    case HCI_READ_REMOTE_VERSION_INFORMATION_COMPLETE_EVENT:
        IotLogDebug("Received HCI_READ_REMOTE_VERSION_INFORMATION_COMPLETE_EVENT\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        IotLogDebug("\tStatus: 0x%02X\n", status);
        event_data += 1;

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        IotLogDebug("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2;

        /* LMP Version */
        hci_unpack_1_byte_param(&value_1, event_data);
        IotLogDebug("\tLMP Version: 0x%02X\n", value_1);
        event_data += 1;

        /* Manufacturer Name */
        hci_unpack_2_byte_param(&value_2, event_data);
        IotLogDebug("\tManufacturer Name (Company ID): 0x%04X\n", value_2);
        event_data += 2;

        /* LMP Subversion */
        hci_unpack_2_byte_param(&value_2, event_data);
        IotLogDebug("\tLMP Subversion: 0x%04X\n", value_2);
        event_data += 2;

        break;

    case HCI_QOS_SETUP_COMPLETE_EVENT:
        IotLogDebug("Received HCI_QOS_SETUP_COMPLETE_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        IotLogDebug("\tStatus: 0x%02X\n", status);
        event_data += 1;

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        IotLogDebug("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2;

        /* Flags */
        hci_unpack_1_byte_param(&value_1, event_data);
        IotLogDebug("\tFlags: 0x%02X\n", value_1);
        event_data += 1;

        /* Service Type */
        hci_unpack_1_byte_param(&value_1, event_data);
        IotLogDebug("\tService Type: 0x%02X", value_1);
        switch (value_1)
        {
        case 0x00:
            IotLogDebug(" -> No Traffic Available\n");
            break;
        case 0x01:
            IotLogDebug(" -> Best Effort Available\n");
            break;
        case 0x02:
            IotLogDebug(" -> Guaranteed Available\n");
            break;
        default:
            IotLogDebug(" -> ???\n");
            break;
        }
        event_data += 1;

        /* Token Rate */
        hci_unpack_4_byte_param(&value_4, event_data);
        IotLogDebug("\tToken Rate: 0x%08X -> %d octets/sec\n",
        (unsigned int) value_4, (unsigned int) value_4);
        event_data += 4;

        /* Peak Bandwidth */
        hci_unpack_4_byte_param(&value_4, event_data);
        IotLogDebug("\tPeak Bandwidth 0x%08X -> %d octets/sec\n",
        (unsigned int) value_4, (unsigned int) value_4);
        event_data += 4;

        /* Latency */
        hci_unpack_4_byte_param(&value_4, event_data);
        IotLogDebug("\tLatency 0x%08X -> %d microseconds\n",
        (unsigned int) value_4, (unsigned int) value_4);
        event_data += 4;

        /* Delay Variation */
        hci_unpack_4_byte_param(&value_4, event_data);
        IotLogDebug("\tDelay Variation 0x%08X -> %d microseconds\n",
        (unsigned int) value_4, (unsigned int) value_4);
        event_data += 4;

        break;

    case HCI_COMMAND_COMPLETE_EVENT:
        IotLogDebug("Received HCI_COMMAND_COMPLETE_EVENT.\n");

        /* Number of Command Packets */
        hci_unpack_1_byte_param(&value_1, event_data);
        IotLogDebug("\tNum Command Packets: %d (0x%02X)\n", value_1, value_1);
        event_data += 1;

        /* Command Opcode */
        hci_unpack_2_byte_param(&value_2, event_data);
        IotLogDebug("\tCommand Opcode: 0x%04X -> %s\n",
        value_2, appl_hci_get_command_name(value_2));
        event_data += 2;

        /* Command Status */
        hci_unpack_1_byte_param(&status, event_data);
        IotLogDebug("\tCommand Status: 0x%02X\n", status);
        event_data += 1;

        /* Command Return Parameters */
        if (event_datalen > 4)
        {
            switch (value_2)
            {
            #ifdef BT_EIR
                case HCI_READ_EXTENDED_INQUIRY_RESPONSE_OPCODE:
                {
                    IotLogDebug("\tRead EIR Response Status: 0x%02X", status);

                    IotLogDebug("\tFEC_Required: 0x%02X", *event_data);
                    event_data ++;

                    appl_print_extended_inquiry_response
                    (
                        event_data,
                        (UINT16)(event_datalen - 4)
                    );

                    IotLogDebug("\n");
                    break;
                }

                case HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_OPCODE:
                {
                    IotLogDebug("\tWrite EIR Response Status: 0x%02X\n", status);
                    break;
                }
            #endif /* BT_EIR */

            #ifdef BT_3_0
                case HCI_READ_ENCRYPTION_KEY_SIZE_OPCODE:
                {
                    IotLogDebug("\tRead Encryption Key Size Status: 0x%02X\n", status);

                    hci_unpack_2_byte_param(&connection_handle, event_data);
                    IotLogDebug("\tConnection Handle: 0x%04X\n", connection_handle);

                    event_data += 2;
                    IotLogDebug("\tKey Size: 0x%02X\n", *event_data);
                    break;
                }
            #endif /* BT_3_0 */

#ifdef HCI_ENH_SCO
                case HCI_READ_LOCAL_SUPPORTED_CODECS_OPCODE:
                {
                    UCHAR num_codecs;
                    UCHAR num_vendor_specific_codecs;
                    UCHAR i;
                    UINT16 company_id, vendor_codec_id;

                    IotLogDebug("\tRead Local Supported Codecs Status: 0x%02X\n", status);

                    /* Get no. of supported codecs */
                    num_codecs = *event_data;
                    event_data ++;

                    IotLogDebug ("Supported Codecs: 0x%02X\n", num_codecs);

                    /* Print supported codecs */
                    IotLogDebug ("\tSupported Codec IDs:");
                    for( i = 0; i < num_codecs; i++)
                    {
                        IotLogDebug ("\t\tCodec %02X:", *event_data);
                        event_data ++;
                    }

                    /* Get no. of supported vendor specific codecs */
                    num_vendor_specific_codecs = *event_data;
                    event_data ++;

                    IotLogDebug ("Supported Vendor Specific Codecs: 0x%02X\n",
                            num_vendor_specific_codecs);

                    /* Print supported vendor specific codecs */
                    IotLogDebug ("\tSupported Vendor Specific Codec IDs:");
                    for( i = 0; i < num_vendor_specific_codecs; i++)
                    {
                        hci_unpack_2_byte_param(&company_id, event_data);
                        event_data += 2;
                        hci_unpack_2_byte_param(&vendor_codec_id, event_data);
                        event_data += 2;

                        IotLogDebug ("\t\tCompany ID 0x%04X: Vendor Codec ID 0x%04X\n",
                        company_id, vendor_codec_id);
                    }

                    break;
                }
#endif /* HCI_ENH_SCO */

                case HCI_READ_LOCAL_VERSION_INFORMATION_OPCODE:
                {
                    IotLogDebug("\tRead Local Version Information Status: 0x%02X\n", status);

                    hci_unpack_1_byte_param(&value_1, event_data);
                    IotLogDebug("\tHCI_Version: 0x%02X\n", value_1);
                    event_data += 1;

                    hci_unpack_2_byte_param(&value_2, event_data);
                    IotLogDebug("\tHCI_Revision: 0x%04X\n", value_2);
                    event_data += 2;

                    hci_unpack_1_byte_param(&value_1, event_data);
                    IotLogDebug("\tLMP_PAL_Version: 0x%02X\n", value_1);
                    event_data += 1;

                    hci_unpack_2_byte_param(&value_2, event_data);
                    IotLogDebug("\tManufacturer_Name: 0x%04X\n", value_2);
                    event_data += 2;

                    hci_unpack_2_byte_param(&value_2, event_data);
                    IotLogDebug("\tLMP_PAL_Subverion: 0x%04X\n", value_2);
                    event_data += 2;

                    break;
                }

                default:
                {
                    IotLogDebug("\tReturn Parameters: ");
                    appl_dump_bytes(event_data, event_datalen - 4);
                    IotLogDebug("\n");
                    break;
                }
            }
        }

    #ifdef BT_VENDOR_SPECIFIC_INIT
        app_vendor_specific_init(NULL);
    #endif /* BT_VENDOR_SPECIFIC_INIT */

        break;

    case HCI_COMMAND_STATUS_EVENT:
        IotLogDebug("Received HCI_COMMAND_STATUS_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        IotLogDebug("\tCommand Status: 0x%02X\n", status);
        event_data += 1;

        /* Number of Command Packets */
        hci_unpack_1_byte_param(&value_1, event_data);
        IotLogDebug("\tNum Command Packets: %d (0x%02X)\n", value_1, value_1);
        event_data += 1;

        /* Command Opcode */
        hci_unpack_2_byte_param(&value_2, event_data);
        IotLogDebug("\tCommand Opcode: 0x%04X (%s)\n",
        value_2, appl_hci_get_command_name(value_2));
        event_data += 2;

        break;

    case HCI_HARDWARE_ERROR_EVENT:
        IotLogDebug("Received HCI_HARDWARE_ERROR_EVENT.\n");

        /* Hardware Code */
        hci_unpack_1_byte_param(&value_1, event_data);
        IotLogDebug("\tHardware Code: 0x%02X\n", value_1);
        event_data += 1;

        break;

    case HCI_FLUSH_OCCURRED_EVENT:
        IotLogDebug("Received HCI_FLUSH_OCCURRED_EVENT.\n");

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        IotLogDebug("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2;

        break;

    case HCI_ROLE_CHANGE_EVENT:
        IotLogDebug("Received HCI_ROLE_CHANGE_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        IotLogDebug("\tStatus: 0x%02X\n", status);
        event_data += 1;

        /* BD_ADDR of Remote Device */
        IotLogDebug("\tBD_ADDR: ");
        appl_hci_print_bd_addr(event_data);
        event_data += 6;

        /* New Role */
        hci_unpack_1_byte_param(&value_1, event_data);
        IotLogDebug("\tNew Role: 0x%02X", value_1);
        switch (value_1)
        {
        case 0x00:
            IotLogDebug(" -> Master\n");
            break;
        case 0x01:
            IotLogDebug(" -> Slave\n");
            break;
        default:
            IotLogDebug(" -> ???\n");
            break;
        }
        event_data += 1;

        break;

    case HCI_NUMBER_OF_COMPLETED_PACKETS_EVENT:
        /* IotLogDebug("Received HCI_NUMBER_OF_COMPLETED_PACKETS_EVENT.\n"); */
        break;

    case HCI_MODE_CHANGE_EVENT:
        IotLogDebug("Received HCI_MODE_CHANGE_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        IotLogDebug("\tStatus: 0x%02X\n", status);
        event_data += 1;

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        IotLogDebug("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2;

        /* Current Mode */
        hci_unpack_1_byte_param(&value_1, event_data);
        IotLogDebug("\tCurrent Mode: 0x%02X", value_1);
        switch (value_1)
        {
        case 0x00:
            IotLogDebug(" -> Active Mode\n");
            break;
        case 0x01:
            IotLogDebug(" -> Hold Mode\n");
            break;
        case 0x02:
            IotLogDebug(" -> Sniff Mode\n");
            break;
        case 0x03:
            IotLogDebug(" -> Park Mode\n");
            break;
        default:
            IotLogDebug(" -> ???\n");
            break;
        }
        event_data += 1;

        /* Interval */
        hci_unpack_2_byte_param(&value_2, event_data);
        IotLogDebug("\tInterval: 0x%04X\n", value_2);
        event_data += 2;

        break;

    case HCI_RETURN_LINK_KEYS_EVENT:
        IotLogDebug("Received HCI_RETURN_LINK_KEYS_EVENT.\n");

        /* Number of Keys */
        hci_unpack_1_byte_param(&value_1, event_data);
        IotLogDebug("\tNumber of Keys: %d (0x%02X)\n", value_1, value_1);
        event_data += 1;

        while (value_1 > 0)
        {
            /* BD_ADDR */
            IotLogDebug("\tBD_ADDR: ");
            appl_hci_print_bd_addr(event_data);
            event_data += 6;

            /* Link Key */
            IotLogDebug("\tLink Key: ");
            appl_dump_bytes(event_data, 16);
            event_data += 16;

            value_1 --;
        }

        break;

    case HCI_PIN_CODE_REQUEST_EVENT:
        IotLogDebug("Received (** Unexpected **) HCI_PIN_CODE_REQUEST_EVENT.\n");
        break;

    case HCI_LINK_KEY_REQUEST_EVENT:
        IotLogDebug("Received (** Unexpected **) HCI_LINK_KEY_REQUEST_EVENT.\n");
        break;

    case HCI_LINK_KEY_NOTIFICATION_EVENT:
        IotLogDebug("Received HCI_LINK_KEY_NOTIFICATION_EVENT.\n");

        /* BD_ADDR */
        IotLogDebug("\tBD_ADDR: ");
        appl_hci_print_bd_addr(event_data);
        bd_addr = event_data;
        event_data += 6;

        /* Link Key */
        IotLogDebug("\tLink Key: ");
        appl_dump_bytes(event_data, 16);
        event_data += 16;

        /* Key Type */
        hci_unpack_1_byte_param(&value_1, event_data);
        IotLogDebug("\tKey Type: 0x%02X", value_1);
        switch (value_1)
        {
        case 0x00:
            IotLogDebug(" -> Combination Key\n");
            break;
        case 0x01:
            IotLogDebug(" -> Local Unit Key\n");
            break;
        case 0x02:
            IotLogDebug(" -> Remote Unit Key\n");
            break;
        default:
            IotLogDebug(" -> ???\n");
            break;
        }
        event_data += 1;

        break;

    case HCI_LOOPBACK_COMMAND_EVENT:
        IotLogDebug("Received HCI_LOOPBACK_COMMAND_EVENT.\n");

        /* HCI Command Packet */
        IotLogDebug("\tHCI Command Packet: ");
        appl_dump_bytes(event_data, event_datalen);

        break;

    case HCI_DATA_BUFFER_OVERFLOW_EVENT:
        IotLogDebug("Received HCI_DATA_BUFFER_OVERFLOW_EVENT.\n");

        /* Link Type */
        hci_unpack_1_byte_param(&value_1, event_data);
        IotLogDebug("\tLink Type: 0x%02X", value_1);
        switch (value_1)
        {
        case 0x00:
            IotLogDebug(" -> Synchronous Buffer Overflow\n");
            break;
        case 0x01:
            IotLogDebug(" -> ACL Buffer Overflow\n");
            break;
        default:
            IotLogDebug(" -> ???\n");
            break;
        }
        event_data += 1;

        break;

    case HCI_MAX_SLOTS_CHANGE_EVENT:
        IotLogDebug("Received HCI_MAX_SLOTS_CHANGE_EVENT.\n");

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        IotLogDebug("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2;

        /* LMP Max Slots */
        hci_unpack_1_byte_param(&value_1, event_data);
        IotLogDebug("\tLMP Max Slots: 0x%02X\n", value_1);
        event_data += 1;

        break;

    case HCI_READ_CLOCK_OFFSET_COMPLETE_EVENT:
        IotLogDebug("Received HCI_READ_CLOCK_OFFSET_COMPLETE_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        IotLogDebug("\tStatus: 0x%02X\n", status);
        event_data += 1;

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        IotLogDebug("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2;

        /* Clock Offset */
        hci_unpack_2_byte_param(&value_2, event_data);
        IotLogDebug("\tClock Offset: 0x%04X\n", value_2);
        event_data += 2;

        break;

    case HCI_ENCRYPTION_KEY_REFRESH_COMPLETE_EVENT:
        IotLogDebug ("Received HCI_ENCRYPTION_KEY_REFRESH_COMPLETE_EVENT\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        IotLogDebug("Status: 0x%02X\n", status);
        event_data ++;

        /* Connection Handle */
        hci_unpack_2_byte_param(&value_2, event_data);
        IotLogDebug("\tConnection Handle: 0x%04X\n", value_2);

        break;

    case HCI_CONNECTION_PACKET_TYPE_CHANGED_EVENT:
        IotLogDebug("Received HCI_CONNECTION_PACKET_TYPE_CHANGED_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        IotLogDebug("\tStatus: 0x%02X\n", status);
        event_data += 1;

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        IotLogDebug("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2;

        /* Packet Type */
        hci_unpack_2_byte_param(&value_2, event_data);
        IotLogDebug("\tPacket Type: 0x%02X\n", value_2);
        event_data += 2;

        break;

    case HCI_QOS_VIOLATION_EVENT:
        IotLogDebug("Received HCI_QOS_VIOLATION_EVENT.\n");

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        IotLogDebug("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2;

        break;

    case HCI_PAGE_SCAN_MODE_CHANGE_EVENT:
        IotLogDebug("Received HCI_PAGE_SCAN_MODE_CHANGE_EVENT.\n");

        /* BD_ADDR */
        IotLogDebug("\tBD_ADDR: ");
        appl_hci_print_bd_addr(event_data);
        event_data += 6;

        /* Page Scan Mode */
        hci_unpack_1_byte_param(&value_1, event_data);
        IotLogDebug("\tPage Scan Mode: 0x%02X\n", value_1);
        event_data += 1;

        break;

    case HCI_PAGE_SCAN_REPETITION_MODE_CHANGE_EVENT:
        IotLogDebug("Received HCI_PAGE_SCAN_REPETITION_MODE_CHANGE_EVENT.\n");

        /* BD_ADDR */
        IotLogDebug("\tBD_ADDR: ");
        appl_hci_print_bd_addr(event_data);
        event_data += 6;

        /* Page Scan Repetition Mode */
        hci_unpack_1_byte_param(&value_1, event_data);
        IotLogDebug("\tPage Scan Repetition Mode: 0x%02X\n", value_1);
        event_data += 1;

        break;

#ifdef BT_HCI_1_2
    case HCI_FLOW_SPECIFICATION_COMPLETE_EVENT:
        IotLogDebug("Received HCI_FLOW_SPECIFICATION_COMPLETE_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        IotLogDebug("\tStatus: 0x%02X\n", status);
        event_data += 1;

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        IotLogDebug("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2;

        /* Flags */
        hci_unpack_1_byte_param(&value_1, event_data);
        IotLogDebug("\tFlags: 0x%02X\n", value_1);
        event_data += 1;

        /* Flow Direction */
        hci_unpack_1_byte_param(&value_1, event_data);
        IotLogDebug("\tFlow Direction: 0x%02X", value_1);
        switch (value_1)
        {
        case 0x00:
            IotLogDebug(" -> Outgoing Flow\n");
            break;
        case 0x01:
            IotLogDebug(" -> Incoming Flow\n");
            break;
        default:
            IotLogDebug(" -> ???\n");
            break;
        }
        event_data += 1;

        /* Service Type */
        hci_unpack_1_byte_param(&value_1, event_data);
        IotLogDebug("\tService Type: 0x%02X", value_1);
        switch (value_1)
        {
        case 0x00:
            IotLogDebug(" -> No Traffic\n");
            break;
        case 0x01:
            IotLogDebug(" -> Best Effort\n");
            break;
        case 0x02:
            IotLogDebug(" -> Guaranteed\n");
            break;
        default:
            IotLogDebug(" -> ???\n");
            break;
        }
        event_data += 1;

        /* Token Rate */
        hci_unpack_4_byte_param(&value_4, event_data);
        IotLogDebug("\tToken Rate: 0x%08X -> %d octets/sec\n",
        (unsigned int) value_4, (unsigned int) value_4);
        event_data += 4;

        /* Token Bucket Size */
        hci_unpack_4_byte_param(&value_4, event_data);
        IotLogDebug("\tToken Bucket Size: 0x%08X -> %d octets\n",
        (unsigned int) value_4, (unsigned int) value_4);
        event_data += 4;

        /* Peak Bandwidth */
        hci_unpack_4_byte_param(&value_4, event_data);
        IotLogDebug("\tPeak Bandwidth 0x%08X -> %d octets/sec\n",
        (unsigned int) value_4, (unsigned int) value_4);
        event_data += 4;

        /* Access Latency */
        hci_unpack_4_byte_param(&value_4, event_data);
        IotLogDebug("\tAccess Latency 0x%08X -> %d microseconds\n",
        (unsigned int) value_4, (unsigned int) value_4);
        event_data += 4;

        break;

    case HCI_INQUIRY_RESULT_WITH_RSSI_EVENT:
        IotLogDebug("Received HCI_INQUIRY_RESULT_WITH_RSSI_EVENT.\n");

        /* Number of Responses */
        hci_unpack_1_byte_param(&num_responses, event_data);
        IotLogDebug("\tNum Response: %d (0x%02X)\n", num_responses, num_responses);
        event_data += 1;

        /* For each Response, Print the Inquiry Result */
        for (count = 0; count < num_responses; count ++)
        {
            /* BD_ADDR of the Remote Device */
            IotLogDebug("\tBD_ADDR: ");
            appl_hci_print_bd_addr(event_data);
            event_data += 6;

            /* Page Scan Repetition Mode */
            hci_unpack_1_byte_param(&value_1, event_data);
            IotLogDebug("\tPage Scan Repetition Mode: 0x%02X\n", value_1);
            event_data += 1;

            /* Reserved 1 octet */
            event_data += 1;

            /* Class of Device */
            hci_unpack_3_byte_param(&value_3, event_data)
            IotLogDebug("\tClass of Device: 0x%06X\n", (unsigned int)value_3);
            event_data += 3;

            /* Clock Offset */
            hci_unpack_2_byte_param(&value_2, event_data);
            IotLogDebug("\tClock Offset: 0x%04X\n", value_2);
            event_data += 2;

            /* RSSI */
            hci_unpack_1_byte_param(&value_1, event_data);
            IotLogDebug("\tRSSI: 0x%02X -> %d dBm\n", value_1, (char) value_1);
            event_data += 1;
        }

        break;

    case HCI_REMOTE_EXTENDED_FEATURES_COMPLETE_EVENT:
        IotLogDebug("Received HCI_REMOTE_EXTENDED_FEATURES_COMPLETE_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        IotLogDebug("\tStatus: 0x%02X\n", status);
        event_data += 1;

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        IotLogDebug("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2;

        /* Page Number */
        hci_unpack_1_byte_param(&value_1, event_data);
        IotLogDebug("\tPage Number: 0x%02X\n", value_1);
        event_data += 1;

        /* Max Page Number */
        hci_unpack_1_byte_param(&value_1, event_data);
        IotLogDebug("\tMax Page Number: 0x%02X\n", value_1);
        event_data += 1;

        /* Extended LMP Features */
        IotLogDebug("\tExtended LMP Features: ");
        appl_dump_bytes(event_data, 8);
        event_data += 8;

        break;

    case HCI_SYNCHRONOUS_CONNECTION_COMPLETE_EVENT:
        IotLogDebug("Received HCI_SYNCHRONOUS_CONNECTION_COMPLETE_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        IotLogDebug("\tStatus: 0x%02X\n", status);
        event_data += 1;

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        IotLogDebug("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2;

        /* BD_ADDR */
        IotLogDebug("\tBD_ADDR: ");
        appl_hci_print_bd_addr(event_data);
        event_data += 6;

        /* Link Type */
        hci_unpack_1_byte_param(&link_type, event_data);
        IotLogDebug("\tLink Type: 0x%02X", link_type);
        switch (link_type)
        {
        case HCI_SCO_LINK:
            IotLogDebug(" -> SCO\n");
            break;
        case HCI_ACL_LINK:
            IotLogDebug(" -> ACL ???\n");
            break;
        case HCI_ESCO_LINK:
            IotLogDebug(" -> eSCO\n");
            break;
        default:
            IotLogDebug(" -> ???\n");
            break;
        }
        event_data += 1;

        /* Transmission Interval */
        hci_unpack_1_byte_param(&value_1, event_data);
        IotLogDebug("\tTransmission Interval: 0x%02X\n", value_1);
        event_data += 1;

        /* Retransmission Windows */
        hci_unpack_1_byte_param(&value_1, event_data);
        IotLogDebug("\tRetransmission Windows: 0x%02X\n", value_1);
        event_data += 1;

        /* Rx Packet Length */
        hci_unpack_2_byte_param(&value_2, event_data);
        IotLogDebug("\tRx Packet Length: 0x%04X\n", value_2);
        event_data += 2;

        /* Tx Packet Length */
        hci_unpack_2_byte_param(&value_2, event_data);
        IotLogDebug("\tTx Packet Length: 0x%04X\n", value_2);
        event_data += 2;

        /* Air Mode */
        hci_unpack_1_byte_param(&value_1, event_data);
        IotLogDebug("\tAir Mode: 0x%02X\n", value_1);
        event_data += 1;

    #ifdef HCI_SCO
        /* Setup HCI for SCO Data Reception */
        if ((0x0 == status) &&
            ((HCI_SCO_LINK == link_type) ||
             (HCI_ESCO_LINK == link_type)))
        {
            IotLogDebug("eSCO Connection Complete.\n");

            /**
             * Currently added support for handling mSBC data over HCI for Wideband speech.
             *
             * TODO:
             * Add support for handling default narrow band speech over HCI.
             */

            if ((HCI_ESCO_LINK == link_type) &&
                (LMP_VOICE_AIR_CODING_TRANSPARENT ==
                (appl_esco_params.voice_setting & LMP_VOICE_AIR_CODING_TRANSPARENT)))

            {
                /**
                 * Configure for eSCO Audio and mSBC codec
                 */
                appl_hci_esco_audio_config ();

                /**
                 * Start Voice Record(MIC)
                 */
                hfp_sco_audio_start_pl (&sco_audio_ep_mic_info);
                IotLogDebug ("Started MIC for voice recording \n");

                /**
                 * Start Voice Play(Speaker)
                 */
                hfp_sco_audio_start_pl (&sco_audio_ep_spkr_info);
                IotLogDebug ("Started Speaker for voice playback \n");
            }

            /* Save SCO/eSCO Connection Handle */
            sco_connection_handle = connection_handle;
        }
    #endif /* HCI_SCO */

        break;

    case HCI_SYNCHRONOUS_CONNECTION_CHANGED_EVENT:
        IotLogDebug("Received HCI_SYNCHRONOUS_CONNECTION_CHANGED_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        IotLogDebug("\tStatus: 0x%02X\n", status);
        event_data += 1;

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        IotLogDebug("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2;

        /* BD_ADDR */
        IotLogDebug("\tBD_ADDR: ");
        appl_hci_print_bd_addr(event_data);
        event_data += 6;

        /* Transmission Interval */
        hci_unpack_1_byte_param(&value_1, event_data);
        IotLogDebug("\tTransmission Interval: 0x%02X\n", value_1);
        event_data += 1;

        /* Retransmission Windows */
        hci_unpack_1_byte_param(&value_1, event_data);
        IotLogDebug("\tRetransmission Windows: 0x%02X\n", value_1);
        event_data += 1;

        /* Rx Packet Length */
        hci_unpack_2_byte_param(&value_2, event_data);
        IotLogDebug("\tRx Packet Length: 0x%04X\n", value_2);
        event_data += 2;

        /* Tx Packet Length */
        hci_unpack_2_byte_param(&value_2, event_data);
        IotLogDebug("\tTx Packet Length: 0x%04X\n", value_2);
        event_data += 2;

        break;

#endif /* BT_HCI_1_2 */

    case HCI_VENDOR_SPECIFIC_DEBUG_EVENT:
        IotLogDebug("Received HCI_VENDOR_SPECIFIC_DEBUG_EVENT.\n");
        IotLogDebug("Vendor Specific Parameters\n");
        appl_dump_bytes(event_data, event_datalen);

        break;

    default:
        IotLogDebug("Unknown Event Code 0x%02X Received.\n", event_type);
        break;
    }

#ifdef BT_LE
    /* Check for LE related handling */
    appl_hci_le_event_indication_callback
    (
        event_type,
        revent_data,
        revent_datalen
    );
#endif /* BT_LE */

#ifndef APPL_HCI_NO_PROFILE_EVENT_IND
{
    UINT32 i;

    /* Invoke registered Callback functions */
    for (i = 0; i < appl_num_hci_cb; i++)
    {
        appl_hci_cb_list[i].hci_event_ind_cb
        (
            event_type,
            revent_data,
            revent_datalen
        );
    }
}
#endif /* APPL_HCI_NO_PROFILE_EVENT_IND */

    return API_SUCCESS;
}

#ifndef HCI_LITE
API_RESULT appl_hci_error_indication_callback
           (
               UINT16  opcode,
               UINT16  error_code
           )
{
#ifdef HT_ENABLE_INVALID_RX_IND
    UCHAR rx_octet;
    API_RESULT retval;

    /* This is specific handling of invalid octet received over HCI Transport */
    if (HCI_RX_INVALID_PKT_TYPE == error_code)
    {
        /**
         * The invalid octet (UCHAR) is received as 'opcode' (UINT16).
         * Typecast to get the invalid octet.
         */
        rx_octet = (UCHAR)opcode;

        IotLogDebug("Received invalid Octet 0x%02X\n", rx_octet);

        /**
         * Add the check, if this is the eHCILL related octet.
         * If this is eHCILL related octet, return API_SUCCESS.
         * Else return API_FAILURE
         */
        retval = API_FAILURE;

        if (0x30 == rx_octet)
        {
            /* Handle HCILL_GO_TO_SLEEP_IND */
            retval = API_SUCCESS;
        }
        else if (0x33 == rx_octet)
        {
            /* Handle HCILL_WAKE_UP_ACK */
            retval = API_SUCCESS;
        }

        return retval;
    }
#endif /* HT_ENABLE_INVALID_RX_IND */

    IotLogDebug("\n");
    IotLogDebug("Received HCI Error Indication.\n");
    IotLogDebug("\tCommand Opcode: 0x%04X -> %s\n",
    opcode, appl_hci_get_command_name(opcode));
    IotLogDebug("\tError Code = 0x%04X\n", error_code);

    return API_SUCCESS;
}
#endif /* HCI_LITE */


void main_hci_operations ( void )
{
    UCHAR bd_addr[BT_BD_ADDR_SIZE];
    int choice;
    UINT16 handle;
    static UCHAR first_time = 0x0;

    if (0x0 == first_time)
    {
#ifndef HCI_NO_ESCO_AUTO_ACCEPT
        appl_hci_sync_conn_accept_auto_rsp = 0;
        appl_hci_set_esco_connect_param = BT_FALSE;

        /* Set parameters for CVSD */
        appl_esco_params.tx_bandwidth  = LMP_ESCO_TX_BANDWIDTH_DEFAULT;
        appl_esco_params.rx_bandwidth  = LMP_ESCO_RX_BANDWIDTH_DEFAULT;
        appl_esco_params.max_latency   = LMP_ESCO_MAX_LATENCY_DEFAULT;
        appl_esco_params.voice_setting = LMP_VOICE_SETTING_DEFAULT;
        appl_esco_params.rtx_effort    = LMP_ESCO_RETX_EFFORT_DEFAULT;
        appl_esco_params.packet_type   = LMP_ESCO_PACKET_TYPE_DEFAULT;

#ifdef HCI_ENH_SCO
        /* Set parameters for mSBC */
        appl_enh_sco_params.tx_bandwidth = LMP_ENH_SCO_TX_BANDWIDTH_DEFAULT;
        appl_enh_sco_params.rx_bandwidth = LMP_ENH_SCO_RX_BANDWIDTH_DEFAULT;
        appl_enh_sco_params.max_latency = LMP_ESCO_MAX_LATENCY_DEFAULT;
        appl_enh_sco_params.packet_type = LMP_ESCO_PACKET_TYPE_DEFAULT;
        appl_enh_sco_params.retransmission_effort = LMP_ESCO_RETX_EFFORT_DEFAULT;

        /**
         *  If coding_format != LMP_CODING_FRMT_VS (0xFF), then
         *  Company ID and Vendor Specific Codec ID shall be ignored.
         */
        appl_enh_sco_params.tx_coding_frmt.coding_format   = LMP_CODING_FRMT_TRANSPARENT;
        appl_enh_sco_params.tx_coding_frmt.company_id      = 0x02; /* dummy */
        appl_enh_sco_params.tx_coding_frmt.vendor_codec_id = 0x01; /* dummy */

        appl_enh_sco_params.rx_coding_frmt.coding_format   = LMP_CODING_FRMT_TRANSPARENT;
        appl_enh_sco_params.rx_coding_frmt.company_id      = 0x02; /* dummy */
        appl_enh_sco_params.rx_coding_frmt.vendor_codec_id = 0x01; /* dummy */

        /* Configuring for mSBC codec */
        appl_enh_sco_params.tx_codec_frame_size = MSBC_FRAME_SIZE;
        appl_enh_sco_params.rx_codec_frame_size = MSBC_FRAME_SIZE;

        appl_enh_sco_params.input_bandwidth = LMP_ENH_SCO_RX_BANDWIDTH_DEFAULT;
        appl_enh_sco_params.output_bandwidth = LMP_ENH_SCO_TX_BANDWIDTH_DEFAULT;

        /**
         *  If coding_format != LMP_CODING_FRMT_VS (0xFF), then
         *  Company ID and Vendor Specific Codec ID shall be ignored.
         */
        appl_enh_sco_params.input_coding_frmt.coding_format   = LMP_CODING_FRMT_MSBC;
        appl_enh_sco_params.input_coding_frmt.company_id      = 0x02; /* dummy */
        appl_enh_sco_params.input_coding_frmt.vendor_codec_id = 0x01; /* dummy */

        appl_enh_sco_params.output_coding_frmt.coding_format   = LMP_CODING_FRMT_MSBC;
        appl_enh_sco_params.output_coding_frmt.company_id      = 0x02; /* dummy */
        appl_enh_sco_params.output_coding_frmt.vendor_codec_id = 0x01; /* dummy */

        /**
         * mSBC encoded data is a byte stream. so coded unit size is 8 bits
         */
        appl_enh_sco_params.input_coded_data_size  = 8;
        appl_enh_sco_params.output_coded_data_size = 8;

        /**
         * Below parameters are valide when
         * coding_format equal to LMP_CODING_FRMT_LINEAR_PCM
         */
        appl_enh_sco_params.input_pcm_data_frmt = 0xFF; /* dummy */
        appl_enh_sco_params.output_pcm_data_frmt = 0xFF; /* dummy */
        appl_enh_sco_params.input_pcm_smpl_payload_msb_pos = 0xFF; /* dummy */
        appl_enh_sco_params.output_pcm_smpl_payload_msb_pos = 0xFF; /* dummy */

        /* Exchange SCO packets over HCI transport */
        appl_enh_sco_params.input_data_path  = ENH_SCO_DATA_PATH_HCI;
        appl_enh_sco_params.output_data_path = ENH_SCO_DATA_PATH_HCI;

        /**
         * As per Spec., if SCO packet are exchanged over HCI transport, then
         * below parameters shall be zero
         */
        appl_enh_sco_params.input_transport_unit_size = 0x00;
        appl_enh_sco_params.output_transport_unit_size = 0x00;

#endif /* HCI_ENH_SCO */
#endif /* HCI_NO_ESCO_AUTO_ACCEPT */

        first_time = 0x1;
    }

    BT_LOOP_FOREVER()
    {
        IotLogDebug("\n");
        IotLogDebug("%s", hci_options);
        (void)hci_options;    /*fix build warning: set but never used.*/
        scanf("%d", &choice);

        switch (choice)
        {
        case 0:
            return;

        case 1:
            IotLogDebug("Refreshing ...\n");
            break;

        case 2:
            BT_hci_get_local_bd_addr (bd_addr);
            IotLogDebug("Local " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
            BT_DEVICE_ADDR_ONLY_PRINT_STR (bd_addr));
            break;

        case 3:
            appl_hci_inquiry();
            break;

        case 4:
            appl_hci_write_scan_enable();
            appl_hci_read_scan_enable();
            break;

        case 10:
            appl_hci_create_connection ();
            break;

        case 11:
            appl_hci_add_sco_connection ();
            break;

        case 12:
            appl_hci_disconnect ();
            break;

        case 13:
            appl_hci_remote_name_request ();
            break;

        case 15:
            appl_hci_setup_synchronous_connection ();
            break;

        case 16:
            appl_hci_setup_synchronous_connection_response ();
            break;

        case 17:
#ifdef HCI_ENH_SCO
            appl_hci_enhanced_setup_synchronous_connection ();
#else
        IotLogDebug ("\nHCI_ENH_SCO flag is not defined!\n");
#endif /* HCI_ENH_SCO */
            break;

        case 18:
#ifdef HCI_ENH_SCO
            appl_hci_enhanced_setup_synchronous_connection_response ();
#else
        IotLogDebug ("\nHCI_ENH_SCO flag is not defined!\n");
#endif /* HCI_ENH_SCO */
            break;

        case 19:
            IotLogDebug ("Enter eSCO Auto Accept Response\n"
                    "  0 - Synchronous Connection Accept,\n"
                    "  1 - Enhanced Synchronous Connection Accept,\n"
                    "  2 - No response,\n"
                    "  3 - Synchronous Connection Reject\n"
                    "Your Choice: ");
            scanf ("%d", &choice);
            appl_hci_sync_conn_accept_auto_rsp = (UCHAR) choice;

            if (3 == choice)
            {
                IotLogDebug ("Enter Reason (in HEX) for Reject: ");
                scanf ("%x", &choice);
                appl_hci_sync_conn_reject_reason = (UCHAR)choice;
            }

            break;

        case 20:
            appl_hci_get_connection_details();
            break;

        case 25:
#ifdef HCI_ENH_SCO
            appl_hci_read_local_supported_codecs();
#else
            IotLogDebug ("\nHCI_ENH_SCO flag is not defined!\n");
#endif /* HCI_ENH_SCO */
            break;

        case 26:
            IotLogDebug("Read Local supported and Extended features...\n");

            BT_hci_read_local_supported_features();
            BT_hci_read_local_extended_features(0);
            BT_hci_read_local_extended_features(1);
            break;

        case 27:
            IotLogDebug("Read Local Version Information ...\n");

            BT_hci_read_local_version_information();
            break;

        case 28:
            IotLogDebug("Read Remote supported features. Enter Required Information ...\n");

            IotLogDebug("ACL Handle of the Remote Device (in Hex) = ");
            scanf("%x", &choice);
            handle = (UINT16)choice;

            BT_hci_read_remote_version_information(handle);
            BT_hci_read_remote_supported_features(handle);
            BT_hci_read_remote_extended_features(handle, 0);
            BT_hci_read_remote_extended_features(handle, 1);
            break;

        case 30:
            appl_hci_role_discovery ();
            break;

        case 31:
            appl_hci_switch_role ();
            break;

        case 32:
            appl_hci_sniff_mode();
            break;

        case 33:
            appl_hci_park_mode();
            break;

        case 34:
            appl_hci_hold_mode();
            break;

        case 35:
            appl_hci_exit_sniff_mode();
            break;

        case 36:
            appl_hci_exit_park_mode();
            break;

        case 37:
            appl_hci_write_link_policy();
            break;

        case 40:
            appl_hci_write_connection_accept_timeout();
            appl_hci_read_connection_accept_timeout();
            break;

        case 41:
            appl_hci_write_page_timeout();
            appl_hci_read_page_timeout();
            break;

        case 42:
            appl_hci_write_page_scan_activity();
            appl_hci_read_page_scan_activity();
            break;

        case 43:
            appl_hci_write_inquiry_scan_activity();
            appl_hci_read_inquiry_scan_activity();
            break;

        case 44:
            appl_hci_write_class_of_device();
            appl_hci_read_class_of_device();
            break;

        case 45:
            appl_hci_change_local_name();
            appl_hci_read_local_name();
            break;

    #ifdef BT_HCI_1_2

        case 50:
            /* Read/Write Inquiry Mode */
            appl_hci_write_inquiry_mode();
            appl_hci_read_inquiry_mode();
            break;

        case 51:
            /* Read/Write Inquiry Scan Type */
            appl_hci_write_inquiry_scan_type();
            appl_hci_read_inquiry_scan_type();
            break;

        case 52:
            /* Read/Write Page Scan Type */
            appl_hci_write_page_scan_type();
            appl_hci_read_page_scan_type();
            break;

    #endif /* BT_HCI_1_2 */

        case 53:
        #ifdef HCI_FLOW_CONTROL_COMMANDS
            /* Read/Write Flow Control Mode */
            appl_hci_write_flow_control_mode();
            appl_hci_read_flow_control_mode();
        #else
            IotLogDebug ("\nHCI_FLOW_CONTROL_COMMANDS flag not defined\n");
        #endif /* HCI_FLOW_CONTROL_COMMANDS */
            break;

        case 54:
            appl_hci_read_current_iaclap();
            break;

        case 55:
            appl_hci_write_current_iaclap();
            break;

        case 60:
            BT_hci_read_voice_setting();
            break;

        case 61:
        #ifdef BT_3_0
            appl_hci_read_enhanced_transmit_power_level();
        #else
            IotLogDebug ("\nBT_3_0 flag not defined\n");
        #endif /* BT_3_0 */
            break;

        case 62:
        #if ((defined BT_3_0) && !(defined HCI_LITE))
            appl_hci_read_encryption_key_size();
        #else
            IotLogDebug ("\nBT_3_0 flag not defined\n");
            IotLogDebug ("or HCI_LITE flag is defined\n");
        #endif /* ((defined BT_3_0) && !(defined HCI_LITE)) */
            break;

        case 65:
#ifdef BT_BRSC
            appl_hci_read_secure_connections_host_support();
#else /* BT_BRSC */
            IotLogDebug ("BT_BRSC flag not defined\n");
#endif /* BT_BRSC */
            break;

        case 66:
#ifdef BT_BRSC
            appl_hci_write_secure_connections_host_support();
#else /* BT_BRSC */
            IotLogDebug ("BT_BRSC flag not defined\n");
#endif /* BT_BRSC */
            break;

        case 67:
#ifdef BT_BRSC_TEST
            appl_hci_write_secure_connections_test_mode();
#else /* BT_BRSC_TEST */
            IotLogDebug ("BT_BRSC_TEST flag not defined\n");
#endif /* BT_BRSC_TEST */
            break;

        case 70:
    #ifdef BT_EIR
            appl_hci_write_extended_inquiry_response();
    #else
            IotLogDebug("BT_EIR flag is not defined\n");
    #endif /* BT_EIR */
            break;

#ifdef BT_LE
        case 80:
            main_hci_le_operations();
#endif /* BT_LE */

        default:
            IotLogDebug("Invalid choice - %d. Please try again\n", choice);
            break;
        }
    }
}


char *appl_hci_get_command_name (UINT16 opcode)
{
    int i, size;
    char *command_name;

    command_name = "UNKNOWN";
    size = sizeof(opcode_name_table)/sizeof(APPL_HCI_OPCODE_NAME);

    for (i = 0; i < size; i++)
    {
        if (opcode_name_table[i].opcode == opcode)
        {
            command_name = opcode_name_table[i].command_name;
            break;
        }
    }

    return command_name;
}

#ifndef APPL_HCI_NO_PROFILE_EVENT_IND
/**
 *  appl_hci_register_callback
 *  This function registers the HCI call back function.
 *
 *  @param  void (*hci_event_ind_cb) (UCHAR*, UINT8*, UINT8)(IN):
 *  Function pointer to the callback function that has to be
 *  registered with HCI.
 *
 *  @return
 *          API_SUCCESS : callback function is registered successfully
 *          Error Codes : reason for the failure
 */
API_RESULT appl_hci_register_callback
           (
               API_RESULT (* callback_fn)
                          (
                              UINT8    event_code,
                              UINT8 *  event_data,
                              UINT8    event_datalen
                          )
           )
{
    UINT32 i;

    /* Null Check for Callback Function */
    if (NULL == callback_fn)
    {
        IotLogDebug("Unable to register NULL as HCI Callback\n");
        return API_FAILURE;
    }

    /* Lock HCI */
    APPL_HCI_MUTEX_LOCK();

    /* Check if the number of callbacks registered has reached Max */
    if (APPL_MAX_NUM_HCI_CB == appl_num_hci_cb)
    {
        IotLogDebug("FAILED to Register HCI Callback. Limit Reached - %d\n",
        appl_num_hci_cb);

        APPL_HCI_MUTEX_UNLOCK();
        return API_FAILURE;
    }

    /* Verify that the Callback function is NOT already registered */
    for (i = 0; i < appl_num_hci_cb; i++)
    {
        if (appl_hci_cb_list[i].hci_event_ind_cb == callback_fn)
        {
            IotLogDebug("FAILED to Register HCI Callback. Already Registered\n");

            APPL_HCI_MUTEX_UNLOCK();
            return API_FAILURE;
        }
    }

    IotLogDebug("Registering HCI Callback Function at Location %d\n",
    appl_num_hci_cb);

    /* Register the Callback Function */
    appl_hci_cb_list[appl_num_hci_cb++].hci_event_ind_cb = callback_fn;

    IotLogDebug("Number of Registered HCI Callback Functions = %d\n",
    appl_num_hci_cb);

    /* Unlock HCI */
    APPL_HCI_MUTEX_UNLOCK();

    return API_SUCCESS;
}


/**
 *  appl_hci_unregister_callback
 *  This function unregisters the HCI callback function.
 *
 *  @param  void (*hci_event_ind_cb) (UCHAR*, UINT8*, UINT8)(IN):
 *  Function pointer to the callback function that has to be
 *  unregistered with HCI.
 *
 *  @return
 *          API_SUCCESS : callback function is registered successfully
 *          Error Codes : reason for the failure
 */
API_RESULT appl_hci_unregister_callback
           (
               API_RESULT (* callback_fn)
                          (
                              UINT8    event_code,
                              UINT8 *  event_data,
                              UINT8    event_datalen
                          )
           )
{
    UINT32 i, j;
    API_RESULT retval;

    /* Null Check for Callback Function */
    if (NULL == callback_fn)
    {
        IotLogDebug("Unable to unregister NULL as HCI Callback\n");
        return API_FAILURE;
    }

    /* Lock HCI */
    APPL_HCI_MUTEX_LOCK();

    /*
     *  Check if the Callback Function is registered.
     *  If found, remove from the Callback Function List.
     */
    for (i = 0; i < appl_num_hci_cb; i++)
    {
        if (appl_hci_cb_list[i].hci_event_ind_cb == callback_fn)
        {
            IotLogDebug("Unregistering HCI Callback Function from Location %u\n",
            (unsigned int) i);

            /* Unregister the Callback Function */
            appl_hci_cb_list[i].hci_event_ind_cb = NULL;
            break;
        }
    }

    if (i < appl_num_hci_cb)
    {
        for (j = i+1; j < appl_num_hci_cb; j ++)
        {
            appl_hci_cb_list[j - 1].hci_event_ind_cb =
            appl_hci_cb_list[j].hci_event_ind_cb;
        }

        /* Decrement Number of Callback Functions registered */
        appl_num_hci_cb --;

        IotLogDebug("Number of Registered HCI Callback Functions = %d\n",
        appl_num_hci_cb);

        retval = API_SUCCESS;
    }
    else
    {
        IotLogDebug("Cannot Unregister HCI Function Pointer. NOT in CB List\n");

        retval = API_FAILURE;
    }

    /* Unlock HCI */
    APPL_HCI_MUTEX_UNLOCK();
    return retval;
}
#endif /* APPL_HCI_NO_PROFILE_EVENT_IND */


/**
 *  appl_hci_set_esco_channel_parameters
 *  This function registers the eSCO Connection Parameters.
 *
 *  \param  sco_params (IN):
 *  Pointer to the eSCO Connection Parameters to be saved,
 *  to use while accepting next eSCO Connection request.
 *
 *  \param set_sco_param (IN):
 *  Flag indicates eSCO Parameter to be set or not.
 *
 *  @return
 *          API_SUCCESS : callback function is registered successfully
 *          Error Codes : reason for the failure
 */
#ifndef HCI_NO_ESCO_AUTO_ACCEPT
#ifdef BT_HCI_1_2
API_RESULT appl_hci_set_esco_channel_parameters
           (
               /* IN */ UCHAR               set_sco_param,
               /* IN */ HCI_SCO_IN_PARAMS * sco_params
           )
{
    /* Null Check for eSCO parameters */
    if ((BT_TRUE == set_sco_param) && (NULL == sco_params))
    {
        IotLogDebug("Unable to set NULL as eSCO parameters\n");
        return API_FAILURE;
    }

    /* Lock HCI */
    APPL_HCI_MUTEX_LOCK();

    /* Set flag */
    appl_hci_set_esco_connect_param = set_sco_param;

    if (BT_TRUE == appl_hci_set_esco_connect_param)
    {
        appl_esco_params = *(sco_params);
    }

    /* Unlock HCI */
    APPL_HCI_MUTEX_UNLOCK();

    return API_SUCCESS;
}
#endif /* BT_HCI_1_2 */
#endif /* HCI_NO_ESCO_AUTO_ACCEPT */


#ifdef BT_EIR
void appl_print_extended_inquiry_response(UCHAR *eir_data, UINT16 eir_datalen)
{
    UINT16 index, length, consumed_length;

    IotLogDebug("Extended Inquiry Response\n");

    IotLogDebug("Total Length = %d\n", eir_datalen);

    if((NULL == eir_data) || (0 == eir_datalen))
    {
        IotLogDebug("Empty Response\n");

        return;
    }

    consumed_length = 0;

    while(consumed_length < eir_datalen)
    {
        /* First Byte is the Length Field */
        length = *eir_data;

        IotLogDebug("\tEIR Element Length: 0x%02X\n", length);

        if(0 == length)
        {
            break;
        }

        eir_data ++;
        IotLogDebug("\tEIR Data Type: ");
        switch(*eir_data)
        {
        case HCI_EIR_DATA_TYPE_FLAGS:
            IotLogDebug("Flags\n");
            break;

        case HCI_EIR_DATA_TYPE_INCOMPLETE_16_BIT_UUIDS:
            IotLogDebug("More 16-bit UUIDs available\n");
            break;

        case HCI_EIR_DATA_TYPE_COMPLETE_16_BIT_UUIDS:
            IotLogDebug("Complete list of 16-bit UUIDs\n");
            break;

        case HCI_EIR_DATA_TYPE_INCOMPLETE_32_BIT_UUIDS:
            IotLogDebug("More 32-bit UUIDs available\n");
            break;

        case HCI_EIR_DATA_TYPE_COMPLETE_32_BIT_UUIDS:
            IotLogDebug("Complete list of 32-bit UUIDs\n");
            break;

        case HCI_EIR_DATA_TYPE_INCOMPLETE_128_BIT_UUIDS:
            IotLogDebug("More 128-bit UUIDs available\n");
            break;

        case HCI_EIR_DATA_TYPE_COMPLETE_128_BIT_UUIDS:
            IotLogDebug("Complete list of 128-bit UUIDs\n");
            break;

        case HCI_EIR_DATA_TYPE_SHORTENED_LOCAL_NAME:
            IotLogDebug("Shortened local name\n");
            break;

        case HCI_EIR_DATA_TYPE_COMPLETE_LOCAL_NAME:
            IotLogDebug("Complete local name\n");
            break;

        case HCI_EIR_DATA_TYPE_TX_POWER_LEVEL:
            IotLogDebug("TX Power Level\n");
            break;

        case HCI_EIR_DATA_TYPE_CLASS_OF_DEVICE:
            IotLogDebug("Class of Device\n");
            break;

        case HCI_EIR_DATA_TYPE_SIMPLE_PAIRING_HASH_C:
            IotLogDebug("Simple Pairing Hash C\n");
            break;

        case HCI_EIR_DATA_TYPE_SIMPLE_PAIRING_RANDOMIZER_R:
            IotLogDebug("Simple Pairing Randomizer R\n");
            break;

        case HCI_EIR_DATA_TYPE_DEVICE_ID:
            IotLogDebug("Device ID\n");
            break;

        case HCI_EIR_DATA_TYPE_MANUFACTURER_SPECIFIC_DATA:
            IotLogDebug("Manufacturer Specific Data\n");
            break;
        }

        eir_data ++;

        for (index = 0; index < (length - 1); index ++)
        {
            IotLogDebug("\t%02X ", *eir_data);

            eir_data ++;
        }

        IotLogDebug("\n");

        consumed_length += (length + 1);
    }

    return;
}


void appl_hci_write_extended_inquiry_response(void)
{
    UCHAR           fec_required;

    /* Only Name and Service UUID will be sent through EIR */
    UCHAR           device_name[] = "EtherMind";

#ifdef SDP_SERVER
    UINT32          num_uuids;
    UCHAR          *uuids;
    HCI_EIR_DATA    eir_data[2];
#else
    HCI_EIR_DATA    eir_data[1];
#endif /* SDP_SERVER */

    /* Read the EIR response */
    BT_hci_read_extended_inquiry_response();

    fec_required = (UCHAR)1;

    /* Set Local Name */
    eir_data[0].eir_data_type = HCI_EIR_DATA_TYPE_COMPLETE_LOCAL_NAME;
    eir_data[0].eir_data = device_name;
    eir_data[0].eir_data_len = (UCHAR)BT_str_len(device_name);

#ifdef SDP_SERVER
    /* Set Service UUIDs */
    BT_dbase_get_num_service_class_uuids
    (
       &num_uuids
    );

    IotLogDebug("Number of UUIDs = %lu\n", num_uuids);

    uuids = (UCHAR *)BT_alloc_mem(num_uuids * sizeof(UINT16));

    if (NULL == uuids)
    {
        IotLogDebug("Failed to allocate memory for UUIDs\n");
        return;
    }

    num_uuids *= sizeof(UINT16);

    BT_dbase_get_service_class_uuids
    (
        uuids,
        &num_uuids
    );

    eir_data[1].eir_data_type = HCI_EIR_DATA_TYPE_COMPLETE_16_BIT_UUIDS;
    eir_data[1].eir_data = uuids;
    eir_data[1].eir_data_len = (UCHAR)num_uuids;

    BT_hci_write_extended_inquiry_response
    (
        fec_required,
        eir_data,
        2
    );

    BT_free_mem(uuids);
    uuids = NULL;
#else
    BT_hci_write_extended_inquiry_response
    (
        fec_required,
        eir_data,
        1
    );
#endif /* SDP_SERVER */

    BT_hci_read_extended_inquiry_response();
}

#endif /* BT_EIR */

#ifdef HCI_SCO
/**
 *  \fn appl_hci_esco_audio_config
 *
 *  \brief To configure mSBC codec and MIC & SPKR for SCO audio
 *
 *  \par Description:
 *       This function does the following:
 *        - Initializes Global Variables
 *        - Initializes mSBC Encoder & Decoder for Wideband Speech.
          - Register callback with HCI for SCO data
 *        - Configure Local Wave recorder(MIC) & playback(SPKR) for SCO Audio.
 *
 *  \param None
 *
 *  \return API_RESULT: API_SUCCESS or an Error Code describing
 *                      the cause of failure.
 *
 *  \note
 */
void appl_hci_esco_audio_config(void)
{
    API_RESULT retval;

    /* Reset Global Variables */
    sco_connection_handle = HCI_INVALID_CONNECTION_HANDLE;

    sco_rx_msbc_data_offset            = 0x00;
    expected_sco_rx_msbc_frame_seq_num = 0x00;
    sco_tx_msbc_frame_seq_num          = 0x00;
    sco_tx_pcm_data_offset             = 0x00;
    BT_mem_set
    (
        sco_tx_pcm_data,
        0x00,
        sizeof (sco_tx_pcm_data)
    );

    BT_mem_set
    (
        sco_rx_msbc_data,
        0x00,
        sizeof (sco_rx_msbc_data)
    );

    /**
     * Init mSBC Encoder & Decoder for Wideband Speech.
     */

    /* mSBC Encoder params */
    msbc_enc_io.encoder_params.channel_mode = SBC_CHANNEL_MONO;
    msbc_enc_io.encoder_params.nrof_channels = 1;
    msbc_enc_io.encoder_params.sampling_frequency = SBC_SAMPLING_FREQ_16000;
    msbc_enc_io.encoder_params.allocation_method = SBC_LOUDNESS;
    msbc_enc_io.encoder_params.nrof_blocks = SBC_BLOCK_SIZE_15;
    msbc_enc_io.encoder_params.nrof_subbands = SBC_SUBBAND_8;
    msbc_enc_io.encoder_params.bitpool = 26;

    /* Calculate mSBC Encoded frame length */
    sbc_calculate_frame_length(&msbc_enc_io.encoder_params, &msbc_enc_output_frame_len);

    /* Init mSBC Encoder */
    retval = sbc_encoder_init
             (
                 &msbc_enc_io
             );
    if (API_SUCCESS != retval)
    {
        IotLogDebug ("mSBC Encoder Init Failed, returning ...\n");
        return;
    }
    else
    {
        IotLogDebug ("mSBC Encoder Initialized\n");
    }

    /* Init mSBC Decoder */
    sbc_decoder_init
    (
        &msbc_dec_io
    );

    IotLogDebug ("mSBC Decoder Initialized\n");

    /**
     * Init Wave Record
     */
    sco_audio_ep_mic_info.type = HFP_AUDIO_EP_MIC; /* Endpoint type: MIC */
    sco_audio_ep_mic_info.num_ch = 0x01;     /* No. of channels: Mono */
    sco_audio_ep_mic_info.sample_len = 16;   /* Each PCM sample size */
    sco_audio_ep_mic_info.sampl_freq = 16000; /* Sampling frequency */
    sco_audio_ep_mic_info.data_size  = 512; /* Record data length */
    sco_audio_ep_mic_info.hfp_sco_ep_cb = appl_hci_sco_audio_mic_record_callback;

    /* TBD: How did we arrive at 512? */

    /* Setup MIC */
    retval = hfp_sco_audio_setup_pl
             (
                 &sco_audio_ep_mic_info
             );
    if (API_SUCCESS != retval)
    {
        IotLogDebug ("MIC setup for SCO audio failed\n");
    }
    else
    {
        IotLogDebug ("MIC setup for SCO audio: Success\n");
    }

    /**
     * Init Wave Play
     */
    sco_audio_ep_spkr_info.type = HFP_AUDIO_EP_SPKR; /* Endpoint type: SPEAKER */
    sco_audio_ep_spkr_info.num_ch = 0x01;     /* No. of channels: Mono */
    sco_audio_ep_spkr_info.sample_len = 16;   /* Each PCM sample size */
    sco_audio_ep_spkr_info.sampl_freq = 16000; /* Sampling frequency */
    sco_audio_ep_spkr_info.data_size  = 512; /* Playback data length */
    sco_audio_ep_spkr_info.hfp_sco_ep_cb = appl_hci_sco_audio_spkr_playack_callback; /* callback */

    /* Setup SPEAKER */
    retval = hfp_sco_audio_setup_pl
             (
                 &sco_audio_ep_spkr_info
             );
    if (API_SUCCESS != retval)
    {
        IotLogDebug ("SPEAKER setup for SCO audio failed\n");
    }
    else
    {
        IotLogDebug ("SPEAKER setup for SCO audio: Success\n");
    }

    /* Register callback with HCI for SCO data */
    BT_hci_register_sco_data_handler
    (
        appl_hci_esco_msbc_data_handler
    );

    IotLogDebug ("Registered callback with HCI for SCO data\n");

    return;
}

/**
 *  \fn appl_hci_sco_audio_mic_record_callback
 *
 *  \brief To encode the recorded voice samples and transmits to the peer.
 *
 *  \par Description:
 *       This function will be called by the voice recorder(MIC) and gives the recorded
 *       raw PCM samples to the application. It does the following:
 *        - It encodes the raw PCM samples using mSBC Encoder and forms a eSCO packet &
 *          transmits to the peer.
 *        - If available PCM data length is not sufficent for mSBC Encoder input, It saves
 *          in a buffer and process the same in next callback call.
 *
 *  \param (in) mic_data
 *         Pointer to wave recorder(MIC) audio data.
 *
 *  \param (in) mic_datalen
 *         Length of wave recorderMIC) audio data.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \note
 *    Assumption is mic_datalen WILL alway be < 512 bytes. If it is greater than 512
 *    then change the implementation accordingly.
 */
void appl_hci_sco_audio_mic_record_callback
     (
         /* IN */ UCHAR     *mic_data,
         /* IN */ UINT16     mic_datalen
     )
{
    API_RESULT  retval;
    UINT16      input_data_offset;
    UINT16      outut_data_offset;
    UINT16      pcm_datalen;
    UCHAR       msbc_enc_data[256];

    /**
     * Note:
     * Assumption is mic_datalen WILL alway be < 512 bytes
     */

    if ((NULL == mic_data) ||
        (0x00 == mic_datalen))
    {
        /* Invalid params */
        return;
    }

#ifdef HAVE_SCO_MIC_DATA_DUMP
    /* Write to file */
    if (NULL != mic_fptr)
    {
        BT_fops_file_write(mic_data, mic_datalen, mic_fptr, &actual);
    }
#endif /* HAVE_SCO_MIC_DATA_DUMP */

    /* Init */
    pcm_datalen         = mic_datalen + sco_tx_pcm_data_offset;
    input_data_offset   = 0x00;
    outut_data_offset   = 0x00;

    /**
     * Process the received data
     */
    while (pcm_datalen >= MSBC_ENCODER_INPUT_FRAME_LEN)
    {
        /* Check if previously received pcm_data has to be processed */
        if (0x00 == sco_tx_pcm_data_offset)
        {
            /* === No Previous Data === */
            msbc_enc_io.encoder_input_buffer  = (UINT16 *)&mic_data[input_data_offset];
            msbc_enc_io.encoder_input_buflen  = MSBC_ENCODER_INPUT_FRAME_LEN;
        }
        else
        {
            /* === Yes, then process Previous Data === */
            BT_mem_copy
            (
                &sco_tx_pcm_data[sco_tx_pcm_data_offset],
                mic_data,
                (MSBC_ENCODER_INPUT_FRAME_LEN - sco_tx_pcm_data_offset)
            );

            msbc_enc_io.encoder_input_buffer  = (UINT16 *)&sco_tx_pcm_data[0];
            msbc_enc_io.encoder_input_buflen  = MSBC_ENCODER_INPUT_FRAME_LEN;
        }

        /**
         * Pack the H2 Header with synchronization word and sequence number.
         * Ref. page #115 of HFPv1.7 Spec. for more details.
         */
        BT_PACK_LE_2_BYTE_VAL(&msbc_enc_data[outut_data_offset], HFP_SCO_H2_SYNC_HDR);

        /* Pack Sequence number */
        if (0x00 == sco_tx_msbc_frame_seq_num)
        {
            msbc_enc_data[outut_data_offset + 1] |= 0x00;
        }
        else if (0x01 == sco_tx_msbc_frame_seq_num)
        {
            msbc_enc_data[outut_data_offset + 1] |= 0x30;
        }
        else if (0x02 == sco_tx_msbc_frame_seq_num)
        {
            msbc_enc_data[outut_data_offset + 1] |= 0xC0;
        }
        else if (0x03 == sco_tx_msbc_frame_seq_num)
        {
            msbc_enc_data[outut_data_offset + 1] |= 0xF0;
        }
        else
        {
            /* Ideally should not come here */
            sco_tx_msbc_frame_seq_num = 0x00;
        }

        /* Update the SCO packet offset */
        outut_data_offset += 2;

        msbc_enc_io.encoder_output_buffer = &msbc_enc_data[outut_data_offset];
        msbc_enc_io.encoder_output_buflen = msbc_enc_output_frame_len; /* 57 */

        /* Start Encoding Process */
        retval = sbc_encoder
                 (
                     &msbc_enc_io
                 );
        if (API_SUCCESS != retval)
        {
            IotLogDebug ("mSBC Encoding Failed\n");
        }

#ifdef HAVE_SCO_TX_DATA_DUMP
        /* Write to file */
        if (NULL != sco_tx_fptr)
        {
            BT_fops_file_write
            (
                msbc_enc_io.encoder_output_buffer,
                msbc_enc_io.encoder_output_buflen,
                sco_tx_fptr,
                &actual
            );
        }
#endif /* HAVE_SCO_TX_DATA_DUMP */

        /* Update the processed data lengths */
        pcm_datalen -= MSBC_ENCODER_INPUT_FRAME_LEN;
        input_data_offset += (MSBC_ENCODER_INPUT_FRAME_LEN - sco_tx_pcm_data_offset);
        outut_data_offset += MSBC_ENCODER_OUTPUT_FRAME_LEN; /* 57 */

        /* Update the sequence number */
        sco_tx_msbc_frame_seq_num += 0x01;

        /* Max. value of seq. number is 3 */
        sco_tx_msbc_frame_seq_num &= 0x03;

        sco_tx_pcm_data_offset = 0x00;
    }

    if (outut_data_offset > 0)
    {

        /* appl_dump_bytes(msbc_enc_data, outut_data_offset); */

        /* Send packed SCO data to the peer */
        retval = BT_hci_sco_write
                 (
                     sco_connection_handle,
                     msbc_enc_data,
                     outut_data_offset
                 );
        if (API_SUCCESS != retval)
        {
            IotLogDebug ("Failed: BT_hci_sco_write\n");
        }
    }

    /**
     * If the pcm_datalen NOT equal to 0, then save and process in next callback
     */
    if (0x00 != pcm_datalen)
    {
        BT_mem_copy
        (
            &sco_tx_pcm_data[0],
            &mic_data[input_data_offset],
            pcm_datalen
        );

        sco_tx_pcm_data_offset = pcm_datalen;
    }

    return;
}

void appl_hci_sco_audio_spkr_playack_callback
     (
         /* IN */ UCHAR     *data,
         /* IN */ UINT16     datalen
     )
{
    BT_IGNORE_UNUSED_PARAM(data);
    BT_IGNORE_UNUSED_PARAM(datalen);

    /* For now just return */

    return;
}

/**
 *  \fn appl_hci_esco_msbc_data_handler
 *
 *  \brief To decodes the received mSBC frames from HCI and submit the decoded PCM samples to the
           speaker for local playback.
 *
 *  \par Description:
 *       This function will be called by the HCI and gives the eSCO packet containning
 *       mSBC encoded data to the application. It does the following:
 *        - It decodes the received mSBC frames using mSBC Decoder and sends to
            speaker for playback.
 *        - It also stores the partially received mSBC frame and decodes after receiving
 *          a complete mSBC frame.
 *
 *  \param (in) packet
 *         Pointer to Transparent eSCO data from HCI.
 *
 *  \param (in) packet_len
 *         Length of Transparent eSCO data.
 *
 *  \return API_RESULT: API_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \note
 */
API_RESULT appl_hci_esco_msbc_data_handler
           (
               /* IN */ UCHAR    * packet,
               /* IN */ UINT16     packet_len
           )
{
    API_RESULT retval;
    UINT16     sco_handle;
    UINT16     esco_h2_sync_hdr;
    UCHAR      sco_data_len;
    UCHAR      offset;
    UCHAR      rx_seq_num;
    UCHAR      msbc_decoded_pcm_data[1024];
    UCHAR      msbc_decoded_pcm_data_len;
    UINT16     msbc_decoded_pcm_data_offset;

    /* Check */
    if ((NULL == packet) ||
        (packet_len <= 3))
    {
        IotLogDebug ("Invalide SCO packet \n");
        return API_FAILURE;
    }

    /* Init */
    retval = API_SUCCESS;
    msbc_decoded_pcm_data_len    = 0;
    msbc_decoded_pcm_data_offset = 0;
    rx_seq_num                   = 0;

    /* Extract SCO Handle */
    BT_UNPACK_LE_2_BYTE
    (
        &sco_handle,
         packet
    );

    /* Validate SCO Handle */
    if (sco_handle != sco_connection_handle)
    {
        IotLogDebug ("SCO Connection Handle Does NOT match\n");
        return API_FAILURE;
    }
    else
    {
        ;
        /* IotLogDebug("SCO Connection Handle: 0x%04X\n", sco_handle); */
        /* appl_dump_bytes(packet, packet_len); */
    }

    /* TBD: Remove this return. Just for testing. */
    /* return API_SUCCESS; */

    /* Extract SCO data len */
    sco_data_len = packet[2];

    /* Added previously pending SCO data */
    sco_data_len += sco_rx_msbc_data_offset;

    /* For SCO handle[2], SCO data length[1] */
    offset = 0x03;

    /* Process the data */
    while (sco_data_len >= MSBC_DECODER_INPUT_FRAME_LEN)
    {
        /* Check if previously received mSBC sco_data has to be processed */
        if (0x00 == sco_rx_msbc_data_offset)
        {
            /* === No Previous Data === */

            /* Extract H2 Sync Header */
            BT_UNPACK_LE_2_BYTE
            (
                &esco_h2_sync_hdr,
                &packet[offset]
            );

            /* Update the offset */
            offset += 2;
            sco_data_len -= 2;

            /* Validate eSCO H2 Sync Header */
            if (HFP_SCO_H2_SYNC_HDR != (esco_h2_sync_hdr & 0x0FFF))
            {
                IotLogDebug ("eSCO H2 Header does NOT match\n");
                /* return error ? */
            }

            /* Validate the sequence number */
            switch((esco_h2_sync_hdr & 0xF000))
            {
            case 0x0000:
                rx_seq_num = 0;
                break;

            case 0x3000:
                rx_seq_num = 1;
                break;

            case 0xC000:
                rx_seq_num = 2;
                break;

            case 0xF000:
                rx_seq_num = 3;
                break;
            }

            if (expected_sco_rx_msbc_frame_seq_num == rx_seq_num)
            {
                /* Set mSBC Decoder input buffer params */
                msbc_dec_io.decoder_input_buffer = &packet[offset];
                msbc_dec_io.decoder_input_buflen = MSBC_DECODER_INPUT_FRAME_LEN;

                /* Update next expected sequence number */
                expected_sco_rx_msbc_frame_seq_num += 1;
            }
            else
            {
                IotLogDebug ("Have missed mSBC frame\n");
                IotLogDebug ("Expected frame seq_num:0x%02x, Received seq_num:0x%02x\n",
                    expected_sco_rx_msbc_frame_seq_num, rx_seq_num);

                /* Set next expected based on rx_seq_num */
                expected_sco_rx_msbc_frame_seq_num = (rx_seq_num + 1);

                /**
                 * TODO: Have to handle through PLC.
                 */
                msbc_dec_io.decoder_input_buffer = &packet[offset];
                msbc_dec_io.decoder_input_buflen = MSBC_DECODER_INPUT_FRAME_LEN;
            }

            /* Max. seq. number value is 3 */
            expected_sco_rx_msbc_frame_seq_num &= 0x03;
        }
        else
        {
            /* Yes, Handle previously pending data */
            BT_mem_copy
            (
                &sco_rx_msbc_data[sco_rx_msbc_data_offset],
                &packet[offset],
                (MSBC_DECODER_INPUT_FRAME_LEN - sco_rx_msbc_data_offset)
            );

            /* Set mSBC output buffer params */
            msbc_dec_io.decoder_input_buffer = &sco_rx_msbc_data[0];
            msbc_dec_io.decoder_input_buflen = MSBC_DECODER_INPUT_FRAME_LEN;
        }

        /* Set mSBC output buffer params */
        msbc_dec_io.decoder_output_buffer = (UINT16 *)&msbc_decoded_pcm_data[msbc_decoded_pcm_data_offset];
        msbc_dec_io.decoder_output_buflen = MSBC_DECODER_OUTPUT_FRAME_LEN;

#ifdef HAVE_SCO_RX_DATA_DUMP
        /* Write to file */
        if (NULL != sco_rx_fptr)
        {
            BT_fops_file_write
            (
                msbc_dec_io.decoder_input_buffer,
                msbc_dec_io.decoder_input_buflen,
                sco_rx_fptr,
                &actual
            );
        }
#endif /* HAVE_SCO_RX_DATA_DUMP */

        /* Start Decoding Process */
        retval = sbc_decoder
                 (
                     &msbc_dec_io
                 );
        if (API_SUCCESS != retval)
        {
            IotLogDebug ("mSBC Decoding Failed\n");
        }

#ifdef HAVE_SCO_SPKR_DATA_DUMP
    /* Write to file */
    if (NULL != spkr_fptr)
    {

        BT_fops_file_write
        (
            msbc_dec_io.decoder_output_buffer,
            msbc_dec_io.decoder_output_buflen,
            spkr_fptr,
            &actual
        );

    }
#endif /* HAVE_SCO_SPKR_DATA_DUMP */

        /* Update the processed data lengths */
        sco_data_len -= MSBC_DECODER_INPUT_FRAME_LEN;
        offset += (MSBC_DECODER_INPUT_FRAME_LEN - sco_rx_msbc_data_offset);
        msbc_decoded_pcm_data_offset += MSBC_DECODER_OUTPUT_FRAME_LEN;

        sco_rx_msbc_data_offset = 0;

    }

    if (msbc_decoded_pcm_data_offset >= MSBC_DECODER_OUTPUT_FRAME_LEN)
    {
        /* Write the decoded PCM data to the speaker for local playback */
        hfp_sco_audio_spkr_play_pl
        (
            HFP_AUDIO_EP_SPKR,
            msbc_decoded_pcm_data,
            msbc_decoded_pcm_data_offset
        );
    }

    /**
     * If the sco_data_len NOT equal to 0, then save and process in next callback
     */
    if (0x00 != sco_data_len)
    {
        BT_mem_copy
        (
            &sco_rx_msbc_data[sco_rx_msbc_data_offset],
            &packet[offset],
            sco_data_len
        );

        sco_rx_msbc_data_offset += sco_data_len;
    }

    return retval;
}
#endif /* HCI_SCO */
