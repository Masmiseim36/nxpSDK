/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "nxp_xcvr_ext_ctrl.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Variables
 ******************************************************************************/
static lclRxTxMode_t lcl_patt_match_mode = gLclRxTxNone;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/*******************************************************************************
 * Code
 ******************************************************************************/

 xcvrStatus_t XCVR_CoexistenceInit(void)
 {
     /* Initialize for co-existence feature in the XCVR */
#if (gMWS_UseCoexistence_d || TEST_BUILD_COEX)
     uint32_t temp = 0x00U;
     uint32_t end_of_tx_wu = 0x00U;
     uint32_t end_of_rx_wu = 0x00U;
         
   
     /*********************/
     /*   KW35x / KW36x   */
     /*********************/
#if (defined(CPU_MKW37A512VFT4) || defined(CPU_MKW35Z512VHT4) || defined(CPU_MKW38A512VFT4) || \
     defined(CPU_MKW36A512VHT4) || defined(CPU_MKW39A512VFT4) || defined(CPU_MKW39Z512VFT4))
 
     // RF_ACTIVE = TX_SWITCH (PTC2, gpio2_trig_en)
     uint32_t tsm_timing49 = 0x00U;
 
     // RF_PRIORITY = RX_SWITCH (PTC3, gpio3_trig_en)
     uint32_t tsm_timing50 = 0x00U;
 
     // TSM_TIMING43 is used to control the tsm_irq0_trig signal
     uint32_t tsm_timing43_rx = 0x00;
     uint32_t tsm_timing43_tx = 0x00;
 
     /* Select GPIO mode for FAD pins */
     temp = XCVR_MISC->FAD_CTRL;
     temp &= ~(XCVR_MISC_FAD_CTRL_FAD_NOT_GPIO_MASK);
     XCVR_MISC->FAD_CTRL = temp;
 
     /* Read the END_OF_TX_WU and END_OF_RX_WU for XCVR */
     end_of_tx_wu = (XCVR_TSM->END_OF_SEQ & XCVR_TSM_END_OF_SEQ_END_OF_TX_WU_MASK) >>
                     XCVR_TSM_END_OF_SEQ_END_OF_TX_WU_SHIFT;
     end_of_rx_wu = (XCVR_TSM->END_OF_SEQ & XCVR_TSM_END_OF_SEQ_END_OF_RX_WU_MASK) >>
                     XCVR_TSM_END_OF_SEQ_END_OF_RX_WU_SHIFT;
     
     /*****************
      *  TX SEQUENCE  *
      *****************/
     
     if (end_of_tx_wu < gMWS_CoexRfActiveAssertTime_d)
     {
         temp = end_of_tx_wu;
     }
     else
     {
         temp = gMWS_CoexRfActiveAssertTime_d;
     }
 
     /* Save the TX RF_ACTIVE start time. */
     tsm_timing43_tx = end_of_tx_wu - temp;
 
     /* Set RF_ACTIVE pin HIGH gMWS_CoexRfActiveAssertTime_d uS prior to any TX sequence. */
     tsm_timing49 = (((uint32_t)(tsm_timing43_tx) << XCVR_TSM_TIMING49_GPIO2_TRIG_EN_TX_HI_SHIFT) &
                                                         XCVR_TSM_TIMING49_GPIO2_TRIG_EN_TX_HI_MASK);
 
     /* Set STATUS pin HIGH gMWS_CoexRfActiveAssertTime_d uS prior to any TX sequence. */
     tsm_timing50 = (((uint32_t)(tsm_timing43_tx) << XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_HI_SHIFT) &
                                                         XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_HI_MASK);
     /*****************
      *  RX SEQUENCE  *
      *****************/
     
     if (end_of_rx_wu < gMWS_CoexRfActiveAssertTime_d)
     {
         temp = end_of_rx_wu;
     }
     else
     {
         temp = gMWS_CoexRfActiveAssertTime_d;
     }
 
#if (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d)
     /* Set RF_ACTIVE pin HIGH gMWS_CoexRfActiveAssertTime_d uS prior to any RX sequence. */
     tsm_timing49 |= (((uint32_t)(end_of_rx_wu - temp) << XCVR_TSM_TIMING49_GPIO2_TRIG_EN_RX_HI_SHIFT) &
                                                          XCVR_TSM_TIMING49_GPIO2_TRIG_EN_RX_HI_MASK);
 
     /* Set STATUS pin HIGH gMWS_CoexRfActiveAssertTime_d uS prior to any RX sequence and clear it gMWS_CoexPrioSignalTime_d uS before RX start. */
     tsm_timing50 |= ((((uint32_t)(end_of_rx_wu - temp) << XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_SHIFT) &
                                                           XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_MASK) |
                      (((uint32_t)(end_of_rx_wu - gMWS_CoexPrioSignalTime_d) << XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_LO_SHIFT) &
                                                                              XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_LO_MASK));
 
     temp = XCVR_TSM->TIMING49;
     temp &= ~(XCVR_TSM_TIMING49_GPIO2_TRIG_EN_TX_HI_MASK |
               XCVR_TSM_TIMING49_GPIO2_TRIG_EN_RX_HI_MASK);
     temp |= tsm_timing49;
     XCVR_TSM->TIMING49 = temp;
 
     temp = XCVR_TSM->TIMING50;
     temp &= ~(XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_HI_MASK |
               XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_MASK |
               XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_LO_MASK);
     temp |= tsm_timing50;
     XCVR_TSM->TIMING50 = temp;
#endif /* (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d) */
     
#if (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d)
     /* Set RF_ACTIVE pin HIGH gMWS_CoexRfActiveAssertTime_d uS prior to any RX sequence. */
     tsm_timing49 |= (((uint32_t)(end_of_rx_wu - temp) << XCVR_TSM_TIMING49_GPIO2_TRIG_EN_RX_HI_SHIFT) &
                                                          XCVR_TSM_TIMING49_GPIO2_TRIG_EN_RX_HI_MASK);
 
     /* Set PRIORITY pin HIGH gMWS_CoexRfActiveAssertTime_d uS prior to any RX sequence and clear it gMWS_CoexPrioSignalTime_d uS before RX start. */
     tsm_timing50 |= (((uint32_t)(end_of_rx_wu - temp) << XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_SHIFT) &
                                                           XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_MASK);
 
     /* RF_ACTIVE */
     temp = XCVR_TSM->TIMING49;
     temp &= ~(XCVR_TSM_TIMING49_GPIO2_TRIG_EN_TX_HI_MASK | XCVR_TSM_TIMING49_GPIO2_TRIG_EN_RX_HI_MASK);
     temp |= tsm_timing49;
     XCVR_TSM->TIMING49 = temp;
 
     /* RF_PRIORITY */
     temp = XCVR_TSM->TIMING50;
     temp &= ~(XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_HI_MASK | XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_MASK);
     temp |= tsm_timing50;
     XCVR_TSM->TIMING50 = temp;
