/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "m1_state_machine.h"
#include "m1_mcdrv.h"
#include "safety_error_handler.h"
#include "safety_flow_check.h"
#include "hardware_cfg.h"

MISRAC_DISABLE /* MISRA check disabled (IAR IDE only). */

#include "freemaster_tsa_pmsm.h"

MISRAC_ENABLE /* MISRA check enabled (IAR IDE only). */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* The default SVM sector. */
#define M1_SVM_SECTOR_DEFAULT      (2U)

/* The blocked rotor EMF voltage MA filter window. */
#define M1_BLOCK_ROT_FAULT_SH      (5U)

/* Recoverable fault codes. */
#define MC_FAULT_I_DCBUS_OVER      (0U)  /* Over-current fault flag. */
#define MC_FAULT_U_DCBUS_UNDER     (1U)  /* Under-voltage fault flag. */
#define MC_FAULT_U_DCBUS_OVER      (2U)  /* Over-voltage fault flag. */
#define MC_FAULT_LOAD_OVER         (3U)  /* Over-load fault flag. */
#define MC_FAULT_SPEED_OVER        (4U)  /* Over-speed fault flag. */
#define MC_FAULT_ROTOR_BLOCKED     (5U)  /* Blocked-rotor fault flag. */

/* Macro for raising selected fault flag. */
#define MC_FAULT_SET(faults, faultid) ((faults) |= (1U << (faultid)))

/* Macro for fault flag clearing. */
#define MC_FAULT_CLEAR_ALL(faults) ((faults) = 0U)

/*                              DATA TYPES                                    */
/* Pointer to state function. */
typedef void (*pfcn_void_void)(void);

/*
 * Position and speed estimation algorithm for ALIGN state.
 * -Two-step alignment is used.
 */
typedef struct _mc_pmsm_est_align
{
    uint16_t ui16Time;     /* Alignment time duration. */
} mc_pmsm_est_align_t;

/*
 * Position and speed estimation algorithm for LO_SPD and MI_SPD states.
 * -The OPEN-LOOP STARTUP algorithm is used.
 */
typedef struct _mc_pmsm_est_lospd
{
    /* Speed-to-position integrator structure. */
    GFLIB_INTEGRATOR_T_A32 sSpdElInteg;
    /* Speed ramp structure. */
    GFLIB_RAMP_T_F32       sSpdElRamp;
    /* Absolute value of generated electrical speed. */
    frac16_t f16SpdElEstAbs;
    /* Generated electrical speed. */
    frac16_t f16SpdElEst;
    /* Generated electrical position. */
    frac16_t f16PosElEst;
} mc_pmsm_est_lospd_t;

/*
 * Position and speed estimation algorithm for MI_SPD and HI_SPD states.
 * -The BEMF observer algorithm is used.
 */
typedef struct _mc_pmsm_est_hispd
{
    /* BEMF observer control structure. */
    AMCLIB_BEMF_OBSRV_DQ_T_A32 sBemfObsrv;
    /* Tracking observer. */
    AMCLIB_TRACK_OBSRV_T_F32   sTo;
    /* Estimated speed filter. */
    GDFLIB_FILTER_IIR1_T_F32   sSpdElEstFilt;
    /* Electrical position sin/cos (at the moment of phase current reading). */
    GMCLIB_2COOR_SINCOS_T_F16  sPosElEstSC;
    /* Estimated electrical speed. */
    frac16_t f16SpdElEst;
    /* Estimated electrical position. */
    frac16_t f16PosElEst;
} mc_pmsm_est_hispd_t;

/*
 * Position and speed merging algorithm used in MI_SPD state.
*/
typedef struct _mc_pmsm_est_merge
{
    /* The low- to the medium-speed region transfer threshold. */
    frac16_t f16SpdElThrLSToMS;
    /* The threshold between medium- and the high-speed region. */
    frac16_t f16SpdElThrMerge;
    /* Increment of merging ratio. */
    frac16_t f16Coeff;
    /* Merging weight coefficient. */
    frac16_t f16Ratio;
} mc_pmsm_est_merge_t;


/*
 * The main motor-control state-machine structure data-type.
 */
typedef struct _mc_pmsm
{
    /* Measurement variables. */
    struct
    {
        /* DC-bus voltage IIR filter. */
        GDFLIB_FILTER_IIR1_T_F32  sU_DCBFilt;
        /* Current vector in stator coordinates. */
        GMCLIB_2COOR_ALBE_T_F16   sIAlBe;
        /* Filtered DC-bus voltage. */
        frac16_t  f16U_DCBFilt;
    } sMeas;

    /* Position and speed estimation and generation algorithms. */
    struct
    {
        /* The alignment position and speed generation algorithm. */
        mc_pmsm_est_align_t sAlign;
        /* The low- to mid-speed position and speed generation algorithm. */
        mc_pmsm_est_lospd_t sLoSpd;
        /* The merging algorithm. */
        mc_pmsm_est_merge_t sMerge;
        /* The mid- to high-speed position and speed estimation algorithm. */
        mc_pmsm_est_hispd_t sHiSpd;

        /* The spin direction received during FOC during T.A-LS transition. */
        frac16_t f16Dir;
        /* The final (merged) rotor electrical speed estimate. */
        frac16_t  f16SpdElEst;
        /* The final (merged) electrical position estimate. */
        frac16_t  f16PosElEst;
    } sEst;

    /* The fault diagnostic mechanisms. */
    struct
    {
        /* The blocked-rotor detection. */
        struct
        {
            /* Blocked rotor detection EMF voltage filter */
            GDFLIB_FILTER_MA_T_A32 sUqFilt;
            /* Blocked rotor Q-axis voltage threshold level. */
            frac16_t f16ThrUqBemf;
            /* Number of period to set blocked rotor fault. */
            uint16_t ui16ThrCount;
            /* Actual rotor Uq level. */
            frac16_t f16UqBemf;
            /* The blocked rotor condition timer. */
            uint16_t ui16Ctr;
        } sBlckRot;
        uint16_t ui16Captured;      /* Captured fault codes. */
        uint16_t ui16Pending;       /* Pending fault codes. */
        frac16_t f16ThrUDcBusOver;  /* DC-bus over-voltage level. */
        frac16_t f16ThrUDcBusUnder; /* DC-bus under-voltage level. */
        frac16_t f16ThrUDcBusTrip;  /* DC-bus voltage level to start braking. */
        frac16_t f16ThrSpeedOver;   /* Over-speed level. */
        frac16_t f16ThrSpeedMin;    /* Minimum speed level. */
        frac16_t f16ThrSpeedNom;    /* Nominal speed. */
    } sDiag;

    /* The measurements as input for the FOC control module. */
    mc_foc_input_meas_t sFOCin;

    /* The actuator (inverter) control variables. */
    struct
    {
        /* Applied phase duty cycles. */
        GMCLIB_3COOR_T_F16         sDutyABC;
        /* Required stator voltage vector in stator coordinates. */
        GMCLIB_2COOR_ALBE_T_F16    sUAlBeReq;
        /* DC-bus voltage compensated stator voltage. */
        GMCLIB_2COOR_ALBE_T_F16    sUAlBeComp;
        /* Maximum allowable duty cycle. */
        frac16_t  f16DutyCycleLimit;
        /* SVM sector. */
        uint16_t  ui16SectorSVM;
    } sAct;

    /* State-machine variables. */
    mc_sm_state_t eState;                  /* The actual MC.SM state. */
    foc_status_t  sFOCStatus;              /* The FOC status. */
    mc_status_t   sMCStatus;               /* The MC.SM stat-machine status. */
    uint16_t  ui16TimeFullSpeedFreeWheel;  /* The free-wheel state duration. */
    uint16_t  ui16TimeFaultRelease;        /* The fault release duration. */
    uint16_t  ui16CounterState;            /* State duration timer. */

} mc_pmsm_t;

/*
 * The motor-control state-machine MC.SM constant data.
 */
typedef struct _mc_pmsm_const_t
{
    /* The fast control loop state functions. */
    pfcn_void_void pfStateSlow[kMC_StNumOfStates];

    /* The slow control loop state functions. */
    pfcn_void_void pfStateFast[kMC_StNumOfStates];

    /* SVM sector conversion table. */
    uint8_t pui8SVMConvTable[7U];
} mc_pmsm_const_t;


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*                    UTILITY AND DIAGNOSTIC FUNCTIONS                        */
static        void M1_fsClearStates(void);
static inline void M1_fsDiagFaults_FL(void);
static inline void M1_fsDiagFaults_HiSpd_FL(void);

/*                        ESTIMATION FUNCTIONS                                */
static inline void M1_fsEstimateAlign_FL(void);
static        void M1_fsEstimateLoSpd_FL(void);
static inline void M1_fsEstimateMerge_FL(void);
static        void M1_fsEstimateHiSpd_FL(void);

/*                           STATES FUNCTIONS                                 */
static void M1_fsStateNO_INIT_FL(void);
static void M1_fsStateFAULT_FL(void);
static void M1_fsStateSTOP_FL(void);
static void M1_fsStateCALIB_FL(void);
static void M1_fsStateREADY_FL(void);
static void M1_fsStateALIGN_FL(void);
static void M1_fsStateLO_SPD_FL(void);
static void M1_fsStateMI_SPD_FL(void);
static void M1_fsStateHI_SPD_FL(void);
static void M1_fsStateFREE_FL(void);

static void M1_fsStateNO_INIT_SL(void);
static void M1_fsStateFAULT_SL(void);
static void M1_fsStateSTOP_SL(void);
static void M1_fsStateCALIB_SL(void);
static void M1_fsStateREADY_SL(void);
static void M1_fsStateALIGN_SL(void);
static void M1_fsStateLO_SPD_SL(void);
static void M1_fsStateMI_SPD_SL(void);
static void M1_fsStateHI_SPD_SL(void);
static void M1_fsStateFREE_SL(void);

