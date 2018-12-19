/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*!
 * @file sf_ol23xx.h
 *
 * Definitions of SPI frames and commands for the OL2361 and OL2385.
 */

#ifndef SOURCE_SF_OL23XX_H_
#define SOURCE_SF_OL23XX_H_

/*******************************************************************************
 * General definitions
 ******************************************************************************/

/*!
 * @name General bit operations */
/*! @{ */

/*!
 * @brief Macro for setting value of the bit given by the mask.
 *
 * @param Msg Message to be modified
 * @param Mask Bit selection in message
 */
#define SF_SET_BIT_VALUE(Msg, Mask) ((Msg) | (Mask))

/*!
 * @brief Macro for unsetting value of the bit given by the mask.
 *
 * @param Msg Message to be modified
 * @param Mask Bit selection in message
 */
#define SF_UNSET_BIT_VALUE(Msg, Mask) ((Msg) & ~(Mask))

/*!
 * @brief Macro for getting value of the bit given by the mask.
 *
 * @param Msg Message to be read
 * @param Mask Bit selection in message
 * @param Shift Bit shift in message
 */
#define SF_GET_BIT_VALUE(Msg, Mask, Shift) (((Msg) & (Mask)) >> (Shift))

/*!
 * @brief Macro for setting value of bits given by the mask.
 *
 * @param Msg Message to be modified
 * @param Mask Bits selection in message
 * @param Shift Bits shift in message
 * @param Val Value to be applied
 * @param Range Admissible range of value
 */
#define SF_SET_BITS_VALUE(Msg, Mask, Shift, Val, Range) (((Msg) & ~(Mask)) | (((Val) & (Range)) << (Shift)))

/*!
 * @brief Macro for getting value of bits given by the mask.
 *
 * @param Msg Message to be read
 * @param Mask Bits selection in message
 * @param Shift Bits shift in message
 */
#define SF_GET_BITS_VALUE(Msg, Mask, Shift) (((Msg) & (Mask)) >> (Shift))
/*! @} */

/*! @name Min/max macros */
/* @{ */
#if !defined(SF_MIN)
#define SF_MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#if !defined(SF_MAX)
#define SF_MAX(a, b) ((a) > (b) ? (a) : (b))
#endif
/* @} */

/*******************************************************************************
 * Definitions of SPI frames
 ******************************************************************************/
/*!
 * @name SPI ACK frame
 * Definitions common for all ACK frames.
 */
/*! @{ */
/*! Size of the SPI ACK frame header (including length, error code and state fields)
 * in bytes. */
#define SF_ACK_HEADER_B             0x03U
/*! Max. payload size in bytes ("Get info" returns the longest payload). */
#define SF_ACK_PAYLOAD_MAX_B        SF_UPDATE_PA_CURVE_ACK_PLD_B
/*! Max. ACK frame size in bytes. */
#define SF_ACK_SPI_MSG_MAX_B        (SF_ACK_HEADER_B + SF_ACK_PAYLOAD_MAX_B)
/*! Offset of the Length field in the ACK frame. */
#define SF_ACK_LENGTH_OF            0X00U
/*! Offset of the Error code field in the ACK frame. */
#define SF_ACK_ERROR_OF             0X01U
/*! Offset of the State field in the ACK frame. */
#define SF_ACK_STATE_OF             0x02U
/*! Offset of the Payload field in the ACK frame. */
#define SF_ACK_PAYLOAD_OF           0x03U
/*! @} */

/*!
 * @name SPI Information frame
 * Definitions common for all INF frames.
 * @{
 */
/*! Size of the SPI Information frame header (including length and command fields)
 * in bytes. */
#define SF_INF_HEADER_B             0x02U
/*! Max. payload size in bytes. */
#define SF_INF_PAYLOAD_MAX_B        SF_UPDATE_PA_CURVE_INF_PLD_B
/*! Max. Information frame size in bytes. */
#define SF_INF_SPI_MSG_MAX_B        (SF_INF_HEADER_B + SF_INF_PAYLOAD_MAX_B)
/*! Offset of the Length field in the Information frame. */
#define SF_INF_LENGTH_OF            0x00U
/*! Offset of the Command field in the Information frame. */
#define SF_INF_CMD_OF               0x01U
/*! Offset of the Payload field in the Information frame. */
#define SF_INF_PAYLOAD_OF           0x02U
/*! @} */

/*!
 * @name SPI Information frame
 * The min. and max. value of the command ID.
 * @{
 */
#define SF_CMD_FIRST_ID             SF_CMD_SEND_WAKEUP_ID
#define SF_CMD_LAST_ID              SF_CMD_INIT_RSSI_CALIB_ID
/*! @} */

/*! Max. SPI frame size in bytes. */
#define SF_SPI_MSG_MAX_B            (SF_MAX(SF_INF_SPI_MSG_MAX_B, SF_ACK_SPI_MSG_MAX_B))

/*******************************************************************************
 * Definitions of SPI commands
 ******************************************************************************/
/*!
 * @name Send Wake-up SPI command
 * @{
 */
/*! Send Wake-up command. */
#define SF_CMD_SEND_WAKEUP_ID       0x01U
/*! Size of the "Send Wake-up" information frame payload in bytes. */
#define SF_SEND_WAKEUP_INF_PLD_B    0x00U
/*! Size of the "Send Wake-up" information frame in bytes. */
#define SF_SEND_WAKEUP_INF_B        (SF_INF_HEADER_B + SF_SEND_WAKEUP_INF_PLD_B)
/*! Size of the "Send Wake-up" acknowledgement payload in bytes. */
#define SF_SEND_WAKEUP_ACK_PLD_B    0x00U
/*! Size of the "Send Wake-up" acknowledgement in bytes (no ACK). */
#define SF_SEND_WAKEUP_ACK_B        0x00U
/*! Size of the SPI frame in bytes. It is the max. of ACK and I-frames. */
#define SF_SEND_WAKEUP_FRM_B        (SF_MAX(SF_SEND_WAKEUP_INF_B, SF_SEND_WAKEUP_ACK_B))
/*! @} */

/*!
 * @name Send Echo SPI command
 * @{
 */
/*! Echo command. */
#define SF_CMD_SEND_ECHO_ID         0x02U
/*! Size of the "Send Echo" information frame payload in bytes (max. 5). */
#define SF_SEND_ECHO_INF_PLD_B      0x05U
/*! Size of the "Send Echo" information frame in bytes. */
#define SF_SEND_ECHO_INF_B          (SF_INF_HEADER_B + SF_SEND_ECHO_INF_PLD_B)
/*! Size of the "Send Echo" acknowledgement payload in bytes (max. 5). */
#define SF_SEND_ECHO_ACK_PLD_B      0x05U
/*! Size of the "Send Echo" acknowledgement in bytes. */
#define SF_SEND_ECHO_ACK_B          (SF_ACK_HEADER_B + SF_SEND_ECHO_ACK_PLD_B)
/*! Size of the SPI frame in bytes. It is the max. of ACK frame and I-frame. */
#define SF_SEND_ECHO_FRM_B          (SF_MAX(SF_SEND_ECHO_INF_B, SF_SEND_ECHO_ACK_B))
/*! @} */

