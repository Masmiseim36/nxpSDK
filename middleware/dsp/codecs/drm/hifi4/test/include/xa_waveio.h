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


#ifndef __WAVEIO_H__
#define __WAVEIO_H__

#include <stdio.h>
#include <stdlib.h>

typedef struct _WAVE_HEADER
{
  char				ChunkID[4];
  unsigned int		ChunkSize;
  char				Format[4];
  char				Subchunk1ID[4];
  unsigned int		Subchunk1Size;
  unsigned short	AudioFormat;
  unsigned short	numChannels;
  unsigned int		sampleRate;
  unsigned int		ByteRate;
  unsigned short	BlockAlign;
  unsigned short	bitsPerSample;
  unsigned short	ExtraParamSize;
  char				ExtraParams[16];
  char				Subchunk2ID[4];
  unsigned int		Subchunk2Size;
} WAVE_HEADER;

typedef struct _WAVE
{
  WAVE_HEADER	header;
  FILE			*pfile;
  unsigned int	channelMask;
}WAVE, *HANDLE_WAVE, *HANDLE_WAV;
//#define ENABLE_WAVE_INP_SUPPORT
#define ENABLE_WAVE_OUT_SUPPORT

#ifdef ENABLE_WAVE_INP_SUPPORT
int WAV_InputOpen (HANDLE_WAVE *pWave, const char *ip_filename);
int WAV_InputRead (HANDLE_WAVE wave, void *ip_buffer, unsigned int Samples2Read, int ip_bitspersample);
void WAV_InputClose(HANDLE_WAVE *pWave);
#endif // ENABLE_WAVE_INP_SUPPORT
#ifdef ENABLE_WAVE_OUT_SUPPORT
int WAV_OutputOpen(HANDLE_WAVE *pWav, const char *outputFilename, int sampleRate, int numChannels, int bitsPerSample);
int WAV_OutputWrite(HANDLE_WAVE wav, void *sampleBuffer, unsigned int numberOfSamples, int nBufBits, int nSigBits);
void WAV_OutputClose(HANDLE_WAVE *pWav);
#endif


#ifdef __cplusplus
extern "C" {
#endif



#ifdef __cplusplus
}
#endif


#endif /* __WAVEIO_H__ */
