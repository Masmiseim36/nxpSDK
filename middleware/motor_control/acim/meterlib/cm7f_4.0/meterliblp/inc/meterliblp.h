/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      METERLIBLP.h
 * @author    R55013
 * @version   4.0.0.4
 * @date      May-29-2015
 * @brief     Header file containing common data types, macros and list of
 *            exported functions supporting metering specific calculations.
 ******************************************************************************/
#ifndef __METERLIBLP_H
#define __METERLIBLP_H

/******************************************************************************
 * user data type & macro definitions                                         *
 ******************************************************************************/
/***************************************************************************//*!
 * @brief   Return fine delay of the active energy pulse output transition
 *          scaled to the computation step (1/KWH_CALC_FREQ).
 * @param   p     Pointer to either tMETERLIBLP1PH_DATA, tMETERLIBLP2PH_DATA or
 *                tMETERLIBLP3PH_DATA structure.
 * @return  If raw pulse output state changes then this macro returns fine delay
 *          in range from 0x0000 to 0x7fff otherwise 0x8000. The raw pulse
 *          output state can be read by @ref METERLIBLP_KWH_PS macro.
 ******************************************************************************/
#define METERLIBLP_KWH_PD(p)    (frac16)(p)->wh.puDly

/***************************************************************************//*!
 * @brief   Return raw state of the active energy pulse output.
 * @param   p     Pointer to either tMETERLIBLP1PH_DATA, tMETERLIBLP2PH_DATA or
 *                tMETERLIBLP3PH_DATA structure.
 * @return  If raw pulse output state changes then @ref METERLIBLP_KWH_PD macro
 *          returns fine delay in range from 0x0000 to 0x7fff otherwise 0x8000.
 ******************************************************************************/
#define METERLIBLP_KWH_PS(p)    (int)(p)->wh.puOut

/***************************************************************************//*!
 * @brief   Return fine delay of the reactive energy pulse output transition
 *          scaled to the computation step (1/KVARH_CALC_FREQ).
 * @param   p     Pointer to either tMETERLIBLP1PH_DATA, tMETERLIBLP2PH_DATA or
 *                tMETERLIBLP3PH_DATA structure.
 * @return  If raw pulse output state changes then this macro returns fine delay
 *          in range from 0x0000 to 0x7fff otherwise 0x8000. The raw pulse
 *          output state can be read by @ref METERLIBLP_KVARH_PS macro.
 ******************************************************************************/
#define METERLIBLP_KVARH_PD(p)  (frac16)(p)->varh.puDly

/***************************************************************************//*!
 * @brief   Return raw state of the reactive energy pulse output.
 * @param   p     Pointer to either tMETERLIBLP1PH_DATA, tMETERLIBLP2PH_DATA or
 *                tMETERLIBLP3PH_DATA structure.
 * @return  If raw pulse output state changes then @ref METERLIBLP_KVARH_PD macro
 *          returns fine delay in range from 0x0000 to 0x7fff otherwise 0x8000.
 ******************************************************************************/
#define METERLIBLP_KVARH_PS(p)  (int)(p)->varh.puOut

/***************************************************************************//*!
 * @brief   Convert imp/kWh to pulse output resolution required by the metering
 *          library.
 * @param   x     imp/kWh
 * @see     @ref METERLIBLP_KVARH_PR
 ******************************************************************************/
#define METERLIBLP_KWH_PR(x)    FRAC32(((5e2/(x))/(U_MAX*I_MAX/3600/KWH_CALC_FREQ)))

/***************************************************************************//*!
 * @brief   Convert U-I phase shift in degrees to a 16-bit fractional number with
 *          resolution of fn*360/(32768*KWH_CALC_FREQ) degrees.
 * @param   x     U-I phase shift in degrees.
 * @param   fn    Nominal frequency in Hertz.
 * @see     @ref METERLIBLP1PH_ProcSamples, @ref METERLIBLP2PH_ProcSamples,
 *          @ref METERLIBLP3PH_ProcSamples.
 ******************************************************************************/
#define METERLIBLP_DEG2SH(x,fn) FRAC16((float)(x)*KWH_CALC_FREQ/((float)fn*360.0))

/***************************************************************************//*!
 * @brief   Convert U-I phase shift in radians to a 16-bit fractional number with
 *          resolution of fn*2*Pi/(32768*KWH_CALC_FREQ) radians.
 * @param   x     U-I phase shift in radians.
 * @param   fn    Nominal frequency in Hertz.
 * @see     @ref METERLIBLP1PH_ProcSamples, @ref METERLIBLP2PH_ProcSamples,
 *          @ref METERLIBLP3PH_ProcSamples.
 ******************************************************************************/
#define METERLIBLP_RAD2SH(x,fn)                                               \
          FRAC16((float)(x)*KWH_CALC_FREQ/((float)fn*2.0*3.14159265358979323846))

/***************************************************************************//*!
 * @brief   Convert imp/kVARh to pulse output resolution required by the metering
 *          library.
 * @param   x     imp/kVARh
 * @see     @ref METERLIBLP_KWH_PR
 ******************************************************************************/
#define METERLIBLP_KVARH_PR(x)  FRAC32(((5e2/(x))/(U_MAX*I_MAX/3600/KVARH_CALC_FREQ)))

#define METERLIBLP_KWH_DR(x)    FRAC32(((1e3/(x))/(U_MAX*I_MAX/3600/KWH_CALC_FREQ)))
#define METERLIBLP_KVARH_DR(x)  FRAC32(((1e3/(x))/(U_MAX*I_MAX/3600/KVARH_CALC_FREQ)))
#define MAXCOEF                 102 /*!< maximum number of coefficients       */

