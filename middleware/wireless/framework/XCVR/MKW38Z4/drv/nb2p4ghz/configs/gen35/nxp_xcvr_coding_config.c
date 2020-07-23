/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "nxp_xcvr_coding_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 *******************************************************************************/
const xcvr_coding_config_t xcvr_ble_coded_s2_config =
{
    /***********************************************/
    /*********** START OF GENERATED CODE ***********/
    /*********** xcvr_ble_coded_s2_config **********/
    /***********************************************/
    .coding_mode = BLE_CODED_S2,
    
    /* RBME configs */
    /****************/

    /* CRCW_CFG configuration, dependencies: [None, None] */
    .crcw_cfg = RBME_CRCW_CFG_CRCW_EC_EN(0)|
                RBME_CRCW_CFG_CRCW_EN(1),

    /* CRCW_CFG2 configuration, dependencies: [None, None, None] */
    .crcw_cfg2 = RBME_CRCW_CFG2_CRC_EC_LPKT_WND(0x00)|
                RBME_CRCW_CFG2_CRC_EC_SPKT_BYTES(0x00)|
                RBME_CRCW_CFG2_CRC_EC_SPKT_WND(0x00),

    /* CRCW_CFG3 configuration, dependencies: [None, None, None, None, None] */
    .crcw_cfg3 = RBME_CRCW_CFG3_CRC_BYTE_ORD(0)|
                RBME_CRCW_CFG3_CRC_REF_IN(0)|
                RBME_CRCW_CFG3_CRC_REF_OUT(0)|
                RBME_CRCW_CFG3_CRC_START_BYTE(0x04)|
                RBME_CRCW_CFG3_CRC_SZ(0x03),

    /* CRC_INIT configuration, dependencies: [None] */
    .crc_init = RBME_CRC_INIT_CRC_SEED(0x55555500U),

    /* CRC_PHR_SZ configuration, dependencies: [None] */
    .crc_phr_sz = RBME_CRC_PHR_SZ_PHR_SZ(2),

    /* CRC_POLY configuration, dependencies: [None] */
    .crc_poly = RBME_CRC_POLY_CRC_POLY(0x00065B00),

    /* CRC_XOR_OUT configuration, dependencies: [None] */
    .crc_xor_out = RBME_CRC_XOR_OUT_CRC_XOR_OUT(0x00000000),

    /* FCP_CFG configuration, dependencies: [None] */
    .fcp_cfg = RBME_FCP_CFG_FCP_SUPPORT(0),

    /* FEC_BSZ_OV_B4SP configuration, dependencies: [None, None] */
    .fec_bsz_ov_b4sp = RBME_FEC_BSZ_OV_B4SP_FEC_BSZ_OV(0x0000)|
                RBME_FEC_BSZ_OV_B4SP_FEC_BSZ_OV_B4SP_EN(0),

    /* FEC_CFG1 configuration, dependencies: [None, None, None, None, None] */
    .fec_cfg1 = RBME_FEC_CFG1_FECOV_EN(1)|
                RBME_FEC_CFG1_FEC_EN(0)|
                RBME_FEC_CFG1_FEC_SWAP(0)|
                RBME_FEC_CFG1_INTV_EN(0)|
                RBME_FEC_CFG1_NTERM(3),

    /* FEC_CFG2 configuration, dependencies: [None, None, None, None] */
    .fec_cfg2 = RBME_FEC_CFG2_LARGE_VL(0x7F)|
                RBME_FEC_CFG2_SAT_VL(0x7F)|
                RBME_FEC_CFG2_SDIDX(0)|
                RBME_FEC_CFG2_TB_LENGTH(0x0F),

    /* FRAME_OVER_SZ configuration, dependencies: [None, None] */
    .frame_over_sz = RBME_FRAME_OVER_SZ_STD_FRM_OV(0x0000)|
                RBME_FRAME_OVER_SZ_STD_FRM_OV_EN(0),

    /* LEG0_CFG configuration, dependencies: [None, None, None, None, None] */
    .leg0_cfg = RBME_LEG0_CFG_LEG0_INV_EN(0)|
                RBME_LEG0_CFG_LEG0_SUP(0)|
                RBME_LEG0_CFG_LEG0_XOR_BYTE(0x00)|
                RBME_LEG0_CFG_LEG0_XOR_FST_BYTE(0x00)|
                RBME_LEG0_CFG_LEG0_XOR_RP_BYTE(0x00),

    /* NPAYL_OVER_SZ configuration, dependencies: [None, None, None] */
    .npayl_over_sz = RBME_NPAYL_OVER_SZ_FT_FEC_FLUSH(0x00)|
                RBME_NPAYL_OVER_SZ_NPAYL_OV(0x0000)|
                RBME_NPAYL_OVER_SZ_NPAYL_OV_EN(0),

    /* PKT_SZ configuration, dependencies: [None, None] */
    .pkt_sz = RBME_PKT_SZ_DEF_PKT_SZ(0x0024)|
                RBME_PKT_SZ_MAX_PKT_SZ(0x4000),

    /* RAM_D_ADDR configuration, dependencies: [None] */
    .ram_d_addr = RBME_RAM_D_ADDR_RAM_D_ADDR(0x00000000),

    /* RAM_IF_CFG configuration, dependencies: [None, None, None, None, None, None, None] */
    .ram_if_cfg = RBME_RAM_IF_CFG_H2S_EN(0)|
                RBME_RAM_IF_CFG_RAM_IF_IC(0)|
                RBME_RAM_IF_CFG_RAM_IF_IE(0)|
                RBME_RAM_IF_CFG_RAM_IF_RX_EN(0)|
                RBME_RAM_IF_CFG_RAM_IF_TX_EN(0)|
                RBME_RAM_IF_CFG_SOFT_HD_SEL_RD(0)|
                RBME_RAM_IF_CFG_SOFT_HD_SEL_WR(0),

    /* RAM_S_ADDR configuration, dependencies: [None] */
    .ram_s_addr = RBME_RAM_S_ADDR_RAM_S_ADDR(0x00000000),

    /* RBME_RST configuration, dependencies: [None] */
    .rbme_rst = RBME_RBME_RST_RBME_RST(0),

    /* SPREAD_CFG configuration, dependencies: [None, None, None, None, None] */
    .spread_cfg = RBME_SPREAD_CFG_CI_TX(1)|
                RBME_SPREAD_CFG_SPOV_EN(0)|
                RBME_SPREAD_CFG_SP_EN(0)|
                RBME_SPREAD_CFG_SP_FACTOR(0x02)|
                RBME_SPREAD_CFG_SP_SEQ(0x000C),

    /* WHITEN_CFG configuration, dependencies: [None, None, None, None, None, None, None, None] */
    .whiten_cfg = RBME_WHITEN_CFG_WHITEN_B4_CRC(0)|
                RBME_WHITEN_CFG_WHITEN_END(1)|
                RBME_WHITEN_CFG_WHITEN_INIT(0x0041)|
                RBME_WHITEN_CFG_WHITEN_PAYLOAD_REINIT(0)|
                RBME_WHITEN_CFG_WHITEN_POLY_TYPE(0)|
                RBME_WHITEN_CFG_WHITEN_REF_IN(0)|
                RBME_WHITEN_CFG_WHITEN_SIZE(0x07)|
                RBME_WHITEN_CFG_WHITEN_START(1),

    /* WHITEN_POLY configuration, dependencies: [None] */
    .whiten_poly = RBME_WHITEN_POLY_WHITEN_POLY(0x0004),

    /* WHITEN_SZ_THR configuration, dependencies: [None, None, None] */
    .whiten_sz_thr = RBME_WHITEN_SZ_THR_WHITEN_SZ_THR(0x0000),

    /* WHT_CFG configuration, dependencies: [None, None, None, None, None] */
    .wht_cfg = RBME_WHT_CFG_W1_EN(0)|
                RBME_WHT_CFG_WFIRST(0)|
                RBME_WHT_CFG_WTOV_EN(0)|
                RBME_WHT_CFG_WT_OUT_SEL(0)|
                RBME_WHT_CFG_WT_TPOGY(0),
    
    /* XCVR_2P4GHZ_PHY configs */
    /***************************/

    /* DMD_CTRL1 configuration, dependencies: [None, None, None, None, None, None] */
    .dmd_ctrl1 = XCVR_2P4GHZ_PHY_DMD_CTRL1_FED_IDLE_WIN(0x0008)|
                XCVR_2P4GHZ_PHY_DMD_CTRL1_FED_IMM_MEAS_EN(1)|
                XCVR_2P4GHZ_PHY_DMD_CTRL1_TED_ERR_SCALE(0x04)|
                XCVR_2P4GHZ_PHY_DMD_CTRL1_TED_IDLE_WIN(0x0004)|
                XCVR_2P4GHZ_PHY_DMD_CTRL1_TED_IMM_MEAS_EN(1)|
                XCVR_2P4GHZ_PHY_DMD_CTRL1_TTRK_INT_RANGE(0x03),
    /***********************************************/
    /************ END OF GENERATED CODE ************/
    /*********** xcvr_ble_coded_s2_config **********/
    /***********************************************/
};

