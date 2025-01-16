///////////////////////////////////////////////////////////////////////////////
//! \addtogroup asi_dffi
// @{
//
// Copyright 2004 SigmaTel, Inc.
// Copyright 2024 NXP
//
// SPDX-License-Identifier: BSD-3-Clause
//
//! \file          asi_dffi_aac_utility.c
//! \brief         This file includes the utility Bitstream function used by the Metadata
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Includes
////////////////////////////////////////////////////////////////////////////////

// #include <types.h>
#include "codec_interface.h"
#include "cci_metadata.h"
#include "aac_metadata.h"

////////////////////////////////////////////////////////////////////////////////
//  External Definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Global References
////////////////////////////////////////////////////////////////////////////////
static const uint32_t stc_u32SamplingRateList[13] = {96000, 88200, 64000, 48000, 44100, 32000, 24000,
                                                     22050, 16000, 12000, 11025, 8000,  7350};

////////////////////////////////////////////////////////////////////////////////
//  Prototypes
////////////////////////////////////////////////////////////////////////////////

int GetAACDWord(CCI_Ctx *Ctx, AAC_DFFI_t *ptrAac);
int GetAacWord(CCI_Ctx *Ctx, AAC_DFFI_t *ptrAac);
int GetAACByte(CCI_Ctx *Ctx, AAC_DFFI_t *ptrAac);
void SkipAACData(long AacCkSize, CCI_Ctx *Ctx, AAC_DFFI_t *ptrAac);
/////////////////////////////////////////////////////////////////////////////////
//!
//!    \brief  Gets an unsigned DWord (4 bytes) of Data from the file
//!
//!    \fntype Function
//!
//!    \param[in]  Handle handlenumber
//!    \retval     Returns a unsigned word
//!
/////////////////////////////////////////////////////////////////////////////////
int GetAACDWord(CCI_Ctx *Ctx, AAC_DFFI_t *ptrAac)
{
    unsigned char word[4];
    int retdata = 0;
    ptrAac->aac_file_offset += 4;
    Ctx->cci_dec_read(0, (uint8_t *)&word[0], 4, 0, Ctx->user_data);
    retdata = ((int)word[0] << 24);
    retdata |= ((int)word[1] << 16);
    retdata |= ((int)word[2] << 8);
    retdata |= ((int)word[3]);
    return retdata;
}
/////////////////////////////////////////////////////////////////////////////////
//!
//!    \brief     Gets an unsigned Word of Data from the file
//!
//!    \fntype    Function
//!
//!    \param[in] Handle Handlenumber
//!
//!    \retval   Data from file
//!
//!
/////////////////////////////////////////////////////////////////////////////////
int GetAacWord(CCI_Ctx *Ctx, AAC_DFFI_t *ptrAac)
{
    unsigned char word[2];
    int retdata = 0;
    ptrAac->aac_file_offset += 2;
    Ctx->cci_dec_read(0, (uint8_t *)&word[0], 2, 0, Ctx->user_data);

    retdata = ((int)word[0] << 8);
    retdata |= ((int)word[1]);
    return retdata;
}
//;;///////////////////////////////////////////////////////////////////////////////
//;
//;>   Name:  GetAACByte(INT iHandle)
//;
//;    Type:   Function
//;
//;    void GetAACByte(INT iHandle)
//;
//;    Description:
//;          Gets an unsigned BYTE  of Data from the file
//;
//;    Inputs: handlenumber
//;    Outputs:
//;    Notes:
//;///////////////////////////////////////////////////////////////////////////////
//
int GetAACByte(CCI_Ctx *Ctx, AAC_DFFI_t *ptrAac)
{
    unsigned char word[1];
    int retdata = 0;
    ptrAac->aac_file_offset += 1;
    Ctx->cci_dec_read(0, (uint8_t *)&word[0], 1, 0, Ctx->user_data);
    retdata = (int)word[0];
    return retdata;
}
/////////////////////////////////////////////////////////////////////////////////
//!   \brief    Used to skip unknown chunks
//!
//!   \fntype  Function
//!
//!   \param[in]  AacCkSize amount to skip
//!   \param[in]  Handle handlenumber
//!
//!   \return  None
//!   \note     Used by the decoder to skip unknown chunks
/////////////////////////////////////////////////////////////////////////////////
void SkipAACData(long AacCkSize, CCI_Ctx *Ctx, AAC_DFFI_t *ptrAac)
{
    ptrAac->aac_file_offset += AacCkSize; // to Fix SDK-1198.
    Ctx->cci_dec_seek(0, ptrAac->aac_file_offset, SEEK_SET, Ctx->user_data);
}
/////////////////////////////////////////////////////////////////////////////////
//!   \brief    Used to read required amount of data.
//!
//!   \fntype	Function
//!
//!   \param[in]  n amount of bytes to read.
//!   \param[in]  iHandle handlenumber.
//!
//!   \return	required amount of data.
//!   \note     Used by the decoder to read required amount of data.
/////////////////////////////////////////////////////////////////////////////////
uint32_t ADTS_GetBytes(CCI_Ctx *Ctx, int32_t n)
{
    unsigned char word[4];
    uint32_t retdata = 0;

    Ctx->cci_dec_read(0, (uint8_t *)&word[0], n, 0, Ctx->user_data);
    retdata = ((uint32_t)word[0] << 24);
    retdata |= ((uint32_t)word[1] << 16);
    retdata |= ((uint32_t)word[2] << 8);
    retdata |= ((uint32_t)word[3]);
    return retdata;
}

