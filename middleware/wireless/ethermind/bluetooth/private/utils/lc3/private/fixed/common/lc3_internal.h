
/**
 *  \file lc3_internal.h
 *
 *  \brief This file contains internal data stuctures, functions of encoder, decoder.
 */

/*
 *  Copyright (C) 2021. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_LC3_INTERNAL_
#define _H_LC3_INTERNAL_

/* --------------------------------------------- Header File Inclusion */
#include "LC3_api.h"
#include "lc3_defines.h"
#include "LC3_features.h"
#include "lc3_common_internal.h"

/* --------------------------------------------- Global Definitions */


/* --------------------------------------------- Data Types/ Structures */

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
    INT16        *mdct_mem;
    INT32        *mdct_mem32;
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
    INT16         pd_mem_6_4[178];       /* 64+114= pitch_min_6.4K = 114, len_6.4k = 64 */
    INT16         ltpf_enable;
    INT16         ltpf_mem_in_exp;
    INT16         ltpf_mem_nc;
    INT16         ltpf_mem_mem_nc;
    INT16         ltpf_mem_ltpf_active;
    INT16         ltpf_mem_pitch;
    INT16         ltpf_mem_in[361];      /* 232+128+1 = 232 = max_pitch + 4 , len_12.8k = 128 */
    INT32        *pcm_in;
    UINT8        *bs_out;
    UINT8        *work_buffer;
    INT32         flg_BFI;

} LC3_ENCODER_CHANNEL_CNTX;

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
    INT16         mdct_window_len;       /* mdct window length */
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
    LC3_ENCODER_CHANNEL_CNTX *channel[LC3_CHANNELS_MAX];
    LC3_ENCODER_CONFIG        config;
}LC3_ENCODER;

typedef struct
{
    INT16         nbits_total;
    INT16         nbytes;
    INT16         nbytes_old;          /* last frame's decoded bytes */
    INT16        *ltpf_mem_x;
    INT16        *ltpf_mem_y;
    INT16        *pd_mem_x;
    INT16         pd_mem_x_exp;
    INT16        *mem_x_q;
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
    UINT8        *work_buffer;
    INT16         flg_BFI;
    UINT8        *bs_in;
    INT32        *pcm_out;
} LC3_DECODER_CHANNEL_CNTX;

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
    INT16         mdct_window_len;    /* mdct window length */
    INT16         z_len;              /* leading zeros length */
    INT16         n_b_len;            /* Nb = no.of energy bands */
    INT16         ltpf_mem_x_len;
    INT16         ltpf_mem_y_len;
    INT16         nbits_bw;
    INT16         bps_out;            /* decoder output bits per sample 16,24,32 */
}LC3_DECODER_CONFIG;

typedef struct
{
    LC3_DECODER_CHANNEL_CNTX *channel[LC3_CHANNELS_MAX];
    LC3_DECODER_CONFIG        config;
}LC3_DECODER;



/* ------------------------- Function Declarations */

