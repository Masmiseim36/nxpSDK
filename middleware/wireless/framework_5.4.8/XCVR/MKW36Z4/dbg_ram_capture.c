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
#include "dbg_ram_capture.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1)
#define PKT_RAM_SIZE_16B_WORDS  (1152U) /* Number of 16bit entries in each Packet RAM bank*/
#else
#define PKT_RAM_SIZE_16B_WORDS  (544U) /* Number of 16bit entries in each Packet RAM bank */
#endif /* (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1) */
#define SIGN_EXTND_12_16(x)     ((x) | (((x) & 0x800) ? 0xF000 : 0x0))
#define SIGN_EXTND_5_8(x)       ((x) | (((x) & 0x10) ? 0xE0 : 0x0))

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#if (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1)
dbgRamStatus_t unpack_sequential_data(uint8_t pkt_ram_number, uint16_t pkt_ram_location, uint16_t num_bytes, void * result_buffer);
dbgRamStatus_t unpack_simul_data(uint16_t pkt_ram_location, uint16_t num_bytes, void * result_buffer);
dbgRamStatus_t dbg_ram_set_trigger(dbgRamStartTriggerType start_trig, dbgRamStopTriggerType stop_trig);
#endif /* (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1) */

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t simul_capture_flag; /* captured data is simultaneous */
static uint8_t sequ_capture_flag; /* captured data is sequential capture */

/*******************************************************************************
 * Code
 ******************************************************************************/
void dbg_ram_init(void)
{
    XCVR_RX_DIG->RX_DIG_CTRL |= XCVR_RX_DIG_RX_DIG_CTRL_RX_DMA_DTEST_EN_MASK; /* Turns on clocking to DMA/DBG blocks */
    XCVR_MISC->PACKET_RAM_CTRL |= XCVR_CTRL_PACKET_RAM_CTRL_XCVR_RAM_ALLOW_MASK /* Make PKT RAM available to just XCVR */
#if (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1)
                                | XCVR_CTRL_PACKET_RAM_CTRL_DBG_EN_MASK
#endif /* (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1) */
                                ;
    /* init capture flags to false */
    simul_capture_flag = 0; 
    sequ_capture_flag = 0; 
    
    /* Some external code must perform the RX warmup request */
}

void dbg_ram_release(void)
{
    XCVR_MISC->PACKET_RAM_CTRL &= ~XCVR_CTRL_PACKET_RAM_CTRL_DBG_PAGE_MASK; /* force to idle */
    XCVR_MISC->PACKET_RAM_CTRL &= ~(XCVR_CTRL_PACKET_RAM_CTRL_XCVR_RAM_ALLOW_MASK /* Make PKT RAM available to protocol blocks */
#if (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1)
                                    | XCVR_CTRL_PACKET_RAM_CTRL_DBG_EN_MASK  /* turn off packet RAM debug */
#endif /* (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1) */
                                    );
                                      
    XCVR_RX_DIG->RX_DIG_CTRL &= ~XCVR_RX_DIG_RX_DIG_CTRL_RX_DMA_DTEST_EN_MASK; /* Turns off clocking to DMA/DBG blocks */

    /* Some external code must perform the RX warmdown request */
}


