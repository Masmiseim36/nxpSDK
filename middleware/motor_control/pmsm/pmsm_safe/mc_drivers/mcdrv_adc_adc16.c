/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mcdrv_adc_adc16.h"
#include "mcdrv_periph_init.h"
#include "safety_error_handler.h"
#include "safety_flow_check.h"

MISRAC_DISABLE /* MISRA check disabled (IAR IDE only). */

#include "freemaster_tsa_pmsm.h"

MISRAC_ENABLE /* MISRA check enabled (IAR IDE only). */


/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* The SVM sector identifiers. */
#define MCDRV_SVM_SEC_16  (0U)
#define MCDRV_SVM_SEC_23  (1U)
#define MCDRV_SVM_SEC_45  (2U)

/* Calibration settle time after PWM enablement. */
#define MCDRV_CALIB_SETTLE_TIME    ((uint16_t)(ADC_I_PH_OFFSET_SETTLE * IRQ_FAST_LOOP_FREQ / 1000UL))
/* Number of samples taken for each SVM sector. */
#define MCDRV_CALIB_SAMPLES_SEC    ((uint16_t)(1U << ADC_I_PH_OFFSET_SMPLS))
/* Total number of calibration samples. */
#define MCDRV_CALIB_SAMPLES_VALID  ((uint16_t)(MCDRV_CALIB_SAMPLES_SEC * 3U))
/* Total calibration time in number of fast control loop cycles. */
#define MCDRV_CALIB_SAMPLES_TOTAL  ((uint16_t)(MCDRV_CALIB_SETTLE_TIME + MCDRV_CALIB_SAMPLES_VALID))

/* The phase current offset measurement structure. */
typedef struct _mcdrv_ph_offset
{
    uint32_t  ui32Calib;    /* Calibration accumulator. */
    frac16_t  f16Offset;    /* Measurement offset. */
} mcdrv_ph_offset_t;

/* The secondary measurement channel configuration structure. */
typedef struct _mcdrv_chn_config
{
    frac16_t *pf16RSLT;    /* Pointer to the result register. */
    uint32_t *pui32SC;     /* Pointer to the channel configuration register. */
    uint32_t  ui32Channel; /* The channel number. */
} mcdrv_chn_config_t;

/* The ADC16 driver variable structure. */
typedef struct _mcdrv_adc16_var
{
    struct /* Sector one and six offsets. */
    {
        mcdrv_ph_offset_t sPhB; /* Phase B current measurement offset. */
        mcdrv_ph_offset_t sPhC; /* Phase C current measurement offset. */
        uint16_t  ui16SmplCnt;  /* Number of calibration samples taken. */
    } sSec16;
    struct /* Sector two and three offsets. */
    {
        mcdrv_ph_offset_t sPhA; /* Phase A current measurement offset. */
        mcdrv_ph_offset_t sPhC; /* Phase C current measurement offset. */
        uint16_t  ui16SmplCnt;  /* Number of calibration samples taken. */
    } sSec23;
    struct /* Sector four and five offsets. */
    {
        mcdrv_ph_offset_t sPhA; /* Phase A current measurement offset. */
        mcdrv_ph_offset_t sPhB; /* Phase B current measurement offset. */
        uint16_t  ui16SmplCnt;  /* Number of calibration samples taken. */
    } sSec45;
    uint32_t ui32SVMSecId;         /* The current SVM sector index. */
    uint16_t ui16TotalCalibSmpls;  /* Total number of taken calibration samples. */
    mcdrv_meas_t sOut;             /* The output data structure. */
} mcdrv_adc16_var_t;

/* The channel configuration structure. */
typedef struct _mcdrv_adc16_conf
{
    struct /* Channel configuration for SVM sector 1&6. */
    {
        mcdrv_chn_config_t sPhB;
        mcdrv_chn_config_t sPhC;
    } sSec16;
    struct /* Channel configuration for SVM sector 2&3. */
    {
        mcdrv_chn_config_t sPhA;
        mcdrv_chn_config_t sPhC;
    } sSec23;
    struct /* Channel configuration for SVM sector 4&5. */
    {
        mcdrv_chn_config_t sPhA;
        mcdrv_chn_config_t sPhB;
    } sSec45;
    /* Channel configuration for DC-bus voltage measurement. */
    mcdrv_chn_config_t sUDCB;
    /* Channel configuration for auxiliary quantity measurement. */
    mcdrv_chn_config_t sAUX;
    /* SVM sector index conversion table. */
    uint8_t ui8SvmSecTable[7U];
} mcdrv_adc16_conf_t;


