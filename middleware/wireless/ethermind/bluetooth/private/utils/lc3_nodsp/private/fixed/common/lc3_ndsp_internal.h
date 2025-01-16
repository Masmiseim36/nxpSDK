
/**
 *  \file lc3_ndsp_internal.h
 *
 *  \brief This file contains internal data stuctures, functions of encoder, decoder.
 */

/*
 *  Copyright (C) 2021. Mindtree Ltd.
 *  All rights reserved.
 */
#if defined(LC3_DSP) && (LC3_DSP == 0)
#ifndef _H_LC3_NDSP_INTERNAL_
#define _H_LC3_NDSP_INTERNAL_

/* --------------------------------------------- Header File Inclusion */
#include "EM_os.h"
#include "lc3_ndsp_defines.h"
#include "LC3_ndsp_features.h"

/* --------------------------------------------- Global Definitions */


/* --------------------------------------------- Data Types/ Structures */
typedef struct
{
    const INT16  *mdct_window;
    INT32         f_s;                   /* modified Fs = 8,16,24,32,48 khz */
    INT16         f_s_ind;               /* eq.1 sampling rate index : 0=8khz, 1=16khz, 2=24khz, 3=32khz,4=48khz/44.1khz */
    INT32         bitrate;               /* bitrate to encode */
    INT16         n_f_len;               /* Nf = frame length */
    INT16         n_c;                   /* Nc = no. of channels */
    INT16         n_dms;                 /* Nms in deci-milliseconds i,e. 100 => 10ms, 75 => 7.5ms  */
    INT16         n_e_len;               /* Ne = no. of encoded freq. coeffs eq.9 */
    INT16         ndsp_mdct_window_len;       /* mdct window length */
    INT16         z_len;                 /* leading zeros length = Z eq.3  */
    INT16         n_b_len;               /* Nb = no.of energy bands */
    INT16         ltpf_mem_in_len;
    INT16         rs_mem_in_len;
    INT16         rs_mem_out_len;
    INT16         rs_x12_8_len;
    INT16         nbits_sns;
    INT16         nbits_gain;
    INT16         nbits_nf;
    INT16         nbits_bw;
    INT32         sample_rate;           /* original Fs = 8,16,24,32,44.1,48 khz */
    INT16         bps_in;                /* encoder input bits per sample 16,24,32 */
}LC3_ENCODER_CONFIG;

typedef struct
{
    INT16         nbytes;
    INT32         nbits_offset;
    INT16         nbits_total;
    INT16         nbits_init;
    INT16         nbits_ari;
    INT16         nbits_tns;
    INT16         nbits_spec;
    INT16         nbits_spec_old;
    INT16         nbits_est;
    INT16         x_exp;
    __ALIGNED(4)  INT16         mdct_mem[LC3_MAX_IDCT_MEM_SIZE];				//JB: the size of these buffer is sampling rate dependent.  If 48k is always supported, it can a buffer directly 
    __ALIGNED(4)  INT32         mdct_mem32[LC3_MAX_IDCT_MEM_SIZE];
    INT16         rs_exp;
    INT32         rs_mem32[60];
    INT32         rs_mem_hp50hz[2];
    INT16         rs_mem_in[60];
    INT16         rs_mem_out[44];        /* max(24,44) */
    INT16         att_activate;          /* attack dectector activation flag */
    INT16         att_filter_hp[2];
    INT16         att_detect;
    INT16         att_position;          /* Patt eq.19 */
    INT32         att_energy;            /* Eatt eq 18 */
    INT16         att_scale;
    INT16         tns_lpc_weighting;     /* flag eq. 70 */
    INT16         gg_off;
    INT16         pd_mem_6_4_exp;
    INT16         pd_mem_pitch;          /* old pitch  eq.90 */
    INT16         pd_mem_12_8[3];
    __ALIGNED(4)  INT16         pd_mem_6_4[178];       /* 64+114= pitch_min_6.4K = 114, len_6.4k = 64 */
    INT16         ltpf_enable;
    INT16         ltpf_mem_in_exp;
    INT16         ltpf_mem_nc;
    INT16         ltpf_mem_mem_nc;
    INT16         ltpf_mem_ltpf_active;
    INT16         ltpf_mem_pitch;
    __ALIGNED(4)  INT16         ltpf_mem_in[361];      /* 232+128+1 = 232 = max_pitch + 4 , len_12.8k = 128 */
	LC3_ENCODER_CONFIG *config;
} LC3_ENCODER_CHANNEL_CNTX;


typedef struct
{
    const INT16  *mdct_window;
    INT32         f_s;                /* modified f_s = 8,16,24,32,48 khz */
    INT16         f_s_ind;            /* 0=8khz, 1=16khz, 2=24khz, 3=32khz,4=48khz/44.1khz */
    INT16         n_f_len;            /* Nf = frame length */
    INT16         n_c;                /* Nc = no. of channels */
    INT16         plc_method;         /* PLC method ; 0 = standard, >0 = advanced */
    INT16         n_dms;              /* Nms in deci-milliseconds i,e. 100 => 10ms, 75 => 7.5ms  */
    INT16         n_e_len;            /* Ne = no. of encoded freq. coeffs eq.9 */
    INT16         ndsp_mdct_window_len;    /* mdct window length */
    INT16         z_len;              /* leading zeros length */
    INT16         n_b_len;            /* Nb = no.of energy bands */
    INT16         ltpf_mem_x_len;
    INT16         ltpf_mem_y_len;
    INT16         nbits_bw;
    INT16         bps_out;            /* decoder output bits per sample 16,24,32 */
}LC3_DECODER_CONFIG;

