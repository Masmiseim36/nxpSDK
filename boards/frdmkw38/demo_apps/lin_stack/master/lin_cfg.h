/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIN_CFG_H
#define LIN_CFG_H

#include <stdint.h>
#include "lin_driver.h"

/* SUPPORT_MASTER_MODE_Class */
#define SUPPORT_MASTER_MODE 1U /* Equal to 1 when have at least one master interface */
/* SUPPORT_SLAVE_MODE_Class */
#define SUPPORT_SLAVE_MODE 0U /* Equal to 1 when have at least one slave interface */
/* SPPORT_PROTOCOL_21_Class */
#define SUPPORT_PROTOCOL_21 1U /* Equal to 1 when have at least one LIN2.1 or LIN2.2 interface */
/* SUPPORT_PROTOCOL_J2602_Class */
#define SUPPORT_PROTOCOL_J2602 0U /* Equal to 1 when have at least one J2602 interface */
/* SUPPORT_DIAG_CLASS_III_Class */
#define SUPPORT_DIAG_CLASS_III 0U /* Equal to 1 when have at least one ifc with diagnostic class III */
/* SUPPORT_DIAG_CLASS_II_Class */
#define SUPPORT_DIAG_CLASS_II 0U /* Equal to 1 when have at least one ifc with diagnostic class II or III */
/* SUPPORT_TRANSPORT_LAYER_Class */
#define SUPPORT_TRANSPORT_LAYER 1U /* Equal to 1 to use transport layer */
/* SUPPORT_DIAG_SERVICE_Class */
#define SUPPORT_DIAG_SERVICE 1U /* Equal to 1 to use diagnostic service layer */
/*Timer service period. */
#define TIME_OUT_UNIT_US 500U
/* Hardware instance count. */
#define HARDWARE_INSTANCE_COUNT 2U
/* Numbers of IFCS. */
#define LIN_NUM_OF_IFCS 1U
#define LIN_NUM_OF_SLAVE_IFCS 0U
#define LIN_NUM_OF_MASTER_IFCS 1U

/* Frame buffer size. */
#define LIN_FRAME_BUF_SIZE 28U
#define LIN_FLAG_BUF_SIZE 7U

/* LPUART clock frequency. */
#define LIN_LPUART_CLOCK_FREQ CLOCK_GetOsc0ErClkFreq()
/* Definition macro for LIN stack used. */
#define DEV_ASSERT assert
#define STATUS_SUCCESS LIN_SUCCESS

/* Timer service call back. */
extern void timerGetTimeIntervalCallback0(uint32_t *ns);

/* LIN Interface. */
typedef enum
{
    LI0         = 0x00U,
    INVALID_IFC = 0xFFU
} l_ifc_handle;

typedef enum
{
    INVALID_SLAVE_INDEX = 0xFFU
} l_ifc_slave_handle;
typedef enum
{
    LI0_Master           = 0x00U,
    INVALID_MASTER_INDEX = 0xFFU
} l_ifc_master_handle;

/* Number of signals */
#define LIN_NUM_OF_SIGS 14U

/* List of signals */
typedef enum
{
    /* Interface_name = LI0 */
    LI0_Motor1ErrorCode,
    LI0_Motor1ErrorValue,
    LI0_Motor1LinError,
    LI0_Motor1Selection,
    LI0_Motor1Temp,
    LI0_Motor2ErrorCode,
    LI0_Motor2ErrorValue,
    LI0_Motor2LinError,
    LI0_Motor2Selection,
    LI0_Motor2Temp,
    LI0_MotorDirection,
    LI0_MotorSpeed,
    LI0_Motor1Position,
    LI0_Motor2Position
} l_signal_handle;

/* Number of frames */
#define LIN_NUM_OF_FRMS 10U

/* List of frames */
typedef enum
{
    /* Interface_name = LI0 */
    LI0_Motor1Control,
    LI0_Motor1State_Cycl,
    LI0_Motor1State_Event,
    LI0_Motor2Control,
    LI0_Motor2State_Cycl,
    LI0_Motor2State_Event,
    LI0_MotorsControl,
    LI0_SporadicControlFrame,
    LI0_MasterReq,
    LI0_SlaveResp
} l_frame_handle;

/* Number of schedule tables */
#define LIN_NUM_OF_SCHD_TBL 5U

/* List of schedules */
typedef enum
{
    /* Interface_name = LI0 */
    LI0_LIN_NULL_SCHEDULE,
    LI0_GOTO_SLEEP_SCHEDULE,
    LI0_MasterReqTable,
    LI0_SlaveRespTable,
    LI0_NormalTable
} l_schedule_handle;

/* Size of configuration in ROM and RAM used for interface: LI0 */
#define LI0_LIN_SIZE_OF_CFG 12U

/*!
 * @breief l_bool_rd
 * This function return value of bool signal
 */
#define l_bool_rd(SIGNAL) l_bool_rd_##SIGNAL()

/*!
 * @brief l_bool_wr
 * This function write value A to bool signal
 */
#define l_bool_wr(SIGNAL, A) l_bool_wr_##SIGNAL(A)

/*!
 * @brief l_u8_rd
 * This function read data from (1-8 bit)signal
 */
