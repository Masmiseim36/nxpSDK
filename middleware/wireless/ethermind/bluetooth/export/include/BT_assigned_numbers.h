
/**
 *  \file BT_assigned_numbers.h
 *
 *  This header file describes various definitions from
 *  the Bluetooth Assigned Numbers Specification.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_ASSIGNED_NUMBERS_
#define _H_BT_ASSIGNED_NUMBERS_

/* --------------------------------- Header File Inclusion */

/* --------------------------------- Global Definitions */

/* ===============================================  Inquiry Access Codes */
#define BT_GIAC                      0x9E8B33U
#define BT_LIAC                      0x9E8B00U


/* ===============================================  Class of Device (CoD) */
/*
 *  23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
 * _________________________________________________________________________
 * |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
 * |__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|
 *
 * <--------------------------------><-------------><----------------><---->
 *             |                            |             |             |
 *             v                            |             |             |
 *    MAJOR SERVICE CLASSES                 v             |             v
 *                                MAJOR DEVICE CLASS      |       FORMAT TYPE
 *                                                        v
 *                                                MINOR DEVICE CLASS
 *
 *  FORMAT TYPE:-
 *
 *   01 00
 *  _______
 *  |  |  |
 *  |__|__|
 *
 *   0  0 -> format #1
 */

/*
 * MAJOR SERVICE CLASSES:-
 *
 *    23 22 21 20 19 18 17 16 15 14 13
 *   __________________________________
 *   |  |  |  |  |  |  |  |  |  |  |  |
 *   |__|__|__|__|__|__|__|__|__|__|__|
 *     |  |  |  |  |  |  |  |  |  |  |
 *     |  |  |  |  |  |  |  |  |  |  |_ Limited Discoveral Mode
 *     |  |  |  |  |  |  |  |  |  |
 *     |  |  |  |  |  |  |  |  |  |__ (Reserved)
 *     |  |  |  |  |  |  |  |  |
 *     |  |  |  |  |  |  |  |  |__ (Reserved)
 *     |  |  |  |  |  |  |  |
 *     |  |  |  |  |  |  |  |___ Positioning (Location identification)
 *     |  |  |  |  |  |  |
 *     |  |  |  |  |  |  |____ Networking (LAN, Ad hoc, ...)
 *     |  |  |  |  |  |
 *     |  |  |  |  |  |_____ Rendering (Printing, Speaker, ...)
 *     |  |  |  |  |
 *     |  |  |  |  |______ Capturing (Scanner, Microphone, ...)
 *     |  |  |  |
 *     |  |  |  |_______ Object Transfer (v-Inbox, v-Folder, ...)
 *     |  |  |
 *     |  |  |________ Audio (Speaker, Microphone, Headset service, ...)
 *     |  |
 *     |  |________ Telephony (Cordless telephony, Modem, Headset service, ...)
 *     |
 *     |_________ Information (WEB-server, WAP-server, ...)
 *
 */
#define BT_MSC_LIMITED_DISCOVERABLE  0x00002000U
#define BT_MSC_POSITIONING           0x00010000U
#define BT_MSC_NETWORKING            0x00020000U
#define BT_MSC_RENDERING             0x00040000U
#define BT_MSC_CAPTURING             0x00080000U
#define BT_MSC_OBJECT_TRANSFER       0x00100000U
#define BT_MSC_AUDIO                 0x00200000U
#define BT_MSC_TELEPHONY             0x00400000U
#define BT_MSC_INFORMATION           0x00800000U


/*
 *   MAJOR DEVICE CLASSES:-
 *
 *     12 11 10 09 08
 *    ________________
 *    |  |  |  |  |  |
 *    |__|__|__|__|__|
 *
 *      0  0  0  0  0 -> Miscellaneous
 *
 *      0  0  0  0  1 -> Computer (desktop, notebook, PDA, organizers, ...)
 *
 *      0  0  0  1  0 -> Phone (cellular, cordless, payphone, modem, ...)
 *
 *      0  0  0  1  1 -> LAN / Network Access point
 *
 *      0  0  1  0  0 -> Audio/Video (headset, speaker, stereo, video display,
 *                                    vcr, ...)
 *      0  0  1  0  1 -> Peripheral (mouse, joystick, keyboards, ...)
 *
 *      0  0  1  1  0 -> Imaging (printing, scanner, camera, display, ...)
 *
 *      0  0  1  1  1 -> Wearable
 *
 *      0  1  0  0  0 -> Toy
 *
 *      0  1  0  0  1 -> Health
 *
 *      1  1  1  1  1 -> Uncategorized, specific device code not specified
 *
 *      X  X  X  X  X -> All other values reserved
 *
 */
