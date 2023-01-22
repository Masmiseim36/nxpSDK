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


#include <stdio.h>
#include <ctype.h>
#include "xa_type_def.h"
#include <id3_tag_decode.h>

#ifndef ENABLE_ID3
#define ENABLE_ID3 1
#endif

#if ENABLE_ID3

#define CHAR4(c1, c2, c3, c4) \
  (int)(((unsigned char)c1 << 24) | \
        ((unsigned char)c2 << 16) | \
        ((unsigned char)c3 << 8) | \
        ((unsigned char)c4))

#ifndef MSVC_BUILD
#if 0
static inline unsigned int
    umin(unsigned int a, unsigned int b) {
        return (a < b ? a : b);
}
#endif
unsigned int umin(unsigned int a, unsigned int b) {
    return (a < b ? a : b);
}

#else
unsigned int umin(unsigned int a, unsigned int b) {
    return (a < b ? a : b);
}
#endif

/*******************************************************************************
*  function name   : display
*
*  description     : display ID3 tag contents.
*
*  arguments       : input parameters
*
*  values returned : 0
*******************************************************************************/

static void
    display2(const id3_v2_frame_struct *const src,
    int                              size,
    const char                *const disp)
{
    int j;

    fprintf(stdout, "%s", disp);

    for(j = 0; j < size; j++) {
        int c = src->frame_data[j];
        if (c)
        {
            if (!isprint(c))
                break;
            fprintf(stdout, "%c", c);
        }
    }
    fprintf(stdout, "\n");
}

static VOID display1(WORD8 src[], WORD32 size, WORD8 disp[])
{
    WORD32 j;

    fprintf(stdout, "%s", disp);
    for(j = 0; j < size ; j++)
    {
        int c = src[j];
        if (c)
        {
            if (!isprint(c))
                break;
            fprintf(stdout, "%c", c);
        }
    }
    fprintf(stdout, "\n");
}

/*******************************************************************************
*  function name   : init_id3_header
*
*  description     : initialise the max filed size of teh farem.
*
*  arguments       : input parameters
*
*  values returned : 0
*******************************************************************************/

VOID init_id3v2_field(id3v2_struct *id3v2)
{
    id3v2->album_movie_show_title.max_size = MAX_TAG_FRAME_SIZE;
    id3v2->composer_name.max_size = MAX_TAG_FRAME_SIZE;
    id3v2->content_type.max_size = MAX_TAG_FRAME_SIZE;
    id3v2->encoded_by.max_size = MAX_TAG_FRAME_SIZE;
    id3v2->lyricist_text_writer.max_size = MAX_TAG_FRAME_SIZE;
    id3v2->content_group_description.max_size = MAX_TAG_FRAME_SIZE;
    id3v2->title_songname_content_description.max_size = MAX_TAG_FRAME_SIZE;
    id3v2->medxa_type.max_size = MAX_TAG_FRAME_SIZE;
    id3v2->original_album_movie_show_title.max_size = MAX_TAG_FRAME_SIZE;
    id3v2->original_filename.max_size = MAX_TAG_FRAME_SIZE;
    id3v2->original_lyricist_text_writer.max_size = MAX_TAG_FRAME_SIZE;
    id3v2->original_artist_performer.max_size = MAX_TAG_FRAME_SIZE;
    id3v2->file_owner_licensee.max_size = MAX_TAG_FRAME_SIZE;
    id3v2->lead_performer_soloist.max_size = MAX_TAG_FRAME_SIZE;
    id3v2->publisher.max_size = MAX_TAG_FRAME_SIZE;
    id3v2->private_frame.max_size = MAX_TAG_FRAME_SIZE;
    id3v2->other_info.max_size = MAX_TAG_FRAME_SIZE;

    /* resetting the flag to indicate presese of frame */
    id3v2->album_movie_show_title.tag_present = 0;
    id3v2->composer_name.tag_present = 0;
    id3v2->content_type.tag_present = 0;
    id3v2->encoded_by.tag_present = 0;
    id3v2->lyricist_text_writer.tag_present = 0;
    id3v2->content_group_description.tag_present = 0;
    id3v2->title_songname_content_description.tag_present = 0;
    id3v2->medxa_type.tag_present = 0;
    id3v2->original_album_movie_show_title.tag_present = 0;
    id3v2->original_filename.tag_present = 0;
    id3v2->original_lyricist_text_writer.tag_present = 0;
    id3v2->original_artist_performer.tag_present = 0;
    id3v2->file_owner_licensee.tag_present = 0;
    id3v2->lead_performer_soloist.tag_present = 0;
    id3v2->publisher.tag_present = 0;
    id3v2->private_frame.tag_present = 0;
    id3v2->other_info.tag_present = 0;

    /* resetting the flag which indicates that size of the frame has
    exceeded the max buffer size*/
    id3v2->album_movie_show_title.exceeds_buffer_size = 0;
    id3v2->composer_name.exceeds_buffer_size = 0;
    id3v2->content_type.exceeds_buffer_size = 0;
    id3v2->encoded_by.exceeds_buffer_size = 0;
    id3v2->lyricist_text_writer.exceeds_buffer_size = 0;
    id3v2->content_group_description.exceeds_buffer_size = 0;
    id3v2->title_songname_content_description.exceeds_buffer_size = 0;
    id3v2->medxa_type.exceeds_buffer_size = 0;
    id3v2->original_album_movie_show_title.exceeds_buffer_size = 0;
    id3v2->original_filename.exceeds_buffer_size = 0;
    id3v2->original_lyricist_text_writer.exceeds_buffer_size = 0;
    id3v2->original_artist_performer.exceeds_buffer_size = 0;
    id3v2->file_owner_licensee.exceeds_buffer_size = 0;
    id3v2->lead_performer_soloist.exceeds_buffer_size = 0;
    id3v2->publisher.exceeds_buffer_size = 0;
    id3v2->private_frame.exceeds_buffer_size = 0;
    id3v2->other_info.exceeds_buffer_size = 0;

    id3v2->bytes_consumed = 0;
    id3v2->header_end = 0;
}

