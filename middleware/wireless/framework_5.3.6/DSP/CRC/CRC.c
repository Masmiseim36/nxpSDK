/*
* The Clear BSD License
* Copyright 2016-2017 NXP
* All rights reserved.
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