/*! Samples fifo buffer structure definition                                  */
typedef struct
{
  frac24    u[3];               /*!< buffer for phase voltage samples         */
  frac24    i[3];               /*!< buffer for phase current samples         */
} tLPFIFOBUFF_DATA;

/*! DC bias supressor structure definition                                    */
typedef struct
{
  frac16    cIIR[3];            /*!< filter coefficients                      */
} tLPBIASFILT_COEF;

typedef struct
{
  frac16    xIIR;               /*!< previous values of filter inputs         */
  frac32    yIIR;               /*!< filtered sample - dc bias removed        */
} tLPBIASFILT_DATA;

/*! Phase shifter filter structure definition                                 */
typedef struct
{
  uint16    lenFIR1;            /*!< length of Hilbert FIR filter             */
  frac16    cFIR1[MAXCOEF-1];   /*!< coefficients of the Hilbert FIR filter   */
  uint16    lenFIR2;            /*!< length of Group delay FIR filter         */
  frac16    cFIR2[MAXCOEF/2];   /*!< Group delay filter coefficients          */
} tLPPHSHFILT_COEF;

typedef struct
{
  frac16    xFIR1[MAXCOEF-1];   /*!< previous values of the filter inputs     */
  frac32    yFIR1;              /*!< FIR1 filter output value                 */
  frac16    xFIR2[MAXCOEF/2];   /*!< previous values of the filter inputs     */
  frac32    yFIR2;              /*!< FIR2 filter output value                 */
} tLPPHSHFILT_DATA;

/*! Phase sequence indicator structure definition                             */
typedef struct
{
  int       stPer;              /*!< startup period in calculation steps      */
  int       sgn[3];             /*!< signs of the phase voltage samples       */
  uint32    cnt[3];             /*!< counters of the phase voltage samples    */
} tLPPHSEQIND_DATA;

/*! Voltage RMS, current RMS, active power and reactive power averager        */
/* structure definition                                                       */
typedef struct
{
  frac16    cIIR1[3];           /*!< prefilter coefficients                   */
  frac16    cIIR2[3];           /*!< postfilter coefficients                  */
} tLPAVERFILT_COEF;

typedef struct
{
  frac32    xIIR1;              /*!< previous values of the prefilter input   */
  frac32    yIIR1;              /*!< output value of the prefilter            */
  frac16    xIIR2;              /*!< previous value of the postfilter input   */
  frac32    yIIR2;              /*!< output value of the postfilter           */
} tLPAVERFILT_DATA;

/*! Accumulative energy structure definition                                  */
typedef struct
{
  frac32    offs;       /*!< power offset due to channel crosstalk            */
  frac16    attn;       /*!< attenuation to equal energy calculation with RMS */
                        /*!< values and powers computed by chain of filters   */
  frac64    diRes;      /*!< resolution of energy display counter             */
  frac64    diRem[3];   /*!< reminder of energy display counter               */
  frac64    puRem;      /*!< reminder of energy pulse output                  */
  int       puOut;      /*!< current state of the raw pulse output            */
  frac16    puDly;      /*!< fine pulse delay generated by timer              */
} tLPENERGY_DATA;

/*! Imported/Exported energy counters structure definition.                   */
typedef struct
{
  uint32 im;            /*!< counter for imported energy                      */
  uint32 ex;            /*!< counter for exported energy                      */
} tCNT;

/*! Energy counters structure definition.                                     */
/*! - one phase meter functions updates: ph1                                  */
/*! - two phase meter functions updates: ph1 and ph2                          */
/*! - three phase meter functions updates: ph1, ph2 and ph3                   */
typedef struct
{
  tCNT ph1;             /*!< counter for imported/exported energy in phase 1  */
  tCNT ph2;             /*!< counter for imported/exported energy in phase 2  */
  tCNT ph3;             /*!< counter for imported/exported energy in phase 3  */
} tENERGY_CNT;

/*! Metering library data structure definition - 1PH METER                    */
typedef struct
{
  double            umax;     /*!< maximal voltage measurement range          */
  double            imax;     /*!< maximal current measurement range          */
  frac32            pThr;     /*!< power sensitivity threshold                */
  frac32            iThr;     /*!< starting current sensitivity threshold     */
  tLPFIFOBUFF_DATA  fifo;     /*!< ph1-samples fifo buffer                    */
  tLPBIASFILT_COEF  bias;     /*!< dc bias supressor filter coefficients      */
  tLPBIASFILT_COEF  intg;     /*!< integrator filter coefficients             */
  tLPBIASFILT_DATA  uDcb[2];  /*!< ph1-voltage dc bias supressor data         */
  tLPBIASFILT_DATA  intd;     /*!< ph1-current integrator filter data         */
  tLPBIASFILT_DATA  iDcb[2];  /*!< ph1-current dc bias supressor data         */
  tLPPHSHFILT_COEF  hilb;     /*!< Hilbert & delay filter coefficients        */
  tLPPHSHFILT_DATA  phSh;     /*!< ph1-Hilbert & delay filter data            */
  tLPAVERFILT_COEF  aver;     /*!< averager filter coefficients               */
  tLPAVERFILT_DATA  uRms;     /*!< ph1-voltage RMS averager data              */
  tLPAVERFILT_DATA  iRms;     /*!< ph1-current RMS averager data              */
  tLPAVERFILT_DATA  pAve;     /*!< ph1-active power averager data             */
  tLPAVERFILT_DATA  qAve;     /*!< ph1-reactive power averager data           */
  tLPENERGY_DATA    wh;       /*!< active energy                              */
  tLPENERGY_DATA    varh;     /*!< reactive energy                            */
} tMETERLIBLP1PH_DATA;

