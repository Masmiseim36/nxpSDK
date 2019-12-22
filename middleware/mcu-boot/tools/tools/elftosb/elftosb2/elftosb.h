/*
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _ELFTOSB_
#define _ELFTOSB_

//! Supported chip families.
enum chip_family_t
{
	undefinedFamily,
	k37xxFamily,      //!< 37xx series.
	kMX28Family,      //!< Catskills series.
	kKinetisFamily,   //!< Kinetis devices.
	kKinetisK3Family, //!< Kinetis K3S and up family of devices.
	kiMXFamily,       //!< i.MX family of devices including RTs and Ultra
	kLPC_skbootFamily,//!< LPCNext0, Niobe4 devices
	kLPC54X0XXFamily  //!< LPC540XX, LPC54S0XX devices
};

#endif //_ELFTOSB_