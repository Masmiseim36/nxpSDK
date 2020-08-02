/*
 * The Clear BSD License
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
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
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
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

/*******************************************************************************
* File Name          : lcd.h
* Version            : V1.1.0
* Date               : 11/13/2009
* Description        : This file contains all the functions prototypes for the
*                      lcd firmware driver.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD_H
#define __LCD_H

#include "fsl_gpio.h"

/* Includes ------------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* LCD is Manley's ZYMG12864 and driver IC is ST7565R */

#define LCD_CD_HIGH() GPIO_PortSet(PTC, 1 << 1)
#define LCD_CD_LOW() GPIO_PortClear(PTC, 1 << 1)

#ifdef SW_SPI

#include "gpio.h"
#include "sw_spi.h"
#define LCD_SPI_SEND(x) SW_SPI_RxTx(x)
#define LCD_CLK_HIGH() FSL_LCD_CLK_HIGH()
#define LCD_CLK_LOW() FSL_LCD_CLK_LOW()
#define LCD_DATA_HIGH() FSL_LCD_DATA_HIGH()
#define LCD_DATA_LOW() FSL_LCD_DATA_LOW()

#define LCD_CLK_HIGH() GPIO_PortSet(PTC, 1 << 5)
#define LCD_CLK_LOW() GPIO_PortClear(PTC, 1 << 5)
#define LCD_DATA_HIGH() GPIO_PortSet(PTD, 1 << 2)
#define LCD_DATA_LOW() GPIO_PortClear(PTD, 1 << 2)
#else
#define LCD_SPI_SEND(x) LCD_Spi_Send(x)
#endif

// define the arrtibute of ZYMG12864(LCM)
#define GUI_LCM_XMAX 128 // defined the lcd's line-number is 128
#define GUI_LCM_YMAX 64  // defined the lcd's column-number is 64
#define GUI_LCM_PAGE 8   // defined the lcd's page-number is 8(GUI_LCM_YMAX/8)

/* set LCD command */
#define Display_On 0xAF  // A0,RD,WR:010
#define Display_Off 0xAE // A0,RD,WR:010

#define Set_Start_Line_0 0x40 // A0,RD,WR:010; line0~line63
#define Set_Page_Addr_0 0xB0  // A0,RD,WR:010; addr0~addr8
#define Set_ColH_Addr_0 0x10  // A0,RD,WR:010;
#define Set_ColL_Addr_0 0x00  // A0,RD,WR:010; addr0~addr131

#define Read_Status 0x - 0 // A0,RD,WR:001; BUSY | ADC | ON/OFF |  RESET | 0 0 0 0
#define Status_Busy 0x80
#define Status_ADC_Reverse 0x40 // column address 131-n : SEG n, else column address n : SEG n
#define Status_Display_Off 0x20
#define Status_Reset 0x80

#define Write_Data 0x -- // A0,RD,WR:110
#define Read_Date 0x --  // A0,RD,WR:101; spi mode is unavailable

#define Set_ADC_Normal 0xA0     // A0,RD,WR:010
#define Set_ADC_Reverse 0xA1    // A0,RD,WR:010
#define Display_Normal 0xA6     // A0,RD,WR:010
#define Display_Reverse 0xA7    // A0,RD,WR:010; reverse color
#define Display_All_On 0xA5     // A0,RD,WR:010
#define Display_All_Normal 0xA4 // A0,RD,WR:010

/*************************************************************
*	bias:	1/65duty | 1/49duty | 1/33duty | 1/55duty | 1/53duty *
*	---------------|----------|----------|----------|--------- *
*	A2: 	1/9 bias | 1/8 bias | 1/6 bias | 1/8 bias | 1/8 bias *
*	A3:		1/7 bias | 1/6 bias | 1/5 bias | 1/6 bias | 1/6 bias *
**************************************************************/

#define Set_LCD_Bias_7 0xA3 // A0,RD,WR:010
#define Set_LCD_Bias_9 0xA2 // A0,RD,WR:010

#define RMW_Mode_Enable 0xE0 // A0,RD,WR:010; the column address locked when read command operating
#define RMW_Mode_End 0xEE    // A0,RD,WR:010; returns to the column address when RMW was entered.
#define Reset_LCD 0xE2       // A0,RD,WR:010

/**************************************************************************************
*	Com Scan Dir:	|	1/65duty 		| 1/49duty 		| 1/33duty 		| 1/55duty 		| 1/53duty		*
*	--------------|-------------|-------------|-------------|------------------------ 	*
*	C0: 	Normal	|	COM0:COM63	|	COM0:COM47	|	COM0:COM31	|	COM0:COM53	|	COM0:COM51	*
*	C8:		Reverse	|	COM63:COM0	|	COM47:COM0	|	COM31:COM0	|	COM53:COM0	|	COM51:COM0	*
***************************************************************************************/

#define COM_Scan_Dir_Normal 0xC0  // A0,RD,WR:010
#define COM_Scan_Dir_Reverse 0xC8 // A0,RD,WR:010

// 0 0 1 0 1 | Booster On | Regulator On | Follower On
#define Power_Booster_On 0x2C   // A0,RD,WR:010
#define Power_Regulator_On 0x2E // A0,RD,WR:010
#define Power_Follower_On 0x2F  // A0,RD,WR:010

#define Set_Resistor_Ratio 0x20 // A0,RD,WR:010; 20~27:small~large

#define Set_ElecVol_Mode 0x81 // A0,RD,WR:010; double byte command
#define Set_ElecVol_Reg 0x20  // A0,RD,WR:010; the electronic volume(64 voltage levels:00~3F) function is not used.

#define Sleep_Mode_Enable 0xAC  // A0,RD,WR:010; double byte command, preceding command
#define Sleep_Mode_Disable 0xAD // A0,RD,WR:010; preceding command
#define Sleep_Mode_Deliver 0x00 // A0,RD,WR:010; following command

#define Boost_Ratio_Set 0xF8 // A0,RD,WR:010; double byte command, preceding command
#define Boost_Ratio_234 0x00 // A0,RD,WR:010; following command
#define Boost_Ratio_5 0x01   // A0,RD,WR:010; following command
#define Boost_Ratio_6 0x03   // A0,RD,WR:010; following command

#define Command_Nop 0xE3     // A0,RD,WR:010
#define Command_IC_Test 0xFC // A0,RD,WR:010; don't use

#define FONT_HSIZE 0x06
#define FONT_VSIZE 0x08
extern unsigned char ManleyLogo[];

/* Exported functions ------------------------------------------------------- */
void LCD_Initialize(void);
void LCD_Set_Update_Mode(unsigned char mode);
void LCD_ClearSCR(void);
void LCD_UpdatePoint(unsigned int x, unsigned int y);
void LCD_FillUpdate(unsigned char *data);
void LCD_Draw(void);
unsigned char LCD_PutChar(unsigned long x, unsigned long y, unsigned char ch);
void LCD_PutString(unsigned long x, unsigned long y, char *str);
void LCD_Power_On(void);
void LCD_Power_Off(void);
void LCD_Sleep_Enter(void);
void LCD_Sleep_Exit(void);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
void LCD_WriteCmd(unsigned char command);
void LCD_WriteData(unsigned char data);
void LCD_FillAll(unsigned char *data);
#endif /* __LCD_H */