/*! Metering library data structure definition - 2PH METER                    */
typedef struct
{
  double            umax;     /*!< maximal voltage measurement range          */
  double            imax;     /*!< maximal current measurement range          */
  frac32            pThr;     /*!< power sensitivity threshold                */
  frac32            iThr;     /*!< starting current sensitivity threshold     */
  tLPFIFOBUFF_DATA  fifo1;    /*!< ph1-samples fifo buffer                    */
  tLPFIFOBUFF_DATA  fifo2;    /*!< ph2-samples fifo buffer                    */
  tLPBIASFILT_COEF  bias;     /*!< dc bias supressor filter coefficients      */
  tLPBIASFILT_COEF  intg;     /*!< integrator filter coefficients             */
  tLPBIASFILT_DATA  uDcb1[2]; /*!< ph1-voltage dc bias supressor data         */
  tLPBIASFILT_DATA  intd1;    /*!< ph1-current integrator filter data         */
  tLPBIASFILT_DATA  iDcb1[2]; /*!< ph1-current dc bias supressor data         */
  tLPBIASFILT_DATA  uDcb2[2]; /*!< ph2-voltage dc bias supressor data         */
  tLPBIASFILT_DATA  intd2;    /*!< ph2-current integrator filter data         */
  tLPBIASFILT_DATA  iDcb2[2]; /*!< ph2-current dc bias supressor data         */
  tLPPHSHFILT_COEF  hilb;     /*!< Hilbert & delay filter coefficients        */
  tLPPHSHFILT_DATA  phSh1;    /*!< ph1-Hilbert & delay filter data            */
  tLPPHSHFILT_DATA  phSh2;    /*!< ph2-Hilbert & delay filter data            */
  tLPAVERFILT_COEF  aver;     /*!< averager filter coefficients               */
  tLPAVERFILT_DATA  uRms1;    /*!< ph1-voltage RMS averager data              */
  tLPAVERFILT_DATA  uRms2;    /*!< ph2-voltage RMS averager data              */
  tLPAVERFILT_DATA  iRms1;    /*!< ph1-current RMS averager data              */
  tLPAVERFILT_DATA  iRms2;    /*!< ph2-current RMS averager data              */
  tLPAVERFILT_DATA  pAve1;    /*!< ph1-active power averager data             */
  tLPAVERFILT_DATA  pAve2;    /*!< ph2-active power averager data             */
  tLPAVERFILT_DATA  qAve1;    /*!< ph1-reactive power averager data           */
  tLPAVERFILT_DATA  qAve2;    /*!< ph2-reactive power averager data           */
  tLPENERGY_DATA    wh;       /*!< active energy                              */
  tLPENERGY_DATA    varh;     /*!< reactive energy                            */
} tMETERLIBLP2PH_DATA;

/*! Metering library data structure definition - 3PH METER                    */
typedef struct
{
  double            umax;     /*!< maximal voltage measurement range          */
  double            imax;     /*!< maximal current measurement range          */
  frac32            pThr;     /*!< power sensitivity threshold                */
  frac32            iThr;     /*!< starting current sensitivity threshold     */
  tLPFIFOBUFF_DATA  fifo1;    /*!< ph1-samples fifo buffer                    */
  tLPFIFOBUFF_DATA  fifo2;    /*!< ph2-samples fifo buffer                    */
  tLPFIFOBUFF_DATA  fifo3;    /*!< ph3-samples fifo buffer                    */
  tLPBIASFILT_COEF  bias;     /*!< dc bias supressor filter coefficients      */
  tLPBIASFILT_COEF  intg;     /*!< integrator filter coefficients             */
  tLPBIASFILT_DATA  uDcb1[2]; /*!< ph1-voltage dc bias supressor data         */
  tLPBIASFILT_DATA  intd1;    /*!< ph1-current integrator filter data         */
  tLPBIASFILT_DATA  iDcb1[2]; /*!< ph1-current dc bias supressor data         */
  tLPBIASFILT_DATA  uDcb2[2]; /*!< ph2-voltage dc bias supressor data         */
  tLPBIASFILT_DATA  intd2;    /*!< ph2-current integrator filter data         */
  tLPBIASFILT_DATA  iDcb2[2]; /*!< ph2-current dc bias supressor data         */
  tLPBIASFILT_DATA  uDcb3[2]; /*!< ph3-voltage dc bias supressor data         */
  tLPBIASFILT_DATA  intd3;    /*!< ph3-current integrator filter data         */
  tLPBIASFILT_DATA  iDcb3[2]; /*!< ph3-current dc bias supressor data         */
  tLPPHSHFILT_COEF  hilb;     /*!< Hilbert & delay filter coefficients        */
  tLPPHSHFILT_DATA  phSh1;    /*!< ph1-Hilbert & delay filter data            */
  tLPPHSHFILT_DATA  phSh2;    /*!< ph2-Hilbert & delay filter data            */
  tLPPHSHFILT_DATA  phSh3;    /*!< ph3-Hilbert & delay filter data            */
  tLPPHSEQIND_DATA  phSeq;    /*!< phase sequence indicator data              */
  tLPAVERFILT_COEF  aver;     /*!< averager filter coefficients               */
  tLPAVERFILT_DATA  uRms1;    /*!< ph1-voltage RMS averager data              */
  tLPAVERFILT_DATA  uRms2;    /*!< ph2-voltage RMS averager data              */
  tLPAVERFILT_DATA  uRms3;    /*!< ph3-voltage RMS averager data              */
  tLPAVERFILT_DATA  iRms1;    /*!< ph1-current RMS averager data              */
  tLPAVERFILT_DATA  iRms2;    /*!< ph2-current RMS averager data              */
  tLPAVERFILT_DATA  iRms3;    /*!< ph3-current RMS averager data              */
  tLPAVERFILT_DATA  pAve1;    /*!< ph1-active power averager data             */
  tLPAVERFILT_DATA  pAve2;    /*!< ph2-active power averager data             */
  tLPAVERFILT_DATA  pAve3;    /*!< ph3-active power averager data             */
  tLPAVERFILT_DATA  qAve1;    /*!< ph1-reactive power averager data           */
  tLPAVERFILT_DATA  qAve2;    /*!< ph2-reactive power averager data           */
  tLPAVERFILT_DATA  qAve3;    /*!< ph3-reactive power averager data           */
  tLPENERGY_DATA    wh;       /*!< active energy                              */
  tLPENERGY_DATA    varh;     /*!< reactive energy                            */
} tMETERLIBLP3PH_DATA;

