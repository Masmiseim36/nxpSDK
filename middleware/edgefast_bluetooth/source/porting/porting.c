/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "porting.h"

#include "fsl_component_log.h"
#include "fsl_debug_console.h"

#if ((defined(CONFIG_BT_SMP)) && (CONFIG_BT_SMP))
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
static mbedtls_entropy_context entropy;
static mbedtls_ctr_drbg_context rng_ctx;
#endif /* CONFIG_BT_SMP */

#if defined(__CC_ARM) || (defined(__ARMCC_VERSION))
#ifndef porting_errno
int errno = 0;
#endif
#endif

/* Weak function. */
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
#define __WEAK_FUNC __attribute__((weak))
#elif defined(__ICCARM__)
#define __WEAK_FUNC __weak
#elif defined(__GNUC__)
#define __WEAK_FUNC __attribute__((weak))
#endif

#if (defined(CONFIG_BT_DEBUG) && (CONFIG_BT_DEBUG > 0))

#ifndef CONFIG_PORTING_LOG_PRIO
#define CONFIG_PORTING_LOG_PRIO OSA_TASK_PRIORITY_MIN
#endif

#ifndef CONFIG_PORTING_LOG_STACK_SIZE
#define CONFIG_PORTING_LOG_STACK_SIZE (2048)
#endif

static void porting_log_task(void *param);

static OSA_TASK_HANDLE_DEFINE(porting_log_task_data);
OSA_TASK_DEFINE( porting_log_task, CONFIG_PORTING_LOG_PRIO, 1, CONFIG_PORTING_LOG_STACK_SIZE, 0);

#endif

#if defined(__CC_ARM) || (defined(__ARMCC_VERSION))
size_t strnlen(const char *s, size_t maxlen)
{
    return MIN(strlen(s), maxlen);
}
#endif

__WEAK_FUNC int16_t RNG_GetPseudoRandomNo (uint8_t* pOut,
                               uint8_t  outBytes,
                               uint8_t* pSeed)
{
#if ((defined(CONFIG_BT_SMP)) && (CONFIG_BT_SMP))
    uint32_t rng;

    if (NULL == pOut)
    {
        return (int16_t)-1;
    }

    if (NULL != pSeed)
    {
        mbedtls_entropy_init(&entropy);

        mbedtls_ctr_drbg_init(&rng_ctx);

        if(0 != mbedtls_ctr_drbg_seed(&rng_ctx, mbedtls_entropy_func, &entropy, NULL, 0))
        {
            return -1;
        }
    }

    for (size_t index = 0; index < outBytes; index+=sizeof(rng))
    {
        if(0 != mbedtls_ctr_drbg_random(&rng_ctx, (unsigned char *)&rng, 4))
        {
            return -1;
        }

        for (size_t i = 0; i < MIN(outBytes, (outBytes - index));i++)
        {
            ((uint8_t *)pOut)[index + i] = ((uint8_t *)&rng)[i];
        }
    }

    mbedtls_ctr_drbg_free(&rng_ctx);

    mbedtls_entropy_free(&entropy);

    return 0;
#else
    return -1;
#endif /* CONFIG_BT_SMP */

}

void k_fifo_init(k_fifo_t *fifo)
{
    osa_status_t ret = KOSA_StatusSuccess;

    if ((NULL != fifo) && (NULL == fifo->queue))
    {
        ret = OSA_MsgQCreate((osa_msgq_handle_t)fifo->queue_handle, CONFIG_BT_MSG_QUEUE_COUNT, sizeof(void*));
        if (KOSA_StatusSuccess == ret)
        {
            fifo->queue = (osa_msgq_handle_t)fifo->queue_handle;
        }
    }
    assert (KOSA_StatusSuccess == ret);

    (void)ret;
}

void k_fifo_deinit(k_fifo_t *fifo)
{
    if ((NULL != fifo) && (NULL != fifo->queue))
    {
        (void)OSA_MsgQDestroy((osa_msgq_handle_t)fifo->queue_handle);
        fifo->queue = NULL;
    }
}

int k_fifo_is_empty(k_fifo_t *fifo)
{
    if ((NULL != fifo) && (NULL != fifo->queue))
    {
        return OSA_MsgQAvailableMsgs((osa_msgq_handle_t)fifo->queue_handle) == 0u ? 1 : 0;
    }
    return 1;
}

void k_fifo_put_head(k_fifo_t *fifo, void *data)
{
    if ((NULL != fifo) && (NULL != fifo->queue))
    {
        OSA_MsgQPut((osa_msgq_handle_t)fifo->queue_handle, data);
    }
}

void k_sem_init(struct k_sem *sem, unsigned int initial_count, unsigned int limit)
{
	osa_status_t status;

    assert((NULL != sem));

	if (NULL == sem->sem)
	{
		status = OSA_SemaphoreCreate((osa_semaphore_handle_t)sem->sem_handle, initial_count);
		assert(KOSA_StatusSuccess == status);
		if (KOSA_StatusSuccess == status)
		{
			sem->sem = sem->sem_handle;
		}
	}

    (void)status;
}

void k_sem_deinit(struct k_sem *sem)
{
    assert(NULL != sem);

    if (NULL != sem->sem)
    {
        (void)OSA_SemaphoreDestroy(sem->sem);
        sem->sem = NULL;
    }
}

