/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_device_registers.h"
#include "fsl_common.h"
#include "nxp2p4_xcvr.h"
#include "nxp_xcvr_trim.h"
#include <math.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define gPllDenom_c         0x02000000UL /* Denominator is a constant value */
#define CAL_AT_FREQ_HZ      (2385000000UL) /* Perform DCOC DAC calibration out-of-band to avoid interference */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
link_layer_t map_mode_to_ll(radio_mode_t mode);
#ifndef GCOV_DO_COVERAGE /* routine is local except when testing code coverage */
uint16_t XCVR_IdentifyRadioVer(uint32_t radio_ver);
#endif /* !defined(GCOV_DO_COVERAGE) */
/*******************************************************************************
 * Variables
 ******************************************************************************/
static panic_fptr s_PanicFunctionPtr = NULLPTR;
const uint16_t tx_rx_on_delay = (uint16_t)TX_RX_ON_DELinit;
uint16_t tx_rx_synth_delay = (uint16_t)TX_RX_SYNTH_init_B0; /* This will be overwritten in register setup routine for A0/B0 support */
static xcvr_currConfig_t current_xcvr_config;
static uint8_t default_bb_ldo_fh_tx;
/*******************************************************************************
 * Code
 *******************************************************************************/
xcvrStatus_t XCVR_Init(const xcvr_config_t ** xcvr_config,
                        const xcvr_coding_config_t ** rbme_config)
{
   
    xcvrStatus_t status = gXcvrSuccess_c;

    /* Startup the radio (power mode) and turn on required clocks */
    XCVR_RadioStartup();

    status = XCVR_Configure(xcvr_config,rbme_config,XCVR_FIRST_INIT); /* Setting first_init  */

    if (status == gXcvrSuccess_c) /* don't update the configs in failure case (avoid null pointer deref) */
    {
        current_xcvr_config.radio_mode = (*xcvr_config)->mode_data_rate_cfg->radio_mode;
        current_xcvr_config.data_rate = (*xcvr_config)->mode_data_rate_cfg->data_rate;
        current_xcvr_config.alt_data_rate = (*xcvr_config)->mode_data_rate_cfg->alt_data_rate;
        current_xcvr_config.coding_mode = (*rbme_config)->coding_mode;
    }

    return status;
}

xcvrStatus_t XCVR_InitNoDacTrim(const xcvr_config_t ** xcvr_config,
                                    const xcvr_coding_config_t ** rbme_config)
{
    
    xcvrStatus_t status = gXcvrSuccess_c;

    /* Startup the radio (power mode) and turn on required clocks */
    XCVR_RadioStartup();

    status =  XCVR_RadioRegSetup(xcvr_config);

    /* Sets up RBME only */
    if (status == gXcvrSuccess_c)
    {
        status = XCVR_RBME_Configure(rbme_config);    

        if (status == gXcvrSuccess_c) /* don't update the configs in failure case (avoid null pointer deref) */
        {
            current_xcvr_config.radio_mode = (*xcvr_config)->mode_data_rate_cfg->radio_mode;
            current_xcvr_config.data_rate = (*xcvr_config)->mode_data_rate_cfg->data_rate;
            current_xcvr_config.alt_data_rate = (*xcvr_config)->mode_data_rate_cfg->alt_data_rate;
            current_xcvr_config.coding_mode = (*rbme_config)->coding_mode;
        }
    }
    
    return status;
}

void XCVR_Deinit(void)
{
#if defined(RADIO_IS_GEN_4P0)
    /*  Assert Radio Reset */
    RFMC_radio_reset();
    
    /* Turn off all clocks to radio blocks */
    RADIO_CTRL->LL_CTRL &= ~RADIO_CTRL_LL_CTRL_ACTIVE_LL_MASK; 
    RFMC->RF2P4GHZ_CTRL &= ~(RFMC_RF2P4GHZ_CTRL_BLE_LP_EN_MASK | 
                            RFMC_RF2P4GHZ_CTRL_BLE_WKUP_MASK);
#else
    /*  Assert Radio Reset */
    RSIM_radio_reset();
    
    /* Turn off all clocks to radio blocks */
    SIM->SCGC5 &= ~(SIM_SCGC5_PHYDIG_MASK |
                    SIM_SCGC5_GEN_FSK_MASK |
                    SIM_SCGC5_BTLL_MASK);  
#endif
}

xcvrStatus_t XCVR_RadioRegSetup(const xcvr_config_t ** xcvr_config)
{
    xcvrStatus_t status = gXcvrSuccess_c;
    
    /* check config pointer */
    if (xcvr_config != NULLPTR)
    {    
        /* Configure Radio registers */
        status = XCVR_RadioGenRegSetup(*xcvr_config);
    }
    else
    {
        status = gXcvrInvalidParameters_c;
    }    
    
    return status;
}

xcvrStatus_t XCVR_RBME_Configure(const xcvr_coding_config_t ** rbme)
{
    xcvrStatus_t status = gXcvrSuccess_c;

    /* check config pointer */
    if (rbme != NULLPTR)
    {
        /* Configure RBME registers */
        status = XCVR_RadioGenRBMESetup(*rbme);
    }
    else
    {
        status = gXcvrInvalidParameters_c;
    }
    
    return status;
}

void XCVR_RadioStartup(void)
{
#if defined(RADIO_IS_GEN_4P0)
    /* Turn on clocks to radio */
    RFMC_rf_osc_startup();

    /* Turn on the clocks to the blocks */
    RADIO_CTRL->LL_CTRL = RADIO_CTRL_LL_CTRL_ACTIVE_LL_MASK; 
    RFMC->RF2P4GHZ_CTRL |= RFMC_RF2P4GHZ_CTRL_BLE_LP_EN_MASK | 
                            RFMC_RF2P4GHZ_CTRL_BLE_WKUP_MASK;
#else
    /* Turn on clocks to radio */
    RSIM_rf_osc_startup();
    
    /* Turn on the clocks to the blocks */
    SIM->SCGC5 |= (SIM_SCGC5_PHYDIG_MASK |
                SIM_SCGC5_GEN_FSK_MASK |
                SIM_SCGC5_BTLL_MASK);

#endif
}

xcvrStatus_t XCVR_Configure(const xcvr_config_t ** xcvr_config, 
                            const xcvr_coding_config_t ** rbme_config,
                            XCVR_INIT_MODE_CHG_T first_init)

{
#if (FPGA_TARGET==0)
    xcvr_DcocDacTrim_t dac_trims;
#endif
    xcvrStatus_t config_status = gXcvrSuccess_c;
    
    if ((first_init == XCVR_MODE_CHANGE) || (first_init == XCVR_FIRST_INIT)) /* Verify valid option passed in for first_init */
    {
        /* Setup all of the XCVR registers */
        config_status =  XCVR_RadioRegSetup(xcvr_config);

#if defined(RADIO_IS_GEN_4P0)
        /* Capture default setting for TX LDO trim (used later if PA bump is removed */
        default_bb_ldo_fh_tx = (uint8_t)((XCVR_ANALOG->LDO_1 & XCVR_ANALOG_LDO_1_LDO_ANT_TRIM_MASK)>>XCVR_ANALOG_LDO_1_LDO_ANT_TRIM_SHIFT);
#else
        /* Capture default setting for TX LDO trim (used later if PA bump is removed */
        default_bb_ldo_fh_tx = (uint8_t)((XCVR_ANALOG->BB_LDO_2 & XCVR_ANALOG_BB_LDO_2_BB_LDO_HF_TRIM_TX_MASK)>>XCVR_ANALOG_BB_LDO_2_BB_LDO_HF_TRIM_TX_SHIFT);
#endif
        
        /* Sets up RBME only */
        if (config_status == gXcvrSuccess_c)
        {
            config_status = XCVR_RBME_Configure(rbme_config);    
        }
        
        /* Only run the DCOC DAC trim on first initialization */
        if ((first_init == XCVR_FIRST_INIT) && (config_status == gXcvrSuccess_c))
        {

#if ((FPGA_TARGET==0) && (!defined(RADIO_IS_GEN_4P0)))
            config_status = XCVR_CalculateDcocDacTrims(&dac_trims);
#endif
        }
    }
    else
    {
        config_status = gXcvrInvalidParameters_c;
    }

    return config_status;
}

void XCVR_Reset(void)
{
#if defined(RADIO_IS_GEN_4P0)
    /* Perform a hardware reset on the radio */
    RFMC_radio_reset();
  
#else
    /* Perform a hardware reset on the radio */
    RSIM_radio_reset();
    
#endif
}

xcvrStatus_t XCVR_ChangeMode (const xcvr_config_t ** xcvr_config, /* Change from one radio mode to another */
                                    const xcvr_coding_config_t ** rbme_config)
{
    xcvr_DcocDacTrim_t dac_trims;
    xcvrStatus_t status = gXcvrSuccess_c;

    /* Store off DAC trims that would be over-written by defaults in radio register setup */
    status = XCVR_GetDcocDacTrims(&dac_trims);
    (void)status;
    
#if defined(gXcvrSaveRestoreCoexConfig_d)
    XCVR_CoexistenceSaveRestoreTimings(0x1U); /* save coex related timings to be restored later */
#endif

    /* Performs an initialization of radio registers (all of them) */
    status = XCVR_RadioRegSetup(xcvr_config); /* Set to mode change register programming */

    /* Sets up RBME only */
    if (status == gXcvrSuccess_c)
    {
        status = XCVR_RBME_Configure(rbme_config);    
    }
    
#if defined(gXcvrSaveRestoreCoexConfig_d)
    XCVR_CoexistenceSaveRestoreTimings(0x0U); /* restore  previously saved coex related timings */
#endif
    
    if (status == gXcvrSuccess_c)
    {
        current_xcvr_config.radio_mode = (*xcvr_config)->mode_data_rate_cfg->radio_mode;
        current_xcvr_config.data_rate = (*xcvr_config)->mode_data_rate_cfg->data_rate;
        current_xcvr_config.alt_data_rate = (*xcvr_config)->mode_data_rate_cfg->alt_data_rate;
        current_xcvr_config.coding_mode = (*rbme_config)->coding_mode;

        /* Rewrite  DAC trims that have been over-written (restore from saved DAC trim values) */
        status = XCVR_SetDcocDacTrims(&dac_trims);
    }
    
    return status;
}

