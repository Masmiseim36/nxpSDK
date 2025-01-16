/*
 * Copyright (c) 2014-2022 Cadence Design Systems, Inc.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


 #ifndef __XA_DRM_DEC_USAGE_H__
 #define __XA_DRM_DEC_USAGE_H__
 
 /***************************************************************************************
 This file contains the Usage text printed; 
 Modify keeing STRING("<your content>") for every line;
 * New line character is autamatically added, hence no need. 
 * Don't use \t, and replace by 4 spaces
 * for Quotes ' and ", use escape character \
 ***************************************************************************************/
STRING("    --------------------------------------------------------------              ")
STRING("    xa_drm_dec_test - Sample testbench for HiFi AAC ELD Encode/Decode           ")
STRING("    --------------------------------------------------------------              ")
STRING("    USAGE:                                                                      ")
STRING("      xa_drm_dec_test [OPTIONS] -if inp_file.drm -of out_file.wav               ")
STRING("                                                                                ")
STRING("         -h                     - Show this help                                ")
STRING("         -y <start_frame>       - Start decoding at given frame number (0-32767)")
STRING("         -z <stop_frame>	- Stop decoding at given frame number (0-32767) ")
STRING("         -p <qmf_mode>          - QMF mode:                             	")
STRING("                                   (0) high quality,                            ")
STRING("                                   (1) low power,                               ")
STRING("                                   (-1) auto [default]                          ")
STRING("         -om <output_mode>      - Desired output mode:                          ")
STRING("                                   (0) normal [default],                        ")
STRING("                                   (1) mono,                                    ")
STRING("                                   (2) stereo                                   ")
STRING("         -os <output_silence>   - If 1, Output silent frames during xHE-AAC     ")
STRING("                                  initialization phase (default 0 i.e no silence")
STRING("                                  insertion)                                    ")
STRING("         -cfo <fadeout_len>     - Fade-out slope in frames 0-15(default:6)      ")
STRING("         -cfi <fadein_len>      - Fade-in  slope in frames 0-15(default:5)      ")
STRING("         -cmr <mute_release_len>- Mute-release length in frames 0-31(default:3) ")
STRING("         -ccn <comfort_noise>   - comfort-noise level(default:1048576(0x100000))")
STRING("                                  0 to 2147483647(0x7fffffff)                   ")
STRING("                                  The level of the noise to be inserted after a ")
STRING("                                  fade-out, 0(full_scaled) to 61 (no noise)     ")
STRING("                                  The value -1 switches off comfort noise       ")
STRING("                                                                                ")
#ifdef RAW_AAC_SUPPORTED
STRING("         -fmt <inp_format>      - Input Format      :                           ")
STRING("                                   (5) rawpkts format,                          ")
STRING("                                   (12) DRM (default),                          ")
STRING("      Note:                                                                     ")
STRING("        rawpkts format is a customized format to store raw AAC frames. Refer    ")
STRING("                Programmer's guide to learn the syntax of this format           ")
STRING("      __________________________________________________________________________")
#endif
#endif // __XA_DRM_DEC_USAGE_H__

