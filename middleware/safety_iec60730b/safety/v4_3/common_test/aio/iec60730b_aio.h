/*
 * Copyright 2015 Freescale Semiconductor, Inc.
 * Copyright 2015-2021 NXP.
 *
 * License: LA_OPT_NXP_Software_License
 *
 * This software is owned or controlled by NXP and may
 * only be used strictly in accordance with the applicable license terms.
 * By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that
 * you have read, and that you agree to comply with and are bound by,
 * such license terms. If you do not agree to be bound by the applicable
 * license terms, then you may not retain, install, activate or otherwise
 * use the software.
 *
 * @version 4.3
 *
 * @brief Analog I/O test common routines - IEC60730 Class B.
 */

#ifndef IEC60730B_AIO_H_
#define IEC60730B_AIO_H_

#include "iec60730b_types.h"

/*
 * List of devices and supported functions can be found in library user guide,
 * section Core self test library - source code version.
 */

/******************************************************************************
 * Macros
 ******************************************************************************/
/* Return values */
#define FS_FAIL_AIO          (FS_AIO_CODE + 0x1U)
#define FS_AIO_PROGRESS      (FS_AIO_CODE + 0x2U)
#define FS_AIO_INIT          (FS_AIO_CODE + 0x3U)
#define FS_AIO_SCAN_COMPLETE (FS_AIO_CODE + 0x4U)




/* Assembler skips following section */
#ifndef __ASM__

/******************************************************************************
 * Definitions
 ******************************************************************************/
typedef struct
{
    uint32_t low;
    uint32_t high;
} fs_aio_limits_t;

typedef struct
{
    uint8_t  AdcChannel;
    uint16_t commandBuffer;
    uint8_t  SideSelect; /* 0 = A side, 1 = B side*/
    uint8_t  softwareTriggerEvent;
    fs_aio_limits_t Limits;
    uint32_t   RawResult;
    FS_RESULT  state;
} fs_aio_test_a1_t;

typedef struct
{
    uint8_t AdcChannel;
    fs_aio_limits_t Limits;
    uint32_t RawResult;
    FS_RESULT state;
} fs_aio_test_a2346_t;

typedef struct
{
    uint8_t AdcChannel;
    uint8_t sequence;
    fs_aio_limits_t Limits;
    uint32_t RawResult;
    FS_RESULT state;
} fs_aio_test_a5_t;


typedef struct
{
    uint8_t AdcChannel;
    uint8_t Sample;
    fs_aio_limits_t Limits;
    uint32_t RawResult;
    FS_RESULT state;
} fs_aio_test_a7_t;


/******************************************************************************
 * Functions
 ******************************************************************************/

/*******************************************************************************
 *
 * @brief    The function check if the value "RawResult" fits into predefined limits.
 *
 * @param    RawResult - input argument is rawconversion value from test item which is tested over limits.
 * @param    *pLimits - input argument is pointer to fs_aio_limits_t structure from analog test item.
 * @param    *pState - input argument is pointer to state variable of analog test intem
 *
 * @return   FS_PASS         - successfull execution of test (all channels tested).
 *           FS_FAIL_AIO     - converted value doesn't fit into limits.
 *           In others case, function has no effect and return state of input parameter pState
 *
 ******************************************************************************/
FS_RESULT FS_AIO_LimitCheck(uint32_t RawResult, fs_aio_limits_t *pLimits, FS_RESULT *pState );

/*******************************************************************************
 *
 * @brief    The function sets up the ADC input channel and triggers the conversion.
 *           Then state is changed to FS_AIO_PROGRESS.
 *
 *           This function sets input only if pObj->state == FS_AIO_INIT.
 *
 * @param    *pObj - input argument is pointer to analog test instance.
 * @param    *pAdc - input argument is pointer to fs_aio_a4_t type of analog converter.
 *
 * @return   FS_AIO_PROGRESS - required return value. It Means that input was set.
 *           Any other value - the function had no effect.
 *
 ******************************************************************************/
FS_RESULT FS_AIO_InputSet_A4(fs_aio_test_a2346_t *pObj, fs_aio_a4_t *pAdc);

/*******************************************************************************
 *
 * @brief    The function reads converted analog value
 *
 * @param    *pObj - input argument is pointer to fs_aio_test_a2346_t test instance.
 * @param    *pAdc - input argument is pointer to fs_aio_a4_t type of analog converter.
 *
 * @return   FS_AIO_SCAN_COMPLETE - result register was readed
 *           Others: pObj->state - the function had no effect.
 *
 ******************************************************************************/
FS_RESULT FS_AIO_ReadResult_A4(fs_aio_test_a2346_t *pObj, fs_aio_a4_t *pAdc);


/*******************************************************************************
 *
 * @brief    The function sets up the ADC input channel and triggers the conversion.
 *           Then state is changed to FS_AIO_PROGRESS.
 *
 *           This function sets input only if pObj->state == FS_AIO_INIT.
 *
 * @param    *pObj - input argument is pointer to analog test instance.
 * @param    *pAdc - input argument is pointer to fs_aio_a23_t type of analog converter.
 *
 * @return   FS_AIO_PROGRESS - required return value. It Means that input was set.
 *           Any other value - the function had no effect.
 *
 ******************************************************************************/
FS_RESULT FS_AIO_InputSet_A23(fs_aio_test_a2346_t *pObj, fs_aio_a23_t *pAdc);

