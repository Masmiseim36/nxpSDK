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


#include "xa_drm_dec_sample_testbench.h"

#if !defined(PROFILE)
#define PROFILE 1
#endif /* PROFILE */

#if !defined(DISPLAY_MESSAGE)
#define DISPLAY_MESSAGE 1
#endif /* DISPLAY_MESSAGE */

#if !defined(SHOW_SIZES)
#define SHOW_SIZES 1
#endif /* SHOW_SIZES */

//#define OUTPUT_BUF_PING_PONG_TEST

#if PROFILE
#define PROF_ALLOCATE
#endif /* PROFILE */
#include "xa_profiler.h"

/* Scratch test */
SCRATCH_TRASH_TEST_ALLOCATE

/* Output ping pong test */
OUTPUT_BUF_PING_PONG_TEST_ALLOCATE

/* Input ping pong test */
INPUT_BUF_PING_PONG_TEST_ALLOCATE


char pb_input_file_path[MAX_CMD_LINE_LENGTH];
char pb_output_file_path[MAX_CMD_LINE_LENGTH];

int startFrame =  0;
int stopFrame  = -1;

 XA_ERRORCODE codec_init_process(xa_testapp_drm_dec_state *pobj);
 XA_ERRORCODE codec_exec_process(xa_testapp_drm_dec_state *pobj);