/*                         STATE TRANSITION FUNCITONS                         */
static void M1_fsTransToStop_FL(void);     /* Transition T.S. */
static void M1_fsTransToFault_FL(void);    /* Transition T.FLT. */
static void M1_fsTransFaultStop_FL(void);  /* Transition T.F-S. */
static void M1_fsTransStopCalib_FL(void);  /* Transition T.S-C. */
static void M1_fsTransCalibReady_FL(void); /* Transition T.C-R. */
static void M1_fsTransReadyAlign_FL(void); /* Transition T.R-A. */
static void M1_fsTransAlignLoSpd_FL(void); /* Transition T.A-LS. */
static void M1_fsTransLoSpdMiSpd_FL(void); /* Transition T.LS-MS. */
static void M1_fsTransMiSpdHiSpd_FL(void); /* Transition T.MS-HS. */
static void M1_fsTransToFree_FL(void);     /* Transition T.FREE. */


/*******************************************************************************
 * Variables
 ******************************************************************************/
#if defined(__IAR_SYSTEMS_ICC__) /* IAR */

    #pragma section = ".safety_ram"

    /* Main state-machine structure */
    static mc_pmsm_t       s_sfsM1Drv @ ".safety_ram";

#elif defined(__GNUC__) && (__ARMCC_VERSION >= 6010050) /* KEIL */

    #include "linker_config.h"

    /* Main state-machine structure */
    static mc_pmsm_t       s_sfsM1Drv __attribute__((section(".safety_ram")));
#elif defined(__GNUC__) /* MCUXpresso */

    /* Main state-machine structure */
    static mc_pmsm_t       s_sfsM1Drv __attribute__((section(".safety_ram")));


#endif /* defined(__IAR_SYSTEMS_ICC__) */

/* Initialize the motor-control state-machine MC.SM constant data. */
const mc_pmsm_const_t c_sM1Cnst =
{
    /* The fast control loop states. */
    .pfStateFast =
    {
        M1_fsStateNO_INIT_FL,
        M1_fsStateFAULT_FL,
        M1_fsStateSTOP_FL,
        M1_fsStateCALIB_FL,
        M1_fsStateREADY_FL,
        M1_fsStateALIGN_FL,
        M1_fsStateLO_SPD_FL,
        M1_fsStateMI_SPD_FL,
        M1_fsStateHI_SPD_FL,
        M1_fsStateFREE_FL
    },
    /* The slow control loop states. */
    .pfStateSlow =
    {
        M1_fsStateNO_INIT_SL,
        M1_fsStateFAULT_SL,
        M1_fsStateSTOP_SL,
        M1_fsStateCALIB_SL,
        M1_fsStateREADY_SL,
        M1_fsStateALIGN_SL,
        M1_fsStateLO_SPD_SL,
        M1_fsStateMI_SPD_SL,
        M1_fsStateHI_SPD_SL,
        M1_fsStateFREE_SL
    },
    .pui8SVMConvTable = {1U, 2U, 3U, 4U, 5U, 6U, 1U}
};

/*!
 * @brief   The MC state-machine variables TSA table.
 *
 * @details See freemaster_tsa.h for more details about TSA table construction.
 *
 * @param   TSA table identificator.
 *
 * @return  FMSTR_TSA_ENTRY* - The TSA table entry data type.
 */
MISRAC_DISABLE /* MISRA check disabled (IAR IDE only). */
FMSTR_TSA_TABLE_BEGIN_FS(TSA_MCSM)
    /*               Read-write variables (parameters) below.                 */
    /* Measurement parameters. */
    FMSTR_TSA_RW_VAR_FS(SM00, s_sfsM1Drv.sMeas.sU_DCBFilt.sFltCoeff.f32B0, FMSTR_TSA_FRAC32)
    FMSTR_TSA_RW_VAR_FS(SM01, s_sfsM1Drv.sMeas.sU_DCBFilt.sFltCoeff.f32B1, FMSTR_TSA_FRAC32)
    FMSTR_TSA_RW_VAR_FS(SM02, s_sfsM1Drv.sMeas.sU_DCBFilt.sFltCoeff.f32A1, FMSTR_TSA_FRAC32)

    /* Estimation parameters. */
    FMSTR_TSA_RW_VAR_FS(SM03, s_sfsM1Drv.sEst.sAlign.ui16Time, FMSTR_TSA_UINT16)

    FMSTR_TSA_RW_VAR_FS(SM04, s_sfsM1Drv.sEst.sLoSpd.sSpdElInteg.a32Gain,  FMSTR_TSA_FRAC_Q(16, 15))
    FMSTR_TSA_RW_VAR_FS(SM05, s_sfsM1Drv.sEst.sLoSpd.sSpdElRamp.f32RampUp, FMSTR_TSA_FRAC32)

    FMSTR_TSA_RW_VAR_FS(SM06, s_sfsM1Drv.sEst.sHiSpd.sTo.f16PGain,    FMSTR_TSA_FRAC16)
    FMSTR_TSA_RW_VAR_FS(SM07, s_sfsM1Drv.sEst.sHiSpd.sTo.i16PGainSh,  FMSTR_TSA_SINT16)
    FMSTR_TSA_RW_VAR_FS(SM08, s_sfsM1Drv.sEst.sHiSpd.sTo.f16IGain,    FMSTR_TSA_FRAC16)
    FMSTR_TSA_RW_VAR_FS(SM09, s_sfsM1Drv.sEst.sHiSpd.sTo.i16IGainSh,  FMSTR_TSA_SINT16)
    FMSTR_TSA_RW_VAR_FS(SM10, s_sfsM1Drv.sEst.sHiSpd.sTo.f16ThGain,   FMSTR_TSA_FRAC16)
    FMSTR_TSA_RW_VAR_FS(SM11, s_sfsM1Drv.sEst.sHiSpd.sTo.i16ThGainSh, FMSTR_TSA_SINT16)
    FMSTR_TSA_RW_VAR_FS(SM12, s_sfsM1Drv.sEst.sHiSpd.sBemfObsrv.a32IGain,  FMSTR_TSA_FRAC_Q(16, 15))
    FMSTR_TSA_RW_VAR_FS(SM13, s_sfsM1Drv.sEst.sHiSpd.sBemfObsrv.a32UGain,  FMSTR_TSA_FRAC_Q(16, 15))
    FMSTR_TSA_RW_VAR_FS(SM14, s_sfsM1Drv.sEst.sHiSpd.sBemfObsrv.a32EGain,  FMSTR_TSA_FRAC_Q(16, 15))
    FMSTR_TSA_RW_VAR_FS(SM15, s_sfsM1Drv.sEst.sHiSpd.sBemfObsrv.a32WIGain, FMSTR_TSA_FRAC_Q(16, 15))
    FMSTR_TSA_RW_VAR_FS(SM16, s_sfsM1Drv.sEst.sHiSpd.sBemfObsrv.sCtrl.a32PGain, FMSTR_TSA_FRAC_Q(16, 15))
    FMSTR_TSA_RW_VAR_FS(SM17, s_sfsM1Drv.sEst.sHiSpd.sBemfObsrv.sCtrl.a32IGain, FMSTR_TSA_FRAC_Q(16, 15))
    FMSTR_TSA_RW_VAR_FS(SM18, s_sfsM1Drv.sEst.sHiSpd.sSpdElEstFilt.sFltCoeff.f32B0, FMSTR_TSA_FRAC32)
    FMSTR_TSA_RW_VAR_FS(SM19, s_sfsM1Drv.sEst.sHiSpd.sSpdElEstFilt.sFltCoeff.f32B1, FMSTR_TSA_FRAC32)
    FMSTR_TSA_RW_VAR_FS(SM20, s_sfsM1Drv.sEst.sHiSpd.sSpdElEstFilt.sFltCoeff.f32A1, FMSTR_TSA_FRAC32)

    FMSTR_TSA_RW_VAR_FS(SM21, s_sfsM1Drv.sEst.sMerge.f16SpdElThrLSToMS, FMSTR_TSA_FRAC16)
    FMSTR_TSA_RW_VAR_FS(SM22, s_sfsM1Drv.sEst.sMerge.f16SpdElThrMerge,  FMSTR_TSA_FRAC16)
    FMSTR_TSA_RW_VAR_FS(SM23, s_sfsM1Drv.sEst.sMerge.f16Coeff, FMSTR_TSA_FRAC16)


    /* Diagnostic parameters. */
    FMSTR_TSA_RW_VAR_FS(SM24, s_sfsM1Drv.sDiag.sBlckRot.sUqFilt.u16Sh, FMSTR_TSA_UINT16)
    FMSTR_TSA_RW_VAR_FS(SM25, s_sfsM1Drv.sDiag.sBlckRot.f16ThrUqBemf, FMSTR_TSA_FRAC16)
    FMSTR_TSA_RW_VAR_FS(SM26, s_sfsM1Drv.sDiag.sBlckRot.ui16ThrCount, FMSTR_TSA_UINT16)
    FMSTR_TSA_RW_VAR_FS(SM27, s_sfsM1Drv.sDiag.f16ThrUDcBusOver,  FMSTR_TSA_FRAC16)
    FMSTR_TSA_RW_VAR_FS(SM29, s_sfsM1Drv.sDiag.f16ThrUDcBusUnder, FMSTR_TSA_FRAC16)
    FMSTR_TSA_RW_VAR_FS(SM30, s_sfsM1Drv.sDiag.f16ThrUDcBusTrip,  FMSTR_TSA_FRAC16)
    FMSTR_TSA_RW_VAR_FS(SM31, s_sfsM1Drv.sDiag.f16ThrSpeedOver,   FMSTR_TSA_FRAC16)
    FMSTR_TSA_RW_VAR_FS(SM32, s_sfsM1Drv.sDiag.f16ThrSpeedMin,    FMSTR_TSA_FRAC16)
    FMSTR_TSA_RW_VAR_FS(SM33, s_sfsM1Drv.sDiag.f16ThrSpeedNom,    FMSTR_TSA_FRAC16)

    /* Actuator parameters. */
    FMSTR_TSA_RW_VAR_FS(SM34, s_sfsM1Drv.sAct.ui16SectorSVM,     FMSTR_TSA_UINT16)

    /* State-machine parameters. */
    FMSTR_TSA_RW_VAR_FS(SM35, s_sfsM1Drv.ui16TimeFullSpeedFreeWheel, FMSTR_TSA_UINT16)
    FMSTR_TSA_RW_VAR_FS(SM36, s_sfsM1Drv.ui16TimeFaultRelease,       FMSTR_TSA_UINT16)

    /*          Read-only variables(measurements, states, flags) below.       */
    /* Measurement state variables. */
    FMSTR_TSA_RO_VAR_FS(SM37, s_sfsM1Drv.sMeas.sIAlBe.f16Alpha, FMSTR_TSA_FRAC16)
    FMSTR_TSA_RO_VAR_FS(SM38, s_sfsM1Drv.sMeas.f16U_DCBFilt, FMSTR_TSA_FRAC16)

    /* Estimation state variables. */
    FMSTR_TSA_RO_VAR_FS(SM39, s_sfsM1Drv.sEst.sLoSpd.f16SpdElEst, FMSTR_TSA_FRAC16)
    FMSTR_TSA_RO_VAR_FS(SM40, s_sfsM1Drv.sEst.sLoSpd.f16PosElEst, FMSTR_TSA_FRAC16)

    FMSTR_TSA_RO_VAR_FS(SM41, s_sfsM1Drv.sEst.sMerge.f16Ratio, FMSTR_TSA_FRAC16)

    FMSTR_TSA_RO_VAR_FS(SM42, s_sfsM1Drv.sEst.sHiSpd.f16SpdElEst, FMSTR_TSA_FRAC16)
    FMSTR_TSA_RO_VAR_FS(SM43, s_sfsM1Drv.sEst.sHiSpd.f16PosElEst, FMSTR_TSA_FRAC16)
    FMSTR_TSA_RO_VAR_FS(SM44, s_sfsM1Drv.sEst.f16Dir,      FMSTR_TSA_FRAC16)
    FMSTR_TSA_RO_VAR_FS(SM45, s_sfsM1Drv.sEst.f16SpdElEst, FMSTR_TSA_FRAC16)
    FMSTR_TSA_RO_VAR_FS(SM46, s_sfsM1Drv.sEst.f16PosElEst, FMSTR_TSA_FRAC16)

    /* Diagnostic state variables. */
    FMSTR_TSA_RW_VAR_FS(SM47, s_sfsM1Drv.sDiag.ui16Captured, FMSTR_TSA_UINT32)
    FMSTR_TSA_RO_VAR_FS(SM48, s_sfsM1Drv.sDiag.ui16Pending, FMSTR_TSA_UINT32)
    FMSTR_TSA_RO_VAR_FS(SM49, s_sfsM1Drv.sDiag.sBlckRot.f16UqBemf,FMSTR_TSA_FRAC16)
    FMSTR_TSA_RO_VAR_FS(SM50, s_sfsM1Drv.sDiag.sBlckRot.ui16Ctr,  FMSTR_TSA_UINT16)

    /* Actuator state variables. */
    FMSTR_TSA_RO_VAR_FS(SM51, s_sfsM1Drv.sAct.ui16SectorSVM, FMSTR_TSA_UINT16)
    FMSTR_TSA_RO_VAR_FS(SM52, s_sfsM1Drv.sAct.sDutyABC.f16A, FMSTR_TSA_FRAC16)
    FMSTR_TSA_RO_VAR_FS(SM53, s_sfsM1Drv.sAct.sUAlBeReq.f16Alpha,  FMSTR_TSA_FRAC16)

    /* State-machine states. */
    FMSTR_TSA_RO_VAR_FS(SM54, s_sfsM1Drv.eState,  FMSTR_TSA_SINT16)