/******************************************************************************
 * exported function prototypes                                               *
 ******************************************************************************/
/******************************************************************************
 * 1-PH METER API                                                             *
 ******************************************************************************/
/***************************************************************************//*!
 * @brief   Remove DC bias from phase voltage and phase current samples together
 *          with performing an optional sensor phase shift correction.
 * @param   p     Pointer to tMETERLIBLP1PH_DATA structure.
 * @param   u1Q   Instantaneous voltage sample in 24-bit data format.
 * @param   i1Q   Instantaneous current sample in 24-bit data format.
 * @param   shift Pointer to value for U-I phase shift correction. Set value in
 *                range -32768...32767 to phase shift the voltage with resolution
 *                of 1/(32768*KWH_CALC_FREQ) seconds. Use NULL pointer to disable
 *                software sensor phase shift correction.
 * @note    The @ref METERLIBLP1PH_ProcSamples function must be called at
 *          frequency defined by KWH_CALC_FREQ.
 ******************************************************************************/
extern void METERLIBLP1PH_ProcSamples (tMETERLIBLP1PH_DATA *p, frac24 u1Q,
                                       frac24 i1Q, frac16 *shift);

/***************************************************************************//*!
 * @brief   Recalculate active energy using new voltage and current samples.
 * @param   p     Pointer to tMETERLIBLP1PH_DATA structure.
 * @param   pCnt  Pointer to LCD counters representing active energy.
 * @param   puRes Resolution of pulse output (calculated by @ref METERLIBLP_KWH_PD)
 * @note    The @ref METERLIBLP1PH_CalcWattHours function must be called at
 *          frequency defined by KWH_CALC_FREQ.
 ******************************************************************************/
extern void METERLIBLP1PH_CalcWattHours (tMETERLIBLP1PH_DATA *p, tENERGY_CNT *pCnt,
                                       frac64 puRes);

/***************************************************************************//*!
 * @brief   Recalculate reactive energy.
 * @param   p     Pointer to tMETERLIBLP1PH_DATA structure.
 * @param   pCnt  Pointer to LCD counters representing reactive energy.
 * @param   puRes Resolution of pulse output (calculated by @ref METERLIBLP_KVARH_PD)
 * @note    The @ref METERLIBLP1PH_CalcVarHours function must be called at
 *          frequency defined by KVARH_CALC_FREQ.
 ******************************************************************************/
extern void METERLIBLP1PH_CalcVarHours  (tMETERLIBLP1PH_DATA *p, tENERGY_CNT *pCnt,
                                       frac64 puRes);

/***************************************************************************//*!
 * @brief   Recalculate URMS, IRMS, PAVG, QAVG and S auxiliary quantities.
 * @param   p     Pointer to tMETERLIBLP1PH_DATA structure.
 * @note    The @ref METERLIBLP1PH_CalcAuxiliary function must be called at the
 *          frequency defined by KWH_CALC_FREQ/DECIM_FACTOR.
 ******************************************************************************/
extern void METERLIBLP1PH_CalcAuxiliary (tMETERLIBLP1PH_DATA *p);

/***************************************************************************//*!
 * @brief   Recalculate URMS.
 * @param   p     Pointer to tMETERLIBLP1PH_DATA structure.
 * @note    The @ref METERLIBLP1PH_CalcURMS function must be called at the
 *          frequency defined by ratio KWH_CALC_FREQ/DECIM_FACTOR.
 ******************************************************************************/
extern void METERLIBLP1PH_CalcURMS (tMETERLIBLP1PH_DATA *p);

/***************************************************************************//*!
 * @brief   Recalculate IRMS.
 * @param   p     Pointer to tMETERLIBLP1PH_DATA structure.
 * @note    The @ref METERLIBLP1PH_CalcIRMS function must be called at the
 *          frequency defined by ratio KWH_CALC_FREQ/DECIM_FACTOR.
 ******************************************************************************/
extern void METERLIBLP1PH_CalcIRMS (tMETERLIBLP1PH_DATA *p);

/***************************************************************************//*!
 * @brief   Recalculate PAVG.
 * @param   p     Pointer to tMETERLIBLP1PH_DATA structure.
 * @note    The @ref METERLIBLP1PH_CalcPAVG function must be called at the
 *          frequency defined by ratio KWH_CALC_FREQ/DECIM_FACTOR.
 ******************************************************************************/
extern void METERLIBLP1PH_CalcPAVG (tMETERLIBLP1PH_DATA *p);