XA_ERRORCODE xa_drm_dec_main_process(int argc, char *argv[])
{
    XA_ERRORCODE err_code = XA_NO_ERROR;

    char outFileName[MAX_CMD_LINE_LENGTH];
    xa_testapp_drm_dec_state drm_dec;
    xa_testapp_drm_dec_state *pobj = &drm_dec;
    memset(pobj, 0, sizeof(xa_testapp_drm_dec_state));

    pobj->p_xa_process_api_obj = NULL;
    pobj->p_xa_process_api = xa_drm_dec;

    pobj->p_proc_err_info = &xa_drm_dec_error_info;
    pobj->p_test_err_info = &xa_testbench_error_info ;

    xa_drm_dec_error_handler_init();

#if DISPLAY_MESSAGE
    {
        /* Library Info and Identification strings */
        WORD8 pb_process_name[XA_SCREEN_WIDTH]    = "";
        WORD8 pb_lib_version[XA_SCREEN_WIDTH]     = "";
        API_CALL( XA_API_CMD_GET_LIB_ID_STRINGS, XA_CMD_TYPE_LIB_NAME, pb_process_name, "Lib-name ");

        /* Get the library version string */
        API_CALL( XA_API_CMD_GET_LIB_ID_STRINGS, XA_CMD_TYPE_LIB_VERSION, pb_lib_version, "Lib-version ");

        /* Display the Tensilica identification message */
        fprintf(stderr, "\n\t\t\t%s version %s", pb_process_name, pb_lib_version);
        fprintf(stderr, "\n\t\t\tCadence Design Systems, Inc. http://www.cadence.com\n\n");
    }

#endif
    if(argc == 0)
    {
        printf("\tParameter file \"%s\" not found.\n", PARAMFILE);
        printf("\tCommand line usage:\n");
        printf("\t-if <infile> -of <outfile> <options>\n");
        printf("\tUse -h for more detailes about <options>.\n");
        return XA_NO_ERROR;
    }

    /* ******************************************************************/
    /* Initialize API structure and set config params to default        */
    /* ******************************************************************/
    {
        unsigned int ui_api_size=0;
        /* Get the API size */
        API_CALL( XA_API_CMD_GET_API_SIZE, 0, &ui_api_size, "Api Size ");


#if SHOW_SIZES
        printf("\nAPI structure size: %u bytes\n", ui_api_size);
#endif

        /* Set API object with the memory allocated */
        pobj->p_xa_process_api_obj = (xa_codec_handle_t)MEM_malloc(ui_api_size, 4);

        API_CALL( XA_API_CMD_INIT, XA_CMD_TYPE_INIT_API_PRE_CONFIG_PARAMS, NULL, "Pre Configuration Init ");
    }


    /* ******************************************************************/
    /*  Get Default Configuration from Library                          */
    /*  Update the configurations based on the user input               */
    /*  Modify Codec configurations                                     */
    /* ******************************************************************/
    {
        pobj->cfg.outputMode.val = 0;
        pobj->cfg.frameCorrupt.val = 0;
        pobj->cfg.startFrame.val        = 0;
        pobj->cfg.stopFrame.val        = 32768;
        RETURN_ON_FATAL_ERROR(get_libconfig(pobj));

        err_code = cmdlineparse(&pobj->cfg, argc, argv);
        switch(err_code)
        {
        case XA_NO_ERROR:
            break;
        case XA_TESTBENCH_FATAL_HELP_REQUESTED:
        {
            showusage();
            return XA_TESTBENCH_FATAL_HELP_REQUESTED;
        }
        default:
        {
            TESTBENCH_ERROR(err_code, "parsing command line argument");
            break;
        }
        }

        err_code = INP_Open(&pobj->inp,
                         pobj->cfg.ifile
#ifdef RAW_AAC_SUPPORTED
                         ,pobj->cfg.bsfmt.val
#endif
                            );
        TESTBENCH_ERROR(err_code, "decoder input raw file");

        /* ******************************************************************/
        /* Set config params from API                                       */
        /* ******************************************************************/
        /* Set parameters dervied by analyzing the input arguments */
        RETURN_ON_FATAL_ERROR(set_params(pobj));
        RETURN_ON_FATAL_ERROR(get_libconfig(pobj));

        // TODO: Prints if any
    }

    /* ******************************************************************/
    /* Initialize Memory info tables and post-config init               */
    /* ******************************************************************/
    {
        void * ptemp;
        unsigned int ui_proc_mem_tabs_size=0;

        API_CALL( XA_API_CMD_GET_MEMTABS_SIZE, 0, &ui_proc_mem_tabs_size, "Get mem-tab-size");
#if SHOW_SIZES
        printf("MEMTABS size: %u bytes\n", ui_proc_mem_tabs_size);
#endif
        ptemp = MEM_malloc(ui_proc_mem_tabs_size, 4);

        API_CALL( XA_API_CMD_SET_MEMTABS_PTR, 0, ptemp, "Set mem-tab-ptr ");

        API_CALL( XA_API_CMD_INIT, XA_CMD_TYPE_INIT_API_POST_CONFIG_PARAMS, NULL, "post-config ");

    }

    /*******************************************************************/
    /* Allocate Memory with info from library                                   */
    /*******************************************************************/
    {
        WORD32 n_mems=0, i;

        /* Get number of memory tables required */
        API_CALL( XA_API_CMD_GET_N_MEMTABS, 0, &n_mems, "Get-n-memtabs ");

        for(i = 0; i < (WORD32)n_mems; i++)
        {
            int ui_size=0, ui_alignment=1, ui_type=-1;
            pVOID pv_alloc_ptr;

            /* Get memory size */
            API_CALL( XA_API_CMD_GET_MEM_INFO_SIZE, i, &ui_size, "get-mem-info-size");

            /* Get memory alignment */
            API_CALL( XA_API_CMD_GET_MEM_INFO_ALIGNMENT, i, &ui_alignment, "get-mem-info-alignment");

            /* Get memory type */
            API_CALL( XA_API_CMD_GET_MEM_INFO_TYPE, i, &ui_type, "in get-mem-info-type");

            pv_alloc_ptr = MEM_malloc(ui_size, ui_alignment);

            /* Set the buffer pointer */
            API_CALL( XA_API_CMD_SET_MEM_PTR, i, pv_alloc_ptr, "in set mem-ptr");

            if(ui_type == XA_MEMTYPE_INPUT)
            {
                BUF_INIT(&pobj->inpbuf, pv_alloc_ptr, ui_size);
            }

            if(ui_type == XA_MEMTYPE_OUTPUT)
            {
                BUF_INIT(&pobj->outbuf, pv_alloc_ptr, ui_size);
            }
			/* Scratch test */
			SCRATCH_TRASH_TEST_ASSIGN

			/* Output ping pong test */
			OUTPUT_BUF_PING_PONG_TEST_ASSIGN

			/* Input ping pong test */
			INPUT_BUF_PING_PONG_TEST_ASSIGN

#if SHOW_SIZES
            switch (ui_type)
            {
            case XA_MEMTYPE_INPUT:
                printf("Input buffer size: %u bytes\n", ui_size);
                break;
            case XA_MEMTYPE_OUTPUT:
                printf("Output buffer size: %u bytes\n", ui_size);
                break;
            case XA_MEMTYPE_SCRATCH:
                printf("Scratch buffer size: %u bytes\n", ui_size);
                break;
            case XA_MEMTYPE_PERSIST:
                printf("Persistent buffer size: %u bytes\n", ui_size);
                break;
            default:
                assert(0);
            }
#endif /* SHOW_SIZES */

        }
    }

    strcpy(outFileName, pobj->cfg.ofile);

    pobj->frameCounter = 0;

    err_code = codec_init_process(pobj);

    err_code = get_params(pobj,
                          &pobj->cfg.out_num_chan.val,
                          &pobj->cfg.samp_freq.val,
                          &pobj->cfg.frameSize.val,
                          &pobj->cfg.pcm_width.val);

    if(err_code == XA_NO_ERROR)
    {
        if(WAV_OutputOpen(&pobj->out_wav,
                              outFileName,
                              pobj->cfg.samp_freq.val,
                              pobj->cfg.out_num_chan.val,
                              pobj->cfg.pcm_width.val))
        {
            err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
        }
        TESTBENCH_ERROR(err_code, "decoder output wave file");
        pobj->out_file_count++;

        PROFILER_OPEN((&gProfiler[0]), "drm_dec");

        do {
            pobj->inp.drm.config_change_detected = 0;//will be set during the exec_process
            pobj->inp.drm.b_input_buffer_overrun = 0;//will be set during the exec_process
        
            err_code = codec_exec_process(pobj);

            if(pobj->inp.drm.config_change_detected
                || pobj->inp.drm.b_input_buffer_overrun
            ){
               int out_num_chan, samp_freq, frameSize, pcm_width;
            
               // Handle output config change
               printf("Warning! Stream config parameters changed. Codec will be re-opened.\n");

               /* ******************************************************************/
               /* Set config params from API                                       */
               /* ******************************************************************/
               /* Set parameters derived by analyzing the input arguments */
               RETURN_ON_FATAL_ERROR(set_params(pobj));
               RETURN_ON_FATAL_ERROR(get_libconfig(pobj));
            
               err_code = codec_init_process(pobj);
               if(err_code != XA_NO_ERROR){
                    break;
               }

               err_code = get_params(pobj,
                         &out_num_chan,
                         &samp_freq,
                         &frameSize,
                         &pcm_width);
            
               if ( ((out_num_chan != pobj->cfg.out_num_chan.val) ||
                    (samp_freq != pobj->cfg.samp_freq.val))
               )
               {
                   // Close previously opened file
                   printf(" Closing file: %s and", pobj->cfg.ofile);
                   WAV_OutputClose(&pobj->out_wav);
                   
                   // open New file
                   strcpy(outFileName, pobj->cfg.ofile);
                   sprintf(&(outFileName[strlen(outFileName)-4]), "_%02d.wav",pobj->out_file_count);
                   printf(" Opening New file: %s\n", outFileName);
            
                  if(WAV_OutputOpen(&pobj->out_wav,
                                        outFileName,
                                        samp_freq,
                                        out_num_chan,
                                        pcm_width))
                  {
                      err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
                  }
                  TESTBENCH_ERROR(err_code, "decoder output wave file");
                  pobj->out_file_count++;
               }

               //update the configuration maintained in testbench
               err_code = get_params(pobj,
                                     &pobj->cfg.out_num_chan.val,
                                     &pobj->cfg.samp_freq.val,
                                     &pobj->cfg.frameSize.val,
                                     &pobj->cfg.pcm_width.val);

            }//if(pobj->inp.drm.config_change_detected)
        } while(pobj->inp.drm.config_change_detected 
                 || pobj->inp.drm.b_input_buffer_overrun);

        PROFILER_CLOSE((&gProfiler[0]));

        WAV_OutputClose(&pobj->out_wav);

    }

    INP_Close(&pobj->inp);

    return XA_NO_ERROR;

}