FMSTR_TSA_TABLE_END_FS(TSA_MCSM)
MISRAC_ENABLE /* MISRA check enabled (IAR IDE only). */

/*                         UTILITY FUNCTIONS                                  */

/*!
 * @brief  Clear state variables.
 *
 * @param  void
 *
 * @return none
 */
static void M1_fsClearStates(void)
{
    /*! Measurement initialization. */
    s_sfsM1Drv.sMeas.sIAlBe.f16Alpha = 0;
    s_sfsM1Drv.sMeas.sIAlBe.f16Beta  = 0;


    /*! Estimation initialization. */
    s_sfsM1Drv.sEst.sLoSpd.sSpdElInteg.f32IAccK_1 = 0;
    s_sfsM1Drv.sEst.sLoSpd.sSpdElInteg.f16InValK_1 = 0;
    s_sfsM1Drv.sEst.sLoSpd.sSpdElRamp.f32State = 0;
    s_sfsM1Drv.sEst.sLoSpd.f16SpdElEstAbs = 0;
    s_sfsM1Drv.sEst.sLoSpd.f16SpdElEst = 0;
    s_sfsM1Drv.sEst.sLoSpd.f16PosElEst = 0;

    s_sfsM1Drv.sEst.sHiSpd.sTo.f32Theta = 0;
    s_sfsM1Drv.sEst.sHiSpd.sTo.f32Speed = 0;
    s_sfsM1Drv.sEst.sHiSpd.sTo.f32I_1   = 0;
    s_sfsM1Drv.sEst.sHiSpd.sBemfObsrv.sEObsrv.f32D = 0;
    s_sfsM1Drv.sEst.sHiSpd.sBemfObsrv.sEObsrv.f32Q = 0;
    s_sfsM1Drv.sEst.sHiSpd.sBemfObsrv.sIObsrv.f32D = 0;
    s_sfsM1Drv.sEst.sHiSpd.sBemfObsrv.sIObsrv.f32Q = 0;
    s_sfsM1Drv.sEst.sHiSpd.sBemfObsrv.sCtrl.f32ID_1 = 0;
    s_sfsM1Drv.sEst.sHiSpd.sBemfObsrv.sCtrl.f32IQ_1 = 0;
    s_sfsM1Drv.sEst.sHiSpd.f16SpdElEst = 0;
    s_sfsM1Drv.sEst.sHiSpd.f16PosElEst = 0;
    s_sfsM1Drv.sEst.sHiSpd.sPosElEstSC.f16Sin = 0;
    s_sfsM1Drv.sEst.sHiSpd.sPosElEstSC.f16Cos = FRAC16(1.0);
    GDFLIB_FilterIIR1Init_F16(&s_sfsM1Drv.sEst.sHiSpd.sSpdElEstFilt);

    s_sfsM1Drv.sEst.sMerge.f16Ratio = 0;

    s_sfsM1Drv.sEst.f16Dir = FRAC16(1.0);
    s_sfsM1Drv.sEst.f16SpdElEst = 0;
    s_sfsM1Drv.sEst.f16PosElEst = 0;


    /*! Diagnostic initialization. */
    s_sfsM1Drv.sDiag.sBlckRot.f16UqBemf = 0;
    s_sfsM1Drv.sDiag.sBlckRot.ui16Ctr = 0U;
    GDFLIB_FilterMAInit_F16(0, &s_sfsM1Drv.sDiag.sBlckRot.sUqFilt);


    /*! Control initialization. */
    s_sfsM1Drv.sFOCin.sIAlBe.f16Alpha = 0;
    s_sfsM1Drv.sFOCin.sIAlBe.f16Beta  = 0;
    s_sfsM1Drv.sFOCin.f16SpdEl = 0;
    s_sfsM1Drv.sFOCin.f16PosEl = 0;
    FOC_ClearStates_FL();


    /*! Actuator initialization. */
    s_sfsM1Drv.sAct.ui16SectorSVM = M1_SVM_SECTOR_DEFAULT;
    s_sfsM1Drv.sAct.sDutyABC.f16A = FRAC16(0.5);
    s_sfsM1Drv.sAct.sDutyABC.f16B = FRAC16(0.5);
    s_sfsM1Drv.sAct.sDutyABC.f16C = FRAC16(0.5);
    s_sfsM1Drv.sAct.sUAlBeReq.f16Alpha  = 0;
    s_sfsM1Drv.sAct.sUAlBeReq.f16Beta   = 0;
    s_sfsM1Drv.sAct.sUAlBeComp.f16Alpha = 0;
    s_sfsM1Drv.sAct.sUAlBeComp.f16Beta  = 0;
}


/*!
 * @brief  Common motor-control fault detection routine. Executed in the fast
 *         control loop.
 *
 * @param  void
 *
 * @return none
 */
