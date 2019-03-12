/*
 * Copyright 2014-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "bootloader/bl_context.h"
#include "bootloader_common.h"
#include "fsl_device_registers.h"
#include "lpuart/fsl_lpuart.h"
#include "utilities/fsl_assert.h"
#if BL_ENABLE_CRC_CHECK
#include "bootloader/bl_app_crc_check.h"
#endif
#if BL_FEATURE_FLEXSPI_NOR_MODULE || BL_FEATURE_SPINAND_MODULE
#include "flexspi/fsl_flexspi.h"
#include "flexspi_nor/flexspi_nor_flash.h"
#endif // #if BL_FEATURE_FLEXSPI_NOR_MODULE || BL_FEATURE_SPINAND_MODULE
#if BL_FEATURE_SPI_NOR_EEPROM_MODULE
#include "microseconds.h"
#include "memory/src/spi_nor_eeprom_memory.h"
#endif // BL_FEATURE_SPI_NOR_EEPROM_MODULE
#if BL_FEATURE_SEMC_NAND_MODULE || BL_FEATURE_SEMC_NOR_MODULE
#include "semc/fsl_semc.h"
#endif // #if BL_FEATURE_SEMC_NAND_MODULE || BL_FEATURE_SEMC_NOR_MODULE
#include "fusemap.h"
#include "peripherals_pinmux.h"

////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define FREQ_396MHz (396000000U)
#define FREQ_480MHz (480000000U)
#define FREQ_528MHz (528000000U)
#define FREQ_24MHz (24000000U)

/*******************************************************************************
 * Prototype
 ******************************************************************************/
bool is_flexspi_2nd_bootpin(void);

/*******************************************************************************
 * Codes
 ******************************************************************************/

#if BL_FEATURE_FLEXSPI_NOR_MODULE || BL_FEATURE_SPINAND_MODULE

//!@brief Configure IOMUX for FlexSPI Peripheral
void flexspi_iomux_config(uint32_t instance, flexspi_mem_config_t *config)
{
    uint32_t csPadCtlValue = FLEXSPI_SW_PAD_CTL_VAL;
    uint32_t dqsPadCtlValue = FLEXSPI_DQS_SW_PAD_CTL_VAL;
    uint32_t sclkPadCtlValue = FLEXSPI_SW_PAD_CTL_VAL;
    uint32_t dataPadCtlValue = FLEXSPI_SW_PAD_CTL_VAL;

    if (flexspi_is_padsetting_override_enable(config))
    {
        csPadCtlValue = config->csPadSettingOverride;
        dqsPadCtlValue = config->dqsPadSettingOverride;
        sclkPadCtlValue = config->sclkPadSettingOverride;
        dataPadCtlValue = config->dataPadSettingOverride;
    }

    // The primary FlexSPI pinmux, support octal Flash and up to 4 QuadSPI NOR Flash
    // Pinmux configuration for FLEXSPI PortA
    if (config->sflashA1Size)
    {
        // Basic pinmux configuration for FLEXSPI

        // FLEXSPIA_SS0_B
        IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_FLEXSPIA_SS0_B_IDX] = FLEXSPIA_MUX_VAL;
        IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_FLEXSPIA_SS0_B_IDX] = csPadCtlValue;

        // FLEXSPIA_SCLK
        IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_FLEXSPIA_SCLK_IDX] = FLEXSPIA_MUX_VAL | IOMUXC_SW_MUX_CTL_PAD_SION(1);
        IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_FLEXSPIA_SCLK_IDX] = sclkPadCtlValue;

        // FLEXSPIA_DATA0
        IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_FLEXSPIA_DATA0_IDX] = FLEXSPIA_MUX_VAL;
        IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_FLEXSPIA_DATA0_IDX] = dataPadCtlValue;

        // FLEXSPIA_DATA1
        IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_FLEXSPIA_DATA1_IDX] = FLEXSPIA_MUX_VAL;
        IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_FLEXSPIA_DATA1_IDX] = dataPadCtlValue;

        // FLEXSPIA_DATA2
        IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_FLEXSPIA_DATA2_IDX] = FLEXSPIA_MUX_VAL;
        IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_FLEXSPIA_DATA2_IDX] = dataPadCtlValue;

        // FLEXSPIA_DATA3
        IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_FLEXSPIA_DATA3_IDX] = FLEXSPIA_MUX_VAL;
        IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_FLEXSPIA_DATA3_IDX] = dataPadCtlValue;

