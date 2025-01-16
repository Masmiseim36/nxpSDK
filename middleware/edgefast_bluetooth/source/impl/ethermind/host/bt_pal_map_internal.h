/** @file
 * @brief MAP Profile Internal header.
 */

/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __MAP_INTERNAL_H__
#define __MAP_INTERNAL_H__

#define BT_MAP_OPCODE_SIZE 1U
#define BT_MAP_PACKET_LENGTH_SIZE 2U
#define BT_MAP_FLAGS_SIZE 1U
#define BT_MAP_CONSTANT_SIZE 1U
#define BT_MAP_HI_SIZE 1U
#define BT_MAP_HV_LENGTH_SIZE 2U
#define BT_MAP_HEADER_SIZE (BT_MAP_HI_SIZE + BT_MAP_HV_LENGTH_SIZE)
#define BT_MAP_TAG_ID_SIZE 1U
#define BT_MAP_TAG_ID_LENGTH_SIZE 1U
#define BT_MAP_TAG_SIZE (BT_MAP_TAG_ID_SIZE + BT_MAP_TAG_ID_LENGTH_SIZE)
#define BT_MAP_SRM_HV_SIZE 1U
#define BT_MAP_SRMP_HV_SIZE 1U
#define BT_MAP_CONNECTION_ID_HV_SIZE 4U
#define BT_MAP_CONNECTION_ID_SIZE (BT_MAP_HI_SIZE + BT_MAP_CONNECTION_ID_HV_SIZE)
#define BT_MAP_SRM_SIZE (BT_MAP_HI_SIZE + BT_MAP_SRMP_HV_SIZE)
#define BT_MAP_SRMP_SIZE (BT_MAP_HI_SIZE + BT_MAP_SRM_HV_SIZE)

/**
 * @name MAP Filler Byte
 *
 * Used for PUT message
 */
#define MAP_FILLER_BYTE "0"

#define MAP_MAS_TARGET_UUID {0xBB, 0x58, 0x2B, 0x40, 0x42, 0x0C, 0x11, 0xDB, 0xB0, 0xDE, 0x08, 0x00, 0x20, 0x0CU, 0x9A, 0x66}
#define MAP_MNS_TARGET_UUID {0xBB, 0x58, 0x2B, 0x41, 0x42, 0x0C, 0x11, 0xDB, 0xB0, 0xDE, 0x08, 0x00, 0x20, 0x0CU, 0x9A, 0x66}

#endif /* __MAP_INTERNAL_H__ */
