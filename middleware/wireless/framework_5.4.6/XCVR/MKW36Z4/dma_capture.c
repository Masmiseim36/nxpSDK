/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#include "fsl_common.h"
#include "fsl_xcvr.h"
#include "fsl_edma.h"
#include "fsl_dmamux.h"
#include "dma_capture.h"
#include "math.h"
#include "mathfp.h" /* custom fixed point math for atan2 function */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1)
#define PKT_RAM_SIZE_16B_WORDS  (1152)  /* Number of 16bit entries in each Packet RAM */
#else
#define PKT_RAM_SIZE_16B_WORDS  (544)   /* Number of 16bit entries in each Packet RAM */
#endif

#define SIGN_EXTND_12_16(x)     ((x) | (((x) & 0x800U) ? 0xF000U : 0x0U))
#define SIGN_EXTND_5_8(x)       ((x) | (((x) & 0x10U) ? 0xE0U : 0x0U))

/* Store look up table of OSR values used for each DMA capture page */
static const dma_capture_lut_t dma_table [DMA_PAGE_MAX] = 
{
  /* DMA_PAGE               OSR */
  {DMA_PAGE_IDLE,         0xFU},
  {DMA_PAGE_RXDIGIQ,      0x4U},
  {DMA_PAGE_RXDIGI,       0x2U},
  {DMA_PAGE_RXDIGQ,       0x2U},
  {DMA_PAGE_RAWADCIQ,     0x2U},
  {DMA_PAGE_RAWADCI,      0x1U},
  {DMA_PAGE_RAWADCQ,      0x1U},
  {DMA_PAGE_DCESTIQ,      0x4U},
  {DMA_PAGE_DCESTI,       0x2U},
  {DMA_PAGE_DCESTQ,       0x2U},
  {DMA_PAGE_RXINPH,       0x1U},
  {DMA_PAGE_DEMOD_HARD,   0xFU},
  {DMA_PAGE_DEMOD_SOFT,   0xFU},
  {DMA_PAGE_DEMOD_DATA,   0xFU},
  {DMA_PAGE_DEMOD_CFO_PH, 0xFU},  
};

#if RADIO_IS_GEN_3P0
#define RADIO_DMAMUX_CHAN   (8U)
#define NUM_DMA_CHANNELS    (8U) /* on DMA1 */
#else
#define RADIO_DMAMUX_CHAN   (11U)
#define NUM_DMA_CHANNELS    (4U) /* on DMA1 */
#endif


/*******************************************************************************
 * Variables
 ******************************************************************************/

/* DMA capture configuration struct */
static dma_capture_config_t dma_config;
static bool osr_is_locked = 0;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
 /* common code for DMA init to support OSR locking option */
dmaStatus_t dma_init_common(uint8_t dma_channel_num, dma_capture_callback dma_callback, void *user_data);
void EDMA_Callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds);

/*******************************************************************************
 * Code
 ******************************************************************************/

/* User callback function for EDMA transfer. */
void EDMA_Callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
    if (transferDone) /* Only call routine if transfer is complete */
    {
        dma_config.g_Transfer_Done = true;
        
        if (dma_config.user_callback != NULL)
        {
            dma_config.user_callback(handle, param, transferDone, tcds);
        }
    }
}

dmaStatus_t dma_init(uint8_t dma_channel_num, dma_capture_callback dma_callback, void *user_data)
{
    osr_is_locked = 0; /* This version of the dma_init allows OSR to change as needed for proper DMA capture */
    return dma_init_common(dma_channel_num, dma_callback, user_data);
}

dmaStatus_t dma_init_lock_osr(uint8_t dma_channel_num, dma_capture_callback dma_callback, void *user_data)
{
    osr_is_locked = 1;  /* This version of the dma_init prevents the OSR from changing; This may result in less than complete sample capture */ 
    return dma_init_common(dma_channel_num, dma_callback, user_data);
}

