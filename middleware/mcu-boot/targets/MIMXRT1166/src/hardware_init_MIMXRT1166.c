/*
 * Copyright 2018 - 2019 NXP
 *
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bl_semc.h"
#include "fusemap.h"
#include "memory_config.h"
#include "peripherals_pinmux.h"
#include "spi_nor_eeprom_memory.h"

////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define FREQ_396MHz (396000000U)
#define FREQ_480MHz (480000000U)
#define FREQ_528MHz (528000000U)
#define FREQ_24MHz (24000000U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void update_memory_map(void);

/*******************************************************************************
 * Codes
 ******************************************************************************/
//!@brief Configure IOMUX for SEMC Peripheral
void semc_iomux_config(semc_mem_config_t *config)
{
    uint32_t dataInoutPadCtlValue = SEMC_SW_PAD_CTL_VAL;
    uint32_t addrInputPadCtlValue = SEMC_SW_PAD_CTL_VAL;
    uint32_t rdyOutputPadCtlValue = SEMC_RDY_SW_PAD_CTL_VAL;
    uint32_t ctlInputPadCtlValue = SEMC_SW_PAD_CTL_VAL;
    uint8_t cePortOutputSelection;

    // Pinmux configuration for SEMC DA[15:0] Port (NOR)
    // Pinmux configuration for SEMC D[15:0] Port (NAND)
    IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_SEMC_DATA0_IDX] = SEMC_MUX_VAL;
    IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_SEMC_DATA0_IDX] = dataInoutPadCtlValue;
    IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_SEMC_DATA1_IDX] = SEMC_MUX_VAL;
    IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_SEMC_DATA1_IDX] = dataInoutPadCtlValue;
    IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_SEMC_DATA2_IDX] = SEMC_MUX_VAL;
    IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_SEMC_DATA2_IDX] = dataInoutPadCtlValue;
    IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_SEMC_DATA3_IDX] = SEMC_MUX_VAL;
    IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_SEMC_DATA3_IDX] = dataInoutPadCtlValue;
    IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_SEMC_DATA4_IDX] = SEMC_MUX_VAL;
    IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_SEMC_DATA4_IDX] = dataInoutPadCtlValue;
    IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_SEMC_DATA5_IDX] = SEMC_MUX_VAL;
    IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_SEMC_DATA5_IDX] = dataInoutPadCtlValue;
    IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_SEMC_DATA6_IDX] = SEMC_MUX_VAL;
    IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_SEMC_DATA6_IDX] = dataInoutPadCtlValue;
    IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_SEMC_DATA7_IDX] = SEMC_MUX_VAL;
    IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_SEMC_DATA7_IDX] = dataInoutPadCtlValue;
    if ((config->deviceMemType == kSemcDeviceMemType_NOR) ||
        ((config->deviceMemType == kSemcDeviceMemType_NAND) && (config->nandMemConfig.ioPortWidth == 16u)))
    {
        IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_SEMC_DATA8_IDX] = SEMC_MUX_VAL;
        IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_SEMC_DATA8_IDX] = dataInoutPadCtlValue;
        IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_SEMC_DATA9_IDX] = SEMC_MUX_VAL;
        IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_SEMC_DATA9_IDX] = dataInoutPadCtlValue;
        IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_SEMC_DATA10_IDX] = SEMC_MUX_VAL;
        IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_SEMC_DATA10_IDX] = dataInoutPadCtlValue;
        IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_SEMC_DATA11_IDX] = SEMC_MUX_VAL;
        IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_SEMC_DATA11_IDX] = dataInoutPadCtlValue;
        IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_SEMC_DATA12_IDX] = SEMC_MUX_VAL;
        IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_SEMC_DATA12_IDX] = dataInoutPadCtlValue;
        IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_SEMC_DATA13_IDX] = SEMC_MUX_VAL;
        IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_SEMC_DATA13_IDX] = dataInoutPadCtlValue;
        IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_SEMC_DATA14_IDX] = SEMC_MUX_VAL;
        IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_SEMC_DATA14_IDX] = dataInoutPadCtlValue;
        IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_SEMC_DATA15_IDX] = SEMC_MUX_VAL;
        IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_SEMC_DATA15_IDX] = dataInoutPadCtlValue;
    }

    // Pinmux configuration for SEMC WE,OE,ADV Port (NOR)
    // Pinmux configuration for SEMC WE,RE,ALE Port (NAND)
    IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_SEMC_ADDR11_IDX] = SEMC_MUX_VAL;
    IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_SEMC_ADDR11_IDX] = ctlInputPadCtlValue;
    IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_SEMC_ADDR12_IDX] = SEMC_MUX_VAL;
    IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_SEMC_ADDR12_IDX] = ctlInputPadCtlValue;
    IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_SEMC_BA1_IDX] = SEMC_MUX_VAL;
    IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_SEMC_BA1_IDX] = ctlInputPadCtlValue;