/*******************************************************************************
 *
 * @brief    The function reads converted analog value
 *
 * @param    *pObj - input argument is pointer to fs_aio_test_a2346_t test instance.
 * @param    *pAdc - input argument is pointer to fs_aio_a23_t type of analog converter.
 *
 * @return   FS_AIO_SCAN_COMPLETE - result register was readed
 *           Others: pObj->state - the function had no effect.
 *
 ******************************************************************************/
FS_RESULT FS_AIO_ReadResult_A23(fs_aio_test_a2346_t *pObj, fs_aio_a23_t *pAdc);

/*******************************************************************************
 *
 * @brief    The function sets up the ADC input channel and triggers the conversion.
 *           Then state is changed to FS_AIO_PROGRESS.
 *
 *           This function sets input only if pObj->state == FS_AIO_INIT.
 *
 * @param    *pObj - input argument is pointer to analog test instance.
 * @param    *pAdc - input argument is pointer to fs_aio_a6_t type of analog converter.
 *
 * @return   FS_AIO_PROGRESS - required return value. It Means that input was set.
 *           Any other value - the function had no effect.
 *
 ******************************************************************************/
FS_RESULT FS_AIO_InputSet_A6(fs_aio_test_a2346_t *pObj, fs_aio_a6_t *pAdc);

/*******************************************************************************
 *
 * @brief    The function reads converted analog value
 *
 * @param    *pObj - input argument is pointer to fs_aio_test_a2346_t test instance.
 * @param    *pAdc - input argument is pointer to fs_aio_a6_t type of analog converter.
 *
 * @return   FS_AIO_SCAN_COMPLETE - result register was readed
 *           Others: pObj->state - the function had no effect.
 *
 ******************************************************************************/
FS_RESULT FS_AIO_ReadResult_A6(fs_aio_test_a2346_t *pObj, fs_aio_a6_t *pAdc);

/*******************************************************************************
 *
 * @brief    The function sets up the ADC input channel and triggers the conversion.
 *           Then state is changed to FS_AIO_PROGRESS.
 *
 *           This function sets input only if pObj->state == FS_AIO_INIT.
 *
 * @param    *pObj - input argument is pointer to analog test instance.
 * @param    *pAdc - input argument is pointer to fs_aio_a1_t type of analog converter.
 *
 * @return   FS_AIO_PROGRESS - required return value. It Means that input was set.
 *           Any other value - the function had no effect.
 *
 ******************************************************************************/
FS_RESULT FS_AIO_InputSet_A1(fs_aio_test_a1_t *pObj, fs_aio_a1_t *pAdc);

/*******************************************************************************
 *
 * @brief    The function reads converted analog value
 *
 * @param    *pObj - input argument is pointer to fs_aio_test_a1_t test instance.
 * @param    *pAdc - input argument is pointer to fs_aio_a1_t type of analog converter.
 *
 * @return   FS_AIO_SCAN_COMPLETE - result register was readed
 *           Others: pObj->state - the function had no effect.
 *
 ******************************************************************************/
FS_RESULT FS_AIO_ReadResult_A1(fs_aio_test_a1_t *pObj, fs_aio_a1_t *pAdc);


/*******************************************************************************
 *
 * @brief    The function sets up the ADC input channel and triggers the conversion.
 *           Then state is changed to FS_AIO_PROGRESS.
 *
 *           This function sets input only if pObj->state == FS_AIO_INIT.
 *
 * @param    *pObj - input argument is pointer to analog test instance.
 * @param    *pAdc - input argument is pointer to fs_aio_a5_t type of analog converter.
 *
 * @return   FS_AIO_PROGRESS - required return value. It Means that input was set.
 *           Any other value - the function had no effect.
 *
 ******************************************************************************/
FS_RESULT FS_AIO_InputSet_A5(fs_aio_test_a5_t *pObj, fs_aio_a5_t *pAdc);

/*******************************************************************************
 *
 * @brief    The function reads converted analog value
 *
 * @param    *pObj - input argument is pointer to fs_aio_test_a5_t test instance.
 * @param    *pAdc - input argument is pointer to fs_aio_a5_t type of analog converter.
 *
 * @return   FS_AIO_SCAN_COMPLETE - result register was readed
 *           Others: pObj->state - the function had no effect.
 *
 ******************************************************************************/
FS_RESULT FS_AIO_ReadResult_A5(fs_aio_test_a5_t *pObj, fs_aio_a5_t *pAdc);

/*******************************************************************************
 *
 * @brief    The function sets up the ADC input channel and triggers the conversion.
 *           Then state is changed to FS_AIO_PROGRESS.
 *
 *           This function sets input only if pObj->state == FS_AIO_INIT.
 *
 * @param    *pObj - input argument is pointer to analog test instance.
 * @param    *pAdc - input argument is pointer to fs_aio_a7_t type of analog converter.
 *
 * @return   FS_AIO_PROGRESS - required return value. It Means that input was set.
 *           Any other value - the function had no effect.
 *
 ******************************************************************************/
FS_RESULT FS_AIO_InputSet_A7(fs_aio_test_a7_t *pObj, fs_aio_a7_t *pAdc);

/*******************************************************************************
 *
 * @brief    The function reads converted analog value
 *
 * @param    *pObj - input argument is pointer to fs_aio_test_a7_t test instance.
 * @param    *pAdc - input argument is pointer to fs_aio_a7_t type of analog converter.
 *
 * @return   FS_AIO_SCAN_COMPLETE - result register was readed
 *           Others: pObj->state - the function had no effect.
 *
 ******************************************************************************/
FS_RESULT FS_AIO_ReadResult_A7(fs_aio_test_a7_t *pObj, fs_aio_a7_t *pAdc);


















#endif /* __ASM__ */
#endif /* IEC60730B_AIO_H_ */
