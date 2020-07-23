/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "calc_power.h"
#include "fraclib.h"
#include "meterliblp.h"
#include "meterlibcfg.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

static tMETERLIBLP3PH_DATA mlib = METERLIB_CFG;
static tENERGY_CNT wh_cnt, varh_cnt;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Motor input three-phase power calculation function
 *
 * @param sCalcPower - structure with variables for power measurement
 *
 * @return None
 */
void MID_CalcPwr(mid_calc_power_a1_t *sCalcPower)
{
    /* data processing */
    METERLIBLP3PH_ProcSamples(
        &mlib, FRAC24(sCalcPower->fltUphA * (1.0F / (float)U_MAX)),
        FRAC24(*sCalcPower->pfltIphA * (1.0F / (float)I_MAX)), FRAC24(sCalcPower->fltUphB * (1.0F / (float)U_MAX)),
        FRAC24(*sCalcPower->pfltIphB * (1.0F / (float)I_MAX)), FRAC24(sCalcPower->fltUphC * (1.0F / (float)U_MAX)),
        FRAC24(*sCalcPower->pfltIphC * (1.0F / (float)I_MAX)), NULL);

    /* calculate watt hours */
    METERLIBLP3PH_CalcWattHours(&mlib, &wh_cnt, METERLIBLP_KWH_PR(IMP_PER_KWH));

    /* calculate var hours */
    METERLIBLP3PH_CalcVarHours(&mlib, &varh_cnt, METERLIBLP_KVARH_PR(IMP_PER_KVARH));

    /* calculate auxiliary */
    METERLIBLP3PH_CalcAuxiliary(&mlib);
}

/*!
 * @brief Read motor three-phase input power
 *
 * @param sCalcPower - structure with variables for power measurement
 *
 * @return None
 */
void MID_ReadPwr(mid_calc_power_a1_t *sCalcPower)
{
    struct
    {
        double dblA;
        double dblB;
        double dblC;
    } sUrms;

    struct
    {
        double dblA;
        double dblB;
        double dblC;
    } sIrms;

    struct
    {
        double dblA;
        double dblB;
        double dblC;
    } sP;

    struct
    {
        double dblA;
        double dblB;
        double dblC;
    } sQ;

    struct
    {
        double dblA;
        double dblB;
        double dblC;
    } sS;

    /* read Urms, Irms, P, Q, S on phase A */
    METERLIBLP3PH_ReadResultsPh1(&mlib, &sUrms.dblA, &sIrms.dblA, &sP.dblA, &sQ.dblA, &sS.dblA);

    /* read Urms, Irms, P, Q, S on phase B */
    METERLIBLP3PH_ReadResultsPh1(&mlib, &sUrms.dblB, &sIrms.dblB, &sP.dblB, &sQ.dblB, &sS.dblB);

    /* read Urms, Irms, P, Q, S on phase C */
    METERLIBLP3PH_ReadResultsPh1(&mlib, &sUrms.dblC, &sIrms.dblC, &sP.dblC, &sQ.dblC, &sS.dblC);

    /* calculate average values */
    sCalcPower->fltUrmsAvg = (float)((sUrms.dblA + sUrms.dblB + sUrms.dblC) * MID_ONETHIRD);
    sCalcPower->fltIrmsAvg = (float)((sIrms.dblA + sIrms.dblB + sIrms.dblC) * MID_ONETHIRD);
    sCalcPower->fltPAvg    = (float)((sP.dblA + sP.dblB + sP.dblC) * MID_ONETHIRD);
    sCalcPower->fltQAvg    = (float)((sQ.dblA + sQ.dblB + sQ.dblC) * MID_ONETHIRD);
    sCalcPower->fltSAvg    = (float)((sS.dblA + sS.dblB + sS.dblC) * MID_ONETHIRD);
}
