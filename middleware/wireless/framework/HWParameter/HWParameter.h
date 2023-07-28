/**********************************************************************************
 * Copyright 2019-2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ********************************************************************************** */

#ifndef __HW_PARAMETER_H__
#define __HW_PARAMETER_H__

/**********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */

#include "EmbeddedTypes.h"

/*!
 * @addtogroup HWParameter
 * The HWParameter module
 *
 * HWParameter module provides access to hardware parameters stored in flash. Different
 *      platforms/boards need board/network node-specific settings to function according
 *      to design. For this purpose, one flash sector is reserved and contains
 *      hardware-specific parameters for production data storage. These parameters
 *      pertain to the network node as a distinct entity, for example, a silicon mounted
 *      on a PCB in a specific configuration, rather than to just the silicon itself.
 *
 * @{
 */

/**********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */

/**********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */
/*! \brief  hardware parameter structure content.
 *
 * Data structure containing hardware parameters
 */
typedef PACKED_STRUCT hardwareParameters_tag
{
    uint8_t identificationWord[10]; /* internal usage only: valid data present */
    /*@{*/
    uint8_t bluetooth_address[6];     /*!< Bluetooth address */
    uint8_t ieee_802_15_4_address[8]; /*!< IEEE 802.15.4 MAC address - K32W1 only*/
    uint8_t xtalTrim;                 /*!< XTAL Trim value */
    /*@}*/
#ifdef OBSOLETED_FIELDS
    uint32_t edCalibrationOffset; /* KW01 ED offset */
    uint32_t pllFStepOffset;      /* KW01 fine tune pll */
    uint32_t rngSeed;             /* KW38 RNG Seed Storage for faster boot */
    uint32_t xcvrCal[13];         /* KW38 XCVR Calibration storage for faster Radio init , should be at least sizeof
                                     xcvr_DcocDacTrim_t */
    uint8_t rssiAdjustment;       /* KW38 Rssi Adjustment */
#endif
    /* For forward compatibility additional fields may be added here
       Existing data in flash will not be compatible after modifying the hardwareParameters_t typedef*/
    uint8_t
             reserved[64]; /*!< reserved for adding field to the structure without changing the Crc field adress location*/
    uint16_t hardwareParamsCrc; /* internal usage only: crc for data between start of reserved area and start of
                                   hardwareParamsCrc field (not included). */
}
hardwareParameters_t;

/**********************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */

/**********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
 * \brief  Load the HW parameters from Flash to RAM on first call, give the pointer
 *      on it if HW parameters are already in RAM.
 *
 * \details Verifies that the dedicated flash area starts with the identification
 *      information and that data is valid using a CRC. If any of the checks fail, it
 *      fills the RAM data structure with 0xFF and the function returns 1.
 *
 * Use example of the read function :
 * <pre>
 *      hardwareParameters_t *pHWParams=NULL;
 *      uint32_t status;
 *      status = NV_ReadHWParameters(&pHWParams);
 *      if(status == 0U)
 *      {
 *              ...
 *      } </pre>
 *
 * \param[out] pHwParams  pointer of pointer to a structure where HW parameters will
 *      be stored
 *
 * \return error code:
 *         - 0 : The structure in argument is correctly filled with HW parameters
 *         - 1 : The structure in flash does not start with the indentification word or
 *             the CRC is not correct => The data structure in RAM is filled with 0xFF
 ********************************************************************************** */
uint32_t NV_ReadHWParameters(hardwareParameters_t **pHwParams);

/*! *********************************************************************************
 * \brief  Store the HW parameters structure from RAM to Flash.
 *
 * \details Copies the identification information, calculates and updates the CRCvalue.
 *
 * If you want to change one data from the structure with a read, in a first time you
 *      have to call the read function
 *
 * Use example of the write function :
 * <pre>
 *      uint32_t status;
 *      pHWParams->xtalTrim = trimValue;
 *      status = NV_WriteHWParameters();
 *      if (status==0)
 *      {
 *              ...
 *      } </pre>
 *
 * \return error code of the Flash erase/write functions :
 *         - 0 : The structure in RAM has been succesfully copied in Flash
 *         - Other values : There was a problem when attempting to erase
 *           the previous structure stored in Flash or there was a
 *            problem when trying to copy the RAM structre in Flash
 *
 ********************************************************************************** */
uint32_t NV_WriteHWParameters(void);

/*!
 * @} end of HWParameter addtogroup
 */

#endif
