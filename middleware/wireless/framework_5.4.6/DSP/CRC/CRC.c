/*! *********************************************************************************
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#include "CRC.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
uint32_t CRC_Compute(CRC_config_t crcConfig, uint8_t *dataIn, uint16_t length)
{
    uint32_t shiftReg = crcConfig.crcSeed << ((4 - crcConfig.crcSize) << 3);
    uint32_t crcPoly = crcConfig.crcPoly << ((4 - crcConfig.crcSize) << 3);
    uint32_t crcXorOut = crcConfig.crcXorOut << ((4 - crcConfig.crcSize) << 3);
    uint16_t startOffset = crcConfig.crcStartByte;
    uint8_t crcBits = 8 * crcConfig.crcSize;
    uint32_t computedCRC = 0;
    uint32_t i, j;
    uint8_t data = 0;
    uint8_t bit;
        
    /* Size 0 will bypass CRC calculation. */
    if (crcConfig.crcSize != 0)
    {
        for (i = 0 + startOffset; i < length; i++)
        {
            data = dataIn[i];
            
            if (crcConfig.crcRefIn != gCrcRefInput)
            {
                bit = 0;
                for (j = 0; j < 8; j++)
                {
                    bit = (bit << 1);
                    bit |= (data & 1) ? 1 : 0;
                    data = (data >> 1);
                }
                data = bit;
            }
            
            for (j = 0; j < 8; j++)
            {
                bit = (data & 0x80) ? 1 : 0;
                data = (data << 1);
                
                if (shiftReg & (1 << 31))
                {
                    bit = (bit) ? 0 : 1;
                }
                
                shiftReg = (shiftReg << 1);
                
                if (bit)
                {
                    shiftReg ^= crcPoly;
                }
                
                if (bit && (crcPoly & (1 << (32 - crcBits))))
                {
                    shiftReg |= (1 << (32 - crcBits));
                }
                else
                {
                    shiftReg &= ~(1 << (32 - crcBits));
                }
            }
        }
        
        shiftReg ^= crcXorOut;
                
        if (crcConfig.crcByteOrder == gCrcMSByteFirst)
        {
            computedCRC = (shiftReg >> (32 - crcBits));
        }
        else
        {
            computedCRC = 0;
            j = 1;
            for (i = 0; i < 32; i++)
            {
                computedCRC = (computedCRC << 1);
                computedCRC |= (shiftReg & j) ? 1 : 0;
                j = (j << 1);
            }
        }
    }
    
    return computedCRC;
}