#define BT_MDC_MISCELLANEOUS                 0x00000000U
#define BT_MDC_COMPUTER                      0x00000100U
#define BT_MDC_PHONE                         0x00000200U
#define BT_MDC_LAN_NETWORK_ACCESS_POINT      0x00000300U
#define BT_MDC_LAN_ACCESS_POINT              0x00000300U
#define BT_MDC_NETWORK_ACCESS_POINT          0x00000300U
#define BT_MDC_AUDIO_VIDEO                   0x00000400U
#define BT_MDC_AUDIO                         0x00000400U
#define BT_MDC_VIDEO                         0x00000400U
#define BT_MDC_PERIPHERAL                    0x00000500U
#define BT_MDC_IMAGING                       0x00000600U
#define BT_MDC_WEARABLE                      0x00000700U
#define BT_MDC_TOY                           0x00000800U
#define BT_MDC_HEALTH                        0x00000900U
#define BT_MDC_UNCLASSIFIED                  0x00001F00U


/*
 *  COMPUTER MAJOR CLASS:-
 *
 *     07 06 05 04 03 02
 *    ___________________
 *    |  |  |  |  |  |  |
 *    |__|__|__|__|__|__|
 *
 *      0  0  0  0  0  0 -> Uncategorized, code for device not assigned
 *
 *      0  0  0  0  0  1 -> Desktop workstation
 *
 *      0  0  0  0  1  0 -> Server-class computer
 *
 *      0  0  0  0  1  1 -> Laptop
 *
 *      0  0  0  1  0  0 -> Handheld PC/PDA (clam shell)
 *
 *      0  0  0  1  0  1 -> Palm sized PC/PDA
 *
 *      0  0  0  1  1  0 -> Wearable computer (Watch sized)
 *
 *      X  X  X  X  X  X -> All other values reserved
 */
#define BT_CMC_UNCLASSIFIED                0x00000000U
#define BT_CMC_DESKTOP_WORKSTATION         0x00000004U
#define BT_CMC_SERVER_CLASS_COMPUTER       0x00000008U
#define BT_CMC_LAPTOP                      0x0000000CU
#define BT_CMC_HANDHELD_PC_PDA             0x00000010U
#define BT_CMC_PALM_SIZED_PC_PDA           0x00000014U
#define BT_CMC_WEARABLE_COMPUTER           0x00000018U


/*
 *   PHONE MAJOR CLASS:-
 *
 *    07 06 05 04 03 02
 *   ___________________
 *   |  |  |  |  |  |  |
 *   |__|__|__|__|__|__|
 *
 *     0  0  0  0  0  0 -> Uncategorized, code for device not assigned
 *
 *     0  0  0  0  0  1 -> Cellular
 *
 *     0  0  0  0  1  0 -> Cordless
 *
 *     0  0  0  0  1  1 -> Smart phone
 *
 *     0  0  0  1  0  0 -> Wired modem or voice gateway
 *
 *     0  0  0  1  0  1 -> Common ISDN Access
 *
 *     X  X  X  X  X  X -> All other values reserved
 */
#define BT_PMC_UNCLASSIFIED                           0x00000000U
#define BT_PMC_CELLULAR                               0x00000004U
#define BT_PMC_CORDLESS                               0x00000008U
#define BT_PMC_SMART_PHONE                            0x0000000CU
#define BT_PMC_WIRED_MODEM_OR_VOICE_GATEWAY           0x00000010U
#define BT_PMC_WIRED_MODEM                            0x00000010U
#define BT_PMC_VOICE_GATEWAY                          0x00000010U
#define BT_PMC_COMMON_ISDN_ACCESS                     0x00000014U


/*
 *  LAN ACCESS POINT MAJOR CLASS:-
 *
 *   07 06 05
 *  __________
 *  |  |  |  |
 *  |__|__|__|
 *
 *    0  0  0 -> Fully available
 *
 *    0  0  1 -> 1 - 17% utilized
 *
 *    0  1  0 -> 17 - 33% utilized
 *
 *    0  1  1 -> 33 - 50% utilized
 *
 *    1  0  0 -> 50 - 67% utilized
 *
 *    1  0  1 -> 67 - 83% utilized
 *
 *    1  1  0 -> 83 - 99% utilized
 *
 *    1  1  1 -> No service available
 *
 *
 *   04 03 02
 *  __________
 *  |  |  |  |
 *  |__|__|__|
 *
 *    0  0  0 -> Uncategorized (use this value if no other apply)
 *
 *    X  X  X -> other values reserved
 *
 */
