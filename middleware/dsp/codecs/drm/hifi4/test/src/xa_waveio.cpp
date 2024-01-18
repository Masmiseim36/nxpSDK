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


#include "xa_waveio.h"

#define	UNCOMPRESSED_PCM	0x01

#ifdef ENABLE_WAVE_INP_SUPPORT
int WAV_InputOpen (HANDLE_WAVE *pWave, const char *ip_filename)
{
	unsigned int hdrBuf[16];
	unsigned char *cbuf;
	unsigned int bytes_per_sec;
	int i;
	HANDLE_WAVE wave = (HANDLE_WAVE)calloc(1, sizeof(WAVE));
	FILE *fp_in;

	if(wave == NULL)
	{
		fprintf(stderr, "\n WAVE sturcture Memory allocation failed in wave input open module\n");
WAVE_InputOpen_Fail:	
		if(wave->pfile)
		{
			fclose(wave->pfile);
		}
		if(wave)
		{
			free(wave);
			wave = NULL;
		}
		pWave = NULL;
		return -1;
	}
	
	wave->pfile = fopen(ip_filename, "rb");
	if(wave->pfile == NULL)
	{
		fprintf(stderr, "\n Wave input file open failed in wave input open module : %s\n", ip_filename);
		goto WAVE_InputOpen_Fail;
	}	
	
	{
		fp_in = wave->pfile;
		cbuf = (unsigned char *)hdrBuf;


		fread(cbuf, 1, 4, fp_in);
		if (cbuf[0] != 'R' || cbuf[1] != 'I' || cbuf[2] != 'F' || cbuf[3] != 'F')
		{
			goto WAVE_InputOpen_Fail;
		}

		fread(&i,4,1,fp_in);

		fread(cbuf, 1, 8, fp_in);
		if (cbuf[0] != 'W' || cbuf[1] != 'A' || cbuf[2] != 'V' || cbuf[3] != 'E' ||
			cbuf[4] != 'f' || cbuf[5] != 'm' || cbuf[6] != 't' || cbuf[7] != ' ')
		{
			goto WAVE_InputOpen_Fail;
		}

		fread(&i,4,1,fp_in); /* Read header length */
		if (i>16) i=16;      /* Restrict the read to 1st 16 bytes */
		fread(cbuf,i,1,fp_in);  /* Read header into cbuf */

		wave->header.AudioFormat = *(short *)(&cbuf[0]);    /* AudioFormat: PCM uncompreseed */
		wave->header.numChannels = *(short *)(&cbuf[2]);    /* Number of channels */
		wave->header.sampleRate = *(int *)(&cbuf[4]);         /* Samples per second */

		bytes_per_sec = *(int *)(&cbuf[8]);  /* Read bytes per second */
		wave->header.ByteRate = bytes_per_sec / (wave->header.sampleRate * wave->header.numChannels);    /* Bytes per second */

		wave->header.bitsPerSample = *(short *)(&cbuf[14]);    /* Bits per sample */

		while(1)
		{
			fread(cbuf, 1, 4, fp_in);

			if (cbuf[0] == 'd' && cbuf[1] == 'a' && cbuf[2] == 't' && cbuf[3] == 'a')
			{
				fread(&wave->header.Subchunk2Size, 4, 1, fp_in);
				break;
			}
			if (feof(fp_in))
			{
				fprintf(stderr,"Couldn't find data chunk\n");
				goto WAVE_InputOpen_Fail;
			}

			fseek(fp_in,-3,SEEK_CUR);
		}    

	}

	if((wave->header.AudioFormat != UNCOMPRESSED_PCM) && (wave->header.AudioFormat != 0xFFFE))
	{
		fprintf(stderr, "\n Unsupported PCM compression format in wave input open module\n");
		goto WAVE_InputOpen_Fail;	
	}
	else
	{
		wave->header.AudioFormat = UNCOMPRESSED_PCM;
	}


	*pWave = wave;
	return 0;
}


int WAV_InputRead (HANDLE_WAVE wave, void *ip_buffer, unsigned int Samples2Read, int ip_bitspersample)
{
	int samples_read = 0;
	
	if(wave->header.AudioFormat ==	UNCOMPRESSED_PCM)
	{
	
		if(wave->header.bitsPerSample == ip_bitspersample)
		{
			samples_read = fread(ip_buffer, ip_bitspersample >> 3, Samples2Read, wave->pfile);
		}
		else
		{
			unsigned int i;
			char *bptr = (char*)ip_buffer;
			short *sptr = (short*)ip_buffer;
			int  *lptr = (int*)ip_buffer;
			
			for(i = 0; i < Samples2Read; i++)
			{
				int sample = 0, ret_val;
				ret_val = fread(&sample, wave->header.bitsPerSample >> 3, 1, wave->pfile);
				if(ret_val != (1))
				{
					fprintf(stderr, "\n Unable to read PCM data in wave input read module\n");
					return -1;
				}
				else
				{
					samples_read += ret_val;
				}				
		
				if(wave->header.bitsPerSample >= ip_bitspersample)
				{
					sample >>= (wave->header.bitsPerSample - ip_bitspersample);	
				}
				else
				{
					sample <<= (ip_bitspersample - wave->header.bitsPerSample);	
				}
				
				if (ip_bitspersample == 32)
				{
					*lptr++ = sample;
				}
				else if (ip_bitspersample == 16)
				{
					*sptr++ = (short) sample;
				}
				else if (ip_bitspersample == 8)
				{
					*bptr++ = (char) sample;
				}
				
			}
		}
	}
	else
	{
		fprintf(stderr, "\n Unsupported PCM format in wave input read module\n");
	}
	return (samples_read);
}

