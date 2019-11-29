/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#include "Scrambler.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
 
void Whiten(Whitener_config_t *whitenerConfig, uint8_t *pBuff, uint8_t buffLength)
{
    uint32_t initialState = (whitenerConfig->whitenInit << (32 - whitenerConfig->whitenSize));
    uint32_t polyValue = (whitenerConfig->whitenPoly << (32 - whitenerConfig->whitenSize));
    uint32_t currentValue = 0;
    
    for (uint16_t i = whitenerConfig->whitenStartOffset; i < buffLength - whitenerConfig->whitenEndOffset; i++)
    {
        currentValue = pBuff[i];
            
        if (whitenerConfig->whitenPolyType == 0)
        {
            for (uint8_t j = 0; j < 8; j++)
            {
                if (initialState & 0x80000000U)
                {
                    initialState = (initialState << 1);
                    initialState ^= polyValue;
                        
                    if (whitenerConfig->whitenRefIn == gWhitenRefInput)
                    {
                        currentValue = currentValue ^ (0x80 >> j);
                    }
                    else
                    {
                        currentValue = currentValue ^ (0x1 << j);
                    }
                }
                else
                {
                    initialState = initialState << 1;
                }
            }                          
        }
            
        pBuff[i] = currentValue;
        }    
}