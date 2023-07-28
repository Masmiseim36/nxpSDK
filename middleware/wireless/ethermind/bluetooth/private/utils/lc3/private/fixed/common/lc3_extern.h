
/**
 *  \file lc3_extern.h
 *
 *  \brief This file contains external data stuctures of LC3 encoder, decoder.
 */

/*
 *  Copyright (C) 2021. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_LC3_EXTERN_
#define _H_LC3_EXTERN_

/* --------------------------------------------- Header File Inclusion */


/* --------------------------------------------- Global Definitions */
#ifdef LC3_HAVE_NB
extern DATA_ALIGN const INT16 mdct_window_N80[130];
#endif /* LC3_HAVE_NB */
#ifdef LC3_HAVE_WB
extern DATA_ALIGN const INT16 mdct_window_N160[260];
#endif /* LC3_HAVE_WB */
#ifdef LC3_HAVE_SSWB
extern DATA_ALIGN const INT16 mdct_window_N240[390];
#endif /* LC3_HAVE_SSWB */
#ifdef LC3_HAVE_SWB
extern DATA_ALIGN const INT16 mdct_window_N320[520];
#endif /* LC3_HAVE_SWB */
#ifdef LC3_HAVE_FB
extern DATA_ALIGN const INT16 mdct_window_N480[780];
#endif /* LC3_HAVE_FB */

#ifdef LC3_ENABLE_7_5MS_MODE
#ifdef LC3_HAVE_NB
extern DATA_ALIGN const INT16 mdct_window_N60_7_5ms[106];
#endif /* LC3_HAVE_NB */
#ifdef LC3_HAVE_WB
extern DATA_ALIGN const INT16 mdct_window_N120_7_5ms[212];
#endif /* LC3_HAVE_WB */
#ifdef LC3_HAVE_SSWB
extern DATA_ALIGN const INT16 mdct_window_N180_7_5ms[318];
#endif /* LC3_HAVE_SSWB */
#ifdef LC3_HAVE_SWB
extern DATA_ALIGN const INT16 mdct_window_N240_7_5ms[424];
#endif /* LC3_HAVE_SWB */
#ifdef LC3_HAVE_FB
extern DATA_ALIGN const INT16 mdct_window_N360_7_5ms[636];
#endif /* LC3_HAVE_FB */
#endif /* LC3_ENABLE_7_5MS_MODE */

#ifdef LC3_ENCODER_SUPPORT
extern DATA_ALIGN const INT32 tns_acf_lagwindow[8];
#endif /* LC3_ENCODER_SUPPORT */
extern DATA_ALIGN const INT16 tns_de_quant_lut[17];

extern DATA_ALIGN const INT8   ac_spec_lookup[4096];
extern DATA_ALIGN const UINT16 ac_spec_cumfreq[64][17];
extern DATA_ALIGN const UINT16 ac_spec_freq[64][17];
#ifdef LC3_ENCODER_SUPPORT
extern DATA_ALIGN const UINT16 ari_spec_bits[64][17];
#endif /* LC3_ENCODER_SUPPORT */
extern DATA_ALIGN const INT16 ac_tns_order_bits[2][8];
extern DATA_ALIGN const INT16 ac_tns_order_freq[2][8];
extern DATA_ALIGN const INT16 ac_tns_order_cumfreq[2][8];
#ifdef LC3_ENCODER_SUPPORT
extern DATA_ALIGN const INT16 ac_tns_coef_bits[8][17];
#endif /* LC3_ENCODER_SUPPORT */
extern DATA_ALIGN const INT16 ac_tns_coef_freq[8][17];
extern DATA_ALIGN const INT16 ac_tns_coef_cumfreq[8][17];

#ifdef LC3_ENCODER_SUPPORT
extern DATA_ALIGN const INT16 *const sns_pre_emphasis[5];
extern DATA_ALIGN const INT16 *const sns_pre_emphasis_exp[5];
#endif /* LC3_ENCODER_SUPPORT */

extern DATA_ALIGN const INT16 *const bands_indices_10ms[5];
#ifdef LC3_ENABLE_7_5MS_MODE
extern DATA_ALIGN const INT16 *const bands_indices_7_5ms[5];
#endif /* LC3_ENABLE_7_5MS_MODE */

#ifdef LC3_ENCODER_SUPPORT
#ifdef LC3_HAVE_NB
extern DATA_ALIGN const INT16 ltpf_resample_filter_8khz[240];
#endif /* LC3_HAVE_NB */
#ifdef LC3_HAVE_WB
extern DATA_ALIGN const INT16 ltpf_resample_filter_16khz[240];
#endif /* LC3_HAVE_WB */
#ifdef LC3_HAVE_SSWB
extern DATA_ALIGN const INT16 ltpf_resample_filter_24khz[240];
#endif /* LC3_HAVE_SSWB */
#ifdef LC3_HAVE_SWB
extern DATA_ALIGN const INT16 ltpf_resample_filter_32khz[240];
#endif /* LC3_HAVE_SWB */
#ifdef LC3_HAVE_FB
extern DATA_ALIGN const INT16 ltpf_resample_filter_48khz[240];
#endif /* LC3_HAVE_FB */