#if 0 // Still keep below logic in case we need to add it back in A1 ROM
    if (config->deviceMemType == kSemcDeviceMemType_NOR)
    {
        // Pinmux configuration for SEMC A[23:16], WAIT Port (NOR)
        IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_SEMC_BA0_IDX] = SEMC_MUX_VAL;
        IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_SEMC_BA0_IDX] = ctlInputPadCtlValue;
        IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_SEMC_ADDR0_IDX] = SEMC_MUX_VAL;
        IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_SEMC_ADDR0_IDX] = addrInputPadCtlValue;
        IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_SEMC_ADDR1_IDX] = SEMC_MUX_VAL;
        IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_SEMC_ADDR1_IDX] = addrInputPadCtlValue;
        IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_SEMC_ADDR2_IDX] = SEMC_MUX_VAL;
        IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_SEMC_ADDR2_IDX] = addrInputPadCtlValue;
        IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_SEMC_ADDR3_IDX] = SEMC_MUX_VAL;
        IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_SEMC_ADDR3_IDX] = addrInputPadCtlValue;
        IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_SEMC_ADDR4_IDX] = SEMC_MUX_VAL;
        IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_SEMC_ADDR4_IDX] = addrInputPadCtlValue;
        IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_SEMC_ADDR5_IDX] = SEMC_MUX_VAL;
        IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_SEMC_ADDR5_IDX] = addrInputPadCtlValue;
        IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_SEMC_ADDR6_IDX] = SEMC_MUX_VAL;
        IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_SEMC_ADDR6_IDX] = addrInputPadCtlValue;
        IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_SEMC_ADDR7_IDX] = SEMC_MUX_VAL;
        IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_SEMC_ADDR7_IDX] = addrInputPadCtlValue;

        cePortOutputSelection = config->norMemConfig.cePortOutputSelection;
    }
    else if (config->deviceMemType == kSemcDeviceMemType_NAND)
#endif
    {
        // Pinmux configuration for SEMC CLE,R/B Port (NAND)
        IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_SEMC_RDY_IDX] = SEMC_MUX_VAL;
        IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_SEMC_RDY_IDX] = rdyOutputPadCtlValue;
        IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_SEMC_ADDR9_IDX] = SEMC_MUX_VAL;
        IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_SEMC_ADDR9_IDX] = ctlInputPadCtlValue;

        cePortOutputSelection = config->nandMemConfig.cePortOutputSelection;
    }

    // Pinmux configuration for SEMC CE Port (NAND/NOR)
    switch (cePortOutputSelection)
    {
        case kSemcCeOutputSelection_MUX_A8:
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_SEMC_ADDR8_IDX] = SEMC_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_SEMC_ADDR8_IDX] = ctlInputPadCtlValue;
            break;
        case kSemcCeOutputSelection_MUX_CSX3:
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_SEMC_CSX3_IDX] = SEMC_CSX3_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_SEMC_CSX3_IDX] = ctlInputPadCtlValue;
            break;
        case kSemcCeOutputSelection_MUX_CSX2:
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_SEMC_CSX2_IDX] = SEMC_CSX2_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_SEMC_CSX2_IDX] = ctlInputPadCtlValue;
            break;
        case kSemcCeOutputSelection_MUX_CSX1:
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_SEMC_CSX1_IDX] = SEMC_CSX1_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_SEMC_CSX1_IDX] = ctlInputPadCtlValue;
            break;
        case kSemcCeOutputSelection_MUX_RDY:
        case kSemcCeOutputSelection_MUX_CSX0:
        default:
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_SEMC_CSX0_IDX] = SEMC_CSX0_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_SEMC_CSX0_IDX] = ctlInputPadCtlValue;
            break;
    }
}