void WAV_InputClose(HANDLE_WAVE *pWave)
{
  HANDLE_WAVE wave = *pWave;

  if (wave != NULL) 
  {
    if (wave->pfile != NULL) 
    {
       fclose(wave->pfile);
       wave->pfile = NULL;
    }
    if (wave) 
    {
       free(wave);
    }
  }
  *pWave = NULL;
}

#endif // ENABLE_WAVE_INP_SUPPORT

#ifdef ENABLE_WAVE_OUT_SUPPORT
int WAV_OutputOpen(HANDLE_WAVE *pWave, const char *outputFilename, int sampleRate, int numChannels, int bitsPerSample)
{
    short s;
    int i;
	FILE *fp_out = NULL;

	HANDLE_WAVE wave = (HANDLE_WAVE)calloc(1, sizeof(WAVE));
	if(wave == NULL)
	{
		fprintf(stderr, "\n WAVE sturcture Memory allocation failed in wave output open module\n");
WAVE_OutputOpen_Fail:	
		if(wave->pfile)
		{
			fclose(wave->pfile);
		}
		if(wave)
		{
			free(wave);
			wave = NULL;
		}
		pWave = NULL;
		return -1;
	}

	wave->pfile = fopen(outputFilename, "wb");
	if(wave->pfile == NULL)
	{
		fprintf(stderr, "\n Wave output file open failed in wave output open module : %s\n", outputFilename);
		goto WAVE_OutputOpen_Fail;
	}	

	fp_out =  wave->pfile;

    fwrite("RIFF",4,1,fp_out);
    i = 0;
    fwrite(&i,4,1,fp_out);  /* Current length without wave header */

    fwrite("WAVEfmt ", 1, 8, fp_out);  /* 2 labels */
    i = 16;
    fwrite(&i,4,1,fp_out);

    s = 1;
    fwrite(&s,2,1,fp_out);

    fwrite(&numChannels,2,1,fp_out);   /* Number of channels */
    fwrite(&sampleRate,4,1,fp_out);     /* Samples per second */

    i = sampleRate * numChannels * (bitsPerSample >> 3);
    fwrite(&i,4,1,fp_out);                      /* bytes per sec */

    s = (bitsPerSample >> 3) * numChannels;
    fwrite(&s,2,1,fp_out);                      /* block alignment */

    s = bitsPerSample;
    fwrite(&s,2,1,fp_out);                      /* bits per sample */

    fwrite("data",4,1,fp_out);
    i = 0;
    fwrite(&i,4,1,fp_out);                      /* PCM bytes */

	wave->header.bitsPerSample = bitsPerSample;
	wave->header.numChannels = numChannels;
	wave->header.sampleRate = sampleRate;
	wave->header.AudioFormat = UNCOMPRESSED_PCM;

	wave->header.Subchunk2Size = 44;

	*pWave = wave;
	return 0;
}

int WAV_OutputWrite(HANDLE_WAVE wave, void *op_buffer, unsigned int Samples2Write, int op_bitspersample, int nSigBits)
{
	int samples_written = 0;
	(void)nSigBits;
	
	if(wave->header.AudioFormat ==	UNCOMPRESSED_PCM)
	{
	
		if(wave->header.bitsPerSample == op_bitspersample) 
		{
			samples_written = fwrite(op_buffer, wave->header.bitsPerSample >> 3, Samples2Write, wave->pfile);
		}
		else
		{
			unsigned int i;
			char *bptr = (char*)op_buffer;
			short *sptr = (short*)op_buffer;
			int  *lptr = (int*)op_buffer;
			
			for(i = 0; i < Samples2Write; i++)
			{
				int sample = 0, bits_adj = op_bitspersample - wave->header.bitsPerSample, ret_val;

				if (op_bitspersample == 32)
				{
					sample = *lptr++;
				}
				else if (op_bitspersample == 16)
				{
					sample = *sptr++;
				}
				else if (op_bitspersample == 8)
				{
					sample = *bptr++;
				}

				if(bits_adj >= 0)
				{
					sample >>= (bits_adj);	
				}
				else
				{
					/* sign extension */
					sample = (sample << (32 - op_bitspersample)) >> (32 - op_bitspersample);	
				}

				ret_val = fwrite(&sample, wave->header.bitsPerSample >> 3, 1, wave->pfile);
				if(ret_val != (1))
				{
					fprintf(stderr, "\n Unable to write PCM data in wave output write module\n");
					return -1;
				}
				else
				{
					samples_written += ret_val;
				}
			}
		}
		samples_written *= (wave->header.bitsPerSample >> 3);
	}
	else
	{
		fprintf(stderr, "\n Unsupported PCM format in wave output write module\n");
	}
	wave->header.Subchunk2Size += samples_written;
	return 0;
}

void WAV_OutputClose(HANDLE_WAVE *pWave)
{
  HANDLE_WAVE wave = *pWave;

  if (wave != NULL) 
  {
	FILE *fp_out = wave->pfile;
	int val;

    fseek(fp_out,4,SEEK_SET);
    val = wave->header.Subchunk2Size - 8;
    fwrite(&val,4,1,fp_out);

    fseek(fp_out,40,SEEK_SET);
    val= wave->header.Subchunk2Size - 44;
    fwrite(&val,4,1,fp_out);

    if (wave->pfile != NULL) 
    {
       fclose(wave->pfile);
       wave->pfile = NULL;
    }
    if (wave) 
    {
       free(wave);
    }
  }
  *pWave = NULL;
}
#endif // ENABLE_WAVE_OUT_SUPPORT