/*!
 * @name Send to Sleep SPI command
 * @{
 */
/*! Send to sleep command. */
#define SF_CMD_SEND_TO_SLEEP_ID     0x03U
/*! Size of the "Send to sleep" information frame payload in bytes. */
#define SF_SEND_TO_SLEEP_INF_PLD_B  0x00U
/*! Size of the "Send to sleep" information frame in bytes. */
#define SF_SEND_TO_SLEEP_INF_B      (SF_INF_HEADER_B + SF_SEND_TO_SLEEP_INF_PLD_B)
/*! Size of the "Send to Sleep" acknowledgement payload in bytes. */
#define SF_SEND_TO_SLEEP_ACK_PLD_B  0x00U
/*! Size of the "Send to Sleep" acknowledgement in bytes. */
#define SF_SEND_TO_SLEEP_ACK_B      0x00U
/*! Size of the SPI frame in bytes. It is the max. of ACK frame and I-frame. */
#define SF_SEND_TO_SLEEP_FRM_B      (SF_MAX(SF_SEND_TO_SLEEP_INF_B, SF_SEND_TO_SLEEP_ACK_B))
/*! @} */

/*!
 * @name Send Payload SPI command
 * @{
 */
/*! Send payload command. */
#define SF_CMD_SEND_PAYLOAD_ID      0x04U
/*! Size of the "Send payload" information frame payload in bytes (max. 12). */
#define SF_SEND_PAYLOAD_INF_PLD_B   0x0CU
/*! Size of the "Send payload" information frame in bytes. */
#define SF_SEND_PAYLOAD_INF_B       (SF_INF_HEADER_B + SF_SEND_PAYLOAD_INF_PLD_B)
/*! Size of the "Send payload" acknowledgement payload in bytes. */
#define SF_SEND_PAYLOAD_ACK_PLD_B   0x00U
/*! Size of the "Send payload" acknowledgement in bytes. */
#define SF_SEND_PAYLOAD_ACK_B       (SF_ACK_HEADER_B + SF_SEND_PAYLOAD_ACK_PLD_B)
/*! Size of the SPI frame in bytes. It is the max. of ACK frame and I-frame. */
#define SF_SEND_PAYLOAD_FRM_B       (SF_MAX(SF_SEND_PAYLOAD_INF_B, SF_SEND_PAYLOAD_ACK_B))
/*! @} */

/*!
 * @name Send bit SPI command
 * @{
 */
/*! Send bit command. */
#define SF_CMD_SEND_BIT_ID          0x05U
/*! Size of the "Send bit" information frame payload in bytes. */
#define SF_SEND_BIT_INF_PLD_B       0x02U
/*! Size of the "Send bit" information frame in bytes. */
#define SF_SEND_BIT_INF_B           (SF_INF_HEADER_B + SF_SEND_BIT_INF_PLD_B)
/*! Size of the "Send bit" acknowledgement payload in bytes. */
#define SF_SEND_BIT_ACK_PLD_B       0x00U
/*! Size of the "Send bit" acknowledgement in bytes. */
#define SF_SEND_BIT_ACK_B           (SF_ACK_HEADER_B + SF_SEND_BIT_ACK_PLD_B)
/*! Size of the SPI frame in bytes. It is the max. of ACK frame and I-frame. */
#define SF_SEND_BIT_FRM_B           (SF_MAX(SF_SEND_BIT_INF_B, SF_SEND_BIT_ACK_B))

/*! Offset of the bit value. */
#define SF_SEND_BIT_VAL_OF          (0U + SF_INF_PAYLOAD_OF)
/*! Offset of the bit value. */
#define SF_SEND_BIT_INIT_DOWNLOAD_OF (1U + SF_INF_PAYLOAD_OF)
/*! @} */

/*!
 * @name Send Out of Band SPI command
 * @{
 */
/*! Send Out of Band command. */
#define SF_CMD_SEND_OUT_BAND_ID     0x06U
/*! Size of the "Send Out of Band" information frame payload in bytes. */
#define SF_SEND_OUT_BAND_INF_PLD_B  0x00U
/*! Size of the "Send Out of Band" information frame in bytes. */
#define SF_SEND_OUT_BAND_INF_B      (SF_INF_HEADER_B + SF_SEND_OUT_BAND_INF_PLD_B)
/*! Size of the "Send Out of Band" acknowledgement payload in bytes. */
#define SF_SEND_OUT_BAND_ACK_PLD_B  0x00U
/*! Size of the "Send Out of Band" acknowledgement in bytes. */
#define SF_SEND_OUT_BAND_ACK_B      (SF_ACK_HEADER_B + SF_SEND_OUT_BAND_ACK_PLD_B)
/*! Size of the SPI frame in bytes. It is the max. of ACK frame and I-frame. */
#define SF_SEND_OUT_BAND_FRM_B      (SF_MAX(SF_SEND_OUT_BAND_INF_B, SF_SEND_OUT_BAND_ACK_B))
/*! @} */

#if (SF_MODEL == SF_MODEL_OL2385)
/*!
 * @name Receive Frame SPI command
 * @{
 */
/*! Receive Frame command (available on OL2385). */
#define SF_CMD_RECEIVE_FR_ID        0x07U
/*! Size of the "Receive Frame" information frame payload in bytes. */
#define SF_RECEIVE_FR_INF_PLD_B     12U
/*! Size of the "Receive Frame" information frame in bytes. */
#define SF_RECEIVE_FR_INF_B         (SF_INF_HEADER_B + SF_RECEIVE_FR_INF_PLD_B)
/*! Size of the "Receive Frame" acknowledgement payload in bytes (max. 8). */
#define SF_RECEIVE_FR_ACK_PLD_B     0x08U
/*! Size of the "Receive Frame" acknowledgement in bytes. */
#define SF_RECEIVE_FR_ACK_B         (SF_ACK_HEADER_B + SF_RECEIVE_FR_ACK_PLD_B)
/*! Size of the SPI frame in bytes. It is the max. of ACK frame and I-frame. */
#define SF_RECEIVE_FR_FRM_B         (SF_MAX(SF_RECEIVE_FR_INF_B, SF_RECEIVE_FR_ACK_B))
/*! @} */
#endif /* END of device model check. */

/*!
 * @name Get Info SPI command
 * @{
 */
/*! Get Info command. */
#define SF_CMD_GET_INFO_ID          0x08U
/*! Size of the "Get Info" information frame payload in bytes. */
#define SF_GET_INFO_INF_PLD_B       0x00U
/*! Size of the "Get Info" information frame in bytes. */
#define SF_GET_INFO_INF_B           (SF_INF_HEADER_B + SF_GET_INFO_INF_PLD_B)
/*! Size of the "Get Info" acknowledgement payload in bytes. */
#define SF_GET_INFO_ACK_PLD_B       23U
/*! Size of the "Get Info" acknowledgement in bytes. */
#define SF_GET_INFO_ACK_B           (SF_ACK_HEADER_B + SF_GET_INFO_ACK_PLD_B)
/*! Size of the SPI frame in bytes. It is the max. of ACK frame and I-frame. */
#define SF_GET_INFO_FRM_B           SF_MAX(SF_GET_INFO_INF_B, SF_GET_INFO_ACK_B)