#endif /* (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d) */
     
     /* Overwrite pins settings */
     GPIOC->PDDR |= 0x0CU;
     PORTC->PCR[3] = (PORTC->PCR[3] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(2U);
     PORTC->PCR[2] = (PORTC->PCR[2] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(2U);
 
     /* It should assert 2 ticks prior to END_OF_RX_WU and de-assert 1 tick later at 1 tick prior to END_OF_TX_WU */
     tsm_timing43_tx = end_of_tx_wu - 2U;
     /* It should assert 2 ticks prior to END_OF_RX_WU and de-assert 1 tick later at 1 tick prior to END_OF_RX_WU */
     tsm_timing43_rx = end_of_rx_wu - 2U;
 
     XCVR_TSM->TIMING43 = ((((uint32_t)(tsm_timing43_tx) << XCVR_TSM_TIMING43_TSM_SPARE0_EN_TX_HI_SHIFT) & XCVR_TSM_TIMING43_TSM_SPARE0_EN_TX_HI_MASK) |
                           (((uint32_t)(tsm_timing43_tx + 1U) << XCVR_TSM_TIMING43_TSM_SPARE0_EN_TX_LO_SHIFT) & XCVR_TSM_TIMING43_TSM_SPARE0_EN_TX_LO_MASK) |
                           (((uint32_t)(tsm_timing43_rx) << XCVR_TSM_TIMING43_TSM_SPARE0_EN_RX_HI_SHIFT) & XCVR_TSM_TIMING43_TSM_SPARE0_EN_RX_HI_MASK) |
                           (((uint32_t)(tsm_timing43_rx + 1U) << XCVR_TSM_TIMING43_TSM_SPARE0_EN_RX_LO_SHIFT) & XCVR_TSM_TIMING43_TSM_SPARE0_EN_RX_LO_MASK));
 
     BTLE_RF->MISC_CTRL = 0x02;
 
     /* Enable TSM_IRQ0 interrupt */
     XCVR_TSM->CTRL |= XCVR_TSM_CTRL_TSM_IRQ0_EN_MASK;
 
     /* Save the updated registers values. */
     XCVR_CoexistenceSaveRestoreTimings(1U);
     
#endif /* CPU_MKW37Axxx / CPU_MKW37Axxx/ CPU_MKW39Axxx */
     
#endif /* gMWS_UseCoexistence_d */ 
     
     return gXcvrSuccess_c;
 }

 
 xcvrStatus_t XCVR_CoexistenceSetPriority(XCVR_COEX_PRIORITY_T rxPriority, XCVR_COEX_PRIORITY_T txPriority)
 {
     /* Setup the priority for co-existence feature in the XCVR */
#if (gMWS_UseCoexistence_d || TEST_BUILD_COEX)
         uint32_t temp = 0x00U;
         uint32_t end_of_tx_wu = 0x00U;
         uint32_t end_of_rx_wu = 0x00U;
     
  
     /*********************/
     /*   KW35x / KW36x   */
     /*********************/
#if (defined(CPU_MKW37A512VFT4) || defined(CPU_MKW35Z512VHT4) || defined(CPU_MKW38A512VFT4) || \
          defined(CPU_MKW36A512VHT4) || defined(CPU_MKW39A512VFT4) || defined(CPU_MKW39Z512VFT4))
     
     uint32_t tsm_timing50 = 0x00U;
 
     /* Read the END_OF_TX_WU and END_OF_RX_WU for XCVR */
     end_of_tx_wu = (XCVR_TSM->END_OF_SEQ & XCVR_TSM_END_OF_SEQ_END_OF_TX_WU_MASK) >>
                     XCVR_TSM_END_OF_SEQ_END_OF_TX_WU_SHIFT;
     end_of_rx_wu = (XCVR_TSM->END_OF_SEQ & XCVR_TSM_END_OF_SEQ_END_OF_RX_WU_MASK) >>
                     XCVR_TSM_END_OF_SEQ_END_OF_RX_WU_SHIFT;
     
   /*****************
      *      RX       *
      *****************/
     
     if (XCVR_COEX_HIGH_PRIO == rxPriority)
     {
         if (end_of_rx_wu < gMWS_CoexRfActiveAssertTime_d)
         {
             temp = end_of_rx_wu;
         }
         else
         {
             temp = gMWS_CoexRfActiveAssertTime_d;
         }
 
#if (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d)
         /* Set STATUS pin HIGH gMWS_CoexRfActiveAssertTime_d uS prior to any RX sequence and clear it gMWS_CoexPrioSignalTime_d uS before RX start for high priority RX. */
         tsm_timing50 = ((((uint32_t)(end_of_rx_wu - temp) << XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_SHIFT) &
                                                              XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_MASK) |
                         (((uint32_t)(end_of_rx_wu - gMWS_CoexPrioSignalTime_d) << XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_LO_SHIFT) &
                                                                                       XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_LO_MASK));
#endif /* (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d) */
     
#if (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d)
      /* Set STATUS pin HIGH gMWS_CoexRfActiveAssertTime_d uS prior to any RX sequence */
         tsm_timing50 = (((uint32_t)(end_of_rx_wu - temp) << XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_SHIFT) &
                                                             XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_MASK);
#endif /* (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d) */
     }
     else
     {
         /* Low priority RX */
         tsm_timing50 = (((0xFFUL << XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_SHIFT) &
                                    XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_MASK) |
                         ((0xFFUL << XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_LO_SHIFT) &
                                    XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_LO_MASK));
     }
 
    /*****************
        *      TX       *
        *****************/
     if (XCVR_COEX_HIGH_PRIO == txPriority)
     {
         if (end_of_tx_wu < gMWS_CoexRfActiveAssertTime_d)
         {
             temp = end_of_tx_wu;
         }
         else
         {
             temp = gMWS_CoexRfActiveAssertTime_d;
         }
 
         /* Set STATUS pin HIGH gMWS_CoexRfActiveAssertTime_d uS prior to any TX sequence for HIGH priority TX. */
         tsm_timing50 |= (((uint32_t)(end_of_tx_wu - temp) << XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_HI_SHIFT) &
                                                              XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_HI_MASK);
     }
     else
     {
#if (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d)
         /* Set STATUS pin HIGH at END_OF_TX_WU prior to any TX sequence for LOW priority TX. */
         tsm_timing50 |= (((uint32_t)(end_of_tx_wu) << XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_HI_SHIFT) &
                                                       XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_HI_MASK);
#endif /* (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d) */
#if (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d)
         /* Set STATUS pin LOW at END_OF_TX_WU prior to any TX sequence for LOW priority TX. */
         tsm_timing50 = (((0xFFU << XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_HI_SHIFT) &
                                    XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_HI_MASK) |
                         ((0xFFU << XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_LO_SHIFT) &
                                    XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_LO_MASK));
#endif /* (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d) */
     }
 
#if (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d)
     temp = XCVR_TSM->TIMING50;
     temp &= ~(XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_HI_MASK |
               XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_MASK |
               XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_LO_MASK);
     temp |= tsm_timing50;
     XCVR_TSM->TIMING50 = temp;
#endif /* (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d) */
#if (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d)
     temp = XCVR_TSM->TIMING50;
     temp &= ~(XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_HI_MASK |
               XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_LO_MASK |
               XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_MASK |
               XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_LO_MASK);
     temp |= tsm_timing50;
     XCVR_TSM->TIMING50 = temp;
#endif /* (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d) */
 
#endif /* CPU_MKW37Axxx / CPU_MKW37Axxx/ CPU_MKW39Axxx */
 
     /* Save the updated registers values. */
     XCVR_CoexistenceSaveRestoreTimings(1U);
#endif /* gMWS_UseCoexistence_d */
     
     return gXcvrSuccess_c;
 }
 
 void XCVR_CoexistenceSaveRestoreTimings(uint8_t saveTimings)
 {
     /* Save and restore XCVR timing registers corrupted by the co-existence feature in the XCVR */
#if (gMWS_UseCoexistence_d || TEST_BUILD_COEX)
         static uint32_t tsm_ovrd0_saved = 0x00;
         static uint32_t tsm_ovrd1_saved = 0x00;
         static uint32_t tsm_ovrd2_saved = 0x00;
         static uint32_t tsm_ovrd3_saved = 0x00;
     
#if (defined(CPU_MKW37A512VFT4) || defined(CPU_MKW35Z512VHT4) || defined(CPU_MKW38A512VFT4) || \
               defined(CPU_MKW36A512VHT4) || defined(CPU_MKW39A512VFT4) || defined(CPU_MKW39Z512VFT4))
     
         static uint32_t tsm_timing49_saved = 0x00;
         static uint32_t tsm_timing50_saved = 0x00;
     
         if (saveTimings == 0U)
         {
             /* Restore registers values. */
             XCVR_TSM->OVRD0 = tsm_ovrd0_saved;
             XCVR_TSM->OVRD1 = tsm_ovrd1_saved;
             XCVR_TSM->OVRD2 = tsm_ovrd2_saved;
             XCVR_TSM->OVRD3 = tsm_ovrd3_saved;
     
             XCVR_TSM->TIMING49 = tsm_timing49_saved;
             XCVR_TSM->TIMING50 = tsm_timing50_saved;
         }
         else
         {
             /* Save registers values. */
             tsm_ovrd0_saved = XCVR_TSM->OVRD0;
             tsm_ovrd1_saved = XCVR_TSM->OVRD1;
             tsm_ovrd2_saved = XCVR_TSM->OVRD2;
             tsm_ovrd3_saved = XCVR_TSM->OVRD3;
             tsm_timing49_saved = XCVR_TSM->TIMING49;
             tsm_timing50_saved = XCVR_TSM->TIMING50;
         }
     
