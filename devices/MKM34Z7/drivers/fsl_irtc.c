/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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

#include "fsl_irtc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define IRTC_BASE_YEAR (2112U)
#define YEAR_RANGE_START (1984U) /* Valid values for year range from -128 to 127; 2112 - 128 */
#define YEAR_RANGE_END (2239U)   /* Valid values for year range from -128 to 127; 2112 + 127 */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Checks whether the date and time passed in is valid
 *
 * @param datetime Pointer to structure where the date and time details are stored
 *
 * @return Returns false if the date & time details are out of range; true if in range
 */
static bool IRTC_CheckDatetimeFormat(const irtc_datetime_t *datetime);

/*******************************************************************************
 * Code
 ******************************************************************************/
static bool IRTC_CheckDatetimeFormat(const irtc_datetime_t *datetime)
{
    assert(datetime);

    /* Table of days in a month for a non leap year */
    uint8_t daysPerMonth[] = {0U, 31U, 28U, 31U, 30U, 31U, 30U, 31U, 31U, 30U, 31U, 30U, 31U};

    /* Check year, month, hour, minute, seconds */
    if ((datetime->year < YEAR_RANGE_START) || (datetime->year > YEAR_RANGE_END) || (datetime->month > 12U) ||
        (datetime->month < 1U) || (datetime->weekDay >= 7U) || (datetime->hour >= 24U) || (datetime->minute >= 60U) ||
        (datetime->second >= 60U))
    {
        /* If not correct then error*/
        return false;
    }

    /* Adjust the days in February for a leap year */
    if ((((datetime->year & 3U) == 0) && (datetime->year % 100U != 0)) || (datetime->year % 400U == 0))
    {
        daysPerMonth[2] = 29U;
    }

    /* Check the validity of the day */
    if ((datetime->day > daysPerMonth[datetime->month]) || (datetime->day < 1U))
    {
        return false;
    }

    return true;
}

status_t IRTC_Init(RTC_Type *base, const irtc_config_t *config)
{
    assert(config);

    uint16_t reg;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    CLOCK_EnableClock(kCLOCK_Rtc0);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

    /* Unlock to allow register write operation */
    if (IRTC_SetWriteProtection(RTC, false) == kStatus_Fail)
    {
        return kStatus_Fail;
    }

    /* Issue a software reset */
    IRTC_Reset(RTC);

    /* Setup the wakeup pin select */
    if (config->wakeupSelect)
    {
        base->CTRL2 |= RTC_CTRL2_WAKEUP_MODE_MASK;
    }
    else
    {
        base->CTRL2 &= ~RTC_CTRL2_WAKEUP_MODE_MASK;
    }

    /* Setup alarm match operation and sampling clock operation in standby mode */
    reg = base->CTRL;
    reg &= ~(RTC_CTRL_TIMER_STB_MASK_MASK | RTC_CTRL_ALM_MATCH_MASK);
    reg |= (RTC_CTRL_TIMER_STB_MASK(config->timerStdMask) | RTC_CTRL_ALM_MATCH(config->alrmMatch));
    base->CTRL = reg;

    return kStatus_Success;
}

void IRTC_GetDefaultConfig(irtc_config_t *config)
{
    assert(config);

    /* Tamper pin 0 is used as a wakeup/hibernation pin */
    config->wakeupSelect = true;

    /* Sampling clock are not gated when in standby mode */
    config->timerStdMask = false;

    /* Only seconds, minutes and hours are matched when generating an alarm */
    config->alrmMatch = kRTC_MatchSecMinHr;
}

status_t IRTC_SetDatetime(RTC_Type *base, const irtc_datetime_t *datetime)
{
    assert(datetime);

    /* Return error if the time provided is not valid */
    if (!(IRTC_CheckDatetimeFormat(datetime)))
    {
        return kStatus_InvalidArgument;
    }

    /* The register stores the offset in years from the base year of 2112 */
    if (datetime->year < IRTC_BASE_YEAR)
    {
        /* Values for years less than the base year range from -128 to 1 */
        base->YEARMON =
            RTC_YEARMON_YROFST(0x100U + datetime->year - IRTC_BASE_YEAR) | RTC_YEARMON_MON_CNT(datetime->month);
    }
    else
    {
        /* Values for years greater or equal to the base year range from 0 to 127 */
        base->YEARMON = RTC_YEARMON_YROFST(datetime->year - IRTC_BASE_YEAR) | RTC_YEARMON_MON_CNT(datetime->month);
    }
    /* Update the Day Count and Day of the week field */
    base->DAYS = RTC_DAYS_DOW(datetime->weekDay) | RTC_DAYS_DAY_CNT(datetime->day);

    /* Update hour and minute field */
    base->HOURMIN = RTC_HOURMIN_HOUR_CNT(datetime->hour) | RTC_HOURMIN_MIN_CNT(datetime->minute);

    /* Update the seconds register */
    base->SECONDS = RTC_SECONDS_SEC_CNT(datetime->second);

    return kStatus_Success;
}

