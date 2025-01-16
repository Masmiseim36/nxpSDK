///////////////////////////////////////////////////////////////////////////////
//! \addtogroup asi_dffi
//! @{
//
// Copyright 2004-2005 SigmaTel, Inc.
// Copyright 2024 NXP
//
// SPDX-License-Identifier: BSD-3-Clause
//
//! \file asi_dffi_aac_extractmetadata.c
//! \brief Contains functionality that extracts the metadata from the file
//!                and stores it in the file_meta_data_t structure.
///////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Macros
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Includes
////////////////////////////////////////////////////////////////////////////////
#if defined(AAC_DEC) && !defined(__ICCARM__)
// #include <types.h>
#include <error.h>
#include "codec_interface.h"
#include "cci_metadata.h"
#include "aac_metadata.h"
#include <string.h>
// #include <wchar.h>
////////////////////////////////////////////////////////////////////////////////
//  External Definitions
////////////////////////////////////////////////////////////////////////////////
#define MOOV_BIT 1
#define MP4A_BIT 2
#define ALAC_BIT 2
#define STSZ_BIT 4
#define STCO_BIT 8
#define MDAT_BIT 16
#define TTL_BIT  0x20
#define ALB_BIT  0x40
#define ART_BIT  0x80
#define DAY_BIT  0x100
#define TRKN_BIT 0x200

// #define ALL_ATOMS (FTYP_BIT |MOOV_BIT| MP4A_BIT | STSZ_BIT | MDAT_BIT)
#define MANDATORY_ATOMS \
    (MOOV_BIT | MP4A_BIT | STSZ_BIT | STCO_BIT | MDAT_BIT) //| TTL_BIT | ALB_BIT | ART_BIT | DAY_BIT | TRKN_BIT )
// Atoms related to metadata information are added. (TTL_BIT, ALB_BIT, ART_BIT, DAY_BIT, TRKN_BIT)
#define ALL_ATOMS \
    (MOOV_BIT | MP4A_BIT | STSZ_BIT | STCO_BIT | MDAT_BIT | TTL_BIT | ALB_BIT | ART_BIT | DAY_BIT | TRKN_BIT)
#define AAC_FILEDEC_MAX_CHANNELS 48
#define ADTS_HEADER_THRESHOLD    15000

#define MKU32(a, b, c, d) (d | (c << 8) | (b << 16) | (a << 24))
#define HANDLER_video     MKU32('v', 'i', 'd', 'e')
#define NUMBER_OF_GENRES  149
////////////////////////////////////////////////////////////////////////////////
//    local function prototypes
///////////////////////////////////////////////////////////////////////////////
RtStatus_t AacParser(CCI_Ctx *Ctx, file_meta_data_t *MetaData, AAC_DFFI_t *ptrAac);
int32_t AacProcessChunk(int ckIDval, int32_t *AacCkSize, CCI_Ctx *Ctx, file_meta_data_t *MetaData, AAC_DFFI_t *ptrAac);
int32_t codec_extract_metadata_aac(file_meta_data_t *meta_data, CCI_Ctx *Ctx);
int32_t GetAACckID(CCI_Ctx *Ctx, AAC_DFFI_t *ptrAac);
RtStatus_t AdtsParser(CCI_Ctx *Ctx, file_meta_data_t *MetaData);
RtStatus_t AdifParser(CCI_Ctx *Ctx, file_meta_data_t *MetaData);
int AacReadDecoderSpecificInfo(const uint8_t *pDsi, int iDsiSize, file_meta_data_t *MetaData, AAC_DFFI_t *ptrAac);
// RETCODE Adts_APETag_Parser(int32_t i32Handle, file_meta_data_t *pMetaData);
////////////////////////////////////////////////////////////////////////////////
//  External References
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Global References
////////////////////////////////////////////////////////////////////////////////
const int32_t TITLE_MP4       = 0x6d703400;
const int32_t TITLE_M4A       = 0x4d344100;
const int32_t TITLE_M4A_LOWER = 0x6d346100;
const int32_t TITLE_M4B       = 0x4d344200;
const int32_t TITLE_M4B_LOWER = 0x6d346200;
const int32_t TITLE_3GP       = 0x33677000;
const int32_t TITLE_ISO       = 0x69736F00;

const int32_t Aacckidlist[TOTAL_CKID] = {
    0x6d6f6f76, // MOOV_CKID
    0x7374737a, // STSZ_CKID
    0x6d646174, // MDAT_CKID
    0x6d646961, // MDIA_CKID
    0x6d696e66, // MINF_CKID
    0x7472616b, // TRAK_CKID
    0x7374626c, // STBL_CKID
    0x66747970, // FTYP_CKID
    0x75647461, // UDTA_CKID
    0xa9616c62, // ALB_CKID
    0xa9415254, // ART_CKID
    0x73747364, // STSD_CKID
    0x6d703461, // MP4A_CKID
    0x696c7374, // LIST_CKID
    0xA96E616D,
    0x64726d73, // DRMS_CKID
    0x65736473, // ESDS_CKID         added ESDS atom 4 char code - AML 9/10/04
    0x73696E66, // SINF_CKID         added SINF atom 4 char code - AML 9/10/04
    0x6d657461, // META_CKID
    0x6d646864, // MDHD_CKID
    0xa9646179, // DAY_CKID
    0x74726b6e, // TRKN_CKID
    0x61766331, // AVC1_CKID
    0x6D703476, // MP4V_CKID
    0x68646c72, // HDLR_CKID
    0x7374636F, // STCO_CKID
    0x616c6163, // ALAC_CKID
    0xa967656e, // GEN_CKID
    0x676e7265, // GNRE_CKID
};

enum
{
    alac_decoder = 0x616c6163, //'alac',
    linear_pcm   = 0x6c70636d, //'lpcm'
    aac_decoder  = 0x6d703461  //'aac-lc'
};