#if FLEXSPI_ENABLE_OCTAL_FLASH_SUPPORT
        if (config->sflashPadType == kSerialFlash_8Pads)
        {
            // FLEXSPIA_DATA4 / FLEXSPIB_DATA0
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_FLEXSPIB_DATA0_IDX] = FLEXSPIA_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_FLEXSPIB_DATA0_IDX] = dataPadCtlValue;

            // FLEXSPIA_DATA5 / FLEXSPIB_DATA1
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_FLEXSPIB_DATA1_IDX] = FLEXSPIA_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_FLEXSPIB_DATA1_IDX] = dataPadCtlValue;

            // FLEXSPIA_DATA6 / FLEXSPIB_DATA2
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_FLEXSPIB_DATA2_IDX] = FLEXSPIA_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_FLEXSPIB_DATA2_IDX] = dataPadCtlValue;

            // FLEXSPIA_DATA7 / FLEXSPIB_DATA3
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_FLEXSPIB_DATA3_IDX] = FLEXSPIA_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_FLEXSPIB_DATA3_IDX] = dataPadCtlValue;
        }
#endif 

        // Configure DQS pad
        if ((config->readSampleClkSrc == kFlexSPIReadSampleClk_ExternalInputFromDqsPad) ||
            (config->readSampleClkSrc == kFlexSPIReadSampleClk_LoopbackFromDqsPad))
        {
            // FLEXSPIA_DQS
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_FLEXSPIA_DQS_IDX] = FLEXSPIA_MUX_VAL | IOMUXC_SW_MUX_CTL_PAD_SION(1);
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_FLEXSPIA_DQS_IDX] = dqsPadCtlValue;
        }

#if FLEXSPI_ENABLE_OCTAL_FLASH_SUPPORT
        // Configure Differential Clock pin
        if (flexspi_is_differential_clock_enable(config))
        {
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_FLEXSPIA_SCLK_B_IDX] = FLEXSPIA_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_FLEXSPIA_SCLK_B_IDX] = sclkPadCtlValue;
        }
#endif 
    }
}
#endif // BL_FEATURE_FLEXSPI_NOR_MODULE || BL_FEATURE_SPINAND_MODULE

#if BL_FEATURE_SPI_NOR_EEPROM_MODULE
//!@brief Gate on the clock for the LPSPI peripheral
void spi_clock_gate_enable(uint32_t instance)
{
    switch (instance)
    {
        case 1:
            CCM->CCGR1 |= CCM_CCGR1_CG0_MASK;
            break;
        case 2:
            CCM->CCGR1 |= CCM_CCGR1_CG1_MASK;
            break;
        case 3:
            CCM->CCGR1 |= CCM_CCGR1_CG2_MASK;
            break;
        case 4:
            CCM->CCGR1 |= CCM_CCGR1_CG3_MASK;
            break;
        default:
            break;
    }
}

//!@brief Control pin for LPSPI Peripheral
void spi_pcs_pin_control(uint32_t instance, uint8_t pcsx, bool isSelected)
{
    uint32_t pcsPinNumber;
    uint32_t pcsDelay_us;
    GPIO_Type *gpio = NULL;
    switch (instance)
    {
        case 1:
            pcsPinNumber = LPSPI1_PCS_GPIO_NUM;
            gpio = LPSPI1_PCS_GPIO;
            break;
        case 2:
            pcsPinNumber = LPSPI2_PCS_GPIO_NUM;
            gpio = LPSPI2_PCS_GPIO;
            break;
        case 3:
            pcsPinNumber = LPSPI3_PCS_GPIO_NUM;
            gpio = LPSPI3_PCS_GPIO;
            break;
        case 4:
            pcsPinNumber = LPSPI4_PCS_GPIO_NUM;
            gpio = LPSPI4_PCS_GPIO;
            break;
        default:
            return;
    }

    if (isSelected)
    {
        /* Clear PCS pin output to logic 0 */
        gpio->DR &= ~(1u << pcsPinNumber);
        pcsDelay_us = kSpiEepromModuleTiming_MinPcsSetupHold_ns;
    }
    else
    {
        /* Set PCS pin output as logic 1 */
        gpio->DR |= (1u << pcsPinNumber);
        pcsDelay_us = kSpiEepromModuleTiming_MinPcsHigh_ns;
    }

    /* Since we manualy control the pcs for LPSPI data transfer, we should make sure
     *  the timing is aligned with A.C. characteristics of EEPROM device*/
    if (pcsDelay_us % 1000)
    {
        pcsDelay_us /= 1000;
    }
    else
    {
        pcsDelay_us = (pcsDelay_us / 1000) + 1;
    }
    microseconds_delay(pcsDelay_us);
}

