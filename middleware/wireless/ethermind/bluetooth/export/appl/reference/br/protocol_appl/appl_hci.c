
/**
 *  \file appl_hci.c
 *
 *  Source File for HCI Command Line Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/*   Copyright 2008-2020 NXP   */

/* ----------------------------------------- Header File Inclusion */
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
#endif /* HCI_SCO */

/* For platform playback */
#include "sco_audio_pl.h"

/* ----------------------------------------- External Global Variables */
extern BT_DEVICE_ADDR g_bd_addr;

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */
#ifndef APPL_HCI_NO_PROFILE_EVENT_IND
/** Array of HCI Profile Event Indication Callbacks */
DECL_STATIC APPL_HCI_CB_PTR appl_hci_cb_list [ APPL_MAX_NUM_HCI_CB ];

/** Running Counter of Number of HCI Profile Event Indication CB Registered */
DECL_STATIC UCHAR appl_num_hci_cb;
#endif /* APPL_HCI_NO_PROFILE_EVENT_IND */

#ifdef BR_EDR_HCI
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
#endif /* BR_EDR_HCI */

DECL_STATIC UINT16                 sco_connection_handle = HCI_INVALID_CONNECTION_HANDLE;
#ifdef BR_EDR_HCI
DECL_STATIC SCO_AUDIO_EP_INFO  sco_audio_config;
#endif

#ifdef HCI_SCO
DECL_STATIC UCHAR                  sco_tx_pcm_data[256U];
DECL_STATIC UINT16                 sco_tx_pcm_data_offset;
DECL_STATIC UCHAR                  sco_tx_msbc_frame_seq_num;

DECL_STATIC SBC_ENCODER_IO_STRUCT  msbc_enc_io;
DECL_STATIC SBC_DECODER_IO_STRUCT  msbc_dec_io;

DECL_STATIC UINT16                 msbc_enc_output_frame_len;

DECL_STATIC UCHAR                  sco_rx_msbc_data[64U];
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

static const APPL_HCI_OPCODE_NAME opcode_name_table[] =
{
    /* No Operation Opcode */
    {0x0000U, "HCI_NO_OPERATION_OPCODE"                                 },

    /* Link Control Commands - OGF : 0x01 */
    {0x0401U, "HCI_INQUIRY_OPCODE"                                      },
    {0x0402U, "HCI_INQUIRY_CANCEL_OPCODE"                               },
    {0x0403U, "HCI_PERIODIC_INQUIRY_MODE_OPCODE"                        },
    {0x0404U, "HCI_EXIT_PERIODIC_INQUIRY_MODE_OPCODE"                   },
    {0x0405U, "HCI_CREATE_CONNECTION_OPCODE"                            },
    {0x0406U, "HCI_DISCONNECT_OPCODE"                                   },
    {0x0407U, "HCI_ADD_SCO_CONNECTION_OPCODE"                           },
    {0x0409U, "HCI_ACCEPT_CONNECTION_REQUEST_OPCODE"                    },
    {0x040AU, "HCI_REJECT_CONNECTION_REQUEST_OPCODE"                    },
    {0x040BU, "HCI_LINK_KEY_REQUEST_REPLY_OPCODE"                       },
    {0x040CU, "HCI_LINK_KEY_REQUEST_NEGATIVE_REPLY_OPCODE"              },
    {0x040DU, "HCI_PIN_CODE_REQUEST_REPLY_OPCODE"                       },
    {0x040EU, "HCI_PIN_CODE_REQUEST_NEGATIVE_REPLY_OPCODE"              },
    {0x040FU, "HCI_CHANGE_CONNECTION_PACKET_TYPE_OPCODE"                },
    {0x0411U, "HCI_AUTHENTICATION_REQUESTED_OPCODE"                     },
    {0x0413U, "HCI_SET_CONNECTION_ENCRYPTION_OPCODE"                    },
    {0x0415U, "HCI_CHANGE_CONNECTION_LINK_KEY_OPCODE"                   },
    {0x0417U, "HCI_MASTER_LINK_KEY_OPCODE"                              },
    {0x0419U, "HCI_REMOTE_NAME_REQUEST_OPCODE"                          },
    {0x041BU, "HCI_READ_REMOTE_SUPPORTED_FEATURES_OPCODE"               },
    {0x041DU, "HCI_READ_REMOTE_VERSION_INFORMATION_OPCODE"              },
    {0x041FU, "HCI_READ_CLOCK_OFFSET_OPCODE"                            },

#ifdef BT_HCI_1_2
    {0x0408U, "HCI_CREATE_CONNECTION_CANCEL_OPCODE"                     },
    {0x041AU, "HCI_REMOTE_NAME_REQUEST_CANCEL_OPCODE"                   },
    {0x041CU, "HCI_READ_REMOTE_EXTENDED_FEATURES_OPCODE"                },
    {0x0428U, "HCI_SETUP_SYNCHRONOUS_CONNECTION_OPCODE"                 },
    {0x0429U, "HCI_ACCEPT_SYNCHRONOUS_CONNECTION_REQUEST_OPCODE"        },
    {0x042AU, "HCI_REJECT_SYNCHRONOUS_CONNECTION_REQUEST_OPCODE"        },
#endif /* BT_HCI_1_2 */

    /* Link Policy Commands - OGF : 0x02 */
    {0x0801U, "HCI_HOLD_MODE_OPCODE"                                    },
    {0x0803U, "HCI_SNIFF_MODE_OPCODE"                                   },
    {0x0804U, "HCI_EXIT_SNIFF_MODE_OPCODE"                              },
    {0x0805U, "HCI_PARK_MODE_OPCODE"                                    },
    {0x0806U, "HCI_EXIT_PARK_MODE_OPCODE"                               },
    {0x0807U, "HCI_QOS_SETUP_OPCODE"                                    },
    {0x0809U, "HCI_ROLE_DISCOVERY_OPCODE"                               },
    {0x080BU, "HCI_SWITCH_ROLE_OPCODE"                                  },
    {0x080CU, "HCI_READ_LINK_POLICY_SETTINGS_OPCODE"                    },
    {0x080DU, "HCI_WRITE_LINK_POLICY_SETTINGS_OPCODE"                   },
    {0x0811U, "HCI_SNIFF_SUBRATING_OPCODE"                              },
    {0x0807U, "HCI_QOS_SETUP_OPCODE"                                    },

#ifdef BT_HCI_1_2
    {0x080EU, "HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_OPCODE"            },
    {0x080FU, "HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_OPCODE"           },
    {0x0810U, "HCI_FLOW_SPECIFICATION_OPCODE"                           },
#endif /* BT_HCI_1_2 */

    /* Host Controller & BaseBand Commands - OGF : 0x03 */
    {0x0C01U, "HCI_SET_EVENT_MASK_OPCODE"                               },
    {0x0C03U, "HCI_RESET_OPCODE"                                        },
    {0x0C05U, "HCI_SET_EVENT_FILTER_OPCODE"                             },
    {0x0C08U, "HCI_FLUSH_OPCODE"                                        },
    {0x0C09U, "HCI_READ_PIN_TYPE_OPCODE"                                },
    {0x0C0AU, "HCI_WRITE_PIN_TYPE_OPCODE"                               },
    {0x0C0BU, "HCI_CREATE_NEW_UNIT_KEY_OPCODE"                          },
    {0x0C13U, "HCI_CHANGE_LOCAL_NAME_OPCODE"                            },
    {0x0C14U, "HCI_READ_LOCAL_NAME_OPCODE"                              },
    {0x0C15U, "HCI_READ_CONNECTION_ACCEPT_TIMEOUT_OPCODE"               },
    {0x0C16U, "HCI_WRITE_CONNECTION_ACCEPT_TIMEOUT_OPCODE"              },
    {0x0C17U, "HCI_READ_PAGE_TIMEOUT_OPCODE"                            },
    {0x0C18U, "HCI_WRITE_PAGE_TIMEOUT_OPCODE"                           },
    {0x0C19U, "HCI_READ_SCAN_ENABLE_OPCODE"                             },
    {0x0C1AU, "HCI_WRITE_SCAN_ENABLE_OPCODE"                            },
    {0x0C1BU, "HCI_READ_PAGE_SCAN_ACTIVITY_OPCODE"                      },
    {0x0C1CU, "HCI_WRITE_PAGE_SCAN_ACTIVITY_OPCODE"                     },
    {0x0C1DU, "HCI_READ_INQUIRY_SCAN_ACTIVITY_OPCODE"                   },
    {0x0C1EU, "HCI_WRITE_INQUIRY_SCAN_ACTIVITY_OPCODE"                  },
    {0x0C1FU, "HCI_READ_AUTHENTICATION_ENABLE_OPCODE"                   },
    {0x0C20U, "HCI_WRITE_AUTHENTICATION_ENABLE_OPCODE"                  },
    {0x0C21U, "HCI_READ_ENCRYPTION_MODE_OPCODE"                         },
    {0x0C22U, "HCI_WRITE_ENCRYPTION_MODE_OPCODE"                        },
    {0x0C23U, "HCI_READ_CLASS_OF_DEVICE_OPCODE"                         },
    {0x0C24U, "HCI_WRITE_CLASS_OF_DEVICE_OPCODE"                        },
    {0x0C25U, "HCI_READ_VOICE_SETTING_OPCODE"                           },
    {0x0C26U, "HCI_WRITE_VOICE_SETTING_OPCODE"                          },
    {0x0C27U, "HCI_READ_AUTOMATIC_FLUSH_TIMEOUT_OPCODE"                 },
    {0x0C28U, "HCI_WRITE_AUTOMATIC_FLUSH_TIMEOUT_OPCODE"                },
    {0x0C29U, "HCI_READ_NUM_BROADCAST_RETRANSMISSIONS_OPCODE"           },
    {0x0C2AU, "HCI_WRITE_NUM_BROADCAST_RETRANSMISSIONS_OPCODE"          },
    {0x0C2BU, "HCI_READ_HOLD_MODE_ACTIVITY_OPCODE"                      },
    {0x0C2CU, "HCI_WRITE_HOLD_MODE_ACTIVITY_OPCODE"                     },
    {0x0C2DU, "HCI_READ_TRANSMIT_POWER_LEVEL_OPCODE"                    },
    {0x0C2EU, "HCI_READ_SCO_FLOW_CONTROL_ENABLE_OPCODE"                 },
    {0x0C2FU, "HCI_WRITE_SCO_FLOW_CONTROL_ENABLE_OPCODE"                },
    {0x0C31U, "HCI_SET_HOST_CONTROLLER_TO_HOST_FLOW_CONTROL_OPCODE"     },
    {0x0C33U, "HCI_HOST_BUFFER_SIZE_OPCODE"                             },
    {0x0C35U, "HCI_HOST_NUMBER_OF_COMPLETED_PACKETS_OPCODE"             },
    {0x0C36U, "HCI_READ_LINK_SUPERVISION_TIMEOUT_OPCODE"                },
    {0x0C37U, "HCI_WRITE_LINK_SUPERVISION_TIMEOUT_OPCODE"               },
    {0x0C38U, "HCI_READ_NUMBER_OF_SUPPORTED_IAC_OPCODE"                 },
    {0x0C39U, "HCI_READ_CURRENT_IAC_LAP_OPCODE"                         },
    {0x0C3AU, "HCI_WRITE_CURRENT_IAC_LAP_OPCODE"                        },
    {0x0C3BU, "HCI_READ_PAGE_SCAN_PERIOD_MODE_OPCODE"                   },
    {0x0C3CU, "HCI_WRITE_PAGE_SCAN_PERIOD_MODE_OPCODE"                  },
    {0x0C3DU, "HCI_READ_PAGE_SCAN_MODE_OPCODE"                          },
    {0x0C3EU, "HCI_WRITE_PAGE_SCAN_MODE_OPCODE"                         },

#ifdef BT_HCI_1_2
    {0x0C3FU, "HCI_SET_AFH_HOST_CHANNEL_CLASSIFICATION_OPCODE"          },
    {0x0C42U, "HCI_READ_INQUIRY_SCAN_TYPE_OPCODE"                       },
    {0x0C43U, "HCI_WRITE_INQUIRY_SCAN_TYPE_OPCODE"                      },
    {0x0C44U, "HCI_READ_INQUIRY_MODE_OPCODE"                            },
    {0x0C45U, "HCI_WRITE_INQUIRY_MODE_OPCODE"                           },
    {0x0C46U, "HCI_READ_PAGE_SCAN_TYPE_OPCODE"                          },
    {0x0C47U, "HCI_WRITE_PAGE_SCAN_TYPE_OPCODE"                         },
    {0x0C48U, "HCI_READ_AFH_CHANNEL_ASSESSMENT_MODE_OPCODE"             },
    {0x0C49U, "HCI_WRITE_AFH_CHANNEL_ASSESSMENT_MODE_OPCODE"            },
#endif /* BT_HCI_1_2 */

#ifdef BT_2_1_EDR
    {0x0C51U, "HCI_READ_EXTENDED_INQUIRY_RESPONSE_OPCODE"               },
    {0x0C52U, "HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_OPCODE"              },
    {0x0C53U, "HCI_REFRESH_ENCRYPTION_KEY_OPCODE"                       },
    {0x0C55U, "HCI_READ_SIMPLE_PAIRING_MODE_OPCODE"                     },
    {0x0C56U, "HCI_WRITE_SIMPLE_PAIRING_MODE_OPCODE"                    },
    {0x0C57U, "HCI_READ_LOCAL_OOB_DATA_OPCODE"                          },
    {0x0C58U, "HCI_READ_INQUIRY_RESP_TX_POWER_LEVEL_OPCODE"             },
    {0x0C59U, "HCI_WRITE_INQUIRY_RESP_TX_POWER_LEVEL_OPCODE"            },
    {0x0C5FU, "HCI_ENHANCED_FLUSH_OPCODE"                               },
    {0x0C5AU, "HCI_READ_DEFAULT_ERR_DATA_REPORTING_OPCODE"              },
    {0x0C5BU, "HCI_WRITE_DEFAULT_ERR_DATA_REPORTING_OPCODE"             },
    {0x0C60U, "HCI_SEND_KEY_PRESS_NOTIFICATION_OPCODE"                  },
#endif /* BT_2_1_EDR */

#ifdef BT_3_0
#ifdef HCI_FLOW_CONTROL_COMMANDS
    {0x0C66U, "HCI_READ_FLOW_CONTROL_MODE_OPCODE"                       },
    {0x0C67U, "HCI_WRITE_FLOW_CONTROL_MODE_OPCODE"                      },
#endif /* HCI_FLOW_CONTROL_COMMANDS */

    {0x0C68U, "HCI_READ_ENHANCE_TRANSMIT_POWER_LEVEL_OPCODE"            },
#endif /* BT_3_0 */

#ifdef BT_BRSC
    {0x0C79U, "HCI_READ_SECURE_CONNECTIONS_HOST_SUPPORT_OPCODE"         },
    {0x0C7AU, "HCI_WRITE_SECURE_CONNECTIONS_HOST_SUPPORT_OPCODE"        },
#endif /* BT_BRSC */
    {0x0C7BU, "HCI_READ_AUTHENTICATED_PAYLOAD_TIMEOUT_OPCODE"           },
    {0x0C7CU, "HCI_WRITE_AUTHENTICATED_PAYLOAD_TIMEOUT_OPCODE"          },

    /* Informational Parameters - OGF : 0x04 */
    {0x1001U, "HCI_READ_LOCAL_VERSION_INFORMATION_OPCODE"               },
    {0x1002U, "HCI_READ_LOCAL_SUPPORTED_COMMANDS_OPCODE"                },
    {0x1003U, "HCI_READ_LOCAL_SUPPORTED_FEATURES_OPCODE"                },
    {0x1005U, "HCI_READ_BUFFER_SIZE_OPCODE"                             },
    {0x1007U, "HCI_READ_COUNTRY_CODE_OPCODE"                            },
    {0x1009U, "HCI_READ_BD_ADDR_OPCODE"                                 },

#ifdef BT_HCI_1_2
    {0x1002U, "HCI_READ_LOCAL_SUPPORTED_COMMANDS_OPCODE"                },
    {0x1004U, "HCI_READ_LOCAL_EXTENDED_FEATURES_OPCODE"                 },
#endif /* BT_HCI_1_2 */

    /* Status Parameters - OGF : 0x05 */
    {0x1401U, "HCI_READ_FAILED_CONTACT_COUNTER_OPCODE"                  },
    {0x1402U, "HCI_RESET_FAILED_CONTACT_COUNTER_OPCODE"                 },
    {0x1403U, "HCI_GET_LINK_QUALITY_OPCODE"                             },
    {0x1405U, "HCI_READ_RSSI_OPCODE"                                    },

#ifdef BT_HCI_1_2
    {0x1406U, "HCI_READ_AFH_CHANNEL_MAP_OPCODE"                         },
    {0x1407U, "HCI_READ_CLOCK_OPCODE"                                   },
#endif /* BT_HCI_1_2 */

#ifdef BT_3_0
#ifndef HCI_LITE
    {0x1408U, "HCI_READ_ENCRYPTION_KEY_SIZE_OPCODE"                     },
#endif /* HCI_LITE */
#endif /* BT_3_0 */

    /* Bluetooth Low Energy Commands */
#ifdef BT_4_0
    {0x2001U, "HCI_LE_SET_EVENT_MASK_OPCODE"                            },
    {0x2002U, "HCI_LE_READ_BUFFER_SIZE_OPCODE"                          },
    {0x2003U, "HCI_LE_READ_LOCAL_SUPPORTED_FEATURES_OPCODE"             },
    {0x2005U, "HCI_LE_SET_RANDOM_ADDRESS_OPCODE"                        },
    {0x2006U, "HCI_LE_SET_ADVERTISING_PARAMETERS_OPCODE"                },
    {0x2007U, "HCI_LE_READ_ADVERTISING_CHANNEL_TX_POWER_OPCODE"         },
    {0x2008U, "HCI_LE_SET_ADVERTISING_DATA_OPCODE"                      },
    {0x2009U, "HCI_LE_SET_SCAN_RESPONSE_DATA_OPCODE"                    },
    {0x200AU, "HCI_LE_SET_ADVERTISING_ENABLE_OPCODE"                    },
    {0x200BU, "HCI_LE_SET_SCAN_PARAMETERS_OPCODE"                       },
    {0x200CU, "HCI_LE_SET_SCAN_ENABLE_OPCODE"                           },
    {0x200DU, "HCI_LE_CREATE_CONNECTION_OPCODE"                         },
    {0x200EU, "HCI_LE_CREATE_CONNECTION_CANCEL_OPCODE"                  },
    {0x200FU, "HCI_LE_READ_WHITE_LIST_SIZE_OPCODE"                      },
    {0x2010U, "HCI_LE_CLEAR_WHITE_LIST_OPCODE"                          },
    {0x2011U, "HCI_LE_ADD_DEVICE_TO_WHITE_LIST_OPCODE"                  },
    {0x2012U, "HCI_LE_REMOVE_DEVICE_FROM_WHITE_LIST_OPCODE"             },
    {0x2013U, "HCI_LE_CONNECTION_UPDATE_OPCODE"                         },
    {0x2014U, "HCI_LE_SET_HOST_CHANNEL_CLASSIFICATION_OPCODE"           },
    {0x2015U, "HCI_LE_READ_CHANNEL_CLASSIFICATION_OPCODE"               },
    {0x2016U, "HCI_LE_READ_REMOTE_USED_FEATURES_OPCODE"                 },
    {0x2017U, "HCI_LE_ENCRYPT_OPCODE"                                   },
    {0x2018U, "HCI_LE_RAND_OPCODE"                                      },
    {0x2019U, "HCI_LE_START_ENCRYPTION_OPCODE"                          },
    {0x201AU, "HCI_LE_LONG_TERM_KEY_REQUESTED_REPLY_OPCODE"             },
    {0x201BU, "HCI_LE_LONG_TERM_KEY_REQUESTED_NEGATIVE_REPLY_OPCODE"    },
    {0x201CU, "HCI_LE_READ_SUPPORTED_STATES_OPCODE"                     },
    {0x201DU, "HCI_LE_RECEIVER_TEST_COMMAND_OPCODE"                     },
    {0x201EU, "HCI_LE_TRANSMITTER_TEST_COMMAND_OPCODE"                  },
    {0x201FU, "HCI_LE_TEST_END_OPCODE"                                  },
#endif /* BT_4_0 */

#ifdef HCI_ENH_SCO
    /* HCI opcode for enhanced SCO  */
    {0x100BU, "HCI_READ_LOCAL_SUPPORTED_CODECS_OPCODE"                 },
    {0x043DU, "HCI_SETUP_ENHANCED_SYNCHRONOUS_CONNECTION_OPCODE"       },
    {0x043EU, "HCI_ACCEPT_ENHANCED_SYNCHRONOUS_CONNECTION_OPCODE"      },
#endif /* HCI_ENH_SCO */

    /* Testing Commands - OGF : 0x06 */
    {0X1801U, "HCI_READ_LOOPBACK_MODE_OPCODE"                           },
    {0X1802U, "HCI_WRITE_LOOPBACK_MODE_OPCODE"                          },
    {0X1803U, "HCI_ENABLE_DEVICE_UNDER_TEST_MODE_OPCODE"                },

#ifdef BT_BRSC_TEST
    {0x180AU, "HCI_WRITE_SECURE_CONNECTIONS_TEST_MODE_OPCODE"           }
#endif /* BT_BRSC_TEST */
};


