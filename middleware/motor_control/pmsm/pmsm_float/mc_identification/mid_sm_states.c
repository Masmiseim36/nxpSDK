/*
 * Copyright 2020 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#include "mid_sm_states.h"
#include "mid_auxiliary.h"
#include "ke_measure.h"
#include "pp_measure.h"
#include "mech_measure.h"
#include "hall_calib.h"
#include "mlib.h"
#include "amclib_FP.h"
#include "mcaa_lib_fp.h"

#define M1_SVM_SECTOR_DEFAULT (2)        /* default SVM sector */
#define M1_BLOCK_ROT_FAULT_SH (0.03125F) /* filter window */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Bemf params structure. */
typedef struct _mid_bemf_params_t
{
    float_t fltI_gain;
    float_t fltU_gain;
    float_t fltE_gain;
    float_t fltWI_gain;
    float_t fltBemfDQ_Kps;
    float_t fltBemfDQ_Kis;
}mid_bemf_params_t;

/* Tracking observer params structure. */
typedef struct _mid_to_params_t
{
    float_t fltTO_Kpz;
    float_t fltTO_Kiz;
    float_t fltTO_Theta;
}mid_to_params_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* (user) Motor parameters identification state machine functions */
RAM_FUNC_LIB
static void MID_StateStart(void);
RAM_FUNC_LIB
static void MID_StateRL(void);
RAM_FUNC_LIB
static void MID_StatePp(void);
RAM_FUNC_LIB
static void MID_StateKe(void);
RAM_FUNC_LIB
static void MID_StateMech(void);
RAM_FUNC_LIB
static void MID_StateHall(void);
RAM_FUNC_LIB
static void MID_StateStop(void);
RAM_FUNC_LIB
static void MID_StateFault(void);
RAM_FUNC_LIB
static void MID_StateCalib(void);

/* (user) Motor parameters identification state-transition functions */
RAM_FUNC_LIB
static void MID_TransStart2RL(void);
RAM_FUNC_LIB
static void MID_TransStart2Ke(void);
RAM_FUNC_LIB
static void MID_TransStart2Pp(void);
RAM_FUNC_LIB
static void MID_TransStart2Mech(void);
RAM_FUNC_LIB
static void MID_TransStart2Hall(void);
RAM_FUNC_LIB
static void MID_TransAll2Stop(void);
RAM_FUNC_LIB
static void MID_TransAll2Fault(void);
RAM_FUNC_LIB
static void MID_TransStop2Calib(void);
RAM_FUNC_LIB
static void MID_TransCalib2Start(void);

/* Bemf and Tracking observer calculation function. */
RAM_FUNC_LIB
static void MID_CalcBemfTrackingObsrv(mid_bemf_params_t *sBemfParams, mid_to_params_t *sTOParams);
RAM_FUNC_LIB
static void MID_ClearFOCVariables(void);
RAM_FUNC_LIB
static void MID_FaultDetection(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* User Measurement Type selection */
mid_meas_type_t eUserMIDMeasType;

/* MID measurement status. */
mid_status_t   sUserMIDStatus;

/* User MID start result */
uint32_t       ui32UserMIDStartResult = 0UL;

/* User MID Config result */
uint16_t ui16MeasConfigResult = 0U;

/* MID measurement parameters */
mid_config_t   sUserMIDMeasConfig = MID_DEFAULT_MEAS_CONFIG;

/* User params for setting motor params */
mid_motor_params_user_t sUserMIDMotorParamsKnown = {0.0F, 0.0F, 0.0F, 0.0F,
                                                    0.0F, 0.0F, 1UL};
mid_motor_params_user_t sUserMIDMotorParamsMeas  = {0.0F, 0.0F, 0.0F, 0.0F,
                                                    0.0F, 0.0F, 1UL};

/* Global structure for all measurements */
mid_struct_t g_sMID;

/* FOC and MID */
mid_pmsm_t g_sMidDrive;

volatile float g_fltMIDvoltageScale;
volatile float g_fltMIDDCBvoltageScale;
volatile float g_fltMIDcurrentScale;
volatile float g_fltMIDspeedScale;
volatile float g_fltMIDspeedAngularScale;
volatile float g_fltMIDspeedMechanicalScale;

/* Variables for RL Estim */
MCAA_ESTIMRLINIT_RET_T_FLT  eEstimRetValInit;       /* Return value of the MCAA_EstimRLInit() */
MCAA_ESTIMRL_RET_T_FLT 	  eEstimRetVal;         /* Return value of the MCAA_EstimRL() */
MCAA_ESTIMRL_INIT_T_FLT g_sEstimRLInitCfg;      /* RL estimation initialization structure */
MCAA_ESTIMRL_T_FLT 	  g_sEstimRLStruct;     /* RL estimation configuration structure */
MCAA_ESTIMRL_RUN_T_FLT  g_sEstimRLCtrlRun;      /* Control manual mode and measured values in modes 1, 2 */
uint8_t u8ModeEstimRL;                          /* Selected identification mode */
uint8_t u8PlotCnt;                              /* Plot counter. */
float_t	fltIDcPlot;                             /* DC current [A]. */ 
float_t	fltLdPlot;                              /* Estimated d-axis inductance [H]. */        
float_t	fltLqPlot;                              /* Estimated q-axis inductance [H]. */
float_t fltLdqTable[3*NUM_MEAS];                /* Array for measuring DC current and estimated inductances. */    
rl_estim_cfg_params_t g_sEstimRLInitFMSTR = {
    I_NOMINAL, I_POSMAX, I_NEGMAX, I_LD, I_LQ}; /* Control structure used in FreeMASTER */

#ifdef ESTIMRL_HALF_TS
bool_t bEstimRLHalfTs;
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
/*! @brief MID state machine function */
const mid_pfcn_void_pms g_MID_SM_STATE_TABLE[9U] = {MID_StateStart,
                                                     MID_StateRL,
                                                     MID_StatePp,
                                                     MID_StateKe,
                                                     MID_StateMech,
                                                     MID_StateHall,
                                                     MID_StateStop,
                                                     MID_StateFault,
                                                     MID_StateCalib};

/*!
 * @brief MID START state
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
static void MID_StateStart(void)
{
    /* Type the code to do when in the START state */

    /* MID alignment */
    MID_alignment(&g_sMID.sMIDAlignment);

    /* When MID_alignment is done and Motor Control SM proceeded to SPIN */
    if (g_sMID.sMIDAlignment.bActive == FALSE)
    {
        /* if kMID_PolePairs go to PP state */
        if(g_sMID.eMeasurementType == kMID_PolePairs)
        {
            MID_TransStart2Pp();
        }

        /* if kMID_ElectricalParams go to RL state */
        else if(g_sMID.eMeasurementType == kMID_ElectricalParams)
        {
            MID_TransStart2RL();
        }

        /* if kMID_KeParam go to Ke state */
        else if(g_sMID.eMeasurementType == kMID_KeParam)
        {
            MID_TransStart2Ke();
        }
        
        /* if kMID_MechanicalParams go to MECH state */
        else if(g_sMID.eMeasurementType == kMID_MechanicalParams)
        {
            MID_TransStart2Mech();
        }

        /* if kMID_HallSensors go to HALL state */
        else if(g_sMID.eMeasurementType == kMID_HallSensors)
        {
            MID_TransStart2Hall();
        }

        /* if none of above eMeasurementType applies, go to STOP */
        else
        {
            MID_TransAll2Stop();
        }
    }   
}