#ifdef __cplusplus
extern "C" {
#endif

void lc3_encode
     (
         LC3_ENCODER_CONFIG* config,
         LC3_ENCODER_CHANNEL_CNTX* channel_cntx
     );

INT32 lc3_decode
      (
          LC3_DECODER_CONFIG* config,
          LC3_DECODER_CHANNEL_CNTX* channel_cntx
      );

/* Common for encoder and decoder */
void lc3_cmn_pvq_normalize_vector ( INT16 *xq, const INT16 *y,const INT32  k_val_max );

void lc3_cmn_sns_scalefactor_interpolation
     (
                INT16 *scf_q,
                INT16 mdct_scf[],
                INT16 mdct_scf_exp[],
                INT32 inv_scf,
                INT32 n_bands,
                INT8 *work_buffer
     );

void lc3_cmn_sns_spectral_shaping
     (
         INT32 x[],
         INT16 scf[],
         INT16 scf_exp[],
         INT32 n_dms,
         INT32 f_s_ind,
         INT32 n_b_len
     );

#ifdef LC3_ENCODER_SUPPORT
void lc3_cmn_dct_ii_16(INT16* x, INT16* y);
#endif /* LC3_ENCODER_SUPPORT */
void lc3_cmn_idct_ii_16(INT16 *x, INT16 *y);

void lc3_cmn_dct_iv(INT32 *x, INT16 *x_e, INT32  len, INT32 *work_buffer);

INT32 lc3_cmn_get_bw_cutoff_max(INT32 idx);

void lc3_init_set_mdct_params(INT32 n_dms, INT32 f_s_ind,const INT16 **w, INT16 *w_len, INT16 *z_len);


#ifdef LC3_ENCODER_SUPPORT
INT32 lc3_enc_get_context_buffers_size(INT32 sample_rate, INT32 channels);

INT32 lc3_enc_context_buffers_init(LC3_ENCODER *enc_io, INT32 sample_rate, INT32 channels);

INT32 lc3_enc_set_config_params(LC3_ENCODER *enc_io, INT32 sample_rate, INT32 channels, INT32 n_dms);

INT32 lc3_enc_set_target_bitrate(LC3_ENCODER *enc_io, INT32* bytes_count);
#endif /* LC3_ENCODER_SUPPORT */

#ifdef LC3_DECODER_SUPPORT
INT32 lc3_dec_update_config_on_bitrate_switch(LC3_DECODER *dec_io, INT32 channel_idx, INT32 channel_bytes);

INT32 lc3_dec_get_context_buffers_size( INT32 sample_rate, INT32 channels);

INT32 lc3_dec_context_buffers_init(LC3_DECODER *dec_io, INT32 sample_rate, INT32 channels);

INT32 lc3_dec_set_config_params(LC3_DECODER *dec_io, INT32 sample_rate, INT32 channels, INT32 plc_type, INT32 n_dms);

INT32 lc3_dec_check_external_bfi(INT32 BFI_flag_ext, INT32 bytes_count);

INT32 lc3_dec_check_bitrate_switch(LC3_DECODER *dec_io, INT32 channel_idx, INT32 byte_count, INT32 BFI_flag);
#endif /* LC3_DECODER_SUPPORT */


INT16 lc3_math_get_one_by_sqrt(INT16 x, INT16 *x_e);

#ifdef LC3_ENCODER_SUPPORT
INT16 lc3_math_get_one_by_num(INT16 x, INT16 *x_e);
#endif /* LC3_ENCODER_SUPPORT */

INT16 lc3_cmn_get_norm_scale(INT16 *x, INT16 len);

INT16 lc3_cmn_get_norm_scale_0(INT16 *x, INT16 len);

INT16 lc3_cmn_get_norm_scale_l(INT32 *x, INT16 len);

/* INT16 lc3_cmn_get_norm_scale_l_b20(INT32 *x, INT16 len_b20); */

INT16 lc3_cmn_get_norm_scale_l_0 (INT32 *x, INT16 len);

INT32 lc3_math_pow2(INT32 x);

void lc3_math_scaling(INT16 *x, INT32 len, INT32 e);


#ifdef LC3_ENCODER_SUPPORT
INT16 lc3_cmn_get_fx_division(INT32 x, INT32 y, INT32 *scale);

INT16 lc3_cmn_get_fx_division_s(INT32 x, INT32 y, INT32 *scale);

INT32 lc3_cmn_dot_product(INT32 *x, INT32 scale, INT32 len, INT32 *y_e);

INT32 lc3_math_get_fx_add(INT32 x1, INT32 x1_e, INT32 x2, INT32 x2_e, INT32 *y_e);

INT16 lc3_math_get_fx_compare(INT32 x1, INT32 x1_e, INT32 x2, INT32 x2_e);

INT32 lc3_math_get_one_by_sqrt_l (INT32 x, INT32 *x_e);
#endif /* LC3_ENCODER_SUPPORT */

void lc3_fft_complex_fft(INT32 *x, INT32 length, INT16 *scale, INT32 *work);

#ifdef __cplusplus
};
#endif

#endif /* _H_LC3_INTERNAL_ */
