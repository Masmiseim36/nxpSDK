/*
 * Support code for serial recovery feature of MCUboot
 * Adapted for MCUXpresso SDK
 * 
 * Copyright (C) 2026 NXP Semiconductors
 * Copyright (C) 2018, Nordic Semiconductor ASA
 * Copyright (C) 2017 Intel Corporation
 * Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 * 
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Modified by NXP 2026 - Combined Intel and Nordic/ARM sources
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "serial_recovery_support.h"

#include "clock_config.h"
#include "fsl_clock.h"

#if defined(RW612_SERIES) || defined(MIMXRT595S_cm33_SERIES) ||                \
    defined(MIMXRT685S_cm33_SERIES) || defined(LPC55S69_cm33_core0_SERIES)
#include "fsl_usart.h"
#else
#include "fsl_lpuart.h"
#endif
#include "mcuboot_config.h"
#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define BASE64_SIZE_T_MAX	((size_t) -1) /* SIZE_T_MAX is not standard */

#if defined(RW612_SERIES) || defined(MIMXRT595S_cm33_SERIES) ||                \
    defined(MIMXRT685S_cm33_SERIES) || defined(LPC55S69_cm33_core0_SERIES)
#define SERIAL_TYPE               USART_Type
#define SERIAL_GET_RXFIFO_CNT     USART_GetRxFifoCount
#define SERIAL_READ_BYTE          USART_ReadByte
#define SERIAL_WRITE              USART_WriteBlocking
#else
#define SERIAL_TYPE               LPUART_Type
#define SERIAL_GET_RXFIFO_CNT     LPUART_GetRxFifoCount
#define SERIAL_READ_BYTE          LPUART_ReadByte
#define SERIAL_WRITE              LPUART_WriteBlocking
#endif

/*******************************************************************************
 * Types
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Static
 ******************************************************************************/
static SERIAL_TYPE *serial_recovery_usart = (SERIAL_TYPE *)BOARD_DEBUG_UART_BASEADDR;

static const uint8_t base64_enc_map[64] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
	'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
	'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd',
	'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
	'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', '+', '/'
};

static const uint8_t base64_dec_map[128] = {
	127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
	127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
	127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
	127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
	127, 127, 127,	62, 127, 127, 127,  63,  52,  53,
	 54,  55,  56,	57,  58,  59,  60,  61, 127, 127,
	127,  64, 127, 127, 127,   0,	1,   2,   3,   4,
	  5,   6,   7,	 8,   9,  10,  11,  12,  13,  14,
	 15,  16,  17,	18,  19,  20,  21,  22,  23,  24,
	 25, 127, 127, 127, 127, 127, 127,  26,  27,  28,
	 29,  30,  31,	32,  33,  34,  35,  36,  37,  38,
	 39,  40,  41,	42,  43,  44,  45,  46,  47,  48,
	 49,  50,  51, 127, 127, 127, 127, 127
};

/*******************************************************************************
 * Externs
 ******************************************************************************/
void nxp_delay_usecs(uint32_t usecs)
{
    SDK_DelayAtLeastUs(usecs, CLOCK_GetFreq(kCLOCK_CoreSysClk));;
}

void nxp_system_reset(void)
{
    NVIC_SystemReset();
}

/*
 * Calculate CRC16
 */
uint16_t crc16_itu_t(uint16_t seed, const uint8_t *src, size_t len)
{
	for (; len > 0; len--) {
		seed = (seed >> 8U) | (seed << 8U);
		seed ^= *src;
		++src;
		seed ^= (seed & 0xffU) >> 4U;
		seed ^= seed << 12U;
		seed ^= (seed & 0xffU) << 5U;
	}

	return seed;
}

/*
 * Encode a buffer into base64 format
 */