/*! Size of the Device ID field in the ACK frame. */
#define SF_DEV_ID_B                 4U
/*! Size of the PAC field in the ACK frame. */
#define SF_DEV_PAC_B                8U
/*! Size of the Library version field in the ACK frame. */
#define SF_LIB_VER_B                11U

/*! Offset of the Device ID field in the payload of ACK frame. */
#define SF_DEV_ID_OF                0U
/*! Offset of the PAC field in the payload of ACK frame. */
#define SF_DEV_PAC_OF               4U
/*! Offset of the Library version field in the payload of ACK frame. */
#define SF_LIB_VER_OF               12U

/*!
 * @brief This macro returns a 32 bit value containing device ID
 * from the "Get Info" acknowledgement.
 *
 * @param payload Pointer to the Device ID field in the acknowledgement payload
 * (8 bit pointer).
 */
#define SF_GET_DEV_ID(payload) \
    (((uint32_t)(*(payload))) | \
     ((uint32_t)(*((payload) + 1)) << 8U) | \
     ((uint32_t)(*((payload) + 2)) << 16U) | \
     ((uint32_t)(*((payload) + 3)) << 24U))
/*! @} */

/*!
 * @name UL/DL Frequency definitions
 * Following definitions are used in UL/DL frequency formulas.
 * @{
 */
/*! Value of a crystal frequency used in UL/DL formulas. */
#define SF_FREQ_F_X0              27600000U
/*! FIXDIV_DIV2_EN value, which is defined in OL2361 datasheet. Note that
 * OL2385 formulas uses a factor which is similar to this value. */
#define SF_FREQ_FIXDIV_DIV2_EN    0U
/*! This factor is used in UL/DL frequency formulas. */
#define SF_FREQ_FACTOR            (1U + SF_FREQ_FIXDIV_DIV2_EN)
/*! Recommended VCO divider setting for AFC calibration for all bands. */
#define SF_FREQ_N_FIX             8U
/*! OL2385 system clock divider. */
#define SF_FREQ_SYS_CLK_DIV       0U
/*! T_fix is 4, because up-link and down-link frequency is always in
 * <3 100 000 000 / 4; 4 400 000 000 / 4> Hz. */
#define SF_FREQ_T_FIX             4U
/*! @} */

/*!
 * @name UL Frequency FCFRAC
 * Masks and shifts for FCFRAC value.
 * @{
 */
/*! Mask for frequency control fractional part low-high bits (FCFRAC[8:0]). */
#define SF_FCFRAC_LH_MASK           0x1FFU
/*! Mask for frequency control fractional part high-low bits (FCFRAC[16:9]). */
#define SF_FCFRAC_HL_MASK           0x1FE00U
/*! Mask for frequency control fractional part high-high bits (FCFRAC[18:17]). */
#define SF_FCFRAC_HH_MASK           0x60000U

/*! Shift for frequency control fractional part low-high bits. */
#define SF_FCFRAC_LH_SHIFT          0x00U
/*! Shift for frequency control fractional part high-low bits. */
#define SF_FCFRAC_HL_SHIFT          0x09U
/*! Shift for frequency control fractional part high-high bits. */
#define SF_FCFRAC_HH_SHIFT          0x11U
/*! @} */

/*!
 * @name UL/DL Frequency FCINT
 * Masks and shifts for FCINT value.
 * @{
 */
/*! Mask for integer part frequency control word bits. */
#define SF_FCINT_MASK               0x3FU

/*! Shift for integer part frequency control word bits. */
#define SF_FCINT_SHIFT              0x00U
/*! @} */

/*!
 * @name UL/DL Frequency register FREQCON0
 * FREQCON0 register masks and shifts
 * @{
 */
/*! Mask for frequency control fractional part low-high bits of FREQCON0 register. */
#define SF_FREQCON0_FCFRAC_LH_MASK  0xFF80U
/*! Mask for frequency control disable fractional of FREQCON0 register. */
#define SF_FREQCON0_FCDISFRAC_MASK  0x01U

/*! Shift for frequency control fractional part low-high bits of FREQCON0 register. */
#define SF_FREQCON0_FCFRAC_LH_SHIFT 0x07U
/*! Shift for frequency control disable fractional of FREQCON0 register. */
#define SF_FREQCON0_FCDISFRAC_SHIFT 0x00U
/*! @} */

/*!
 * @name UL/DL Frequency register FREQCON1
 * FREQCON1 register masks and shifts
 * @{
 */
/*! Mask for integer part frequency control word of FREQCON1 register. */
#define SF_FREQCON1_FCINT_MASK      0xFC00U
/*! Mask for frequency control fractional part high-high bits of FREQCON1 register. */
#define SF_FREQCON1_FCFRAC_HH_MASK  0x300U
/*! Mask for frequency control fractional part high-low bits of FREQCON1 register. */
#define SF_FREQCON1_FCFRAC_HL_MASK  0xFFU

/*! Shift for integer part frequency control word of FREQCON1 register. */
#define SF_FREQCON1_FCINT_SHIFT     0x0AU
/*! Shift for frequency control fractional part high-high bits of FREQCON1 register. */
#define SF_FREQCON1_FCFRAC_HH_SHIFT 0x08U
/*! Shift for frequency control fractional part high-low bits of FREQCON1 register. */
#define SF_FREQCON1_FCFRAC_HL_SHIFT 0x00U
/*! @} */

/*!
 * @name Max. and min. up-link/down-link frequency.
 * @{
 */
/* Min. up-link/down-link frequency in Hz. */
#define SF_FREQ_MIN_HZ           775000000U
/* Max. up-link/down-link frequency in Hz. */
#define SF_FREQ_MAX_HZ           1000000000U
/*! @} */

/*! Set UL frequency command code. */
#define SF_CMD_SET_UL_FREQ_ID       0x09U

/*! Set UL frequency command code. */
#define SF_CMD_SET_DL_FREQ_ID       0x1DU

/*!
 * @name Set UL/DL frequency SPI command (these commands have common definition).
 * @{
 */
/*! Size of the information frame payload in bytes. */
#define SF_SET_FREQ_INF_PLD_B       0x06U
/*! Size of the information frame in bytes. */
#define SF_SET_FREQ_INF_B           (SF_INF_HEADER_B + SF_SET_FREQ_INF_PLD_B)
/*! Size of the acknowledgement payload in bytes. */
#define SF_SET_FREQ_ACK_PLD_B       0x00U
/*! Size of the acknowledgement in bytes. */
#define SF_SET_FREQ_ACK_B           (SF_ACK_HEADER_B + SF_SET_FREQ_ACK_PLD_B)
/*! Size of the SPI frame in bytes. It is the max. of ACK frame and I-frame. */
#define SF_SET_FREQ_FRM_B           (SF_MAX(SF_SET_FREQ_INF_B, SF_SET_FREQ_ACK_B))

