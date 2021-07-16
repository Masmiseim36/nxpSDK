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
 * @version 4.1
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
#define FS_FAIL_AIO     (FS_AIO_CODE + 0x1U)
#define FS_AIO_PROGRESS (FS_AIO_CODE + 0x2U)
#define FS_AIO_INIT     (FS_AIO_CODE + 0x3U)
#define FS_AIO_START    (FS_AIO_CODE + 0x4U)

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
    fs_aio_limits_t *pLimits;
    FS_RESULT state;
    uint8_t *pInputs;
    uint8_t *pSamples; /* MDK - For Cyclic ADC we need also SAMPLE number */
    uint16_t result[3];
    uint16_t fifoCounter;
    uint16_t comandSourceFlag;
    uint16_t commandBuffer;
    uint8_t sequence;
    uint8_t cnt;
    uint8_t cntMax;
    uint8_t fifoNumber;
} fs_aio_test_t;

/******************************************************************************
 * Functions
 ******************************************************************************/
/*******************************************************************************
 *
 * @brief    This function initializes one instance of the Analog input test.
 *
 * @param    *pObj    - input argument is pointer to analog test instance.
 * @param    *pLimits - the input argument is a pointer to the array of limits used in the test.
 * @param    *pInputs - the input argument is a pointer to the array of input numbers used in the test.
 * @param    *cntMax  - the input argument is the size of the input and the limits arrays.
 *
 * @return   void
 *
 ******************************************************************************/
void FS_AIO_InputInit(fs_aio_test_t *pObj, fs_aio_limits_t *pLimits, uint8_t *pInputs, uint8_t cntMax);

/*******************************************************************************
 *
 * @brief    This function initializes one instance of the Analog input test.
 *
 * @param    *pObj     - input argument is pointer to analog test instance.
 * @param    *pLimits  - the input argument is a pointer to the array of limits used in the test.
 * @param    *pInputs  - the input argument is a pointer to the array of input numbers used in the test.
 * @param    cntMax    - the input argument is the size of the input and the limits arrays.
 * @param    cmdBuffer - specify the command buffer for conversion. Has effect only in case of k3s devices.
 *
 * @return   void
 *
 ******************************************************************************/
void FS_AIO_InputInit_K3S(
    fs_aio_test_t *pObj, fs_aio_limits_t *pLimits, uint8_t *pInputs, uint8_t cntMax, uint8_t cmdBuffer);

/*******************************************************************************
 *
 * @brief    This function initializes one instance of the Analog input test.
 *
 * @param    *pObj     - input argument is pointer to analog test instance.
 * @param    *pLimits  - the input argument is a pointer to the array of limits used in the test.
 * @param    *pInputs  - the input argument is a pointer to the array of input numbers used in the test.
 * @param    *pSamples - the input argument is a pointer to the array of sample number used in the test.
 * @param    cntMax    - the input argument is the size of the input and the limits arrays.
 *
 * @return   void
 *
 ******************************************************************************/
void FS_AIO_InputInit_CYCLIC(
    fs_aio_test_t *pObj, fs_aio_limits_t *pLimits, uint8_t *pInputs, uint8_t *pSamples, uint8_t cntMax);

/*******************************************************************************
 *
 * @brief    This function initializes one instance of the Analog input test.
 *
 * @param    *pObj    - input argument is pointer to analog test instance.
 * @param    *pLimits - the input argument is a pointer to the array of limits used in the test.
 * @param    *pInputs - the input argument is a pointer to the array of input numbers used in the test.
 * @param    cntMax   - the input argument is the size of the input and the limits arrays.
 * @param    sequence - no effect, just because of compatibility.
 * @param    fifo     - the index of the used FIFO for result.
 *
 * @return   void
 *
 ******************************************************************************/
void FS_AIO_InputInit_LPC_ADC16(
    fs_aio_test_t *pObj, fs_aio_limits_t *pLimits, uint8_t *pInputs, uint8_t cntMax, uint8_t sequence, uint8_t fifo);

/*******************************************************************************
 *
 * @brief    This function initializes one instance of the Analog input test.
 *
 * @param    *pObj    - input argument is pointer to analog test instance.
 * @param    *pLimits - the input argument is a pointer to the array of limits used in the test.
 * @param    *pInputs - the input argument is a pointer to the array of input numbers used in the test.
 * @param    cntMax   - the input argument is the size of the input and the limits arrays.
 * @param    sequence - the input argument is index of used sequence.
 *
 * @return   void
 *
 ******************************************************************************/
void FS_AIO_InputInit_LPC_ADC12(
    fs_aio_test_t *pObj, fs_aio_limits_t *pLimits, uint8_t *pInputs, uint8_t cntMax, uint8_t sequence);