int base64_encode(uint8_t *dst, size_t dlen, size_t *olen, const uint8_t *src,
		  size_t slen)
{
	size_t i, n;
	int C1, C2, C3;
	uint8_t *p;

	if (slen == 0) {
		*olen = 0;
		return 0;
	}

	n = slen / 3 + (slen % 3 != 0);

	if (n > (BASE64_SIZE_T_MAX - 1) / 4) {
		*olen = BASE64_SIZE_T_MAX;
		return -ENOMEM;
	}

	n *= 4;

	if ((dlen < n + 1) || (!dst)) {
		*olen = n + 1;
		return -ENOMEM;
	}

	n = (slen / 3) * 3;

	for (i = 0, p = dst; i < n; i += 3) {
		C1 = *src++;
		C2 = *src++;
		C3 = *src++;

		*p++ = base64_enc_map[(C1 >> 2) & 0x3F];
		*p++ = base64_enc_map[(((C1 &  3) << 4) + (C2 >> 4)) & 0x3F];
		*p++ = base64_enc_map[(((C2 & 15) << 2) + (C3 >> 6)) & 0x3F];
		*p++ = base64_enc_map[C3 & 0x3F];
	}

	if (i < slen) {
		C1 = *src++;
		C2 = ((i + 1) < slen) ? *src++ : 0;

		*p++ = base64_enc_map[(C1 >> 2) & 0x3F];
		*p++ = base64_enc_map[(((C1 & 3) << 4) + (C2 >> 4)) & 0x3F];

		if ((i + 1) < slen) {
			*p++ = base64_enc_map[((C2 & 15) << 2) & 0x3F];
		} else {
			*p++ = '=';
		}

		*p++ = '=';
	}

	*olen = p - dst;
	*p = 0U;

	return 0;
}

/*
 * Decode a base64-formatted buffer
 */
int base64_decode(uint8_t *dst, size_t dlen, size_t *olen, const uint8_t *src,
		  size_t slen)
{
	size_t i, n;
	uint32_t j, x;
	uint8_t *p;

	/* First pass: check for validity and get output length */
	for (i = n = j = 0U; i < slen; i++) {
		/* Skip spaces before checking for EOL */
		x = 0U;
		while (i < slen && src[i] == ' ') {
			++i;
			++x;
		}

		/* Spaces at end of buffer are OK */
		if (i == slen) {
			break;
		}

		if ((slen - i) >= 2 && src[i] == '\r' && src[i + 1] == '\n') {
			continue;
		}

		if (src[i] == '\n') {
			continue;
		}

		/* Space inside a line is an error */
		if (x != 0U) {
			return -EINVAL;
		}

		if (src[i] == '=' && ++j > 2) {
			return -EINVAL;
		}

		if (src[i] > 127 || base64_dec_map[src[i]] == 127U) {
			return -EINVAL;
		}

		if (base64_dec_map[src[i]] < 64 && j != 0U) {
			return -EINVAL;
		}

		n++;
	}

	if (n == 0) {
		*olen = 0;
		return 0;
	}

	/* The following expression is to calculate the following formula
	 * without risk of integer overflow in n:
	 *	   n = ( ( n * 6 ) + 7 ) >> 3;
	 */
	n = (6 * (n >> 3)) + ((6 * (n & 0x7) + 7) >> 3);
	n -= j;

	if (dst == NULL || dlen < n) {
		*olen = n;
		return -ENOMEM;
	}

	for (j = 3U, n = x = 0U, p = dst; i > 0; i--, src++) {

		if (*src == '\r' || *src == '\n' || *src == ' ') {
			continue;
		}

		j -= (base64_dec_map[*src] == 64U);
		x  = (x << 6) | (base64_dec_map[*src] & 0x3F);

		if (++n == 4) {
			n = 0;
			if (j > 0) {
				*p++ = (unsigned char)(x >> 16);
			}
			if (j > 1) {
				*p++ = (unsigned char)(x >> 8);
			}
			if (j > 2) {
				*p++ = (unsigned char)(x);
			}
		}
	}

	*olen = p - dst;

	return 0;
}

int serial_recovery_read(char *str, int cnt, int *newline)
{
    volatile uint32_t read_len = 0;
    volatile uint32_t len = 0;
    volatile bool stop = false;
    do {
        len = SERIAL_GET_RXFIFO_CNT(serial_recovery_usart);
        if (len) {
            for (uint32_t i = 0; i < len; i++) {
                /* Read the character from the RX FIFO */
                str[read_len] = SERIAL_READ_BYTE(serial_recovery_usart);
                read_len++;
                if (read_len == cnt || str[read_len - 1] == '\n') {
                    stop = true;
                    break;
                }
            }
        }
        MCUBOOT_WATCHDOG_FEED();
    } while (!stop);
    *newline = (str[read_len - 1] == '\n') ? 1 : 0;
    str[read_len] = '\0';
    return read_len + 1;
}

void serial_recovery_write(const char *ptr, int cnt)
{
    SERIAL_WRITE(serial_recovery_usart, (uint8_t *)ptr, cnt);
}