typedef struct
{
    INT16         nbits_total;
    INT16         pd_mem_x_exp;
    __ALIGNED(4)  INT16        ltpf_mem_x[LC3_LTPF_X_LEN];		   //JB: the size of these buffer is sampling rate dependent.  We may define the size of the array based on the highest fs supported
    __ALIGNED(4)  INT16        ltpf_mem_y[LC3_LTPF_Y_LEN];
    __ALIGNED(4)  INT16        pd_mem_x[LC3_MAX_IDCT_MEM_SIZE];
    __ALIGNED(4)  INT16        mem_x_q[LC3_X_Q_LEN];
    INT16         mem_x_q_exp;
    INT16         tns_lpc_weighting;
    INT16         ltpf_mem_x_exp;
    INT16         ltpf_mem_pitch_int;
    INT16         ltpf_mem_pitch_fr;
    INT16         ltpf_mem_gain;       /* prev gain_ltpf */
    INT16         ltpf_mem_active;
    INT16         ltpf_gain_ind;
    INT16         ltpf_mem_gain_ind;   /* prev gain_ind */
    INT16         gg_off;
    INT16         plc_type;
    INT16         plc_nblostcmpt;
    INT16         plc_seed;
    INT16         plc_cum_alpha;
	LC3_DECODER_CONFIG *config;
} LC3_DECODER_CHANNEL_CNTX;




/* ------------------------- Function Declarations */

#ifdef __cplusplus
extern "C" {
#endif

/* Common for encoder and decoder */
void lc3_ndsp_cmn_pvq_normalize_vector ( INT16 *xq, const INT16 *y,const INT32  k_val_max );

void lc3_ndsp_cmn_sns_scalefactor_interpolation
     (
                INT16 *scf_q,
                INT16 mdct_scf[],
                INT16 mdct_scf_exp[],
                INT32 inv_scf,
                INT32 n_bands,
                INT8 *work_buffer
     );

void lc3_ndsp_cmn_sns_spectral_shaping
     (
         INT32 x[],
         INT16 scf[],
         INT16 scf_exp[],
         INT32 n_dms,
         INT32 f_s_ind,
         INT32 n_b_len
     );

void lc3_ndsp_cmn_dct_ii_16( INT16 *in, INT16 *out);

void lc3_ndsp_cmn_idct_ii_16( INT16 *in, INT16 *out);

void lc3_ndsp_cmn_dct_iv( INT32 *pDat,INT16 *pDat_e, INT32  len, INT32 *work_buffer);

INT32 lc3_ndsp_cmn_get_bw_cutoff_max(INT32 idx);

void lc3_ndsp_init_set_mdct_params(INT32 n_dms, INT32 f_s_ind,const INT16 **w, INT16 *w_len, INT16 *z_len);

INT32 lc3_ndsp_dec_check_external_bfi(INT32 BFI_flag_ext, INT32 bytes_count);

INT16 lc3_ndsp_math_get_one_by_sqrt(INT16 x, INT16 *x_e);

INT16 lc3_ndsp_math_get_one_by_num(INT16 x, INT16 *x_e);

INT16 lc3_ndsp_cmn_get_norm_scale_16bit(INT16 *x, INT16 len);

INT16 lc3_ndsp_cmn_get_norm_scale_l_0 (INT32 *x, INT16 len);

INT16 lc3_ndsp_cmn_get_fx_division(INT32 x, INT32 y, INT32 *scale);

INT16 lc3_ndsp_cmn_get_fx_division_s(INT32 x, INT32 y, INT32 *scale);

INT32 lc3_ndsp_math_pow2(INT32 x);

INT32 lc3_ndsp_cmn_dot_product(INT32 *x, INT32 scale, INT32 len, INT32 *y_e);

void lc3_ndsp_math_scaling(INT16 *x, INT32 len, INT32 e);

INT32 lc3_ndsp_math_get_fx_add(INT32 x1, INT32 x1_e, INT32 x2, INT32 x2_e, INT32 *y_e);

INT16 lc3_ndsp_math_get_fx_compare(INT32 x1, INT32 x1_e, INT32 x2, INT32 x2_e);

INT32 lc3_ndsp_math_get_one_by_sqrt_l (INT32 x, INT32 *x_e);

void lc3_ndsp_fft_complex_fft(INT32 *real, INT32 *imag, INT32 length, INT16 *scale, INT32 *x);

#ifdef __cplusplus
};
#endif

#endif /* _H_LC3_NDSP_INTERNAL_ */
#endif /*defined(LC3_DSP) && (LC3_DSP == 0)*/