/***************************************************************************//*!
 * @brief   Read URMS, IRMS, PAVG, QAVG and S auxiliary quantities.
 * @param   p     Pointer to tMETERLIBLP1PH_DATA structure.
 * @param   urms  Pointer to phase voltage RMS value in Volts.
 * @param   irms  Pointer to phase current RMS value in Amperes.
 * @param   pavg  Pointer to active power in Watts.
 * @param   qavg  Pointer to reactive power in Volt-amperes reactive.
 * @param   s     Pointer to apparent power in Volt-amperes.
 * @note    The IRMS, PAVG, QAVG and S quantities are zeroed if either of powers
 *          is below the power sense threshold or rms current is below starting
 *          current threshold.
 ******************************************************************************/
extern void METERLIBLP1PH_ReadResults (tMETERLIBLP1PH_DATA *p, double *urms,
                                       double *irms, double *pavg, double *qavg,
                                       double *s);

/***************************************************************************//*!
 * @brief   Read URMS.
 * @param   p     Pointer to tMETERLIBLP1PH_DATA structure.
 * @param   urms  Pointer to phase voltage RMS value in Volts.
 ******************************************************************************/
extern void METERLIBLP1PH_ReadURMS (tMETERLIBLP1PH_DATA *p, double *urms);

/***************************************************************************//*!
 * @brief   Read IRMS.
 * @param   p     Pointer to tMETERLIBLP1PH_DATA structure.
 * @param   irms  Pointer to phase current RMS value in Amperes.
 * @note    The IRMS value is not zeroed below starting current threshold.
 ******************************************************************************/
extern void METERLIBLP1PH_ReadIRMS (tMETERLIBLP1PH_DATA *p, double *irms);

/***************************************************************************//*!
 * @brief   Read PAVG.
 * @param   p     Pointer to tMETERLIBLP1PH_DATA structure.
 * @param   pavg  Pointer to active power in Watts.
 * @note    The PAVG value is not zeroed below power sense threshold.
 ******************************************************************************/
extern void METERLIBLP1PH_ReadPAVG (tMETERLIBLP1PH_DATA *p, double *pavg);

/***************************************************************************//*!
 * @brief   Calculate and read S.
 * @param   p     Pointer to tMETERLIBLP1PH_DATA structure.
 * @param   s     Pointer to apparent power in Volt-amperes reactive.
 * @note    The @ref METERLIBLP1PH_ReadS function returns apparent power.
 *          The S value is not zeroed below power sense threshold.
 ******************************************************************************/
extern void METERLIBLP1PH_ReadS    (tMETERLIBLP1PH_DATA *p, double *s);

/******************************************************************************
 * 2-PH METER API                                                             *
 ******************************************************************************/
/***************************************************************************//*!
 * @brief   Remove DC bias from phase voltage and phase current samples together
 *          with performing an optional sensor phase shift correction.
 * @param   p     Pointer to tMETERLIBLP2PH_DATA structure.
 * @param   u1Q   Instantaneous voltage sample in 24-bit data format.
 * @param   i1Q   Instantaneous current sample in 24-bit data format.
 * @param   u2Q   Instantaneous voltage sample in 24-bit data format.
 * @param   i2Q   Instantaneous current sample in 24-bit data format.
 * @param   shift Pointer to two values for U-I phase shift correction. Set each
 *                value in range -32768...32767 to phase shift the voltage with
 *                resolution of 1/(32768*KWH_CALC_FREQ) seconds. Use NULL pointer
 *                to disable software sensor phase shift correction.
 * @note    The @ref METERLIBLP2PH_ProcSamples function must be called at
 *          frequency defined by KWH_CALC_FREQ.
 ******************************************************************************/
extern void METERLIBLP2PH_ProcSamples (tMETERLIBLP2PH_DATA *p, frac24 u1Q,
                                       frac24 i1Q, frac24 u2Q, frac24 i2Q,
                                       frac16 *shift);

/***************************************************************************//*!
 * @brief   Recalculate active energy using new voltage and current samples.
 * @param   p     Pointer to tMETERLIBLP2PH_DATA structure.
 * @param   pCnt  Pointer to LCD counters representing active energy.
 * @param   puRes Resolution of pulse output (calculated by @ref METERLIBLP_KWH_PD)
 * @note    The @ref METERLIBLP2PH_CalcWattHours function must be called at
 *          frequency defined by KWH_CALC_FREQ.
 ******************************************************************************/
extern void METERLIBLP2PH_CalcWattHours (tMETERLIBLP2PH_DATA *p, tENERGY_CNT *pCnt,
                                      frac64 puRes);

/***************************************************************************//*!
 * @brief   Recalculate reactive energy.
 * @param   p     Pointer to tMETERLIBLP2PH_DATA structure.
 * @param   pCnt  Pointer to LCD counters representing reactive energy.
 * @param   puRes Resolution of pulse output (calculated by @ref METERLIBLP_KVARH_PD)
 * @note    The @ref METERLIBLP2PH_CalcVarHours function must be called at
 *          frequency defined by KVARH_CALC_FREQ.
 ******************************************************************************/
extern void METERLIBLP2PH_CalcVarHours  (tMETERLIBLP2PH_DATA *p, tENERGY_CNT *pCnt,
                                       frac64 puRes);

/***************************************************************************//*!
 * @brief   Recalculate URMS, IRMS, PAVG, QAVG and S auxiliary quantities.
 * @param   p     Pointer to tMETERLIBLP2PH_DATA structure.
 * @note    The @ref METERLIBLP2PH_CalcAuxiliary function must be called at the
 *          frequency defined by ratio KWH_CALC_FREQ/DECIM_FACTOR.
 ******************************************************************************/
extern void METERLIBLP2PH_CalcAuxiliary (tMETERLIBLP2PH_DATA *p);