#if (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1)
/*! *********************************************************************************
* \brief  Generic routine to unpack sequentially stored data from PKT_RAM to an output buffer.
*
* \param[in] pkt_ram_number - The packet RAM 0 or 1 where the unpack should be started (only values of 0 or 1 are allowed)
* \param[in] pkt_ram_location - The packet RAM location where the unpack should be started (0...PKT_RAM_SIZE_16B_WORDS, in 16 bit words)
* \param[in] num_bytes - The number of bytes to be unpacked (in bytes)
* \param[in] result_buffer - The pointer to the output buffer of a size large enough for the samples.
*
* \return Result of the unpack.
*
* \details 
* This routine starts at the designated location in the specified packet RAM bank (0/1) and unpacks until it has
* processed the required number of bytes. It makes no inferences about the validity of the unpacked data, that is
* assumed to be handled outside this routine. It does NOT handle crossing boundaries, it is intended to be called
* multiple times to implement wraparound and boundary crossings between the two banks of packet RAM.
***********************************************************************************/
dbgRamStatus_t unpack_sequential_data(uint8_t pkt_ram_number, uint16_t pkt_ram_location, uint16_t num_bytes, void * result_buffer)
{
    dbgRamStatus_t status = DBG_RAM_SUCCESS;
    uint32_t temp;
    uint16_t i;
    volatile uint8_t *pkt_ram_ptr0;
    uint8_t * output_ptr;
    if ((pkt_ram_location > PKT_RAM_SIZE_16B_WORDS) || /* Location is outside of packet ram number of words */
        (num_bytes > (PKT_RAM_SIZE_16B_WORDS*2U)) || /* Impossible number of bytes */
        (num_bytes > (2U*(PKT_RAM_SIZE_16B_WORDS-pkt_ram_location))) || /* Requested # of bytes goes past end of packet RAM */
        (pkt_ram_number >1U) /* Invalid packet RAM bank number  */
        )
    {
        status = DBG_RAM_FAIL_SAMPLE_NUM_LIMIT;
    }
    else
    {
        if (result_buffer == NULL)
        {
            status = DBG_RAM_FAIL_NULL_POINTER;
        }
        else
        {
            /* Setup the input and output buffers */
            output_ptr = result_buffer;
            pkt_ram_ptr0 = (volatile uint8_t *)XCVR_PKT_RAM_BASE; /* Address is same for bank 0 or 1 */
            temp = XCVR_MISC->PACKET_RAM_CTRL;
            temp &= ~XCVR_CTRL_PACKET_RAM_CTRL_XCVR_RAM_PAGE_MASK;
            temp |= XCVR_CTRL_PACKET_RAM_CTRL_XCVR_RAM_PAGE(pkt_ram_number);
            XCVR_MISC->PACKET_RAM_CTRL = temp;
            /* Copy out the required number of bytes */
            for (i=0;i<num_bytes;i++)
            {
                *output_ptr = *pkt_ram_ptr0;
                pkt_ram_ptr0++;
                output_ptr++;
            }
        }
    }
    return status;
}

/*! *********************************************************************************
* \brief  Generic routine to unpack simultaneously stored data to an output buffer.
*
* \param[in] pkt_ram_location - The packet RAM location where the unpack should be started (0...PKT_RAM_SIZE_16B_WORDS, in 16 bit words)
* \param[in] num_bytes - The number of bytes to be unpacked (in bytes). Must be a multiple of 4. If not, up to 3 bytes may not be copied.
* \param[in] result_buffer - The pointer to the output buffer of a size large enough for the samples.
*
* \return Result of the unpack.
*
* \details 
***********************************************************************************/
dbgRamStatus_t unpack_simul_data(uint16_t pkt_ram_location, uint16_t num_bytes, void * result_buffer)
{
    dbgRamStatus_t status = DBG_RAM_SUCCESS;
    uint32_t xcvr_ctrl_pkt_ram_0_sel;
    uint32_t xcvr_ctrl_pkt_ram_1_sel;
    volatile uint8_t *pkt_ram_ptr0, *pkt_ram_ptr1; /* Pointers to both packet RAMs */
    uint8_t * output_ptr;
    uint16_t i;
    if ((pkt_ram_location > PKT_RAM_SIZE_16B_WORDS) || /* Location is outside of packet ram number of words */
        (num_bytes > PKT_RAM_SIZE_16B_WORDS*2U*2U) || /* Impossible number of bytes */
        (num_bytes > (2U*2U*(PKT_RAM_SIZE_16B_WORDS-pkt_ram_location))) /* Requested # of bytes goes past end of packet RAM */
        )
    {
        status = DBG_RAM_FAIL_SAMPLE_NUM_LIMIT;
    }
    else
    {
        output_ptr = result_buffer;
        /* packet RAMs are overlaid in the same memory space for Gen2.1 and 3.0 radios */
        pkt_ram_ptr0 = (volatile uint8_t *)XCVR_PKT_RAM_BASE;
        pkt_ram_ptr1 = (volatile uint8_t *)XCVR_PKT_RAM_BASE;
        xcvr_ctrl_pkt_ram_0_sel = XCVR_MISC->PACKET_RAM_CTRL & ~XCVR_CTRL_PACKET_RAM_CTRL_XCVR_RAM_PAGE_MASK;
        xcvr_ctrl_pkt_ram_1_sel = XCVR_MISC->PACKET_RAM_CTRL | XCVR_CTRL_PACKET_RAM_CTRL_XCVR_RAM_PAGE_MASK;
        // for *IQ pages I and Q are stored alternately in packet ram 0 & 1
        for (i=0;i<(num_bytes/4U)-1U;i++)
        {
            /* Bank 0 of packet RAM */
            XCVR_MISC->PACKET_RAM_CTRL = xcvr_ctrl_pkt_ram_0_sel;
            *output_ptr++ = *pkt_ram_ptr0++;
            *output_ptr++ = *pkt_ram_ptr0++;

            /* Bank 1 of packet RAM */
            XCVR_MISC->PACKET_RAM_CTRL = xcvr_ctrl_pkt_ram_1_sel;
            *output_ptr++ = *pkt_ram_ptr1++;
            *output_ptr++ = *pkt_ram_ptr1++;
        }
        XCVR_MISC->PACKET_RAM_CTRL = xcvr_ctrl_pkt_ram_0_sel; /* leave selection set to bank 0 of packet RAM */
    }
    return status;
}