/*!
 * @brief MID RL state
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
static void MID_StateRL(void)
{
    /* Type the code to do when in the RL state */
#ifdef ESTIMRL_HALF_TS
    if(bEstimRLHalfTs == TRUE)
    {
#endif  
    GMCLIB_Clark_FLT(&g_sMidDrive.sFocPMSM.sIABC, &g_sMidDrive.sFocPMSM.sIAlBe);
    
    eEstimRetVal = MCAA_EstimRL_FLT(g_sMidDrive.sFocPMSM.fltUDcBus, &g_sMidDrive.sFocPMSM.sIAlBe, &g_sEstimRLStruct, &g_sEstimRLCtrlRun, &g_sMidDrive.sFocPMSM.sUAlBeReq);
        
    /* DCBus ripple elimination */
    GMCLIB_ElimDcBusRipFOC_F16ff(g_sMidDrive.sFocPMSM.fltUDcBus, &g_sMidDrive.sFocPMSM.sUAlBeReq, &g_sMidDrive.sFocPMSM.sUAlBeCompFrac);

    /* Space vector modulation */
    g_sMidDrive.sFocPMSM.ui16SectorSVM = GMCLIB_SvmStd_F16(&g_sMidDrive.sFocPMSM.sUAlBeCompFrac, &g_sMidDrive.sFocPMSM.sDutyABC);
    
    switch(eEstimRetVal)
    {
        case RET_IN_PROGRESS:
        break;
        case RET_DONE: 
              /* Store estimated parameters */
              g_sMID.sMotorParams.fltRs = g_sEstimRLStruct.fltR;
              g_sMID.sMotorParams.fltLd = g_sEstimRLStruct.fltLd;
              g_sMID.sMotorParams.fltLq = g_sEstimRLStruct.fltLq;
              
              /* Indicate finished measurement. */
              g_sMID.ui32FinishedMeas |= MID_EL_RL;
              
              /* Go to STOP state */
              MID_TransAll2Stop();  
            break;
        default:
              /* Error during parameters estimation */
              g_sMID.ui16FaultMID = MID_FAULT_RL_ESTIM; 
              MID_TransAll2Fault();
        break;
    }

#ifdef ESTIMRL_HALF_TS
      bEstimRLHalfTs = FALSE;
    }
    else
      bEstimRLHalfTs = TRUE;
#endif
}

/*!
 * @brief MID PP state
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
static void MID_StatePp(void)
{
    /* Type the code to do when in the PP state */

    /* Call Pp measurement routine */
    MID_getPp(&g_sMID.sMIDPp);

    /* Escape MID_StatePp when measurement ends */
    if(g_sMID.sMIDPp.bActive == FALSE)
    {
        /* Go to STOP state */
        MID_TransAll2Stop();
    }
}

/*!
 * @brief MID KE state
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
static void MID_StateKe(void)
{
    /* Type the code to do when in the KE state */
    /* Call Ke measurement routine */
    MID_getKe(&g_sMID.sMIDKe);

    /* Escape MID_StateKe when measurement ends */
    if(g_sMID.sMIDKe.bActive == FALSE)
    {
        /* Store measured Ke */
        g_sMID.sMotorParams.fltKe = g_sMID.sMIDKe.fltKe;

        /* Indicate finished measurement. */
        g_sMID.ui32FinishedMeas |= MID_EL_KE;

        /* Go to STOP state */
        MID_TransAll2Stop();
    }
}

/*!
 * @brief MID MECH state
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
static void MID_StateMech(void)
{
    /* Type the code to do when in the MECH state */

    /* Call Rs measurement routine */
    MID_getMech(&g_sMID.sMIDMech);

    /* Escape MID_StateKe when measurement ends */
    if(g_sMID.sMIDMech.bActive == FALSE)
    {
        /* Store measured J and B */
        g_sMID.sMotorParams.fltJ = g_sMID.sMIDMech.fltJ;
        g_sMID.sMotorParams.fltB = g_sMID.sMIDMech.fltB;

        /* Indicate finished measurement. */
        g_sMID.ui32FinishedMeas |= MID_EL_ME;

        /* Go to STOP state */
        MID_TransAll2Stop();
    }
}

/*!
 * @brief MID HALL state
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
static void MID_StateHall(void)
{
    /* Type the code to do when in the HALL state */

    /* Escape MID_StateKe when measurement ends */
    if(g_sMID.sMIDHall.bActive == FALSE)
    {
        /* Go to STOP state */
        MID_TransAll2Stop();
    }
}

/*!
 * @brief MID STOP state
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
static void MID_StateStop(void)
{
    /* Type the code to do when in the STOP state */
  
    /* Plot dq-inductance */
    fltIDcPlot = fltLdqTable[u8PlotCnt*3]; 
    fltLdPlot = fltLdqTable[u8PlotCnt*3 + 1];
    fltLqPlot = fltLdqTable[u8PlotCnt*3 + 2];

    u8PlotCnt++;
    if(u8PlotCnt>=NUM_MEAS)
            u8PlotCnt=0;      
    
    /* Wait in STOP unless measurement is triggered. */
    if(g_sMID.bMIDStart == TRUE)
    {     
      /* Go to MID CALIB (ADC calibration) */
      MID_TransStop2Calib();
    }
}

/*!
 * @brief MID FAULT state
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
static void MID_StateFault(void)
{
    /* After manual clear fault go to STOP state */
    if (!FAULT_ANY(g_sMidDrive.sFaultIdCaptured))
    {
      //MID_TransFault2Stop();
      MID_TransAll2Stop();
    }
}

/*!
 * @brief MID CALIB state
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
static void MID_StateCalib(void)
{
  if (--g_sMidDrive.ui16CounterState == 0U)
  {
      /* Write calibrated offset values */
      M1_MCDRV_CURR_3PH_CALIB_SET(&g_sM1AdcSensor);

      /* To switch to the START state */
      MID_TransCalib2Start();
  }
  else
  {
      /* Call offset measurement */
      M1_MCDRV_CURR_3PH_CALIB(&g_sM1AdcSensor);

      /* Change SVM sector in range <1;6> to measure all AD channel mapping combinations */
      if (++g_sMidDrive.sFocPMSM.ui16SectorSVM > 6)
          g_sMidDrive.sFocPMSM.ui16SectorSVM = 1;
  }
  
}

