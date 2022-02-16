/*
 * The Clear BSD License
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


 /* This is a Serial NOR Configuration Block definition for Adesto's EcoXiP 
  * flash (ATXP032). Among other parameters it configues the system to operate
  * with:
  * - Octal-SPI
  * - Double Data Rate (DDR)
  * - SCLK = 133MHz
  */

#include "flexspi_flash.h"
#if defined XIP_BOOT_OCTASPI	// Is defined in the iMXRT CPU Support package depended on the selected placement

/*******************************************************************************
 * Code
 ******************************************************************************/
#if defined(__CC_ARM) || defined(__GNUC__) || defined __SES_ARM || defined __CROSSWORKS_ARM
	__attribute__((section(".boot_hdr.conf")))
#elif defined(__ICCARM__)
	#pragma section="app_image"
	#pragma location=".boot_hdr.conf"
	__root
#else
	#error "Unknown Compiler"
#endif

#define DUMMY_CYCLES 18	/* Number of dummy cycles after Read Command for Adesto-Flash */
#define CTRL_REG_BYTE3_VAL (((DUMMY_CYCLES - 8) >> 1) | 0x10)

const flexspi_nor_config_t FlashBootHeader =
{
	.memConfig =
	{
		.tag                  = FLEXSPI_CFG_BLK_TAG,
		.version              = FLEXSPI_CFG_BLK_VERSION,
		.readSampleClkSrc     = kFlexSPIReadSampleClk_ExternalInputFromDqsPad,
		.csHoldTime           = 3U,
		.csSetupTime          = 3U,
		.columnAddressWidth   = 0U,
		.deviceModeCfgEnable  = 1U,
		// Sequence for changing device mode. In this sequence we write to status/control regs 2-3.
		// This will switch EcoXiP to Octal-DDR mode and modify the number of dummy cycles used by it.
		.deviceModeSeq        = {.seqId=14, .seqNum=1}, // index/size Status/Control Registers sequence
		.deviceModeArg        = 0x88 | (CTRL_REG_BYTE3_VAL << 8), // values to be written to status/control regs 2-3
		// Enable DDR mode, Safe configuration
		.controllerMiscOption = (1u << kFlexSpiMiscOffset_DdrModeEnable) | (1u << kFlexSpiMiscOffset_SafeConfigFreqEnable),
		.deviceType           = kFlexSpiDeviceType_SerialNOR, // serial NOR
		.sflashPadType        = kSerialFlash_8Pads,
		.serialClkFreq        = kFlexSpiSerialClk_133MHz,
		.lutCustomSeqEnable   = 0, // Use pre-defined LUT sequence index and number
		.sflashA1Size         = 8 * 1024U * 1024U,
		.dataValidTime        = {[0] = 20}, //2ns from DQS to data
		.busyOffset           = 0U, // busy bit in bit 0
		.busyBitPolarity      = 0U, // busy bit is 1 when device is busy
		.lookupTable =
		{
			// (0) Read
			[ 0] = FLEXSPI_LUT_SEQ (CMD_SDR,   FLEXSPI_8PAD, 0x0B, RADDR_DDR, FLEXSPI_8PAD, 0x20),
			[ 1] = FLEXSPI_LUT_SEQ (DUMMY_DDR, FLEXSPI_8PAD,(DUMMY_CYCLES*2+1), READ_DDR, FLEXSPI_8PAD, 0x80),

			// (1) Read Status (byte 1)
			[ 4] = FLEXSPI_LUT_SEQ (CMD_SDR,   FLEXSPI_8PAD, 0x05, DUMMY_DDR, FLEXSPI_8PAD, 0x08),
			[ 5] = FLEXSPI_LUT_SEQ (READ_DDR,  FLEXSPI_8PAD, 0x01, STOP,      FLEXSPI_1PAD, 0x0),

			// (3) Write Enable
			[12] = FLEXSPI_LUT_SEQ (CMD_SDR,   FLEXSPI_1PAD, 0x06, STOP,      FLEXSPI_1PAD, 0x0),

			// (4) Page Program
			[16] = FLEXSPI_LUT_SEQ (CMD_SDR,   FLEXSPI_8PAD, 0x02, RADDR_DDR, FLEXSPI_8PAD, 32),
			[17] = FLEXSPI_LUT_SEQ (WRITE_SDR, FLEXSPI_8PAD, 128,  STOP,      FLEXSPI_1PAD, 0),

			// (5) Write Status Register Byte 1
			[20] = FLEXSPI_LUT_SEQ(CMD_SDR,   FLEXSPI_1PAD, 0x01, WRITE_SDR, FLEXSPI_1PAD, 1),

			// (6) Write Status Register Byte 2
			[24] = FLEXSPI_LUT_SEQ(CMD_SDR,   FLEXSPI_1PAD, 0x31, WRITE_SDR, FLEXSPI_1PAD, 1),

			// (14) Write Status/Control Registers (this specifc sequence will writes 2 bytes to status/control regs 2-3)
			[56] = FLEXSPI_LUT_SEQ (CMD_SDR,   FLEXSPI_1PAD, 0x71, CMD_SDR,   FLEXSPI_1PAD, 0x02),
			[57] = FLEXSPI_LUT_SEQ (WRITE_SDR, FLEXSPI_1PAD, 0x02, STOP,      FLEXSPI_1PAD, 0x0),
		},
	},
	.pageSize           = 256U,
	.sectorSize         = 4U * 1024U, // 4K - that's actually a block not a sector (has to match erase size)
	.ipcmdSerialClkFreq = 1, // 30MHz
	.blockSize          = 4U * 1024U,
	.isUniformBlockSize = true,
};

#endif	// #if defined XIP_BOOT_OCTASPI