//!@brief Control pin for LPSPI Peripheral
void spi_pcs_pin_control(uint32_t instance, uint8_t pcsx, bool isSelected)
{
    uint32_t pcsPinNumber;
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
        gpio->DR_CLEAR = (1u << pcsPinNumber);
    }
    else
    {
        /* Set PCS pin output to logic 1 */
        gpio->DR_SET = (1u << pcsPinNumber);
    }
}

//!@brief Configure clock for LPSPI peripheral
void spi_clock_config(uint32_t instance, spi_module_clock_freq_t freq)
{
    if ((instance >= 1) && (instance <= 4))
    {
        uint32_t clockDiv = 10;
        CCM->CLOCK_ROOT[kCLOCK_Root_Lpspi1 + instance - 1].CONTROL =
            CCM_CLOCK_ROOT_CONTROL_DIV(clockDiv - 1) | CCM_CLOCK_ROOT_CONTROL_MUX(2);
    }
}

//!@brief Configure IOMUX for LPSPI Peripheral
void spi_iomux_config(spi_nor_eeprom_peripheral_config_t *config)
{
    switch (config->spiIndex)
    {
        case 1:
            /* LPSPI1_SCK*/
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_LPSPI1_SCK_IDX] = LPSPI1_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_LPSPI1_SCK_IDX] = LPSPI1_SW_PAD_CTL_VAL;

            /* LPSPI1_SIN*/
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_LPSPI1_SIN_IDX] = LPSPI1_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_LPSPI1_SIN_IDX] = LPSPI1_SW_PAD_CTL_VAL;
            /* For input pin, we must set corresponding SELECT_INPUT register */
            IOMUXC->SELECT_INPUT[SELECT_INPUT_LPSPI1_SDI_IDX] = LPSPI1_SDI_SELECT_INPUT_VAL;

            /* LPSPI1_SOUT*/
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_LPSPI1_SOUT_IDX] = LPSPI1_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_LPSPI1_SOUT_IDX] = LPSPI1_SW_PAD_CTL_VAL;

            /* LPSPI1_PCS */
            /* Note: So far we cannot do separated send and recive operation in one active pcs period in LPSPI driver
             *  To achieve this goal, we should config PCSx pin as GPIO and control it manually. */
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_LPSPI1_PCS0_IDX] = GPIO_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_LPSPI1_PCS0_IDX] = LPSPI1_SW_PAD_CTL_VAL;

            /* Set PCS pin direction as general-purpose output */
            LPSPI1_PCS_GPIO->GDIR |= (1u << LPSPI1_PCS_GPIO_NUM);
            /* Set PCS pin output as logic 1 */
            LPSPI1_PCS_GPIO->DR |= (1u << LPSPI1_PCS_GPIO_NUM);

            break;

        case 2:
            /* LPSPI2_SCK*/
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_LPSPI2_SCK_IDX] = LPSPI2_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_LPSPI2_SCK_IDX] = LPSPI2_SW_PAD_CTL_VAL;

            /* LPSPI2_SIN*/
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_LPSPI2_SIN_IDX] = LPSPI2_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_LPSPI2_SIN_IDX] = LPSPI2_SW_PAD_CTL_VAL;
            /* For input pin, we must set corresponding SELECT_INPUT register */
            IOMUXC->SELECT_INPUT[SELECT_INPUT_LPSPI2_SDI_IDX] = LPSPI2_SDI_SELECT_INPUT_VAL;

            /* LPSPI2_SOUT*/
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_LPSPI2_SOUT_IDX] = LPSPI2_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_LPSPI2_SOUT_IDX] = LPSPI2_SW_PAD_CTL_VAL;

            /* LPSPI2_PCS */
            /* Note: So far we cannot do separated send and recive operation in one active pcs period in LPSPI driver
             *  To achieve this goal, we should config PCSx pin as GPIO and control it manually. */
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_LPSPI2_PCS0_IDX] = GPIO_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_LPSPI2_PCS0_IDX] = LPSPI2_SW_PAD_CTL_VAL;

            /* Set PCS pin direction as general-purpose output */
            LPSPI2_PCS_GPIO->GDIR |= (1u << LPSPI2_PCS_GPIO_NUM);
            /* Set PCS pin output as logic 1 */
            LPSPI2_PCS_GPIO->DR |= (1u << LPSPI2_PCS_GPIO_NUM);

            break;

        case 3:
            /* LPSPI3_SCK*/
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_LPSPI3_SCK_IDX] = LPSPI3_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_LPSPI3_SCK_IDX] = LPSPI3_SW_PAD_CTL_VAL;

            /* LPSPI3_SIN*/
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_LPSPI3_SIN_IDX] = LPSPI3_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_LPSPI3_SIN_IDX] = LPSPI3_SW_PAD_CTL_VAL;
            /* For input pin, we must set corresponding SELECT_INPUT register */
            IOMUXC->SELECT_INPUT[SELECT_INPUT_LPSPI3_SDI_IDX] = LPSPI3_SDI_SELECT_INPUT_VAL;

            /* LPSPI3_SOUT*/
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_LPSPI3_SOUT_IDX] = LPSPI3_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_LPSPI3_SOUT_IDX] = LPSPI3_SW_PAD_CTL_VAL;

            /* LPSPI3_PCS */
            /* Note: So far we cannot do separated send and recive operation in one active pcs period in LPSPI driver
             *  To achieve this goal, we should config PCSx pin as GPIO and control it manually. */
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_LPSPI3_PCS0_IDX] = GPIO_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_LPSPI3_PCS0_IDX] = LPSPI3_SW_PAD_CTL_VAL;

            /* Set PCS pin direction as general-purpose output */
            LPSPI3_PCS_GPIO->GDIR |= (1u << LPSPI3_PCS_GPIO_NUM);
            /* Set PCS pin output as logic 1 */
            LPSPI3_PCS_GPIO->DR |= (1u << LPSPI3_PCS_GPIO_NUM);

            break;

        case 4:
            /* LPSPI4_SCK*/
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_LPSPI4_SCK_IDX] = LPSPI4_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_LPSPI4_SCK_IDX] = LPSPI4_SW_PAD_CTL_VAL;

            /* LPSPI4_SIN*/
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_LPSPI4_SIN_IDX] = LPSPI4_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_LPSPI4_SIN_IDX] = LPSPI4_SW_PAD_CTL_VAL;
            /* For input pin, we must set corresponding SELECT_INPUT register */
            IOMUXC->SELECT_INPUT[SELECT_INPUT_LPSPI4_SDI_IDX] = LPSPI2_SDI_SELECT_INPUT_VAL;

            /* LPSPI4_SOUT*/
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_LPSPI4_SOUT_IDX] = LPSPI4_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_LPSPI4_SOUT_IDX] = LPSPI4_SW_PAD_CTL_VAL;

            /* LPSPI4_PCS */
            /* Note: So far we cannot do separated send and recive operation in one active pcs period in LPSPI driver
             *  To achieve this goal, we should config PCSx pin as GPIO and control it manually. */
            IOMUXC->SW_MUX_CTL_PAD[SW_MUX_CTL_PAD_LPSPI4_PCS0_IDX] = GPIO_MUX_VAL;
            IOMUXC->SW_PAD_CTL_PAD[SW_PAD_CTL_PAD_LPSPI4_PCS0_IDX] = LPSPI4_SW_PAD_CTL_VAL;

            /* Set PCS pin direction as general-purpose output */
            LPSPI4_PCS_GPIO->GDIR |= (1u << LPSPI4_PCS_GPIO_NUM);
            /* Set PCS pin output as logic 1 */
            LPSPI4_PCS_GPIO->DR |= (1u << LPSPI4_PCS_GPIO_NUM);

            break;
        default:
            break;
    }
}

