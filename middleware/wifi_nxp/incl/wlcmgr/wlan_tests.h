/*  Copyright 2008-2020 NXP
 *
 *  Licensed under the LA_OPT_NXP_Software_License.txt (the "Agreement")
 *
 */

/*! \file wlan_tests.h
 *  \brief WLAN Connection Manager Tests
 */

#ifndef __WLAN_TESTS_H__
#define __WLAN_TESTS_H__

/** Print the TX PWR Limit table received from Wi-Fi firmware
 *
 * \param[in] txpwrlimit A \ref wlan_txpwrlimit_t struct holding the
 * 		the TX PWR Limit table received from Wi-Fi firmware.
 *
 */
void print_txpwrlimit(wlan_txpwrlimit_t txpwrlimit);
#endif /* WLAN_TESTS_H */