/*******************************************************************************
 *
 * @brief    This function initializes one instance of the Analog input test.
 *
 * @param    *pObj    - input argument is pointer to analog test instance.
 * @param    *pLimits - the input argument is a pointer to the array of limits used in the test.
 * @param    *pInputs - the input argument is a pointer to the array of input numbers used in the test.
 * @param    cntMax   - the input argument is the size of the input and the limits arrays.
 * @param    commandBuffer - the input argument is number of command
 * @param    fifoNumber - the input argument is number of trigger register
 * @param    sequence - the input argument is index of used sequence - trigger source.
 *
 * @return   void
 *
 ******************************************************************************/
void FS_AIO_InputInit_IMXRT117X(fs_aio_test_t *pObj,
                                fs_aio_limits_t *pLimits,
                                uint8_t *pInputs,
                                uint8_t cntMax,
                                uint16_t commandBuffer,
                                uint8_t fifo,
                                uint8_t sequence);

/*******************************************************************************
 *
 * @brief    This function sets up the analog input test to start the
 *           execution of the test (sets state FS_AIO_START for pObj).
 *
 * @param    *pObj - input argument is pointer to analog test instance.
 *
 * @return   void
 *
 ******************************************************************************/
void FS_AIO_InputTrigger(fs_aio_test_t *pObj);

/*******************************************************************************
 *
 * @brief    The function sets up the ADC input channel and triggers the conversion.
 *           Then state is changed to FS_AIO_PROGRESS.
 *
 *           This function sets input only if pObj->state == FS_AIO_START.
 *
 * @param    *pObj - input argument is pointer to analog test instance.
 * @param    *pAdc - input argument is pointer of analog converter.
 *
 * @return   FS_AIO_PROGRESS - required return value. It Means that input was set.
 *           Any other value - the function had no effect.
 *
 ******************************************************************************/
FS_RESULT FS_AIO_InputSet(fs_aio_test_t *pObj, fs_aio_t *pAdc);

/*******************************************************************************
 *
 * @brief    The function sets up the ADC input channel and triggers the conversion.
 *           Then state is changed to FS_AIO_PROGRESS.
 *
 *           This function sets input only if pObj->state == FS_AIO_START.
 *
 * @param    *pObj - input argument is pointer to analog test instance.
 * @param    *pAdc - input argument is pointer of analog converter.
 *
 * @return   FS_AIO_PROGRESS - required return value. It Means that input was set.
 *           Any other value - the function had no effect.
 *
 ******************************************************************************/
FS_RESULT FS_AIO_InputSet_KE(fs_aio_test_t *pObj, fs_aio_ke_t *pAdc);

/*******************************************************************************
 *
 * @brief    The function sets up the ADC input channel and triggers the conversion.
 *           Then state is changed to FS_AIO_PROGRESS.
 *
 *           This function sets input only if pObj->state == FS_AIO_START.
 *
 * @param    *pObj - input argument is pointer to analog test instance.
 * @param    *pAdc  - input argument is pointer of analog converter.
 *
 * @return   FS_AIO_PROGRESS - required return value. It Means that input was set.
 *           Any other value - the function had no effect.
 *
 ******************************************************************************/
FS_RESULT FS_AIO_InputSet_K3S(fs_aio_test_t *pObj, fs_aio_k3s_t *pAdc);

/*******************************************************************************
 *
 * @brief    The function sets up the ADC input channel and triggers the conversion.
 *           Then state is changed to FS_AIO_PROGRESS.
 *
 *           This function sets input only if pObj->state == FS_AIO_START.
 *
 * @param    *pObj - input argument is pointer to analog test instance.
 * @param    *pAdc - input argument is pointer to analog converter.
 *
 * @return   FS_AIO_PROGRESS - required return value. It Means that input was set.
 *           Any other value - the function had no effect.
 *
 ******************************************************************************/
FS_RESULT FS_AIO_InputSet_CYCLIC(fs_aio_test_t *pObj, fs_aio_cyclic_t *pAdc);

/*******************************************************************************
 *
 * @brief    The function sets up the ADC input channel and triggers the conversion.
 *           Then state is changed to FS_AIO_PROGRESS.
 *
 *           This function sets input only if pObj->state == FS_AIO_START.
 *
 * @param    *pObj - input argument is pointer to analog test instance.
 * @param    *pAdc - input argument is pointer to analog converter.
 *
 * @return   FS_AIO_PROGRESS - required return value. It Means that input was set.
 *           Any other value - the function had no effect.
 *
 ******************************************************************************/
FS_RESULT FS_AIO_InputSet_LPC8XX(fs_aio_test_t *pObj, fs_aio_lpc8xx_t *pAdc);

