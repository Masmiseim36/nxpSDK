/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __FWK_SWO_DEBUG_H__
#define __FWK_SWO_DEBUG_H__

/* -------------------------------------------------------------------------- */
/*                        Data types  declaration                             */
/* -------------------------------------------------------------------------- */

typedef enum trace_swo_protocol
{
    kSwoProtocolManchester = 1U, /* Asynchronous SWO, using Manchester encoding */
    kSwoProtocolNrz        = 2U, /* Asynchronous SWO, using NRZ encoding */
} trace_swo_protocol_t;

typedef struct trace_swo_config
{
    uint32_t             clockRate; /* System CPUclock rate */
    uint32_t             baudRate;  /* Desired SWO baud rate */
    uint32_t             itmPort;   /* Port ( ITM stimulus register ) used to transfer data */
    trace_swo_protocol_t protocol;  /* SWO physical protocol used */
    uint32_t             traceId;   /* Used to identify trace source for multi core debug */
} trace_swo_config_t;

/* -------------------------------------------------------------------------- */
/*                        Public functions declaration                        */
/* -------------------------------------------------------------------------- */

/*!
 * \brief Initializes SWO configuration via FW embedded code i.s.o. the debug probe
 *
 * \param[in] swoConfig pointer swo trace_swo_config_t structure
 */
void DBG_SwInit_SWO(trace_swo_config_t *swoConfig);

/*!
 * \brief  Configure CoreSight ATP funneling
 *
 * \param[in] funnel integer value representing the desired coresight funnel muxing value
 *            refer to arm documentation CSTF Control Register
 */
void DBG_InitSWOFunnelMuxing(int funnel);

/*!
 * \brief  Print a char on provided ITM stimulus port
 * \param[in] ch character to be output on SWO trace
 * \param[in] itmPort ARM ITM stimulus port to be used
 */
void DBG_SWO_PrintChar(uint32_t ch, uint32_t itmPort);

/*!
 * \brief  Print a double word on provided ITM stimulus port
 * \param[in] dw double word to be output on SWO trace
 * \param[in] itmPort ARM ITM stimulus port to be used
 */
void DBG_SWO_PrintDoubleWord(uint32_t dw, uint32_t itmPort);

/*!
 * \brief  Print a double word on provided ITM stimulus port 0
 *  This function is designed for speed
 * \param[in] dw double word to be output on SWO trace
 */
void DBG_SWO_PrintDoubleWordStim0(uint32_t dw);

#endif /*  __FWK_SWO_DEBUG_H__ */