/*!
 * @brief MID START to RL transition
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
static void MID_TransStart2RL(void)
{
    /* Type the code to do when going from the Start to the RL state */  
  
    /* Check selected current range */
    if(g_sEstimRLInitFMSTR.fltIDcPosMax > MID_I_MEAS_MAX)
    {
      g_sEstimRLInitFMSTR.fltIDcPosMax = MID_I_MEAS_MAX;
    }

    if(g_sEstimRLInitFMSTR.fltIDcNom > MID_I_MEAS_MAX)
    {
      g_sEstimRLInitFMSTR.fltIDcNom = MID_I_MEAS_MAX;
    }    

    if(g_sEstimRLInitFMSTR.fltIDcNegMax < (-MID_I_MEAS_MAX))
    {
      g_sEstimRLInitFMSTR.fltIDcNegMax = -MID_I_MEAS_MAX;
    }     
    
    /* Identification init */    
    switch(u8ModeEstimRL)
    {
      case 1:
          /* Mode 1 */ 
          g_sEstimRLInitCfg.fltIDcMax = g_sEstimRLInitFMSTR.fltIDcPosMax;
          g_sEstimRLInitCfg.fltIDcLd = 0.0F;
          g_sEstimRLInitCfg.fltIDcLq = g_sEstimRLInitFMSTR.fltIDcLq;
          g_sEstimRLInitCfg.fltIDcNegMax = 0.0F;
          g_sEstimRLInitCfg.u16LdqNumMeas = NUM_MEAS;
          
          g_sEstimRLCtrlRun.pLdqTable = fltLdqTable;
          g_sEstimRLCtrlRun.fltIDcDReq = 0.0F;
          g_sEstimRLCtrlRun.fltIDcQReq = 0.0F;
          g_sEstimRLCtrlRun.fltIAcReq = 0.0F;
          g_sEstimRLCtrlRun.u16FAc = 0U;               
          g_sEstimRLCtrlRun.u8LdqSwitch = 0U;      
          break;
      case 2:
          /* Mode 2 */
          g_sEstimRLInitCfg.fltIDcMax = g_sEstimRLInitFMSTR.fltIDcPosMax;
          g_sEstimRLInitCfg.fltIDcLd = g_sEstimRLInitFMSTR.fltIDcLd;
          g_sEstimRLInitCfg.fltIDcLq = g_sEstimRLInitFMSTR.fltIDcLq;
          g_sEstimRLInitCfg.fltIDcNegMax = g_sEstimRLInitFMSTR.fltIDcNegMax;
          g_sEstimRLInitCfg.u16LdqNumMeas = NUM_MEAS;
          
          g_sEstimRLCtrlRun.pLdqTable = fltLdqTable;
          g_sEstimRLCtrlRun.fltIDcDReq = 0.0F;
          g_sEstimRLCtrlRun.fltIDcQReq = 0.0F;
          g_sEstimRLCtrlRun.fltIAcReq = 0.0F;
          g_sEstimRLCtrlRun.u16FAc = 0U;               
          g_sEstimRLCtrlRun.u8LdqSwitch = 0U;
          break;
      case 3:
          /* Mode 3 */
          
          g_sEstimRLInitCfg.fltIDcMax = 0.0F;
          g_sEstimRLInitCfg.fltIDcLd = 0.0F;
          g_sEstimRLInitCfg.fltIDcLq = 0.0F;
          g_sEstimRLInitCfg.fltIDcNegMax = 0.0F;
          g_sEstimRLInitCfg.u16LdqNumMeas = 1U;
          
          g_sEstimRLCtrlRun.fltIDcDReq = 0.0F;
          g_sEstimRLCtrlRun.fltIDcQReq = 0.0F;
          g_sEstimRLCtrlRun.fltIAcReq = 0.0F;
          g_sEstimRLCtrlRun.u16FAc = 1U;                /* Set frequency greater than zero to avoid returning error from MCAA_EstimRL_FLT. */
          g_sEstimRLCtrlRun.u8LdqSwitch = 0U;
          break;
      default:
          /* Mode 0 */
          g_sEstimRLInitCfg.fltIDcMax = g_sEstimRLInitFMSTR.fltIDcNom;
          g_sEstimRLInitCfg.fltIDcLd = 0.0F;
          g_sEstimRLInitCfg.fltIDcLq = 0.0F;
          g_sEstimRLInitCfg.fltIDcNegMax = 0.0F;
          g_sEstimRLInitCfg.u16LdqNumMeas = 0U;
          
          g_sEstimRLCtrlRun.fltIDcDReq = 0.0F;
          g_sEstimRLCtrlRun.fltIDcQReq = 0.0F;
          g_sEstimRLCtrlRun.fltIAcReq = 0.0F;
          g_sEstimRLCtrlRun.u16FAc = 0U;               
          g_sEstimRLCtrlRun.u8LdqSwitch = 0U;                
          break;
    }
        
    eEstimRetValInit = MCAA_EstimRLInit_FLT(F_SAMPLING, &g_sEstimRLInitCfg ,&g_sEstimRLStruct);
    
    switch(eEstimRetValInit)
    {
      case RET_INIT_OK:
          /* Next is RL state */
          M1_MCDRV_PWM3PH_EN(&g_sM1Pwm3ph);
          g_sMID.eMIDState = kMID_RL;
          break;
      default:
          ui32UserMIDStartResult |= MID_RL_INIT_FAIL; 
          MID_TransAll2Fault();            
          break;
    }
    
#ifdef ESTIMRL_HALF_TS
    bEstimRLHalfTs = TRUE;
#endif
}

/*!
 * @brief MID START to PP transition
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
static void MID_TransStart2Pp(void)
{
    /* Type the code to do when going from the Start to the Pp state */
    g_sMID.sMIDPp.bActive = FALSE;
    g_sMID.sMIDPp.ui16PpDetermined = FALSE;

    /* Disable FOC current loop */
    MID_MC_EnableCurrentLoop(TRUE);

    /* Use OL position for Park transformation. */
    MID_MC_UseEstimatedPos(FALSE);

    /* Next is PP state */
    g_sMID.eMIDState = kMID_Pp;
}

/*!
 * @brief MID START to MECH transition
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
static void MID_TransStart2Mech(void)
{
    mid_bemf_params_t sBemfParams;
    mid_to_params_t sTOParams;
    float_t fltPpInverse = 1.0F / g_sMID.sMotorParams.fltPp;

    /* Type the code to do when going from the LS to the STOP state */
    g_sMID.sMIDMech.bActive    = FALSE;

    /* Calculate Bemf and Tracking parameters */
    MID_CalcBemfTrackingObsrv(&sBemfParams, &sTOParams);

    /* Update the Bemf observer parameters. */
    MID_MC_UpdateBemfObsrv(sBemfParams.fltI_gain,
                       sBemfParams.fltU_gain,
                       sBemfParams.fltE_gain,
                       sBemfParams.fltWI_gain,
                       sBemfParams.fltBemfDQ_Kps,
                       sBemfParams.fltBemfDQ_Kis);

    /* Update the Tracking observer parameters. */
    MID_MC_UpdateTrackingObsrv(sTOParams.fltTO_Kpz,
                               sTOParams.fltTO_Kiz,
                               sTOParams.fltTO_Theta);

    /* Enable FOC current loop */
    MID_MC_EnableCurrentLoop(TRUE);

    /* Use OL position for Park transformation. */
    MID_MC_UseEstimatedPos(TRUE);

    /* fltSpeedCatchUp(merging speed) = Nnom * 0.15; */
    g_sMID.sMIDMech.sStartup.fltSpeedCatchUp = 3000.0F * 0.15F;

    /* I_gain  = (MergCoeff/100) * (60/(Pp*Nmerging))/Ts */
    g_sMID.sMIDMech.sStartup.f16CoeffMerging = MLIB_Conv_F16f((MID_MECH_STARTUP_MERGE_COEFF/100.0F)*(60.0F * fltPpInverse / g_sMID.sMIDMech.sStartup.fltSpeedCatchUp)/MID_FAST_LOOP_TS/2.0F/32768.0F);

    /* Ramp RampUpInc =  RampUp * Ts/SpeedAngScale */
    g_sMID.sMIDMech.sStartup.sSpeedRampOpenLoopParams.fltRampUp = MID_MECH_STARTUP_RAMP_COEFF * MID_FAST_LOOP_TS / (60.0F * fltPpInverse / (2.0F * FLOAT_PI));

    /* Next is MECH state */
    g_sMID.eMIDState = kMID_Mech;
}