void XCVR_GetCRCWConfig(rbme_crc_config_t * crc_config, rbme_whiten_config_t * whiten_config)
{
    if (crc_config != NULLPTR) /* if the pointer is NULLPTR then don't fetch the CRC configuration */
    {
        crc_config->crcw_cfg = RBME->CRCW_CFG;
        crc_config->crcw_cfg3 = RBME->CRCW_CFG3;
        crc_config->crc_init = RBME->CRC_INIT;
        crc_config->crc_poly = RBME->CRC_POLY;
    }

    if (whiten_config != NULLPTR) /* if the pointer is NULLPTR then don't fetch the Whitener configuration */
    {
        whiten_config->whiten_cfg = RBME->WHITEN_CFG;
        whiten_config->whiten_poly = RBME->WHITEN_POLY;
        whiten_config->whiten_sz_thr = RBME->WHITEN_SZ_THR;
        whiten_config->wht_cfg = RBME->WHT_CFG;
    }
    /* Calling this routine with both pointers == NULLPTR results in nothing happening...*/
}

void XCVR_SetCRCWConfig(rbme_crc_config_t * crc_config, rbme_whiten_config_t * whiten_config)
{
    if (crc_config != NULLPTR) /* if the pointer is NULLPTR then don't set the CRC configuration */
    {
        RBME->CRCW_CFG = crc_config->crcw_cfg;
        RBME->CRCW_CFG3 = crc_config->crcw_cfg3;
        RBME->CRC_INIT = crc_config->crc_init;
        RBME->CRC_POLY = crc_config->crc_poly;
    }

    if (whiten_config != NULLPTR) /* if the pointer is NULLPTR then don't set the Whitener configuration */
    {
        RBME->WHITEN_CFG = whiten_config->whiten_cfg;
        RBME->WHITEN_POLY = whiten_config->whiten_poly;
        RBME->WHITEN_SZ_THR = whiten_config->whiten_sz_thr;
        RBME->WHT_CFG = whiten_config->wht_cfg;
    }
    /* Calling this routine with both pointers == NULLPTR results in nothing happening...*/
}


void XCVR_EnaNBRSSIMeas(bool IIRnbEnable )
{
#if defined(RADIO_IS_GEN_4P0)
  
#else
    /* Change RSSI measurement to narrowband method instead of wideband */
    if (IIRnbEnable)
    {
        XCVR_RX_DIG->RSSI_CTRL_0 |= XCVR_RX_DIG_RSSI_CTRL_0_RSSI_IIR_CW_WEIGHT_MASK; 
    }
    else
    {
        XCVR_RX_DIG->RSSI_CTRL_0 &= ~XCVR_RX_DIG_RSSI_CTRL_0_RSSI_IIR_CW_WEIGHT_MASK; 
    }
#endif
}

void XCVR_RegisterPanicCb ( panic_fptr fptr ) /* Allow upper layers to provide PANIC callback */
{
    /* Set the pointer to a panic function for the XCVR to use */
    s_PanicFunctionPtr = fptr;
}

void XcvrPanic(XCVR_PANIC_ID_T panic_id, uint32_t panic_address)
{
    /* Wrap the panic function pointer into the upper layers in a module for XCVR to use locally */
    /* enters a while(1) loop if the function pointer is NULLpointer */
    if ( s_PanicFunctionPtr != NULLPTR)
    {
        s_PanicFunctionPtr(panic_id, panic_address, 0, 0);
    }
    else
    {
        uint8_t dummy;
        while(true)
        {
            dummy = dummy;
        }
    }
}

link_layer_t map_mode_to_ll(radio_mode_t mode)
{
    link_layer_t llret = UNASSIGNED_LL;
    /* Helper function to map radio mode to LL usage */
    switch (mode)
    {
        /* Supported BLE modes */
        case BLE_MODE:
        case BLE_LR:    
            llret = BLE_LL; /* Only BLE andBLE_LR modes use BLE LL */
            break;
        /* Supported GENFSK modes */
        case GFSK_BT_0p5_h_0p5:
        case GFSK_BT_0p5_h_0p32:
        case GFSK_BT_0p5_h_0p7:
        case GFSK_BT_0p5_h_1p0:
#if defined(RADIO_IS_GEN_4P0)
        case GFSK_BT_0p3_h_0p5:
        case GFSK_BT_0p7_h_0p5:
#endif /* RADIO_IS_GEN_4P0 */
        case MSK: 
            llret = GENFSK_LL;
            break;
        default:
            /* Unsupported GENFSK modes will get caught in this case */
            llret = UNASSIGNED_LL;
            break;
    }
    return llret;
}


xcvrStatus_t XCVR_SetIRQMapping(radio_mode_t irq0_mapping, radio_mode_t irq1_mapping)
{
#if defined(RADIO_IS_GEN_4P0)
    return gXcvrUnsupportedOperation_c;

#else
    xcvrStatus_t status = gXcvrSuccess_c;
    /* Setup IRQ mapping to LL interrupt outputs in XCVR_CTRL */
    link_layer_t int0 = map_mode_to_ll(irq0_mapping);
    link_layer_t int1 = map_mode_to_ll(irq1_mapping);

    /* Make sure the two LL's requested aren't the same */
    if (int0 == int1) /* Cannot have two irqs to the same LL */
    {
        status = gXcvrInvalidConfiguration_c;
    }
    
    /* Make sure neither irq is mapping to UNASSIGNED (error) */
    if ((int0 == UNASSIGNED_LL) || (int1 == UNASSIGNED_LL))
    {
        status = gXcvrInvalidConfiguration_c;
    }

    if (status == gXcvrSuccess_c)
    {
        uint32_t temp;
        temp = XCVR_MISC->XCVR_CTRL;
        temp &= ~(XCVR_MISC_XCVR_CTRL_RADIO0_IRQ_SEL_MASK | XCVR_MISC_XCVR_CTRL_RADIO1_IRQ_SEL_MASK);
        temp |= (XCVR_MISC_XCVR_CTRL_RADIO0_IRQ_SEL(int0) | XCVR_MISC_XCVR_CTRL_RADIO1_IRQ_SEL(int1));
        XCVR_MISC->XCVR_CTRL = temp;
    }

    return status;
#endif /* RADIO_IS_GEN_4P0 */
}

link_layer_t XCVR_GetIRQMapping(uint8_t int_num)
{
#if defined(RADIO_IS_GEN_4P0)
    return BLE_LL;

#else
    link_layer_t ll_int;
    uint8_t temp = 0;

    assert((int_num == 0U) || (int_num == 1U)); /* Make sure input is in allowable range */
    
    /* Get current state of IRQ mapping for either  radio INT0 or INT1 */
    if (int_num == 0U)
    {
        temp = (uint8_t)((XCVR_MISC->XCVR_CTRL & XCVR_MISC_XCVR_CTRL_RADIO0_IRQ_SEL_MASK)>>XCVR_MISC_XCVR_CTRL_RADIO0_IRQ_SEL_SHIFT);
    }
    else /* Anything other than 0 is treated as 1, relies on the assert catching anything else */
    {
        temp = (uint8_t)((XCVR_MISC->XCVR_CTRL & XCVR_MISC_XCVR_CTRL_RADIO1_IRQ_SEL_MASK)>>XCVR_MISC_XCVR_CTRL_RADIO1_IRQ_SEL_SHIFT);
    }
    switch (temp) /* Map the field back to the enumeration type */
    {
        case 0:
            ll_int = BLE_LL;
            break;
        case 3:
            ll_int = GENFSK_LL;
            break;
        default:
            ll_int = UNASSIGNED_LL; /* Catches all other values. */
            break;
    }
    return ll_int;
#endif /* RADIO_IS_GEN_4P0 */
}

xcvrStatus_t XCVR_GetCurrentConfig(xcvr_currConfig_t * curr_config)
{
    xcvrStatus_t status = gXcvrSuccess_c;
    if (curr_config == NULLPTR) /* Check for NULLPTR */
    {
        status = gXcvrInvalidParameters_c;
    }
    else
    {
        /* Get current state of LL, radio mode and data rate */
        if ((current_xcvr_config.radio_mode == GFSK_BT_0p5_h_0p5) && 
            (XCVR_GetActiveLL() == XCVR_ACTIVE_LL_BTLE))
        {
            curr_config->radio_mode = BLE_MODE;
        }
        else
        {
            curr_config->radio_mode = current_xcvr_config.radio_mode;
        }
        curr_config->data_rate = current_xcvr_config.data_rate;
        curr_config->alt_data_rate = current_xcvr_config.alt_data_rate;
        curr_config->coding_mode = current_xcvr_config.coding_mode;
    }
    return status;
}

xcvrStatus_t XCVR_SetActiveLL(XCVR_ACTIVE_LL_SEL_T active_ll)
{
    xcvrStatus_t status = gXcvrSuccess_c;
    if ((active_ll == XCVR_ACTIVE_LL_RESERVED) || (active_ll >= XCVR_ACTIVE_LL_MAX)) /* Disallow the reserved field and invalid higher values */
    {
        status = gXcvrInvalidParameters_c;
    }
    else
    {
        RADIO_CTRL->LL_CTRL = RADIO_CTRL_LL_CTRL_ACTIVE_LL(active_ll);
        /* When changing to a new link layer the PLL band select must also be changed */
        uint32_t temp = XCVR_PLL_DIG->CHAN_MAP;
        temp &= ~(XCVR_PLL_DIG_CHAN_MAP_BAND_SELECT_MASK);
        if (active_ll == XCVR_ACTIVE_LL_BTLE)
        {
            temp |= XCVR_PLL_DIG_CHAN_MAP_BAND_SELECT(0); /* Select BTLE channel map */
        }
        else
        {
            temp |= XCVR_PLL_DIG_CHAN_MAP_BAND_SELECT(7); /* Select GENFSK 0-127 channel map */
        }
        XCVR_PLL_DIG->CHAN_MAP = temp;
    }
    return status;
}

