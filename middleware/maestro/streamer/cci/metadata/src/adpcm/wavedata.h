/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

// Extern references
#define MAX_CHANNEL 2

extern short *InputBuf;
extern short *OutputBuf;

typedef struct IOStream__
{
    uint8_t *pu8ReadBaseAddr;
    uint8_t *pu8ReadPtr;
    uint8_t *pu8ReadEndAddr;
    uint8_t *pu8WriteBaseAddr;
    uint8_t *pu8WritePtr;
    uint8_t *pu8WriteEndAddr;
} IOStream_t;

// Function Declarations
typedef struct
{
    int pred;
    int step;
    int index;
} tADPCM;

typedef struct _ms_adpcm_memory
{
    int iPredSample[2];
    short iSamp1[2];
    short iSamp2[2];
    short iCoef1[2];
    short iCoef2[2];
    short iDelta[2];
    short wCoef1[10]; // Provision for 10 coefficient sets. Signed data.
    short wCoef2[10];
    unsigned char bPredictor[2];
    int first_block;
} ms_adpcm_memory_t;

typedef struct _ima_adpcm_memory
{
    tADPCM sIma[MAX_CHANNEL];
} ima_adpcm_memory_t;

typedef struct
{
    union
    {
        ms_adpcm_memory_t ms;
        ima_adpcm_memory_t ima;
    } u;
    // Frequently used variables
    short wnSamplesPerBlock;
    short gwBlockAlign;
    short gwChannels;

    int gwWavDuration;
    int gwSamplesPerSec;
    int gwAvgBytesPerSec;
    unsigned short gwFormatTag;
    short wBitsPerSample;
    short wcbSize;
    short wnNumCoef;
    int datachunksize; // Used in AIFF
    int dataOffset;
    short wValidBitsPerSample;
    int dwChannelMask;
    //    int nReadBytes;
    //    int nWriteBytes;
    int OutputBinaryDataWritten;
    int InputDataPtr;
    int gnInputBufModulo;
    int gnOutputBufModulo;
    uint8_t *OutputBinaryData;
    uint8_t *InputBinaryData;
} WAVDEC;

extern int waveparser();
extern int Initialize_Wave_Header(int formattag, int num_channels, int SamplingRate);
void Write_Wave_Header();

void InitIMAStates(void);
extern void Encode_PCM(int);
/* Decoder prototypes */
extern void Decode_PCM_Block(WAVDEC *ptrThis, int SamplestoDecode, int *pi32BytesRead, int *pi32BytesWrite);
extern void Decode_MS_ADPCM_Block(
    WAVDEC *ptrThis, int SamplestoDecode, int channels, int ContinuationFlag, int *pi32BytesRead, int *pi32BytesWrite);
extern void Decode_IMA_ADPCM_Block(
    WAVDEC *ptrThis, int SamplestoDecode, int ContinuationFlag, int *pi32BytesRead, int *pi32BytesWrite);
extern int setupWaveDecoderStream(IOStream_t *IOStream,
                                  WAVDEC *wav,
                                  int *fileOffset,
                                  unsigned char i32eof,
                                  int i32InitialBufferSize,
                                  int decType,
                                  int i32DataOffset);
extern void Decode_AIFF(WAVDEC *ptrThis, int SamplestoDecode, int *pi32BytesRead, int *pi32BytesWrite);
