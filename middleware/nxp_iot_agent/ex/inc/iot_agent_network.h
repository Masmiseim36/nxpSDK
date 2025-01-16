/* 
 * Copyright 2019-2021 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 * 
 */
#ifndef _IOT_AGENT_NETWORK_H_
#define _IOT_AGENT_NETWORK_H_

#include <nxp_iot_agent_status.h>

#ifdef __cplusplus
extern "C" {
#endif

 /*! @ingroup edgelock2go_agent_network
 *
 * @page page_macros Helper macros to keep the code more compact with regard to error handling
 *
 * @brief 
 *
 *
 * @addtogroup edgelock2go_agent_network
 * @{
 */

iot_agent_status_t network_init(void);

#ifdef __cplusplus
} // extern "C"
#endif
  /*!
  *@}
  */ /* end of edgelock2go_agent_network */

#endif // #ifndef _IOT_AGENT_NETWORK_H_