XCVR_ACTIVE_LL_SEL_T  XCVR_GetActiveLL(void)
{
    uint32_t temp_ll = ((RADIO_CTRL->LL_CTRL&RADIO_CTRL_LL_CTRL_ACTIVE_LL_MASK)>>RADIO_CTRL_LL_CTRL_ACTIVE_LL_SHIFT); /*  Fetch directly from the register */
    return  (XCVR_ACTIVE_LL_SEL_T)temp_ll;
}

xcvrStatus_t XCVR_SetPLLBand(XCVR_BAND_SEL_T band)
{
    xcvrStatus_t status = gXcvrSuccess_c;
    if ((band == XCVR_BAND_SEL_RESERVED1) || 
            (band == XCVR_BAND_SEL_RESERVED2) || 
            (band >= XCVR_BAND_SEL_MAX)) /* Disallow the reserved fields and invalid higher values */
    {
        status = gXcvrInvalidParameters_c;
    }
    else
    {
        /* Manually set the PLL band selection */
        uint32_t temp = XCVR_PLL_DIG->CHAN_MAP;
        temp &= ~(XCVR_PLL_DIG_CHAN_MAP_BAND_SELECT_MASK);
        temp |= XCVR_PLL_DIG_CHAN_MAP_BAND_SELECT(band); /* Select BTLE channel map */
        XCVR_PLL_DIG->CHAN_MAP = temp;
    }
    return status;
}

XCVR_BAND_SEL_T  XCVR_GetPLLBand(void)
{
    uint32_t temp_band = ((XCVR_PLL_DIG->CHAN_MAP&XCVR_PLL_DIG_CHAN_MAP_BAND_SELECT_MASK)>>XCVR_PLL_DIG_CHAN_MAP_BAND_SELECT_SHIFT); /*  Fetch directly from the register */
    return  (XCVR_BAND_SEL_T)temp_band;
}

xcvrStatus_t XCVR_SetMSKPreambleSeed(uint8_t last_preamble_bit)
{
    xcvrStatus_t status = gXcvrSuccess_c;
    uint32_t temp;
    /* Valid input values are only 0 & 1 */
    if ((last_preamble_bit & 0xFEU) != 0U)
    {
        status = gXcvrInvalidParameters_c; 
    }
    else
    {
        /* Set the seed bit according to the last bit of the preamble */
#if defined(RADIO_IS_GEN_3P5)
        temp = XCVR_2P4GHZ_PHY->FSK_CFG0;
        temp &= ~(XCVR_2P4GHZ_PHY_FSK_CFG0_MSK2FSK_SEED_MASK);
        temp |= XCVR_2P4GHZ_PHY_FSK_CFG0_MSK2FSK_SEED(last_preamble_bit);
        XCVR_2P4GHZ_PHY->FSK_CFG0 = temp;
#elif defined(RADIO_IS_GEN_4P0)
        temp = XCVR_2P4GHZ_PHY->FSK_CFG0;
        temp &= ~(GEN4PHY_FSK_CFG0_MSK2FSK_SEED_MASK);
        temp |= GEN4PHY_FSK_CFG0_MSK2FSK_SEED(last_preamble_bit);
        XCVR_2P4GHZ_PHY->FSK_CFG0 = temp;
#else
   #error "Unsupported radio version selected in compile flags."
#endif /* RADIO_IS_GEN_3P5  */
    }
    return status;
}

/* ************************ */
/* Customer level trim functions */
/* ************************ */
xcvrStatus_t XCVR_SetXtalTrim(uint8_t xtalTrim)
{
#if defined(RADIO_IS_GEN_4P0)
    return gXcvrUnsupportedOperation_c;

#else
    xcvrStatus_t status = gXcvrSuccess_c;
    uint32_t temp;
    /* Apply a trim value to the crystal oscillator */
    if ((xtalTrim & 0x80U) != 0U) /* High bit must not be set */
    {
        status = gXcvrInvalidParameters_c; 
    }
    else
    {
        temp = RSIM->ANA_TRIM;
        temp &= ~(RSIM_ANA_TRIM_BB_XTAL_TRIM_MASK);
        RSIM->ANA_TRIM = temp | RSIM_ANA_TRIM_BB_XTAL_TRIM(xtalTrim);
    }
    return status;
#endif /* RADIO_IS_GEN_4P0 */
}

uint8_t  XCVR_GetXtalTrim(void)
{
#if defined(RADIO_IS_GEN_4P0)
    return 0;

#else
    uint8_t temp_xtal;
    /* Fetch a trim value from the crystal oscillator trim bitfield */
    temp_xtal = (uint8_t)((RSIM->ANA_TRIM & RSIM_ANA_TRIM_BB_XTAL_TRIM_MASK)>>RSIM_ANA_TRIM_BB_XTAL_TRIM_SHIFT);
    return temp_xtal;
    
#endif /* RADIO_IS_GEN_4P0 */
}

/* DCOC DAC Trim */
xcvrStatus_t XCVR_CalculateDcocDacTrims(xcvr_DcocDacTrim_t * trim_values)
{
#if defined(RADIO_IS_GEN_4P0)
    return gXcvrUnsupportedOperation_c;

#else

#if (!defined(SIMULATION) && (FPGA_TARGET==0)) /* Don't run this code on the FPGA or simulator */
    xcvrStatus_t status = gXcvrInvalidParameters_c;
    uint8_t count = 0;
    uint32_t dcoc_cal_check_en_stack = (XCVR_RX_DIG->DCOC_CTRL_0&XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CAL_CHECK_EN_MASK)>>XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CAL_CHECK_EN_SHIFT;
    if (trim_values != NULLPTR) /* Check for NULLPTR, status for return initialized to invalid parameters above */
    {
        /* store current DCOC_CAL_CHECK_EN value and disable it during initial dc calibration */
        XCVR_RX_DIG->DCOC_CTRL_0 &= ~XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CAL_CHECK_EN_MASK;

        /* Set XCVR to an out-of-band frequency to avoid possible intereference to the DCOC DAC trim process */
        status = XCVR_OverrideFrequency(CAL_AT_FREQ_HZ);

        XCVR_ForceRxWu();
        /* Wait for TSM to reach the end of warmup  */
        XCVR_WaitRxWu();
        
        /* Perform the DCOC DAC trim using DC estimate as input */
        do 
        {
            count++;
            if (rx_bba_dcoc_dac_trim_DCest() == 0U)
            {
                status = gXcvrTrimFailure_c;
            }
            else
            {
                status = gXcvrSuccess_c;
            }
        } while ((gXcvrSuccess_c != status) && (count <= XCVR_DAC_TRIM_RETRY_COUNT)); /* Allow for retry of the trim for robustness in interference */

        /* Calibrate the DCOC_DAC_INIT value */
        DCOC_DAC_INIT_Cal(false);
        XCVR_ForceRxWd();
        
        /* Release channel over-rides and return PLL to Link Layer Control */
        /* Remove any PLL settings that caused out-of-band receive operations (for safety) */
        if (status == gXcvrSuccess_c)
        {
            status = XCVR_OverrideFrequency(2402000000UL);
        }
        XCVR_ReleasePLLOverride();

        /* Store trim values to a structure for later re-use */
        if (status == gXcvrSuccess_c)
        {
            status = XCVR_GetDcocDacTrims(trim_values);
        }

        XCVR_RX_DIG->DCOC_CTRL_0 |= XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CAL_CHECK_EN(dcoc_cal_check_en_stack); /* restore previously programmed value */
    }
    
    return status;
#else
    /* Return success in all SIMULATION use cases */
    return gXcvrSuccess_c;
#endif /* (!defined(SIMULATION) && (FPGA_TARGET==0)) */    

#endif /* RADIO_IS_GEN_4P0 */
}

xcvrStatus_t XCVR_SetDcocDacTrims(const xcvr_DcocDacTrim_t * trim_values)
{
#if defined(RADIO_IS_GEN_4P0)
    return gXcvrUnsupportedOperation_c;
#else    
    xcvrStatus_t status = gXcvrInvalidParameters_c;
    /* Apply a stored structure of DCOC DAC trim values to the associated registers */
    if (trim_values != NULLPTR)
    {
        XCVR_RX_DIG->DCOC_BBA_STEP = trim_values->dcoc_bba_step_reg;       /* Restore the BBA Step value */
        XCVR_RX_DIG->DCOC_TZA_STEP_0 = trim_values->dcoc_tza_step_0_reg;   /* Restore the TZA Setup values (11 total) */
        XCVR_RX_DIG->DCOC_TZA_STEP_1 = trim_values->dcoc_tza_step_1_reg;
        XCVR_RX_DIG->DCOC_TZA_STEP_2 = trim_values->dcoc_tza_step_2_reg;
        XCVR_RX_DIG->DCOC_TZA_STEP_3 = trim_values->dcoc_tza_step_3_reg;
        XCVR_RX_DIG->DCOC_TZA_STEP_4 = trim_values->dcoc_tza_step_4_reg;
        XCVR_RX_DIG->DCOC_TZA_STEP_5 = trim_values->dcoc_tza_step_5_reg;
        XCVR_RX_DIG->DCOC_TZA_STEP_6 = trim_values->dcoc_tza_step_6_reg;
        XCVR_RX_DIG->DCOC_TZA_STEP_7 = trim_values->dcoc_tza_step_7_reg;
        XCVR_RX_DIG->DCOC_TZA_STEP_8 = trim_values->dcoc_tza_step_8_reg;
        XCVR_RX_DIG->DCOC_TZA_STEP_9 = trim_values->dcoc_tza_step_9_reg;
        XCVR_RX_DIG->DCOC_TZA_STEP_10 = trim_values->dcoc_tza_step_10_reg;
        XCVR_RX_DIG->DCOC_DAC_INIT = trim_values->dcoc_dac_init_reg;       /* Restore the DCOC DAC init value */
        status = gXcvrSuccess_c;
    }
    
    return status;
#endif /* RADIO_IS_GEN_4P0 */
}