bool is_boot_pin_asserted(void)
{
    // Boot pin for Flash only target
    return false;
}

//!@brief Get Primary boot device type
uint32_t get_primary_boot_device(void)
{
    uint8_t flash_device = 0xFF;

    switch (FUSE_BOOT_DEVICE_VALUE)
    {
        /* 0000 */
        case 0x0:
            flash_device = kBootDevice_FlexSpiNOR; // FlexSPI NOR
            break;
        /* 001x */
        case 0x2:
        case 0x3:
            flash_device = kBootDevice_SemcNAND; // Semc NAND
            break;
        /* 01xx */
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
            flash_device = kBootDevice_SD; // SD
            break;
        /* 10xx */
        case 0x8:
        case 0x9:
        case 0xa:
        case 0xb:
            flash_device = kBootDevice_MMC; // MMC/eMMC
            break;
        /* 11xx */
        case 0xc:
        case 0xd:
        case 0xe:
        case 0xf:
            flash_device = kBootDevice_FlexSpiNAND; // MMC/eMMC
            break;
    }

    return flash_device;
}

#if __ICCARM__

size_t __write(int handle, const unsigned char *buf, size_t size)
{
    return size;
}

#endif // __ICCARM__

void update_available_peripherals()
{
    // Flashloader always uses NXP Kboot PID and VID.
}

