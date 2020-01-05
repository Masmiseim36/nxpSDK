/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __EMVL1_INTERFACE_H__
#define __EMVL1_INTERFACE_H__

#if defined(USING_PHY_TDA8035)
#include "fsl_smartcard_phy.h"
#endif
#if defined(USING_PHY_EMVSIM)
#include "fsl_smartcard_phy_emvsim.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef void (*emv_callback_t)(uint8_t evnet, void *buffer, uint32_t size);

#define EMV_SMART_CARD_PROTOCOL_T0 (0x01U)
#define EMV_SMART_CARD_PROTOCOL_T1 (0x02U)
#define MSEC_TO_TICK(msec) ((1000L + ((uint32_t)configTICK_RATE_HZ * (uint32_t)(msec - 1U))) / 1000L)

typedef enum _emv_smart_card_convention
{
    kEmvSmartCardConventionDirect = (0x00U),
    kEmvSmartCardConventionInverse = (0x01U),
} emv_smart_card_convention_t;

typedef enum _emv_smart_card_checksum_type
{
    kEmvSmartCardChecksumTypeLRC = (0x00U),
    kEmvSmartCardChecksumTypeCRC = (0x01U),
} emv_smart_card_checksum_type_t;

enum
{
    kEmvSmartCardEventCardInserted = 1U,
    kEmvSmartCardEventCardRemoved,
};

enum
{
    kStatus_CCID_EMV_Success = 0x00U,
    kStatus_CCID_EMV_CardRemoved = 0x80U,
    kStatus_CCID_EMV_InvalidParameter = 0x81U,
    kStatus_CCID_EMV_Unsupported = 0x82U,

    kStatus_CCID_EMV_Error = 0xFFU,
};

typedef struct _emv_card_data_information_struct
{
    uint8_t FI;
    uint8_t DI;
    uint8_t GTN;
    emv_smart_card_convention_t convention;
    union
    {
        struct
        {
            uint8_t WI;
        } t0;
        struct
        {
            emv_smart_card_checksum_type_t checksumType;
            uint8_t BWI;
            uint8_t CWI;
            uint8_t IFSC;
        } t1;
    } paramUnion;
} emv_card_data_information_struct_t;

#if defined(__cplusplus)
extern "C" {
#endif

void EMVL1_SmartCardInit(emv_callback_t callback);
uint8_t EMVL1_SmartCardPowerOn(uint8_t *atrBuffer, uint32_t *length);
uint8_t EMVL1_SmartCardPowerOff(void);
uint8_t EMVL1_SmartCardGetProtocol(void);
uint8_t EMVL1_SmartCardPresence(void);
uint8_t EMVL1_SmartCardGetInformation(emv_card_data_information_struct_t *information);
uint8_t EMVL1_SendApduCommand(uint8_t *commandApdu,
                              uint32_t commandApduLength,
                              uint8_t *ResponseApdu,
                              uint32_t *ResponseApduLength);
void EMVL1_StartCardTask(void *param);
#if defined(__cplusplus)
}
#endif

#endif /* __EMVL1_INTERFACE_H__ */