xcvrStatus_t XCVR_GetDcocDacTrims(xcvr_DcocDacTrim_t * trim_values)
{
#if defined(RADIO_IS_GEN_4P0)
    return gXcvrUnsupportedOperation_c;
  
#else
    xcvrStatus_t status = gXcvrInvalidParameters_c;
    /* Read DCOC DAC trim values from the associated registers to a stored structure  */
    if (trim_values != NULLPTR)
    {
        trim_values->dcoc_bba_step_reg = XCVR_RX_DIG->DCOC_BBA_STEP;       /* Save the BBA Step value */
        trim_values->dcoc_tza_step_0_reg = XCVR_RX_DIG->DCOC_TZA_STEP_0;   /* Save the TZA Setup values (11 total) */
        trim_values->dcoc_tza_step_1_reg = XCVR_RX_DIG->DCOC_TZA_STEP_1;
        trim_values->dcoc_tza_step_2_reg = XCVR_RX_DIG->DCOC_TZA_STEP_2;
        trim_values->dcoc_tza_step_3_reg = XCVR_RX_DIG->DCOC_TZA_STEP_3;
        trim_values->dcoc_tza_step_4_reg = XCVR_RX_DIG->DCOC_TZA_STEP_4;
        trim_values->dcoc_tza_step_5_reg = XCVR_RX_DIG->DCOC_TZA_STEP_5;
        trim_values->dcoc_tza_step_6_reg = XCVR_RX_DIG->DCOC_TZA_STEP_6;
        trim_values->dcoc_tza_step_7_reg = XCVR_RX_DIG->DCOC_TZA_STEP_7;
        trim_values->dcoc_tza_step_8_reg = XCVR_RX_DIG->DCOC_TZA_STEP_8;
        trim_values->dcoc_tza_step_9_reg = XCVR_RX_DIG->DCOC_TZA_STEP_9;
        trim_values->dcoc_tza_step_10_reg= XCVR_RX_DIG->DCOC_TZA_STEP_10;
        trim_values->dcoc_dac_init_reg = XCVR_RX_DIG->DCOC_DAC_INIT;       /* Save the DCOC DAC init value */
        status = gXcvrSuccess_c;
    }
    
    return status;

#endif /* RADIO_IS_GEN_4P0 */
}

/* RSSI adjustment */
xcvrStatus_t XCVR_SetRssiAdjustment(int8_t adj)
{

#if defined(RADIO_IS_GEN_4P0)
    return gXcvrUnsupportedOperation_c;
  
#else

    /* Allow customer adjustment of RSSI values to match their board design */
    XCVR_RX_DIG->RSSI_CTRL_0 &= ~XCVR_RX_DIG_RSSI_CTRL_0_RSSI_ADJ_MASK;
    XCVR_RX_DIG->RSSI_CTRL_0 |= XCVR_RX_DIG_RSSI_CTRL_0_RSSI_ADJ(adj);
    return gXcvrSuccess_c;

#endif /* RADIO_IS_GEN_4P0 */
}

int8_t  XCVR_GetRssiAdjustment(void)
{
#if defined(RADIO_IS_GEN_4P0)
    return 0;

#else
    /* Allow customer to fetch the current value of RSSI values from the adjustment register */
    uint32_t temp = (XCVR_RX_DIG->RSSI_CTRL_0 & XCVR_RX_DIG_RSSI_CTRL_0_RSSI_ADJ_MASK) >> XCVR_RX_DIG_RSSI_CTRL_0_RSSI_ADJ_SHIFT;
    int8_t adj = (int8_t)temp;
    return adj;

#endif /* RADIO_IS_GEN_4P0 */
}


int8_t XCVR_GetInstantRssi(void)
{
#if defined(RADIO_IS_GEN_4P0)
    return gXcvrUnsupportedOperation_c;
  
#else
    uint8_t u8agc;
    uint8_t u8prev_agc = 26;
    uint8_t u8_agc_stable_count = 0x00;
    uint8_t u8_RSSI_stabilized_count = 3;
    uint8_t u8RSSI_n_window_wb;
    int8_t i8Rssi;
    uint32_t t1,t2,t3;
    t1 = XCVR_RX_DIG->RX_DIG_CTRL;
    t2 = XCVR_RX_DIG->RSSI_CTRL_0;
    t3 = XCVR_2P4GHZ_PHY->FSK_PD_CFG2;
    XCVR_RX_DIG->RX_DIG_CTRL = XCVR_RX_DIG_RX_DIG_CTRL_RX_ADC_NEGEDGE(0) |
                               XCVR_RX_DIG_RX_DIG_CTRL_RX_CH_FILT_BYPASS(1) |
                               XCVR_RX_DIG_RX_DIG_CTRL_RX_ADC_RAW_EN(0) |
                               XCVR_RX_DIG_RX_DIG_CTRL_RX_ADC_POL(0) |
                               XCVR_RX_DIG_RX_DIG_CTRL_RX_DEC_FILT_OSR(1) | /* 1=OSR8, 2=OSR16, 4=OSR32 */
                               XCVR_RX_DIG_RX_DIG_CTRL_RX_FSK_ZB_SEL(0) |
                               XCVR_RX_DIG_RX_DIG_CTRL_RX_RSSI_EN(1) |
                               XCVR_RX_DIG_RX_DIG_CTRL_RX_AGC_EN(1) |
                               XCVR_RX_DIG_RX_DIG_CTRL_RX_DCOC_EN(1) |
                               XCVR_RX_DIG_RX_DIG_CTRL_RX_DCOC_CAL_EN(1) |
                               XCVR_RX_DIG_RX_DIG_CTRL_RX_IQ_SWAP(0) |
                               XCVR_RX_DIG_RX_DIG_CTRL_RX_DC_RESID_EN(0) |
                               XCVR_RX_DIG_RX_DIG_CTRL_RX_SRC_EN(0) |
                               XCVR_RX_DIG_RX_DIG_CTRL_RX_SRC_RATE(0) | /* Source Rate 0 is default */
                               XCVR_RX_DIG_RX_DIG_CTRL_RX_DMA_DTEST_EN(0) |
                               XCVR_RX_DIG_RX_DIG_CTRL_RX_DEC_FILT_GAIN(22) | /* Dec filt gain for SRC rate ==  0 */
                               XCVR_RX_DIG_RX_DIG_CTRL_RX_DEC_FILT_HZD_CORR_DIS(1) ;
    /* Select Narrowband RSSI measurement */
    XCVR_RX_DIG->RSSI_CTRL_0 &= ~(XCVR_RX_DIG_RSSI_CTRL_0_RSSI_IIR_CW_WEIGHT_MASK |
                                XCVR_RX_DIG_RSSI_CTRL_0_RSSI_N_WINDOW_NB_MASK);
    XCVR_RX_DIG->RSSI_CTRL_0 |=  XCVR_RX_DIG_RSSI_CTRL_0_RSSI_IIR_CW_WEIGHT(0x5)|
                                XCVR_RX_DIG_RSSI_CTRL_0_RSSI_N_WINDOW_NB(0x3);

    /* Set preamble detect thresholds to max to prevent packet detection */
    XCVR_2P4GHZ_PHY->FSK_PD_CFG2 = XCVR_2P4GHZ_PHY_FSK_PD_CFG2_PD_THRESH_ACQ_1_3_1M(0xFFU) |
                                    XCVR_2P4GHZ_PHY_FSK_PD_CFG2_PD_THRESH_ACQ_1_3_2M(0xFFU);    

    /* read average window size */
    u8RSSI_n_window_wb = (uint8_t)(1U<< ((XCVR_RX_DIG->RSSI_CTRL_1 & XCVR_RX_DIG_RSSI_CTRL_1_RSSI_N_WINDOW_WB_MASK) >> XCVR_RX_DIG_RSSI_CTRL_1_RSSI_N_WINDOW_WB_SHIFT));
    u8_agc_stable_count = u8RSSI_n_window_wb + u8_RSSI_stabilized_count;

    uint32_t u32regvalue = XCVR_TSM->END_OF_SEQ;
    uint8_t end_of_wu = (uint8_t)((u32regvalue & XCVR_TSM_END_OF_SEQ_END_OF_RX_WU_MASK) >> XCVR_TSM_END_OF_SEQ_END_OF_RX_WU_SHIFT);
    
    /* Start RX and wait long enough to get valid RSSI */    
    XCVR_ForceRxWu();
    
    /* Wait for end of warmup */
    while ((( XCVR_MISC->XCVR_STATUS & XCVR_MISC_XCVR_STATUS_TSM_COUNT_MASK) >> XCVR_MISC_XCVR_STATUS_TSM_COUNT_SHIFT ) != end_of_wu) {};
    
    // wait 8 AGC read, then wait AGC frozen since 8 read before to read RSSI
    for(uint16_t i = 0U; i < (uint16_t)512; i++) // force a limit to 512 read
    {
        u8agc = (uint8_t)((XCVR_RX_DIG->AGC_STAT & XCVR_RX_DIG_AGC_STAT_CURR_AGC_IDX_MASK) >> XCVR_RX_DIG_AGC_STAT_CURR_AGC_IDX_SHIFT);
        uint8_t u8Rssi = (uint8_t)((XCVR_RX_DIG->RSSI_CTRL_1 &
                            XCVR_RX_DIG_RSSI_CTRL_1_RSSI_OUT_MASK) >> 
                            XCVR_RX_DIG_RSSI_CTRL_1_RSSI_OUT_SHIFT);
        i8Rssi = (int8_t)u8Rssi;
        if (u8_agc_stable_count != 0U)
        {
            u8_agc_stable_count--;
        }
        if ((i >0U) && (u8agc != u8prev_agc))
        {
            u8_agc_stable_count =u8RSSI_n_window_wb + u8_RSSI_stabilized_count;
        }
        if (u8_agc_stable_count == 0U)
        {
          break;
        }
        u8prev_agc = u8agc;
    }
    
    XCVR_ForceRxWd();

    /* Restore corrupted registers */
    XCVR_RX_DIG->RX_DIG_CTRL = t1;
    XCVR_RX_DIG->RSSI_CTRL_0 = t2;
    XCVR_2P4GHZ_PHY->FSK_PD_CFG2 = t3;
    return i8Rssi;
#endif /* RADIO_IS_GEN_4P0 */
}