dbgRamStatus_t dbg_ram_set_trigger(dbgRamStartTriggerType start_trig, dbgRamStopTriggerType stop_trig)
{
    dbgRamStatus_t status = DBG_RAM_SUCCESS;
    uint32_t temp;
    /* Check that not trying to trigger on two different things (START and STOP) */
    if ((start_trig != NO_START_TRIG) && (stop_trig != NO_STOP_TRIG)) /* one trigger can be set, not both */
    {
        status = DBG_RAM_INVALID_TRIG_SETTING;
    }

    /* Check that the triggers are in valid ranges */
    if ((start_trig >= INVALID_START_TRIG) || (stop_trig >= INVALID_STOP_TRIG)) /* check for validity */
    {
        status = DBG_RAM_INVALID_TRIG_SETTING;
    }

    /* If above checks pass then actually set the register */
    if (status == DBG_RAM_SUCCESS)
    {
        /* set triggers as specified */
        temp = XCVR_MISC->PACKET_RAM_CTRL;
        temp &= ~(XCVR_CTRL_PACKET_RAM_CTRL_DBG_START_TRG_MASK | XCVR_CTRL_PACKET_RAM_CTRL_DBG_STOP_TRG_MASK);
        temp |= XCVR_CTRL_PACKET_RAM_CTRL_DBG_START_TRG(start_trig) | XCVR_CTRL_PACKET_RAM_CTRL_DBG_STOP_TRG(stop_trig);
        XCVR_MISC->PACKET_RAM_CTRL = temp;
    }
    
    return status;
}

dbgRamStatus_t dbg_ram_postproc_capture(uint8_t dbg_page, uint16_t buffer_sz_bytes, void * result_buffer) /* postprocess a capture to unpack data */
{
    dbgRamStatus_t status = DBG_RAM_SUCCESS;
    uint32_t temp;
    /* process buffer unpacking after a capture completes */
    /* since this may require reversing through the sample capture set which may have */
    /* wrapped around in the PKT_RAM size */

    /* Null pointer check first */
    if (result_buffer == NULL)
    {
        status = DBG_RAM_FAIL_NULL_POINTER;
    }

    /* stop trigger processing is currently unimplemented */
    temp = XCVR_MISC->PACKET_RAM_CTRL;
    if ((temp & XCVR_CTRL_PACKET_RAM_CTRL_DBG_STOP_TRG_MASK) != 0U)
    {
        /* postprocess stop trigger data */
        status = DBG_RAM_INVALID_TRIG_SETTING; 
        /* TODO: implement stop trigger postprocessing */
    }

    /* this check ensures that simul and sequ captures are neither both off nor both on */
    /* LOGIC below relies on this check to function correctly */
    if (((!simul_capture_flag) && (!sequ_capture_flag)) ||
        (simul_capture_flag && sequ_capture_flag))
    {
        status = DBG_RAM_INVALID_TRIG_SETTING;
    }

    /* Start trigger processing */
    if (status == DBG_RAM_SUCCESS)
    {
        /* postprocess start trigger or SW trigger data */
        if (simul_capture_flag)
        {
            status = unpack_simul_data(0, buffer_sz_bytes, result_buffer);
        }
        
        if (sequ_capture_flag)
        {
            /* unpack the required number of bytes; relies on prior check of the number of bytes not exceeding the total # of bytes in the PKT RAM banks */
            if (buffer_sz_bytes > 2U*PKT_RAM_SIZE_16B_WORDS) /* need to get bytes from both banks of Pkt RAM */
            {
                uint8_t * buffer2 = ((uint8_t *)result_buffer+(2U*PKT_RAM_SIZE_16B_WORDS));
                status = unpack_sequential_data(0, 0, PKT_RAM_SIZE_16B_WORDS*2U, result_buffer);
                status = unpack_sequential_data(1, 0, buffer_sz_bytes-(2U*PKT_RAM_SIZE_16B_WORDS), buffer2);
            }
            else
            {
                status = unpack_sequential_data(0, 0, buffer_sz_bytes, result_buffer);
            }
        }
    }
    
    return status;
}