/*******************************************************************************
 * Variables
 ******************************************************************************/

#if defined(__IAR_SYSTEMS_ICC__) /* IAR */

    #pragma section = ".safety_ram"

    /* Place the channel variables structure to safety RAM. */
    static mcdrv_adc16_var_t s_sfsAdc16Vars @ ".safety_ram";

#elif defined(__GNUC__) && (__ARMCC_VERSION >= 6010050) /* KEIL */

    #include "linker_config.h"

    /* Place the channel variables structure to safety RAM. */
    static mcdrv_adc16_var_t s_sfsAdc16Vars __attribute__((section(".safety_ram")));
#elif defined(__GNUC__) /* MCUXpresso */

    /* Place the channel variables structure to safety RAM. */
    static mcdrv_adc16_var_t s_sfsAdc16Vars __attribute__((section(".safety_ram")));


#endif /* defined(__IAR_SYSTEMS_ICC__) */

/* Initialize the ADC16 driver variables structure. */
static mcdrv_adc16_var_t s_sfsAdc16Vars =
{
    {{0UL, 0}, {0UL, 0}, 0U},
    {{0UL, 0}, {0UL, 0}, 0U},
    {{0UL, 0}, {0UL, 0}, 0U},
    0UL, MCDRV_CALIB_SAMPLES_TOTAL,
    {{0,0,0},0,0}
};

/* Initialize the channel configuration structure. */
static const mcdrv_adc16_conf_t c_sfsAdc16Conf =
{
    /* Channel configuration for phase current measurement in SVM sector 1&6. */
    .sSec16 = {
        /* Phase current B is measured. */
        .sPhB = {
            .pf16RSLT = (frac16_t*)(uint32_t)&M1_ADC_SVM16_I_PHB_ADC->R[0U],
            .pui32SC  = (uint32_t*)(uint32_t)&M1_ADC_SVM16_I_PHB_ADC->SC1[0U],
            .ui32Channel = M1_ADC_SVM16_I_PHB_CHN
        },
        /* Phase current C is measured. */
        .sPhC = {
            .pf16RSLT = (frac16_t*)(uint32_t)&M1_ADC_SVM16_I_PHC_ADC->R[0U],
            .pui32SC  = (uint32_t*)(uint32_t)&M1_ADC_SVM16_I_PHC_ADC->SC1[0U],
            .ui32Channel = M1_ADC_SVM16_I_PHC_CHN
        }
    },
    /* Channel configuration for phase current measurement in SVM sector 2&3. */
    .sSec23 = {
        /* Phase current A is measured. */
        .sPhA = {
            .pf16RSLT = (frac16_t*)(uint32_t)&M1_ADC_SVM23_I_PHA_ADC->R[0U],
            .pui32SC  = (uint32_t*)(uint32_t)&M1_ADC_SVM23_I_PHA_ADC->SC1[0U],
            .ui32Channel = M1_ADC_SVM23_I_PHA_CHN
        },
        /* Phase current C is measured. */
        .sPhC = {
            .pf16RSLT = (frac16_t*)(uint32_t)&M1_ADC_SVM23_I_PHC_ADC->R[0U],
            .pui32SC  = (uint32_t*)(uint32_t)&M1_ADC_SVM23_I_PHC_ADC->SC1[0U],
            .ui32Channel = M1_ADC_SVM23_I_PHC_CHN
        }
    },
    /* Channel configuration for phase current measurement in SVM sector 4&5. */
    .sSec45 = {
        /* Phase current A is measured. */
        .sPhA = {
            .pf16RSLT = (frac16_t*)(uint32_t)&M1_ADC_SVM45_I_PHA_ADC->R[0U],
            .pui32SC  = (uint32_t*)(uint32_t)&M1_ADC_SVM45_I_PHA_ADC->SC1[0U],
            .ui32Channel = M1_ADC_SVM45_I_PHA_CHN
        },
        /* Phase current B is measured. */
        .sPhB = {
            .pf16RSLT = (frac16_t*)(uint32_t)&M1_ADC_SVM45_I_PHB_ADC->R[0U],
            .pui32SC  = (uint32_t*)(uint32_t)&M1_ADC_SVM45_I_PHB_ADC->SC1[0U],
            .ui32Channel = M1_ADC_SVM45_I_PHB_CHN
        }
    },
    /* Channel configuration for DC-bus voltage measurement. */
    .sUDCB = {
        .pf16RSLT = (frac16_t*)(uint32_t)&M1_ADC_U_DCB_ADC->R[1U],
        .pui32SC  = (uint32_t*)(uint32_t)&M1_ADC_U_DCB_ADC->SC1[1U],
        .ui32Channel = M1_ADC_U_DCB_CHN
    },
    /* Channel configuration for auxiliary quantity measurement. */
    .sAUX = {
        .pf16RSLT = (frac16_t*)(uint32_t)&M1_ADC_AUX_ADC->R[1U],
        .pui32SC  = (uint32_t*)(uint32_t)&M1_ADC_AUX_ADC->SC1[1U],
        .ui32Channel = M1_ADC_AUX_CHN
    },
    {MCDRV_SVM_SEC_16,
     MCDRV_SVM_SEC_16,
     MCDRV_SVM_SEC_23,
     MCDRV_SVM_SEC_23,
     MCDRV_SVM_SEC_45,
     MCDRV_SVM_SEC_45,
     MCDRV_SVM_SEC_16}
};