uint8_t u8_to_dec(char *buf, uint8_t buflen, uint8_t value)
{
	uint8_t divisor = 100;
	uint8_t num_digits = 0;
	uint8_t digit;
    uint8_t temp;

	while (buflen > (uint8_t)0 && divisor > (uint8_t)0) {
		digit = value / divisor;
		if (digit != (uint8_t)0 || divisor == (uint8_t)1 || num_digits != (uint8_t)0) {
            temp =(digit + (uint8_t)'0');
			*buf = (char)temp;
			buf++;
			buflen--;
			num_digits++;
		}

		value -= digit * divisor;
		divisor /= (uint8_t)10;
	}

	if (((uint8_t) 0) != buflen) {
		*buf = '\0';
	}

	return num_digits;
}

int char2hex(char c, uint8_t *x)
{
    int temp;
	if (c >= '0' && c <= '9') {
        temp = (c - (char)'0');
		*x = (uint8_t)temp;
	} else if (c >= 'a' && c <= 'f') {
        temp = c - (char)'a';
		*x = (uint8_t)((uint8_t)temp + (uint8_t)10);
	} else if (c >= 'A' && c <= 'F') {
        temp = (c - (char)'A');
		*x = ((uint8_t)temp+ (uint8_t)10);
	}else {
		return -EINVAL;
	}

	return 0;
}

int hex2char(uint8_t x, char *c)
{
	if (x <= (uint8_t)9) {
		*c = (char)(x + (char)'0');
	} else  if (x <= (uint8_t)15) {
		*c = (char)(x - (uint8_t)10 + (char)'a');
	} else {
		return -EINVAL;
	}

	return 0;
}

size_t bin2hex(const uint8_t *buf, size_t buflen, char *hex, size_t hexlen)
{
	if ((hexlen + (size_t)1) < buflen * ((size_t)2)) {
		return 0;
	}

	for (size_t i = 0; i < buflen; i++) {
		if (hex2char((uint8_t)(buf[i] >> 4), &hex[((size_t)2) * i]) < 0) {
			return 0;
		}
		if (hex2char((uint8_t)(buf[i] & ((uint8_t)0xf)), &hex[((size_t)2) * i + ((size_t)1)]) < 0) {
			return 0;
		}
	}

	hex[(uint8_t)2 * buflen] = '\0';
	return (uint8_t)2 * buflen;
}

size_t hex2bin(const char *hex, size_t hexlen, uint8_t *buf, size_t buflen)
{
	uint8_t dec;

	if (buflen < hexlen / ((size_t)2) + hexlen % ((size_t)2)) {
		return 0;
	}

	/* if hexlen is uneven, insert leading zero nibble */
	if ((size_t)0 != (hexlen % ((size_t)2))) {
		if (char2hex(hex[0], &dec) < 0) {
			return 0;
		}
		buf[0] = dec;
		hex++;
		buf++;
	}

	/* regular hex conversion */
	for (size_t i = 0; i < hexlen / ((size_t)2); i++) {
		if (char2hex(hex[(size_t)2 * i], &dec) < 0) {
			return 0;
		}
		buf[i] = dec << 4;

		if (char2hex(hex[(size_t)2 * i + (size_t)1], &dec) < 0) {
			return 0;
		}
		buf[i] += dec;
	}

	return hexlen / ((size_t)2) + hexlen % ((size_t)2);
}

#if (defined(CONFIG_BT_DEBUG) && (CONFIG_BT_DEBUG > 0))
#if LOG_ENABLE_TIMESTAMP
static unsigned int bt_ble_get_timestamp(void)
{
	return (unsigned int)OSA_TimeGetMsec();
}
#endif
#endif

#if (defined(CONFIG_BT_DEBUG) && (CONFIG_BT_DEBUG > 0))
static void porting_log_task(void* param)
{
    static uint8_t dumpBuffer[512];
    size_t dumpLength;

	BT_DBG("started");

	while (1) {
        dumpLength = 0;
        LOG_Dump(dumpBuffer, sizeof(dumpBuffer) - 1, &dumpLength);
        if (dumpLength > 0)
        {
            dumpBuffer[dumpLength] = 0;
            PRINTF("%s", dumpBuffer);
        }
		/* Make sure we don't hog the CPU if the rx_queue never
		 * gets empty.
		 */
		OSA_TaskYield();
	}
}
#endif

void bt_ble_porting_init(void)
{
#if (defined(CONFIG_BT_DEBUG) && (CONFIG_BT_DEBUG > 0))
    osa_status_t ret;

    (void)LOG_Init();
#if LOG_ENABLE_TIMESTAMP
	(void)LOG_SetTimestamp(bt_ble_get_timestamp);
#endif

	ret = OSA_TaskCreate((osa_task_handle_t)porting_log_task_data, OSA_TASK(porting_log_task), NULL);
	assert(KOSA_StatusSuccess == ret);

    (void)ret;
#endif
}

const char *bt_addr_le_str(const bt_addr_le_t *addr)
{
	static char str[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(addr, str, sizeof(str));

	return str;
}

const char *bt_hex(const void *buf, size_t len)
{
	static const char hex[] = "0123456789abcdef";
	static char str[129];
	const uint8_t *b = (const uint8_t *)buf;
	size_t i;

	len = MIN(len, (sizeof(str) - 1) / 2);

	for (i = 0; i < len; i++) {
		str[i * 2]     = hex[b[i] >> 4];
		str[i * 2 + 1] = hex[b[i] & 0xf];
	}

	str[i * 2] = '\0';

	return str;
}

const char *bt_addr_str(const bt_addr_t *addr)
{
	static char str[BT_ADDR_STR_LEN];

	bt_addr_to_str(addr, str, sizeof(str));

	return str;
}

const char *bt_uuid_str(const struct bt_uuid *uuid)
{
	static char str[BT_UUID_STR_LEN];

	bt_uuid_to_str(uuid, str, sizeof(str));

	return str;
}