#endif /* CPU_MKW37Axxx / CPU_MKW37Axxx/ CPU_MKW39Axxx */
     
#endif /* gMWS_UseCoexistence_d */
}
 
 
/* ******************************************************************* */
/* ******************** PA/FEM control routines *************************** */
/* ******************************************************************* */
xcvrStatus_t XCVR_ExternalFadPaFemInit(xcvr_pa_fem_config_t * pa_fem_settings_ptr)
{
     xcvrStatus_t status = gXcvrSuccess_c; 
     uint32_t temp_reg = 0;
     uint8_t subfield;
     uint8_t wu_time;
     uint8_t wd_time;
     /* Initialize the external PA and FEM control feature */
     
     /* Perform any input verification checks needed and update status appropriately */
    if (pa_fem_settings_ptr == NULLPTR)
    {
        status = gXcvrInvalidParameters_c;
    }
    else
    {
        /* Check contents of the initialization structure */

        /* when FAD state machine is in use, ANT_A and ANT_B must be under FAD state machine control */
        if ((pa_fem_settings_ptr->use_fad_state_machine == 1U) && 
            ((pa_fem_settings_ptr->ant_a_pad_control == XCVR_FAD_TSM_GPIO) || 
            (pa_fem_settings_ptr->ant_b_pad_control == XCVR_FAD_TSM_GPIO)))
        {
            status = gXcvrInvalidConfiguration_c;
        }

        /* TX and RX warmdown times are restricted to be 0 in the current TSM design */
        if ((pa_fem_settings_ptr->pa_tx_wd != 0U) || (pa_fem_settings_ptr->lna_rx_wd != 0U))
        {
            status = gXcvrInvalidParameters_c;
        }

        /* All of the below parameters must be either 0 or 1 and are treated as booleans so OR them together and check for > 1 to validate them */
        if (((uint32_t)pa_fem_settings_ptr->op_mode | 
            (uint32_t)(pa_fem_settings_ptr->high_z_enable) | 
            (uint32_t)pa_fem_settings_ptr->ant_sel_pins_enable | 
            (uint32_t)pa_fem_settings_ptr->tx_rx_switch_pins_enable | 
            (uint32_t)pa_fem_settings_ptr->use_fad_state_machine | 
            (uint32_t)pa_fem_settings_ptr->ant_a_pad_control | 
            (uint32_t)pa_fem_settings_ptr->ant_b_pad_control | 
            (uint32_t)pa_fem_settings_ptr->tx_switch_pad_control | 
            (uint32_t)pa_fem_settings_ptr->rx_switch_pad_control |
            (uint32_t)pa_fem_settings_ptr->tx_switch_pol_control |
            (uint32_t)pa_fem_settings_ptr->rx_switch_pol_control) > 1U)
        {
            status = gXcvrInvalidParameters_c;
        }
    }

    /* If input verification checks have passed then perform the PA/FEM initialization */
    if (status == gXcvrSuccess_c)
    {
#if defined(RADIO_IS_GEN_4P0)
        temp_reg = XCVR_ANALOG_LDO_1_LDO_ANT_HIZ(pa_fem_settings_ptr->high_z_enable);      /* Set High Z if enabled */
#else
        temp_reg = XCVR_MISC_FAD_CTRL_ANTX_HZ(pa_fem_settings_ptr->high_z_enable);      /* Set High Z if enabled */
#endif
        temp_reg |= XCVR_MISC_FAD_CTRL_ANTX_CTRLMODE(pa_fem_settings_ptr->op_mode);     /* Set control mode */
        subfield = (uint8_t)(((uint32_t)pa_fem_settings_ptr->rx_switch_pol_control<<3) | ((uint32_t)pa_fem_settings_ptr->tx_switch_pol_control<<2)); /* Build field value for ANTX_POL */
        temp_reg |= XCVR_MISC_FAD_CTRL_ANTX_POL(subfield);
        subfield = (uint8_t)(((uint32_t)pa_fem_settings_ptr->rx_switch_pad_control<<3) |    /* Build field value for FAD_NOT_GPIO */
                    ((uint32_t)pa_fem_settings_ptr->tx_switch_pad_control<<2) |
                    ((uint32_t)pa_fem_settings_ptr->ant_b_pad_control<<1) |
                    ((uint32_t)pa_fem_settings_ptr->ant_a_pad_control<<0));
        temp_reg |= XCVR_MISC_FAD_CTRL_FAD_NOT_GPIO(subfield);
        subfield = (uint8_t)(((uint32_t)pa_fem_settings_ptr->ant_sel_pins_enable<<1) | ((uint32_t)pa_fem_settings_ptr->tx_rx_switch_pins_enable<<0));
        temp_reg |= XCVR_MISC_FAD_CTRL_ANTX_EN(subfield);
        XCVR_MISC->FAD_CTRL = temp_reg;

        /* Program TSM Timing registers for TSM_GPIO_2 (tx switch) and TSM_GPIO_3 (rx switch) */

        /* External PA needs to start before intnernal PA, which starts at TX_DIG_EN so apply advance relative to TX_DIG_EN */
#if defined(RADIO_IS_GEN_4P0)
        wu_time = (uint8_t)(((XCVR_TSM->TIMING14&XCVR_TSM_TIMING14_TX_DIG_EN_TX_HI_MASK)>>XCVR_TSM_TIMING14_TX_DIG_EN_TX_HI_SHIFT)
            - pa_fem_settings_ptr->pa_tx_wu);
#else
        wu_time = (uint8_t)(((XCVR_TSM->TIMING35&XCVR_TSM_TIMING35_TX_DIG_EN_TX_HI_MASK)>>XCVR_TSM_TIMING35_TX_DIG_EN_TX_HI_SHIFT)
            - pa_fem_settings_ptr->pa_tx_wu);
#endif
        wd_time = (uint8_t)(((XCVR_TSM->END_OF_SEQ&XCVR_TSM_END_OF_SEQ_END_OF_TX_WD_MASK)>>XCVR_TSM_END_OF_SEQ_END_OF_TX_WD_SHIFT)+pa_fem_settings_ptr->pa_tx_wd);
        /* The TX timing advance value must be strictly less than the difference between wd_time and wu_time due to TSM counter restrictions. */
        if (pa_fem_settings_ptr->pa_tx_wd < (wd_time-wu_time))
        {
            XCVR_TSM->TIMING49 = B3(0xFFU) | B2(0xFFU) | B1(wd_time) | B0(wu_time); /* TSM_GPIO_2 (tx switch) */
        }
        else
        {
            status = gXcvrInvalidConfiguration_c;
        }
        /* RX timing calculations */
        wu_time = (uint8_t)(((XCVR_TSM->END_OF_SEQ&XCVR_TSM_END_OF_SEQ_END_OF_RX_WU_MASK)>>XCVR_TSM_END_OF_SEQ_END_OF_RX_WU_SHIFT)-pa_fem_settings_ptr->lna_rx_wu);
        wd_time = (uint8_t)(((XCVR_TSM->END_OF_SEQ&XCVR_TSM_END_OF_SEQ_END_OF_RX_WD_MASK)>>XCVR_TSM_END_OF_SEQ_END_OF_RX_WD_SHIFT)+pa_fem_settings_ptr->lna_rx_wd);
        /* The RX timing advance value must be strictly less than the difference between wd_time and wu_time due to TSM counter restrictions. */
        if (pa_fem_settings_ptr->lna_rx_wd < (wd_time-wu_time))
        {
            XCVR_TSM->TIMING50 = B3(wd_time) | B2(wu_time) | B1(0xFFU) | B0(0xFFU); /* TSM_GPIO_3 (rx switch) */
        }
        else
        {
            status = gXcvrInvalidConfiguration_c;
        }
    }    

    return status;
              
}
 