dmaStatus_t dma_init_common(uint8_t dma_channel_num, dma_capture_callback dma_callback, void *user_data)
{   
    dmaStatus_t status = DMA_SUCCESS;

    /* EDMA peripheral is the same on KW35 and K3 but the DMAMUX is assigned differently */
#if RADIO_IS_GEN_3P0
    DMAMUX_Type *dmamux_base_addr = DMAMUX1;
    DMA_Type *dma_base_addr = DMA1;
#else
    DMAMUX_Type *dmamux_base_addr = DMAMUX0;
    DMA_Type *dma_base_addr = DMA0;
#endif

    if (dma_channel_num < NUM_DMA_CHANNELS)
    {
        /* Configure DMA handler */
        dma_config.osr_temp = 0xF;
        dma_config.g_Transfer_Done = false;
        dma_config.user_callback = dma_callback;
        
        /* Configure DMAMUX */
        DMAMUX_Init(dmamux_base_addr);
        DMAMUX_SetSource(dmamux_base_addr, dma_channel_num, RADIO_DMAMUX_CHAN);
        DMAMUX_EnableChannel(dmamux_base_addr, dma_channel_num);
        
        /* Configure EDMA transfer */
        EDMA_GetDefaultConfig(&dma_config.dmaConfig);
        dma_config.dmaConfig.enableDebugMode = 1;
        EDMA_Init(dma_base_addr, &dma_config.dmaConfig);
        EDMA_CreateHandle(&dma_config.g_EDMA_Handle, dma_base_addr, dma_channel_num);
        EDMA_SetCallback(&dma_config.g_EDMA_Handle, EDMA_Callback, user_data);
        
        /* Turns on clocking to DMA/DBG blocks */
        XCVR_RX_DIG->RX_DIG_CTRL |= XCVR_RX_DIG_RX_DIG_CTRL_RX_DMA_DTEST_EN_MASK;
#if !RADIO_IS_GEN_2P0
        /* Gen 2.0 also requires DMA_EN to be set */
        XCVR_MISC->DMA_CTRL |= XCVR_CTRL_DMA_CTRL_DMA_EN_MASK; 
#endif
        /* Save current osr config */
        dma_config.osr_temp = (XCVR_RX_DIG->RX_DIG_CTRL & XCVR_RX_DIG_RX_DIG_CTRL_RX_DEC_FILT_OSR_MASK) >> XCVR_RX_DIG_RX_DIG_CTRL_RX_DEC_FILT_OSR_SHIFT;

        /* Some external code must perform the RX warmup request */

    }
    else
    {
        status = DMA_FAIL_INVALID_CHANNEL;
    }

    return status;
    
}

dmaStatus_t dma_wait_for_complete(void)
{
    dmaStatus_t status = DMA_SUCCESS;
    
    /* Wait for EDMA transfer finish */
    while (dma_config.g_Transfer_Done != true)
    {
        /* Timeout */
        if (XCVR_MISC->DMA_CTRL & XCVR_CTRL_DMA_CTRL_DMA_TIMED_OUT_MASK)
        {
            /* Clear flag */
            XCVR_MISC->DMA_CTRL |= XCVR_CTRL_DMA_CTRL_DMA_TIMED_OUT_MASK;

            /* timeout */
            status = DMA_CAPTURE_TIMEOUT;
            
            break;
        }
    }
    
    /* Release DMA module */
    dma_release();
    
    return status;
}

dmaStatus_t dma_poll_capture_status(void)
{
  
    dmaStatus_t status = DMA_CAPTURE_NOT_COMPLETE;
  
    if (dma_config.g_Transfer_Done != true)
    {
        /* Timeout */
        if (XCVR_MISC->DMA_CTRL & XCVR_CTRL_DMA_CTRL_DMA_TIMED_OUT_MASK)
        {
            /* Clear flag */
            XCVR_MISC->DMA_CTRL |= XCVR_CTRL_DMA_CTRL_DMA_TIMED_OUT_MASK;
            /* Release DMA module */
            dma_release();
            /* timeout */
            status = DMA_CAPTURE_TIMEOUT;
        }
    }
    else
    {
        status = DMA_SUCCESS;
    }
  
    return status;
}

void dma_abort(void)
{
    /* Use the EDMA driver to abort the ongoing transfer */
    EDMA_StopTransfer(&dma_config.g_EDMA_Handle);
}