/*!
 * @brief MID START to HALL transition
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
static void MID_TransStart2Hall(void)
{
    /* Type the code to do when going from the LS to the STOP state */
    g_sMID.sMIDHall.bActive    = FALSE;

    /* Next is HALL state */
    g_sMID.eMIDState = kMID_Hall;
}

/*!
 * @brief MID START to KE transition
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
static void MID_TransStart2Ke(void)
{
    mid_bemf_params_t sBemfParams;
    mid_to_params_t sTOParams;

    /* Type the code to do when going from the START to the KE state */
    g_sMID.sMIDKe.bActive = FALSE;

    /* Calculate Bemf and Tracking parameters */
    MID_CalcBemfTrackingObsrv(&sBemfParams, &sTOParams);

    /* Update the Bemf observer parameters. */
    MID_MC_UpdateBemfObsrv(sBemfParams.fltI_gain,
                       sBemfParams.fltU_gain,
                       sBemfParams.fltE_gain,
                       sBemfParams.fltWI_gain,
                       sBemfParams.fltBemfDQ_Kps,
                       sBemfParams.fltBemfDQ_Kis);

    /* Update the Tracking observer parameters. */
    MID_MC_UpdateTrackingObsrv(sTOParams.fltTO_Kpz,
                               sTOParams.fltTO_Kiz,
                               sTOParams.fltTO_Theta);

    /* Enable FOC current loop */
    MID_MC_EnableCurrentLoop(TRUE);

    /* Use OL position for Park transformation. */
    MID_MC_UseEstimatedPos(TRUE);

    /* Next is KE state */
    g_sMID.eMIDState = kMID_Ke;
}

/*!
 * @brief (general) Any state to MID STOP transition
 *
 * @param None
 *
 * @return None
 */
RAM_FUNC_LIB
static void MID_TransAll2Stop(void)
{
    /* Type the code to do when going to the STOP state */  
    /* Disable PWM output */
    M1_MCDRV_PWM3PH_DIS(&g_sM1Pwm3ph);
    
    /* Reset active flags */
    g_sMID.sMIDAlignment.bActive  = FALSE;
    g_sMID.sMIDKe.bActive         = FALSE;
    g_sMID.sMIDPp.bActive         = FALSE;
    g_sMID.sMIDMech.bActive       = FALSE;

    /* Clear the measurement trigger if measurement has been finished. */
    if((g_sMID.eMeasurementType == kMID_KeParam) ||
       (g_sMID.eMeasurementType == kMID_ElectricalParams) ||
       (g_sMID.eMeasurementType == kMID_PolePairs) ||
       (g_sMID.eMeasurementType == kMID_MechanicalParams))
    {
        g_sMID.bMIDStart = FALSE;
    }

    MID_ClearFOCVariables();
    
    /* Next is STOP state */
    g_sMID.eMIDState = kMID_Stop;
}

/*!
 * @brief (general) Any state to FAULT transition
 *
 * @param None
 *
 * @return None
 */
RAM_FUNC_LIB
static void MID_TransAll2Fault(void)
{ 
  /* Disable PWM output */
   M1_MCDRV_PWM3PH_DIS(&g_sM1Pwm3ph);
   g_sMID.bMIDStart = FALSE;
   g_sMID.eMIDState = kMID_Fault;
}

/*!
 * @brief (general) MID STOP to CALIB transition
 *
 * @param None
 *
 * @return None
 */
RAM_FUNC_LIB
static void MID_TransStop2Calib(void)
{
  /* Type the code to do when going from the STOP to the RUN state */
  /* 50% duty cycle */
  g_sMidDrive.sFocPMSM.sDutyABC.f16A = FRAC16(0.5);
  g_sMidDrive.sFocPMSM.sDutyABC.f16B = FRAC16(0.5);
  g_sMidDrive.sFocPMSM.sDutyABC.f16C = FRAC16(0.5);
  
  /* PWM duty cycles calculation and update */
  M1_MCDRV_PWM3PH_SET(&g_sM1Pwm3ph);
  
  /* Clear offset filters */
  M1_MCDRV_CURR_3PH_CALIB_INIT(&g_sM1AdcSensor);

  /* pass calibration routine duration to state counter*/
  g_sMidDrive.ui16CounterState = g_sMidDrive.ui16TimeCalibration;
  
  M1_MCDRV_PWM3PH_EN(&g_sM1Pwm3ph);
  
  /* Next is START state */
  g_sMID.eMIDState = kMID_Calib;
}

/*!
 * @brief (general) MID CALIB to START transition
 *
 * @param None
 *
 * @return None
 */
RAM_FUNC_LIB
static void MID_TransCalib2Start(void)
{
    /* type the code to do when going to the START state */
    /* Enable FOC current loop */
    MID_MC_EnableCurrentLoop(TRUE);

    /* Use OL position for Park transformation. */
    MID_MC_UseEstimatedPos(FALSE);

    M1_MCDRV_PWM3PH_EN(&g_sM1Pwm3ph);
    
    /* Next is START state */
    g_sMID.eMIDState = kMID_Start;
    
}

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * MID after-reset initialization function.
 */