/* ************************ */
/* Radio debug functions */
/* ************************ */

xcvrStatus_t XCVR_OverrideFrequency (uint32_t freq)
{
  xcvrStatus_t status = gXcvrSuccess_c;
#if RF_OSC_26MHZ == 1
    const uint32_t refOsc = 26000000;
#else
    const uint32_t refOsc = 32000000;
#endif /* RF_OSC_26MHZ == 1 */
    const  uint32_t denominator = 0x08000000UL-1UL; /* Configure the PLL Denominator to the max for best resolution */
    const float64_t sdm_lsb = (float64_t)refOsc / (((float64_t)denominator)/2.0F);

    /* Check the input frequency */
    if ((freq < 2360000000UL) || (freq > 2487000000UL))
    {
        status = gXcvrInvalidParameters_c;
    }
    else
    {
        /* Override the frequency setting in the PLL */
        float64_t   integer_used_in_Hz, 
                 integer_used_in_LSB, 
                 numerator_fraction, 
                 numerator_in_Hz, 
                 numerator_in_LSB, 
                 numerator_unrounded,
                 real_int_and_fraction, 
                 real_fraction, 
                 requested_freq_in_LSB;
        uint32_t temp;
        static uint32_t integer_truncated,
                        integer_to_use;
        static  int32_t numerator_rounded;
        
        /* Write the Low Port Denomintor value */
        XCVR_PLL_DIG->LPM_SDM_CTRL3 = denominator;
        
        /* Configure for Coarse Tune */
        uint32_t coarse_tune_target = freq / 1000000UL;
        temp  =  XCVR_PLL_DIG->CTUNE_CTRL;
        temp &= ~(XCVR_PLL_DIG_CTUNE_CTRL_CTUNE_TARGET_MANUAL_MASK);
        temp |=  XCVR_PLL_DIG_CTUNE_CTRL_CTUNE_TARGET_MANUAL(coarse_tune_target) | XCVR_PLL_DIG_CTUNE_CTRL_CTUNE_TARGET_DISABLE_MASK;
        XCVR_PLL_DIG->CTUNE_CTRL = temp;

        /* Calculate the Low Port values */
        real_int_and_fraction = (float64_t)freq / ((float64_t)refOsc * 2.0F);
        integer_truncated = (uint32_t) trunc(real_int_and_fraction);
        real_fraction = real_int_and_fraction - (float64_t)integer_truncated;

        if (real_fraction > 0.5F) 
        {
            integer_to_use = integer_truncated + 1U;
        } 
        else                     
        {
            integer_to_use = integer_truncated    ;
        }

        numerator_fraction = real_int_and_fraction - (float64_t)integer_to_use; /* Fractional portion of numerator */

        integer_used_in_Hz  = (float64_t)integer_to_use * (float64_t)refOsc * 2.0F;
        integer_used_in_LSB = integer_used_in_Hz / sdm_lsb;

        numerator_in_Hz  = numerator_fraction * (float64_t)refOsc * 2.0F;
        numerator_in_LSB = numerator_in_Hz    / sdm_lsb;

        requested_freq_in_LSB = integer_used_in_LSB + numerator_in_LSB;
        numerator_unrounded = (requested_freq_in_LSB - integer_used_in_LSB);
        numerator_rounded = (int32_t) round(numerator_unrounded);

        /* Write the Low Port Integer and Numerator */
        temp  =  XCVR_PLL_DIG->LPM_SDM_CTRL1;
        temp &= ~XCVR_PLL_DIG_LPM_SDM_CTRL1_LPM_INTG_MASK;
        temp |= (XCVR_PLL_DIG_LPM_SDM_CTRL1_LPM_INTG(integer_to_use) | 
                 XCVR_PLL_DIG_LPM_SDM_CTRL1_SDM_MAP_DISABLE_MASK);
        XCVR_PLL_DIG->LPM_SDM_CTRL1 = temp;
        XCVR_PLL_DIG->LPM_SDM_CTRL2 = (uint32_t)numerator_rounded; /* putting the signed quantity into a 32 bit (unsigned) register */

        /* Ensure that the TSM does not allow Fast warmup */
        XCVR_TSM->FAST_CTRL1 |= XCVR_TSM_FAST_CTRL1_PWRSAVE_WU_CLEAR_MASK;      /* Must assert the bit first */
        XCVR_TSM->FAST_CTRL1 &= ~(XCVR_TSM_FAST_CTRL1_PWRSAVE_WU_CLEAR_MASK);   /* then deasert the bit manually (it is not self clearing) */
    }
    
    return status;
}

xcvrStatus_t XCVR_OverrideCustomChannel (const xcvr_pllChannel_t * customChannel)
{
    xcvrStatus_t status = gXcvrSuccess_c;
    uint32_t temp;

    if (customChannel == NULLPTR)
    {
        status = gXcvrInvalidParameters_c;;
    }
    else
    {
        /* denom = 33554432; abs(numerator) must be <= 20132659 (0x1333333) to meet rule  -.6 < num/denom < .6*/
        temp = customChannel->numerator;
        if ((temp & 0x08000000U) != 0U)
        {
            /* Number was negative so convert to positive twos complement */
            temp = ~temp;   /* ones complement */
            temp &= 0xF0000000U; /* mask off 4 high bits since we're only using the lower 28 bits */
            temp += 1U; 
        }
        if (temp > 0x1333333U)
        {
            status = gXcvrInvalidParameters_c;;
        }
        else
        {
            /* Use a pre-defined denominator and custom numerator */
            XCVR_PLL_DIG->LPM_SDM_CTRL3 = XCVR_PLL_DIG_LPM_SDM_CTRL3_LPM_DENOM(gPllDenom_c);
            XCVR_PLL_DIG->LPM_SDM_CTRL2 = XCVR_PLL_DIG_LPM_SDM_CTRL2_LPM_NUM(customChannel->numerator);
            
            temp = XCVR_PLL_DIG->LPM_SDM_CTRL1;
            temp &= ~XCVR_PLL_DIG_LPM_SDM_CTRL1_LPM_INTG_MASK;
            temp |= XCVR_PLL_DIG_LPM_SDM_CTRL1_LPM_INTG(customChannel->intpart);
            XCVR_PLL_DIG->LPM_SDM_CTRL1 = temp;
            
            /* Stop using the LL channel map and use the manual frequency setting */
            XCVR_PLL_DIG->CHAN_MAP &= ~(XCVR_PLL_DIG_CHAN_MAP_CHANNEL_NUM_OVRD_MASK |
                        XCVR_PLL_DIG_CHAN_MAP_HOP_TBL_CFG_OVRD_MASK | /* Clearing this selects the default channel num behavior */
                        XCVR_PLL_DIG_CHAN_MAP_HOP_TBL_CFG_OVRD_EN_MASK);
            /* Selects manual low port control */
            XCVR_PLL_DIG->LPM_SDM_CTRL1 |= XCVR_PLL_DIG_LPM_SDM_CTRL1_SDM_MAP_DISABLE_MASK;
            
            /* Ensure that the TSM does not allow Fast warmup */
            XCVR_TSM->FAST_CTRL1 |= XCVR_TSM_FAST_CTRL1_PWRSAVE_WU_CLEAR_MASK;      /* Must assert the bit first */
            XCVR_TSM->FAST_CTRL1 &= ~(XCVR_TSM_FAST_CTRL1_PWRSAVE_WU_CLEAR_MASK);   /* then deasert the bit manually (it is not self clearing) */
        }
    }
    return status;
}

xcvrStatus_t XCVR_OverrideChannel (uint8_t channel)
{
    xcvrStatus_t status = gXcvrSuccess_c;
    uint32_t temp;

    /* Over-ride using the hardware channel mapping table */
    if(channel == 0xFFU)
    {
        XCVR_ReleasePLLOverride();
    }
    else
    {
        if (channel >= 128U)
        {
            status = gXcvrInvalidParameters_c;;
        }
        else
        {
            /* apply the channel over-ride */
            temp = XCVR_PLL_DIG->CHAN_MAP;
            temp &= ~(XCVR_PLL_DIG_CHAN_MAP_CHANNEL_NUM_OVRD_MASK |
                        XCVR_PLL_DIG_CHAN_MAP_HOP_TBL_CFG_OVRD_MASK | /* Clearing this selects the default channel num behavior */
                        XCVR_PLL_DIG_CHAN_MAP_HOP_TBL_CFG_OVRD_EN_MASK);
            temp |= (XCVR_PLL_DIG_CHAN_MAP_CHANNEL_NUM_OVRD(channel) |
                    XCVR_PLL_DIG_CHAN_MAP_HOP_TBL_CFG_OVRD_EN(1) |
                    XCVR_PLL_DIG_CHAN_MAP_HOP_TBL_CFG_OVRD(1));
            XCVR_PLL_DIG->CHAN_MAP = temp; 
        }
    }

    return status;
}

