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


#if 1 //DRM_SUPPORTED
#include "xa_drm_dec_drm_utils.h"

static int ntos(int x)
{
    int v = 0;

    v |= (x & 0x000000FF) << 24;
    v |= (x & 0x0000FF00) <<  8;
    v |= (x & 0x00FF0000) >>  8;
    v |= (x & 0xFF000000) >> 24;

    return v;
}


 XA_ERRORCODE DRM_InputOpen(HANDLE_DRM *hDataSrc, char *filename)
{

    if(filename[0] == '\0')
    {
        return XA_TESTBENCH_FATAL_INPUT_NOT_SPECIFIED;
    }
    /* Open file */
    else
    {
        hDataSrc->fp = fopen(filename, "rb") ;
        if(hDataSrc->fp== NULL)
        {
            return XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
        }

    }
    return XA_NO_ERROR;
}



 XA_ERRORCODE DRM_InputRead(HANDLE_DRM *drm,buffer_t *inpbuf)
{
    XA_ERRORCODE err = XA_NO_ERROR;

#ifdef ENABLE_INTERFACE_HEADER_SUPPORT
    XA_DRM_DEC_DRM_AUDIO_CONFIG *p_drmconfig = &drm->drmAudioConf;
#endif
    if(inpbuf->data_length >= inpbuf->bytes_consumed)
    {

        /* Shift consumed bytes out of the input buffer */
        xa_shift_input_buffer(inpbuf->base_ptr, inpbuf->data_length, inpbuf->bytes_consumed);
        inpbuf->data_length -= inpbuf->bytes_consumed;
        inpbuf->bytes_consumed = 0;

    }
    if(inpbuf->inp_over)
    {
        return XA_NO_ERROR;
    }
#ifdef INPUT_READ_ON_REQUEST
    if( (inpbuf->needmoredata == 0)  && (inpbuf->data_length > 0))
    {
        return 0;
    }
#endif // INPUT_READ_ON_REQUEST
    int hdrlength = 0;
#ifdef ENABLE_INTERFACE_HEADER_SUPPORT
    err = inputReadInterfaceHeader(drm, p_drmconfig, &hdrlength);
    if(err)
    {
        inpbuf->inp_over = 1;
        if (ferror(drm->fp))
        {
            _XA_HANDLE_ERROR(&xa_testbench_error_info, "File Read Error",
                             XA_TESTBENCH_MFMAN_FATAL_FILE_READ_FAILED);
        }
        return XA_TESTBENCH_MFMAN_NONFATAL_PARTIAL_FRAME;
    }
#endif
    if( (UWORD32)BUF_EMPTY_SPACE(inpbuf) < p_drmconfig->lengthOfAudioSuperFrame)
    {
        if(drm->b_exec_error_insufficient_input){ /* Act only on insufficient data error from library */
            drm->b_input_buffer_overrun=1;
            /* Input buffer overrun condition.
               Prepare DUT to flush the remaining input and reopen the codec */
            inpbuf->data_length=0;
        }
        // You don't have space to read one frame !
        // TODO: This may now work correctly as we have to seek back the header for next call.
        fseek(drm->fp, -hdrlength, SEEK_CUR);
        return XA_NO_ERROR;
    }
    
    {
        int bytes2read = p_drmconfig->lengthOfAudioSuperFrame;;
        
        int fread_bytes = fread(BUF_EMPTY_STARTPTR(inpbuf), 1, bytes2read, drm->fp) ;
        
        if (fread_bytes != bytes2read && ! inpbuf->inp_over)
        {

            if (feof(drm->fp))
            {
                inpbuf->inp_over = 1;
            }
            else
            {
                _XA_HANDLE_ERROR(&xa_testbench_error_info, "File Read Error",
                                 XA_TESTBENCH_MFMAN_FATAL_FILE_READ_FAILED);
            }
        }
        inpbuf->data_length   += fread_bytes ;
    }


    return err;
}


#ifdef ENABLE_INTERFACE_HEADER_SUPPORT

 XA_ERRORCODE inputReadInterfaceHeader(
    HANDLE_DRM *hDataSrc,
    XA_DRM_DEC_DRM_AUDIO_CONFIG *p_drmconfig,
    int *p_headerlength
)
{

    unsigned long     interface_header[INTERFACE_HEADER_SIZE];
    XA_DRM_DEC_DRM_AUDIO_CONFIG old_config;
    memcpy(&old_config,p_drmconfig,sizeof(XA_DRM_DEC_DRM_AUDIO_CONFIG));
    unsigned int interfaceFrameLen = 0;

    int readBytes = fread(interface_header, sizeof(unsigned char), INTERFACE_HEADER_SIZE, hDataSrc->fp);
    *p_headerlength = readBytes;
    if(readBytes != INTERFACE_HEADER_SIZE)
    {
        return XA_TESTBENCH_MFMAN_FATAL_FILE_READ_FAILED;
    }
    /* Fill Drm decoder audio info */
    memset(p_drmconfig, 0, sizeof(XA_DRM_DEC_DRM_AUDIO_CONFIG));
    interfaceFrameLen                     =   ntos(interface_header[0]);
    p_drmconfig->audioSamplingRate       =   ntos(interface_header[2]);
    p_drmconfig->coderField              =   ntos(interface_header[3]) & 0x1F;
    p_drmconfig->audioCoding             =  (ntos(interface_header[4]) % 10) & 0x03;
    p_drmconfig->audioMode               =   ntos(interface_header[5]) & 0x03;
    p_drmconfig->sbrFlag                 =   ntos(interface_header[6]) & 0x01;
    p_drmconfig->lengthOfAudioSuperFrame =   ntos(interface_header[7]);
    p_drmconfig->lengthHigherProtected   =   ntos(interface_header[8]);
    p_drmconfig->drmPlus                 = ((ntos(interface_header[4]) % 10) & 0x04) >> 2;
    p_drmconfig->configLength            = (interfaceFrameLen  - (p_drmconfig->lengthOfAudioSuperFrame)- INTERFACE_HEADER_SIZE);

    if(p_drmconfig->configLength > 0)
    {
        if(p_drmconfig->configLength > INTERFACE_MAX_CONFIG_SIZE)
        {
            memcpy(p_drmconfig, &old_config, sizeof(XA_DRM_DEC_DRM_AUDIO_CONFIG));
            return XA_TESTBENCH_MFMAN_FATAL_FILE_READ_FAILED;
        }

        readBytes = fread( p_drmconfig->config, 
                          sizeof(unsigned char), 
                          p_drmconfig->configLength,
                          hDataSrc->fp );

        *p_headerlength += readBytes;
        if(readBytes != p_drmconfig->configLength)
        {
            return XA_TESTBENCH_MFMAN_FATAL_FILE_READ_FAILED;
        }
    }
    return XA_NO_ERROR;
}
#endif  /* ENABLE_INTERFACE_HEADER_SUPPORT */


 void DRM_inputClose(HANDLE_DRM *hDataSrc)
{
    if (hDataSrc != NULL)
    {
        if ((hDataSrc)->fp != NULL)
        {
            fclose((hDataSrc)->fp);
            (hDataSrc)->fp = NULL;
        }
    }
}

#endif
