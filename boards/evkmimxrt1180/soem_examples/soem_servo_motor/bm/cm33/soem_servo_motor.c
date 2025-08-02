/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdlib.h>
#include <stdio.h>

#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_netc_endpoint.h"
#include "fsl_netc_switch.h"
#include "fsl_netc_mdio.h"
#include "fsl_phyrtl8211f.h"
#include "fsl_phyrtl8201.h"
#include "fsl_msgintr.h"
#include "fsl_gpt.h"

#include "ethercattype.h"
#include "nicdrv.h"
#include "ethercatbase.h"
#include "ethercatmain.h"
#include "ethercatdc.h"
#include "ethercatcoe.h"
#include "ethercatfoe.h"
#include "ethercatconfig.h"
#include "ethercatprint.h"
#include "soem_port.h"
#include "netc_ep/soem_netc_ep.h"
#include "netc_ep/netc_ep.h"

#include "cia402.h"
#include "servo.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define CYCLE_PERIOD_NS 1000000 // 1ms

#define asda_b3_VendorId 0x000001dd
#define asda_b3_ProductID 0x00006080

#define sv680_VendorId 0x00100000
#define sv680_ProductID 0x000c0116

#define nxp_VendorId 0x00000CC2
#define nxp_ProductID 0x00000002

#define MAX_SERVO 2
#define MAX_AXIS 2

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern struct netc_ep_if_port if_port;

volatile uint64_t system_time_ns = 0;

/* Buffer descriptor and buffer memeory. */
typedef uint8_t rx_buffer_t[EP_RXBUFF_SIZE_ALIGN];

AT_NONCACHEABLE_SECTION_ALIGN(static netc_rx_bd_t g_rxBuffDescrip[EP_RING_NUM][EP_RXBD_NUM], EP_BD_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static netc_tx_bd_t g_txBuffDescrip[EP_RING_NUM][EP_TXBD_NUM], EP_BD_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static rx_buffer_t g_rxDataBuff[EP_RING_NUM][EP_RXBD_NUM], EP_BUFF_SIZE_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t g_txFrame[EP_TXBUFF_SIZE], EP_BUFF_SIZE_ALIGN);
uint64_t rxBuffAddrArray[EP_RING_NUM][EP_RXBD_NUM];

static netc_tx_frame_info_t g_txDirty[EP_RING_NUM][EP_TXBD_NUM];

static char IOmap[1500];

static char *tp[MAX_SERVO] = {

// DELTA
"Cyclic=1; Scale=46603; Bias=0; Accel=8; Decel=8; Max_speed=3600; TpArrays=[(0:2000),(270:1000),(270:2000),(180:1000),(180:2000),(0:1000),(0:2000),(0:1000)];",

// INOVANCE
"Cyclic=1; Scale=186414; Bias=0; Accel=8; Decel=8; Max_speed=3600; TpArrays=[(0:2000),(270:1000),(270:2000),(180:1000),(180:2000),(0:1000),(0:2000),(0:1000)];",

};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

int _write(int handle, char *buffer, int size)
{
    if (NULL == buffer)
    {
        /* return -1 if error. */
        return -1;
    }

    /* This function only writes to "standard out" and "standard err" for all other file handles it returns failure. */
    if ((handle != 1) && (handle != 2))
    {
        return -1;
    }

    int i;
    for(i = 0; i < size; i++) {
        if (buffer[i] == '\n') {
            DbgConsole_Putchar('\r');
        }
        DbgConsole_Putchar(buffer[i]);
    }

    return size;
}

struct servo_t servo[MAX_SERVO];
struct axis_t axis[MAX_AXIS];