static const char hci_options[] = "\
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
    22. Read page timeout. \n\
    23. Read connection accept timeout. \n\
    24. Read Class of Device. \n\
    25. Read Local Supported Codecs. \n\
    26. Read Local Supported Features. \n\
    27. Read Local Version Information. \n\
    28. Read Remote Version and Supported Features. \n\
    29. Read inquiry mode. \n\
 \n\
    30. Role Discovery. \n\
    31. Switch Role. \n\
    32. Sniff Mode. \n\
    33. Park Mode. \n\
    34. Hold Mode. \n\
    35. Exit Sniff Mode. \n\
    36. Exit Park Mode. \n\
    37. Write Link Policy. \n\
    38. Sniff subrating. \n\
    39. QOS Setup. \n\
    40. Write Connection Accept Timeout.\n\
    41. Write Page Timeout.\n\
    42. Write Page Scan Activity.\n\
    43. Write Inquiry Scan Activity.\n\
    44. Write Class of Device.\n\
    45. Write Local Name.\n\
    46. Read Default Link Policy Settings.\n\
    47. Write Default Link Policy Settings.\n\
 \n\
    50. Write Inquiry Mode. \n\
    51. Write Inquiry Scan Type. \n\
    52. Write Page Scan Type. \n\
    53. Write Flow Control Mode. \n\
    54. Read current IAC LAP. \n\
    55. Write current IAC LAP. \n\
    56. Change ACL/SCO packet type. \n\
    57. Change eSCO packet type. \n\
    58. Cancel inquiry scan\n\
 \n\
    60. Read Voice Settings. \n\
    61. Read Enhanced Transmit Power Level. \n\
    62. Read Encryption Key Size. \n\
    63. Read Link Policy \n\
 \n\
    65. Read Secure Connections Host Support. \n\
    66. Write Secure Connections Host Support. \n\
    67. Write Secure Connections Test Mode. \n\
    68. Read HCI Buffer Size. \n\
 \n\
    70. Write Extended Inquiry Response. \n\
    71. Test Mode - RX Test. \n\
    72. Test Mode - TX Test. \n\
 \n\
    74. Vendor Specific commands.\n\
    75. HCI generic command. \n\
    76. Enable Test Mode. \n\
 \n\
    80. LE Operations. \n\
 \n\
   100. HCI Reset. \n\
 \n\
Your Option ?";

static const char appl_vendor_specific_menu[] = "\
================ Vendor Specific Commands MENU ============\n\
    0.  Exit \n\
    1.  Refresh \n\
    2.  Write BD Address. \n\
";

#ifdef BR_EDR_HCI
static UCHAR link_type;
#endif
/* ----------------------------------------- Functions */

void Write_BD_Address(void)
{
    UCHAR          param[8];

    BT_DEVICE_ADDR   bd_addr;

    param[0] = 0xfe;
    param[1] = 0x6;

    APPL_TRC(" Enter bd address:\n");

    appl_get_bd_addr(BT_BD_ADDR(&bd_addr));

    BT_mem_copy(&param[2],&bd_addr,6);

    BT_hci_vendor_specific_command(0x22U, (UCHAR *)param, sizeof(param));
}

