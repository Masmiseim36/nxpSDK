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
* Example Source for Card product type detection.
* $Author: Purnank G (ing05193) $
* $Revision: 5458 $ (v06.01.00)
* $Date: 2016-07-06 14:59:09 +0530 (Wed, 06 Jul 2016) $
*
* History:
* BK: Generated 25. Sept 2014
*
*/

/**
* Example Header
*/
#include <phApp_Init.h>
/* Status code definitions */
#include "cards.h"

/*******************************************************************************
**   DetectClassic
**   This function recognize mifare Classic card is in field.
**   PH_ON will be returned if mifare Classic is detected else PH_OFF.
*******************************************************************************/
uint8_t DetectClassic(uint8_t *pAtqa, uint8_t *bSak)
{
    uint32_t sak_atqa = 0;
    uint8_t bStatus = PH_OFF;

    sak_atqa = bSak[0] << 24 | pAtqa[0] << 8 | pAtqa[1];
    sak_atqa &= 0xFFFF0FFF;

    /* Detect mini or classic */
    switch (sak_atqa)
    {
    case sak_mfc_1k << 24 | atqa_mfc:
    case sak_mfc_4k << 24 | atqa_mfc:
    case sak_mfp_2k_sl1 << 24 | atqa_mfp_s:
    case sak_mini << 24 | atqa_mini:
    case sak_mfp_4k_sl1 << 24 | atqa_mfp_s:
    case sak_mfp_2k_sl1 << 24 | atqa_mfp_x:
    case sak_mfp_4k_sl1 << 24 | atqa_mfp_x:
        bStatus = PH_ON;
        break;
    default:
        break;
    }
    return bStatus;
}

/*******************************************************************************
**   DetectMifare
**   This function recognize which kind of mifare card is in field.
**   Card type will be returned.
**   If 0 returned, card isn't recognized.
*******************************************************************************/
uint32_t DetectMifare(uint8_t *pAtqa, uint8_t *bSak)
{
  uint32_t sak_atqa = 0;
  uint16_t detected_card = 0xFFFF;

  sak_atqa = bSak[0] << 24 | pAtqa[0] << 8 | pAtqa[1];
  sak_atqa &= 0xFFFF0FFF;

  /* Detect mini or classic */
  switch (sak_atqa)
  {
    case sak_mfc_1k << 24 | atqa_mfc:
      DEBUG_PRINTF("Product: MIFARE Classic\n");
      detected_card &= mifare_classic;
      break;
    case sak_mfc_4k << 24 | atqa_mfc:
      DEBUG_PRINTF("Product: MIFARE Classic\n");
      detected_card &= mifare_classic;
      break;
    case sak_mfp_2k_sl1 << 24 | atqa_mfp_s:
      DEBUG_PRINTF("Product: MIFARE Classic\n");
      detected_card &= mifare_classic;
      break;
    case sak_mini << 24 | atqa_mini:
      DEBUG_PRINTF("Product: MIFARE Mini\n");
      detected_card &= mifare_mini;
      break;
    case sak_mfp_4k_sl1 << 24 | atqa_mfp_s:
      DEBUG_PRINTF("Product: MIFARE Classic\n");
      detected_card &= mifare_classic;
      break;
    case sak_mfp_2k_sl1 << 24 | atqa_mfp_x:
      DEBUG_PRINTF("Product: MIFARE Classic\n");
      detected_card &= mifare_classic;
      break;
    case sak_mfp_4k_sl1 << 24 | atqa_mfp_x:
      DEBUG_PRINTF("Product: MIFARE Classic\n");
      detected_card &= mifare_classic;
      break;
    default:
      detected_card = 0xFFFF;
      break;
  }

  if (detected_card == 0xFFFF)
  {
    sak_atqa = bSak[0] << 24 | pAtqa[0] << 8 | pAtqa[1];
    switch (sak_atqa)
    {
      case sak_ul << 24 | atqa_ul:
        DEBUG_PRINTF("Product: MIFARE Ultralight\n");
        detected_card &= mifare_ultralight;
        break;
      case sak_mfp_2k_sl2 << 24 | atqa_mfp_s:
        DEBUG_PRINTF("Product: MIFARE Plus\n");
        detected_card &= mifare_plus;
        break;
      case sak_mfp_2k_sl3 << 24 | atqa_mfp_s:
        DEBUG_PRINTF("Product: MIFARE Plus\n");
        detected_card &= mifare_plus;
        break;
      case sak_mfp_4k_sl2 << 24 | atqa_mfp_s:
        DEBUG_PRINTF("Product: MIFARE Plus\n");
        detected_card &= mifare_plus;
        break;
      case sak_mfp_2k_sl2 << 24 | atqa_mfp_x:
        DEBUG_PRINTF("Product: MIFARE Plus\n");
        detected_card &= mifare_plus;
        break;
      case sak_mfp_2k_sl3 << 24 | atqa_mfp_x:
        DEBUG_PRINTF("Product: MIFARE Plus\n");
        detected_card &= mifare_plus;
        break;
      case sak_mfp_4k_sl2 << 24 | atqa_mfp_x:
        DEBUG_PRINTF("Product: MIFARE Plus\n");
        detected_card &= mifare_plus;
        break;
      case sak_desfire << 24 | atqa_desfire:
        DEBUG_PRINTF("Product: MIFARE DESFire\n");
        detected_card &= mifare_desfire;
        break;
      case sak_jcop << 24 | atqa_jcop:
        DEBUG_PRINTF("Payment card:\n");
        detected_card &= jcop;
        break;
      case sak_layer4 << 24 | atqa_nPA:
        DEBUG_PRINTF("German eID (neuer Personalausweis) detected\n");
        detected_card &= nPA;
        break;
      default:
        return 0;
    }
  }
  return detected_card;
}

/**
* Dump buffer content in both HEX and ASCII character.
*/
void DumpBuffer(
                uint8_t *pBuffer,
                uint32_t dwBufferLength
                )
{
    uint32_t i;
    uint8_t aTempBuffer[17] = {0};

    /* Debug print is not enabled in Release Mode. Required only for release mode. */
    PH_UNUSED_VARIABLE(aTempBuffer[0]);

    for(i = 0; i < dwBufferLength; i++)
    {
        if((i % 16) == 0)
        {
            if(i != 0)
            {
                DEBUG_PRINTF("  |%s|\n", aTempBuffer);
            }

            DEBUG_PRINTF("\t\t[%04X] ", i);
        }

        DEBUG_PRINTF(" %02X", pBuffer[i]);

        if((pBuffer[i] < 0x20) || (pBuffer[i] > 0x7e))
        {
            aTempBuffer[i % 16] = '.';
        }
        else
        {
            aTempBuffer[i % 16] = pBuffer[i];
        }

        aTempBuffer[16] = 0;
    }

    while((i % 16) != 0)
    {
        DEBUG_PRINTF("   ");
        aTempBuffer[((i++) % 16)] = ' ';
    }

    DEBUG_PRINTF("  |%s|\n", aTempBuffer);
}

/******************************************************************************
**                            End Of File
******************************************************************************/