void dma_release(void)
{   
#if RADIO_IS_GEN_3P0
    DMAMUX_Type *dmamux_base_addr = DMAMUX1;
    DMA_Type *dma_base_addr = DMA1;
#else
    DMAMUX_Type *dmamux_base_addr = DMAMUX0;
    DMA_Type *dma_base_addr = DMA0;
#endif

    /* Restart flag */
    dma_config.g_Transfer_Done = false;

    /* Shutdown eDMA and DMAMUX */
    EDMA_StopTransfer(&dma_config.g_EDMA_Handle);
    EDMA_Deinit(dma_base_addr);
    DMAMUX_DisableChannel(dmamux_base_addr, dma_config.g_EDMA_Handle.channel);
    
    /* Disable DMA */
#if RADIO_IS_GEN_2P0
    XCVR_MISC->DMA_CTRL &= ~(XCVR_CTRL_DMA_CTRL_DMA_PAGE_MASK); 
#else
    XCVR_MISC->DMA_CTRL &= ~(XCVR_CTRL_DMA_CTRL_DMA_PAGE_MASK | XCVR_CTRL_DMA_CTRL_DMA_EN_MASK); 
#endif
    XCVR_RX_DIG->RX_DIG_CTRL &= ~XCVR_RX_DIG_RX_DIG_CTRL_RX_DMA_DTEST_EN_MASK;
    
    /* Gasket bypass disable */
    RSIM->CONTROL &= ~(RSIM_CONTROL_RADIO_GASKET_BYPASS_OVRD_EN_MASK | 
                       RSIM_CONTROL_RADIO_GASKET_BYPASS_OVRD_MASK); 

    /* Restore previous osr config (if any) */
    if (!osr_is_locked)
    {
        uint32_t temp_reg = XCVR_RX_DIG->RX_DIG_CTRL;
        temp_reg &= ~XCVR_RX_DIG_RX_DIG_CTRL_RX_DEC_FILT_OSR_MASK;
        temp_reg |= XCVR_RX_DIG_RX_DIG_CTRL_RX_DEC_FILT_OSR(dma_config.osr_temp);
        XCVR_RX_DIG->RX_DIG_CTRL = temp_reg;
        dma_config.osr_temp = 0xF;
    }

    osr_is_locked = 0; /* reset the lock flag to OFF */
}