static void EtherCAT_servo_init(struct servo_t *svo, struct axis_t *ax)
{
	int i;
	memset(svo, 0, sizeof(struct servo_t) * MAX_SERVO);
	memset(ax, 0, sizeof(struct axis_t) * MAX_AXIS);
	for (i = 0; i < MAX_SERVO; i++) {
		svo[i].slave_id = i;
		svo[i].axis_num = 1;
	}
	
	for (i = 0; i < 1; i++) {
		svo[i].VendorId = asda_b3_VendorId;
		svo[i].ProductID = asda_b3_ProductID;
	}

    for (i = 1; i < 2; i++) {
		svo[i].VendorId = sv680_VendorId;
		svo[i].ProductID = sv680_ProductID;
	}

	for (i = 0; i < MAX_SERVO; i++) {
		ax[i].servo = svo + i;
		ax[i].axis_offset = 0;
	}
}

static int general_servo_setup(uint16 slave) {
	int i;
	int ret = 0;
	int chk = 10;
	struct servo_t *svo = NULL;
	for (i = 0; i < MAX_SERVO; i++) {
		if (servo[i].slave_id + 1 == slave) {
			svo = &servo[i];
			break;
		}
	}
	if (svo) {
		while (chk--) {
			ret = servo_pdo_remap(svo);
			if (ret)
				break;
		}
	}

	if (!ret)
		return 0;
	else
		return -1;
}


static int nxp_servo_setup(uint16 slave) {
	int i;
	int ret = 0;
	int chk = 10;
	struct servo_t *svo = NULL;
	for (i = 0; i < MAX_SERVO; i++) {
		if (servo[i].slave_id + 1 == slave) {
			svo = &servo[i];
			break;
		}
	}
	if (svo) {
		while (chk--) {
			ret = servo_pdo_activate_map(svo);
			if (ret)
				break;
		}
	}

	if (!ret)
		return 0;
	else
		return -1;
}

static int asda_b3_servo_setup(uint16 slave) {
	int i;
	int ret = 0;
	int chk = 10;
	struct servo_t *svo = NULL;
	for (i = 0; i < MAX_SERVO; i++) {
		if (servo[i].slave_id + 1 == slave) {
			svo = &servo[i];
			break;
		}
	}
	if (svo) {
		while (chk--) {
			ret = servo_pdo_remap(svo);
			if (ret)
				break;
		}
	}

	if (!ret)
		return 0;

	int8_t  Obj60c2[9][2] = {{12, -5},{25, -5}, {37, -5}, {5, -4},{62, -5}, {75, -5},{87, -5},{1, -3}, {2, -3}};
	int8_t num_8b[2];
	int wkc = 0;
	if (CYCLE_PERIOD_NS > 1000000) {
		num_8b[0] = CYCLE_PERIOD_NS / 1000000;
		num_8b[1] = -3;
	} else {
		int index = ((CYCLE_PERIOD_NS - 1) / 125000);
		num_8b[0] = Obj60c2[index][0];
		num_8b[1] = Obj60c2[index][1];
	}
	int obj_60c2_index = 0x60c2;
	for ( i = 0; i < svo->axis_num; i++) {
		obj_60c2_index += i * 0x800;
		wkc += ec_SDOwrite(slave, obj_60c2_index, 0x01, 0, 1, &num_8b[0], EC_TIMEOUTSAFE);
		wkc += ec_SDOwrite(slave, obj_60c2_index, 0x02, 0, 1, &num_8b[1], EC_TIMEOUTSAFE);
	}
	return wkc == svo->axis_num * 2 ? 1 : 0;
}

static void servo_setup(struct servo_t *servo, int servo_num) {
	int i;
	for (i = 0; i < servo_num; i++) {
		if (servo[i].VendorId == asda_b3_VendorId && servo[i].ProductID == asda_b3_ProductID) {
			servo[i].slave->PO2SOconfig = asda_b3_servo_setup;
			PRINTF("\r\n delta_servo_setup success!\r\n");
		} else if (servo[i].VendorId == nxp_VendorId && servo[i].ProductID == nxp_ProductID) {
			servo[i].slave->PO2SOconfig = nxp_servo_setup;
			PRINTF("\r\n nxp_servo_setup success!\r\n");
		} else {
			servo[i].slave->PO2SOconfig = general_servo_setup;
		}

	}
}