const xcvr_coding_config_t xcvr_ble_coded_s8_config =
{
    /***********************************************/
    /*********** START OF GENERATED CODE ***********/
    /*********** xcvr_ble_coded_s8_config **********/
    /***********************************************/
    .coding_mode = BLE_CODED_S8,
    
    /* RBME configs */
    /****************/

    /* CRCW_CFG configuration, dependencies: [None, None] */
    .crcw_cfg = RBME_CRCW_CFG_CRCW_EC_EN(0)|
                RBME_CRCW_CFG_CRCW_EN(1),

    /* CRCW_CFG2 configuration, dependencies: [None, None, None] */
    .crcw_cfg2 = RBME_CRCW_CFG2_CRC_EC_LPKT_WND(0x00)|
                RBME_CRCW_CFG2_CRC_EC_SPKT_BYTES(0x00)|
                RBME_CRCW_CFG2_CRC_EC_SPKT_WND(0x00),

    /* CRCW_CFG3 configuration, dependencies: [None, None, None, None, None] */
    .crcw_cfg3 = RBME_CRCW_CFG3_CRC_BYTE_ORD(0)|
                RBME_CRCW_CFG3_CRC_REF_IN(0)|
                RBME_CRCW_CFG3_CRC_REF_OUT(0)|
                RBME_CRCW_CFG3_CRC_START_BYTE(0x04)|
                RBME_CRCW_CFG3_CRC_SZ(0x03),

    /* CRC_INIT configuration, dependencies: [None] */
    .crc_init = RBME_CRC_INIT_CRC_SEED(0x55555500U),

    /* CRC_PHR_SZ configuration, dependencies: [None] */
    .crc_phr_sz = RBME_CRC_PHR_SZ_PHR_SZ(2),

    /* CRC_POLY configuration, dependencies: [None] */
    .crc_poly = RBME_CRC_POLY_CRC_POLY(0x00065B00),

    /* CRC_XOR_OUT configuration, dependencies: [None] */
    .crc_xor_out = RBME_CRC_XOR_OUT_CRC_XOR_OUT(0x00000000),

    /* FCP_CFG configuration, dependencies: [None] */
    .fcp_cfg = RBME_FCP_CFG_FCP_SUPPORT(0),

    /* FEC_BSZ_OV_B4SP configuration, dependencies: [None, None] */
    .fec_bsz_ov_b4sp = RBME_FEC_BSZ_OV_B4SP_FEC_BSZ_OV(0x0000)|
                RBME_FEC_BSZ_OV_B4SP_FEC_BSZ_OV_B4SP_EN(0),

    /* FEC_CFG1 configuration, dependencies: [None, None, None, None, None] */
    .fec_cfg1 = RBME_FEC_CFG1_FECOV_EN(1)|
                RBME_FEC_CFG1_FEC_EN(0)|
                RBME_FEC_CFG1_FEC_SWAP(0)|
                RBME_FEC_CFG1_INTV_EN(0)|
                RBME_FEC_CFG1_NTERM(3),

    /* FEC_CFG2 configuration, dependencies: [None, None, None, None] */
    .fec_cfg2 = RBME_FEC_CFG2_LARGE_VL(0x7F)|
                RBME_FEC_CFG2_SAT_VL(0x7F)|
                RBME_FEC_CFG2_SDIDX(0)|
                RBME_FEC_CFG2_TB_LENGTH(0x0F),

    /* FRAME_OVER_SZ configuration, dependencies: [None, None] */
    .frame_over_sz = RBME_FRAME_OVER_SZ_STD_FRM_OV(0x0000)|
                RBME_FRAME_OVER_SZ_STD_FRM_OV_EN(0),

    /* LEG0_CFG configuration, dependencies: [None, None, None, None, None] */
    .leg0_cfg = RBME_LEG0_CFG_LEG0_INV_EN(0)|
                RBME_LEG0_CFG_LEG0_SUP(0)|
                RBME_LEG0_CFG_LEG0_XOR_BYTE(0x00)|
                RBME_LEG0_CFG_LEG0_XOR_FST_BYTE(0x00)|
                RBME_LEG0_CFG_LEG0_XOR_RP_BYTE(0x00),

    /* NPAYL_OVER_SZ configuration, dependencies: [None, None, None] */
    .npayl_over_sz = RBME_NPAYL_OVER_SZ_FT_FEC_FLUSH(0x00)|
                RBME_NPAYL_OVER_SZ_NPAYL_OV(0x0000)|
                RBME_NPAYL_OVER_SZ_NPAYL_OV_EN(0),

    /* PKT_SZ configuration, dependencies: [None, None] */
    .pkt_sz = RBME_PKT_SZ_DEF_PKT_SZ(0x0024)|
                RBME_PKT_SZ_MAX_PKT_SZ(0x4000),

    /* RAM_D_ADDR configuration, dependencies: [None] */
    .ram_d_addr = RBME_RAM_D_ADDR_RAM_D_ADDR(0x00000000),

    /* RAM_IF_CFG configuration, dependencies: [None, None, None, None, None, None, None] */
    .ram_if_cfg = RBME_RAM_IF_CFG_H2S_EN(0)|
                RBME_RAM_IF_CFG_RAM_IF_IC(0)|
                RBME_RAM_IF_CFG_RAM_IF_IE(0)|
                RBME_RAM_IF_CFG_RAM_IF_RX_EN(0)|
                RBME_RAM_IF_CFG_RAM_IF_TX_EN(0)|
                RBME_RAM_IF_CFG_SOFT_HD_SEL_RD(0)|
                RBME_RAM_IF_CFG_SOFT_HD_SEL_WR(0),

    /* RAM_S_ADDR configuration, dependencies: [None] */
    .ram_s_addr = RBME_RAM_S_ADDR_RAM_S_ADDR(0x00000000),

    /* RBME_RST configuration, dependencies: [None] */
    .rbme_rst = RBME_RBME_RST_RBME_RST(0),

    /* SPREAD_CFG configuration, dependencies: [None, None, None, None, None] */
    .spread_cfg = RBME_SPREAD_CFG_CI_TX(0)|
                RBME_SPREAD_CFG_SPOV_EN(1)|
                RBME_SPREAD_CFG_SP_EN(0)|
                RBME_SPREAD_CFG_SP_FACTOR(0x02)|
                RBME_SPREAD_CFG_SP_SEQ(0x000C),

    /* WHITEN_CFG configuration, dependencies: [None, None, None, None, None, None, None, None] */
    .whiten_cfg = RBME_WHITEN_CFG_WHITEN_B4_CRC(0)|
                RBME_WHITEN_CFG_WHITEN_END(1)|
                RBME_WHITEN_CFG_WHITEN_INIT(0x0041)|
                RBME_WHITEN_CFG_WHITEN_PAYLOAD_REINIT(0)|
                RBME_WHITEN_CFG_WHITEN_POLY_TYPE(0)|
                RBME_WHITEN_CFG_WHITEN_REF_IN(0)|
                RBME_WHITEN_CFG_WHITEN_SIZE(0x07)|
                RBME_WHITEN_CFG_WHITEN_START(1),

    /* WHITEN_POLY configuration, dependencies: [None] */
    .whiten_poly = RBME_WHITEN_POLY_WHITEN_POLY(0x0004),

    /* WHITEN_SZ_THR configuration, dependencies: [None, None, None] */
    .whiten_sz_thr = RBME_WHITEN_SZ_THR_WHITEN_SZ_THR(0x0000),

    /* WHT_CFG configuration, dependencies: [None, None, None, None, None] */
    .wht_cfg = RBME_WHT_CFG_W1_EN(0)|
                RBME_WHT_CFG_WFIRST(0)|
                RBME_WHT_CFG_WTOV_EN(0)|
                RBME_WHT_CFG_WT_OUT_SEL(0)|
                RBME_WHT_CFG_WT_TPOGY(0),
    
    /* XCVR_2P4GHZ_PHY configs */
    /***************************/

    /* DMD_CTRL1 configuration, dependencies: [None, None, None, None, None, None] */
    .dmd_ctrl1 = XCVR_2P4GHZ_PHY_DMD_CTRL1_FED_IDLE_WIN(0x0008)|
                XCVR_2P4GHZ_PHY_DMD_CTRL1_FED_IMM_MEAS_EN(1)|
                XCVR_2P4GHZ_PHY_DMD_CTRL1_TED_ERR_SCALE(0x04)|
                XCVR_2P4GHZ_PHY_DMD_CTRL1_TED_IDLE_WIN(0x0004)|
                XCVR_2P4GHZ_PHY_DMD_CTRL1_TED_IMM_MEAS_EN(1)|
                XCVR_2P4GHZ_PHY_DMD_CTRL1_TTRK_INT_RANGE(0x03),
    /***********************************************/
    /************ END OF GENERATED CODE ************/
    /*********** xcvr_ble_coded_s8_config **********/
    /***********************************************/
};