/* Initialize the output data structure pointer. */
const mcdrv_meas_t* const c_psfsADC16Meas = (const mcdrv_meas_t*)&s_sfsAdc16Vars.sOut;


/*!
 * @brief   The safety variables TSA table.
 *
 * @details See freemaster_tsa.h for more details about TSA table construction.
 *
 * @param   TSA table identificator.
 *
 * @return  FMSTR_TSA_ENTRY* - The TSA table entry data type.
 */
MISRAC_DISABLE /* MISRA check disabled (IAR IDE only). */
FMSTR_TSA_TABLE_BEGIN_FS(TSA_ADC)
    /* The ADC measurements. */
    FMSTR_TSA_RO_VAR_FS(A01, s_sfsAdc16Vars.sOut.sIABC.f16A,   FMSTR_TSA_FRAC16)
    FMSTR_TSA_RO_VAR_FS(A02, s_sfsAdc16Vars.sOut.f16UDcBus,    FMSTR_TSA_FRAC16)
    FMSTR_TSA_RO_VAR_FS(A03, s_sfsAdc16Vars.sOut.f16AUX,       FMSTR_TSA_FRAC16)
FMSTR_TSA_TABLE_END_FS(TSA_ADC)
MISRAC_ENABLE /* MISRA check enabled (IAR IDE only). */

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * Set the initial channel assignment for phase currents, DC-bus voltage and
 * the auxiliary quantity.
 */
