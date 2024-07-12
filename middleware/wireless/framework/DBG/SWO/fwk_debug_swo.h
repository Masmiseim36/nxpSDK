/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __FWK_SWO_DEBUG_H__
#define __FWK_SWO_DEBUG_H__

/* -------------------------------------------------------------------------- */
/*                        Public functions declaration                        */
/* -------------------------------------------------------------------------- */

/*!
 * \brief  Configure SoC circuitry so that SWO feature can be enabled
 *
 * \param[in] funnel configure on which core the SWO is plugged in
 *                   0 : application core
 *                   1 : nbu core
 */
void DBG_InitSWO(int funnel);

#endif /*  __FWK_SWO_DEBUG_H__ */
