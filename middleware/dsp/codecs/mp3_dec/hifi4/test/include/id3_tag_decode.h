/*
 * Copyright (c) 2022 Cadence Design Systems, Inc.
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



/*******************************************************************************

*  file name       : id3_tag_decode.h

*

*  description     : stores typedefs of structures specific to MP3 tag

*

*  revision history:

*       29 04 2004       DK     creation

******************************************************************************/

#ifndef ID3_TAG_DECODE_H

#define ID3_TAG_DECODE_H



#define MAX_TAG_FRAME_SIZE		100

#define ID3V1		                (0x544147)      /* 0x544147 is TAG in WORD8 */

#define ID3V2		                (0x494433)      /* 0x494433 is ID3 in WORD8 */





/*

	structure corresponding to ID3 tag v1 header.

	this structure has all the field corresponding to ID3 tag v1 header.

*/

typedef struct

{

	WORD32 tag;				// 3 bytes

	WORD16 version;			// 2 bytes

	WORD8 flag;				//1 byte

	WORD32  size;		//4 bytes

}id3_v2_header_struct;

/* structure which will store the frame data and also put a limit max data to be stored */

typedef struct

{

	WORD8 frame_data[MAX_TAG_FRAME_SIZE];

	WORD32  max_size;		//4 bytes

	WORD16 tag_present;

	WORD16 exceeds_buffer_size;

}id3_v2_frame_struct;

/*

	structure corresponding to ID3 tag v2.

	this structure has some of the  field corresponding to ID3 tag v2.

	if user wants to read some more tag information from the MP3 file, he can add that field

	in this structure and pass address of that element to get_inf function in id3_tag_decode.c

	under the corresponding field frame header.

	few fields which are needed are already populated by reading from the TAG header.

*/

typedef struct

{

	id3_v2_frame_struct  album_movie_show_title;

	id3_v2_frame_struct  composer_name;

	id3_v2_frame_struct  content_type;

	id3_v2_frame_struct  encoded_by;

	id3_v2_frame_struct  lyricist_text_writer;

	id3_v2_frame_struct  content_group_description;

	id3_v2_frame_struct  title_songname_content_description;

	id3_v2_frame_struct  medxa_type;

	id3_v2_frame_struct  original_album_movie_show_title;

	id3_v2_frame_struct  original_filename;

	id3_v2_frame_struct  original_lyricist_text_writer;

	id3_v2_frame_struct  original_artist_performer;

	id3_v2_frame_struct  file_owner_licensee;

	id3_v2_frame_struct  lead_performer_soloist;

	id3_v2_frame_struct  publisher;

	id3_v2_frame_struct  private_frame;

	id3_v2_frame_struct  other_info;

    id3_v2_header_struct id3_v2_header;

    WORD32 header_end;

    WORD32 bytes_consumed;

}id3v2_struct;



/*

	structure corresponding to ID3 tag v1.

	this structure has all the field corresponding to ID3 tag v1.

*/

typedef struct

{

	WORD8 song_title[30]; //30 word8acters

	WORD8 artist[30]; //30 word8acters

	WORD8 album[30]; //30 word8acters

	WORD8 year[4]; //4 word8acters

	WORD8 comment[30]; //30 word8acters

	WORD8 genre[1]; //1 byte

}id3v1_struct;



WORD32 get_info(const    char       *inp_buffer,

		unsigned int         avail_inp,

		WORD32               tag_size,

		id3_v2_frame_struct *dest);



WORD32 search_id3_v2(

                UWORD8 *buffer);



WORD32 decode_id3_v2(const char   *const buffer,

		     id3v2_struct *id3v2,

		     WORD32        continue_flag,

		     WORD32        insize);



WORD32 get_id3_v2_bytes(UWORD8 *buffer);



WORD32 get_v1_info(

                UWORD8 *buffer,

                id3v1_struct *id3v1);



WORD32 search_id3_v1(

                UWORD8 *buffer);



WORD32 decode_id3_v1(

                UWORD8 *buffer,

                id3v1_struct *id3v1);



void init_id3v2_field(id3v2_struct *id3v2);



#endif

