
/**
 *  \file appl_pts_gap_config_params.c
 *
 *  This file contains GAP Configuration Parameters used by the application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_service.h"

#if (defined ATT && defined PTS)

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Static Global Variables */

/* --------------------------------------------- Exported Global Variables */
/** Advertising type to be used during Advertising */
UCHAR advertising_type;

/** Own Address type during advertising */
UCHAR own_addr_type;

/** Direct Address type for Advertising */
UCHAR peer_addr_type;

/** Direct Address for Advertising */
UCHAR peer_addr[BT_BD_ADDR_SIZE];

#if ((defined APPL_GAP_BROACASTER) || defined (APPL_GAP_PERIPHERAL))
/** Advertisement Data */
APPL_GAP_ADV_DATA appl_gap_adv_data =
{
    /* GAP Advertisement Parameters */
    {
        /**
         *  Flags:
         *      0x01: LE Limited Discoverable Mode
         *      0x02: LE General Discoverable Mode
         *      0x04: BR/EDR Not Supported
         *      0x08: Simultaneous LE and BR/EDR to Same Device
         *            Capable (Controller)
         *      0x10: Simultaneous LE and BR/EDR to Same Device
         *            Capable (Host)
         */
        0x02U, 0x01U,
        (BT_AD_FLAGS_LE_GENERAL_DISC_MODE | BT_AD_FLAGS_LE_BR_EDR_HOST),

        /**
         *  Service UUID List:
         *      Battery Service (0x180F)
         *      Dummy Service 1 Service (0xED00)
         *      Dummy Service 2 Service (0xEE00)
         *      Dummy Service 3 Service (0xEF00)
         *      Dummy Service 4 Service (0xEA00)
         *      Dummy Service 5 Service (0xEB00)
         *      Dummy Service 6 Service (0xDEAD)
         *      Dummy Service 7 Service (0xEC00)
         */
        0x11U,
        0x03U,
        0x0FU, 0x18U,
        0x00U, 0xEDU,
        0x00U, 0xEEU,
        0x00U, 0xEFU,
        0x00U, 0xEAU,
        0x00U, 0xEBU,
        0xADU, 0xDEU,
        0x00U, 0xECU,

        /**
         *  Shortened Device Name: Mt-PTS
         */
        0x07U, 0x08U, 'M', 't', '-', 'P', 'T', 'S',

        /* MISRA C-2012 Rule 9.3 */
        0x00U, 0x00U
    },

    29
};

/** Advertisement parameters options */
const APPL_GAP_ADV_PARAM appl_gap_adv_param[APPL_GAP_PROC_TYPES] =
{
    /* 0 - Fast Connection Advertising Params */
    {
        32U,

        48U,

        7U
    },

    /* 1 - Normal Advertising Params */
    {
        32U,

        32U,

        7U
    },

    /* 2 - Low Power Advertising Params */
    {
        0x00A0U,

        0x00A0U,

        7U
    }
};
#endif /* ((defined APPL_GAP_BROACASTER) || defined (APPL_GAP_PERIPHERAL)) */

#if ((defined APPL_GAP_OBSERVER) || (defined APPL_GAP_CENTRAL))
/* Scan Parameters Option */
const APPL_GAP_SCAN_PARAM appl_gap_scan_param[APPL_GAP_PROC_TYPES] =
{
    /* 0 - Fast Connection Scan Params */
    {
        48U,

        7U
    },

    /* 1 - Normal Scan Params */
    {
        32U,

        7U
    },

    /* 2 - Low Power Scan Params */
    {
        4000U,

        7U
    }
};
#endif /* ((defined APPL_GAP_OBSERVER) || (defined APPL_GAP_CENTRAL)) */

#ifdef APPL_GAP_CENTRAL
/* Connection Parameters Options */
const APPL_GAP_CONN_PARAM appl_gap_conn_param =
{
    4U,

    4U,

    40U,

    56U,

    0U,

    955U,

    32U,

    32U
};
#endif /* APPL_GAP_CENTRAL */
#endif /* (defined ATT && defined PTS) */
