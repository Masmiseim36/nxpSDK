/**
 * @file sci2c_cfg.h
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright(C) NXP Semiconductors, 2016
 * All rights reserved.
 *
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * A7-series security ICs.  This software is supplied "AS IS" without any
 * warranties of any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * Permission to use, copy and modify this software is hereby granted,
 * under NXP Semiconductors' and its licensor's relevant copyrights in
 * the software, without fee, provided that it is used in conjunction with
 * NXP Semiconductors products. This copyright, permission, and disclaimer notice
 * must appear in all copies of this code.
 * @par Description
 * configuration file for communication between Host and Secure Module.
 */


#ifndef _SCI2C_CFG_H
#define _SCI2C_CFG_H

// requested slave to master length
// #define REQ_S2M_LENGTH               (eMax254BytesS2M)

// max retries
#define N_RETRY_PD                  (3)
#define N_RETRY_PE                  (3)
#define N_RETRY_SRST                (3)

// timeouts & delays
#define T_PDE                       (0) // 150 usec
#define T_RSTG                      (5)
#define T_WNCMD_ACTUAL              (100) // 100 ms
// tCMDG : 180 usec - minimum. Safeguard to 200us
#define T_CMDG_USec                 (200)

#define APP_SCI2C_TIMEOUT_ms        (100000) // 100 sec
#ifdef SPI
#define SCI2C_tMD_ms                (50)
#else
#define SCI2C_tMD_ms                (2)
#endif

#define SMCOM_I2C_ADDRESS           (0x90)
#define SMCOM_EDC_MODE              (eEdc_NoErrorDetection)
#ifdef PLATFORM_IMX
    #define SMCOM_MAX_BYTES             (eMax31BytesS2M)
#else
    #define SMCOM_MAX_BYTES             (eMax254BytesS2M)
#endif
//

#endif //_SCI2C_CFG_H