xcvrStatus_t XCVR_ExternalFadPaFemDeInit(void)
{
     /* De-iInitialize the external PA and FEM control feature */
     XCVR_TSM->TIMING49 = 0xFFFFFFFFUL;
     XCVR_TSM->TIMING50 = 0xFFFFFFFFUL;
     XCVR_MISC->FAD_CTRL = 0x0UL;
     return gXcvrSuccess_c;
}
 
void XCVR_ExtAntOvrd(XCVR_FAD_OVRD_ANT_A_B_SEL_MODE_T antenna_sel)
{
     
     /* Force an external antenna selection in the FAD feature */
    uint32_t temp_reg = XCVR_MISC->FAD_CTRL;
    if (antenna_sel == XCVR_FAD_OVRD_SEL_ANT_A)
    {
        temp_reg &= ~XCVR_MISC_FAD_CTRL_ANTX_OVRD_MASK; /* Select Antenna A */
    }
    else
    {
        temp_reg |= XCVR_MISC_FAD_CTRL_ANTX_OVRD_MASK; /* Select Antenna B */
    }
    XCVR_MISC->FAD_CTRL = temp_reg | XCVR_MISC_FAD_CTRL_ANTX_OVRD_EN_MASK;
} 
 
 void XCVR_ExtAntRelease(void)
 {
     /* Release an external antenna selection in the FAD feature */
     XCVR_MISC->FAD_CTRL &= ~XCVR_MISC_FAD_CTRL_ANTX_OVRD_EN_MASK;
 }

xcvrStatus_t XCVR_FadPaFemOnCoexInit(xcvr_pa_fem_config_t * test_settings, tx_rx_coex_pin_func_t rf_status_func, tx_rx_coex_pin_func_t rf_priority_func)
{
    uint8_t tx_wu_time;
    uint8_t tx_wd_time;
    uint8_t rx_wu_time;
    uint8_t rx_wd_time;
    uint32_t temp;
    xcvrStatus_t status = gXcvrSuccess_c;

    /* Perform any input verification checks needed and update status appropriately */
    if (test_settings == NULLPTR)
    {
        status = gXcvrInvalidParameters_c;
    }
    else
    {
        /* TX and RX warmdown times are restricted to be 0 in the current TSM design */
        if ((test_settings->pa_tx_wd != 0U) || (test_settings->lna_rx_wd != 0U))
        {
            status = gXcvrInvalidParameters_c;
        }

        /* All of the below parameters must be either 0 or 1 and are treated as booleans so OR them together and check for > 1 to validate them */
        if (((uint32_t)test_settings->op_mode | 
            (uint32_t)(test_settings->high_z_enable) | 
            (uint32_t)test_settings->ant_sel_pins_enable | 
            (uint32_t)test_settings->tx_rx_switch_pins_enable | 
            (uint32_t)test_settings->use_fad_state_machine | 
            (uint32_t)test_settings->ant_a_pad_control | 
            (uint32_t)test_settings->ant_b_pad_control | 
            (uint32_t)test_settings->tx_switch_pad_control | 
            (uint32_t)test_settings->rx_switch_pad_control |
            (uint32_t)test_settings->tx_switch_pol_control |
            (uint32_t)test_settings->rx_switch_pol_control) > 1U)
        {
            status = gXcvrInvalidParameters_c;
        }
    }

    /* Input checks have all passed */
    if (status == gXcvrSuccess_c)
    {
#if defined(RADIO_IS_GEN_4P0)
        RFMC->RF2P4GHZ_COEXT &= ~(RFMC_RF2P4GHZ_COEXT_RFPRI_OBE_MASK | RFMC_RF2P4GHZ_COEXT_RFSTAT_OBE_MASK); /* Ensure both enable bits are cleared to start out. */
        tx_wu_time = (uint8_t)(((XCVR_TSM->TIMING14&XCVR_TSM_TIMING14_TX_DIG_EN_TX_HI_MASK)>>XCVR_TSM_TIMING14_TX_DIG_EN_TX_HI_SHIFT)
            - test_settings->pa_tx_wu);
#else
        RSIM->SW_CONFIG &= ~(RSIM_SW_CONFIG_IPP_OBE_RF_PRIORITY_MASK | RSIM_SW_CONFIG_IPP_OBE_RF_STATUS_MASK); /* Ensure both enable bits are cleared to start out. */
        /* External PA should start up before the internal PA */
        tx_wu_time = (uint8_t)(((XCVR_TSM->TIMING35&XCVR_TSM_TIMING35_TX_DIG_EN_TX_HI_MASK)>>XCVR_TSM_TIMING35_TX_DIG_EN_TX_HI_SHIFT)
            - test_settings->pa_tx_wu);
#endif
        tx_wd_time = (uint8_t)(((XCVR_TSM->END_OF_SEQ&XCVR_TSM_END_OF_SEQ_END_OF_TX_WD_MASK)>>XCVR_TSM_END_OF_SEQ_END_OF_TX_WD_SHIFT)+test_settings->pa_tx_wd);
        rx_wu_time = (uint8_t)(((XCVR_TSM->END_OF_SEQ&XCVR_TSM_END_OF_SEQ_END_OF_RX_WU_MASK)>>XCVR_TSM_END_OF_SEQ_END_OF_RX_WU_SHIFT)-test_settings->lna_rx_wu);
        rx_wd_time = (uint8_t)(((XCVR_TSM->END_OF_SEQ&XCVR_TSM_END_OF_SEQ_END_OF_RX_WD_MASK)>>XCVR_TSM_END_OF_SEQ_END_OF_RX_WD_SHIFT)+test_settings->lna_rx_wd);
        /* The TX & RX timing advance values must be strictly less than the difference between wd_time and wu_time due to TSM counter restrictions. */
        if ((test_settings->pa_tx_wd < (tx_wd_time-tx_wu_time)) && (test_settings->lna_rx_wd < (rx_wd_time-rx_wu_time)))
        {
            status = gXcvrSuccess_c;
            if (rf_status_func != NO_FUNC)
            {
                switch (rf_status_func)
                {
                    case TX_FUNC:
                        temp = B3(0xFFU) | B2(0xFFU) | B1(tx_wd_time) | B0(tx_wu_time); /*  (tx switch) */
                        break;
                    case RX_FUNC:
                        temp = B3(rx_wd_time) | B2(rx_wu_time) | B1(0xFFU) | B0(0xFFU); /*  (rx switch) */
                        break;
                    case BOTH_TX_RX_FUNC:
                        temp = B3(rx_wd_time) | B2(rx_wu_time) | B1(tx_wd_time) | B0(tx_wu_time); /*  (both tx switch and rx switch together) */
                        break;
                    default:
                        temp = B3(0xFFU) | B2(0xFFU) | B1(0xFFU) | B0(0xFFU); /* default to no function in case of error */
                        status = gXcvrInvalidConfiguration_c;
                        break;
                }
#if defined(RADIO_IS_GEN_4P0)

#else
                /* setup RF_STATUS pin for TSM_SPARE2 output */
                RSIM->DSM_CONTROL &= ~(RSIM_SW_CONFIG_WIFI_COEXIST_2_MASK); /* Clear WIFI_COEXIST_2 bit to use TSM_SPARE2 on RF_STATUS pin */
                XCVR_TSM->TIMING45 = temp; /* set the timing */
                /* Enable the output drivers for RF_STATUS and RF_PRIORITY */
                RSIM->SW_CONFIG |= RSIM_SW_CONFIG_IPP_OBE_RF_STATUS_MASK;
#endif
            }
            if ((rf_priority_func != NO_FUNC) && (status == gXcvrSuccess_c))
            {
                switch (rf_priority_func)
                {
                    case TX_FUNC:
                        temp = B3(0xFFU) | B2(0xFFU) | B1(tx_wd_time) | B0(tx_wu_time); /*  (tx switch) */
                        break;
                    case RX_FUNC:
                        temp = B3(rx_wd_time) | B2(rx_wu_time) | B1(0xFFU) | B0(0xFFU); /*  (rx switch) */
                        break;
                    case BOTH_TX_RX_FUNC:
                        temp = B3(rx_wd_time) | B2(rx_wu_time) | B1(tx_wd_time) | B0(tx_wu_time); /*  (both tx switch and rx switch together) */
                        break;
                    default:
                        temp = B3(0xFFU) | B2(0xFFU) | B1(0xFFU) | B0(0xFFU); /* default to no function in case of error */
                        status = gXcvrInvalidConfiguration_c;
                        break;
                }
#if defined(RADIO_IS_GEN_4P0)

#else
                /* setup RF_STATUS pin for TSM_SPARE3 output */
                RSIM->DSM_CONTROL |= RSIM_SW_CONFIG_WIFI_COEXIST_3_MASK; /* Set WIFI_COEXIST_3 bit to use TSM_SPARE3 on RF_PRIORITY pin */
                XCVR_TSM->TIMING46 = temp; /* set the timing */
                /* Enable the output drivers for RF_STATUS and RF_PRIORITY */
                RSIM->SW_CONFIG |= RSIM_SW_CONFIG_IPP_OBE_RF_PRIORITY_MASK;
#endif
            }
        }
        else
        {
            status = gXcvrInvalidConfiguration_c;
        }
    }

    return status;
}