/* blocking wait for capture completion, no matter what trigger type */
dbgRamStatus_t dbg_ram_wait_for_complete(void) 
{
    /* Poll on the capture status, waiting for success */
    while (dbg_ram_poll_capture_status() != DBG_RAM_SUCCESS)
    {

    }
    return DBG_RAM_SUCCESS;
}

/* non-blocking completion check, just reads the current status of the capure */
dbgRamStatus_t dbg_ram_poll_capture_status(void) 
{
    dbgRamStatus_t status = DBG_RAM_CAPTURE_NOT_COMPLETE;
    
    uint32_t temp = XCVR_MISC->PACKET_RAM_CTRL;
    uint8_t dbg_ram1_full = (temp & XCVR_CTRL_PACKET_RAM_CTRL_DBG_RAM_FULL_MASK)>>(XCVR_CTRL_PACKET_RAM_CTRL_DBG_RAM_FULL_SHIFT+1U); /* shift +1 to get the upper bit of packet RAM full */
    if ((temp & XCVR_CTRL_PACKET_RAM_CTRL_DBG_START_TRG_MASK) != 0U)
    {
        /* Start trigger enabled && (sequential || simultaneous) */
        /* DBG_START_TRIGGERED && DBG_RAM_FULL[1] */
        if ((temp & XCVR_CTRL_PACKET_RAM_CTRL_DBG_START_TRIGGERED_MASK) && dbg_ram1_full)
        {
            status = DBG_RAM_SUCCESS;
        }
    }
    else
    {
        if ((temp & XCVR_CTRL_PACKET_RAM_CTRL_DBG_STOP_TRG_MASK) != 0U)
        {
            /* Stop trigger enabled  && (sequential || simultaneous) */
            /* DBG_STOP_TRIGGERED */
            if ((temp & XCVR_CTRL_PACKET_RAM_CTRL_DBG_STOP_TRIGGERED_MASK))
            {
                status = DBG_RAM_SUCCESS;
            }
        }
        else
        {
            /* no start or stop trigger, software initiated */
            /* DBG_RAM_FULL[1] */
            if (dbg_ram1_full)
            {
                status = DBG_RAM_SUCCESS;
            }
        }
    }
    return status;
}

dbgRamStatus_t dbg_ram_start_capture(uint8_t dbg_page, dbgRamStartTriggerType start_trig, dbgRamStopTriggerType stop_trig)
{
    uint32_t temp;
    dbgRamStatus_t status = DBG_RAM_SUCCESS;

    status = dbg_ram_set_trigger(start_trig, stop_trig); /* This routine checks the validity of the trigger selection */
    if (status == DBG_RAM_SUCCESS)
    {
        temp = XCVR_MISC->PACKET_RAM_CTRL & ~XCVR_CTRL_PACKET_RAM_CTRL_DBG_PAGE_MASK; /* make sure there isn't an ongoing transfer */
        /* Setup simultaneous or sequential capture flags for control of later processing */
        switch (dbg_page)
        {

            /* Simultaneous capture pages */
            case DBG_PAGE_RXDIGIQ:
            case DBG_PAGE_RAWADCIQ:
            case DBG_PAGE_DCESTIQ:
                /* store flags to make later unpack easier */
                simul_capture_flag = 1;
                sequ_capture_flag = 0;
                XCVR_MISC->PACKET_RAM_CTRL =  temp | XCVR_CTRL_PACKET_RAM_CTRL_DBG_PAGE(dbg_page);
                break;                

            /* Sequential capture pages */
            case DBG_PAGE_RXINPH:
            case DBG_PAGE_DEMOD_SOFT:
            case DBG_PAGE_DEMOD_DATA:
            case DBG_PAGE_DEMOD_CFO_PH:
                /* store flags to make later unpack easier */
                simul_capture_flag = 0;
                sequ_capture_flag = 1;
                XCVR_MISC->PACKET_RAM_CTRL =  temp | XCVR_CTRL_PACKET_RAM_CTRL_DBG_PAGE(dbg_page);
                break;

            /* Invalid pages for starting capture */
            case DBG_PAGE_IDLE:
            default:    
                status = DBG_RAM_FAIL_PAGE_ERROR; /* Illegal capture page request */
                break;
        }
    }
    return status;
}

#else /* radio must be Gen2.0 */