void irq_wake_task(void);

void OSAL_TIMER_IRQHandler(void)
{
	/* Clear interrupt flag. */
	GPT_ClearStatusFlags(OSAL_TIMER, kGPT_OutputCompare1Flag);

	system_time_ns += CLOCK_INCREASE_PER_SEC;

	irq_wake_task();
	/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F, Cortex-M7, Cortex-M7F Store immediate overlapping
 	 exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U || __CORTEX_M == 7U)
	__DSB();
#endif
}

static void osal_timer_init(uint32_t priority)
{
	uint32_t gptFreq;
	gpt_config_t gptConfig;

	assert(priority < (1UL << __NVIC_PRIO_BITS));

	GPT_GetDefaultConfig(&gptConfig);
	GPT_Init(OSAL_TIMER, &gptConfig);
	gptFreq = OSAL_TIMER_CLK_FREQ;
	/* Divide GPT clock source frequency to 1MHz */
	GPT_SetClockDivider(OSAL_TIMER, gptFreq / CLOCK_GRANULARITY_FRE);
	/* Set both GPT modules to 1 second duration */
	GPT_SetOutputCompareValue(OSAL_TIMER, kGPT_OutputCompare_Channel1, CLOCK_GRANULARITY_FRE);
	/* Enable GPT Output Compare1 interrupt */
	GPT_EnableInterrupts(OSAL_TIMER, kGPT_OutputCompare1InterruptEnable);
	/* Enable at the Interrupt */
	NVIC_SetPriority(OSAL_TIMER_IRQ_ID, priority);
	EnableIRQ(OSAL_TIMER_IRQ_ID);
	GPT_StartTimer(OSAL_TIMER);
}

void msgintrCallback(MSGINTR_Type *base, uint8_t channel, uint32_t pendingIntr)
{
    /* Transmit interrupt */
    if ((pendingIntr & (1U << TX_INTR_MSG_DATA)) != 0U)
    {
        EP_CleanTxIntrFlags(&if_port.g_ep_handle, 1, 0);
        if_port.txOver = true;
    }
    /* Receive interrupt */
    if ((pendingIntr & (1U << RX_INTR_MSG_DATA)) != 0U)
    {
        EP_CleanRxIntrFlags(&if_port.g_ep_handle, 1);
    }
}

static status_t ReclaimCallback(ep_handle_t *handle, uint8_t ring, netc_tx_frame_info_t *frameInfo, void *userData)
{
    return kStatus_Success;
}

static uint64_t gettime()
{
	uint64_t nsec_base;
	uint32_t cur_nsec;

	nsec_base  = system_time_ns;
	cur_nsec   = GPT_GetCurrentTimerCount(OSAL_TIMER);

	if (nsec_base != system_time_ns)
	{
		nsec_base  = system_time_ns;
		cur_nsec   = GPT_GetCurrentTimerCount(OSAL_TIMER);
	}
	return nsec_base + cur_nsec * CLOCK_GRANULARITY_NS;
}

static void nsleep_to (uint64_t nsec_target)
{
	while (nsec_target > gettime());
}

void osal_gettime(struct timeval *current_time)
{
	uint64_t nsec = gettime();
	
	current_time->tv_sec  = nsec / CLOCK_INCREASE_PER_SEC;
	current_time->tv_usec = (nsec % CLOCK_INCREASE_PER_SEC) / 1000;

	return;
}

static netc_rx_bdr_config_t rxBdrConfig = {0};
static netc_tx_bdr_config_t txBdrConfig = {0};
static netc_bdr_config_t bdrConfig;
static netc_msix_entry_t msixEntry[2];
static ep_config_t ep_config;