/*! Frequency offset of the high byte of the FREQCON0 register (2 bytes)
 * in the payload. */
#define SF_FREQCON0_HIGH_OF         0x00U
/*! Frequency offset of the low byte of the FREQCON0 register (2 bytes)
 * in the payload. */
#define SF_FREQCON0_LOW_OF          0x01U
/*! Frequency offset of the high byte of the FREQCON1 register (2 bytes)
 * in the payload. */
#define SF_FREQCON1_HIGH_OF         0x02U
/*! Frequency offset of the low byte of the FREQCON1 register (2 bytes)
 * in the payload. */
#define SF_FREQCON1_LOW_OF          0x03U
/*! Offset of the MDES high byte. */
#define SF_MDES_HIGH_OF             0x04U
/*! Offset of the MDES low byte. */
#define SF_MDES_LOW_OF              0x05U
/*! @} */

/*! Get UL frequency command code. */
#define SF_CMD_GET_UL_FREQ_ID       0x0AU
/*! Get DL frequency command code. */
#define SF_CMD_GET_DL_FREQ_ID       0x1EU

/*!
 * @name Get UL/DL frequency SPI command (these commands have common definition).
 * @{
 */
/*! Size of the information frame payload in bytes. */
#define SF_GET_FREQ_INF_PLD_B    0x00U
/*! Size of the information frame in bytes. */
#define SF_GET_FREQ_INF_B        (SF_INF_HEADER_B + SF_GET_FREQ_INF_PLD_B)
/*! Size of the acknowledgement payload in bytes. */
#define SF_GET_FREQ_ACK_PLD_B    0x04U
/*! Size of the acknowledgement in bytes. */
#define SF_GET_FREQ_ACK_B        (SF_ACK_HEADER_B + SF_GET_FREQ_ACK_PLD_B)
/*! Size of the SPI frame in bytes. It is the max. of ACK frame and I-frame. */
#define SF_GET_FREQ_FRM_B        (SF_MAX(SF_GET_FREQ_INF_B, SF_GET_FREQ_ACK_B))
/*! @} */

/*!
 * @brief This macro returns value of the FREQCON0 register stored in the
 * payload of the acknowledgement frame.
 *
 * @param payload Pointer to the acknowledgement payload (8 bit).
 */
#define SF_GET_FREQCON0(payload) \
    (((uint16_t)(*((payload) + (SF_FREQCON0_HIGH_OF))) << 8U) | \
      (uint16_t)(*((payload) + (SF_FREQCON0_LOW_OF))))

/*!
 * @brief This macro returns value of the FREQCON1 register stored in the
 * payload of the acknowledment frame.
 *
 * @param payload Pointer to the acknowledgement payload (8 bit).
 */
#define SF_GET_FREQCON1(payload) \
    (((uint16_t)(*((payload) + (SF_FREQCON1_HIGH_OF))) << 8U) | \
      (uint16_t)(*((payload) + (SF_FREQCON1_LOW_OF))))
/*! @} */

/*!
 * @name Continuous Wave SPI command
 * @{
 */
/*! Continuous Wave command. */
#define SF_CMD_CONT_WAVE_ID         0x0BU
/*! Size of the "Continuous Wave" information frame payload in bytes. */
#define SF_CONT_WAVE_INF_PLD_B      0x01U
/*! Size of the "Continuous Wave" information frame in bytes. */
#define SF_CONT_WAVE_INF_B          (SF_INF_HEADER_B + SF_CONT_WAVE_INF_PLD_B)
/*! Size of the "Continuous Wave" acknowledgement payload in bytes. */
#define SF_CONT_WAVE_ACK_PLD_B      0x00U
/*! Size of the "Continuous Wave" acknowledgement in bytes. */
#define SF_CONT_WAVE_ACK_B          0x00U
/*! Size of the SPI frame in bytes. It is the max. of ACK frame and I-frame. */
#define SF_CONT_WAVE_FRM_B          (SF_MAX(SF_CONT_WAVE_INF_B, SF_CONT_WAVE_ACK_B))

/*! Offset of the switch enabling continuous wave. */
#define SF_CONT_WAVE_SWITCH_OF      (0x00 + SF_INF_PAYLOAD_OF)

/*! Continuous wave enabled. */
#define SF_CONT_WAVE_EN             0x01U
/*! Continuous wave disabled. */
#define SF_CONT_WAVE_DIS            0x00U
/*! @} */

/*!
 * @name Get Device Version SPI command
 * @{
 */
/*! Get Device Version command. */
#define SF_CMD_GET_DEV_VER_ID       0x0DU
/*! Size of the "Get Device Version" information frame payload in bytes. */
#define SF_GET_DEV_VER_INF_PLD_B    0x00U
/*! Size of the "Get Device Version" information frame in bytes. */
#define SF_GET_DEV_VER_INF_B        (SF_INF_HEADER_B + SF_GET_DEV_VER_INF_PLD_B)
/*! Size of the "Get Device Version" acknowledgement payload in bytes. */
#define SF_GET_DEV_VER_ACK_PLD_B    0x0FU
/*! Size of the "Get Device Version" acknowledgement in bytes. */
#define SF_GET_DEV_VER_ACK_B        (SF_ACK_HEADER_B + SF_GET_DEV_VER_ACK_PLD_B)
/*! Size of the SPI frame in bytes. It is the max. of ACK frame and I-frame. */
#define SF_GET_DEV_VER_FRM_B        (SF_MAX(SF_GET_DEV_VER_INF_B, SF_GET_DEV_VER_ACK_B))

/*! Size of the Device version field in the ACK frame. */
#define SF_DEV_VER_B                7U

/*! Offset of the Device version field in the ACK frame. */
#define SF_DEV_VER_OF               8U
/*! @} */

/*!
 * @name Trigger Watchdog SPI command, which resets the device through SW.
 * @{
 */
/*! Trigger Watchdog command. */
#define SF_CMD_TRIGGER_WD_ID        0x12U
/*! Size of the "Trigger Watchdog" information frame payload in bytes. */
#define SF_TRIGGER_WD_INF_PLD_B     0x00U
/*! Size of the "Trigger Watchdog" information frame in bytes. */
#define SF_TRIGGER_WD_INF_B         (SF_INF_HEADER_B + SF_TRIGGER_WD_INF_PLD_B)
/*! Size of the "Trigger Watchdog" acknowledgement payload in bytes. */
#define SF_TRIGGER_WD_ACK_PLD_B     0x00U
/*! Size of the "Trigger Watchdog" acknowledgement in bytes. */
#define SF_TRIGGER_WD_ACK_B         0x00U
/*! Size of the SPI frame in bytes. It is the max. of ACK frame and I-frame. */
#define SF_TRIGGER_WD_FRM_B         (SF_MAX(SF_TRIGGER_WD_INF_B, SF_TRIGGER_WD_ACK_B))
/*! @} */