#define BT_LMC_FULLY_AVAILABLE                0x00000000U
#define BT_LMC_01_TO_17_PERCENT_UTILIZED      0x00000020U
#define BT_LMC_17_TO_33_PERCENT_UTILIZED      0x00000040U
#define BT_LMC_33_TO_50_PERCENT_UTILIZED      0x00000060U
#define BT_LMC_50_TO_67_PERCENT_UTILIZED      0x00000080U
#define BT_LMC_67_TO_83_PERCENT_UTILIZED      0x000000A0U
#define BT_LMC_83_TO_99_PERCENT_UTILIZED      0x000000C0U
#define BT_LMC_NO_SERVICE_AVAILABLE           0x000000E0U


/*
 *  AUDIO/VIDEO MAJOR CLASS:-
 *
 *     07 06 05 04 03 02
 *    ___________________
 *    |  |  |  |  |  |  |
 *    |__|__|__|__|__|__|
 *
 *      0  0  0  0  0  0 -> Uncategorized, code not assigned
 *
 *      0  0  0  0  0  1 -> Wearable Headset Device
 *
 *      0  0  0  0  1  0 -> Hands-free Device
 *
 *      0  0  0  0  1  1 -> (Reserved)
 *
 *      0  0  0  1  0  0 -> Microphone
 *
 *      0  0  0  1  0  1 -> Loudspeaker
 *
 *      0  0  0  1  1  0 -> Headphones
 *
 *      0  0  0  1  1  1 -> Portable Audio
 *
 *      0  0  1  0  0  0 -> Car audio
 *
 *      0  0  1  0  0  1 -> Set-top box
 *
 *      0  0  1  0  1  0 -> HiFi Audio Device
 *
 *      0  0  1  0  1  1 -> VCR
 *
 *      0  0  1  1  0  0 -> Video Camera
 *
 *      0  0  1  1  0  1 -> Camcorder
 *
 *      0  0  1  1  1  0 -> Video Monitor
 *
 *      0  0  1  1  1  1 -> Video Display and Loudspeaker
 *
 *      0  1  0  0  0  0 -> Video Conferencing
 *
 *      0  1  0  0  0  1 -> (Reserved)
 *
 *      0  1  0  0  1  0 -> Gaming/Toy
 *
 *      X  X  X  X  X  X -> All other values reserved
 *
 */
#define BT_AMC_UNCLASSIFIED                        0x00000000U
#define BT_AMC_HEADSET_PROFILE                     0x00000004U
#define BT_AMC_HANDS_FREE                          0x00000008U
#define BT_AMC_MICROPHONE                          0x00000010U
#define BT_AMC_LOUDSPEAKER                         0x00000014U
#define BT_AMC_HEADPHONE                           0x00000018U
#define BT_AMC_PORTABLE_AUDIO                      0x0000001CU
#define BT_AMC_CAR_AUDIO                           0x00000020U
#define BT_AMC_SET_TOP_BOX                         0x00000024U
#define BT_AMC_HI_FI_AUDIO_DEVICE                  0x00000028U
#define BT_AMC_VCR                                 0x0000002CU
#define BT_AMC_VIDEO_CAMERA                        0x00000030U
#define BT_AMC_CAMCORDER                           0x00000034U
#define BT_AMC_VIDEO_MONITOR                       0x00000038U
#define BT_AMC_VIDEO_DISPLAY_AND_LOUDSPEAKER       0x0000003CU
#define BT_AMC_VIDEO_CONFERENCING                  0x00000040U
#define BT_AMC_GAMING_TOY                          0x00000048U


/*
 *  PERIPHERAL MAJOR CLASS:-
 *
 *    07 06
 *   _______
 *   |  |  |
 *   |__|__|
 *
 *     0  0 -> Not Keyboard / Not Pointing Device
 *
 *     0  1 -> Keyboard
 *
 *     1  0 -> Pointing device
 *
 *     1  1 -> Combo keyboard/pointing device
 *
 *
 *
 *    05 04 03 02
 *   _____________
 *   |  |  |  |  |
 *   |__|__|__|__|
 *
 *     0  0  0  0 -> Uncategorized device
 *
 *     0  0  0  1 -> Joystick
 *
 *     0  0  1  0 -> Gamepad
 *
 *     0  0  1  1 -> Remote control
 *
 *     0  1  0  0 -> Sensing device
 *
 *     0  1  0  1 -> Digitizer tablet
 *
 *     0  1  1  0 -> Card Reader (e.g. SIM Card Reader)
 *
 *     X  X  X  X -> All other values reserved
 *
 */
#define BT_PPMC_NOT_KEYBOARD                   0x00000000U
#define BT_PPMC_NOT_POINTING_DEVICE            0x00000000U
#define BT_PPMC_KEYBOARD                       0x00000040U
#define BT_PPMC_POINTING_DEVICE                0x00000080U
#define BT_PPMC_COMBO_KEYBOARD_POINTING_DEVICE 0x000000C0U