static const uint16_t stc_genres[][18] = {
    {'B', 'l', 'u', 'e', 's', '\0'},
    {'C', 'l', 'a', 's', 's', 'i', 'c', ' ', 'R', 'o', 'c', 'k', '\0'},
    {'C', 'o', 'u', 'n', 't', 'r', 'y', '\0'},
    {'D', 'a', 'n', 'c', 'e', '\0'},
    {'D', 'i', 's', 'c', 'o', '\0'},
    {'F', 'u', 'n', 'k', '\0'},
    {'G', 'r', 'u', 'n', 'g', 'e', '\0'},
    {'H', 'i', 'p', '-', 'H', 'o', 'p', '\0'},
    {'J', 'a', 'z', 'z', '\0'},
    {'M', 'e', 't', 'a', 'l', '\0'},
    {'N', 'e', 'w', ' ', 'A', 'g', 'e', '\0'},
    {'O', 'l', 'd', 'i', 'e', 's', '\0'},
    {'O', 't', 'h', 'e', 'r', '\0'},
    {'P', 'o', 'p', '\0'},
    {'R', '&', 'B', '\0'},
    {'R', 'a', 'p', '\0'},
    {'R', 'e', 'g', 'g', 'a', 'e', '\0'},
    {'R', 'o', 'c', 'k', '\0'},
    {'T', 'e', 'c', 'h', 'n', 'o', '\0'},
    {'I', 'n', 'd', 'u', 's', 't', 'r', 'i', 'a', 'l', '\0'},
    {'A', 'l', 't', 'e', 'r', 'n', 'a', 't', 'i', 'v', 'e', '\0'},
    {'S', 'k', 'a', '\0'},
    {'D', 'e', 'a', 't', 'h', ' ', 'M', 'e', 't', 'a', 'l', '\0'},
    {'P', 'r', 'a', 'n', 'k', 's', '\0'},
    {'S', 'o', 'u', 'n', 'd', 't', 'r', 'a', 'c', 'k', '\0'},
    {'E', 'u', 'r', 'o', '-', 'T', 'e', 'c', 'h', 'n', 'o', '\0'},
    {'A', 'm', 'b', 'i', 'e', 'n', 't', '\0'},
    {'T', 'r', 'i', 'p', '-', 'H', 'o', 'p', '\0'},
    {'V', 'o', 'c', 'a', 'l', '\0'},
    {'J', 'a', 'z', 'z', '+', 'F', 'u', 'n', 'k', '\0'},
    {'F', 'u', 's', 'i', 'o', 'n', '\0'},
    {'T', 'r', 'a', 'n', 'c', 'e', '\0'},
    {'C', 'l', 'a', 's', 's', 'i', 'c', 'a', 'l', '\0'},
    {'I', 'n', 's', 't', 'r', 'u', 'm', 'e', 'n', 't', 'a', 'l', '\0'},
    {'A', 'c', 'i', 'd', '\0'},
    {'H', 'o', 'u', 's', 'e', '\0'},
    {'G', 'a', 'm', 'e', '\0'},
    {'S', 'o', 'u', 'n', 'd', ' ', 'C', 'l', 'i', 'p', '\0'},
    {'G', 'o', 's', 'p', 'e', 'l', '\0'},
    {'N', 'o', 'i', 's', 'e', '\0'},
    {'A', 'l', 't', 'e', 'r', 'n', 'R', 'o', 'c', 'k', '\0'},
    {'B', 'a', 's', 's', '\0'},
    {'S', 'o', 'u', 'l', '\0'},
    {'P', 'u', 'n', 'k', '\0'},
    {'S', 'p', 'a', 'c', 'e', '\0'},
    {'M', 'e', 'd', 'i', 't', 'a', 't', 'i', 'v', 'e', '\0'},
    {'I', 'n', 's', 't', 'r', 'u', 'm', 'e', 'n', 't', 'a', 'l', ' ', 'P', 'o', 'p', '\0'},
    {'I', 'n', 's', 't', 'r', 'u', 'm', 'e', 'n', 't', 'a', 'l', ' ', 'R', 'o', 'c', 'k', '\0'},
    {'E', 't', 'h', 'n', 'i', 'c', '\0'},
    {'G', 'o', 't', 'h', 'i', 'c', '\0'},
    {'D', 'a', 'r', 'k', 'w', 'a', 'v', 'e', '\0'},
    {'T', 'e', 'c', 'h', 'n', 'o', '-', 'I', 'n', 'd', 'u', 's', 't', 'r', 'i', 'a', 'l', '\0'},
    {'E', 'l', 'e', 'c', 't', 'r', 'o', 'n', 'i', 'c', '\0'},
    {'P', 'o', 'p', '-', 'F', 'o', 'l', 'k', '\0'},
    {'E', 'u', 'r', 'o', 'd', 'a', 'n', 'c', 'e', '\0'},
    {'D', 'r', 'e', 'a', 'm', '\0'},
    {'S', 'o', 'u', 't', 'h', 'e', 'r', 'n', ' ', 'R', 'o', 'c', 'k', '\0'},
    {'C', 'o', 'm', 'e', 'd', 'y', '\0'},
    {'C', 'u', 'l', 't', '\0'},
    {'G', 'a', 'n', 'g', 's', 't', 'a', '\0'},
    {'T', 'o', 'p', ' ', '4', '0', '\0'},
    {'C', 'h', 'r', 'i', 's', 't', 'i', 'a', 'n', ' ', 'R', 'a', 'p', '\0'},
    {'P', 'o', 'p', '/', 'F', 'u', 'n', 'k', '\0'},
    {'J', 'u', 'n', 'g', 'l', 'e', '\0'},
    {'N', 'a', 't', 'i', 'v', 'e', ' ', 'A', 'm', 'e', 'r', 'i', 'c', 'a', 'n', '\0'},
    {'C', 'a', 'b', 'a', 'r', 'e', 't', '\0'},
    {'N', 'e', 'w', ' ', 'W', 'a', 'v', 'e', '\0'},
    {'P', 's', 'y', 'c', 'h', 'e', 'd', 'e', 'l', 'i', 'c', '\0'},
    {'R', 'a', 'v', 'e', '\0'},
    {'S', 'h', 'o', 'w', 't', 'u', 'n', 'e', 's', '\0'},
    {'T', 'r', 'a', 'i', 'l', 'e', 'r', '\0'},
    {'L', 'o', '-', 'F', 'i', '\0'},
    {'T', 'r', 'i', 'b', 'a', 'l', '\0'},
    {'A', 'c', 'i', 'd', ' ', 'P', 'u', 'n', 'k', '\0'},
    {'A', 'c', 'i', 'd', ' ', 'J', 'a', 'z', 'z', '\0'},
    {'P', 'o', 'l', 'k', 'a', '\0'},
    {'R', 'e', 't', 'r', 'o', '\0'},
    {'M', 'u', 's', 'i', 'c', 'a', 'l', '\0'},
    {'R', 'o', 'c', 'k', ' ', '&', ' ', 'R', 'o', 'l', 'l', '\0'},
    {'H', 'a', 'r', 'd', ' ', 'R', 'o', 'c', 'k', '\0'},
    {'F', 'o', 'l', 'k', '\0'},
    {'F', 'o', 'l', 'k', '-', 'R', 'o', 'c', 'k', '\0'},
    {'N', 'a', 't', 'i', 'o', 'n', 'a', 'l', ' ', 'F', 'o', 'l', 'k', '\0'},
    {'S', 'w', 'i', 'n', 'g', '\0'},
    {'F', 'a', 's', 't', ' ', 'F', 'u', 's', 'i', 'o', 'n', '\0'},
    {'B', 'e', 'b', 'o', 'b', '\0'},
    {'L', 'a', 't', 'i', 'n', '\0'},
    {'R', 'e', 'v', 'i', 'v', 'a', 'l', '\0'},
    {'C', 'e', 'l', 't', 'i', 'c', '\0'},
    {'B', 'l', 'u', 'e', 'g', 'r', 'a', 's', 's', '\0'},
    {'A', 'v', 'a', 'n', 't', 'g', 'a', 'r', 'd', 'e', '\0'},
    {'G', 'o', 't', 'h', 'i', 'c', ' ', 'R', 'o', 'c', 'k', '\0'},
    {'P', 'r', 'o', 'g', 'r', 'e', 's', 's', 'i', 'v', 'e', ' ', 'R', 'o', 'c', 'k', '\0'},
    {'P', 's', 'y', 'c', 'h', 'e', 'd', 'e', 'l', 'i', 'c', ' ', 'R', 'o', 'c', 'k', '\0'},
    {'S', 'y', 'm', 'p', 'h', 'o', 'n', 'i', 'c', ' ', 'R', 'o', 'c', 'k', '\0'},
    {'S', 'l', 'o', 'w', ' ', 'R', 'o', 'c', 'k', '\0'},
    {'B', 'i', 'g', ' ', 'B', 'a', 'n', 'd', '\0'},
    {'C', 'h', 'o', 'r', 'u', 's', '\0'},
    {'E', 'a', 's', 'y', ' ', 'L', 'i', 's', 't', 'e', 'n', 'i', 'n', 'g', '\0'},
    {'A', 'c', 'o', 'u', 's', 't', 'i', 'c', '\0'},
    {'H', 'u', 'm', 'o', 'u', 'r', '\0'},
    {'S', 'p', 'e', 'e', 'c', 'h', '\0'},
    {'C', 'h', 'a', 'n', 's', 'o', 'n', '\0'},
    {'O', 'p', 'e', 'r', 'a', '\0'},
    {'C', 'h', 'a', 'm', 'b', 'e', 'r', ' ', 'M', 'u', 's', 'i', 'c', '\0'},
    {'S', 'o', 'n', 'a', 't', 'a', '\0'},
    {'S', 'y', 'm', 'p', 'h', 'o', 'n', 'y', '\0'},
    {'B', 'o', 'o', 't', 'y', ' ', 'B', 'a', 's', 's', '\0'},
    {'P', 'r', 'i', 'm', 'u', 's', '\0'},
    {'P', 'o', 'r', 'n', ' ', 'G', 'r', 'o', 'o', 'v', 'e', '\0'},
    {'S', 'a', 't', 'i', 'r', 'e', '\0'},
    {'S', 'l', 'o', 'w', ' ', 'J', 'a', 'm', '\0'},
    {'C', 'l', 'u', 'b', '\0'},
    {'T', 'a', 'n', 'g', 'o', '\0'},
    {'S', 'a', 'm', 'b', 'a', '\0'},
    {'F', 'o', 'l', 'k', 'l', 'o', 'r', 'e', '\0'},
    {'B', 'a', 'l', 'l', 'a', 'd', '\0'},
    {'P', 'o', 'w', 'e', 'r', ' ', 'B', 'a', 'l', 'l', 'a', 'd', '\0'},
    {'R', 'h', 'y', 't', 'h', 'm', 'i', 'c', ' ', 'S', 'o', 'u', 'l', '\0'},
    {'F', 'r', 'e', 'e', 's', 't', 'y', 'l', 'e', '\0'},
    {'D', 'u', 'e', 't', '\0'},
    {'P', 'u', 'n', 'k', ' ', 'R', 'o', 'c', 'k', '\0'},
    {'D', 'r', 'u', 'm', ' ', 'S', 'o', 'l', 'o', '\0'},
    {'A', 'c', 'a', 'p', 'e', 'l', 'l', 'a', '\0'},
    {'E', 'u', 'r', 'o', '-', 'H', 'o', 'u', 's', 'e', '\0'},
    {'D', 'a', 'n', 'c', 'e', ' ', 'H', 'a', 'l', 'l', '\0'},
    {'G', 'o', 'a', '\0'},
    {'D', 'r', 'u', 'm', ' ', '&', ' ', 'B', 'a', 's', 's', '\0'},
    {'C', 'l', 'u', 'b', '-', 'H', 'o', 'u', 's', 'e', '\0'},
    {'H', 'a', 'r', 'd', 'c', 'o', 'r', 'e', '\0'},
    {'T', 'e', 'r', 'r', 'o', 'r', '\0'},
    {'I', 'n', 'd', 'i', 'e', '\0'},
    {'B', 'r', 'i', 't', 'p', 'o', 'p', '\0'},
    {'N', 'e', 'g', 'e', 'r', 'p', 'u', 'n', 'k', '\0'},
    {'P', 'o', 'l', 's', 'k', ' ', 'P', 'u', 'n', 'k', '\0'},
    {'B', 'e', 'a', 't', '\0'},
    {'C', 'h', 'r', 'i', 's', 't', 'i', 'a', 'n', ' ', 'G', 'a', 'n', 'g', 's', 't', 'a', '\0'},
    {'H', 'e', 'a', 'v', 'y', ' ', 'M', 'e', 't', 'a', 'l', '\0'},
    {'B', 'l', 'a', 'c', 'k', ' ', 'M', 'e', 't', 'a', 'l', '\0'},
    {'C', 'r', 'o', 's', 's', 'o', 'v', 'e', 'r', '\0'},
    {'C', 'o', 'n', 't', 'e', 'm', 'p', 'o', 'r', 'a', 'r', 'y', ' ', 'C', '\0'},
    {'C', 'h', 'r', 'i', 's', 't', 'i', 'a', 'n', ' ', 'R', 'o', 'c', 'k', '\0'},
    {'M', 'e', 'r', 'e', 'n', 'g', 'u', 'e', '\0'},
    {'S', 'a', 'l', 's', 'a', '\0'},
    {'T', 'h', 'r', 'a', 's', 'h', ' ', 'M', 'e', 't', 'a', 'l', '\0'},
    {'A', 'n', 'i', 'm', 'e', '\0'},
    {'J', 'P', 'o', 'p', '\0'},
    {'S', 'y', 'n', 't', 'h', ' ', 'P', 'o', 'p', '\0'},
    {'S', 'w', 'e', 'e', 't', 'c', 'o', 'r', 'n', ' ', 'D', 'u', 'b', '\0'},
    {'D', 'a', 'n', 'c', 'e', '\0'},
    {'D', 'i', 's', 'c', 'o', '\0'},
    {'F', 'u', 'n', 'k', '\0'},
    {'G', 'r', 'u', 'n', 'g', 'e', '\0'},
    {'H', 'i', 'p', '-', 'H', 'o', 'p', '\0'},
    {'J', 'a', 'z', 'z', '\0'},
    {'M', 'e', 't', 'a', 'l', '\0'},
};
////////////////////////////////////////////////////////////////////////////////
//  Equates
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Prototypes
////////////////////////////////////////////////////////////////////////////////
extern int GetAacWord(CCI_Ctx *Ctx, AAC_DFFI_t *ptrAac);
extern int GetAACDWord(CCI_Ctx *Ctx, AAC_DFFI_t *ptrAac);
extern int GetAACByte(CCI_Ctx *Ctx, AAC_DFFI_t *ptrAac);
extern void SkipAACData(long AacCkSize, CCI_Ctx *Ctx, AAC_DFFI_t *ptrAac);

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
uint8_t aac_copy_one_byte(char *ptr, char *ptr1)
{
    *ptr1 = *ptr;
    return 1;
}
////////////////////////////////////////////////////////////////////////////////
//! \brief get file size Callback API for all metadata extraction.
//!
//! \fntype Function
//!
//! \param[in] *Ctx         Pointer to CCI Context structure
//!
//! \retval File Size.
////////////////////////////////////////////////////////////////////////////////
int32_t aac_GetFileSize(CCI_Ctx *Ctx)
{
    int32_t offset;
    int32_t size;

    offset = Ctx->cci_dec_tell(0, Ctx->user_data);
    Ctx->cci_dec_seek(0, 0, SEEK_END, Ctx->user_data);
    size = Ctx->cci_dec_tell(0, Ctx->user_data);
    Ctx->cci_dec_seek(0, offset, SEEK_SET, Ctx->user_data);

    return size;
}
uint64_t BigNum_Divide_64_by_32_aac(uint64_t n, uint32_t base)
{
    uint64_t rem = n;
    uint64_t b   = base;
    uint64_t res, d = 1;
    uint32_t high = rem >> 32;

    /* Reduce the thing a bit first */
    res = 0;
    if (high >= base)
    {
        high /= base;
        res = (uint64_t)high << 32;
        rem -= (uint64_t)(high * base) << 32;
    }

    while ((int64_t)b > 0 && b < rem)
    {
        b = b + b;
        d = d + d;
    }

    do
    {
        if (rem >= b)
        {
            rem -= b;
            res += d;
        }
        b >>= 1;
        d >>= 1;
    } while (d);

    // n = res;
    return res;
}