void MCDRV_fsCurr3Ph2ShChanAssignInit(uint16_t ui16SVMSector)
{
    /* Assign phase-current measurement channels. */
    MCDRV_fsCurr3Ph2ShChanAssign(ui16SVMSector);

    /* Initialize DC-bus voltage U_DCB sampling. */
    *c_sfsAdc16Conf.sUDCB.pui32SC = c_sfsAdc16Conf.sUDCB.ui32Channel;

    /* Initialize auxiliary quantity AUX sampling. */
    *c_sfsAdc16Conf.sAUX.pui32SC = c_sfsAdc16Conf.sAUX.ui32Channel;

    /* ADC peripheral driver initialization node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkAR, FS_FLCK_AR_NODE_MCDRV_INIT_ADC);
}

/*!
 * Function reads and calculates three-phase samples based on the actual SVM
 * sector
 */
void MCDRV_fsADCMeasGet(void)
{
    /* Check which SVM sector we are at. */
    switch(s_sfsAdc16Vars.ui32SVMSecId)
    {
    case MCDRV_SVM_SEC_16:
        /* Direct sensing of phase B and phase C. Calculation the phase A. */
        s_sfsAdc16Vars.sOut.sIABC.f16B = MLIB_ShLSat_F16(
            (*c_sfsAdc16Conf.sSec16.sPhB.pf16RSLT - s_sfsAdc16Vars.sSec16.sPhB.f16Offset), 4U);
        s_sfsAdc16Vars.sOut.sIABC.f16C = MLIB_ShLSat_F16(
            (*c_sfsAdc16Conf.sSec16.sPhC.pf16RSLT - s_sfsAdc16Vars.sSec16.sPhC.f16Offset), 4U);
        s_sfsAdc16Vars.sOut.sIABC.f16A = MLIB_NegSat_F16(MLIB_AddSat_F16(s_sfsAdc16Vars.sOut.sIABC.f16C,
                                                                         s_sfsAdc16Vars.sOut.sIABC.f16B));
        break;

    case MCDRV_SVM_SEC_23:
        /* Direct sensing of phase A and phase C. Calculation the phase B. */
        s_sfsAdc16Vars.sOut.sIABC.f16A = MLIB_ShLSat_F16((frac16_t)
            (*c_sfsAdc16Conf.sSec23.sPhA.pf16RSLT - s_sfsAdc16Vars.sSec23.sPhA.f16Offset), 4U);
        s_sfsAdc16Vars.sOut.sIABC.f16C = MLIB_ShLSat_F16((frac16_t)
            (*c_sfsAdc16Conf.sSec23.sPhC.pf16RSLT - s_sfsAdc16Vars.sSec23.sPhC.f16Offset), 4U);
        s_sfsAdc16Vars.sOut.sIABC.f16B = MLIB_NegSat_F16(MLIB_AddSat_F16(s_sfsAdc16Vars.sOut.sIABC.f16C,
                                                                         s_sfsAdc16Vars.sOut.sIABC.f16A));
        break;

    default:
        /* Direct sensing of phase A and phase B. Calculation the phase C. */
        s_sfsAdc16Vars.sOut.sIABC.f16A = MLIB_ShLSat_F16((frac16_t)
            (*c_sfsAdc16Conf.sSec45.sPhA.pf16RSLT - s_sfsAdc16Vars.sSec45.sPhA.f16Offset), 4U);
        s_sfsAdc16Vars.sOut.sIABC.f16B = MLIB_ShLSat_F16((frac16_t)
            (*c_sfsAdc16Conf.sSec45.sPhB.pf16RSLT - s_sfsAdc16Vars.sSec45.sPhB.f16Offset), 4U);
        s_sfsAdc16Vars.sOut.sIABC.f16C = MLIB_NegSat_F16(MLIB_AddSat_F16(s_sfsAdc16Vars.sOut.sIABC.f16B,
                                                                         s_sfsAdc16Vars.sOut.sIABC.f16A));
        break;
    }

    /* Read the DC-bus voltage U_DCB sample value. */
    s_sfsAdc16Vars.sOut.f16UDcBus = MLIB_ShLSat_F16(*c_sfsAdc16Conf.sUDCB.pf16RSLT, 3U);

    /* Read the auxiliary quantity sample value. */
    s_sfsAdc16Vars.sOut.f16AUX = MLIB_ShLSat_F16(*c_sfsAdc16Conf.sAUX.pf16RSLT, 3U);

    /* The MC.SM ADC driver measurement acquisition node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_MCDRV_ADC_MEAS);
}

/*!
 * Function sets new channel assignment for the next sampling based on the SVM
 * sector parameter.
 */
void MCDRV_fsCurr3Ph2ShChanAssign(uint16_t ui16SVMSector)
{
    /* Decrement the sector number to fit in the table. */
    s_sfsAdc16Vars.ui32SVMSecId = c_sfsAdc16Conf.ui8SvmSecTable[ui16SVMSector];

    /* Check which SVM sector we are at to update the ADC_SC1n registers based
       on the actual SVM sector. */
    switch(s_sfsAdc16Vars.ui32SVMSecId)
    {
    case MCDRV_SVM_SEC_16:
        /* Phase currents I_PHB and I_PHC will be measured. */
        *c_sfsAdc16Conf.sSec16.sPhB.pui32SC = c_sfsAdc16Conf.sSec16.sPhB.ui32Channel;
        *c_sfsAdc16Conf.sSec16.sPhC.pui32SC = c_sfsAdc16Conf.sSec16.sPhC.ui32Channel;
        break;
    case MCDRV_SVM_SEC_23:
        /* Phase currents I_PHA and I_PHC will be measured. */
        *c_sfsAdc16Conf.sSec23.sPhA.pui32SC = c_sfsAdc16Conf.sSec23.sPhA.ui32Channel;
        *c_sfsAdc16Conf.sSec23.sPhC.pui32SC = c_sfsAdc16Conf.sSec23.sPhC.ui32Channel;
        break;
    default:
        /* Phase currents I_PHA and I_PHB will be measured. */
        *c_sfsAdc16Conf.sSec45.sPhA.pui32SC = c_sfsAdc16Conf.sSec45.sPhA.ui32Channel;
        *c_sfsAdc16Conf.sSec45.sPhB.pui32SC = c_sfsAdc16Conf.sSec45.sPhB.ui32Channel;
        break;
    }

    /* The MC.SM ADC driver channel assignment node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_MCDRV_ADC_ASSIGN);
}

/*!
 * Function initializes phase current channel offset measurement.
 */
void MCDRV_fsCurr3Ph2ShCalibInit(void)
{
    /* Initialize the calibration accumulators. */
    s_sfsAdc16Vars.sSec16.sPhB.ui32Calib = 0UL;
    s_sfsAdc16Vars.sSec16.sPhC.ui32Calib = 0UL;
    s_sfsAdc16Vars.sSec23.sPhA.ui32Calib = 0UL;
    s_sfsAdc16Vars.sSec23.sPhC.ui32Calib = 0UL;
    s_sfsAdc16Vars.sSec45.sPhA.ui32Calib = 0UL;
    s_sfsAdc16Vars.sSec45.sPhB.ui32Calib = 0UL;

    /* Clear sector calibration sample counters. */
    s_sfsAdc16Vars.sSec16.ui16SmplCnt = 0U;
    s_sfsAdc16Vars.sSec23.ui16SmplCnt = 0U;
    s_sfsAdc16Vars.sSec45.ui16SmplCnt = 0U;

    /* Initialize the calibration sample counter. */
    s_sfsAdc16Vars.ui16TotalCalibSmpls = MCDRV_CALIB_SAMPLES_TOTAL;
}

/*!
 * Function reads current samples and filter them based on the actual SVM
 * sector.
 */
uint16_t MCDRV_fsCurr3Ph2ShCalib(void)
{
    /* Check whether we already reached the end of calibration. */
    if(0U < s_sfsAdc16Vars.ui16TotalCalibSmpls)
    {
        /* Check whether the measurement settle time passed. */
        if(MCDRV_CALIB_SAMPLES_VALID >= s_sfsAdc16Vars.ui16TotalCalibSmpls)
        {
            /* Check which SVM sector we are at to update the current measurement offset. */
            switch(s_sfsAdc16Vars.ui32SVMSecId)
            {
            case MCDRV_SVM_SEC_16:
                /* Sensing of offset of I_PHB and I_PHC measurement. */
                s_sfsAdc16Vars.sSec16.sPhB.ui32Calib += (uint32_t)(*c_sfsAdc16Conf.sSec16.sPhB.pf16RSLT);
                s_sfsAdc16Vars.sSec16.sPhC.ui32Calib += (uint32_t)*c_sfsAdc16Conf.sSec16.sPhC.pf16RSLT;
                s_sfsAdc16Vars.sSec16.ui16SmplCnt++;
                break;
            case MCDRV_SVM_SEC_23:
                /* Sensing of offset of I_PHA and I_PHC measurement. */
                s_sfsAdc16Vars.sSec23.sPhA.ui32Calib += (uint32_t)*c_sfsAdc16Conf.sSec23.sPhA.pf16RSLT;
                s_sfsAdc16Vars.sSec23.sPhC.ui32Calib += (uint32_t)*c_sfsAdc16Conf.sSec23.sPhC.pf16RSLT;
                s_sfsAdc16Vars.sSec23.ui16SmplCnt++;
                break;
            default:
                /* Sensing of offset of I_PHA and I_PHB measurement. */
                s_sfsAdc16Vars.sSec45.sPhA.ui32Calib += (uint32_t)*c_sfsAdc16Conf.sSec45.sPhA.pf16RSLT;
                s_sfsAdc16Vars.sSec45.sPhB.ui32Calib += (uint32_t)*c_sfsAdc16Conf.sSec45.sPhB.pf16RSLT;
                s_sfsAdc16Vars.sSec45.ui16SmplCnt++;
                break;
            }
        }

        /* Decrement the sample counter. */
        s_sfsAdc16Vars.ui16TotalCalibSmpls--;
    }
    else
    {
        /* The calibration was completed or not initialized. */
        FS_fsErrorHandling(FS_ERROR_MCDRV_ADC_FAIL, SAFETY_ERROR_ADC_OFFST_CAL_MEAS_FAIL);
    }


    /* Return number of remaining samples. */
    return s_sfsAdc16Vars.ui16TotalCalibSmpls;
}

/*!
 * Function passes measured offset values to the main ADC structure.
 */
void MCDRV_fsCurr3Ph2ShCalibSet(void)
{
    uint16_t ui16SmplCntCheck;

    /* Check the number of calibration samples taken. */
    ui16SmplCntCheck = s_sfsAdc16Vars.ui16TotalCalibSmpls;
    ui16SmplCntCheck |= (uint16_t)(s_sfsAdc16Vars.sSec16.ui16SmplCnt ^ MCDRV_CALIB_SAMPLES_SEC);
    ui16SmplCntCheck |= (uint16_t)(s_sfsAdc16Vars.sSec23.ui16SmplCnt ^ MCDRV_CALIB_SAMPLES_SEC);
    ui16SmplCntCheck |= (uint16_t)(s_sfsAdc16Vars.sSec45.ui16SmplCnt ^ MCDRV_CALIB_SAMPLES_SEC);

    /* Check whether all the calibration samples were taken. */
    if(0U < ui16SmplCntCheck)
    {
        /* The calibration was not completed. */
        FS_fsErrorHandling(FS_ERROR_MCDRV_ADC_FAIL, SAFETY_ERROR_ADC_OFFST_CAL_PROC_FAIL);
    }

    /* Assert the new offsets. */
    s_sfsAdc16Vars.sSec16.sPhB.f16Offset = (frac16_t)(uint16_t)
        (s_sfsAdc16Vars.sSec16.sPhB.ui32Calib >> (ADC_I_PH_OFFSET_SMPLS));
    s_sfsAdc16Vars.sSec16.sPhC.f16Offset = (frac16_t)(uint16_t)
        (s_sfsAdc16Vars.sSec16.sPhC.ui32Calib >> (ADC_I_PH_OFFSET_SMPLS));
    s_sfsAdc16Vars.sSec23.sPhA.f16Offset = (frac16_t)(uint16_t)
        (s_sfsAdc16Vars.sSec23.sPhA.ui32Calib >> (ADC_I_PH_OFFSET_SMPLS));
    s_sfsAdc16Vars.sSec23.sPhC.f16Offset = (frac16_t)(uint16_t)
        (s_sfsAdc16Vars.sSec23.sPhC.ui32Calib >> (ADC_I_PH_OFFSET_SMPLS));
    s_sfsAdc16Vars.sSec45.sPhA.f16Offset = (frac16_t)(uint16_t)
        (s_sfsAdc16Vars.sSec45.sPhA.ui32Calib >> (ADC_I_PH_OFFSET_SMPLS));
    s_sfsAdc16Vars.sSec45.sPhB.f16Offset = (frac16_t)(uint16_t)
        (s_sfsAdc16Vars.sSec45.sPhB.ui32Calib >> (ADC_I_PH_OFFSET_SMPLS));
}

