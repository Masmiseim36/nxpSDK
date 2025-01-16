/** @file wls_api.c
  *
  * @brief This file contains source code for CSI processing API.
  *
  * Copyright 2023-2024 NXP
  *
  * SPDX-License-Identifier: BSD-3-Clause
  */

/************************************************************************
* DFW source code for CSI processing API.
************************************************************************/

#include <osa.h>
#if CONFIG_WLS_CSI_PROC

// Standard includes.
//#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include <math.h>

// Specific includes.
#include "wls_param_defines.h"
#include "wls_structure_defs.h"

#include "wls_radix4Fft.h"
#ifdef ENABLE_AOA
#include "wls_aoa_processing.h"
#endif
#ifdef ARM_DS5
#include "wls_processing_Neon_Intrinsic.h"
#endif

#include "wls_processing.h"
#ifdef FFT_PARALLEL
#include "wls_processing_parallel.h"
#endif
#ifdef ENABLE_SUBSPACE_FTIMING
#include "wls_subspace_processing.h"
#endif

#ifdef DEBUG_OUT_FD
void saveDebug(unsigned int *writePointer, unsigned int *readPointer, int size){
	int ii;

	for(ii=0;ii<size;ii++){
		writePointer[ii] = readPointer[ii];
	}
}
#endif


#ifdef TDDE_FIRSTPATH
int wls_process_csi(
	unsigned int *bufferMemory,						// CSI buffer
	unsigned int *fftBuffer,						// 2k times (MAX_RX*MAX_TX+1) scratch memory
	hal_wls_packet_params_t *packetparams,			// values from Rx/Tx-Info, used mostly to find correct CSI buffer
	hal_wls_processing_input_params_t *inputVals,	// WLS / AoA CSI Processing Parameters Structure
	unsigned int *resArray)							// outputs � phase roll, first path, pktinfo, difference of rxInfo CSI TSF counters
{													// weight1, weight2, angle, delay (16 bits each) for AoA processing per Peak

	int tddeDataSize;
	unsigned int headerBuffer[HEADER_LEN];
	int start_tdde = 0;
	unsigned int tempVec[2] = { 0,0 };

	hal_csirxinfo_t *csirxinfo;
	hal_pktinfo_t *pktinfo;
	hal_tddestruct_t *tdde_ptr;

	int firstPath;
	char MIN_comb = inputVals->useToaMin;

	memcpy(&headerBuffer, bufferMemory, HEADER_LEN*sizeof(unsigned int));

	if (!(inputVals->enableCsi)) {
		return -1;
	}

	csirxinfo = (hal_csirxinfo_t*)headerBuffer;

	tempVec[0] = (unsigned int)csirxinfo->pktinfo;
	pktinfo = (hal_pktinfo_t*)tempVec;

	if (pktinfo->packetType < 3) { // HT, legacy
		if (pktinfo->packetType == 0) { // legacy
			pktinfo->sigBw = pktinfo->rxDevBw;
		}
	}

	start_tdde = HEADER_LEN;
	if (csirxinfo->ltf) {
		start_tdde += bufferMemory[start_tdde];
	}
	if (csirxinfo->csi) {
		start_tdde += bufferMemory[start_tdde];
	}
	if (csirxinfo->tbd) {
		tddeDataSize = bufferMemory[start_tdde] - 1;
		start_tdde++;
	}
	else
		return -1;

	tdde_ptr = (hal_tddestruct_t *) (bufferMemory + start_tdde);

	firstPath = determine_tdde_firstpath(pktinfo, packetparams, tdde_ptr, fftBuffer, MIN_comb);

	resArray[0] = 0xffffffff; // no phase-roll
	resArray[1] = (unsigned int)firstPath;
	resArray[2] = (unsigned int)tempVec[0]; // = pktinfo
	resArray[3] = (unsigned int)tempVec[1]; // = pktinfo

	return 0;
}
#else // TDDE_FIRSTPATH