//!@brief Configure clock for LPSPI peripheral
void spi_clock_config(uint32_t instance, spi_module_clock_freq_t freq)
{
    /* LPSPI Clk source is controlled by CCM->CBCMR, the clk source selected for LPSPI
     *  is USB PLL (480MHz), see hapi_clock_init() in hapi_irom_clock.c */
    uint32_t topClkFreq = 246860000;
    uint32_t podf;

    uint32_t cbcmr = CCM->CBCMR & (~CCM_CBCMR_LPSPI_PODF_MASK);
    podf = topClkFreq / freq;
    cbcmr |= CCM_CBCMR_LPSPI_PODF(podf - 1);
    CCM->CBCMR = cbcmr;
}

//!@brief Configure IOMUX for LPSPI Peripheral
void spi_iomux_config(spi_nor_eeprom_peripheral_config_t *config)
{
    switch (config->spiIndex)
    {
        case 1:
            /* LPSPI1_SCK*/
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_LPSPI1_SCK_IDX] = LPSPI1_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_LPSPI1_SCK_IDX] = LPSPI_SW_PAD_CTL_VAL;

            /* LPSPI1_SIN*/
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_LPSPI1_SIN_IDX] = LPSPI1_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_LPSPI1_SIN_IDX] = LPSPI_SW_PAD_CTL_VAL;
            /* For input pin, we must set corresponding SELECT_INPUT register */
            IOMUXC->SELECT_INPUT[SELECT_INPUT_LPSPI1_SDI_IDX] = LPSPI1_SDI_SELECT_INPUT_VAL;

            /* LPSPI1_SOUT*/
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_LPSPI1_SOUT_IDX] = LPSPI1_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_LPSPI1_SOUT_IDX] = LPSPI_SW_PAD_CTL_VAL;

            /* LPSPI1_PCS */
            /* Note: So far we cannot do separated send and recive operation in one active pcs period in LPSPI driver
             *  To achieve this goal, we should config PCSx pin as GPIO and control it manually. */
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_LPSPI1_PCS0_IDX] = GPIO_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_LPSPI1_PCS0_IDX] = GPIO_SW_PAD_CTL_VAL;

            /* Set PCS pin direction as general-purpose output */
            LPSPI1_PCS_GPIO->GDIR |= (1u << LPSPI1_PCS_GPIO_NUM);
            /* Set PCS pin output as logic 1 */
            LPSPI1_PCS_GPIO->DR |= (1u << LPSPI1_PCS_GPIO_NUM);

            break;

        case 2:
            /* LPSPI2_SCK*/
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_LPSPI2_SCK_IDX] = LPSPI2_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_LPSPI2_SCK_IDX] = LPSPI_SW_PAD_CTL_VAL;

            /* LPSPI2_SIN*/
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_LPSPI2_SIN_IDX] = LPSPI2_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_LPSPI2_SIN_IDX] = LPSPI_SW_PAD_CTL_VAL;
            /* For input pin, we must set corresponding SELECT_INPUT register */
            IOMUXC->SELECT_INPUT[SELECT_INPUT_LPSPI2_SDI_IDX] = LPSPI2_SDI_SELECT_INPUT_VAL;

            /* LPSPI2_SOUT*/
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_LPSPI2_SOUT_IDX] = LPSPI2_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_LPSPI2_SOUT_IDX] = LPSPI_SW_PAD_CTL_VAL;

            /* LPSPI2_PCS */
            /* Note: So far we cannot do separated send and recive operation in one active pcs period in LPSPI driver
             *  To achieve this goal, we should config PCSx pin as GPIO and control it manually. */
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_LPSPI2_PCS0_IDX] = GPIO_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_LPSPI2_PCS0_IDX] = GPIO_SW_PAD_CTL_VAL;

            /* Set PCS pin direction as general-purpose output */
            LPSPI2_PCS_GPIO->GDIR |= (1u << LPSPI2_PCS_GPIO_NUM);
            /* Set PCS pin output as logic 1 */
            LPSPI2_PCS_GPIO->DR |= (1u << LPSPI2_PCS_GPIO_NUM);

            break;

        case 3:
            /* LPSPI3_SCK*/
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_LPSPI3_SCK_IDX] = LPSPI3_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_LPSPI3_SCK_IDX] = LPSPI_SW_PAD_CTL_VAL;

            /* LPSPI3_SIN*/
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_LPSPI3_SIN_IDX] = LPSPI3_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_LPSPI3_SIN_IDX] = LPSPI_SW_PAD_CTL_VAL;
#if defined(SELECT_INPUT_LPSPI3_SDI_IDX)
            /* For input pin, we must set corresponding SELECT_INPUT register */
            IOMUXC->SELECT_INPUT[SELECT_INPUT_LPSPI3_SDI_IDX] = LPSPI3_SDI_SELECT_INPUT_VAL;