/*******************************************************************************
*  function name   : search_id3_v2
*
*  description     : finds if ID3V2 starts at the start of given buffer.
*
*  arguments       : input parameters
*                   buffer          input buffer
*
*  values returned : FLAG   1: ID3 found   0: ID3 not found
*******************************************************************************/
WORD32 search_id3_v2(UWORD8 *buffer)
{
    UWORD32 temp;

    temp  = buffer[0] << 16;
    temp |= buffer[1] <<  8;
    temp |= buffer[2];

    if(ID3V2 == temp)
    {
        return 1;   /* ID3 found */
    }

    return 0;       /* ID3 not found */
}

/*******************************************************************************
*  function name   : search_id3_v1
*
*  description     : finds if ID3V1 starts at the start of given buffer.
*
*  arguments       : input parameters
*                   buffer          input buffer
*
*  values returned : FLAG   1: ID3 found   0: ID3 not found
*******************************************************************************/
WORD32 search_id3_v1(UWORD8 *buffer)
{
    UWORD32 temp;

    temp  = buffer[0] << 16;
    temp |= buffer[1] <<  8;
    temp |= buffer[2];

    if(ID3V1 == temp)
    {
        return 1;   /* ID3 found */
    }

    return 0;       /* ID3 not found */
}

/*******************************************************************************
*  function name   : decode_id3_v1
*
*  description     : decodes ID3V1 tag.
*
*  arguments       : input parameters
*                   buffer              input buffer
*                   id3v1               structure
*
*  values returned : bytes consumed
*******************************************************************************/
WORD32 decode_id3_v1(
    UWORD8 *buffer,
    id3v1_struct *id3v1)
{
    WORD32 bytes_consumed = 0;
    short tag_type;


    /* setting the tag type */
    tag_type = 1;

    bytes_consumed = get_v1_info(buffer, id3v1);

    return bytes_consumed;

}

/*******************************************************************************
*  function name   : get_v1_info
*
*  description     : gets ID3V1 information fields.
*
*  arguments       : input parameters
*                   buffer              input buffer
*                   id3v1               structure
*
*  values returned : bytes consumed
*******************************************************************************/
WORD32 get_v1_info(
    UWORD8 *buffer,
    id3v1_struct *id3v1)
{
    WORD32 i;
    WORD32 bytes_consumed = 0;

    /* get song_title */
    for (i = 0; i < 30; i++)
    {
        id3v1->song_title[i] = buffer[i];
    }
    buffer += 30;
    bytes_consumed += 30;
    display1(id3v1->song_title, 30, (WORD8 *)"song_title : ");

    /* get artist */
    for (i = 0; i < 30; i++)
    {
        id3v1->artist[i] = buffer[i];
    }
    buffer += 30;
    bytes_consumed += 30;
    display1(id3v1->artist, 30, (WORD8 *)"artist : ");

    /* get album */
    for (i = 0; i < 30; i++)
    {
        id3v1->album[i] = buffer[i];
    }
    buffer += 30;
    bytes_consumed += 30;
    display1(id3v1->album, 30, (WORD8 *)"album : ");

    /* get year */
    for (i = 0; i < 4; i++)
    {
        id3v1->year[i] = buffer[i];
    }
    buffer += 4;
    bytes_consumed += 4;
    display1(id3v1->year, 4, (WORD8 *)"year : ");

    /* get comment */
    for (i = 0; i < 30; i++)
    {
        id3v1->comment[i] = buffer[i];
    }
    buffer += 30;
    bytes_consumed += 30;
    display1(id3v1->comment, 30, (WORD8 *)"comment : ");

    /* get genre */
    for (i = 0; i < 1; i++)
    {
        id3v1->genre[i] = buffer[i];
    }
    buffer += 1;
    bytes_consumed += 1;
    fprintf(stdout, "genre : %d\n", id3v1->genre[0]);

    return bytes_consumed;
}

