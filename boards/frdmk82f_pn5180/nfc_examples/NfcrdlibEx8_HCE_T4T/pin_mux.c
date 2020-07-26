/*
 * Copyright 2018-2019 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v5.0
processor: MK82FN256xxx15
package_id: MK82FN256VLL15
mcu_data: ksdk2_0
processor_version: 0.0.18
board: FRDM-K82F
pin_labels:
- {pin_num: '17', pin_signal: USB0_DP, label: 'J11[3]/USB_DP'}
- {pin_num: '18', pin_signal: USB0_DM, label: 'J11[2]/USB_DN'}
- {pin_num: '90', pin_signal: PTC17/LPUART3_TX/FB_CS4_b/FB_TSIZ0/FB_BE31_24_BLS7_0_b/SDRAM_DQM3, label: 'U12[3]', identifier: USB_FLAG}
- {pin_num: '40', pin_signal: TSI0_CH5/PTA4/LLWU_P3/FTM0_CH1/FXIO0_D14/EMVSIM0_VCCEN/NMI_b, label: SW2, identifier: SW2}
- {pin_num: '39', pin_signal: TSI0_CH4/PTA3/LPUART0_RTS_b/FTM0_CH0/FXIO0_D13/EMVSIM0_RST/JTAG_TMS/SWD_DIO, label: 'J19[2]'}
- {pin_num: '33', pin_signal: VBAT, label: VBAT, identifier: VBAT}
- {pin_num: '6', pin_signal: VDDIO_E6, label: VDDIO_E_1V8}
- {pin_num: '15', pin_signal: VDDIO_E17, label: VDDIO_E_1V8}
- {pin_num: '20', pin_signal: VREGIN, label: P5V_K81F}
- {pin_num: '23', pin_signal: VREFH, label: VDD_K81F}
- {pin_num: '22', pin_signal: VDDA, label: VDD_K81F}
- {pin_num: '34', pin_signal: VDD46, label: VDD_K81F}
- {pin_num: '48', pin_signal: VDD70, label: VDD_K81F}
- {pin_num: '61', pin_signal: VDD94, label: VDD_K81F}
- {pin_num: '75', pin_signal: VDD108, label: VDD_K81F}
- {pin_num: '89', pin_signal: VDD122, label: VDD_K81F}
- {pin_num: '99', pin_signal: VDD135, label: VDD_K81F}
- {pin_num: '19', pin_signal: VOUT33, label: C43/VOUT33}
- {pin_num: '31', pin_signal: XTAL32, label: 'Y1[2]/XTAL32_RTC', identifier: XTAL32K}
- {pin_num: '32', pin_signal: EXTAL32, label: 'Y1[1]/EXTAL32_RTC', identifier: EXTAL32K}
- {pin_num: '50', pin_signal: EXTAL0/PTA18/FTM0_FLT2/FTM_CLKIN0/TPM_CLKIN0, label: 'Y2[3]/EXTAL', identifier: EXTAL0}
- {pin_num: '51', pin_signal: XTAL0/PTA19/FTM1_FLT0/FTM_CLKIN1/LPTMR0_ALT1/LPTMR1_ALT1/TPM_CLKIN1, label: 'Y2[1]/XTAL', identifier: XTAL0}
- {pin_num: '24', pin_signal: VREFL, label: GND}
- {pin_num: '25', pin_signal: VSSA, label: GND}
- {pin_num: '5', pin_signal: VSS5, label: GND}
- {pin_num: '16', pin_signal: VSS23, label: GND}
- {pin_num: '35', pin_signal: VSS47, label: GND}
- {pin_num: '49', pin_signal: VSS71, label: GND}
- {pin_num: '60', pin_signal: VSS93, label: GND}
- {pin_num: '74', pin_signal: VSS107, label: GND}
- {pin_num: '88', pin_signal: VSS121, label: GND}
- {pin_num: '98', pin_signal: VSS134, label: GND}
- {pin_num: '80', pin_signal: CMP0_IN2/PTC8/FTM3_CH4/I2S0_MCLK/FB_AD7/SDRAM_A15/FXIO0_D16, label: 'J1[14]/J12[17]/D6/LEDRGB_RED/Camera Reset', identifier: LED_RED;FLEXIO_RESET}
- {pin_num: '81', pin_signal: CMP0_IN3/PTC9/FTM3_CH5/I2S0_RX_BCLK/FB_AD6/SDRAM_A14/FTM2_FLT0/FXIO0_D17, label: 'J1[16]/D7/LEDRGB_GREEN', identifier: LED_GREEN}
- {pin_num: '82', pin_signal: PTC10/I2C1_SCL/FTM3_CH6/I2S0_RX_FS/FB_AD5/SDRAM_A13/FXIO0_D18, label: 'J1[12]/D5/LEDRGB_BLUE', identifier: LED_BLUE}
- {pin_num: '36', pin_signal: TSI0_CH1/PTA0/LPUART0_CTS_b/FTM0_CH5/FXIO0_D10/EMVSIM0_CLK/JTAG_TCLK/SWD_CLK, label: 'J19[4]'}
- {pin_num: '38', pin_signal: TSI0_CH3/PTA2/LPUART0_TX/FTM0_CH7/I2C3_SCL/FXIO0_D12/EMVSIM0_PD/JTAG_TDO/TRACE_SWO, label: 'J2[20]J12[3]/J19[6]/U6[4]/I2C_SCL', identifier: I2C_SCL}
- {pin_num: '52', pin_signal: RESET_b, label: 'J3[6]/J19[10]/RESET'}
- {pin_num: '7', pin_signal: PTE4/LLWU_P2/SPI1_SIN/LPUART3_TX/SDHC0_D3/QSPI0A_DATA1, label: 'U11[2]/QSPIA_DATA1', identifier: QSPIA_DATA1}
- {pin_num: '3', pin_signal: PTE2/LLWU_P1/SPI1_SOUT/LPUART1_CTS_b/SDHC0_DCLK/QSPI0A_DATA0/SPI1_SCK, label: 'U11[5]/QSPIA_DATA0', identifier: QSPIA_DATA0}
- {pin_num: '4', pin_signal: PTE3/SPI1_PCS2/LPUART1_RTS_b/SDHC0_CMD/QSPI0A_DATA2/SPI1_SOUT, label: 'U11[3]/QSPIA_DATA2', identifier: QSPIA_DATA2}
- {pin_num: '1', pin_signal: PTE0/SPI1_PCS1/LPUART1_TX/SDHC0_D1/QSPI0A_DATA3/I2C1_SDA/RTC_CLKOUT, label: 'U11[7]/QSPIA_DATA3', identifier: QSPIA_DATA3}
- {pin_num: '8', pin_signal: PTE5/SPI1_PCS0/LPUART3_RX/SDHC0_D2/QSPI0A_SS0_B/FTM3_CH0/USB0_SOF_OUT, label: 'U11[1]/QSPIA_SS', identifier: QSPIA_SS}
- {pin_num: '2', pin_signal: PTE1/LLWU_P0/SPI1_SCK/LPUART1_RX/SDHC0_D0/QSPI0A_SCLK/I2C1_SCL/SPI1_SIN, label: 'U11[6]/QSPIA_SCLK', identifier: QSPIA_SCLK}
- {pin_num: '11', pin_signal: PTE8/I2S0_RXD1/SPI2_SOUT/I2S0_RX_FS/QSPI0B_DATA0/FTM3_CH3/SDHC0_D5, label: 'U13[5]/QSPIB_DATA0', identifier: QSPIB_DATA0}
- {pin_num: '12', pin_signal: PTE9/LLWU_P17/I2S0_TXD1/SPI2_PCS1/I2S0_RX_BCLK/QSPI0B_DATA2/FTM3_CH4/SDHC0_D6, label: 'U13[3]/QSPIB_DATA2', identifier: QSPIB_DATA2}
- {pin_num: '9', pin_signal: PTE6/LLWU_P16/SPI1_PCS3/LPUART3_CTS_b/I2S0_MCLK/QSPI0B_DATA3/FTM3_CH1/SDHC0_D4, label: 'U13[7]/QSPIB_DATA3', identifier: QSPIB_DATA3}
- {pin_num: '13', pin_signal: PTE10/LLWU_P18/I2C3_SDA/SPI2_SIN/I2S0_TXD0/QSPI0B_DATA1/FTM3_CH5/SDHC0_D7, label: 'U13[2]/QSPIB_DATA1', identifier: QSPIB_DATA1}
- {pin_num: '14', pin_signal: PTE11/I2C3_SCL/SPI2_PCS0/I2S0_TX_FS/QSPI0B_SS0_B/FTM3_CH6/QSPI0A_DQS, label: 'U13[1]/QSPIB_SS', identifier: QSPIB_SS0}
- {pin_num: '10', pin_signal: PTE7/SPI2_SCK/LPUART3_RTS_b/I2S0_RXD0/QSPI0B_SCLK/FTM3_CH2/QSPI0A_SS1_B, label: 'U13[6]/QSPIB_SCLK', identifier: QSPIB_SCLK}
- {pin_num: '64', pin_signal: TSI0_CH11/PTB18/FTM2_CH0/I2S0_TX_BCLK/FB_AD15/SDRAM_A23/FTM2_QD_PHA/TPM2_CH0/FXIO0_D6, label: 'J12[14]/E1/TSI_CH11/FXIO_D6', identifier: TSI_ELECTRODE_1}
- {pin_num: '65', pin_signal: TSI0_CH12/PTB19/FTM2_CH1/I2S0_TX_FS/FB_OE_b/FTM2_QD_PHB/TPM2_CH1/FXIO0_D7, label: 'J12[13]/E2/TSI_CH12/FXIO_D7', identifier: TSI_ELECTRODE_2}
- {pin_num: '73', pin_signal: CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/LPUART1_RX/FTM0_CH2/CLKOUT/I2S0_TX_BCLK, label: 'J2[2]/J12[8]/J24[3]/D8/BLUETOOTH_UART1_RX/XCLK', identifier: BT_RX;FLEXIO_DATA}
- {pin_num: '76', pin_signal: PTC4/LLWU_P8/SPI0_PCS0/LPUART1_TX/FTM0_CH3/FB_AD11/SDRAM_A19/CMP1_OUT, label: 'J24[4]/BLUETOOTH_UART1_TX', identifier: BT_TX}
- {pin_num: '55', pin_signal: ADC0_SE12/TSI0_CH7/PTB2/I2C0_SCL/LPUART0_RTS_b/SDRAM_WE/FTM0_FLT3/FXIO0_D2, label: 'J4[12]/J12[5]/A4/I2C0_SCL/FXIO_D2/VSYNC', identifier: FLEXIO_VSYNC}
- {pin_num: '53', pin_signal: ADC0_SE8/TSI0_CH0/PTB0/LLWU_P5/I2C0_SCL/FTM1_CH0/SDRAM_CAS_b/FTM1_QD_PHA/TPM1_CH0/FXIO0_D0, label: 'J4[2]/J12[7]/D0/A0/FXIO_D0/PCLK',
  identifier: FLEXIO_PCLK}
- {pin_num: '69', pin_signal: PTB23/SPI2_SIN/SPI0_PCS5/FB_AD28/SDRAM_D28/FXIO0_D11, label: 'J2[7]/J12[9]/J22[8]/FXIO_D11/RF_IRQ', identifier: FLEXIO_DATA}
- {pin_num: '67', pin_signal: PTB21/SPI2_SCK/FB_AD30/SDRAM_D30/CMP1_OUT/FXIO0_D9, label: 'J2[3]/J12[11]/FXIO_D9', identifier: FLEXIO_DATA}
- {pin_num: '59', pin_signal: PTB11/SPI1_SCK/LPUART3_TX/I2C2_SDA/FB_AD18/SDRAM_D18/FTM0_FLT2/FXIO0_D5, label: 'J12[15]/FXIO_D7', identifier: FLEXIO_DATA}
- {pin_num: '37', pin_signal: TSI0_CH2/PTA1/LPUART0_RX/FTM0_CH6/I2C3_SDA/FXIO0_D11/EMVSIM0_IO/JTAG_TDI, label: 'J2[18]/J12[4]/U6[6]/D14/I2C SDA', identifier: I2C_SDA}
- {pin_num: '56', pin_signal: ADC0_SE13/TSI0_CH8/PTB3/I2C0_SDA/LPUART0_CTS_b/SDRAM_CS0_b/FTM0_FLT0/FXIO0_D3, label: 'J4[10]/J12[6]/A4/I2C0_SDA/FXIO_D3/HREF', identifier: FLEXIO_HREF}
- {pin_num: '68', pin_signal: PTB22/SPI2_SOUT/FB_AD29/SDRAM_D29/FXIO0_D10, label: 'J2[5]/J12[10]/FXIO_D10', identifier: FLEXIO_DATA}
- {pin_num: '66', pin_signal: PTB20/SPI2_PCS0/FB_AD31/SDRAM_D31/CMP0_OUT/FXIO0_D8, label: 'J2[1]/J4[9]/J12[12]/FXIO_D8', identifier: FLEXIO_DATA}
- {pin_num: '58', pin_signal: PTB10/SPI1_PCS0/LPUART3_RX/I2C2_SCL/FB_AD19/SDRAM_D19/FTM0_FLT1/FXIO0_D4, label: 'J12[16]/FXIO_D4', identifier: FLEXIO_DATA}
- {pin_num: '57', pin_signal: PTB9/SPI1_PCS1/LPUART3_CTS_b/FB_AD20/SDRAM_D20, label: 'J12[9]/J22[4]/RF_SPI1_PCS1', identifier: CAM_PDWN}
- {pin_num: '70', pin_signal: ADC0_SE14/TSI0_CH13/PTC0/SPI0_PCS4/PDB0_EXTRG/USB0_SOF_OUT/FB_AD14/SDRAM_A22/I2S0_TXD1/FXIO0_D12, label: 'J2[9]/J22[3]/FXIO_D12/RF_GPIO_CE'}
- {pin_num: '96', pin_signal: ADC0_SE6b/PTD5/SPI0_PCS2/LPUART0_CTS_b/FTM0_CH5/FB_AD1/SDRAM_A9/EWM_OUT_b/SPI1_SCK, label: 'J22[5]/RF_SPI1_SCK'}
- {pin_num: '100', pin_signal: PTD7/CMT_IRO/LPUART0_TX/FTM0_CH7/SDRAM_CKE/FTM0_FLT1/SPI1_SIN, label: 'J22[7]/RF_SPI1_SIN'}
- {pin_num: '97', pin_signal: ADC0_SE7b/PTD6/LLWU_P15/SPI0_PCS3/LPUART0_RX/FTM0_CH6/FB_AD0/FTM0_FLT0/SPI1_SOUT, label: 'J22[6]/RF_SPI1_SOUT'}
- {pin_num: '85', pin_signal: PTC13/LPUART4_CTS_b/FTM_CLKIN1/FB_AD26/SDRAM_D26/TPM_CLKIN1, label: 'U6[11]', identifier: ACCEL_INT1}
- {pin_num: '62', pin_signal: TSI0_CH9/PTB16/SPI1_SOUT/LPUART0_RX/FTM_CLKIN0/FB_AD17/SDRAM_D17/EWM_IN/TPM_CLKIN0, label: 'J1[2]/D0_UART0_RX'}
- {pin_num: '63', pin_signal: TSI0_CH10/PTB17/SPI1_SIN/LPUART0_TX/FTM_CLKIN1/FB_AD16/SDRAM_D16/EWM_OUT_b/TPM_CLKIN1, label: 'J1[4]/D0_UART0_TX'}
- {pin_num: '84', pin_signal: PTC12/LPUART4_RTS_b/FTM_CLKIN0/FB_AD27/SDRAM_D27/FTM3_FLT0/TPM_CLKIN0, label: 'J1[6]/D2'}
- {pin_num: '91', pin_signal: PTD0/LLWU_P12/SPI0_PCS0/LPUART2_RTS_b/FTM3_CH0/FB_ALE/FB_CS1_b/FB_TS_b/FXIO0_D22, label: 'J1[8]/D3'}
- {pin_num: '83', pin_signal: PTC11/LLWU_P11/I2C1_SDA/FTM3_CH7/I2S0_RXD1/FB_RW_b/FXIO0_D19, label: 'J1[10]/D4'}
- {pin_num: '41', pin_signal: PTA5/USB0_CLKIN/FTM0_CH2/FXIO0_D15/I2S0_TX_BCLK/JTAG_TRST_b, label: 'J1[1]/I2S_TX_BCLK'}
- {pin_num: '43', pin_signal: PTA13/LLWU_P4/FTM1_CH1/TRACE_D3/FXIO0_D19/I2S0_TX_FS/FTM1_QD_PHB/TPM1_CH1, label: 'J1[3]/I2S_TX_FS'}
- {pin_num: '42', pin_signal: PTA12/FTM1_CH0/TRACE_CLKOUT/FXIO0_D18/I2S0_TXD0/FTM1_QD_PHA/TPM1_CH0, label: 'J1[5]/I2S_TXD'}
- {pin_num: '47', pin_signal: PTA17/SPI0_SIN/LPUART0_RTS_b/FXIO0_D23/I2S0_MCLK, label: 'J1[9]/I2S_MCLK'}
- {pin_num: '44', pin_signal: PTA14/SPI0_PCS0/LPUART0_TX/TRACE_D2/FXIO0_D20/I2S0_RX_BCLK/I2S0_TXD1, label: 'J1[9]/I2S_RX_BCLK'}
- {pin_num: '79', pin_signal: CMP0_IN1/PTC7/SPI0_SIN/USB0_SOF_OUT/I2S0_RX_FS/FB_AD8/SDRAM_A16/FXIO0_D15, label: 'J1[11]/SOF_OUT'}
- {pin_num: '46', pin_signal: PTA16/SPI0_SOUT/LPUART0_CTS_b/TRACE_D0/FXIO0_D22/I2S0_RX_FS/I2S0_RXD1, label: 'J1[13]/I2S_RX_FS'}
- {pin_num: '45', pin_signal: PTA15/SPI0_SCK/LPUART0_RX/TRACE_D1/FXIO0_D21/I2S0_RXD0, label: 'J1[15]/I2S_RXD'}
- {pin_num: '77', pin_signal: PTC5/LLWU_P9/SPI0_SCK/LPTMR0_ALT2/LPTMR1_ALT2/I2S0_RXD0/FB_AD10/SDRAM_A18/CMP0_OUT/FTM0_CH2, label: 'J2[4]/D9'}
- {pin_num: '95', pin_signal: PTD4/LLWU_P14/SPI0_PCS1/LPUART0_RTS_b/FTM0_CH4/FB_AD2/SDRAM_A10/EWM_IN/SPI1_PCS0, label: 'J2[6]/D10/SPI0_PCS1'}
- {pin_num: '93', pin_signal: PTD2/LLWU_P13/SPI0_SOUT/LPUART2_RX/FTM3_CH2/FB_AD4/SDRAM_A12/I2C0_SCL, label: 'J2[8]/D11/SPI0_SOUT'}
- {pin_num: '94', pin_signal: PTD3/SPI0_SIN/LPUART2_TX/FTM3_CH3/FB_AD3/SDRAM_A11/I2C0_SDA, label: 'J2[10]/D12/SPI0_IN'}
- {pin_num: '92', pin_signal: ADC0_SE5b/PTD1/SPI0_SCK/LPUART2_CTS_b/FTM3_CH1/FB_CS0_b/FXIO0_D23, label: 'J2[12]/D13/SPI0_SCK'}
- {pin_num: '29', pin_signal: DAC0_OUT/CMP1_IN3/ADC0_SE23, label: 'J4[11]/DAC0_OUT'}
- {pin_num: '72', pin_signal: ADC0_SE4b/CMP1_IN0/TSI0_CH15/PTC2/SPI0_PCS2/LPUART1_CTS_b/FTM0_CH1/FB_AD12/SDRAM_A20/I2S0_TX_FS, label: 'J4[8]/A3'}
- {pin_num: '71', pin_signal: ADC0_SE15/TSI0_CH14/PTC1/LLWU_P6/SPI0_PCS3/LPUART1_RTS_b/FTM0_CH0/FB_AD13/SDRAM_A21/I2S0_TXD0/FXIO0_D13, label: 'J4[6]/A2'}
- {pin_num: '54', pin_signal: ADC0_SE9/TSI0_CH6/PTB1/I2C0_SDA/FTM1_CH1/SDRAM_RAS_b/FTM1_QD_PHB/TPM1_CH1/FXIO0_D1, label: 'J4[4]/D1/A1'}
- {pin_num: '30', pin_signal: RTC_WAKEUP_B, label: 'J3[7]'}
- {pin_num: '26', pin_signal: ADC0_DP1, label: 'J3[5]'}
- {pin_num: '27', pin_signal: ADC0_DM1, label: 'J3[1]'}
- {pin_num: '87', pin_signal: PTC15/LPUART4_TX/FB_AD24/SDRAM_D24/FXIO0_D21, label: OpenSDA_UART, identifier: DEBUG_UART_TX}
- {pin_num: '86', pin_signal: PTC14/LPUART4_RX/FB_AD25/SDRAM_D25/FXIO0_D20, label: OpenSDA_UART, identifier: DEBUG_UART_RX}
- {pin_num: '78', pin_signal: CMP0_IN0/PTC6/LLWU_P10/SPI0_SOUT/PDB0_EXTRG/I2S0_RX_BCLK/FB_AD9/SDRAM_A17/I2S0_MCLK/FXIO0_D14, label: SW3, identifier: SW3}
- {pin_num: '28', pin_signal: VREF_OUT/CMP1_IN5/CMP0_IN5/ADC0_SE22, label: 'J3[3]/VREF_OUT'}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "pin_mux.h"

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 *
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void)
{
    BOARD_InitPins();
    BOARD_InitDEBUG_UARTPins();
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '38', peripheral: TPIU, signal: SWO, pin_signal: TSI0_CH3/PTA2/LPUART0_TX/FTM0_CH7/I2C3_SCL/FXIO0_D12/EMVSIM0_PD/JTAG_TDO/TRACE_SWO, identifier: '',
    pull_select: down, pull_enable: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPins(void)
{
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);

    /* PORTA2 (pin 38) is configured as TRACE_SWO */
    PORT_SetPinMux(PORTA, 2U, kPORT_MuxAlt7);

    PORTA->PCR[2] = ((PORTA->PCR[2] &
                      /* Mask bits to zero which are setting */
                      (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK)))

                     /* Pull Select: Internal pulldown resistor is enabled on the corresponding pin, if the
                      * corresponding PE field is set. */
                     | PORT_PCR_PS(kPORT_PullDown)

                     /* Pull Enable: Internal pullup or pulldown resistor is not enabled on the corresponding pin. */
                     | PORT_PCR_PE(kPORT_PullDisable));
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitButtonsPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '78', peripheral: GPIOC, signal: 'GPIO, 6', pin_signal: CMP0_IN0/PTC6/LLWU_P10/SPI0_SOUT/PDB0_EXTRG/I2S0_RX_BCLK/FB_AD9/SDRAM_A17/I2S0_MCLK/FXIO0_D14,
    direction: INPUT, slew_rate: slow, open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: '40', peripheral: GPIOA, signal: 'GPIO, 4', pin_signal: TSI0_CH5/PTA4/LLWU_P3/FTM0_CH1/FXIO0_D14/EMVSIM0_VCCEN/NMI_b, direction: INPUT, slew_rate: slow,
    open_drain: disable, pull_select: down, pull_enable: disable, passive_filter: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitButtonsPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitButtonsPins(void)
{
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    gpio_pin_config_t SW2_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTA4 (pin 40)  */
    GPIO_PinInit(BOARD_SW2_GPIO, BOARD_SW2_PIN, &SW2_config);

    gpio_pin_config_t SW3_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTC6 (pin 78)  */
    GPIO_PinInit(BOARD_SW3_GPIO, BOARD_SW3_PIN, &SW3_config);

    const port_pin_config_t SW2 = {/* Internal pull-up/down resistor is disabled */
                                   kPORT_PullDisable,
                                   /* Slow slew rate is configured */
                                   kPORT_SlowSlewRate,
                                   /* Passive filter is disabled */
                                   kPORT_PassiveFilterDisable,
                                   /* Open drain is disabled */
                                   kPORT_OpenDrainDisable,
                                   /* Low drive strength is configured */
                                   kPORT_LowDriveStrength,
                                   /* Pin is configured as PTA4 */
                                   kPORT_MuxAsGpio,
                                   /* Pin Control Register fields [15:0] are not locked */
                                   kPORT_UnlockRegister};
    /* PORTA4 (pin 40) is configured as PTA4 */
    PORT_SetPinConfig(BOARD_SW2_PORT, BOARD_SW2_PIN, &SW2);

    const port_pin_config_t SW3 = {/* Internal pull-up/down resistor is disabled */
                                   kPORT_PullDisable,
                                   /* Slow slew rate is configured */
                                   kPORT_SlowSlewRate,
                                   /* Passive filter is disabled */
                                   kPORT_PassiveFilterDisable,
                                   /* Open drain is disabled */
                                   kPORT_OpenDrainDisable,
                                   /* Low drive strength is configured */
                                   kPORT_LowDriveStrength,
                                   /* Pin is configured as PTC6 */
                                   kPORT_MuxAsGpio,
                                   /* Pin Control Register fields [15:0] are not locked */
                                   kPORT_UnlockRegister};
    /* PORTC6 (pin 78) is configured as PTC6 */
    PORT_SetPinConfig(BOARD_SW3_PORT, BOARD_SW3_PIN, &SW3);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitOSCPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '50', peripheral: OSC, signal: EXTAL0, pin_signal: EXTAL0/PTA18/FTM0_FLT2/FTM_CLKIN0/TPM_CLKIN0, slew_rate: no_init, open_drain: no_init, pull_select: no_init,
    pull_enable: no_init}
  - {pin_num: '51', peripheral: OSC, signal: XTAL0, pin_signal: XTAL0/PTA19/FTM1_FLT0/FTM_CLKIN1/LPTMR0_ALT1/LPTMR1_ALT1/TPM_CLKIN1, slew_rate: no_init, open_drain: no_init,
    pull_select: no_init, pull_enable: no_init}
  - {pin_num: '31', peripheral: RTC, signal: XTAL32, pin_signal: XTAL32}
  - {pin_num: '32', peripheral: RTC, signal: EXTAL32, pin_signal: EXTAL32}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitOSCPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitOSCPins(void)
{
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);

    /* PORTA18 (pin 50) is configured as EXTAL0 */
    PORT_SetPinMux(BOARD_EXTAL0_PORT, BOARD_EXTAL0_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTA19 (pin 51) is configured as XTAL0 */
    PORT_SetPinMux(BOARD_XTAL0_PORT, BOARD_XTAL0_PIN, kPORT_PinDisabledOrAnalog);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitLEDsPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '80', peripheral: GPIOC, signal: 'GPIO, 8', pin_signal: CMP0_IN2/PTC8/FTM3_CH4/I2S0_MCLK/FB_AD7/SDRAM_A15/FXIO0_D16, identifier: LED_RED, direction: OUTPUT,
    gpio_init_state: 'true', slew_rate: slow, open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: '81', peripheral: GPIOC, signal: 'GPIO, 9', pin_signal: CMP0_IN3/PTC9/FTM3_CH5/I2S0_RX_BCLK/FB_AD6/SDRAM_A14/FTM2_FLT0/FXIO0_D17, direction: OUTPUT,
    gpio_init_state: 'true', slew_rate: slow, open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: '82', peripheral: GPIOC, signal: 'GPIO, 10', pin_signal: PTC10/I2C1_SCL/FTM3_CH6/I2S0_RX_FS/FB_AD5/SDRAM_A13/FXIO0_D18, direction: OUTPUT, gpio_init_state: 'true',
    slew_rate: slow, open_drain: disable, pull_select: down, pull_enable: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitLEDsPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitLEDsPins(void)
{
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    gpio_pin_config_t LED_RED_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };
    /* Initialize GPIO functionality on pin PTC8 (pin 80)  */
    GPIO_PinInit(BOARD_LED_RED_GPIO, BOARD_LED_RED_PIN, &LED_RED_config);

    gpio_pin_config_t LED_GREEN_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };
    /* Initialize GPIO functionality on pin PTC9 (pin 81)  */
    GPIO_PinInit(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, &LED_GREEN_config);

    gpio_pin_config_t LED_BLUE_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };
    /* Initialize GPIO functionality on pin PTC10 (pin 82)  */
    GPIO_PinInit(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN, &LED_BLUE_config);

    const port_pin_config_t LED_BLUE = {/* Internal pull-up/down resistor is disabled */
                                        kPORT_PullDisable,
                                        /* Slow slew rate is configured */
                                        kPORT_SlowSlewRate,
                                        /* Passive filter is disabled */
                                        kPORT_PassiveFilterDisable,
                                        /* Open drain is disabled */
                                        kPORT_OpenDrainDisable,
                                        /* Low drive strength is configured */
                                        kPORT_LowDriveStrength,
                                        /* Pin is configured as PTC10 */
                                        kPORT_MuxAsGpio,
                                        /* Pin Control Register fields [15:0] are not locked */
                                        kPORT_UnlockRegister};
    /* PORTC10 (pin 82) is configured as PTC10 */
    PORT_SetPinConfig(BOARD_LED_BLUE_PORT, BOARD_LED_BLUE_PIN, &LED_BLUE);

    const port_pin_config_t LED_RED = {/* Internal pull-up/down resistor is disabled */
                                       kPORT_PullDisable,
                                       /* Slow slew rate is configured */
                                       kPORT_SlowSlewRate,
                                       /* Passive filter is disabled */
                                       kPORT_PassiveFilterDisable,
                                       /* Open drain is disabled */
                                       kPORT_OpenDrainDisable,
                                       /* Low drive strength is configured */
                                       kPORT_LowDriveStrength,
                                       /* Pin is configured as PTC8 */
                                       kPORT_MuxAsGpio,
                                       /* Pin Control Register fields [15:0] are not locked */
                                       kPORT_UnlockRegister};
    /* PORTC8 (pin 80) is configured as PTC8 */
    PORT_SetPinConfig(BOARD_LED_RED_PORT, BOARD_LED_RED_PIN, &LED_RED);

    const port_pin_config_t LED_GREEN = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Slow slew rate is configured */
                                         kPORT_SlowSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as PTC9 */
                                         kPORT_MuxAsGpio,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTC9 (pin 81) is configured as PTC9 */
    PORT_SetPinConfig(BOARD_LED_GREEN_PORT, BOARD_LED_GREEN_PIN, &LED_GREEN);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitDEBUG_UARTPins:
