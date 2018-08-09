/*!
* The Clear BSD License
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* \file
*
* This is a source file for the main application.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
* 
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* 
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
* 
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
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