RAM_FUNC_LIB
void MID_Init_AR(void)
{
    /* Clean the internal parameters. */
    g_sMID.sMotorParams.fltPp = 1.0F;
    g_sMID.sMotorParams.fltRs = 0.0F;
    g_sMID.sMotorParams.fltLd = 0.0F;
    g_sMID.sMotorParams.fltLq = 0.0F;
    g_sMID.sMotorParams.fltKe = 0.0F;
    g_sMID.sMotorParams.fltJ  = 0.0F;
    g_sMID.sMotorParams.fltB  = 0.0F;

    /* Clear status variables. */
    g_sMID.bMIDStart    = FALSE;
    g_sMID.ui16FaultMID = 0U;
    g_sMID.ui16WarnMID  = 0U;
    g_sMID.ui32FinishedMeas = 0UL;
    g_sMID.fltSpeedAngScale = (60.0F / (2.0F * FLOAT_PI));

    /* Set the initial MID state. */
    g_sMID.eMIDState = kMID_Stop;
      
    /**** Init FOC structure and pointer to drivers *****/
    /* Type the code to do when in the INIT state */
    g_sMidDrive.sFocPMSM.sIdPiParams.fltInErrK_1 = 0.0F;
    g_sMidDrive.sFocPMSM.sIdPiParams.bLimFlag    = FALSE;

    g_sMidDrive.sFocPMSM.sIqPiParams.fltInErrK_1 = 0.0F;
    g_sMidDrive.sFocPMSM.sIqPiParams.bLimFlag    = FALSE;

    /* PMSM FOC params */
    g_sMidDrive.sFocPMSM.sIdPiParams.fltPGain    = M1_D_KP_GAIN;
    g_sMidDrive.sFocPMSM.sIdPiParams.fltIGain    = M1_D_KI_GAIN;
    g_sMidDrive.sFocPMSM.sIdPiParams.fltUpperLim = M1_U_MAX;
    g_sMidDrive.sFocPMSM.sIdPiParams.fltLowerLim = -M1_U_MAX;

    g_sMidDrive.sFocPMSM.sIqPiParams.fltPGain    = M1_Q_KP_GAIN;
    g_sMidDrive.sFocPMSM.sIqPiParams.fltIGain    = M1_Q_KI_GAIN;
    g_sMidDrive.sFocPMSM.sIqPiParams.fltUpperLim = M1_U_MAX;
    g_sMidDrive.sFocPMSM.sIqPiParams.fltLowerLim = -M1_U_MAX;

    g_sMidDrive.sFocPMSM.ui16SectorSVM     = M1_SVM_SECTOR_DEFAULT;
    g_sMidDrive.sFocPMSM.fltDutyCycleLimit = M1_CLOOP_LIMIT;


    g_sMidDrive.sFocPMSM.fltUDcBus                     = 0.0F;
    g_sMidDrive.sFocPMSM.fltUDcBusFilt                 = 0.0F;
    g_sMidDrive.sFocPMSM.sUDcBusFilter.sFltCoeff.fltB0 = M1_UDCB_IIR_B0;
    g_sMidDrive.sFocPMSM.sUDcBusFilter.sFltCoeff.fltB1 = M1_UDCB_IIR_B1;
    g_sMidDrive.sFocPMSM.sUDcBusFilter.sFltCoeff.fltA1 = M1_UDCB_IIR_A1;
    /* Filter init not to enter to fault */
    g_sMidDrive.sFocPMSM.sUDcBusFilter.fltFltBfrX[0] = (M1_U_DCB_UNDERVOLTAGE / 2.0F) + (M1_U_DCB_OVERVOLTAGE / 2.0F);
    g_sMidDrive.sFocPMSM.sUDcBusFilter.fltFltBfrY[0] = (M1_U_DCB_UNDERVOLTAGE / 2.0F) + (M1_U_DCB_OVERVOLTAGE / 2.0F);


    /* Position and speed observer */
    g_sMidDrive.sFocPMSM.sTo.fltPGain  = M1_TO_KP_GAIN;
    g_sMidDrive.sFocPMSM.sTo.fltIGain  = M1_TO_KI_GAIN;
    g_sMidDrive.sFocPMSM.sTo.fltThGain = M1_TO_THETA_GAIN;

    g_sMidDrive.sFocPMSM.sBemfObsrv.fltIGain       = M1_I_SCALE;
    g_sMidDrive.sFocPMSM.sBemfObsrv.fltUGain       = M1_U_SCALE;
    g_sMidDrive.sFocPMSM.sBemfObsrv.fltEGain       = M1_E_SCALE;
    g_sMidDrive.sFocPMSM.sBemfObsrv.fltWIGain      = M1_WI_SCALE;
    g_sMidDrive.sFocPMSM.sBemfObsrv.sCtrl.fltPGain = M1_BEMF_DQ_KP_GAIN;
    g_sMidDrive.sFocPMSM.sBemfObsrv.sCtrl.fltIGain = M1_BEMF_DQ_KI_GAIN;

    g_sMidDrive.sFocPMSM.sSpeedElEstFilt.sFltCoeff.fltB0 = M1_TO_SPEED_IIR_B0;
    g_sMidDrive.sFocPMSM.sSpeedElEstFilt.sFltCoeff.fltB1 = M1_TO_SPEED_IIR_B1;
    g_sMidDrive.sFocPMSM.sSpeedElEstFilt.sFltCoeff.fltA1 = M1_TO_SPEED_IIR_A1;
    GDFLIB_FilterIIR1Init_FLT(&g_sMidDrive.sFocPMSM.sSpeedElEstFilt);

    /* Timing control and general variables */
    g_sMidDrive.ui16CounterState           = 0U;
    g_sMidDrive.ui16TimeCalibration        = M1_CALIB_DURATION;             /* Multiply due to absence slow loop in MID */
    
    /* fault set to init states */
    FAULT_CLEAR_ALL(g_sMidDrive.sFaultIdCaptured);
    FAULT_CLEAR_ALL(g_sMidDrive.sFaultIdPending);

    /* fault thresholds */
    g_sMidDrive.sFaultThresholds.fltUDcBusOver     = M1_U_DCB_OVERVOLTAGE;
    g_sMidDrive.sFaultThresholds.fltUDcBusUnder    = M1_U_DCB_UNDERVOLTAGE;
    g_sMidDrive.sFaultThresholds.fltUDcBusTrip     = M1_U_DCB_TRIP;
    g_sMidDrive.sFaultThresholds.fltSpeedOver      = M1_N_OVERSPEED;
    g_sMidDrive.sFaultThresholds.fltSpeedMin       = M1_N_MIN;
    g_sMidDrive.sFaultThresholds.fltSpeedNom       = M1_N_NOM;
    g_sMidDrive.sFaultThresholds.fltUqBemf         = M1_E_BLOCK_TRH;
    g_sMidDrive.sFaultThresholds.ui16BlockedPerNum = M1_E_BLOCK_PER;

    /* Defined scaling for FreeMASTER */
    g_fltMIDvoltageScale         = M1_U_MAX;
    g_fltMIDcurrentScale         = M1_I_MAX;
    g_fltMIDDCBvoltageScale      = M1_U_DCB_MAX;
    g_fltMIDspeedScale           = M1_N_MAX;
    g_fltMIDspeedAngularScale    = (60.0F / (M1_MOTOR_PP * 2.0F * FLOAT_PI));
    g_fltMIDspeedMechanicalScale = (60.0F / (2.0F * FLOAT_PI));

    /* Clear rest of variables  */
    MID_ClearFOCVariables();

    /* Init sensors/actuators pointers */
    /* For PWM driver */
    g_sM1Pwm3ph.psUABC = &(g_sMidDrive.sFocPMSM.sDutyABC);
    
    /* For ADC driver */
    g_sM1AdcSensor.pf16UDcBus     = &(g_sMidDrive.sFocPMSM.f16UDcBus);
    g_sM1AdcSensor.psIABC         = &(g_sMidDrive.sFocPMSM.sIABCFrac);
    g_sM1AdcSensor.pui16SVMSector = &(g_sMidDrive.sFocPMSM.ui16SectorSVM);
    g_sM1AdcSensor.pui16AuxChan   = &(g_sMidDrive.f16AdcAuxSample);
    
    /* Disable PWM output */
    M1_MCDRV_PWM3PH_DIS(&g_sM1Pwm3ph);
}

/*!
 * MID fast-loop process function.
 */
RAM_FUNC_LIB
void MID_ProcessFast_FL(void)
{
  
    /* Read measurements from FOC motor-control module. */
    MID_MC_ReadSignals();
    
    /* Detects faults */
    MID_FaultDetection();
    
    /* if fault flag */
    if (g_sMidDrive.sFaultIdPending  > 0U)
    {   
      MID_TransAll2Fault();
    }
    
    /* Execute the MID state machine. */
    g_MID_SM_STATE_TABLE[g_sMID.eMIDState]();

    if(g_sMID.eMIDState != kMID_Calib && g_sMID.eMIDState != kMID_RL)
    {
      /* Apply control signals results to FOC motor-control module. */
      MID_MC_ApplySignals();
    }
    else
    {
      /* PWM peripheral update */
      M1_MCDRV_PWM3PH_SET(&g_sM1Pwm3ph);   
    
      /* set current sensor for  sampling */
      M1_MCDRV_CURR_3PH_CHAN_ASSIGN(&g_sM1AdcSensor); 
    }
    
    
}

/*!
 * MID start function.
 */
