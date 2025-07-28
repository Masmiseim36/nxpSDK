/**********************************************************************************
 * Copyright 2019-2023 NXP
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
#include "fwk_platform_definitions.h"

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
/*
 * Size of 'magic string used to identify area in persistent storage.
 */
#define PROD_DATA_ID_STRING_SZ 10U

/*
 * The Organizationally unique identifier OUI is the 3 octet identifier present in
 * all MAC addresses whether BLE of IEEE 802.15.4.
 */
#define MAC_ADDR_OUI_PART_SIZE 3U
/*
 * Size of EUI for IEEE 802.15.4 addresses including OUI.
 */
#define EUI_64_SZ (64U / 8U)
/*
 * Size of EUI for BLE MAC addresses including OUI.
 */
#define EUI_48_SZ (48U / 8U)

#define BLE_MAC_ADDR_SZ  EUI_48_SZ
#define IEEE_802_15_4_SZ EUI_64_SZ

/*
 * the Hardware Parameters structure is padded to be 128 bytes in size.
 */
#define PROD_DATA_PADDING_SZ                                                                          \
    (PROD_DATA_LEN - PROD_DATA_ID_STRING_SZ - sizeof(uint16_t) - BLE_MAC_ADDR_SZ - IEEE_802_15_4_SZ - \
     2U * sizeof(uint8_t))

#define LEGACY_PROD_DATA_PADDING_SZ 63U
#define LEGACY_PROD_DATA_LEN                                                                \
    (PROD_DATA_ID_STRING_SZ + BLE_MAC_ADDR_SZ + IEEE_802_15_4_SZ + (2U * sizeof(uint8_t)) + \
     LEGACY_PROD_DATA_PADDING_SZ + sizeof(uint16_t))

/*
 * Size of 'magic' string used as tag to identify App Factory Data in storage.
 */
#define APP_FACTORY_DATA_ID_STRING_SZ 16U

#define gHWParameterSuccess_c  (0x00U)
#define gHWParameterError_c    (0x01U)
#define gHWParameterCrcError_c (0x02U)
#define gHWParameterLenError_c (0x03U)
#define gHWParameterBlank_c    (0x04U)

/**********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */

/*! \brief  hardware parameter structure content.
 *
 * Data structure containing hardware parameters
 */
typedef PACKED_STRUCT HwParameters_tag
{
    uint8_t identificationWord[PROD_DATA_ID_STRING_SZ]; /* internal usage only: valid data present */
    /*@{*/
    uint8_t bluetooth_address[BLE_MAC_ADDR_SZ];      /*!< Bluetooth address */
    uint8_t ieee_802_15_4_address[IEEE_802_15_4_SZ]; /*!< IEEE 802.15.4 MAC address - K32W1 only */
    uint8_t xtalTrim;                                /*!< XTAL 32MHz Trim value */
    uint8_t xtalCap32K;                              /*!< XTAL 32kHz capacitance value */
    /* For forward compatibility additional fields may be added here
       Existing data in flash will not be compatible after modifying the hardwareParameters_t typedef.
       In this case the size of the padding has to be adjusted.
    */
    uint8_t  reserved[PROD_DATA_PADDING_SZ];
    uint16_t crc;
    /* first byte of padding : actual size if 63 for legacy HwParameters but
      complement to 128 bytes in the new structure */
}
hardwareParameters_t;

/*! \brief  factory data parameter structure content.
 *
 * Data structure containing factory data.
 * The structure only contains the 1rst byte of the app factory data. The length is open and left to the app
 * discretion (up to 2kB). Head room for a terminating 2 byte CRC16 must be reserved in RAM in the
 * programming buffer.
 */
typedef PACKED_STRUCT extendedAppFactoryData_tag
{
    uint8_t  identificationWord[APP_FACTORY_DATA_ID_STRING_SZ]; /*!< internal usage only: valid data present */
    uint32_t extendedDataLength;                                /*!< size of application factory data octet string */
    uint8_t  app_factory_data[1];
}
extendedAppFactoryData_t;

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
 *         - 1 : The structure in flash does not start with the identification word or
 *             the CRC is not correct => The data structure in RAM is filled with 0xFF
 ********************************************************************************** */
uint32_t NV_ReadHWParameters(hardwareParameters_t **pHwParams);

/*! *********************************************************************************
 * \brief  Store the HW parameters structure from RAM to Flash.
 *
 * \details Copies the identification information, calculates and updates the CRCvalue.
 *
 * If you want to change one data from the structure with a read, in a first time you
 *      have to call the read function before hand so as to perform a copy in RAM.
 * Additionally if App Factory Data are colocated in the same sector (gHwParamsAppFactoryDataExtension_d)
 * and HW Parameters need to be rewritten, following a crystal calibration for instance, their contents is
 * liable to be lost. When the App Factory Data are programmed before a board calibration, preserving the
 * their contents is done by defining gHwParamsAppFactoryDataPreserveOnHwParamUpdate_d as 1.
 * Otherwise reprogramming App Factory Data is required.
 * Defining this involves a dynamic allocation of a buffer large enough to hold the whole App Factory Data.
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
 *         - 0 : The structure in RAM has been successfully copied in Flash
 *         - Other values : There was a problem when attempting to erase
 *           the previous structure stored in Flash or there was a problem when
 *           trying to copy the RAM structure in Flash
 *
 ********************************************************************************** */
uint32_t NV_WriteHWParameters(void);

/*! *********************************************************************************
 * \brief  Program Application Factory Data in flash.
 *
 * \details The implementation assumes that App Factory Data are colocated in the same
 *  flash  sector as HWParameters. Whenever Factory Data are not mirrored in RAM.
 *
 * \param src_data  pointer on a RAM buffer containing the App Factory Data structure.
 * \param extended_data_len  length in bytes of App Factory Data payload
 *
 * \note Enough RAM must have been allocated to hold the identification word string (16)
 *       the length (4), the actual payload (extended_data_len) plus 2 bytes of CRC16.
 *
 * \return 0 if the operation succeeds. Other values in case of error.
 *
 ********************************************************************************** */
uint8_t Nv_WriteAppFactoryData(extendedAppFactoryData_t *src_data, uint32_t extended_data_len);

/*! *********************************************************************************
 * \brief  Retrieve pointer on Factory data in flash.
 *
 * \details Unlike HWParameters, Factory Data are not mirrored in RAM.
 *
 * \return pointer of extendedFactoryData_t structure in Flash if CRC and identification word
 *         are correct, NULL otherwise.
 *
 ********************************************************************************** */
extendedAppFactoryData_t *Nv_GetAppFactoryData(void);

/*! *********************************************************************************
 * \brief  Reset HWParameter data for test purposes.
 * Used to force re-reading HWParameters from flash.
 *
 ********************************************************************************** */
void HWParametersReset(void);

/*! *********************************************************************************
 * \brief  Erase current version of HW Parameters.
 * Used mostly in unit tests.
 *
 ********************************************************************************** */
void HWParametersErase(void);

/*!
 * @} end of HWParameter addtogroup
 */

#endif