#define l_u8_rd(SIGNAL) l_u8_rd_##SIGNAL()

/*!
 * @brief l_u8_wr
 * This this function write data to (1-8 bit)signal
 */
#define l_u8_wr(SIGNAL, A) l_u8_wr_##SIGNAL(A)

/*!
 * @brief l_u16_rd
 * This function read data from (8-16 bit)signal
 */
#define l_u16_rd(SIGNAL) l_u16_rd_##SIGNAL()

/*!
 * @brief l_u16_wr
 * This function write data to (8-16 bit) signal
 */
#define l_u16_wr(SIGNAL, A) l_u16_wr_##SIGNAL(A)

/*!
 * @brief l_bytes_rd
 * This function read data from bytes signal
 */
#define l_bytes_rd(SIGNAL, start, count, data) l_bytes_rd_##SIGNAL(start, count, data)

/*!
 * @brief l_bytes_wr
 * This function write data to bytes signal
 */
#define l_bytes_wr(SIGNAL, start, count, data) l_bytes_wr_##SIGNAL(start, count, data)

/*!
 * @brief l_flg_tst
 * This function return value of flag which related with signal
 */
#define l_flg_tst(FLAG) l_flg_tst_##FLAG()

/*!
 * @brief l_flg_clr
 * This function clear flag which related with signal
 */
#define l_flg_clr(FLAG) l_flg_clr_##FLAG()

/* Macro for bit operation. */
#define LIN_TEST_BIT(A, B) ((l_bool)((((A) & (1U << (B))) != 0U) ? 1U : 0U))
#define LIN_SET_BIT(A, B) ((A) |= (l_u8)(1U << (B)))
#define LIN_CLEAR_BIT(A, B) ((A) &= ((l_u8)(~(1U << (B)))))
#define LIN_BYTE_MASK ((l_u16)(((l_u16)((l_u16)1 << CHAR_BIT)) - (l_u16)1))

/* Returns the low byte of the 32-bit value    */
#define BYTE_0(n) ((l_u8)((n) & (l_u8)0xFF))
/* Returns the second byte of the 32-bit value */
#define BYTE_1(n) ((l_u8)(BYTE_0((n) >> (l_u8)8)))
/* Returns the third byte of the 32-bit value  */
#define BYTE_2(n) ((l_u8)(BYTE_0((n) >> (l_u8)16)))
/* Returns high byte of the 32-bit value       */
#define BYTE_3(n) ((l_u8)(BYTE_0((n) >> (l_u8)24)))

/* Definition access. */
#define LIN_LI0_Motor1ErrorCode_BYTE_OFFSET 8U
#define LIN_LI0_Motor1ErrorCode_BIT_OFFSET 0U
#define LIN_LI0_Motor1ErrorCode_SIGNAL_SIZE 8U
#define LIN_LI0_Motor1ErrorCode_FLAG_BYTE_OFFSET 2U
#define LIN_LI0_Motor1ErrorCode_FLAG_BIT_OFFSET 0U

#define LIN_LI0_Motor1ErrorValue_BYTE_OFFSET 9U
#define LIN_LI0_Motor1ErrorValue_BIT_OFFSET 0U
#define LIN_LI0_Motor1ErrorValue_SIGNAL_SIZE 8U
#define LIN_LI0_Motor1ErrorValue_FLAG_BYTE_OFFSET 2U
#define LIN_LI0_Motor1ErrorValue_FLAG_BIT_OFFSET 1U

#define LIN_LI0_Motor1LinError_BYTE_OFFSET 6U
#define LIN_LI0_Motor1LinError_BIT_OFFSET 0U
#define LIN_LI0_Motor1LinError_SIGNAL_SIZE 1U
#define LIN_LI0_Motor1LinError_FLAG_BYTE_OFFSET 1U
#define LIN_LI0_Motor1LinError_FLAG_BIT_OFFSET 2U

#define LIN_LI0_Motor1Selection_BYTE_OFFSET 0U
#define LIN_LI0_Motor1Selection_BIT_OFFSET 0U
#define LIN_LI0_Motor1Selection_SIGNAL_SIZE 2U
#define LIN_LI0_Motor1Selection_FLAG_BYTE_OFFSET 0U
#define LIN_LI0_Motor1Selection_FLAG_BIT_OFFSET 0U

#define LIN_LI0_Motor1Temp_BYTE_OFFSET 1U
#define LIN_LI0_Motor1Temp_BIT_OFFSET 0U
#define LIN_LI0_Motor1Temp_SIGNAL_SIZE 8U
#define LIN_LI0_Motor1Temp_FLAG_BYTE_OFFSET 1U
#define LIN_LI0_Motor1Temp_FLAG_BIT_OFFSET 0U

#define LIN_LI0_Motor2ErrorCode_BYTE_OFFSET 18U
#define LIN_LI0_Motor2ErrorCode_BIT_OFFSET 0U
#define LIN_LI0_Motor2ErrorCode_SIGNAL_SIZE 8U
#define LIN_LI0_Motor2ErrorCode_FLAG_BYTE_OFFSET 5U
#define LIN_LI0_Motor2ErrorCode_FLAG_BIT_OFFSET 0U