/*!
 * @name Test Mode SPI command
 * @{
 */
/*! Send Test Mode command. */
#define SF_CMD_SEND_TEST_ID         0x14U
/*! Size of the "Send Test Mode" information frame payload in bytes. */
#define SF_SEND_TEST_INF_PLD_B      0x02U
/*! Size of the "Send Test Mode" information frame in bytes. */
#define SF_SEND_TEST_INF_B          (SF_INF_HEADER_B + SF_SEND_TEST_INF_PLD_B)
/*! Size of the "Send Test Mode" acknowledgement payload in bytes. */
#define SF_SEND_TEST_ACK_PLD_B      0x00U
/*! Size of the "Send Test Mode" acknowledgement in bytes. */
#define SF_SEND_TEST_ACK_B          (SF_ACK_HEADER_B + SF_SEND_TEST_ACK_PLD_B)
/*! Size of the SPI frame in bytes. It is the max. of ACK frame and I-frame. */
#define SF_SEND_TEST_FRM_B          (SF_MAX(SF_SEND_TEST_INF_B, SF_SEND_TEST_ACK_B))

/*! Offset of the Mode field (1 byte) in the Information message. */
#define SF_TEST_MODE_OF             (0U + SF_INF_PAYLOAD_OF)
/*! Offset of the Config field (1 byte) in the Information message. */
#define SF_TEST_CFG_OF              (1U + SF_INF_PAYLOAD_OF)
/*! @} */

/*!
 * @name Change To RCZx commands
 * Definition of macros for Change To RCZ1, Change To RCZ2, Change To RCZ3 and
 * Change To RCZ4 commands.
 * @{
 */
/*! Change To RCZ1 command. */
#define SF_CMD_CHANGE_TO_RCZ1_ID    0x15U
/*! Change To RCZ2 command. */
#define SF_CMD_CHANGE_TO_RCZ2_ID    0x16U
/*! Change To RCZ3 command. */
#define SF_CMD_CHANGE_TO_RCZ3_ID    0x17U
/*! Change To RCZ4 command. */
#define SF_CMD_CHANGE_TO_RCZ4_ID    0x18U
/*! Size of the "Change To RCZX" information frame payload in bytes. */
#define SF_CHANGE_TO_RCZX_INF_PLD_B 0x04U
/*! Size of the "Change To RCZX" information frame in bytes. */
#define SF_CHANGE_TO_RCZX_INF_B     (SF_INF_HEADER_B + SF_CHANGE_TO_RCZX_INF_PLD_B)
/*! Size of the "Change To RCZX" acknowledgement payload in bytes. */
#define SF_CHANGE_TO_RCZX_ACK_PLD_B 0x00U
/*! Size of the "Change To RCZX" acknowledgement in bytes. */
#define SF_CHANGE_TO_RCZX_ACK_B     (SF_ACK_HEADER_B + SF_CHANGE_TO_RCZX_ACK_PLD_B)
/*! Size of the SPI frame in bytes. It is the max. of ACK frame and I-frame. */
#define SF_CHANGE_TO_RCZX_FRM_B     SF_MAX(SF_CHANGE_TO_RCZX_INF_B, SF_CHANGE_TO_RCZX_ACK_B)

/*! Offset of the TX Attenuation steps in the I-frame payload. */
#define SF_CHANGE_TO_RCZX_TX_STEPS_INF_OF  (0x00U + SF_INF_PAYLOAD_OF)
/*! Offset of the XTAL used in the I-frame payload. */
#define SF_CHANGE_TO_RCZX_XTAL_INF_OF      (0x01U + SF_INF_PAYLOAD_OF)
/*! Offset of the TX repeat number in the I-frame payload. */
#define SF_CHANGE_TO_RCZX_TX_REPEAT_INF_OF (0x02U + SF_INF_PAYLOAD_OF)
/*! Offset of the PA type in the I-frame payload. */
#define SF_CHANGE_TO_RCZX_PA_TYPE_INF_OF   (0x03U + SF_INF_PAYLOAD_OF)
/*! @} */

/*!
 * @name Switch to private key command
 *
 * This command switches to the private key provided by SIGFOX and is used for
 * normal RF.
 * @{
 */
/*! Switch to private key command. */
#define SF_CMD_SWITCH_PRIVATE_ID    0x19U
/*! Size of the "Switch to private key" information frame payload in bytes. */
#define SF_SWITCH_PRIVATE_INF_PLD_B 0x00U
/*! Size of the "Switch to private key" information frame in bytes. */
#define SF_SWITCH_PRIVATE_INF_B     (SF_INF_HEADER_B + SF_SWITCH_PRIVATE_INF_PLD_B)
/*! Size of the "Switch to private key" acknowledgement payload in bytes. */
#define SF_SWITCH_PRIVATE_ACK_PLD_B 0x00U
/*! Size of the "Switch to private key" acknowledgement in bytes. */
#define SF_SWITCH_PRIVATE_ACK_B     (SF_ACK_HEADER_B + SF_SWITCH_PRIVATE_ACK_PLD_B)
/*! Size of the SPI frame in bytes. It is the max. of ACK frame and I-frame. */
#define SF_SWITCH_PRIVATE_FRM_B     (SF_MAX(SF_SWITCH_PRIVATE_INF_B, SF_SWITCH_PRIVATE_ACK_B))
/*! @} */

/*!
 * @name Switch to public key command
 *
 * This command switches to the public key provided by SIGFOX and is used for
 * protocol tests.
 * @{
 */
/*! Switch to public key command. */
#define SF_CMD_SWITCH_PUBLIC_ID    0x1AU
/*! Size of the "Switch to public key" information frame payload in bytes. */
#define SF_SWITCH_PUBLIC_INF_PLD_B 0x00U
/*! Size of the "Switch to public key" information frame in bytes. */
#define SF_SWITCH_PUBLIC_INF_B     (SF_INF_HEADER_B + SF_SWITCH_PUBLIC_INF_PLD_B)
/*! Size of the "Switch to public key" acknowledgement payload in bytes. */
#define SF_SWITCH_PUBLIC_ACK_PLD_B 0x00U
/*! Size of the "Switch to public key" acknowledgement in bytes. */
#define SF_SWITCH_PUBLIC_ACK_B     0x00U
/*! Size of the SPI frame in bytes. It is the max. of ACK frame and I-frame. */
#define SF_SWITCH_PUBLIC_FRM_B     (SF_MAX(SF_SWITCH_PUBLIC_INF_B, SF_SWITCH_PUBLIC_ACK_B))
/*! @} */

/*!
 * @name PSK ASK Zero Sequence command
 *
 * Sends modulated zeros at currently set frequency.
 * @{
 */
