/******************************************************************************
 *
 * Copyright 2010-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 *******************************************************************************
 * meterlibfft1ph_test.c                                                       * 
 ******************************************************************************/
#include "fraclib.h"           /* fractional library header */ 
#include "meterlibfft.h"       /* metering library header */   
#include "inputdata.h"         /* library of a typical harmonic signals */ 
#include "board.h"
#include "pin_mux.h"

/*******************************************************************************
 * General project settings                                                    * 
 ******************************************************************************/  
#define FFT_SAMPLES  SAMPLES32    /* number of required FFT samples (power-of-two) */   
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
   #define INP_SAMPLES 65       /* supposed maximum number of input samples (the worst case) */   
#else
   #define INP_SAMPLES FFT_SAMPLES /* accurate number of input samples (power-of-two) */   
#endif
#define AUX_SAMPLES FFT_SAMPLES/2  /* number of required auxiliary FFT variables in Polar form */

/*******************************************************************************
 * function prototypes                                                         *
 ******************************************************************************/    
unsigned long fill_buffers (frac24 *u, frac24 *i);

/*******************************************************************************
 * Buffers definitions                                                         *
 ******************************************************************************/  
/* multiplexed mandatory buffers (time domain / frequency domain in the Cartesian form) */
/* U-ADC output buffer/FFT real part output buffer */ 
frac24 u_re[FFT_SAMPLES>INP_SAMPLES?FFT_SAMPLES:INP_SAMPLES];   
/* I-ADC output buffer/FFT real part output buffer */   
frac24 i_re[FFT_SAMPLES>INP_SAMPLES?FFT_SAMPLES:INP_SAMPLES]; 

/* dedicated mandatory buffers (frequency domain in the Cartesian form) */
frac24 u_im[FFT_SAMPLES];     /* U-FFT imaginary part output buffer */   
frac24 i_im[FFT_SAMPLES];     /* I-FFT imaginary part output buffer */   

/* auxiliary buffers (frequency domain in the Polar form) */
frac24 i_mag[AUX_SAMPLES];    /* U-magnitudes output buffer */ 
frac24 u_mag[AUX_SAMPLES];    /* I-magnitudes output buffer */ 
long i_ph[AUX_SAMPLES];       /* U-phases output buffer */ 
long u_ph[AUX_SAMPLES];       /* I-phases output buffer */ 

/* auxiliary U-I phase shift buffer [0.001°] - used for S/W phase shift correction */
long shift[FFT_SAMPLES/2] = {0,-45000,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

/*******************************************************************************
 * Variables definitions                                                       *
 ******************************************************************************/  
unsigned long frequency = 50000;      /* line frequency [mHz] - should be measured finaly */
unsigned long adc_samples;            /* number of true ADC samples (it can be slightly changed  
                                         due to mains frequency varying in real application) */ 
long fcn_out;                         /* metering function output or function error state */
tMETERLIBFFT1PH_DATA ui;              /* 1-PH main metering structure */ 
unsigned long whi,whe,varhi,varhe;    /* billing (accumulative) metering values */
double urms,irms,p,q,s,pf,thdu,thdi;  /* non-billing (instantaneous) metering values */
long read_period = 0;                 /* reading period for non-billing values */
unsigned long tmr_cmp;                /* timer compare register value for energy LED driving */

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
   fcn_out = METERLIBFFT1PH_InitParam(&ui,FFT_SAMPLES, SENS_PROP, IMP5000, IMP5000, EN_RES10);
   METERLIBFFT1PH_InitMainBuff(&ui, u_re, i_re, u_im, i_im, shift);
   fcn_out = METERLIBFFT1PH_SetCalibCoeff(&ui, 325.27, 141.422, NULL, 0.0, 0.0);
   
   /* Energy registers can be zeroed or filled-up with some initial values */
   METERLIBFFT_SetEnergy(ui, 0, 0, 0, 0); 
   
   /* Auxiliary initialization section - for magnitudes and phase shifts computing only */
   METERLIBFFT1PH_InitAuxBuff(&ui, u_mag, i_mag, u_ph, i_ph);

   /* main loop */ 
   while(1)
   {
      /* ADC sampling simulation (function fills-up both U and I buffers) */  
      adc_samples = fill_buffers(u_re, i_re);  

#if AFE_MODE == 1             /* asynchronous or synchronous processing? */
      /* performs interpolation only for asynchronous processing */    
      fcn_out = METERLIBFFT1PH_Interpolation(&ui, ORD2, ORD2, adc_samples); 
#endif

      /* main calculation (FFT, I-signal conditioning, scaling, averaging) */
      METERLIBFFT1PH_CalcMain(&ui); 

      /* gets 16 FFT magnitudes (auxiliary output only) */
      METERLIBFFT1PH_GetMagnitudes(&ui, 16); 

      /* gets 16 FFT phases (auxiliary output only) */
      METERLIBFFT1PH_GetPhases(&ui, 16); 

      /* calculates active energy increment and threshold value for active energy timer */
      tmr_cmp = TMRCMPVAL(METERLIBFFT1PH_CalcWattHours(&ui, &whi, &whe, frequency),TMRCLK); 
      /* calculates reactive energy increment and threshold value for reactive energy timer */
      tmr_cmp = TMRCMPVAL(METERLIBFFT1PH_CalcVarHours(&ui, &varhi, &varhe, frequency),TMRCLK); 

      /* reads all non-billing instantaneous metering values */
      METERLIBFFT1PH_GetInstValues(&ui, &urms, &irms, &p, &q, &s, &pf, &thdu, &thdi); 
        
       /* reads all non-billing averaged metering values every 25-th cycle */
       if (++read_period == 25) 
       {   /* simulate asynchronous reading */
          METERLIBFFT1PH_GetAvrgValues(&ui, &urms, &irms, &p, &q, &s, &pf, &thdu, &thdi); 
          read_period = 0;
       }
    }
}  

/*******************************************************************************
* ADC sampling simulation function (not included in the real application)      *
*******************************************************************************/
unsigned long fill_buffers (frac24 *u, frac24 *i)
{
   unsigned long samp = INP_SAMPLES;/* integer or power-of two input samples */
   long *pu,*pi;                /* pointers to U and I data table */
    
   #if AFE_MODE == 1            /* Asynchronous S/W triggered continuous conversion mode */
       pu = sinus65_24bit;      /* set pointer to the beginning of U-LUT, 65 points */
       pi = sinus65_24bit_45deg;/* set pointer to the beginning of I-LUT,65 points,45° shift */    
   #else                        /* Synchronous H/W triggered single conversion mode */
       pu = sinus32_24bit;      /* set pointer to the beginning of U-LUT, 32 points */
       pi = sinus32_24bit_45deg;/* set pointer to the beginning of I-LUT,32 points,45° shift */
   #endif
   for (unsigned long cnt = 0; cnt < samp; cnt++) 
   {
      u[cnt] = (*pu++);        /* copy U-binary values from LUT to the U-output buffer */
      i[cnt] = (*pi++);        /* copy I-binary values from LUT to the I-output buffer */
   }    
   return (samp);
}

/*******************************************************************************
 * End of module                                                               *
 ******************************************************************************/  
