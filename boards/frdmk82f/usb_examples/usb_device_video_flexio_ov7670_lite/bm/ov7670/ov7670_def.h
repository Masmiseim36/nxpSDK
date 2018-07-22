/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _OV7670_DEF_H_
#define _OV7670_DEF_H_

/*!
 * @addtogroup ov7670
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Register definitions for the OV7670.*/

#define OV7670_GAIN_REG 0x00U /*!< Gain lower 8 bits (rest in vref). */

#define OV7670_BLUE_REG 0x01U /*!< blue gain. */

#define OV7670_RED_REG 0x02U /*!< red gain. */

#define OV7670_VREF_REG 0x03U /*!< Pieces of GAIN, VSTART, VSTOP. */

#define OV7670_COM1_REG 0x04U          /*!< Control 1. */
#define OV7670_COM1_CCIR656_MASK 0x40U /*!< CCIR656 enable. */

#define OV7670_BAVE_REG 0x05U /*!< U/B Average level. */

#define OV7670_GbAVE_REG 0x06U /*!< Y/Gb Average level. */

#define OV7670_AECHH_REG 0x07U /*!< AEC MS 5 bits. */

#define OV7670_RAVE_REG 0x08U /*!< V/R Average level. */

#define OV7670_COM2_REG 0x09U         /*!< Control 2. */
#define OV7670_COM2_SSLEEP_MASK 0x10U /*!< Soft sleep mode. */

#define OV7670_PID_REG 0x0AU /*!< Product ID MSB register address. */
#define OV7670_PID_NUM 0x76U /*!< Product ID. */

#define OV7670_VER_REG 0x0BU /*!< Product ID LSB register address. */
#define OV7670_VER_NUM 0x73U /*!< Product VERION. */

#define OV7670_COM3_REG 0x0CU          /*!< Control 3. */
#define OV7670_COM3_SWAP_MASK 0x40U    /*!< Byte swap. */
#define OV7670_COM3_SCALEEN_MASK 0x08U /*!< Enable scaling. */
#define OV7670_COM3_DCWEN_MASK 0x04U   /*!< Enable downsamp/crop/window. */

#define OV7670_COM4_REG 0x0DU /*!< Control 4. */

#define OV7670_COM5_REG 0x0EU /*!< All "reserved". */

#define OV7670_COM6_REG 0x0FU /*!< Control 6. */

#define OV7670_AECH_REG 0x10U /*!< More bits of AEC value. */

#define OV7670_CLKRC_REG 0x11U      /*!< Clocl control. */
#define OV7670_CLK_EXT_MASK 0x40U   /*!< Use external clock directly. */
#define OV7670_CLK_SCALE_MASK 0x3FU /*!< Mask for internal clock scale. */

#define OV7670_COM7_REG 0x12U        /*!< Control 7. */
#define OV7670_COM7_RESET_MASK 0x80U /*!< Register reset. */
#define OV7670_COM7_FMT_MASK_MASK 0x38U
#define OV7670_COM7_FMT_VGA_MASK 0x00U
#define OV7670_COM7_FMT_CIF_MASK 0x20U  /*!< CIF format. */
#define OV7670_COM7_FMT_QVGA_MASK 0x10U /*!< QVGA format. */
#define OV7670_COM7_FMT_QCIF_MASK 0x08U /*!< QCIF format. */
#define OV7670_COM7_RGB_MASK 0x04U      /*!< bits 0 and 2 - RGB format. */
#define OV7670_COM7_YUV_MASK 0x00U      /*!< YUV. */
#define OV7670_COM7_BAYER_MASK 0x01U    /*!< Bayer format. */
#define OV7670_COM7_PBAYER_MASK 0x05U   /*!< "Processed bayer". */

#define OV7670_COM8_REG 0x13U          /*!< Control 8. */
#define OV7670_COM8_FASTAEC_MASK 0x80U /*!< Enable fast AGC/AEC. */
#define OV7670_COM8_AECSTEP_MASK 0x40U /*!< Unlimited AEC step size. */
#define OV7670_COM8_BFILT_MASK 0x20U   /*!< Band filter enable. */
#define OV7670_COM8_AGC_MASK 0x04U     /*!< Auto gain enable. */
#define OV7670_COM8_AWB_MASK 0x02U     /*!< White balance enable. */
#define OV7670_COM8_AEC_MASK 0x01U     /*!< Auto exposure enable. */

#define OV7670_COM9_REG 0x14U /*!< Control 9  - gain ceiling. */

