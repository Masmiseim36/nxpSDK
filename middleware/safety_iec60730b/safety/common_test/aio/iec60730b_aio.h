/************************************************************************************************
 * Copyright 2015 Freescale Semiconductor, Inc.
 * Copyright 2015-2019 NXP.
 *
 * License: LA_OPT_NXP_Software_License
 *
 * This software is owned or controlled by NXP and may
 * only be used strictly in accordance with the applicable license terms.
 * By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that
 * you have read, and that you agree to comply with and are bound by,
 * such license terms.  If you do not agree to be bound by the applicable
 * license terms, then you may not retain, install, activate or otherwise
 * use the software.
 *
 * 
 * @version   4.0
 * 
 * @brief     IEC60730 Class B Safety Routines for Analog I/O
 *
 ***********************************************************************************************/

#ifndef _IEC60730B_AIO_H_
#define _IEC60730B_AIO_H_

/*******************************************************************************
* Macros
*******************************************************************************/
/* AIO test result constants */
#define FS_AIO_PASS      (0x0000)
#define FS_AIO_FAIL      (FS_AIO_FAIL_CODE|0x1)
#define FS_AIO_PROGRESS  (FS_AIO_FAIL_CODE|0x2)
#define FS_AIO_INIT      (FS_AIO_FAIL_CODE|0x3)
#define FS_AIO_START     (FS_AIO_FAIL_CODE|0x4)

#ifndef __ASM__

/*******************************************************************************
* Structures
*******************************************************************************/
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
* Global functions
******************************************************************************/
/*******************************************************************************
*
* brief        FS_AIO_InputInit
*
* param[in]    pObj    - Input argument is pointer to analog test instance
* param[in]    pLimits - The input argument is a pointer to the array of limits used in the test.
* param[in]    pInputs – The input argument is a pointer to the array of input numbers used in the test.
* param[in]    cntMax  – The input argument is the size of the input and the limits arrays.
*
* return       void
*                         
* remarks      This function initializes one instance of the Analog input test. 
*
*******************************************************************************/
void FS_AIO_InputInit(fs_aio_test_t *pObj, fs_aio_limits_t *pLimits, uint8_t *pInputs, uint8_t cntMax);

/*******************************************************************************
*
* brief        FS_AIO_InputInit_K3S
*
* param[in]    pObj    - Input argument is pointer to analog test instance
* param[in]    pLimits - The input argument is a pointer to the array of limits used in the test.
* param[in]    pInputs – The input argument is a pointer to the array of input numbers used in the test.
* param[in]    cntMax  – The input argument is the size of the input and the limits arrays.
* param[in]    cmdBuffer - specify the command buffer for conversion. Has effect only in case of k3s devices
*
* return       void
*                         
* remarks      This function initializes one instance of the Analog input test. 
*
*******************************************************************************/
void FS_AIO_InputInit_K3S(fs_aio_test_t *pObj, fs_aio_limits_t *pLimits, uint8_t *pInputs, uint8_t cntMax, uint8_t cmdBuffer);

/*******************************************************************************
*
* brief        FS_AIO_InputInit_CYCLIC
*
* param[in]    pObj    - Input argument is pointer to analog test instance
* param[in]    pLimits - The input argument is a pointer to the array of limits used in the test.
* param[in]    pInputs – The input argument is a pointer to the array of input numbers used in the test.
* param[in]    pSamples – The input argument is a pointer to the array of sample number used in the test.
* param[in]    cntMax  – The input argument is the size of the input and the limits arrays.
*
* return       void
*
* remarks      This function initializes one instance of the Analog input test.
*
*******************************************************************************/
void FS_AIO_InputInit_CYCLIC(fs_aio_test_t *pObj, fs_aio_limits_t *pLimits, uint8_t *pInputs, uint8_t *pSamples, uint8_t cntMax);

/*******************************************************************************
*
* brief        FS_AIO_InputInit_LPC_ADC16
*
* param[in]    pObj     - Input argument is pointer to analog test instance
* param[in]    pLimits  - The input argument is a pointer to the array of limits used in the test.
* param[in]    pInputs  – The input argument is a pointer to the array of input numbers used in the test.
* param[in]    cntMax   – The input argument is the size of the input and the limits arrays.
* param[in]    sequence - The input argument is index of used sequence
* param[in]    fifo     - The input argument is index of used fifo
*
* return       void
*                         
* remarks      This function initializes one instance of the Analog input test. 
*
*******************************************************************************/
void FS_AIO_InputInit_LPC_ADC16(fs_aio_test_t *pObj, fs_aio_limits_t *pLimits, uint8_t *pInputs, uint8_t cntMax, uint8_t sequence, uint8_t fifo);