/*! PSK ASK Zero Sequence command. */
#define SF_CMD_PSK_ASK_ZERO_ID      0x1BU
/*! Size of the "PSK ASK Zero Sequence" information frame payload in bytes. */
#define SF_PSK_ASK_ZERO_INF_PLD_B   0x02U
/*! Size of the "PSK ASK Zero Sequence" information frame in bytes. */
#define SF_PSK_ASK_ZERO_INF_B       (SF_INF_HEADER_B + SF_PSK_ASK_ZERO_INF_PLD_B)
/*! Size of the "PSK ASK Zero Sequence" acknowledgement payload in bytes. */
#define SF_PSK_ASK_ZERO_ACK_PLD_B   0x00U
/*! Size of the "PSK ASK Zero Sequence" acknowledgement in bytes. */
#define SF_PSK_ASK_ZERO_ACK_B       0U
/*! Size of the SPI frame in bytes. It is the max. of ACK frame and I-frame. */
#define SF_PSK_ASK_ZERO_FRM_B       (SF_MAX(SF_PSK_ASK_ZERO_INF_B, SF_PSK_ASK_ZERO_ACK_B))

/*! Offset of the Infinite loop flag in the Information message. */
#define SF_ZERO_INFIN_LOOP_OF       (0U + SF_INF_PAYLOAD_OF)
/*! Offset of the Number of repetitions in the Information message. */
#define SF_ZERO_REPETITIONS_OF      (1U + SF_INF_PAYLOAD_OF)

/*! Max. number of repetitions. */
#define SF_ZERO_MAX_REPETITIONS     0xFFU
/*! Min. number of repetitions. */
#define SF_ZERO_MIN_REPETITIONS     0x00U

/*! Infinite loop enabled. */
#define SF_ZERO_INFIN_LOOP_EN       0x01
/*! Infinite loop disabled. */
#define SF_ZERO_INFIN_LOOP_DIS      0x00
/*! @} */

/*!
 * @name PSK ASK AAAA Sequence command
 *
 * Sends modulated AA at currently set frequency.
 * @{
 */
/*! PSK ASK AAAA Sequence command. */
#define SF_CMD_PSK_ASK_AAAA_ID      0x1CU
/*! Size of the "PSK ASK AAAA Sequence" information frame payload in bytes. */
#define SF_PSK_ASK_AAAA_INF_PLD_B   0x02U
/*! Size of the "PSK ASK AAAA Sequence" information frame in bytes. */
#define SF_PSK_ASK_AAAA_INF_B       (SF_INF_HEADER_B + SF_PSK_ASK_AAAA_INF_PLD_B)
/*! Size of the "PSK ASK AAAA Sequence" acknowledgement payload in bytes. */
#define SF_PSK_ASK_AAAA_ACK_PLD_B   0x00U
/*! Size of the "PSK ASK AAAA Sequence" acknowledgement in bytes. */
#define SF_PSK_ASK_AAAA_ACK_B       0U
/*! Size of the SPI frame in bytes. It is the max. of ACK frame and I-frame. */
#define SF_PSK_ASK_AAAA_FRM_B       (SF_MAX(SF_PSK_ASK_AAAA_INF_B, SF_PSK_ASK_AAAA_ACK_B))

/*! Offset of the Infinite loop flag in the Information message. */
#define SF_AAAA_INFIN_LOOP_OF       (0U + SF_INF_PAYLOAD_OF)
/*! Offset of the Number of repetitions in the Information message. */
#define SF_AAAA_REPETITIONS_OF      (1U + SF_INF_PAYLOAD_OF)
/*! Max. number of repetitions. */
#define SF_AAAA_MAX_REPETITIONS     0xFFU
/*! Min. number of repetitions. */
#define SF_AAAA_MIN_REPETITIONS     0x00U
/*! Infinite loop enabled. */
#define SF_AAAA_INFIN_LOOP_EN       0x01
/*! Infinite loop disabled. */
#define SF_AAAA_INFIN_LOOP_DIS      0x00
/*! @} */

/*!
 * @name Set FCC macro channel command
 * @{
 */
/*! Set FCC macro channel command. */
#define SF_CMD_SET_FCC_CH_ID        0x1FU
/*! Size of the "Set FCC macro channel" information frame payload in bytes. */
#define SF_SET_FCC_CH_INF_PLD_B     0x0DU
/*! Size of the "Set FCC macro channel" information frame in bytes. */
#define SF_SET_FCC_CH_INF_B         (SF_INF_HEADER_B + SF_SET_FCC_CH_INF_PLD_B)
/*! Size of the "Set FCC macro channel" acknowledgement payload in bytes. */
#define SF_SET_FCC_CH_ACK_PLD_B     0x00U
/*! Size of the "Set FCC macro channel" acknowledgement in bytes. */
#define SF_SET_FCC_CH_ACK_B         (SF_ACK_HEADER_B + SF_SET_FCC_CH_ACK_PLD_B)
/*! Size of the SPI frame in bytes. It is the max. of ACK frame and I-frame. */
#define SF_SET_FCC_CH_FRM_B         (SF_MAX(SF_SET_FCC_CH_INF_B, SF_SET_FCC_CH_ACK_B))

/*! Size of the macro channel configuration array (frequency hopping) in bytes. */
#define SF_MACRO_CH_CFG_B           0x0CU
#define SF_DEFAULT_CHANNEL_OF       (0x0CU + SF_INF_PAYLOAD_OF)
/*! @} */

/*!
 * @name Get FCC macro channel usage command
 *
 * Gets the FCC Macro channel usage
 * @{
 */
/*! Get FCC macro channel usage command. */
#define SF_CMD_GET_FCC_CH_ID        0x20U
/*! Size of the "Get FCC macro channel usage" information frame payload in bytes. */
#define SF_GET_FCC_CH_INF_PLD_B     0x00U
/*! Size of the "Get FCC macro channel usage" information frame in bytes. */
#define SF_GET_FCC_CH_INF_B         (SF_INF_HEADER_B + SF_GET_FCC_CH_INF_PLD_B)
/*! Size of the "Get FCC macro channel usage" acknowledgement payload in bytes. */
#define SF_GET_FCC_CH_ACK_PLD_B     0x01U
/*! Size of the "Get FCC macro channel usage" acknowledgement in bytes. */
#define SF_GET_FCC_CH_ACK_B         (SF_ACK_HEADER_B + SF_GET_FCC_CH_ACK_PLD_B)
/*! Size of the SPI frame in bytes. It is the max. of ACK frame and I-frame. */
#define SF_GET_FCC_CH_FRM_B         (SF_MAX(SF_GET_FCC_CH_INF_B, SF_GET_FCC_CH_ACK_B))

/*! Offset of the FCC Macro channel usage in the payload. */
#define SF_FCC_CH_OF                (0x00U + SF_ACK_PAYLOAD_OF)

/*! Mask of the Number of free micro channels in current FCC macro channel
 * (applicable only in FCC mode). */
#define SF_FCC_CH_FCC_FREE_MCH_MASK 0xF0U
/*! Mask of the Number of sent frames (applicable only in ARIB mode). */
#define SF_ARIB_CH_FCC_SENT_FRAMES_MASK 0x07U
/*! Mask of the Carrier sense attempts (applicable only in ARIB mode). */
#define SF_ARIB_CH_FCC_CS_ATTEPTS_MASK 0xF8U
/*! @} */