static inline void M1_fsDiagFaults_FL(void)
{
    /* Clear all pending faults from the previous fault detection function
       execution. */
    MC_FAULT_CLEAR_ALL(s_sfsM1Drv.sDiag.ui16Pending);

    /* Check for the DC-bus over-current fault condition. */
    if(0U < M1_MCDRV_PWM3PH_FLT_GET())
    {
        /* Activate the DC-bus over-current fault. */
        MC_FAULT_SET(s_sfsM1Drv.sDiag.ui16Pending, MC_FAULT_I_DCBUS_OVER);
    }

    /* Check for the DC-bus over-voltage braking condition. */
    if(s_sfsM1Drv.sMeas.f16U_DCBFilt > s_sfsM1Drv.sDiag.f16ThrUDcBusTrip)
    {
        /* Activate the DC-bus braking circuit. */
        M1_MCDRV_BRAKE_ON();

        /* Check for the DC-bus over-voltage fault condition. */
        if(s_sfsM1Drv.sMeas.f16U_DCBFilt > s_sfsM1Drv.sDiag.f16ThrUDcBusOver)
        {
            /* Activate the DC-bus over-voltage fault. */
            MC_FAULT_SET(s_sfsM1Drv.sDiag.ui16Pending, MC_FAULT_U_DCBUS_OVER);
        }
    }
    else
    {
        /* Deactivate the DC-bus braking circuit. */
        M1_MCDRV_BRAKE_OFF();

        /* Check for the DC-bus under-voltage fault condition. */
        if(s_sfsM1Drv.sMeas.f16U_DCBFilt < s_sfsM1Drv.sDiag.f16ThrUDcBusUnder)
        {
            /* Activate the DC-bus over-voltage fault. */
            MC_FAULT_SET(s_sfsM1Drv.sDiag.ui16Pending, MC_FAULT_U_DCBUS_UNDER);
        }
    }

    /* Capture pending faults. */
    s_sfsM1Drv.sDiag.ui16Captured |= s_sfsM1Drv.sDiag.ui16Pending;

    /* The MC.SM common fault detection node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_MCSM_FAULT_DIAG);
}

/*!
 * @brief  High-speed state HI_SPD specific motor-control fault detection routine.
 *
 * @param  void
 *
 * @return none
 */
static inline void M1_fsDiagFaults_HiSpd_FL(void)
{
    /* Speed variables for over-load and over-speed fault condition check. */
    frac16_t f16SpdElFiltAbs;

    /* Get the absolute value of the rotor speed. */
    f16SpdElFiltAbs = MLIB_AbsSat_F16(s_sfsM1Drv.sEst.f16SpdElEst);

    /* Check whether the rotor speed dropped below the minimal value. */
    if(f16SpdElFiltAbs < s_sfsM1Drv.sDiag.f16ThrSpeedMin)
    {
        /* Check whether the speed controller is saturated (maximal
           torque is applied). If yes, then activate over-load fault. */
        s_sfsM1Drv.sDiag.ui16Pending |=
            (((s_sfsM1Drv.sFOCStatus.ui16Flags & FOC_SPD_CTRL_SATURATED_MASK) >>
              FOC_SPD_CTRL_SATURATED) << MC_FAULT_LOAD_OVER);

    }
    /* Check whether the speed is above its maximal value. */
    else if(f16SpdElFiltAbs > s_sfsM1Drv.sDiag.f16ThrSpeedOver)
    {
        /* Activate the over-speed fault. */
        MC_FAULT_SET(s_sfsM1Drv.sDiag.ui16Pending, MC_FAULT_SPEED_OVER);
    }
    else
    {
        /*  Filter the quadrature axis BEMF voltage. */
        s_sfsM1Drv.sDiag.sBlckRot.f16UqBemf = MLIB_AbsSat_F16(
            GDFLIB_FilterMA_F16(MLIB_Conv_F16l(
            s_sfsM1Drv.sEst.sHiSpd.sBemfObsrv.sEObsrv.f32Q),
            &s_sfsM1Drv.sDiag.sBlckRot.sUqFilt));

        /* Check whether the quadrature BEMF voltage threshold was violated. */
        if(s_sfsM1Drv.sDiag.sBlckRot.f16UqBemf < s_sfsM1Drv.sDiag.sBlckRot.f16ThrUqBemf)
        {
            /* Increment the blocked-rotor consequent condition counter. */
            s_sfsM1Drv.sDiag.sBlckRot.ui16Ctr++;

            /* Check whether the blocked-rotor fault condition lasted the
               minimal fault activation time. */
            if(s_sfsM1Drv.sDiag.sBlckRot.ui16Ctr >
               s_sfsM1Drv.sDiag.sBlckRot.ui16ThrCount)
            {
                /* Activate the blocked-rotor fault. */
                MC_FAULT_SET(s_sfsM1Drv.sDiag.ui16Pending, MC_FAULT_ROTOR_BLOCKED);
            }
        }
        else
        {
            /* Clear the block rotor consequent condition counter. */
            s_sfsM1Drv.sDiag.sBlckRot.ui16Ctr = 0U;
        }
    }

#if defined(DEBUG)
    /*! @note Fault detection can be selectively disabled only in debug
              configuration. */
    /* Disable blocked-rotor, over-load, and over-speed fault if requested by FOC. */
    s_sfsM1Drv.sDiag.ui16Pending &=
        (uint16_t)(~(uint16_t)(s_sfsM1Drv.sFOCStatus.ui16Flags &
                              (FOC_DBG_DIS_FAULT_OVERLOAD_MASK  |
                               FOC_DBG_DIS_FAULT_OVERSPEED_MASK |
                               FOC_DBG_DIS_FAULT_BLCKTOR_MASK)));
#endif /* defined(DEBUG)*/

    /* Capture pending faults. */
    s_sfsM1Drv.sDiag.ui16Captured |= s_sfsM1Drv.sDiag.ui16Pending;

    /* The MC.SM state-specific fault diagnostics node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_MCSM_ST_DIAG);
}




/*                         ESTIMATION FUNCTIONS                               */
/*!
 * The ALIGN position and speed estimation function.
 */
static inline void M1_fsEstimateAlign_FL(void)
{
    /* The position set to 120 degree for the first half  of alignment
       duration. */
    if(s_sfsM1Drv.sEst.sAlign.ui16Time < s_sfsM1Drv.ui16CounterState)
    {
        /* Set  position to 120°. */
        s_sfsM1Drv.sEst.f16PosElEst = FRAC16(120.0 / 180.0);
    }
    else
    {
        /* Set position to 0°. */
        s_sfsM1Drv.sEst.f16PosElEst = FRAC16(0.0);
    }
}

/*!
 * The LO_SPD position and speed estimation function.
 */
static void M1_fsEstimateLoSpd_FL(void)
{
    /* Open-loop startup speed ramp. */
    s_sfsM1Drv.sEst.sLoSpd.f16SpdElEst =
        MLIB_Conv_F16l(GFLIB_Ramp_F32(MLIB_Conv_F32s(s_sfsM1Drv.sEst.f16Dir),
                                     &s_sfsM1Drv.sEst.sLoSpd.sSpdElRamp));

    /* Get an absolute value of the speed. */
    s_sfsM1Drv.sEst.sLoSpd.f16SpdElEstAbs =
        MLIB_Abs_F16(s_sfsM1Drv.sEst.sLoSpd.f16SpdElEst);

    /* Generation of open loop position from the open-loop speed. */
    s_sfsM1Drv.sEst.sLoSpd.f16PosElEst =
        GFLIB_Integrator_F16(s_sfsM1Drv.sEst.sLoSpd.f16SpdElEst,
                            &s_sfsM1Drv.sEst.sLoSpd.sSpdElInteg);
}

/*!
 * The position and speed merge function.
 */
static inline void M1_fsEstimateMerge_FL(void)
{
    /* Check whether we reached the merge speed. */
    if(s_sfsM1Drv.sEst.sLoSpd.f16SpdElEstAbs >
       s_sfsM1Drv.sEst.sMerge.f16SpdElThrMerge)
    {
        /* Increment position merging coefficient */
        s_sfsM1Drv.sEst.sMerge.f16Ratio =
            MLIB_AddSat_F16(s_sfsM1Drv.sEst.sMerge.f16Ratio,
                            s_sfsM1Drv.sEst.sMerge.f16Coeff);
    }

    /* Position merging equation. */
    s_sfsM1Drv.sEst.f16PosElEst =
        MLIB_Mac_F16(s_sfsM1Drv.sEst.sLoSpd.f16PosElEst,
                     s_sfsM1Drv.sEst.sMerge.f16Ratio,
                     MLIB_Sub_F16(s_sfsM1Drv.sEst.sHiSpd.f16PosElEst,
                                  s_sfsM1Drv.sEst.sLoSpd.f16PosElEst));

    /* Speed merging equation. */
    s_sfsM1Drv.sEst.f16SpdElEst =
        MLIB_Mac_F16(s_sfsM1Drv.sEst.sLoSpd.f16SpdElEst,
                     s_sfsM1Drv.sEst.sMerge.f16Ratio,
                     MLIB_Sub_F16(s_sfsM1Drv.sEst.sHiSpd.f16SpdElEst,
                                  s_sfsM1Drv.sEst.sLoSpd.f16SpdElEst));
}

/*!
 * The HI_SPD position and speed estimation function.
 */
static void M1_fsEstimateHiSpd_FL(void)
{
    GMCLIB_2COOR_DQ_T_F16 sIDQest, sUDQest;

    /* Calculate the sine and cosine of the actual rotor angle.  */
    s_sfsM1Drv.sEst.sHiSpd.sPosElEstSC.f16Sin =
        GFLIB_Sin_F16(s_sfsM1Drv.sEst.sHiSpd.f16PosElEst);
    s_sfsM1Drv.sEst.sHiSpd.sPosElEstSC.f16Cos =
        GFLIB_Cos_F16(s_sfsM1Drv.sEst.sHiSpd.f16PosElEst);

    /* 2-phase to 2-phase transformation to rotary ref. frame */
    GMCLIB_Park_F16(&s_sfsM1Drv.sMeas.sIAlBe,
                    &s_sfsM1Drv.sEst.sHiSpd.sPosElEstSC,
                    &sIDQest);
    GMCLIB_Park_F16(&s_sfsM1Drv.sAct.sUAlBeReq,
                    &s_sfsM1Drv.sEst.sHiSpd.sPosElEstSC,
                    &sUDQest);

    /* BEMF observer in DQ coordinates. */
    AMCLIB_PMSMBemfObsrvDQ_F16(&sIDQest,
                               &sUDQest,
                                s_sfsM1Drv.sEst.sHiSpd.f16SpdElEst,
                               &s_sfsM1Drv.sEst.sHiSpd.sBemfObsrv);

    /* Tracking observer calculation. */
    s_sfsM1Drv.sEst.sHiSpd.f16PosElEst =
        AMCLIB_TrackObsrv_F16(s_sfsM1Drv.sEst.sHiSpd.sBemfObsrv.f16Error,
                             &s_sfsM1Drv.sEst.sHiSpd.sTo);

    /* Speed estimation IIR filter.  */
    s_sfsM1Drv.sEst.sHiSpd.f16SpdElEst =
        GDFLIB_FilterIIR1_F16(MLIB_Conv_F16l(s_sfsM1Drv.sEst.sHiSpd.sTo.f32Speed),
                             &s_sfsM1Drv.sEst.sHiSpd.sSpdElEstFilt);
}


/*                         APPLICATION INTERFACE                              */
/*!
 * State-machine initialization routine, which must be called during the MCU
 * initialization phase.
 */
void M1_fsStateMachine_AR(void)
{
    /*! Measurement initialization. */
    /* DC-bus voltage U_DCB filtration parameter initialization. */
    s_sfsM1Drv.sMeas.sU_DCBFilt.sFltCoeff.f32B0 = M1_UDCB_IIR_B0;
    s_sfsM1Drv.sMeas.sU_DCBFilt.sFltCoeff.f32B1 = M1_UDCB_IIR_B1;
    s_sfsM1Drv.sMeas.sU_DCBFilt.sFltCoeff.f32A1 = M1_UDCB_IIR_A1;
    s_sfsM1Drv.sMeas.f16U_DCBFilt = (frac16_t)(((frac32_t)M1_U_DCB_UNDERVOLTAGE +
        (frac32_t)M1_U_DCB_OVERVOLTAGE) >> 1U);

    /* Initialize DC-bus voltage U_DCB filter to center value to avoid
       under-voltage fault trigger after startup. */
    s_sfsM1Drv.sMeas.sU_DCBFilt.f16FltBfrX[0U] = s_sfsM1Drv.sMeas.f16U_DCBFilt;
    s_sfsM1Drv.sMeas.sU_DCBFilt.f32FltBfrY[0U] =
        (((frac32_t)M1_U_DCB_UNDERVOLTAGE + (frac32_t)M1_U_DCB_OVERVOLTAGE) << 15U);


    /*! Estimation initialization. */
    /* Alignment position generation algorithm. */
    s_sfsM1Drv.sEst.sAlign.ui16Time = M1_ALIGN_DURATION;

    /* The low-speed open-loop position generation algorithm. */
    s_sfsM1Drv.sEst.sLoSpd.sSpdElInteg.a32Gain    = M1_SCALAR_INTEG_GAIN;
    s_sfsM1Drv.sEst.sLoSpd.sSpdElRamp.f32RampUp   = M1_OL_START_RAMP_INC;
    s_sfsM1Drv.sEst.sLoSpd.sSpdElRamp.f32RampDown = M1_OL_START_RAMP_INC;

    /* Position and speed tracking observer parameter initialization. */
    s_sfsM1Drv.sEst.sHiSpd.sTo.f16PGain    = M1_TO_KP_GAIN;
    s_sfsM1Drv.sEst.sHiSpd.sTo.i16PGainSh  = M1_TO_KP_SHIFT;
    s_sfsM1Drv.sEst.sHiSpd.sTo.f16IGain    = M1_TO_KI_GAIN;
    s_sfsM1Drv.sEst.sHiSpd.sTo.i16IGainSh  = M1_TO_KI_SHIFT;
    s_sfsM1Drv.sEst.sHiSpd.sTo.f16ThGain   = M1_TO_THETA_GAIN;
    s_sfsM1Drv.sEst.sHiSpd.sTo.i16ThGainSh = M1_TO_THETA_SHIFT;
    s_sfsM1Drv.sEst.sHiSpd.sBemfObsrv.a32IGain = M1_I_SCALE;
    s_sfsM1Drv.sEst.sHiSpd.sBemfObsrv.a32UGain = M1_U_SCALE;
    s_sfsM1Drv.sEst.sHiSpd.sBemfObsrv.a32EGain = M1_E_SCALE;
    s_sfsM1Drv.sEst.sHiSpd.sBemfObsrv.a32WIGain = M1_WI_SCALE;
    s_sfsM1Drv.sEst.sHiSpd.sBemfObsrv.sCtrl.a32PGain = M1_BEMF_DQ_KP_GAIN;
    s_sfsM1Drv.sEst.sHiSpd.sBemfObsrv.sCtrl.a32IGain = M1_BEMF_DQ_KI_GAIN;

    /* The low- and high-speed merging algorithm. */
    s_sfsM1Drv.sEst.sMerge.f16SpdElThrLSToMS = (M1_MERG_SPEED_TRH >> 1U);
    s_sfsM1Drv.sEst.sMerge.f16SpdElThrMerge = M1_MERG_SPEED_TRH;
    s_sfsM1Drv.sEst.sMerge.f16Coeff = M1_MERG_COEFF;

    /* The estimated speed filter parameter and state initialization. */
    s_sfsM1Drv.sEst.sHiSpd.sSpdElEstFilt.sFltCoeff.f32B0 = M1_TO_SPEED_IIR_B0;
    s_sfsM1Drv.sEst.sHiSpd.sSpdElEstFilt.sFltCoeff.f32B1 = M1_TO_SPEED_IIR_B1;
    s_sfsM1Drv.sEst.sHiSpd.sSpdElEstFilt.sFltCoeff.f32A1 = M1_TO_SPEED_IIR_A1;
    GDFLIB_FilterIIR1Init_F16(&s_sfsM1Drv.sEst.sHiSpd.sSpdElEstFilt);


    /*! Diagnostic initialization. */
    /* Clear fault registers. */
    MC_FAULT_CLEAR_ALL(s_sfsM1Drv.sDiag.ui16Captured);
    MC_FAULT_CLEAR_ALL(s_sfsM1Drv.sDiag.ui16Pending);

    /* Initialize fault thresholds. */
    s_sfsM1Drv.sDiag.sBlckRot.sUqFilt.u16Sh = M1_BLOCK_ROT_FAULT_SH;
    s_sfsM1Drv.sDiag.sBlckRot.f16ThrUqBemf  = M1_E_BLOCK_TRH;
    s_sfsM1Drv.sDiag.sBlckRot.ui16ThrCount  = M1_E_BLOCK_PER;
    s_sfsM1Drv.sDiag.f16ThrUDcBusOver  = M1_U_DCB_OVERVOLTAGE;
    s_sfsM1Drv.sDiag.f16ThrUDcBusUnder = M1_U_DCB_UNDERVOLTAGE;
    s_sfsM1Drv.sDiag.f16ThrUDcBusTrip  = M1_U_DCB_TRIP;
    s_sfsM1Drv.sDiag.f16ThrSpeedOver = M1_N_OVERSPEED;
    s_sfsM1Drv.sDiag.f16ThrSpeedMin  = M1_N_MIN;
    s_sfsM1Drv.sDiag.f16ThrSpeedNom  = M1_N_NOM;


    /*! Control initialization. */
    FOC_Init_AR();


    /*! Actuator (PWM generation for inverter) initialization. */
     /* PWM generation parameter initialization. */
    s_sfsM1Drv.sAct.f16DutyCycleLimit = M1_CLOOP_LIMIT;
    s_sfsM1Drv.sAct.ui16SectorSVM     = M1_SVM_SECTOR_DEFAULT;


    /*! Initialize drivers. */
    /* Initialize the ADC driver. */
    M1_MCDRV_ADC_MEAS_INIT(s_sfsM1Drv.sAct.ui16SectorSVM);


    /*! State-machine initialization. */
    /* Timing control and general variables initialization. */
    s_sfsM1Drv.ui16TimeFullSpeedFreeWheel = M1_FREEWHEEL_DURATION;
    s_sfsM1Drv.ui16TimeFaultRelease = M1_FAULT_DURATION;
    s_sfsM1Drv.ui16CounterState = 0U;

#if defined(DEBUG)
    /* Signalize that that transition occurred. */
    FMSTR_PipeMessage(FMSTR_MSG_SM_S);
#endif /* defined(DEBUG)*/

    /*! Clear states. */
    M1_fsClearStates();

    /* Continue to the STOP state. */
    s_sfsM1Drv.eState = kMC_StSTOP;

    /* Motor-control state-machine initialization node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkAR, FS_FLCK_AR_NODE_MCSM_INIT);
}

/*!
 * State-machine portion, which must be executed in high-priority interrupt
 * routine.
 */
void M1_fsStateMachine_FL(void)
{
    /*! Measure. */
    /* Get all the ADC samples updated in the M1_MCDRV_MEAS structure (DC-bus
       voltage U_DCB, phase currents I_PHn, and the auxiliary sample AUX). */
    M1_MCDRV_ADC_GET_ALL();

    /* Filter the DC-bus voltage U_DCB. */
    s_sfsM1Drv.sMeas.f16U_DCBFilt = GDFLIB_FilterIIR1_F16(
        M1_MCDRV_MEAS->f16UDcBus, &s_sfsM1Drv.sMeas.sU_DCBFilt);

    /* Three-phase to two-phase transformation of stator current in the stator
       reference frame. */
    GMCLIB_Clark_F16(&M1_MCDRV_MEAS->sIABC, &s_sfsM1Drv.sMeas.sIAlBe);

    /* Copy measurements to the FOC control structure. */
    s_sfsM1Drv.sFOCin.sIAlBe = s_sfsM1Drv.sMeas.sIAlBe;
    s_sfsM1Drv.sFOCin.f16U_DCBFilt = s_sfsM1Drv.sMeas.f16U_DCBFilt;
    s_sfsM1Drv.sFOCin.f16AUX = M1_MCDRV_MEAS->f16AUX;


    /*! Diagnostics of common faults prior control. */
    M1_fsDiagFaults_FL();


    /*! Estimation and the FOC control (state-dependent). */
    c_sM1Cnst.pfStateFast[s_sfsM1Drv.eState]();


    /*! Update PWM peripheral driver (actuator). */
    /* Assign ADC channels for the three-phase current measurement. */
    M1_MCDRV_CURR_3PH_CHAN_ASSIGN(s_sfsM1Drv.sAct.ui16SectorSVM);

    /* Update three-phase PWM duty cycles. */
    M1_MCDRV_PWM3PH_SET(s_sfsM1Drv.sAct.sDutyABC);
}

/*!
 * State-machine portion, which should be executed in low-priority interrupt
 * routine.
 */
void M1_fsStateMachine_SL(void)
{
    /* Gather the MC state-machine status.  */
    s_sfsM1Drv.sMCStatus.eState = s_sfsM1Drv.eState;
    s_sfsM1Drv.sMCStatus.ui16FaultPending  = s_sfsM1Drv.sDiag.ui16Pending;
    s_sfsM1Drv.sMCStatus.ui16FaultCaptured = s_sfsM1Drv.sDiag.ui16Captured;

    /*! Execute the state-dependent slow control loop part. */
    c_sM1Cnst.pfStateSlow[s_sfsM1Drv.eState]();
}



/*                         FAST LOOP STATES                                   */
/*!
 * The NO_INIT state executed in the FAST loop.
 */
static void M1_fsStateNO_INIT_FL(void)
{
    /* The state-machine was executed without initialization. */
    FS_fsErrorHandling(FS_ERROR_MCSM_FAIL, SAFETY_ERROR_NO_INIT);
}

/*!
 * The FAULT state executed in the FAST loop.
 */
static void M1_fsStateFAULT_FL(void)
{
    /* The MC.SM state-specific fault diagnostics node (no diagnostic
       currently done). */
    FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_MCSM_ST_DIAG);

    /* Check whether the fault release timer in slow control loop overflowed. */
    if(0U == s_sfsM1Drv.ui16CounterState)
    {
        /* Transition to the STOP state. */
        M1_fsTransFaultStop_FL();
    }
    else
    {
         /* Execute the FOC control. */
        FOC_RoutineIdle_FL(s_sfsM1Drv.sFOCin);

        /* MC.SM state control node. */
        FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_MCSM_ST_CTRL);
    }
}

/*!
 * The STOP state executed in the FAST loop.
 */
static void M1_fsStateSTOP_FL(void)
{
    /* The MC.SM state-specific fault diagnostics node (no diagnostic
       currently done). */
    FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_MCSM_ST_DIAG);

    /* Check whether any fault condition is active. */
    if(0UL < s_sfsM1Drv.sDiag.ui16Pending)
    {
        /* Transition to the FAULT state. */
        M1_fsTransToFault_FL();
    }
    /* Check whether RUN or READY request was issued by FOC. */
    else if((kMC_ReqReady == s_sfsM1Drv.sFOCStatus.eRequest) ||
            (kMC_ReqRun   == s_sfsM1Drv.sFOCStatus.eRequest))
    {
        /* Transition to the CALIB state. */
        M1_fsTransStopCalib_FL();
    }
    else
    {
        /* Execute the FOC control. */
        FOC_RoutineIdle_FL(s_sfsM1Drv.sFOCin);

        /* MC.SM state control node. */
        FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_MCSM_ST_CTRL);
    }
}