XA_ERRORCODE codec_init_process(xa_testapp_drm_dec_state *pobj)
{
    XA_ERRORCODE err_code = XA_NO_ERROR;
	XA_ERRORCODE init_err = XA_NO_ERROR;

    pobj->ui_init_done=0;
#ifdef INPUT_READ_ON_REQUEST
    pobj->inpbuf.needmoredata = 1;
#endif //INPUT_READ_ON_REQUEST
    do
    {

        {
			err_code = INP_Read(&pobj->inp,&pobj->inpbuf );
            TESTBENCH_ERROR(err_code, "decoder input file");


#ifdef RAW_AAC_SUPPORTED
			if(pobj->cfg.bsfmt.val == XA_DRM_DEC_INPUT_FORMAT_DRM)
#endif
            {
				SET_CONFIG( XA_DRM_DEC_CONFIG_PARAM_DRM_AUDIO_CONFIG, &pobj->inp.drm.drmAudioConf, "Set Current drmAudioConf");//RJ??
            }
#ifdef RAW_AAC_SUPPORTED
            if (pobj->cfg.bsfmt.val == XA_DRM_DEC_INPUT_FORMAT_RAW_AAC)
            {

                SET_CONFIG( XA_DRM_DEC_AAC_CONFIG_PARAM_ASC_LENGTH, &pobj->inp.raw.asc_length, "");
                SET_CONFIG( XA_DRM_DEC_AAC_CONFIG_PARAM_ASC, pobj->inp.raw.asc, "");
            }
#endif
            if(pobj->inpbuf.inp_over == 1)
            {
                /* Tell that the input is over in this buffer */
                API_CALL( XA_API_CMD_INPUT_OVER, 0, NULL, "Input Over");
            }
            /* Set number of bytes to be processed */
            API_CALL( XA_API_CMD_SET_INPUT_BYTES, 0, &pobj->inpbuf.data_length, "Setting input-Bytes ");
        }

        /* Initialize the process */
        API_CALL( XA_API_CMD_INIT, XA_CMD_TYPE_INIT_PROCESS, NULL, "Init Processing");
        init_err = err_code;

        /* Checking for end of initialization */
        API_CALL( XA_API_CMD_INIT, XA_CMD_TYPE_INIT_DONE_QUERY, &pobj->ui_init_done, "Init done query");
         /* How much buffer is used in input buffers */
        API_CALL( XA_API_CMD_GET_CURIDX_INPUT_BUF, 0, &pobj->inpbuf.bytes_consumed, "Input Buffer Consumed Check ");
        /* It is assumed that the entire input frame is consumded; you may add check if required */
#ifdef INPUT_READ_ON_REQUEST
        if(init_err == XA_DRM_DEC_EXECUTE_NONFATAL_INSUFFICIENT_FRAME_DATA)
        {
            pobj->inpbuf.needmoredata = 1;
        }
#endif //INPUT_READ_ON_REQUEST
    } while(!pobj->ui_init_done);

    /* Break if init is done */
    if (!pobj->ui_init_done)
    {
        TESTBENCH_ERROR(XA_TESTBENCH_FATAL_INIT_FAILED, "Init Processing");
    }

    /* Init end of process condition end */

    err_code = 0;


    /* ******************************************************************/
    /* Get config params from API                                       */
    /* ******************************************************************/
    {
    int samp_freq, out_num_chan, pcm_width, frameSize;
    /* Read parameters dervied by analyzing the input bit-streams */
    err_code = get_params(pobj,
                          &out_num_chan,
                          &samp_freq,
                          &frameSize,
                          &pcm_width);

    fprintf (stderr, "Output Sample rate		: %d Hz\n", samp_freq);
    fprintf (stderr, "Output Channels			: %d\n", out_num_chan);
    fprintf (stderr, "Output PCM sample width	: %d bits\n", pcm_width);
    fprintf (stderr, "Output PCM frame size		: %d samples\n", frameSize);

    if ( err_code )
    {
        /* Non-fatal error */
        fprintf(stderr, "Invalid Parameters detected after initialization\n");
    }
    }

    return XA_NO_ERROR;
}

