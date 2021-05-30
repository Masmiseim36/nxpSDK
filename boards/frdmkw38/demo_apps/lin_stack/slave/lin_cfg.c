/*
 * Copyright 2019-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "lin.h"

static const l_u8 LI0_lin_diag_services_supported[LI0_DIAG_NUMBER_OF_SERVICES] = {0xB0, 0xB2, 0xB3, 0xB6, 0xB7};
static l_u8 LI0_lin_diag_services_flag[LI0_DIAG_NUMBER_OF_SERVICES]            = {0, 0, 0, 0, 0};

static lin_tl_pdu_data_t LI0_tl_tx_queue_data[2];
static lin_tl_pdu_data_t LI0_tl_rx_queue_data[2];

const l_ifc_handle g_lin_hardware_ifc[HARDWARE_INSTANCE_COUNT] = {INVALID_IFC, LI0};
const l_u32 g_lin_virtual_ifc[LIN_NUM_OF_IFCS]                 = {1};
volatile l_u8 g_buffer_backup_data[8];

/* Frame data buffer of signal array. */
volatile l_u8 g_lin_frame_data_buffer[LIN_FRAME_BUF_SIZE] = {
    0xfc, /* 0 : 11111100, Frame LI0_Motor1Control. */
    0x00, /* 1 : 00000101, Frame LI0_Motor1State_Cycl. */
    0x00, /* 2 : 00000000 */
    0x00, /* 3 : 00000000 */
    0x00, /* 4 : 00000000 */
    0x00, /* 5 : 00000000 */
    0xfe, /* 6 : 11111110 */
    0xff, /* 7 : 11111111, Frame LI0_Motor1State_Event. */
    0x05, /* 8 : 00000101 */
    0x01, /* 9 : 00000001 */
    0x00, /* 10 : 00000000, Frame LI0_MotorsControl. */
    0xf0, /* 11 : 11110000 */
    0xff, /* 12 : 11111111 */
    0xff, /* 13 : 11111111 */
    0xff, /* 14 : 11111111 */
    0xff, /* 15 : 11111111 */
    0xff, /* 16 : 11111111 */
    0xff, /* 17 : 11111111 */
};

/* Flag handle table of signal array */
volatile l_u8 g_lin_flag_handle_tbl[LIN_FLAG_BUF_SIZE] = {
    0x00, /* 0: start of flag frame LI0_Motor1Control */
    0x00, /* 1: start of flag frame LI0_Motor1State_Cycl */
    0x00, /* 2: start of flag frame LI0_Motor1State_Event */
    0x00  /* 3: start of flag frame LI0_MotorsControl */
};

/* Frame flag table. */
volatile l_bool g_lin_frame_flag_handle_tbl[LIN_NUM_OF_FRMS] = {0, 0, 0, 0, 0, 0, 0};

/* Frame flag for updating signal in frame. */
volatile l_u8 g_lin_frame_updating_flag_tbl[LIN_NUM_OF_FRMS] = {0, 0, 0, 0, 0, 0, 0};

/*!
 * lin configuration structure
 */
lin_user_config_t lin1_InitConfig0 = {
    .baudRate                     = 19200,                         /* Baud rate. */
    .nodeFunction                 = (bool)LIN_SLAVE,               /* Node funcrion: master or slave. */
    .autobaudEnable               = true,                          /* Enable auto baud, slave node only. */
    .timerGetTimeIntervalCallback = timerGetTimeIntervalCallback0, /* Call back fucntion for timer service. */
};

/* LIN frame table. */
static const lin_frame_t lin_frame_tbl[LIN_NUM_OF_FRMS] = {

    {LIN_FRM_UNCD, 1, LIN_RES_SUB, 0, 0, 1, (lin_associate_frame_t *)0},
    {LIN_FRM_UNCD, 6, LIN_RES_PUB, 1, 1, 1, (lin_associate_frame_t *)0},
    {LIN_FRM_UNCD, 3, LIN_RES_PUB, 7, 2, 1, (lin_associate_frame_t *)0},
    {LIN_FRM_UNCD, 8, LIN_RES_SUB, 10, 3, 1, (lin_associate_frame_t *)0},
    {LIN_FRM_SPRDC, 1, LIN_RES_SUB, 0, 0, 0, (lin_associate_frame_t *)0},
    {LIN_FRM_DIAG, 8, LIN_RES_SUB, 0, 0, 0, (lin_associate_frame_t *)0},
    {LIN_FRM_DIAG, 8, LIN_RES_PUB, 0, 0, 0, (lin_associate_frame_t *)0}};

/*  LIN configuration stored in RAM. */
static l_u8 LI0_lin_configuration_RAM[LI0_LIN_SIZE_OF_CFG] = {0x00, 0x30, 0x33, 0x36, 0x2D, 0xFF, 0x3C, 0x3D, 0xFF};
/* LIN configuration stored in ROM, can't be changed. */
static const l_u16 LI0_lin_configuration_ROM[LI0_LIN_SIZE_OF_CFG] = {0x0000, 0x30, 0x33, 0x36,  0x2D,
                                                                     0xFF,   0x3C, 0x3D, 0xFFFF};

