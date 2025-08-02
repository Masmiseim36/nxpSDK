/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _CONTROLLER_LOW_POWER_H__
#define _CONTROLLER_LOW_POWER_H__

/*******************************************************************************
 * Include
 ******************************************************************************/
#include <stdint.h>
#include <cmsis_compiler.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if defined(APP_LOWPOWER_ENABLED) && (APP_LOWPOWER_ENABLED > 0)
#define WEAK_FUNC
#else
#define WEAK_FUNC __WEAK
#endif /* APP_LOWPOWER_ENABLED */

/* C2H/H2C wakeup value. */
#ifndef C2H_WAKEUP_VALUE
#define C2H_WAKEUP_VALUE    (0U)
#endif /* C2H_WAKEUP_VALUE */
#ifndef H2C_WAKEUP_VALUE
#define H2C_WAKEUP_VALUE    (0U)
#endif /* H2C_WAKEUP_VALUE */

/* Host wakeup interrupt trigger mode. */
#ifndef HOST_WAKEUP_MODE
#define HOST_WAKEUP_MODE kHAL_GpioInterruptFallingEdge
#endif /* HOST_WAKEUP_MODE */

#define WAIT_CMD_COMPLETE 100    /* 0.1s */

#define BT_OP(ogf, ocf)                         ((ocf) | ((ogf) << 10))
#define BT_OGF_VS                               0x3f

/* HCI command struct. */
typedef struct
{
    uint16_t opcode;
    uint8_t *cmd;
    uint8_t cmd_len;
} lp_config_cmd_t;

typedef enum
{
    CONTROLLER_WAKE = 0U,
    CONTROLLER_SLEEP = 1U,
} lp_controller_state_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*
 * brief Initialize wake up pin and send vendor specific HCI command to configure controller.
 *
 * return Zero on success or negative error code otherwise.
 */
WEAK_FUNC uint8_t controller_low_power_init(void);

/*
 * brief Check the power state of controller.
 *
 * return Zero on ready or 1 on not ready.
 */
WEAK_FUNC uint8_t controller_power_state(void);

/*
 * brief Host allows controller to enter lowpower mode.
 *
 * return Zero on success.
 */
WEAK_FUNC uint8_t controller_enter_low_power(void);

/*
 * brief Host wake up controller.
 *
 * return Zero on success.
 */
WEAK_FUNC uint8_t controller_exit_low_power(void);
#endif /* _CONTROLLER_LOW_POWER_H__ */