#define BT_PPMC_JOYSTICK                       0x00000004U
#define BT_PPMC_GAMEPAD                        0x00000008U
#define BT_PPMC_REMOTE_CONTROL                 0x0000000CU
#define BT_PPMC_SENSING_DEVICE                 0x00000010U
#define BT_PPMC_DIGITIZER_TABLET               0x00000014U
#define BT_PPMC_CARD_READER                    0x00000018U


/*
 *  IMAGING MAJOR CLASS:-
 *
 *    07 06 05 04
 *   _____________
 *   |  |  |  |  |
 *   |__|__|__|__|
 *
 *     0  0  0  1 -> Display
 *
 *     0  0  1  0 -> Camera
 *
 *     0  1  0  0 -> Scanner
 *
 *     1  0  0  0 -> Printer
 *
 *     X  X  X  X -> All other values reserved
 *
 *
 *    03 02
 *   _______
 *   |  |  |
 *   |__|__|
 *
 *     0  0 -> Uncategorized, default
 *
 *     X  X -> All other values reserved
 *
 */
#define BT_IMC_DISPLAY               0x00000010U
#define BT_IMC_CAMERA                0x00000020U
#define BT_IMC_SCANNER               0x00000040U
#define BT_IMC_PRINTER               0x00000080U


/*
 * WEARABLE MAJOR CLASS:-
 *
 *     07 06 05 04 03 02
 *    ___________________
 *    |  |  |  |  |  |  |
 *    |__|__|__|__|__|__|
 *
 *      0  0  0  0  0  1 -> Wrist Watch
 *
 *      0  0  0  0  1  0 -> Pager
 *
 *      0  0  0  0  1  1 -> Jacket
 *
 *      0  0  0  1  0  0 -> Helmet
 *
 *      0  0  0  1  0  1 -> Glasses
 *
 *      X  X  X  X  X  X -> All other values reserved
 *
 */
#define BT_WMC_WRIST_WATCH           0x00000004U
#define BT_WMC_PAGER                 0x00000008U
#define BT_WMC_JACKET                0x0000000CU
#define BT_WMC_HELMET                0x00000010U
#define BT_WMC_GLASSES               0x00000014U


/*
 * TOY MAJOR CLASS:-
 *
 *     07 06 05 04 03 02
 *    ___________________
 *    |  |  |  |  |  |  |
 *    |__|__|__|__|__|__|
 *
 *      0  0  0  0  0  1 -> Robot
 *
 *      0  0  0  0  1  0 -> Vehicle
 *
 *      0  0  0  0  1  1 -> Doll / Action Figure
 *
 *      0  0  0  1  0  0 -> Controller
 *
 *      0  0  0  1  0  1 -> Game
 *
 *      X  X  X  X  X  X -> All other values reserved
 *
 */
#define BT_TMC_ROBOT           0x00000004U
#define BT_TMC_VEHICLE         0x00000008U
#define BT_TMC_DOLL            0x0000000CU
#define BT_TMC_ACTION_FIGURE   0x0000000CU
#define BT_TMC_CONTROLLER      0x00000010U
#define BT_TMC_GAME            0x00000014U


/*
 * HEALTH MAJOR CLASS:-
 *
 *     07 06 05 04 03 02
 *    ___________________
 *    |  |  |  |  |  |  |
 *    |__|__|__|__|__|__|
 *
 *      0  0  0  0  0  0 -> Undefined
 *
 *      0  0  0  0  0  1 -> Blood Pressure Monitor
 *
 *      0  0  0  0  1  0 -> Thermometer
 *
 *      0  0  0  0  1  1 -> Weighing Scale
 *
 *      0  0  0  1  0  0 -> Glucose Meter
 *
 *      0  0  0  1  0  1 -> Pulse Oximeter
 *
 *      0  0  0  1  1  0 -> Heart/Pulse Rate Monitor
 *
 *      0  0  0  1  1  1 -> Health Data Display
 *
 *      0  0  1  0  0  0 -> Step Counter
 *
 *      X  X  X  X  X  X -> All other values reserved
 *
 */
#define BT_HMC_UNDEFINED                           0x00000000U
#define BT_HMC_BLOOD_PRESSURE_MONITOR              0x00000004U
#define BT_HMC_THERMOMETER                         0x00000008U
#define BT_HMC_WEIGHING_SCALE                      0x0000000CU
#define BT_HMC_GLUCOSE_METER                       0x00000010U
#define BT_HMC_PULSE_OXIMETER                      0x00000014U
#define BT_HMC_HEART_PULSE_RATE_MONITOR            0x00000018U
#define BT_HMC_HEALTH_DATA_DISPLAY                 0x0000001CU
#define BT_HMC_STEP_COUNTER                        0x00000020U


