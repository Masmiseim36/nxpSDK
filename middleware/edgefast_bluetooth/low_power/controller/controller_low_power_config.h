/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _CONTROLLER_LOW_POWER_CONFIG_H__
#define _CONTROLLER_LOW_POWER_CONFIG_H__
 
/* External Wakeup schemems - 6 wire 
   --------------------------       --------------------------
   |          Host          |       |       Controller       |
   --------------------------       --------------------------
   |      HCI UART TXD      |  -->  |      HCI UART RXD      |
   |      HCI UART RXD      |  <--  |      HCI UART TXD      |
   |      HCI UART RTS      |  -->  |      HCI UART CTS      |
   |      HCI UART CTS      |  <--  |      HCI UART RTS      |
   | H2C(Controller Wakeup) |  -->  | H2C(Controller Wakeup) |
   |    C2H(Host Wakeup)    |  <--  |    C2H(Host Wakeup)    |
   --------------------------       -------------------------- */

/* Controller configuration list */

/* Murata 1XK with M2 interface*/
#if defined(WIFI_IW416_BOARD_MURATA_1XK_M2)
#define BT_C2H_GPIO           (4U)
#define BT_H2C_GPIO           (12U)

/* 2EL Firecrest module with M2 interface */
#elif defined(WIFI_IW612_BOARD_MURATA_2EL_M2)
#define BT_C2H_GPIO           (19U)
#define BT_H2C_GPIO           (18U)
#endif

#endif /* _CONTROLLER_LOW_POWER_CONFIG_H__ */