#if (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1)
dmaStatus_t dma_start_capture(uint8_t dma_page, uint32_t buffer_sz_bytes, void * result_buffer, dmaStartTriggerType start_trig)
{
    dmaStatus_t status = DMA_SUCCESS;
    uint8_t new_osr = 0;
    uint32_t byte_per_req = 0;

    /* Some external code must perform the RX warmup request after the d_init() call for SW triggered DMA. */
    /* if hardware triggered DMA is being used then RX warmup should typically be AFTER dma_start_capture() is completed. */
    if (result_buffer == NULL)
    {
        status = DMA_FAIL_NULL_POINTER;
    }
    else
    {
        edma_transfer_config_t transferConfig;
           
        switch (dma_page)
        {
            case DMA_PAGE_RXDIGIQ:
            case DMA_PAGE_RAWADCIQ:
            case DMA_PAGE_DCESTIQ:
            case DMA_PAGE_RXINPH:
            case DMA_PAGE_DEMOD_CFO_PH:
                new_osr = dma_table[dma_page].osr;
                
                /* Set OSR */
                if (!osr_is_locked) /* the choice of dma_int() vs. dma_init_lock_osr() determines whether or not to override the OSR */
                {
                    uint32_t temp_reg = XCVR_RX_DIG->RX_DIG_CTRL;
                    temp_reg &= ~XCVR_RX_DIG_RX_DIG_CTRL_RX_DEC_FILT_OSR_MASK;
                    temp_reg |= XCVR_RX_DIG_RX_DIG_CTRL_RX_DEC_FILT_OSR(new_osr); 
                    XCVR_RX_DIG->RX_DIG_CTRL = temp_reg;
                }

                /* Single request mode */
#if (0)
                XCVR_MISC->DMA_CTRL |= XCVR_CTRL_DMA_CTRL_SINGLE_REQ_MODE_MASK;
                XCVR_MISC->DMA_CTRL &= ~XCVR_CTRL_DMA_CTRL_BYPASS_DMA_SYNC_MASK;
                byte_per_req = buffer_sz_bytes;
#else           /* No single request mode */
                XCVR_MISC->DMA_CTRL &= ~XCVR_CTRL_DMA_CTRL_SINGLE_REQ_MODE_MASK;
                //XCVR_MISC->DMA_CTRL |= XCVR_CTRL_DMA_CTRL_BYPASS_DMA_SYNC_MASK;
                byte_per_req = sizeof(uint32_t);
#endif

                if (new_osr < 0x8U) /* need gasket bypass when using 2MHz DMA request rate */
                {
                    /* Gasket bypass enable */
                    RSIM->CONTROL |= (RSIM_CONTROL_RADIO_GASKET_BYPASS_OVRD_EN_MASK | 
                                      RSIM_CONTROL_RADIO_GASKET_BYPASS_OVRD_MASK);
                }
                else
                {
                    /* Gasket bypass disable */
                    RSIM->CONTROL &= ~(RSIM_CONTROL_RADIO_GASKET_BYPASS_OVRD_EN_MASK | 
                                       RSIM_CONTROL_RADIO_GASKET_BYPASS_OVRD_MASK); 
                }
              break;
        
            case DMA_PAGE_DEMOD_HARD:
            case DMA_PAGE_DEMOD_SOFT:
            case DMA_PAGE_DEMOD_DATA:
                
                /* Data rate is too low to use Single Req mode */
            
                /* Request per sample mode */
                XCVR_MISC->DMA_CTRL &= XCVR_CTRL_DMA_CTRL_SINGLE_REQ_MODE_MASK;
                XCVR_MISC->DMA_CTRL |= XCVR_CTRL_DMA_CTRL_BYPASS_DMA_SYNC_MASK;
                byte_per_req = sizeof(uint32_t);
                
                /* Gasket bypass enable */
                RSIM->CONTROL |= (RSIM_CONTROL_RADIO_GASKET_BYPASS_OVRD_EN_MASK | 
                                  RSIM_CONTROL_RADIO_GASKET_BYPASS_OVRD_MASK);
                break;
                
            case DMA_PAGE_IDLE:
            default:    
                /* Illegal capture page request */
                status = DMA_FAIL_PAGE_ERROR;
                break;
        }

        if (status == DMA_SUCCESS)
        {
            /* setup start trigger if applicable */
            switch (start_trig)
            {
                case NO_DMA_START_TRIG:
                case START_DMA_ON_FSK_PREAMBLE_FOUND:
                case START_DMA_ON_FSK_AA_MATCH:
#if !RADIO_IS_GEN_2P1
                case START_DMA_ON_ZBDEMOD_PREAMBLE_FOUND:
                case START_DMA_ON_ZBDEMOD_SFD_MATCH:
#endif
                case START_DMA_ON_AGC_DCOC_GAIN_CHG:
                case START_DMA_ON_TSM_RX_DIG_EN:
                case START_DMA_ON_TSM_SPARE2_EN:
                    XCVR_MISC->DMA_CTRL &= ~XCVR_CTRL_DMA_CTRL_DMA_START_TRG_MASK;
                    XCVR_MISC->DMA_CTRL |=  XCVR_CTRL_DMA_CTRL_DMA_START_TRG(start_trig);
                    break;
                case INVALID_DMA_START_TRIG:
#if RADIO_IS_GEN_2P1
                case START_DMA_ON_ZBDEMOD_PREAMBLE_FOUND:
                case START_DMA_ON_ZBDEMOD_SFD_MATCH:
#endif
                default:
                    status = DMA_INVALID_TRIG_SETTING;
                    break;
            }
        }
        
        if (status == DMA_SUCCESS)
        {
            uint32_t dma_data_ptr = (uint32_t)&XCVR_MISC->DMA_DATA;

            /* Select DMA_page */
            XCVR_MISC->DMA_CTRL &=  ~XCVR_CTRL_DMA_CTRL_DMA_PAGE_MASK;
            XCVR_MISC->DMA_CTRL |=  XCVR_CTRL_DMA_CTRL_DMA_PAGE(dma_page);

            /* EDMA transfer config */
            EDMA_PrepareTransfer(&transferConfig, /* Config */
                                 (void *)dma_data_ptr, /* Source address */
                                 sizeof(uint32_t), /* Source address width(bytes) */
                                 result_buffer, /* Destination address */
                                 4, /* Destination address width */
                                 byte_per_req, /* Transfer bytes per channel request */
                                 buffer_sz_bytes, /* Bytes to be transferred */
                                 kEDMA_PeripheralToMemory); /* Transfer type */
            
            EDMA_SubmitTransfer(&dma_config.g_EDMA_Handle, &transferConfig);
            EDMA_StartTransfer(&dma_config.g_EDMA_Handle);
        }
    
    }

    return status;  

}