/* =============================================  AD Type 'Flags' Definitions */
/*
 *  Flags (Bit Field):-
 *
 *  07 06 05 04 03 02 01 00
 * _________________________
 * |  |  |  |  |  |  |  |  |
 * |__|__|__|__|__|__|__|__|
 *
 *   0  0  0  0  0  0  0  1 -> LE Limited Discoverable Mode
 *
 *   0  0  0  0  0  0  1  0 -> LE General Discoverable Mode
 *
 *   0  0  0  0  0  1  0  0 -> BR/EDR Not Supported
 *                             [Bit 37 of LMP Feature Mask Definition. (Page 0)]
 *
 *   0  0  0  0  1  0  0  0 -> Simultaneous LE and BR/EDR to Same Device
 *                             Capable (Controller)
 *                             [Bit 49 of LMP Feature Mask Definition. (Page 0)]
 *
 *   0  0  0  1  0  0  0  0 -> Simultaneous LE and BR/EDR to Same Device
 *                             Capable (Host)
 *                             [Bit 66 of LMP Feature Mask Definition. (Page 1)]
 *
 *   X  X  X  X  X  X  X  X -> All other values reserved
 */
#define BT_AD_FLAGS_LE_LIMITED_DISC_MODE          0x01U
#define BT_AD_FLAGS_LE_GENERAL_DISC_MODE          0x02U
#define BT_AD_FLAGS_BR_EDR_NOT_SUPPORTED          0x04U
#define BT_AD_FLAGS_LE_BR_EDR_CONTROLLER          0x08U
#define BT_AD_FLAGS_LE_BR_EDR_HOST                0x10U


/* =============================================  Link Layer Definitions */
/*
 * The Link Layer Version parameter
 *
 * Parameter Name    Assigned Values
 * VersNr            0 - 5     Reserved
 *                   6         Bluetooth Core Specification 4.0
 *                   7 - 255   Reserved
 */
#define BT_LL_VERSION_4_0                         6


/* ============================  Link Manager Protocol (LMP) Definitions */
/*
 * The Link Manager Version parameter
 *
 * Parameter Name    Assigned Values
 * VersNr            0         Bluetooth Core Specification 1.0b
 *                   1         Bluetooth Core Specification 1.1
 *                   2         Bluetooth Core Specification 1.2
 *                   3         Bluetooth Core Specification 2.0 + EDR
 *                   4         Bluetooth Core Specification 2.1 + EDR
 *                   5         Bluetooth Core Specification 3.0 + HS
 *                   6         Bluetooth Core Specification 4.0
 *                   7         Bluetooth Core Specification 4.1
 *                   8         Bluetooth Core Specification 4.2
 *                   9         Bluetooth Core Specification 5.0
 *                  10         Bluetooth Core Specification 5.1
 *                  11         Bluetooth Core Specification 5.2
 *                  12 - 255   Reserved
 */
#define BT_LMP_VERSION_1_0_B                       0U
#define BT_LMP_VERSION_1_1                         1U
#define BT_LMP_VERSION_1_2                         2U
#define BT_LMP_VERSION_2_0_EDR                     3U
#define BT_LMP_VERSION_2_1_EDR                     4U
#define BT_LMP_VERSION_3_0_HS                      5U
#define BT_LMP_VERSION_4_0                         6U
#define BT_LMP_VERSION_4_1                         7U
#define BT_LMP_VERSION_4_2                         8U
#define BT_LMP_VERSION_5_0                         9U
#define BT_LMP_VERSION_5_1                         10U
#define BT_LMP_VERSION_5_2                         11U


/* ===============================  HCI Controller Interface Definitions */
/*
 * Parameter Name    Assigned Values
 * HCI_Version       0         Bluetooth Core Specification 1.0b
 *                   1         Bluetooth Core Specification 1.1
 *                   2         Bluetooth Core Specification 1.2
 *                   3         Bluetooth Core Specification 2.0 + EDR
 *                   4         Bluetooth Core Specification 2.1 + EDR
 *                   5         Bluetooth Core Specification 3.0 + HS
 *                   6         Bluetooth Core Specification 4.0
 *                   7         Bluetooth Core Specification 4.1
 *                   8         Bluetooth Core Specification 4.2
 *                   9         Bluetooth Core Specification 5.0
 *                  10         Bluetooth Core Specification 5.1
 *                  11         Bluetooth Core Specification 5.2
 *                  12 - 255   Reserved
 */