/*******************************************************************************
*  function name   : decode_id3_v2
*
*  description     : decodes ID3V2 tag.
*
*  arguments       : input parameters
*                   buffer              input buffer
*                   id3v2               structure
*                   continue_flag       FLAG to indicate whether it is first call or not
*                   insize              input buffer size
*
*  values returned : bytes consumed
*******************************************************************************/
WORD32 decode_id3_v2(const char   *const buffer,
    id3v2_struct *const id3v2,
    WORD32              continue_flag,
    WORD32              insize)
{
    UWORD32 size = 0, flag;
    WORD32 i, buf_update_val;
    UWORD8 buf[4], frame_header[10], id3_buffer[10];
    WORD8 *bitstream_ptr;
    short tag_type;

    WORD32 bytes_consumed = 0;

    if (1 == id3v2->header_end) {
        id3v2->bytes_consumed += insize;
        if (id3v2->bytes_consumed < id3v2->id3_v2_header.size) {
            return 0;
        }
        else {
            id3v2->bytes_consumed = (id3v2->id3_v2_header.size + 10);
            return 1;
        }
    }

    bitstream_ptr = (WORD8 *) id3_buffer;

    if (!continue_flag) {
        bytes_consumed += 3;
        /* setting the tag type */
        tag_type = 2;
        id3v2->id3_v2_header.version  = buffer[bytes_consumed + 0] << 8;
        id3v2->id3_v2_header.version |= buffer[bytes_consumed + 1];
        id3v2->id3_v2_header.flag = buffer[bytes_consumed + 2];

        /* making the msb of each byte zero */
        buf[0] = buffer[bytes_consumed + 6] & 0x7f;
        buf[1] = buffer[bytes_consumed + 5] & 0x7f;
        buf[2] = buffer[bytes_consumed + 4] & 0x7f;
        buf[3] = buffer[bytes_consumed + 3] & 0x7f;

        bytes_consumed += 7;

        /* concatenation the bytes after making 7th bit zero to get 28 bits size */
        size  = buf[0];
        size |= (buf[1] <<7);
        size |= (buf[2] <<14);
        size |= (buf[3] <<21);
        /* storing the size */
        id3v2->id3_v2_header.size = size;

        /* check for extended header */
        if (id3v2->id3_v2_header.flag & 0x20) {
            for (i = 0; i < 10; i++) {
                bitstream_ptr[i] = buffer[bytes_consumed + i];
            }

            i = 0;
            bytes_consumed += 10;

            size = bitstream_ptr[i++]  << 24;
            size |= bitstream_ptr[i++] << 16;
            size |= bitstream_ptr[i++] << 8;
            size |= bitstream_ptr[i++];

            /* two bytes for flag */
            i += 2;
            {
                UWORD32 padding_size;
                padding_size   = bitstream_ptr[i++]  << 24;
                padding_size   |= bitstream_ptr[i++] << 16;
                padding_size   |= bitstream_ptr[i++] << 8;
                padding_size   |= bitstream_ptr[i++];

                /* skipping the padding and frame size number of bytes */
                bytes_consumed += (padding_size + size);
            }
        }
    }

    while (id3v2->header_end != 1) {
        char *key;
        id3_v2_frame_struct *value;
        unsigned int avail_inp;

        /* reading the 10 bytes to get the frame header */

        for (i = 0; i < 10; i++) {
            frame_header[i] = buffer[bytes_consumed + i];
        }
        bytes_consumed += 10;

        /* getting the size from the header */
        size = frame_header[4]  << 24;
        size |= frame_header[5] << 16;
        size |= frame_header[6] << 8;
        size |= frame_header[7];

        /* decoding the flag, currently not used */
        flag = frame_header[8]<<8;
        flag |= frame_header[9];

        avail_inp = insize - bytes_consumed;

        /* switching to the frame type */
        switch (CHAR4(frame_header[0],
            frame_header[1],
            frame_header[2],
            frame_header[3])) {
        case CHAR4('A', 'E', 'N', 'C'):
        case CHAR4('A', 'P', 'I', 'C'):
        case CHAR4('C', 'O', 'M', 'M'):
        case CHAR4('C', 'O', 'M', 'R'):
        case CHAR4('E', 'N', 'C', 'R'):
        case CHAR4('E', 'Q', 'U', 'A'):
        case CHAR4('E', 'T', 'C', 'O'):
        case CHAR4('G', 'E', 'O', 'B'):
        case CHAR4('G', 'R', 'I', 'D'):
        case CHAR4('I', 'P', 'L', 'S'):
        case CHAR4('L', 'I', 'N', 'K'):
        case CHAR4('M', 'C', 'D', 'I'):
        case CHAR4('M', 'L', 'L', 'T'):
        case CHAR4('O', 'W', 'N', 'E'):
        case CHAR4('P', 'C', 'N', 'T'):
        case CHAR4('P', 'O', 'P', 'M'):
        case CHAR4('P', 'O', 'S', 'S'):
        case CHAR4('R', 'B', 'U', 'F'):
        case CHAR4('R', 'V', 'A', 'D'):
        case CHAR4('R', 'V', 'R', 'B'):
        case CHAR4('S', 'Y', 'L', 'T'):
        case CHAR4('S', 'Y', 'T', 'C'):
        case CHAR4('T', 'B', 'P', 'M'):
        case CHAR4('T', 'C', 'O', 'P'):
        case CHAR4('T', 'D', 'A', 'T'):
        case CHAR4('T', 'D', 'L', 'Y'):
        case CHAR4('T', 'F', 'L', 'T'):
        case CHAR4('T', 'I', 'M', 'E'):
        case CHAR4('T', 'K', 'E', 'Y'):
        case CHAR4('T', 'L', 'A', 'N'):
        case CHAR4('T', 'L', 'E', 'N'):
        case CHAR4('T', 'M', 'E', 'D'):
        case CHAR4('T', 'O', 'F', 'N'):
        case CHAR4('T', 'O', 'L', 'Y'):
        case CHAR4('T', 'O', 'R', 'Y'):
        case CHAR4('T', 'P', 'E', '2'):
        case CHAR4('T', 'P', 'E', '3'):
        case CHAR4('T', 'P', 'E', '4'):
        case CHAR4('T', 'P', 'O', 'S'):
        case CHAR4('T', 'R', 'C', 'K'):
        case CHAR4('T', 'R', 'D', 'A'):
        case CHAR4('T', 'R', 'S', 'N'):
        case CHAR4('T', 'R', 'S', 'O'):
        case CHAR4('T', 'S', 'I', 'Z'):
        case CHAR4('T', 'S', 'R', 'C'):
        case CHAR4('T', 'S', 'S', 'E'):
        case CHAR4('T', 'Y', 'E', 'R'):
        case CHAR4('T', 'X', 'X', 'X'):
        case CHAR4('U', 'F', 'I', 'D'):
        case CHAR4('U', 'S', 'E', 'R'):
        case CHAR4('U', 'S', 'L', 'T'):
        case CHAR4('W', 'C', 'O', 'M'):
        case CHAR4('W', 'C', 'O', 'P'):
        case CHAR4('W', 'O', 'A', 'F'):
        case CHAR4('W', 'O', 'A', 'R'):
        case CHAR4('W', 'O', 'A', 'S'):
        case CHAR4('W', 'O', 'R', 'S'):
        case CHAR4('W', 'P', 'A', 'Y'):
        case CHAR4('W', 'P', 'U', 'B'):
        case CHAR4('W', 'X', 'X', 'X'):
        case CHAR4('T', 'I', 'T', '3'):
            key = "other_info : ";
            value = &id3v2->other_info;
            break;
        case CHAR4('P', 'R', 'I', 'V'):
            key = "private_frame : ";
            value = &id3v2->private_frame;
            break;
        case CHAR4('T', 'A', 'L', 'B'):
            key = "album_movie_show_title : ";
            value = &id3v2->album_movie_show_title;
            break;
        case CHAR4('T', 'C', 'O', 'M'):
            key = "composer_name : ";
            value = &id3v2->composer_name;
            break;
        case CHAR4('T', 'C', 'O', 'N'):
            key = "content_type : ";
            value = &id3v2->content_type;
            break;
        case CHAR4('T', 'E', 'N', 'C'):
            key = "encoded_by : ";
            value = &id3v2->encoded_by;
            break;
        case CHAR4('T', 'E', 'X', 'T'):
            key = "lyricist_text_writer : ";
            value = &id3v2->lyricist_text_writer;
            break;
        case CHAR4('T', 'I', 'T', '1'):
            key = "content_group_description : ";
            value = &id3v2->content_group_description;
            break;
        case CHAR4('T', 'I', 'T', '2'):
            key = "title_songname_content_description : ";
            value = &id3v2->title_songname_content_description;
            break;
        case CHAR4('T', 'O', 'A', 'L'):
            key = "original_album_movie_show_title : ";
            value = &id3v2->original_album_movie_show_title;
            break;
        case CHAR4('T', 'O', 'P', 'E'):
            key = "original_artist_performer : ";
            value = &id3v2->original_artist_performer;
            break;
        case CHAR4('T', 'O', 'W', 'N'):
            key = "file_owner_licensee : ";
            value = &id3v2->file_owner_licensee;
            break;
        case CHAR4('T', 'P', 'E', '1'):
            key = "lead_performer_soloist : ";
            value = &id3v2->lead_performer_soloist;
            break;
        case CHAR4('T', 'P', 'U', 'B'):
            key =  "publisher : ";
            value = &id3v2->publisher;
            break;
        default:
            /* skipping the read 10 bytes */
            buf_update_val = -10;
            id3v2->header_end = 1;
            value = 0;
            key = 0;
            break;
        }

        if (value != 0) {
            buf_update_val = get_info(&buffer[bytes_consumed], avail_inp, size, value);
        }

        /* Negative value for buf_update_val means one of two things:
        * 1. The default case happened and we're done with ID3V2 tag
        *    frames, or
        * 2. get_info() returned -1 to indicate that more input is
        *    required to decode this frame of the tag.
        */
        if (buf_update_val >= 0) {
            display2(value, umin(value->max_size, buf_update_val), key);
        }

        if (buf_update_val == -1) {
            id3v2->bytes_consumed += bytes_consumed;
            return 1;
        }

        bytes_consumed += buf_update_val;

        /* Is there enough input left (10 bytes) to begin decoding another
        * frame? If not, bag out temporarily now. The caller will refill
        * our input buffer and call us again with continue_flag == 1.
        */
        if (insize - bytes_consumed < 10) {
            id3v2->bytes_consumed += bytes_consumed;
            return 0; /* not completely decoded */
        }
    }

    id3v2->bytes_consumed += bytes_consumed;
    if ((id3v2->bytes_consumed + 10) < id3v2->id3_v2_header.size) {
        return 0; /* not completely decoded */
    }

    return 1; /* completely decoded */
}