#define LIN_LI0_Motor2ErrorValue_BYTE_OFFSET 19U
#define LIN_LI0_Motor2ErrorValue_BIT_OFFSET 0U
#define LIN_LI0_Motor2ErrorValue_SIGNAL_SIZE 8U
#define LIN_LI0_Motor2ErrorValue_FLAG_BYTE_OFFSET 5U
#define LIN_LI0_Motor2ErrorValue_FLAG_BIT_OFFSET 1U

#define LIN_LI0_Motor2LinError_BYTE_OFFSET 16U
#define LIN_LI0_Motor2LinError_BIT_OFFSET 0U
#define LIN_LI0_Motor2LinError_SIGNAL_SIZE 1U
#define LIN_LI0_Motor2LinError_FLAG_BYTE_OFFSET 4U
#define LIN_LI0_Motor2LinError_FLAG_BIT_OFFSET 2U

#define LIN_LI0_Motor2Selection_BYTE_OFFSET 10U
#define LIN_LI0_Motor2Selection_BIT_OFFSET 0U
#define LIN_LI0_Motor2Selection_SIGNAL_SIZE 2U
#define LIN_LI0_Motor2Selection_FLAG_BYTE_OFFSET 3U
#define LIN_LI0_Motor2Selection_FLAG_BIT_OFFSET 0U

#define LIN_LI0_Motor2Temp_BYTE_OFFSET 11U
#define LIN_LI0_Motor2Temp_BIT_OFFSET 0U
#define LIN_LI0_Motor2Temp_SIGNAL_SIZE 8U
#define LIN_LI0_Motor2Temp_FLAG_BYTE_OFFSET 4U
#define LIN_LI0_Motor2Temp_FLAG_BIT_OFFSET 0U

#define LIN_LI0_MotorDirection_BYTE_OFFSET 20U
#define LIN_LI0_MotorDirection_BIT_OFFSET 0U
#define LIN_LI0_MotorDirection_SIGNAL_SIZE 2U
#define LIN_LI0_MotorDirection_FLAG_BYTE_OFFSET 6U
#define LIN_LI0_MotorDirection_FLAG_BIT_OFFSET 0U

#define LIN_LI0_MotorSpeed_BYTE_OFFSET 20U
#define LIN_LI0_MotorSpeed_BIT_OFFSET 2U
#define LIN_LI0_MotorSpeed_SIGNAL_SIZE 10U
#define LIN_LI0_MotorSpeed_FLAG_BYTE_OFFSET 6U
#define LIN_LI0_MotorSpeed_FLAG_BIT_OFFSET 1U

#define LIN_LI0_Motor1Position_BYTE_OFFSET 2U
#define LIN_LI0_Motor1Position_BIT_OFFSET 0U
#define LIN_LI0_Motor1Position_SIGNAL_SIZE 32U
#define LIN_LI0_Motor1Position_FLAG_BYTE_OFFSET 1U
#define LIN_LI0_Motor1Position_FLAG_BIT_OFFSET 1U

#define LIN_LI0_Motor2Position_BYTE_OFFSET 12U
#define LIN_LI0_Motor2Position_BIT_OFFSET 0U
#define LIN_LI0_Motor2Position_SIGNAL_SIZE 32U
#define LIN_LI0_Motor2Position_FLAG_BYTE_OFFSET 4U
#define LIN_LI0_Motor2Position_FLAG_BIT_OFFSET 1U

/* Static access macros for signal LI0_Motor1ErrorCode */
#define l_u8_rd_LI0_Motor1ErrorCode() \
    ((l_u8)(((g_lin_frame_data_buffer[LIN_LI0_Motor1ErrorCode_BYTE_OFFSET]) >> 0U) & 0xffU))
#define l_u8_wr_LI0_Motor1ErrorCode(A)                                                                              \
    {                                                                                                               \
        g_lin_frame_data_buffer[LIN_LI0_Motor1ErrorCode_BYTE_OFFSET] =                                              \
            (l_u8)((g_lin_frame_data_buffer[LIN_LI0_Motor1ErrorCode_BYTE_OFFSET] & 0x00U) | (((A) << 0U) & 0xffU)); \
        LIN_CLEAR_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor1ErrorCode_FLAG_BYTE_OFFSET],                              \
                      LIN_LI0_Motor1ErrorCode_FLAG_BIT_OFFSET);                                                     \
    }

/* Static access macros for signal LI0_Motor1ErrorValue */
#define l_u8_rd_LI0_Motor1ErrorValue() \
    ((l_u8)(((g_lin_frame_data_buffer[LIN_LI0_Motor1ErrorValue_BYTE_OFFSET]) >> 0U) & 0xffU))
#define l_u8_wr_LI0_Motor1ErrorValue(A)                                                                              \
    {                                                                                                                \
        g_lin_frame_data_buffer[LIN_LI0_Motor1ErrorValue_BYTE_OFFSET] =                                              \
            (l_u8)((g_lin_frame_data_buffer[LIN_LI0_Motor1ErrorValue_BYTE_OFFSET] & 0x00U) | (((A) << 0U) & 0xffU)); \
        LIN_CLEAR_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor1ErrorValue_FLAG_BYTE_OFFSET],                              \
                      LIN_LI0_Motor1ErrorValue_FLAG_BIT_OFFSET);                                                     \
    }