/*!
 * The CALIB state executed in the FAST loop.
 */
static void M1_fsStateCALIB_FL(void)
{
    /* The MC.SM state-specific fault diagnostics node (no diagnostic
       currently done). */
    FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_MCSM_ST_DIAG);

    /* Check whether any fault condition is active. */
    if(0UL < s_sfsM1Drv.sDiag.ui16Pending)
    {
        /* Transition to the FAULT state. */
        M1_fsTransToFault_FL();
    }
    /* Check whether STOP command was issued by FOC. */
    else if(kMC_ReqStop == s_sfsM1Drv.sFOCStatus.eRequest)
    {
        /* Transition to the STOP state. */
        M1_fsTransToStop_FL();
    }
    /* Perform the offset measurement. */
    else if(0U == M1_MCDRV_CURR_3PH_CALIB())
    {
        /* Transition to READY state. */
        M1_fsTransCalibReady_FL();
    }
    else
    {
        /* Execute the FOC control. */
        FOC_RoutineIdle_FL(s_sfsM1Drv.sFOCin);

        /* Swap the SVM sector to measure offset for all SVM sectors. */
        s_sfsM1Drv.sAct.ui16SectorSVM =
            c_sM1Cnst.pui8SVMConvTable[s_sfsM1Drv.sAct.ui16SectorSVM];

        /* MC.SM state control node. */
        FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_MCSM_ST_CTRL);
    }
}

