/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*!
 * @file sf_model.h
 *
 * SIGFOX model selection. Supported models are OL2361 and OL2385.
 * This header is used to determine which features of this driver will be used
 * or not.
 *
 */

#ifndef SOURCE_SF_MODEL_H_
#define SOURCE_SF_MODEL_H_

/*******************************************************************************
 * Defines
 ******************************************************************************/
/*!
 * @addtogroup models_group
 * @{
 */

/*! @brief SIGFOX models supported by this driver. */

#define SF_MODEL_OL2361             1U  /*!< Model OL2361. */
#define SF_MODEL_OL2385             2U  /*!< Model OL2385. */

/*! @brief Selection of SIGFOX device model. */
#define SF_MODEL                    SF_MODEL_OL2385

#if ((SF_MODEL != SF_MODEL_OL2361) && (SF_MODEL != SF_MODEL_OL2385))
#warning SF_MODEL does not comply with supported models, OL2385 selected by default.
#undef SF_MODEL
#define SF_MODEL SF_MODEL_OL2385
#endif
/* END of model check. */

/*! @} */
#endif /* SOURCE_SF_MODEL_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/