/* Static access macros for signal LI0_Motor1LinError */
#define l_bool_rd_LI0_Motor1LinError() \
    (LIN_TEST_BIT(g_lin_frame_data_buffer[LIN_LI0_Motor1LinError_BYTE_OFFSET], LIN_LI0_Motor1LinError_BIT_OFFSET))
#define l_bool_wr_LI0_Motor1LinError(A)                                                   \
    {                                                                                     \
        (A) ? (LIN_SET_BIT(g_lin_frame_data_buffer[LIN_LI0_Motor1LinError_BYTE_OFFSET],   \
                           LIN_LI0_Motor1LinError_BIT_OFFSET)) :                          \
              (LIN_CLEAR_BIT(g_lin_frame_data_buffer[LIN_LI0_Motor1LinError_BYTE_OFFSET], \
                             LIN_LI0_Motor1LinError_BIT_OFFSET));                         \
        LIN_CLEAR_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor1LinError_FLAG_BYTE_OFFSET],     \
                      LIN_LI0_Motor1LinError_FLAG_BIT_OFFSET);                            \
    }

/* Static access macros for signal LI0_Motor1Selection */
#define l_u8_rd_LI0_Motor1Selection() \
    ((l_u8)(((g_lin_frame_data_buffer[LIN_LI0_Motor1Selection_BYTE_OFFSET]) >> 0U) & 0x03U))
#define l_u8_wr_LI0_Motor1Selection(A)                                                                              \
    {                                                                                                               \
        g_lin_frame_data_buffer[LIN_LI0_Motor1Selection_BYTE_OFFSET] =                                              \
            (l_u8)((g_lin_frame_data_buffer[LIN_LI0_Motor1Selection_BYTE_OFFSET] & 0xfcU) | (((A) << 0U) & 0x03U)); \
        LIN_CLEAR_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor1Selection_FLAG_BYTE_OFFSET],                              \
                      LIN_LI0_Motor1Selection_FLAG_BIT_OFFSET);                                                     \
    }

/* Static access macros for signal LI0_Motor1Temp */
#define l_u8_rd_LI0_Motor1Temp() ((l_u8)(((g_lin_frame_data_buffer[LIN_LI0_Motor1Temp_BYTE_OFFSET]) >> 0U) & 0xffU))
#define l_u8_wr_LI0_Motor1Temp(A)                                                                                      \
    {                                                                                                                  \
        g_lin_frame_data_buffer[LIN_LI0_Motor1Temp_BYTE_OFFSET] =                                                      \
            (l_u8)((g_lin_frame_data_buffer[LIN_LI0_Motor1Temp_BYTE_OFFSET] & 0x00U) | (((A) << 0U) & 0xffU));         \
        LIN_CLEAR_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor1Temp_FLAG_BYTE_OFFSET], LIN_LI0_Motor1Temp_FLAG_BIT_OFFSET); \
    }

/* Static access macros for signal LI0_Motor2ErrorCode */
#define l_u8_rd_LI0_Motor2ErrorCode() \
    ((l_u8)(((g_lin_frame_data_buffer[LIN_LI0_Motor2ErrorCode_BYTE_OFFSET]) >> 0U) & 0xffU))
#define l_u8_wr_LI0_Motor2ErrorCode(A)                                                                              \
    {                                                                                                               \
        g_lin_frame_data_buffer[LIN_LI0_Motor2ErrorCode_BYTE_OFFSET] =                                              \
            (l_u8)((g_lin_frame_data_buffer[LIN_LI0_Motor2ErrorCode_BYTE_OFFSET] & 0x00U) | (((A) << 0U) & 0xffU)); \
        LIN_CLEAR_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor2ErrorCode_FLAG_BYTE_OFFSET],                              \
                      LIN_LI0_Motor2ErrorCode_FLAG_BIT_OFFSET);                                                     \
    }

/* Static access macros for signal LI0_Motor2ErrorValue */
#define l_u8_rd_LI0_Motor2ErrorValue() \
    ((l_u8)(((g_lin_frame_data_buffer[LIN_LI0_Motor2ErrorValue_BYTE_OFFSET]) >> 0U) & 0xffU))
#define l_u8_wr_LI0_Motor2ErrorValue(A)                                                                              \
    {                                                                                                                \
        g_lin_frame_data_buffer[LIN_LI0_Motor2ErrorValue_BYTE_OFFSET] =                                              \
            (l_u8)((g_lin_frame_data_buffer[LIN_LI0_Motor2ErrorValue_BYTE_OFFSET] & 0x00U) | (((A) << 0U) & 0xffU)); \
        LIN_CLEAR_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor2ErrorValue_FLAG_BYTE_OFFSET],                              \
                      LIN_LI0_Motor2ErrorValue_FLAG_BIT_OFFSET);                                                     \
    }

/* Static access macros for signal LI0_Motor2LinError */
#define l_bool_rd_LI0_Motor2LinError() \
    (LIN_TEST_BIT(g_lin_frame_data_buffer[LIN_LI0_Motor2LinError_BYTE_OFFSET], LIN_LI0_Motor2LinError_BIT_OFFSET))
