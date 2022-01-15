////////////////////////////////////////////////////////////////////////////////
//! \addtogroup opus OPUS CCI RAW
//! @{
//
// Copyright (c) 2013-2020 NXP
// @todo which license goes in this file?
//
//! \file opus_cci_raw.h
//! \brief Contains OPUS CCI RAW defines and data structures
////////////////////////////////////////////////////////////////////////////////

#ifndef _OPUS_CCI_RAW_H
#define _OPUS_CCI_RAW_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdarg.h>
#include <opus_types.h>
#include "control.h"

#define MAX_PACKET 1500

#ifdef _LINUX_
#define MAX_CHANNEL 2//2
#define MAX_FRAME 960//160//320//480//960 // Sample Rate/50
#define STACK_SIZE  22*1024
#define FRAMES_PER_PACKET 1

#define OPUS_PAGE_SIZE 20*1024 + 200
#define LACING_SIZE 256*4
#define OUTPUT_BUFFER MAX_FRAME*FRAMES_PER_PACKET*MAX_CHANNEL*2 // Frame size ( for 16khz ) * one frame * channel * sizeof(short)

#else

#define MAX_CHANNEL 2
#define MAX_FRAME 960//160//320//480//960 // Sample Rate/50
#define STACK_SIZE  30*1024
#define FRAMES_PER_PACKET 1

#define OPUS_PAGE_SIZE (2*1024)
#define LACING_SIZE 256*4
#define OUTPUT_BUFFER MAX_FRAME*FRAMES_PER_PACKET*MAX_CHANNEL*2 // Frame size ( for 16khz ) * one frame * channel * sizeof(short)

#endif

//#ifdef FIXED_POINT
//#define STACK_SIZE ((24*1024) + ((MAX_FRAME >> 2 )*MAX_CHANNEL*2) + ((MAX_FRAME >> 2 )*MAX_CHANNEL*2) + (MAX_FRAME*MAX_CHANNEL*2) )
//#else
//#define STACK_SIZE ((30*1024) + ((MAX_FRAME >> 2 )*MAX_CHANNEL*2) + ((MAX_FRAME >> 2 )*MAX_CHANNEL*2) + (MAX_FRAME*MAX_CHANNEL*2) )
//#endif
#if 0
//! \brief Declares pfnReadCallback as a ptr to a function that Read data from the input stream
typedef int (pfnReadCallback)(int StreamNo, unsigned char *pAlgorithmBuffer, int Size, int UpdateTail, void *UserData);
//! \brief Declares pfnSeekCallback as a pointer to a function that seek the input stream
typedef int (pfnSeekCallback)(int StreamNo, int offset, int position, void *UserData );
//! \brief Declares pfnFtellCallback as a pointer to a function that gives current position of the input stream
typedef int (pfnFtellCallback)(int StreamNo, void *UserData );

//! \brief This structure is used by the decoder to register the callback APIs.
typedef struct _decoder_callback_FunctionTable
{
   //! \brief Pointer to a function that Reads data from the input stream
   pfnReadCallback     *pfnRead;
   //! \brief Pointer to a function that seeks the input stream
   pfnSeekCallback     *pfnSeek;
   //! \brief Pointer to a function that gives current position of the input stream
   pfnFtellCallback    *pfnFtell;

} decoder_callback_FunctionTable_t;
#endif

typedef struct _OpusDecoder
{
   int          celt_dec_offset;
   int          silk_dec_offset;
   int          channels;
   opus_int32   Fs;          /** Sampling rate (at the API level) */
   silk_DecControlStruct DecControl;
   int          decode_gain;

   /* Everything beyond this point gets cleared on a reset */
#define OPUS_DECODER_RESET_START stream_channels
   int          stream_channels;

   int          bandwidth;
   int          mode;
   int          prev_mode;
   int          frame_size;
   int          prev_redundancy;
   int          last_packet_duration;

   opus_uint32  rangeFinal;


    int max_payload_bytes;
    int use_inbandfec;
    int packet_loss_perc;
    int max_frame_size;
#ifdef FIXED_POINT
  short *out;
#else
  float *out;
#endif
  unsigned char *data[2];
  int len[2];
  int toggle;
  opus_int32 count;
  int skip;
  int lost_prev;
  int infile;
  int outfile;
  opus_int64 total_bytes;

#if 0
  pfnReadCallback     *pfnRead;
  pfnSeekCallback     *pfnSeek;
  pfnFtellCallback    *pfnFtell;
#endif
  char *g_stackBase;
  char *g_stackEnd;
  char *g_stackptr;
  int g_stack_size;

  unsigned char g_stack_buffer[STACK_SIZE];

}OpusDecoder_t;

#endif //_OPUS_CCI_RAW_H
///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
//! @}