/* legacy function for capturing from dbg_ram, used by existing tests and only supports Gen2.0 radio. Do not break it... */
dbgRamStatus_t dbg_ram_capture(uint8_t dbg_page, uint16_t buffer_sz_bytes, void * result_buffer)
{
    dbgRamStatus_t status = DBG_RAM_SUCCESS;
    uint32_t temp;
    volatile uint8_t *pkt_ram_ptr0, *pkt_ram_ptr1;
    uint8_t * output_ptr;
    uint16_t i;

    /* Some external code must perform the RX warmup request after the dbg_ram_init() call */

    if (result_buffer == NULL)
    {
        status = DBG_RAM_FAIL_NULL_POINTER;
    }
    else
    {
        if (buffer_sz_bytes > (PKT_RAM_SIZE_16B_WORDS*2U*2U))
        {
            status = DBG_RAM_FAIL_SAMPLE_NUM_LIMIT;
        }
        else
        {
            temp = XCVR_MISC->PACKET_RAM_CTRL & ~XCVR_CTRL_PACKET_RAM_CTRL_DBG_PAGE_MASK; /* make sure there isn't an ongoing transfer */
            switch (dbg_page)
            {
                case DBG_PAGE_RXDIGIQ:
                case DBG_PAGE_RAWADCIQ:
                case DBG_PAGE_DCESTIQ:
                    XCVR_MISC->PACKET_RAM_CTRL =  temp | XCVR_CTRL_PACKET_RAM_CTRL_DBG_PAGE(dbg_page);
                    while (!(XCVR_MISC->PACKET_RAM_CTRL & XCVR_CTRL_PACKET_RAM_CTRL_DBG_RAM_FULL(2)))
                    {
                        /* waiting for PKT_RAM to fill, wait for PKT_RAM_1 full to ensure complete memory is filled */
                    }   
                    /* copy to output by bytes to avoid any access size problems in 16 bit packet RAM */
                    output_ptr = result_buffer;
                    pkt_ram_ptr0 = (volatile uint8_t *)&(XCVR_PKT_RAM->PACKET_RAM_0[0]);
                    pkt_ram_ptr1 = (volatile uint8_t *)&(XCVR_PKT_RAM->PACKET_RAM_1[0]);
                    // for *IQ pages I and Q are stored alternately in packet ram 0 & 1
                    for (i=0;i<buffer_sz_bytes/4;i++)
                    {
                        *output_ptr++ = *pkt_ram_ptr0++;
                        *output_ptr++ = *pkt_ram_ptr0++;
                        *output_ptr++ = *pkt_ram_ptr1++;
                        *output_ptr++ = *pkt_ram_ptr1++;
                    }
                    
                    break;
                case DBG_PAGE_RXINPH:
                case DBG_PAGE_DEMOD_HARD: /* only supported on Gen2 radio */
                case DBG_PAGE_DEMOD_SOFT:
                case DBG_PAGE_DEMOD_DATA:
                case DBG_PAGE_DEMOD_CFO_PH:
                    XCVR_MISC->PACKET_RAM_CTRL =  temp | XCVR_CTRL_PACKET_RAM_CTRL_DBG_PAGE(dbg_page);
                    while (!(XCVR_MISC->PACKET_RAM_CTRL & XCVR_CTRL_PACKET_RAM_CTRL_DBG_RAM_FULL(2)))
                    {
                        /* waiting for PKT_RAM to fill, wait for PKT_RAM_1 full to ensure complete memory is filled */
                    }   
                    /* copy to output by bytes to avoid any access size problems in 16 bit packet RAM */
                    output_ptr = result_buffer;
                    pkt_ram_ptr0 = (volatile uint8_t *)&(XCVR_PKT_RAM->PACKET_RAM_0[0]);
                    // this is for non I/Q
                    for (i=0;i<buffer_sz_bytes;i++)
                    {
                        *output_ptr = *pkt_ram_ptr0;
                        pkt_ram_ptr0++;
                        output_ptr++;
                    }
                    
                    break;
                case DBG_PAGE_IDLE:
                default:    
                    status = DBG_RAM_FAIL_PAGE_ERROR; /* Illegal capture page request */
                    break;
            }
        }
    }

    XCVR_MISC->PACKET_RAM_CTRL &= ~XCVR_CTRL_PACKET_RAM_CTRL_DBG_PAGE_MASK; /*Clear DBG_PAGE to terminate the acquisition */

    /* Process the samples and copy to output pointer */

    return status;
    
}
#endif /* (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1) */