#else
dmaStatus_t dma_capture(uint8_t dma_page, uint16_t buffer_sz_bytes, uint32_t * result_buffer)
{
    dmaStatus_t status = DMA_SUCCESS;

    /* Some external code must perform the RX warmup request after the d_init() call. */
    if (result_buffer == NULL)
    {
        status = DMA_FAIL_NULL_POINTER;
    }
    else
    {
        if (buffer_sz_bytes > (4096))
        {
            status = DMA_FAIL_SAMPLE_NUM_LIMIT;
        }
        else
        {
            uint32_t temp = XCVR_MISC->DMA_CTRL & ~XCVR_CTRL_DMA_CTRL_DMA_PAGE_MASK;
            edma_transfer_config_t transferConfig;
            
            /* Wait for Network Address Match to start capturing */
            /* while (!((XCVR_MISC->DMA_CTRL & XCVR_CTRL_D  MA_CTRL_DMA_TRIGGERRED_MASK))); */
            
            switch (dma_page)
            {
            case DMA_PAGE_RXDIGIQ:
            case DMA_PAGE_RAWADCIQ:
            case DMA_PAGE_DCESTIQ:
            case DMA_PAGE_RXINPH:
            case DMA_PAGE_DEMOD_CFO_PH:

                /* Set OSR */
                if (!osr_is_locked) /* the choice of dma_int() vs. dma_init_lock_osr() determines whether or not to override the OSR */
                {
                    XCVR_RX_DIG->RX_DIG_CTRL &= ~XCVR_RX_DIG_RX_DIG_CTRL_RX_DEC_FILT_OSR_MASK;
                    XCVR_RX_DIG->RX_DIG_CTRL |= XCVR_RX_DIG_RX_DIG_CTRL_RX_DEC_FILT_OSR(dma_table[dma_page].osr); 
                }

                /* Single request mode */
                XCVR_MISC->DMA_CTRL |= XCVR_CTRL_DMA_CTRL_SINGLE_REQ_MODE_MASK;
                XCVR_MISC->DMA_CTRL &= ~XCVR_CTRL_DMA_CTRL_BYPASS_DMA_SYNC_MASK;
                
                /* Gasket bypass disable */
                RSIM->CONTROL &= ~(RSIM_CONTROL_RADIO_GASKET_BYPASS_OVRD_EN_MASK | 
                                   RSIM_CONTROL_RADIO_GASKET_BYPASS_OVRD_MASK); 
                break;

            case DMA_PAGE_DEMOD_HARD:
            case DMA_PAGE_DEMOD_SOFT:
            case DMA_PAGE_DEMOD_DATA:

                /* Data rate is too low to use Single Req mode */
                
                /* Request per sample mode */
                XCVR_MISC->DMA_CTRL &= XCVR_CTRL_DMA_CTRL_SINGLE_REQ_MODE_MASK;
                XCVR_MISC->DMA_CTRL |= XCVR_CTRL_DMA_CTRL_BYPASS_DMA_SYNC_MASK;
                
                /* Gasket bypass enable */
                RSIM->CONTROL |= (RSIM_CONTROL_RADIO_GASKET_BYPASS_OVRD_EN_MASK | 
                                  RSIM_CONTROL_RADIO_GASKET_BYPASS_OVRD_MASK);
                break;
                
            case DMA_PAGE_IDLE:
            default:    
                /* Illegal capture page request */
                status = DMA_FAIL_PAGE_ERROR;
                break;
            }
            
            if (status == DMA_SUCCESS)
            {
                uint32_t dma_data_ptr = (uint32_t)&XCVR_MISC->DMA_DATA;
                /* Select DMA_page */
                XCVR_MISC->DMA_CTRL =  temp | XCVR_CTRL_DMA_CTRL_DMA_PAGE(dma_page);

                /* EDMA transfer config */
                EDMA_PrepareTransfer(&transferConfig, /* Config */
                                     (void *)dma_data_ptr, /* Source address */
                                     sizeof(uint32_t), /* Source address width(bytes) */
                                     result_buffer, /* Destination address */
                                     sizeof(*result_buffer), /* Destination address width */
                                     sizeof(uint32_t), /* Transfer bytes per channel request */
                                     buffer_sz_bytes, /* Bytes to be transferred */
                                     kEDMA_PeripheralToMemory); /* Transfer type */
                
                EDMA_SubmitTransfer(&dma_config.g_EDMA_Handle, &transferConfig);
                EDMA_StartTransfer(&dma_config.g_EDMA_Handle);
            }
        }
    }

    return status;	
}
#endif /* (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1) */

