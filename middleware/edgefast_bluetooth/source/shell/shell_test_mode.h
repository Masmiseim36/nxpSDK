/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _SHELL_TEST_MODE_H__
#define _SHELL_TEST_MODE_H__
#include "BT_common.h"



#define BT_OGF_TEST 0x06

#define BT_HCI_OP_ENABLE_TEST_MODE BT_OP(BT_OGF_TEST,0x03)

#define BT_HCI_OP_LE_SET_TX_POWER               BT_OP(BT_OGF_VS, 0x0087)
STRUCT_PACKED_PRE
struct bt_hci_le_config {
	uint8_t  tx_power;
} STRUCT_PACKED_POST;



STRUCT_PACKED_PRE
struct bt_hci_command {
	uint8_t ogf;
	uint8_t ocf;
	uint16_t opcode;
	uint8_t param_len;
};


/**
 *  \brief To start a test where the DUT generates test reference
 *         packets at a fixed interval.
 *
 *  \par Description:
 *       This API sets the transmit test parameters. An HCI Reset command is required
 *       after this test to resume normal Bluetooth operations.
 *  \param [in] RxOnStart
 *				SyntOnStart
 *				TxOnStart
 *				PhdOffStart
 *		   		These 4 parameters should be set to 0x80.
 *  \param [in] TestScenario
 *  			0x01 = PATTERN_00 (data pattern: 0x00)
 *				0x02 = PATTERN_FF (data pattern: 0xFF)
 *				0x03 = PATTERN_55 (data pattern: 0x55)
 *				0x04 = PATTERN_PRBS (data pattern: 0xFE)
 *				0x09 = PATTERN_0F (data pattern: 0x0F)
 *				0xFF = exit test
 *	\param [in]	HoppingMode
 *				0x00 = fix frequency
 *				0x01 = hopping set
 *  \param [in] tx_channel
 *         		Transmit Channel<br>
 *         		tx_channel = (F - 2402) / 2<br>
 *         		Value Range: 0x00 to 0x27.<br>
 *         		Frequency Range : 2402 MHz to 2480 MHz
 *  \param [in] rx_channel
 *         		Receive Channel<br>
 *         		tx_channel = (F - 2402) / 2<br>
 *         		Value Range: 0x00 to 0x27.<br>
 *         		Frequency Range : 2402 MHz to 2480 MHz
 *  \param [in] TxTestInterval
 *				Poll interval in frames for the link (units, 1.25 ms)
 *				Max Poll interval is 0x0E
 *	\param [in] PacketType
 *				Transmit Packet Type
 *				0x03 = DM1
 *				0x04 = DH1
 *				0x0A = DM3
 *				0x0B = DH3
 *				0x0E = DM5
 *				0x0F = DH5
 *				0x14 = 2-DH1
 *				0x18 = 3-DH1
 *				0x1A = 2-DH3
 *				0x1B = 3-DH3
 *				0x1E = 2-DH5
 *				0x1F = 3-DH5
 *  \param [in] test_data_length
 *         		Length in bytes of payload data in each packet<br>
 *         		Range: 0x00 to 0xFF
 *  \param [in] Whitening
 *  			0x00 = disabled
 *  			0x01 = enabled
 *  \param [in] Number of Test Packets
 *  			0 = infinite
 *  \param [in] Tx Power
 *  			Signed value of Tx power (dBm)
 *  			Range = -20 dBm to 12 dBm
 *
 *  \return void
 *
 */

#define BT_HCI_OP_TX_TEST                BT_OP(BT_OGF_VS, 0x0019)
STRUCT_PACKED_PRE
struct bt_hci_cp_tx_test {
	uint8_t          rx_on_start;
	uint8_t 	     synt_on_start;
	uint8_t 	     tx_on_start;
	uint8_t          phd_off_start;
	uint8_t 	     test_scenario;
	uint8_t 	     hopping_mode;
	uint8_t          tx_channel;
	uint8_t 	     rx_channel;
	uint8_t 	     tx_test_interval;
	uint8_t 	     pkt_type;
	uint8_t 	     data_length[2];
	uint8_t 	     whitening;
	uint8_t 	     num_pkt[4];
	uint8_t 	     tx_pwr;
} STRUCT_PACKED_POST;

/**
 *  \brief To start a test where the DUT generates test reference
 *         packets at a fixed interval.
 *
 *  \par Description:
 *       This API sets the receive test parameters. An HCI Reset command is required
 *		 after this test to resume normal Bluetooth operations.
 *
 *  \param [in] TestScenario
 *  			0x01 = receiver test, 0-pattern
 *  			0x02 = receiver test, 1-pattern
 *  			0x03 = receiver test, 1010-pattern
 *  			0x04 = receiver test, PRBS-pattern
 *  			0x09 = receiver test, 1111 0000-pattern
 *  			0xFF = abort test mode
 *  \param [in] tx_channel
 *         		Transmit Channel<br>
 *         		tx_channel = (F - 2402) / 2<br>
 *         		Value Range: 0x00 to 0x27.<br>
 *         		Frequency Range : 2402 MHz to 2480 MHz
 *  \param [in] rx_channel
 *         		Receive Channel<br>
 *         		tx_channel = (F - 2402) / 2<br>
 *         		Value Range: 0x00 to 0x27.<br>
 *         		Frequency Range : 2402 MHz to 2480 MHz
 *	\param [in] TestPacketType
 *				Transmit Packet Type
 *				0x03 = DM1
 *				0x04 = DH1
 *				0x0A = DM3
 *				0x0B = DH3
 *				0x0E = DM5
 *				0x0F = DH5
 *				0x14 = 2-DH1
 *				0x18 = 3-DH1
 *				0x1A = 2-DH3
 *				0x1B = 3-DH3
 *				0x1E = 2-DH5
 *				0x1F = 3-DH5
 *	\param [in] Expected number of packets
 *  \param [in] test_data_length
 *         		Should not be bigger than the maximum size of the
 *				specified test packet type
 *  \param [in] Tx AM Address
 *  			Default = 0x01
 *  \param [in] Transmitter BD Address
 *  			This is used to derive access code
 *  \param [in] Report Error Packets
 *  			0x00 = none (default)
 *  			0x01 to 0xFE = number of packets to report
 *  \return void
 *
 */

#define BT_HCI_OP_RX_TEST                BT_OP(BT_OGF_VS, 0x0018)
STRUCT_PACKED_PRE
struct bt_hci_cp_rx_test {
	uint8_t	       test_scenario;
	uint8_t        tx_channel;
	uint8_t        rx_channel;
	uint8_t        pkt_type;
	uint8_t	   	   num_pkt[4];
	uint8_t        data_length[2];
	uint8_t 	   tx_am_addr;
	uint8_t        tx_addr[BT_BD_ADDR_SIZE];
	uint8_t 	   report_err_pkt;
} STRUCT_PACKED_POST;

#endif /* _SHELL_TEST_MODE_H__ */
