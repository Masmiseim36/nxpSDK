/*! *********************************************************************************
 * \addtogroup WPT Custom Profile
 * @{
 ********************************************************************************** */
/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* \file
*
* This file is the interface file for the Wireless Power Transfer System Service
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
* 
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* 
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
* 
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _WPT_INTERFACE_H_
#define _WPT_INTERFACE_H_

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public constants & macros
*************************************************************************************
************************************************************************************/

/*! PRU RSSI field values - all ones if unknown */
#define gWpt_PruPwr_c                      0xF8
#define gWpt_PruAGain_c                    0x07
#define gWpt_PruRssi_c                     ((gWpt_PruPwr_c) | (gWpt_PruAGain_c))

#define gWpt_PruStaticLen_c                (0x14)
#define gWpt_PruDynamicLen_c               (0x14)
#define gWpt_PtuStaticLen_c                (0x11)
#define gWpt_PruControlLen_c               (0x05)
#define gWpt_PruAlertLen_c                 (0x01)

#define Wpt_ReadPruDynamic(buff,len)       Wpt_ReadCharacteristic(uuid_characteristic_pru_dynamic,\
                                                                   buff, len)

#define Wpt_ReadPruControl(buff,len)       Wpt_ReadCharacteristic(uuid_characteristic_pru_control,\
                                                                   buff, len)

#define Wpt_ReadPruAlert(buff,len)         Wpt_ReadCharacteristic(uuid_characteristic_pru_alert,\
                                                                   buff, len)

#define Wpt_ReadPtuStatic(buff,len)        Wpt_ReadCharacteristic(uuid_characteristic_ptu_static,\
                                                                   buff, len)
/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/
/*! PRU ADV Flags : Impedance shift (7:5); Reboot bit(4); OVP Status (3);
Time Set Support (2); RFU (1:0) */
typedef union wptAdvFlags_tag
{
    struct flags_tag
    {
        uint8_t RFU                         :2;
        uint8_t timeSetSupport              :1;
        uint8_t ovpStatus                   :1;
        uint8_t rebootBit                   :1;
        uint8_t impedanceShift              :3;
    } fields;
    uint8_t rawAdvFlags;
} wptAdvFlags_t;

/*! Values for the Adjust Power Command sub-field
of the Enables field inside PRU Control Characteristic*/
typedef enum adjustPowerCommandValues_tag
{
    gAdjustPowerCommandMaxPower_c           = 0x00,
    gAdjustPowerCommand66Percent_c          = 0x01,
    gAdjustPowerCommand33Percent_c          = 0x02,
    gAdjustPowerCommand2_5Watts_c           = 0x03
} adjustPowerCommandValues_t;

/*! Interface for the Enables field inside 
PRU Control Characteristic*/
typedef union wptPruControlCharEnables_tag
{
    struct enables_tag
    {
        uint8_t RFU                         :4;
        uint8_t adjustPowerCommand          :2;
        uint8_t enablePruChargeIndicator    :1;
        uint8_t enablePruOutput             :1;
    }fields;
    uint8_t rawEnables;
} wptPruControlCharEnables_t;

/*! Interface for the Permission field inside 
PRU Control Characteristic */
typedef enum wptPruControlCharPermValues_tag
{
    gPermittedWithoutReason_c               = 0x00,
    gPermittedWithWaitingTime_c             = 0x01,
    gDeniedDueToCrossConnection_c           = 0x80,
    gDeniedDueToLtdAvailablePower_c         = 0x81,
    gDeniedDueToLtdPtuNumOfDevices_c        = 0x82,
    gDeniedDueToLtdPtuClassSupport_c        = 0x83,
    gDeniedDueToHighTemperaturePtu_c        = 0x84 
} wptPruControlCharPermValues_t;