/*******************************************************************************
 *
 * @brief    The function sets up the ADC input channel and triggers the conversion.
 *           Then state is changed to FS_AIO_PROGRESS.
 *
 *           This function sets input only if pObj->state == FS_AIO_START.
 *
 * @param    *pObj - input argument is pointer to analog test instance.
 * @param    *pAdc - input argument is pointer to analog converter.
 *
 * @return   FS_AIO_PROGRESS - required return value. It Means that input was set.
 *           Any other value - the function had no effect.
 *
 ******************************************************************************/
FS_RESULT FS_AIO_InputSet_LPC55SXX(fs_aio_test_t *pObj, fs_aio_lpc55sxx_t *pAdc);

/*******************************************************************************
 *
 * @brief    The function sets up the ADC input channel and triggers the conversion.
 *           Then state is changed to FS_AIO_PROGRESS.
 *
 *           This function sets input only if pObj->state == FS_AIO_START.
 *
 * @param    *pObj - input argument is pointer to analog test instance.
 * @param    *pAdc  - input argument is address of analog converter.
 *
 * @return   FS_AIO_PROGRESS - required return value. It Means that input was set.
 *           Any other value - the function had no effect.
 *
 ******************************************************************************/
FS_RESULT FS_AIO_InputSet_IMXRT10XX_SWTRIG(fs_aio_test_t *pObj, fs_aio_imxrt10xx_t *pAdc);

/*******************************************************************************
 *
 * @brief    The function sets up the ADC input channel and triggers the conversion.
 *           Then state is changed to FS_AIO_PROGRESS.
 *
 *           This function sets input only if pObj->state == FS_AIO_START.
 *
 * @param    *pObj - input argument is pointer to analog test instance.
 * @param    *pAdc  - input argument is address of analog converter.
 *
 * @return   FS_AIO_PROGRESS - required return value. It Means that input was set.
 *           Any other value - the function had no effect.
 *
 ******************************************************************************/
FS_RESULT FS_AIO_InputSet_IMXRT117X_SWTRIG(fs_aio_test_t *pObj, fs_aio_imxrt117x_t *pAdc);

/*******************************************************************************
 *
 * @brief    The function reads converted analog value and
 *           check if the value fits into predefined limits.
 *
 *           This function reads converted value only
 *           if pObj->state == FS_AIO_PROGRESS.
 *
 * @param    *pObj - input argument is pointer to analog test instance.
 * @param    *pAdc - input argument is address of analog converter.
 *
 * @return   FS_PASS         - successfull execution of test (all channels tested).
 *           FS_FAIL_AIO     - converted value doesn't fit into limits.
 *           FS_AIO_START    - successfull conversion, setup input for next conversion.
 *           FS_AIO_PROGRESS - input not converted yet.
 *           FS_AIO_INIT     - the function had no effect.
 *
 ******************************************************************************/
FS_RESULT FS_AIO_InputCheck(fs_aio_test_t *pObj, fs_aio_t *pAdc);
/*******************************************************************************
 *
 * @brief    The function reads converted analog value and
 *           check if the value fits into predefined limits.
 *
 *           This function reads converted value only
 *           if pObj->state == FS_AIO_PROGRESS.
 *
 * @param    *pObj - input argument is pointer to analog test instance.
 * @param    pAdc  - input argument is address of analog converter.
 *
 * @return   FS_PASS         - successfull execution of test (all channels tested).
 *           FS_FAIL_AIO     - converted value doesn't fit into limits.
 *           FS_AIO_START    - successfull conversion, setup input for next conversion.
 *           FS_AIO_PROGRESS - input not converted yet.
 *           FS_AIO_INIT     - the function had no effect.
 *
 ******************************************************************************/
FS_RESULT FS_AIO_InputCheck_KE(fs_aio_test_t *pObj, fs_aio_ke_t *pAdc);

/*******************************************************************************
 *
 * @brief    The function reads converted analog value and
 *           check if the value fits into predefined limits.
 *           Dedicated for 12bit Low Power ADC.
 *
 *           This function reads converted value only
 *           if pObj->state == FS_AIO_PROGRESS.
 *
 * @param    *pObj   - input argument is pointer to analog test instance.
 * @param    pAdc  - input argument is address of analog converter.
 * @param    resFifo - 32-bit value of the RESFIFO register.
 *
 * @return   FS_PASS         - successfull execution of test (all channels tested).
 *           FS_FAIL_AIO     - converted value doesn't fit into limits.
 *           FS_AIO_START    - successfull conversion, setup input for next conversion.
 *           FS_AIO_PROGRESS - input not converted yet.
 *           FS_AIO_INIT     - the function had no effect.
 *
 ******************************************************************************/