void XCVR_FadPaFemOnCoexDeInit(void)
{
    /* Disable the PA/FEM over coexistence feature */
     XCVR_TSM->TIMING45 = 0xFFFFFFFFUL;
     XCVR_TSM->TIMING46 = 0xFFFFFFFFUL;
#if defined(RADIO_IS_GEN_4P0)
     RFMC->RF2P4GHZ_COEXT &= ~(RFMC_RF2P4GHZ_COEXT_RFPRI_OBE_MASK | RFMC_RF2P4GHZ_COEXT_RFSTAT_OBE_MASK); /* Ensure both enable bits are cleared to start out. */
#else
     RSIM->SW_CONFIG &= ~(RSIM_SW_CONFIG_IPP_OBE_RF_PRIORITY_MASK | RSIM_SW_CONFIG_IPP_OBE_RF_STATUS_MASK); /* Ensure both enable bits are cleared to prevent usage. */
#endif
}



/* ************************************************************************** */
/* **************** Localization and antenna control routines *************** */
/* ************************************************************************** */
 
/*! *********************************************************************************
* \brief  Handles LCL module programming for AoA/AoD antenna switching
********************************************************************************** */

xcvrLclStatus_t XCVR_LCL_AntennaSwitchInit(lclRxTxMode_t mode, lclAntennaSwitchingPatternConfig_t *pConfig)
{ 
    uint32_t temp = 0U;
    xcvrLclStatus_t status = gXcvrLclStatusSuccess;
    
    /* Verify input parameters in configuration structure */
    if (pConfig == NULLPTR)
    {
        status = gXcvrLclStatusInvalidArgs;
    }
    else
    {
        uint8_t radio_is_b0 = 0U;
        uint8_t sample_per_interval;
        if (XCVR_ReadRadioVer() == (uint16_t)XCVR_RADIO_GEN_3P5_B0)
        {
            radio_is_b0 = 1U;
        }
        /* Check number of samples */
        if (radio_is_b0 == 0U) /* KW37 A0 revision */
        {
            assert(pConfig->samplesPerInterval < XCVR_LCL_SAMPLES_MAX_LEN); /* Error check for A0 version, max length */
            assert(pConfig->samplesPerInterval > 0U); /* Error check for A0 version, zero length interval is an error */
        }
        else
        {
            assert(pConfig->samplesPerInterval < (XCVR_LCL_SAMPLES_MAX_LEN+1U)); /* Error check for B0 version, max length+1 */
        }
        
        /* Check sample duration */
        assert((pConfig->lowPeriodDuration < XCVR_LCL_SAMPLES_MAX_LEN) &&
               (pConfig->highPeriodDuration < XCVR_LCL_SAMPLES_MAX_LEN));
        /* Check trigger configuration */
        assert((pConfig->txTrig < lclTxTriggerInvalid) &&
               (pConfig->rxTrig < lclRxTriggerInvalid));
        /* Check mode */    
        assert((mode == gLclTxMode) ||
               (mode == gLclRxMode));


        /* RX_SPINT and TX_SPINT meaning is different in A0 and B0 silicon. A0 is directly programmed, B0 gets inputvalue-1 */
        if (radio_is_b0 == 1U) /* KW37 B0 revision */
        {
            sample_per_interval = pConfig->samplesPerInterval-1U; /* B0 registers store the interval value less 1. API value is the interval */
        }
        else
        {
            sample_per_interval = pConfig->samplesPerInterval; /* A0 registers store the interval value. API value is the interval, therefore no change */
        }
        
        /* Configure specific LANT settings */
        temp = XCVR_MISC->LCL_CFG0;
        temp &= ~(XCVR_MISC_LCL_CFG0_CTE_DUR_MASK | 
                  XCVR_MISC_LCL_CFG0_LANT_INV_MASK | 
                  XCVR_MISC_LCL_CFG0_LANT_SW_WIGGLE_MASK |
                  XCVR_MISC_LCL_CFG0_LANT_BLOCK_RX_MASK |
                  XCVR_MISC_LCL_CFG0_LANT_BLOCK_TX_MASK);                        
        temp |= 
          XCVR_MISC_LCL_CFG0_CTE_DUR(pConfig->numberOfSwitchesInPattern) | /* set pattern duration */
          XCVR_MISC_LCL_CFG0_LANT_INV(pConfig->lantSwInvert) | /* Polarity of lant_sw signal */
          XCVR_MISC_LCL_CFG0_LANT_SW_WIGGLE((uint8_t)pConfig->lantSwWiggle); /* Enables SW wiggle output on lant_sw signal */

        /* Register write */
        XCVR_MISC->LCL_CFG0 = temp;
       
        /* Configure switching delay, switching trigger and switching pattern for RX mode*/
        if ((mode == gLclRxMode) || (mode == gLclRxTxMode))
        {
            /* Configure trigger delay and offset */
            XCVR_MISC->LCL_RX_CFG0 = 
              XCVR_MISC_LCL_RX_CFG0_RX_DELAY(pConfig->triggerDelay) |
              XCVR_MISC_LCL_RX_CFG0_RX_DELAY_OFF(pConfig->triggerOffset);
            
            /* Configure trigger RX settings */
            XCVR_MISC->LCL_RX_CFG1 = 
              XCVR_MISC_LCL_RX_CFG1_RX_ANT_TRIG_SEL((uint8_t)pConfig->rxTrig) |
              XCVR_MISC_LCL_RX_CFG1_RX_SPINT(sample_per_interval) |
              XCVR_MISC_LCL_RX_CFG1_RX_HI_PER(pConfig->highPeriodDuration) |
              XCVR_MISC_LCL_RX_CFG1_RX_LO_PER(pConfig->lowPeriodDuration);
        }

        /* Configure switching delay, switching trigger and switching pattern for TX mode*/
        if ((mode == gLclTxMode) || (mode == gLclRxTxMode))
        {
            /* Configure trigger delay and offset */
            XCVR_MISC->LCL_TX_CFG0 = 
              XCVR_MISC_LCL_TX_CFG0_TX_DELAY(pConfig->triggerDelay) |
              XCVR_MISC_LCL_TX_CFG0_TX_DELAY_OFF(pConfig->triggerOffset);
            
            /* Configure trigger TX settings */
            XCVR_MISC->LCL_TX_CFG1 = 
              XCVR_MISC_LCL_TX_CFG1_TX_ANT_TRIG_SEL((uint8_t)pConfig->txTrig) |
              XCVR_MISC_LCL_TX_CFG1_TX_SPINT(sample_per_interval) |
              XCVR_MISC_LCL_TX_CFG1_TX_HI_PER(pConfig->highPeriodDuration) |
              XCVR_MISC_LCL_TX_CFG1_TX_LO_PER(pConfig->lowPeriodDuration);
        }

    }
    
    return status;
}
         