/***************************************************************************//*!
 * @brief   Recalculate URMS.
 * @param   p     Pointer to tMETERLIBLP2PH_DATA structure.
 * @note    The @ref METERLIBLP2PH_CalcURMS function must be called at the
 *          frequency defined by ratio KWH_CALC_FREQ/DECIM_FACTOR.
 ******************************************************************************/
extern void METERLIBLP2PH_CalcURMS (tMETERLIBLP2PH_DATA *p);

/***************************************************************************//*!
 * @brief   Recalculate IRMS.
 * @param   p     Pointer to tMETERLIBLP2PH_DATA structure.
 * @note    The @ref METERLIBLP2PH_CalcIRMS function must be called at the
 *          frequency defined by ratio KWH_CALC_FREQ/DECIM_FACTOR.
 ******************************************************************************/
extern void METERLIBLP2PH_CalcIRMS (tMETERLIBLP2PH_DATA *p);

/***************************************************************************//*!
 * @brief   Recalculate PAVG.
 * @param   p     Pointer to tMETERLIBLP2PH_DATA structure.
 * @note    The @ref METERLIBLP2PH_CalcPAVG function must be called at the
 *          frequency defined by ratio KWH_CALC_FREQ/DECIM_FACTOR.
 ******************************************************************************/
extern void METERLIBLP2PH_CalcPAVG (tMETERLIBLP2PH_DATA *p);

/***************************************************************************//*!
 * @brief   Read URMS, IRMS, PAVG, QAVG and S auxiliary quantities from phase 1.
 * @param   p     Pointer to tMETERLIBLP2PH_DATA structure.
 * @param   urms  Pointer to phase voltage RMS value in Volts.
 * @param   irms  Pointer to phase current RMS value in Amperes.
 * @param   pavg  Pointer to active power in Watts.
 * @param   qavg  Pointer to reactive power in Volt-amperes reactive.
 * @param   s     Pointer to apparent power in Volt-amperes.
 * @note    The IRMS, PAVG, QAVG and S quantities are zeroed if either of powers
 *          is below the power sense threshold or rms current is below starting
 *          current threshold.
 ******************************************************************************/
extern void METERLIBLP2PH_ReadResultsPh1 (tMETERLIBLP2PH_DATA *p, double *urms,
                                       double *irms, double *pavg, double *qavg,
                                       double *s);

/***************************************************************************//*!
 * @brief   Read URMS, IRMS, PAVG, QAVG and S auxiliary quantities from phase 2.
 * @param   p     Pointer to tMETERLIBLP2PH_DATA structure.
 * @param   urms  Pointer to phase voltage RMS value in Volts.
 * @param   irms  Pointer to phase current RMS value in Amperes.
 * @param   pavg  Pointer to active power in Watts.
 * @param   qavg  Pointer to reactive power in Volt-amperes reactive.
 * @param   s     Pointer to apparent power in Volt-amperes.
 * @note    The IRMS, PAVG, QAVG and S quantities are zeroed if either of powers
 *          is below the power sense threshold or rms current is below starting
 *          current threshold.
 ******************************************************************************/
extern void METERLIBLP2PH_ReadResultsPh2 (tMETERLIBLP2PH_DATA *p, double *urms,
                                       double *irms, double *pavg, double *qavg,
                                       double *s);

/***************************************************************************//*!
 * @brief   Read URMS.
 * @param   p     Pointer to tMETERLIBLP2PH_DATA structure.
 * @param   urms1 Pointer to phase 1 voltage RMS value in Volts.
 * @param   urms2 Pointer to phase 2 voltage RMS value in Volts.
 ******************************************************************************/
extern void METERLIBLP2PH_ReadURMS (tMETERLIBLP2PH_DATA *p, double *urms1,
                                  double *urms2);

/***************************************************************************//*!
 * @brief   Read IRMS.
 * @param   p     Pointer to tMETERLIBLP2PH_DATA structure.
 * @param   irms1 Pointer to phase 1 current RMS value in Amperes.
 * @param   irms2 Pointer to phase 2 current RMS value in Amperes.
 * @note    The IRMS values are not zeroed below starting current threshold.
 ******************************************************************************/
extern void METERLIBLP2PH_ReadIRMS (tMETERLIBLP2PH_DATA *p, double *irms1,
                                  double *irms2);

/***************************************************************************//*!
 * @brief   Read PAVG.
 * @param   p     Pointer to tMETERLIBLP2PH_DATA structure.
 * @param   pavg1 Pointer to phase 1 active power in Watts.
 * @param   pavg2 Pointer to phase 2 active power in Watts.
 * @note    The PAVG values are not zeroed below power sense threshold.
 ******************************************************************************/
extern void METERLIBLP2PH_ReadPAVG (tMETERLIBLP2PH_DATA *p, double *pavg1,
                                  double *pavg2);

/***************************************************************************//*!
 * @brief   Calculate and read S.
 * @param   p     Pointer to tMETERLIBLP2PH_DATA structure.
 * @param   s1    Pointer to phase 1 apparent power in Volt-amperes reactive.
 * @param   s2    Pointer to phase 2 apparent power in Volt-amperes reactive.
 * @note    The @ref METERLIBLP2PH_ReadS function returns apparent powers.
 *          The S values are not zeroed below power sense threshold.
 ******************************************************************************/
extern void METERLIBLP2PH_ReadS    (tMETERLIBLP2PH_DATA *p, double *s1, double *s2);

/******************************************************************************
 * 3-PH METER API                                                             *
 ******************************************************************************/