XA_ERRORCODE codec_exec_process(xa_testapp_drm_dec_state *pobj)
{
    XA_ERRORCODE err_code = XA_NO_ERROR;
	XA_ERRORCODE exec_err = XA_NO_ERROR;
	XA_DRM_CONCEAL_STATE concealState = XA_DRM_CONCEAL_STATE_NORMAL;
    pobj->ui_exec_done = 0;

    /* Third part for executing each process */
    do /* until we run out of data from input file */
    {
        {
            pobj->inp.drm.b_exec_error_insufficient_input=0;
            if(exec_err == XA_DRM_DEC_EXECUTE_NONFATAL_INSUFFICIENT_FRAME_DATA){
                /* for input buffer reader to detect buffer overrun instances */
                pobj->inp.drm.b_exec_error_insufficient_input = 1;
            }
            err_code = INP_Read(&pobj->inp,&pobj->inpbuf );
            TESTBENCH_ERROR(err_code, "decoder input file");

            if(pobj->inp.drm.b_input_buffer_overrun){
                /* reopen codec if input buffer overrun is detected */
                return exec_err;
            }

            if(exec_err == XA_DRM_DEC_EXECUTE_NONFATAL_STREAM_CHANGE_DETECTED){
                pobj->inp.drm.config_change_detected = 1;
                return exec_err;
            }

#ifdef RAW_AAC_SUPPORTED
			if(pobj->cfg.bsfmt.val == XA_DRM_DEC_INPUT_FORMAT_DRM)
#endif
            {
				SET_CONFIG( XA_DRM_DEC_CONFIG_PARAM_DRM_AUDIO_CONFIG, &pobj->inp.drm.drmAudioConf, "Set Current drmAudioConf");//RJ??
            }

            if(pobj->inpbuf.inp_over == 1)
            {
                /* Tell that the input is over in this buffer */
                API_CALL( XA_API_CMD_INPUT_OVER, 0, NULL, "Input Over");
            }
            /* Set number of bytes to be processed */
            API_CALL( XA_API_CMD_SET_INPUT_BYTES, 0, &pobj->inpbuf.data_length, "Setting input-Bytes ");
        }
        PROFILER_START((&gProfiler[0]))
        /* Execute process */
        API_CALL( XA_API_CMD_EXECUTE, XA_CMD_TYPE_DO_EXECUTE, NULL, "Decoding / Encoding ");
        exec_err = err_code;

        PROFILER_STOP((&gProfiler[0]))
        
        GET_CONFIG( XA_DRM_DEC_CONFIG_PARAM_CONCEAL_STATE, &concealState, "Get current concealment state");
        
        if( concealState != XA_DRM_CONCEAL_STATE_NORMAL )
        {
            switch(concealState)
            {
                case XA_DRM_CONCEAL_STATE_NORMAL:
                    break;
                case XA_DRM_CONCEAL_STATE_FADE_IN:
                    printf("Fade-in concealment applied\n");
                    break;
                case XA_DRM_CONCEAL_STATE_MUTE:
                    printf("Mute concealment applied\n");
                    break;
                case XA_DRM_CONCEAL_STATE_FADE_OUT:
                    printf("Fade-out concealment applied\n");
                    break;
                default:
                    break;
            }
            
        }

        /* Checking for end of processing */
        API_CALL( XA_API_CMD_EXECUTE, XA_CMD_TYPE_DONE_QUERY, &pobj->ui_exec_done, "End-exec Check ");

        /* Get the output bytes */
        API_CALL( XA_API_CMD_GET_OUTPUT_BYTES, 0, &pobj->outbuf.data_length, "Get Output Bytes ");

        /* How much buffer is used in input buffers */
        API_CALL( XA_API_CMD_GET_CURIDX_INPUT_BUF, 0, &pobj->inpbuf.bytes_consumed, "Input Buffer Consumed Check ");
        /* It is assumed that the entire input frame is consumded; you may add check if required */


        GET_CONFIG( XA_CONFIG_PARAM_GEN_INPUT_STREAM_POS, &gProfiler[0].streampos, "Get Generated PCM Position");

#ifdef INPUT_READ_ON_REQUEST
        if(exec_err == XA_DRM_DEC_EXECUTE_NONFATAL_INSUFFICIENT_FRAME_DATA)
        {
            pobj->inpbuf.needmoredata = 1;
        }
#endif //INPUT_READ_ON_REQUEST
        if ( pobj->outbuf.data_length ) /* Frame decoded */
        {
			{
			    //if( exec_err == XA_DRM_DEC_EXECUTE_NONFATAL_STREAM_CHANGE_DETECTED) //commented out to match the reference FHG dec
                {
                    int out_num_chan;
                    int samp_freq;
                    int frameSize;
                    int pcm_width;
                    err_code = get_params(pobj,
                              &out_num_chan,
                              &samp_freq,
                              &frameSize,
                              &pcm_width);
                    if(
                       (out_num_chan != pobj->cfg.out_num_chan.val) ||
                       (samp_freq != pobj->cfg.samp_freq.val))
                    {
                        // Handle output config change
                        printf("output parameters changed : ");
                        // Close previously opened file
                        WAV_OutputClose(&pobj->out_wav);

                        // update config
                        pobj->cfg.out_num_chan.val = out_num_chan;
                        pobj->cfg.samp_freq.val = samp_freq;
                        printf(" Closing file: %s and", pobj->cfg.ofile);
                        // open New file
                        char temp_fname[MAX_CMD_LINE_LENGTH];
                        strcpy(temp_fname, pobj->cfg.ofile);


                        sprintf(&(temp_fname[strlen(temp_fname)-4]),
                                "_%02d.wav",pobj->out_file_count);
                        pobj->out_file_count++;
                        printf(" Opening New file: %s\n", temp_fname);
                        if(WAV_OutputOpen(&pobj->out_wav,
                                  temp_fname,
                                  pobj->cfg.samp_freq.val,
                                  pobj->cfg.out_num_chan.val,
                                  pobj->cfg.pcm_width.val))
                                  {
                                  err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
                                }
                            TESTBENCH_ERROR(err_code, "Open new wave file");

                    }

			    }
			    if (pobj->frameCounter >= pobj->cfg.startFrame.val)
				{
            	    /* write audio channels to pcm file */
					int sample_bits, num_samples;
						sample_bits = (pobj->cfg.pcm_width.val == 16) ? 16 :32;
						num_samples = (pobj->outbuf.data_length/(sample_bits/8))/pobj->cfg.out_num_chan.val;
						if(WAV_OutputWrite(pobj->out_wav,
								   pobj->outbuf.base_ptr,
								   num_samples * pobj->cfg.out_num_chan.val,
								   sample_bits,
								   sample_bits))
						{
							TESTBENCH_ERROR(XA_TESTBENCH_MFMAN_FATAL_FILE_WRITE_FAILED, "decoder output wave file");
						}
				}

			}
            pobj->frameCounter++;
            PROFILER_UPDATE( (&gProfiler[0]), pobj->cfg.frameSize.val, pobj->cfg.samp_freq.val )
            if ((pobj->cfg.stopFrame.val != -1) && (pobj->frameCounter >= pobj->cfg.stopFrame.val))
            {
                break;
            }

        }

        /* Check if done */
        if (pobj->ui_exec_done) break ;

    }
    while (1);   /* Loop to feed new data chunks from the input file */


    return 0;
}