extern DATA_ALIGN const INT16 ltpf_ac_weighting[98];
extern DATA_ALIGN const INT16 ltpf_pitch_interp_filter[7][9];
#endif /* LC3_ENCODER_SUPPORT */

#ifdef LC3_DECODER_SUPPORT
#if (defined(LC3_HAVE_NB) || defined(LC3_HAVE_WB))
extern DATA_ALIGN const INT16 ltpf_synth_filter_den_8_16khz[4][4];
#endif /* (LC3_HAVE_NB) || defined(LC3_HAVE_WB)) */

#ifdef LC3_HAVE_SSWB
extern DATA_ALIGN const INT16 ltpf_synth_filter_den_24khz[4][6];
#endif /* LC3_HAVE_SSWB */

#ifdef LC3_HAVE_SWB
extern DATA_ALIGN const INT16 ltpf_synth_filter_den_32khz[4][8];
#endif /* LC3_HAVE_SWB */

#ifdef LC3_HAVE_FB
extern DATA_ALIGN const INT16 ltpf_synth_filter_den_48khz[4][12];
#endif /* LC3_HAVE_FB */

#if (defined(LC3_HAVE_NB) || defined(LC3_HAVE_WB))
extern DATA_ALIGN const INT16 ltpf_synth_filter_num_8_16khz[4][3];
#endif /* (defined(LC3_HAVE_NB) || defined(LC3_HAVE_WB)) */

#ifdef LC3_HAVE_SSWB
extern DATA_ALIGN const INT16 ltpf_synth_filter_num_24khz[4][5];
#endif /* LC3_HAVE_SSWB */

#ifdef LC3_HAVE_SWB
extern DATA_ALIGN const INT16 ltpf_synth_filter_num_32khz[4][7];
#endif /* LC3_HAVE_SWB */

#ifdef LC3_HAVE_FB
extern DATA_ALIGN const INT16 ltpf_synth_filter_num_48khz[4][11];
#endif /* LC3_HAVE_FB */

#endif /* LC3_DECODER_SUPPORT */

extern DATA_ALIGN const INT16 ltpf_gain_scale[4];

extern DATA_ALIGN const INT16 sns_vq_reg_adj_gains[2];
extern DATA_ALIGN const INT16 sns_vq_reg_lf_adj_gains[4];
extern DATA_ALIGN const INT16 sns_vq_near_adj_gains[4];
extern DATA_ALIGN const INT16 sns_vq_far_adj_gains[8];

extern DATA_ALIGN const INT16 sns_gainMSBbits[4];
extern DATA_ALIGN const INT16 sns_gainLSBbits[4];

extern DATA_ALIGN const INT16 sns_LFCB[256];
extern DATA_ALIGN const INT16 sns_HFCB[256];

extern DATA_ALIGN const UINT32 MPVQ_offsets_N6_K1[3];
extern DATA_ALIGN const UINT32 MPVQ_offsets_N10_K10[12];
extern DATA_ALIGN const UINT32 MPVQ_offsets_N16_K10[12];

extern DATA_ALIGN const INT16 global_gain_adjust_tables[5][5];

/* TODO : Check DATA_ALIGN */
#if (defined(LC3_HAVE_NB) || defined(LC3_HAVE_WB) || defined(LC3_HAVE_SSWB) || defined(LC3_HAVE_SWB) || defined(LC3_HAVE_FB))
extern const INT16 twid_fact_sine_cos_lut_post_fft_480[482];
#endif /* (defined(LC3_HAVE_NB) || defined(LC3_HAVE_WB) || defined(LC3_HAVE_SSWB) || defined(LC3_HAVE_SWB) || defined(LC3_HAVE_FB)) */
#ifdef LC3_ENABLE_7_5MS_MODE
#if ( defined(LC3_HAVE_SSWB) || defined(LC3_HAVE_FB))
extern const INT16 twid_fact_sine_cos_lut_post_fft_360[362];
#endif /* ( defined(LC3_HAVE_SSWB) || defined(LC3_HAVE_FB)) */
#endif /* LC3_ENABLE_7_5MS_MODE */
#if ( defined(LC3_HAVE_NB) || defined(LC3_HAVE_WB) || defined(LC3_HAVE_SWB))
extern const INT16 twid_fact_sine_cos_lut_post_fft_320[322];
#endif /* ( defined(LC3_HAVE_NB) || defined(LC3_HAVE_WB) || defined(LC3_HAVE_SWB)) */