/***************************************************************************//*!
 * @brief   Remove DC bias from phase voltage and phase current samples together
 *          with determining the phase sequence and performing an optional sensor
 *          phase shift correction.
 * @param   p     Pointer to tMETERLIBLP3PH_DATA structure.
 * @param   u1Q   Instantaneous voltage sample in 24-bit data format.
 * @param   i1Q   Instantaneous current sample in 24-bit data format.
 * @param   u2Q   Instantaneous voltage sample in 24-bit data format.
 * @param   i2Q   Instantaneous current sample in 24-bit data format.
 * @param   u3Q   Instantaneous voltage sample in 24-bit data format.
 * @param   i3Q   Instantaneous current sample in 24-bit data format.
 * @param   shift Pointer to three values for U-I phase shift correction. Set each
 *                value in range -32768...32767 to phase shift the voltage with
 *                resolution of 1/(32768*KWH_CALC_FREQ) seconds. Use NULL pointer
 *                to disable software sensor phase shift correction.
 * @return   1    Phase sequence 1-2-3 (clockwise rotation).
 *           0    Phase sequence not detected.
 *          -1    Phase sequence 3-2-1 (counter-clockwise rotation).
 * @note    The @ref METERLIBLP3PH_ProcSamples function must be called at
 *          frequency defined by KWH_CALC_FREQ.
 ******************************************************************************/
extern int METERLIBLP3PH_ProcSamples (tMETERLIBLP3PH_DATA *p, frac24 u1Q,
                                      frac24 i1Q, frac24 u2Q, frac24 i2Q,
                                      frac24 u3Q, frac24 i3Q, frac16 *shift);

/***************************************************************************//*!
 * @brief   Recalculate active energy using new voltage and current samples.
 * @param   p     Pointer to tMETERLIBLP3PH_DATA structure.
 * @param   pCnt  Pointer to LCD counters representing active energy.
 * @param   puRes Resolution of pulse output (calculated by @ref METERLIBLP_KWH_PD)
 * @note    The @ref METERLIBLP3PH_CalcWattHours function must be called at
 *          frequency defined by KWH_CALC_FREQ.
 ******************************************************************************/
extern void METERLIBLP3PH_CalcWattHours (tMETERLIBLP3PH_DATA *p, tENERGY_CNT *pCnt,
                                      frac64 puRes);

/***************************************************************************//*!
 * @brief   Recalculate reactive energy.
 * @param   p     Pointer to tMETERLIBLP3PH_DATA structure.
 * @param   pCnt  Pointer to LCD counters representing reactive energy.
 * @param   puRes Resolution of pulse output (calculated by @ref METERLIBLP_KVARH_PD)
 * @note    The @ref METERLIBLP3PH_CalcVarHours function must be called at
 *          frequency defined by KVARH_CALC_FREQ.
 ******************************************************************************/
extern void METERLIBLP3PH_CalcVarHours  (tMETERLIBLP3PH_DATA *p, tENERGY_CNT *pCnt,
                                      frac64 puRes);

/***************************************************************************//*!
 * @brief   Recalculate URMS, IRMS, PAVG, QAVG and S auxiliary quantities.
 * @param   p     Pointer to tMETERLIBLP3PH_DATA structure.
 * @note    The @ref METERLIBLP3PH_CalcAuxiliary function must be called at the
 *          frequency defined by ratio KWH_CALC_FREQ/DECIM_FACTOR.
 ******************************************************************************/
extern void METERLIBLP3PH_CalcAuxiliary (tMETERLIBLP3PH_DATA *p);

/***************************************************************************//*!
 * @brief   Recalculate URMS.
 * @param   p     Pointer to tMETERLIBLP3PH_DATA structure.
 * @note    The @ref METERLIBLP3PH_CalcURMS function must be called at the
 *          frequency defined by ratio KWH_CALC_FREQ/DECIM_FACTOR.
 ******************************************************************************/
extern void METERLIBLP3PH_CalcURMS (tMETERLIBLP3PH_DATA *p);

/***************************************************************************//*!
 * @brief   Recalculate IRMS.
 * @param   p     Pointer to tMETERLIBLP3PH_DATA structure.
 * @note    The @ref METERLIBLP3PH_CalcIRMS function must be called at the
 *          frequency defined by ratio KWH_CALC_FREQ/DECIM_FACTOR.
 ******************************************************************************/
extern void METERLIBLP3PH_CalcIRMS (tMETERLIBLP3PH_DATA *p);

/***************************************************************************//*!
 * @brief   Recalculate PAVG.
 * @param   p     Pointer to tMETERLIBLP3PH_DATA structure.
 * @note    The @ref METERLIBLP3PH_CalcPAVG function must be called at the
 *          frequency defined by ratio KWH_CALC_FREQ/DECIM_FACTOR.
 ******************************************************************************/
extern void METERLIBLP3PH_CalcPAVG (tMETERLIBLP3PH_DATA *p);

/***************************************************************************//*!
 * @brief   Read URMS, IRMS, PAVG, QAVG and S auxiliary quantities from phase 1.
 * @param   p     Pointer to tMETERLIBLP3PH_DATA structure.
 * @param   urms  Pointer to phase voltage RMS value in Volts.
 * @param   irms  Pointer to phase current RMS value in Amperes.
 * @param   pavg  Pointer to active power in Watts.
 * @param   qavg  Pointer to reactive power in Volt-amperes reactive.
 * @param   s     Pointer to apparent power in Volt-amperes.
 * @note    The IRMS, PAVG, QAVG and S quantities are zeroed if either of powers
 *          is below the power sense threshold or rms current is below starting
 *          current threshold.
 ******************************************************************************/
extern void METERLIBLP3PH_ReadResultsPh1 (tMETERLIBLP3PH_DATA *p, double *urms,
                                       double *irms, double *pavg, double *qavg,
                                       double *s);