#define OV7670_COM10_REG 0x15U           /*!< Control 10. */
#define OV7670_COM10_HSYNC_MASK 0x40U    /*!< HSYNC instead of HREF. */
#define OV7670_COM10_PCLK_HB_MASK 0x20U  /*!< Suppress PCLK on horiz blank. */
#define OV7670_COM10_HREF_REV_MASK 0x08U /*!< Reverse HREF. */
#define OV7670_COM10_VS_LEAD_MASK 0x04U  /*!< VSYNC on clock leading edge. */
#define OV7670_COM10_VS_NEG_MASK 0x02U   /*!< VSYNC negative. */
#define OV7670_COM10_HS_NEG_MASK 0x01U   /*!< HSYNC negative. */

#define OV7670_RSVD_REG 0x16U /*!< reserved. */

#define OV7670_HSTART_REG 0x17U /*!< Horiz start high bits. */

#define OV7670_HSTOP_REG 0x18U /*!< Horiz stop high bits. */

#define OV7670_VSTART_REG 0x19U /*!< Vert start high bits. */

#define OV7670_VSTOP_REG 0x1AU /*!< Vert stop high bits. */

#define OV7670_PSHFT_REG 0x1BU /*!< Pixel delay after HREF. */

#define OV7670_MIDH_REG 0x1CU /*!< Manuf. ID high. */

#define OV7670_MIDL_REG 0x1DU /*!< Manuf. ID low. */

#define OV7670_MVFP_REG 0x1EU         /*!< Mirror / vflip. */
#define OV7670_MVFP_MIRROR_MASK 0x20U /*!< Mirror image. */
#define OV7670_MVFP_FLIP_MASK 0x10U   /*!< Vertical flip. */

#define OV7670_LAEC_REG 0x1FU /*!< reserved. */

#define OV7670_ADCCTR0_REG 0x20U            /*!< ADC control. */
#define OV7670_ADCCTR0_RANGE_ADJ_MASK 0x08U /*!< ADC range adjustment. */

#define OV7670_ADCCTR1_REG 0x21U /*!< reserved. */

#define OV7670_ADCCTR2_REG 0x22U /*!< reserved. */

#define OV7670_ADCCTR3_REG 0x23U /*!< reserved. */

#define OV7670_AEW_REG 0x24U /*!< AGC upper limit. */

#define OV7670_AEB_REG 0x25U /*!< AGC lower limit. */

#define OV7670_VPT_REG 0x26U /*!< AGC/AEC fast mode op region. */

#define OV7670_BBIAS_REG 0x27U /*!< B channel signal output bias. */

#define OV7670_GbBIAS_REG 0x28U /*!< Gb channel signal output bias. */

#define OV7670_RSVD1_REG 0x29U /*!< reserved 1. */

#define OV7670_EXHCH_REG 0x2AU /*!< dummy pixel insert MSB. */

#define OV7670_EXHCL_REG 0x2BU /*!< dummy pixel insert LSB. */

#define OV7670_HSYST_REG 0x30U /*!< HSYNC rising edge delay. */

#define OV7670_HSYEN_REG 0x31U /*!< HSYNC falling edge delay. */

#define OV7670_HREF_REG 0x32U /*!< HREF pieces. */

#define OV7670_CHLF_REG 0x33U /*!< array current control. */

#define OV7670_ARBLM_REG 0x34U /*!< array reference control. */

#define OV7670_RSVD2_REG 0x35U /*!< reserved 2. */

#define OV7670_RSVD3_REG 0x36U /*!< reserved 3. */

#define OV7670_ADC_REG 0x37U /*!< ADC control. */

#define OV7670_ACOM_REG 0x38U /*!< ADC and Analog common mode control. */

#define OV7670_OFON_REG 0x39U /*!< ADC offset control. */

#define OV7670_TSLB_REG 0x3AU        /*!< lots of stuff. */
#define OV7670_TSLB_YLAST_MASK 0x04U /*!< UYVY or VYUY - see com13. */

#define OV7670_COM11_REG 0x3BU         /*!< Control 11. */
#define OV7670_COM11_NIGHT_MASK 0x80U  /*!< NIght mode enable. */
#define OV7670_COM11_NMFR_MASK 0x60U   /*!< Two bit NM frame rate. */
#define OV7670_COM11_HZAUTO_MASK 0x10U /*!< Auto detect 50/60 Hz. */
#define OV7670_COM11_50HZ_MASK 0x08U   /*!< Manual 50Hz select. */
#define OV7670_COM11_EXP_MASK 0x02U

#define OV7670_COM12_REG 0x3CU       /*!< Control 12. */
#define OV7670_COM12_HREF_MASK 0x80U /*!< HREF always. */