xcvrLclStatus_t XCVR_LCL_AntennaSwitchRead(lclRxTxMode_t mode, lclAntennaSwitchingPatternConfig_t *pConfig)
{
    xcvrLclStatus_t status;
    uint8_t temp = 0U;
  
    /* Verify input parameters in configuration structure */
    if (pConfig == NULLPTR)
    {
        status = gXcvrLclStatusInvalidArgs;
    }
    else
    {
        uint8_t radio_is_b0 = 0U;
        if (XCVR_ReadRadioVer() == (uint16_t)XCVR_RADIO_GEN_3P5_B0)
        {
            radio_is_b0 = 1U;
        }
        /* Read pattern duration */
        pConfig->numberOfSwitchesInPattern = (uint16_t)((XCVR_MISC->LCL_CFG0 & XCVR_MISC_LCL_CFG0_CTE_DUR_MASK ) >> 
          XCVR_MISC_LCL_CFG0_CTE_DUR_SHIFT);
        /* Read invert configuration */
        pConfig->lantSwInvert = (bool)(XCVR_MISC->LCL_CFG0 & XCVR_MISC_LCL_CFG0_LANT_INV_MASK);
        /* Read wiggle configuration */      
        pConfig->lantSwWiggle = (bool)(XCVR_MISC->LCL_CFG0 & XCVR_MISC_LCL_CFG0_LANT_SW_WIGGLE_MASK);
           
        /* Read switching settings for RX mode */
        if (mode == gLclRxMode)
        {
            /* Read RX trigger delay */ 
            pConfig->triggerDelay = (uint16_t)((XCVR_MISC->LCL_RX_CFG0 & XCVR_MISC_LCL_RX_CFG0_RX_DELAY_MASK) >>
              XCVR_MISC_LCL_RX_CFG0_RX_DELAY_SHIFT);
            /* Read RX trigger offset */ 
            pConfig->triggerOffset = (uint8_t)((XCVR_MISC->LCL_RX_CFG0 & XCVR_MISC_LCL_RX_CFG0_RX_DELAY_OFF_MASK) >>
              XCVR_MISC_LCL_RX_CFG0_RX_DELAY_OFF_SHIFT);
            
            /* Read trigger settings */
            temp = (uint8_t)((XCVR_MISC->LCL_RX_CFG1 & XCVR_MISC_LCL_RX_CFG1_RX_ANT_TRIG_SEL_MASK) >>
              XCVR_MISC_LCL_RX_CFG1_RX_ANT_TRIG_SEL_SHIFT);
            switch (temp)
            {
                case 0U: pConfig->rxTrig = lclRxTriggerSoftware; break;
                case 1U: pConfig->rxTrig = lclRxTriggerPatternFound; break;
                case 2U: pConfig->rxTrig = lclRxTriggerCrcComplete; break;
                case 3U: pConfig->rxTrig = lclRxTriggerCrcPass; break;
                case 4U: pConfig->rxTrig = lclRxTriggerCtePresent; break;
                case 5U: pConfig->rxTrig = lclRxTriggerAccessAddressFound; break;
                default: pConfig->rxTrig = lclRxTriggerInvalid; break;
            }
            
            /* Read number of samples per interval */
            pConfig->samplesPerInterval = (uint8_t)((XCVR_MISC->LCL_RX_CFG1 & XCVR_MISC_LCL_RX_CFG1_RX_SPINT_MASK) >>
              XCVR_MISC_LCL_RX_CFG1_RX_SPINT_SHIFT);
            /* Read high and low period durations */
            pConfig->highPeriodDuration = (uint8_t)((XCVR_MISC->LCL_RX_CFG1 & XCVR_MISC_LCL_RX_CFG1_RX_HI_PER_MASK) >>
              XCVR_MISC_LCL_RX_CFG1_RX_HI_PER_SHIFT);
            pConfig->lowPeriodDuration = (uint8_t)((XCVR_MISC->LCL_RX_CFG1 & XCVR_MISC_LCL_RX_CFG1_RX_LO_PER_MASK) >>
              XCVR_MISC_LCL_RX_CFG1_RX_LO_PER_SHIFT);
         }
        
        /* Read switching settings for TX mode */
        if (mode == gLclTxMode)
        {
            /* Read TX trigger delay */ 
            pConfig->triggerDelay = (uint16_t)((XCVR_MISC->LCL_TX_CFG0 & XCVR_MISC_LCL_TX_CFG0_TX_DELAY_MASK) >>
              XCVR_MISC_LCL_TX_CFG0_TX_DELAY_SHIFT);
            /* Read TX trigger offset */ 
            pConfig->triggerOffset = (uint8_t)((XCVR_MISC->LCL_TX_CFG0 & XCVR_MISC_LCL_TX_CFG0_TX_DELAY_OFF_MASK) >>
              XCVR_MISC_LCL_TX_CFG0_TX_DELAY_OFF_SHIFT);
            
            /* Read trigger settings */
            temp = (uint8_t)(((uint32_t)XCVR_MISC->LCL_TX_CFG1 & XCVR_MISC_LCL_TX_CFG1_TX_ANT_TRIG_SEL_MASK) >>
              XCVR_MISC_LCL_TX_CFG1_TX_ANT_TRIG_SEL_SHIFT);
            switch (temp)
            {
                case 0U: pConfig->txTrig = lclTxTriggerSoftware; break;
                case 1U: pConfig->txTrig = lclTxTriggerPatternFound; break;
                case 2U: pConfig->txTrig = lclTxTriggerCrcComplete; break;
                case 3U: pConfig->txTrig = lclTxTriggerPaWuComplete; break;
                case 4U: pConfig->txTrig = lclTxTriggerRbmeTxDonePre; break;
                default: pConfig->txTrig = lclTxTriggerInvalid; break;
            }
           
            /* Read number of samples per interval */
            pConfig->samplesPerInterval = (uint8_t)((XCVR_MISC->LCL_TX_CFG1 & XCVR_MISC_LCL_TX_CFG1_TX_SPINT_MASK) >>
              XCVR_MISC_LCL_TX_CFG1_TX_SPINT_SHIFT);
            /* Read high and low period durations */
            pConfig->highPeriodDuration = (uint8_t)((XCVR_MISC->LCL_TX_CFG1 & XCVR_MISC_LCL_TX_CFG1_TX_HI_PER_MASK) >>
              XCVR_MISC_LCL_TX_CFG1_TX_HI_PER_SHIFT);
            pConfig->lowPeriodDuration = (uint8_t)((XCVR_MISC->LCL_TX_CFG1 & XCVR_MISC_LCL_TX_CFG1_TX_LO_PER_MASK) >>
              XCVR_MISC_LCL_TX_CFG1_TX_LO_PER_SHIFT);
        }

        /* RX_SPINT and TX_SPINT meaning is different in A0 and B0 silicon. A0 is directly programmed, B0 stores the interval-1 */
        if (radio_is_b0 == 1U) /* KW37 B0 revision */
        {
            pConfig->samplesPerInterval += 1U;
        }

        /* Check mode */    
        if ((mode != gLclTxMode) && (mode != gLclRxMode)) /* if neither of these is set then above code was skipped and result is invalid */
        {
            status = gXcvrLclStatusInvalidArgs;
        }
        else
        {
            status = gXcvrLclStatusSuccess;
        }
    }
    
    return status;
}
              