void IRTC_GetDatetime(RTC_Type *base, irtc_datetime_t *datetime)
{
    assert(datetime);

    uint16_t temp = base->YEARMON;

    datetime->year = IRTC_BASE_YEAR + (int8_t)(temp >> RTC_YEARMON_YROFST_SHIFT);
    datetime->month = temp & RTC_YEARMON_MON_CNT_MASK;

    temp = base->DAYS;
    datetime->weekDay = (temp & RTC_DAYS_DOW_MASK) >> RTC_DAYS_DOW_SHIFT;
    datetime->day = temp & RTC_DAYS_DAY_CNT_MASK;

    temp = base->HOURMIN;
    datetime->hour = (temp & RTC_HOURMIN_HOUR_CNT_MASK) >> RTC_HOURMIN_HOUR_CNT_SHIFT;
    datetime->minute = temp & RTC_HOURMIN_MIN_CNT_MASK;

    datetime->second = base->SECONDS & RTC_SECONDS_SEC_CNT_MASK;
}

status_t IRTC_SetAlarm(RTC_Type *base, const irtc_datetime_t *alarmTime)
{
    assert(alarmTime);

    /* Return error if the alarm time provided is not valid */
    if (!(IRTC_CheckDatetimeFormat(alarmTime)))
    {
        return kStatus_InvalidArgument;
    }

    /* Set the alarm year */
    if (alarmTime->year < IRTC_BASE_YEAR)
    {
        base->ALM_YEARMON = RTC_ALM_YEARMON_ALM_YEAR(0x100U + alarmTime->year - IRTC_BASE_YEAR) |
                            RTC_ALM_YEARMON_ALM_MON(alarmTime->month);
    }
    else
    {
        base->ALM_YEARMON =
            RTC_ALM_YEARMON_ALM_YEAR(alarmTime->year - IRTC_BASE_YEAR) | RTC_ALM_YEARMON_ALM_MON(alarmTime->month);
    }

    /* Set the alarm day */
    base->ALM_DAYS = RTC_ALM_DAYS_ALM_DAY(alarmTime->day);

    /* Set the alarm hour and minute */
    base->ALM_HOURMIN = RTC_ALM_HOURMIN_ALM_HOUR(alarmTime->hour) | RTC_ALM_HOURMIN_ALM_MIN(alarmTime->minute);

    /* Set the alarm seconds */
    base->ALM_SECONDS = RTC_ALM_SECONDS_ALM_SEC(alarmTime->second);

    return kStatus_Success;
}

void IRTC_GetAlarm(RTC_Type *base, irtc_datetime_t *datetime)
{
    assert(datetime);

    uint16_t temp = base->ALM_YEARMON;

    datetime->year = IRTC_BASE_YEAR + (int8_t)(temp >> RTC_ALM_YEARMON_ALM_YEAR_SHIFT);
    datetime->month = temp & RTC_ALM_YEARMON_ALM_MON_MASK;

    datetime->day = base->ALM_DAYS & RTC_ALM_DAYS_ALM_DAY_MASK;

    temp = base->ALM_HOURMIN;
    datetime->hour = (temp & RTC_ALM_HOURMIN_ALM_HOUR_MASK) >> RTC_ALM_HOURMIN_ALM_HOUR_SHIFT;
    datetime->minute = temp & RTC_ALM_HOURMIN_ALM_MIN_MASK;

    datetime->second = base->ALM_SECONDS & RTC_ALM_SECONDS_ALM_SEC_MASK;
}

status_t IRTC_SetWriteProtection(RTC_Type *base, bool lock)
{
    /* Retry before giving up */
    uint8_t repeatProtectSequence = 100;

    if (!lock)
    {
        /* Unlock IRTC registers */
        while (((base->STATUS & RTC_STATUS_WRITE_PROT_EN_MASK) >> RTC_STATUS_WRITE_PROT_EN_SHIFT) &&
               repeatProtectSequence)
        {
            /* Access in 8-bit mode while storing the value */
            *(__IO uint8_t *)(&base->STATUS) = 0U;
            *(__IO uint8_t *)(&base->STATUS) = 0x40U;
            *(__IO uint8_t *)(&base->STATUS) = 0xC0U;
            *(__IO uint8_t *)(&base->STATUS) = 0x80U;
            repeatProtectSequence--;
        }
    }
    else
    {
        /* Lock IRTC registers */
        while ((!((base->STATUS & RTC_STATUS_WRITE_PROT_EN_MASK) >> RTC_STATUS_WRITE_PROT_EN_SHIFT)) &&
               repeatProtectSequence)
        {
            *(__IO uint8_t *)(&base->STATUS) = 0x80U;
            repeatProtectSequence--;
        }
    }

    /* Lock/unlock was not successful even after trying 10 times */
    if (repeatProtectSequence == 0)
    {
        return kStatus_Fail;
    }

    return kStatus_Success;
}