/*! Interface for the Time Set field inside 
PRU Control Characteristic */
typedef enum wptPruControlCharTimeSetValues_tag
{
    gTimeSetDoNotPerformTimeSet_c           = 0x00,
    gTimeSetTime10Ms_c                      = 0x01,
    gTimeSetTime20Ms_c                      = 0x02,
    gTimeSetTime30Ms_c                      = 0x03,
    gTimeSetTime40Ms_c                      = 0x04,
    gTimeSetTime50Ms_c                      = 0x05,
    gTimeSetTime60Ms_c                      = 0x06,
    gTimeSetTime70Ms_c                      = 0x07,
    gTimeSetTime80Ms_c                      = 0x08,
    gTimeSetTimeRFU_c                       = 0x09
} wptPruControlCharTimeSetValues_t;

/*! Interface for the Optional Fields field inside
the PTU static parameter*/
typedef union wptPtuStaticParamCharOptFields_tag{
    struct ptuOptionalFields_tag
    {
        uint8_t RFU             :6;
        uint8_t maxResistance   :1;
        uint8_t maxImpedance    :1;
    }fields;
    uint8_t rawOptionalFields;
} wptPtuStaticParamCharOptFields_t;

/*! Interface for the PTU class inside 
PTU Static Parameter Characteristic */
typedef enum wptPtuStaticParamCharPtuClass_tag
{
    gPtuClass1_c                = 0x00,
    gPtuClass2_c                = 0x01,
    gPtuClass3_c                = 0x02,
    gPtuClass4_c                = 0x03,
    gPtuClass5_c                = 0x04,
    gPtuClassMax_c              = 0x05
} wptPtuStaticParamCharPtuClass_t;

/*! Interface for the Protocol Revision inside 
PTU Static Parameter Characteristic */
typedef enum wptPtuStaticParamCharProtoRev_tag
{
    gProtocolRevision_121_c     = 0x00,
    gProtocolRevision_130_c     = 0x01,
    gProtocolRevisionMax_c      = 0x02
} wptPtuStaticParamCharProtoRev_t;

/*! Interface for the Protocol Revision inside 
PRU Static Parameter Characteristic */
typedef wptPtuStaticParamCharProtoRev_t wptPruStaticParamCharProtoRev_t;

/*! Interface for the Number of Devices inside 
PTU Static Parameter Characteristic */
typedef enum wptPtuStaticParamCharNumOfDev_tag
{
    gNumberOfDevices1_c         = 0x00,
    gNumberOfDevices2_c         = 0x01,
    gNumberOfDevices3_c         = 0x02,
    gNumberOfDevices4_c         = 0x03,
    gNumberOfDevices5_c         = 0x04,
    gNumberOfDevices6_c         = 0x05,
    gNumberOfDevices7_c         = 0x06,
    gNumberOfDevices8_c         = 0x07,
    gNumberOfDevicesRFU_c       = 0x08
} wptPtuStaticParamCharNumOfDev_t;

/*! Interface for the Optional Fields inside 
PRU Static Parameter Characteristic */
typedef union wptPruStaticParamCharOptional_tag
{
    struct pruOptionalFields_tag
    {
        uint8_t RFU                  :7;
        uint8_t deltaR1              :1;
    }fields;
    uint8_t rawOptionalFields;
} wptPruStaticParamCharOptional_t;

/*! Interface for the PRU Category inside 
PRU Static Parameter Characteristic */
typedef enum wptPruStaticParamCharPruCategory_tag
{
    gPruCategoryUndefined_c          = 0x00,
    gPruCategory1_c                  = 0x01,
    gPruCategory2_c                  = 0x02,
    gPruCategory3_c                  = 0x03,
    gPruCategory4_c                  = 0x04,
    gPruCategory5_c                  = 0x05,
    gPruCategoryMax_c                = 0x06
} wptPruStaticParamCharPruCategory_t;

/*! Interface for the PRU Information inside 
PRU Static Parameter Characteristic */
typedef union wptPruStaticParamCharPruInfo_tag
{
    struct pruInfo_tag
    {
        uint8_t RFU                  :2;
        uint8_t ptuTestMode          :1;
        uint8_t chrgCompleteConnMode :1;
        uint8_t adjustPwrCapability  :1;
        uint8_t pwrCtrlAlgPreference :1;
        uint8_t separateBtleRadioPru :1;
        uint8_t nfcReceiver          :1;
    }fields;
    uint8_t rawPruInfo;
} wptPruStaticParamCharPruInfo_t;