/* TODO: Check for better protection logic */
#ifndef BT_HCI_VERSION_1_0_B
#define BT_HCI_VERSION_1_0_B                        0U
#endif /*BT_HCI_VERSION_1_0_B*/
#ifndef BT_HCI_VERSION_1_1
#define BT_HCI_VERSION_1_1                          1U
#endif /*BT_HCI_VERSION_1_1*/
#ifndef BT_HCI_VERSION_1_2
#define BT_HCI_VERSION_1_2                          2U
#endif /*BT_HCI_VERSION_1_2*/
#ifndef BT_HCI_VERSION_2_0_EDR
#define BT_HCI_VERSION_2_0_EDR                      3U
#endif /*BT_HCI_VERSION_2_0_EDR*/
#ifndef BT_HCI_VERSION_2_1_EDR
#define BT_HCI_VERSION_2_1_EDR                      4U
#endif /*BT_HCI_VERSION_2_1_EDR*/
#ifndef BT_HCI_VERSION_3_0_HS
#define BT_HCI_VERSION_3_0_HS                       5U
#endif /*BT_HCI_VERSION_3_0_HS*/
#ifndef BT_HCI_VERSION_4_0
#define BT_HCI_VERSION_4_0                          6U
#endif /*BT_HCI_VERSION_4_0*/
#ifndef BT_HCI_VERSION_4_1
#define BT_HCI_VERSION_4_1                          7U
#endif /*BT_HCI_VERSION_4_1*/
#ifndef BT_HCI_VERSION_4_2
#define BT_HCI_VERSION_4_2                          8U
#endif /*BT_HCI_VERSION_4_2*/
#ifndef BT_HCI_VERSION_5_0
#define BT_HCI_VERSION_5_0                          9U
#endif /*BT_HCI_VERSION_5_0*/
#ifndef BT_HCI_VERSION_5_1
#define BT_HCI_VERSION_5_1                          10U
#endif /*BT_HCI_VERSION_5_1*/
#ifndef BT_HCI_VERSION_5_2
#define BT_HCI_VERSION_5_2                          11U
#endif /*BT_HCI_VERSION_5_2*/

/* ========================  Protocol Adaptation Layer (PAL) Definitions */
/*
 * The PAL Version Parameters
 *
 * Parameter Name    Assigned Values
 * PAL_Version       0         Reserved
 *                   1         Bluetooth Core Specification 3.0 + HS
 *                   2 - 255   Reserved
 */
#define BT_PAL_VERSION_3_0_HS                      1U

/* ==========================================  BD_ADDR type Definitions */
/* Types of Bluetooth device Addresses */
/**
 * The public address is the Bluetooth hardware address. The public device
 * address shall be created in accordance with section 9.2 ("48-bit universal
 * LAN MAC addresses") of the IEEE 802-2001 standard
 * (http://standards.ieee.org/getieee802/download/802-2001.pdf)
 * and using a valid Organizationally Unique Identifier (OUI) obtained from
 *  the IEEE Registration Authority
 * (http://standards.ieee.org/regauth/oui/forms/ and sections 9 and 9.1
 * of the IEEE 802-2001 specification).
 */
#define BT_BD_PUBLIC_ADDRESS_TYPE               0U

/**
 * The random device address may be of either of two type:
 *  <ol>
 *  <li> Static address:\n
 *    A device may choose to initialize its static address to a new value after
 *    each power cycle. A device shall not change its static address value once
 *    initialized until the device is power cycled.
 *    Note: If the static address of a device is changed then the address
 *    stored in peer devices will not be valid and the ability to reconnect
 *    using the old address will be lost.
 *    A static address is a 48-bit randomly generated address and shall meet
 *    the following requirements:
 *    - The two most significant bits of the static address shall be equal to
 *       1
 *    - All bits of the random part of the static address shall not be equal
 *       to 1
 *    - All bits of the random part of the static address shall not be equal
 *       to 0
 *   </li>
 *
 * <li> Private address:\n
 *    The private address may be of either of the following two sub-types:
 *      <ol type='a'>
 *       <li> Non-resolvable private address:\n
 *       To generate a non-resolvable address or a reconnection address, the
 *       Host shall generate a 48-bit address with the following requirements:
 *        - The two most significant bits of the address shall be equal to 0
 *        - All bits of the random part of the address shall not be equal
 *           to 0
 *        - All bits of the random part of the address shall not be equal to
 *           1
 *        - The address shall not be equal to the static address
 *        - The address shall not be equal to the public address
 *       </li>
 *       <li> Resolvable private address:\n
 *       To generate a resolvable private address the Host must have its
 *       Identity Resolving Key (IRK). The resolvable private address shall be
 *       generated with the IRK and a randomly generated 24-bit random number.
 *       The random number is known as prand and shall meet the following
 *       requirements:
 *        - The two most significant bits of prand shall be equal to
 *           0 and 1
 *        - All bits of the random field of prand shall not be equal to 0
 *        - All bits of the random field of prand shall not be equal to 1
 *       </li>
 *      </ol>
 *   </li>
 *   </ol>
 */
#define BT_BD_RANDOM_ADDRESS_TYPE               1U