/*******************************************************************************
*
* brief        FS_AIO_InputInit_LPC_ADC12
*
* param[in]    pObj     - Input argument is pointer to analog test instance
* param[in]    pLimits  - The input argument is a pointer to the array of limits used in the test.
* param[in]    pInputs  – The input argument is a pointer to the array of input numbers used in the test.
* param[in]    cntMax   – The input argument is the size of the input and the limits arrays.
* param[in]    sequence - The input argument is index of used sequence
*
* return       void
*                         
* remarks      This function initializes one instance of the Analog input test. 
*
*******************************************************************************/
void FS_AIO_InputInit_LPC_ADC12(fs_aio_test_t *pObj, fs_aio_limits_t *pLimits, uint8_t *pInputs, uint8_t cntMax, uint8_t sequence);

/*******************************************************************************
*
* brief      FS_AIO_InputTrigger
*
* param[in]  pObj    - Input argument is pointer to analog test instance
*
* return     void
*                         
* remarks    This function sets up the analog input test to start the execution of the test.
*
*******************************************************************************/
void FS_AIO_InputTrigger(fs_aio_test_t *pObj);

/*******************************************************************************
*
* brief        FS_AIO_InputSet
*
* param[in]    pObj  - Input argument is pointer to analog test instance
* param[in]    pAdc  - Input argument is pointer to analog converter.
*
* return       - FS_AIO_FAIL     (0x00000701)        
*              - FS_AIO_PASS     (0x00000000)
*              - FS_AIO_PROGRESS (0x00000702)
*              - FS_AIO_INIT     (0x00000703)
*              - FS_AIO_START    (0x00000704)
*         
* remarks      The function sets up the ADC input channel. 
*              When the ADC converter is configured for SW trigger, 
*              this function also triggers the conversion. 
*
*******************************************************************************/
FS_RESULT FS_AIO_InputSet(fs_aio_test_t *pObj, uint32_t *pAdc);

/*******************************************************************************
*
* brief        FS_AIO_InputSet_K3S
*
* param[in]    pObj  - Input argument is pointer to analog test instance
* param[in]    pAdc  - Input argument is pointer to analog converter.
*
* return       - FS_AIO_FAIL     (0x00000701)        
*              - FS_AIO_PASS     (0x00000000)
*              - FS_AIO_PROGRESS (0x00000702)
*              - FS_AIO_INIT     (0x00000703)
*              - FS_AIO_START    (0x00000704)
*         
* remarks      The function sets up the ADC input channel and triggers the conversion. 
*              If the channel and trigger are set, state is changed to PROGRESS.
*
*******************************************************************************/
FS_RESULT FS_AIO_InputSet_K3S(fs_aio_test_t *pObj, uint32_t *pAdc);

/*******************************************************************************
*
* brief        FS_AIO_InputSet_CYCLIC
*
* param[in]    pObj  - Input argument is pointer to analog test instance
* param[in]    pAdc  - Input argument is pointer to analog converter.
*
* return       - FS_AIO_FAIL     (0x00000701)        
*              - FS_AIO_PASS     (0x00000000)
*              - FS_AIO_PROGRESS (0x00000702)
*              - FS_AIO_INIT     (0x00000703)
*              - FS_AIO_START    (0x00000704)
*         
* remarks      The function sets up the ADC input channel. 
*              When the ADC converter is configured for SW trigger, 
*              this function also triggers the conversion. 
*
*******************************************************************************/
FS_RESULT FS_AIO_InputSet_CYCLIC(fs_aio_test_t *pObj, uint32_t *pAdc);

/*******************************************************************************
*
* brief        FS_AIO_InputSet_LPC8XX
*
* param[in]    pObj  - Input argument is pointer to analog test instance
* param[in]    pAdc  - Input argument is pointer to analog converter.
*
* return       - FS_AIO_FAIL     (0x00000701)        
*              - FS_AIO_PASS     (0x00000000)
*              - FS_AIO_PROGRESS (0x00000702)
*              - FS_AIO_INIT     (0x00000703)
*              - FS_AIO_START    (0x00000704)
*         
* remarks      The function sets up the ADC input channel. 
*              When the ADC converter is configured for SW trigger, 
*              this function also triggers the conversion. 
*
*******************************************************************************/
FS_RESULT FS_AIO_InputSet_LPC8XX(fs_aio_test_t *pObj, uint32_t *pAdc);

/*******************************************************************************
*
* brief        FS_AIO_InputSet_LPC55SXX
*
* param[in]    pObj  - Input argument is pointer to analog test instance
* param[in]    pAdc  - Input argument is pointer to analog converter.
*
* return       - FS_AIO_FAIL     (0x00000701)        
*              - FS_AIO_PASS     (0x00000000)
*              - FS_AIO_PROGRESS (0x00000702)
*              - FS_AIO_INIT     (0x00000703)
*              - FS_AIO_START    (0x00000704)
*         
* remarks      The function sets up the ADC input channel. 
*              When the ADC converter is configured for SW trigger, 
*              this function also triggers the conversion. 
*
*******************************************************************************/
FS_RESULT FS_AIO_InputSet_LPC55SXX(fs_aio_test_t *pObj, uint32_t *pAdc);