RAM_FUNC_LIB
uint32_t MID_Start_BL(mid_meas_type_t eMeasurementType)
{
    uint32_t ui32RetVal = 0UL;

    /* Check validity of the selected measurement type (all necessary motor parameters set). */
    /* Check Rs non-zero (for Ld, Lq, Ke measurement). */
    if(kMID_KeParam == eMeasurementType ||
       kMID_MechanicalParams == eMeasurementType)
    {
        /* Check whether Rs is non-zero. */
        if(!(g_sMID.sMotorParams.fltRs > 0.0F))
        {
            /* Stator resistance wa not set. */
            ui32RetVal |= MID_RS_MISSING;
        }
        
        /* Check whether Ld is non-zero. */
        if(!(g_sMID.sMotorParams.fltLd > 0.0F))
        {
            /* Ld inductance was not set. */
            ui32RetVal |= MID_LD_MISSING;
        }
        
        /* Check whether Lq is non-zero. */
        if(!(g_sMID.sMotorParams.fltLq > 0.0F))
        {
            /* Lq inductance was not set. */
            ui32RetVal |= MID_LQ_MISSING;
        }
    }

    /* Check Rs non-zero (for Ld, Lq, Ke measurement). */
    if(kMID_MechanicalParams == eMeasurementType)
    {
        /* Check whether Ke is non-zero. */
        if(!(g_sMID.sMotorParams.fltKe > 0.0F))
        {
            /* Electrical constant was nt set. */
            ui32RetVal |= MID_KE_MISSING;
        }

        /* Check whether Pp is non-zero. */
        if(!(g_sMID.sMotorParams.fltPp > 0.0F))
        {
            /* Number of pole pairs was not set. */
            ui32RetVal |= MID_PP_MISSING;
        }

    }

    /* If none of above conditions were fulfilled, all necessary parameter for
       measurement were set. */
    if(0UL == ui32RetVal)
    {
        /* Pass the measurement type */
        g_sMID.eMeasurementType = eMeasurementType;

        /* Clear the warning and fault registers. */
        g_sMID.ui16FaultMID = 0U;
        g_sMID.ui16WarnMID  = 0U;

        /* Trigger the measurement */
        g_sMID.bMIDStart = TRUE;
    }

    return ui32RetVal;
}

/*!
 * MID stop function
 */
RAM_FUNC_LIB
void MID_Stop_BL(void)
{
    /* Clear the start trigger to avoid MID restart. */
    g_sMID.bMIDStart = FALSE;

    /* Check whether the pole-pair measurement is ongoing. */
    if(TRUE == g_sMID.sMIDPp.bActive)
    {
        /* Stop the Pp assistant */
        g_sMID.sMIDPp.ui16PpDetermined = TRUE;
        
        /* Indicate finished measurement. */
        g_sMID.ui32FinishedMeas |= MID_EL_PP;
    }

    /* Go to STOP state immediately */
    MID_TransAll2Stop();
}

/*!
 * Return the MID status.
 */
RAM_FUNC_LIB
bool_t MID_GetStatus_BL(mid_status_t *psMIDStatus)
{
    /* Copy the statuses. */
    psMIDStatus->eMIDState        = g_sMID.eMIDState;
    psMIDStatus->ui32FinishedMeas = g_sMID.ui32FinishedMeas;
    psMIDStatus->ui16FaultMID     = g_sMID.ui16FaultMID;
    psMIDStatus->ui16WarnMID      = g_sMID.ui16WarnMID;
    psMIDStatus->fltSpeedAngScale = g_sMID.fltSpeedAngScale;

    /* Return the MID start status variable. */
    return g_sMID.bMIDStart;
}

/*!
 * Function sets known motor parameters before the measurement.
 */
RAM_FUNC_LIB
void MID_SetKnownMotorParams_BL(mid_motor_params_user_t *psMotorParams)
{
    /* Copy the parameters into the internal structure. */
    /* If Pp non-zero. */
    if(psMotorParams->ui32Pp > 0UL)
    {
        g_sMID.sMotorParams.fltPp = (float_t)psMotorParams->ui32Pp;
    }
    /* If Rs non-zero. */
    if(psMotorParams->fltRs > 0.0F)
    {
        g_sMID.sMotorParams.fltRs = psMotorParams->fltRs;
    }

    /* If Ld non-zero. */
    if(psMotorParams->fltLd > 0.0F)
    {
        g_sMID.sMotorParams.fltLd = psMotorParams->fltLd;
    }
    /* If Lq non-zero. */
    if(psMotorParams->fltLq > 0.0F)
    {
        g_sMID.sMotorParams.fltLq = psMotorParams->fltLq;
    }

    /* If Ke non-zero. */
    if(psMotorParams->fltKe > 0.0F)
    {
        g_sMID.sMotorParams.fltKe = psMotorParams->fltKe;
    }
    /* If J non-zero. */
    if(psMotorParams->fltJ > 0.0F)
    {
        g_sMID.sMotorParams.fltJ  = psMotorParams->fltJ;
    }
    /* If B non-zero. */
    if(psMotorParams->fltB > 0.0F)
    {
        g_sMID.sMotorParams.fltB  = psMotorParams->fltB;
    }
}

/*!
 * Function gets motor parameters (both measured and known).
 */
RAM_FUNC_LIB
void MID_GetMotorParams_BL(mid_motor_params_user_t *psMotorParams)
{
    /* Copy the parameters into the internal structure. */
    psMotorParams->ui32Pp = (uint32_t)g_sMID.sMotorParams.fltPp;
    psMotorParams->fltRs  = g_sMID.sMotorParams.fltRs;
    psMotorParams->fltLd  = g_sMID.sMotorParams.fltLd;
    psMotorParams->fltLq  = g_sMID.sMotorParams.fltLq;
    psMotorParams->fltKe  = g_sMID.sMotorParams.fltKe;
    psMotorParams->fltJ   = g_sMID.sMotorParams.fltJ;
    psMotorParams->fltB   = g_sMID.sMotorParams.fltB;
}

/*!
 * Function configures the upcoming measurement.
 */
