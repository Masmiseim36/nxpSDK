/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef DECODER_PADS_H
#define DECODER_PADS_H

/*!
 * @file    decoder_pads.h
 * @brief   This file defines the pad functions used for the decoder element.
 *
 */

/*!
 * @brief   decoder_src_pad_activation_handler
 * @details This function is an empty implementation of the src pad
 *          activation handler.
 *
 * @param   pad pad handle
 * @param   active activate/deactivate pad
 *
 * @returns PAD ERROR
 */
uint8_t decoder_src_pad_activation_handler(StreamPad *pad, uint8_t active);

/*!
 * @brief   decoder_src_pad_event_handler
 * @details This function is the empty implementation of the src pad
 *          event handler.
 *
 * @param   pad pad handle
 * @param   event event enumeration
 *
 * @returns PAD ERROR
 */
uint8_t decoder_src_pad_event_handler(StreamPad *pad, StreamEvent *event);

/*!
 * @brief   decoder_src_pad_query_handler
 * @details This function is the empty implementation of the src pad
 *          query handler.
 *
 * @param   pad pad handle
 * @param   query query
 *
 * @returns uint8_t
 */
uint8_t decoder_src_pad_query_handler(StreamPad *pad, StreamQuery *query);

/*!
 * @brief   decoder_sink_pad_activation_handler
 * @details This function is calls the specified decoder implementation of
 *          the sink pad activation handler.
 *
 * @param   pad pad handle
 * @param   active activate/deactivate pad
 *
 * @returns PAD ERROR
 */
uint8_t decoder_sink_pad_activation_handler(StreamPad *pad, uint8_t active);

/*!
 * @brief   decoder_sink_pad_event_handler
 * @details This function is the calls the specified decoder
 *          implementation ofthe sink pad event handler.
 *
 * @param   pad pad handle
 * @param   event event enumeration
 *
 * @returns PAD ERROR
 */
uint8_t decoder_sink_pad_event_handler(StreamPad *pad, StreamEvent *event);

/*!
 * @brief   decoder_sink_pad_chain_handler
 * @details This function calls the specified decoder implementation of the
 *          sink pad chain handler. This is used in push mode, to receive
 *          data passed from peer.
 *
 * @param   pad pad handle
 * @param   buffer input buffer
 *
 * @returns PAD ERROR
 */
FlowReturn decoder_sink_pad_chain_handler(StreamPad *pad, StreamBuffer *buffer);

/*!
 * @brief   decoder_sink_pad_process_handler
 * @details This function calls the specified decoder implementation of the
 *          sink pad process handler.  This is used in pull mode, to request
 *          data from peer.
 *
 * @param   pad pad handle
 *
 * @returns PAD ERROR
 */
int32_t decoder_sink_pad_process_handler(StreamPad *pad);

#endif
