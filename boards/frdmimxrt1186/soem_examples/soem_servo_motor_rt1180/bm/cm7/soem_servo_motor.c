/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdlib.h>
#include <stdio.h>

#include "cia402.h"
#include "servo.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define CYCLE_PERIOD_NS 	250000 // 250us

#define ECAT_SUBDEVICE_1180_XSERVO 	1
#define ECAT_SUBDEVICE_RT1180_EVK 	0
#define ECAT_SUBDEVICE_IMX943_EVK 	0

#define nxp_VendorId 		0x00000CC2
#define nxp_ProductID 		0x00000002

#define MAX_SERVO 	1
#define MAX_AXIS 	1

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* dc sync time */
uint64_t dc_start_time_ns = 0LL;
extern int32_t  dc_diff_ns;

static char IOmap[1500];

static char *tp[MAX_AXIS] = {

/* NXP i.MXRT1180EVK */
#if ((defined(ECAT_SUBDEVICE_RT1180_EVK) && ECAT_SUBDEVICE_RT1180_EVK))
"Cyclic=1; Scale=22; Bias=0; Accel=8; Decel=8; Max_speed=3600; TpArrays=[(0:2000),(270:1000),(270:2000),(180:1000),(180:2000),(0:1000),(0:2000),(0:1000)];",
#endif

/* NXP XSERVO-MTR-DRV RT1180 */
#if ((defined(ECAT_SUBDEVICE_1180_XSERVO) && ECAT_SUBDEVICE_1180_XSERVO))
"Cyclic=1; Scale=364; Bias=0; Accel=8; Decel=8; Max_speed=3600; TpArrays=[(0:2000),(270:1000),(270:2000),(180:1000),(180:2000),(0:1000),(0:2000),(0:1000)];",
#endif

/* NXP i.MX943 */
#if ((defined(ECAT_SUBDEVICE_IMX943_EVK) && ECAT_SUBDEVICE_IMX943_EVK))
"Cyclic=1; Scale=93206; Bias=0; Accel=8; Decel=8; Max_speed=3600; TpArrays=[(0:2000),(270:1000),(270:2000),(180:1000),(180:2000),(0:1000),(0:2000),(0:1000)];",
#endif
};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

 int __write(int handle, char *buffer, int size)
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
	
	for (i = 0; i < MAX_SERVO; i++) {
		svo[i].VendorId = nxp_VendorId;
		svo[i].ProductID = nxp_ProductID;
	}

	for (i = 0; i < MAX_AXIS; i++) {
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

static void servo_setup(struct servo_t *servo, int servo_num) {
	int i;
	for (i = 0; i < servo_num; i++) {
		if (servo[i].VendorId == nxp_VendorId && servo[i].ProductID == nxp_ProductID) {
			servo[i].slave->PO2SOconfig = nxp_servo_setup;
			PRINTF("\r\n nxp_servo_setup success!\r\n");
		} else {
			servo[i].slave->PO2SOconfig = general_servo_setup;
		}

	}
}

void control_task(char *ifname)
{
	int expectedWKC;
	volatile int wkc;
	int chk, i;
	uint64_t target_time;
    int wkc_lost = 0;
    uint64_t curr_time;
	PRINTF("Starting motion task\r\n");
	EtherCAT_servo_init(servo, axis);

	/* initialise SOEM, and if_port */
	if (ec_init(ifname)) {
#if !(defined(EXAMPLE_NETC_HAS_NO_SWITCH) && EXAMPLE_NETC_HAS_NO_SWITCH)
        if (kStatus_Success != APP_SWT_AddTableEntry())
        {
            PRINTF("APP_SWT_AddTableEntry failed.\r\n");
            return;
        }
#endif
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
				ec_dcsync0(servo[i].slave_id + 1, TRUE, CYCLE_PERIOD_NS, CYCLE_SHIFT_NS);
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
				chk = 100;
				target_time = system_time64_ns();
				do {
					dc_start_time_ns = system_time64_ns();
					ec_send_processdata();
					if (ec_receive_processdata(EC_TIMEOUTRET) >= expectedWKC) {
						dc_diff_ns = dc_start_time_ns - *ecx_context.DCtime;
						update_master_clock();
					}
					target_time += CYCLE_PERIOD_NS;
					nsleep_to(target_time);
				} while (chk--);
				
				target_time = system_time64_ns();
				target_time += CYCLE_PERIOD_NS;
				target_time = target_time/CYCLE_PERIOD_NS * CYCLE_PERIOD_NS;
				nsleep_to(target_time);
				dc_start_time_ns = system_time64_ns();
				ec_send_processdata();
				int op_num = 0;
				while (1) {
					target_time += CYCLE_PERIOD_NS;
					/* SOEM receive data */
					wkc = ec_receive_processdata(EC_TIMEOUTRET);
					if (wkc >= expectedWKC) {
						dc_diff_ns = dc_start_time_ns - *ecx_context.DCtime;
						update_master_clock();
					}

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
					dc_start_time_ns = system_time64_ns();
					ec_send_processdata();

					/* SOEM transmit data */
					curr_time = system_time64_ns();

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

	PRINTF("Start the soem_servo_motor_rt1180 baremetal example...\r\n");
	
	osal_timer_init(0);

#if ((defined(EXAMPLE_EP_NUM) && EXAMPLE_EP_NUM) || (defined(EXAMPLE_ENET_NUM) && EXAMPLE_ENET_NUM))
	if_port_init();
#endif
#if !(defined(EXAMPLE_NETC_HAS_NO_SWITCH) && EXAMPLE_NETC_HAS_NO_SWITCH)
    if_port_swt_init();
#endif

	control_task(SOEM_PORT_NAME);
	return 0;
}