#endif
            /* LPSPI3_SOUT*/
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_LPSPI3_SOUT_IDX] = LPSPI3_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_LPSPI3_SOUT_IDX] = LPSPI_SW_PAD_CTL_VAL;

            /* LPSPI3_PCS */
            /* Note: So far we cannot do separated send and recive operation in one active pcs period in LPSPI driver
             *  To achieve this goal, we should config PCSx pin as GPIO and control it manually. */
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_LPSPI3_PCS0_IDX] = GPIO_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_LPSPI3_PCS0_IDX] = GPIO_SW_PAD_CTL_VAL;

            /* Set PCS pin direction as general-purpose output */
            LPSPI3_PCS_GPIO->GDIR |= (1u << LPSPI3_PCS_GPIO_NUM);
            /* Set PCS pin output as logic 1 */
            LPSPI3_PCS_GPIO->DR |= (1u << LPSPI3_PCS_GPIO_NUM);

            break;
        case 4:
            /* LPSPI4_SCK*/
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_LPSPI4_SCK_IDX] = LPSPI4_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_LPSPI4_SCK_IDX] = LPSPI_SW_PAD_CTL_VAL;

            /* LPSPI4_SIN*/
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_LPSPI4_SIN_IDX] = LPSPI4_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_LPSPI4_SIN_IDX] = LPSPI_SW_PAD_CTL_VAL;
            /* For input pin, we must set corresponding SELECT_INPUT register */
            IOMUXC->SELECT_INPUT[SELECT_INPUT_LPSPI4_SDI_IDX] = LPSPI4_SDI_SELECT_INPUT_VAL;

            /* LPSPI4_SOUT*/
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_LPSPI4_SOUT_IDX] = LPSPI4_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_LPSPI4_SOUT_IDX] = LPSPI_SW_PAD_CTL_VAL;

            /* LPSPI4_PCS */
            /* Note: So far we cannot do separated send and recive operation in one active pcs period in LPSPI driver
             *  To achieve this goal, we should config PCSx pin as GPIO and control it manually. */
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_LPSPI4_PCS0_IDX] = GPIO_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_LPSPI4_PCS0_IDX] = GPIO_SW_PAD_CTL_VAL;

            /* Set PCS pin direction as general-purpose output */
            LPSPI4_PCS_GPIO->GDIR |= (1u << LPSPI4_PCS_GPIO_NUM);
            /* Set PCS pin output as logic 1 */
            LPSPI4_PCS_GPIO->DR |= (1u << LPSPI4_PCS_GPIO_NUM);
            break;
        default:
            break;
    }
}
#endif // BL_FEATURE_SPI_NOR_EEPROM_MODULE