#ifdef LC3_ENABLE_7_5MS_MODE
#ifdef LC3_HAVE_NB
extern const INT16 twid_fact_sine_cos_lut_pre_fft_60[60];
#endif /* LC3_HAVE_NB */
#endif /* LC3_ENABLE_7_5MS_MODE */
#ifdef LC3_HAVE_NB
extern const INT16 twid_fact_sine_cos_lut_pre_fft_80[80];
#endif /* LC3_HAVE_NB */
#ifdef LC3_ENABLE_7_5MS_MODE
#ifdef LC3_HAVE_WB
extern const INT16 twid_fact_sine_cos_lut_pre_fft_120[120];
#endif /* LC3_HAVE_WB */
#endif /* LC3_ENABLE_7_5MS_MODE */
#ifdef LC3_HAVE_WB
extern const INT16 twid_fact_sine_cos_lut_pre_fft_160[160];
#endif /* LC3_HAVE_WB */
#ifdef LC3_ENABLE_7_5MS_MODE
#ifdef LC3_HAVE_SSWB
extern const INT16 twid_fact_sine_cos_lut_pre_fft_180[180];
#endif /* LC3_HAVE_SSWB */
#endif /* LC3_ENABLE_7_5MS_MODE */
#if ( defined(LC3_HAVE_SSWB) || defined(LC3_HAVE_SWB) )
extern const INT16 twid_fact_sine_cos_lut_pre_fft_240[240];
#endif  /*  ( defined(LC3_HAVE_SSWB) || defined(LC3_HAVE_SWB) ) */
#ifdef LC3_HAVE_SWB
extern const INT16 twid_fact_sine_cos_lut_pre_fft_320[320];
#endif /* LC3_HAVE_SWB */
#ifdef LC3_ENABLE_7_5MS_MODE
#ifdef LC3_HAVE_FB
extern const INT16 twid_fact_sine_cos_lut_pre_fft_360[360];
#endif /* LC3_HAVE_FB */
#endif /* LC3_ENABLE_7_5MS_MODE */
#ifdef LC3_HAVE_FB
extern const INT16 twid_fact_sine_cos_lut_pre_fft_480[480];
#endif /* LC3_HAVE_FB */

#ifdef LC3_HAVE_NB
extern const INT32 rotation_vector_40_32[56];
#endif /* LC3_HAVE_NB */
#if (defined(LC3_HAVE_WB) || defined(LC3_HAVE_SWB))
extern const INT16 rotation_vector_320[600];
#endif /* #if (defined(LC3_HAVE_WB) || defined(LC3_HAVE_SWB)) */
#ifdef LC3_ENABLE_7_5MS_MODE
#ifdef LC3_HAVE_FB
extern const INT16 rotation_vector_360[660];
#endif /* LC3_HAVE_FB */
#endif /* LC3_ENABLE_7_5MS_MODE */
#if (defined(LC3_HAVE_WB) || defined(LC3_HAVE_SWB) ||  defined(LC3_HAVE_SSWB) || defined(LC3_HAVE_FB))
extern const INT16 rotation_vector_480[900];
#endif /*  (defined(LC3_HAVE_WB) || defined(LC3_HAVE_SWB) ||  defined(LC3_HAVE_SSWB) || defined(LC3_HAVE_FB)) */

#ifdef LC3_ENABLE_7_5MS_MODE
#ifdef LC3_HAVE_SSWB
extern const INT16 rotation_vector_15_6[150];
#endif /* LC3_HAVE_SSWB */
#endif /* LC3_ENABLE_7_5MS_MODE */


extern const UINT32 pow2_lut1[32];
extern const UINT32 pow2_lut2[32];
extern const UINT32 pow2_lut3[32];

#ifdef LC3_ENCODER_SUPPORT

extern const INT16 inverse_integer_lut[32];
extern const INT32 inverse_sqrt_lut[130];

extern const INT32 log2_lut1[16];
extern const INT16 log2_lut2[16];

extern const INT32 inverse_lut1[32];
extern const INT16 inverse_lut2[32];

#endif /* LC3_ENCODER_SUPPORT */

extern const INT32 pow2_s_lut1[64];
extern const INT16 pow2_s_lut2[64];

extern const INT32 inverse_sqrt_lut1[32];
extern const INT16 inverse_sqrt_lut2[32];


extern DATA_ALIGN const INT16 inverse_sqrt_s_lut[65];


/* extern DATA_ALIGN const INT16 dct2_coeff[256]; */

extern DATA_ALIGN  const UINT8  fft30_index1[30];
extern DATA_ALIGN  const UINT8  fft30_index2[30];
extern DATA_ALIGN  const UINT8  fft30_index3[30];
extern DATA_ALIGN  const UINT8  fft30_index4[30];
extern DATA_ALIGN  const UINT8  fft30_index5[15];
extern DATA_ALIGN  const UINT8  fft30_index6[15];
extern DATA_ALIGN  const UINT8  fft30_index7[15];

extern DATA_ALIGN  const UINT8  fft20_index1[40];
extern DATA_ALIGN  const UINT8  fft20_index2[40];
extern DATA_ALIGN  const UINT8  fft20_index3[40];

extern DATA_ALIGN  const UINT8  fft15_index1[30];
extern DATA_ALIGN  const UINT8  fft15_index2[30];
extern DATA_ALIGN  const UINT8  fft15_index3[30];

extern DATA_ALIGN  const UINT8  fft10_index1[20];
extern DATA_ALIGN  const UINT8  fft10_index2[20];

/* extern DATA_ALIGN const UINT8  energy_bandscale[32]; */

#endif /* _H_LC3_EXTERN_ */