/////////////////////////////////////////////////////////////////////////////////
//!  \brief Extract the meta data.
//!
//!  \fntype         Function
//!
//!  \param[in]      file_handle -- this key allows the file to be quickly found and opened
//!  \param[in]      *MetaData -- this structure will be filled in by the function
//!
//!  \retval         SUCCESS
//!  \retval         ERROR_ASI_DFFI_METADATA_FILETYPE_FAILED -- metadata failed
//!  \retval         ERROR_OS_FILESYSTEM_FILE_OPEN -- the file failed to open
//!
//!  \note           This should be accessed via the generic function DffiExtractMetaData.
//!                  the pathname pointer must be passed in as an integer because of the calling conventions of
//!                  SysCallFunction.  It holds the numeric value of the the uint8_t pointer.
//!
//!
/////////////////////////////////////////////////////////////////////////////////
int32_t codec_extract_metadata_aac(file_meta_data_t *MetaData, CCI_Ctx *Ctx)
{
    RtStatus_t Retcode = SUCCESS;
    uint8_t aac_buf[140];
    AAC_DFFI_t *pAacdffi;

    /* Set some defaults */
    MetaData->start_pos        = 0;
    MetaData->total_time       = 0;
    MetaData->valid_vbr_header = 0;
    MetaData->avg_bit_rate     = 0;
    pAacdffi                   = (AAC_DFFI_t *)&aac_buf[0];

    memset((void *)pAacdffi, 0, sizeof(AAC_DFFI_t));

    /* Get the number of bytes in song (updated as all the headers are found) */
    MetaData->num_bytes = aac_GetFileSize(Ctx);
    MetaData->start_pos = 0;

    Retcode = AacParser(Ctx, MetaData, pAacdffi);
    char *genre, *album, *artist;
    int j;

    for (j = 0; j < CASCFG_TRACK_GENRE_LEN; j++)
    {
        genre = (char *)(&MetaData->song_genre);
        if (j == 0)
        {
            if ((genre[j] >= 'a') && (genre[j] <= 'z'))
            {
                genre[j] -= ('a' - 'A');
            }
        }
        else
        {
            if ((genre[j] >= 'A') && (genre[j] <= 'Z'))
            {
                genre[j] += ('a' - 'A');
            }
        }
    }
    for (j = 0; j < CASCFG_TRACK_ALBUM_LEN; j++)
    {
        album = (char *)(&MetaData->song_album);
        if (j == 0)
        {
            if ((album[j] >= 'a') && (album[j] <= 'z'))
            {
                album[j] -= ('a' - 'A');
            }
        }
        else
        {
            if ((album[j] >= 'A') && (album[j] <= 'Z'))
            {
                album[j] += ('a' - 'A');
            }
        }
    }
    for (j = 0; j < CASCFG_TRACK_ARTIST_LEN; j++)
    {
        artist = (char *)(&MetaData->song_artist);
        if (j == 0)
        {
            if ((artist[j] >= 'a') && (artist[j] <= 'z'))
            {
                artist[j] -= ('a' - 'A');
            }
        }
        else
        {
            if ((artist[j] >= 'A') && (artist[j] <= 'Z'))
            {
                artist[j] += ('a' - 'A');
            }
        }
    }

    if (Retcode == SUCCESS)
    {
        /* we know this is an aac file, so fill in file type */
        if (MetaData->stream_type == STREAM_TYPE_AAC)
        {
            MetaData->num_channels    = 2;
            MetaData->bits_per_sample = 16;
        }
        MetaData->sample_rate  = pAacdffi->Aac_SamplingRate;
        MetaData->bit_rate     = pAacdffi->Aac_BitRate;
        MetaData->avg_bit_rate = MetaData->bit_rate;
        // starting positon of Audiodata
        MetaData->start_pos  = pAacdffi->AacDataBegin[pAacdffi->AacMDATCount];
        MetaData->total_time = (uint32_t)BigNum_Divide_64_by_32_aac(
            ((uint64_t)pAacdffi->AacMDATSize[pAacdffi->AacMDATCount] * 8 * 1000),
            (MetaData->bit_rate)); //(((uint64_t)pAacdffi->AacMDATSize[pAacdffi->AacMDATCount] * 8 *1000)/
                                   //(MetaData->bit_rate)); // convert to mSec
        MetaData->audio_sub_type = MEDIA_SUBTYPE_M4A;
    }
    else
    {
        Ctx->cci_dec_seek(0, 0, SEEK_SET, Ctx->user_data);
        Retcode = AdtsParser(Ctx, MetaData);
        if (Retcode == SUCCESS)
        {
            MetaData->stream_type     = STREAM_TYPE_AAC;
            MetaData->audio_sub_type  = MEDIA_SUBTYPE_ADTS;
            MetaData->bitrate_type    = VARIABLE_BITRATE;
            MetaData->bit_rate        = 0; // To Fix -> SDK-2413
            MetaData->bits_per_sample = 16;
            MetaData->total_time      = 0;
        }
        else
        {
            Ctx->cci_dec_seek(0, 0, SEEK_SET, Ctx->user_data);
            Retcode = AdifParser(Ctx, MetaData);
            if (Retcode == SUCCESS)
            {
                MetaData->stream_type    = STREAM_TYPE_AAC;
                MetaData->audio_sub_type = MEDIA_SUBTYPE_ADIF;
                MetaData->avg_bit_rate   = MetaData->bit_rate;
                MetaData->total_time = (uint32_t)BigNum_Divide_64_by_32_aac(((uint64_t)MetaData->num_bytes * 8 * 1000),
                                                                            (MetaData->bit_rate));
                MetaData->bits_per_sample = 16;
            }
        }
    }

    return Retcode;
}