/* OSHW: register enet port to SOEM stack */
static int if_port_init(void)
{
	struct soem_if_port soem_port;

    bdrConfig.rxBdrConfig = &rxBdrConfig;
	bdrConfig.txBdrConfig = &txBdrConfig;
    uint32_t msgAddr;

    PRINTF("\r\nNETC EP frame loopback example start.\r\n");

    /* MSIX and interrupt configuration. */
    MSGINTR_Init(MSGINTR1, &msgintrCallback);
    msgAddr              = MSGINTR_GetIntrSelectAddr(MSGINTR1, 0);
    msixEntry[0].control = kNETC_MsixIntrMaskBit;
    msixEntry[0].msgAddr = msgAddr;
    msixEntry[0].msgData = TX_INTR_MSG_DATA;
    msixEntry[1].control = kNETC_MsixIntrMaskBit;
    msixEntry[1].msgAddr = msgAddr;
    msixEntry[1].msgData = RX_INTR_MSG_DATA;

    /* BD ring configuration. */
    bdrConfig.rxBdrConfig[0].bdArray       = &g_rxBuffDescrip[0][0];
    bdrConfig.rxBdrConfig[0].len           = EP_RXBD_NUM;
    bdrConfig.rxBdrConfig[0].buffAddrArray = &rxBuffAddrArray[0][0];
    bdrConfig.rxBdrConfig[0].buffSize      = EP_RXBUFF_SIZE_ALIGN;
    bdrConfig.rxBdrConfig[0].msixEntryIdx  = RX_MSIX_ENTRY_IDX;
    bdrConfig.rxBdrConfig[0].extendDescEn  = false;
    bdrConfig.rxBdrConfig[0].enThresIntr   = true;
    bdrConfig.rxBdrConfig[0].enCoalIntr    = true;
    bdrConfig.rxBdrConfig[0].intrThreshold = 1;

    bdrConfig.txBdrConfig[0].bdArray      = &g_txBuffDescrip[0][0];
    bdrConfig.txBdrConfig[0].len          = EP_TXBD_NUM;
    bdrConfig.txBdrConfig[0].dirtyArray   = &g_txDirty[0][0];
    bdrConfig.txBdrConfig[0].msixEntryIdx = TX_MSIX_ENTRY_IDX;
    bdrConfig.txBdrConfig[0].enIntr       = true;

    /* Endpoint configuration. */
    (void)EP_GetDefaultConfig(&ep_config);
    ep_config.si                    = KNETC_EP_CONFIG_SI;
    ep_config.siConfig.txRingUse    = 1;
    ep_config.siConfig.rxRingUse    = 1;
    ep_config.reclaimCallback       = ReclaimCallback;
    ep_config.msixEntry             = &msixEntry[0];
    ep_config.entryNum              = 2;
    ep_config.port.ethMac.miiMode   = KNETC_HW_MII_MODE;
    ep_config.port.ethMac.miiSpeed  = kNETC_MiiSpeed100M;
    ep_config.port.ethMac.miiDuplex = kNETC_MiiFullDuplex;
#ifdef EXAMPLE_ENABLE_CACHE_MAINTAIN
    ep_config.rxCacheMaintain = true;
    ep_config.txCacheMaintain = true;
#endif
    
	for (uint8_t ring = 0U; ring < EP_RING_NUM; ring++)
    {
        for (uint8_t index = 0U; index < EP_RXBD_NUM; index++)
        {
            rxBuffAddrArray[ring][index] = (uint64_t)(uintptr_t)&g_rxDataBuff[ring][index];
        }
    }
	if_port.ep_config = &ep_config;
	if_port.bdrConfig = &bdrConfig;
	if_port.g_txFrame = g_txFrame;
	strncpy(soem_port.ifname, SOEM_PORT_NAME, SOEM_IF_NAME_MAXLEN);
	strncpy(soem_port.dev_name, "net_ep", SOEM_DEV_NAME_MAXLEN);
	soem_port.port_init = netc_ep_init;
	soem_port.port_send = netc_ep_send;
	soem_port.port_recv = netc_ep_recv;
	soem_port.port_close = netc_ep_close;
	soem_port.port_link_status= netc_ep_link_status;
	soem_port.port_pri = &if_port;
    return register_soem_port(&soem_port);
}