RAM_FUNC_LIB
uint16_t MID_SetMeasConfig_BL(mid_config_t *psMeasConfig)
{
    uint16_t ui16RetVal = MID_CFG_SUCCESSFUL;

    /* Clear finished measurement flag. */
    g_sMID.ui32FinishedMeas = 0UL;   
    
    /* Init MID state machine to STOP. */
    MID_TransAll2Stop();

    /* Check range of the pole-pair parameter. */
    if((g_sMID.sMotorParams.fltPp > 20.0F) ||
       (g_sMID.sMotorParams.fltPp < 1.0F))
    {
        /* Report pole-pair parameter out of range. */
        ui16RetVal |= MID_PP_PARAM_OUT_OF_RANGE;

        /* Update the speed scale to default value. */
        g_sMID.fltSpeedAngScale = (60.0F / (2.0F * FLOAT_PI));
    }
    else
    {
        /* Set the pole-pair parameter in mechanical measurement configuration. */
        g_sMID.sMIDMech.fltPp = g_sMID.sMotorParams.fltPp;

        /* Update the speed scale (mechanical rpm -> electrical rad/s) */
        g_sMID.fltSpeedAngScale = (60.0F / (g_sMID.sMotorParams.fltPp * 2.0F * FLOAT_PI));
    }

    /* Align configuration. */
    /* Check range of the measurement configuration. */
    if((psMeasConfig->fltAlignId > MID_I_MEAS_MAX) ||
       (psMeasConfig->fltAlignId < 0.0F))
    {
        /* Report Alignment configuration out of range. */
        ui16RetVal |= MID_ALIGN_CFG_OUT_OF_RANGE;
    }
    else
    {
        /* Set the Align measurement configuration. */
        g_sMID.sMIDAlignment.fltCurrentAlign = psMeasConfig->fltAlignId;
        
        /* Set time of alignment */
        g_sMID.sMIDAlignment.ui16AlignDuration = 10000U;
    }
    
    /* Ke measurement configuration. */
    /* Check range of the measurement configuration. */
    if((psMeasConfig->fltKeIdReqOpenLoop > MID_I_MEAS_MAX) ||
       (psMeasConfig->fltKeIdReqOpenLoop < 0.0F)           ||
       (psMeasConfig->fltKeFreqElReq > MID_FREQ_MEAS_MAX)  ||
       (psMeasConfig->fltKeFreqElReq < 0.0F))
    {
        /* Report Ke parameter measurement configuration out of range. */
        ui16RetVal |= MID_KE_MEAS_CFG_OUT_OF_RANGE;
    }
    else
    {
        /* Set the Ke measurement configuration. */
        g_sMID.sMIDKe.fltIdReqOpenLoop = psMeasConfig->fltKeIdReqOpenLoop;
        g_sMID.sMIDKe.fltFreqElReq     = psMeasConfig->fltKeFreqElReq;
    }

    /* Pp measurement configuration. */
    /* Check range of the measurement configuration. */
    if((psMeasConfig->fltPpIdReqOpenLoop > MID_I_MEAS_MAX) ||
       (psMeasConfig->fltPpIdReqOpenLoop < 0.0F)           ||
       (psMeasConfig->fltPpFreqElReq > MID_FREQ_MEAS_MAX)  ||
       (psMeasConfig->fltPpFreqElReq < 0.0F))
    {
        /* Report Pp parameter measurement configuration out of range. */
        ui16RetVal |= MID_PP_MEAS_CFG_OUT_OF_RANGE;
    }
    else
    {
        /* Set the Pp measurement configuration. */
        g_sMID.sMIDPp.fltIdReqOpenLoop = psMeasConfig->fltPpIdReqOpenLoop;
        g_sMID.sMIDPp.fltFreqElReq     = psMeasConfig->fltPpFreqElReq;
    }

    /* Mech measurement configuration. */
    /* Check range of the measurement configuration. */
    if((psMeasConfig->fltMechKt < 0.0F)                       ||
       (psMeasConfig->fltMechIqStartup > MID_I_MEAS_MAX)      ||
       (psMeasConfig->fltMechIqStartup < 0.0F)                ||
       (psMeasConfig->fltMechMergingCoeff < 10.0F)            ||
       (psMeasConfig->fltMechMergingCoeff > 200.0F)           ||
       (psMeasConfig->fltMechIqAccelerate > MID_I_MEAS_MAX)   ||
       (psMeasConfig->fltMechIqAccelerate < 0.0F)             ||
       (psMeasConfig->fltMechIqDecelerate > MID_I_MEAS_MAX)   ||
       (psMeasConfig->fltMechIqDecelerate < 0.0F)             ||
       (psMeasConfig->fltMechSpeedThrsAccel > MID_N_MEAS_MAX) ||
       (psMeasConfig->fltMechSpeedThrsAccel < MID_N_MEAS_MIN) ||
       (psMeasConfig->fltMechSpeedThrsDecel > MID_N_MEAS_MAX) ||
       (psMeasConfig->fltMechSpeedThrsDecel < MID_N_MEAS_MIN) ||
       (psMeasConfig->fltMechSpeedThrsInteg > MID_N_MEAS_MAX) ||
       (psMeasConfig->fltMechSpeedThrsInteg < MID_N_MEAS_MIN))
    {
        /* Report Mech parameters out of range. */
        ui16RetVal |= MID_MECH_MEAS_CFG_OUT_OF_RANGE;
    }
    else
    {
        /* Set the Mech measurement configuration. */
        g_sMID.sMIDMech.fltKt               = psMeasConfig->fltMechKt;
        g_sMID.sMIDMech.fltIqStartup        = psMeasConfig->fltMechIqStartup;
        g_sMID.sMIDMech.fltMechMergingCoeff = psMeasConfig->fltMechMergingCoeff;
        g_sMID.sMIDMech.fltIqAccelerate     = psMeasConfig->fltMechIqAccelerate;
        g_sMID.sMIDMech.fltIqDecelerate     = psMeasConfig->fltMechIqDecelerate;
        g_sMID.sMIDMech.fltSpeedThrsAccel   = psMeasConfig->fltMechSpeedThrsAccel;
        g_sMID.sMIDMech.fltSpeedThrsDecel   = psMeasConfig->fltMechSpeedThrsDecel;
        g_sMID.sMIDMech.fltSpeedThrsInteg   = psMeasConfig->fltMechSpeedThrsInteg;
    }

    return ui16RetVal;
}

/*!
 * Function starts or stops MID.
 */
RAM_FUNC_LIB
void MID_Process_BL(mid_app_cmd_t *pMidCmd)
{
    /* Get the MID status and check whether the measurement is
    currently active. */
    if(TRUE == MID_GetStatus_BL(&sUserMIDStatus))
    {
        /* Check whether the measurement stop was requested. */
        if(kMID_Cmd_Stop == *pMidCmd)
        {
            /* Stop the identification. */
            MID_Stop_BL();
        }
    }
    else
    {
        /* Check whether the measurement start was requested. */
        if(kMID_Cmd_Run == *pMidCmd)
        {
             if (!FAULT_ANY(g_sMidDrive.sFaultIdCaptured))
             {
               ui16MeasConfigResult = MID_CFG_SUCCESSFUL;
                /* Update motor parameters in the MID. All valid
                   electrical and mechanical parameters wont be
                   identified again. */
                MID_SetKnownMotorParams_BL(&sUserMIDMotorParamsKnown);

                /* Set the measurement configuration. */
                ui16MeasConfigResult = MID_SetMeasConfig_BL(&sUserMIDMeasConfig);
                if(MID_CFG_SUCCESSFUL == ui16MeasConfigResult)
                {
                    /* Start/trigger the MID. */
                    ui32UserMIDStartResult = MID_Start_BL(eUserMIDMeasType);

                    /* Clear the start command. */
                    *pMidCmd = kMID_Cmd_Ready;   
                }
                else
                {
                    *pMidCmd = kMID_Cmd_Stop;
                }
             }
             else
               *pMidCmd = kMID_Cmd_Stop;
        }
        /* Check whether the measurements were completed. */
        else if(0UL < sUserMIDStatus.ui32FinishedMeas)
        {
            /* Get the measured motor parameters. */
            MID_GetMotorParams_BL(&sUserMIDMotorParamsMeas);

            /* Stop the measurement. */
            *pMidCmd = kMID_Cmd_Stop;
        }
        else{*pMidCmd = kMID_Cmd_Stop;}
    }
}

/*!
 * Function returns actual state of MID.
 */
RAM_FUNC_LIB
uint16_t MID_GetActualState(void)
{
  return ((uint16_t)g_sMID.eMIDState);
}

/*                             STATIC FUNCTIONS                               */
/*!
 * @brief Function calculated BEMF and Tracking observer params from g_sMID.sMotorParams
 *
 * @param sBemfParams - Bemf observer parameters.
 *        sTOParams - Tracking observer parameters.
 *
 * @return MID configuration result
 */
