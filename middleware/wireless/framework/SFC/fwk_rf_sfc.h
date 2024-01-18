/* -------------------------------------------------------------------------- */
/*                           Copyright 2020-2023 NXP                          */
/*                            All rights reserved.                            */
/*                    SPDX-License-Identifier: BSD-3-Clause                   */
/* -------------------------------------------------------------------------- */

#ifndef _FWK_SFC_H_
#define _FWK_SFC_H_

/*!
 * \struct sfc_config_t
 * \brief SFC configuration structure passed in SFC_Init or SFC_UpdateConfig
 * \var sfc_config_t::filterSize
 * Defines number of samples used in the filter
 * Note: it will be converted to power of two to allow for bit-shift operations
 * \var sfc_config_t::ppmTarget
 * Defines the ppm target
 */
typedef struct
{
    uint32_t filterSize;
    uint32_t ppmTarget;
} sfc_config_t;

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

/*!
 * \brief Configures SFC and runs a calibration
 *
 */
void SFC_Init(void);

/*!
 * \brief Enables/Disables SFC calibrations
 *
 * \param[in] enable
 */
void SFC_Enable(bool enable);

/*!
 * \brief Processes available measurement results
 *        Measurement results are not processed in the SFA IRQ to not spend too much time in the IRQ handler
 *        This function can be called from low priority tasks such as Idle task
 *
 */
void SFC_Process(void);

/*!
 * \brief Indicates if SFC module is busy, can be used by low power policy manager to cancel low power entry
 *
 * \return true Calibration is ongoing
 * \return false SFC is idling
 */
bool SFC_IsBusy(void);

/*!
 * \brief Updates the SFC config with the config passed by the user
 *        Allows to change the ppm target and the filter period
 *        Note: if the filter period is changed, the filter is reset
 *
 * \param[in] config
 */
void SFC_UpdateConfig(const sfc_config_t *config);

/*!
 * \brief Enanbles/Disables SFC notification by messages send to the host core.
 * \details It will send the frequency mesured, the mean ppm, and the instant values of ppm
 *
 * \param[in] enable
 */
void SFC_EnableNotification(int enable);

#endif /* _FWK_SFC_H_ */