FS_RESULT FS_AIO_InputCheck_K3S(fs_aio_test_t *pObj, uint32_t pAdc, uint32_t resFifo);

/*******************************************************************************
 *
 * @brief    The function reads converted analog value and
 *           check if the value fits into predefined limits.
 *
 *           This function reads converted value only
 *           if pObj->state == FS_AIO_PROGRESS.
 *
 * @param    *pObj - input argument is pointer to analog test instance.
 * @param    *pAdc  - input argument is pointer to analog converter.
 *
 * @return   FS_PASS         - successfull execution of test (all channels tested).
 *           FS_FAIL_AIO     - converted value doesn't fit into limits.
 *           FS_AIO_START    - successfull conversion, setup input for next conversion.
 *           FS_AIO_PROGRESS - input not converted yet.
 *           FS_AIO_INIT     - the function had no effect.
 *
 ******************************************************************************/
FS_RESULT FS_AIO_InputCheck_CYCLIC(fs_aio_test_t *pObj, fs_aio_cyclic_t *pAdc);

/*******************************************************************************
 *
 * @brief    The function reads converted analog value and
 *           check if the value fits into predefined limits.
 *
 *           This function reads converted value only
 *           if pObj->state == FS_AIO_PROGRESS.
 *
 * @param    *pObj - input argument is pointer to analog test instance.
 * @param    *pAdc - input argument is pointer to analog converter.
 *
 * @return   FS_PASS         - successfull execution of test (all channels tested).
 *           FS_FAIL_AIO     - converted value doesn't fit into limits.
 *           FS_AIO_START    - successfull conversion, setup input for next conversion.
 *           FS_AIO_PROGRESS - input not converted yet.
 *           FS_AIO_INIT     - the function had no effect.
 *
 ******************************************************************************/
FS_RESULT FS_AIO_InputCheck_LPC8XX(fs_aio_test_t *pObj, fs_aio_lpc8xx_t *pAdc);

/*******************************************************************************
 *
 * @brief    The function reads converted analog value and
 *           check if the value fits into predefined limits.
 *
 *           This function reads converted value only
 *           if pObj->state == FS_AIO_PROGRESS.
 *
 * @param    *pObj - input argument is pointer to analog test instance.
 * @param    *pAdc  - input argument is address of analog converter.
 *
 * @return   FS_PASS         - successfull execution of test (all channels tested).
 *           FS_FAIL_AIO     - converted value doesn't fit into limits.
 *           FS_AIO_START    - successfull conversion, setup input for next conversion.
 *           FS_AIO_PROGRESS - input not converted yet.
 *           FS_AIO_INIT     - the function had no effect.
 *
 ******************************************************************************/
FS_RESULT FS_AIO_InputCheck_LPC55SXX(fs_aio_test_t *pObj, fs_aio_lpc55sxx_t *pAdc);

/*******************************************************************************
 *
 * @brief    The function reads converted analog value and
 *           check if the value fits into predefined limits.
 *
 *           This function should be used with software
 *           triggered analog I/O test.
 *
 * @param    *pObj - input argument is pointer to analog test instance.
 * @param    *pAdc - input argument is pointer to analog converter.
 *
 * @return   FS_PASS         - successfull execution of test (all channels tested).
 *           FS_FAIL_AIO     - converted value doesn't fit into limits.
 *           FS_AIO_START    - successfull conversion, setup input for next conversion.
 *           FS_AIO_PROGRESS - input not converted yet.
 *           FS_AIO_INIT     - the function had no effect.
 *
 ******************************************************************************/
FS_RESULT FS_AIO_InputCheck_IMXRT10XX_SWTRIG(fs_aio_test_t *pObj, fs_aio_imxrt10xx_t *pAdc);

/*******************************************************************************
 *
 * @brief    The function reads converted analog value and
 *           check if the value fits into predefined limits.
 *
 *           This function should be used with software
 *           triggered analog I/O test.
 *
 * @param    *pObj - input argument is pointer to analog test instance.
 * @param    *pAdc - input argument is pointer to analog converter.
 *
 * @return   FS_PASS         - successfull execution of test (all channels tested).
 *           FS_FAIL_AIO     - converted value doesn't fit into limits.
 *           FS_AIO_START    - successfull conversion, setup input for next conversion.
 *           FS_AIO_PROGRESS - input not converted yet.
 *           FS_AIO_INIT     - the function had no effect.
 *
 ******************************************************************************/
FS_RESULT FS_AIO_InputCheck_IMXRT117X(fs_aio_test_t *pObj, fs_aio_imxrt117x_t *pAdc);

#endif /* __ASM__ */
#endif /* IEC60730B_AIO_H_ */
