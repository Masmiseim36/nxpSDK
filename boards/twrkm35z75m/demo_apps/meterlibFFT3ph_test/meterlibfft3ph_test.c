/******************************************************************************
 *
 * Copyright 2010-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 *******************************************************************************
 * meterlibfft3ph_test.c                                                       * 
 ******************************************************************************/
#include "fraclib.h"           /* fractional library header */ 
#include "meterlibfft.h"       /* metering library header */   
#include "inputdata.h"         /* library of a typical harmonic signals */ 
#include "pin_mux.h"
#include "board.h"

/*******************************************************************************
 * General project settings                                                    * 
 ******************************************************************************/  
#define FFT_SAMPLES  SAMPLES64    /* number of required FFT samples (power-of-two) */   
#define AFE_MODE     1             
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
   #define INP_SAMPLES 120         /* supposed maximum number of input samples (the worst case) */   
#else
   #define INP_SAMPLES FFT_SAMPLES /* accurate number of input samples (power-of-two) */   
#endif
#define AUX_SAMPLES FFT_SAMPLES/2  /* number of required auxiliary FFT variables in Polar form */

/*******************************************************************************
 * function prototypes                                                         *
 ******************************************************************************/    
unsigned long fill_buffers (frac24 *u, frac24 *i, uint8 line);

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
/* U-ADC output buffer/FFT real part output buffer (Ph3) */   
frac24 u3_re[FFT_SAMPLES>INP_SAMPLES?FFT_SAMPLES:INP_SAMPLES];    
/* I-ADC output buffer/FFT real part output buffer (Ph3) */   
frac24 i3_re[FFT_SAMPLES>INP_SAMPLES?FFT_SAMPLES:INP_SAMPLES];     

/* dedicated mandatory buffers (frequency domain in the Cartesian form) */
frac24 u1_im[FFT_SAMPLES];     /* U-FFT imaginary part output buffer (Ph1) */   
frac24 i1_im[FFT_SAMPLES];     /* I-FFT imaginary part output buffer (Ph1) */   
frac24 u2_im[FFT_SAMPLES];     /* U-FFT imaginary part output buffer (Ph2) */   
frac24 i2_im[FFT_SAMPLES];     /* I-FFT imaginary part output buffer (Ph2) */   
frac24 u3_im[FFT_SAMPLES];     /* U-FFT imaginary part output buffer (Ph3) */   
frac24 i3_im[FFT_SAMPLES];     /* I-FFT imaginary part output buffer (Ph3) */   

/* auxiliary buffers (frequency domain in the Polar form) */
frac24 i1_mag[AUX_SAMPLES];    /* U-magnitudes output buffer (Ph1) */ 
frac24 u1_mag[AUX_SAMPLES];    /* I-magnitudes output buffer (Ph1) */ 
frac24 i2_mag[AUX_SAMPLES];    /* U-magnitudes output buffer (Ph2) */ 
frac24 u2_mag[AUX_SAMPLES];    /* I-magnitudes output buffer (Ph2) */ 
frac24 i3_mag[AUX_SAMPLES];    /* U-magnitudes output buffer (Ph3) */ 
frac24 u3_mag[AUX_SAMPLES];    /* I-magnitudes output buffer (Ph3) */ 
long i1_ph[AUX_SAMPLES];       /* U-phases output buffer (Ph1) */ 
long u1_ph[AUX_SAMPLES];       /* I-phases output buffer (Ph1) */ 
long i2_ph[AUX_SAMPLES];       /* U-phases output buffer (Ph2) */ 
long u2_ph[AUX_SAMPLES];       /* I-phases output buffer (Ph2) */ 
long i3_ph[AUX_SAMPLES];       /* U-phases output buffer (Ph3) */ 
long u3_ph[AUX_SAMPLES];       /* I-phases output buffer (Ph3) */ 