const xcvr_coding_config_t xcvr_ble_uncoded_config =
{
    /***********************************************/
    /*********** START OF GENERATED CODE ***********/
    /*********** xcvr_ble_uncoded_config ***********/
    /***********************************************/
    .coding_mode = BLE_UNCODED,
    
    /* RBME configs */
    /****************/

    /* CRCW_CFG configuration, dependencies: [None, None] */
    .crcw_cfg = RBME_CRCW_CFG_CRCW_EC_EN(0)|
                RBME_CRCW_CFG_CRCW_EN(1),

    /* CRCW_CFG2 configuration, dependencies: [None, None, None] */
    .crcw_cfg2 = RBME_CRCW_CFG2_CRC_EC_LPKT_WND(0x00)|
                RBME_CRCW_CFG2_CRC_EC_SPKT_BYTES(0x00)|
                RBME_CRCW_CFG2_CRC_EC_SPKT_WND(0x00),

    /* CRCW_CFG3 configuration, dependencies: [None, None, None, None, None] */
    .crcw_cfg3 = RBME_CRCW_CFG3_CRC_BYTE_ORD(0)|
                RBME_CRCW_CFG3_CRC_REF_IN(0)|
                RBME_CRCW_CFG3_CRC_REF_OUT(0)|
                RBME_CRCW_CFG3_CRC_START_BYTE(0x04)|
                RBME_CRCW_CFG3_CRC_SZ(0x03),

    /* CRC_INIT configuration, dependencies: [None] */
    .crc_init = RBME_CRC_INIT_CRC_SEED(0x55555500U),

    /* CRC_PHR_SZ configuration, dependencies: [None] */
    .crc_phr_sz = RBME_CRC_PHR_SZ_PHR_SZ(2),

    /* CRC_POLY configuration, dependencies: [None] */
    .crc_poly = RBME_CRC_POLY_CRC_POLY(0x00065B00),

    /* CRC_XOR_OUT configuration, dependencies: [None] */
    .crc_xor_out = RBME_CRC_XOR_OUT_CRC_XOR_OUT(0x00000000),

    /* FCP_CFG configuration, dependencies: [None] */
    .fcp_cfg = RBME_FCP_CFG_FCP_SUPPORT(0),

    /* FEC_BSZ_OV_B4SP configuration, dependencies: [None, None] */
    .fec_bsz_ov_b4sp = RBME_FEC_BSZ_OV_B4SP_FEC_BSZ_OV(0x0000)|
                RBME_FEC_BSZ_OV_B4SP_FEC_BSZ_OV_B4SP_EN(0),

    /* FEC_CFG1 configuration, dependencies: [None, None, None, None, None] */
    .fec_cfg1 = RBME_FEC_CFG1_FECOV_EN(0)|
                RBME_FEC_CFG1_FEC_EN(0)|
                RBME_FEC_CFG1_FEC_SWAP(0)|
                RBME_FEC_CFG1_INTV_EN(0)|
                RBME_FEC_CFG1_NTERM(0),

    /* FEC_CFG2 configuration, dependencies: [None, None, None, None] */
    .fec_cfg2 = RBME_FEC_CFG2_LARGE_VL(0x00)|
                RBME_FEC_CFG2_SAT_VL(0x00)|
                RBME_FEC_CFG2_SDIDX(0)|
                RBME_FEC_CFG2_TB_LENGTH(0x00),

    /* FRAME_OVER_SZ configuration, dependencies: [None, None] */
    .frame_over_sz = RBME_FRAME_OVER_SZ_STD_FRM_OV(0x0000)|
                RBME_FRAME_OVER_SZ_STD_FRM_OV_EN(0),

    /* LEG0_CFG configuration, dependencies: [None, None, None, None, None] */
    .leg0_cfg = RBME_LEG0_CFG_LEG0_INV_EN(0)|
                RBME_LEG0_CFG_LEG0_SUP(0)|
                RBME_LEG0_CFG_LEG0_XOR_BYTE(0x00)|
                RBME_LEG0_CFG_LEG0_XOR_FST_BYTE(0x00)|
                RBME_LEG0_CFG_LEG0_XOR_RP_BYTE(0x00),

    /* NPAYL_OVER_SZ configuration, dependencies: [None, None, None] */
    .npayl_over_sz = RBME_NPAYL_OVER_SZ_FT_FEC_FLUSH(0x00)|
                RBME_NPAYL_OVER_SZ_NPAYL_OV(0x0000)|
                RBME_NPAYL_OVER_SZ_NPAYL_OV_EN(0),

    /* PKT_SZ configuration, dependencies: [None, None] */
    .pkt_sz = RBME_PKT_SZ_DEF_PKT_SZ(0x0024)|
                RBME_PKT_SZ_MAX_PKT_SZ(0x4000),

    /* RAM_D_ADDR configuration, dependencies: [None] */
    .ram_d_addr = RBME_RAM_D_ADDR_RAM_D_ADDR(0x00000000),

    /* RAM_IF_CFG configuration, dependencies: [None, None, None, None, None, None, None] */
    .ram_if_cfg = RBME_RAM_IF_CFG_H2S_EN(0)|
                RBME_RAM_IF_CFG_RAM_IF_IC(0)|
                RBME_RAM_IF_CFG_RAM_IF_IE(0)|
                RBME_RAM_IF_CFG_RAM_IF_RX_EN(0)|
                RBME_RAM_IF_CFG_RAM_IF_TX_EN(0)|
                RBME_RAM_IF_CFG_SOFT_HD_SEL_RD(0)|
                RBME_RAM_IF_CFG_SOFT_HD_SEL_WR(0),

    /* RAM_S_ADDR configuration, dependencies: [None] */
    .ram_s_addr = RBME_RAM_S_ADDR_RAM_S_ADDR(0x00000000),

    /* RBME_RST configuration, dependencies: [None] */
    .rbme_rst = RBME_RBME_RST_RBME_RST(0),

    /* SPREAD_CFG configuration, dependencies: [None, None, None, None, None] */
    .spread_cfg = RBME_SPREAD_CFG_CI_TX(0)|
                RBME_SPREAD_CFG_SPOV_EN(0)|
                RBME_SPREAD_CFG_SP_EN(0)|
                RBME_SPREAD_CFG_SP_FACTOR(0x00)|
                RBME_SPREAD_CFG_SP_SEQ(0x0000),

    /* WHITEN_CFG configuration, dependencies: [None, None, None, None, None, None, None, None] */
    .whiten_cfg = RBME_WHITEN_CFG_WHITEN_B4_CRC(0)|
                RBME_WHITEN_CFG_WHITEN_END(1)|
                RBME_WHITEN_CFG_WHITEN_INIT(0x0041)|
                RBME_WHITEN_CFG_WHITEN_PAYLOAD_REINIT(0)|
                RBME_WHITEN_CFG_WHITEN_POLY_TYPE(0)|
                RBME_WHITEN_CFG_WHITEN_REF_IN(0)|
                RBME_WHITEN_CFG_WHITEN_SIZE(0x07)|
                RBME_WHITEN_CFG_WHITEN_START(1),

    /* WHITEN_POLY configuration, dependencies: [None] */
    .whiten_poly = RBME_WHITEN_POLY_WHITEN_POLY(0x0004),

    /* WHITEN_SZ_THR configuration, dependencies: [None, None, None] */
    .whiten_sz_thr = RBME_WHITEN_SZ_THR_WHITEN_SZ_THR(0x0000),

    /* WHT_CFG configuration, dependencies: [None, None, None, None, None] */
    .wht_cfg = RBME_WHT_CFG_W1_EN(0)|
                RBME_WHT_CFG_WFIRST(0)|
                RBME_WHT_CFG_WTOV_EN(0)|
                RBME_WHT_CFG_WT_OUT_SEL(0)|
                RBME_WHT_CFG_WT_TPOGY(0),
    
    /* XCVR_2P4GHZ_PHY configs */
    /***************************/

    /* DMD_CTRL1 configuration, dependencies: [None, None, None, None, None, None] */
    .dmd_ctrl1 = XCVR_2P4GHZ_PHY_DMD_CTRL1_FED_IDLE_WIN(0x0008)|
                XCVR_2P4GHZ_PHY_DMD_CTRL1_FED_IMM_MEAS_EN(1)|
                XCVR_2P4GHZ_PHY_DMD_CTRL1_TED_ERR_SCALE(0x04)|
                XCVR_2P4GHZ_PHY_DMD_CTRL1_TED_IDLE_WIN(0x0004)|
                XCVR_2P4GHZ_PHY_DMD_CTRL1_TED_IMM_MEAS_EN(1)|
                XCVR_2P4GHZ_PHY_DMD_CTRL1_TTRK_INT_RANGE(0x03),
    /***********************************************/
    /************ END OF GENERATED CODE ************/
    /*********** xcvr_ble_uncoded_config ***********/
    /***********************************************/
};