/**
 *  Random Address Type can be
 *  - Static
 *  - Resolvable Private
 *  - Non-Resolvable Private
 *
 *  No Specific values for the above three sub-types are defined in
 *  Bluetooth Assigned Number.
 *
 *  Defining these sub-types, which can be used by the application, if required.
 *
 *  Note: These definitions can be placed in BT_hci_api.h.
 *  Keeping this non-assigned number defined values in this file
 *  - To have all possible Device Address Type defintions in a single header
 *  - In future, the assigned number might define them, if required
 *    # In that case corresponding defined values might vary
 */
#define BT_BD_STATIC_ADDRESS_TYPE              1U
#define BT_BD_RESOLVALE_PVT_ADDRESS_TYPE       2U
#define BT_BD_NON_RESOLVALE_PVT_ADDRESS_TYPE   3U

/* ==========================================  EIR Data Type Definitions */
/* Flags */
#define HCI_EIR_DATA_TYPE_FLAGS                       0x01U

/* Service Class UUIDs */
#define HCI_EIR_DATA_TYPE_INCOMPLETE_16_BIT_UUIDS     0x02U
#define HCI_EIR_DATA_TYPE_COMPLETE_16_BIT_UUIDS       0x03U
#define HCI_EIR_DATA_TYPE_INCOMPLETE_32_BIT_UUIDS     0x04U
#define HCI_EIR_DATA_TYPE_COMPLETE_32_BIT_UUIDS       0x05U
#define HCI_EIR_DATA_TYPE_INCOMPLETE_128_BIT_UUIDS    0x06U
#define HCI_EIR_DATA_TYPE_COMPLETE_128_BIT_UUIDS      0x07U

/* Local Name */
#define HCI_EIR_DATA_TYPE_SHORTENED_LOCAL_NAME        0x08U
#define HCI_EIR_DATA_TYPE_COMPLETE_LOCAL_NAME         0x09U

/* TX Power Level */
#define HCI_EIR_DATA_TYPE_TX_POWER_LEVEL              0x0AU

/* Class of Device */
#define HCI_EIR_DATA_TYPE_CLASS_OF_DEVICE             0x0DU

/* Secure Simple Pairing Out of Band (OOB) */
#define HCI_EIR_DATA_TYPE_SIMPLE_PAIRING_HASH_C       0x0EU
#define HCI_EIR_DATA_TYPE_SIMPLE_PAIRING_HASH_C_192   0x0EU
#define HCI_EIR_DATA_TYPE_SIMPLE_PAIRING_RANDOMIZER_R 0x0FU
#define HCI_EIR_DATA_TYPE_SMPL_PAIRNG_RNDMIZER_R_192  0x0FU

/* Device ID */
#define HCI_EIR_DATA_TYPE_DEVICE_ID                   0x10U

/* Security Manager */
#define HCI_EIR_DATE_TYPE_SECURITY_MANAGER_TK_VALUE   0x10U
#define HCI_EIR_DATA_TYPE_SECURITY_MANAGER_OOB_FLAGS  0x11U

/* Slave Connection Interval Range */
#define HCI_EIR_DATA_TYPE_SL_CONNECTION_INTRVL_RANGE  0x12U

/* Service Solicitation UUIDs */
#define HCI_EIR_DATA_TYPE_16_BIT_SRVC_SOLICTN_UUIDS   0x14U
#define HCI_EIR_DATA_TYPE_128_BIT_SRVC_SOLICTN_UUIDS  0x15U

/* Service Data */
#define HCI_EIR_DATA_TYPE_SERV_DATA                   0x16U
#define HCI_EIR_DATA_TYPE_SERV_DATA_16_BIT_UUIDS      0x16U

/* Advertising Data Type - Target Address */
#define HCI_AD_TYPE_PUBLIC_TARGET_ADDRESS             0x17U
#define HCI_AD_TYPE_RANDOM_TARGET_ADDRESS             0x18U

/* Advertising Data Type - Appearance */
#define HCI_AD_TYPE_APPEARANCE                        0x19U

/* Advertising Data Type - Advertising Interval */
#define HCI_AD_TYPE_ADVERTISING_INTERVAL              0x1AU

/* Advertising Data Type - LE  Bluetooth Device Address */
#define HCI_AD_TYPE_LE_BLUETOOTH_DEVICE_ADDRESS       0x1BU

/* Advertising Data Type - LE ROLE */
#define HCI_AD_TYPE_LE_ROLE                           0x1CU

/* Advertising Data Type - Simple Pairing */
#define HCI_AD_TYPE_SIMPLE_PAIRING_HASH_C_256         0x1DU
#define HCI_AD_TYPE_SIMPLE_PAIRING_RANDOMIZER_R_256   0x1EU

/* Advertising Data Type - Service Solicitation UUIDs */
#define HCI_AD_TYPE_32_BIT_SERVICE_SOLICITATION_UUIDS 0x1FU