void IRTC_SetDaylightTime(RTC_Type *base, const irtc_daylight_time_t *datetime)
{
    assert(datetime);

    /* Disable daylight saving time */
    base->CTRL &= ~RTC_CTRL_DST_EN_MASK;

    /* Set the daylight saving time start month and end month value */
    base->DST_MONTH =
        RTC_DST_MONTH_DST_START_MONTH(datetime->startMonth) | RTC_DST_MONTH_DST_END_MONTH(datetime->endMonth);

    /* Set the daylight saving time start day and end day value */
    base->DST_DAY = RTC_DST_DAY_DST_START_DAY(datetime->startDay) | RTC_DST_DAY_DST_END_DAY(datetime->endDay);

    /* Set the daylight saving time start hour and end hour value */
    base->DST_HOUR = RTC_DST_HOUR_DST_START_HOUR(datetime->startHour) | RTC_DST_HOUR_DST_END_HOUR(datetime->endHour);

    /* Enable daylight saving time */
    base->CTRL |= RTC_CTRL_DST_EN_MASK;
}

void IRTC_GetDaylightTime(RTC_Type *base, irtc_daylight_time_t *datetime)
{
    assert(datetime);

    uint16_t temp = base->DST_MONTH;

    /* Get the daylight savings time start and end month value */
    datetime->startMonth = (temp & RTC_DST_MONTH_DST_START_MONTH_MASK) >> RTC_DST_MONTH_DST_START_MONTH_SHIFT;
    datetime->endMonth = (temp & RTC_DST_MONTH_DST_END_MONTH_MASK) >> RTC_DST_MONTH_DST_END_MONTH_SHIFT;

    /* Get the daylight savings time start and end day value */
    temp = base->DST_DAY;
    datetime->startDay = (temp & RTC_DST_DAY_DST_START_DAY_MASK) >> RTC_DST_DAY_DST_START_DAY_SHIFT;
    datetime->endDay = (temp & RTC_DST_DAY_DST_END_DAY_MASK) >> RTC_DST_DAY_DST_END_DAY_SHIFT;

    /* Get the daylight savings time start and end hour value */
    temp = base->DST_HOUR;
    datetime->startHour = (temp & RTC_DST_HOUR_DST_START_HOUR_MASK) >> RTC_DST_HOUR_DST_START_HOUR_SHIFT;
    datetime->endHour = (temp & RTC_DST_HOUR_DST_END_HOUR_MASK) >> RTC_DST_HOUR_DST_END_HOUR_SHIFT;
}

void IRTC_SetCoarseCompensation(RTC_Type *base, uint8_t compensationValue, uint8_t compensationInterval)
{
    uint16_t reg;

    /* Set the compensation value and interval */
    base->COMPEN = (compensationValue & 0xFFU) | ((compensationInterval & 0xFFU) << 8U);

    /* Disable fine and enable coarse compensation */
    reg = base->CTRL;
    reg &= ~RTC_CTRL_FINEEN_MASK;
    reg |= RTC_CTRL_COMP_EN_MASK;
    base->CTRL = reg;
}

void IRTC_SetFineCompensation(RTC_Type *base, uint8_t integralValue, uint8_t fractionValue, bool accumulateFractional)
{
    if (!accumulateFractional)
    {
        /* Disable compensation to clear previous accumulated fractional part */
        base->CTRL &= ~((1U << RTC_CTRL_COMP_EN_SHIFT) | (1U << RTC_CTRL_FINEEN_SHIFT));
    }

    /* Set the compensation fractional and integral parts */
    base->COMPEN = (fractionValue & 0x7FU) | ((integralValue & 0xFU) << 12U);
    /* Enable fine compensation */
    base->CTRL |= ((1U << RTC_CTRL_COMP_EN_SHIFT) | (1U << RTC_CTRL_FINEEN_SHIFT));
}

