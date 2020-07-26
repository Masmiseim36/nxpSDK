/*
*         Copyright (c), NXP Semiconductors Bangalore / India
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
*particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* Internal functions for Tag Operation Application Layer Component of
* Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*/
#ifndef PHALT1T_SW_H
#define PHALT1T_SW_H

#include <ph_Status.h>

/** \addtogroup ph_Private
* @{
*/

#define PHAL_T1T_READUID_RESP_LEN              6U                       /**< T1T RID response len */

#define PHAL_T1T_READALL_RESP_LEN              122U                     /**< T1T RALL response len */
#define PHAL_T1T_READBYTE_RESP_LEN             2U                       /**< T1T Read response len */
#define PHAL_T1T_WRITEERASEBYTE_RESP_LEN       2U                       /**< T1T Write_E response len */
#define PHAL_T1T_WRITENOERASEBYTE_RESP_LEN     2U                       /**< T1T Write_NE response len */

#define PHAL_T1T_READSEG_RESP_LEN              129U                     /**< T1T RSEG response len */
#define PHAL_T1T_READBLOCK_RESP_LEN            9U                       /**< T1T Read8 response len */
#define PHAL_T1T_WRITEERASEBLOCK_RESP_LEN      9U                       /**< T1T Write_E8 response len */
#define PHAL_T1T_WRITENOERASEBLOCK_RESP_LEN    9U                       /**< T1T Write_NE8 response len */

#define PHAL_T1T_CMD_READUID                   0x78U                    /**< T1T RID command byte */

#define PHAL_T1T_CMD_READALL                   0x00U                    /**< T1T RALL command byte */
#define PHAL_T1T_CMD_READBYTE                  0x01U                    /**< T1T Read command byte */
#define PHAL_T1T_CMD_WRITEERASEBYTE            0x53U                    /**< T1T Write_E8 command byte */
#define PHAL_T1T_CMD_WRITENOERASEBYTE          0x1AU                    /**< T1T Write_NE8 command byte */

#define PHAL_T1T_CMD_READSEG                   0x10U                    /**< T1T RID command byte */
#define PHAL_T1T_CMD_READBLOCK                 0x02U                    /**< T1T Read8 command byte */
#define PHAL_T1T_CMD_WRITEERASEBLOCK           0x54U                    /**< T1T Write_E8 command byte */
#define PHAL_T1T_CMD_WRITENOERASEBLOCK         0x1BU                    /**< T1T Write_NE8 command byte.*/
#define PHAL_T1T_HEADER_ROM0                   0x10U                    /**< T1T Header Rom 0 byte to check.*/

#define PHAL_T1T_SW_READ_TIME_US               91U                      /**< Read commands timeout in microseconds. */
#define PHAL_T1T_SW_WRITE_ERASE_TIME_US        5236U                    /**< Write Erase commands timeout in microseconds. */
#define PHAL_T1T_SW_WRITE_NO_ERASE_TIME_US     2659U                    /**< Write No Erase commands timeout in microseconds. */
#define PHAL_T1T_SW_EXT_TIME_US                200U                     /**< Time extension in microseconds. */
/** @} */

phStatus_t phalT1T_Sw_ReadUID  (
                                phalT1T_Sw_DataParams_t * pDataParams,     /**< [In] */
                                uint8_t * pUid,                            /**< [Out] */
                                uint16_t * pLength                         /**< [Out] */
                                );

phStatus_t phalT1T_Sw_ReadAll  (
                                phalT1T_Sw_DataParams_t * pDataParams,     /**< [In] */
                                uint8_t * pUid,                            /**< [In] */
                                uint8_t ** pData,                          /**< [Out] */
                                uint16_t * pLength                         /**< [Out] */
                                );

phStatus_t phalT1T_Sw_ReadByte  (
                                 phalT1T_Sw_DataParams_t * pDataParams,     /**< [In] */
                                 uint8_t * pUid,                            /**< [In] */
                                 uint8_t   bAddress,                        /**< [In] */
                                 uint8_t * pData,                           /**< [Out] */
                                 uint16_t * pLength                         /**< [Out] */
                                 );

phStatus_t phalT1T_Sw_WriteEraseByte  (
                                       phalT1T_Sw_DataParams_t * pDataParams,     /**< [In] */
                                       uint8_t * pUid,                            /**< [In] */
                                       uint8_t   bAddress,                        /**< [In] */
                                       uint8_t   bTxData,                         /**< [In] */
                                       uint8_t * pRxData,                         /**< [Out] */
                                       uint16_t * pLength                         /**< [Out] */
                                       );

phStatus_t phalT1T_Sw_WriteNoEraseByte  (
    phalT1T_Sw_DataParams_t * pDataParams,     /**< [In] */
    uint8_t * pUid,                            /**< [In] */
    uint8_t   bAddress,                        /**< [In] */
    uint8_t   bTxData,                         /**< [In] */
    uint8_t * pRxData,                         /**< [Out] */
    uint16_t * pLength                         /**< [Out] */
    );

phStatus_t phalT1T_Sw_ReadSegment  (
                                    phalT1T_Sw_DataParams_t * pDataParams,     /**< [In] */
                                    uint8_t * pUid,                            /**< [In] */
                                    uint8_t   bAddress,                        /**< [In] */
                                    uint8_t ** pData,                          /**< [Out] */
                                    uint16_t * pLength                         /**< [Out] */
                                    );

phStatus_t phalT1T_Sw_ReadBlock  (
                                  phalT1T_Sw_DataParams_t * pDataParams,     /**< [In] */
                                  uint8_t * pUid,                            /**< [In] */
                                  uint8_t   bAddress,                        /**< [In] */
                                  uint8_t * pData,                           /**< [Out] */
                                  uint16_t * pLength                         /**< [Out] */
                                  );

phStatus_t phalT1T_Sw_WriteEraseBlock  (
                                        phalT1T_Sw_DataParams_t * pDataParams,     /**< [In] */
                                        uint8_t * pUid,                            /**< [In] */
                                        uint8_t   bAddress,                        /**< [In] */
                                        uint8_t * pTxData,                         /**< [In] */
                                        uint8_t * pRxData,                         /**< [Out] */
                                        uint16_t * pLength                         /**< [Out] */
                                        );

phStatus_t phalT1T_Sw_WriteNoEraseBlock  (
    phalT1T_Sw_DataParams_t * pDataParams,     /**< [In] */
    uint8_t * pUid,                            /**< [In] */
    uint8_t   bAddress,                        /**< [In] */
    uint8_t * pTxData,                         /**< [In] */
    uint8_t * pRxData,                         /**< [Out] */
    uint16_t * pLength                         /**< [Out] */
    );

#ifdef __DEBUG

phStatus_t phalT1T_Sw_SetPtr(
                             phalT1T_Sw_DataParams_t * pDataParams,
                             void * pI14443p3a
                             );
#endif /* __DEBUG */

#endif /* PHALT1T_SW_H */