void XCVR_ReleasePLLOverride(void)
{
    uint32_t temp;
    /* Stop using the manual frequency setting */
    XCVR_PLL_DIG->LPM_SDM_CTRL1 &= ~XCVR_PLL_DIG_LPM_SDM_CTRL1_SDM_MAP_DISABLE_MASK;
    
    /* Clear all of the overrides and restore to LL channel control */
    temp = XCVR_PLL_DIG->CHAN_MAP;
    temp &= ~(XCVR_PLL_DIG_CHAN_MAP_CHANNEL_NUM_OVRD_MASK | 
                XCVR_PLL_DIG_CHAN_MAP_HOP_TBL_CFG_OVRD_MASK | /* Clearing this selects the default channel num behavior */
                XCVR_PLL_DIG_CHAN_MAP_HOP_TBL_CFG_OVRD_EN_MASK);
    XCVR_PLL_DIG->CHAN_MAP = temp; 

    XCVR_PLL_DIG->CTUNE_CTRL &= ~(XCVR_PLL_DIG_CTUNE_CTRL_CTUNE_TARGET_DISABLE_MASK); /* Release CTUNE_TARGET_DISABLE */

    /* Ensure that the TSM does not allow Fast warmup */
    XCVR_TSM->FAST_CTRL1 |= XCVR_TSM_FAST_CTRL1_PWRSAVE_WU_CLEAR_MASK;      /* Must assert the bit first */
    XCVR_TSM->FAST_CTRL1 &= ~(XCVR_TSM_FAST_CTRL1_PWRSAVE_WU_CLEAR_MASK);   /* then deasert the bit manually (it is not self clearing) */
}

uint32_t XCVR_GetFreq ( void )
{
    /* Allow readback of the current PLL frequency */
    uint32_t pll_int;
    uint32_t pll_num_unsigned;
    uint32_t pll_num_sign = 0x04000000UL;
    int32_t pll_num;
    uint32_t pll_denom;
    float64_t freq_float;
    uint32_t freq = 0;
    /* Using mapped channels or not makes a difference what PLL values are read to calculate frequency */
    if ((XCVR_PLL_DIG->LPM_SDM_CTRL1 & XCVR_PLL_DIG_LPM_SDM_CTRL1_SDM_MAP_DISABLE_MASK) != 0U) /* Not using mapped channels */
    {
        pll_int = (XCVR_PLL_DIG->LPM_SDM_CTRL1 & XCVR_PLL_DIG_LPM_SDM_CTRL1_LPM_INTG_MASK) >>
          XCVR_PLL_DIG_LPM_SDM_CTRL1_LPM_INTG_SHIFT;                 
        
        pll_num_unsigned = XCVR_PLL_DIG->LPM_SDM_CTRL2;
        pll_denom = XCVR_PLL_DIG->LPM_SDM_CTRL3;
        pll_num_sign = 0x08000000UL;
    }
    else
    {
        /* Using mapped channels so need to read from the _SELECTED fields to get the values being used */
        pll_int = (XCVR_PLL_DIG->LPM_SDM_CTRL1 & XCVR_PLL_DIG_LPM_SDM_CTRL1_LPM_INTG_SELECTED_MASK) >>
          XCVR_PLL_DIG_LPM_SDM_CTRL1_LPM_INTG_SELECTED_SHIFT;                 
        
        pll_num_unsigned = XCVR_PLL_DIG->LPM_SDM_RES1;
        pll_denom = XCVR_PLL_DIG->LPM_SDM_RES2;
        pll_num_sign = 0x04000000UL;
    }
    
#if RF_OSC_26MHZ == 1
    float64_t ref_clk2x = 52000000.0F;
#else
    float64_t ref_clk2x = 64000000.0F;
#endif /* RF_OSC_26MHZ == 1 */

    float64_t flt_num;
    float64_t flt_denom = (float32_t)pll_denom;
    float64_t flt_int = (float32_t)pll_int;
    /* Check if sign bit is asserted */
    if ((pll_num_unsigned & pll_num_sign) == pll_num_sign)
    {
        /* Sign extend the numerator */
        uint32_t pll_num_utmp = ((~pll_num_unsigned + 1U) & (pll_num_sign-1U)); /* PLL_NUM is signed 27 bits but stored in 32 bits unsigned */
        pll_num = (int32_t)(pll_num_utmp);
        
        /* Calculate the frequency in MHz */
#if (0)
        freq_float = ((float32_t)ref_clk*2.0F*(((float32_t)pll_int) - (((float32_t)pll_num) / ((float32_t)pll_denom)))); 
#else
        flt_num = (float64_t)pll_num;
        freq_float = ref_clk2x*(flt_int - (flt_num/flt_denom)); 
#endif
    }
    else
    {
        /* Calculate the frequency in MHz */
        pll_num = (int32_t)pll_num_unsigned;
#if (0)
        freq_float = ((float32_t)ref_clk*2.0F*(((float32_t)pll_int) + (((float32_t)pll_num) / ((float32_t)pll_denom)))); 
#else
        flt_num = (float64_t)pll_num;
        freq_float = ref_clk2x*(flt_int + (flt_num/flt_denom)); 
#endif
    }
    freq = (uint32_t)freq_float; /* Integer conversion drops any fractional Hz */
    
    return freq;
}


void XCVR_ForceRxWu ( void )
{
    /* Manual force of RX Warmup */
    XCVR_TSM->CTRL |= XCVR_TSM_CTRL_FORCE_RX_EN_MASK;
}

void XCVR_ForceRxWd ( void )
{
    /* Manual force of RX Warmdown */
    XCVR_TSM->CTRL &= ~XCVR_TSM_CTRL_FORCE_RX_EN_MASK;
}

void XCVR_ForceTxWu ( void )
{
    /* Manual force of TX Warmup */
    XCVR_TSM->CTRL |= XCVR_TSM_CTRL_FORCE_TX_EN_MASK;
}

void XCVR_ForceTxWd ( void )
{
    /* Manual force of TX Warmdown */
    XCVR_TSM->CTRL &= ~XCVR_TSM_CTRL_FORCE_TX_EN_MASK;
}

void XCVR_WaitRxWu ( void )
{
    uint32_t end_of_wu = (XCVR_TSM->END_OF_SEQ & XCVR_TSM_END_OF_SEQ_END_OF_RX_WU_MASK) >> XCVR_TSM_END_OF_SEQ_END_OF_RX_WU_SHIFT;
    while ((( XCVR_MISC->XCVR_STATUS & XCVR_MISC_XCVR_STATUS_TSM_COUNT_MASK) >> XCVR_MISC_XCVR_STATUS_TSM_COUNT_SHIFT ) != end_of_wu) {}; /* Wait for TSM RX WU complete */
}

void XCVR_WaitTxWu ( void )
{
    uint32_t end_of_wu = (XCVR_TSM->END_OF_SEQ & XCVR_TSM_END_OF_SEQ_END_OF_TX_WU_MASK) >> XCVR_TSM_END_OF_SEQ_END_OF_TX_WU_SHIFT;
    while ((( XCVR_MISC->XCVR_STATUS & XCVR_MISC_XCVR_STATUS_TSM_COUNT_MASK) >> XCVR_MISC_XCVR_STATUS_TSM_COUNT_SHIFT ) != end_of_wu) {}; /* Wait for TSM TX WU complete */
}


xcvrStatus_t XCVR_DftTxCW(uint32_t rf_freq)
{
#if defined(RADIO_IS_GEN_4P0)
    return gXcvrUnsupportedOperation_c;
  
#else
    xcvrStatus_t status = gXcvrSuccess_c;

    /* Implements a CW transmit function for testing and trimming operations */

    /* Set the RF Frequency to a specific value (1Hz resolution) */
    status = XCVR_OverrideFrequency(rf_freq);
    if (status == gXcvrSuccess_c)
    {
        /* No modulation */
        /* GFSK configuration */
        XCVR_TX_DIG->TXDIG_CTRL = XCVR_TX_DIG_TXDIG_CTRL_MODULATOR_SEL(0U) |
                                XCVR_TX_DIG_TXDIG_CTRL_PFC_EN(0U) |
                                XCVR_TX_DIG_TXDIG_CTRL_DATA_STREAM_SEL(0U);

        /* First shut everything off */
        XCVR_MISC->RADIO_DFT = XCVR_MISC_RADIO_DFT_RADIO_DFT_MODE(0U);     /* Clear RADIO_DFT_MODE */
        XCVR_TX_DIG->RF_DFT_TX_CTRL0 &= ~(XCVR_TX_DIG_RF_DFT_TX_CTRL0_DFT_RAM_EN_MASK);/* Clear DFT_RAM_EN */
        XCVR_TX_DIG->RF_DFT_TX_CTRL1 &= ~(XCVR_TX_DIG_RF_DFT_TX_CTRL1_LFSR_EN_MASK);/* Clear LFSR_EN */
        XCVR_TX_DIG->RF_DFT_TX_CTRL2 &= ~(XCVR_TX_DIG_RF_DFT_TX_CTRL2_DFT_PATTERN_EN_MASK); /* Clear DFT_PATTERN_EN */

        XCVR_MISC->RADIO_DFT = XCVR_MISC_RADIO_DFT_RADIO_DFT_MODE(1U);     /* Set CW TX RADIO_DFT_MODE */

        /* Warm-up the Radio */
        XCVR_ForceTxWu();
    }
    
    return status; /* Success */
#endif /* RADIO_IS_GEN_4P0 */
}