//! @brief Return uart clock frequency according to instance
uint32_t get_uart_clock(uint32_t instance)
{
    // LPUART1 clock has been configured to 20MHz in clock_configure
    uint32_t lpuart_clock = 20000000UL;

    return lpuart_clock;
}

/*
uint32_t read_autobaud_pin( uint32_t instance )
{
    switch(instance)
    {
        case 0:
            return (GPIO_RD_PDIR(GPIOB) >> UART0_RX_GPIO_PIN_NUM) & 1;
        case 1:
            return (GPIO_RD_PDIR(GPIOC) >> UART1_RX_GPIO_PIN_NUM) & 1;
        case 2:
            return (GPIO_RD_PDIR(GPIOD) >> UART2_RX_GPIO_PIN_NUM) & 1;
        default:
            return 0;
    }
}
*/
bool is_boot_pin_asserted(void)
{
    // Boot pin for Flash only target
    return false;
}

//!@brief Get Primary boot device type
uint32_t get_primary_boot_device(void)
{
#define MEMORY_DEV_HIGH_MASK 0xC0
#define MEMORY_DEV_HIGH_SHIFT 0x06
#define MEMORY_DEV_LOW_MASK 0x30
#define MEMORY_DEV_LOW_SHIFT 0x04

    uint8_t flash_device = 0xFF;

    uint32_t high_value = (SRC->SBMR1 & MEMORY_DEV_HIGH_MASK) >> MEMORY_DEV_HIGH_SHIFT;
    uint32_t low_value = (SRC->SBMR1 & MEMORY_DEV_LOW_MASK) >> MEMORY_DEV_LOW_SHIFT;

    switch (high_value)
    {
        case 0:
            switch (low_value)
            {
                case 0:
                    flash_device = kBootDevice_FlexSpiNOR; // FlexSPI NOR
                    break;
            }
            break;
        case 3:
            flash_device = kBootDevice_FlexSpiNAND; // FlexSPI NAND
            break;
    }

    return flash_device;
}

#if BL_FEATURE_FLEXSPI_NOR_MODULE || BL_FEATURE_SPINAND_MODULE
// Set failsafe settings
status_t flexspi_set_failsafe_setting(flexspi_mem_config_t *config)
{
    status_t status = kStatus_InvalidArgument;
    do
    {
        if (config == NULL)
        {
            break;
        }
// This is an example that shows how to override the default pad setting in ROM, for now, the pad setting in ROM is
// idential to below values
// So, below codes are not required.
#if 0
        // See IOMUXC pad setting definitions for more details.
        config->controllerMiscOption |= (1<<kFlexSpiMiscOffset_PadSettingOverrideEnable);
        config->dqsPadSettingOverride = 0x130f1;
        config->sclkPadSettingOverride = 0x10f1;
        config->csPadSettingOverride = 0x10f1;
        config->dataPadSettingOverride = 0x10f1;
#endif
        if (config->readSampleClkSrc == kFlexSPIReadSampleClk_ExternalInputFromDqsPad)
        {
            if (config->controllerMiscOption & (1 << kFlexSpiMiscOffset_DdrModeEnable))
            {
                config->dataValidTime[0].time_100ps = 19; // 1.9 ns // 1/4 * cycle of 133MHz DDR
            }
            else
            {
                if (config->dataValidTime[0].delay_cells < 1)
                {
                    config->dataValidTime[0].time_100ps = 38; // 3.8 ns // 1/2 * cycle of 133MHz DDR
                }
            }
        }
        status = kStatus_Success;

    } while (0);

    return status;
}
#endif // #if BL_FEATURE_FLEXSPI_NOR_MODULE || BL_FEATURE_SPINAND_MODULE

void dummy_byte_callback(uint8_t byte)
{
    (void)byte;
}

void debug_init(void)
{
}

#if __ICCARM__

size_t __write(int handle, const unsigned char *buf, size_t size)
{
    return size;
}

#endif // __ICCARM__

void update_available_peripherals()
{
}

void init_hardware(void)
{
    CLOCK_EnableClock(kCLOCK_UsbOh3);
}

void deinit_hardware(void)
{
    CLOCK_DisableClock(kCLOCK_UsbOh3);
}