RAM_FUNC_LIB
static void MID_CalcBemfTrackingObsrv(mid_bemf_params_t *sBemfParams, mid_to_params_t *sTOParams)
{
    /* Calculate Bemf observer parameters:
        I_gain  = Ld/(Ld+Ts*Rs);
        U_gain  = Ts/(Ld+Ts*Rs);
        E_gain  = Ts/(Ld+Ts*Rs);
        WI_gain = Lq*Ts/(Ld+Ts*Rs);
        BemfDQ_Kpz = 2*BEMF_DQ_att*2*Math.PI*BEMF_DQ_f0*Ld-Rs
        BemfDQ_Kiz = Ld*Math.pow(2*Math.PI*BEMF_DQ_f0,2)*Ts
    */
    sBemfParams->fltI_gain  = g_sMID.sMotorParams.fltLd/(g_sMID.sMotorParams.fltLd+MID_FAST_LOOP_TS*g_sMID.sMotorParams.fltRs);
    sBemfParams->fltU_gain  = MID_FAST_LOOP_TS/(g_sMID.sMotorParams.fltLd+MID_FAST_LOOP_TS*g_sMID.sMotorParams.fltRs);
    sBemfParams->fltE_gain  = MID_FAST_LOOP_TS/(g_sMID.sMotorParams.fltLd+MID_FAST_LOOP_TS*g_sMID.sMotorParams.fltRs);
    sBemfParams->fltWI_gain = g_sMID.sMotorParams.fltLq*MID_FAST_LOOP_TS/(g_sMID.sMotorParams.fltLd+MID_FAST_LOOP_TS*g_sMID.sMotorParams.fltRs);
    sBemfParams->fltBemfDQ_Kps = MID_BEMF_COEFF_KPZ*g_sMID.sMotorParams.fltLd-g_sMID.sMotorParams.fltRs;
    sBemfParams->fltBemfDQ_Kis = g_sMID.sMotorParams.fltLd*MID_BEMF_COEFF_KIZ*MID_FAST_LOOP_TS;

    /* Calculate Tracking observer parameters:
        TO_Kpz = 2*TO_att*2*Math.PI*TO_f0
        TO_Kiz = Math.pow(2*Math.PI*TO_f0,2)*Ts
        TO_Theta = Ts/Math.PI
    */
    sTOParams->fltTO_Kpz = MID_TO_COEFF_KPZ;
    sTOParams->fltTO_Kiz = MID_TO_COEFF_KIZ*MID_FAST_LOOP_TS;
    sTOParams->fltTO_Theta = MID_TO_COEFF_THETA*MID_FAST_LOOP_TS;
}

/*!
 * @brief Clear FOC variables in global variable
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
static void MID_ClearFOCVariables(void)
{
    /* Clear FOC variables */
    g_sMidDrive.sFocPMSM.sIABC.fltA             = 0.0F;
    g_sMidDrive.sFocPMSM.sIABC.fltB             = 0.0F;
    g_sMidDrive.sFocPMSM.sIABC.fltC             = 0.0F;
    g_sMidDrive.sFocPMSM.sIAlBe.fltAlpha        = 0.0F;
    g_sMidDrive.sFocPMSM.sIAlBe.fltBeta         = 0.0F;
    g_sMidDrive.sFocPMSM.sIDQ.fltD              = 0.0F;
    g_sMidDrive.sFocPMSM.sIDQ.fltQ              = 0.0F;
    g_sMidDrive.sFocPMSM.sIDQReq.fltD           = 0.0F;
    g_sMidDrive.sFocPMSM.sIDQReq.fltQ           = 0.0F;
    g_sMidDrive.sFocPMSM.sIDQError.fltD         = 0.0F;
    g_sMidDrive.sFocPMSM.sIDQError.fltQ         = 0.0F;
    g_sMidDrive.sFocPMSM.sDutyABC.f16A          = FRAC16(0.5);
    g_sMidDrive.sFocPMSM.sDutyABC.f16B          = FRAC16(0.5);
    g_sMidDrive.sFocPMSM.sDutyABC.f16C          = FRAC16(0.5);
    g_sMidDrive.sFocPMSM.sUAlBeReq.fltAlpha     = 0.0F;
    g_sMidDrive.sFocPMSM.sUAlBeReq.fltBeta      = 0.0F;
    g_sMidDrive.sFocPMSM.sUDQReq.fltD           = 0.0F;
    g_sMidDrive.sFocPMSM.sUDQReq.fltQ           = 0.0F;
    g_sMidDrive.sFocPMSM.sAnglePosEl.fltSin     = 0.0F;
    g_sMidDrive.sFocPMSM.sAnglePosEl.fltCos     = 0.0F;
    g_sMidDrive.sFocPMSM.sAnglePosEl.fltSin     = 0.0F;
    g_sMidDrive.sFocPMSM.sAnglePosEl.fltCos     = 0.0F;
    g_sMidDrive.sFocPMSM.sIdPiParams.bLimFlag   = FALSE;
    g_sMidDrive.sFocPMSM.sIqPiParams.bLimFlag   = FALSE;
    g_sMidDrive.sFocPMSM.sIdPiParams.fltIAccK_1 = 0.0F;
    g_sMidDrive.sFocPMSM.sIdPiParams.fltIAccK_1 = 0.0F;
    g_sMidDrive.sFocPMSM.sIqPiParams.fltIAccK_1 = 0.0F;
    g_sMidDrive.sFocPMSM.sIqPiParams.fltIAccK_1 = 0.0F;
    GDFLIB_FilterIIR1Init_FLT(&g_sMidDrive.sFocPMSM.sSpeedElEstFilt);
    g_sMidDrive.sFocPMSM.bIdPiStopInteg = FALSE;
    g_sMidDrive.sFocPMSM.bIqPiStopInteg = FALSE;

    /* Clear BEMF and Tracking observers state variables */
    AMCLIB_PMSMBemfObsrvDQInit_A32fff(&g_sMidDrive.sFocPMSM.sBemfObsrv);
    AMCLIB_TrackObsrvInit_A32af(ACC32(0.0), &g_sMidDrive.sFocPMSM.sTo);
}

/*!
 * @brief Fault detention routine - check various faults
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
static void MID_FaultDetection(void)
{
    /* Clearing actual faults before detecting them again  */
    /* Clear all pending faults */
    FAULT_CLEAR_ALL(g_sMidDrive.sFaultIdPending);

    /* Clear fault captured manually if required. */
    if (g_sMidDrive.bFaultClearMan)
    {
        /* Clear fault captured */
        FAULT_CLEAR_ALL(g_sMidDrive.sFaultIdCaptured);
        g_sMidDrive.bFaultClearMan = FALSE;
    }

    /* Fault:   DC-bus over-current */
    if (M1_MCDRV_PWM3PH_FLT_GET(&g_sM1Pwm3ph))
        FAULT_SET(g_sMidDrive.sFaultIdPending, FAULT_I_DCBUS_OVER);

    /* Fault:   DC-bus over-voltage */
    if (g_sMidDrive.sFocPMSM.fltUDcBusFilt > g_sMidDrive.sFaultThresholds.fltUDcBusOver)
        FAULT_SET(g_sMidDrive.sFaultIdPending, FAULT_U_DCBUS_OVER);

    /* Fault:   DC-bus under-voltage */
    if (g_sMidDrive.sFocPMSM.fltUDcBusFilt < g_sMidDrive.sFaultThresholds.fltUDcBusUnder)
        FAULT_SET(g_sMidDrive.sFaultIdPending, FAULT_U_DCBUS_UNDER);

    /* Pass fault to Fault ID Captured */
    g_sMidDrive.sFaultIdCaptured |= g_sMidDrive.sFaultIdPending;
}