xcvrStatus_t XCVR_DftTxPatternReg(uint8_t channel_num, const xcvr_config_t ** xcvr_config, const xcvr_coding_config_t ** rbme_config, uint32_t tx_pattern)
{
#if defined(RADIO_IS_GEN_4P0)
    return gXcvrUnsupportedOperation_c;
  
#else
    uint8_t dft_clk_sel = 0U;
    xcvrStatus_t status = gXcvrSuccess_c;
    const xcvr_config_t * cfgptr;
    data_rate_t temp;
    
    /* Implements a Pattern Reg based TX for testing and trimming operations */

    if ((xcvr_config == NULLPTR) || (rbme_config == NULLPTR))
    {
        status = gXcvrInvalidParameters_c;
    }
    else
    {
        cfgptr = *xcvr_config;
        status = XCVR_ChangeMode(xcvr_config, rbme_config); /* this sets the main modulation parameters but doesn't set RBME */
        status = XCVR_SetActiveLL(XCVR_ACTIVE_LL_GENFSK);   /* Make sure GENFSK LL is active */

        /* Select the RF Channel, using the Channel Number register */
        status = XCVR_OverrideChannel(channel_num);
        if (status == gXcvrSuccess_c)
        {
        
            /* DFT modulation is determined by radio mode */
            if (cfgptr->mode_data_rate_cfg->radio_mode == MSK)
            {
                /* MSK configuration */
                XCVR_TX_DIG->TXDIG_CTRL = XCVR_TX_DIG_TXDIG_CTRL_MODULATOR_SEL(1U) |
                                        XCVR_TX_DIG_TXDIG_CTRL_PFC_EN(1U) |
                                        XCVR_TX_DIG_TXDIG_CTRL_DATA_STREAM_SEL(0U);
            }
            else
            {
                XCVR_TX_DIG->TXDIG_CTRL = XCVR_TX_DIG_TXDIG_CTRL_MODULATOR_SEL(0U) |
                                        XCVR_TX_DIG_TXDIG_CTRL_PFC_EN(0U) |
                                        XCVR_TX_DIG_TXDIG_CTRL_DATA_STREAM_SEL(0U);
            }

            /* Clock selection is controlled by the desired data rate */
            if ((GENFSK->ENH_FEATURE & GENFSK_ENH_FEATURE_DATARATE_CONFIG_SEL_MASK) == 0U)
            {
                /* Primary data rate */
                temp = cfgptr->mode_data_rate_cfg->data_rate;
            }
            else
            {
                /* Alternate data rate */
                temp = cfgptr->mode_data_rate_cfg->alt_data_rate;
            }
            switch (temp)
            {
                case DR_2MBPS:
                    dft_clk_sel = 5U;
                    break;
                case DR_1MBPS:
                    dft_clk_sel = 4U;
                    break;
                case DR_500KBPS:
                    dft_clk_sel = 3U;
                    break;
                case DR_250KBPS:
                    dft_clk_sel = 2U;
                    break;
                default:
                    status = gXcvrInvalidParameters_c;
                    break;
            }

            /* First shut everything off */
            XCVR_MISC->RADIO_DFT = XCVR_MISC_RADIO_DFT_RADIO_DFT_MODE(0U);     /* Clear RADIO_DFT_MODE */
            XCVR_TX_DIG->RF_DFT_TX_CTRL0 &= ~(XCVR_TX_DIG_RF_DFT_TX_CTRL0_DFT_RAM_EN_MASK);/* Clear DFT_RAM_EN */
            XCVR_TX_DIG->RF_DFT_TX_CTRL1 &= ~(XCVR_TX_DIG_RF_DFT_TX_CTRL1_LFSR_EN_MASK);/* Clear LFSR_EN */
            XCVR_TX_DIG->RF_DFT_TX_CTRL2 &= ~(XCVR_TX_DIG_RF_DFT_TX_CTRL2_DFT_PATTERN_EN_MASK); /* Clear DFT_PATTERN_EN */

            XCVR_MISC->RADIO_DFT = XCVR_MISC_RADIO_DFT_RADIO_DFT_MODE(2U);     /* Set PATTERN RADIO_DFT_MODE */
            XCVR_TX_DIG->RF_DFT_TX_CTRL1 = XCVR_TX_DIG_RF_DFT_TX_CTRL1_LFSR_CLK_SEL(dft_clk_sel); /* leaves LFSR mode disabled but sets clock field which is common */    
            XCVR_TX_DIG->RF_DFT_PATTERN = tx_pattern;

            XCVR_TX_DIG->DATA_PADDING_CTRL = XCVR_TX_DIG_DATA_PADDING_CTRL_PAD_DLY_EN(0);  /* PAD_DLY_EN=0 is required for TX DFT to be operational */

            if (status == gXcvrSuccess_c)
            {
                /* Warm-up the Radio */
                XCVR_ForceTxWu();
                XCVR_TX_DIG->RF_DFT_TX_CTRL2 = XCVR_TX_DIG_RF_DFT_TX_CTRL2_DFT_PATTERN_EN(1); /* now enable the PATTERN mode */
            }
        }
    }
    
    return status;
    
#endif /* RADIO_IS_GEN_4P0 */
}

xcvrStatus_t XCVR_DftTxLfsrReg(uint8_t channel_num, const xcvr_config_t ** xcvr_config, const xcvr_coding_config_t ** rbme_config, uint8_t lfsr_length)
{
#if defined(RADIO_IS_GEN_4P0)
    return gXcvrUnsupportedOperation_c;
  
#else
    xcvrStatus_t status = gXcvrSuccess_c;
    uint8_t dft_clk_sel = 0;
    const xcvr_config_t * cfgptr;
    data_rate_t temp;

    /* Implements a LFSR Reg based TX for testing and trimming operations */

    if ((lfsr_length > 5U) || (xcvr_config == NULLPTR) || (rbme_config == NULLPTR))
    {
        status =  gXcvrInvalidParameters_c;
    }
    else
    {
        cfgptr = *xcvr_config;
        status = XCVR_ChangeMode(xcvr_config, rbme_config); /* this sets the main modulation parameters */
        status = XCVR_SetActiveLL(XCVR_ACTIVE_LL_GENFSK);   /* Make sure GENFSK LL is active */

        /* Select the RF Channel, using the Channel Number register */
            status = XCVR_OverrideChannel(channel_num);
        }
        
        if (status == gXcvrSuccess_c)
        {
            /* DFT modulation is determined by radio mode */
            if (cfgptr->mode_data_rate_cfg->radio_mode == MSK)
            {
                /* MSK configuration */
                XCVR_TX_DIG->TXDIG_CTRL = XCVR_TX_DIG_TXDIG_CTRL_MODULATOR_SEL(1U) |
                                        XCVR_TX_DIG_TXDIG_CTRL_PFC_EN(1U) |
                                        XCVR_TX_DIG_TXDIG_CTRL_DATA_STREAM_SEL(0U);
            }
            else
            {
                XCVR_TX_DIG->TXDIG_CTRL = XCVR_TX_DIG_TXDIG_CTRL_MODULATOR_SEL(0U) |
                                        XCVR_TX_DIG_TXDIG_CTRL_PFC_EN(0U) |
                                        XCVR_TX_DIG_TXDIG_CTRL_DATA_STREAM_SEL(0U);
            }
            
            /* Clock selection is controlled by the desired data rate */
            if ((GENFSK->ENH_FEATURE & GENFSK_ENH_FEATURE_DATARATE_CONFIG_SEL_MASK) == 0U)
            {
                /* Primary data rate */
                temp = cfgptr->mode_data_rate_cfg->data_rate;
            }
            else
            {
                /* Alternate data rate */
                temp = cfgptr->mode_data_rate_cfg->alt_data_rate;
            }
            switch (temp)
            {
                case DR_2MBPS:
                    dft_clk_sel = 5U;
                    break;
                case DR_1MBPS:
                    dft_clk_sel = 4U;
                    break;
                case DR_500KBPS:
                    dft_clk_sel = 3U;
                    break;
                case DR_250KBPS:
                    dft_clk_sel = 2U;
                    break;
                default:
                    status = gXcvrInvalidParameters_c;
                    break;
            }

            /* First shut everything off */
            XCVR_MISC->RADIO_DFT = XCVR_MISC_RADIO_DFT_RADIO_DFT_MODE(0U);     /* Clear RADIO_DFT_MODE */
            XCVR_TX_DIG->RF_DFT_TX_CTRL0 &= ~(XCVR_TX_DIG_RF_DFT_TX_CTRL0_DFT_RAM_EN_MASK);/* Clear DFT_RAM_EN */
            XCVR_TX_DIG->RF_DFT_TX_CTRL1 &= ~(XCVR_TX_DIG_RF_DFT_TX_CTRL1_LFSR_EN_MASK);/* Clear LFSR_EN */
            XCVR_TX_DIG->RF_DFT_TX_CTRL2 &= ~(XCVR_TX_DIG_RF_DFT_TX_CTRL2_DFT_PATTERN_EN_MASK); /* Clear DFT_PATTERN_EN */

            XCVR_MISC->RADIO_DFT = XCVR_MISC_RADIO_DFT_RADIO_DFT_MODE(3U);     /* Set LFSR RADIO_DFT_MODE */
            XCVR_TX_DIG->RF_DFT_TX_CTRL1 = XCVR_TX_DIG_RF_DFT_TX_CTRL1_LFSR_CLK_SEL(dft_clk_sel) |
                                            XCVR_TX_DIG_RF_DFT_TX_CTRL1_LFSR_LENGTH(lfsr_length) |
                                            XCVR_TX_DIG_RF_DFT_TX_CTRL1_LRM(0U) | /* hardcode the LFSR to reset the sequence when disabled */
                                            XCVR_TX_DIG_RF_DFT_TX_CTRL1_LFSR_EN(0U); /* leave the LFSR disabled until after TX warmup */
            
            XCVR_TX_DIG->DATA_PADDING_CTRL = XCVR_TX_DIG_DATA_PADDING_CTRL_PAD_DLY_EN(0);  /* PAD_DLY_EN=0 is required for TX DFT to be operational */

            if (status == gXcvrSuccess_c)
            {
                /* Warm-up the Radio */
                XCVR_ForceTxWu();
                XCVR_TX_DIG->RF_DFT_TX_CTRL1 |= XCVR_TX_DIG_RF_DFT_TX_CTRL1_LFSR_EN(1); /* now enable the LFSR */
            }
        }
    return status;
#endif /* RADIO_IS_GEN_4P0 */
}