void IRTC_SetTamperParams(RTC_Type *base, irtc_tamper_pins_t tamperNumber, const irtc_tamper_config_t *tamperConfig)
{
    assert(tamperConfig);

    uint16_t reg = 0;

#if defined(FSL_FEATURE_RTC_HAS_TAMPER_DIRECTION) && (FSL_FEATURE_RTC_HAS_TAMPER_DIRECTION)
    reg = base->TAMPER_DIRECTION;
    /* Set whether tamper pin is active or passive */
    if (tamperConfig->activePassive)
    {
        /* In case of active tamper, set the direction */
        reg |= (1U << tamperNumber);
        if (tamperConfig->direction)
        {
            /* Tamper direction is output */
            reg |= (1U << (RTC_TAMPER_DIRECTION_I_O_TAMP_SHIFT + tamperNumber));
        }
        else
        {
            /* Tamper direction is input */
            reg &= ~(1U << (RTC_TAMPER_DIRECTION_I_O_TAMP_SHIFT + tamperNumber));
        }
    }
    else
    {
        /* Passive tampers are input only and the direction bit is read only in this case */
        reg &= ~(1U << tamperNumber);
    }
    base->TAMPER_DIRECTION = reg;
#endif /* FSL_FEATURE_RTC_HAS_TAMPER_DIRECTION */

    /* Set the filter properties for the external tamper pins */
    switch (tamperNumber)
    {
        case kIRTC_Tamper_0:
            /* Set the pin for Tamper 0 */
            base->CTRL2 &= ~RTC_CTRL2_WAKEUP_MODE_MASK;
            reg = base->FILTER01_CFG;
            reg &= ~(RTC_FILTER01_CFG_POL0_MASK | RTC_FILTER01_CFG_FIL_DUR0_MASK | RTC_FILTER01_CFG_CLK_SEL0_MASK);
            reg |= (RTC_FILTER01_CFG_POL0(tamperConfig->pinPolarity) |
                    RTC_FILTER01_CFG_FIL_DUR0(tamperConfig->filterDuration) |
                    RTC_FILTER01_CFG_CLK_SEL0(tamperConfig->filterClk));
            base->FILTER01_CFG = reg;
            break;
        case kIRTC_Tamper_1:
            reg = base->FILTER01_CFG;
            reg &= ~(RTC_FILTER01_CFG_POL1_MASK | RTC_FILTER01_CFG_FIL_DUR1_MASK | RTC_FILTER01_CFG_CLK_SEL1_MASK);
            reg |= (RTC_FILTER01_CFG_POL1(tamperConfig->pinPolarity) |
                    RTC_FILTER01_CFG_FIL_DUR1(tamperConfig->filterDuration) |
                    RTC_FILTER01_CFG_CLK_SEL1(tamperConfig->filterClk));
            base->FILTER01_CFG = reg;
            break;
        case kIRTC_Tamper_2:
            reg = base->FILTER2_CFG;
            reg &= ~(RTC_FILTER2_CFG_POL2_MASK | RTC_FILTER2_CFG_FIL_DUR2_MASK | RTC_FILTER2_CFG_CLK_SEL2_MASK);
            reg |= (RTC_FILTER2_CFG_POL2(tamperConfig->pinPolarity) |
                    RTC_FILTER2_CFG_FIL_DUR2(tamperConfig->filterDuration) |
                    RTC_FILTER2_CFG_CLK_SEL2(tamperConfig->filterClk));
            base->FILTER2_CFG = reg;
            break;
        default:
            break;
    }
}

#if defined(FSL_FEATURE_RTC_HAS_TAMPER_QUEUE) && (FSL_FEATURE_RTC_HAS_TAMPER_QUEUE)

uint8_t IRTC_ReadTamperQueue(RTC_Type *base, irtc_datetime_t *tamperTimestamp)
{
    assert(tamperTimestamp);

    /* Read the register 2 times to get a entry*/
    uint16_t temp1 = base->TAMPER_QUEUE;
    uint16_t temp2 = base->TAMPER_QUEUE;
    uint8_t tamperNum;

    /*
     * Tamper queue does not store the year field as this value can be read from RTC_YEARMON.
     * It is expected that the queue will be read at least once in a year.
     */
    tamperTimestamp->year = 0;
    /* From the first read; TAMPER_DATA[4:0] is the hour field */
    tamperTimestamp->hour = temp1 & 0x1FU;
    /* From the first read; TAMPER_DATA[9:5] is the day field */
    tamperTimestamp->day = (temp1 >> 5U) & 0x1FU;
    /* From the first read; TAMPER_DATA[13:10] is the month field */
    tamperTimestamp->month = (temp1 >> 10U) & 0xFU;

    /* From the second read; TAMPER_DATA[5:0] is the seconds field */
    tamperTimestamp->second = temp2 & 0x3FU;
    /* From the second read; TAMPER_DATA[11:6] is the minutes field */
    tamperTimestamp->minute = (temp2 >> 6U) & 0x3FU;
    /* From the second read; TAMPER_DATA[14:12] is the tamper index */
    tamperNum = (temp2 >> 12U) & 0x7U;

    return tamperNum;
}

#endif /* FSL_FEATURE_RTC_HAS_TAMPER_QUEUE */