/*!
 * The READY state executed in the FAST loop.
 */
static void M1_fsStateREADY_FL(void)
{
    /* The MC.SM state-specific fault diagnostics node (no diagnostic
       currently done). */
    FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_MCSM_ST_DIAG);

    /* Check whether any fault condition is active. */
    if(0UL < s_sfsM1Drv.sDiag.ui16Pending)
    {
        /* Transition to the FAULT state. */
        M1_fsTransToFault_FL();
    }
    /* Check whether STOP command was issued by FOC. */
    else if(kMC_ReqStop == s_sfsM1Drv.sFOCStatus.eRequest)
    {
        /* Transition to the STOP state. */
        M1_fsTransToStop_FL();
    }
    else if(kMC_ReqRun == s_sfsM1Drv.sFOCStatus.eRequest)
    {
        /* Transition to the ALIGN state. */
        M1_fsTransReadyAlign_FL();
    }
    else
    {
        /* Execute the FOC control. */
        FOC_RoutineIdle_FL(s_sfsM1Drv.sFOCin);

        /* MC.SM state control node. */
        FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_MCSM_ST_CTRL);
    }
}

/*!
 * The ALIGN state executed in the FAST loop.
 */
static void M1_fsStateALIGN_FL(void)
{
    /* The MC.SM state-specific fault diagnostics node (no diagnostic
       currently done). */
    FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_MCSM_ST_DIAG);

    /* Check whether any fault condition is active. */
    if(0UL < s_sfsM1Drv.sDiag.ui16Pending)
    {
        /* Transition to the FAULT state. */
        M1_fsTransToFault_FL();
    }
    /* Check whether STOP command was issued by FOC. */
    else if(kMC_ReqStop == s_sfsM1Drv.sFOCStatus.eRequest)
    {
        /* Transition to the STOP state. */
        M1_fsTransToStop_FL();
    }
    /* Check whether we reached the end of the alignment procedure. */
    else if(0U == s_sfsM1Drv.ui16CounterState)
    {
        /* Transfer to the LO_SPD state. */
        M1_fsTransAlignLoSpd_FL();
    }
    else
    {
        /*! Estimate: Generate the position to perform two-step alignment. */
        M1_fsEstimateAlign_FL();

        /* Pass the rotor position. */
        s_sfsM1Drv.sFOCin.f16PosEl = s_sfsM1Drv.sEst.f16PosElEst;

        /*! Execute the FOC control. */
        s_sfsM1Drv.sAct.sUAlBeReq = FOC_RoutineAlign_FL(s_sfsM1Drv.sFOCin);

        /* DC-bus voltage ripple elimination. */
        GMCLIB_ElimDcBusRipFOC_F16( s_sfsM1Drv.sMeas.f16U_DCBFilt,
                                   &s_sfsM1Drv.sAct.sUAlBeReq,
                                   &s_sfsM1Drv.sAct.sUAlBeComp);

        /* Get the phase duty cycles using the space vector modulation. */
        s_sfsM1Drv.sAct.ui16SectorSVM = GMCLIB_SvmStd_F16(
            &s_sfsM1Drv.sAct.sUAlBeComp, &s_sfsM1Drv.sAct.sDutyABC);

        /* MC.SM state control node. */
        FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_MCSM_ST_CTRL);
    }
}

/*!
 * The LO_SPD state executed in the FAST loop.
 */
static void M1_fsStateLO_SPD_FL(void)
{
    /* The MC.SM state-specific fault diagnostics node (no diagnostic
       currently done). */
    FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_MCSM_ST_DIAG);

    /* Check whether any fault condition is active. */
    if(0UL < s_sfsM1Drv.sDiag.ui16Pending)
    {
        /* Transition to the FAULT state. */
        M1_fsTransToFault_FL();
    }
    /* Check whether STOP command was issued by FOC. */
    else if(kMC_ReqStop == s_sfsM1Drv.sFOCStatus.eRequest)
    {
        /* Transition to the FREE state. */
        M1_fsTransToFree_FL();
    }
    /* Check whether the medium-speed region threshold was reached. */
    else if(s_sfsM1Drv.sEst.sLoSpd.f16SpdElEstAbs >
            s_sfsM1Drv.sEst.sMerge.f16SpdElThrLSToMS)
    {
        /* Transfer to the MI_SPD state. */
        M1_fsTransLoSpdMiSpd_FL();
    }
    else
    {
        /*! Estimate: Generate the position for the open-loop start-up. */
        M1_fsEstimateLoSpd_FL();

        /* Use the low-speed position and speed estimation algorithm. */
        s_sfsM1Drv.sEst.f16PosElEst = s_sfsM1Drv.sEst.sLoSpd.f16PosElEst;
        s_sfsM1Drv.sEst.f16SpdElEst = s_sfsM1Drv.sEst.sLoSpd.f16SpdElEst;

        /* Pass the rotor position and speed. */
        s_sfsM1Drv.sFOCin.f16PosEl = s_sfsM1Drv.sEst.f16PosElEst;
        s_sfsM1Drv.sFOCin.f16SpdEl = s_sfsM1Drv.sEst.f16SpdElEst;

        /*! Execute the FOC control. */
        s_sfsM1Drv.sAct.sUAlBeReq = FOC_RoutineLoSpd_FL(s_sfsM1Drv.sFOCin);

        /* DC-bus voltage ripple elimination. */
        GMCLIB_ElimDcBusRipFOC_F16( s_sfsM1Drv.sMeas.f16U_DCBFilt,
                                   &s_sfsM1Drv.sAct.sUAlBeReq,
                                   &s_sfsM1Drv.sAct.sUAlBeComp);

        /* Get the phase duty cycles using the space vector modulation. */
        s_sfsM1Drv.sAct.ui16SectorSVM = GMCLIB_SvmStd_F16(
            &s_sfsM1Drv.sAct.sUAlBeComp, &s_sfsM1Drv.sAct.sDutyABC);

        /* MC.SM state control node. */
        FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_MCSM_ST_CTRL);
    }
}