void XCVR_DftTxOff(void)
{
#if defined(RADIO_IS_GEN_4P0)
  
#else
    xcvrStatus_t status;
    /* Release DFT TX that is in progress */
    XCVR_ForceTxWd();
    XCVR_TX_DIG->PA_CTRL |= XCVR_TX_DIG_PA_CTRL_TGT_PWR_SRC_MASK; /* Use PA_POWER in LL registers */
    /* Clear the RF Channel over-ride */
    status = XCVR_OverrideChannel(0xFF); /* 0xFF is the release override code */
    (void) status; /* Workaround unused variable warnings in GCC */
    XCVR_MISC->RADIO_DFT &= ~(XCVR_MISC_RADIO_DFT_RADIO_DFT_MODE_MASK);     /* Clear RADIO_DFT_MODE */
    XCVR_TX_DIG->RF_DFT_TX_CTRL0 &= ~(XCVR_TX_DIG_RF_DFT_TX_CTRL0_DFT_RAM_EN_MASK);/* Clear DFT_RAM_EN */
    XCVR_TX_DIG->RF_DFT_TX_CTRL1 &= ~(XCVR_TX_DIG_RF_DFT_TX_CTRL1_LFSR_EN_MASK);/* Clear LFSR_EN */
    XCVR_TX_DIG->RF_DFT_TX_CTRL2 &= ~(XCVR_TX_DIG_RF_DFT_TX_CTRL2_DFT_PATTERN_EN_MASK); /* Clear DFT_PATTERN_EN */
#endif /* RADIO_IS_GEN_4P0 */
}

xcvrStatus_t XCVR_ForcePAPower(uint8_t pa_power)
{
    xcvrStatus_t status = gXcvrInvalidParameters_c;

    /* Force a PA power value to override the LL controlled setting */
    if (pa_power <= 0x3FU)
    {
        uint32_t temp_pa_pwr = pa_power;
        temp_pa_pwr &= (pa_power > 1U) ? 0xFEU : 0xFFU; /* Ensure the lowest bit is cleared to prevent incorrect PA power setting */
        XCVR_TX_DIG->PA_CTRL &= ~(XCVR_TX_DIG_PA_CTRL_PA_TGT_POWER_MASK | XCVR_TX_DIG_PA_CTRL_TGT_PWR_SRC_MASK); 
        XCVR_TX_DIG->PA_CTRL |= (XCVR_TX_DIG_PA_CTRL_PA_TGT_POWER(temp_pa_pwr) |
                                XCVR_TX_DIG_PA_CTRL_TGT_PWR_SRC(0UL)); /* Select power source to be TX_DIG PA_TGT_POWER field */
        status = gXcvrSuccess_c;
    }

    return status; /* Success */
}

void XCVR_ReleasePAPower(void)
{
    /* return a previously forced PA power setting (using TX_DIG PA_TGT_POWER value) to the normal link layer controlled power setting */
    
    XCVR_TX_DIG->PA_CTRL |= XCVR_TX_DIG_PA_CTRL_TGT_PWR_SRC_MASK;
}

xcvrStatus_t XCVR_SetManAgc(uint8_t man_agc_idx)
{
#if defined(RADIO_IS_GEN_4P0)
    return gXcvrUnsupportedOperation_c;
  
#else
    xcvrStatus_t status = gXcvrSuccess_c;
    /* Force a Manual AGC value to override the RX digital controlled setting */
    if (man_agc_idx >26U)
    {
        /* error case */
        status = gXcvrInvalidParameters_c;
    }
    else
    {
        /* Set AGC to manual mode at a specific AGC index */
        /* DCOC calibration only happens during RX initialization, so pulse the RX INIT signal manually */
        XCVR_RX_DIG->AGC_MAN_AGC_IDX = (XCVR_RX_DIG->AGC_MAN_AGC_IDX & ~XCVR_RX_DIG_AGC_MAN_AGC_IDX_AGC_MAN_IDX_MASK) | XCVR_RX_DIG_AGC_MAN_AGC_IDX_AGC_MAN_IDX(man_agc_idx);  
        XCVR_RX_DIG->AGC_MAN_AGC_IDX |= XCVR_RX_DIG_AGC_MAN_AGC_IDX_AGC_DCOC_START_PT_MASK | XCVR_RX_DIG_AGC_MAN_AGC_IDX_AGC_MAN_IDX_EN_MASK;
        XCVR_RX_DIG->RX_DIG_CTRL |= XCVR_RX_DIG_RX_DIG_CTRL_RX_AGC_EN_MASK; /* AGC must be enabled for the manual value to be applied */
        force_rx_init_pulse();
        XCVR_RX_DIG->RX_DIG_CTRL &= ~(XCVR_RX_DIG_RX_DIG_CTRL_RX_AGC_EN_MASK); /* AGC must be disabled for the manual value to remain applied */
    }

    return status;
#endif /* RADIO_IS_GEN_4P0 */
}

void XCVR_ClearManAgc(void)
{
#if defined(RADIO_IS_GEN_4P0)
  
#else
    /* Release a forced Manual AGC setting */
    /* Clear manual AGC and return to automatic */
    /* DCOC calibration only happens during RX initialization, so pulse the RX INIT signal manually */
    XCVR_RX_DIG->RX_DIG_CTRL |= XCVR_RX_DIG_RX_DIG_CTRL_RX_AGC_EN_MASK; /* AGC must be enabled for the automatic gain to be resumed */
    XCVR_RX_DIG->AGC_MAN_AGC_IDX &= ~(XCVR_RX_DIG_AGC_MAN_AGC_IDX_AGC_DCOC_START_PT_MASK | XCVR_RX_DIG_AGC_MAN_AGC_IDX_AGC_MAN_IDX_EN_MASK);
    force_rx_init_pulse();
#endif /* RADIO_IS_GEN_4P0 */
}

void XCVR_bumpPaPowerUp(void)
{
#if defined(RADIO_IS_GEN_4P0)
  
#else
    uint32_t temp_trim;
    temp_trim = XCVR_ANALOG->BB_LDO_2;
    temp_trim &= ~(XCVR_ANALOG_BB_LDO_2_BB_LDO_HF_TRIM_TX_MASK);
    temp_trim |= XCVR_ANALOG_BB_LDO_2_BB_LDO_HF_TRIM_TX(0x5U);  /* Measured value required for high PA power output (+5dBm) */
    XCVR_ANALOG->BB_LDO_2 = temp_trim;
#endif /* RADIO_IS_GEN_4P0 */
}

void XCVR_releasePaPowerBump(void)
{
#if defined(RADIO_IS_GEN_4P0)
  
#else
    uint32_t temp_trim;
    temp_trim = XCVR_ANALOG->BB_LDO_2;
    temp_trim &= ~(XCVR_ANALOG_BB_LDO_2_BB_LDO_HF_TRIM_TX_MASK);
    temp_trim |= XCVR_ANALOG_BB_LDO_2_BB_LDO_HF_TRIM_TX(default_bb_ldo_fh_tx);  /* Put back the original value */
    XCVR_ANALOG->BB_LDO_2 = temp_trim;
#endif /* RADIO_IS_GEN_4P0 */
}

    
uint16_t XCVR_ReadRadioVer(void)
{
    uint16_t radio_id_gen = 0xFFFFU;

#if defined(RADIO_IS_GEN_4P0)
    radio_id_gen = (uint16_t)((RFMC->VERID&maskvalue )>> shiftvalue); /* Gen4 radio version is in RFMC */
#else
    /* Read RSIM->MISC RADIO_VER & parse with a switch statement */
    uint32_t temp = ((RSIM->MISC&RSIM_MISC_RADIO_VERSION_MASK)>>RSIM_MISC_RADIO_VERSION_SHIFT);  /* Gen3.5/3.6 (and earlier) radio version is in RSIM */
    radio_id_gen = XCVR_IdentifyRadioVer(temp);
#endif /* RADIO_IS_GEN_4P0 */
    return radio_id_gen; /* Represents both radio_id or radio_ver field, depending upon radio generation */
}


/*!
 * @brief Helper function to identify the radio version for this silicon.
 *
 * This function maps the value read from the RADIO_VER field and to an enumeration based on that value.
 *
 * @return The radio version of the silicon.
 */
uint16_t XCVR_IdentifyRadioVer(uint32_t radio_ver)
{
    uint16_t radio_id_gen;
    switch (radio_ver)
    {
      case 0xBU:
            radio_id_gen = (uint16_t)XCVR_RADIO_GEN_2P0; /* Legacy, should never occur but included for completeness */
            break;
      case 0x8U:
            radio_id_gen = (uint16_t)XCVR_RADIO_GEN_2P1; /* Legacy, should never occur but included for completeness */
            break;
      case 0x4U:
            radio_id_gen = (uint16_t)XCVR_RADIO_GEN_3P0; /* Legacy, should never occur but included for completeness */
            break;
      case 0x5U:
            radio_id_gen = (uint16_t)XCVR_RADIO_GEN_3P1; /* Legacy, should never occur but included for completeness */
            break;
      case 0xCU:
            radio_id_gen = (uint16_t)XCVR_RADIO_GEN_3P5;
            break;
      case 0x6U:
            radio_id_gen = (uint16_t)XCVR_RADIO_GEN_3P5_B0;
            break;
        default:    
            radio_id_gen = (uint16_t)XCVR_RADIO_GEN_INVALID;
            break;
    }
    return radio_id_gen; /* Intentionally returns uint16_t for compatibility with future revision numbers */
}

