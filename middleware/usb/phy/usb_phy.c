/*
 * The Clear BSD License
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
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

#include "usb.h"
#include "fsl_device_registers.h"

#include "usb_phy.h"

void *USB_EhciPhyGetBase(uint8_t controllerId)
{
    void *usbPhyBase = NULL;
#if ((defined FSL_FEATURE_SOC_USBPHY_COUNT) && (FSL_FEATURE_SOC_USBPHY_COUNT > 0U))
    uint32_t instance;
    uint32_t newinstance = 0;
    uint32_t usbphy_base_temp[] = USBPHY_BASE_ADDRS;
    uint32_t usbphy_base[] = USBPHY_BASE_ADDRS;

    if (controllerId < kUSB_ControllerEhci0)
    {
        return NULL;
    }

    controllerId = controllerId - kUSB_ControllerEhci0;

    for (instance = 0; instance < (sizeof(usbphy_base_temp) / sizeof(usbphy_base_temp[0])); instance++)
    {
        if (usbphy_base_temp[instance])
        {
            usbphy_base[newinstance++] = usbphy_base_temp[instance];
        }
    }
    if (controllerId > newinstance)
    {
        return NULL;
    }

    usbPhyBase = (void *)usbphy_base[controllerId];
#endif
    return usbPhyBase;
}

/*!
 * @brief ehci phy initialization.
 *
 * This function initialize ehci phy IP.
 *
 * @param[in] controllerId   ehci controller id, please reference to #usb_controller_index_t.
 * @param[in] freq            the external input clock.
 *                            for example: if the external input clock is 16M, the parameter freq should be 16000000.
 *
 * @retval kStatus_USB_Success      cancel successfully.
 * @retval kStatus_USB_Error        the freq value is incorrect.
 */
uint32_t USB_EhciPhyInit(uint8_t controllerId, uint32_t freq, usb_phy_config_struct_t *phyConfig)
{
#if ((defined FSL_FEATURE_SOC_USBPHY_COUNT) && (FSL_FEATURE_SOC_USBPHY_COUNT > 0U))
    USBPHY_Type *usbPhyBase;

    usbPhyBase = (USBPHY_Type *)USB_EhciPhyGetBase(controllerId);
    if (NULL == usbPhyBase)
    {
        return kStatus_USB_Error;
    }

#if ((defined FSL_FEATURE_SOC_ANATOP_COUNT) && (FSL_FEATURE_SOC_ANATOP_COUNT > 0U))    
    ANATOP->HW_ANADIG_REG_3P0.RW =
        (ANATOP->HW_ANADIG_REG_3P0.RW &
         (~(ANATOP_HW_ANADIG_REG_3P0_OUTPUT_TRG(0x1F) | ANATOP_HW_ANADIG_REG_3P0_ENABLE_ILIMIT_MASK))) |
        ANATOP_HW_ANADIG_REG_3P0_OUTPUT_TRG(0x17) | ANATOP_HW_ANADIG_REG_3P0_ENABLE_LINREG_MASK;
    ANATOP->HW_ANADIG_USB2_CHRG_DETECT.SET =
        ANATOP_HW_ANADIG_USB2_CHRG_DETECT_CHK_CHRG_B_MASK | ANATOP_HW_ANADIG_USB2_CHRG_DETECT_EN_B_MASK;
#endif
    
#if (defined USB_ANALOG)
    USB_ANALOG->INSTANCE[controllerId - kUSB_ControllerEhci0].CHRG_DETECT_SET = USB_ANALOG_CHRG_DETECT_CHK_CHRG_B(1) | USB_ANALOG_CHRG_DETECT_EN_B(1); 
#endif
    
#if ((!(defined FSL_FEATURE_SOC_CCM_ANALOG_COUNT)) && (!(defined FSL_FEATURE_SOC_ANATOP_COUNT)))

    usbPhyBase->TRIM_OVERRIDE_EN = 0x001fU; /* override IFR value */
#endif
    usbPhyBase->CTRL |= USBPHY_CTRL_SET_ENUTMILEVEL2_MASK; /* support LS device. */
    usbPhyBase->CTRL |= USBPHY_CTRL_SET_ENUTMILEVEL3_MASK; /* support external FS Hub with LS device connected. */
    /* PWD register provides overall control of the PHY power state */
    usbPhyBase->PWD = 0U;

    /* Decode to trim the nominal 17.78mA current source for the High Speed TX drivers on USB_DP and USB_DM. */
    usbPhyBase->TX =
        ((usbPhyBase->TX & (~(USBPHY_TX_D_CAL_MASK | USBPHY_TX_TXCAL45DM_MASK | USBPHY_TX_TXCAL45DP_MASK))) |
         (USBPHY_TX_D_CAL(phyConfig->D_CAL) | USBPHY_TX_TXCAL45DP(phyConfig->TXCAL45DP) |
          USBPHY_TX_TXCAL45DM(phyConfig->TXCAL45DM)));
#endif

    return kStatus_USB_Success;
}

/*!
 * @brief ehci phy initialization for suspend and resume.
 *
 * This function initialize ehci phy IP for suspend and resume.
 *
 * @param[in] controllerId   ehci controller id, please reference to #usb_controller_index_t.
 * @param[in] freq            the external input clock.
 *                            for example: if the external input clock is 16M, the parameter freq should be 16000000.
 *
 * @retval kStatus_USB_Success      cancel successfully.
 * @retval kStatus_USB_Error        the freq value is incorrect.
 */