///////////////////////////////////////////////////////////////////////////////
//!
//!  \brief   Parses up to Data chunk.
//!
//!  \fntype  Function
//!
//!  \param[in]  iHandle File handler
//!  \param[in]  MetaData contents
//!
//!  \retval 0 if success
//!
//!  \note Can be overlaid
//!
//////////////////////////////////////////////////////////////////////////////

RtStatus_t AacParser(CCI_Ctx *Ctx, file_meta_data_t *MetaData, AAC_DFFI_t *ptrAac)
{
    int32_t ret_val                    = 0;
    int32_t ckIDval                    = 0;
    int32_t ckid_found                 = 0;
    int32_t i32OffsetOfFirstValidChunk = 0;
    int32_t i;
    ptrAac->MandatoryAtoms = 0;
    ptrAac->AllAtoms       = 0;

    ptrAac->aac_file_flag   = 0;
    ptrAac->overwrite       = 1;
    ptrAac->tracktype       = 1;
    ptrAac->AACMetaDataFlag = 0;
    ptrAac->AacCkSize       = 0;
    ptrAac->chunkcount      = 0;
    ptrAac->AacFlag         = 0;

    ptrAac->aac_file_offset = 0;

    // initialize MDAT offset array to -1
    for (i = 0; i < MAX_MDAT_ATOM; i++)
        ptrAac->AacDataBegin[i] = -1;

    //  this code is to reach to the first known atom, because some files have other non-atom data at start of the file.
    for (i = 0; (i < 1024 && i < MetaData->num_bytes); i++)
    //  for ( i = 0; i < 1024 ; i++ )
    {
        ckIDval = GetAACckID(Ctx, ptrAac);
        if (ckIDval != UNKNOWN_CKID)
        {
            // Found a valid Ckid so break and try to search for other atoms
            //
            SkipAACData(-8, Ctx, ptrAac);
            ckid_found = 1;

            // save offset of first valid chunk
            i32OffsetOfFirstValidChunk = ptrAac->aac_file_offset;
            break;
        }
        else
        {
            SkipAACData(-7, Ctx, ptrAac);
        }
    }

    // We searched first 512 bytes and could not find a valid atom from our list. This is a compromise setting.
    // If there are other known atoms likely, add them to the list to ensure a faster search
    if (ckid_found == 0)
        return CODEC_METADATA_FILETYPE_FAILED;

    while (ptrAac->AllAtoms ^ ALL_ATOMS) // Returns -1 on error
    {
        // Gets the size and type of the next atom in the file
        // Size is stored in a global AacCkSize, the type is converted from 4 char code
        // to an index defined in aacdefs.h.
        ckIDval = GetAACckID(Ctx, ptrAac);

        // Skip unknown chunks
        if (ckIDval == UNKNOWN_CKID)
        {
            if (((ptrAac->AacCkSize + ptrAac->aac_file_offset) > MetaData->num_bytes) || (ptrAac->AacCkSize < 0))
            {
                if (ptrAac->aac_file_offset >= MetaData->num_bytes)
                {
                    // If entire file is traversed through and 5 compulsory atoms are found
                    // then the file should be considered as a valid file.
                    if (!(ptrAac->MandatoryAtoms ^ MANDATORY_ATOMS))
                        break;
                    else
                        return CODEC_METADATA_FILETYPE_FAILED;
                }
                else
                {
                    SkipAACData(-7, Ctx, ptrAac);
                }
            }
            else
            {
                SkipAACData((long)ptrAac->AacCkSize, Ctx, ptrAac); // Align the file reads correctly.
            }
        }
        else
        {
            // Handle each known atom, leaving the parser at the beginning of the
            // appropriate next atom.
            ret_val = AacProcessChunk(ckIDval, &ptrAac->AacCkSize, Ctx, MetaData, ptrAac);
            if (ret_val == -1)
                return CODEC_METADATA_FILETYPE_FAILED;
        }
    }

    if (!(ptrAac->MandatoryAtoms ^ MANDATORY_ATOMS)) // data found // 5 = compulsory atoms
    {
        int i = 0;
        if (ptrAac->AacMDATCount > 1)
        {
            for (i = 0; i < MAX_MDAT_ATOM; i++)
            {
                if ((ptrAac->AacDataBegin[i] - i32OffsetOfFirstValidChunk) == MetaData->start_pos)
                {
                    ptrAac->AacMDATCount = i;
                    break;
                }
            }
        }
        else
        {
            ptrAac->AacMDATCount = i;
            MetaData->start_pos  = ptrAac->AacDataBegin[i];
        }

        // no valid mdat found.
        if (i >= MAX_MDAT_ATOM)
            return CODEC_METADATA_FILETYPE_FAILED;

        ptrAac->Aac_BitRate = BigNum_Divide_64_by_32_aac(
            (int64_t)(ptrAac->AacMDATSize[ptrAac->AacMDATCount] * 8 * ptrAac->Aac_TimeScale),
            (ptrAac->Aac_Duration)); // BigNum_Divide_64_by_32_aac(((int64_t)(ptrAac->AacMDATSize[ptrAac->AacMDATCount]
                                     // * ptrAac->Aac_SamplingRate )) , ( 128 *  ptrAac->Aac_NumSamples ));
    }

    if (ptrAac->aac_file_flag < 2) // error in file
        return CODEC_METADATA_FILETYPE_FAILED;

    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
//!  \brief Processes a chunk
//!
//!  \fntype Function
//!
//!  \param[in] ckIDval Chunk ID value
//!  \param[in] AacCkSize Chunk size
//!  \param[in] iHandle File handler
//!  \param[in] MetaData File mata data
//!
//!  \retval 0 = if there are no nested chunks.
//!  \retval 1 = if there are nested chunks
//!  \retval 2 = if it is a Data chunk
//!  \retval -1 = FAILED
//!  \note Can be overlaid
//////////////////////////////////////////////////////////////////////////////

int32_t AacProcessChunk(int ckIDval, int32_t *AacCkSize, CCI_Ctx *Ctx, file_meta_data_t *MetaData, AAC_DFFI_t *ptrAac)
{
    int32_t i;
    int32_t ret_val = 0;
    uint8_t u8Temp;
    uint8_t dsi[8];
    int dsiSize;
    uint32_t u32Counter = 0;
    uint32_t u32Handler = 0;
    int32_t filetype;

    switch (ckIDval)
    {
        case FTYP_CKID:
            SkipAACData(8, Ctx, ptrAac);
            filetype = GetAACDWord(Ctx, ptrAac);
            filetype = filetype & 0xffffff00;
            if ((*AacCkSize - 12) > 0)
                SkipAACData(*AacCkSize - 12, Ctx, ptrAac);
            if ((filetype != TITLE_M4A) && (filetype != TITLE_M4A_LOWER) && (filetype != TITLE_MP4) &&
                (filetype != TITLE_M4B) && (filetype != TITLE_M4B_LOWER) && (filetype != TITLE_3GP) &&
                (filetype != TITLE_ISO))
            {
                ret_val = -1; // file corrupted
            }
            break;
        case MOOV_CKID:
            ptrAac->aac_file_flag++; // this atom is compulsary indicates file is valid
            ptrAac->MandatoryAtoms |= MOOV_BIT;
            ptrAac->AllAtoms |= MOOV_BIT;
            break;

        case STBL_CKID:
            if (!ptrAac->overwrite)
                SkipAACData(*AacCkSize, Ctx, ptrAac);
            break;

        case DRMS_CKID:
        case MP4A_CKID:
            ptrAac->aac_file_flag++;
            SkipAACData(28, Ctx, ptrAac); // skips to time scale
            ptrAac->tracktype     = 0;    // tracktype MP4A
            ptrAac->overwrite     = 0;
            MetaData->stream_type = STREAM_TYPE_AAC;
            ptrAac->MandatoryAtoms |= MP4A_BIT;
            ptrAac->AllAtoms |= MP4A_BIT;
            break;
        case ALAC_CKID:
            ptrAac->aac_file_flag++;
            // Skip ALAC specific Info
            SkipAACData(36, Ctx, ptrAac);
            MetaData->stream_type = STREAM_TYPE_AAC;
            // Skip Version flag(4 byte), frame length(4 byte) and compatibleVersion(1 byte)
            SkipAACData(9, Ctx, ptrAac);
            MetaData->bits_per_sample = GetAACByte(Ctx, ptrAac);
            SkipAACData(3, Ctx, ptrAac);
            MetaData->num_channels = GetAACByte(Ctx, ptrAac);
            SkipAACData(10, Ctx, ptrAac);
            MetaData->sample_rate = GetAACDWord(Ctx, ptrAac);
            SkipAACData(*AacCkSize - 36 - 9 - 1 - 3 - 1 - 10 - 4, Ctx, ptrAac);

            ptrAac->MandatoryAtoms |= ALAC_BIT;
            ptrAac->AllAtoms |= ALAC_BIT;
            break;

        case HDLR_CKID:
            SkipAACData(8, Ctx, ptrAac);
            u32Handler = GetAACDWord(Ctx, ptrAac);
            if (u32Handler == HANDLER_video) // If Video Handler then Do not append
            {
                ret_val = -1;
            }
            SkipAACData(*AacCkSize - 12, Ctx, ptrAac);
            break; // skips to time scale

        case AVC1_CKID:
        case MP4V_CKID:
            SkipAACData(*AacCkSize, Ctx, ptrAac);
            ret_val = -1;
            break; // skips to time scale

        case STSD_CKID:
            SkipAACData(8, Ctx, ptrAac);
            if (ptrAac->tracktype == 1)
                ptrAac->overwrite = 1;
            break;

        case ESDS_CKID: // added ESDS case, inside DRMS/MP4A atom - AML 9/10/04

            // Flags
            GetAACDWord(Ctx, ptrAac);
            u32Counter = 4;
            // ES descriptor type
            u8Temp = GetAACByte(Ctx, ptrAac);
            ++u32Counter;

            if (u8Temp == 0x03)
            {
                // Have we got a tag, or the length?
                u8Temp = GetAACByte(Ctx, ptrAac);
                ++u32Counter;

                if (u8Temp == 0x80 || u8Temp == 0xFE)
                {
                    // Skip forward the two remaining bytes
                    SkipAACData(2, Ctx, ptrAac);
                    u32Counter += 2;

                    u8Temp = GetAACByte(Ctx, ptrAac);
                    ++u32Counter;
                }

                // ID
                GetAacWord(Ctx, ptrAac);
                u32Counter += 2;

                // Stream priority
                u8Temp = GetAACByte(Ctx, ptrAac);
                ++u32Counter;

                // Decoder config descriptor
                u8Temp = GetAACByte(Ctx, ptrAac);
                ++u32Counter;

                // Check the descriptor is 0x04
                if (u8Temp == 0x04)
                {
                    // Have we got a tag, or the length?
                    u8Temp = GetAACByte(Ctx, ptrAac);
                    ++u32Counter;

                    if (u8Temp == 0x80 || u8Temp == 0xFE)
                    {
                        // Skip forward the two remaining bytes
                        SkipAACData(2, Ctx, ptrAac);
                        u32Counter += 2;

                        u8Temp = GetAACByte(Ctx, ptrAac);
                        ++u32Counter;
                    }

                    // Object type ID
                    u8Temp = GetAACByte(Ctx, ptrAac);
                    ++u32Counter;

                    // read 1 byte version string and 3-byte flag string
                    GetAACDWord(Ctx, ptrAac);
                    u32Counter += 4;

                    // Max bitrate
                    GetAACDWord(Ctx, ptrAac);
                    u32Counter += 4;

                    // Avg bitrate
                    GetAACDWord(Ctx, ptrAac);
                    u32Counter += 4;

                    // Decoder specific descriptor
                    u8Temp = GetAACByte(Ctx, ptrAac);
                    ++u32Counter;

                    // Have we got a tag, or the length?
                    u8Temp = GetAACByte(Ctx, ptrAac);
                    ++u32Counter;

                    if (u8Temp == 0x80 || u8Temp == 0xFE)
                    {
                        // Skip forward the two remaining bytes
                        SkipAACData(2, Ctx, ptrAac);
                        u32Counter += 2;

                        u8Temp = GetAACByte(Ctx, ptrAac);
                        ++u32Counter;
                    }

                    // This is the all important descriptor config info (well, at least for the codec)
                    dsiSize = u8Temp;

                    // Save it off...
                    for (i = 0; i < dsiSize && i < 8; i++)
                    {
                        dsi[i] = GetAACByte(Ctx, ptrAac);
                        ++u32Counter;
                    }

                    // calculate bytes to skip and skips that bytes to correctly move to next atom.
                    // 8 bytes are for atom size(4 bytes) and atom type('e','s','d','s' (4 bytes))
                    u32Counter = ((uint32_t)(*AacCkSize - u32Counter));
                    // Skip forward the remaining bytes as we are not using any value so skip it instead of parsing it.
                    SkipAACData(u32Counter, Ctx, ptrAac);

                    ret_val = AacReadDecoderSpecificInfo(dsi, dsiSize, MetaData, ptrAac);
                }
            }

            break;

        case STSZ_CKID:
            // skip first 8 bytes of data
            SkipAACData(8, Ctx, ptrAac);
            // get the number of samples
            ptrAac->Aac_NumSamples = GetAACDWord(Ctx, ptrAac);
            // skip the remaining data of the atom
            SkipAACData(*AacCkSize - 8 - 4, Ctx, ptrAac);
            ptrAac->MandatoryAtoms |= STSZ_BIT;
            ptrAac->AllAtoms |= STSZ_BIT;
            // normal return
            break;

        case STCO_CKID:
            // skip first 8 bytes of data
            SkipAACData(8, Ctx, ptrAac);
            // this should be same as mdat offset
            MetaData->start_pos = GetAACDWord(Ctx, ptrAac);
            // skip the remaining data of the atom
            SkipAACData(*AacCkSize - 8 - 4, Ctx, ptrAac);
            ptrAac->MandatoryAtoms |= STCO_BIT;
            ptrAac->AllAtoms |= STCO_BIT;
            break;

        case TTL_CKID: // get title name
            SkipAACData(16, Ctx, ptrAac);
            *AacCkSize = *AacCkSize - 16;
            {
                char cMetadataString[TITLE_SIZE];
                char *pString = cMetadataString;
                int32_t i32i;

                for (i = 0; i < min(TITLE_SIZE - 1, *AacCkSize); i++)
                    cMetadataString[i] = GetAACByte(Ctx, ptrAac);
                cMetadataString[i] = 0;
                if (*AacCkSize > TITLE_SIZE - 1)
                    SkipAACData(*AacCkSize - TITLE_SIZE - 1, Ctx, ptrAac);

                // Convert to UTF16
                for (i32i = 0; i32i < TITLE_SIZE; i32i++)
                {
                    uint8_t uBytesConsumed = aac_copy_one_byte(pString, (char *)&MetaData->song_title[i32i]);
                    if (*pString == 0)
                    {
                        break;
                    }
                    pString += uBytesConsumed;
                }
            }

            ptrAac->AACMetaDataFlag = 1;
            ptrAac->AllAtoms |= TTL_BIT;
            break;

        case ALB_CKID: // get album name
            SkipAACData(16, Ctx, ptrAac);
            *AacCkSize = *AacCkSize - 16;
            {
                char cMetadataString[ALBUM_SIZE];
                char *pString = cMetadataString;
                int32_t i32i;

                for (i = 0; i < min(ALBUM_SIZE - 1, *AacCkSize); i++)
                    cMetadataString[i] = GetAACByte(Ctx, ptrAac);
                cMetadataString[i] = 0;

                if (*AacCkSize > ALBUM_SIZE - 1)
                    SkipAACData(*AacCkSize - ALBUM_SIZE - 1, Ctx, ptrAac);

                // Convert to UTF16
                for (i32i = 0; i32i < ALBUM_SIZE; i32i++)
                {
                    uint8_t uBytesConsumed = aac_copy_one_byte(pString, (char *)&MetaData->song_album[i32i]);
                    if (*pString == 0)
                    {
                        break;
                    }
                    pString += uBytesConsumed;
                }

            }
            // printf("Album:                 %s \n", MetaData->wAlbum);
            ptrAac->AACMetaDataFlag = 1;
            ptrAac->AllAtoms |= ALB_BIT;
            break;

        case GEN_CKID: // get genre name
        case GNRE_CKID:
            SkipAACData(16, Ctx, ptrAac);
            *AacCkSize = *AacCkSize - 16;
            {
                char cMetadataString[GENRE_SIZE];
                char *pString = cMetadataString;
                int32_t i32i;

                for (i = 0; i < min(GENRE_SIZE - 1, *AacCkSize); i++)
                    cMetadataString[i] = GetAACByte(Ctx, ptrAac);
                cMetadataString[i] = 0;

                if (*AacCkSize > GENRE_SIZE - 1)
                    SkipAACData(*AacCkSize - GENRE_SIZE - 1, Ctx, ptrAac);

                if (*AacCkSize == 2)
                {
                    uint32_t index = cMetadataString[1];

                    if (index < NUMBER_OF_GENRES)
                    {
                        uint16_t *song_genre = (uint16_t *)stc_genres[index - 1];
                        for (i32i = 0; i32i < GENRE_SIZE; i32i++)
                        {
                            uint8_t uBytesConsumed =
                                aac_copy_one_byte((char *)song_genre, (char *)&MetaData->song_genre[i32i]);
                            if (*song_genre == 0)
                            {
                                break;
                            }
                            song_genre += uBytesConsumed;
                        }
                    }
                }
                else
                {
                    // Convert to UTF16
                    for (i32i = 0; i32i < GENRE_SIZE; i32i++)
                    {
                        uint8_t uBytesConsumed = aac_copy_one_byte(pString, (char *)&MetaData->song_genre[i32i]);
                        if (*pString == 0)
                        {
                            break;
                        }
                        pString += uBytesConsumed;
                    }
                }

            }
            // printf("Genre:                 %s \n", MetaData->wGenre);
            ptrAac->AACMetaDataFlag = 1;

            break;

        case ART_CKID: // get artist's name
            SkipAACData(16, Ctx, ptrAac);
            *AacCkSize = *AacCkSize - 16;
            {
                char cMetadataString[ARTIST_SIZE];
                char *pString = cMetadataString;
                int32_t i32i;

                for (i = 0; i < min(ARTIST_SIZE - 1, *AacCkSize); i++)
                    cMetadataString[i] = GetAACByte(Ctx, ptrAac);
                cMetadataString[i] = 0;

                if (*AacCkSize > ARTIST_SIZE - 1)
                    SkipAACData(*AacCkSize - ARTIST_SIZE - 1, Ctx, ptrAac);

                // Convert to UTF16
                for (i32i = 0; i32i < ARTIST_SIZE; i32i++)
                {
                    uint8_t uBytesConsumed = aac_copy_one_byte(pString, (char *)&MetaData->song_artist[i32i]);
                    if (*pString == 0)
                    {
                        break;
                    }
                    pString += uBytesConsumed;
                }
            }
            ptrAac->AACMetaDataFlag = 1;
            ptrAac->AllAtoms |= ART_BIT;
            break;

        case DAY_CKID: // get Year
            SkipAACData(16, Ctx, ptrAac);
            *AacCkSize = *AacCkSize - 16;
            {
                char cMetadataString[YEAR_SIZE];
                char *pString = cMetadataString;
                int32_t i32i;

                for (i = 0; i < min(YEAR_SIZE - 1, *AacCkSize); i++)
                    cMetadataString[i] = GetAACByte(Ctx, ptrAac);
                cMetadataString[i] = 0;

                if (*AacCkSize > YEAR_SIZE - 1)
                    SkipAACData(*AacCkSize - YEAR_SIZE - 1, Ctx, ptrAac);

                // Convert to UTF16
                for (i32i = 0; i32i < YEAR_SIZE; i32i++)
                {
                    uint8_t uBytesConsumed = aac_copy_one_byte(pString, (char *)&MetaData->song_year[i32i]);
                    if (*pString == 0)
                    {
                        break;
                    }
                    pString += uBytesConsumed;
                }

            }
            ptrAac->AACMetaDataFlag = 1;
            ptrAac->AllAtoms |= DAY_BIT;
            break;

        case MDAT_CKID:
            if (*AacCkSize > 0)
            {
                // ignore MDAT atoms after MAX_MDAT_ATOM reached.
                if (ptrAac->AacMDATCount < MAX_MDAT_ATOM)
                {
                    // media data found
                    ptrAac->AacDataBegin[ptrAac->AacMDATCount] = ptrAac->aac_file_offset;
                    ptrAac->AacMDATSize[ptrAac->AacMDATCount]  = *AacCkSize;
                    ptrAac->AacMDATCount++;
                }
                // Skips the MDAT Atom
                ptrAac->MandatoryAtoms |= MDAT_BIT;
                ptrAac->AllAtoms |= MDAT_BIT;
                if (ptrAac->AllAtoms ^ ALL_ATOMS)
                    SkipAACData(*AacCkSize, Ctx, ptrAac); // Align the file reads correctly.
            }
            break;

            //  this case was added to extract title,album and artist
        case META_CKID:
            SkipAACData(4, Ctx, ptrAac); // Align the file reads correctly.
            break;
        case MDHD_CKID:
            if (ptrAac->AacFlag == 0)
            {
                SkipAACData(12, Ctx, ptrAac); // Align the file reads correctly.
                ptrAac->Aac_TimeScale    = GetAACDWord(Ctx, ptrAac);
                ptrAac->Aac_SamplingRate = ptrAac->Aac_TimeScale;
                ptrAac->AacFlag          = 1;
                ptrAac->Aac_Duration     = GetAACDWord(Ctx, ptrAac);
                SkipAACData(*AacCkSize - 20, Ctx, ptrAac); // Align the file reads correctly.
            }
            else
                SkipAACData(*AacCkSize, Ctx, ptrAac);

            break;

        case TRKN_CKID:
            SkipAACData(16, Ctx, ptrAac);
            SkipAACData(2, Ctx, ptrAac); // skip first two zeros
            // Extrack track number which is always in big indian form.
            MetaData->track_number = GetAACByte(Ctx, ptrAac);
            MetaData->track_number <<= 8;
            MetaData->track_number |= GetAACByte(Ctx, ptrAac);
            SkipAACData(2, Ctx, ptrAac); // Skip two bytes of total track
            SkipAACData(2, Ctx, ptrAac); // skip last two bytes of zeros.
            ptrAac->AllAtoms |= TRKN_BIT;
            break;

        case MDIA_CKID:
        case MINF_CKID:
        case TRAK_CKID:
        case LIST_CKID:
        case UDTA_CKID:
        default:
            ret_val = 0;
            break;
    }

    return ret_val;
}

///////////////////////////////////////////////////////////////////////////////
//!
//!   \brief This function finds the chunk id.
//!   \fntype  Function
//!
//!    \param[in]   Ctx Handle value
//!
//!   \retval ckid
//!   \note Can be overlaid
/////////////////////////////////////////////////////////////////////////////

int32_t GetAACckID(CCI_Ctx *Ctx, AAC_DFFI_t *ptrAac)
{
    int32_t i;
    int32_t ckid = UNKNOWN_CKID;

    ptrAac->AacCkSize = GetAACDWord(Ctx, ptrAac) - 8;

    ptrAac->AacCkVal = GetAACDWord(Ctx, ptrAac);

    for (i = 0; i < TOTAL_CKID; i++)
    {
        if (ptrAac->AacCkVal == Aacckidlist[i])
        {
            ckid = i;
            break;
        }
    }
    return ckid;
}
///////////////////////////////////////////////////////////////////////////////
//!
//!  \brief   Parses up to Data for Adif header.
//!
//!  \fntype  Function
//!
//!  \param[in]  iHandle File handler
//!  \param[in]  MetaData File's containt
//!
//!  \retval 0 if success  error else.
//!
//!
//////////////////////////////////////////////////////////////////////////////
const int32_t g_iSampleRate_adif[12] = {96000, 88200, 64000, 48000, 44100, 32000,
                                        24000, 22050, 16000, 12000, 11025, 8000};
// bit stream data
// bit stream data
typedef struct _BitStream_adif
{
    // common
    uint32_t bsCurrentWord;
    int32_t i32BitsLeft;
    uint8_t *bsBufferPtr;

    // more variables
    uint32_t u32TotalBitsRead;
    uint8_t *pBufStartAdd;
    uint8_t *pBufEndAdd;

} BitStream_adif_t;

void BitstreamInit_adif(BitStream_adif_t *pbs, uint8_t *pu8Buffer, uint32_t u32Size)
{
    pbs->bsBufferPtr      = pu8Buffer;
    pbs->pBufStartAdd     = pu8Buffer;
    pbs->pBufEndAdd       = pu8Buffer + u32Size;
    pbs->i32BitsLeft      = 0;
    pbs->bsCurrentWord    = 0;
    pbs->u32TotalBitsRead = 0;
}
uint32_t BitstreamGetBits_adif(BitStream_adif_t *pBitStream, uint8_t u8BitsToRead)
{
    uint32_t u32Data = 0;
    uint8_t *pPtr;
    int32_t i32Shift;

    // update bits read
    pBitStream->u32TotalBitsRead += u8BitsToRead;

    // update bits left
    pBitStream->i32BitsLeft -= u8BitsToRead;

    // calculate shift for word
    i32Shift = 32 - u8BitsToRead;

    // required bits are available in word
    if (pBitStream->i32BitsLeft >= 0)
    {
        // extract data from word
        u32Data = pBitStream->bsCurrentWord >> i32Shift;

        // update word after removing read bits
        pBitStream->bsCurrentWord <<= u8BitsToRead;

        // return data
        return u32Data;
    }

    // required bits are not available in word, so read new word

    // store partial data
    u32Data = pBitStream->bsCurrentWord >> i32Shift;

    pPtr = pBitStream->bsBufferPtr;

    // read next word
    pBitStream->bsCurrentWord =
        ((*pPtr & 0xff) << 24) + ((*(pPtr + 1) & 0xff) << 16) + ((*(pPtr + 2) & 0xff) << 8) + *(pPtr + 3);

    // update buffer pointer
    pBitStream->bsBufferPtr += 4;

    // update bits left
    pBitStream->i32BitsLeft = 32 + pBitStream->i32BitsLeft; // Because bitsleft is negative

    // update data
    u32Data = u32Data | (pBitStream->bsCurrentWord >> pBitStream->i32BitsLeft);

    // update word after removing read bits
    if (pBitStream->i32BitsLeft == 0)
        pBitStream->bsCurrentWord = 0;
    else
        pBitStream->bsCurrentWord <<= (32 - pBitStream->i32BitsLeft);

    // return data bits
    return u32Data;
}

RtStatus_t AdifParser(CCI_Ctx *Ctx, file_meta_data_t *MetaData)
{
    uint32_t u32Header, u32Value, bitstreamtype;
    uint32_t temp;
    uint8_t buf[128];
    BitStream_adif_t pbs;
    uint8_t u8PCECount, u8SampleFreqIdx, u8NumFrontCh, u8NumCh = 0;
    uint8_t i;
    uint8_t u8IsCPE;

    temp = Ctx->cci_dec_read(0, (uint8_t *)&buf[0], 128, 0, Ctx->user_data);
    if (temp != 128)
        return CODEC_METADATA_FILETYPE_FAILED;

    BitstreamInit_adif(&pbs, buf, 128);

    u32Header = BitstreamGetBits_adif(&pbs, 32);
    if (u32Header != 0x41444946)
        return CODEC_METADATA_FILETYPE_FAILED;

    // 1 bit ID
    temp = BitstreamGetBits_adif(&pbs, 1);
    if (temp)
    {
        // 72 bit copyright id
        for (u32Value = 0; u32Value < 9; u32Value++)
            BitstreamGetBits_adif(&pbs, 8);
    }
    // 1 bit Original copy
    temp = BitstreamGetBits_adif(&pbs, 1);
    // 1 bit home
    temp = BitstreamGetBits_adif(&pbs, 1);
    // 1 bit Bitstream type
    bitstreamtype = BitstreamGetBits_adif(&pbs, 1);
    // 23 bit bitrate
    temp               = BitstreamGetBits_adif(&pbs, 23);
    MetaData->bit_rate = temp;

    // 4 bit number of program config elements
    u8PCECount = BitstreamGetBits_adif(&pbs, 4);

    if (bitstreamtype == 0)
        BitstreamGetBits_adif(&pbs, 20);

    for (u32Value = 0; u32Value <= u8PCECount; u32Value++)
    {
        // instance tag
        temp = BitstreamGetBits_adif(&pbs, 4);

        // object type(profile)
        temp = (uint8_t)BitstreamGetBits_adif(&pbs, 2);
        // sample rate freq index
        u8SampleFreqIdx = (uint8_t)BitstreamGetBits_adif(&pbs, 4);

        // channel count for various elements
        u8NumFrontCh = (uint8_t)BitstreamGetBits_adif(&pbs, 4);
        BitstreamGetBits_adif(&pbs, 20);

        for (i = 0; i < u8NumFrontCh; i++)
        {
            // is mono or stereo
            u8IsCPE = BitstreamGetBits_adif(&pbs, 1);
            (void)BitstreamGetBits_adif(&pbs, 4);

            if (u8IsCPE)
                u8NumCh += 2;
            else
                u8NumCh++;
        }
    }
    MetaData->sample_rate  = g_iSampleRate_adif[u8SampleFreqIdx];
    MetaData->num_channels = u8NumCh;

    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
//!
//!  \brief   Parses up to Data for Adts header.
//!
//!  \fntype  Function
//!
//!  \param[in]  iHandle File handler
//!  \param[in]  MetaData File's containt
//!
//!  \retval 0 if success  error else.
//!
//!
//////////////////////////////////////////////////////////////////////////////
RtStatus_t AdtsParser(CCI_Ctx *Ctx, file_meta_data_t *MetaData)
{
    uint32_t u32Header, u32NextH, u32NextB;
    uint32_t u32Vh;
    int32_t i32SkipBytes;
    int32_t i32EndBytes = ADTS_HEADER_THRESHOLD;

    u32Header = ADTS_GetBytes(Ctx, 4);
    u32Vh     = ADTS_GetBytes(Ctx, 3);
    u32Vh     = (u32Vh >> 8);

    do
    {
        if ((u32Header & 0xFFF60000) == 0xFFF00000)
        {
            uint32_t u32Index     = (u32Header >> 10) & 15;
            uint32_t u32FrameSize = ((u32Header & 3) << 11) | ((u32Vh >> 13) & 0x7FF);

            if (u32Index <= 11 && u32FrameSize > 7)
            {
                int32_t i32Found;
                uint32_t u32FixedHeader = u32Header >> 4; // 28 bit of fixed ADTS header
                int32_t cur_offset      = ADTS_GetCurrentOffset(Ctx);

                // Read next header.
                i32SkipBytes = (int32_t)(u32FrameSize - (56 / 8));
                if (MetaData->num_bytes < i32SkipBytes)
                {
                    return CODEC_METADATA_FILETYPE_FAILED;
                }
                ADTS_SkipBytes(Ctx, i32SkipBytes);
                u32NextH = ADTS_GetBytes(Ctx, 4);
                u32NextH = (u32NextH >> 4);
                i32Found = (u32FixedHeader == u32NextH);

                if (i32Found)
                {
                    if (!((u32FixedHeader >> 12) & 1))
                    {
                        // skip rdb positions and CRC field
                        i32SkipBytes = 16 * ((u32Vh & 3) + 1);
                        ADTS_SkipBytes(Ctx, i32SkipBytes);
                    }
                    MetaData->num_channels = (int32_t)((u32FixedHeader >> 2) & 7);
                    MetaData->sample_rate  = ADTS_IndexToSampleRate(u32Index);
                    i32SkipBytes           = 0 - 4;
                    ADTS_SkipBytes(Ctx, i32SkipBytes);
                    MetaData->start_pos = cur_offset - 7;
                    return SUCCESS;
                }
                else
                {
                    i32SkipBytes = 0 - i32SkipBytes - 4;
                    ADTS_SkipBytes(Ctx, i32SkipBytes);
                }
            }
        }
        u32Header   = (u32Header << 8) | ((u32Vh >> 16) & 0xFF);
        u32NextB    = ((ADTS_GetBytes(Ctx, 1)) >> 24);
        u32Vh       = ((u32Vh << 16) >> 8) | u32NextB;
        i32EndBytes = i32EndBytes - 1;
    } while (i32EndBytes > 0);

    return CODEC_METADATA_FILETYPE_FAILED;
}
int AacReadDecoderSpecificInfo(const uint8_t *pDsi, int iDsiSize, file_meta_data_t *MetaData, AAC_DFFI_t *ptrAac)
{
    static const uint32_t sr_index[13] = {96000, 88200, 64000, 48000, 44100, 32000, 24000,
                                          22050, 16000, 12000, 11025, 8000,  7350};

    int freqindex;

    if (iDsiSize < 2) // for now
        return 1;

    // 5 Bits for ObjectTypeIndex
    // 4 bits for SamplingFrequencyIndex
    freqindex = ((pDsi[0] & 0x7) << 1) + ((pDsi[1] & 0x80) >> 7);

    if (freqindex < 13)
    {
        MetaData->sample_rate    = sr_index[freqindex];
        ptrAac->Aac_SamplingRate = sr_index[freqindex];
    }
    else
    {
        MetaData->sample_rate = 0;
    }

    // to do parse all DSI buffer

    return 1;
}
///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
//! @}
#endif //defined(AAC_DEC) && !defined(__ICCARM__)
