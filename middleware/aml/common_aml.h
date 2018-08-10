/*
 * Copyright (c) 2016 - 2018, NXP Semiconductors, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 * @file common_aml.h
 *
 * Constants and types definition common for AML drivers.
 */

#ifndef SOURCE_COMMON_AML_H_
#define SOURCE_COMMON_AML_H_

 /*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>
#include "assert.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @addtogroup macro_group
 * @{
 */
/*! @brief SDK versions supported by AML layer. */
#define SDK_S32     0U              /*! S32 SDK 0.8.6 EAR. */
#define SDK_KSDK    1U              /*! MCUXpresso SDK 2.3. */

/*!
 * @brief Selection of SDK version you are using (S32 SDK or KSDK).
 *
 * Use macros defined above (SDK_S32 or SDK_KSDK).
 */
#define SDK_VERSION SDK_KSDK

/*!
 * @brief Unused variable. This macro is used when some variable is not used in SDK.
 */
#define AML_UNUSED(x) (void)(x)

#if (SDK_VERSION == SDK_KSDK)

/* Definition of error codes. */
#include "fsl_common.h"

#endif

/*! @brief Assertion settings. */

#if (SDK_VERSION == SDK_KSDK)

/* #define NDEBUG */                /*!< Uncomment this macro to disable assertions in KSDK. */

/*! @brief Assert function for KSDK. */
#define AML_ASSERT(condition) \
    (assert(condition))

#elif (SDK_VERSION == SDK_S32)

//#define DEV_ERROR_DETECT            /*!< Comment this macro to disable assertions in S32 SDK. */

/*! @brief Assert function for KSDK. */
#define AML_ASSERT(condition) \
    (DEV_ASSERT(condition))

#endif

#if (SDK_VERSION == SDK_S32)

/*! @brief Construct a status code value from a group and code number. This
 * code is taken from KSDK. */
#define MAKE_STATUS(group, code) ((((group) * 100U) + (code)))

#endif
/*! @} */

/*!
 * @addtogroup enum_group
 * @{
 */
#if (SDK_VERSION == SDK_S32)
  
#include "status.h"

/*! @brief Status group numbers. This code is taken from KSDK. */
enum _status_groups
{
    kStatusGroup_Generic = 0,                 /*!< Group number for generic status codes. */
    kStatusGroup_LPSPI = 4,                   /*!< Group number for LPSPI status codes. */
    kStatusGroup_FLEXIO_SPI = 5,              /*!< Group number for FLEXIO SPI status codes. */
    kStatusGroup_DSPI = 6,                    /*!< Group number for DSPI status codes. */
    kStatusGroup_FLEXIO_UART = 7,             /*!< Group number for FLEXIO UART status codes. */
    kStatusGroup_FLEXIO_I2C = 8,              /*!< Group number for FLEXIO I2C status codes. */
    kStatusGroup_LPI2C = 9,                   /*!< Group number for LPI2C status codes. */
    kStatusGroup_UART = 10,                   /*!< Group number for UART status codes. */
    kStatusGroup_I2C = 11,                    /*!< Group number for UART status codes. */
    kStatusGroup_LPSCI = 12,                  /*!< Group number for LPSCI status codes. */
    kStatusGroup_LPUART = 13,                 /*!< Group number for LPUART status codes. */
    kStatusGroup_SPI = 14,                    /*!< Group number for SPI status code.*/
    kStatusGroup_SDSPI = 22,                  /*!< Group number for SDSPI status codes. */
    kStatusGroup_FLEXIO_I2S = 23,             /*!< Group number for FLEXIO I2S status codes */
    kStatusGroup_PHY = 41,                    /*!< Group number for PHY status codes. */
    kStatusGroup_TRGMUX = 42,                 /*!< Group number for TRGMUX status codes. */
    kStatusGroup_QSPI = 45,                   /*!< Group number for QSPI status codes. */
    kStatusGroup_DMA = 50,                    /*!< Group number for DMA status codes. */
    kStatusGroup_EDMA = 51,                   /*!< Group number for EDMA status codes. */
    kStatusGroup_DMAMGR = 52,                 /*!< Group number for DMAMGR status codes. */
    kStatusGroup_FLEXCAN = 53,                /*!< Group number for FlexCAN status codes. */
    kStatusGroup_ApplicationRangeStart = 100, /*!< Starting number for application groups. */
};

/*! @brief Generic status return codes. This code is taken from KSDK. */
enum _generic_status
{
    kStatus_Success = MAKE_STATUS(kStatusGroup_Generic, 0),
    kStatus_Fail = MAKE_STATUS(kStatusGroup_Generic, 1),
    kStatus_ReadOnly = MAKE_STATUS(kStatusGroup_Generic, 2),
    kStatus_OutOfRange = MAKE_STATUS(kStatusGroup_Generic, 3),
    kStatus_InvalidArgument = MAKE_STATUS(kStatusGroup_Generic, 4),
    kStatus_Timeout = MAKE_STATUS(kStatusGroup_Generic, 5),
    kStatus_NoTransferInProgress = MAKE_STATUS(kStatusGroup_Generic, 6),
    kStatus_Busy = MAKE_STATUS(kStatusGroup_Generic, 7),
};

/*! @brief Type used for all status and error return values. This code is
 * taken from KSDK. This status is returned from AML but S32 SDK driver returns
 * status_t from status.h file. AML needs to be compatible with older existing
 * drivers. Be careful if using status from AML or S32 SDK driver directly. This
 * was done because S32 SDK modifies status to same name as AML layer.
 *  */

#define status_t aml_status_t
typedef int32_t status_t;

#endif /* END of S32 SDK specific enumerations. */

/* Additional error values. */
enum aml_groups{
    kStatusGroup_AML_SPI = 101,
    kStatusGroup_AML_TMR = 102,
    kStatusGroup_AML_I2C = 103,
    kStatusGroup_AML_GPIO = 104,
    kStatusGroup_Reserved = 110,
    kStatusGroup_ValveDriver = 111, /* Valve Controller */
    kStatusGroup_SHB = 112,         /* SPI H-Bridge */
    kStatusGroup_SF = 113,          /* Sigfox */
    kStatusGroup_LZ = 114,          /* Lizard */
    kStatusGroup_TPP = 115,         /* Three Phase Predriver */
    kStatusGroup_MVHB = 116,        /* Medium Voltage H-Bridge */
    kStatusGroup_LVHB = 117,        /* Low Voltage H-Bridge */
    kStatusGroup_BCC = 118,         /* Battery Cell Controller */
    kStatusGroup_ASL = 119,         /* Boost/Buck Converters */
    kStatusGroup_FS65 = 120,        /* FS65/45 SBC. */
};
/*! @} */

/*! @brief Type for peripheral instance number. */
typedef uint32_t aml_instance_t;



#endif /* SOURCE_COMMON_AML_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