/*!
 * The MI_SPD state executed in the FAST loop.
 */
static void M1_fsStateMI_SPD_FL(void)
{
    /* The MC.SM state-specific fault diagnostics node (no diagnostic
       currently done). */
    FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_MCSM_ST_DIAG);

    /* Check whether any fault condition is active. */
    if(0UL < s_sfsM1Drv.sDiag.ui16Pending)
    {
        /* Transition to the FAULT state. */
        M1_fsTransToFault_FL();
    }
    /* Check whether STOP command was issued by FOC. */
    else if(kMC_ReqStop == s_sfsM1Drv.sFOCStatus.eRequest)
    {
        /* Transition to the FREE state. */
        M1_fsTransToFree_FL();
    }
    /* Check whether the merging was completed. */
    else if(FRAC16(1.0) == s_sfsM1Drv.sEst.sMerge.f16Ratio)
    {
        /* Transfer to the HI_SPD state. */
        M1_fsTransMiSpdHiSpd_FL();
    }
    else
    {
        /*! Estimate: Generate the position for the open-loop start-up and
            estimate rotor position using the BEMF observer. */
        M1_fsEstimateLoSpd_FL();
        M1_fsEstimateHiSpd_FL();

        /* Merge the final position. */
        M1_fsEstimateMerge_FL();

        /* Pass the rotor position and speed. */
        s_sfsM1Drv.sFOCin.f16PosEl = s_sfsM1Drv.sEst.f16PosElEst;
        s_sfsM1Drv.sFOCin.f16SpdEl = s_sfsM1Drv.sEst.f16SpdElEst;

        /*! Execute the FOC control. */
        s_sfsM1Drv.sAct.sUAlBeReq = FOC_RoutineMiSpd_FL(s_sfsM1Drv.sFOCin);

        /* DC-bus voltage ripple elimination. */
        GMCLIB_ElimDcBusRipFOC_F16( s_sfsM1Drv.sMeas.f16U_DCBFilt,
                                   &s_sfsM1Drv.sAct.sUAlBeReq,
                                   &s_sfsM1Drv.sAct.sUAlBeComp);

        /* Get the phase duty cycles using the space vector modulation. */
        s_sfsM1Drv.sAct.ui16SectorSVM = GMCLIB_SvmStd_F16(
            &s_sfsM1Drv.sAct.sUAlBeComp, &s_sfsM1Drv.sAct.sDutyABC);

        /* MC.SM state control node. */
        FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_MCSM_ST_CTRL);
    }
}

/*!
 * The HI_SPD state executed in the FAST loop.
 */
static void M1_fsStateHI_SPD_FL(void)
{
    /* The HI_SPD state-specific fault diagnostics. */
    M1_fsDiagFaults_HiSpd_FL();

    /* Check whether any fault condition is active. */
    if(0UL < s_sfsM1Drv.sDiag.ui16Pending)
    {
        /* Transition to the FAULT state. */
        M1_fsTransToFault_FL();
    }
    /* Check whether STOP command was issued by FOC. */
    else if(kMC_ReqStop == s_sfsM1Drv.sFOCStatus.eRequest)
    {
        /* Transition to the FREE state. */
        M1_fsTransToFree_FL();
    }
    else
    {
        /*! Estimate: Estimate rotor position using the BEMF observer. */
        M1_fsEstimateHiSpd_FL();

        /* Use the low-speed position and speed estimation algorithm. */
        s_sfsM1Drv.sEst.f16PosElEst = s_sfsM1Drv.sEst.sHiSpd.f16PosElEst;
        s_sfsM1Drv.sEst.f16SpdElEst = s_sfsM1Drv.sEst.sHiSpd.f16SpdElEst;

        /* Pass the rotor position and speed. */
        s_sfsM1Drv.sFOCin.f16PosEl = s_sfsM1Drv.sEst.f16PosElEst;
        s_sfsM1Drv.sFOCin.f16SpdEl = s_sfsM1Drv.sEst.f16SpdElEst;

        /*! Execute the FOC control. */
        s_sfsM1Drv.sAct.sUAlBeReq = FOC_RoutineHiSpd_FL(s_sfsM1Drv.sFOCin);

        /* DC-bus voltage ripple elimination. */
        GMCLIB_ElimDcBusRipFOC_F16( s_sfsM1Drv.sMeas.f16U_DCBFilt,
                                   &s_sfsM1Drv.sAct.sUAlBeReq,
                                   &s_sfsM1Drv.sAct.sUAlBeComp);

        /* Get the phase duty cycles using the space vector modulation. */
        s_sfsM1Drv.sAct.ui16SectorSVM = GMCLIB_SvmStd_F16(
            &s_sfsM1Drv.sAct.sUAlBeComp, &s_sfsM1Drv.sAct.sDutyABC);

        /* MC.SM state control node. */
        FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_MCSM_ST_CTRL);
    }
}

/*!
 * The FREE state executed in the FAST loop.
 */
static void M1_fsStateFREE_FL(void)
{
    /* The MC.SM state-specific fault diagnostics node (no diagnostic
       currently done). */
    FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_MCSM_ST_DIAG);

    /* Check whether any fault condition is active. */
    if(0UL < s_sfsM1Drv.sDiag.ui16Pending)
    {
        /* Transition to the FAULT state. */
        M1_fsTransToFault_FL();
    }
    /* Check whether we reached the end of the freewheel procedure. */
    else if(0U == s_sfsM1Drv.ui16CounterState)
    {
        /* Transfer to the STOP state. */
        M1_fsTransToStop_FL();
    }
    else
    {
        /*! Execute the FOC control. */
        FOC_RoutineIdle_FL(s_sfsM1Drv.sFOCin);

        /* MC.SM state control node. */
        FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_MCSM_ST_CTRL);
    }
}


/*                          SLOW LOOP STATES                                 */
/*!
 * The NO_INIT state executed in the SLOW loop.
 */
static void M1_fsStateNO_INIT_SL(void)
{
    /* The state machine was executed without initialization. */
    FS_fsErrorHandling(FS_ERROR_MCSM_FAIL, SAFETY_ERROR_NO_INIT);
}

/*!
 * The FAULT state executed in the SLOW loop.
 */