#define l_bool_wr_LI0_Motor2LinError(A)                                                   \
    {                                                                                     \
        (A) ? (LIN_SET_BIT(g_lin_frame_data_buffer[LIN_LI0_Motor2LinError_BYTE_OFFSET],   \
                           LIN_LI0_Motor2LinError_BIT_OFFSET)) :                          \
              (LIN_CLEAR_BIT(g_lin_frame_data_buffer[LIN_LI0_Motor2LinError_BYTE_OFFSET], \
                             LIN_LI0_Motor2LinError_BIT_OFFSET));                         \
        LIN_CLEAR_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor2LinError_FLAG_BYTE_OFFSET],     \
                      LIN_LI0_Motor2LinError_FLAG_BIT_OFFSET);                            \
    }

/*Static access macros for signal LI0_Motor2Selection */
#define l_u8_rd_LI0_Motor2Selection() \
    ((l_u8)(((g_lin_frame_data_buffer[LIN_LI0_Motor2Selection_BYTE_OFFSET]) >> 0U) & 0x03U))
#define l_u8_wr_LI0_Motor2Selection(A)                                                                              \
    {                                                                                                               \
        g_lin_frame_data_buffer[LIN_LI0_Motor2Selection_BYTE_OFFSET] =                                              \
            (l_u8)((g_lin_frame_data_buffer[LIN_LI0_Motor2Selection_BYTE_OFFSET] & 0xfcU) | (((A) << 0U) & 0x03U)); \
        LIN_CLEAR_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor2Selection_FLAG_BYTE_OFFSET],                              \
                      LIN_LI0_Motor2Selection_FLAG_BIT_OFFSET);                                                     \
    }

/* Static access macros for signal LI0_Motor2Temp */
#define l_u8_rd_LI0_Motor2Temp() ((l_u8)(((g_lin_frame_data_buffer[LIN_LI0_Motor2Temp_BYTE_OFFSET]) >> 0U) & 0xffU))
#define l_u8_wr_LI0_Motor2Temp(A)                                                                                      \
    {                                                                                                                  \
        g_lin_frame_data_buffer[LIN_LI0_Motor2Temp_BYTE_OFFSET] =                                                      \
            (l_u8)((g_lin_frame_data_buffer[LIN_LI0_Motor2Temp_BYTE_OFFSET] & 0x00U) | (((A) << 0U) & 0xffU));         \
        LIN_CLEAR_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor2Temp_FLAG_BYTE_OFFSET], LIN_LI0_Motor2Temp_FLAG_BIT_OFFSET); \
    }

/* Static access macros for signal LI0_MotorDirection */
#define l_u8_rd_LI0_MotorDirection() \
    ((l_u8)(((g_lin_frame_data_buffer[LIN_LI0_MotorDirection_BYTE_OFFSET]) >> 0U) & 0x03U))
#define l_u8_wr_LI0_MotorDirection(A)                                                                              \
    {                                                                                                              \
        g_lin_frame_data_buffer[LIN_LI0_MotorDirection_BYTE_OFFSET] =                                              \
            (l_u8)((g_lin_frame_data_buffer[LIN_LI0_MotorDirection_BYTE_OFFSET] & 0xfcU) | (((A) << 0U) & 0x03U)); \
        LIN_CLEAR_BIT(g_lin_flag_handle_tbl[LIN_LI0_MotorDirection_FLAG_BYTE_OFFSET],                              \
                      LIN_LI0_MotorDirection_FLAG_BIT_OFFSET);                                                     \
    }

/* Static access macros for signal LI0_MotorSpeed */
#define l_u16_rd_LI0_MotorSpeed()                                                       \
    ((l_u16)(((g_lin_frame_data_buffer[LIN_LI0_MotorSpeed_BYTE_OFFSET] +                \
               (g_lin_frame_data_buffer[LIN_LI0_MotorSpeed_BYTE_OFFSET + 1U] << 8U)) >> \
              2U) &                                                                     \
             0x3ffU))
#define l_u16_wr_LI0_MotorSpeed(A)                                                                                     \
    {                                                                                                                  \
        g_buffer_backup_data[0U] = g_lin_frame_data_buffer[LIN_LI0_MotorSpeed_BYTE_OFFSET];                            \
        g_lin_frame_updating_flag_tbl[LI0_MotorsControl] |= (1U << 0);                                                 \
        g_lin_frame_data_buffer[LIN_LI0_MotorSpeed_BYTE_OFFSET] =                                                      \
            (l_u8)((g_lin_frame_data_buffer[LIN_LI0_MotorSpeed_BYTE_OFFSET] & 0x03U) | (((A) << 2U) & 0xfcU));         \
        g_buffer_backup_data[0U + 1U] = g_lin_frame_data_buffer[LIN_LI0_MotorSpeed_BYTE_OFFSET + 1U];                  \
        g_lin_frame_updating_flag_tbl[LI0_MotorsControl] |= (1U << (0 + 1U));                                          \
        g_lin_frame_data_buffer[LIN_LI0_MotorSpeed_BYTE_OFFSET + 1U] =                                                 \
            (l_u8)((g_lin_frame_data_buffer[LIN_LI0_MotorSpeed_BYTE_OFFSET + 1U] & 0xf0U) | (((A) >> 6U) & 0x0fU));    \
        g_lin_frame_updating_flag_tbl[LI0_MotorsControl] &= (~(0x03 << 0));                                            \
        LIN_CLEAR_BIT(g_lin_flag_handle_tbl[LIN_LI0_MotorSpeed_FLAG_BYTE_OFFSET], LIN_LI0_MotorSpeed_FLAG_BIT_OFFSET); \
    }

