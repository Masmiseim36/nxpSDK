/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

//! \addtogroup common_interface
//! @{
//
// Copyright (c) 2013-2014 Freescale, Inc.
//
//! \file mp3_id3v2.c
//! \brief Contains functionality that initializes the decoder variables
//!        and resets the internal filter states.

////////////////////////////////////////////////////////////////////////////////
//  Macros
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Includes
////////////////////////////////////////////////////////////////////////////////
#include <string.h>
#include "cci_metadata.h"
#include "mp3_metadata.h"
#include "mp3_id3v2_internal.h"
////////////////////////////////////////////////////////////////////////////////
//  Externs
////////////////////////////////////////////////////////////////////////////////
extern int32_t mp3_GetFileSize(CCI_Ctx *Ctx);
extern uint8_t mp3_copy_one_byte(char *ptr, char *ptr1);
extern uint8_t mp3_copy_two_byte(char *ptr, uint16_t *ptr1);
extern void mp3_copy_16_to_8(uint8_t *ptr, uint16_t *ptr1, int32_t size);

////////////////////////////////////////////////////////////////////////////////
//  Equates
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Prototypes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

#define _T(x) (L##x)

// Changed so that stc_genres can end up into .rodata section rather than .data section
static const uint16_t stc_genres[][17] = {
    {'B', 'l', 'u', 'e', 's'},
    {'C', 'l', 'a', 's', 's', 'i', 'c', ' ', 'R', 'o', 'c', 'k'},
    {'C', 'o', 'u', 'n', 't', 'r', 'y'},
    {'D', 'a', 'n', 'c', 'e'},
    {'D', 'i', 's', 'c', 'o'},
    {'F', 'u', 'n', 'k'},
    {'G', 'r', 'u', 'n', 'g', 'e'},
    {'H', 'i', 'p', '-', 'H', 'o', 'p'},
    {'J', 'a', 'z', 'z'},
    {'M', 'e', 't', 'a', 'l'},
    {'N', 'e', 'w', ' ', 'A', 'g', 'e'},
    {'O', 'l', 'd', 'i', 'e', 's'},
    {'O', 't', 'h', 'e', 'r'},
    {'P', 'o', 'p'},
    {'R', '&', 'B'},
    {'R', 'a', 'p'},
    {'R', 'e', 'g', 'g', 'a', 'e'},
    {'R', 'o', 'c', 'k'},
    {'T', 'e', 'c', 'h', 'n', 'o'},
    {'I', 'n', 'd', 'u', 's', 't', 'r', 'i', 'a', 'l'},
    {'A', 'l', 't', 'e', 'r', 'n', 'a', 't', 'i', 'v', 'e'},
    {'S', 'k', 'a'},
    {'D', 'e', 'a', 't', 'h', ' ', 'M', 'e', 't', 'a', 'l'},
    {'P', 'r', 'a', 'n', 'k', 's'},
    {'S', 'o', 'u', 'n', 'd', 't', 'r', 'a', 'c', 'k'},
    {'E', 'u', 'r', 'o', '-', 'T', 'e', 'c', 'h', 'n', 'o'},
    {'A', 'm', 'b', 'i', 'e', 'n', 't'},
    {'T', 'r', 'i', 'p', '-', 'H', 'o', 'p'},
    {'V', 'o', 'c', 'a', 'l'},
    {'J', 'a', 'z', 'z', '+', 'F', 'u', 'n', 'k'},
    {'F', 'u', 's', 'i', 'o', 'n'},
    {'T', 'r', 'a', 'n', 'c', 'e'},
    {'C', 'l', 'a', 's', 's', 'i', 'c', 'a', 'l'},
    {'I', 'n', 's', 't', 'r', 'u', 'm', 'e', 'n', 't', 'a', 'l'},
    {'A', 'c', 'i', 'd'},
    {'H', 'o', 'u', 's', 'e'},
    {'G', 'a', 'm', 'e'},
    {'S', 'o', 'u', 'n', 'd', ' ', 'C', 'l', 'i', 'p'},
    {'G', 'o', 's', 'p', 'e', 'l'},
    {'N', 'o', 'i', 's', 'e'},
    {'A', 'l', 't', 'e', 'r', 'n', 'R', 'o', 'c', 'k'},
    {'B', 'a', 's', 's'},
    {'S', 'o', 'u', 'l'},
    {'P', 'u', 'n', 'k'},
    {'S', 'p', 'a', 'c', 'e'},
    {'M', 'e', 'd', 'i', 't', 'a', 't', 'i', 'v', 'e'},
    {'I', 'n', 's', 't', 'r', 'u', 'm', 'e', 'n', 't', 'a', 'l', ' ', 'P', 'o', 'p'},
    {'I', 'n', 's', 't', 'r', 'u', 'm', 'e', 'n', 't', 'a', 'l', ' ', 'R', 'o', 'c', 'k'},
    {'E', 't', 'h', 'n', 'i', 'c'},
    {'G', 'o', 't', 'h', 'i', 'c'},
    {'D', 'a', 'r', 'k', 'w', 'a', 'v', 'e'},
    {'T', 'e', 'c', 'h', 'n', 'o', '-', 'I', 'n', 'd', 'u', 's', 't', 'r', 'i', 'a', 'l'},
    {'E', 'l', 'e', 'c', 't', 'r', 'o', 'n', 'i', 'c'},
    {'P', 'o', 'p', '-', 'F', 'o', 'l', 'k'},
    {'E', 'u', 'r', 'o', 'd', 'a', 'n', 'c', 'e'},
    {'D', 'r', 'e', 'a', 'm'},
    {'S', 'o', 'u', 't', 'h', 'e', 'r', 'n', ' ', 'R', 'o', 'c', 'k'},
    {'C', 'o', 'm', 'e', 'd', 'y'},
    {'C', 'u', 'l', 't'},
    {'G', 'a', 'n', 'g', 's', 't', 'a'},
    {'T', 'o', 'p', ' ', '4', '0'},
    {'C', 'h', 'r', 'i', 's', 't', 'i', 'a', 'n', ' ', 'R', 'a', 'p'},
    {'P', 'o', 'p', '/', 'F', 'u', 'n', 'k'},
    {'J', 'u', 'n', 'g', 'l', 'e'},
    {'N', 'a', 't', 'i', 'v', 'e', ' ', 'A', 'm', 'e', 'r', 'i', 'c', 'a', 'n'},
    {'C', 'a', 'b', 'a', 'r', 'e', 't'},
    {'N', 'e', 'w', ' ', 'W', 'a', 'v', 'e'},
    {'P', 's', 'y', 'c', 'h', 'e', 'd', 'e', 'l', 'i', 'c'},
    {'R', 'a', 'v', 'e'},
    {'S', 'h', 'o', 'w', 't', 'u', 'n', 'e', 's'},
    {'T', 'r', 'a', 'i', 'l', 'e', 'r'},
    {'L', 'o', '-', 'F', 'i'},
    {'T', 'r', 'i', 'b', 'a', 'l'},
    {'A', 'c', 'i', 'd', ' ', 'P', 'u', 'n', 'k'},
    {'A', 'c', 'i', 'd', ' ', 'J', 'a', 'z', 'z'},
    {'P', 'o', 'l', 'k', 'a'},
    {'R', 'e', 't', 'r', 'o'},
    {'M', 'u', 's', 'i', 'c', 'a', 'l'},
    {'R', 'o', 'c', 'k', ' ', '&', ' ', 'R', 'o', 'l', 'l'},
    {'H', 'a', 'r', 'd', ' ', 'R', 'o', 'c', 'k'},
    {'F', 'o', 'l', 'k'},
    {'F', 'o', 'l', 'k', '-', 'R', 'o', 'c', 'k'},
    {'N', 'a', 't', 'i', 'o', 'n', 'a', 'l', ' ', 'F', 'o', 'l', 'k'},
    {'S', 'w', 'i', 'n', 'g'},
    {'F', 'a', 's', 't', ' ', 'F', 'u', 's', 'i', 'o', 'n'},
    {'B', 'e', 'b', 'o', 'b'},
    {'L', 'a', 't', 'i', 'n'},
    {'R', 'e', 'v', 'i', 'v', 'a', 'l'},
    {'C', 'e', 'l', 't', 'i', 'c'},
    {'B', 'l', 'u', 'e', 'g', 'r', 'a', 's', 's'},
    {'A', 'v', 'a', 'n', 't', 'g', 'a', 'r', 'd', 'e'},
    {'G', 'o', 't', 'h', 'i', 'c', ' ', 'R', 'o', 'c', 'k'},
    {'P', 'r', 'o', 'g', 'r', 'e', 's', 's', 'i', 'v', 'e', ' ', 'R', 'o', 'c', 'k'},
    {'P', 's', 'y', 'c', 'h', 'e', 'd', 'e', 'l', 'i', 'c', ' ', 'R', 'o', 'c', 'k'},
    {'S', 'y', 'm', 'p', 'h', 'o', 'n', 'i', 'c', ' ', 'R', 'o', 'c', 'k'},
    {'S', 'l', 'o', 'w', ' ', 'R', 'o', 'c', 'k'},
    {'B', 'i', 'g', ' ', 'B', 'a', 'n', 'd'},
    {'C', 'h', 'o', 'r', 'u', 's'},
    {'E', 'a', 's', 'y', ' ', 'L', 'i', 's', 't', 'e', 'n', 'i', 'n', 'g'},
    {'A', 'c', 'o', 'u', 's', 't', 'i', 'c'},
    {'H', 'u', 'm', 'o', 'u', 'r'},
    {'S', 'p', 'e', 'e', 'c', 'h'},
    {'C', 'h', 'a', 'n', 's', 'o', 'n'},
    {'O', 'p', 'e', 'r', 'a'},
    {'C', 'h', 'a', 'm', 'b', 'e', 'r', ' ', 'M', 'u', 's', 'i', 'c'},
    {'S', 'o', 'n', 'a', 't', 'a'},
    {'S', 'y', 'm', 'p', 'h', 'o', 'n', 'y'},
    {'B', 'o', 'o', 't', 'y', ' ', 'B', 'a', 's', 's'},
    {'P', 'r', 'i', 'm', 'u', 's'},
    {'P', 'o', 'r', 'n', ' ', 'G', 'r', 'o', 'o', 'v', 'e'},
    {'S', 'a', 't', 'i', 'r', 'e'},
    {'S', 'l', 'o', 'w', ' ', 'J', 'a', 'm'},
    {'C', 'l', 'u', 'b'},
    {'T', 'a', 'n', 'g', 'o'},
    {'S', 'a', 'm', 'b', 'a'},
    {'F', 'o', 'l', 'k', 'l', 'o', 'r', 'e'},
    {'B', 'a', 'l', 'l', 'a', 'd'},
    {'P', 'o', 'w', 'e', 'r', ' ', 'B', 'a', 'l', 'l', 'a', 'd'},
    {'R', 'h', 'y', 't', 'h', 'm', 'i', 'c', ' ', 'S', 'o', 'u', 'l'},
    {'F', 'r', 'e', 'e', 's', 't', 'y', 'l', 'e'},
    {'D', 'u', 'e', 't'},
    {'P', 'u', 'n', 'k', ' ', 'R', 'o', 'c', 'k'},
    {'D', 'r', 'u', 'm', ' ', 'S', 'o', 'l', 'o'},
    {'A', 'c', 'a', 'p', 'e', 'l', 'l', 'a'},
    {'E', 'u', 'r', 'o', '-', 'H', 'o', 'u', 's', 'e'},
    {'D', 'a', 'n', 'c', 'e', ' ', 'H', 'a', 'l', 'l'},
    {'G', 'o', 'a'},
    {'D', 'r', 'u', 'm', ' ', '&', ' ', 'B', 'a', 's', 's'},
    {'C', 'l', 'u', 'b', '-', 'H', 'o', 'u', 's', 'e'},
    {'H', 'a', 'r', 'd', 'c', 'o', 'r', 'e'},
    {'T', 'e', 'r', 'r', 'o', 'r'},
    {'I', 'n', 'd', 'i', 'e'},
    {'B', 'r', 'i', 't', 'p', 'o', 'p'},
    {'N', 'e', 'g', 'e', 'r', 'p', 'u', 'n', 'k'},
    {'P', 'o', 'l', 's', 'k', ' ', 'P', 'u', 'n', 'k'},
    {'B', 'e', 'a', 't'},
    {'C', 'h', 'r', 'i', 's', 't', 'i', 'a', 'n', ' ', 'G', 'a', 'n', 'g', 's', 't', 'a'},
    {'H', 'e', 'a', 'v', 'y', ' ', 'M', 'e', 't', 'a', 'l'},
    {'B', 'l', 'a', 'c', 'k', ' ', 'M', 'e', 't', 'a', 'l'},
    {'C', 'r', 'o', 's', 's', 'o', 'v', 'e', 'r'},
    {'C', 'o', 'n', 't', 'e', 'm', 'p', 'o', 'r', 'a', 'r', 'y', ' ', 'C'},
    {'C', 'h', 'r', 'i', 's', 't', 'i', 'a', 'n', ' ', 'R', 'o', 'c', 'k'},
    {'M', 'e', 'r', 'e', 'n', 'g', 'u', 'e'},
    {'S', 'a', 'l', 's', 'a'},
    {'T', 'h', 'r', 'a', 's', 'h', ' ', 'M', 'e', 't', 'a', 'l'},
    {'A', 'n', 'i', 'm', 'e'},
    {'J', 'P', 'o', 'p'},
    {'S', 'y', 'n', 't', 'h', ' ', 'P', 'o', 'p'},
    {'S', 'w', 'e', 'e', 't', 'c', 'o', 'r', 'n', ' ', 'D', 'u', 'b'},
    {'D', 'a', 'n', 'c', 'e'},
    {'D', 'i', 's', 'c', 'o'},
    {'F', 'u', 'n', 'k'},
    {'G', 'r', 'u', 'n', 'g', 'e'},
    {'H', 'i', 'p', '-', 'H', 'o', 'p'},
    {'J', 'a', 'z', 'z'},
    {'M', 'e', 't', 'a', 'l'},

    /*
        _T("Blues"),       // 0
        _T("Classic Rock"),
        _T("Country"),
        _T("Dance"),
        _T("Disco"),
        _T("Funk"),
        _T("Grunge"),
        _T("Hip-Hop"),
        _T("Jazz"),
        _T("Metal"),

        _T("New Age"),		// 10
        _T("Oldies"),
        _T("Other"),
        _T("Pop"),
        _T("R&B"),
        _T("Rap"),
        _T("Reggae"),
        _T("Rock"),
        _T("Techno"),
        _T("Industrial"),

        _T("Alternative"),	// 20
        _T("Ska"),
        _T("Death Metal"),
        _T("Pranks"),
        _T("Soundtrack"),
        _T("Euro-Techno"),
        _T("Ambient"),
        _T("Trip-Hop"),
        _T("Vocal"),
        _T("Jazz+Funk"),

        _T("Fusion"),		// 30
        _T("Trance"),
        _T("Classical"),
        _T("Instrumental"),
        _T("Acid"),
        _T("House"),
        _T("Game"),
        _T("Sound Clip"),
        _T("Gospel"),
        _T("Noise"),

        _T("AlternRock"),	// 40
        _T("Bass"),
        _T("Soul"),
        _T("Punk"),
        _T("Space"),
        _T("Meditative"),
        _T("Instrumental Pop"),
        _T("Instrumental Rock"),
        _T("Ethnic"),
        _T("Gothic"),

        _T("Darkwave"),		// 50
        _T("Techno-Industrial"),
        _T("Electronic"),
        _T("Pop-Folk"),
        _T("Eurodance"),
        _T("Dream"),
        _T("Southern Rock"),
        _T("Comedy"),
        _T("Cult"),
        _T("Gangsta"),

        _T("Top 40"),		// 60
        _T("Christian Rap"),
        _T("Pop/Funk"),
        _T("Jungle"),
        _T("Native American"),
        _T("Cabaret"),
        _T("New Wave"),
        _T("Psychedelic"),
        _T("Rave"),
        _T("Showtunes"),

        _T("Trailer"),		// 70
        _T("Lo-Fi"),
        _T("Tribal"),
        _T("Acid Punk"),
        _T("Acid Jazz"),
        _T("Polka"),
        _T("Retro"),
        _T("Musical"),
        _T("Rock & Roll"),
        _T("Hard Rock"),

        _T("Folk"),		// 80
        _T("Folk-Rock"),
        _T("National Folk"),
        _T("Swing"),
        _T("Fast Fusion"),
        _T("Bebob"),
        _T("Latin"),
        _T("Revival"),
        _T("Celtic"),
        _T("Bluegrass"),

        _T("Avantgarde"),	// 90
        _T("Gothic Rock"),
        _T("Progressive Rock"),
        _T("Psychedelic Rock"),
        _T("Symphonic Rock"),
        _T("Slow Rock"),
        _T("Big Band"),
        _T("Chorus"),
        _T("Easy Listening"),
        _T("Acoustic"),

        _T("Humour"),		// 100
        _T("Speech"),
        _T("Chanson"),
        _T("Opera"),
        _T("Chamber Music"),

        _T("Sonata"),
        _T("Symphony"),
        _T("Booty Bass"),
        _T("Primus"),
        _T("Porn Groove"),

        _T("Satire"),		// 110
        _T("Slow Jam"),
        _T("Club"),
        _T("Tango"),
        _T("Samba"),

        _T("Folklore"),
        _T("Ballad"),
        _T("Power Ballad"),
        _T("Rhythmic Soul"),
        _T("Freestyle"),

        _T("Duet"),		// 120
        _T("Punk Rock"),
        _T("Drum Solo"),
        _T("Acapella"),
        _T("Euro-House"),

        _T("Dance Hall"),
        _T("Goa"),		// 126
        _T("Drum & Bass"),
        _T("Club-House"),
        _T("Hardcore"),

        _T("Terror"),		// 130
        _T("Indie"),
        _T("Britpop"),
        _T("Negerpunk"),
        _T("Polsk Punk"),

        _T("Beat"),
        _T("Christian Gangsta"),
        _T("Heavy Metal"),
        _T("Black Metal"),
        _T("Crossover"),

        _T("Contemporary C"),	// 140
        _T("Christian Rock"),
        _T("Merengue"),
        _T("Salsa"),
        _T("Thrash Metal"),
        _T("Anime"),
        _T("JPop"),
        _T("Synth Pop"),	// 147
        _T("Sweetcorn Dub"),     // 148 */
};

