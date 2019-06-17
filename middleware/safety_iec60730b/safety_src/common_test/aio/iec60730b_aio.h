/*******************************************************************************
*
* Copyright 2013 - 2016, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
*
* This software is owned or controlled by NXP and may only be used
* strictly in accordance with the applicable license terms.  By expressly
* accepting such terms or by downloading, installing, activating and/or
* otherwise using the software, you are agreeing that you have read, and
* that you agree to comply with and are bound by, such license terms.  If
* you do not agree to be bound by the applicable license terms, then you
* may not retain, install, activate or otherwise use the software.
* 
*
*******************************************************************************/


/***********************************************************************************************
*
* @file      IEC60730_B_CM4_CM7_aio.h
*
* @author    B48632
* 
* @version   3.0
*
* @date      Nov-01-2018
* 
* @brief     IEC60730 Class B Safety Routines aio tests for Cortex CM4, CM7 cores
*
************************************************************************************************/

#ifndef _IEC60730_B_CM4_CM7_AIO_H_
#define _IEC60730_B_CM4_CM7_AIO_H_


/*******************************************************************************
* Macros
*******************************************************************************/
#define IEC60730B_ST_AIO_PASS      (0x0000)
#define IEC60730B_ST_AIO_FAIL      (IEC60730B_ST_AIO_FAIL_CODE|1)
#define IEC60730B_ST_AIO_PROGRESS  (IEC60730B_ST_AIO_FAIL_CODE|2)
#define IEC60730B_ST_AIO_INIT      (IEC60730B_ST_AIO_FAIL_CODE|3)
#define IEC60730B_ST_AIO_START     (IEC60730B_ST_AIO_FAIL_CODE|4)


#ifndef __ASM__

/*******************************************************************************
* Structures
*******************************************************************************/
typedef struct {
  unsigned short low;
  unsigned short high;
}IEC60730B_CM4_CM7_AIO_LIMITS_STR;

typedef struct {
  IEC60730B_CM4_CM7_AIO_LIMITS_STR* pLimits;
  unsigned char* pInputs;
  unsigned char* pSamples; /* MDK - For Cyclic ADC we need also SAMPLE number */
  IEC60730B_RESULT state;
  unsigned short result[3];
  unsigned char cnt;
  unsigned char cntMax;
  unsigned short fifoCounter;
  unsigned short comandSourceFlag;
  unsigned short commandBuffer;
}IEC60730B_CM4_CM7_AIO_TEST_STR;
#endif

/******************************************************************************
* Global functions
******************************************************************************/
#ifndef __ASM__

/*******************************************************************************
*
* brief        IEC60730B_CM4_CM7_AIO_InputInit
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
void IEC60730B_CM4_CM7_AIO_InputInit(IEC60730B_CM4_CM7_AIO_TEST_STR* pObj, IEC60730B_CM4_CM7_AIO_LIMITS_STR* pLimits, unsigned char* pInputs, unsigned char cntMax, unsigned char cmdBuffer);

/*******************************************************************************
*
* brief      IEC60730B_CM4_CM7_AIO_InputTrigger
*
* param[in]  pObj    - Input argument is pointer to analog test instance
*
* return     void
*                         
* remarks    This function sets up the analog input test to start the execution of the test.
*
*******************************************************************************/
void IEC60730B_CM4_CM7_AIO_InputTrigger(IEC60730B_CM4_CM7_AIO_TEST_STR* pObj);

/*******************************************************************************
*
* brief        IEC60730B_CM4_CM7_AIO_InputSet
*
* param[in]    pObj  - Input argument is pointer to analog test instance
* param[in]    pAdc  - Input argument is pointer to analog converter.
*
* return       - IEC60730B_ST_AIO_FAIL (0x00000701)        
*              - IEC60730B_ST_AIO_PASS (0x00000000)
*              - IEC60730B_ST_AIO_PROGRESS (0x00000702)
*              - IEC60730B_ST_AIO_INIT (0x00000704)
*         
* remarks      The function sets up the ADC input channel. 
*              When the ADC converter is configured for SW trigger, 
*              this function also triggers the conversion. 
*
*******************************************************************************/
IEC60730B_RESULT IEC60730B_CM4_CM7_AIO_InputSet(IEC60730B_CM4_CM7_AIO_TEST_STR* pObj, unsigned long* pAdc);

/*******************************************************************************
*
* brief        IEC60730B_CM4_CM7_AIO_InputCheck
*
* param[in]    pObj  - Input argument is pointer to analog test instance
* param[in]    pAdc  - Input argument is pointer to analog converter.
*
* return       - IEC60730B_ST_AIO_FAIL (0x00000701)        
*              - IEC60730B_ST_AIO_PASS (0x00000000)
*              - IEC60730B_ST_AIO_PROGRESS (0x00000702)
*              - IEC60730B_ST_AIO_START (0x00000703)
*              - IEC60730B_ST_AIO_INIT (0x00000704)
*         
* remarks      The function reads converted analog value and check if the value fits into predefined limits.
*
*******************************************************************************/
IEC60730B_RESULT IEC60730B_CM4_CM7_AIO_InputCheck(IEC60730B_CM4_CM7_AIO_TEST_STR* pObj, unsigned long* pAdc);