#define OV7670_COM13_REG 0x3DU         /*!< Control 13. */
#define OV7670_COM13_GAMMA_MASK 0x80U  /*!< Gamma enable. */
#define OV7670_COM13_UVSAT_MASK 0x40U  /*!< UV saturation auto adjustment. */
#define OV7670_COM13_UVSWAP_MASK 0x01U /*!< V before U - w/TSLB. */

#define OV7670_COM14_REG 0x3EU        /*!< Control 14. */
#define OV7670_COM14_DCWEN_MASK 0x10U /*!< DCW/PCLK-scale enable. */

#define OV7670_EDGE_REG 0x3FU /*!< Edge enhancement factor. */

#define OV7670_COM15_REG 0x40U         /*!< Control 15. */
#define OV7670_COM15_R10F0_MASK 0x00U  /*!< Data range 10 to F0. */
#define OV7670_COM15_R01FE_MASK 0x80U  /*!< 01 to FE. */
#define OV7670_COM15_R00FF_MASK 0xC0U  /*!<  00 to FF. */
#define OV7670_COM15_RGB565_MASK 0x10U /*!< RGB565 output. */
#define OV7670_COM15_RGB555_MASK 0x30U /*!< RGB555 output. */

#define OV7670_COM16_REG 0x41U          /*!< Control 16. */
#define OV7670_COM16_AWBGAIN_MASK 0x08U /*!< AWB gain enable. */

#define OV7670_COM17_REG 0x42U         /*!< Control 17. */
#define OV7670_COM17_AECWIN_MASK 0xc0U /*!< AEC window - must match COM4. */
#define OV7670_COM17_CBAR_MASK 0x08U   /*!< DSP Color bar. */

#define OV7670_AWBC1_REG 0x43U /*!< AWB control 1. */

#define OV7670_AWBC2_REG 0x44U /*!< AWB control 2. */

#define OV7670_AWBC3_REG 0x45U /*!< AWB control 3. */

#define OV7670_AWBC4_REG 0x46U /*!< AWB control 4. */

#define OV7670_AWBC5_REG 0x47U /*!< AWB control 5. */

#define OV7670_AWBC6_REG 0x48U /*!< AWB control 6. */

#define OV7670_MTX1_REG 0x4fU /*!< Matrix Coefficient 1. */

#define OV7670_MTX2_REG 0x50U /*!< Matrix Coefficient 2. */

#define OV7670_MTX3_REG 0x51U /*!< Matrix Coefficient 3. */

#define OV7670_MTX4_REG 0x52U /*!< Matrix Coefficient 4. */

#define OV7670_MTX5_REG 0x53U /*!< Matrix Coefficient 5. */

#define OV7670_MTX6_REG 0x54U /*!< Matrix Coefficient 6. */

#define OV7670_BRIGHT_REG 0x55U /*!< Brightness. */

#define OV7670_CONTRAS_REG 0x56U /*!< Contrast control. */

#define OV7670_CONTRAS_CENTER_REG 0x57U /*!< Contrast cetnter control. */

#define OV7670_MTXS_REG 0x58U    /*!< Matrix Coefficient Sign. */
#define OV7670_AWBC7_MASK 0x59U  /*!< AWB Control 7. */
#define OV7670_AWBC8_MASK 0x5AU  /*!< AWB Control 8. */
#define OV7670_AWBC9_MASK 0x5BU  /*!< AWB Control 9. */
#define OV7670_AWBC10_MASK 0x5CU /*!< AWB Control 10. */
#define OV7670_AWBC11_MASK 0x5DU /*!< AWB Control 11. */
#define OV7670_AWBC12_MASK 0x5EU /*!< AWB Control 12. */

#define OV7670_MANU_REG 0x67U /*!< Manual U value. */

#define OV7670_MANV_REG 0x68U /*!< Manual V value. */

#define OV7670_GFIX_REG 0x69U /*!< Fix gain control. */

#define OV7670_GGAIN_REG 0x6AU /*!< G Channel AWB Gain. */

#define OV7670_DBLV_REG 0x6BU

#define OV7670_AWBCTR3_REG 0x6CU /*!< AWB Control 3. */

#define OV7670_AWBCTR2_REG 0x6DU /*!< AWB Control 2. */

#define OV7670_AWBCTR1_REG 0x6EU /*!< AWB Control 1. */

#define OV7670_AWBCTR0_REG 0x6FU /*!< AWB Control 0. */

#define OV7670_SCALING_XSC_REG 0x70U /*!< horizontal scale factor. */