// Set cache attribute to Write Back No Allocate when the last argument is -wbna

void set_wbna(int *argc, char *argv[])
{
    if ( *argc > 1 && !strcmp(argv[*argc-1], "-wbna") )
    {
#ifdef __XCC__
        extern char _memmap_cacheattr_wbna_trapnull;

        xthal_set_cacheattr((unsigned)&_memmap_cacheattr_wbna_trapnull);
#endif
        (*argc)--;
    }
}


/****************************************************************************
 *   Main function for test-wrapper                                         *
 ****************************************************************************/

int
main (int   argc, char *argv[])
{
    FILE *param_file_id;

    char curr_cmd[MAX_CMD_LINE_LENGTH];
    int fargc, curpos;
    int processcmd = 0;

    char fargv[XA_MAX_ARGS][MAX_CMD_LINE_LENGTH];
    char *pargv[XA_MAX_ARGS];

    memset(pb_input_file_path,  0, MAX_CMD_LINE_LENGTH);
    memset(pb_output_file_path, 0, MAX_CMD_LINE_LENGTH);

    // NOTE: set_wbna() should be called before any other dynamic
    // adjustment of the region attributes for cache.
    set_wbna(&argc, argv);

    xa_testbench_error_handler_init();

    if (argc == 1) 
    {
        if ((param_file_id = fopen(PARAMFILE, "r")) == NULL )
        {
            xa_drm_dec_main_process(0, NULL);

            exit(0);
        }

        /* Process one line at a time */
        while(fgets(curr_cmd, MAX_CMD_LINE_LENGTH, param_file_id))
        {
            curpos = 0;
            fargc = 0;
            /* if it is not a param_file command and if */
            /* CLP processing is not enabled */
            if(curr_cmd[0] != '@' && !processcmd)     /* skip it */
            {
                continue;
            }

            while(sscanf(curr_cmd + curpos, "%s", fargv[fargc]) != EOF)
            {
                if(fargv[0][0]=='/' && fargv[0][1]=='/')
                    break;
                if(strcmp(fargv[0], "@echo") == 0)
                    break;
                if(strcmp(fargv[fargc], "@New_line") == 0)
                {
                    char * str = fgets(curr_cmd + curpos, MAX_CMD_LINE_LENGTH,
                                       param_file_id);
                    (void)str;
                    continue;
                }
                curpos += strlen(fargv[fargc]);
                while(*(curr_cmd + curpos)==' ' || *(curr_cmd + curpos)=='\t')
                    curpos++;
                fargc++;
            }

            if(fargc < 1) /* for blank lines etc. */
                continue;

            if(strcmp(fargv[0], "@Output_path") == 0)
            {
                if(fargc > 1) strcpy(pb_output_file_path, fargv[1]);
                else strcpy(pb_output_file_path, "");
                continue;
            }

            if(strcmp(fargv[0], "@Input_path") == 0)
            {
                if(fargc > 1) strcpy(pb_input_file_path, fargv[1]);
                else strcpy(pb_input_file_path, "");
                continue;
            }

            if(strcmp(fargv[0], "@Start") == 0)
            {
                processcmd = 1;
                continue;
            }

            if(strcmp(fargv[0], "@Stop") == 0)
            {
                processcmd = 0;
                continue;
            }

            /* otherwise if this a normal command and its enabled for execution */
            if(processcmd)
            {
                int i;
                for(i=0; i<fargc; i++)
                {
                    pargv[i] = fargv[i];
                }

                MEM_init();
                xa_drm_dec_main_process(fargc, pargv);

                MEM_freeall();

            }
        }
    }
    else
    {
        MEM_init();

        xa_drm_dec_main_process(argc - 1, &argv[1]);

        MEM_freeall();
    }

    return XA_NO_ERROR;
} /* end main decode function */