//!@brief Write FlexSPI persistent content
status_t flexspi_nor_write_persistent(const uint32_t data)
{
    SRC->GPR[2] = data;

    return kStatus_Success;
}
//!@brief Read FlexSPI persistent content
status_t flexspi_nor_read_persistent(uint32_t *data)
{
    *data = SRC->GPR[2];

    return kStatus_Success;
}

//!@brief Get the hab status.
habstatus_option_t get_hab_status(void)
{
    if (ROM_OCOTP_SEC_CONFIG_VALUE() & 0x2)
    {
        return kHabStatus_Close;
    }
    else
    {
        return kHabStatus_Open;
    }
}

void flexspi_update_padsetting(flexspi_mem_config_t *config, uint32_t driveStrength)
{
#define IOMUXC_PAD_SETTING_DSE_SHIFT (3)
#define IOMUXC_PAD_SETTING_DSE_MASK (0x07 << IOMUXC_PAD_SETTING_DSE_SHIFT)
#define IOMUXC_PAD_SETTING_DSE(x) (((x) << IOMUXC_PAD_SETTING_DSE_SHIFT) & IOMUXC_PAD_SETTING_DSE_MASK)
    if (driveStrength)
    {
        config->controllerMiscOption |= FLEXSPI_BITMASK(kFlexSpiMiscOffset_PadSettingOverrideEnable);
        config->dqsPadSettingOverride =
            (FLEXSPI_DQS_SW_PAD_CTL_VAL & ~IOMUXC_PAD_SETTING_DSE_MASK) | IOMUXC_PAD_SETTING_DSE(driveStrength);
        config->sclkPadSettingOverride =
            (FLEXSPI_SW_PAD_CTL_VAL & ~IOMUXC_PAD_SETTING_DSE_MASK) | IOMUXC_PAD_SETTING_DSE(driveStrength);
        config->dataPadSettingOverride =
            (FLEXSPI_SW_PAD_CTL_VAL & ~IOMUXC_PAD_SETTING_DSE_MASK) | IOMUXC_PAD_SETTING_DSE(driveStrength);

        config->csPadSettingOverride =
            (FLEXSPI_DQS_SW_PAD_CTL_VAL & ~IOMUXC_PAD_SETTING_DSE_MASK) | IOMUXC_PAD_SETTING_DSE(driveStrength);
    }
}

void normal_mem_init(void)
{
    typedef struct
    {
        uint32_t dtcmSizeKB;
        uint32_t itcmSizeKB;
        uint32_t ocramSizeKB;
    } flexram_cfg_t;

    const flexram_cfg_t k_flexramCfgList[] = {
        { 64, 64, 128 }, { 128, 64, 64 }, { 128, 0, 128 },  { 128, 32, 96 }, { 64, 128, 64 }, { 192, 0, 64 },
        { 64, 32, 160 }, { 64, 0, 192 },  { 256, 64, 192 }, { 32, 64, 160 }, { 32, 128, 96 }, { 32, 160, 64 },
        { 0, 128, 128 }, { 32, 32, 192 }, { 0, 192, 64 },   { 0, 0, 256 },
    };

    uint32_t ramCfgIndex = ROM_OCOTP_FLEXRAM_CFG_VALUE();

    uint32_t itcmSize = k_flexramCfgList[ramCfgIndex].itcmSizeKB * 1024u;
    uint32_t dtcmSize = k_flexramCfgList[ramCfgIndex].dtcmSizeKB * 1024u;
    uint32_t ocramSize = k_flexramCfgList[ramCfgIndex].ocramSizeKB * 1024u;

    if (itcmSize < 1)
    {
        itcmSize = 1;
    }
    if (dtcmSize < 1)
    {
        dtcmSize = 1;
    }
    if (ocramSize < 1)
    {
        ocramSize = 1;
    }

    g_memoryMap[0].startAddress = 0;
    g_memoryMap[0].endAddress = g_memoryMap[0].startAddress + itcmSize - 1;
    g_memoryMap[1].startAddress = 0x20000000;
    g_memoryMap[1].endAddress = g_memoryMap[1].startAddress + dtcmSize - 1;
    g_memoryMap[2].startAddress = 0x20200000;
    g_memoryMap[2].endAddress = g_memoryMap[2].startAddress + ocramSize - 1;
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