/////////////////////////////////////////////////////////////////////////////////
//!   \brief   Skip arbitrary number of bytes.
//!
//!   \fntype	Function
//!
//!   \param[in]  n amount of bytes to skip.
//!   \param[in]  iHandle handlenumber.
//!
//!   \return	None
//!   \note     Used by the decoder to skip arbitrary number of bytes.
/////////////////////////////////////////////////////////////////////////////////
void ADTS_SkipBytes(CCI_Ctx *Ctx, int32_t n)
{
    Ctx->cci_dec_seek(0, n, SEEK_CUR, Ctx->user_data);
}

/////////////////////////////////////////////////////////////////////////////////
//!   \brief   get current offset.
//!
//!   \fntype	Function
//!
//!   \param[in]  iHandle handlenumber.
//!
//!   \return	current offset
/////////////////////////////////////////////////////////////////////////////////
int32_t ADTS_GetCurrentOffset(CCI_Ctx *Ctx)
{
    return Ctx->cci_dec_tell(0, Ctx->user_data);
}

/////////////////////////////////////////////////////////////////////////////////
//!   \brief    Used to return sample rate (Hz) for given sample rate index.
//!
//!   \fntype	Function
//!
//!   \param[in]  iIdx index number for sample rate.
//!
//!   \return   sample rate (Hz).
//!   \return	0 in case of error.
//!   \note     Used by the decoder to return sample rate (Hz) for given sample rate index.
/////////////////////////////////////////////////////////////////////////////////
uint32_t ADTS_IndexToSampleRate(int Index)
{
    if ((unsigned)Index >= sizeof(stc_u32SamplingRateList) / sizeof(stc_u32SamplingRateList[0]))
    {
        return 0;
    }
    return stc_u32SamplingRateList[Index];
}

/////////////////////////////////////////////////////////////////////////////////
//!   \brief    Used to return Bit rate (bps).
//!
//!   \fntype	Function
//!
//!   \param[in]  iHandle handlenumber.
//!   \param[in]  frame_size first frame size.
//!   \param[in]  u32Samplingrate sample rate.
//!   \param[in]  u32EndBytes endbyte position.
//!
//!   \return   Bit rate (bps).
//!   \note     Used by the decoder to return Bit rate (bps).
/////////////////////////////////////////////////////////////////////////////////
uint32_t ADTS_Bitrate(CCI_Ctx *Ctx, uint32_t u32FrameSize, uint32_t u32Samplingrate) //,uint32_t u32EndBytes)
{
    uint32_t u32Bitrate = 0, u32TempFrameSize = 0, u32Frames = 1;
    uint32_t u32NextHeader;
    uint32_t u32TempEndBytes = (30 * 1024);
    uint32_t u32NextVh;
    int32_t i32NewSkipBytes;

    u32TempFrameSize += u32FrameSize;

    while (u32TempEndBytes > Ctx->cci_dec_tell(0, Ctx->user_data))
    {
        u32NextHeader = ADTS_GetBytes(Ctx, 4);
        u32NextVh     = ADTS_GetBytes(Ctx, 3);
        u32NextVh     = (u32NextVh >> 8);
        if ((u32NextHeader & 0xFFF60000) == 0xFFF00000)
        {
            uint32_t u32Index        = (u32NextHeader >> 10) & 15;
            uint32_t u32NewFramesize = ((u32NextHeader & 3) << 11) | ((u32NextVh >> 13) & 0x7FF);

            if (u32Index <= 11 && u32NewFramesize > 7)
            {
                u32TempFrameSize += u32NewFramesize;
                u32Frames       = u32Frames + 1;
                i32NewSkipBytes = (int32_t)(u32NewFramesize - (56 / 8));
                ADTS_SkipBytes(Ctx, i32NewSkipBytes);
            }
        }
        else
        {
            i32NewSkipBytes = 0 - 6;
            ADTS_SkipBytes(Ctx, i32NewSkipBytes);
        }
    };

    if (u32Frames > 0)
    {
        uint32_t u32SecPerFrame, u32BytesPerFrame;
        u32SecPerFrame   = u32Samplingrate / 1024;
        u32BytesPerFrame = u32TempFrameSize / u32Frames;
        u32Bitrate       = 8 * (u32BytesPerFrame * u32SecPerFrame);
    }
    return (u32Bitrate);
}

////////////////////////////////////////////////////////////////////////////////
// End of file
////////////////////////////////////////////////////////////////////////////////
//! @}