/*! *********************************************************************************
* \brief  Enables LCL module for AoA/AoD antenna switching
********************************************************************************** */

xcvrLclStatus_t XCVR_LCL_AntennaSwitchEn(bool enTXmode, bool enRXmode)
{
    uint32_t temp = XCVR_MISC->LCL_CFG0;
    
    /* Disable TX/RX module */    
    temp &= ~(XCVR_MISC_LCL_CFG0_RX_LCL_EN_MASK |
                XCVR_MISC_LCL_CFG0_TX_LCL_EN_MASK | 
                XCVR_MISC_LCL_CFG0_LCL_EN_MASK);
    
    if (enTXmode || enRXmode)
    {
        /* Enable TX/RX module */
        temp |= XCVR_MISC_LCL_CFG0_RX_LCL_EN((uint8_t)enRXmode) |
                XCVR_MISC_LCL_CFG0_TX_LCL_EN((uint8_t)enTXmode) |
                XCVR_MISC_LCL_CFG0_LCL_EN(1U);
    }
    
    /* Write TX/RX configuration */   
    XCVR_MISC->LCL_CFG0 = temp;    

    return gXcvrLclStatusSuccess;
}

/*! *********************************************************************************
* \brief  Blocks Tx/RX in LCL module for AoA/AoD antenna switching
********************************************************************************** */

xcvrLclStatus_t XCVR_LCL_AntennaSwitchBlock(bool blockTXmode, bool blockRXmode)
{
    uint32_t temp = XCVR_MISC->LCL_CFG0;
    
    /* Disable blocking on TX/RX module */    
    temp &= ~(XCVR_MISC_LCL_CFG0_LANT_BLOCK_RX_MASK |
                XCVR_MISC_LCL_CFG0_LANT_BLOCK_TX_MASK);
    
    if (blockTXmode || blockRXmode)
    {
        /* Block TX/RX module */
        temp |= XCVR_MISC_LCL_CFG0_LANT_BLOCK_RX((uint8_t)blockRXmode) |
                XCVR_MISC_LCL_CFG0_LANT_BLOCK_TX((uint8_t)blockTXmode);
    }
    
    /* Write TX/RX configuration */   
    XCVR_MISC->LCL_CFG0 = temp;    

    return gXcvrLclStatusSuccess;
}
              
/*! *********************************************************************************
* \brief  Reads current LCL antenna switching mode, RX TX or both
********************************************************************************** */

xcvrLclStatus_t XCVR_LCL_AntennaSwitchModeRead(lclRxTxMode_t *mode)
{
    xcvrLclStatus_t status;  
    uint8_t rxEn=0U, txEn=0U;
  
    /* Verify input parameters in configuration structure */
    if (mode == NULLPTR)
    {
        status = gXcvrLclStatusInvalidArgs;
    }
    else

    {
        /* Read block TX configuration */      
        rxEn = (uint8_t)((XCVR_MISC->LCL_CFG0 & XCVR_MISC_LCL_CFG0_RX_LCL_EN_MASK) >> 
          XCVR_MISC_LCL_CFG0_RX_LCL_EN_SHIFT);
        /* Read block RX configuration */      
        txEn = (uint8_t)((XCVR_MISC->LCL_CFG0 & XCVR_MISC_LCL_CFG0_TX_LCL_EN_MASK) >>
          XCVR_MISC_LCL_CFG0_TX_LCL_EN_SHIFT);
        
        /* Return TX RX status*/
        switch ((rxEn << 1U) | txEn)
        {
            case 0x01U: *mode = gLclTxMode; break;
            case 0x02U: *mode = gLclRxMode; break;
            case 0x03U: *mode = gLclRxTxMode; break; 
            default: *mode = gLclRxTxNone; break;
        }
        status = gXcvrLclStatusSuccess;
    }
    
    return status;
}

/*! *********************************************************************************
* \brief  Handles LCL module programming for pattern matching
********************************************************************************** */
xcvrLclStatus_t XCVR_LCL_PatternMatchInit(lclRxTxMode_t mode, lclPatternMatchConfig_t *pConfig)
{
    uint32_t temp = 0U, temp_lsb = 0U, temp_msb = 0U;
    xcvrLclStatus_t status;  
    /* Verify input parameters in configuration structure */
    if (pConfig == NULLPTR)
    {
        status = gXcvrLclStatusInvalidArgs;
    }
    else
    {
        /* Verify number of bytes is valid */
        assert(pConfig->numBytes < lclPatternMatchNumBytesInvalid);
        /* Check mode */    
        assert((mode > gLclRxTxNone) &&
               (mode < gLclRxTxMode));

        lcl_patt_match_mode = gLclRxTxNone;
        
        /* Configure Pattern match */
        temp = XCVR_MISC->LCL_CFG0;
        temp &= ~XCVR_MISC_LCL_CFG0_PM_NUM_BYTES_MASK;
        temp |= XCVR_MISC_LCL_CFG0_PM_NUM_BYTES(pConfig->numBytes);
        XCVR_MISC->LCL_CFG0 = temp;
        
        /* Fill LSB and MSB pattern registers */ 
        for(uint8_t i=0U; i<4U; i++)
        {
            temp_lsb <<= 8U;
            temp_lsb |= (uint32_t)(pConfig->pattern[3U-i]);
            temp_msb <<= 8U;
            temp_msb |= (uint32_t)(pConfig->pattern[7U-i]);
        }
        XCVR_MISC->LCL_PM_LSB = temp_lsb;
        XCVR_MISC->LCL_PM_MSB = temp_msb;
             
        status =  gXcvrLclStatusSuccess;
    }

    return status;
}
             
/*! *********************************************************************************
* \brief  Enables LCL module programming for pattern matching
********************************************************************************** */
xcvrLclStatus_t XCVR_LCL_PatternMatchEn(lclRxTxMode_t mode)
{
    uint32_t temp = 0U;
    
    /* Check mode */    
    assert((mode > gLclRxTxNone) &&
           (mode < gLclRxTxInvalid));
    
    
    /* Configure Pattern match */
    temp = XCVR_MISC->LCL_CFG0;

    /* Ensure everything is clear to start */
    temp &= ~(XCVR_MISC_LCL_CFG0_COMP_TX_EN_MASK |
                    XCVR_MISC_LCL_CFG0_COMP_EN_MASK); 
    
    switch(mode)
    {
        case gLclRxTxMode:
            {
                lcl_patt_match_mode = mode;
                temp |= XCVR_MISC_LCL_CFG0_COMP_TX_EN_MASK |
                        XCVR_MISC_LCL_CFG0_COMP_EN_MASK;
            } break;
        case gLclTxMode:
        {
            if (lcl_patt_match_mode == gLclRxMode)
            {
                lcl_patt_match_mode = gLclRxTxMode;
            }
            else
            {
                lcl_patt_match_mode = gLclTxMode;
            }
            temp |= XCVR_MISC_LCL_CFG0_COMP_TX_EN_MASK |
                    XCVR_MISC_LCL_CFG0_COMP_EN_MASK;
        } break;
        case gLclRxMode:
        {
            if (lcl_patt_match_mode == gLclTxMode)
            {
                lcl_patt_match_mode = gLclRxTxMode;
            }
            else
            {
                lcl_patt_match_mode = gLclRxMode;
            }
            temp |= XCVR_MISC_LCL_CFG0_COMP_EN_MASK;
        } break;
        default:
            temp |= 0U; /* need default clause to be non-empty */
            break;          
    }
    
    // Enable block
    temp |= XCVR_MISC_LCL_CFG0_LCL_EN_MASK;
    XCVR_MISC->LCL_CFG0 = temp;

    return gXcvrLclStatusSuccess;
}