const xcvr_coding_config_t xcvr_ant_uncoded_config =
{
    /***********************************************/
    /*********** START OF GENERATED CODE ***********/
    /*********** xcvr_ant_uncoded_config ***********/
    /***********************************************/
    .coding_mode = ANT_UNCODED,
    
    /* RBME configs */
    /****************/

    /* CRCW_CFG configuration, dependencies: [None, None] */
    .crcw_cfg = RBME_CRCW_CFG_CRCW_EC_EN(0)|
                RBME_CRCW_CFG_CRCW_EN(1),

    /* CRCW_CFG2 configuration, dependencies: [None, None, None] */
    .crcw_cfg2 = RBME_CRCW_CFG2_CRC_EC_LPKT_WND(0x00)|
                RBME_CRCW_CFG2_CRC_EC_SPKT_BYTES(0x00)|
                RBME_CRCW_CFG2_CRC_EC_SPKT_WND(0x00),

    /* CRCW_CFG3 configuration, dependencies: [None, None, None, None, None] */
    .crcw_cfg3 = RBME_CRCW_CFG3_CRC_BYTE_ORD(0)|
                RBME_CRCW_CFG3_CRC_REF_IN(0)|
                RBME_CRCW_CFG3_CRC_REF_OUT(0)|
                RBME_CRCW_CFG3_CRC_START_BYTE(0x04)|
                RBME_CRCW_CFG3_CRC_SZ(0x02),

    /* CRC_INIT configuration, dependencies: [None] */
    .crc_init = RBME_CRC_INIT_CRC_SEED(0xFFFF0000U),

    /* CRC_PHR_SZ configuration, dependencies: [None] */
    .crc_phr_sz = RBME_CRC_PHR_SZ_PHR_SZ(2),

    /* CRC_POLY configuration, dependencies: [None] */
    .crc_poly = RBME_CRC_POLY_CRC_POLY(0x10210000),

    /* CRC_XOR_OUT configuration, dependencies: [None] */
    .crc_xor_out = RBME_CRC_XOR_OUT_CRC_XOR_OUT(0x00000000),

    /* FCP_CFG configuration, dependencies: [None] */
    .fcp_cfg = RBME_FCP_CFG_FCP_SUPPORT(1),

    /* FEC_BSZ_OV_B4SP configuration, dependencies: [None, None] */
    .fec_bsz_ov_b4sp = RBME_FEC_BSZ_OV_B4SP_FEC_BSZ_OV(0x0000)|
                RBME_FEC_BSZ_OV_B4SP_FEC_BSZ_OV_B4SP_EN(0),

    /* FEC_CFG1 configuration, dependencies: [None, None, None, None, None] */
    .fec_cfg1 = RBME_FEC_CFG1_FECOV_EN(0)|
                RBME_FEC_CFG1_FEC_EN(0)|
                RBME_FEC_CFG1_FEC_SWAP(0)|
                RBME_FEC_CFG1_INTV_EN(0)|
                RBME_FEC_CFG1_NTERM(0),

    /* FEC_CFG2 configuration, dependencies: [None, None, None, None] */
    .fec_cfg2 = RBME_FEC_CFG2_LARGE_VL(0x00)|
                RBME_FEC_CFG2_SAT_VL(0x00)|
                RBME_FEC_CFG2_SDIDX(0)|
                RBME_FEC_CFG2_TB_LENGTH(0x00),

    /* FRAME_OVER_SZ configuration, dependencies: [None, None] */
    .frame_over_sz = RBME_FRAME_OVER_SZ_STD_FRM_OV(0x0000)|
                RBME_FRAME_OVER_SZ_STD_FRM_OV_EN(0),

    /* LEG0_CFG configuration, dependencies: [None, None, None, None, None] */
    .leg0_cfg = RBME_LEG0_CFG_LEG0_INV_EN(0)|
                RBME_LEG0_CFG_LEG0_SUP(0)|
                RBME_LEG0_CFG_LEG0_XOR_BYTE(0x00)|
                RBME_LEG0_CFG_LEG0_XOR_FST_BYTE(0x00)|
                RBME_LEG0_CFG_LEG0_XOR_RP_BYTE(0x00),

    /* NPAYL_OVER_SZ configuration, dependencies: [None, None, None] */
    .npayl_over_sz = RBME_NPAYL_OVER_SZ_FT_FEC_FLUSH(0x00)|
                RBME_NPAYL_OVER_SZ_NPAYL_OV(0x0000)|
                RBME_NPAYL_OVER_SZ_NPAYL_OV_EN(0),

    /* PKT_SZ configuration, dependencies: [None, None] */
    .pkt_sz = RBME_PKT_SZ_DEF_PKT_SZ(0x0000)|
                RBME_PKT_SZ_MAX_PKT_SZ(0x0000),

    /* RAM_D_ADDR configuration, dependencies: [None] */
    .ram_d_addr = RBME_RAM_D_ADDR_RAM_D_ADDR(0x00000000),

    /* RAM_IF_CFG configuration, dependencies: [None, None, None, None, None, None, None] */
    .ram_if_cfg = RBME_RAM_IF_CFG_H2S_EN(0)|
                RBME_RAM_IF_CFG_RAM_IF_IC(0)|
                RBME_RAM_IF_CFG_RAM_IF_IE(0)|
                RBME_RAM_IF_CFG_RAM_IF_RX_EN(0)|
                RBME_RAM_IF_CFG_RAM_IF_TX_EN(0)|
                RBME_RAM_IF_CFG_SOFT_HD_SEL_RD(0)|
                RBME_RAM_IF_CFG_SOFT_HD_SEL_WR(0),

    /* RAM_S_ADDR configuration, dependencies: [None] */
    .ram_s_addr = RBME_RAM_S_ADDR_RAM_S_ADDR(0x00000000),

    /* RBME_RST configuration, dependencies: [None] */
    .rbme_rst = RBME_RBME_RST_RBME_RST(0),

    /* SPREAD_CFG configuration, dependencies: [None, None, None, None, None] */
    .spread_cfg = RBME_SPREAD_CFG_CI_TX(0)|
                RBME_SPREAD_CFG_SPOV_EN(0)|
                RBME_SPREAD_CFG_SP_EN(0)|
                RBME_SPREAD_CFG_SP_FACTOR(0x00)|
                RBME_SPREAD_CFG_SP_SEQ(0x0000),

    /* WHITEN_CFG configuration, dependencies: [None, None, None, None, None, None, None, None] */
    .whiten_cfg = RBME_WHITEN_CFG_WHITEN_B4_CRC(1)|
                RBME_WHITEN_CFG_WHITEN_END(0)|
                RBME_WHITEN_CFG_WHITEN_INIT(0x01FF)|
                RBME_WHITEN_CFG_WHITEN_PAYLOAD_REINIT(0)|
                RBME_WHITEN_CFG_WHITEN_POLY_TYPE(1)|
                RBME_WHITEN_CFG_WHITEN_REF_IN(1)|
                RBME_WHITEN_CFG_WHITEN_SIZE(0x09)|
                RBME_WHITEN_CFG_WHITEN_START(1),

    /* WHITEN_POLY configuration, dependencies: [None] */
    .whiten_poly = RBME_WHITEN_POLY_WHITEN_POLY(0x0021),

    /* WHITEN_SZ_THR configuration, dependencies: [None, None, None] */
    .whiten_sz_thr = RBME_WHITEN_SZ_THR_WHITEN_SZ_THR(0x0012),

    /* WHT_CFG configuration, dependencies: [None, None, None, None, None] */
    .wht_cfg = RBME_WHT_CFG_W1_EN(0)|
                RBME_WHT_CFG_WFIRST(0)|
                RBME_WHT_CFG_WTOV_EN(0)|
                RBME_WHT_CFG_WT_OUT_SEL(0)|
                RBME_WHT_CFG_WT_TPOGY(0),
    
    /* XCVR_2P4GHZ_PHY configs */
    /***************************/

    /* DMD_CTRL1 configuration, dependencies: [None, None, None, None, None, None] */
    .dmd_ctrl1 = XCVR_2P4GHZ_PHY_DMD_CTRL1_FED_IDLE_WIN(0x0008)|
                XCVR_2P4GHZ_PHY_DMD_CTRL1_FED_IMM_MEAS_EN(1)|
                XCVR_2P4GHZ_PHY_DMD_CTRL1_TED_ERR_SCALE(0x04)|
                XCVR_2P4GHZ_PHY_DMD_CTRL1_TED_IDLE_WIN(0x0004)|
                XCVR_2P4GHZ_PHY_DMD_CTRL1_TED_IMM_MEAS_EN(1)|
                XCVR_2P4GHZ_PHY_DMD_CTRL1_TTRK_INT_RANGE(0x03),
    /***********************************************/
    /************ END OF GENERATED CODE ************/
    /*********** xcvr_ant_uncoded_config ***********/
    /***********************************************/
};


