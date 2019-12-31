/*! *********************************************************************************
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef __GEN_FSK_TESTS_STATES_H__
#define __GEN_FSK_TESTS_STATES_H__

typedef enum ct_per_tx_states_tag
{
    gPerTxStateInit_c = 0,
    gPerTxStateSelectPacketNum_c,
    gPerTxStateInputPacketDelay_c,
    gPerTxStateStartTest_c,
    gPerTxStateRunningTest_c,
    gPerTxStateIdle_c
}ct_per_tx_states_t;

typedef enum ct_per_rx_states_tag
{
    gPerRxStateInit_c = 0,
    gPerRxStateWaitStartTest_c,
    gPerRxStateStartTest_c,
    gPerRxStateIdle_c
}ct_per_rx_states_t;

typedef enum ct_range_rx_states_tag
{
    gRangeRxStateInit_c = 0,
    gRangeRxStateStartTest_c,
    gRangeRxStateRunningTest_c,
    gRangeRxStatePrintTestResults_c,
    gRangeRxStateIdle_c
}ct_range_rx_states_t;

typedef enum ct_range_tx_states_tag
{
    gRangeTxStateInit_c = 0,
    gRangeTxWaitStartTest_c,
    gRangeTxStateStartTest_c,
    gRangeTxStateRunningTest_c,
    gRangeTxStatePrintTestResults_c,
    gRangeTxStateIdle_c
}ct_range_tx_states_t;

typedef enum ct_cont_tests_states_tag
{
    gContStateInit_c = 0,
    gContStateSelectTest_c,
    gContStateRunIdle_c,
    gContStateRunPRBS_c,
    gContStateRunModTxOne_c,
    gContStateRunModTxZero_c,
    gContStateRunModTxPN_c,
    gContStateRunUnmodTx_c,
    gContStateRunRx_c,
    gContStateRunEd_c,
    gContStateIdle_c,
    gContStateInvalid_c
}ct_cont_tests_states_t;

#endif