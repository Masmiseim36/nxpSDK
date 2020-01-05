/*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <string.h>
#include "nt_drv_tsi_driver.h"
#include "nt_module_tsi.h"
#if FSL_FEATURE_SOC_TSI_COUNT
/*******************************************************************************
 * Definitions
 ******************************************************************************/
extern IRQn_Type tsi_irq_ids[FSL_FEATURE_SOC_TSI_COUNT];
extern void TSI_DRV_IRQHandler0(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile tsi_lpwr_status_flags_t tsi_lpwr_status;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
*
* Function Name : NT_TSI_DRV_Init
* Description   : Encapsulates TSI v5 init.
*
*END**************************************************************************/
void NT_TSI_DRV_InitSpecific(TSI_Type *base, const tsi_config_t *config)
{
    TSI_InitSelfCapMode(base, &config->configSelfCap);
    TSI_EnableModule(base, true);
    #if (defined(FSL_FEATURE_TSI_HAS_M_TX_USED) && FSL_FEATURE_TSI_HAS_M_TX_USED)   
    /* Clear M_TX_USED bitfield, reset values are 1 values*/
    TSI_ClearUsedTxChannel(base, kTSI_MutualTxChannel_0);   
    TSI_ClearUsedTxChannel(base, kTSI_MutualTxChannel_1);   
    TSI_ClearUsedTxChannel(base, kTSI_MutualTxChannel_2);   
    TSI_ClearUsedTxChannel(base, kTSI_MutualTxChannel_3);   
    TSI_ClearUsedTxChannel(base, kTSI_MutualTxChannel_4);   
    TSI_ClearUsedTxChannel(base, kTSI_MutualTxChannel_5);    
    #endif      
    
}

/*FUNCTION**********************************************************************
*
* Function Name : NT_TSI_DRV_EnableElectrode
* Description   : Enables/Disables the electrode for measuring.
*
*END**************************************************************************/
tsi_status_t NT_TSI_DRV_EnableElectrode(uint32_t instance, const uint32_t channel, const bool enable)
{
    NT_ASSERT(instance < FSL_FEATURE_SOC_TSI_COUNT);
    NT_ASSERT(channel < TF_TSI_TOTAL_CHANNEL_COUNT);
    
    nt_tsi_state_t * tsiState = g_tsiStatePtr[instance];

    /* OS: Critical section. Access to global variable */
    if (knt_Status_OSA_Success != NT_OSA_MutexLock(&tsiState->lock, OSA_WAIT_FOREVER))
    {
        return kStatus_TSI_Error;
    }

    if (tsiState->status != kStatus_TSI_Initialized)
    {
      /* OS: End of critical section. */
        NT_OSA_MutexUnlock(&tsiState->lock);
        
        return tsiState->status;
    }

    /* Check the condition for low power mode. */
    if((tsiState->opMode == tsi_OpModeLowPower) || (tsiState->opMode == tsi_OpModeProximity))
    {
        tsiState->opModesData[tsiState->opMode].enabledElectrodes = 0;
    }

    if(enable)
    {
        tsiState->opModesData[tsiState->opMode].enabledElectrodes |= (1ULL << channel);
    }
    else
    {
        tsiState->opModesData[tsiState->opMode].enabledElectrodes &= ~(1ULL << channel);
    }
    
    /* Check self/mutual setting feasibility */
    if(channel > (TF_TSI_SELF_CAP_CHANNEL_COUNT-1U))
    {
        NT_ASSERT(!(tsiState->opModesData[tsiState->opMode].enabledElectrodes & (1ULL<<((channel-TF_TSI_SELF_CAP_CHANNEL_COUNT)/TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT))));
        NT_ASSERT(!(tsiState->opModesData[tsiState->opMode].enabledElectrodes & (1ULL<<(((channel-TF_TSI_SELF_CAP_CHANNEL_COUNT)%TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT)+TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT))));
        #if (defined(FSL_FEATURE_TSI_HAS_M_TX_USED) && FSL_FEATURE_TSI_HAS_M_TX_USED)  
        /* Set or clear M_TX_USED bitfield to make normal GPIO function for pins not used as TSI mutual TX pins */
        TSI_Type * base = g_tsiBase[instance];
        if(enable)
            TSI_SetUsedTxChannel(base, (tsi_mutual_tx_channel_t)((channel - TF_TSI_SELF_CAP_CHANNEL_COUNT) / TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT));
        else
            TSI_ClearUsedTxChannel(base, (tsi_mutual_tx_channel_t)((channel - TF_TSI_SELF_CAP_CHANNEL_COUNT) / TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT));    
        #endif  
    }

    /* OS: End of critical section. */
    NT_OSA_MutexUnlock(&tsiState->lock);

    return kStatus_TSI_Success;
}

/*FUNCTION**********************************************************************
*
* Function Name : NT_TSI_DRV_GetCounter
* Description   : Function returns the counter value of selected channel
*
*END**************************************************************************/
tsi_status_t NT_TSI_DRV_GetCounter(uint32_t instance, const uint32_t channel, uint16_t * counter)
{
    NT_ASSERT(instance < FSL_FEATURE_SOC_TSI_COUNT);
    NT_ASSERT(channel < TF_TSI_TOTAL_CHANNEL_COUNT);
    NT_ASSERT(counter);

    nt_tsi_state_t * tsiState = g_tsiStatePtr[instance];
      
    if(!((1ULL << channel) & (tsiState->opModesData[tsiState->opMode].enabledElectrodes)))   /* Check the channel number. */
    {
        return kStatus_TSI_InvalidChannel;
    }

    *counter = tsiState->counters[channel];

    return kStatus_TSI_Success;
}

/*FUNCTION**********************************************************************
*
* Function Name : NT_TSI_DRV_Measure
* Description   : This function gets (measure) capacitance of enabled electrodes
*               from the TSI module using a non-blocking method.
*
*END**************************************************************************/
tsi_status_t NT_TSI_DRV_Measure(uint32_t instance)
{
    NT_ASSERT(instance < FSL_FEATURE_SOC_TSI_COUNT);

    TSI_Type * base = g_tsiBase[instance];
    nt_tsi_state_t * tsiState = g_tsiStatePtr[instance];
    uint64_t    pen;
    uint32_t    first_pen;
    uint32_t    mutual_rx, mutual_tx;
    
    /* OS: Critical section. Access to global variable */
    if (knt_Status_OSA_Success != NT_OSA_MutexLock(&tsiState->lock, OSA_WAIT_FOREVER))
    {
        return kStatus_TSI_Error;
    }

    if (tsiState->status != kStatus_TSI_Initialized)
    {
        /* OS: End of critical section. */
        NT_OSA_MutexUnlock(&tsiState->lock);

        return tsiState->status;
    }

    if(!tsiState->opModesData[tsiState->opMode].enabledElectrodes)
    {
        /* OS: End of critical section. */
        NT_OSA_MutexUnlock(&tsiState->lock);

        return kStatus_TSI_InvalidChannel;
    }
    
    tsiState->status = kStatus_TSI_Busy;

    /* If there is a self-cap electrode used, initialize self-cap sensing*/
    if(tsiState->opModesData[tsiState->opMode].enabledElectrodes & 0x01FFFFFFU)
    {
        /* Find electrode with the lowest index */
        first_pen = 0U;
        pen = tsiState->opModesData[tsiState->opMode].enabledElectrodes;
        while (((pen >> first_pen) & 0x1U) == 0U) {
            first_pen++;
        }
    
        /* OS: End of critical section. */
        NT_OSA_MutexUnlock(&tsiState->lock);

        /* Init self-cap sensing */
        TSI_EnableModule(base, false);
        TSI_InitSelfCapMode(base, &tsiState->opModesData[tsiState->opMode].config.configSelfCap);
        TSI_SetSelfCapMeasuredChannel(base, first_pen);
        TSI_EnableHardwareTriggerScan(base, false);
        TSI_EnableModule(base, true);
        TSI_StartSoftwareTrigger(base);
    }
    /* If there are no self-cap electrodes used, initialize mutual-cap sensing*/
    else
    {
        /* Find electrode with the lowest index */
        first_pen = TF_TSI_SELF_CAP_CHANNEL_COUNT;
        pen = tsiState->opModesData[tsiState->opMode].enabledElectrodes;
        while (((pen >> first_pen) & 0x1U) == 0U) {
            first_pen++;
        }
        
        /*Parse electrode number into rx, tx components*/
        mutual_tx = (first_pen - TF_TSI_SELF_CAP_CHANNEL_COUNT) / TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT;
        mutual_rx = (first_pen - TF_TSI_SELF_CAP_CHANNEL_COUNT) % TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT;
        
        /* OS: End of critical section. */
        NT_OSA_MutexUnlock(&tsiState->lock);

        /* Init mutual-cap sensing */
        TSI_EnableModule(base, false);
        TSI_InitMutualCapMode(base, &tsiState->opModesData[tsiState->opMode].config.configMutual);
        TSI_SetMutualCapTxChannel(base, (tsi_mutual_tx_channel_t)mutual_tx);
        TSI_SetMutualCapRxChannel(base, (tsi_mutual_rx_channel_t)mutual_rx);
        TSI_EnableHardwareTriggerScan(base, false);
        TSI_EnableModule(base, true);
        TSI_StartSoftwareTrigger(base);
    }
    
    return kStatus_TSI_Success;
}

/*FUNCTION**********************************************************************
*
* Function Name : NT_TSI_DRV_EnableLowPower
* Description   : Enables/Disables the low power module.
*
*END**************************************************************************/
tsi_status_t NT_TSI_DRV_EnableLowPower(uint32_t instance)
{
    NT_ASSERT(instance < FSL_FEATURE_SOC_TSI_COUNT);

    TSI_Type * base = g_tsiBase[instance];
    nt_tsi_state_t * tsiState = g_tsiStatePtr[instance];
    tsi_status_t status;

    /* OS: Critical section. Access to global variable */
    if (knt_Status_OSA_Success != NT_OSA_MutexLock(&tsiState->lock, OSA_WAIT_FOREVER))
    {
        return kStatus_TSI_Error;
    }
        
    if ((status = NT_TSI_DRV_ChangeMode(instance, tsi_OpModeLowPower)) != kStatus_TSI_Success)
    {
        /* OS: End of critical section. */
        NT_OSA_MutexUnlock(&tsiState->lock);

        return status;
    }

    if(tsiState->opModesData[tsiState->opMode].enabledElectrodes == 0)
    {
      /* OS: End of critical section. */
        NT_OSA_MutexUnlock(&tsiState->lock);

        return kStatus_TSI_InvalidChannel;
    }
    
    tsiState->status = kStatus_TSI_LowPower;
    
    /* Enable TSI to run in STOP and VLPS mode */
    TSI_EnableModule(base, false);
    TSI_EnableLowPower(base, true);
    TSI_EnableModule(base, true);
    
    /* OS: End of critical section. */
    NT_OSA_MutexUnlock(&tsiState->lock);

    return kStatus_TSI_Success;
}


/*FUNCTION**********************************************************************
*
* Function Name : NT_TSI_DRV_ChangeMode
* Description   : The function change the current mode.
*
*END**************************************************************************/
tsi_status_t NT_TSI_DRV_ChangeMode(uint32_t instance, const nt_tsi_modes_t mode)
{
    NT_ASSERT(instance < FSL_FEATURE_SOC_TSI_COUNT);

    TSI_Type * base = g_tsiBase[instance];
    nt_tsi_state_t * tsiState = g_tsiStatePtr[instance];

    if((mode == tsiState->opMode) || (mode == tsi_OpModeNoChange))
    {
        return  kStatus_TSI_Success;
    }

    if(mode >= tsi_OpModeCnt)
    {
        return kStatus_TSI_InvalidMode;
    }

    /* OS: Critical section. Access to global variable */
    if (knt_Status_OSA_Success != NT_OSA_MutexLock(&tsiState->lockChangeMode, OSA_WAIT_FOREVER))
    {
        return kStatus_TSI_Error;
    }

    if (tsiState->status != kStatus_TSI_Initialized)
    {
        /* OS: End of critical section. */
        NT_OSA_MutexUnlock(&tsiState->lockChangeMode);
        
        return tsiState->status;
    }
        
    tsiState->opMode = mode;

    NT_TSI_DRV_InitSpecific(base, &tsiState->opModesData[mode].config);

    /* OS: End of critical section. */
    NT_OSA_MutexUnlock(&tsiState->lockChangeMode);

    return  kStatus_TSI_Success;
}


/*FUNCTION**********************************************************************
*
* Function Name : NT_TSI_DRV_LoadConfiguration
* Description   : The function load the configuration for one mode of operation.
*
*END**************************************************************************/
tsi_status_t NT_TSI_DRV_LoadConfiguration(uint32_t instance, const nt_tsi_modes_t mode, const nt_tsi_operation_mode_t * operationMode)
{
    NT_ASSERT(instance < FSL_FEATURE_SOC_TSI_COUNT);
    NT_ASSERT(operationMode);
    TSI_Type * base;
    nt_tsi_state_t * tsiState = g_tsiStatePtr[instance];
    
    if(mode >= tsi_OpModeCnt)
    {
        return kStatus_TSI_InvalidMode;
    }

    /* OS: Critical section. Access to global variable */
    if (knt_Status_OSA_Success != NT_OSA_MutexLock(&tsiState->lock, OSA_WAIT_FOREVER))
    {
        return kStatus_TSI_Error;
    }

    tsiState->opModesData[mode] = *operationMode;

    /* In case that the loaded configuration is active one, update the HW also. */
    if(mode == tsiState->opMode)
    {
        base = g_tsiBase[instance];

        NT_TSI_DRV_InitSpecific(base, &tsiState->opModesData[mode].config);
        TSI_EnableInterrupts(base, kTSI_GlobalInterruptEnable);
        TSI_EnableInterrupts(base, kTSI_EndOfScanInterruptEnable);
    }

    /* OS: End of critical section. */
    NT_OSA_MutexUnlock(&tsiState->lock);

    return  kStatus_TSI_Success;
}

/*!
* @brief Get Sinc status flags
* This function get tsi Sinc status flags.
*
* @param    base  TSI peripheral base address.
* @return         The mask of these status flags combination.
*/
static inline uint32_t TSI_GetSincFlags(TSI_Type *base)
{
    return (base->SINC & (kTSI_EndOfScanFlag | (uint32_t)kTSI_OutOfRangeFlag));
}

/*!
 * @brief Interrupt handler for TSI.
 * This handler uses the tsi State structure to handle the instance depend data.
 * This is not a public API as it is called whenever an interrupt occurs.
 */
void TSI_DRV_IRQHandler(uint32_t instance)
{
    TSI_Type * base = g_tsiBase[instance];
    nt_tsi_state_t * tsiState = g_tsiStatePtr[instance];
    uint64_t channels = tsiState->opModesData[tsiState->opMode].enabledElectrodes;
    uint64_t next_pen, pen;
    uint32_t curr_channel;

    if(TSI_GetSincFlags(base) & kTSI_SincOverflowFlag)
    {
      tsiState->status = kStatus_TSI_Overflow;
    }
    
    if(tsi_lpwr_status.TSILowPower)
    {
      tsi_lpwr_status.SelfLowPowerCountBuff =  TSI_GetCounter(base);
      tsi_lpwr_status.SelfLowPowerChannelBuff = TSI_GetSelfCapMeasuredChannel(base);
      tsi_lpwr_status.TSILowPower = 0;
    }
    
    /* Clear interrupt flags. */
    TSI_ClearStatusFlags(base, (uint32_t)kTSI_OutOfRangeFlag);
    TSI_ClearStatusFlags(base, kTSI_EndOfScanFlag);
    
    /* self-cap electrode was measured */
    if(!(TSI_GetSensingMode(base)))
    {
        curr_channel = TSI_GetSelfCapMeasuredChannel(base);
        
        /* Save TSICNT value */
        if((uint64_t)(1 << curr_channel) & channels)
        {
            tsiState->counters[curr_channel] = ((uint16_t)0xFFFF - TSI_GetCounter(base));
        }
        
        /* Find next enabled electrode */
        next_pen = curr_channel + 1;
        pen = channels;
        while (((((pen >> next_pen) & 0x1U)) == 0U) && (next_pen < (TF_TSI_TOTAL_CHANNEL_COUNT))) 
        {
            next_pen++;
        }
        
        /* If the next electrode is self-cap, get ready for next self-cap measurement */
        if(next_pen < TF_TSI_SELF_CAP_CHANNEL_COUNT)
        {
            /* Measurement must continue on next channel. */
            TSI_SetSelfCapMeasuredChannel(base, next_pen);
            TSI_StartSoftwareTrigger(base);
            tsi_lpwr_status.TSIScanCompleteFlag = 0;
            return;
        }
        /* If the next electrode is mutual-cap, get ready for next mutual-cap measurement (init TSI for mutual-cap)*/
        else if(next_pen < TF_TSI_TOTAL_CHANNEL_COUNT)
        {
            TSI_EnableModule(base, false);
            TSI_InitMutualCapMode(base, &tsiState->opModesData[tsiState->opMode].config.configMutual);
            TSI_SetMutualCapTxChannel(base, (tsi_mutual_tx_channel_t)((next_pen - TF_TSI_SELF_CAP_CHANNEL_COUNT) / TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT));
            TSI_SetMutualCapRxChannel(base, (tsi_mutual_rx_channel_t)((next_pen - TF_TSI_SELF_CAP_CHANNEL_COUNT) % TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT));
            TSI_EnableHardwareTriggerScan(base, false);
            TSI_EnableModule(base, true);
            TSI_StartSoftwareTrigger(base);
            tsi_lpwr_status.TSIScanCompleteFlag = 0;
            return;
        }
        else
        {
          tsi_lpwr_status.TSIScanCompleteFlag = 1;
        }
    }
    
    /* mutual-cap electrode was measured */
    else
    {
        curr_channel = (uint32_t)TSI_GetRxMutualCapMeasuredChannel(base) + (uint32_t)TSI_GetTxMutualCapMeasuredChannel(base)*TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT + TF_TSI_SELF_CAP_CHANNEL_COUNT;
        
        /* Save TSICNT value, artificially increase sensitivity and inverse counter value */
        if(((uint64_t)1 << curr_channel) & channels)
        {
            tsiState->counters[curr_channel] = TSI_GetCounter(base);
        }
        
        /* Find next enabled electrode */
        next_pen = curr_channel + 1;
        pen = channels;
        while (((((pen >> next_pen) & 0x1U)) == 0U) && (next_pen < (TF_TSI_TOTAL_CHANNEL_COUNT))) 
        {
            next_pen++;
        }
        
        /* Get ready for next mutual-cap measurement if there is any */
        if(next_pen < TF_TSI_TOTAL_CHANNEL_COUNT)
        {
            TSI_SetMutualCapTxChannel(base, (tsi_mutual_tx_channel_t)((next_pen - TF_TSI_SELF_CAP_CHANNEL_COUNT) / TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT));
            TSI_SetMutualCapRxChannel(base, (tsi_mutual_rx_channel_t)((next_pen - TF_TSI_SELF_CAP_CHANNEL_COUNT) % TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT));
            TSI_StartSoftwareTrigger(base);
            return;
        }
        else
        {
          tsi_lpwr_status.TSIScanCompleteFlag = 1;
        }
    }
    
    if(tsiState->pCallBackFunc)
    {
        tsiState->pCallBackFunc(instance, tsiState->usrData);
    }

    if(tsiState->status != kStatus_TSI_LowPower)
    {
        /* Return status of the driver to initialized state */
        tsiState->status = kStatus_TSI_Initialized;
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : NT_TSI_DRV_MeasureAllElect
* Description   : Automatically measures all used TSI electrodes.
*
*END**************************************************************************/
tsi_status_t NT_TSI_DRV_MeasureAllElect(uint32_t instance, uint16_t (*counters)[])
{
    NT_ASSERT(instance < FSL_FEATURE_SOC_TSI_COUNT);
    NT_ASSERT(counters != NULL);
    
    nt_tsi_state_t * tsiState = g_tsiStatePtr[instance];
    TSI_Type * base = g_tsiBase[instance];
    uint64_t pen;
    uint64_t next_pen = 0;
    uint32_t first_pen;
    uint32_t mutual_rx;
    uint32_t mutual_tx;
    uint32_t gencs = 0;
    uint32_t timeout = 1000000;      
    bool is_enabled;
    bool is_int_enabled;            
    
    /* OS: Critical section. Access to global variable */
    if (knt_Status_OSA_Success != NT_OSA_MutexLock(&tsiState->lock, OSA_WAIT_FOREVER))
    {
        return kStatus_TSI_Error;
    }

    if (tsiState->status != kStatus_TSI_Initialized)
    {
      /* OS: End of critical section. */
        NT_OSA_MutexUnlock(&tsiState->lock);
        return tsiState->status;
    }
    
    if(!tsiState->opModesData[tsiState->opMode].enabledElectrodes)
    {
        /* OS: End of critical section. */
        NT_OSA_MutexUnlock(&tsiState->lock);

        return kStatus_TSI_InvalidChannel;
    }

    CLOCK_EnableClock(kCLOCK_Tsi0);
    if (base->GENCS & TSI_GENCS_TSIEN_MASK)
    {
        is_enabled = true;
        TSI_EnableModule(base, false);
    }
    if (base->GENCS & TSI_GENCS_TSIIEN_MASK)
    {
        is_int_enabled = true;
        TSI_DisableInterrupts(base, kTSI_GlobalInterruptEnable);
    }

    while (next_pen < TF_TSI_TOTAL_CHANNEL_COUNT)  /* reused from NT_TSI_DRV_Measure */ 
    {      
        /* If there is a self-cap electrode used, initialize self-cap sensing*/
        if(tsiState->opModesData[tsiState->opMode].enabledElectrodes & 0x01FFFFFFU)
        {
            /* Find electrode with the lowest index */
            first_pen = 0U;
            pen = tsiState->opModesData[tsiState->opMode].enabledElectrodes;
            while (((pen >> first_pen) & 0x1U) == 0U)
            {
                first_pen++;
            }

            /* OS: End of critical section. */
            NT_OSA_MutexUnlock(&tsiState->lock);

            /* Init self-cap sensing */
            TSI_EnableModule(base, false);
            TSI_InitSelfCapMode(base, &tsiState->opModesData[tsiState->opMode].config.configSelfCap);
            TSI_SetSelfCapMeasuredChannel(base, first_pen);
            TSI_EnableHardwareTriggerScan(base, false);
            TSI_EnableModule(base, true);
            TSI_StartSoftwareTrigger(base);
        }
        /* If there are no self-cap electrodes used, initialize mutual-cap sensing*/
        else
        {
            /* Find electrode with the lowest index */
            first_pen = TF_TSI_SELF_CAP_CHANNEL_COUNT;
            pen = tsiState->opModesData[tsiState->opMode].enabledElectrodes;
            while (((pen >> first_pen) & 0x1U) == 0U) {
                first_pen++;
            }
            
            /*Parse electrode number into rx, tx components*/
            mutual_tx = (first_pen - TF_TSI_SELF_CAP_CHANNEL_COUNT) / TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT;
            mutual_rx = (first_pen - TF_TSI_SELF_CAP_CHANNEL_COUNT) % TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT;
            
            /* OS: End of critical section. */
            NT_OSA_MutexUnlock(&tsiState->lock);

            /* Init mutual-cap sensing */
            TSI_EnableModule(base, false);
            TSI_InitMutualCapMode(base, &tsiState->opModesData[tsiState->opMode].config.configMutual);
            TSI_SetMutualCapTxChannel(base, (tsi_mutual_tx_channel_t)mutual_tx);
            TSI_SetMutualCapRxChannel(base, (tsi_mutual_rx_channel_t)mutual_rx);
            TSI_EnableHardwareTriggerScan(base, false);
            TSI_EnableModule(base, true);
            TSI_StartSoftwareTrigger(base);
        } 
        
        while(1)   /* not an infinite loop, there is break on the end of loop,it can start again a few time. */             
        {  
            uint64_t channels = tsiState->opModesData[tsiState->opMode].enabledElectrodes;
            uint64_t pen;
            uint32_t curr_channel;   
            
            gencs = base->GENCS;
            while(((gencs & TSI_GENCS_EOSF_MASK) == 0U) && (--timeout))
            {
                gencs = base->GENCS;
                /* Do nothing, just to meet MISRA C 2004 rule 14.3 . */            
            }         
            
            /* Clear interrupt flags. */
            TSI_ClearStatusFlags(base, (uint32_t)kTSI_OutOfRangeFlag);
            TSI_ClearStatusFlags(base, kTSI_EndOfScanFlag);
            
            /* self-cap electrode was measured */
            if(!(TSI_GetSensingMode(base)))
            {
                curr_channel = TSI_GetSelfCapMeasuredChannel(base);
                
                /* Save TSICNT value */
                if((uint64_t)(1 << curr_channel) & channels)
                {
                    (*counters)[curr_channel] = ((uint16_t)0xFFFF - TSI_GetCounter(base));
                }
                
                /* Find next enabled electrode */
                next_pen = curr_channel + 1;
                pen = channels;
                while (((((pen >> next_pen) & 0x1U)) == 0U) && (next_pen < (TF_TSI_TOTAL_CHANNEL_COUNT))) 
                {
                    next_pen++;
                }
                
                /* If the next electrode is self-cap, get ready for next self-cap measurement */
                if(next_pen < TF_TSI_SELF_CAP_CHANNEL_COUNT)
                {
                    /* Measurement must continue on next channel. */
                    TSI_SetSelfCapMeasuredChannel(base, next_pen);
                    TSI_StartSoftwareTrigger(base);
                    continue;  /* go to next electrode measurement */
                }
                /* If the next electrode is mutual-cap, get ready for next mutual-cap measurement (init TSI for mutual-cap)*/
                else if(next_pen < TF_TSI_TOTAL_CHANNEL_COUNT)
                {
                    TSI_EnableModule(base, false);
                    TSI_InitMutualCapMode(base, &tsiState->opModesData[tsiState->opMode].config.configMutual);
                    TSI_SetMutualCapTxChannel(base, (tsi_mutual_tx_channel_t)((next_pen - TF_TSI_SELF_CAP_CHANNEL_COUNT) / TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT));
                    TSI_SetMutualCapRxChannel(base, (tsi_mutual_rx_channel_t)((next_pen - TF_TSI_SELF_CAP_CHANNEL_COUNT) % TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT));
                    TSI_EnableHardwareTriggerScan(base, false);
                    TSI_EnableModule(base, true);
                    TSI_StartSoftwareTrigger(base);
                    continue;  /* go to next electrode measurement */
                }
            }       
            /* mutual-cap electrode was measured */
            else
            {
                curr_channel = (uint32_t)TSI_GetRxMutualCapMeasuredChannel(base) + (uint32_t)TSI_GetTxMutualCapMeasuredChannel(base)*TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT + TF_TSI_SELF_CAP_CHANNEL_COUNT;
                
                /* Save TSICNT value, artificially increase sensitivity and inverse counter value */
                if(((uint64_t)1 << curr_channel) & channels)
                {
                    (*counters)[curr_channel] = TSI_GetCounter(base);
                }
                
                /* Find next enabled electrode */
                next_pen = curr_channel + 1;
                pen = channels;
                while (((((pen >> next_pen) & 0x1U)) == 0U) && (next_pen < (TF_TSI_TOTAL_CHANNEL_COUNT))) 
                {
                    next_pen++;
                }
                
                /* Get ready for next mutual-cap measurement if there is any */
                if(next_pen < TF_TSI_TOTAL_CHANNEL_COUNT)
                {
                    TSI_SetMutualCapTxChannel(base, (tsi_mutual_tx_channel_t)((next_pen - TF_TSI_SELF_CAP_CHANNEL_COUNT) / TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT));
                    TSI_SetMutualCapRxChannel(base, (tsi_mutual_rx_channel_t)((next_pen - TF_TSI_SELF_CAP_CHANNEL_COUNT) % TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT));
                    TSI_StartSoftwareTrigger(base);
                    continue; /* go to next electrode measurement */
                }
            }
            break;  /* all electrode measurement finished go out*/
        }    
    }   
       
    if (is_enabled)
    {
        TSI_EnableModule(base, true);
    }
    if (is_int_enabled)
    {
        TSI_EnableInterrupts(base, kTSI_GlobalInterruptEnable);
    }

    tsiState->status = kStatus_TSI_Initialized;

    /* OS: End of critical section. */
    NT_OSA_MutexUnlock(&tsiState->lock);

    return kStatus_TSI_Success;
}
/*FUNCTION**********************************************************************
*
* Function Name : NT_TSI_DRV_MeasureMoreElect
* Description   : Automatically measures selected TSI electrodes.
*
*END**************************************************************************/
tsi_status_t NT_TSI_DRV_MeasureMoreElect(uint32_t instance, uint16_t (*counters)[], uint64_t measuredElectrodes)
{
    NT_ASSERT(instance < FSL_FEATURE_SOC_TSI_COUNT);
    NT_ASSERT(counters != NULL);
    
    nt_tsi_state_t * tsiState = g_tsiStatePtr[instance];
    TSI_Type * base = g_tsiBase[instance];
    uint64_t pen;
    uint64_t next_pen = 0;
    uint32_t first_pen;
    uint32_t mutual_rx;
    uint32_t mutual_tx;
    uint32_t gencs = 0;
    uint32_t timeout = 1000000;      
    bool is_enabled;
    bool is_int_enabled;            
    
    /* OS: Critical section. Access to global variable */
    if (knt_Status_OSA_Success != NT_OSA_MutexLock(&tsiState->lock, OSA_WAIT_FOREVER))
    {
        return kStatus_TSI_Error;
    }

    if (tsiState->status != kStatus_TSI_Initialized)
    {
      /* OS: End of critical section. */
        NT_OSA_MutexUnlock(&tsiState->lock);
        return tsiState->status;
    }
    
    if(!measuredElectrodes)
    {
        /* OS: End of critical section. */
        NT_OSA_MutexUnlock(&tsiState->lock);

        return kStatus_TSI_InvalidChannel;
    }

    CLOCK_EnableClock(kCLOCK_Tsi0);
    if (base->GENCS & TSI_GENCS_TSIEN_MASK)
    {
        is_enabled = true;
        TSI_EnableModule(base, false);
    }
    if (base->GENCS & TSI_GENCS_TSIIEN_MASK)
    {
        is_int_enabled = true;
        TSI_DisableInterrupts(base, kTSI_GlobalInterruptEnable);
    }

    while (next_pen < TF_TSI_TOTAL_CHANNEL_COUNT)  /* reused from NT_TSI_DRV_Measure */ 
    {      
        /* If there is a self-cap electrode used, initialize self-cap sensing*/
        if(measuredElectrodes & 0x01FFFFFFU)
        {
            /* Find electrode with the lowest index */
            first_pen = 0U;
            pen = measuredElectrodes;
            while (((pen >> first_pen) & 0x1U) == 0U)
            {
                first_pen++;
            }

            /* OS: End of critical section. */
            NT_OSA_MutexUnlock(&tsiState->lock);

            /* Init self-cap sensing */
            TSI_EnableModule(base, false);
            TSI_InitSelfCapMode(base, &tsiState->opModesData[tsiState->opMode].config.configSelfCap);
            TSI_SetSelfCapMeasuredChannel(base, first_pen);
            TSI_EnableHardwareTriggerScan(base, false);
            TSI_EnableModule(base, true);
            TSI_StartSoftwareTrigger(base);
        }
        /* If there are no self-cap electrodes used, initialize mutual-cap sensing*/
        else
        {
            /* Find electrode with the lowest index */
            first_pen = TF_TSI_SELF_CAP_CHANNEL_COUNT;
            pen = measuredElectrodes;
            while (((pen >> first_pen) & 0x1U) == 0U) {
                first_pen++;
            }
            
            /*Parse electrode number into rx, tx components*/
            mutual_tx = (first_pen - TF_TSI_SELF_CAP_CHANNEL_COUNT) / TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT;
            mutual_rx = (first_pen - TF_TSI_SELF_CAP_CHANNEL_COUNT) % TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT;
            
            /* OS: End of critical section. */
            NT_OSA_MutexUnlock(&tsiState->lock);

            /* Init mutual-cap sensing */
            TSI_EnableModule(base, false);
            TSI_InitMutualCapMode(base, &tsiState->opModesData[tsiState->opMode].config.configMutual);
            TSI_SetMutualCapTxChannel(base, (tsi_mutual_tx_channel_t)mutual_tx);
            TSI_SetMutualCapRxChannel(base, (tsi_mutual_rx_channel_t)mutual_rx);
            TSI_EnableHardwareTriggerScan(base, false);
            TSI_EnableModule(base, true);
            TSI_StartSoftwareTrigger(base);
        } 
        
        while(1)   /* not an infinite loop, there is break on the end of loop,it can start again a few time. */             
        {  
            uint64_t channels = measuredElectrodes;
            uint64_t pen;
            uint32_t curr_channel;   
            
            gencs = base->GENCS;
            while(((gencs & TSI_GENCS_EOSF_MASK) == 0U) && (--timeout))
            {
                gencs = base->GENCS;
                /* Do nothing, just to meet MISRA C 2004 rule 14.3 . */            
            }         
            
            /* Clear interrupt flags. */
            TSI_ClearStatusFlags(base, (uint32_t)kTSI_OutOfRangeFlag);
            TSI_ClearStatusFlags(base, kTSI_EndOfScanFlag);
            
            /* self-cap electrode was measured */
            if(!(TSI_GetSensingMode(base)))
            {
                curr_channel = TSI_GetSelfCapMeasuredChannel(base);
                
                /* Save TSICNT value */
                if((uint64_t)(1 << curr_channel) & channels)
                {
                    (*counters)[curr_channel] = ((uint16_t)0xFFFF - TSI_GetCounter(base));
                }
                
                /* Find next enabled electrode */
                next_pen = curr_channel + 1;
                pen = channels;
                while (((((pen >> next_pen) & 0x1U)) == 0U) && (next_pen < (TF_TSI_TOTAL_CHANNEL_COUNT))) 
                {
                    next_pen++;
                }
                
                /* If the next electrode is self-cap, get ready for next self-cap measurement */
                if(next_pen < TF_TSI_SELF_CAP_CHANNEL_COUNT)
                {
                    /* Measurement must continue on next channel. */
                    TSI_SetSelfCapMeasuredChannel(base, next_pen);
                    TSI_StartSoftwareTrigger(base);
                    continue;  /* go to next electrode measurement */
                }
                /* If the next electrode is mutual-cap, get ready for next mutual-cap measurement (init TSI for mutual-cap)*/
                else if(next_pen < TF_TSI_TOTAL_CHANNEL_COUNT)
                {
                    TSI_EnableModule(base, false);
                    TSI_InitMutualCapMode(base, &tsiState->opModesData[tsiState->opMode].config.configMutual);
                    TSI_SetMutualCapTxChannel(base, (tsi_mutual_tx_channel_t)((next_pen - TF_TSI_SELF_CAP_CHANNEL_COUNT) / TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT));
                    TSI_SetMutualCapRxChannel(base, (tsi_mutual_rx_channel_t)((next_pen - TF_TSI_SELF_CAP_CHANNEL_COUNT) % TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT));
                    TSI_EnableHardwareTriggerScan(base, false);
                    TSI_EnableModule(base, true);
                    TSI_StartSoftwareTrigger(base);
                    continue;  /* go to next electrode measurement */
                }
            }       
            /* mutual-cap electrode was measured */
            else
            {
                curr_channel = (uint32_t)TSI_GetRxMutualCapMeasuredChannel(base) + (uint32_t)TSI_GetTxMutualCapMeasuredChannel(base)*TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT + TF_TSI_SELF_CAP_CHANNEL_COUNT;
                
                /* Save TSICNT value, artificially increase sensitivity and inverse counter value */
                if(((uint64_t)1 << curr_channel) & channels)
                {
                    (*counters)[curr_channel] = TSI_GetCounter(base);
                }
                
                /* Find next enabled electrode */
                next_pen = curr_channel + 1;
                pen = channels;
                while (((((pen >> next_pen) & 0x1U)) == 0U) && (next_pen < (TF_TSI_TOTAL_CHANNEL_COUNT))) 
                {
                    next_pen++;
                }
                
                /* Get ready for next mutual-cap measurement if there is any */
                if(next_pen < TF_TSI_TOTAL_CHANNEL_COUNT)
                {
                    TSI_SetMutualCapTxChannel(base, (tsi_mutual_tx_channel_t)((next_pen - TF_TSI_SELF_CAP_CHANNEL_COUNT) / TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT));
                    TSI_SetMutualCapRxChannel(base, (tsi_mutual_rx_channel_t)((next_pen - TF_TSI_SELF_CAP_CHANNEL_COUNT) % TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT));
                    TSI_StartSoftwareTrigger(base);
                    continue; /* go to next electrode measurement */
                }
            }
            break;  /* all electrode measurement finished go out*/
        }    
    }   
       
    if (is_enabled)
    {
        TSI_EnableModule(base, true);
    }
    if (is_int_enabled)
    {
        TSI_EnableInterrupts(base, kTSI_GlobalInterruptEnable);
    }

    tsiState->status = kStatus_TSI_Initialized;

    /* OS: End of critical section. */
    NT_OSA_MutexUnlock(&tsiState->lock);

    return kStatus_TSI_Success;
}

/*FUNCTION**********************************************************************
*
* Function Name : NT_TSI_DRV_MeasureOneElect
* Description   : Automatically measures one electrode.
*
*END**************************************************************************/
int32_t NT_TSI_DRV_MeasureOneElect(uint32_t instance, uint32_t electrode)
{
    NT_ASSERT(instance < FSL_FEATURE_SOC_TSI_COUNT);
    
    nt_tsi_state_t * tsiState = g_tsiStatePtr[instance];
    TSI_Type * base = g_tsiBase[instance];
    uint32_t mutual_rx, mutual_tx;
    uint32_t gencs = 0;
    uint32_t timeout = 1000000;  
    uint32_t ret_val;    
    bool is_enabled;
    bool is_int_enabled;            
    
    /* OS: Critical section. Access to global variable */
    if (knt_Status_OSA_Success != NT_OSA_MutexLock(&tsiState->lock, OSA_WAIT_FOREVER))
    {
        return kStatus_TSI_Error;
    }

    if (tsiState->status != kStatus_TSI_Initialized)
    {
      /* OS: End of critical section. */
        NT_OSA_MutexUnlock(&tsiState->lock);
        return tsiState->status;
    }
    
    if(!tsiState->opModesData[tsiState->opMode].enabledElectrodes)
    {
        /* OS: End of critical section. */
        NT_OSA_MutexUnlock(&tsiState->lock);

        return kStatus_TSI_InvalidChannel;
    }

    CLOCK_EnableClock(kCLOCK_Tsi0);
    if (base->GENCS & TSI_GENCS_TSIEN_MASK)
    {
        is_enabled = true;
        TSI_EnableModule(base, false);
    }
    if (base->GENCS & TSI_GENCS_TSIIEN_MASK)
    {
        is_int_enabled = true;
        TSI_DisableInterrupts(base, kTSI_GlobalInterruptEnable);
    }
     
        /* If there is a self-cap electrode used, initialize self-cap sensing*/
        if (electrode < TF_TSI_SELF_CAP_CHANNEL_COUNT)    
        {
            /* OS: End of critical section. */
            NT_OSA_MutexUnlock(&tsiState->lock);

            /* Init self-cap sensing */
            TSI_EnableModule(base, false);
            TSI_InitSelfCapMode(base, &tsiState->opModesData[tsiState->opMode].config.configSelfCap);
            TSI_SetSelfCapMeasuredChannel(base, electrode);
            TSI_EnableHardwareTriggerScan(base, false);
            TSI_EnableModule(base, true);
            TSI_StartSoftwareTrigger(base);
        }
        /* If there are no self-cap electrodes used, initialize mutual-cap sensing*/
        else
        {           
            /*Parse electrode number into rx, tx components*/
            mutual_tx = (electrode - TF_TSI_SELF_CAP_CHANNEL_COUNT) / TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT;
            mutual_rx = (electrode - TF_TSI_SELF_CAP_CHANNEL_COUNT) % TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT;
            
            /* OS: End of critical section. */
            NT_OSA_MutexUnlock(&tsiState->lock);

            /* Init mutual-cap sensing */
            TSI_EnableModule(base, false);
            TSI_InitMutualCapMode(base, &tsiState->opModesData[tsiState->opMode].config.configMutual);
            TSI_SetMutualCapTxChannel(base, (tsi_mutual_tx_channel_t)mutual_tx);
            TSI_SetMutualCapRxChannel(base, (tsi_mutual_rx_channel_t)mutual_rx);
            TSI_EnableHardwareTriggerScan(base, false);
            TSI_EnableModule(base, true);
            TSI_StartSoftwareTrigger(base);
        }  
            
            gencs = base->GENCS;
            while(((gencs & TSI_GENCS_EOSF_MASK) == 0U) && (--timeout))
            {
                gencs = base->GENCS;
                /* Do nothing, just to meet MISRA C 2004 rule 14.3 . */            
            }         
            
            /* Clear interrupt flags. */
            TSI_ClearStatusFlags(base, (uint32_t)kTSI_OutOfRangeFlag);
            TSI_ClearStatusFlags(base, kTSI_EndOfScanFlag);
            
            /* self-cap electrode was measured */
            if(!(TSI_GetSensingMode(base)))
            {
                ret_val = ((uint16_t)0xFFFF - TSI_GetCounter(base));
            }       
            /* mutual-cap electrode was measured */
            else
            {             
                ret_val = TSI_GetCounter(base);
            }       
    if (is_enabled)
    {
        TSI_EnableModule(base, true);
    }
    if (is_int_enabled)
    {
        TSI_EnableInterrupts(base, kTSI_GlobalInterruptEnable);
    }

    tsiState->status = kStatus_TSI_Initialized;

    /* OS: End of critical section. */
    NT_OSA_MutexUnlock(&tsiState->lock);

    return ret_val;
}

/*FUNCTION**********************************************************************
*
* Function Name : NT_TSI_DRV_SensitivityMeasurement
* Description   : Measures delta values of selected channels and calculates sensitivity.
*
*END**************************************************************************/
#if (NT_SAFETY_SUPPORT == 1)
tsi_status_t NT_TSI_DRV_SensitivityMeasurement (uint32_t instance, uint16_t * minElectVal, uint16_t * maxElectVal, uint64_t enabledElectrodes)
{
    NT_ASSERT(minElectVal != NULL);
    NT_ASSERT(maxElectVal != NULL);
    
    int32_t delta; 
    uint16_t curr_counters[TF_TSI_TOTAL_CHANNEL_COUNT];
    uint16_t prev_counters[TF_TSI_TOTAL_CHANNEL_COUNT];    
    uint16_t curr_channel;
    uint16_t channel_count;
    tsi_status_t status;

    *minElectVal = 0xffff;
    *maxElectVal = 0;
    
    /* waiting to touch event to obtain delta value */
    for (curr_channel=0; curr_channel<TF_TSI_TOTAL_CHANNEL_COUNT; curr_channel++)
    {   prev_counters[curr_channel] = 0;    /* clear counter array */
        curr_counters[curr_channel] = 0;    /* clear counter array */
    }
       
    status = NT_TSI_DRV_MeasureMoreElect(instance, &prev_counters, enabledElectrodes);  
    status |= nt_module_TSI_Touch(instance, enabledElectrodes);
    status |= NT_TSI_DRV_MeasureMoreElect(instance, &curr_counters, enabledElectrodes);  
    status |= nt_module_TSI_Release(instance, enabledElectrodes);
    
    if (enabledElectrodes & TF_TSI_SELF_CAP_CHANNELS_MASK)
        channel_count = TF_TSI_SELF_CAP_CHANNEL_COUNT; 
    else   
        channel_count = TF_TSI_TOTAL_CHANNEL_COUNT; 
    
    for (curr_channel=0; curr_channel < channel_count; curr_channel++)
    {   if (((enabledElectrodes >> curr_channel) & 0x1U) == 0U)       
            continue;           
        if (curr_counters[curr_channel] != 0 && prev_counters[curr_channel] != 0)  
        {   delta = (int32_t)curr_counters[curr_channel] - (int32_t)prev_counters[curr_channel];
            if (delta <= 0)                    
                continue;
            if (delta < *minElectVal)               /* find min value */
                *minElectVal = delta; 
            if (delta > *maxElectVal)               /* find max value */
                *maxElectVal = delta; 
        }
    }
    return status; 
}            
#endif /* NT_SAFETY_SUPPORT */
/*FUNCTION**********************************************************************
*
* Function Name : NT_TSI_DRV_UntouchedMeasurement
* Description   : Measures counter values of selected channels and calculates counter value.
*
*END**************************************************************************/
tsi_status_t NT_TSI_DRV_UntouchedMeasurement (uint32_t instance, uint16_t * minElectVal, uint16_t * maxElectVal, uint64_t enabledElectrodes)
{
    NT_ASSERT(minElectVal != NULL);
    NT_ASSERT(maxElectVal != NULL);
    
    uint16_t curr_counters[TF_TSI_TOTAL_CHANNEL_COUNT];
    uint16_t curr_channel;
    tsi_status_t status;
    
    *minElectVal = 0xffff;
    *maxElectVal = 0;
    
    for (curr_channel=0; curr_channel<TF_TSI_TOTAL_CHANNEL_COUNT; curr_channel++)
    {  curr_counters[curr_channel] = 0;                              /* clear array counter */         
    }                                        
    status = NT_TSI_DRV_MeasureMoreElect(instance, &curr_counters, enabledElectrodes);    /* measure enabled electrodes */
    
    for (curr_channel=0; curr_channel<TF_TSI_TOTAL_CHANNEL_COUNT; curr_channel++)
    {                                               
        if (((enabledElectrodes >> curr_channel) & 0x1U) == 0U)      /* avoid calculation with disabled channels */
            continue;                                                
        if (curr_counters[curr_channel] == 0)                         
            continue;       
        if (curr_counters[curr_channel] < *minElectVal)               /* find min value */
            *minElectVal = curr_counters[curr_channel];               
        if (curr_counters[curr_channel] > *maxElectVal)               /* find max value */
            *maxElectVal = curr_counters[curr_channel];  
    }
    return status; 
}

/*FUNCTION**********************************************************************
*
* Function Name : NT_TSI_DRV_Recalibrate
* Description   : Automatically recalibrates all important TSI settings.
*
*END**************************************************************************/
#if (NT_SAFETY_SUPPORT == 1)
tsi_status_t NT_TSI_DRV_Recalibrate(uint32_t instance, void *configuration)
{
    NT_ASSERT(instance < FSL_FEATURE_SOC_TSI_COUNT);
    NT_ASSERT(configuration != NULL);

    nt_tsi_state_t * tsiState = g_tsiStatePtr[instance];
    struct nt_kernel * system = _nt_system_get(); 
    nt_tsi_recalib_config_t *recalib_config = system->rom->modules[instance]->recalib_config;
    tsi_config_t *tsi_config;
    tsi_status_t result;

    uint64_t enabledElectrodes;    

    uint16_t minElectVal;
    uint16_t maxElectVal;    
    uint8_t cycleCounter;
        
    /* OS: Critical section. Access to global variable */
    if (knt_Status_OSA_Success != NT_OSA_MutexLock(&tsiState->lock, OSA_WAIT_FOREVER))
    {
        return kStatus_TSI_Error;
    }

    if (tsiState->status != kStatus_TSI_Initialized)
    {
      /* OS: End of critical section. */
        NT_OSA_MutexUnlock(&tsiState->lock);
        return tsiState->status;
    }
    
    if(!tsiState->opModesData[tsiState->opMode].enabledElectrodes)
    {
        /* OS: End of critical section. */
        NT_OSA_MutexUnlock(&tsiState->lock);

        return kStatus_TSI_InvalidChannel;
    }
    
    tsi_config = &(tsiState->opModesData[tsiState->opMode].config);

    /* Default configuration parameter for tuning process */
    /* Self capacitance measurement configuration */   
    tsi_config->configSelfCap.commonConfig.mainClock = kTSI_MainClockSlection_1,        /*!< Set main clock */
    tsi_config->configSelfCap.commonConfig.mode = kTSI_SensingModeSlection_Self,        /*!< Choose sensing mode */
    tsi_config->configSelfCap.commonConfig.dvolt = kTSI_DvoltOption_0,                  /*!< DVOLT option value  */
    tsi_config->configSelfCap.commonConfig.cutoff = kTSI_SincCutoffDiv_2,               /*!< Cut-off divider  */
    tsi_config->configSelfCap.commonConfig.decimation = kTSI_SincDecimationValue_28,    /*!< SINC decimation value */ 
    tsi_config->configSelfCap.commonConfig.order = kTSI_SincFilterOrder_2,              /*!< SINC filter order */
    tsi_config->configSelfCap.commonConfig.chargeNum = kTSI_SscChargeNumValue_2,        /*!< SSC output bit0's period setting */
    tsi_config->configSelfCap.commonConfig.noChargeNum = kTSI_SscNoChargeNumValue_2,    /*!< SSC output bit1's period setting */
    tsi_config->configSelfCap.commonConfig.prbsOutsel = kTSI_SscPrbsOutsel_15,  
    tsi_config->configSelfCap.enableSensitivity = true,                                 /*!< Enable sensitivity boost of self-cap or not */
    tsi_config->configSelfCap.enableShield = false,
    tsi_config->configSelfCap.inputCurrent = kTSI_CurrentMultipleInputValue_0,          /*!< Input current multiple   */
    tsi_config->configSelfCap.xdn = kTSI_SensitivityXdnOption_2,                        /*!< Sensitivity XDN option   */
    tsi_config->configSelfCap.chargeCurrent = kTSI_CurrentMultipleChargeValue_2,        /*!< Charge/Discharge current multiple */
    tsi_config->configSelfCap.ctrim = kTSI_SensitivityCtrimOption_2,                    /*!< Sensitivity CTRIM option */
    /* Mutual capacitance measurement configuration */
    tsi_config->configMutual.commonConfig.mainClock = kTSI_MainClockSlection_0,         /*!< Set main clock */ 
    tsi_config->configMutual.commonConfig.mode = kTSI_SensingModeSlection_Mutual,       /*!< Choose sensing mode */
    tsi_config->configMutual.commonConfig.dvolt = kTSI_DvoltOption_0,                   /*!< DVOLT option value  */
    tsi_config->configMutual.commonConfig.cutoff = kTSI_SincCutoffDiv_0,                /*!< Cut-off divider  */
    tsi_config->configMutual.commonConfig.order = kTSI_SincFilterOrder_1,               /*!< SINC filter order */
    tsi_config->configMutual.commonConfig.decimation = kTSI_SincDecimationValue_8,      /*!< SINC decimation value */
    tsi_config->configMutual.commonConfig.chargeNum = kTSI_SscChargeNumValue_4,         /*!< SSC output bit0's period setting */
    tsi_config->configMutual.commonConfig.noChargeNum = kTSI_SscNoChargeNumValue_2,     /*!< SSC output bit1's period setting */
    tsi_config->configMutual.preCurrent = kTSI_MutualPreCurrent_4uA,                    /*!< Vref generator current  */
    tsi_config->configMutual.preResistor = kTSI_MutualPreResistor_4k,                   /*!< Vref generator resistor */
    tsi_config->configMutual.senseResistor = kTSI_MutualSenseResistor_10k,              /*!< I-sense generator resistor */
    tsi_config->configMutual.boostCurrent = kTSI_MutualSenseBoostCurrent_0uA,           /*!< Sensitivity boost current setting */
    tsi_config->configMutual.txDriveMode = kTSI_MutualTxDriveModeOption_0,              /*!< TX drive mode control setting */
    tsi_config->configMutual.pmosLeftCurrent = kTSI_MutualPmosCurrentMirrorLeft_32,     /*!< Pmos current mirror on the left side  */
    tsi_config->configMutual.pmosRightCurrent = kTSI_MutualPmosCurrentMirrorRight_1,    /*!< Pmos current mirror on the right side */
    tsi_config->configMutual.enableNmosMirror = true,                                   /*!< Enable Nmos current mirror setting or not */
    tsi_config->configMutual.nmosCurrent = kTSI_MutualNmosCurrentMirror_1,              /*!< Nmos current mirror setting */

    /* untouched counter calibration for self-cap mode */
    cycleCounter = 0;
    enabledElectrodes = tsiState->opModesData[tsiState->opMode].enabledElectrodes & TF_TSI_SELF_CAP_CHANNELS_MASK;
    if (enabledElectrodes)          /* if self-capacitance is not enabled go out */
    {
        do 
        {               
            result = NT_TSI_DRV_UntouchedMeasurement(instance, &minElectVal, &maxElectVal, enabledElectrodes);
            if (result !=  kStatus_TSI_Success || minElectVal == 65535 || maxElectVal == 0)
            {   return kStatus_TSI_SelfUntouchRecalibError; 
            } 
            cycleCounter++;
            
            if (minElectVal < recalib_config->SelfRangeMin)       /* untouched counter value is too low */
            {                
                if (tsi_config->configSelfCap.commonConfig.decimation > kTSI_SincDecimationValue_1 && 
                    cycleCounter <= 32)                                                    /* SINC decimation calibration, 32 values */
                {   tsi_config->configSelfCap.commonConfig.decimation -= 1;          
                    continue;
                }            
                else if (tsi_config->configSelfCap.chargeCurrent < kTSI_CurrentMultipleChargeValue_7 && 
                         cycleCounter > 32 && cycleCounter <= 32+8)                        /* Charge/Discharge current multiple calibration, 8 values */
                {   tsi_config->configSelfCap.chargeCurrent += 1;   
                    continue;
                } 
                else if (tsi_config->configSelfCap.inputCurrent < kTSI_CurrentMultipleInputValue_1 && 
                         cycleCounter > 32+8 && cycleCounter <= 32+8+2)                    /* Input current multiple calibration, 2 values */
                {   tsi_config->configSelfCap.inputCurrent = kTSI_CurrentMultipleInputValue_1;         
                    continue;
                }
                else if (tsi_config->configSelfCap.commonConfig.dvolt > kTSI_DvoltOption_0 && 
                         cycleCounter > 32+8+2 && cycleCounter <= 32+8+2+4)                /* DVOLT option value calibration, 4 values */
                {   tsi_config->configSelfCap.commonConfig.dvolt -= 1;                                 
                    continue;
                }
                else if (tsi_config->configSelfCap.commonConfig.cutoff < kTSI_SincCutoffDiv_128 &&
                         cycleCounter > 32+8+2+4 && cycleCounter <= 32+8+2+4+8)            /* Cut-off divider calibration, 8 values */
                {   tsi_config->configSelfCap.commonConfig.cutoff += 1;                                
                    continue;
                } 
                else if (tsi_config->configSelfCap.commonConfig.order > kTSI_SincFilterOrder_1 && 
                         cycleCounter > 32+8+2+4+8 && cycleCounter <= 32+8+2+4+8+2)        /* SINC filter order, 2 values */
                {   tsi_config->configSelfCap.commonConfig.order -= 1;                                
                    continue;
                }                  
                else if (cycleCounter > 0 && cycleCounter <= 32+8+2+4+8+2)
                {   continue; 
                }
                else 
                {   return kStatus_TSI_SelfUntouchRecalibError;   /* no configuration setting available - go out from calibration */
                }
            }
            else if (maxElectVal > recalib_config->SelfRangeMax)                         /* untouched counter value is too high */
            {
                if (tsi_config->configSelfCap.commonConfig.decimation < kTSI_SincDecimationValue_32 && 
                    cycleCounter <= 32)                                                  /* SINC decimation calibration, 32 values  */
                {   tsi_config->configSelfCap.commonConfig.decimation += 1;                           
                    continue;
                }                                                                                 
                else if (tsi_config->configSelfCap.chargeCurrent > kTSI_CurrentMultipleChargeValue_0 && 
                         cycleCounter > 32 && cycleCounter <= 32+8)                     /* Charge/Discharge current multiple calibration, 8 values */
                {   tsi_config->configSelfCap.chargeCurrent -= 1;                                     
                    continue;
                }                                                                                 
                else if (tsi_config->configSelfCap.inputCurrent > kTSI_CurrentMultipleInputValue_0 && 
                         cycleCounter > 32+8 && cycleCounter <= 32+8+2)                 /* Input current multiple calibration, 2 values */
                {   tsi_config->configSelfCap.inputCurrent = kTSI_CurrentMultipleInputValue_0;         
                    continue;
                }                                                                                 
                else if (tsi_config->configSelfCap.commonConfig.dvolt < kTSI_DvoltOption_3 && 
                         cycleCounter > 32+8+2 && cycleCounter <= 32+8+2+4)             /* DVOLT option value calibration, 4 values */
                {   tsi_config->configSelfCap.commonConfig.dvolt += 1;                                 
                    continue;
                }                                                                                 
                else if (tsi_config->configSelfCap.commonConfig.cutoff > kTSI_SincCutoffDiv_0 && 
                         cycleCounter > 32+8+2+4 && cycleCounter <= 32+8+2+4+8)         /* Cut-off divider calibration, 8 values */
                {   tsi_config->configSelfCap.commonConfig.cutoff -= 1;                               
                    continue;
                } 
                else if (tsi_config->configSelfCap.commonConfig.order < kTSI_SincFilterOrder_2 &&
                         cycleCounter > 32+8+2+4+8 && cycleCounter <= 32+8+2+4+8+2)     /* SINC filter order, 2 values */
                {   tsi_config->configSelfCap.commonConfig.order += 1;                                
                    continue;
                }                  
                else if (cycleCounter > 0 && cycleCounter <= 32+8+2+4+8+2)
                {   continue; 
                }
                else 
                {   return kStatus_TSI_SelfUntouchRecalibError;   /* no configuration setting available - go out from calibration */
                }
            }    
        } while ((minElectVal < recalib_config->SelfRangeMin) || (maxElectVal > recalib_config->SelfRangeMax));    /* next calibration cycle required, TSI setting changed */

        /* sensitivity calibration for self-cap mode, it requests one or more touches on self-cap electrode */
        cycleCounter = 0;        
        do
        { 
            result = NT_TSI_DRV_SensitivityMeasurement(instance, &minElectVal, &maxElectVal, enabledElectrodes);
            if (result !=  kStatus_TSI_Success || minElectVal == 65535 || maxElectVal == 0)
            {  
                return kStatus_TSI_SelfSensitivRecalibError; 
            } 

            cycleCounter++;
            
            if (minElectVal < recalib_config->SelfRangeDeltaMin) 
            {   tsi_config->configSelfCap.enableSensitivity = true; 
                if (tsi_config->configSelfCap.xdn < kTSI_SensitivityXdnOption_7 && 
                    cycleCounter <= 8)                                                     /* Sensitivity XDN option calibration, 8 values   */
                {   tsi_config->configSelfCap.xdn += 1;             
                    continue;
                }                  
                else if (tsi_config->configSelfCap.ctrim < kTSI_SensitivityCtrimOption_7 && 
                         cycleCounter > 8 && cycleCounter <= 8+8)                          /* Sensitivity CTRIM option calibration, 8 values */
                {   tsi_config->configSelfCap.ctrim += 1;  
                    continue;
                }                 
                else if (tsi_config->configSelfCap.commonConfig.dvolt > kTSI_DvoltOption_0 && 
                         cycleCounter > 8+8 && cycleCounter <= 8+8+4)                      /* DVOLT option value calibration, 4 values */
                {   tsi_config->configSelfCap.commonConfig.dvolt -= 1;                                 
                    continue;
                }
                else if (tsi_config->configSelfCap.commonConfig.cutoff > kTSI_SincCutoffDiv_0 && 
                         cycleCounter > 8+8+4 && cycleCounter <= 8+8+4+8)                  /* Cut-off divider calibration, 8 values */
                {   tsi_config->configSelfCap.commonConfig.cutoff -= 1;                                
                    continue;
                }               
                else if (cycleCounter > 0 && cycleCounter <= 8+8+4+8)
                {   continue; 
                }
                else 
                {   return kStatus_TSI_SelfSensitivRecalibError;   /* no configuration setting available - go out from calibration */
                }
            } 
            else if (maxElectVal > recalib_config->SelfRangeDeltaMax) 
            {         
                tsi_config->configSelfCap.enableSensitivity = true; 
                if (tsi_config->configSelfCap.xdn > kTSI_SensitivityXdnOption_0 && 
                    cycleCounter <= 8)                                                     /* Sensitivity XDN option calibration, 8 values   */
                {   tsi_config->configSelfCap.xdn -= 1;                                   
                    continue;
                } 
                else if (tsi_config->configSelfCap.ctrim > kTSI_SensitivityCtrimOption_0 && 
                         cycleCounter > 8 && cycleCounter <= 8+8)                          /* Sensitivity CTRIM option calibration, 8 values */
                {   tsi_config->configSelfCap.ctrim -= 1; 
                    continue;
                }     
                else if (tsi_config->configSelfCap.commonConfig.dvolt < kTSI_DvoltOption_3 && 
                         cycleCounter > 8+8 && cycleCounter <= 8+8+4)                      /* DVOLT option value calibration, 4 values */
                {   tsi_config->configSelfCap.commonConfig.dvolt += 1;                                 
                    continue;
                }                                                                                 
                else if (tsi_config->configSelfCap.commonConfig.cutoff < kTSI_SincCutoffDiv_128 && 
                         cycleCounter > 8+8+4 && cycleCounter <= 8+8+4+8)                  /* Cut-off divider calibration, 8 values */
                {   tsi_config->configSelfCap.commonConfig.cutoff += 1;                               
                    continue;
                }                   
                else if (cycleCounter > 0 && cycleCounter <= 8+8+4+8)
                {   continue; 
                }
                else 
                {   return kStatus_TSI_SelfSensitivRecalibError;   /* no configuration setting available - go out from calibration */
                }
            }
        } while ((minElectVal < recalib_config->SelfRangeDeltaMin) || (maxElectVal > recalib_config->SelfRangeDeltaMax));        
    }

    /* untouched counter calibration for mutual-cap mode */   
    cycleCounter = 0;                                                                             
    enabledElectrodes = tsiState->opModesData[tsiState->opMode].enabledElectrodes & TF_TSI_MUTUAL_CAP_CHANNELS_MASK ;
    if (enabledElectrodes)          /* if mutual-capacitance is not enabled go out */
    {   
        do 
        {   
            result = NT_TSI_DRV_UntouchedMeasurement(instance, &minElectVal, &maxElectVal, enabledElectrodes);         
            if (result !=  kStatus_TSI_Success || minElectVal == 65535 || maxElectVal == 0)
            {   
                return kStatus_TSI_MutUntouchRecalibError; 
            } 
            cycleCounter++;
            
            if (minElectVal < recalib_config->MutRangeMin)         /* untouched counter value is too low */
            {                    
                if (tsi_config->configMutual.commonConfig.decimation < kTSI_SincDecimationValue_32 && 
                   cycleCounter <= 32)                                         /* SINC decimation calibration, 32 values */
                {   tsi_config->configMutual.commonConfig.decimation += 1;                           
                    continue;                                                                        
                }                                                                                    
                else if (tsi_config->configMutual.commonConfig.cutoff > kTSI_SincCutoffDiv_0  && 
                        cycleCounter > 32 && cycleCounter <= 32+8)        /* Cut-off divider calibration, 8 values */
                {   tsi_config->configMutual.commonConfig.cutoff -= 1;                                
                    continue;                                                                        
                }                                                                                    
                else if (tsi_config->configMutual.commonConfig.chargeNum > kTSI_SscChargeNumValue_1  && 
                        cycleCounter > 32+8 && cycleCounter <= 32+8+16) /* SSC output bit0's period setting calibration, 16 values */
                {   tsi_config->configMutual.commonConfig.chargeNum -= 1;                            
                    continue;                                                                        
                }                                                                                    
                else if (tsi_config->configMutual.preResistor < kTSI_MutualPreResistor_8k && 
                        cycleCounter > 32+8+16 && cycleCounter <= 32+8+16+8)           /* Vref generator resistor  calibration, 8 values */
                {   tsi_config->configMutual.preResistor += 1;                                       
                    continue;                                                                        
                }                                                                                    
                else if (tsi_config->configMutual.commonConfig.dvolt < kTSI_DvoltOption_3  && 
                        cycleCounter > 32+8+16+8 && cycleCounter <= 32+8+16+8+4)           /* DVOLT option value calibration, 4 values */
                {   tsi_config->configMutual.commonConfig.dvolt += 1;                                 
                    continue;                                                                        
                }                                                                                    
                else if (tsi_config->configMutual.boostCurrent > kTSI_MutualSenseBoostCurrent_0uA  && 
                        cycleCounter > 32+8+16+8+4 && cycleCounter <= 32+8+16+8+4+32)   /* Sensitivity boost current setting, 32 values */
                {   tsi_config->configMutual.boostCurrent += 1;                                 
                    continue;
                }  
                else if (tsi_config->configMutual.pmosLeftCurrent > kTSI_MutualPmosCurrentMirrorLeft_4  && 
                        cycleCounter > 32+8+16+8+4+32 && cycleCounter <= 32+8+16+8+4+32+8)   /* Pmos current mirror on the left side, 8 values */
                {   tsi_config->configMutual.pmosLeftCurrent += 1;                                 
                    continue;
                }    
                else if (tsi_config->configMutual.pmosRightCurrent < kTSI_MutualPmosCurrentMirrorRight_4  && 
                        cycleCounter > 32+8+16+8+4+32+8 && cycleCounter <= 32+8+16+8+4+32+8+4) /* Pmos current mirror on the right side, 4 values */
                {   tsi_config->configMutual.pmosRightCurrent -= 1;                                 
                    continue;
                }
                else if (tsi_config->configMutual.commonConfig.order > kTSI_SincFilterOrder_1  && 
                        cycleCounter > 32+8+16+8+4+32+8+4 && cycleCounter <= 32+8+16+8+4+32+8+4+2)            /* SINC filter order, 2 values */
                {   tsi_config->configMutual.commonConfig.order -= 1;                                
                    continue;
                } 
                else if (cycleCounter > 0 && cycleCounter <= 32+8+16+8+4+32+8+4+2)
                {   continue; 
                }
                else 
                {   return kStatus_TSI_MutUntouchRecalibError;   /* no configuration setting available - go out from calibration */
                }
            }
            else if (maxElectVal > recalib_config->MutRangeMax)  /* untouched counter value is too high */
            {           
                if (tsi_config->configMutual.commonConfig.decimation > kTSI_SincDecimationValue_1 &&    
                    cycleCounter <= 32)                                         /* SINC decimation calibration, 32 values */
                {   tsi_config->configMutual.commonConfig.decimation -= 1;                           
                    continue;
                } 
                else if (tsi_config->configMutual.commonConfig.cutoff < kTSI_SincCutoffDiv_128 &&    
                         cycleCounter > 32 && cycleCounter <= 32+8)        /* Cut-off divider calibration, 8 values */
                {   tsi_config->configMutual.commonConfig.cutoff += 1;                               
                    continue;
                }                 
                else if (tsi_config->configMutual.commonConfig.chargeNum < kTSI_SscChargeNumValue_16 && 
                         cycleCounter > 32+8 && cycleCounter <= 32+8+16) /* SSC output bit0's period setting calibration, 16 values */      
                {   tsi_config->configMutual.commonConfig.chargeNum += 1;   
                    continue;
                }                                                                                 
                else if (tsi_config->configMutual.preResistor > kTSI_MutualPreResistor_1k &&   
                         cycleCounter > 32+8+16 && cycleCounter <= 32+8+16+8)           /* Vref generator resistor  calibration, 8 values */                        
                {   tsi_config->configMutual.preResistor -= 1;         
                    continue;
                }                                                                                 
                else if (tsi_config->configMutual.commonConfig.dvolt > kTSI_DvoltOption_0 &&   
                         cycleCounter > 32+8+16+8 && cycleCounter <= 32+8+16+8+4)           /* DVOLT option value calibration, 4 values */
                {   tsi_config->configMutual.commonConfig.dvolt -= 1;                                 
                    continue;
                }                                                                                 
                else if (tsi_config->configMutual.boostCurrent < kTSI_MutualSenseBoostCurrent_62uA &&   
                         cycleCounter > 32+8+16+8+4 && cycleCounter <= 32+8+16+8+4+32)   /* Sensitivity boost current setting, 32 values */
                {   tsi_config->configMutual.boostCurrent -= 1;                                 
                    continue;
                }  
                else if (tsi_config->configMutual.pmosLeftCurrent < kTSI_MutualPmosCurrentMirrorLeft_32 &&  
                         cycleCounter > 32+8+16+8+4+32 && cycleCounter <= 32+8+16+8+4+32+8)   /* Pmos current mirror on the left side, 8 values */                       
                {   tsi_config->configMutual.pmosLeftCurrent -= 1;                                 
                    continue;
                }    
                else if (tsi_config->configMutual.pmosRightCurrent > kTSI_MutualPmosCurrentMirrorRight_1 &&  
                         cycleCounter > 32+8+16+8+4+32+8 && cycleCounter <= 32+8+16+8+4+32+8+4) /* Pmos current mirror on the right side, 4 values */                         
                {   tsi_config->configMutual.pmosRightCurrent += 1;                                 
                    continue;
                }  
                else if (tsi_config->configMutual.commonConfig.order < kTSI_SincFilterOrder_2 &&  
                         cycleCounter > 32+8+16+8+4+32+8+4 && cycleCounter <= 32+8+16+8+4+32+8+4+2)            /* SINC filter order, 2 values */                     
                {   tsi_config->configMutual.commonConfig.order += 1;                                
                    continue;
                }
                else if (cycleCounter > 0 && cycleCounter <= 32+8+16+8+4+32+8+4+2)
                {   continue; 
                }
                else 
                {   return kStatus_TSI_MutUntouchRecalibError;   /* no configuration setting available - go out from calibration */
                }
            }    
        } while ((minElectVal < recalib_config->MutRangeMin) || (maxElectVal > recalib_config->MutRangeMax));     /* next calibration cycle required, TSI setting changed */
        
        /* sensitivity calibration for mutual-cap mode, it requests one or more touches on mutual-cap electrode */
        tsi_config->configMutual.enableNmosMirror = true;
        cycleCounter = 0;
        do
        { 
            result = NT_TSI_DRV_SensitivityMeasurement(instance, &minElectVal, &maxElectVal, enabledElectrodes);            
            if (result !=  kStatus_TSI_Success || minElectVal == 65535 || maxElectVal == 0)
            {   
                return kStatus_TSI_MutSensitivRecalibError;
            } 

            cycleCounter++;
            
            if (minElectVal < recalib_config->MutRangeDeltaMin)
            {           
                tsi_config->configMutual.enableNmosMirror = true;
                if (tsi_config->configMutual.boostCurrent < kTSI_MutualSenseBoostCurrent_62uA && 
                   cycleCounter <= 32)  /* Sensitivity boost current calibration, 32 values  */
                {   tsi_config->configMutual.boostCurrent += 1;             
                    continue;
                }
                if (tsi_config->configMutual.commonConfig.decimation > kTSI_SincDecimationValue_1 &&    
                         cycleCounter > 32 && cycleCounter <= 32+32)   /* SINC decimation calibration, 32 values  */
                {   tsi_config->configMutual.commonConfig.decimation -= 1;          
                    continue;
                }            
                else if (tsi_config->configMutual.commonConfig.cutoff > kTSI_SincCutoffDiv_0 &&    
                         cycleCounter > 32+32 && cycleCounter <= 32+32+8)        /* Cut-off divider, 8 calibration values  */
                {   tsi_config->configMutual.commonConfig.cutoff -= 1;                                
                    continue;
                }                 
                else if (cycleCounter > 0 && cycleCounter <= 32+32+8)
                {   continue; 
                }
                else 
                {   return kStatus_TSI_MutSensitivRecalibError;   /* no configuration setting available - go out from calibration */
                }
            } 
            else if (maxElectVal > recalib_config->MutRangeDeltaMax) 
            {                    
                tsi_config->configMutual.enableNmosMirror = true;
                if (tsi_config->configMutual.boostCurrent > kTSI_MutualSenseBoostCurrent_0uA &&  
                    cycleCounter <= 32)  /* Sensitivity boost current calibration, 32 values  */
                {  tsi_config->configMutual.boostCurrent -= 1;             
                    continue;
                } 
                else if (tsi_config->configMutual.commonConfig.decimation < kTSI_SincDecimationValue_32 && 
                         cycleCounter > 32 && cycleCounter <= 32+32)   /* SINC decimation calibration, 32 values  */
                {   tsi_config->configMutual.commonConfig.decimation += 1;                           
                    continue;
                }                                                                                                                                                              
                else if (tsi_config->configMutual.commonConfig.cutoff < kTSI_SincCutoffDiv_128 &&   
                         cycleCounter > 32+32 && cycleCounter <= 32+32+8)        /* Cut-off divider, 8 calibration values  */
                {   tsi_config->configMutual.commonConfig.cutoff += 1;                               
                    continue;
                } 
                else if (cycleCounter > 0 && cycleCounter <= 32+32+8)
                {   continue; 
                }
                else 
                {   return kStatus_TSI_MutSensitivRecalibError;   /* no configuration setting available - go out from calibration */
                }
            }
        } while ((minElectVal < recalib_config->MutRangeDeltaMin) || (maxElectVal > recalib_config->MutRangeDeltaMax));  
    }
 
    tsiState->status = kStatus_TSI_Initialized;

    /* OS: End of critical section. */
    NT_OSA_MutexUnlock(&tsiState->lock);

    if(tsiState->opModesData[tsiState->opMode].enabledElectrodes == 0)
    {
        return kStatus_TSI_Error;
    }
    else
    {
        return kStatus_TSI_Success;
    }

}
#endif /* NT_SAFETY_SUPPORT */
#endif
/*******************************************************************************
 * EOF
 ******************************************************************************/