/* Static access macros for signal LI0_Motor1Position */
#define l_bytes_rd_LI0_Motor1Position(start, count, data)                                          \
    {                                                                                              \
        l_u8 i;                                                                                    \
        for (i = 0; i < (count); ++i)                                                              \
            (data)[i] = g_lin_frame_data_buffer[LIN_LI0_Motor1Position_BYTE_OFFSET + i + (start)]; \
    }
#define l_bytes_wr_LI0_Motor1Position(start, count, data)                                              \
    {                                                                                                  \
        l_u8 i;                                                                                        \
        for (i = 0; i < (count); ++i)                                                                  \
        {                                                                                              \
            g_buffer_backup_data[i + (start) + 1] =                                                    \
                g_lin_frame_data_buffer[LIN_LI0_Motor1Position_BYTE_OFFSET + i + (start)];             \
            g_lin_frame_updating_flag_tbl[LI0_Motor1State_Cycl] |= (1 << (i + (start) + 1));           \
            g_lin_frame_data_buffer[LIN_LI0_Motor1Position_BYTE_OFFSET + i + (start)] = (data)[i];     \
        }                                                                                              \
        g_lin_frame_updating_flag_tbl[LI0_Motor1State_Cycl] &= (~(((1 << count) - 1) << (start + 1))); \
        LIN_CLEAR_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor1Position_FLAG_BYTE_OFFSET],                  \
                      LIN_LI0_Motor1Position_FLAG_BIT_OFFSET);                                         \
    }
/* Static access macros for signal LI0_Motor2Position */
#define l_bytes_rd_LI0_Motor2Position(start, count, data)                                          \
    {                                                                                              \
        l_u8 i;                                                                                    \
        for (i = 0; i < (count); ++i)                                                              \
            (data)[i] = g_lin_frame_data_buffer[LIN_LI0_Motor2Position_BYTE_OFFSET + i + (start)]; \
    }
#define l_bytes_wr_LI0_Motor2Position(start, count, data)                                              \
    {                                                                                                  \
        l_u8 i;                                                                                        \
        for (i = 0; i < (count); ++i)                                                                  \
        {                                                                                              \
            g_buffer_backup_data[i + (start) + 1] =                                                    \
                g_lin_frame_data_buffer[LIN_LI0_Motor2Position_BYTE_OFFSET + i + (start)];             \
            g_lin_frame_updating_flag_tbl[LI0_Motor2State_Cycl] |= (1 << (i + (start) + 1));           \
            g_lin_frame_data_buffer[LIN_LI0_Motor2Position_BYTE_OFFSET + i + (start)] = (data)[i];     \
        }                                                                                              \
        g_lin_frame_updating_flag_tbl[LI0_Motor2State_Cycl] &= (~(((1 << count) - 1) << (start + 1))); \
        LIN_CLEAR_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor2Position_FLAG_BYTE_OFFSET],                  \
                      LIN_LI0_Motor2Position_FLAG_BIT_OFFSET);                                         \
    }

/* Signal flag APIs */
#define l_flg_tst_LI0_Motor1ErrorCode_flag()                                      \
    LIN_TEST_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor1ErrorCode_FLAG_BYTE_OFFSET], \
                 LIN_LI0_Motor1ErrorCode_FLAG_BIT_OFFSET)
#define l_flg_clr_LI0_Motor1ErrorCode_flag()                                       \
    LIN_CLEAR_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor1ErrorCode_FLAG_BYTE_OFFSET], \
                  LIN_LI0_Motor1ErrorCode_FLAG_BIT_OFFSET)

#define l_flg_tst_LI0_Motor1ErrorValue_flag()                                      \
    LIN_TEST_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor1ErrorValue_FLAG_BYTE_OFFSET], \
                 LIN_LI0_Motor1ErrorValue_FLAG_BIT_OFFSET)
#define l_flg_clr_LI0_Motor1ErrorValue_flag()                                       \
    LIN_CLEAR_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor1ErrorValue_FLAG_BYTE_OFFSET], \
                  LIN_LI0_Motor1ErrorValue_FLAG_BIT_OFFSET)

#define l_flg_tst_LI0_Motor1LinError_flag() \
    LIN_TEST_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor1LinError_FLAG_BYTE_OFFSET], LIN_LI0_Motor1LinError_FLAG_BIT_OFFSET)
#define l_flg_clr_LI0_Motor1LinError_flag()                                       \
    LIN_CLEAR_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor1LinError_FLAG_BYTE_OFFSET], \
                  LIN_LI0_Motor1LinError_FLAG_BIT_OFFSET)

#define l_flg_tst_LI0_Motor1Selection_flag()                                      \
    LIN_TEST_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor1Selection_FLAG_BYTE_OFFSET], \
                 LIN_LI0_Motor1Selection_FLAG_BIT_OFFSET)