/*!
 * @name Reset FCC macro channel command
 *
 * Resets the FCC Macro Channel.
 * @{
 */
/*! Reset FCC macro channel command. */
#define SF_CMD_RESET_FCC_CH_ID      0x21U
/*! Size of the "Reset FCC macro channel" information frame payload in bytes. */
#define SF_RESET_FCC_CH_INF_PLD_B   0x00U
/*! Size of the "Reset FCC macro channel" information frame in bytes. */
#define SF_RESET_FCC_CH_INF_B       (SF_INF_HEADER_B + SF_RESET_FCC_CH_INF_PLD_B)
/*! Size of the "Reset FCC macro channel" acknowledgement payload in bytes. */
#define SF_RESET_FCC_CH_ACK_PLD_B   0x00U
/*! Size of the "Reset FCC macro channel" acknowledgement in bytes. */
#define SF_RESET_FCC_CH_ACK_B       (SF_ACK_HEADER_B + SF_RESET_FCC_CH_ACK_PLD_B)
/*! Size of the SPI frame in bytes. It is the max. of ACK frame and I-frame. */
#define SF_RESET_FCC_CH_FRM_B       (SF_MAX(SF_RESET_FCC_CH_INF_B, SF_RESET_FCC_CH_ACK_B))
/*! @} */

/*!
 * @name Static frequency calibration command
 *
 * The passed drift offset measured in Hz is added or substracted to the LO when
 * doing a RF TX.
 * @{
 */
/*! Static frequency calibration command. */
#define SF_CMD_STATIC_FREQ_CAL_ID   0x22U
/*! Size of the "Static frequency calibration" information frame payload in bytes. */
#define SF_STATIC_FREQ_CAL_INF_PLD_B 0x05U
/*! Size of the "Static frequency calibration" information frame in bytes. */
#define SF_STATIC_FREQ_CAL_INF_B    (SF_INF_HEADER_B + SF_STATIC_FREQ_CAL_INF_PLD_B)
/*! Size of the "Static frequency calibration" acknowledgement payload in bytes. */
#define SF_STATIC_FREQ_CAL_ACK_PLD_B 0x00U
/*! Size of the "Static frequency calibration" acknowledgement in bytes. */
#define SF_STATIC_FREQ_CAL_ACK_B    (SF_ACK_HEADER_B + SF_STATIC_FREQ_CAL_ACK_PLD_B)
/*! Size of the SPI frame in bytes. It is the max. of ACK frame and I-frame. */
#define SF_STATIC_FREQ_CAL_FRM_B    (SF_MAX(SF_STATIC_FREQ_CAL_INF_B, SF_STATIC_FREQ_CAL_ACK_B))

/*! Offset of the sign in the payload. */
#define SF_STATIC_FREQ_CAL_SIGN_OF  (0x00U + SF_INF_PAYLOAD_OF)
/*! Offset of the drift (Hz) in the payload (byte[0] is MSB). */
#define SF_STATIC_FREQ_CAL_DRIFT_OF (0x01U + SF_INF_PAYLOAD_OF)

/*! Positive drift sign (drift added to LO). */
#define SF_DRIFT_POSITIVE           1U
/*! Negative drift sign (drift subtracted from LO). */
#define SF_DRIFT_NEGATIVE           2U
/*! @} */

/*!
 * @name Temperature frequency calibration command
 *
 * This command will import into the OL2385 a crystal dependent drift table.
 * @{
 */
/*! Temperature frequency calibration command. */
#define SF_CMD_TEMP_FREQ_CAL_ID     0x23U
/*! Size of the "Temperature frequency calibration" information frame payload in bytes. */
#define SF_TEMP_FREQ_CAL_INF_PLD_B  0x1BU
/*! Size of the "Temperature frequency calibration" information frame in bytes. */
#define SF_TEMP_FREQ_CAL_INF_B      (SF_INF_HEADER_B + SF_TEMP_FREQ_CAL_INF_PLD_B)
/*! Size of the "Temperature frequency calibration" acknowledgement payload in bytes. */
#define SF_TEMP_FREQ_CAL_ACK_PLD_B  0x1AU
/*! Size of the "Temperature frequency calibration" acknowledgement in bytes. */
#define SF_TEMP_FREQ_CAL_ACK_B      (SF_ACK_HEADER_B + SF_TEMP_FREQ_CAL_ACK_PLD_B)
/*! Size of the SPI frame in bytes. It is the max. of ACK frame and I-frame. */
#define SF_TEMP_FREQ_CAL_FRM_B      (SF_MAX(SF_TEMP_FREQ_CAL_INF_B, SF_TEMP_FREQ_CAL_ACK_B))

/*! Offset of the sub-command in the I-frame payload. */
#define SF_TEMP_FREQ_CAL_SUBCMD_INF_OF  (0x00U + SF_INF_PAYLOAD_OF)
/*! Offset of the sub-command in the I-frame payload. */
#define SF_TEMP_FREQ_CAL_TEMP_INF_OF    (0x01U + SF_INF_PAYLOAD_OF)

/*! Offset of the sub-command in the ACK-frame payload. */
#define SF_TEMP_FREQ_CAL_TEMP_ACK_OF    (0x00U + SF_ACK_PAYLOAD_OF)

/*! Read current table values sub-command. */
#define SF_SUBCMD_READ_TBL          0x00U
/*! Update table sub-command. */
#define SF_SUBCMD_UPDATE_TBL        0x01U
/*! Set default table sub-command. */
#define SF_SUBCMD_SET_DEFAULT_TBL   0x02U

/*! Size of the temperature calibration table in bytes. */
#define SF_TEMP_FREQ_CAL_TBL_SIZE   26U
/*! @} */

/*!
 * @name Get last sent RSSI command
 *
 * This command delivers the recently computed RSSI measurement
 * or resets buffer which holds the recently computed RSSI measurements.
 * @{
 */
/*! Get last sent RSSI command. */
#define SF_CMD_GET_LAST_SENT_RSSI_ID  0x24U
/*! Size of the "Get last sent RSSI" information frame payload in bytes (0x00 or 0x01). */
#define SF_GET_LAST_SENT_RSSI_INF_PLD_B 0x01U
/*! Size of the "Get last sent RSSI" information frame in bytes. */
#define SF_GET_LAST_SENT_RSSI_INF_B   (SF_INF_HEADER_B + SF_GET_LAST_SENT_RSSI_INF_PLD_B)
/*! Size of the "Get last sent RSSI" acknowledgement payload in bytes (0x01 or 0x1F). */
#define SF_GET_LAST_SENT_RSSI_ACK_PLD_B   0x1FU
/*! Size of the "Get last sent RSSI" acknowledgement in bytes (0x4 or 0x22). */
#define SF_GET_LAST_SENT_RSSI_ACK_B       (SF_ACK_HEADER_B + SF_GET_LAST_SENT_RSSI_ACK_PLD_B)
/*! Size of the SPI frame in bytes. It is the max. of ACK frame and I-frame. */
#define SF_GET_LAST_SENT_RSSI_FRM_B       (SF_MAX(SF_GET_LAST_SENT_RSSI_INF_B, SF_GET_LAST_SENT_RSSI_ACK_B))

