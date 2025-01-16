///////////////////////////////////////////////////////////////////////////////
//! \addtogroup asi_dffi
//! @{
//
// Copyright 2004-2005 SigmaTel, Inc.
// Copyright 2024 NXP
//
// SPDX-License-Identifier: BSD-3-Clause
//
//! \file asi_dffi_aac_metadata.h
//! \brief Contains defines for MP3 metadata functions.
///////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Definitions
////////////////////////////////////////////////////////////////////////////////
#define MOOV_CKID 0  //!< \brief main metadata atom
#define STSZ_CKID 1  //!< \brief to gather the metadata information
#define MDAT_CKID 2  //!< \brief to indicate that metadata portion is over
#define MDIA_CKID 3  //!< \brief to break to go to sub-atom
#define MINF_CKID 4  //!< \brief to break to go to sub-atom
#define TRAK_CKID 5  //!< \brief to break to go to sub-atom
#define STBL_CKID 6  //!< \brief to break to go to sub-atom
#define FTYP_CKID 7  //!< \brief to check whether mp4 file
#define UDTA_CKID 8  //!< \brief to break
#define ALB_CKID  9  //!< \brief to get album name
#define ART_CKID  10 //!< \brief to get artists name
#define STSD_CKID 11
#define MP4A_CKID 12
#define LIST_CKID 13
#define TTL_CKID  14 //!< \brief to get title name
#define DRMS_CKID 15

#define ESDS_CKID 16 //!< \brief APPLE: add ESDS atom index
#define SINF_CKID 17 //!< \brief APPLE: add SINF atom index
// Add more Ckids here
#define META_CKID 18
#define MDHD_CKID 19
#define DAY_CKID  20 //!< \brief Year metadata here
#define TRKN_CKID 21 //!< \brief Track Number Metadata
#define MP4V_CKID 22 //!< \brief video chunk
#define AVC1_CKID 23 //!< \brief video chunk
#define HDLR_CKID 24 //!< \brief Handler Chunk to identify Video/Audio handler
#define STCO_CKID 25
#define ALAC_CKID 26
#define GEN_CKID  27
#define GNRE_CKID 28

#define UNKNOWN_CKID 29
#define TOTAL_CKID   29

#define BUFFERFILL 96 * 3

#define ATOM_NAME_LENGTH   4
#define ATOM_SIZE_LENGTH   4
#define ATOM_HEADER_LENGTH (ATOM_NAME_LENGTH + ATOM_SIZE_LENGTH)
#define MAX_MDAT_ATOM      4

uint32_t ADTS_GetBytes(CCI_Ctx *Ctx, int32_t);
void ADTS_SkipBytes(CCI_Ctx *Ctx, int32_t);
uint32_t ADTS_IndexToSampleRate(int);
uint32_t ADTS_Bitrate(CCI_Ctx *Ctx, uint32_t, uint32_t);
int32_t ADTS_GetCurrentOffset(CCI_Ctx *Ctx);

typedef struct AAC_DFFI
{
    int32_t Aac_NumSamples;
    int32_t Aac_TimeScale;
    int32_t Aac_SamplingRate;
    int32_t Aac_BitRate;
    int32_t aac_file_flag;
    int32_t overwrite;       // overwrite
    int32_t tracktype;       // tracktype = UNKNOWN
    int32_t AACMetaDataFlag; // if it is 1 it indiacates that title name/album name/artist's name is available
    int32_t AacCkSize;       // this variable is global
    int32_t AacDataBegin[MAX_MDAT_ATOM];
    // to get the multiplication right!!!
    int32_t AacMDATCount;
    int64_t AacMDATSize[MAX_MDAT_ATOM];
    int32_t chunkcount;
    int32_t MandatoryAtoms;
    int32_t AllAtoms;
    int32_t SamplingRate;
    int32_t AacFlag;
    int32_t aac_file_offset;
    int32_t AacCkVal;
    int32_t Aac_Duration;
} AAC_DFFI_t;

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
//! @}