int wls_process_csi(
	unsigned int *bufferMemory,						// CSI buffer
	unsigned int *fftBuffer,						// 2k times (MAX_RX*MAX_TX+1) scratch memory
	hal_wls_packet_params_t *packetparams,			// values from Rx/Tx-Info, used mostly to find correct CSI buffer
	hal_wls_processing_input_params_t *inputVals,	// WLS / AoA CSI Processing Parameters Structure
	unsigned int *resArray)							// outputs � phase roll, first path, pktinfo, difference of rxInfo CSI TSF counters
{													// weight1, weight2, angle, delay (16 bits each) for AoA processing per Peak

	int csiDataSize;
	int fftSize, ifftSizeOsf, firstPathDelay, maxIdx;
	unsigned int maxVal;
	unsigned int *fftInBuffer, *pdpOut;
	unsigned int powerPerSubband[4 * MAX_RX*MAX_TX]; // max num. of 40 MHz subbands
	unsigned int totalpower[MAX_RX*MAX_TX + 1];
	int phaseRollNg[MAX_RX*MAX_TX];
	unsigned int headerBuffer[HEADER_LEN];


	int header_length, start_csi = 0;
	unsigned int tempVec[2] = { 0,0 };
#ifdef ENABLE_AOA
	hal_cal_struc_t calData;
#endif
	hal_csirxinfo_t *csirxinfo;
	hal_pktinfo_t *pktinfo;

	memcpy(&headerBuffer, bufferMemory, HEADER_LEN*sizeof(unsigned int));

	if(!(inputVals->enableCsi)){
		return -1;
	}

#if defined(DEBUG_CYCLE_TIME)
    DEBUG_DELTA_TIME_US1 = ((UINT64)HAL_REGS32(0x8000a604) << 32) + HAL_REGS32(0x8000a600);
#endif

	csirxinfo = (hal_csirxinfo_t*)headerBuffer;

	tempVec[0] = (unsigned int)csirxinfo->pktinfo;
	pktinfo = (hal_pktinfo_t*)tempVec;

#ifdef LEG_LTF_NG1
	if (pktinfo->packetType == 1) { // HT only
#else
	if (pktinfo->packetType < 3) { // HT, legacy
		if (pktinfo->packetType == 0) { // legacy
			pktinfo->sigBw = pktinfo->rxDevBw;
		}
#endif
		if (pktinfo->sigBw){ // bw > 20 MHz
			pktinfo->NgDsfShift = pktinfo->Ng+1;
		}
		else { // 20 MHz
			pktinfo->NgDsfShift = 0;
		}
	}
	else { // VHT, HE, legacy
		pktinfo->NgDsfShift = 0;
		//pktinfo->Ng = 0; // not used
	}

#ifdef SMAC_BFINFO
	header_length = csirxinfo->header_length;
	csiDataSize = bufferMemory[header_length]-1;
	start_csi = header_length + 1;
#else
	header_length = HEADER_LEN;
	start_csi = header_length;
#if defined(RAW_HEADER)
	if (csirxinfo->ltf) {
		start_csi += bufferMemory[header_length];
	}
#endif
	csiDataSize = (UINT16)(bufferMemory[start_csi]) - 1;
	start_csi++;
#endif

	fftSize = pktinfo->sigBw+IFFT_OSF_SHIFT-pktinfo->NgDsfShift;
	pktinfo->fftSize = (fftSize < MAX_IFFT_SIZE_SHIFT)? fftSize:MAX_IFFT_SIZE_SHIFT;
	pktinfo->scOffset = 0;
	ifftSizeOsf = 1<<(pktinfo->fftSize+6);
#if defined(FFT_INPLACE)
	fftInBuffer = fftBuffer;
#else
	fftInBuffer = fftBuffer + NUM_PARALLEL * ifftSizeOsf;
#endif
	// expand data from 8->16 bit, demodulate pilots for L-LTF, measure power and linear phase
#ifdef LEG_LTF_NG1
	if (pktinfo->packetType != 1){ // Ng=1 (VHT/legacy+HE)
#else
	if (pktinfo->packetType > 2) { // VHT+HE (Ng=1)
#endif
#ifdef FFT_PARALLEL
		readHexDataDemodulateProcessVhtHeNg1Parallel(pktinfo, inputVals, bufferMemory + start_csi, csiDataSize, fftInBuffer, powerPerSubband, phaseRollNg, packetparams->chNum);
#else
		readHexDataDemodulateProcessVhtHeNg1(pktinfo, inputVals, bufferMemory + start_csi, csiDataSize, fftInBuffer, powerPerSubband, phaseRollNg, packetparams->chNum);
#endif
		if (pktinfo->sigBw == 3) // 160 MHz only
			detectPhaseJump(pktinfo, inputVals, fftInBuffer, phaseRollNg);
	}
	else { // Ng=2/4 (HT only)
#ifdef FFT_PARALLEL
		readHexDataDemodulateProcessParallel(pktinfo, inputVals, bufferMemory + start_csi, csiDataSize, fftInBuffer, powerPerSubband, phaseRollNg, packetparams->chNum);
#else
		readHexDataDemodulateProcess(pktinfo, inputVals, bufferMemory + start_csi, csiDataSize, fftInBuffer, powerPerSubband, phaseRollNg, packetparams->chNum);
#endif
	}
#ifndef LEG_LTF_NG1
	if (pktinfo->packetType == 0) { // in case of legacy packets, check active subbands  && (packetparams->ftmSignalBW<pktinfo->sigBw)
		findActiveSubbands(pktinfo, powerPerSubband, totalpower, packetparams->chNum, packetparams->ftmSignalBW);
		zeroOutTones(pktinfo, fftInBuffer, ifftSizeOsf);
	} 	else
#endif
	{
		calculateTotalPower(pktinfo, powerPerSubband, totalpower);
	}
#ifndef STA_20_ONLY
	if (((pktinfo->packetType < 4) && (pktinfo->sigBw > 0)) || (pktinfo->rxDevBw == 3) ) { // not for HE and BW > 20 MHz
#if defined(FFT_PARALLEL) && defined(ARM_DS5)
		removeToneRotationIntrinsic(pktinfo, fftInBuffer, ifftSizeOsf);
#elif defined(FFT_PARALLEL) && !defined(ARM_DS5)
		removeToneRotationParallel(pktinfo, fftInBuffer, ifftSizeOsf);
#else
		removeToneRotation(pktinfo, fftInBuffer, ifftSizeOsf);
#endif
	}
	if ((pktinfo->packetType == 0) && (pktinfo->sigBw > 0)) { // all legacy except full interpolation for 20in20
		processLegacyPackets(pktinfo, fftInBuffer, ifftSizeOsf, phaseRollNg);
	}
#endif
#ifndef LEG_LTF_NG1
	if ((pktinfo->packetType > 2) || ((pktinfo->packetType == 1) && (pktinfo->Ng == 0)) // add HT20, HT40 case, Ng=2
		|| ((pktinfo->packetType == 0) && (pktinfo->sigBw == 0) && (pktinfo->rxDevBw == 0))) // add Leg20 case, DevBw=0
#endif
	{
#if defined(FFT_PARALLEL) && defined(ARM_DS5)
		interpolatePilotsIntrinsic(pktinfo, fftInBuffer, ifftSizeOsf, phaseRollNg, totalpower);
#elif defined(FFT_PARALLEL) && !defined(ARM_DS5)
		interpolatePilotsParallel(pktinfo, fftInBuffer, ifftSizeOsf, phaseRollNg, totalpower);
#else
		interpolatePilots(pktinfo, fftInBuffer, ifftSizeOsf, phaseRollNg, totalpower);
#endif
	}
#if defined(DEBUG_OUT_FD) && !defined(ARM_DS5)
    saveDebug(bufferMemory, fftInBuffer+0*ifftSizeOsf, ifftSizeOsf);
#endif
	// ifft processing
	ifftProcessing(pktinfo, fftInBuffer, fftBuffer, ifftSizeOsf); // ifftSizeOsf might not match pktinfo->sigBw

	// update
	fftSize = (1<<(pktinfo->sigBw+6));
	ifftSizeOsf = 1<<(pktinfo->fftSize+6);
    pktinfo->rsvd1 = 0;

	resArray[2] = (unsigned int)tempVec[0]; // = pktinfo
    resArray[3] = (unsigned int)tempVec[1]; // = pktinfo

#if defined(DEBUG_OUT_TD) && !defined(ARM_DS5)
	saveDebug(bufferMemory+data_length, fftBuffer+0*ifftSizeOsf, NUM_PARALLEL*ifftSizeOsf);
#endif
	// determine first path
	pdpOut = fftBuffer+(MAX_RX*MAX_TX)*ifftSizeOsf; // last buffer

	if(inputVals->useToaMin==1){
		calcPdpAndFirstPathMin(pktinfo, fftBuffer, pdpOut, totalpower, &maxIdx, &maxVal, &firstPathDelay);
	}
	else
	{
#if defined(FFT_PARALLEL) && defined(ARM_DS5)
		calcPdpAndMaxIntrinsic(pktinfo, fftBuffer, pdpOut, totalpower, &maxIdx, &maxVal);
#else
		calcPdpAndMaxParallel(pktinfo, fftBuffer, pdpOut, totalpower, &maxIdx, &maxVal);
#endif
		firstPathDelay = findFirstPath(pktinfo, pdpOut, maxIdx, maxVal, 1);
	}
	if(pktinfo->packetType>2){ // Ng=1 case
		resArray[0] = ((phaseRollNg[0]*fftSize<<2)/5)>>(pktinfo->sigBw);
	}
	else
	{ // Ng=2/4 case
		resArray[0] = ((phaseRollNg[0]*fftSize<<1)/5)>>(pktinfo->sigBw+pktinfo->Ng);
	}
#ifdef ENABLE_SUBSPACE_FTIMING
	{
		int fineTimingRes, retVal = 1;
		if(inputVals->useSubspace==1){
			retVal = calcSubspaceFineTiming(pktinfo, fftBuffer, totalpower, firstPathDelay, &fineTimingRes, pdpOut, packetparams);
		}
		if(retVal){ // error or not uses, use first path
			fineTimingRes = firstPathDelay;
		}
		resArray[1] = ((fineTimingRes<<(14-TOA_FPATH_BIPT))/5)>>(pktinfo->fftSize+pktinfo->NgDsfShift);
	}
#else
	// final format is 32.TOA_FPATH_BIPT in micro seconds
	resArray[1] = ((firstPathDelay<<(14-TOA_FPATH_BIPT))/5)>>(pktinfo->fftSize+pktinfo->NgDsfShift);
#endif
#ifdef ENABLE_AOA
	if(inputVals->enableAoA && pktinfo->nRx){ // at least 2 Rx paths
		readCalDataNew(&calData, packetparams, pktinfo, inputVals);
		resArray[4] = maxVal>>15;
		if(inputVals->useFindAngleDelayPeaks && (pktinfo->nRx>1)){ // at least 3 Rx paths
			findAngleDelayPeaks(pktinfo, packetparams, inputVals, &calData, fftBuffer, pdpOut, totalpower, maxIdx, resArray+4);
		}
		else{
			findAngleLinPhase(pktinfo, packetparams, &calData, fftBuffer, totalpower, firstPathDelay, resArray+5);
		}
	}
	if(inputVals->dumpRawAngle){
		dumpRawComplex(pktinfo,fftBuffer,firstPathDelay,resArray+8);
	}
#endif
#if defined(DEBUG_CYCLE_TIME)
    DEBUG_DELTA_TIME_US2 = ((UINT64)HAL_REGS32(0x8000a604) << 32) + HAL_REGS32(0x8000a600);
    DEBUG_DELTA_TIME_US  = DEBUG_DELTA_TIME_US2 - DEBUG_DELTA_TIME_US1;
#endif

	return 0;
}
#endif

void wls_unpack_csi (
	unsigned int *bufferMemory,						// CSI buffer
	unsigned int *outBuffer,						// 2k times (MAX_RX*MAX_TX+1) memory to output unpacked CSI
	hal_wls_packet_params_t *packetparams,			// values from Rx/Tx-Info, used mostly to find correct CSI buffer
	hal_wls_processing_input_params_t *inputVals,	// WLS / AoA CSI Processing Parameters Structure
	unsigned int *totalpower)						// [MAX_RX*MAX_TX + 1] size to output power per Rx/Tx stream
{
	int csiDataSize;
	int fftSize, ifftSizeOsf;
	unsigned int powerPerSubband[4 * MAX_RX*MAX_TX]; // max num. of 40 MHz subbands
	//unsigned int totalpower[MAX_RX*MAX_TX + 1];
	int phaseRollNg[MAX_RX*MAX_TX];
	unsigned int headerBuffer[HEADER_LEN];

	int header_length, start_csi = 0;
	unsigned int tempVec[2] = { 0,0 };
	hal_csirxinfo_t *csirxinfo;
	hal_pktinfo_t *pktinfo;

	memcpy(&headerBuffer, bufferMemory, HEADER_LEN*sizeof(unsigned int));

#if defined(DEBUG_CYCLE_TIME)
	DEBUG_DELTA_TIME_US1 = ((UINT64)HAL_REGS32(0x8000a604) << 32) + HAL_REGS32(0x8000a600);
#endif

	csirxinfo = (hal_csirxinfo_t*)headerBuffer;

	tempVec[0] = (unsigned int)csirxinfo->pktinfo;
	pktinfo = (hal_pktinfo_t*)tempVec;

#ifdef LEG_LTF_NG1
	if (pktinfo->packetType == 1) { // HT only
#else
	if (pktinfo->packetType < 3) { // HT, legacy
		if (pktinfo->packetType == 0) { // legacy
			pktinfo->sigBw = pktinfo->rxDevBw;
		}
#endif
		if (pktinfo->sigBw) { // bw > 20 MHz
			pktinfo->NgDsfShift = pktinfo->Ng + 1;
		}
		else { // 20 MHz
			pktinfo->NgDsfShift = 0;
		}
	}
	else { // VHT, HE, legacy
		pktinfo->NgDsfShift = 0;
		//pktinfo->Ng = 0; // not used
	}

#ifdef SMAC_BFINFO
	header_length = csirxinfo->header_length;
	csiDataSize = bufferMemory[header_length] - 1;
	start_csi = header_length + 1;
#else
	header_length = HEADER_LEN;
	start_csi = header_length;
#if defined(RAW_HEADER)
	if (csirxinfo->ltf) {
		start_csi += bufferMemory[header_length];
	}
#endif
	csiDataSize = (UINT16)(bufferMemory[start_csi]) - 1;
	start_csi++;
#endif

	fftSize = pktinfo->sigBw + IFFT_OSF_SHIFT - pktinfo->NgDsfShift;
	pktinfo->fftSize = (fftSize < MAX_IFFT_SIZE_SHIFT) ? fftSize : MAX_IFFT_SIZE_SHIFT;
	pktinfo->scOffset = 0;
	ifftSizeOsf = 1 << (pktinfo->fftSize + 6);

	// expand data from 8->16 bit, demodulate pilots for L-LTF, measure power and linear phase
#ifdef LEG_LTF_NG1
	if (pktinfo->packetType != 1) { // Ng=1 (VHT/legacy+HE)
#else
	if (pktinfo->packetType > 2) { // VHT+HE (Ng=1)
#endif
#ifdef FFT_PARALLEL
		readHexDataDemodulateProcessVhtHeNg1Parallel(pktinfo, inputVals, bufferMemory + start_csi, csiDataSize, outBuffer, powerPerSubband, phaseRollNg, packetparams->chNum);
#else
		readHexDataDemodulateProcessVhtHeNg1(pktinfo, inputVals, bufferMemory + start_csi, csiDataSize, outBuffer, powerPerSubband, phaseRollNg, packetparams->chNum);
#endif
		if (pktinfo->sigBw == 3) // 160 MHz only
			detectPhaseJump(pktinfo, inputVals, outBuffer, phaseRollNg);
	}
	else { // Ng=2/4 (HT only)
#ifdef FFT_PARALLEL
		readHexDataDemodulateProcessParallel(pktinfo, inputVals, bufferMemory + start_csi, csiDataSize, outBuffer, powerPerSubband, phaseRollNg, packetparams->chNum);
#else
		readHexDataDemodulateProcess(pktinfo, inputVals, bufferMemory + start_csi, csiDataSize, outBuffer, powerPerSubband, phaseRollNg, packetparams->chNum);
#endif
	}
#ifndef LEG_LTF_NG1
	if (pktinfo->packetType == 0) { // in case of legacy packets, check active subbands  && (packetparams->ftmSignalBW<pktinfo->sigBw)
		findActiveSubbands(pktinfo, powerPerSubband, totalpower, packetparams->chNum, packetparams->ftmSignalBW);
		zeroOutTones(pktinfo, outBuffer, ifftSizeOsf);
	}
	else
#endif
	{
		calculateTotalPower(pktinfo, powerPerSubband, totalpower);
	}
#ifndef STA_20_ONLY
	if (((pktinfo->packetType < 4) && (pktinfo->sigBw > 0)) || (pktinfo->rxDevBw == 3)) { // not for HE and BW > 20 MHz
#if defined(FFT_PARALLEL) && defined(ARM_DS5)
		removeToneRotationIntrinsic(pktinfo, outBuffer, ifftSizeOsf);
#elif defined(FFT_PARALLEL) && !defined(ARM_DS5)
		removeToneRotationParallel(pktinfo, outBuffer, ifftSizeOsf);
#else
		removeToneRotation(pktinfo, outBuffer, ifftSizeOsf);
#endif
	}
	if ((pktinfo->packetType == 0) && (pktinfo->sigBw > 0)) { // all legacy except full interpolation for 20in20
		processLegacyPackets(pktinfo, outBuffer, ifftSizeOsf, phaseRollNg);
	}
#endif
#ifndef LEG_LTF_NG1
	if ((pktinfo->packetType > 2) || ((pktinfo->packetType == 1) && (pktinfo->Ng == 0)) // add HT20, HT40 case, Ng=2
		|| ((pktinfo->packetType == 0) && (pktinfo->sigBw == 0) && (pktinfo->rxDevBw == 0))) // add Leg20 case, DevBw=0
#endif
	{
#if defined(FFT_PARALLEL) && defined(ARM_DS5)
		interpolatePilotsIntrinsic(pktinfo, outBuffer, ifftSizeOsf, phaseRollNg, totalpower);
#elif defined(FFT_PARALLEL) && !defined(ARM_DS5)
		interpolatePilotsParallel(pktinfo, outBuffer, ifftSizeOsf, phaseRollNg, totalpower);
#else
		interpolatePilots(pktinfo, outBuffer, ifftSizeOsf, phaseRollNg, totalpower);
#endif
	}
#if defined(DEBUG_OUT_FD) && !defined(ARM_DS5)
	saveDebug(bufferMemory, outBuffer + 0 * ifftSizeOsf, ifftSizeOsf);
#endif

#if defined(DEBUG_CYCLE_TIME)
	DEBUG_DELTA_TIME_US2 = ((UINT64)HAL_REGS32(0x8000a604) << 32) + HAL_REGS32(0x8000a600);
	DEBUG_DELTA_TIME_US = DEBUG_DELTA_TIME_US2 - DEBUG_DELTA_TIME_US1;
#endif
}

int wls_calculate_toa (
	unsigned int *headerBuffer,						// CSI BF_INFO header
	unsigned int *fftInBuffer,						// Input buffer (freq. domain CSI)
	unsigned int *fftBuffer,						// 2k times (MAX_RX*MAX_TX+1) scratch memory for time domain CSI
	unsigned int *totalpower,						// size [MAX_RX*MAX_TX + 1] power per Rx/Tx stream
	hal_wls_packet_params_t *packetparams,			// values from Rx/Tx-Info, used mostly to find correct CSI buffer
	hal_wls_processing_input_params_t *inputVals	// WLS / AoA CSI Processing Parameters Structure
	)
{
	int resVal, fftSize, ifftSizeOsf, firstPathDelay, maxIdx;
	unsigned int maxVal;
	unsigned int *pdpOut;
	unsigned int tempVec[2] = { 0,0 };
#ifdef ENABLE_AOA
	hal_cal_struc_t calData;
#endif
	hal_csirxinfo_t *csirxinfo;
	hal_pktinfo_t *pktinfo;

#if defined(DEBUG_CYCLE_TIME)
	DEBUG_DELTA_TIME_US1 = ((UINT64)HAL_REGS32(0x8000a604) << 32) + HAL_REGS32(0x8000a600);
#endif

	csirxinfo = (hal_csirxinfo_t*)headerBuffer;

	tempVec[0] = (unsigned int)csirxinfo->pktinfo;
	pktinfo = (hal_pktinfo_t*)tempVec;

#ifdef LEG_LTF_NG1
	if (pktinfo->packetType == 1) { // HT only
#else
	if (pktinfo->packetType < 3) { // HT, legacy
		if (pktinfo->packetType == 0) { // legacy
			pktinfo->sigBw = pktinfo->rxDevBw;
		}
#endif
		if (pktinfo->sigBw) { // bw > 20 MHz
			pktinfo->NgDsfShift = pktinfo->Ng + 1;
		}
		else { // 20 MHz
			pktinfo->NgDsfShift = 0;
		}
	}
	else { // VHT, HE, legacy
		pktinfo->NgDsfShift = 0;
		//pktinfo->Ng = 0; // not used
	}

	fftSize = pktinfo->sigBw + IFFT_OSF_SHIFT - pktinfo->NgDsfShift;
	pktinfo->fftSize = (fftSize < MAX_IFFT_SIZE_SHIFT) ? fftSize : MAX_IFFT_SIZE_SHIFT;
	pktinfo->scOffset = 0;
	ifftSizeOsf = 1 << (pktinfo->fftSize + 6);
// #if defined(FFT_INPLACE)
// if FFT_INPLACE need to copy input data to new buffer

	// ifft processing
	ifftProcessing(pktinfo, fftInBuffer, fftBuffer, ifftSizeOsf); // ifftSizeOsf might not match pktinfo->sigBw

																  // update
	fftSize = (1 << (pktinfo->sigBw + 6));
	ifftSizeOsf = 1 << (pktinfo->fftSize + 6);
	pktinfo->rsvd1 = 0;

#if defined(DEBUG_OUT_TD) && !defined(ARM_DS5)
	saveDebug(bufferMemory + data_length, fftBuffer + 0 * ifftSizeOsf, NUM_PARALLEL*ifftSizeOsf);
#endif
	// determine first path
	pdpOut = fftBuffer + (MAX_RX*MAX_TX)*ifftSizeOsf; // last buffer

	if (inputVals->useToaMin == 1) {
		calcPdpAndFirstPathMin(pktinfo, fftBuffer, pdpOut, totalpower, &maxIdx, &maxVal, &firstPathDelay);
	}
	else
	{
		calcPdpAndMaxParallel(pktinfo, fftBuffer, pdpOut, totalpower, &maxIdx, &maxVal);
		firstPathDelay = findFirstPath(pktinfo, pdpOut, maxIdx, maxVal, 1);
	}

#ifdef ENABLE_SUBSPACE_FTIMING
	{
		int fineTimingRes, retVal = 1;
		if (inputVals->useSubspace == 1) {
			retVal = calcSubspaceFineTiming(pktinfo, fftBuffer, totalpower, firstPathDelay, &fineTimingRes, pdpOut, packetparams);
		}
		if (retVal) { // error or not uses, use first path
			fineTimingRes = firstPathDelay;
		}
		resVal = ((fineTimingRes << (14 - TOA_FPATH_BIPT)) / 5) >> (pktinfo->fftSize + pktinfo->NgDsfShift);
	}
#else
	// final format is 32.TOA_FPATH_BIPT in micro seconds
	resVal = ((firstPathDelay << (14 - TOA_FPATH_BIPT)) / 5) >> (pktinfo->fftSize + pktinfo->NgDsfShift);
#endif
#if defined(DEBUG_CYCLE_TIME)
	DEBUG_DELTA_TIME_US2 = ((UINT64)HAL_REGS32(0x8000a604) << 32) + HAL_REGS32(0x8000a600);
	DEBUG_DELTA_TIME_US = DEBUG_DELTA_TIME_US2 - DEBUG_DELTA_TIME_US1;
#endif
	return resVal;
}

//#define PI_ALPHA_FACTOR 0.1f
#define KALMAN_UPDATE
#ifdef KALMAN_UPDATE
UINT64 kalman_prev_tsf;
float kalman_p0;
#define KALMAN_N0  0.2f
#define KALMAN_ALPHA 0.005f
#endif

void wls_intialize_reference(
	unsigned int *headerBuffer,						// CSI BF_INFO header
	unsigned int *fftInBuffer,						// Input buffer (freq. domain CSI)
	float *fftRefBuffer						// Refernce CSI buffer (freq. domain CSI)
	)
{
	int fftSize, ifftSizeOsf;
	int ii, jj, kk;
	int nRx, nTx;
	float *writePtr;
	unsigned int* readPtr;
	unsigned int tempVec[2] = { 0,0 };
	hal_csirxinfo_t *csirxinfo;
	hal_pktinfo_t *pktinfo;

	csirxinfo = (hal_csirxinfo_t*)headerBuffer;

	tempVec[0] = (unsigned int)csirxinfo->pktinfo;
	pktinfo = (hal_pktinfo_t*)tempVec;
	nRx = pktinfo->nRx + 1;
	nTx = pktinfo->nTx + 1;

#ifdef KALMAN_UPDATE
	kalman_prev_tsf = csirxinfo->tsf;
#ifndef RAW_HEADER
	kalman_prev_tsf += ((UINT64)csirxinfo->tsf_hi) << 32;
#endif
	kalman_p0 = 0.5f;
#endif
#ifdef LEG_LTF_NG1
	if (pktinfo->packetType == 1) { // HT only
#else
	if (pktinfo->packetType < 3) { // HT, legacy
		if (pktinfo->packetType == 0) { // legacy
			pktinfo->sigBw = pktinfo->rxDevBw;
		}
#endif
		if (pktinfo->sigBw) { // bw > 20 MHz
			pktinfo->NgDsfShift = pktinfo->Ng + 1;
		}
		else { // 20 MHz
			pktinfo->NgDsfShift = 0;
		}
	}
	else { // VHT, HE, legacy
		pktinfo->NgDsfShift = 0;
		//pktinfo->Ng = 0; // not used
	}

	fftSize = pktinfo->sigBw + IFFT_OSF_SHIFT - pktinfo->NgDsfShift;
	pktinfo->fftSize = (fftSize < MAX_IFFT_SIZE_SHIFT) ? fftSize : MAX_IFFT_SIZE_SHIFT;
	ifftSizeOsf = 1 << (pktinfo->fftSize + 6);

	//bufferSize = (pktinfo->nRx + 1) * (pktinfo->nTx + 1) * ifftSizeOsf;
	//memcpy(fftRefBuffer, fftInBuffer, bufferSize*sizeof(unsigned int));

	for (ii = 0; ii < nTx; ii++) {
		for (jj = 0; jj < nRx; jj++) {
			readPtr = fftInBuffer + ifftSizeOsf * (jj + ii*nRx);
			writePtr = (float*)fftRefBuffer + 2 * ifftSizeOsf * (jj + ii*nRx);

			for (kk = 0; kk < ifftSizeOsf; kk++)
			{
				unsigned int tempValLd;
				short tempValI, tempValQ;
				float tempI, tempQ;

				tempValLd = readPtr[kk];
				tempValI = (tempValLd & 0xffff);
				tempValQ = (tempValLd >> 16);

				tempI = (float)tempValI;
				tempQ = (float)tempValQ;

				writePtr[2 * kk] = tempI / (1 << 8);
				writePtr[2 * kk + 1] = tempQ / (1 << 8);
			}
		}
	}
}

void wls_update_reference_iir(hal_pktinfo_t *pktinfo,
	float *fftRefBuffer,							// Refernce CSI buffer (freq. domain CSI)
	float *fftOutBuffer,							// Output buffer float (freq. domain CSI)
	float alpha										// IIR filter update coefficient
	)
{
	int ii, jj, kk;
	int ifftSizeOsf = 1 << (pktinfo->fftSize + 6);
	int nRx = pktinfo->nRx + 1;
	int nTx = pktinfo->nTx + 1;
	float *H_comp, *H_old;


	// calculate H_proj = sum(conj(H_old).*H_comp, 1)
	for (ii = 0; ii < nTx; ii++) {
		for (jj = 0; jj < nRx; jj++) {
			H_old = fftRefBuffer + 2 * ifftSizeOsf * (jj + ii*nRx);
			H_comp = fftOutBuffer + 2 * ifftSizeOsf * (jj + ii*nRx);
			for (kk = 0; kk < ifftSizeOsf; kk++)
			{
				float tempRefI = H_old[2 * kk];
				float tempRefQ = H_old[2 * kk + 1];
				float tempValI = H_comp[2 * kk];
				float tempValQ = H_comp[2 * kk + 1];

				H_old[2 * kk] = (1 - alpha) * tempRefI + alpha * tempValI;
				H_old[2 * kk + 1] = (1 - alpha) * tempRefQ + alpha * tempValQ;
			}
		}
	}
}

void wls_update_reference_kalmman(hal_pktinfo_t *pktinfo,
	float *fftRefBuffer,							// Refernce CSI buffer (freq. domain CSI)
	float *fftOutBuffer,							// Output buffer float (freq. domain CSI)
	UINT64 current_tsf
	)
{
	int ii, jj, kk;
	int ifftSizeOsf = 1 << (pktinfo->fftSize + 6);
	int nRx = pktinfo->nRx + 1;
	int nTx = pktinfo->nTx + 1;
	float *H_comp, *H_old;

	SINT64 delta_tsf = current_tsf - kalman_prev_tsf;
	float delta_time = delta_tsf / 1e6f;

	float p_n = kalman_p0 + KALMAN_ALPHA*delta_time;
	float S_n = p_n + KALMAN_N0;
	float K_n = p_n / S_n;

	// update state
	kalman_p0 = (1 - K_n) * p_n;
	kalman_prev_tsf = current_tsf;

	// calculate H_proj = sum(conj(H_old).*H_comp, 1)
	for (ii = 0; ii < nTx; ii++) {
		for (jj = 0; jj < nRx; jj++) {

			H_old = fftRefBuffer + 2 * ifftSizeOsf * (jj + ii*nRx);
			H_comp = fftOutBuffer + 2 * ifftSizeOsf * (jj + ii*nRx);
			for (kk = 0; kk < ifftSizeOsf; kk++)
			{
				float tempRefI = H_old[2 * kk];
				float tempRefQ = H_old[2 * kk + 1];
				float tempValI = H_comp[2 * kk];
				float tempValQ = H_comp[2 * kk + 1];

				H_old[2 * kk] = (1 - K_n) * tempRefI + K_n * tempValI;
				H_old[2 * kk + 1] = (1 - K_n) * tempRefQ + K_n * tempValQ;
			}
		}
	}
}

float wls_calc_rel_timing(hal_pktinfo_t *pktinfo,
	unsigned int *fftInBuffer,						// Input buffer (freq. domain CSI)
	float *fftRefBuffer,							// Refernce CSI buffer (freq. domain CSI)
	unsigned int *tempBuffer,						// 2k times (MAX_RX*MAX_TX+1) scratch memory
	float *psiArray									// [MAX_RX*MAX_TX] float array of phase on each path
	) {

	int ii, jj, kk, maxIdx = -1;
	int ifftSizeOsf = 1 << (pktinfo->fftSize + 6);
	int nRx = pktinfo->nRx + 1;
	int nTx = pktinfo->nTx + 1;
	unsigned int *readPtr;
	float *procBuffer, *corrBuffer, *refPtr;
	float maxVal = 0.0f;
	float phi_hat;

	corrBuffer = (float*)tempBuffer + 2 * nTx * nRx * ifftSizeOsf;

	memset(corrBuffer, 0, ifftSizeOsf * sizeof(unsigned int));

	for (ii = 0; ii < nTx; ii++) {
		for (jj = 0; jj < nRx; jj++) {
			readPtr = fftInBuffer + ifftSizeOsf * (jj + ii*nRx);
			refPtr = fftRefBuffer + 2 * ifftSizeOsf * (jj + ii*nRx);
			procBuffer = (float*)tempBuffer + 2 * ifftSizeOsf * (jj + ii*nRx);

			// procuct conj(H_old(:,ii,hh)).*H_vec(:,ii)
			for (kk = 0; kk < ifftSizeOsf; kk++)
			{
				unsigned int tempValLd;
				short tempValI, tempValQ;
				float tempI, tempQ, tempRefI, tempRefQ;

				tempValLd = readPtr[kk];
				tempValI = (tempValLd & 0xffff);
				tempValQ = (tempValLd >> 16);

				tempRefI = refPtr[2 * kk];
				tempRefQ = refPtr[2 * kk + 1];

				tempI = tempValI * tempRefI + tempValQ * tempRefQ;
				tempQ = tempValQ * tempRefI - tempValI * tempRefQ;

				procBuffer[2 * kk] = tempI / (1 << 8);
				procBuffer[2 * kk + 1] = tempQ / (1 << 8);
			}

			// IFFT
			radix2IfftFlt(procBuffer, ifftSizeOsf, twiddleTableFlt, MAX_FFT_FLT);
			for (kk = 0; kk < ifftSizeOsf; kk++)
			{
				double sumVal = (double)procBuffer[2 * kk] * procBuffer[2 * kk];
				sumVal += (double)procBuffer[2 * kk + 1] * procBuffer[2 * kk + 1];
				corrBuffer[kk] += (float)(sumVal / ifftSizeOsf);
			}
		}
	}
	// find peak
	for (kk = 0; kk < ifftSizeOsf; kk++)
	{
		if (corrBuffer[kk] > maxVal) {
			maxVal = corrBuffer[kk];
			maxIdx = kk;
		}
	}
	// calculate phase at peak
	for (ii = 0; ii < nTx; ii++) {
		for (jj = 0; jj < nRx; jj++) {
			float valI, valQ, psiVal;
			procBuffer = (float*)tempBuffer + 2 * ifftSizeOsf * (jj + ii*nRx);
			valI = procBuffer[2 * maxIdx];
			valQ = procBuffer[2 * maxIdx + 1];

			psiVal = atan2f(valQ, valI);
			psiArray[jj + ii*nRx] = psiVal;
		}
	}
	// three point inter-polation
	{
		// interpolate peak
		unsigned int mask = ifftSizeOsf - 1;
		// shift to avoid overflow
		float ym1 = corrBuffer[(maxIdx - 1) & mask];
		float y0 = corrBuffer[maxIdx & mask];
		float yp1 = corrBuffer[(maxIdx + 1) & mask];

		float denominator = 2 * (2 * y0 - yp1 - ym1);
		float numerator = (yp1 - ym1);
		float delta = numerator / denominator;
		phi_hat = maxIdx + delta;
		if (phi_hat > ifftSizeOsf / 2)
			phi_hat -= ifftSizeOsf;
	}

	return phi_hat;
}


void wls_remove_phase(hal_pktinfo_t *pktinfo,
	float phi_hat,									// phase to remove
	float *psiArray,								// [MAX_RX*MAX_TX] float array of phase on each path
	unsigned int *fftInBuffer,						// Input buffer (freq. domain CSI)
	float *fftOutBuffer								// Output buffer float (freq. domain CSI)
	) {
	int ii, jj, kk;
	int ifftSizeOsf = 1 << (pktinfo->fftSize + 6);
	int nRx = pktinfo->nRx + 1;
	int nTx = pktinfo->nTx + 1;
	unsigned int *readPtr;
	float *procBuffer;
	float phaseVal = 2 * PI * phi_hat / ifftSizeOsf;

	for (ii = 0; ii < nTx; ii++) {
		for (jj = 0; jj < nRx; jj++) {
			float offset = psiArray[jj + ii*nRx];
			readPtr = fftInBuffer + ifftSizeOsf * (jj + ii*nRx);
			procBuffer = fftOutBuffer + 2*ifftSizeOsf * (jj + ii*nRx);
			for (kk = 0; kk < ifftSizeOsf; kk++)
			{
				unsigned int tempValLd;
				short tempValI, tempValQ;
				float tempI, tempQ;
				float cosVal, sinVal;
				cosVal = cosf(kk * phaseVal - offset);
				sinVal = sinf(kk * phaseVal - offset);

				tempValLd = readPtr[kk];
				tempValI = (tempValLd & 0xffff);
				tempValQ = (tempValLd >> 16);

				tempI = tempValI * cosVal - tempValQ * sinVal;
				tempQ = tempValI * sinVal + tempValQ * cosVal;

				procBuffer[2 * kk] = tempI / (1 << 8);
				procBuffer[2 * kk + 1] = tempQ / (1 << 8);
			}
		}
	}

}

float wls_calc_perturbation(hal_pktinfo_t *pktinfo,
	float *fftRefBuffer,								// H_old - refernce CSI buffer (freq. domain / float)
	float *fftOutBuffer									// H_comp - compensated CSI (freq. domain / float)
	)
{
	int ii, jj, kk;
	int ifftSizeOsf = 1 << (pktinfo->fftSize + 6);
	int nRx = pktinfo->nRx + 1;
	int nTx = pktinfo->nTx + 1;
	float *H_comp, *H_old;
	float perturbationVal = 0.0f;
	float perturbVal_dB;
	float H_proj[2 * MAX_RX * MAX_TX];
	float H_comp_norm[MAX_RX * MAX_TX];
	float H_ref_norm[MAX_RX * MAX_TX];

	// calculate H_proj = sum(conj(H_old).*H_comp, 1)
	for (ii = 0; ii < nTx; ii++) {
		for (jj = 0; jj < nRx; jj++) {
			float sumI = 0.0f, sumQ = 0.0f;
			float norm_comp = 0.0f, norm_ref = 0.0f;
			H_old = fftRefBuffer + 2 * ifftSizeOsf * (jj + ii*nRx);
			H_comp = fftOutBuffer + 2 * ifftSizeOsf * (jj + ii*nRx);
			for (kk = 0; kk < ifftSizeOsf; kk++)
			{
				float tempValI, tempValQ;
				float tempRefI, tempRefQ;

				tempValI = H_comp[2 * kk];
				tempValQ = H_comp[2 * kk + 1];

				tempRefI = H_old[2 * kk];
				tempRefQ = H_old[2 * kk + 1];

				sumI += tempValI * tempRefI + tempValQ * tempRefQ;
				sumQ += tempValQ * tempRefI - tempValI * tempRefQ;

				norm_comp += tempValI * tempValI + tempValQ * tempValQ;
				norm_ref += tempRefI * tempRefI + tempRefQ * tempRefQ;
			}
			H_proj[2 * (jj + ii*nRx)] = sumI / ifftSizeOsf;
			H_proj[2 * (jj + ii*nRx) + 1] = sumQ / ifftSizeOsf;
			H_comp_norm[jj + ii*nRx] = norm_comp / ifftSizeOsf;
			H_ref_norm[jj + ii*nRx] = norm_ref / ifftSizeOsf;
		}
	}

	// calculate H_diff = H_comp - H_old*diag(H_proj);
	// InitHcorr0(kk) = sum(sum(abs(H_diff).^2,2))/(N_t*N_r);
	// InitHcorr0(kk) = sum(sum(abs(H_comp). ^ 2) - 2 * abs(H_proj). ^ 2 + sum(abs(H_old). ^ 2).*abs(H_proj). ^ 2) / (N_t*N_r);
	// InitHcorr0(kk) = sum(1 - abs(H_proj). ^ 2) / (N_t*N_r);
	for (ii = 0; ii < nTx; ii++) {
		for (jj = 0; jj < nRx; jj++) {
			float H_proj_I = H_proj[2 * (jj + ii*nRx)];
			float H_proj_Q = H_proj[2 * (jj + ii*nRx) + 1];
			float abs_H_proj = H_proj_I * H_proj_I + H_proj_Q * H_proj_Q;
			float norm_comp = H_comp_norm[jj + ii*nRx];
			float norm_ref = H_ref_norm[jj + ii*nRx];
			float tempVal = 1.0f - abs_H_proj / (norm_comp * norm_ref);
			perturbationVal += tempVal;
		}
	}
	perturbationVal /= nTx * nRx;

	perturbVal_dB = 10 * log10f(perturbationVal);

	return perturbVal_dB;
}

float wls_update_cross_corr_pi_calc(
		unsigned int *headerBuffer,						// CSI BF_INFO header
		unsigned int *fftInBuffer,						// Input buffer (freq. domain CSI)
		float *fftRefBuffer,							// Refernce CSI buffer (freq. domain CSI)
		unsigned int *tempBuffer						// 2k times (MAX_RX*MAX_TX+1) scratch memory
		)
{
	int fftSize;
	unsigned int tempVec[2] = { 0,0 };
	float psiArray[MAX_RX*MAX_TX];
	float phi_hat, perturbVal;
	hal_csirxinfo_t *csirxinfo;
	hal_pktinfo_t *pktinfo;

	csirxinfo = (hal_csirxinfo_t*)headerBuffer;

	tempVec[0] = (unsigned int)csirxinfo->pktinfo;
	pktinfo = (hal_pktinfo_t*)tempVec;


#ifdef LEG_LTF_NG1
	if (pktinfo->packetType == 1) { // HT only
#else
	if (pktinfo->packetType < 3) { // HT, legacy
		if (pktinfo->packetType == 0) { // legacy
			pktinfo->sigBw = pktinfo->rxDevBw;
		}
#endif
		if (pktinfo->sigBw) { // bw > 20 MHz
			pktinfo->NgDsfShift = pktinfo->Ng + 1;
		}
		else { // 20 MHz
			pktinfo->NgDsfShift = 0;
		}
	}
	else { // VHT, HE, legacy
		pktinfo->NgDsfShift = 0;
		//pktinfo->Ng = 0; // not used
	}

	fftSize = pktinfo->sigBw + IFFT_OSF_SHIFT - pktinfo->NgDsfShift;
	pktinfo->fftSize = (fftSize < MAX_IFFT_SIZE_SHIFT) ? fftSize : MAX_IFFT_SIZE_SHIFT;

	phi_hat = wls_calc_rel_timing(pktinfo, fftInBuffer, fftRefBuffer, tempBuffer, psiArray);

	wls_remove_phase(pktinfo, phi_hat, psiArray, fftInBuffer, (float*)tempBuffer);

	perturbVal = wls_calc_perturbation(pktinfo, fftRefBuffer, (float*)tempBuffer);
#if defined(PI_ALPHA_FACTOR)
	wls_update_reference_iir(pktinfo, fftRefBuffer, (float*)tempBuffer, PI_ALPHA_FACTOR);
#elif defined(KALMAN_UPDATE)
	{
		UINT64 TSF = csirxinfo->tsf;
#ifndef RAW_HEADER
		TSF += ((UINT64)csirxinfo->tsf_hi) << 32;
#endif
		wls_update_reference_kalmman(pktinfo, fftRefBuffer, (float*)tempBuffer, TSF);
	}
#endif
	return perturbVal;
}

#endif /* CONFIG_WLS_CSI_PROC */
