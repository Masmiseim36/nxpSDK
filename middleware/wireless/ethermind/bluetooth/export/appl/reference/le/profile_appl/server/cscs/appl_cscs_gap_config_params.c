
/**
 *  \file appl_cscs_gap_config_params.c
 *
 *  This file contains GAP Configuration Parameters used by the application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_service.h"

#if (defined ATT && defined CSCS)

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
const APPL_GAP_ADV_DATA appl_gap_adv_data =
{
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
         *      DeviceInformation Service (0x180A)
         *      Battery Service (0x180F)
         *      CSC Service (0x1816)
         */
        0x07U, 0x03U, 0x0AU, 0x18U, 0x0FU, 0x18U, 0x16U, 0x18U,

        /**
         *  Shortened Device Name: Mt-CSCS
         */
        0x08U, 0x08U, 'M', 't', '-', 'C', 'S', 'C', 'S',

        /**
         * Appearance: Cycling: Speed and Cadence Sensor
         */
        0x03U, 0x19U, 0x85U, 0x04U,

        /* MISRA C-2012 Rule 9.3 */
        0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U
    },

    /** Advertising Data length */
    24U
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
        1600U,

        4000U,

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
#endif /* (defined ATT && defined CSCS) */
