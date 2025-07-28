/* -------------------------------------------------------------------------- */
/*                           Copyright 2021-2022 NXP                          */
/*                            All rights reserved.                            */
/*                    SPDX-License-Identifier: BSD-3-Clause                   */
/* -------------------------------------------------------------------------- */

#ifndef _SBTSNOOP_H_
#define _SBTSNOOP_H_

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */
#include <stdint.h>

/* -------------------------------------------------------------------------- */
/*                                   Macros                                   */
/* -------------------------------------------------------------------------- */
#ifndef SBTSNOOP_LOG_SIZE
#define SBTSNOOP_LOG_SIZE 8192U
#endif

#ifndef SBTSNOOP_BYTES_PER_LINE_SHIFT
/* using power of two to allow efficient shift
 * 4 means 16 characters, 5 means 32, ... */
#define SBTSNOOP_BYTES_PER_LINE_SHIFT 4u
#define SBTSNOOP_BYTES_PER_LINE       (1u << SBTSNOOP_BYTES_PER_LINE_SHIFT)
#endif

/* -------------------------------------------------------------------------- */
/*                              Public prototypes                             */
/* -------------------------------------------------------------------------- */

/*!
 * \brief Opens a new file (will discard any already opened file)
 *
 */
void sbtsnoop_fopen(void);

/*!
 * \brief Closes current file and print its content over serial
 *
 */
void sbtsnoop_fclose(void);

/*!
 * \brief Writes BTSNOOP formatted packet directly to file
 *        Example: In Ethermind case, btsnoop_write_pl can call this API
 *
 * \param[in] pkt Pointer to packet
 * \param[in] size Packet size
 */
void sbtsnoop_fwrite(const void *pkt, uint16_t size);

/*!
 * \brief Prints current content of the file, further write operation will
 *        override the content
 */
void sbtsnoop_fread(void);

/*!
 * \brief Converts HCI packet to BTSNOOP packet and writes to BTSNOOP file
 *
 * \param[in] pkt_type Packet type marker
 * \param[in] is_rxed 0 for TX, other for RX packets
 * \param[in] pkt Pointer to packet
 * \param[in] pkt_len Packet size
 */
void sbtsnoop_write_hci_pkt(uint8_t pkt_type, uint8_t is_rxed, uint8_t *pkt, uint16_t pkt_len);

/*!
 * \brief Opens the SBTSNOOP menu over serial (requires RX line for choice
 *        selection)
 */
void main_serial_btsnoop_operations(void);

#endif /* _SBTSNOOP_H_ */
