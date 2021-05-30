/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "slcd_engine.h"
#include <stdio.h>
#include <stdint.h>

int32_t SLCD_Engine_Show_Num(tSLCD_Engine *slcd_engine, int32_t num, int32_t pos, int32_t on)
{
    int32_t slcd_pin = 0, pin_val = 0;
    const uint16_t **ppSLCD_NumPos = NULL;
    const uint16_t *pSLCD_NumPosNVal = NULL;

    if ((pos < 0) || (pos >= NUM_POSEND))
    {
        return -1;
    }
    ppSLCD_NumPos = SLCD_NumPos[pos];
    /* A number is comprised of several bytes' value */
    for (pSLCD_NumPosNVal = ppSLCD_NumPos[num]; *pSLCD_NumPosNVal != 0; pSLCD_NumPosNVal++)
    {
        slcd_pin = *pSLCD_NumPosNVal >> 8;
        pin_val = *pSLCD_NumPosNVal & 0xff;
        slcd_engine->slcd_set_pin_func(SLCD_Set_Num, slcd_pin, pin_val, on);
    }

    return 0;
}

int32_t SLCD_Engine_Show_Icon(tSLCD_Engine *slcd_engine, int32_t icon_pos, int32_t on)
{
    int32_t slcd_pin = 0;
    int32_t pin_val = 0;
    uint16_t pos_val = 0U;

    if ((icon_pos < 0) || (icon_pos >= ICON_END))
    {
        return -1;
    }
    pos_val = SLCD_Icon[icon_pos];
    slcd_pin = pos_val >> 8;
    pin_val = pos_val & 0xff;
    slcd_engine->slcd_set_pin_func(SLCD_Set_Icon, slcd_pin, pin_val, on);

    return 0;
}

void SLCD_Engine_Init(tSLCD_Engine *slcd_engine, SLCD_SET_PIN_FUNC pslcd_hw_handler)
{
    slcd_engine->slcd_set_pin_func = pslcd_hw_handler;
}