static void M1_fsStateFAULT_SL(void)
{
    /* Check whether no pending fault is active. */
    if(0UL == s_sfsM1Drv.sDiag.ui16Pending)
    {
        /* Check whether the fault recovery timer has timed out. */
        if(0U < s_sfsM1Drv.ui16CounterState)
        {
            /* Decrement the fault recovery timer. */
            s_sfsM1Drv.ui16CounterState--;
        }
    }
    else
    {
        /* Pending fault is still present. Re-initialize the fault recovery
           timer. */
        s_sfsM1Drv.ui16CounterState = s_sfsM1Drv.ui16TimeFaultRelease;
    }

    /* Call the FOC IDLE routine. */
    s_sfsM1Drv.sFOCStatus = FOC_RoutineIdle_SL(s_sfsM1Drv.sMCStatus);

    /* Slow control loop MC.SM state-machine execution. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkSL, FS_FLCK_SL_MCSM_CTRL);
}

/*!
 * The STOP state executed in the SLOW loop.
 */
static void M1_fsStateSTOP_SL(void)
{
    /* Call the FOC IDLE routine. */
    s_sfsM1Drv.sFOCStatus = FOC_RoutineIdle_SL(s_sfsM1Drv.sMCStatus);

    /* Slow control loop MC.SM state-machine execution. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkSL, FS_FLCK_SL_MCSM_CTRL);
}

/*!
 * The CALIB state executed in the SLOW loop.
 */
static void M1_fsStateCALIB_SL(void)
{
    /* Call the FOC IDLE routine. */
    s_sfsM1Drv.sFOCStatus = FOC_RoutineIdle_SL(s_sfsM1Drv.sMCStatus);

    /* Slow control loop MC.SM state-machine execution. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkSL, FS_FLCK_SL_MCSM_CTRL);
}

/*!
 * The READY state executed in the SLOW loop.
 */
static void M1_fsStateREADY_SL(void)
{
    /* Call the FOC IDLE routine. */
    s_sfsM1Drv.sFOCStatus = FOC_RoutineIdle_SL(s_sfsM1Drv.sMCStatus);

    /* Slow control loop MC.SM state-machine execution. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkSL, FS_FLCK_SL_MCSM_CTRL);
}

/*!
 * The ALIGN state executed in the SLOW loop.
 */
static void M1_fsStateALIGN_SL(void)
{
    /* Check whether we reached the end of the alignment procedure. */
    if(0U < s_sfsM1Drv.ui16CounterState)
    {
        /* Decrement the timer. */
        s_sfsM1Drv.ui16CounterState--;
    }

    /* Call the FOC ALIGN routine. */
    s_sfsM1Drv.sFOCStatus = FOC_RoutineAlign_SL(s_sfsM1Drv.sMCStatus);

    /* Slow control loop MC.SM state-machine execution. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkSL, FS_FLCK_SL_MCSM_CTRL);
}

/*!
 * The LO_SPD state executed in the SLOW loop.
 */
static void M1_fsStateLO_SPD_SL(void)
{
    /* Call the FOC LO_SPD routine. */
    s_sfsM1Drv.sFOCStatus = FOC_RoutineLoSpd_SL(s_sfsM1Drv.sMCStatus);

    /* Slow control loop MC.SM state-machine execution. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkSL, FS_FLCK_SL_MCSM_CTRL);
}

/*!
 * The MI_SPD state executed in the SLOW loop.
 */
static void M1_fsStateMI_SPD_SL(void)
{
    /* Call the FOC MI_SPD routine. */
    s_sfsM1Drv.sFOCStatus = FOC_RoutineMiSpd_SL(s_sfsM1Drv.sMCStatus);

    /* Slow control loop MC.SM state-machine execution. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkSL, FS_FLCK_SL_MCSM_CTRL);
}

/*!
 * The HI_SPD state executed in the SLOW loop.
 */
static void M1_fsStateHI_SPD_SL(void)
{
    /* Call the FOC HI_SPD routine. */
    s_sfsM1Drv.sFOCStatus = FOC_RoutineHiSpd_SL(s_sfsM1Drv.sMCStatus);

    /* Slow control loop MC.SM state-machine execution. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkSL, FS_FLCK_SL_MCSM_CTRL);
}

/*!
 * The FREE state executed in the SLOW loop.
 */
static void M1_fsStateFREE_SL(void)
{
    /* Check whether we reached the end of the freewheel procedure. */
    if(0U < s_sfsM1Drv.ui16CounterState)
    {
        /* Decrement the timer. */
        s_sfsM1Drv.ui16CounterState--;
    }

    /* Call the FOC IDLE routine. */
    s_sfsM1Drv.sFOCStatus = FOC_RoutineIdle_SL(s_sfsM1Drv.sMCStatus);

    /* Slow control loop MC.SM state-machine execution. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkSL, FS_FLCK_SL_MCSM_CTRL);
}



/*                     STATE TRANSITION FUNCTIONS                             */
/*!
 * Transition into STOP state (T.S transition).
 */
static void M1_fsTransToStop_FL(void)
{
    /* Disable the PWM output. */
    M1_MCDRV_PWM3PH_DIS();

    /* Clear the state-machine states. */
    M1_fsClearStates();

    /* Call idle FOC routine transition function. */
    FOC_TransToIdle_FL();

#if defined(DEBUG)
    /* Signalize that that transition occurred. */
    FMSTR_PipeMessage(FMSTR_MSG_SM_S);
#endif /* defined(DEBUG)*/

    /* Continue to the STOP state. */
    s_sfsM1Drv.eState = kMC_StSTOP;

    /* The MC.SM state transition branch node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_MCSM_ST_TRANS);
}

/*!
 * Transition into FAULT state (T.FLT transition).
 */
static void M1_fsTransToFault_FL(void)
{
    /* Disable the PWM output. */
    M1_MCDRV_PWM3PH_DIS();

    /* Clear the state-machine states. */
    M1_fsClearStates();

    /* Call idle FOC routine transition function. */
    FOC_TransToIdle_FL();

    /* Initialize the state timer. */
    s_sfsM1Drv.ui16CounterState = s_sfsM1Drv.ui16TimeFaultRelease;

#if defined(DEBUG)
    /* Signalize that that transition occurred. */
    FMSTR_PipeMessage(FMSTR_MSG_SM_FLT);
#endif /* defined(DEBUG)*/

    /* Continue to the FAULT state. */
    s_sfsM1Drv.eState = kMC_StFAULT;

    /* The MC.SM state transition branch node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_MCSM_ST_TRANS);
}

/*!
 * Transition FAULT->STOP state (T.F-S transition).
 */
static void M1_fsTransFaultStop_FL(void)
{
    /* Clear the state-machine states. */
    M1_fsClearStates();

    /* Call idle FOC routine transition function. */
    FOC_TransToIdle_FL();

#if defined(DEBUG)
    /* Signalize that that transition occurred. */
    FMSTR_PipeMessage(FMSTR_MSG_SM_F_S);
#endif /* defined(DEBUG)*/

    /* Continue to the STOP state. */
    s_sfsM1Drv.eState = kMC_StSTOP;

    /* The MC.SM state transition branch node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_MCSM_ST_TRANS);
}

/*!
 * Transition STOP->CALIB state (T.S-C transition).
 */
static void M1_fsTransStopCalib_FL(void)
{
    /* Initialize the measurement calibration. */
    M1_MCDRV_CURR_3PH_CALIB_INIT();

    /* Enable the PWM output. */
    M1_MCDRV_PWM3PH_EN();

#if defined(DEBUG)
    /* Signalize that that transition occurred. */
    FMSTR_PipeMessage(FMSTR_MSG_SM_S_C);
#endif /* defined(DEBUG)*/

    /* Continue to the CALIB state. */
    s_sfsM1Drv.eState = kMC_StCALIB;

    /* The MC.SM state transition branch node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_MCSM_ST_TRANS);
}

/*!
 * Transition CALIB->READY state (T.C-R transition).
 */
static void M1_fsTransCalibReady_FL(void)
{
    /* Write the calibrated offset values. */
    M1_MCDRV_CURR_3PH_CALIB_SET();

#if defined(DEBUG)
    /* Signalize that that transition occurred. */
    FMSTR_PipeMessage(FMSTR_MSG_SM_C_R);
#endif /* defined(DEBUG)*/

    /* Continue to the READY state. */
    s_sfsM1Drv.eState = kMC_StREADY;

    /* The MC.SM state transition branch node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_MCSM_ST_TRANS);
}

/*!
 * Transition READY->ALIGN state (T.R-A transition).
 */
static void M1_fsTransReadyAlign_FL(void)
{
    /* Set voltage vector position to 120°. */
    s_sfsM1Drv.sEst.f16PosElEst = FRAC16(120.0 / 180.0);

    /* Initialize the alignment timer. */
    s_sfsM1Drv.ui16CounterState = s_sfsM1Drv.sEst.sAlign.ui16Time << 1U;

    /* Call align FOC routine transition function. */
    FOC_TransToAlign_FL();

#if defined(DEBUG)
    /* Signalize that that transition occurred. */
    FMSTR_PipeMessage(FMSTR_MSG_SM_R_A);
#endif /* defined(DEBUG)*/

    /* Continue to the ALIGN state. */
    s_sfsM1Drv.eState = kMC_StALIGN;

    /* The MC.SM state transition branch node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_MCSM_ST_TRANS);
}

/*!
 * Transition ALIGN->LO_SPD state (T.A-LS transition).
 */
static void M1_fsTransAlignLoSpd_FL(void)
{
    /* Call low-speed FOC routine transition function. */
    s_sfsM1Drv.sEst.f16Dir = FOC_TransToLoSpd_FL();

#if defined(DEBUG)
    /* Signalize that that transition occurred. */
    FMSTR_PipeMessage(FMSTR_MSG_SM_A_LS);
#endif /* defined(DEBUG)*/

    /* Continue to the LO_SPD state. */
    s_sfsM1Drv.eState = kMC_StLO_SPD;

    /* The MC.SM state transition branch node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_MCSM_ST_TRANS);
}

/*!
 * Transition LO_SPD->MI_SPD state (T.LS-MS transition).
 */
static void M1_fsTransLoSpdMiSpd_FL(void)
{
    /* Initialize the high-speed region estimator (BEMF observer). */
    AMCLIB_PMSMBemfObsrvDQInit_F16(&s_sfsM1Drv.sEst.sHiSpd.sBemfObsrv);

    /* Initialize the tracking observer. */
    AMCLIB_TrackObsrvInit_F16(s_sfsM1Drv.sEst.sLoSpd.f16PosElEst,
                             &s_sfsM1Drv.sEst.sHiSpd.sTo);
    s_sfsM1Drv.sEst.sHiSpd.sTo.f32Speed =
        MLIB_Conv_F32s(s_sfsM1Drv.sEst.sLoSpd.f16SpdElEst);
    s_sfsM1Drv.sEst.sHiSpd.sTo.f32I_1 = s_sfsM1Drv.sEst.sHiSpd.sTo.f32Speed;

    /* Call mid-speed FOC routine transition function. */
    FOC_TransToMiSpd_FL();

#if defined(DEBUG)
    /* Signalize that that transition occurred. */
    FMSTR_PipeMessage(FMSTR_MSG_SM_LS_MS);
#endif /* defined(DEBUG)*/

    /* Continue to the MI_SPD state. */
    s_sfsM1Drv.eState = kMC_StMI_SPD;

    /* The MC.SM state transition branch node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_MCSM_ST_TRANS);
}

/*!
 * Transition MI_SPD->HI_SPD state (T.MS-HS transition).
 */
static void M1_fsTransMiSpdHiSpd_FL(void)
{
    /* Call high-speed FOC routine transition function. */
    FOC_TransToHiSpd_FL();

#if defined(DEBUG)
    /* Signalize that that transition occurred. */
    FMSTR_PipeMessage(FMSTR_MSG_SM_MS_HS);
#endif /* defined(DEBUG)*/

    /* Continue to the HI_SPD state. */
    s_sfsM1Drv.eState = kMC_StHI_SPD;

    /* The MC.SM state transition branch node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_MCSM_ST_TRANS);
}

/*!
 * Transition into FREE state (T.FREE transition).
 */
static void M1_fsTransToFree_FL(void)
{
    /* Disable the PWM output. */
    M1_MCDRV_PWM3PH_DIS();

    /* Clear the state-machine states. */
    M1_fsClearStates();

    /* Call idle FOC routine transition function. */
    FOC_TransToIdle_FL();

    /* Initialize the state timer. */
    s_sfsM1Drv.ui16CounterState = s_sfsM1Drv.ui16TimeFullSpeedFreeWheel;

#if defined(DEBUG)
    /* Signalize that that transition occurred. */
    FMSTR_PipeMessage(FMSTR_MSG_SM_TRF_FREE);
#endif /* defined(DEBUG)*/

    /* Continue to the FREE state. */
    s_sfsM1Drv.eState = kMC_StFREE;

    /* The MC.SM state transition branch node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_MCSM_ST_TRANS);
}