void irq_wake_task(void)
{
    return;
}

void control_task(char *ifname)
{
	int expectedWKC;
	volatile int wkc;
	int chk, i;
	uint64_t target_time;
    int wkc_lost = 0;
    uint64_t curr_time;
	RGPIO_WritePinOutput(BOARD_LED_RGPIO, BOARD_LED_RGPIO_PIN, 1);
	PRINTF("Starting motion task\r\n");
	EtherCAT_servo_init(servo, axis);

	/* initialise SOEM, and if_port */
	if (ec_init(ifname)) {
		PRINTF("ec_init on %s succeeded.\n",ifname);
		/* find and auto-config slaves */
		if ( ec_config_init(FALSE) > 0 ) {
			PRINTF("%d slaves found and configured.\n",ec_slavecount);
			if (ec_slavecount < MAX_SERVO) {
				PRINTF("The number of Servo scanned is not consistent with configed, please reconfirm\n");
				return;
			}

			i = servo_slave_check(servo, MAX_SERVO);
			if (i < 0) {
				PRINTF("The infomation of Servo:%d is not consistent with scanned, please reconfirm\n", -i);
				return;
			}

			ec_configdc();
			chk = 100;
			while (chk--) {
				if (servo_synced_check(servo, MAX_SERVO) == 1) {
					break;
				}
			}

			for (i = 0; i < MAX_SERVO; i++) {
				if(servo[i].slave->hasdc > 0) {
				ec_dcsync0(servo[i].slave_id + 1, TRUE, CYCLE_PERIOD_NS, CYCLE_PERIOD_NS * 3);
				}
			}

			servo_setup(servo, MAX_SERVO);	
			ec_config_map(&IOmap);
			for (i = 0; i < MAX_AXIS; i++) {
				axis_nc_init(&axis[i], tp[i], CYCLE_PERIOD_NS);
			}
			PRINTF("Slaves mapped, state to SAFE_OP.\n");
			/* wait for all slaves to reach SAFE_OP state */
			ec_statecheck(0, EC_STATE_SAFE_OP,  EC_TIMEOUTSTATE * 4);
			for (i = 0; i < MAX_AXIS; i++) {
				PDO_write_targe_position(&axis[i], axis[i].current_position);
				axis_nc_start(&axis[i]);
			}
			PRINTF("segments : %d : %lu %lu %lu %lu\n",ec_group[0].nsegments ,ec_group[0].IOsegment[0],ec_group[0].IOsegment[1],ec_group[0].IOsegment[2],ec_group[0].IOsegment[3]);

			PRINTF("Request operational state for all slaves\n");
			expectedWKC = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;
			PRINTF("Calculated workcounter %d\n", expectedWKC);
			ec_slave[0].state = EC_STATE_OPERATIONAL;
			/* send one valid process data to make outputs in slaves happy*/
			ec_send_processdata();
			ec_receive_processdata(EC_TIMEOUTRET);
			/* request OP state for all slaves */
			ec_writestate(0);
			chk = 500;
			/* wait for all slaves to reach OP state */
			do {
				ec_send_processdata();
				ec_receive_processdata(EC_TIMEOUTRET);
				ec_statecheck(1, EC_STATE_OPERATIONAL, 50000);
			} while (chk-- && (ec_slave[0].state != EC_STATE_OPERATIONAL));

			for (i = 0; i < MAX_AXIS; i++) {
				PDO_write_targe_position(&axis[i], axis[i].current_position);
			}

			PRINTF("Request operational state for all slaves\r\n");
			expectedWKC = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;
			PRINTF("Calculated workcounter %d\r\n", expectedWKC);
			ec_slave[0].state = EC_STATE_OPERATIONAL;
			/* send one valid process data to make outputs in slaves happy*/
			ec_send_processdata();
			ec_receive_processdata(EC_TIMEOUTRET);
			/* request OP state for all slaves */
			ec_writestate(0);
			chk = 500;
			/* wait for all slaves to reach OP state */
			do {
				ec_send_processdata();
				ec_receive_processdata(EC_TIMEOUTRET);
				ec_statecheck(1, EC_STATE_OPERATIONAL, 50000);
			} while (chk-- && (ec_slave[0].state != EC_STATE_OPERATIONAL));

			
			if (ec_slave[0].state != EC_STATE_OPERATIONAL) {
				PRINTF("Not all slaves reached operational state.\r\n");
			} else {
				PRINTF("Operational state reached for all slaves.\r\n");
				/* send one valid process data to make outputs in slaves happy*/
				ec_send_processdata();
				
				target_time = gettime();
				int op_num = 0;
				while (1) {
					RGPIO_WritePinOutput(BOARD_LED_RGPIO, BOARD_LED_RGPIO_PIN, 1);
					target_time += CYCLE_PERIOD_NS;
					/* SOEM receive data */
					wkc = ec_receive_processdata(EC_TIMEOUTRET);

					/* servo motor application processing code */
					for(i = 0; i < MAX_AXIS; i++) {
						if (axis_start(&axis[i], op_mode_csp) != 1) {
								op_num++;
						}
					}
					if (op_num == 0) {
						if(wkc >= expectedWKC) {
							for(i = 0; i < MAX_AXIS; i++) {
								axis[i].current_velocity = PDO_read_actual_velocity(&axis[i]);
								axis[i].current_position = PDO_read_actual_position(&axis[i]);
								if (axis[i].axis_status.csp_status == csp_status_running || axis[i].axis_status.csp_status == csp_status_pre_stop) {
									int pos = axis_nc_get_next_pos(&axis[i]);
									PDO_write_targe_position(&axis[i], pos);
								}
							} 
							if (axis[MAX_AXIS-1].axis_status.csp_status == csp_status_stop) {
								break;
							}
						} else {
							wkc_lost++;
							ec_slave[0].state = EC_STATE_OPERATIONAL;
							ec_writestate(0);
						}
					} else {
						op_num = 0;
					}

					/* SOEM transmit data */
					ec_send_processdata();
					curr_time = gettime();
					RGPIO_WritePinOutput(BOARD_LED_RGPIO, BOARD_LED_RGPIO_PIN, 0);

					if (curr_time < target_time) {
						nsleep_to(target_time);
					} else {
						PRINTF("expired\r\n");
					}
				}
				PRINTF("wkc_lost = %d\r\n", wkc_lost);
				PRINTF("\r\nRequest init state for all slaves\r\n");
				for(i = 1; i<=ec_slavecount ; i++) {
					if(ec_slave[i].state != EC_STATE_OPERATIONAL) {
						PRINTF("Slave %d State=0x%2.2x StatusCode=0x%4.4x : %s\r\n",
						i, ec_slave[i].state, ec_slave[i].ALstatuscode, ec_ALstatuscode2string(ec_slave[i].ALstatuscode));
					}
				}

				ec_slave[0].state = EC_STATE_INIT;
				/* request INIT state for all slaves */
				ec_writestate(0);
			}
			/* stop SOEM, close socket */
			ec_close();
		} else {
			PRINTF("No socket connection on %s\nExecute as root\r\n",ifname);
		}
	}
	return;
}

/*!
 * @brief Main function
 */
int main(void)
{
	BOARD_InitHardware();

	PRINTF("Start the soem_servo_motor baremetal example...\r\n");

	osal_timer_init(0);
	if_port_init();
	control_task(SOEM_PORT_NAME);
	return 0;
}