uint32_t USB_EhciLowPowerPhyInit(uint8_t controllerId, uint32_t freq, usb_phy_config_struct_t *phyConfig)
{
#if ((defined FSL_FEATURE_SOC_USBPHY_COUNT) && (FSL_FEATURE_SOC_USBPHY_COUNT > 0U))
    USBPHY_Type *usbPhyBase;

    usbPhyBase = (USBPHY_Type *)USB_EhciPhyGetBase(controllerId);
    if (NULL == usbPhyBase)
    {
        return kStatus_USB_Error;
    }

#if ((!(defined FSL_FEATURE_SOC_CCM_ANALOG_COUNT)) && (!(defined FSL_FEATURE_SOC_ANATOP_COUNT)))
    usbPhyBase->TRIM_OVERRIDE_EN = 0x001fU; /* override IFR value */
#endif

#if ((defined USBPHY_CTRL_AUTORESUME_EN_MASK) && (USBPHY_CTRL_AUTORESUME_EN_MASK > 0U))
    usbPhyBase->CTRL |= USBPHY_CTRL_AUTORESUME_EN_MASK;
#else
    usbPhyBase->CTRL |= USBPHY_CTRL_ENAUTO_PWRON_PLL_MASK;
#endif
    usbPhyBase->CTRL |= USBPHY_CTRL_ENAUTOCLR_CLKGATE_MASK | USBPHY_CTRL_ENAUTOCLR_PHY_PWD_MASK;
    usbPhyBase->CTRL |= USBPHY_CTRL_SET_ENUTMILEVEL2_MASK; /* support LS device. */
    usbPhyBase->CTRL |= USBPHY_CTRL_SET_ENUTMILEVEL3_MASK; /* support external FS Hub with LS device connected. */
    /* PWD register provides overall control of the PHY power state */
    usbPhyBase->PWD = 0U;
#if ((!(defined FSL_FEATURE_SOC_CCM_ANALOG_COUNT)) && (!(defined FSL_FEATURE_SOC_ANATOP_COUNT)))
    /* now the 480MHz USB clock is up, then configure fractional divider after PLL with PFD
     * pfd clock = 480MHz*18/N, where N=18~35
     * Please note that USB1PFDCLK has to be less than 180MHz for RUN or HSRUN mode
     */
    usbPhyBase->ANACTRL |= USBPHY_ANACTRL_PFD_FRAC(24);   /* N=24 */
    usbPhyBase->ANACTRL |= USBPHY_ANACTRL_PFD_CLK_SEL(1); /* div by 4 */

    usbPhyBase->ANACTRL &= ~USBPHY_ANACTRL_DEV_PULLDOWN_MASK;
    usbPhyBase->ANACTRL &= ~USBPHY_ANACTRL_PFD_CLKGATE_MASK;
    while (!(usbPhyBase->ANACTRL & USBPHY_ANACTRL_PFD_STABLE_MASK))
    {
    }
#endif
    /* Decode to trim the nominal 17.78mA current source for the High Speed TX drivers on USB_DP and USB_DM. */
    usbPhyBase->TX =
        ((usbPhyBase->TX & (~(USBPHY_TX_D_CAL_MASK | USBPHY_TX_TXCAL45DM_MASK | USBPHY_TX_TXCAL45DP_MASK))) |
         (USBPHY_TX_D_CAL(phyConfig->D_CAL) | USBPHY_TX_TXCAL45DP(phyConfig->TXCAL45DP) |
          USBPHY_TX_TXCAL45DM(phyConfig->TXCAL45DM)));
#endif

    return kStatus_USB_Success;
}

/*!
 * @brief ehci phy de-initialization.
 *
 * This function de-initialize ehci phy IP.
 *
 * @param[in] controllerId   ehci controller id, please reference to #usb_controller_index_t.
 */
void USB_EhciPhyDeinit(uint8_t controllerId)
{
#if ((defined FSL_FEATURE_SOC_USBPHY_COUNT) && (FSL_FEATURE_SOC_USBPHY_COUNT > 0U))
    USBPHY_Type *usbPhyBase;

    usbPhyBase = (USBPHY_Type *)USB_EhciPhyGetBase(controllerId);
    if (NULL == usbPhyBase)
    {
        return;
    }
#if ((!(defined FSL_FEATURE_SOC_CCM_ANALOG_COUNT)) && (!(defined FSL_FEATURE_SOC_ANATOP_COUNT)))
    usbPhyBase->PLL_SIC &= ~USBPHY_PLL_SIC_PLL_POWER_MASK;       /* power down PLL */
    usbPhyBase->PLL_SIC &= ~USBPHY_PLL_SIC_PLL_EN_USB_CLKS_MASK; /* disable USB clock output from USB PHY PLL */
#endif
    usbPhyBase->CTRL |= USBPHY_CTRL_CLKGATE_MASK; /* set to 1U to gate clocks */
#endif
}

/*!
 * @brief ehci phy disconnect detection enable or disable.
 *
 * This function enable/disable host ehci disconnect detection.
 *
 * @param[in] controllerId   ehci controller id, please reference to #usb_controller_index_t.
 * @param[in] enable
 *            1U - enable;
 *            0U - disable;
 */
void USB_EhcihostPhyDisconnectDetectCmd(uint8_t controllerId, uint8_t enable)
{
#if ((defined FSL_FEATURE_SOC_USBPHY_COUNT) && (FSL_FEATURE_SOC_USBPHY_COUNT > 0U))
    USBPHY_Type *usbPhyBase;

    usbPhyBase = (USBPHY_Type *)USB_EhciPhyGetBase(controllerId);
    if (NULL == usbPhyBase)
    {
        return;
    }

    if (enable)
    {
        usbPhyBase->CTRL |= USBPHY_CTRL_ENHOSTDISCONDETECT_MASK;
    }
    else
    {
        usbPhyBase->CTRL &= (~USBPHY_CTRL_ENHOSTDISCONDETECT_MASK);
    }
#endif
}
