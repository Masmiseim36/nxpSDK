/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_mipi_csi2rx.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.mipi_csi2rx"
#endif

#if (defined(FSL_FEATURE_CSI2RX_CSR_OFFSET) && FSL_FEATURE_CSI2RX_CSR_OFFSET)
#define CSI2RX_GET_CSR(csi_base) (MIPI_CSI_CSR_Type *)((uint32_t)(csi_base)-FSL_FEATURE_CSI2RX_CSR_OFFSET)
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Returns an instance number given a base address.
 *
 * If an invalid base address is passed, debug builds will assert. Release builds will just return
 * instance number 0.
 *
 * @param base The CSI2RX peripheral base address.
 * @return CSI2RX instance number starting from 0.
 */
uint32_t CSI2RX_GetInstance(MIPI_CSI2RX_Type *base);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Array to map MIPI CSI2RX instance number to base address. */
static const uint32_t s_csi2rxBaseAddrs[] = MIPI_CSI2RX_BASE_ADDRS;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/*! @brief Pointers to MPI CSI2RX clocks for each instance. */
static const clock_ip_name_t s_csi2rxClocks[] = MIPI_CSI2RX_CLOCKS;
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

/*******************************************************************************
 * Code
 ******************************************************************************/

uint32_t CSI2RX_GetInstance(MIPI_CSI2RX_Type *base)
{
    uint32_t i;

    for (i = 0U; i < ARRAY_SIZE(s_csi2rxBaseAddrs); i++)
    {
        if ((uint32_t)base == s_csi2rxBaseAddrs[i])
        {
            return i;
        }
    }

    assert(false);

    return 0;
}

/*!
 * brief Enables and configures the CSI2RX peripheral module.
 *
 * param base CSI2RX peripheral address.
 * param config CSI2RX module configuration structure.
 */
void CSI2RX_Init(MIPI_CSI2RX_Type *base, const csi2rx_config_t *config)
{
    assert(config);

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* un-gate clock */
    CLOCK_EnableClock(s_csi2rxClocks[CSI2RX_GetInstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

#if (defined(FSL_FEATURE_CSI2RX_CSR_OFFSET) && FSL_FEATURE_CSI2RX_CSR_OFFSET)
    MIPI_CSI_CSR_Type *csr = CSI2RX_GET_CSR(base);
#endif

    base->CSI2RX_CFG_NUM_LANES = config->laneNum - 1;
    base->CSI2RX_CFG_DISABLE_DATA_LANES =
        MIPI_CSI2RX_CSI2RX_CFG_NUM_LANES_csi2rx_cfg_num_lanes_MASK & ~((1U << (uint32_t)config->laneNum) - 1);

    /* Don't disable data types. */
    base->CSI2RX_CFG_DISABLE_PAYLOAD_0 = 0;
    base->CSI2RX_CFG_DISABLE_PAYLOAD_1 = 0;

    /* Disable all interrupts. */
    base->CSI2RX_IRQ_MASK = MIPI_CSI2RX_CSI2RX_IRQ_MASK_csi2rx_irq_mask_MASK;

#if (defined(FSL_FEATURE_CSI2RX_CSR_OFFSET) && FSL_FEATURE_CSI2RX_CSR_OFFSET)
    /* Perform controller reset when clock stoped. */
    csr->CONTROLLER_CLOCK_RESET_CONTROL = MIPI_CSI_CSR_CONTROLLER_CLOCK_RESET_CONTROL_CTL_CLK_OFF_MASK;
    csr->CONTROLLER_CLOCK_RESET_CONTROL = MIPI_CSI_CSR_CONTROLLER_CLOCK_RESET_CONTROL_SW_RESET_MASK |
                                          MIPI_CSI_CSR_CONTROLLER_CLOCK_RESET_CONTROL_CTL_CLK_OFF_MASK;

    /* Pixel link control */
    csr->PLM_CTRL = 0;

    /* Configure the PHY. */
    csr->PHY_CTRL = MIPI_CSI_CSR_PHY_CTRL_RX_ENABLE_MASK |  /* Enable RX. */
                    MIPI_CSI_CSR_PHY_CTRL_AUTO_PD_EN_MASK | /* Auto power down unused lanes. */
                    MIPI_CSI_CSR_PHY_CTRL_PD_MASK | MIPI_CSI_CSR_PHY_CTRL_DDRCLK_EN_MASK | /* Enable the DDR clock. */
                    MIPI_CSI_CSR_PHY_CTRL_CONTI_CLK_MODE_MASK |                            /* Continue clock. */
                    MIPI_CSI_CSR_PHY_CTRL_RTERM_SEL_MASK | /*  LPRX voltage level enable HS termination */
                    MIPI_CSI_CSR_PHY_CTRL_PRG_RXHS_SETTLE(config->tHsSettle_EscClk + 1U); /* T(HS-SETTLE) */

    /* Don't support interlace currently. */
    csr->VC_INTERLACED = 0U;
    /* Don't mask any data type */
    csr->DATA_TYPE_DIS = 0U;
    /* VC fence. */
    csr->STREAM_FENCING_CTRL = 0U;

    /* Wait for PL clock active. */
    while (0 != (csr->PLM_CTRL & MIPI_CSI_CSR_PLM_CTRL_PL_CLOCK_RUNNING_MASK))
    {
    }

    /* Enable pixel link master. */
    csr->PLM_CTRL |= (MIPI_CSI_CSR_PLM_CTRL_ENABLE_MASK | MIPI_CSI_CSR_PLM_CTRL_VALID_OVERRIDE_MASK);

    /* Power up PHY. */
    csr->PHY_CTRL &= ~MIPI_CSI_CSR_PHY_CTRL_PD_MASK;

    /* Start clock. */
    csr->CONTROLLER_CLOCK_RESET_CONTROL = MIPI_CSI_CSR_CONTROLLER_CLOCK_RESET_CONTROL_SW_RESET_MASK;
#endif
}

/*!
 * brief Disables the CSI2RX peripheral module.
 *
 * param base CSI2RX peripheral address.
 */
void CSI2RX_Deinit(MIPI_CSI2RX_Type *base)
{
#if (defined(FSL_FEATURE_CSI2RX_CSR_OFFSET) && FSL_FEATURE_CSI2RX_CSR_OFFSET)
    MIPI_CSI_CSR_Type *csr = CSI2RX_GET_CSR(base);

    /* Disable the PHY. */
    csr->PHY_CTRL = 0;

    /* Disable the pixel link master. */
    csr->PLM_CTRL = 0;

    /* Stop the clock and assert reset. */
    csr->CONTROLLER_CLOCK_RESET_CONTROL = MIPI_CSI_CSR_CONTROLLER_CLOCK_RESET_CONTROL_CTL_CLK_OFF_MASK;
#endif

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* gate clock */
    CLOCK_DisableClock(s_csi2rxClocks[CSI2RX_GetInstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
}
