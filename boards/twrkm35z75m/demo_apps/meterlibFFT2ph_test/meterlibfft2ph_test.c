/******************************************************************************
 *
 * Copyright 2010-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 *******************************************************************************
 * meterlibfft2ph_test.c                                                       * 
 ******************************************************************************/
#include "fraclib.h"           /* fractional library header */ 
#include "meterlibfft.h"       /* metering library header */   
#include "inputdata.h"         /* library of a typical harmonic signals */  
#include "board.h"
#include "pin_mux.h"

/*******************************************************************************
 * General project settings                                                    * 
 ******************************************************************************/  
#define FFT_SAMPLES  SAMPLES64    /* number of required FFT samples (power-of-two) */   
#define AFE_MODE     0              
/* AFE_MODE=1 - asynchronous mode, sampling isn't synchronized with the mains, AFE is running in 
                the S/W-triggered continuous conversion mode
   AFE_MODE=0 - synchronous mode, sampling is synchronized with the mains, AFE is running in the 
                H/W-triggered single conversion mode */   
#define TMRCLK 2998272 /* timer clock base (MHz) */

/*******************************************************************************
 * Macro definitions                                                           * 
 ******************************************************************************/  
#define TMRCMPVAL(ledclk,tmrclk)  (((unsigned long)(tmrclk)*1000)/(unsigned long)(ledclk))
/* Computes timer compare value for energy LED driving:
   ledclk - active/reactive energy LED flashing frequency in the current line period [mHz] 
   tmrclk - timer clock base [Hz], should be < 4294967 Hz
*/

/*******************************************************************************
 * Conditioned settings                                                        *
 ******************************************************************************/  
#if AFE_MODE == 1
   #define INP_SAMPLES 120       /* supposed maximum number of input samples (the worst case) */   
#else
   #define INP_SAMPLES FFT_SAMPLES /* accurate number of input samples (power-of-two) */   
#endif
#define AUX_SAMPLES FFT_SAMPLES/2  /* number of required auxiliary FFT variables in Polar form */

/*******************************************************************************
 * Function prototypes                                                         *
 ******************************************************************************/    
unsigned long fill_buffers (frac24 *u, frac24 *i);

/*******************************************************************************
 * Buffers definitions                                                         *
 ******************************************************************************/  
/* multiplexed mandatory buffers (time domain / frequency domain in the Cartesian form) */
/* U-ADC output buffer/FFT real part output buffer (Ph1) */ 
frac24 u1_re[FFT_SAMPLES>INP_SAMPLES?FFT_SAMPLES:INP_SAMPLES];   
/* I-ADC output buffer/FFT real part output buffer (Ph1) */   
frac24 i1_re[FFT_SAMPLES>INP_SAMPLES?FFT_SAMPLES:INP_SAMPLES]; 
/* U-ADC output buffer/FFT real part output buffer (Ph2) */   
frac24 u2_re[FFT_SAMPLES>INP_SAMPLES?FFT_SAMPLES:INP_SAMPLES]; 
/* I-ADC output buffer/FFT real part output buffer (Ph2) */   
frac24 i2_re[FFT_SAMPLES>INP_SAMPLES?FFT_SAMPLES:INP_SAMPLES];

/* dedicated mandatory buffers (frequency domain in the Cartesian form) */
frac24 u1_im[FFT_SAMPLES];     /* U-FFT imaginary part output buffer (Ph1) */   
frac24 i1_im[FFT_SAMPLES];     /* I-FFT imaginary part output buffer (Ph1) */   
frac24 u2_im[FFT_SAMPLES];     /* U-FFT imaginary part output buffer (Ph2) */   
frac24 i2_im[FFT_SAMPLES];     /* I-FFT imaginary part output buffer (Ph2) */   

/* auxiliary buffers (frequency domain in the Polar form) */
frac24 i1_mag[AUX_SAMPLES];    /* U-magnitudes output buffer (Ph1) */ 
frac24 u1_mag[AUX_SAMPLES];    /* I-magnitudes output buffer (Ph1) */ 
frac24 i2_mag[AUX_SAMPLES];    /* U-magnitudes output buffer (Ph2) */ 
frac24 u2_mag[AUX_SAMPLES];    /* I-magnitudes output buffer (Ph2) */ 
long i1_ph[AUX_SAMPLES];       /* U-phases output buffer (Ph1) */ 
long u1_ph[AUX_SAMPLES];       /* I-phases output buffer (Ph1) */ 
long i2_ph[AUX_SAMPLES];       /* U-phases output buffer (Ph2) */ 
long u2_ph[AUX_SAMPLES];       /* I-phases output buffer (Ph2) */ 

/*******************************************************************************
 * Variables definitions                                                       *
 ******************************************************************************/  
unsigned long frequency = 60000;       /* line frequency [mHz] - should be measured finaly */
unsigned long adc_samples;             /* number of true ADC samples (it can be slightly changed  
                                          due to mains frequency varying in real application) */ 
long fcn_out;                          /* metering functions output or functions error state */
tMETERLIBFFT2PH_DATA ui;               /* 2-PH main metering structure */ 
double urms1,irms1,p1,q1,s1,pf1,thdu1,thdi1;/* non-billing (instantaneous) metering values Ph1*/
double urms2,irms2,p2,q2,s2,pf2,thdu2,thdi2;/* non-billing (instantaneous) metering values Ph2*/
unsigned long whi,whe,varhi,varhe;     /* billing (accumulative) metering values */
long read_period1 = 0;                 /* reading period for non-billing values Ph1 */
long read_period2 = 0;                 /* reading period for non-billing values Ph2 */
unsigned long tmr_cmp;                 /* timer compare register value for energy LED driving */