- options: {callFromInitBoot: 'true', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '87', peripheral: LPUART4, signal: TX, pin_signal: PTC15/LPUART4_TX/FB_AD24/SDRAM_D24/FXIO0_D21, direction: OUTPUT, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
  - {pin_num: '86', peripheral: LPUART4, signal: RX, pin_signal: PTC14/LPUART4_RX/FB_AD25/SDRAM_D25/FXIO0_D20, slew_rate: fast, open_drain: disable, pull_select: down,
    pull_enable: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitDEBUG_UARTPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitDEBUG_UARTPins(void)
{
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    const port_pin_config_t DEBUG_UART_RX = {/* Internal pull-up/down resistor is disabled */
                                             kPORT_PullDisable,
                                             /* Fast slew rate is configured */
                                             kPORT_FastSlewRate,
                                             /* Passive filter is disabled */
                                             kPORT_PassiveFilterDisable,
                                             /* Open drain is disabled */
                                             kPORT_OpenDrainDisable,
                                             /* Low drive strength is configured */
                                             kPORT_LowDriveStrength,
                                             /* Pin is configured as LPUART4_RX */
                                             kPORT_MuxAlt3,
                                             /* Pin Control Register fields [15:0] are not locked */
                                             kPORT_UnlockRegister};
    /* PORTC14 (pin 86) is configured as LPUART4_RX */
    PORT_SetPinConfig(BOARD_DEBUG_UART_RX_PORT, BOARD_DEBUG_UART_RX_PIN, &DEBUG_UART_RX);

    const port_pin_config_t DEBUG_UART_TX = {/* Internal pull-up/down resistor is disabled */
                                             kPORT_PullDisable,
                                             /* Fast slew rate is configured */
                                             kPORT_FastSlewRate,
                                             /* Passive filter is disabled */
                                             kPORT_PassiveFilterDisable,
                                             /* Open drain is disabled */
                                             kPORT_OpenDrainDisable,
                                             /* Low drive strength is configured */
                                             kPORT_LowDriveStrength,
                                             /* Pin is configured as LPUART4_TX */
                                             kPORT_MuxAlt3,
                                             /* Pin Control Register fields [15:0] are not locked */
                                             kPORT_UnlockRegister};
    /* PORTC15 (pin 87) is configured as LPUART4_TX */
    PORT_SetPinConfig(BOARD_DEBUG_UART_TX_PORT, BOARD_DEBUG_UART_TX_PIN, &DEBUG_UART_TX);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitACCELPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '37', peripheral: I2C3, signal: SDA, pin_signal: TSI0_CH2/PTA1/LPUART0_RX/FTM0_CH6/I2C3_SDA/FXIO0_D11/EMVSIM0_IO/JTAG_TDI, slew_rate: fast, open_drain: disable}
  - {pin_num: '38', peripheral: I2C3, signal: SCL, pin_signal: TSI0_CH3/PTA2/LPUART0_TX/FTM0_CH7/I2C3_SCL/FXIO0_D12/EMVSIM0_PD/JTAG_TDO/TRACE_SWO, slew_rate: fast,
    open_drain: disable}
  - {pin_num: '85', peripheral: GPIOC, signal: 'GPIO, 13', pin_signal: PTC13/LPUART4_CTS_b/FTM_CLKIN1/FB_AD26/SDRAM_D26/TPM_CLKIN1, direction: INPUT, slew_rate: slow,
    open_drain: enable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitACCELPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitACCELPins(void)
{
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    gpio_pin_config_t ACCEL_INT1_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTC13 (pin 85)  */
    GPIO_PinInit(BOARD_ACCEL_INT1_GPIO, BOARD_ACCEL_INT1_PIN, &ACCEL_INT1_config);

    /* PORTA1 (pin 37) is configured as I2C3_SDA */
    PORT_SetPinMux(BOARD_I2C_SDA_PORT, BOARD_I2C_SDA_PIN, kPORT_MuxAlt4);

    PORTA->PCR[1] = ((PORTA->PCR[1] &
                      /* Mask bits to zero which are setting */
                      (~(PORT_PCR_SRE_MASK | PORT_PCR_ODE_MASK | PORT_PCR_ISF_MASK)))

                     /* Slew Rate Enable: Fast slew rate is configured on the corresponding pin, if the pin is
                      * configured as a digital output. */
                     | PORT_PCR_SRE(kPORT_FastSlewRate)

                     /* Open Drain Enable: Open drain output is disabled on the corresponding pin. */
                     | PORT_PCR_ODE(kPORT_OpenDrainDisable));

    /* PORTA2 (pin 38) is configured as I2C3_SCL */
    PORT_SetPinMux(BOARD_I2C_SCL_PORT, BOARD_I2C_SCL_PIN, kPORT_MuxAlt4);

    PORTA->PCR[2] = ((PORTA->PCR[2] &
                      /* Mask bits to zero which are setting */
                      (~(PORT_PCR_SRE_MASK | PORT_PCR_ODE_MASK | PORT_PCR_ISF_MASK)))

                     /* Slew Rate Enable: Fast slew rate is configured on the corresponding pin, if the pin is
                      * configured as a digital output. */
                     | PORT_PCR_SRE(kPORT_FastSlewRate)

                     /* Open Drain Enable: Open drain output is disabled on the corresponding pin. */
                     | PORT_PCR_ODE(kPORT_OpenDrainDisable));

    /* PORTC13 (pin 85) is configured as PTC13 */
    PORT_SetPinMux(BOARD_ACCEL_INT1_PORT, BOARD_ACCEL_INT1_PIN, kPORT_MuxAsGpio);

    PORTC->PCR[13] = ((PORTC->PCR[13] &
                       /* Mask bits to zero which are setting */
                       (~(PORT_PCR_SRE_MASK | PORT_PCR_ODE_MASK | PORT_PCR_ISF_MASK)))

                      /* Slew Rate Enable: Slow slew rate is configured on the corresponding pin, if the pin is
                       * configured as a digital output. */
                      | PORT_PCR_SRE(kPORT_SlowSlewRate)

                      /* Open Drain Enable: Open drain output is enabled on the corresponding pin, if the pin is
                       * configured as a digital output. */
                      | PORT_PCR_ODE(kPORT_OpenDrainEnable));
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitQSPI_FLASHPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '1', peripheral: QuadSPI0, signal: 'DATA3, A', pin_signal: PTE0/SPI1_PCS1/LPUART1_TX/SDHC0_D1/QSPI0A_DATA3/I2C1_SDA/RTC_CLKOUT, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: '2', peripheral: QuadSPI0, signal: 'SCLK, A', pin_signal: PTE1/LLWU_P0/SPI1_SCK/LPUART1_RX/SDHC0_D0/QSPI0A_SCLK/I2C1_SCL/SPI1_SIN, slew_rate: fast,
    open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: '3', peripheral: QuadSPI0, signal: 'DATA0, A', pin_signal: PTE2/LLWU_P1/SPI1_SOUT/LPUART1_CTS_b/SDHC0_DCLK/QSPI0A_DATA0/SPI1_SCK, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: '4', peripheral: QuadSPI0, signal: 'DATA2, A', pin_signal: PTE3/SPI1_PCS2/LPUART1_RTS_b/SDHC0_CMD/QSPI0A_DATA2/SPI1_SOUT, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: '7', peripheral: QuadSPI0, signal: 'DATA1, A', pin_signal: PTE4/LLWU_P2/SPI1_SIN/LPUART3_TX/SDHC0_D3/QSPI0A_DATA1, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: '8', peripheral: QuadSPI0, signal: 'SS0, A', pin_signal: PTE5/SPI1_PCS0/LPUART3_RX/SDHC0_D2/QSPI0A_SS0_B/FTM3_CH0/USB0_SOF_OUT, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: '9', peripheral: QuadSPI0, signal: 'DATA3, B', pin_signal: PTE6/LLWU_P16/SPI1_PCS3/LPUART3_CTS_b/I2S0_MCLK/QSPI0B_DATA3/FTM3_CH1/SDHC0_D4, slew_rate: fast,
    open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: '10', peripheral: QuadSPI0, signal: 'SCLK, B', pin_signal: PTE7/SPI2_SCK/LPUART3_RTS_b/I2S0_RXD0/QSPI0B_SCLK/FTM3_CH2/QSPI0A_SS1_B, slew_rate: fast,
    open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: '11', peripheral: QuadSPI0, signal: 'DATA0, B', pin_signal: PTE8/I2S0_RXD1/SPI2_SOUT/I2S0_RX_FS/QSPI0B_DATA0/FTM3_CH3/SDHC0_D5, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: '12', peripheral: QuadSPI0, signal: 'DATA2, B', pin_signal: PTE9/LLWU_P17/I2S0_TXD1/SPI2_PCS1/I2S0_RX_BCLK/QSPI0B_DATA2/FTM3_CH4/SDHC0_D6, slew_rate: fast,
    open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: '13', peripheral: QuadSPI0, signal: 'DATA1, B', pin_signal: PTE10/LLWU_P18/I2C3_SDA/SPI2_SIN/I2S0_TXD0/QSPI0B_DATA1/FTM3_CH5/SDHC0_D7, slew_rate: fast,
    open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: '14', peripheral: QuadSPI0, signal: 'SS0, B', pin_signal: PTE11/I2C3_SCL/SPI2_PCS0/I2S0_TX_FS/QSPI0B_SS0_B/FTM3_CH6/QSPI0A_DQS, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitQSPI_FLASHPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitQSPI_FLASHPins(void)
{
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    const port_pin_config_t QSPIA_DATA3 = {/* Internal pull-up/down resistor is disabled */
                                           kPORT_PullDisable,
                                           /* Fast slew rate is configured */
                                           kPORT_FastSlewRate,
                                           /* Passive filter is disabled */
                                           kPORT_PassiveFilterDisable,
                                           /* Open drain is disabled */
                                           kPORT_OpenDrainDisable,
                                           /* Low drive strength is configured */
                                           kPORT_LowDriveStrength,
                                           /* Pin is configured as QSPI0A_DATA3 */
                                           kPORT_MuxAlt5,
                                           /* Pin Control Register fields [15:0] are not locked */
                                           kPORT_UnlockRegister};
    /* PORTE0 (pin 1) is configured as QSPI0A_DATA3 */
    PORT_SetPinConfig(BOARD_QSPIA_DATA3_PORT, BOARD_QSPIA_DATA3_PIN, &QSPIA_DATA3);

    const port_pin_config_t QSPIA_SCLK = {/* Internal pull-up/down resistor is disabled */
                                          kPORT_PullDisable,
                                          /* Fast slew rate is configured */
                                          kPORT_FastSlewRate,
                                          /* Passive filter is disabled */
                                          kPORT_PassiveFilterDisable,
                                          /* Open drain is disabled */
                                          kPORT_OpenDrainDisable,
                                          /* Low drive strength is configured */
                                          kPORT_LowDriveStrength,
                                          /* Pin is configured as QSPI0A_SCLK */
                                          kPORT_MuxAlt5,
                                          /* Pin Control Register fields [15:0] are not locked */
                                          kPORT_UnlockRegister};
    /* PORTE1 (pin 2) is configured as QSPI0A_SCLK */
    PORT_SetPinConfig(BOARD_QSPIA_SCLK_PORT, BOARD_QSPIA_SCLK_PIN, &QSPIA_SCLK);

    const port_pin_config_t QSPIB_DATA1 = {/* Internal pull-up/down resistor is disabled */
                                           kPORT_PullDisable,
                                           /* Fast slew rate is configured */
                                           kPORT_FastSlewRate,
                                           /* Passive filter is disabled */
                                           kPORT_PassiveFilterDisable,
                                           /* Open drain is disabled */
                                           kPORT_OpenDrainDisable,
                                           /* Low drive strength is configured */
                                           kPORT_LowDriveStrength,
                                           /* Pin is configured as QSPI0B_DATA1 */
                                           kPORT_MuxAlt5,
                                           /* Pin Control Register fields [15:0] are not locked */
                                           kPORT_UnlockRegister};
    /* PORTE10 (pin 13) is configured as QSPI0B_DATA1 */
    PORT_SetPinConfig(BOARD_QSPIB_DATA1_PORT, BOARD_QSPIB_DATA1_PIN, &QSPIB_DATA1);

    const port_pin_config_t QSPIB_SS0 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as QSPI0B_SS0_B */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTE11 (pin 14) is configured as QSPI0B_SS0_B */
    PORT_SetPinConfig(BOARD_QSPIB_SS0_PORT, BOARD_QSPIB_SS0_PIN, &QSPIB_SS0);

    const port_pin_config_t QSPIA_DATA0 = {/* Internal pull-up/down resistor is disabled */
                                           kPORT_PullDisable,
                                           /* Fast slew rate is configured */
                                           kPORT_FastSlewRate,
                                           /* Passive filter is disabled */
                                           kPORT_PassiveFilterDisable,
                                           /* Open drain is disabled */
                                           kPORT_OpenDrainDisable,
                                           /* Low drive strength is configured */
                                           kPORT_LowDriveStrength,
                                           /* Pin is configured as QSPI0A_DATA0 */
                                           kPORT_MuxAlt5,
                                           /* Pin Control Register fields [15:0] are not locked */
                                           kPORT_UnlockRegister};
    /* PORTE2 (pin 3) is configured as QSPI0A_DATA0 */
    PORT_SetPinConfig(BOARD_QSPIA_DATA0_PORT, BOARD_QSPIA_DATA0_PIN, &QSPIA_DATA0);

    const port_pin_config_t QSPIA_DATA2 = {/* Internal pull-up/down resistor is disabled */
                                           kPORT_PullDisable,
                                           /* Fast slew rate is configured */
                                           kPORT_FastSlewRate,
                                           /* Passive filter is disabled */
                                           kPORT_PassiveFilterDisable,
                                           /* Open drain is disabled */
                                           kPORT_OpenDrainDisable,
                                           /* Low drive strength is configured */
                                           kPORT_LowDriveStrength,
                                           /* Pin is configured as QSPI0A_DATA2 */
                                           kPORT_MuxAlt5,
                                           /* Pin Control Register fields [15:0] are not locked */
                                           kPORT_UnlockRegister};
    /* PORTE3 (pin 4) is configured as QSPI0A_DATA2 */
    PORT_SetPinConfig(BOARD_QSPIA_DATA2_PORT, BOARD_QSPIA_DATA2_PIN, &QSPIA_DATA2);

    const port_pin_config_t QSPIA_DATA1 = {/* Internal pull-up/down resistor is disabled */
                                           kPORT_PullDisable,
                                           /* Fast slew rate is configured */
                                           kPORT_FastSlewRate,
                                           /* Passive filter is disabled */
                                           kPORT_PassiveFilterDisable,
                                           /* Open drain is disabled */
                                           kPORT_OpenDrainDisable,
                                           /* Low drive strength is configured */
                                           kPORT_LowDriveStrength,
                                           /* Pin is configured as QSPI0A_DATA1 */
                                           kPORT_MuxAlt5,
                                           /* Pin Control Register fields [15:0] are not locked */
                                           kPORT_UnlockRegister};
    /* PORTE4 (pin 7) is configured as QSPI0A_DATA1 */
    PORT_SetPinConfig(BOARD_QSPIA_DATA1_PORT, BOARD_QSPIA_DATA1_PIN, &QSPIA_DATA1);

    const port_pin_config_t QSPIA_SS = {/* Internal pull-up/down resistor is disabled */
                                        kPORT_PullDisable,
                                        /* Fast slew rate is configured */
                                        kPORT_FastSlewRate,
                                        /* Passive filter is disabled */
                                        kPORT_PassiveFilterDisable,
                                        /* Open drain is disabled */
                                        kPORT_OpenDrainDisable,
                                        /* Low drive strength is configured */
                                        kPORT_LowDriveStrength,
                                        /* Pin is configured as QSPI0A_SS0_B */
                                        kPORT_MuxAlt5,
                                        /* Pin Control Register fields [15:0] are not locked */
                                        kPORT_UnlockRegister};
    /* PORTE5 (pin 8) is configured as QSPI0A_SS0_B */
    PORT_SetPinConfig(BOARD_QSPIA_SS_PORT, BOARD_QSPIA_SS_PIN, &QSPIA_SS);

    const port_pin_config_t QSPIB_DATA3 = {/* Internal pull-up/down resistor is disabled */
                                           kPORT_PullDisable,
                                           /* Fast slew rate is configured */
                                           kPORT_FastSlewRate,
                                           /* Passive filter is disabled */
                                           kPORT_PassiveFilterDisable,
                                           /* Open drain is disabled */
                                           kPORT_OpenDrainDisable,
                                           /* Low drive strength is configured */
                                           kPORT_LowDriveStrength,
                                           /* Pin is configured as QSPI0B_DATA3 */
                                           kPORT_MuxAlt5,
                                           /* Pin Control Register fields [15:0] are not locked */
                                           kPORT_UnlockRegister};
    /* PORTE6 (pin 9) is configured as QSPI0B_DATA3 */
    PORT_SetPinConfig(BOARD_QSPIB_DATA3_PORT, BOARD_QSPIB_DATA3_PIN, &QSPIB_DATA3);

    const port_pin_config_t QSPIB_SCLK = {/* Internal pull-up/down resistor is disabled */
                                          kPORT_PullDisable,
                                          /* Fast slew rate is configured */
                                          kPORT_FastSlewRate,
                                          /* Passive filter is disabled */
                                          kPORT_PassiveFilterDisable,
                                          /* Open drain is disabled */
                                          kPORT_OpenDrainDisable,
                                          /* Low drive strength is configured */
                                          kPORT_LowDriveStrength,
                                          /* Pin is configured as QSPI0B_SCLK */
                                          kPORT_MuxAlt5,
                                          /* Pin Control Register fields [15:0] are not locked */
                                          kPORT_UnlockRegister};
    /* PORTE7 (pin 10) is configured as QSPI0B_SCLK */
    PORT_SetPinConfig(BOARD_QSPIB_SCLK_PORT, BOARD_QSPIB_SCLK_PIN, &QSPIB_SCLK);

    const port_pin_config_t QSPIB_DATA0 = {/* Internal pull-up/down resistor is disabled */
                                           kPORT_PullDisable,
                                           /* Fast slew rate is configured */
                                           kPORT_FastSlewRate,
                                           /* Passive filter is disabled */
                                           kPORT_PassiveFilterDisable,
                                           /* Open drain is disabled */
                                           kPORT_OpenDrainDisable,
                                           /* Low drive strength is configured */
                                           kPORT_LowDriveStrength,
                                           /* Pin is configured as QSPI0B_DATA0 */
                                           kPORT_MuxAlt5,
                                           /* Pin Control Register fields [15:0] are not locked */
                                           kPORT_UnlockRegister};
    /* PORTE8 (pin 11) is configured as QSPI0B_DATA0 */
    PORT_SetPinConfig(BOARD_QSPIB_DATA0_PORT, BOARD_QSPIB_DATA0_PIN, &QSPIB_DATA0);

    const port_pin_config_t QSPIB_DATA2 = {/* Internal pull-up/down resistor is disabled */
                                           kPORT_PullDisable,
                                           /* Fast slew rate is configured */
                                           kPORT_FastSlewRate,
                                           /* Passive filter is disabled */
                                           kPORT_PassiveFilterDisable,
                                           /* Open drain is disabled */
                                           kPORT_OpenDrainDisable,
                                           /* Low drive strength is configured */
                                           kPORT_LowDriveStrength,
                                           /* Pin is configured as QSPI0B_DATA2 */
                                           kPORT_MuxAlt5,
                                           /* Pin Control Register fields [15:0] are not locked */
                                           kPORT_UnlockRegister};
    /* PORTE9 (pin 12) is configured as QSPI0B_DATA2 */
    PORT_SetPinConfig(BOARD_QSPIB_DATA2_PORT, BOARD_QSPIB_DATA2_PIN, &QSPIB_DATA2);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitTSI_TOUCH_PADPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '64', peripheral: TSI0, signal: 'CH, 11', pin_signal: TSI0_CH11/PTB18/FTM2_CH0/I2S0_TX_BCLK/FB_AD15/SDRAM_A23/FTM2_QD_PHA/TPM2_CH0/FXIO0_D6, slew_rate: fast,
    open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: '65', peripheral: TSI0, signal: 'CH, 12', pin_signal: TSI0_CH12/PTB19/FTM2_CH1/I2S0_TX_FS/FB_OE_b/FTM2_QD_PHB/TPM2_CH1/FXIO0_D7, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitTSI_TOUCH_PADPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitTSI_TOUCH_PADPins(void)
{
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);

    const port_pin_config_t TSI_ELECTRODE_1 = {/* Internal pull-up/down resistor is disabled */
                                               kPORT_PullDisable,
                                               /* Fast slew rate is configured */
                                               kPORT_FastSlewRate,
                                               /* Passive filter is disabled */
                                               kPORT_PassiveFilterDisable,
                                               /* Open drain is disabled */
                                               kPORT_OpenDrainDisable,
                                               /* Low drive strength is configured */
                                               kPORT_LowDriveStrength,
                                               /* Pin is configured as TSI0_CH11 */
                                               kPORT_PinDisabledOrAnalog,
                                               /* Pin Control Register fields [15:0] are not locked */
                                               kPORT_UnlockRegister};
    /* PORTB18 (pin 64) is configured as TSI0_CH11 */
    PORT_SetPinConfig(BOARD_TSI_ELECTRODE_1_PORT, BOARD_TSI_ELECTRODE_1_PIN, &TSI_ELECTRODE_1);

    const port_pin_config_t TSI_ELECTRODE_2 = {/* Internal pull-up/down resistor is disabled */
                                               kPORT_PullDisable,
                                               /* Fast slew rate is configured */
                                               kPORT_FastSlewRate,
                                               /* Passive filter is disabled */
                                               kPORT_PassiveFilterDisable,
                                               /* Open drain is disabled */
                                               kPORT_OpenDrainDisable,
                                               /* Low drive strength is configured */
                                               kPORT_LowDriveStrength,
                                               /* Pin is configured as TSI0_CH12 */
                                               kPORT_PinDisabledOrAnalog,
                                               /* Pin Control Register fields [15:0] are not locked */
                                               kPORT_UnlockRegister};
    /* PORTB19 (pin 65) is configured as TSI0_CH12 */
    PORT_SetPinConfig(BOARD_TSI_ELECTRODE_2_PORT, BOARD_TSI_ELECTRODE_2_PIN, &TSI_ELECTRODE_2);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