#define l_flg_clr_LI0_Motor1Selection_flag()                                       \
    LIN_CLEAR_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor1Selection_FLAG_BYTE_OFFSET], \
                  LIN_LI0_Motor1Selection_FLAG_BIT_OFFSET)

#define l_flg_tst_LI0_Motor1Temp_flag() \
    LIN_TEST_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor1Temp_FLAG_BYTE_OFFSET], LIN_LI0_Motor1Temp_FLAG_BIT_OFFSET)
#define l_flg_clr_LI0_Motor1Temp_flag() \
    LIN_CLEAR_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor1Temp_FLAG_BYTE_OFFSET], LIN_LI0_Motor1Temp_FLAG_BIT_OFFSET)

#define l_flg_tst_LI0_Motor2ErrorCode_flag()                                      \
    LIN_TEST_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor2ErrorCode_FLAG_BYTE_OFFSET], \
                 LIN_LI0_Motor2ErrorCode_FLAG_BIT_OFFSET)
#define l_flg_clr_LI0_Motor2ErrorCode_flag()                                       \
    LIN_CLEAR_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor2ErrorCode_FLAG_BYTE_OFFSET], \
                  LIN_LI0_Motor2ErrorCode_FLAG_BIT_OFFSET)

#define l_flg_tst_LI0_Motor2ErrorValue_flag()                                      \
    LIN_TEST_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor2ErrorValue_FLAG_BYTE_OFFSET], \
                 LIN_LI0_Motor2ErrorValue_FLAG_BIT_OFFSET)
#define l_flg_clr_LI0_Motor2ErrorValue_flag()                                       \
    LIN_CLEAR_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor2ErrorValue_FLAG_BYTE_OFFSET], \
                  LIN_LI0_Motor2ErrorValue_FLAG_BIT_OFFSET)

#define l_flg_tst_LI0_Motor2LinError_flag() \
    LIN_TEST_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor2LinError_FLAG_BYTE_OFFSET], LIN_LI0_Motor2LinError_FLAG_BIT_OFFSET)
#define l_flg_clr_LI0_Motor2LinError_flag()                                       \
    LIN_CLEAR_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor2LinError_FLAG_BYTE_OFFSET], \
                  LIN_LI0_Motor2LinError_FLAG_BIT_OFFSET)

#define l_flg_tst_LI0_Motor2Selection_flag()                                      \
    LIN_TEST_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor2Selection_FLAG_BYTE_OFFSET], \
                 LIN_LI0_Motor2Selection_FLAG_BIT_OFFSET)
#define l_flg_clr_LI0_Motor2Selection_flag()                                       \
    LIN_CLEAR_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor2Selection_FLAG_BYTE_OFFSET], \
                  LIN_LI0_Motor2Selection_FLAG_BIT_OFFSET)

#define l_flg_tst_LI0_Motor2Temp_flag() \
    LIN_TEST_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor2Temp_FLAG_BYTE_OFFSET], LIN_LI0_Motor2Temp_FLAG_BIT_OFFSET)
#define l_flg_clr_LI0_Motor2Temp_flag() \
    LIN_CLEAR_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor2Temp_FLAG_BYTE_OFFSET], LIN_LI0_Motor2Temp_FLAG_BIT_OFFSET)

#define l_flg_tst_LI0_MotorDirection_flag() \
    LIN_TEST_BIT(g_lin_flag_handle_tbl[LIN_LI0_MotorDirection_FLAG_BYTE_OFFSET], LIN_LI0_MotorDirection_FLAG_BIT_OFFSET)
#define l_flg_clr_LI0_MotorDirection_flag()                                       \
    LIN_CLEAR_BIT(g_lin_flag_handle_tbl[LIN_LI0_MotorDirection_FLAG_BYTE_OFFSET], \
                  LIN_LI0_MotorDirection_FLAG_BIT_OFFSET)

#define l_flg_tst_LI0_MotorSpeed_flag() \
    LIN_TEST_BIT(g_lin_flag_handle_tbl[LIN_LI0_MotorSpeed_FLAG_BYTE_OFFSET], LIN_LI0_MotorSpeed_FLAG_BIT_OFFSET)
#define l_flg_clr_LI0_MotorSpeed_flag() \
    LIN_CLEAR_BIT(g_lin_flag_handle_tbl[LIN_LI0_MotorSpeed_FLAG_BYTE_OFFSET], LIN_LI0_MotorSpeed_FLAG_BIT_OFFSET)

#define l_flg_tst_LI0_Motor1Position_flag() \
    LIN_TEST_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor1Position_FLAG_BYTE_OFFSET], LIN_LI0_Motor1Position_FLAG_BIT_OFFSET)
#define l_flg_clr_LI0_Motor1Position_flag()                                       \
    LIN_CLEAR_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor1Position_FLAG_BYTE_OFFSET], \
                  LIN_LI0_Motor1Position_FLAG_BIT_OFFSET)

#define l_flg_tst_LI0_Motor2Position_flag() \
    LIN_TEST_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor2Position_FLAG_BYTE_OFFSET], LIN_LI0_Motor2Position_FLAG_BIT_OFFSET)