#define NUMBER_OF_GENRES 149

uint16_t *Mp3MetaDataGetGenreString(uint32_t uGenre)
{
    if (uGenre < NUMBER_OF_GENRES)
        return (uint16_t *)stc_genres[uGenre];
    else
        return NULL;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          int32_t Mp3MetaDataReadID3V2(CCI_Ctx *Ctx, int32_t iIgnored, file_meta_data_t* pMetaData, MP3_DFFI_t
//*ptrThis)
//
//   Type:          Function (code bank callable)
//
//   Description:   Attempts to read an ID3V2 tag from a file starting at the
//                  current position.
//
//   Inputs:        CCI_Ctx *Ctx                    Pointer to CCI Context structure
//                  int32_t iIgnored                There because of codebank callable requirements.
//                  file_meta_data_t *pMetaData     Pointer to meta-data struct.
//
//   Outputs:       int
//
//   Notes:         (it also leaves the file position at the end of the
//                  ID3V2 tagging information, or where we entered if there
//                  was no ID3V2 tag)
//<
////////////////////////////////////////////////////////////////////////////////
int32_t Mp3MetaDataReadID3V2(CCI_Ctx *Ctx, int32_t iIgnored, file_meta_data_t *pMetaData, MP3_DFFI_t *ptrThis)
{
    // remember the current position in the file.
    uint32_t uHeaderSize = 0;
    bool bSyncSafe       = 0;
    int32_t uCurrentPos  = Ctx->cci_dec_tell(0, Ctx->user_data);
    if (uCurrentPos != -1)
    {
        uint32_t uTemp = 0;

        while (1)
        {
            if (Ctx->cci_dec_read(0, (uint8_t *)&uTemp, 3, 0, Ctx->user_data) == 3)
            {
                if (uTemp == 0x334449) //'ID3'
                {                      // we've got a ID3 V2 tag at the beginning
                    uTemp = 0;
                    if (Ctx->cci_dec_read(0, (uint8_t *)&uTemp, 2, 0, Ctx->user_data) == 2)
                    {
                        if (((uTemp & 0x00ff) <= 4) && ((uTemp & 0xff00) != 0xff00))
                        { // its a version we support
                            uint32_t uFlags = 0;
                            // read the flags.
                            Ctx->cci_dec_read(0, (uint8_t *)&uFlags, 1, 0, Ctx->user_data);
                            // Decide whether frame lengths must be read as Sync Safe Integers.
                            uTemp &= 0xff;                 // Only keep the Low Byte.
                            bSyncSafe = uTemp < 4 ? 0 : 1; // Sync-Safe Integer reads in V2.4
                            // get the header size
                            uHeaderSize = 10 + Id3V2ReadInt(Ctx, 1); // Header always SyncSafe
                            // add 10 to the header size if the header has a footer
                            if (uFlags & ID3V2_HEADER_FLAG_FOOTER)
                                uHeaderSize += 10;
                            // update the start position of the MP3 data (must be after the ID3V2Header and padding)
                            pMetaData->start_pos += uHeaderSize;
                            // update the length of the song (its shorter by the header size now)
                            pMetaData->num_bytes -= uHeaderSize;
                            if (uFlags & ID3V2_HEADER_EXTENDED_HEADER)
                            { // if it has an extended header, consume it.  (We don't care about the contents)
                                uint32_t uLength = Id3V2ReadInt(Ctx, bSyncSafe);
                                Ctx->cci_dec_seek(0, uLength - 4, SEEK_CUR, Ctx->user_data);
                            }
                            // if(!(uFlags & ID3V2_HEADER_UNSYNCRONIZATION))
                            { // we don't support unsyncronization
                                // Id3V2ReadFrame will return false if it does not find a valid Id3V2 Header Frame
                                // identifier. (this will break out of the loop below)
                                if (uTemp == 2) // ID3 Tag for version V2.2
                                    while (Id3V2ReadFramev2(Ctx, pMetaData, bSyncSafe))
                                        ;
                                else
                                    while (Id3V2ReadFrame(Ctx, pMetaData, bSyncSafe))
                                        ;
                                // removed pMetaData->song_genre not null test, because it is struct and always exists
                                if (pMetaData->song_genre[0] == '(')
                                {
                                    uint16_t *pGenreString;
                                    uint16_t genre = pMetaData->song_genre[1] - '0';
                                    if (pMetaData->song_genre[2] != ')')
                                    {
                                        genre = genre * 10 + (pMetaData->song_genre[2] - '0');
                                        if (pMetaData->song_genre[3] != ')')
                                        {
                                            genre = genre * 10 + (pMetaData->song_genre[3] - '0');
                                        }
                                    }

                                    pGenreString = Mp3MetaDataGetGenreString(genre);
                                    if (pGenreString)
                                    {
                                        mp3_copy_16_to_8(pMetaData->song_genre, pGenreString, 17);
                                    }
                                }
                                ptrThis->g_iTagSelected = ID3V2_TAG_SELECTED;
                            }
                        } // if(((uTemp&0x00ff) <= 4) && ((uTemp&0xff00) != 0xff00))
                        else
                        {
                            // Not supported version ...
                            Ctx->cci_dec_seek(0, uCurrentPos + uHeaderSize, SEEK_SET, Ctx->user_data);
                            break;
                        }
                    }
                    // seek past the ID3V2.4 header, or back to where we started if there was no tag present.
                    Ctx->cci_dec_seek(0, uCurrentPos + uHeaderSize, SEEK_SET, Ctx->user_data);
                    uCurrentPos = Ctx->cci_dec_tell(0, Ctx->user_data);

                    uHeaderSize = 0;

                } // ID3
                else
                {
                    Ctx->cci_dec_seek(0, uCurrentPos + uHeaderSize, SEEK_SET, Ctx->user_data);
                    break;
                }
            }
            else
            {
                return -1;
            }
        } // while(1)
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          bool Id3V2ReadFrame(CCI_Ctx *Ctx, file_meta_data_t *pMetaData, bool bSyncSafe)
//
//   Type:          Function
//
//   Description:   Attempts to read an ID3V2 frame
//
//   Inputs:        CCI_Ctx *Ctx                    Pointer to CCI Context structure
//                  file_meta_data_t *pMetaData     Pointer to meta-data struct.
//                  bool bSyncSafe                  Decide whether this is a sync-safe integer read.
//
//   Outputs:       bool:   true if frame was found
//                          false if no frame was found
//
//   Notes:         Advances file position to after where the frame ends if frame found, or back to
//                  where it started reading if no valid frame found.
//<
////////////////////////////////////////////////////////////////////////////////
bool Id3V2ReadFrame(CCI_Ctx *Ctx, file_meta_data_t *pMetaData, bool bSyncSafe)
{
    bool bValidFrame       = 0;
    uint32_t uFrameType    = 0; /* Need to initialize because FSFileRead only reads data into the current word */
    uint32_t uFrameFlags   = 0; /* and does not initialize the rest of the word. */
    uint32_t uFrameSubType = 0;
    uint32_t uFrameSize    = 0;
    uint32_t u32FilePosAfterFrameSubType;

    Ctx->cci_dec_read(0, (uint8_t *)&uFrameType, 1, 0, Ctx->user_data);
    Ctx->cci_dec_read(0, (uint8_t *)&uFrameSubType, 3, 0, Ctx->user_data);
    u32FilePosAfterFrameSubType = Ctx->cci_dec_tell(0, Ctx->user_data);
    uFrameSize                  = Id3V2ReadInt(Ctx, bSyncSafe);
    Ctx->cci_dec_read(0, (uint8_t *)&uFrameFlags, 2, 0, Ctx->user_data);

    if (((uFrameType >= 'A' && uFrameType <= 'Z') || (uFrameType >= '0' && uFrameType <= '9')) &&
        ((pMetaData->start_pos) > u32FilePosAfterFrameSubType))
    {
        if (!(uFrameFlags &
              (ID3V2_FRAME_HEADER_UNSYNCRONIZATION | ID3V2_FRAME_HEADER_COMPRESSION | ID3V2_FRAME_HEADER_ENCRYPTION)))
        {
            /* we don't support Unsync, Compressed, or Encrypted frames. */
            int32_t lStartOfFrame;
            bValidFrame = 1;
            /* remember where the start of the frame data is */
            lStartOfFrame = Ctx->cci_dec_tell(0, Ctx->user_data);
            if (uFrameType == 'T')
            {
                /* If its a text frame, lets process it this way to save a little memory. */
                uint16_t *pString = NULL;
                uint16_t ShortBuf[64];
                uint32_t iBufferLength;
                memset((void *)ShortBuf, 0, 64 * 2);
                switch (uFrameSubType)
                {
                    case TALB: /* The Album name */
                        pString       = ShortBuf;
                        iBufferLength = ALBUM_SIZE;
                        if (pString)
                            /* if we've set up pString, go and read it... */
                            Id3V2ReadVariedStringToUnicodeBuffer(Ctx, uFrameFlags, uFrameSize, pString, iBufferLength);
                        // changed from pMetaData->song_album to pMetaData only. it is struct, so it exists always
                        if (pMetaData)
                        {
                            mp3_copy_16_to_8(pMetaData->song_album, pString, iBufferLength);
                        }
                        break;
                    case TIT2: /* The title of the composition */
                        pString       = ShortBuf;
                        iBufferLength = TITLE_SIZE;
                        if (pString)
                            /* if we've set up pString, go and read it... */
                            Id3V2ReadVariedStringToUnicodeBuffer(Ctx, uFrameFlags, uFrameSize, pString, iBufferLength);
                        // changed from pMetaData->song_title to pMetaData only. it is struct, so it exists always
                        if (pMetaData)
                        {
                            mp3_copy_16_to_8(pMetaData->song_title, pString, iBufferLength);
                        }
                        break;
                    case TPE1: /* The artist */
                        pString       = ShortBuf;
                        iBufferLength = ARTIST_SIZE;
                        if (pString)
                            /* if we've set up pString, go and read it... */
                            Id3V2ReadVariedStringToUnicodeBuffer(Ctx, uFrameFlags, uFrameSize, pString, iBufferLength);
                        // changed from pMetaData->song_artist to pMetaData only. it is struct, so it exists always
                        if (pMetaData)
                        {
                            mp3_copy_16_to_8(pMetaData->song_artist, pString, iBufferLength);
                        }
                        break;
                        /* if you want it to read in a new text frame, set up pString and iBuffer to point to your
                        own uint16_t string and length. */
                    case TCON: /* The Genre */
                        pString       = ShortBuf;
                        iBufferLength = GENRE_SIZE;
                        if (pString)
                            /* if we've set up pString, go and read it... */
                            Id3V2ReadVariedStringToUnicodeBuffer(Ctx, uFrameFlags, uFrameSize, pString, iBufferLength);
                        // changed from pMetaData->song_genre to pMetaData only. it is struct, so it exists always
                        if (pMetaData)
                        {
                            mp3_copy_16_to_8(pMetaData->song_genre, pString, iBufferLength);
                        }
                        break;
                    case TYER: /* The Year */
                    case TDRC:
                        pString       = ShortBuf;
                        iBufferLength = YEAR_SIZE;
                        if (pString)
                            /* if we've set up pString, go and read it... */
                            Id3V2ReadVariedStringToUnicodeBuffer(Ctx, uFrameFlags, uFrameSize, pString, iBufferLength);
                        // changed from pMetaData->song_year to pMetaData only. it is struct, so it exists always
                        if (pMetaData)
                        {
                            mp3_copy_16_to_8(pMetaData->song_year, pString, iBufferLength);
                        }
                        break;
                    case TRCK:
                    {
                        uint16_t pTracknum[TRACKNUM_SIZE];
                        uint32_t TracknumSize = 0;
                        uint32_t TmpVal[3];
                        uint16_t *pTmp;

                        Id3V2ReadVariedStringToUnicodeBuffer(Ctx, uFrameFlags, uFrameSize, pTracknum, TRACKNUM_SIZE);
                        for (pTmp = pTracknum, TracknumSize = 0; (*pTmp != 0) && (*pTmp != '/'); pTmp++)
                        {
                            if (*pTmp != ' ') // ignoring initial spaces from actual number
                                TracknumSize++;
                        }

                        TmpVal[0] = 0;
                        TmpVal[1] = 1;
                        while (TracknumSize)
                        {
                            pTmp--;
                            TmpVal[2] = *pTmp - '0';
                            TmpVal[0] += TmpVal[1] * TmpVal[2];
                            TmpVal[1] *= 10;
                            TracknumSize--;
                        }

                        pMetaData->track_number = TmpVal[0];
                        break;
                    }
                    default:
                        pString       = NULL;
                        iBufferLength = 0;
                        break;
                }
                /* Some of the 'Txxx' frames aren't strings, but numbers stored as text strings (i.e. "100" instead of
                100. These don't fit the above processing, so you'll have to rewrite the algorithm a bit. */
            }
            /* if you want to handle a new type of frame, put that here! */
            if ((lStartOfFrame + uFrameSize) >= mp3_GetFileSize(Ctx))
            {
                bValidFrame = 0;
                return bValidFrame;
            }
            /* go back to the start of the frame data */
            Ctx->cci_dec_seek(0, lStartOfFrame + uFrameSize, SEEK_SET, Ctx->user_data);
        }
        else
            /* seek to the next frame. */
            Ctx->cci_dec_seek(0, uFrameSize, SEEK_CUR, Ctx->user_data);
    }
    else
        /* seek to where we started reading */
        Ctx->cci_dec_seek(0, -6, SEEK_CUR, Ctx->user_data);

    return bValidFrame;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          bool Id3V2ReadFramev2(CCI_Ctx *Ctx, file_meta_data_t *pMetaData, bool bSyncSafe)
//
//   Type:          Function
//
//   Description:   Attempts to read an ID3V2 frame for ID3 version 02.
//
//   Inputs:        CCI_Ctx *Ctx                    Pointer to CCI Context structure
//                  file_meta_data_t *pMetaData     Pointer to meta-data struct
//                  bool bSyncSafe                  Decide whether this is a sync-safe integer read
//
//   Outputs:       bool:   true if frame was found
//                          false if no frame was found
//
//   Notes:         Advances file position to after where the frame ends if frame found, or back to
//                  where it started reading if no valid frame found.
//<
////////////////////////////////////////////////////////////////////////////////
bool Id3V2ReadFramev2(CCI_Ctx *Ctx, file_meta_data_t *pMetaData, bool bSyncSafe)
{
    bool bValidFrame       = 0;
    uint32_t uFrameType    = 0; // Need to initialize because FSFileRead only reads data into the current word
    uint32_t uFrameFlags   = 0; // and does not initialize the rest of the word.
    uint32_t uFrameSubType = 0;
    uint32_t uFrameSize    = 0;

    Ctx->cci_dec_read(0, (uint8_t *)&uFrameType, 1, 0, Ctx->user_data);
    Ctx->cci_dec_read(0, (uint8_t *)&uFrameSubType, 2, 0, Ctx->user_data);
    uFrameSize = Id3V2ReadIntv2(Ctx, bSyncSafe);
    if ((uFrameType >= 'A' && uFrameType <= 'Z') || (uFrameType >= '0' && uFrameType <= '9'))
    {
        if (!(uFrameFlags &
              (ID3V2_FRAME_HEADER_UNSYNCRONIZATION | ID3V2_FRAME_HEADER_COMPRESSION | ID3V2_FRAME_HEADER_ENCRYPTION)))
        {
            /* we don't support Unsync, Compressed, or Encrypted frames. */
            int32_t lStartOfFrame;
            bValidFrame = 1;
            /* remember where the start of the frame data is */
            lStartOfFrame = Ctx->cci_dec_tell(0, Ctx->user_data);
            if (uFrameType == 'T')
            {
                /* If its a text frame, lets process it this way to save a little memory. */
                uint16_t *pString = NULL;
                uint16_t ShortBuf[64];
                uint32_t iBufferLength;
                switch (uFrameSubType)
                {
                    case TAL: /* The Album name */
                        pString       = ShortBuf;
                        iBufferLength = ALBUM_SIZE;
                        if (pString)
                            /* if we've set up pString, go and read it... */
                            Id3V2ReadVariedStringToUnicodeBuffer(Ctx, uFrameFlags, uFrameSize, pString, iBufferLength);
                        // changed from pMetaData->song_album to pMetaData only. it is struct, so it exists always
                        if (pMetaData)
                        {
                            mp3_copy_16_to_8(pMetaData->song_album, pString, iBufferLength);
                        }
                        break;
                    case TT2: /* The title of the composition */
                        pString       = ShortBuf;
                        iBufferLength = TITLE_SIZE;
                        if (pString)
                            /* if we've set up pString, go and read it... */
                            Id3V2ReadVariedStringToUnicodeBuffer(Ctx, uFrameFlags, uFrameSize, pString, iBufferLength);
                        // changed from pMetaData->song_title to pMetaData only. it is struct, so it exists always
                        if (pMetaData)
                        {
                            mp3_copy_16_to_8(pMetaData->song_title, pString, iBufferLength);
                        }
                        break;
                    case TP1: /* The artist */
                        pString       = ShortBuf;
                        iBufferLength = ARTIST_SIZE;
                        if (pString)
                            /* if we've set up pString, go and read it... */
                            Id3V2ReadVariedStringToUnicodeBuffer(Ctx, uFrameFlags, uFrameSize, pString, iBufferLength);
                        // changed from pMetaData->song_artist to pMetaData only. it is struct, so it exists always
                        if (pMetaData)
                        {
                            mp3_copy_16_to_8(pMetaData->song_artist, pString, iBufferLength);
                        }
                        break;
                        /* if you want it to read in a new text frame, set up pString and iBuffer to point to your
                        own uint16_t string and length. */
                    case TCO: /* The Genre */
                        pString       = ShortBuf;
                        iBufferLength = GENRE_SIZE;
                        if (pString)
                            /* if we've set up pString, go and read it... */
                            Id3V2ReadVariedStringToUnicodeBuffer(Ctx, uFrameFlags, uFrameSize, pString, iBufferLength);
                        // changed from pMetaData->song_genre to pMetaData only. it is struct, so it exists always
                        if (pMetaData)
                        {
                            mp3_copy_16_to_8(pMetaData->song_genre, pString, iBufferLength);
                        }
                        break;
                    case TYE: /* The Year */
                        pString       = ShortBuf;
                        iBufferLength = YEAR_SIZE;
                        if (pString)
                            /* if we've set up pString, go and read it... */
                            Id3V2ReadVariedStringToUnicodeBuffer(Ctx, uFrameFlags, uFrameSize, pString, iBufferLength);
                        // changed from pMetaData->song_year to pMetaData only. it is struct, so it exists always
                        if (pMetaData)
                        {
                            mp3_copy_16_to_8(pMetaData->song_year, pString, iBufferLength);
                        }
                        break;
                    case TRK:
                    {
                        /* Track ID */
                        uint16_t pTracknum[TRACKNUM_SIZE];
                        uint32_t TracknumSize;
                        uint32_t TmpVal[3];
                        uint16_t *pTmp;

                        Id3V2ReadVariedStringToUnicodeBuffer(Ctx, uFrameFlags, uFrameSize, pTracknum, TRACKNUM_SIZE);
                        for (pTmp = pTracknum, TracknumSize = 0; (*pTmp != 0) && (*pTmp != '/'); pTmp++)
                            TracknumSize++;

                        TmpVal[0] = 0;
                        TmpVal[1] = 1;
                        while (TracknumSize)
                        {
                            pTmp--;
                            TmpVal[2] = *pTmp - '0';
                            TmpVal[0] += TmpVal[1] * TmpVal[2];
                            TmpVal[1] *= 10;
                            TracknumSize--;
                        }

                        pMetaData->track_number = TmpVal[0];
                        break;
                    }
                    default:
                        pString       = NULL;
                        iBufferLength = 0;
                        break;
                }

                /* Some of the 'Txxx' frames aren't strings, but numbers stored as text strings (i.e. "100" instead of
                100. these don't fit the above processing, so you'll have to rewrite the algorithm a bit. */
            }
            /* if you want to handle a new type of frame, put that here! */
            Ctx->cci_dec_seek(0, lStartOfFrame + uFrameSize, SEEK_SET, Ctx->user_data);
        }
        else
            /* seek to the next frame. */
            Ctx->cci_dec_seek(0, uFrameSize, SEEK_CUR, Ctx->user_data);
    }
    else
        /* seek to where we started reading */
        Ctx->cci_dec_seek(0, -6, SEEK_CUR, Ctx->user_data);

    return bValidFrame;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          uint32_t Id3V2ReadInt(CCI_Ctx *Ctx, bool bSyncSafe)
//
//   Type:          Function
//
//   Description:   Reads in an Integer from an ID3V2 tag
//                  Decision about whether this should be a Sync Safe read
//                  is dependent upon bSyncSafe.
//                  Note: Only Version 4 supports Sync Safe lengths in frames.
//                  Reference documents at www.id3.org
//
//   Inputs:        CCI_Ctx *Ctx            Pointer to CCI Context structure
//                  bool bSyncSafe          Flag that this is a SyncSafe read.
//
//   Outputs:       The SyncSafe integer read
//
//   Notes:
//<
////////////////////////////////////////////////////////////////////////////////
uint32_t Id3V2ReadInt(CCI_Ctx *Ctx, bool bSyncSafe)
{
    int32_t i;
    uint32_t iTemp   = 0;
    uint32_t iReturn = 0;
    for (i = 0; i < 4; i++)
    {
        Ctx->cci_dec_read(0, (uint8_t *)&iTemp, 1, 0, Ctx->user_data);
        if (!bSyncSafe)
            iReturn = (iReturn << 8) + iTemp; // Version 3 is not sync-safe.
        else                                  // Assume future releases will include sync-safe integers.
            iReturn = (iReturn << 7) + iTemp; // Version 4 is sync-safe
    }
    return iReturn;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          uint32_t Id3V2ReadIntv2(CCI_Ctx *Ctx, bool bSyncSafe)
//
//   Type:          Function
//
//   Description:   Reads in an Integer from an ID3V2 tag for version 02
//                  Decision about whether this should be a Sync Safe read
//                  is dependent upon bSyncSafe.
//                  Note: Only Version 4 supports Sync Safe lengths in frames.
//                  Reference documents at www.id3.org
//
//   Inputs:        CCI_Ctx *Ctx        Pointer to CCI Context structure
//                  bool bSyncSafe      Flag that this is a SyncSafe read.
//
//   Outputs:       The SyncSafe integer read
//
//   Notes:
//<
////////////////////////////////////////////////////////////////////////////////
uint32_t Id3V2ReadIntv2(CCI_Ctx *Ctx, bool bSyncSafe)
{
    int32_t i;
    uint32_t iTemp   = 0;
    uint32_t iReturn = 0;
    for (i = 0; i < 3; i++)
    {
        Ctx->cci_dec_read(0, (uint8_t *)&iTemp, 1, 0, Ctx->user_data);
        if (!bSyncSafe)
            iReturn = (iReturn << 8) + iTemp; // Version 3 is not sync-safe.
        else                                  // Assume future releases will include sync-safe integers.
            iReturn = (iReturn << 7) + iTemp; // Version 4 is sync-safe
    }
    return iReturn;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          int Id3V2ReadVariedStringToUnicodeBuffer(CCI_Ctx *Ctx, uint32_t uFlags, uint32_t
// uFrameSize,uint16_t*pString,int32_t iBufferLength)
//
//   Type:          Function
//
//   Description:   Reads a string in from a TEXT ID3V2 tag frame.  These strings are prepended with a encoding byte
//
//   Inputs:        CCI_Ctx *Ctx                Pointer to CCI Context structure
//                  uint32_t uFlags             Frame flags
//                  uint32_t uFrameSize         Size of the frame data
//                  uint16_t *pString               Pointer to the buffer to write the string into
//                  int32_t iBufferLength       Length of the buffer pointed to
//
//   Outputs:       pString is filled in. (and null terminated)
//
//   Notes:
//<
////////////////////////////////////////////////////////////////////////////////
int Id3V2ReadVariedStringToUnicodeBuffer(
    CCI_Ctx *Ctx, uint32_t uFlags, uint32_t uFrameSize, uint16_t *pString, int32_t iBufferLength)
{
    int32_t iTemp = 0;
    /* Read in the encoding type */
    Ctx->cci_dec_read(0, (uint8_t *)&iTemp, 1, 0, Ctx->user_data);
    uFrameSize--; /* subtract one off for the encoding type */

    switch (iTemp)
    {
        case ENCODING_ISO_8559:
        {
            int32_t iCounter = 0;
            if (iBufferLength > 0)
            {
                // ID3V2 is always IS0-8859 which is a direct DBCS->UTF16 map so no need to use code page!
                //				#ifdef NON_UTF16_BUILD  // Systemtest Build
                //				#else
                //	    			uint8_t Bufstring[MAX_PTP_STRING_CHARS*4];
                //					uint8_t *pStringbuf = &Bufstring[0];
                //				#endif

                if (uFrameSize >= iBufferLength)
                    uFrameSize = iBufferLength - 1;
                // ID3V2 is always IS0-8859 which is a direct DBCS->UTF16 map so no need to use code page!
                //				#ifdef NON_UTF16_BUILD  // Systemtest Build
                //				#else
                //			    	memset((void *)Bufstring, 0 ,(uFrameSize+1));
                //			        Fread(iHandle, (uint8_t  *)Bufstring, uFrameSize);
                //				#endif

                /*
                    while(iCounter < uFrameSize)
                  {

                //ID3V2 is always IS0-8859 which is a direct DBCS->UTF16 map so no need to use code page!
                //					#ifdef NON_UTF16_BUILD  // Systemtest Build
                      // Here I'm assuming ASCII char set.
                      // Due to unavailablility of Unicode Map I'll update it for DBCS latter.
                      pString[iCounter] = 0x0020; // Space in MSB //0;
                      Fread(iHandle, (uint8_t *)&pString[iCounter], 1);
                      iCounter++;
                      pString[iCounter] = 0x0000; // Terminate String
                //					#else
                //						pStringbuf += txt_ConvertDbcsToUtf16c((char *)pStringbuf,pString);
                //						pString++;
                //						iCounter++;
                //					#endif
                    }
                */

                {
                    uint8_t TempDBCSBuffer[MAX_PTP_STRING_CHARS];
                    char *pTempDBCSBuffer = (char *)TempDBCSBuffer;
                    uint8_t ByteConsumed  = 0;

                    if (uFrameSize >= MAX_PTP_STRING_CHARS)
                        uFrameSize = MAX_PTP_STRING_CHARS - 1;

                    Ctx->cci_dec_read(0, (uint8_t *)&TempDBCSBuffer, uFrameSize, 0, Ctx->user_data);
                    TempDBCSBuffer[uFrameSize] = 0;

                    while (uFrameSize > iCounter)
                    {
                        ByteConsumed = mp3_copy_two_byte(pTempDBCSBuffer, pString);
                        pTempDBCSBuffer += ByteConsumed;
                        iCounter = iCounter + ByteConsumed;
                        if (*pString > 0)
                            pString++;
                    }
                    *pString = 0x00;
                }
            }
            // DBCSToUnicode(TempString,pString,iBufferLength);
        }
        break;
        case ENCODING_UTF_16:
            /* read in the BOM */
            Ctx->cci_dec_read(0, (uint8_t *)&iTemp, 2, 0, Ctx->user_data);
            if (iTemp == BOM_BIG_ENDIAN || iTemp == BOM_LITTLE_ENDIAN)
            {
                uFrameSize -= 2;
                Id3V2ReadUTF16String(Ctx, iTemp, uFlags, uFrameSize, pString, iBufferLength);
            }
            break;
        case ENCODING_UTF_16_NOBOM:
            Id3V2ReadUTF16String(Ctx, BOM_BIG_ENDIAN, uFlags, uFrameSize, pString, iBufferLength);
            break;
        case ENCODING_UTF_8:
            Id3V2ReadUTF8String(Ctx, uFrameSize, pString, iBufferLength);
            break;
    }
    return 1;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          int Id3V2ReadUTF16String(CCI_Ctx *Ctx, uint32_t uEndianNess, uint32_t uFlags, uint32_t uFrameSize,
// uint16_t *pString, int32_t iBufferLength)
//
//   Type:          Function
//
//   Description:   Reads a UTF-16 string in from a TEXT ID3V2 tag frame.
//
//   Inputs:        CCI_Ctx *Ctx                    Pointer to CCI Context structure
//                  uint32_t uEndianNess            BOM_BIG_ENDIAN or BOM_LITTLE_ENDIAN
//                  uint32_t uFlags                 The flags of the frame that contains the string
//                  uint32_t uFrameSize             Size of the frame data
//                  uint16_t *pString               Pointer to the buffer to write the string into
//                  int32_t iBufferLength           Length of the buffer pointed to
//
//   Outputs:       pString is filled in. (and null terminated)
//
//   Notes:
//<
////////////////////////////////////////////////////////////////////////////////
int Id3V2ReadUTF16String(
    CCI_Ctx *Ctx, uint32_t uEndianNess, uint32_t uFlags, uint32_t uFrameSize, uint16_t *pString, int32_t iBufferLength)
{
    int32_t iTemp1 = 0;
    int32_t iTemp2 = 0;
    uint16_t uValue;
    int32_t iCounter = 0;
    uFrameSize       = (uFrameSize + 1) / 2; /* Reference from 3500. The frame size should be bytes, now its words. */
    if (uFrameSize >= iBufferLength)
        uFrameSize = iBufferLength - 1;

    while (iCounter < uFrameSize)
    {
        /* don't need to check for iBUfferlength overflow (handled below in DBCSToUnicode) */
        Ctx->cci_dec_read(0, (uint8_t *)&iTemp1, 1, 0, Ctx->user_data);
        Ctx->cci_dec_read(0, (uint8_t *)&iTemp2, 1, 0, Ctx->user_data);
        if (uEndianNess == BOM_BIG_ENDIAN)
            uValue = (iTemp1 << 8) | (iTemp2);
        else
            uValue = (iTemp2 << 8) | (iTemp1);
        pString[iCounter++] = uValue;
        pString[iCounter]   = 0; /* null terminate */
    }
    return 1;
}
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          int Id3V2ReadUTF8String(CCI_Ctx *Ctx, uint32_t uFrameSize, uint16_t *pString, int32_t iBufferLength)
//
//   Type:          Function
//
//   Description:   Reads a UTF-8 string in from a tag frame.
//
//   Inputs:        CCI_Ctx *Ctx                    Pointer to CCI Context structure
//                  uint32_t uFrameSize             Size of the frame data
//                  uint16_t *pString               Pointer to the buffer to write the string into
//                  int32_t iBufferLength           Length of the buffer pointed to
//
//   Outputs:       pString is filled in. (and null terminated)
//
//   Notes:
//<
////////////////////////////////////////////////////////////////////////////////
int Id3V2ReadUTF8String(CCI_Ctx *Ctx, uint32_t uFrameSize, uint16_t *pString, int32_t iBufferLength)
{
    int32_t i       = 0;
    uint16_t iTemp1 = 0;
    if (uFrameSize >= iBufferLength)
        uFrameSize = iBufferLength - 1;

    for (i = 0; i < uFrameSize; i++)
    {
        Ctx->cci_dec_read(0, (uint8_t *)&iTemp1, 1, 0, Ctx->user_data);
        pString[i] = iTemp1;
    }
    pString[uFrameSize] = 0; /* null terminate */

    return 1;
}
///////////////////////////////////////////////////////////////////////////////
// End of file
////////////////////////////////////////////////////////////////////////////////
//! @}