#define OV7670_SCALING_YSC_REG 0x71U /*!< vertical scale factor. */

#define OV7670_SCALING_DCWCTR_REG 0x72U /*!< DCW control. */

#define OV7670_SCALING_PCLK_DIV_REG 0x73U /*!< clock divider control. */

#define OV7670_REG74_REG 0x74U /*!< register 74. */

#define OV7670_REG76_REG 0x76U          /*!< OV's name. */
#define OV7670_REG76_BLKPCOR_MASK 0x80U /*!< Black pixel correction enable. */
#define OV7670_REG76_WHTPCOR_MASK 0x40U /*!< White pixel correction enable. */

#define OV7670_SLOP_REG 0x7AU /*!< gamma curve highest segment slop. */

#define OV7670_GAM1_REG 0x7BU         /*!< gamma curve 1 segment slop. */
#define OV7670_GAM2_REG 0x7CU         /*!< gamma curve 2 segment slop. */
#define OV7670_GAM3_REG 0x7DU         /*!< gamma curve 3 segment slop. */
#define OV7670_GAM4_REG 0x7EU         /*!< gamma curve 4 segment slop. */
#define OV7670_GAM5_REG 0x7FU         /*!< gamma curve 5 segment slop. */
#define OV7670_GAM6_REG 0x80U         /*!< gamma curve 6 segment slop. */
#define OV7670_GAM7_REG 0x81U         /*!< gamma curve 7 segment slop. */
#define OV7670_GAM8_REG 0x82U         /*!< gamma curve 8 segment slop. */
#define OV7670_GAM9_REG 0x83U         /*!< gamma curve 9 segment slop. */
#define OV7670_GAM10_REG 0x84U        /*!< gamma curve 10 segment slop. */
#define OV7670_GAM11_REG 0x85U        /*!< gamma curve 11 segment slop. */
#define OV7670_GAM12_REG 0x86U        /*!< gamma curve 12 segment slop. */
#define OV7670_GAM13_REG 0x87U        /*!< gamma curve 13 segment slop. */
#define OV7670_GAM14_REG 0x88U        /*!< gamma curve 14 segment slop. */
#define OV7670_GAM15_REG 0x89U        /*!< gamma curve 15 segment slop. */
#define OV7670_RGB444_REG 0x8cU       /*!< RGB 444 control. */
#define OV7670_R444_ENABLE_MASK 0x02U /*!< Turn on RGB444, overrides 5x5. */
#define OV7670_R444_RGBX_MASK 0x01U   /*!< Empty nibble at end. */

#define OV7670_DM_LNL_REG 0x92U /*!< dummy line low 8 bits. */

#define OV7670_DM_LNH_REG 0x93U /*!< dummy line high 8 bits. */

#define OV7670_BD50ST_REG 0x9dU /*!< 50Hz banding filter value. */

#define OV7670_BD60ST_REG 0x9eU /*!< 60Hz banding filter value. */

#define OV7670_HAECC1_REG 0x9fU /*!< Hist AEC/AGC control 1. */

#define OV7670_HAECC2_REG 0xa0U /*!< Hist AEC/AGC control 2. */

#define OV7670_SCALING_PCLK_DELAY_REG 0xa2U /*!< pixel clock delay. */

#define OV7670_BD50MAX_REG 0xa5U /*!< 50hz banding step limit. */

#define OV7670_HAECC3_REG 0xa6U /*!< Hist AEC/AGC control 3. */

#define OV7670_HAECC4_REG 0xa7U /*!< Hist AEC/AGC control 4. */

#define OV7670_HAECC5_REG 0xa8U /*!< Hist AEC/AGC control 5. */

#define OV7670_HAECC6_REG 0xa9U /*!< Hist AEC/AGC control 6. */

#define OV7670_HAECC7_REG 0xaaU /*!< Hist AEC/AGC control 7. */

#define OV7670_BD60MAX_REG 0xabU /*!< 60hz banding step limit. */

#define OV7670_STR_OPT_REG 0xacU /*!< strobe control. */

#define OV7670_STR_R_REG 0xadU /*!< R gain for LED output frame. */

#define OV7670_STR_G_REG 0xaeU /*!< G gain for LED output frame. */

#define OV7670_STR_B_REG 0xafU /*!< B gain for LED output frame. */

#define OV7670_ABLC1_REG 0xb1U /*!< ABLC function control. */

#define OV7670_THL_ST_REG 0xb3U /*!< ABLC target. */

/*! @} */

#endif /*_OV7670_DEF_H_*/