#define l_flg_clr_LI0_Motor2Position_flag()                                       \
    LIN_CLEAR_BIT(g_lin_flag_handle_tbl[LIN_LI0_Motor2Position_FLAG_BYTE_OFFSET], \
                  LIN_LI0_Motor2Position_FLAG_BIT_OFFSET)

/* Interface_name = LI0 */
#define l_flg_tst_LI0_Motor1Control_flag() g_lin_frame_flag_handle_tbl[LI0_Motor1Control]
#define l_flg_clr_LI0_Motor1Control_flag() g_lin_frame_flag_handle_tbl[LI0_Motor1Control] = 0

#define l_flg_tst_LI0_Motor1State_Cycl_flag() g_lin_frame_flag_handle_tbl[LI0_Motor1State_Cycl]
#define l_flg_clr_LI0_Motor1State_Cycl_flag() g_lin_frame_flag_handle_tbl[LI0_Motor1State_Cycl] = 0

#define l_flg_tst_LI0_Motor1State_Event_flag() g_lin_frame_flag_handle_tbl[LI0_Motor1State_Event]
#define l_flg_clr_LI0_Motor1State_Event_flag() g_lin_frame_flag_handle_tbl[LI0_Motor1State_Event] = 0

#define l_flg_tst_LI0_Motor2Control_flag() g_lin_frame_flag_handle_tbl[LI0_Motor2Control]
#define l_flg_clr_LI0_Motor2Control_flag() g_lin_frame_flag_handle_tbl[LI0_Motor2Control] = 0

#define l_flg_tst_LI0_Motor2State_Cycl_flag() g_lin_frame_flag_handle_tbl[LI0_Motor2State_Cycl]
#define l_flg_clr_LI0_Motor2State_Cycl_flag() g_lin_frame_flag_handle_tbl[LI0_Motor2State_Cycl] = 0

#define l_flg_tst_LI0_Motor2State_Event_flag() g_lin_frame_flag_handle_tbl[LI0_Motor2State_Event]
#define l_flg_clr_LI0_Motor2State_Event_flag() g_lin_frame_flag_handle_tbl[LI0_Motor2State_Event] = 0

#define l_flg_tst_LI0_MotorsControl_flag() g_lin_frame_flag_handle_tbl[LI0_MotorsControl]
#define l_flg_clr_LI0_MotorsControl_flag() g_lin_frame_flag_handle_tbl[LI0_MotorsControl] = 0

#define l_flg_tst_LI0_SporadicControlFrame_flag() g_lin_frame_flag_handle_tbl[LI0_SporadicControlFrame]
#define l_flg_clr_LI0_SporadicControlFrame_flag() g_lin_frame_flag_handle_tbl[LI0_SporadicControlFrame] = 0

#define l_flg_tst_LI0_MasterReq_flag() g_lin_frame_flag_handle_tbl[LI0_MasterReq]
#define l_flg_clr_LI0_MasterReq_flag() g_lin_frame_flag_handle_tbl[LI0_MasterReq] = 0

#define l_flg_tst_LI0_SlaveResp_flag() g_lin_frame_flag_handle_tbl[LI0_SlaveResp]
#define l_flg_clr_LI0_SlaveResp_flag() g_lin_frame_flag_handle_tbl[LI0_SlaveResp] = 0

/*!
 * @brief l_sch_tick_interface
 * This static function call to dynamic function l_sch_tick()
 */
#define l_sch_tick_LI0() l_sch_tick(LI0)

/*!
 * @brief l_sch_set_interface
 * This static function call to dynamic function l_sch_set()
 */
#define l_sch_set_LI0(schedule, entry) l_sch_set(LI0, schedule, entry)

/*!
 * @brief l_ifc_init_interface
 * This static function call to dynamic function l_ifc_init()
 */
#define l_ifc_init_LI0() l_ifc_init(LI0)

/*!
 * @brief l_ifc_goto_sleep_interface
 * This static function call to dynamic function l_ifc_goto_sleep()
 */
#define l_ifc_goto_sleep_LI0() l_ifc_goto_sleep(LI0)

/*!
 * @brief l_ifc_wake_up_interface
 * This static function call to dynamic function l_ifc_wake_up()
 */
#define l_ifc_wake_up_LI0() l_ifc_wake_up(LI0)

/*!
 * @brief l_ifc_read_status_interface
 * This static function call to dynamic function l_ifc_read_status()
 */
#define l_ifc_read_status_LI0() l_ifc_read_status(LI0)

#define LI0_DIAG_NUMBER_OF_SERVICES 5U
#define LI0_DIAGSRV_ASSIGN_NAD_ORDER 0U
#define LI0_DIAGSRV_READ_BY_IDENTIFIER_ORDER 1U
#define LI0_DIAGSRV_CONDITIONAL_CHANGE_NAD_ORDER 2U
#define LI0_DIAGSRV_SAVE_CONFIGURATION_ORDER 3U
#define LI0_DIAGSRV_ASSIGN_FRAME_ID_RANGE_ORDER 4U
#define MAX_LENGTH_SERVICE 6

/* Extern user LIN configuration. */
extern lin_user_config_t lin1_InitConfig0;

#endif /* _LIN_CFG_H_ */