#if defined(__ICCARM__)
   #pragma diag_suppress=Pa082
#endif

/*******************************************************************************
 * Main                                                                        *
 ******************************************************************************/  
void main (void)
{
   BOARD_InitPins();
   BOARD_BootClockRUN();  
  
   /* Mandatory initialization section - for main FFT calculation */
   fcn_out = METERLIBFFT2PH_InitParam(&ui,FFT_SAMPLES, SENS_DERIV, IMP5000, IMP5000, EN_RES10);
   METERLIBFFT2PH_InitMainBuffPh1(&ui, u1_re, i1_re, u1_im, i1_im, NULL);
   METERLIBFFT2PH_InitMainBuffPh2(&ui, u2_re, i2_re, u2_im, i2_im, NULL);
   fcn_out = METERLIBFFT2PH_SetCalibCoeffPh1(&ui, 355.9, 191.0475, NULL, 0.0, 0.0);
   fcn_out = METERLIBFFT2PH_SetCalibCoeffPh2(&ui, 355.9, 191.0475, NULL, 0.0, 0.0);
  
   /* Energy registers can be zeroed or filled-up with some initial values */
   METERLIBFFT_SetEnergy(ui, 0, 0, 0, 0); 
  
   /* Auxiliary initialization section - for magnitudes and phase shifts computing only */
   METERLIBFFT2PH_InitAuxBuffPh1(&ui, u1_mag, i1_mag, u1_ph, i1_ph);
   METERLIBFFT2PH_InitAuxBuffPh2(&ui, u2_mag, i2_mag, u2_ph, i2_ph);

   /* main loop */
   while(1)
   { 
      /* ADC sampling simulation (function fills-up all U and I buffers) */  
      adc_samples = fill_buffers(u1_re, i1_re);/* phase 1 U-I buffers */  
      fill_buffers(u2_re, i2_re);              /* phase 2 U-I buffers */  

#if AFE_MODE == 1             /* asynchronous or synchronous processing? */
      /* performs interpolation only for asynchronous processing */    
      fcn_out = METERLIBFFT2PH_Interpolation(&ui, ORD2, ORD2, adc_samples); 
#endif

      /* main calculation (FFT, scaling, averaging) */
      METERLIBFFT2PH_CalcMain(&ui); 

      /* gets max. 16 FFT magnitudes for both lines (auxiliary output only) */
      METERLIBFFT2PH_GetMagnitudesPh1(&ui, 16); 
      METERLIBFFT2PH_GetMagnitudesPh2(&ui, 16); 

      /* gets max. 16 FFT phases for both lines (auxiliary output only) */
      METERLIBFFT2PH_GetPhasesPh1(&ui, 16); 
      METERLIBFFT2PH_GetPhasesPh2(&ui, 16); 

      /* calculates active energy increment and threshold value for active energy timer */
      tmr_cmp = TMRCMPVAL(METERLIBFFT2PH_CalcWattHours(&ui, &whi, &whe, frequency),TMRCLK); 
      /* calculates reactive energy increment and threshold value for reactive energy timer */
      tmr_cmp = TMRCMPVAL(METERLIBFFT2PH_CalcVarHours(&ui, &varhi, &varhe, frequency),TMRCLK); 

      /* reads all non-billing instantaneous metering values (both lines) */
      METERLIBFFT2PH_GetInstValuesPh1(&ui, &urms1, &irms1, &p1, &q1, &s1, &pf1, &thdu1, &thdi1); 
      METERLIBFFT2PH_GetInstValuesPh2(&ui, &urms2, &irms2, &p2, &q2, &s2, &pf2, &thdu2, &thdi2); 
    
      /* reads all non-billing averaged metering values of phase 1 every 25-th cycle */
      if (++read_period1 == 25) 
      { /* simulate asynchronous reading of Ph1 values */
         METERLIBFFT2PH_GetAvrgValuesPh1(&ui, &urms1, &irms1, &p1, &q1, &s1, &pf1, &thdu1, &thdi1); 
         read_period1 = 0;
      }

      /* reads all non-billing averaged metering values of phase 2 every 40-th cycle */
      if (++read_period2 == 40) 
      { /* simulate asynchronous reading of Ph2 values */
         METERLIBFFT2PH_GetAvrgValuesPh2(&ui, &urms2, &irms2, &p2, &q2, &s2, &pf2, &thdu2, &thdi2); 
         read_period2 = 0;
      }
   }
}  

/*******************************************************************************
* ADC sampling simulation function (not included in the real application)      *
*******************************************************************************/
unsigned long fill_buffers (frac24 *u, frac24 *i)
{
   unsigned long samp = INP_SAMPLES; /* integer or power-of two input samples */
   long *pu,*pi;                     /* pointers to U and I data table */
  
#if AFE_MODE == 1          /* Asynchronous S/W triggered continuous conversion mode */
   pu = sin_120s_4e6;      /* set pointer to the beginning of U-LUT, 120 points */
   pi = sin_120s_4e6_5h_40p_90deg;/* set pointer to the beginning of I-LUT,120 points,90° ph. */
#else                      /* Synchronous H/W triggered single conversion mode */
   pu = sin_64s_4e6;       /* set pointer to the beginning of U-LUT, 64 points */
   pi = sin_64s_4e6_5h_40p_90deg;/* set pointer to the beginning of I-LUT, 64 points, 90° ph. */
#endif
   for (unsigned long cnt = 0; cnt < samp; cnt++) 
   { 
      u[cnt] = (*pu++);    /* copy U-binary values from LUT to the U-output buffer */
      i[cnt] = (*pi++);    /* copy I-binary values from LUT to the I-output buffer */
   }    
   return (samp);
}
/*******************************************************************************
 * End of module                                                               *
 ******************************************************************************/  