/*! Interface for the Optional Fields inside 
PRU Static Parameter Characteristic */
typedef union wptPruDynamicParamCharOptional_tag
{
    struct pruDynOptionalFields_tag
    {
        uint8_t RFU                     :2;
        uint8_t vRectHighDyn            :1;
        uint8_t vRectSetDyn             :1;
        uint8_t vRectMinDyn             :1;
        uint8_t temperature             :1;
        uint8_t iOut                    :1;
        uint8_t vOut                    :1;
    } fields;
    uint8_t rawOptionalFields;
} wptPruDynamicParamCharOptional_t;

/*! Interface for the PRU Alert field inside the PRU Alert Characteristic */
typedef union wptPruAlertCharAlert_tag
{
    struct pruAlert_tag
    {
        uint8_t modeTransition          :2;
        uint8_t wiredChargerDetected    :1;
        uint8_t chargeComplete          :1;
        uint8_t pruSelfProtection       :1;
        uint8_t pruOverTemperature      :1;
        uint8_t pruOverCurrent          :1;
        uint8_t pruOverVoltage          :1;
    } fields;
    uint8_t rawPruAlert;
}wptPruAlertCharAlert_t;

/*! Interface for the PRU Alert field inside the PRU Alert Characteristic */
typedef union wptPruAlertCharDyn_tag
{
    struct pruAlertDyn_tag
    {
        uint8_t adjPowerResponse        :1;
        uint8_t pruChargePort           :1;
        uint8_t wiredChargerDetected    :1;
        uint8_t chargeComplete          :1;
        uint8_t pruSelfProtection       :1;
        uint8_t pruOverTemperature      :1;
        uint8_t pruOverCurrent          :1;
        uint8_t pruOverVoltage          :1;
    } fields;
    uint8_t rawPruAlert;
}wptPruAlertCharDyn_t;

/******************** Characteristics definitions ****************/

/*! Interface for the PRU Control Characteristic */
typedef PACKED_STRUCT wptPruControlCharacteristic_tag
{
    wptPruControlCharEnables_t          enables;
    uint8_t                             permission;
    uint8_t                             timeSetMs;
    uint8_t                             RFU[2];
} wptPruControlCharacteristic_t;

/*! Interface for the PTU Static Parameter Characteristic */
typedef PACKED_STRUCT wptPtuStaticParamCharacteristic_tag
{
    wptPtuStaticParamCharOptFields_t    optFields;
    uint8_t                             ptuPower;
    uint8_t                             maxSourceImpedance;
    uint8_t                             maxLoadResistance;
    uint8_t                             RFU_undef[2];
    uint8_t                             ptuClass;
    uint8_t                             hwRev;
    uint8_t                             swRev;
    uint8_t                             protocolRevision;
    uint8_t                             numberOfDevices;
    uint8_t                             RFU[6];
} wptPtuStaticParamCharacteristic_t;

/*! Interface for the PRU Static Parameter Characteristic */
typedef PACKED_STRUCT wptPruStaticParamCharacteristic_tag
{
    wptPruStaticParamCharOptional_t     optFieldsValidity;
    uint8_t                             protocolRevision;
    uint8_t                             RFU_undef;
    uint8_t                             pruCategory;
    wptPruStaticParamCharPruInfo_t      pruInformation;
    uint8_t                             hwRevision;
    uint8_t                             swRevision;
    uint8_t                             PRectMax_100mW;
    uint16_t                            vRectMinStatic_mV;
    uint16_t                            vRectHighStatic_mV;
    uint16_t                            vRectSet_mV;
    uint16_t                            deltaR1_001Ohm;
    uint8_t                             RFU[4];
} wptPruStaticParamCharacteristic_t;