/*******************************************************************************
*
* brief        IEC60730B_CM4_CM7_AIO_InputCheck_ke
*
* param[in]    pObj  - Input argument is pointer to analog test instance
* param[in]    pAdc  - Input argument is pointer to analog converter.
*
* return       - IEC60730B_ST_AIO_FAIL (0x00000701)        
*              - IEC60730B_ST_AIO_PASS (0x00000000)
*              - IEC60730B_ST_AIO_PROGRESS (0x00000702)
*              - IEC60730B_ST_AIO_START (0x00000703)
*              - IEC60730B_ST_AIO_INIT (0x00000704)
*         
* remarks      The function reads converted analog value and check if the value fits into predefined limits.
*
*******************************************************************************/
IEC60730B_RESULT IEC60730B_CM4_CM7_AIO_InputCheck_ke(IEC60730B_CM4_CM7_AIO_TEST_STR* pObj, unsigned long* pAdc);


/*******************************************************************************
*
* brief        IEC60730B_CM4_CM7_AIO_InputCheck_k3s
*
* param[in]    pObj  - Input argument is pointer to analog test instance
* param[in]    pAdc  - Input argument is pointer to analog converter.
* param[in]    resFifo - 32-bit value of the RESFIFO register
*
* return       - IEC60730B_ST_AIO_FAIL (0x00000701)        
*              - IEC60730B_ST_AIO_PASS (0x00000000)
*              - IEC60730B_ST_AIO_PROGRESS (0x00000702)
*              - IEC60730B_ST_AIO_START (0x00000703)
*              - IEC60730B_ST_AIO_INIT (0x00000704)
*         
* remarks      The function reads converted analog value and check if the value fits into predefined limits.
*              Dedicated for 12bit Low Power ADC.
*
*******************************************************************************/
IEC60730B_RESULT IEC60730B_CM4_CM7_AIO_InputCheck_k3s(IEC60730B_CM4_CM7_AIO_TEST_STR* pObj, unsigned long* pAdc, unsigned long resFifo);


/*******************************************************************************
*
* brief        IEC60730B_CM4_CM7_AIO_InputSet_k3s
*
* param[in]    pObj  - Input argument is pointer to analog test instance
* param[in]    pAdc  - Input argument is pointer to analog converter.
*
* return       - IEC60730B_ST_AIO_FAIL (0x00000701)        
*              - IEC60730B_ST_AIO_PASS (0x00000000)
*              - IEC60730B_ST_AIO_PROGRESS (0x00000702)
*              - IEC60730B_ST_AIO_INIT (0x00000704)
*         
* remarks      The function sets up the ADC input channel and triggers the conversion. 
*              If the channel and trigger are set, state is changed to PROGRESS.
*
*******************************************************************************/
IEC60730B_RESULT IEC60730B_CM4_CM7_AIO_InputSet_k3s(IEC60730B_CM4_CM7_AIO_TEST_STR* pObj, unsigned long* pAdc);

/*******************************************************************************
*
* brief        IEC60730B_CM4_CM7_AIO_InputInit_Cyclic
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
void IEC60730B_CM4_CM7_AIO_InputInit_Cyclic(IEC60730B_CM4_CM7_AIO_TEST_STR* pObj, IEC60730B_CM4_CM7_AIO_LIMITS_STR* pLimits, unsigned char* pInputs, unsigned char* pSamples, unsigned char cntMax);

/*******************************************************************************
*
* brief        IEC60730B_CM4_CM7_AIO_InputSet_Cyclic
*
* param[in]    pObj  - Input argument is pointer to analog test instance
* param[in]    pAdc  - Input argument is pointer to analog converter.
*
* return       - IEC60730B_ST_AIO_FAIL (0x00000701)        
*              - IEC60730B_ST_AIO_PASS (0x00000000)
*              - IEC60730B_ST_AIO_PROGRESS (0x00000702)
*              - IEC60730B_ST_AIO_INIT (0x00000704)
*         
* remarks      The function sets up the ADC input channel. 
*              When the ADC converter is configured for SW trigger, 
*              this function also triggers the conversion. 
*
*******************************************************************************/
IEC60730B_RESULT IEC60730B_CM4_CM7_AIO_InputSet_Cyclic(IEC60730B_CM4_CM7_AIO_TEST_STR* pObj, unsigned long* pAdc);

/*******************************************************************************
*
* brief        IEC60730B_CM4_CM7_AIO_InputCheck_Cyclic
*
* param[in]    pObj  - Input argument is pointer to analog test instance
* param[in]    pAdc  - Input argument is pointer to analog converter.
*
* return       - IEC60730B_ST_AIO_FAIL (0x00000701)
*              - IEC60730B_ST_AIO_PASS (0x00000000)
*              - IEC60730B_ST_AIO_PROGRESS (0x00000702)
*              - IEC60730B_ST_AIO_START (0x00000703)
*              - IEC60730B_ST_AIO_INIT (0x00000704)
*
* remarks      The function reads converted analog value and check if the value fits into predefined limits.
*
*******************************************************************************/
IEC60730B_RESULT IEC60730B_CM4_CM7_AIO_InputCheck_Cyclic(IEC60730B_CM4_CM7_AIO_TEST_STR* pObj, unsigned long* pAdc);

#endif /* __ASM__ */

#endif /* _IEC60730_B_CM4_CM7_AIO_H_ */
    