/* auxiliary U-I phase shift buffers [0.001°] - used for S/W phase shift correction (Ph1..Ph3) */
long shift1[FFT_SAMPLES/2] = {0,-5300,0,0,0,-4500,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
long shift2[FFT_SAMPLES/2] = {0};
long shift3[FFT_SAMPLES/2] = {0,-90000,0,0,0,-90000,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

/*******************************************************************************
 * Variables definitions                                                       *
 ******************************************************************************/  
unsigned long frequency = 50000;       /* line frequency [mHz] - should be measured finaly */
unsigned long adc_samples;             /* number of true ADC samples (it can be slightly changed  
                                          due to mains frequency varying in real application) */ 
long fcn_out;                          /* metering functions output or functions error state */
tMETERLIBFFT3PH_DATA ui;               /* 3-PH main metering structure */ 
double urms1,irms1,p1,q1,s1,pf1,thdu1,thdi1;/* non-billing (instantaneous) metering values Ph1*/
double urms2,irms2,p2,q2,s2,pf2,thdu2,thdi2;/* non-billing (instantaneous) metering values Ph2*/
double urms3,irms3,p3,q3,s3,pf3,thdu3,thdi3;/* non-billing (instantaneous) metering values Ph3*/
double u12_ph, u13_ph, u23_ph;        /* phase shifts */
unsigned long whi,whe,varhi,varhe;    /* billing (accumulative) metering values */
long read_period = 0;                 /* reading period for non-billing averaged values */
unsigned long tmr_cmp;                /* timer compare register value for energy LED driving */
long seq;                             /* sense of rotation */
frac24 afe1_offs = 600000;            /* AFE1 I-offset correction value [bit] */
frac24 afe2_offs = 0;                 /* AFE2 I-offset correction value [bit] */
frac24 afe3_offs = 0;                 /* AFE3 I-offset correction value [bit] */

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

   /* Mandatory initialization section - for main FFT calculation             */
   fcn_out = METERLIBFFT3PH_InitParam(&ui,FFT_SAMPLES, SENS_PROP, IMP2000, IMP2000, EN_RES10);
   METERLIBFFT3PH_InitMainBuffPh1(&ui, u1_re, i1_re, u1_im, i1_im, shift1);
   METERLIBFFT3PH_InitMainBuffPh2(&ui, u2_re, i2_re, u2_im, i2_im, shift2);
   METERLIBFFT3PH_InitMainBuffPh3(&ui, u3_re, i3_re, u3_im, i3_im, shift3);
   fcn_out = METERLIBFFT3PH_SetCalibCoeffPh1(&ui, 355.9, 191.0475, &afe1_offs, 0.0, 0.0);
   fcn_out = METERLIBFFT3PH_SetCalibCoeffPh2(&ui, 355.9, 191.0475, &afe2_offs, 0.0, 0.0);
   fcn_out = METERLIBFFT3PH_SetCalibCoeffPh3(&ui, 355.9, 191.0475, &afe3_offs, 0.0, 0.0);
  
   /* Energy registers can be zeroed or filled-up with some initial values    */
   METERLIBFFT_SetEnergy(ui, 0, 0, 0, 0); 
  
   /* Auxiliary initialization - for magnitudes and phase shifts computing only */
   METERLIBFFT3PH_InitAuxBuffPh1(&ui, u1_mag, i1_mag, u1_ph, i1_ph);
   METERLIBFFT3PH_InitAuxBuffPh2(&ui, u2_mag, i2_mag, u2_ph, i2_ph);
   METERLIBFFT3PH_InitAuxBuffPh3(&ui, u3_mag, i3_mag, u3_ph, i3_ph);

   /* main loop */
   while(1)
   {
      /* ADC sampling simulation (function fills-up all U and I buffers)      */  
      adc_samples = fill_buffers(u1_re, i1_re,1);      /* phase 1 U-I buffers */  
      fill_buffers(u2_re, i2_re,2);                    /* phase 2 U-I buffers */
      fill_buffers(u3_re, i3_re,3);                    /* phase 3 U-I buffers */
    
#if AFE_MODE == 1                  /* asynchronous or synchronous processing? */
      /* performs interpolation only for asynchronous processing */    
      fcn_out = METERLIBFFT3PH_Interpolation(&ui, ORD2, ORD3, adc_samples); 
#endif

      /* main calculation (FFT, scaling, averaging) */
      METERLIBFFT3PH_CalcMain(&ui); 

      /* gets max. 32 FFT magnitudes for all lines (auxiliary output only) */
      METERLIBFFT3PH_GetMagnitudesPh1(&ui, AUX_SAMPLES); 
      METERLIBFFT3PH_GetMagnitudesPh2(&ui, AUX_SAMPLES); 
      METERLIBFFT3PH_GetMagnitudesPh3(&ui, AUX_SAMPLES); 

      /* gets max. 32 FFT phases for all lines (auxiliary output only) */
      METERLIBFFT3PH_GetPhasesPh1(&ui, AUX_SAMPLES); 
      METERLIBFFT3PH_GetPhasesPh2(&ui, AUX_SAMPLES); 
      METERLIBFFT3PH_GetPhasesPh3(&ui, AUX_SAMPLES); 

      /* sense of rotation measurement */ 
      seq = METERLIBFFT3PH_GetRotation(&ui, &u12_ph, &u13_ph, &u23_ph);

      /* calculates active energy increment and threshold value for active energy timer */
      tmr_cmp = TMRCMPVAL(METERLIBFFT3PH_CalcWattHours(&ui, &whi, &whe, frequency),TMRCLK); 
      /* calculates reactive energy increment and threshold value for reactive energy timer */
      tmr_cmp = TMRCMPVAL(METERLIBFFT3PH_CalcVarHours(&ui, &varhi, &varhe, frequency),TMRCLK); 

      /* reads all non-billing instantaneous metering values (all lines) */
      METERLIBFFT3PH_GetInstValuesPh1(&ui, &urms1, &irms1, &p1, &q1, &s1, &pf1, &thdu1, &thdi1); 
      METERLIBFFT3PH_GetInstValuesPh2(&ui, &urms2, &irms2, &p2, &q2, &s2, &pf2, &thdu2, &thdi2); 
      METERLIBFFT3PH_GetInstValuesPh3(&ui, &urms3, &irms3, &p3, &q3, &s3, &pf3, &thdu3, &thdi3); 

      /* reads all non-billing averaged metering values every 25-th cycle */
      if (++read_period == 25) 
      { /* simulate asynchronous reading of all averaged values */
         METERLIBFFT3PH_GetAvrgValuesPh1(&ui, &urms1, &irms1, &p1, &q1, &s1, &pf1, &thdu1, &thdi1); 
         METERLIBFFT3PH_GetAvrgValuesPh2(&ui, &urms2, &irms2, &p2, &q2, &s2, &pf2, &thdu2, &thdi2); 
         METERLIBFFT3PH_GetAvrgValuesPh3(&ui, &urms3, &irms3, &p3, &q3, &s3, &pf3, &thdu3, &thdi3); 
         read_period = 0;
      }
   }
}  
/*******************************************************************************
* ADC sampling simulation function (not included in the real application)      *
*******************************************************************************/
unsigned long fill_buffers (frac24 *u, frac24 *i, uint8 line)
{
   unsigned long samp = INP_SAMPLES; /* integer or power-of two input samples */
   long *pu,*pi;                            /* pointers to U and I data table */
  
   /* Voltage and current vectors assigning according to the: phase number,
      conversion mode and rotation parameter */

   if  (line == 1)                     /* Line 1 */
   {
#if AFE_MODE == 1    /* Asynchronous S/W triggered continuous conversion mode */
      pu = sin_120s_6e6_5h_10p;        /* U-buffer, 120-points */
      pi = sin_120s_4e6_5h_40p_sh_offs;/* I-buffer, 120-points, shifted */
#else                     /* Synchronous H/W triggered single conversion mode */
      pu = sin_64s_6e6_5h_10p;         /* U-buffer, 64-points */
      pi = sin_64s_4e6_5h_40p_sh_offs; /* I-buffer, 64-points, shifted */
#endif
   }  

   else if  (line == 2)                 /* Line 2 */
   {
#if AFE_MODE == 1    /* Asynchronous S/W triggered continuous conversion mode */
      pu = sin_120s_6e6_5h_10p_121deg;  /* U-buffer, 120-points */
      pi = sin_120s_1e6_5h_10p;         /* I-buffer, 120-points */
#else                     /* Synchronous H/W triggered single conversion mode */
      pu = sin_64s_6e6_5h_10p_121deg;   /* U-buffer, 64-points */
      pi = sin_64s_1e6_5h_10p;          /* I-buffer, 64-points */
#endif
   }
   
   else                                 /* Line 3 */
   {
#if AFE_MODE == 1    /* Asynchronous S/W triggered continuous conversion mode */
      pu = sin_120s_6e6_5h_10p_239deg;  /* U-buffer, 120-points */
      pi = cos_120s_4e6_5h_40p_90deg2;  /* I-buffer, 120-points */
#else                     /* Synchronous H/W triggered single conversion mode */
      pu = sin_64s_6e6_5h_10p_239deg;   /* U-buffer, 64-points */
      pi = cos_64s_4e6_5h_40p_90deg2;   /* I-buffer, 64-points */
#endif
   }      
   for (unsigned long cnt = 0; cnt < samp; cnt++) 
   {
      u[cnt] = (*pu++); /* copy U-binary values from LUT to the U-output buffer */
      i[cnt] = (*pi++); /* copy I-binary values from LUT to the I-output buffer */
   }    
   return (samp);  /* returns the right number of input samples per one period */
}
/*******************************************************************************
 * End of module                                                               *
 ******************************************************************************/  