/***************************************************************************//*!
 * @brief   Read URMS, IRMS, PAVG, QAVG and S auxiliary quantities from phase 2.
 * @param   p     Pointer to tMETERLIBLP3PH_DATA structure.
 * @param   urms  Pointer to phase voltage RMS value in Volts.
 * @param   irms  Pointer to phase current RMS value in Amperes.
 * @param   pavg  Pointer to active power in Watts.
 * @param   qavg  Pointer to reactive power in Volt-amperes reactive.
 * @param   s     Pointer to apparent power in Volt-amperes.
 * @note    The IRMS, PAVG, QAVG and S quantities are zeroed if either of powers
 *          is below the power sense threshold or rms current is below starting
 *          current threshold.
 ******************************************************************************/
extern void METERLIBLP3PH_ReadResultsPh2 (tMETERLIBLP3PH_DATA *p, double *urms,
                                       double *irms, double *pavg, double *qavg,
                                       double *s);

/***************************************************************************//*!
 * @brief   Read URMS, IRMS, PAVG, QAVG and S auxiliary quantities from phase 3.
 * @param   p     Pointer to tMETERLIBLP3PH_DATA structure.
 * @param   urms  Pointer to phase voltage RMS value in Volts.
 * @param   irms  Pointer to phase current RMS value in Amperes.
 * @param   pavg  Pointer to active power in Watts.
 * @param   qavg  Pointer to reactive power in Volt-amperes reactive.
 * @param   s     Pointer to apparent power in Volt-amperes.
 * @note    The IRMS, PAVG, QAVG and S quantities are zeroed if either of powers
 *          is below the power sense threshold or rms current is below starting
 *          current threshold.
 ******************************************************************************/
extern void METERLIBLP3PH_ReadResultsPh3 (tMETERLIBLP3PH_DATA *p, double *urms,
                                       double *irms, double *pavg, double *qavg,
                                       double *s);

/***************************************************************************//*!
 * @brief   Read URMS.
 * @param   p     Pointer to tMETERLIBLP3PH_DATA structure.
 * @param   urms1 Pointer to phase 1 voltage RMS value in Volts.
 * @param   urms2 Pointer to phase 2 voltage RMS value in Volts.
 * @param   urms3 Pointer to phase 3 voltage RMS value in Volts.
 ******************************************************************************/
extern void METERLIBLP3PH_ReadURMS (tMETERLIBLP3PH_DATA *p, double *urms1,
                                  double *urms2, double *urms3);

/***************************************************************************//*!
 * @brief   Read IRMS.
 * @param   p     Pointer to tMETERLIBLP3PH_DATA structure.
 * @param   irms1 Pointer to phase 1 current RMS value in Amperes.
 * @param   irms2 Pointer to phase 2 current RMS value in Amperes.
 * @param   irms3 Pointer to phase 3 current RMS value in Amperes.
 * @note    The IRMS values are not zeroed below starting current threshold.
 ******************************************************************************/
extern void METERLIBLP3PH_ReadIRMS (tMETERLIBLP3PH_DATA *p, double *irms1,
                                  double *irms2, double *irms3);

/***************************************************************************//*!
 * @brief   Read PAVG.
 * @param   p     Pointer to tMETERLIBLP3PH_DATA structure.
 * @param   pavg1 Pointer to phase 1 active power in Watts.
 * @param   pavg2 Pointer to phase 2 active power in Watts.
 * @param   pavg3 Pointer to phase 3 active power in Watts.
 * @note    The PAVG values are not zeroed below power sense threshold.
 ******************************************************************************/
extern void METERLIBLP3PH_ReadPAVG (tMETERLIBLP3PH_DATA *p, double *pavg1,
                                  double *pavg2, double *pavg3);

/***************************************************************************//*!
 * @brief   Calculate and read S.
 * @param   p     Pointer to tMETERLIBLP3PH_DATA structure.
 * @param   s1    Pointer to phase 1 apparent power in Volt-amperes reactive.
 * @param   s2    Pointer to phase 2 apparent power in Volt-amperes reactive.
 * @param   s3    Pointer to phase 3 apparent power in Volt-amperes reactive.
 * @note    The @ref METERLIBLP3PH_ReadS function returns apparent powers.
 *          The S values are not zeroed below power sense threshold.
 ******************************************************************************/
extern void METERLIBLP3PH_ReadS    (tMETERLIBLP3PH_DATA *p, double *s1, double *s2,
                                  double *s3);

/***************************************************************************//*!
 * @example   meterliblp1ph_test.c
 * Example I. This example demonstrates use of the low power metering library
 * in typical one-phase power meter application.
 ******************************************************************************/
/***************************************************************************//*!
 * @example   meterliblp2ph_test.c
 * Example II. This example demonstrates use of the low power metering library
 * in typical two-phase power meter application.
 ******************************************************************************/
/***************************************************************************//*!
 * @example   meterliblp3ph_test.c
 * Example III. This example demonstrates use of the low power metering library
 * in typical polyphase power meter application.
 ******************************************************************************/
/***************************************************************************//*!
 * @example   meterliblp1phkwh_test.c
 * Example IV. This example demonstrates use of the low power metering library
 * in active energy one-phase power meter application.
 ******************************************************************************/
/***************************************************************************//*!
 * @example   meterliblp2phkwh_test.c
 * Example V. This example demonstrates use of the low power metering library
 * in active energy two-phase power meter application.
 ******************************************************************************/
/***************************************************************************//*!
 * @example   meterliblp3phkwh_test.c
 * Example VI. This example demonstrates use of the low power metering library
 * in active energy polyphase power meter application.
 ******************************************************************************/
#endif /* __METERLIBLP_H */