void init_hardware(void)
{
    // Configure Clocks
    configure_clocks(kClockOption_EnterBootloader);

    // Update memory map according to actual Fuse definitions
    update_memory_map();

    CLOCK_EnableClock(kCLOCK_Usb);
}

void deinit_hardware(void)
{
    CLOCK_DisableClock(kCLOCK_Usb);
}

//!@brief Get the hab status.
habstatus_option_t get_hab_status(void)
{
    habstatus_option_t habStatus = kHabStatus_Close;

    if (FUSE_FIELD_RETURN_VALUE)
    {
        habStatus = kHabStatus_Open;
    }
    else
    {
        if (FUSE_SEC_CONFIG_VALUE == 1)
        {
            habStatus = kHabStatus_Open;
        }
        else if (FUSE_SEC_CONFIG_VALUE < 1)
        {
            habStatus = kHabStatus_Open;
        }
        else
        {
            habStatus = kHabStatus_Close;
        }
    }

    return habStatus;
}

bool is_cm4_boot(void)
{
    enum
    {
        kPartNumber_CM4 = 0xC24,
        kPartNumber_CM7 = 0xC27,
    };

    uint32_t partNumber = (SCB->CPUID & SCB_CPUID_PARTNO_Msk) >> SCB_CPUID_PARTNO_Pos;
    if (partNumber == kPartNumber_CM4)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void update_memory_map(void)
{
    typedef struct
    {
        uint16_t dtcmSize;
        uint16_t itcmSize;
        uint16_t ocramSize;
    } flexram_cfg_t;

    const flexram_cfg_t k_flexramCfgList[] = {
        { 256, 256, 0 },   { 320, 192, 0 },  { 384, 128, 0 },  { 448, 64, 0 },    { 512, 0, 0 },    { 192, 320, 0 },
        { 128, 384, 0 },   { 64, 448, 0 },   { 0, 512, 0 },    { 256, 192, 64 },  { 320, 128, 64 }, { 384, 64, 64 },
        { 448, 0, 64 },    { 192, 256, 64 }, { 128, 320, 64 }, { 64, 384, 64 },   { 0, 448, 64 },   { 192, 192, 128 },
        { 256, 128, 128 }, { 328, 64, 128 }, { 384, 0, 128 },  { 128, 256, 128 }, { 64, 320, 128 }, { 0, 384, 128 },
        { 192, 128, 192 }, { 256, 64, 192 }, { 320, 0, 192 },  { 128, 192, 192 }, { 64, 256, 192 }, { 0, 320, 192 },
        { 128, 128, 256 }, { 192, 64, 256 }, { 256, 0, 256 },  { 64, 192, 256 },  { 0, 256, 256 },  { 128, 64, 320 },
        { 192, 0, 320 },   { 64, 128, 320 }, { 0, 192, 320 },  { 64, 64, 384 },   { 128, 0, 384 },  { 0, 128, 384 },
        { 64, 0, 448 },    { 0, 64, 448 },   { 0, 0, 512 },
    };

    uint32_t flexramCfgIndex = FUSE_FLEXRAM_CFG_VALUE;
    if (flexramCfgIndex >= ARRAY_SIZE(k_flexramCfgList))
    {
        // Reset the device and try to reboot again in case this issue is caused by abnormals
        NVIC_SystemReset();
    }
    debug_printf("FLEXRAM config, index = %x, dtcmSize = %x, itcmSize = %x, ocramSize = %x\n", flexramCfgIndex,
                 k_flexramCfgList[flexramCfgIndex].dtcmSize, k_flexramCfgList[flexramCfgIndex].itcmSize,
                 k_flexramCfgList[flexramCfgIndex].ocramSize);

    if (is_cm4_boot())
    {
        // ITCM + DTCM
        /*
         * Note: DTCM is next to ITCM, merge DTCM into ITCM to make boundary-crossing access pass.
         */
        g_memoryMap[kIndexITCM].startAddress = M4_TCM_SRAM_START_ADDRESS;
        g_memoryMap[kIndexITCM].endAddress = M4_TCM_SRAM_END_ADDRESS;

        // Duplicated index, the same with kIndexITCM.
        g_memoryMap[kIndexDTCM].startAddress = M4_TCM_SRAM_START_ADDRESS;
        g_memoryMap[kIndexDTCM].endAddress = M4_TCM_SRAM_END_ADDRESS;

        // OCRAM, in CM7, RAM address 0x2020_0000 - 0x2023_ffff is used as ITCM and DTCM,
        // So SW should avoid using above address
        uint32_t flexramOcramSize = 1024u * k_flexramCfgList[flexramCfgIndex].ocramSize;
        uint32_t fixedOcramSize = 1536u * 1024u;
        g_memoryMap[kIndexOCRAM].startAddress = OCRAM_START_ADDRESS;
        g_memoryMap[kIndexOCRAM].endAddress =
            g_memoryMap[kIndexOCRAM].startAddress + fixedOcramSize + flexramOcramSize - 1;
    }
    else
    {
        uint32_t itcmSize = 1024u * k_flexramCfgList[flexramCfgIndex].itcmSize;
        uint32_t dtcmSize = 1024u * k_flexramCfgList[flexramCfgIndex].dtcmSize;
        uint32_t flexramOcramSize = 1024u * k_flexramCfgList[flexramCfgIndex].ocramSize;

        // ITCM
        g_memoryMap[kIndexITCM].startAddress = M7_ITCM_SRAM_START_ADDRESS;
        g_memoryMap[kIndexITCM].endAddress = g_memoryMap[kIndexITCM].startAddress + itcmSize - 1;

        // DTCM
        g_memoryMap[kIndexDTCM].startAddress = M7_DTCM_SRAM_START_ADDRESS;
        g_memoryMap[kIndexDTCM].endAddress = g_memoryMap[kIndexDTCM].startAddress + dtcmSize - 1;

        // OCRAM
        uint32_t fixedOcramSize = M4_OCRAM_SIZE + OCRAM1_SIZE + OCRAM2_SIZE + OCRAM_ECC_SIZE + FLEXRAM_ECC_SIZE;
        g_memoryMap[kIndexOCRAM].startAddress = OCRAM_START_ADDRESS;
        g_memoryMap[kIndexOCRAM].endAddress =
            g_memoryMap[kIndexOCRAM].startAddress + fixedOcramSize + flexramOcramSize - 1;
    }
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