void appl_vendor_specific_commands (void)
{
    int choice, menu_choice;

    CONSOLE_OUT ("%s", appl_vendor_specific_menu);

    BT_LOOP_FOREVER()
    {
        CONSOLE_OUT ("\nEnter your choice : ");
        CONSOLE_IN ("%d", &choice);
        menu_choice = choice;

        switch(choice)
        {
        case 0:
            break; /* return; */

        case 1:
            CONSOLE_OUT ("%s", appl_vendor_specific_commands);
            break;

        /* write BD address */
        case 2:
             Write_BD_Address();
             break;
        default:
            CONSOLE_OUT ("Invalid choice. try again..\n");
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

API_RESULT appl_hci_event_indication_callback
           (
               UCHAR  event_type,
               UCHAR  * event_data,
               UCHAR  event_datalen
           )
{
    UINT32 value_4;
    UINT16 connection_handle, value_2;
    UCHAR  status, value_1;

#ifdef BR_EDR_HCI
    UCHAR * bd_addr;
    UCHAR num_responses;
    UINT32 count, value_3;
#endif /* BR_EDR_HCI */

    UCHAR * revent_data;
    UCHAR revent_datalen;
    API_RESULT  retval;

    /* Store event data and datalen */
    revent_data = event_data;
    revent_datalen = event_datalen;

    /* Init */
    retval = API_SUCCESS;
    value_2 = 0U;

    /* Switch on the Event Code */
    switch (event_type)
    {
#ifdef BR_EDR_HCI
    case HCI_INQUIRY_COMPLETE_EVENT:
        printf("Received HCI_INQUIRY_COMPLETE_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        printf("\tStatus: 0x%02X\n", status);

        break;

    case HCI_INQUIRY_RESULT_EVENT:
        printf("Received HCI_INQUIRY_RESULT_EVENT.\n");

        /* Number of Responses */
        hci_unpack_1_byte_param(&num_responses, event_data);
        printf("\tNum Response: %d (0x%02X)\n", num_responses, num_responses);
        event_data += 1U;

        /* For each Response, Print the Inquiry Result */
        for (count = 0U; count < num_responses; count ++)
        {
            /* BD_ADDR of the Remote Device */
            printf("\tBD_ADDR: ");
            appl_dump_bytes_no_limit_logs(event_data, 6U);
            event_data += 6U;

            /* Page Scan Repetition Mode */
            hci_unpack_1_byte_param(&value_1, event_data);
            printf("\tPage Scan Repetition Mode: 0x%02X\n", value_1);
            event_data += 1U;

            /* Reserved 2 octets */
            event_data += 2U;

            /* Class of Device */
            hci_unpack_3_byte_param(&value_3, event_data)
            printf("\tClass of Device: 0x%06X\n", (unsigned int)value_3);
            event_data += 3U;

            /* Clock Offset */
            hci_unpack_2_byte_param(&value_2, event_data);
            printf("\tClock Offset: 0x%04X\n", value_2);
            event_data += 2U;
        }

        break;

    case HCI_SNIFF_SUBRATING_EVENT:
        printf("Received HCI_SNIFF_SUBRATING_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        printf("\tStatus: 0x%02X\n", status);
        event_data += 1U;

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        printf("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2U;

        /* Max Tx Latency */
        hci_unpack_2_byte_param(&value_2, event_data);
        printf("\tMax Tx Latency: 0x%04X\n", value_2);
        event_data += 2U;

        /* Max Rx Latency */
        hci_unpack_2_byte_param(&value_2, event_data);
        printf("\tMax Rx Latency: 0x%04X\n", value_2);
        event_data += 2U;

        /* Min Remote Timeout */
        hci_unpack_2_byte_param(&value_2, event_data);
        printf("\tMin Remote Timeout: 0x%04X\n", value_2);
        event_data += 2U;

        /* Min Local Timeout */
        hci_unpack_2_byte_param(&value_2, event_data);
        printf("\tMin Local Timeout: 0x%04X\n", value_2);
        event_data += 2U;

        break;

#ifdef BT_EIR
    case HCI_EXTENDED_INQUIRY_RESULT_EVENT:
        printf("Received HCI_EXTENDED_INQUIRY_RESULT_EVENT.\n");

        /* Number of Responses */
        hci_unpack_1_byte_param(&num_responses, event_data);
        printf("\tNum Response: %d (0x%02X)\n", num_responses, num_responses);
        event_data += 1U;

        /* For each Response, Print the Inquiry Result */
        for (count = 0U; count < num_responses; count ++)
        {
            /* BD_ADDR of the Remote Device */
            printf("\tBD_ADDR: ");
            appl_hci_print_bd_addr(event_data);
            event_data += 6U;

            /* Page Scan Repetition Mode */
            hci_unpack_1_byte_param(&value_1, event_data);
            printf("\tPage Scan Repetition Mode: 0x%02X\n", value_1);
            event_data += 1U;

            /* Reserved 1 octet */
            event_data += 1U;

            /* Class of Device */
            hci_unpack_3_byte_param(&value_3, event_data)
            printf("\tClass of Device: 0x%06X\n", (unsigned int)value_3);
            event_data += 3U;

            /* Clock Offset */
            hci_unpack_2_byte_param(&value_2, event_data);
            printf("\tClock Offset: 0x%04X\n", value_2);
            event_data += 2U;

            /* RSSI */
            hci_unpack_1_byte_param(&value_1, event_data);
            printf("\tRSSI: 0x%02X -> %d dBm\n", value_1, (char) value_1);
            event_data += 1U;

            appl_print_extended_inquiry_response
            (
                event_data,
                (UINT16)(event_datalen - 15U)
            );
        }

        break;
#endif /* BT_EIR */

    case HCI_CONNECTION_COMPLETE_EVENT:
        printf("Received HCI_CONNECTION_COMPLETE_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        printf("\tStatus: 0x%02X\n", status);
        event_data += 1U;

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        printf("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2U;

        /* BD_ADDR */
        printf("\tBD_ADDR: ");
        bd_addr = event_data;
        appl_hci_print_bd_addr(bd_addr);
        event_data += 6U;

        /* Update the global address */
        BT_COPY_BD_ADDR(g_bd_addr.addr, bd_addr);
        g_bd_addr.type = 0x00U;

        /* Link Type */
        hci_unpack_1_byte_param(&link_type, event_data);
        printf("\tLink Type: 0x%02X", link_type);
        switch (link_type)
        {
        case HCI_SCO_LINK:
            printf(" -> SCO\n");
            break;
        case HCI_ACL_LINK:
            printf(" -> ACL\n");
            break;
    #ifdef BT_HCI_1_2
        case HCI_ESCO_LINK:
            printf(" -> eSCO\n");
            break;
    #endif /* BT_HCI_1_2 */
        default:
            printf(" -> ???\n");
            break;
        }
        event_data += 1U;

        /* Encryption Mode */
        hci_unpack_1_byte_param(&value_1, event_data);
        printf("\tEncryption Mode: 0x%02X", value_1);
        switch (value_1)
        {
        case 0x00U:
            printf(" -> Encryption OFF\n");
            break;
        case 0x01U:
            printf(" -> Encryption ON\n");
            break;
        default:
            printf(" -> ???\n");
            break;
        }
        event_data += 1U;

        /* Setup HCI-USB for SCO Data Reception */
        if ((0x0U == status) &&
            ((HCI_SCO_LINK == link_type) ||
             (HCI_ESCO_LINK == link_type)))
        {
            printf("SCO Connection Complete.\n");

            /**
             * Configure for eSCO Audio codec
             */
            appl_hci_esco_audio_config (LMP_VOICE_AIR_CODING_CVSD);

            /**
             * Start Voice Path
             */
            (BT_IGNORE_RETURN_VALUE) sco_audio_start_pl ();
            printf ("Started for voice recording \n");

            /* Save SCO/eSCO Connection Handle */
            sco_connection_handle = connection_handle;
        }

        break;

    case HCI_CONNECTION_REQUEST_EVENT:
        printf("Received (** Unexpected **) HCI_CONNECTION_REQUEST_EVENT.\n");

        /* BD_ADDR */
        printf("\tBD_ADDR: ");
        appl_hci_print_bd_addr(event_data);
        event_data += 6U;

        /* Class of Device */
        hci_unpack_3_byte_param(&value_3, event_data);
        printf("\tClass of Device: 0x%06X\n", (unsigned int)value_3);
        event_data += 3U;

        /* Link Type */
        hci_unpack_1_byte_param(&value_1, event_data);
        printf("\tLink Type: 0x%02X", value_1);
        switch (value_1)
        {
        case HCI_SCO_LINK:
            printf(" -> SCO\n");
            break;
        case HCI_ACL_LINK:
            printf(" -> ACL\n");
            break;
    #ifdef BT_HCI_1_2
        case HCI_ESCO_LINK:
            printf(" -> eSCO\n");

#ifdef BR_EDR_HCI
#ifndef HCI_NO_ESCO_AUTO_ACCEPT
            if (0U == appl_hci_sync_conn_accept_auto_rsp)
            {
                if (BT_TRUE == appl_hci_set_esco_connect_param)
                {
                    (BT_IGNORE_RETURN_VALUE) BT_hci_set_esco_channel_parameters (&appl_esco_params);
                }
            }
            else if (1U == appl_hci_sync_conn_accept_auto_rsp)
            {
#ifdef HCI_ENH_SCO
                /*
                 * The parameters will get updated with the stack, but will be
                 * used only when HCI_RETURN_ENHANCED_SYNCHRONOUS_ACCEPT is
                 * returned from the callback after this setting
                 */
                (BT_IGNORE_RETURN_VALUE) BT_hci_set_enh_sco_channel_parameters (&appl_enh_sco_params);

                value_2 = HCI_ESCO_REQ_RETURN_ENH_SYNCHRONOUS_ACCEPT; /* return HCI_ESCO_REQ_RETURN_ENH_SYNCHRONOUS_ACCEPT; */
                retval = API_FAILURE;
#else /* HCI_ENH_SCO */
                printf ("HCI_ENH_SCO is not defined!\n");
#endif /* HCI_ENH_SCO */
            }
            else if (2U == appl_hci_sync_conn_accept_auto_rsp)
            {
                /* Return no auto accept */
                value_2 = HCI_ESCO_REQ_RETURN_NO_AUTO_RESPONSE; /* return HCI_ESCO_REQ_RETURN_NO_AUTO_RESPONSE; */
                retval = API_FAILURE;
            }
            else if (3U == appl_hci_sync_conn_accept_auto_rsp)
            {
                /* Return Reject reason */
                value_2 = appl_hci_sync_conn_reject_reason; /*  return appl_hci_sync_conn_reject_reason; */
                retval = API_FAILURE;
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }
#endif /* HCI_NO_ESCO_AUTO_ACCEPT */
#endif /* BR_EDR_HCI */

            break;
#endif /* BT_HCI_1_2 */

        default:
            printf(" -> ???\n");
            break;
        }
        event_data += 1U;

        break;
#endif /* BR_EDR_HCI */

#ifdef BR_EDR_HCI
    case HCI_DISCONNECTION_COMPLETE_EVENT:
        printf("Received HCI_DISCONNECTION_COMPLETE_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        printf("\tStatus: 0x%02X\n", status);
        event_data += 1U;

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        printf("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2U;

        /* Reason */
        hci_unpack_1_byte_param(&value_1, event_data);
        printf("\tReason: 0x%02X\n", value_1);
        event_data += 1U;

        if ( (connection_handle == sco_connection_handle) && ((link_type == HCI_SCO_LINK) || (link_type == HCI_ESCO_LINK)) )
        {
            /**
             * Stop Voice Path
             */
            (BT_IGNORE_RETURN_VALUE) sco_audio_stop_pl ();
            printf ("Voice stoped\n");

            /* Reset SCO connection handle */
            sco_connection_handle = HCI_INVALID_CONNECTION_HANDLE;
        }

        break;
#endif /* BR_EDR_HCI */
        
#ifdef BR_EDR_HCI
    case HCI_AUTHENTICATION_COMPLETE_EVENT:
        printf("Received HCI_AUTHENTICATION_COMPLETE_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        printf("\tStatus: 0x%02X\n", status);
        event_data += 1U;

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        printf("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2U;

        if (HC_KEY_MISSING == status)
        {
            printf("\n");

            printf("___________________________________________________________\n");
            printf("The peer device seems to have lost the bonding information.\n");
            printf("Kindly delete the bonding information of the peer from the\n");
            printf("Security Manager menu, option 18, and try again.\n\n");
            printf("NOTE:\n");
            printf("Ensure to disconnect the HCI link connection with the peer\n");
            printf("device before attempting to delete the bonding information.\n");
            printf("___________________________________________________________\n");

            printf("\n");
        }

        break;

    case HCI_REMOTE_NAME_REQUEST_COMPLETE_EVENT:
        printf("Received HCI_REMOTE_NAME_REQUEST_COMPLETE_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        printf("\tStatus: 0x%02X\n", status);
        event_data += 1U;

        /* BD_ADDR of Remote Device */
        printf("\tBD_ADDR: ");
        appl_hci_print_bd_addr(event_data);
        event_data += 6U;

        /* Remote Name */
        printf("\tRemote Name: ");
        if (7U < event_datalen)
        {
            for (count = 7U; count < event_datalen; count ++)
            {
                if ('\0' == (*event_data))
                {
                    break;
                }

                printf("%c", *event_data);
                event_data += 1U;
            }
        }
        printf(".\n");

        break;

    case HCI_ENCRYPTION_CHANGE_EVENT:
        printf("Received HCI_ENCRYPTION_CHANGE_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        printf("\tStatus: 0x%02X\n", status);
        switch (status)
        {
        case 0x00U:
            printf(" -> HCI_Encryption Status Succeeded\n");
            break;
        default:
            printf(" -> HCI_Encryption Status Failed\n");
            break;
        }
        event_data += 1U;

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        printf("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2U;

        /* Encryption Enable */
        hci_unpack_1_byte_param(&value_1, event_data);
        printf("\tEncryption Enable: 0x%02X", value_1);
        switch (value_1)
        {
        case 0x00U:
            printf(" -> Encryption OFF\n");
            break;
        case 0x01U:
        case 0x02U:
            printf(" -> Encryption ON\n");
            break;
        default:
            printf(" -> ???\n");
            break;
        }
        event_data += 1U;

        break;

    case HCI_CHANGE_CONNECTION_LINK_KEY_COMPLETE_EVENT:
        printf("Received HCI_CHANGE_CONNECTION_LINK_KEY_COMPLETE_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        printf("\tStatus: 0x%02X\n", status);
        event_data += 1U;

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        printf("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2U;

        break;

    case HCI_MASTER_LINK_KEY_COMPLETE_EVENT:
        printf("Received HCI_MASTER_LINK_KEY_COMPLETE_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        printf("\tStatus: 0x%02X\n", status);
        event_data += 1U;

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        printf("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2U;

        /* Key Flag */
        hci_unpack_1_byte_param(&value_1, event_data);
        printf("\tKey Flag: 0x%02X", value_1);
        switch (value_1)
        {
        case 0x00U:
            printf(" -> Using Semi-parmanent Link Key\n");
            break;
        case 0x01U:
            printf(" -> Using Temporary Link Key\n");
            break;
        default:
            printf(" -> ???\n");
            break;
        }
        event_data += 1U;

        break;
#endif /* BR_EDR_HCI */

    case HCI_READ_REMOTE_SUPPORTED_FEATURES_COMPLETE_EVENT:
        printf("Received HCI_READ_REMOTE_SUPPORTED_FEATURES_COMPLETE_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        printf("\tStatus: 0x%02X\n", status);
        event_data += 1U;

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        printf("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2U;

        /* LMP Features */
        printf("\tLMP Features: %02X %02X %02X %02X %02X %02X %02X %02X\n",
        event_data[0U], event_data[1U], event_data[2U], event_data[3U],
        event_data[4U], event_data[5U], event_data[6U], event_data[7U]);
        event_data += 8U;

        break;

    case HCI_READ_REMOTE_VERSION_INFORMATION_COMPLETE_EVENT:
        printf("Received HCI_READ_REMOTE_VERSION_INFORMATION_COMPLETE_EVENT\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        printf("\tStatus: 0x%02X\n", status);
        event_data += 1U;

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        printf("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2U;

        /* LMP Version */
        hci_unpack_1_byte_param(&value_1, event_data);
        printf("\tLMP Version: 0x%02X\n", value_1);
        event_data += 1U;

        /* Manufacturer Name */
        hci_unpack_2_byte_param(&value_2, event_data);
        printf("\tManufacturer Name (Company ID): 0x%04X\n", value_2);
        event_data += 2U;

        /* LMP Subversion */
        hci_unpack_2_byte_param(&value_2, event_data);
        printf("\tLMP Subversion: 0x%04X\n", value_2);
        event_data += 2U;

        break;

    case HCI_QOS_SETUP_COMPLETE_EVENT:
        printf("Received HCI_QOS_SETUP_COMPLETE_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        printf("\tStatus: 0x%02X\n", status);
        event_data += 1U;

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        printf("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2U;

        /* Flags */
        hci_unpack_1_byte_param(&value_1, event_data);
        printf("\tFlags: 0x%02X\n", value_1);
        event_data += 1U;

        /* Service Type */
        hci_unpack_1_byte_param(&value_1, event_data);
        printf("\tService Type: 0x%02X", value_1);
        switch (value_1)
        {
        case 0x00U:
            printf(" -> No Traffic Available\n");
            break;
        case 0x01U:
            printf(" -> Best Effort Available\n");
            break;
        case 0x02U:
            printf(" -> Guaranteed Available\n");
            break;
        default:
            printf(" -> ???\n");
            break;
        }
        event_data += 1U;

        /* Token Rate */
        hci_unpack_4_byte_param(&value_4, event_data);
        printf("\tToken Rate: 0x%08X -> %d octets/sec\n",
        (unsigned int) value_4, (unsigned int) value_4);
        event_data += 4U;

        /* Peak Bandwidth */
        hci_unpack_4_byte_param(&value_4, event_data);
        printf("\tPeak Bandwidth 0x%08X -> %d octets/sec\n",
        (unsigned int) value_4, (unsigned int) value_4);
        event_data += 4U;

        /* Latency */
        hci_unpack_4_byte_param(&value_4, event_data);
        printf("\tLatency 0x%08X -> %d microseconds\n",
        (unsigned int) value_4, (unsigned int) value_4);
        event_data += 4U;

        /* Delay Variation */
        hci_unpack_4_byte_param(&value_4, event_data);
        printf("\tDelay Variation 0x%08X -> %d microseconds\n",
        (unsigned int) value_4, (unsigned int) value_4);
        event_data += 4U;

        break;

#ifdef APPL_LIMIT_LOGS
#ifdef printf
#undef printf
#define printf(...)
#endif /* printf */
#endif /* APPL_LIMIT_LOGS */

    case HCI_COMMAND_COMPLETE_EVENT:
        printf("Received HCI_COMMAND_COMPLETE_EVENT.\n");

        /* Number of Command Packets */
        hci_unpack_1_byte_param(&value_1, event_data);
        printf("\tNum Command Packets: %d (0x%02X)\n", value_1, value_1);
        event_data += 1U;

        /* Command Opcode */
        hci_unpack_2_byte_param(&value_2, event_data);
        printf("\tCommand Opcode: 0x%04X -> %s\n",
        value_2, appl_hci_get_command_name(value_2));
        event_data += 2U;

        /* Command Status */
        hci_unpack_1_byte_param(&status, event_data);
        printf("\tCommand Status: 0x%02X\n", status);
        event_data += 1U;

        /* Command Return Parameters */
        if (4U < event_datalen)
        {
            switch (value_2)
            {
            #ifdef BT_EIR
                case HCI_READ_EXTENDED_INQUIRY_RESPONSE_OPCODE:
                {
                    printf("\tRead EIR Response Status: 0x%02X", status);

                    printf("\tFEC_Required: 0x%02X", *event_data);
                    event_data ++;

                    appl_print_extended_inquiry_response
                    (
                        event_data,
                        (UINT16)(event_datalen - 4U)
                    );

                    printf("\n");
                    break;
                }

                case HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_OPCODE:
                {
                    printf("\tWrite EIR Response Status: 0x%02X\n", status);
                    break;
                }
            #endif /* BT_EIR */

            #ifdef BT_3_0
                case HCI_READ_ENCRYPTION_KEY_SIZE_OPCODE:
                {
                    printf("\tRead Encryption Key Size Status: 0x%02X\n", status);

                    hci_unpack_2_byte_param(&connection_handle, event_data);
                    printf("\tConnection Handle: 0x%04X\n", connection_handle);

                    event_data += 2U;
                    printf("\tKey Size: 0x%02X\n", *event_data);
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

                    printf("\tRead Local Supported Codecs Status: 0x%02X\n", status);

                    /* Get no. of supported codecs */
                    num_codecs = *event_data;
                    event_data ++;

                    printf ("Supported Codecs: 0x%02X\n", num_codecs);

                    /* Print supported codecs */
                    printf ("\tSupported Codec IDs:");
                    for( i = 0U; i < num_codecs; i++)
                    {
                        printf ("\t\tCodec %02X:", *event_data);
                        event_data ++;
                    }

                    /* Get no. of supported vendor specific codecs */
                    num_vendor_specific_codecs = *event_data;
                    event_data ++;

                    printf ("Supported Vendor Specific Codecs: 0x%02X\n",
                            num_vendor_specific_codecs);

                    /* Print supported vendor specific codecs */
                    printf ("\tSupported Vendor Specific Codec IDs:");
                    for( i = 0U; i < num_vendor_specific_codecs; i++)
                    {
                        hci_unpack_2_byte_param(&company_id, event_data);
                        event_data += 2U;
                        hci_unpack_2_byte_param(&vendor_codec_id, event_data);
                        event_data += 2U;

                        printf ("\t\tCompany ID 0x%04X: Vendor Codec ID 0x%04X\n",
                        company_id, vendor_codec_id);
                    }

                    break;
                }
#endif /* HCI_ENH_SCO */

                case HCI_READ_LOCAL_VERSION_INFORMATION_OPCODE:
                {
                    printf("\tRead Local Version Information Status: 0x%02X\n", status);

                    hci_unpack_1_byte_param(&value_1, event_data);
                    printf("\tHCI_Version: 0x%02X\n", value_1);
                    event_data += 1U;

                    hci_unpack_2_byte_param(&value_2, event_data);
                    printf("\tHCI_Revision: 0x%04X\n", value_2);
                    event_data += 2U;

                    hci_unpack_1_byte_param(&value_1, event_data);
                    printf("\tLMP_PAL_Version: 0x%02X\n", value_1);
                    event_data += 1U;

                    hci_unpack_2_byte_param(&value_2, event_data);
                    printf("\tManufacturer_Name: 0x%04X\n", value_2);
                    event_data += 2U;

                    hci_unpack_2_byte_param(&value_2, event_data);
                    printf("\tLMP_PAL_Subverion: 0x%04X\n", value_2);
                    event_data += 2U;

                    break;
                }
                case HCI_READ_LOCAL_NAME_OPCODE:
                {
                    UCHAR c, i;
                    for(i = 0U; i < (event_datalen - 4U); i ++)
                    {
                        c =  event_data[i];
                        /* UTF-8 chars range is 0x20U to 0x7EU.*/
                        if ( (c < 0x20U) || (c > 0x7EU) )
                        {
                            break;
                        }
                    }
                    printf("\tReturn Parameters: ");
                    appl_dump_bytes(event_data, i);
                    printf("\n");
                    break;
                }

                default:
                {
                    printf("\tReturn Parameters: ");
                    appl_dump_bytes(event_data, event_datalen - 4U);
                    printf("\n");
                    break;
                }
            }
        }

    #ifdef BT_VENDOR_SPECIFIC_INIT
        app_vendor_specific_init(NULL);
    #endif /* BT_VENDOR_SPECIFIC_INIT */

        break;

#ifdef APPL_LIMIT_LOGS
#ifdef printf
#undef printf
#define printf PRINTF
#endif /* APPL_TRC */
#endif /* APPL_LIMIT_LOGS */

    case HCI_COMMAND_STATUS_EVENT:
#ifndef APPL_LIMIT_LOGS
        printf("Received HCI_COMMAND_STATUS_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        printf("\tCommand Status: 0x%02X\n", status);
        event_data += 1U;

        /* Number of Command Packets */
        hci_unpack_1_byte_param(&value_1, event_data);
        printf("\tNum Command Packets: %d (0x%02X)\n", value_1, value_1);
        event_data += 1U;

        /* Command Opcode */
        hci_unpack_2_byte_param(&value_2, event_data);
        printf("\tCommand Opcode: 0x%04X (%s)\n",
        value_2, appl_hci_get_command_name(value_2));
        event_data += 2U;
#endif
        break;

    case HCI_HARDWARE_ERROR_EVENT:
        printf("Received HCI_HARDWARE_ERROR_EVENT.\n");

        /* Hardware Code */
        hci_unpack_1_byte_param(&value_1, event_data);
        printf("\tHardware Code: 0x%02X\n", value_1);
        event_data += 1U;

        break;

    case HCI_FLUSH_OCCURRED_EVENT:
#ifndef APPL_LIMIT_LOGS
        printf("Received HCI_FLUSH_OCCURRED_EVENT.\n");
#endif
        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
#ifndef APPL_LIMIT_LOGS
        printf("\tConnection Handle: 0x%04X\n", connection_handle);
#endif
        event_data += 2U;

        break;

    case HCI_NUMBER_OF_COMPLETED_PACKETS_EVENT:
        /* printf("Received HCI_NUMBER_OF_COMPLETED_PACKETS_EVENT.\n"); */
        break;

#ifdef BR_EDR_HCI
    case HCI_ROLE_CHANGE_EVENT:
        printf("Received HCI_ROLE_CHANGE_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        printf("\tStatus: 0x%02X\n", status);
        event_data += 1U;

        /* BD_ADDR of Remote Device */
        printf("\tBD_ADDR: ");
        appl_hci_print_bd_addr(event_data);
        event_data += 6U;

        /* New Role */
        hci_unpack_1_byte_param(&value_1, event_data);
        printf("\tNew Role: 0x%02X", value_1);
        switch (value_1)
        {
        case 0x00U:
            printf(" -> Master\n");
            break;
        case 0x01U:
            printf(" -> Slave\n");
            break;
        default:
            printf(" -> ???\n");
            break;
        }
        event_data += 1U;

        break;

    case HCI_MODE_CHANGE_EVENT:
        printf("Received HCI_MODE_CHANGE_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        printf("\tStatus: 0x%02X\n", status);
        event_data += 1U;

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        printf("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2U;

        /* Current Mode */
        hci_unpack_1_byte_param(&value_1, event_data);
        printf("\tCurrent Mode: 0x%02X", value_1);
        switch (value_1)
        {
        case 0x00U:
            printf(" -> Active Mode\n");
            break;
        case 0x01U:
            printf(" -> Hold Mode\n");
            break;
        case 0x02U:
            printf(" -> Sniff Mode\n");
            break;
        case 0x03U:
            printf(" -> Park Mode\n");
            break;
        default:
            printf(" -> ???\n");
            break;
        }
        event_data += 1U;

        /* Interval */
        hci_unpack_2_byte_param(&value_2, event_data);
        printf("\tInterval: 0x%04X\n", value_2);
        event_data += 2U;

        break;

    case HCI_RETURN_LINK_KEYS_EVENT:
        printf("Received HCI_RETURN_LINK_KEYS_EVENT.\n");

        /* Number of Keys */
        hci_unpack_1_byte_param(&value_1, event_data);
        printf("\tNumber of Keys: %d (0x%02X)\n", value_1, value_1);
        event_data += 1U;

        while (value_1 > 0U)
        {
            /* BD_ADDR */
            printf("\tBD_ADDR: ");
            appl_hci_print_bd_addr(event_data);
            event_data += 6U;

            /* Link Key */
            printf("\tLink Key: ");
            appl_dump_bytes(event_data, 16U);
            event_data += 16U;

            value_1 --;
        }

        break;

    case HCI_PIN_CODE_REQUEST_EVENT:
        printf("Received (** Unexpected **) HCI_PIN_CODE_REQUEST_EVENT.\n");
        break;

    case HCI_LINK_KEY_REQUEST_EVENT:
        printf("Received (** Unexpected **) HCI_LINK_KEY_REQUEST_EVENT.\n");
        break;

    case HCI_LINK_KEY_NOTIFICATION_EVENT:
        printf("Received HCI_LINK_KEY_NOTIFICATION_EVENT.\n");

        /* BD_ADDR */
        printf("\tBD_ADDR: ");
        appl_dump_bytes_no_limit_logs(event_data, 6U);
        bd_addr = event_data;
        event_data += 6U;

        /* Link Key */
        printf("\tLink Key: ");
        appl_dump_bytes_no_limit_logs(event_data, 16U);
        event_data += 16U;

        /* Key Type */
        hci_unpack_1_byte_param(&value_1, event_data);
        printf("\tKey Type: 0x%02X", value_1);
        switch (value_1)
        {
        case 0x00U:
            printf(" -> Combination Key\n");
            break;
        case 0x03U:
            printf(" -> Debug Combination Key\n");
            break;
        case 0x04U:
            printf(" -> Unauthenticated Combination Key (P-192)\n");
            break;
        case 0x05U:
            printf(" -> Authenticated Combination Key (P-192)\n");
            break;
        case 0x06U:
            printf(" -> Changed Combination Key\n");
            break;
        case 0x07U:
            printf(" -> Unauthenticated Combination Key (P-256)\n");
            break;
        case 0x08U:
            printf(" -> Authenticated Combination Key (P-256)\n");
            break;
        default:
            printf(" -> ???\n");
            break;
        }
        event_data += 1U;

        break;
#endif /* BR_EDR_HCI */

    case HCI_LOOPBACK_COMMAND_EVENT:
        printf("Received HCI_LOOPBACK_COMMAND_EVENT.\n");

        /* HCI Command Packet */
        printf("\tHCI Command Packet: ");
        appl_dump_bytes(event_data, event_datalen);

        break;

    case HCI_DATA_BUFFER_OVERFLOW_EVENT:
        printf("Received HCI_DATA_BUFFER_OVERFLOW_EVENT.\n");

        /* Link Type */
        hci_unpack_1_byte_param(&value_1, event_data);
        printf("\tLink Type: 0x%02X", value_1);
        switch (value_1)
        {
        case 0x00U:
            printf(" -> Synchronous Buffer Overflow\n");
            break;
        case 0x01U:
            printf(" -> ACL Buffer Overflow\n");
            break;
        default:
            printf(" -> ???\n");
            break;
        }
        event_data += 1U;

        break;

    case HCI_MAX_SLOTS_CHANGE_EVENT:
        printf("Received HCI_MAX_SLOTS_CHANGE_EVENT.\n");

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        printf("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2U;

        /* LMP Max Slots */
        hci_unpack_1_byte_param(&value_1, event_data);
        printf("\tLMP Max Slots: 0x%02X\n", value_1);
        event_data += 1U;

        break;

    case HCI_READ_CLOCK_OFFSET_COMPLETE_EVENT:
        printf("Received HCI_READ_CLOCK_OFFSET_COMPLETE_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        printf("\tStatus: 0x%02X\n", status);
        event_data += 1U;

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        printf("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2U;

        /* Clock Offset */
        hci_unpack_2_byte_param(&value_2, event_data);
        printf("\tClock Offset: 0x%04X\n", value_2);
        event_data += 2U;

        break;

    case HCI_ENCRYPTION_KEY_REFRESH_COMPLETE_EVENT:
        printf ("Received HCI_ENCRYPTION_KEY_REFRESH_COMPLETE_EVENT\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        printf("Status: 0x%02X\n", status);
        event_data ++;

        /* Connection Handle */
        hci_unpack_2_byte_param(&value_2, event_data);
        printf("\tConnection Handle: 0x%04X\n", value_2);

        break;

#ifdef BR_EDR_HCI
    case HCI_CONNECTION_PACKET_TYPE_CHANGED_EVENT:
        printf("Received HCI_CONNECTION_PACKET_TYPE_CHANGED_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        printf("\tStatus: 0x%02X\n", status);
        event_data += 1U;

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        printf("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2U;

        /* Packet Type */
        hci_unpack_2_byte_param(&value_2, event_data);
        printf("\tPacket Type: 0x%02X\n", value_2);
        event_data += 2U;

        break;

    case HCI_QOS_VIOLATION_EVENT:
        printf("Received HCI_QOS_VIOLATION_EVENT.\n");

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        printf("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2U;

        break;

    case HCI_PAGE_SCAN_REPETITION_MODE_CHANGE_EVENT:
        printf("Received HCI_PAGE_SCAN_REPETITION_MODE_CHANGE_EVENT.\n");

        /* BD_ADDR */
        printf("\tBD_ADDR: ");
        appl_hci_print_bd_addr(event_data);
        event_data += 6U;

        /* Page Scan Repetition Mode */
        hci_unpack_1_byte_param(&value_1, event_data);
        printf("\tPage Scan Repetition Mode: 0x%02X\n", value_1);
        event_data += 1U;

        break;

#ifdef BT_HCI_1_2
    case HCI_FLOW_SPECIFICATION_COMPLETE_EVENT:
        printf("Received HCI_FLOW_SPECIFICATION_COMPLETE_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        printf("\tStatus: 0x%02X\n", status);
        event_data += 1U;

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        printf("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2U;

        /* Flags */
        hci_unpack_1_byte_param(&value_1, event_data);
        printf("\tFlags: 0x%02X\n", value_1);
        event_data += 1U;

        /* Flow Direction */
        hci_unpack_1_byte_param(&value_1, event_data);
        printf("\tFlow Direction: 0x%02X", value_1);
        switch (value_1)
        {
        case 0x00U:
            printf(" -> Outgoing Flow\n");
            break;
        case 0x01U:
            printf(" -> Incoming Flow\n");
            break;
        default:
            printf(" -> ???\n");
            break;
        }
        event_data += 1U;

        /* Service Type */
        hci_unpack_1_byte_param(&value_1, event_data);
        printf("\tService Type: 0x%02X", value_1);
        switch (value_1)
        {
        case 0x00U:
            printf(" -> No Traffic\n");
            break;
        case 0x01U:
            printf(" -> Best Effort\n");
            break;
        case 0x02U:
            printf(" -> Guaranteed\n");
            break;
        default:
            printf(" -> ???\n");
            break;
        }
        event_data += 1U;

        /* Token Rate */
        hci_unpack_4_byte_param(&value_4, event_data);
        printf("\tToken Rate: 0x%08X -> %d octets/sec\n",
        (unsigned int) value_4, (unsigned int) value_4);
        event_data += 4U;

        /* Token Bucket Size */
        hci_unpack_4_byte_param(&value_4, event_data);
        printf("\tToken Bucket Size: 0x%08X -> %d octets\n",
        (unsigned int) value_4, (unsigned int) value_4);
        event_data += 4U;

        /* Peak Bandwidth */
        hci_unpack_4_byte_param(&value_4, event_data);
        printf("\tPeak Bandwidth 0x%08X -> %d octets/sec\n",
        (unsigned int) value_4, (unsigned int) value_4);
        event_data += 4U;

        /* Access Latency */
        hci_unpack_4_byte_param(&value_4, event_data);
        printf("\tAccess Latency 0x%08X -> %d microseconds\n",
        (unsigned int) value_4, (unsigned int) value_4);
        event_data += 4U;

        break;

    case HCI_INQUIRY_RESULT_WITH_RSSI_EVENT:
        printf("Received HCI_INQUIRY_RESULT_WITH_RSSI_EVENT.\n");

        /* Number of Responses */
        hci_unpack_1_byte_param(&num_responses, event_data);
        printf("\tNum Response: %d (0x%02X)\n", num_responses, num_responses);
        event_data += 1U;

        /* For each Response, Print the Inquiry Result */
        for (count = 0U; count < num_responses; count ++)
        {
            /* BD_ADDR of the Remote Device */
            printf("\tBD_ADDR: ");
            appl_hci_print_bd_addr(event_data);
            event_data += 6U;

            /* Page Scan Repetition Mode */
            hci_unpack_1_byte_param(&value_1, event_data);
            printf("\tPage Scan Repetition Mode: 0x%02X\n", value_1);
            event_data += 1U;

            /* Reserved 1 octet */
            event_data += 1U;

            /* Class of Device */
            hci_unpack_3_byte_param(&value_3, event_data)
            printf("\tClass of Device: 0x%06X\n", (unsigned int)value_3);
            event_data += 3U;

            /* Clock Offset */
            hci_unpack_2_byte_param(&value_2, event_data);
            printf("\tClock Offset: 0x%04X\n", value_2);
            event_data += 2U;

            /* RSSI */
            hci_unpack_1_byte_param(&value_1, event_data);
            printf("\tRSSI: 0x%02X -> %d dBm\n", value_1, (char) value_1);
            event_data += 1U;
        }

        break;

    case HCI_READ_REMOTE_EXTENDED_FEATURES_COMPLETE_EVENT:
        printf("Received HCI_READ_REMOTE_EXTENDED_FEATURES_COMPLETE_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        printf("\tStatus: 0x%02X\n", status);
        event_data += 1U;

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        printf("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2U;

        /* Page Number */
        hci_unpack_1_byte_param(&value_1, event_data);
        printf("\tPage Number: 0x%02X\n", value_1);
        event_data += 1U;

        /* Max Page Number */
        hci_unpack_1_byte_param(&value_1, event_data);
        printf("\tMax Page Number: 0x%02X\n", value_1);
        event_data += 1U;

        /* Extended LMP Features */
        printf("\tExtended LMP Features: ");
        appl_dump_bytes(event_data, 8U);
        event_data += 8U;

        break;

    case HCI_SYNCHRONOUS_CONNECTION_COMPLETE_EVENT:
        printf("Received HCI_SYNCHRONOUS_CONNECTION_COMPLETE_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        printf("\tStatus: 0x%02X\n", status);
        event_data += 1U;

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        printf("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2U;

        /* BD_ADDR */
        printf("\tBD_ADDR: ");
        appl_hci_print_bd_addr(event_data);
        event_data += 6U;

        /* Link Type */
        hci_unpack_1_byte_param(&link_type, event_data);
        printf("\tLink Type: 0x%02X", link_type);
        switch (link_type)
        {
        case HCI_SCO_LINK:
            printf(" -> SCO\n");
            break;
        case HCI_ACL_LINK:
            printf(" -> ACL ???\n");
            break;
        case HCI_ESCO_LINK:
            printf(" -> eSCO\n");
            break;
        default:
            printf(" -> ???\n");
            break;
        }
        event_data += 1U;

        /* Transmission Interval */
        hci_unpack_1_byte_param(&value_1, event_data);
        printf("\tTransmission Interval: 0x%02X\n", value_1);
        event_data += 1U;

        /* Retransmission Windows */
        hci_unpack_1_byte_param(&value_1, event_data);
        printf("\tRetransmission Windows: 0x%02X\n", value_1);
        event_data += 1U;

        /* Rx Packet Length */
        hci_unpack_2_byte_param(&value_2, event_data);
        printf("\tRx Packet Length: 0x%04X\n", value_2);
        event_data += 2U;

        /* Tx Packet Length */
        hci_unpack_2_byte_param(&value_2, event_data);
        printf("\tTx Packet Length: 0x%04X\n", value_2);
        event_data += 2U;

        /* Air Mode */
        hci_unpack_1_byte_param(&value_1, event_data);
        printf("\tAir Mode: 0x%02X\n", value_1);
        event_data += 1U;

        /* Setup HCI for SCO Data Reception */
        if ((0x0U == status) &&
            ((HCI_SCO_LINK == link_type) ||
             (HCI_ESCO_LINK == link_type)))
        {
            printf("eSCO Connection Complete.\n");

            /**
             * Currently added support for handling mSBC data over HCI for Wideband speech.
             *
             * TODO:
             * Add support for handling default narrow band speech over HCI.
             */

            if (HCI_ESCO_LINK == link_type)
            {
                /**
                 * Configure for eSCO Audio codec
                 */
                appl_hci_esco_audio_config (value_1);
            }
            else
            {
                /**
                 * Configure for eSCO Audio codec
                 */
                appl_hci_esco_audio_config (LMP_VOICE_AIR_CODING_CVSD);
            }

            /**
             * Start Voice Path
             */
            (BT_IGNORE_RETURN_VALUE) sco_audio_start_pl ();
            printf ("Started for voice recording \n");

            /* Save SCO/eSCO Connection Handle */
            sco_connection_handle = connection_handle;
        }

        break;

    case HCI_SYNCHRONOUS_CONNECTION_CHANGED_EVENT:
        printf("Received HCI_SYNCHRONOUS_CONNECTION_CHANGED_EVENT.\n");

        /* Status */
        hci_unpack_1_byte_param(&status, event_data);
        printf("\tStatus: 0x%02X\n", status);
        event_data += 1U;

        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        printf("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2U;

        /* Transmission Interval */
        hci_unpack_1_byte_param(&value_1, event_data);
        printf("\tTransmission Interval: 0x%02X\n", value_1);
        event_data += 1U;

        /* Retransmission Windows */
        hci_unpack_1_byte_param(&value_1, event_data);
        printf("\tRetransmission Windows: 0x%02X\n", value_1);
        event_data += 1U;

        /* Rx Packet Length */
        hci_unpack_2_byte_param(&value_2, event_data);
        printf("\tRx Packet Length: 0x%04X\n", value_2);
        event_data += 2U;

        /* Tx Packet Length */
        hci_unpack_2_byte_param(&value_2, event_data);
        printf("\tTx Packet Length: 0x%04X\n", value_2);
        event_data += 2U;

        break;
#endif /* BT_HCI_1_2 */
#endif /* BR_EDR_HCI */

    case HCI_VENDOR_SPECIFIC_DEBUG_EVENT:
        printf("Received HCI_VENDOR_SPECIFIC_DEBUG_EVENT.\n");
        printf("Vendor Specific Parameters\n");
        appl_dump_bytes(event_data, event_datalen);

        break;

    default:
#ifdef BT_LE
        /* If LE is enabled, then account for LE Meta Events */
        if (HCI_LE_META_EVENT != event_type)
#endif /* BT_LE */
        {
            printf("Unknown Event Code 0x%02X Received.\n", event_type);
        }
        break;
    }

    if (API_SUCCESS == retval)
    {
#ifdef BT_LE
            /* Check for LE related handling */
            (BT_IGNORE_RETURN_VALUE) appl_hci_le_event_indication_callback
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
            for (i = 0U; i < appl_num_hci_cb; i++)
            {
                (BT_IGNORE_RETURN_VALUE) appl_hci_cb_list[i].hci_event_ind_cb
                (
                    event_type,
                    revent_data,
                    revent_datalen
                );
            }
        }
#endif /* APPL_HCI_NO_PROFILE_EVENT_IND */
    }
    else
    {
        retval = value_2;
    }

    return retval;
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

        printf("Received invalid Octet 0x%02X\n", rx_octet);

        /**
         * Add the check, if this is the eHCILL related octet.
         * If this is eHCILL related octet, return API_SUCCESS.
         * Else return API_FAILURE
         */
        retval = API_FAILURE;

        if (0x30U == rx_octet)
        {
            /* Handle HCILL_GO_TO_SLEEP_IND */
            retval = API_SUCCESS;
        }
        else if (0x33U == rx_octet)
        {
            /* Handle HCILL_WAKE_UP_ACK */
            retval = API_SUCCESS;
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

        return retval;
    }
#endif /* HT_ENABLE_INVALID_RX_IND */

    printf("\n");
    printf("Received HCI Error Indication.\n");
    printf("\tCommand Opcode: 0x%04X -> %s\n",
    opcode, appl_hci_get_command_name(opcode));
    printf("\tError Code = 0x%04X\n", error_code);

    return API_SUCCESS;
}
#endif /* HCI_LITE */


void main_hci_operations ( void )
{
    UCHAR bd_addr[BT_BD_ADDR_SIZE];
    int choice, menu_choice;
    UINT16 handle;
    static UCHAR first_time = 0x0U;
    UCHAR  str[5] = "";
    UINT16 length,j = 0;

    if (0x0U == first_time)
    {
#ifdef BR_EDR_HCI
#ifndef HCI_NO_ESCO_AUTO_ACCEPT
        appl_hci_sync_conn_accept_auto_rsp = 0U;
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
        appl_enh_sco_params.tx_bandwidth = LMP_ESCO_TX_BANDWIDTH_DEFAULT;
        appl_enh_sco_params.rx_bandwidth = LMP_ESCO_RX_BANDWIDTH_DEFAULT;
        appl_enh_sco_params.max_latency = LMP_ESCO_MAX_LATENCY_DEFAULT;
        appl_enh_sco_params.packet_type = LMP_ESCO_PACKET_TYPE_DEFAULT;
        appl_enh_sco_params.retransmission_effort = LMP_ESCO_RETX_EFFORT_DEFAULT;

        /**
         *  If coding_format != LMP_CODING_FRMT_VS (0xFF), then
         *  Company ID and Vendor Specific Codec ID shall be ignored.
         */
        appl_enh_sco_params.tx_coding_frmt.coding_format   = LMP_CODING_FRMT_CVSD;
        appl_enh_sco_params.tx_coding_frmt.company_id      = 0x02U; /* dummy */
        appl_enh_sco_params.tx_coding_frmt.vendor_codec_id = 0x01U; /* dummy */

        appl_enh_sco_params.rx_coding_frmt.coding_format   = LMP_CODING_FRMT_CVSD;
        appl_enh_sco_params.rx_coding_frmt.company_id      = 0x02U; /* dummy */
        appl_enh_sco_params.rx_coding_frmt.vendor_codec_id = 0x01U; /* dummy */

        /* Configuring for mSBC codec */
        appl_enh_sco_params.tx_codec_frame_size = MSBC_FRAME_SIZE;
        appl_enh_sco_params.rx_codec_frame_size = MSBC_FRAME_SIZE;

        appl_enh_sco_params.input_bandwidth = LMP_ESCO_RX_BANDWIDTH_DEFAULT;
        appl_enh_sco_params.output_bandwidth = LMP_ESCO_TX_BANDWIDTH_DEFAULT;

        /**
         *  If coding_format != LMP_CODING_FRMT_VS (0xFF), then
         *  Company ID and Vendor Specific Codec ID shall be ignored.
         */
        appl_enh_sco_params.input_coding_frmt.coding_format   = LMP_CODING_FRMT_CVSD;
        appl_enh_sco_params.input_coding_frmt.company_id      = 0x02U; /* dummy */
        appl_enh_sco_params.input_coding_frmt.vendor_codec_id = 0x01U; /* dummy */

        appl_enh_sco_params.output_coding_frmt.coding_format   = LMP_CODING_FRMT_CVSD;
        appl_enh_sco_params.output_coding_frmt.company_id      = 0x02U; /* dummy */
        appl_enh_sco_params.output_coding_frmt.vendor_codec_id = 0x01U; /* dummy */

        /**
         * mSBC encoded data is a byte stream. so coded unit size is 8 bits
         */
        appl_enh_sco_params.input_coded_data_size  = 8U;
        appl_enh_sco_params.output_coded_data_size = 8U;

        /**
         * Below parameters are valide when
         * coding_format equal to LMP_CODING_FRMT_LINEAR_PCM
         */
        appl_enh_sco_params.input_pcm_data_frmt = 0xFFU; /* dummy */
        appl_enh_sco_params.output_pcm_data_frmt = 0xFFU; /* dummy */
        appl_enh_sco_params.input_pcm_smpl_payload_msb_pos = 0xFFU; /* dummy */
        appl_enh_sco_params.output_pcm_smpl_payload_msb_pos = 0xFFU; /* dummy */

        /* Exchange SCO packets over HCI transport */
        appl_enh_sco_params.input_data_path  = ENH_SCO_DATA_PATH_HCI;
        appl_enh_sco_params.output_data_path = ENH_SCO_DATA_PATH_HCI;

        /**
         * As per Spec., if SCO packet are exchanged over HCI transport, then
         * below parameters shall be zero
         */
        appl_enh_sco_params.input_transport_unit_size = 0x00U;
        appl_enh_sco_params.output_transport_unit_size = 0x00U;

#endif /* HCI_ENH_SCO */
#endif /* HCI_NO_ESCO_AUTO_ACCEPT */
#endif /* BR_EDR_HCI */

        first_time = 0x1U;
    }

    printf("\n");
    printf("%s", hci_options);

    BT_LOOP_FOREVER()
    {
        scanf("%d", &choice);
        menu_choice = choice;

        switch (choice)
        {
        case 0:
            break; /* return; */

        case 1:
            printf("\nRefreshing ...\n");
            printf("%s", hci_options);
            break;

        case 2:
            (BT_IGNORE_RETURN_VALUE) BT_hci_get_local_bd_addr (bd_addr);
            printf("Local " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
            BT_DEVICE_ADDR_ONLY_PRINT_STR (bd_addr));
            break;

        case 3:
            appl_hci_inquiry();
            break;

        case 4:
            if(API_SUCCESS == appl_hci_write_scan_enable())
            {
                appl_hci_read_scan_enable();
            }
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
        printf ("\nHCI_ENH_SCO flag is not defined!\n");
#endif /* HCI_ENH_SCO */
            break;

        case 18:
#ifdef HCI_ENH_SCO
            appl_hci_enhanced_setup_synchronous_connection_response ();
#else
        printf ("\nHCI_ENH_SCO flag is not defined!\n");
#endif /* HCI_ENH_SCO */
            break;

#ifdef BR_EDR_HCI
#ifndef HCI_NO_ESCO_AUTO_ACCEPT
#ifdef BT_HCI_1_2
            
        case 19:
            printf ("Enter eSCO Auto Accept Response\n"
                    "  0 - Synchronous Connection Accept,\n"
                    "  1 - Enhanced Synchronous Connection Accept,\n"
                    "  2 - No response,\n"
                    "  3 - Synchronous Connection Reject\n"
                    "Your Choice: ");
            scanf ("%s", str);
            length = (UINT16)BT_str_len(str);

            if ((1U < length ) || !((str[j] >= '0') && (str[j] <= '3')))
            {
                printf ("Invalid option\n");
            }
            else
            {
                choice = appl_str_to_num(str,length);
                if (3 == choice)
                {
                    printf("Enter Reason (in HEX) for Reject: ");
                    scanf("%x", &choice);
                    appl_hci_sync_conn_reject_reason = (UCHAR)choice;
                }
                else if (3 > choice)
                {
                    appl_hci_sync_conn_accept_auto_rsp = (UCHAR)choice;
                }
                else
                {
                    /* MISRA C-2012 Rule 15.7 */
                }
            }
            break;
            
#endif /* BT_HCI_1_2 */
#endif /* HCI_NO_ESCO_AUTO_ACCEPT */
#endif /* BR_EDR_HCI */

        case 20:
            appl_hci_get_connection_details();
            break;

        case 22:
            appl_hci_read_page_timeout();
            break;

        case 23:
            appl_hci_read_connection_accept_timeout();
            break;

        case 24:
            appl_hci_read_class_of_device();
            break;

        case 25:
#ifdef HCI_ENH_SCO
            appl_hci_read_local_supported_codecs();
#else
            printf ("\nHCI_ENH_SCO flag is not defined!\n");
#endif /* HCI_ENH_SCO */
            break;

        case 26:
#ifdef BR_EDR_HCI
            printf("Read Local supported and Extended features...\n");

            (BT_IGNORE_RETURN_VALUE) BT_hci_read_local_supported_features();
            (BT_IGNORE_RETURN_VALUE) BT_hci_read_local_extended_features(0U);
            (BT_IGNORE_RETURN_VALUE) BT_hci_read_local_extended_features(1U);
#else /* BR_EDR_HCI */
            printf("BR_EDR_HCI flag is not defined!\n");
#endif /* BR_EDR_HCI */
            break;

        case 27:
            printf("Read Local Version Information ...\n");

            (BT_IGNORE_RETURN_VALUE) BT_hci_read_local_version_information();
            break;

        case 28:
            printf("Read Remote supported features. Enter Required Information ...\n");

            printf("ACL Handle of the Remote Device (in Hex) = ");
            scanf("%x", &choice);
            handle = (UINT16)choice;

            (BT_IGNORE_RETURN_VALUE) BT_hci_read_remote_version_information(handle);
            (BT_IGNORE_RETURN_VALUE) BT_hci_read_remote_supported_features(handle);
            (BT_IGNORE_RETURN_VALUE) BT_hci_read_remote_extended_features(handle, 0U);
            (BT_IGNORE_RETURN_VALUE) BT_hci_read_remote_extended_features(handle, 1U);
            break;

        case 29:
            printf("Read inquiry mode\n");
            (BT_IGNORE_RETURN_VALUE) BT_hci_read_inquiry_mode();
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

        case 38:
            appl_hci_sniff_subrating();
            break;

        case 39:
            appl_hci_qos_setup();
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
            if (API_SUCCESS == appl_hci_write_class_of_device())
            {
                appl_hci_read_class_of_device();
            }
            break;

        case 45:
            appl_hci_change_local_name();
            appl_hci_read_local_name();
            break;

        case 46:
            appl_hci_read_default_link_policy_settings();
            break;

        case 47:
            appl_hci_write_default_link_policy_settings();
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
            printf ("\nHCI_FLOW_CONTROL_COMMANDS flag not defined\n");
        #endif /* HCI_FLOW_CONTROL_COMMANDS */
            break;

        case 54:
            appl_hci_read_current_iaclap();
            break;

        case 55:
            appl_hci_write_current_iaclap();
            break;

        case 56:
            appl_hci_change_packet_type();
            break;

        case 57:
            appl_change_esco_packet_type();
            break;

        case 58:
            appl_cancel_inquiry_scan();
            break;

        case 60:
            (BT_IGNORE_RETURN_VALUE) BT_hci_read_voice_setting();
            break;

        case 61:
        #ifdef BT_3_0
            appl_hci_read_enhanced_transmit_power_level();
        #else
            printf ("\nBT_3_0 flag not defined\n");
        #endif /* BT_3_0 */
            break;

        case 62:
        #if ((defined BT_3_0) && !(defined HCI_LITE))
            appl_hci_read_encryption_key_size();
        #else
            printf ("\nBT_3_0 flag not defined\n");
            printf ("or HCI_LITE flag is defined\n");
        #endif /* ((defined BT_3_0) && !(defined HCI_LITE)) */
            break;

        case 63:
            appl_hci_read_link_policy();
            break;

        case 65:
#ifdef BT_BRSC
            appl_hci_read_secure_connections_host_support();
#else /* BT_BRSC */
            printf ("BT_BRSC flag not defined\n");
#endif /* BT_BRSC */
            break;

        case 66:
#ifdef BT_BRSC
            appl_hci_write_secure_connections_host_support();
#else /* BT_BRSC */
            printf ("BT_BRSC flag not defined\n");
#endif /* BT_BRSC */
            break;

        case 67:
#ifdef BT_BRSC_TEST
            appl_hci_write_secure_connections_test_mode();
#else /* BT_BRSC_TEST */
            printf ("BT_BRSC_TEST flag not defined\n");
#endif /* BT_BRSC_TEST */
            break;

        case 68:
#ifdef HCI_READ_BUFFER_SIZE_SUPPORT
            appl_hci_read_buffersize();
#else
            printf ("HCI_READ_BUFFER_SIZE_SUPPORT flag not defined\n");
#endif /* HCI_READ_BUFFER_SIZE_SUPPORT */
            break;

        case 70:
    #ifdef BT_EIR
            appl_hci_write_extended_inquiry_response();
    #else
            printf("BT_EIR flag is not defined\n");
    #endif /* BT_EIR */
            break;
        case 71:
#ifdef BT_RF_TEST
            appl_hci_rx_test_command();
#else
            printf("Test mode Flag not set");
#endif
            break;

        case 72:
#ifdef BT_RF_TEST
            appl_hci_tx_test_command();
#else
            printf("Test mode Flag not set");
#endif
            break;

        case 74:
            printf("Vendor Specific command\n");
            appl_vendor_specific_commands();
            break;

        case 75:
            printf("Generic HCI command\n");
            appl_hci_generic_command();
            break;

        case 76:
#ifdef BT_RF_TEST
            printf("Enable device under test mode\n");
            BT_hci_enable_device_under_test_mode();
#else
            printf("Test mode Flag not set");
#endif
            break;

#ifdef BT_LE
        case 80:
            main_hci_le_operations();

            printf("\n");
            printf("%s", hci_options);
            break;
#endif /* BT_LE */

        case 100:
            (BT_IGNORE_RETURN_VALUE) BT_hci_reset();
            break;

        default:
            printf("Invalid choice - %d. Please try again\n", choice);
            break;
        }

        if (0 == menu_choice)
        {
            /* return */
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

    for (i = 0U; i < size; i++)
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
    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;

    /* Null Check for Callback Function */
    if (NULL == callback_fn)
    {
        printf("Unable to register NULL as HCI Callback\n");
        retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
        /* Lock HCI */
        APPL_HCI_MUTEX_LOCK();

        /* Check if the number of callbacks registered has reached Max */
        if (APPL_MAX_NUM_HCI_CB == appl_num_hci_cb)
        {
            printf("FAILED to Register HCI Callback. Limit Reached - %d\n",
            appl_num_hci_cb);

            /* APPL_HCI_MUTEX_UNLOCK(); */
            retval = API_FAILURE; /* return API_FAILURE; */
        }

        /* Verify that the Callback function is NOT already registered */
        for (i = 0U; i < appl_num_hci_cb; i++)
        {
            if (appl_hci_cb_list[i].hci_event_ind_cb == callback_fn)
            {
                printf("FAILED to Register HCI Callback. Already Registered\n");

                /* APPL_HCI_MUTEX_UNLOCK(); */
                retval = API_FAILURE; /* return API_FAILURE; */
            }
        }

        if (API_SUCCESS == retval)
        {
            printf("Registering HCI Callback Function at Location %d\n",
            appl_num_hci_cb);

            /* Register the Callback Function */
            appl_hci_cb_list[appl_num_hci_cb++].hci_event_ind_cb = callback_fn;

            printf("Number of Registered HCI Callback Functions = %d\n",
            appl_num_hci_cb);
        }

        /* Unlock HCI */
        APPL_HCI_MUTEX_UNLOCK();
    }

    return retval; /* API_SUCCESS; */
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

    /* Init */
    retval = API_SUCCESS;

    /* Null Check for Callback Function */
    if (NULL == callback_fn)
    {
        printf("Unable to unregister NULL as HCI Callback\n");
        retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
        /* Lock HCI */
        APPL_HCI_MUTEX_LOCK();

        /*
         *  Check if the Callback Function is registered.
         *  If found, remove from the Callback Function List.
         */
        for (i = 0U; i < appl_num_hci_cb; i++)
        {
            if (appl_hci_cb_list[i].hci_event_ind_cb == callback_fn)
            {
                printf("Unregistering HCI Callback Function from Location %u\n",
                (unsigned int) i);

                /* Unregister the Callback Function */
                appl_hci_cb_list[i].hci_event_ind_cb = NULL;
                break;
            }
        }

        if (i < appl_num_hci_cb)
        {
            for (j = i+1U; j < appl_num_hci_cb; j ++)
            {
                appl_hci_cb_list[j - 1U].hci_event_ind_cb =
                appl_hci_cb_list[j].hci_event_ind_cb;
            }

            /* Decrement Number of Callback Functions registered */
            appl_num_hci_cb --;

            printf("Number of Registered HCI Callback Functions = %d\n",
            appl_num_hci_cb);

            retval = API_SUCCESS;
        }
        else
        {
            printf("Cannot Unregister HCI Function Pointer. NOT in CB List\n");

            retval = API_FAILURE;
        }

        /* Unlock HCI */
        APPL_HCI_MUTEX_UNLOCK();
    }

    return retval;
}
#endif /* APPL_HCI_NO_PROFILE_EVENT_IND */

#ifdef BR_EDR_HCI
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
    UCHAR enable;
    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;

    /* Null Check for eSCO parameters */
    if ((BT_TRUE == set_sco_param) && (NULL == sco_params))
    {
        printf("Unable to set NULL as eSCO parameters\n");
        retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
        /* Lock HCI */
        APPL_HCI_MUTEX_LOCK();

        /* Set flag */
        appl_hci_set_esco_connect_param = set_sco_param;

        if (BT_TRUE == appl_hci_set_esco_connect_param)
        {
            appl_esco_params = *(sco_params);
        }

        /* Update the audio config */
        enable =
        (LMP_VOICE_AIR_CODING_TRANSPARENT ==
        (appl_esco_params.voice_setting & LMP_VOICE_AIR_CODING_TRANSPARENT))? BT_TRUE: BT_FALSE;
        sco_audio_set_wideband_pl(enable);

        (BT_IGNORE_RETURN_VALUE)BT_hci_set_esco_channel_parameters (&appl_esco_params);

        /* Unlock HCI */
        APPL_HCI_MUTEX_UNLOCK();
    }

    return retval;
}

void appl_hci_get_esco_channel_parameters(HCI_SCO_IN_PARAMS * esco_param)
{
    BT_mem_copy (esco_param, &appl_esco_params, sizeof(HCI_SCO_IN_PARAMS));
}
#ifdef HCI_ENH_SCO
API_RESULT appl_hci_set_enh_esco_channel_parameters
           (
               /* IN */ HCI_ENH_SCO_PARAMS * enh_sco_params
           )
{
    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;

    /* Null Check for eSCO parameters */
    if (NULL == enh_sco_params)
    {
        printf("Unable to set NULL as Enh SCO parameters\n");
        retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
        /* Lock HCI */
        APPL_HCI_MUTEX_LOCK();
        appl_enh_sco_params = *(enh_sco_params);
        (BT_IGNORE_RETURN_VALUE) BT_hci_set_enh_sco_channel_parameters (&appl_enh_sco_params);
        /* Unlock HCI */
        APPL_HCI_MUTEX_UNLOCK();
    }
    return retval;
}

void appl_hci_get_enh_esco_channel_parameters(HCI_ENH_SCO_PARAMS * enh_esco_param)
{
    BT_mem_copy (enh_esco_param, &appl_enh_sco_params, sizeof(HCI_ENH_SCO_PARAMS));
}
#endif /* HCI_ENH_SCO */
#endif /* BT_HCI_1_2 */
#endif /* HCI_NO_ESCO_AUTO_ACCEPT */

#ifdef BT_EIR
void appl_print_extended_inquiry_response(UCHAR *eir_data, UINT16 eir_datalen)
{
    UINT16 index, length, consumed_length;
    UCHAR name;

    printf("Extended Inquiry Response\n");

    printf("Total Length = %d\n", eir_datalen);

    if((NULL == eir_data) || (0U == eir_datalen))
    {
        printf("Empty Response\n");

        /* return; */
    }
    else
    {
        consumed_length = 0U;

        while(consumed_length < eir_datalen)
        {
            /* First Byte is the Length Field */
            length = *eir_data;

            printf("\tEIR Element Length: 0x%02X\n", length);

            if(0U == length)
            {
                break;
            }

            name = 0U;
            eir_data ++;
            printf("\tEIR Data Type: ");
            switch(*eir_data)
            {
            case HCI_EIR_DATA_TYPE_FLAGS:
                printf("Flags\n");
                break;

            case HCI_EIR_DATA_TYPE_INCOMPLETE_16_BIT_UUIDS:
                printf("More 16-bit UUIDs available\n");
                break;

            case HCI_EIR_DATA_TYPE_COMPLETE_16_BIT_UUIDS:
                printf("Complete list of 16-bit UUIDs\n");
                break;

            case HCI_EIR_DATA_TYPE_INCOMPLETE_32_BIT_UUIDS:
                printf("More 32-bit UUIDs available\n");
                break;

            case HCI_EIR_DATA_TYPE_COMPLETE_32_BIT_UUIDS:
                printf("Complete list of 32-bit UUIDs\n");
                break;

            case HCI_EIR_DATA_TYPE_INCOMPLETE_128_BIT_UUIDS:
                printf("More 128-bit UUIDs available\n");
                break;

            case HCI_EIR_DATA_TYPE_COMPLETE_128_BIT_UUIDS:
                printf("Complete list of 128-bit UUIDs\n");
                break;

            case HCI_EIR_DATA_TYPE_SHORTENED_LOCAL_NAME:
                printf("Shortened local name\n");
                name = 1U;
                break;

            case HCI_EIR_DATA_TYPE_COMPLETE_LOCAL_NAME:
                printf("Complete local name\n");
                name = 1U;
                break;

            case HCI_EIR_DATA_TYPE_TX_POWER_LEVEL:
                printf("TX Power Level\n");
                break;

            case HCI_EIR_DATA_TYPE_CLASS_OF_DEVICE:
                printf("Class of Device\n");
                break;

            case HCI_EIR_DATA_TYPE_SIMPLE_PAIRING_HASH_C:
                printf("Simple Pairing Hash C\n");
                break;

            case HCI_EIR_DATA_TYPE_SIMPLE_PAIRING_RANDOMIZER_R:
                printf("Simple Pairing Randomizer R\n");
                break;

            case HCI_EIR_DATA_TYPE_DEVICE_ID:
                printf("Device ID\n");
                break;

            case HCI_EIR_DATA_TYPE_MANUFACTURER_SPECIFIC_DATA:
                printf("Manufacturer Specific Data\n");
                break;

            default:
                printf("Invalid EIR Data Type: %d\n", (*eir_data));
                break;
            }

            eir_data ++;

            printf("\t");
            for (index = 0U; index < (length - 1U); index ++)
            {
                if (0U != name)
                {
                    printf("%c", *eir_data);
                }
                else
                {
                    printf("%02X ", *eir_data);
                }

                eir_data ++;
            }

            printf("\n");

            consumed_length += (length + 1U);
        }
    }

    return;
}


void appl_hci_write_extended_inquiry_response(void)
{
    UCHAR           fec_required;

    /* Only Name and Service UUID will be sent through EIR */
    UCHAR           device_name[] = "EtherMind";
    API_RESULT      retval;

#ifdef SDP_SERVER
    UINT32          num_uuids;
    UCHAR          *uuids;
    HCI_EIR_DATA    eir_data[2U];
#else
    HCI_EIR_DATA    eir_data[1U];
#endif /* SDP_SERVER */

    /* Init */
    retval = API_SUCCESS;

    /* Read the EIR response */
    (BT_IGNORE_RETURN_VALUE)BT_hci_read_extended_inquiry_response();

    fec_required = (UCHAR)1U;

    /* Set Local Name */
    eir_data[0U].eir_data_type = HCI_EIR_DATA_TYPE_COMPLETE_LOCAL_NAME;
    eir_data[0U].eir_data = device_name;
    eir_data[0U].eir_data_len = (UCHAR)BT_str_len(device_name);

#ifdef SDP_SERVER
    /* Set Service UUIDs */
    (BT_IGNORE_RETURN_VALUE) BT_dbase_get_num_service_class_uuids
    (
        &num_uuids
    );

    printf("Number of UUIDs = %lu\n", num_uuids);

    uuids = (UCHAR *)BT_alloc_mem(num_uuids * sizeof(UINT16));

    if (NULL == uuids)
    {
        printf("Failed to allocate memory for UUIDs\n");
        retval = API_FAILURE; /*  return; */
    }

    if (API_SUCCESS == retval)
    {
        num_uuids *= sizeof(UINT16);

        (BT_IGNORE_RETURN_VALUE)BT_dbase_get_service_class_uuids
        (
            uuids,
            &num_uuids
        );

        eir_data[1U].eir_data_type = HCI_EIR_DATA_TYPE_COMPLETE_16_BIT_UUIDS;
        eir_data[1U].eir_data = uuids;
        eir_data[1U].eir_data_len = (UCHAR)num_uuids;

        (BT_IGNORE_RETURN_VALUE)BT_hci_write_extended_inquiry_response
        (
            fec_required,
            eir_data,
            2U
        );

        BT_free_mem(uuids);
        uuids = NULL;
    }
#else

    if (API_SUCCESS == retval)
    {
        (BT_IGNORE_RETURN_VALUE)BT_hci_write_extended_inquiry_response
        (
            fec_required,
            eir_data,
            1U
        );
    }
#endif /* SDP_SERVER */

    if (API_SUCCESS == retval)
    {
        (BT_IGNORE_RETURN_VALUE)BT_hci_read_extended_inquiry_response();
    }
}

#endif /* BT_EIR */

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
void appl_hci_esco_audio_config(UCHAR coding)
{
    API_RESULT retval;

    /* Reset Global Variables */
    sco_connection_handle = HCI_INVALID_CONNECTION_HANDLE;

#ifdef HCI_SCO
    sco_rx_msbc_data_offset            = 0x00U;
    expected_sco_rx_msbc_frame_seq_num = 0x00U;
    sco_tx_msbc_frame_seq_num          = 0x00U;
    sco_tx_pcm_data_offset             = 0x00U;
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
    msbc_enc_io.encoder_params.nrof_channels = 1U;
    msbc_enc_io.encoder_params.sampling_frequency = SBC_SAMPLING_FREQ_16000;
    msbc_enc_io.encoder_params.allocation_method = SBC_LOUDNESS;
    msbc_enc_io.encoder_params.nrof_blocks = SBC_BLOCK_SIZE_15;
    msbc_enc_io.encoder_params.nrof_subbands = SBC_SUBBAND_8;
    msbc_enc_io.encoder_params.bitpool = 26U;

    /* Calculate mSBC Encoded frame length */
    sbc_calculate_frame_length(&msbc_enc_io.encoder_params, &msbc_enc_output_frame_len);

    /* Init mSBC Encoder */
    retval = sbc_encoder_init
             (
                 &msbc_enc_io
             );
    if (API_SUCCESS != retval)
    {
        printf ("mSBC Encoder Init Failed, returning ...\n");
        return;
    }
    else
    {
        printf ("mSBC Encoder Initialized\n");
    }

    /* Init mSBC Decoder */
    sbc_decoder_init
    (
        &msbc_dec_io
    );

    printf ("mSBC Decoder Initialized\n");
#endif /* HCI_SCO */

    /**
     * Setup Config
     */
    sco_audio_config.num_ch = 0x01U;     /* No. of channels: Mono */
    sco_audio_config.sample_len = 16U;   /* Each PCM sample size */
    sco_audio_config.sampl_freq = (LMP_VOICE_AIR_CODING_TRANSPARENT == coding)? 16000U: 8000U; /* Sampling frequency */
    sco_audio_config.data_size  = 512U; /* Record data length */

#ifdef HCI_SCO
    sco_audio_config.sco_in_complete_cb = appl_hci_sco_audio_mic_record_callback;
    sco_audio_config.sco_out_complete_cb = appl_hci_sco_audio_spkr_playack_callback;
#else /* HCI_SCO */
    sco_audio_config.sco_in_complete_cb = NULL;
    sco_audio_config.sco_out_complete_cb = NULL;
#endif /* HCI_SCO */

    /* TBD: How did we arrive at 512? */

    /* Setup MIC */
    retval = sco_audio_setup_pl
             (
                 &sco_audio_config
             );
    if (API_SUCCESS != retval)
    {
        printf ("Setup for SCO audio failed\n");
    }
    else
    {
        printf ("Setup for SCO audio: Success\n");
    }

#ifdef HCI_SCO
    /* Register callback with HCI for SCO data */
    (BT_IGNORE_RETURN_VALUE) BT_hci_register_sco_data_handler
    (
        appl_hci_esco_msbc_data_handler
    );

    printf ("Registered callback with HCI for SCO data\n");
#endif /* HCI_SCO */

    return;
}

#ifdef HCI_SCO
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
    UCHAR       msbc_enc_data[256U];

    /**
     * Note:
     * Assumption is mic_datalen WILL alway be < 512 bytes
     */

    if ((NULL == mic_data) || (0x00U == mic_datalen))
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
    input_data_offset   = 0x00U;
    outut_data_offset   = 0x00U;

    /**
     * Process the received data
     */
    while (pcm_datalen >= MSBC_ENCODER_INPUT_FRAME_LEN)
    {
        /* Check if previously received pcm_data has to be processed */
        if (0x00U == sco_tx_pcm_data_offset)
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

            msbc_enc_io.encoder_input_buffer  = (UINT16 *)&sco_tx_pcm_data[0U];
            msbc_enc_io.encoder_input_buflen  = MSBC_ENCODER_INPUT_FRAME_LEN;
        }

        /**
         * Pack the H2 Header with synchronization word and sequence number.
         * Ref. page #115 of HFPv1.7 Spec. for more details.
         */
        BT_PACK_LE_2_BYTE_VAL(&msbc_enc_data[outut_data_offset], HFP_SCO_H2_SYNC_HDR);

        /* Pack Sequence number */
        if (0x00U == sco_tx_msbc_frame_seq_num)
        {
            msbc_enc_data[outut_data_offset + 1U] |= 0x00U;
        }
        else if (0x01U == sco_tx_msbc_frame_seq_num)
        {
            msbc_enc_data[outut_data_offset + 1U] |= 0x30U;
        }
        else if (0x02U == sco_tx_msbc_frame_seq_num)
        {
            msbc_enc_data[outut_data_offset + 1U] |= 0xC0U;
        }
        else if (0x03U == sco_tx_msbc_frame_seq_num)
        {
            msbc_enc_data[outut_data_offset + 1U] |= 0xF0U;
        }
        else
        {
            /* Ideally should not come here */
            sco_tx_msbc_frame_seq_num = 0x00U;
        }

        /* Update the SCO packet offset */
        outut_data_offset += 2U;

        msbc_enc_io.encoder_output_buffer = &msbc_enc_data[outut_data_offset];
        msbc_enc_io.encoder_output_buflen = msbc_enc_output_frame_len; /* 57 */

        /* Start Encoding Process */
        retval = sbc_encoder
                 (
                     &msbc_enc_io
                 );
        if (API_SUCCESS != retval)
        {
            printf ("mSBC Encoding Failed\n");
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
        sco_tx_msbc_frame_seq_num += 0x01U;

        /* Max. value of seq. number is 3 */
        sco_tx_msbc_frame_seq_num &= 0x03U;

        sco_tx_pcm_data_offset = 0x00U;
    }

    if (0U < outut_data_offset)
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
            printf ("Failed: BT_hci_sco_write\n");
        }
    }

    /**
     * If the pcm_datalen NOT equal to 0, then save and process in next callback
     */
    if (0x00U != pcm_datalen)
    {
        BT_mem_copy
        (
            &sco_tx_pcm_data[0U],
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
    UCHAR      msbc_decoded_pcm_data[1024U];
    UCHAR      msbc_decoded_pcm_data_len;
    UINT16     msbc_decoded_pcm_data_offset;

    /* Check */
    if ((NULL == packet) ||
        (packet_len <= 3U))
    {
        printf ("Invalide SCO packet \n");
        return API_FAILURE;
    }

    /* Init */
    retval = API_SUCCESS;
    msbc_decoded_pcm_data_len    = 0U;
    msbc_decoded_pcm_data_offset = 0U;
    rx_seq_num                   = 0U;

    /* Extract SCO Handle */
    BT_UNPACK_LE_2_BYTE
    (
        &sco_handle,
         packet
    );

    /* Validate SCO Handle */
    if (sco_handle != sco_connection_handle)
    {
        printf ("SCO Connection Handle Does NOT match\n");
        return API_FAILURE;
    }
    else
    {
        ;
        /* printf("SCO Connection Handle: 0x%04X\n", sco_handle); */
        /* appl_dump_bytes(packet, packet_len); */
    }

    /* TBD: Remove this return. Just for testing. */
    /* return API_SUCCESS; */

    /* Extract SCO data len */
    sco_data_len = packet[2U];

    /* Added previously pending SCO data */
    sco_data_len += sco_rx_msbc_data_offset;

    /* For SCO handle[2], SCO data length[1] */
    offset = 0x03U;

    /* Process the data */
    while (sco_data_len >= MSBC_DECODER_INPUT_FRAME_LEN)
    {
        /* Check if previously received mSBC sco_data has to be processed */
        if (0x00U == sco_rx_msbc_data_offset)
        {
            /* === No Previous Data === */

            /* Extract H2 Sync Header */
            BT_UNPACK_LE_2_BYTE
            (
                &esco_h2_sync_hdr,
                &packet[offset]
            );

            /* Update the offset */
            offset += 2U;
            sco_data_len -= 2U;

            /* Validate eSCO H2 Sync Header */
            if (HFP_SCO_H2_SYNC_HDR != (esco_h2_sync_hdr & 0x0FFFU))
            {
                printf ("eSCO H2 Header does NOT match\n");
                /* return error ? */
            }

            /* Validate the sequence number */
            switch((esco_h2_sync_hdr & 0xF000U))
            {
            case 0x0000U:
                rx_seq_num = 0U;
                break;

            case 0x3000U:
                rx_seq_num = 1U;
                break;

            case 0xC000U:
                rx_seq_num = 2U;
                break;

            case 0xF000U:
                rx_seq_num = 3U;
                break;

            default:
                printf("Invalid Sequence Number: 0x%04X\n", (esco_h2_sync_hdr & 0xF000U));
                break;
            }

            if (expected_sco_rx_msbc_frame_seq_num == rx_seq_num)
            {
                /* Set mSBC Decoder input buffer params */
                msbc_dec_io.decoder_input_buffer = &packet[offset];
                msbc_dec_io.decoder_input_buflen = MSBC_DECODER_INPUT_FRAME_LEN;

                /* Update next expected sequence number */
                expected_sco_rx_msbc_frame_seq_num += 1U;
            }
            else
            {
                printf ("Have missed mSBC frame\n");
                printf ("Expected frame seq_num:0x%02x, Received seq_num:0x%02x\n",
                    expected_sco_rx_msbc_frame_seq_num, rx_seq_num);

                /* Set next expected based on rx_seq_num */
                expected_sco_rx_msbc_frame_seq_num = (rx_seq_num + 1U);

                /**
                 * TODO: Have to handle through PLC.
                 */
                msbc_dec_io.decoder_input_buffer = &packet[offset];
                msbc_dec_io.decoder_input_buflen = MSBC_DECODER_INPUT_FRAME_LEN;
            }

            /* Max. seq. number value is 3 */
            expected_sco_rx_msbc_frame_seq_num &= 0x03U;
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
            msbc_dec_io.decoder_input_buffer = &sco_rx_msbc_data[0U];
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
            printf ("mSBC Decoding Failed\n");
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

        sco_rx_msbc_data_offset = 0U;

    }

    if (msbc_decoded_pcm_data_offset >= MSBC_DECODER_OUTPUT_FRAME_LEN)
    {
        /* Write the decoded PCM data to the speaker for local playback */
        sco_audio_spkr_play_pl
        (
            msbc_decoded_pcm_data,
            msbc_decoded_pcm_data_offset
        );
    }

    /**
     * If the sco_data_len NOT equal to 0, then save and process in next callback
     */
    if (0x00U != sco_data_len)
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
#endif /* BR_EDR_HCI */

#ifdef BT_RF_TEST
void appl_hci_rx_test_command(void)
{
    printf("HCI_CMD_MARVELL_RX_TEST. Enter Required Information ...\n");

    API_RESULT   retval;
    struct __attribute__((__packed__)) rx_test_cmd
    {
        UCHAR        test_scenario;
        UCHAR        tx_frequency;
        UCHAR          rx_frequency;
        UCHAR          pkt_type;
        UINT32         num_pkt;
        UINT16       len;
        UCHAR          tx_am_addr;
        UCHAR        tx_addr[BT_BD_ADDR_SIZE];
        UCHAR          report_err_pkt;
    };

    unsigned long int read_val, i;
    int   bd[BT_BD_ADDR_SIZE];

    struct rx_test_cmd * rx_test;
    rx_test = (struct rx_test_cmd *)BT_alloc_mem(sizeof(struct rx_test_cmd));

    APPL_TRC("Enter Test Scenario : \n");
    scanf("%X",&read_val);
    rx_test->test_scenario = (UCHAR) read_val;

    APPL_TRC("Enter Tx Frequency : \n");
    scanf("%X",&read_val);
    rx_test->tx_frequency = (UCHAR) read_val;

    APPL_TRC("Enter Rx Frequency: \n");
    scanf("%X",&read_val);
    rx_test->rx_frequency = (UCHAR) read_val;

    APPL_TRC("Enter Packet Type : \n");
    scanf("%X",&read_val);
    rx_test->pkt_type = (UCHAR) read_val;
    printf("Packet Type : %X\n", rx_test->pkt_type);

    APPL_TRC("Enter Number Of Packets : \n");
    scanf("%X",&read_val);
    rx_test->num_pkt = (UINT32) read_val;
    printf("num_packets= %X\n", rx_test->num_pkt);

    APPL_TRC("Enter Length Of Test Data : \n");
    scanf("%X",&read_val);
    rx_test->len = (UINT16) read_val;
    printf("Length OF Test Data= %X\n", rx_test->len);

    APPL_TRC("Enter Tx AM Address : \n");
    scanf("%X",&read_val);
    rx_test->tx_am_addr = (UCHAR) read_val;

    BT_mem_set(rx_test->tx_addr, 0x0, BT_BD_ADDR_SIZE);
    APPL_TRC("Enter Tx BD Address : \n");
    scanf("%x %x %x %x %x %x", &bd[0], &bd[1], &bd[2], &bd[3], &bd[4], &bd[5]);
    for(i = 0; i < BT_BD_ADDR_SIZE; i++)
    {
        rx_test->tx_addr[i] = (UCHAR)bd[i];
    }

    APPL_TRC("Enter Report Error Packets : \n");
    scanf("%X",&read_val);
    rx_test->report_err_pkt = (UCHAR) read_val;

    BT_hci_vendor_specific_command(0x0018U, (UCHAR *)rx_test, sizeof(struct rx_test_cmd));
    APPL_TRC("Sent Command RX_TEST\n");

    BT_free_mem(rx_test);
}

void appl_hci_tx_test_command(void)
{
    printf("HCI_CMD_MARVELL_TX_TEST. Enter Required Information ...\n");
    struct __attribute__((__packed__)) tx_test_cmd
    {
        UCHAR        rx_on_start;
        UCHAR          synt_on_start;
        UCHAR          tx_on_start;
        UCHAR        phd_off_start;
        UCHAR          test_scenario;
        UCHAR          hopping_mode;
        UCHAR        tx_channel;
        UCHAR          rx_channel;
        UCHAR          tx_test_interval;
        UCHAR          pkt_type;
        UINT16          len;
        UCHAR          whitening;
        UINT32          num_pkt;
        UCHAR          tx_pwr;
    };
    unsigned int read_val;
    struct tx_test_cmd * tx_test;

    tx_test = (struct tx_test_cmd *)BT_alloc_mem(sizeof(struct tx_test_cmd));
    /*Set to default*/
    tx_test->rx_on_start = 0x80;
    tx_test->synt_on_start = 0x80;
    tx_test->tx_on_start = 0x80;
    tx_test->phd_off_start = 0x80;
    /* */
    APPL_TRC("Enter Test Scenario : \n");
    scanf("%X",&read_val);
    tx_test->test_scenario = (UCHAR) read_val;

    APPL_TRC("Enter Hopping Mode : \n");
    scanf("%X",&read_val);
    tx_test->hopping_mode = (UCHAR) read_val;

    APPL_TRC("Enter Tx Channel : \n");
    scanf("%X",&read_val);
    tx_test->tx_channel = (UCHAR) read_val;

    APPL_TRC("Enter Rx Channel : \n");
    scanf("%X",&read_val);
    tx_test->rx_channel = (UCHAR) read_val;

    APPL_TRC("Enter Tx Test Interval : \n");
    scanf("%X",&read_val);
    tx_test->tx_test_interval = (UCHAR) read_val;

    APPL_TRC("Enter Packet Type : \n");
    scanf("%X",&read_val);
    tx_test->pkt_type = (UCHAR) read_val;

    APPL_TRC("Enter Length Of Test Data : \n");
    scanf("%X",&read_val);
    tx_test->len = (UINT16) read_val;

    APPL_TRC("Enter Whitening : \n");
    scanf("%X",&read_val);
    tx_test->whitening = (UCHAR) read_val;

    APPL_TRC("Enter Number Of Packets : \n");
    scanf("%X",&read_val);
    tx_test->num_pkt = (UINT32) read_val;

    APPL_TRC("Enter Tx Power : \n");
    scanf("%X",&read_val);
    tx_test->tx_pwr = (UCHAR) read_val;

    BT_hci_vendor_specific_command(0x0019U, (UCHAR *)tx_test, sizeof(struct tx_test_cmd));
    APPL_TRC("Sent Command TX_TEST\n");

    BT_free_mem(tx_test);

}
#endif/*BT_RF_TEST*/


void appl_hci_generic_command(void)
{
    UCHAR          ogf;
    UINT16         ocf;
    UCHAR          param_len;
    UCHAR          *param;
    unsigned int read_val;
    unsigned int i;

    APPL_TRC(" Enter OGF =  ...\n");
    scanf("%X",&read_val);
    ogf = (UCHAR) read_val;

    APPL_TRC(" Enter OCF =  ...\n");
    scanf("%X",&read_val);
    ocf = (UINT16) read_val;

    APPL_TRC(" Enter para length =  ...\n");
    scanf("%X",&read_val);
    param_len = (UCHAR) read_val;

    param = (UCHAR *)BT_alloc_mem(param_len);

    for(i = 0;i < param_len;i++)
    {
        APPL_TRC(" Enter param[%d] = ", i);
        scanf("%X",&read_val);
        param[i] = (UCHAR) read_val;
        APPL_TRC("\n");
    }

    (BT_IGNORE_RETURN_VALUE)BT_hci_send_command(ogf, ocf, (UCHAR *)param, param_len);

    BT_free_mem(param);
};

