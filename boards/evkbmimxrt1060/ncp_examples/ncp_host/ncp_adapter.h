/*
 * Copyright 2022-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

#ifndef __NCP_ADAPTER_H__
#define __NCP_ADAPTER_H__

#include "ncp_debug.h"
#include "ncp_intf_pm.h"

#if defined(__GNUC__) /* for armgcc compiler */
/** Structure packing begins */
#define NCP_TLV_PACK_START
/** Structure packeing end */
#define NCP_TLV_PACK_END __attribute__((packed))
#elif defined(__ICCARM__) /* for iar compiler */
/** Structure packing begins */
#define NCP_TLV_PACK_START __packed
/** Structure packing end */
#define NCP_TLV_PACK_END
#elif defined(PRAGMA_PACK)
/** Structure packing begins */
#define NCP_TLV_PACK_START
/** Structure packeing end */
#define NCP_TLV_PACK_END
#endif /* __GNUC__ */

#define ncp_adap_e(...) ncplog_e("NCP Adap", ##__VA_ARGS__)
#define ncp_adap_w(...) ncplog_w("NCP Adap", ##__VA_ARGS__)
#if CONFIG_NCP_DEBUG_ADAP
#define ncp_adap_d(...) ncplog("NCP Adap", ##__VA_ARGS__)
#else
#define ncp_adap_d(...)
#endif

/* NCP status */
typedef enum _ncp_status
{
    NCP_STATUS_ERROR      = -1,
    NCP_STATUS_CHKSUMERR  = -2,
    NCP_STATUS_NOMEM      = -3,
    NCP_STATUS_QUEUE_FULL = -4,
    NCP_STATUS_SUCCESS    = 0,
} ncp_status_t;

typedef struct _stats_intf
{
    /* set to 1 when start send something, set to 0 when finish send become to idle status  */
    uint8_t tx;
    /* set to 1 when start receive something, set to 0 when finish receive become to idle status */
    uint8_t rx;
    /* set to 1 interface init(or re-init) finish, set to 0, when interface de-init */
    uint8_t ready;
} stats_inft_t;

extern stats_inft_t ncp_intf;

#define NCP_INTF_STATUS_SET(x) (ncp_intf.x = 1)
#define NCP_INTF_STATUS_CLEAR(x) (ncp_intf.x = 0)
#define NCP_INTF_STATUS_CHECK(x) ((ncp_intf.x == 1) ? true : false)

ncp_status_t ncp_adapter_init(void);
ncp_status_t ncp_adapter_deinit(void);
void ncp_tlv_install_handler(uint8_t class, void *func_cb);
void ncp_tlv_uninstall_handler(uint8_t class);

#endif /* __NCP_ADAPTER_H__ */