/* Related LIN configurations. */
static l_u8 LI0_lin_configured_NAD                = 0x02;
static l_u8 LI0_lin_frm_err_resp_sig[1]           = {0x33};
static l_u8 LI0_lin_response_error_byte_offset[1] = {LIN_LI0_Motor1LinError_BYTE_OFFSET};
static l_u8 LI0_lin_response_error_bit_offset[1]  = {LIN_LI0_Motor1LinError_BIT_OFFSET};

/* Node attributes Initialization. */
const lin_node_attribute_t g_lin_node_attribute_array[LIN_NUM_OF_SLAVE_IFCS] = {
    /** LI0 **/
    {.configured_NAD_ptr             = &LI0_lin_configured_NAD, /*configured_NAD*/
     .initial_NAD                    = 0x0A,                    /*initial_NAD*/
     .serial_number                  = {0x1, 0x1, 0x1, 0x1},
     .product_id                     = {0x001E, 0x0001, 0x00},             /*{<supplier_id>,<function_id>,<variant>}*/
     .resp_err_frm_id_ptr            = LI0_lin_frm_err_resp_sig,           /*list index of frame error*/
     .num_frame_have_esignal         = 1,                                  /*number of frame contain error signal*/
     .response_error                 = LI0_Motor1LinError,                 /*<interface_name>_< response_error>*/
     .response_error_byte_offset_ptr = LI0_lin_response_error_byte_offset, /*<interface_name>_< response_error>*/
     .response_error_bit_offset_ptr  = LI0_lin_response_error_bit_offset,  /*<interface_name>_< response_error>*/

     .P2_min       = 100,  /*<P2_min>*/
     .ST_min       = 20,   /*<ST_min>*/
     .N_As_timeout = 2000, /*< N_As_timeout >*/
     .N_Cr_timeout = 2000, /*<N_Cr_timeout >*/

     .number_support_sid    = LI0_DIAG_NUMBER_OF_SERVICES,
     .service_supported_ptr = LI0_lin_diag_services_supported,
     .service_flags_ptr     = LI0_lin_diag_services_flag}};

/* LIN interface configuration. */
const lin_protocol_user_config_t g_lin_protocol_user_cfg_array[LIN_NUM_OF_IFCS] = {

    /* Interface_name = LI0 */
    {
        .protocol_version = LIN_PROTOCOL_21,        /*lin_protocol_version */
        .language_version = LIN_PROTOCOL_21,        /*lin_language_version */
        .diagnostic_class = LIN_DIAGNOSTIC_CLASS_I, /* LIN Diagnostic Class */
        .function         = (bool)LIN_SLAVE,        /*  function LIN_SLAVE*/

        .number_of_configurable_frames = 7,             /*  num_of_frames */
        .frame_start                   = 0,             /*  frame_start */
        .frame_tbl_ptr                 = lin_frame_tbl, /*  frame_tbl */

        .list_identifiers_ROM_ptr = LI0_lin_configuration_ROM, /*  *configuration_ROM */
        .list_identifiers_RAM_ptr = LI0_lin_configuration_RAM, /*  *configuration_RAM */
        .max_idle_timeout_cnt     = 10000,                     /* Max Idle Timeout Count */
        .num_of_schedules         = 0,                         /*  num_of_schedules */
        .schedule_start           = 0,                         /*  schedule_start */
        .schedule_tbl             = (const lin_schedule_t *)0, /* schedule_tbl */

        .slave_ifc_handle    = LI0_Slave,
        .master_ifc_handle   = INVALID_MASTER_INDEX,
        .lin_user_config_ptr = &lin1_InitConfig0,
#if (SUPPORT_TRANSPORT_LAYER == 1U)
        .tl_tx_queue_data_ptr = LI0_tl_tx_queue_data,
        .tl_rx_queue_data_ptr = LI0_tl_rx_queue_data,
        .max_message_length   = 6 /* Max message length */
#endif                            /* end (SUPPORT_TRANSPORT_LAYER == 1U) */
    }};

/*!
 * @brief ld_read_by_id_callout
 *
 * This function is an example of response; real implementation is application-dependent
 * You can use one of the following define to set PCI of response frame for
 * this service to the correct value
 * @param   iii   Interface handle instance.
 * @param   id    Value of id.
 * @param   data  Data to be transferred
 * @return  Identification Value.
 */
l_u8 ld_read_by_id_callout(l_ifc_handle iii, l_u8 id, l_u8 *data)
{
    l_u8 retval = LD_NEGATIVE_RESPONSE;

    /* Get the current configuration */
    const lin_protocol_user_config_t *prot_user_config_ptr = &g_lin_protocol_user_cfg_array[iii];

    if (prot_user_config_ptr->function == (bool)LIN_SLAVE)
    {
        /* Following code is an example - Real implementation is application-dependent,
         * the handling does essentially depend on the id of the requested item.
         * This example implement with ID = 32 - LIN_READ_USR_DEF_MIN.
         */
        if (id == LIN_READ_USR_DEF_MIN)
        {
            /* A positive response is ready to be sent to the user defined request */
            data[0] = (l_u8)(id + 1U);
            data[1] = (l_u8)(id + 2U);
            data[2] = (l_u8)(id + 3U);
            data[3] = (l_u8)(id + 4U);
            data[4] = (l_u8)(id + 5U);
            retval  = LD_POSITIVE_RESPONSE;
        }
        else
        {
            /*
             * User code.
             */
        }
    }

    return retval;
}