/*******************************************************************************
*  function name   : get_id3_v2_bytes
*
*  description     : tells the size of ID3V2 tag.
*
*  arguments       : input parameters
*                   buffer          input buffer
*
*  values returned : bytes consumed
*******************************************************************************/
WORD32 get_id3_v2_bytes(UWORD8 *buffer)
{
    WORD32 size;

    /* making the msb of each byte zero */
    size  =  (buffer[9] & 0x7f);
    size |= ((buffer[8] & 0x7f) << 7 );
    size |= ((buffer[7] & 0x7f) << 14);
    size |= ((buffer[6] & 0x7f) << 21);

    return (size + 10);
}

/*******************************************************************************
*  function name   : get_info
*
*  description     : read the frame information from the input buffer.
*
*  arguments       : input parameters
*
*  values returned : update value for buffer
*******************************************************************************/
WORD32 get_info(const    char       *inp_buffer,
    unsigned int         avail_inp,
    WORD32               tag_size,
    id3_v2_frame_struct *dest)
{
    WORD32 j;

    /* setting the tag to indicate the presence of frame */
    dest->tag_present = 1;
    /* If there isn't enough input available, we punt back to the top
    * level and ask for more.
    */
    if (avail_inp < umin(tag_size, dest->max_size)) {
        return -1;
    }
    if (dest->max_size >= tag_size) {
        for(j = 0; j < tag_size ; j++) {
            dest->frame_data[j] = inp_buffer[j];
        }
    }
    else {
        dest->exceeds_buffer_size = 1;
        for(j = 0; j < dest->max_size ; j++) {
            dest->frame_data[j] = inp_buffer[j];
        }
    }
    return tag_size;
}

#endif /* ENABLE_ID3 */