/*! Offset of the sub-command in the I-frame payload. */
#define SF_GET_LAST_SENT_RSSI_SUBCMD_INF_OF  (0x00U + SF_INF_PAYLOAD_OF)

/*! Delivers the recently computed RSSI measured by OL2385 and sent to SIGFOX network filled in OOB message sub-command. */
#define SF_SUBCMD_GET_LAST_RSSI       0x00U
/*! Delivers the recently computed RSSI measured by OL2385 for 30 frames during RX GFSK test sub-command. */
#define SF_SUBCMD_GET_LAST_30_RSSI    0x01U
/*! Resets buffer which holds the recently computed RSSI measured by OL2385 for 30 frames during RX GFSK test sub-command. */
#define SF_SUBCMD_RESET_RSSI_BUFFER   0x02U

/*! Offset of the RSSI measure in the payload. */
#define SF_SENT_RSSI_DATA_OF          (0x00U + SF_ACK_PAYLOAD_OF)
/*! Offset of the RSSI measure over 30 frames in the payload. */
#define SF_SENT_RSSI_30_DATA_OF       (0x01U + SF_ACK_PAYLOAD_OF)

/*! Size of the recently computed RSSI for 30 frames in bytes. */
#define SF_SENT_RSSI_TBL_SIZE   30U
/*! @} */

/*!
 * @name Update PA curve command
 *
 * This command reads or updates or sets default PA table for desired
 * network standard.
 * @{
 */
/*! Update PA curve command.  */
#define SF_CMD_UPDATE_PA_CURVE_ID 0x25U
/*! Size of the "Update PA curve" information frame payload in bytes (0x01 or 0xC9). */
#define SF_UPDATE_PA_CURVE_INF_PLD_B 0xC9U
/*! Size of the "Update PA curve" information frame in bytes (0x03 or 0xCB). */
#define SF_UPDATE_PA_CURVE_INF_B (SF_INF_HEADER_B + SF_UPDATE_PA_CURVE_INF_PLD_B)
/*! Size of the "Update PA curve" acknowledgement payload in bytes (0x00 or 0xC8). */
#define SF_UPDATE_PA_CURVE_ACK_PLD_B 0xC8U
/*! Size of the "Update PA curve" acknowledgement in bytes (0x03 or 0xCB). */
#define SF_UPDATE_PA_CURVE_ACK_B (SF_ACK_HEADER_B + SF_UPDATE_PA_CURVE_ACK_PLD_B)
/*! Size of the SPI frame in bytes. It is the max. of ACK frame and I-frame. */
#define SF_UPDATE_PA_CURVE_FRM_B    (SF_MAX(SF_UPDATE_PA_CURVE_INF_B, SF_UPDATE_PA_CURVE_ACK_B))

/*! SPI command PA table size (ETSI/FCC independent). */
#define SF_PA_TBL_SIZE       200
/*! ETSI PA table size. */
#define SF_PA_TBL_ETSI_SIZE  200
/*! FCC PA table size. */
#define SF_PA_TBL_FCC_SIZE   86

/*! Read current ETSI PA table sub-command. */
#define SF_SUBCMD_READ_ETSI_PA_TABLE       0x00U
/*! Update ETSI PA table sub-command. */
#define SF_SUBCMD_UPDATE_ETSI_PA_TABLE     0x01U
/*! Set default ETSI PA table sub-command. */
#define SF_SUBCMD_DEFAULT_ETSI_PA_TABLE    0x02U
/*! Read current FCC PA table sub-command. */
#define SF_SUBCMD_READ_FCC_PA_TABLE        0x03U
/*! Update FCC PA table sub-command. */
#define SF_SUBCMD_UPDATE_FCC_PA_TABLE      0x04U
/*! Set default FCC PA table sub-command. */
#define SF_SUBCMD_DEFAULT_FCC_PA_TABLE     0x05U

/*! Offset of the sub-command in the I-frame payload. */
#define SF_UPDATE_PA_CURVE_SUBCMD_INF_OF  (0x00U + SF_INF_PAYLOAD_OF)
/*! Offset of the curve values in the I-frame payload. */
#define SF_UPDATE_PA_CURVE_CURVE_INF_OF   (0x01U + SF_INF_PAYLOAD_OF)

/*! Offset of the PA curve in the ACK-frame payload. */
#define SF_SENT_PA_CURVE_OF               (0x00U + SF_ACK_PAYLOAD_OF)
/*! @} */

/*!
 * @name Initial RSSI calibration command
 *
 * This command sets the initial RSSI drift offset.
 * @{
 */
/*! Initial RSSI calibration command. */
#define SF_CMD_INIT_RSSI_CALIB_ID 0x26U
/*! Size of the "Initial RSSI calibration" information frame payload in bytes. */
#define SF_INIT_RSSI_CALIB_INF_PLD_B 0x03U
/*! Size of the "Initial RSSI calibration" information frame in bytes. */
#define SF_INIT_RSSI_CALIB_INF_B (SF_INF_HEADER_B + SF_INIT_RSSI_CALIB_INF_PLD_B)
/*! Size of the "Initial RSSI calibration" acknowledgement payload in bytes. */
#define SF_INIT_RSSI_CALIB_ACK_PLD_B 0x00U
/*! Size of the "Initial RSSI calibration" acknowledgement in bytes. */
#define SF_INIT_RSSI_CALIB_ACK_B (SF_ACK_HEADER_B + SF_INIT_RSSI_CALIB_ACK_PLD_B)
/*! Size of the SPI frame in bytes. It is the max. of ACK frame and I-frame. */
#define SF_INIT_RSSI_CALIB_FRM_B (SF_MAX(SF_INIT_RSSI_CALIB_INF_B, SF_INIT_RSSI_CALIB_ACK_B))

/*! Offset of the sign in the I-frame payload. */
#define SF_INIT_RSSI_CALIB_SIGN_INF_OF       (0x00U + SF_INF_PAYLOAD_OF)
/*! Offset of the RSSI offset MSB in the I-frame payload. */
#define SF_INIT_RSSI_CALIB_RSSI_MSB_INF_OF   (0x01U + SF_INF_PAYLOAD_OF)
/*! Offset of the RSSI offset LSB in the I-frame payload. */
#define SF_INIT_RSSI_CALIB_RSSI_LSB_INF_OF   (0x02U + SF_INF_PAYLOAD_OF)

/*! Positive RSSI drift sign. */
#define SF_INIT_RSSI_POSITIVE           1U
/*! Negative RSSI drift sign. */
#define SF_INIT_RSSI_NEGATIVE           2U
/*! @} */

#endif /* SOURCE_SF_OL23XX_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