/*! Interface for the PRU Dynamic Parameter Characteristic */
typedef PACKED_STRUCT wptPruDynamicParamCharacteristic_tag
{
    wptPruDynamicParamCharOptional_t    optFieldsValidity;
    uint16_t                            vRect;
    uint16_t                            iRect;
    uint16_t                            vOut;
    uint16_t                            iOut;
    uint8_t                             temperature_degC; /* from -40C */
    uint16_t                            vRectMinDyn_mV;
    uint16_t                            vRectSetDyn_mV;
    uint16_t                            vRectHighDyn_mV;
    wptPruAlertCharDyn_t                pruAlert;
    uint8_t                             testerCommand;
    uint8_t                             RFU[2];
} wptPruDynamicParamCharacteristic_t;

/*! Interface for the PRU Alert Characteristic */
typedef PACKED_STRUCT wptPruAlertCharacteristic_tag
{
    wptPruAlertCharAlert_t              pruAlert;
    uint8_t                             deviceAddressModeTransition[6];
} wptPruAlertCharacteristic_t;


/*! WPT Client - Configuration */
typedef struct wptPtuConfig_tag
{
    uint16_t    hService;
    uint16_t    hPruControl;
    uint16_t    hPruStatic;
    uint16_t    hPtuStatic;
    uint16_t    hPruDynamic;
    uint16_t    hPruAlert;
    uint16_t    hPruAlertCccd;
} wptPtuConfig_t;

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*!**********************************************************************************
* \brief        Starts WPT Service functionality
*
* \param[in]    pPruStaticParameters  Pointer to structure that contains PRU 
*                                     configuration information.
*
* \param[in]    serviceHandle  Handle for the A4WPT service
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Wpt_Start
(
 uint16_t serviceHandle, 
 wptPruStaticParamCharacteristic_t*  pPruStaticParameters
);

/*!**********************************************************************************
* \brief        Stops WPT Service functionality
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Wpt_Stop();

/*!**********************************************************************************
* \brief        Subscribes a GATT client to the WPT service
*
* \param[in]    clientDeviceId  Client Id in Device DB.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Wpt_Subscribe(deviceId_t deviceId);

/*!**********************************************************************************
* \brief        Unsubscribes a GATT client from the WPT service
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Wpt_Unsubscribe();

/*!**********************************************************************************
* \brief        Records the dynamic parameters of the PRU
*
* \param[in]    pPruDynamic Pointer to a PRU Dynamic structure
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Wpt_RecordPruDynamic
(
wptPruDynamicParamCharacteristic_t* pPruDynamic
);

/*!**********************************************************************************
* \brief        Records the PRU alert char value and sends notification if cccd 
*               is written
*
* \param[in]    pPruAlert Pointer to a PRU Alert structure
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Wpt_RecordPruAlertAndNotifyIfCccdWritten
(
wptPruAlertCharacteristic_t* pPruAlert
);

/*!**********************************************************************************
* \brief        Records the PTU static parameters
*
* \param[in]    pPtuStatic Pointer to a PTU Static structure
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Wpt_PruRecordPtuStaticParamCharacteristic
(
 wptPtuStaticParamCharacteristic_t* pPtuStatic
);

/*!**********************************************************************************
* \brief        Records the PRU control parameters
*
* \param[in]    pPtuStatic Pointer to a PRU Control structure
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Wpt_PruRecordPruControlCharacteristic
(
 wptPruControlCharacteristic_t* pPruControl
);

/*!**********************************************************************************
* \brief        Reads the PRU characteristic value of the given characteristic
*
* \param[in]    characteristic UUID of the characteristic as byte array.
*
* \param[out]   aOutBuffer Pointer to location where to write the characteristic value
*
* \param[out]   pOutLen Pointer to location where to write the length of the 
*                       characteristic value
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Wpt_ReadCharacteristic
(
uint8_t* charUuid128,
uint8_t* aOutBuffer,
uint16_t* pOutLen
);
#ifdef __cplusplus
}
#endif

#endif /* _WPT_INTERFACE_H_ */

/*! **********************************************************************************
 * @}
 ************************************************************************************/
