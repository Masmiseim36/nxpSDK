/*
 * Copyright (c) 2011-2020 Cadence Design Systems, Inc.
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
#include "xa_src_pp_waveio.h"

int read_wave_hdr(FILE *fp_in, wave_header *pwave_info)
{
    unsigned int hdrBuf[16];
    unsigned char *cbuf;
    unsigned int bytes_per_sec;
    int i;
    cbuf = (unsigned char *)hdrBuf;

    fread(cbuf, 1, 4, fp_in);
    if (cbuf[0] != 'R' || cbuf[1] != 'I' || cbuf[2] != 'F' || cbuf[3] != 'F')
        return -1;

    fread(&i,4,1,fp_in);

    fread(cbuf, 1, 8, fp_in);
    if (cbuf[0] != 'W' || cbuf[1] != 'A' || cbuf[2] != 'V' || cbuf[3] != 'E' ||
        cbuf[4] != 'f' || cbuf[5] != 'm' || cbuf[6] != 't' || cbuf[7] != ' ')
        return -1;

    fread(&i,4,1,fp_in); /* Read header length */
    if (i>16) i=16;      /* Restrict the read to 1st 16 bytes */
    fread(cbuf,i,1,fp_in);  /* Read header into cbuf */

    pwave_info->channels = *(short *)(&cbuf[2]);    /* Number of channels */
    pwave_info->fs_in = *(int *)(&cbuf[4]);         /* Samples per second */

    bytes_per_sec = *(int *)(&cbuf[8]);  /* Read bytes per second */
    pwave_info->bytes_per_sample = bytes_per_sec / (pwave_info->fs_in * pwave_info->channels);    /* Bytes per sample */

    while(1)
    {
        fread(cbuf, 1, 4, fp_in);

		if (cbuf[0] == 'd' && cbuf[1] == 'a' && cbuf[2] == 't' && cbuf[3] == 'a')
		{
			fread(&pwave_info->length, 4, 1, fp_in);
			break;
		}
		if (feof(fp_in))
		{
			fprintf(stderr,"Couldn't find data chunk\n");
			return -1;
		}

		fseek(fp_in,-3,SEEK_CUR);
	}    

    return 0;
}

void write_wav_hdr(FILE *fp_out, wave_header *pwave_info)
{
    short s;
    int i;

    fwrite("RIFF",4,1,fp_out);
    i = 0;
    fwrite(&i,4,1,fp_out);  /* Current length without wave header */

    fwrite("WAVEfmt ", 1, 8, fp_out);  /* 2 labels */
    i = 16;
    fwrite(&i,4,1,fp_out);

    s = 1;
    fwrite(&s,2,1,fp_out);

    fwrite(&pwave_info->channels,2,1,fp_out);   /* Number of channels */
    fwrite(&pwave_info->fs_out,4,1,fp_out);     /* Samples per second */

    i = pwave_info->fs_out * pwave_info->channels * pwave_info->out_bytes_per_sample;
    fwrite(&i,4,1,fp_out);                      /* bytes per sec */

    s = pwave_info->out_bytes_per_sample * pwave_info->channels;
    fwrite(&s,2,1,fp_out);                      /* block alignment */

    s = pwave_info->out_bytes_per_sample * 8;
    fwrite(&s,2,1,fp_out);                      /* bits per sample */

    fwrite("data",4,1,fp_out);
    i = 0;
    fwrite(&i,4,1,fp_out);                      /* PCM bytes */
}

void write_pcm_length(FILE *fp_out)
{
    unsigned int val;
    unsigned int file_length;

    fseek(fp_out,0,SEEK_END);
    file_length = ftell(fp_out);

    fseek(fp_out,4,SEEK_SET);
    val = file_length-8;
    fwrite(&val,4,1,fp_out);

    fseek(fp_out,40,SEEK_SET);
    val= file_length-44;
    fwrite(&val,4,1,fp_out);
}