/*******************************************************************************
*
* brief        FS_AIO_InputCheck
*
* param[in]    pObj  - Input argument is pointer to analog test instance
* param[in]    pAdc  - Input argument is pointer to analog converter.
*
* return       - FS_AIO_FAIL     (0x00000701)        
*              - FS_AIO_PASS     (0x00000000)
*              - FS_AIO_PROGRESS (0x00000702)
*              - FS_AIO_INIT     (0x00000703)
*              - FS_AIO_START    (0x00000704)
*         
* remarks      The function reads converted analog value and check if the value fits into predefined limits.
*
*******************************************************************************/
FS_RESULT FS_AIO_InputCheck(fs_aio_test_t *pObj, uint32_t *pAdc);

/*******************************************************************************
*
* brief        FS_AIO_InputCheck_KE
*
* param[in]    pObj  - Input argument is pointer to analog test instance
* param[in]    pAdc  - Input argument is pointer to analog converter.
*
* return       - FS_AIO_FAIL     (0x00000701)        
*              - FS_AIO_PASS     (0x00000000)
*              - FS_AIO_PROGRESS (0x00000702)
*              - FS_AIO_INIT     (0x00000703)
*              - FS_AIO_START    (0x00000704)
*         
* remarks      The function reads converted analog value and check if the value fits into predefined limits.
*
*******************************************************************************/
FS_RESULT FS_AIO_InputCheck_KE(fs_aio_test_t *pObj, uint32_t *pAdc);

/*******************************************************************************
*
* brief        FS_AIO_InputCheck_K3S
*
* param[in]    pObj  - Input argument is pointer to analog test instance
* param[in]    pAdc  - Input argument is pointer to analog converter.
* param[in]    resFifo - 32-bit value of the RESFIFO register
*
* return       - FS_AIO_FAIL     (0x00000701)        
*              - FS_AIO_PASS     (0x00000000)
*              - FS_AIO_PROGRESS (0x00000702)
*              - FS_AIO_INIT     (0x00000703)
*              - FS_AIO_START    (0x00000704)
*         
* remarks      The function reads converted analog value and check if the value fits into predefined limits.
*              Dedicated for 12bit Low Power ADC.
*
*******************************************************************************/
FS_RESULT FS_AIO_InputCheck_K3S(fs_aio_test_t *pObj, uint32_t *pAdc, uint32_t resFifo);

/*******************************************************************************
*
* brief        FS_AIO_InputCheck_CYCLIC
*
* param[in]    pObj  - Input argument is pointer to analog test instance
* param[in]    pAdc  - Input argument is pointer to analog converter.
*
* return       - FS_AIO_FAIL     (0x00000701)        
*              - FS_AIO_PASS     (0x00000000)
*              - FS_AIO_PROGRESS (0x00000702)
*              - FS_AIO_INIT     (0x00000703)
*              - FS_AIO_START    (0x00000704)
*
* remarks      The function reads converted analog value and check if the value fits into predefined limits.
*
*******************************************************************************/
FS_RESULT FS_AIO_InputCheck_CYCLIC(fs_aio_test_t *pObj, uint32_t *pAdc);

/*******************************************************************************
*
* brief        FS_AIO_InputCheck_LPC8XX
*
* param[in]    pObj  - Input argument is pointer to analog test instance
* param[in]    pAdc  - Input argument is pointer to analog converter.
*
* return       - FS_AIO_FAIL     (0x00000701)        
*              - FS_AIO_PASS     (0x00000000)
*              - FS_AIO_PROGRESS (0x00000702)
*              - FS_AIO_INIT     (0x00000703)
*              - FS_AIO_START    (0x00000704)
*
* remarks      The function reads converted analog value and check if the value fits into predefined limits.
*
*******************************************************************************/
FS_RESULT FS_AIO_InputCheck_LPC8XX(fs_aio_test_t* pObj, uint32_t *pAdc);

/*******************************************************************************
*
* brief        FS_AIO_InputCheck_LPC55SXX
*
* param[in]    pObj  - Input argument is pointer to analog test instance
* param[in]    pAdc  - Input argument is pointer to analog converter.
*
* return       - FS_AIO_FAIL     (0x00000701)        
*              - FS_AIO_PASS     (0x00000000)
*              - FS_AIO_PROGRESS (0x00000702)
*              - FS_AIO_INIT     (0x00000703)
*              - FS_AIO_START    (0x00000704)
*
* remarks      The function reads converted analog value and check if the value fits into predefined limits.
*
*******************************************************************************/
FS_RESULT FS_AIO_InputCheck_LPC55SXX(fs_aio_test_t* pObj, uint32_t *pAdc);

#endif /* __ASM__ */

#endif /* _IEC60730B_AIO_H_ */