/*! *********************************************************************************
* \brief  Handles LCL module programming for pattern matching
********************************************************************************** */
xcvrLclStatus_t XCVR_LCL_PatternMatchModeRead(lclRxTxMode_t *mode)
{
    xcvrLclStatus_t status;  
    /* Check pointer */    
    if (mode == NULLPTR)
    {
        status = gXcvrLclStatusInvalidArgs;
    }
    else
    {
        /* Must use a static global because the mode can't be derived from COMP_EN + COMP_TX_EN */
        *mode = lcl_patt_match_mode;
        status = gXcvrLclStatusSuccess;
    }
    
    return status;
}

/*! *********************************************************************************
* \brief  Handles LCL module programming for pattern matching
********************************************************************************** */
xcvrLclStatus_t XCVR_LCL_PatternMatchRead( lclPatternMatchConfig_t *pConfig )
{
    xcvrLclStatus_t status = gXcvrLclStatusSuccess;
    uint32_t temp_pattern;
    
    /* Verify input parameters in configuration structure */
    if (pConfig == NULLPTR)
    {
        status = gXcvrLclStatusInvalidArgs;
    }
    else
    {
        /* Get Number of Bytes*/
        uint32_t numBytesLcl = (XCVR_MISC->LCL_CFG0 & XCVR_MISC_LCL_CFG0_PM_NUM_BYTES_MASK) >>
        XCVR_MISC_LCL_CFG0_PM_NUM_BYTES_SHIFT;

        pConfig->numBytes = (lclPatternMatchNumBytes_t) numBytesLcl;

        /* Get pattern LSB */
        temp_pattern = XCVR_MISC->LCL_PM_LSB;      
        pConfig->pattern[0] = (uint8_t)(temp_pattern & 0xFFU);
        pConfig->pattern[1] = (uint8_t)((temp_pattern >> 8U) & 0xFFU);
        pConfig->pattern[2] = (uint8_t)((temp_pattern >> 16U) & 0xFFU);
        pConfig->pattern[3] = (uint8_t)((temp_pattern >> 24U) & 0xFFU);

        /* Get pattern MSB */
        temp_pattern = XCVR_MISC->LCL_PM_MSB;      
        pConfig->pattern[4] = (uint8_t)(temp_pattern & 0xFFU);
        pConfig->pattern[5] = (uint8_t)((temp_pattern >> 8U) & 0xFFU);
        pConfig->pattern[6] = (uint8_t)((temp_pattern >> 16U) & 0xFFU);
        pConfig->pattern[7] = (uint8_t)((temp_pattern >> 24U) & 0xFFU);
    }
    
    return status;
}

/*! *********************************************************************************
* \brief  Disable pattern matching in LCL
********************************************************************************** */
xcvrLclStatus_t XCVR_LCL_PatternMatchDisable(lclRxTxMode_t mode)
{ 
    uint32_t temp = 0U;
    
    /* Check mode */    
    assert((mode > gLclRxTxNone) &&
           (mode < gLclRxTxInvalid));
    
    /* Configure Pattern match */
    temp = XCVR_MISC->LCL_CFG0;

    switch(mode)
    {
        case gLclRxTxMode:
        {
            temp &= ~(XCVR_MISC_LCL_CFG0_COMP_TX_EN_MASK |
                      XCVR_MISC_LCL_CFG0_COMP_EN_MASK);
            lcl_patt_match_mode = gLclRxTxNone; 
        } break;
        case gLclTxMode:
        {
            temp &= ~XCVR_MISC_LCL_CFG0_COMP_TX_EN_MASK;
            /* Only disable the specified part of the pattern match mode */
            if (lcl_patt_match_mode == gLclRxTxMode)
            {
                lcl_patt_match_mode = gLclRxMode;
            }
            else
            {
                if (lcl_patt_match_mode == gLclTxMode)
                {
                    lcl_patt_match_mode = gLclRxTxNone;
                }
            }
        } break;
        case gLclRxMode:
        {
            temp &= ~XCVR_MISC_LCL_CFG0_COMP_EN_MASK;
            /* Only disable the specified part of the pattern match mode */
            if (lcl_patt_match_mode == gLclRxTxMode)
            {
                lcl_patt_match_mode = gLclTxMode;
            }
            else
            {
                if (lcl_patt_match_mode == gLclRxMode)
                {
                    lcl_patt_match_mode = gLclRxTxNone;
                }
            }
        } break;
        default:
            temp |= 0U; /* need default clause to be non-empty */
            break;          
    }
    // Write new config
    XCVR_MISC->LCL_CFG0 = temp;

    return gXcvrLclStatusSuccess;
}

/*! *********************************************************************************
* \brief  Force a manual trigger to LCL
********************************************************************************** */
void XCVR_LCL_SwTriggerAssert(void)
{
    lclRxTxMode_t mode = gLclRxTxNone;
    xcvrLclStatus_t status = gXcvrLclStatusFail;
    
    /* Read current mode */
    status = XCVR_LCL_AntennaSwitchModeRead(&mode);
    (void)status;

    /* Clear the SW_TRIG bit in LCL_CFG0 */
    XCVR_MISC->LCL_CFG0 &= ~XCVR_MISC_LCL_CFG0_SW_TRIG_MASK;

    /* Rearm module for multiple triggers - TX */
    if ((mode == gLclTxMode) || (mode == gLclRxTxMode))
    {
        /* Disable TX */
        XCVR_MISC->LCL_CFG0 &= ~XCVR_MISC_LCL_CFG0_TX_LCL_EN_MASK;
        /* Enable TX */
        XCVR_MISC->LCL_CFG0 |= XCVR_MISC_LCL_CFG0_TX_LCL_EN_MASK;
    }
    /* Rearm module for multiple triggers - RX */
    if ((mode == gLclRxMode) || (mode == gLclRxTxMode))
    {
        /* Disable RX */
        XCVR_MISC->LCL_CFG0 &= XCVR_MISC_LCL_CFG0_RX_LCL_EN_MASK;
        /* Enable RX */
        XCVR_MISC->LCL_CFG0 |= XCVR_MISC_LCL_CFG0_RX_LCL_EN_MASK;
    }

    /* Assert the SW_TRIG bit in LCL_CFG0 */
    XCVR_MISC->LCL_CFG0 |=  XCVR_MISC_LCL_CFG0_SW_TRIG_MASK;
}

/*! *********************************************************************************
* \brief  Disable LCL module, both antenna switch and pattern match
********************************************************************************** */
void XCVR_LCL_DeInit(void)
{   
    /* Clear all individual LCL_EN enable bit */
    XCVR_MISC->LCL_CFG0 &= ~XCVR_MISC_LCL_CFG0_LCL_EN_MASK;
}

/*! *********************************************************************************
* \brief  Gets LCL module enable status
********************************************************************************** */
bool XCVR_LCL_GetEn(void)
{   
    /* Clear all individual TX and RX enables and the module enable bit */
    return ( (bool) NXP_FLD2VAL( XCVR_MISC_LCL_CFG0_LCL_EN,  XCVR_MISC->LCL_CFG0 ));
}