/* Advertising Data Type - Service Data UUIDs */
#define HCI_AD_TYPE_SERVICE_DATA_32_BIT_UUIDS         0x20U
#define HCI_AD_TYPE_SERVICE_DATA_128_BIT_UUIDS        0x21U

/* Advertising Data Type - LE Secure Connections Values */
#define HCI_AD_TYPE_LE_SECURE_CONNECTIONS_CONF_VAL    0x22U
#define HCI_AD_TYPE_LE_SECURE_CONNECTIONS_RNDM_VAL    0x23U

/* Advertising Data Type - URI */
#define HCI_AD_TYPE_URI                               0x24U

/* Advertising Data Type - Indoor Positioning */
#define HCI_AD_TYPE_INDOOR_POSITIONING                0x25U

/* Advertising Data Type - Transport Discovery */
#define HCI_AD_TYPE_TRANSPORT_DISCOVERY               0x26U

/* Advertising Data Type - LE Supported Features */
#define HCI_AD_TYPE_LE_SUPPORTED_FEATURES             0x27U

/* Advertising Data Type - Channel Map Update Indication */
#define HCI_AD_TYPE_CHANNEL_MAP_UPDATE_INDICATION     0x28U

/* Advertising Data Type - PB-ADV */
#define HCI_AD_TYPE_PB_ADV                            0x29U

/* Advertising Data Type - Mesh*/
#define HCI_AD_TYPE_MESH_MESSAGE                      0x2AU
#define HCI_AD_TYPE_MESH_BEACON                       0x2BU

/* Advertising Data Type - BIGInfo */
#define HCI_AD_TYPE_BIGINFO                           0x2CU

/* Advertising Data Type - Broadcast_Code */
#define HCI_AD_TYPE_BROADCAST_CODE                    0x2DU

/* Advertising Data Type - Resolvable Set Identifier */
#define HCI_AD_TYPE_RESOLVABLE_SET_IDENTIFIER         0x2EU

/* Advertising Data Type - Advertising Interval*/
#define HCI_AD_TYPE_ADVERTISING_INTERVAL_LONG         0x2FU

/* Advertising Data Type - 3D Information Data */
#define HCI_AD_TYPE_3D_INFORMATION_DATA               0x3DU

/* Manufacturer Specific Data */
#define HCI_EIR_DATA_TYPE_MANUFACTURER_SPECIFIC_DATA  0xFFU


/* ==========================================  Device Data Specializations */
/* Pulse oximeter IEEE 11073-10404 */
#define HDP_MDEP_TYPE_PULSE_OXIMETER                  0x1004U

/* Blood pressure monitor IEEE 11073-10407 */
#define HDP_MDEP_TYPE_BP_MONITOR                      0x1007U

/* Body thermometer IEEE 11073-10408 */
#define HDP_MDEP_TYPE_THERMOMETER                     0x1008U

/* Body weight scale IEEE 11073-10415 */
#define HDP_MDEP_TYPE_WEIGHT_SCALE                    0x100FU

/* Glucose meter IEEE 11073-10417 */
#define HDP_MDEP_TYPE_GLUCOSE_METER                   0x1011U

/* Step Counter based on 10441 IEEE 11073-10441 */
/* Device specialization - Cardiovascular fitness and activity monitor */
#define HDP_MDEP_TYPE_STEP_COUNTER                    0x1068U

/* ==========================================  L2CAP Definitions */

/* L2CAP PSM Values */
#define SDP_PSM                                 0x0001U
#define RFCOMM_PSM                              0x0003U
#define TCS_BIN_PSM                             0x0005U
#define BNEP_PSM                                0x000FU
#define HID_CONTROL_PSM                         0x0011U
#define HID_INTERRUPT_PSM                       0x0013U
#define AVCTP_PSM                               0x0017U
#define AVDTP_PSM                               0x0019U
#define AVCTP_BOW_PSM                           0x001BU
#define ATT_PSM                                 0x001FU

/** L2CAP Fixed channel Connection Identifiers (CID) */
#define L2CAP_NULL_CID                          0x0000U
#define L2CAP_SIG_CID                           0x0001U
#define L2CAP_CNL_CID                           0x0002U
#define L2CAP_ATT_CID                           0x0004U
#define L2CAP_LE_CID                            0x0005U
#define L2CAP_SMP_CID                           0x0006U
#define L2CAP_SMP_BREDR_CID                     0x0007U

/** L2CAP PSM for GATT Profiles */
#define IPSP_LE_PSM                             0x0023
#define OTS_PSM                                 0x0025

/** L2CAP PSM for Enhanced ATT Bearer */
#define EATT_PSM                                0x0027

#endif /* _H_BT_ASSIGNED_NUMBERS_ */