void sign_extend_12_16(uint16_t * buffer_ptr, uint32_t count)
{
    /* Make sure the buffer pointer is not NULL */
    if (buffer_ptr != NULL)
    {
        uint32_t i;
        uint16_t * sample_ptr = buffer_ptr; /* Input is a buffer of bytes */
        for (i=0;i<count;i++)
        {
            /* Perform in-place sign extension on each sample */
            *sample_ptr = SIGN_EXTND_12_16((uint16_t)*sample_ptr);
            sample_ptr++; /* Processing one byte at a time */
        }
    }
}

void sign_extend_5_8(uint8_t * buffer_ptr, uint32_t count)
{
    /* Make sure the buffer pointer is not NULL */
    if (buffer_ptr != NULL)
    {
        uint32_t i;
        uint8_t * sample_ptr = buffer_ptr; /* Input is a buffer of bytes */
        for (i=0;i<count;i++)
        {
            /* Perform in-place sign extension on each sample */
            *sample_ptr = SIGN_EXTND_5_8((uint8_t)*sample_ptr);
            sample_ptr++; /* Processing one byte at a time */
        }
    }
}

void iq_to_phase_float(uint16_t * iq_buffer, float * phase_buffer, uint16_t num_points)
{
    /* Make sure the IQ and PHASE buffer pointers are not NULL */
    if ((iq_buffer != NULL) &&
        (phase_buffer != NULL))
    {
        uint16_t i;
        static int16_t i_sample, q_sample;
        /* Point to input and output buffers in memory */
        uint16_t * in_ptr = iq_buffer;  /* IQ are input */
        float * out_ptr = phase_buffer; /* Floating point phase is ouput */
        static float result = 0;
        for (i=0;i<num_points;i++)
        {
            /* I and Q samples are stored in alternating halves of a 32 bit word */
            i_sample = *in_ptr;
            q_sample = *(in_ptr+1);
            /* Perform atan using math library atan2 routine */
            result = (float)atan2((double)q_sample,(double)i_sample);
            *out_ptr++ = result;
            in_ptr += 2; /* Increment by 2 since we consumed two samples */
        }
    }
}

void iq_to_phase_fixedpt(uint16_t * iq_buffer, uint16_t * phase_buffer, uint16_t num_points)
{
    /* Make sure the IQ and PHASE buffer pointers are not NULL */
    if ((iq_buffer != NULL) &&
        (phase_buffer != NULL))
    {
        uint16_t i;
        static int16_t i_sample, q_sample;
        /* Point to input and output buffers in memory */
        uint16_t * in_ptr = iq_buffer;   /* IQ are input */
        uint16_t * out_ptr = phase_buffer; /* Fixed point phase (stored in a uint16_t) is ouput */
        static int16_t result = 0;
        for (i=0;i<num_points;i++)
        {
            /* I and Q samples are stored in alternating halves of a 32 bit word */
            i_sample = *in_ptr;
            q_sample = *(in_ptr+1);
            /* Perform atan using custom fixed point approximation routine */
            result = atan2fp(q_sample,i_sample);
            *out_ptr++ = result;
            in_ptr += 2; /* Increment by 2 since we consumed two samples */
        }
    }
}

