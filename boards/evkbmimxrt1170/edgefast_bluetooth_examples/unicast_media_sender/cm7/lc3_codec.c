/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "lc3_codec.h"

#include <string.h>

#if defined(LC3_HIFI4) && (LC3_HIFI4 != 0)
extern int BOARD_DSP_IPC_Send(uint8_t *data, int size);
extern int BOARD_DSP_IPC_Recv(uint8_t *data, int size);

static uint8_t lc3_msg[2*1024];
static uint8_t lc3_msg_res[2*1024];

#define LC3_MSG_HEADER_SIZE 3

#define LC3_MSG_SUCCESS 0x00
#define LC3_MSG_ERROR 0xff

struct lc3_msg_header {
	uint8_t type;
	uint16_t len;
	uint8_t data[0];
} __attribute__((packed));

struct lc3_msg_res_headr {
	uint8_t type;
	uint16_t len;
	uint8_t res;
	uint8_t data[0];
} __attribute__((packed));

#else
static int input_precessing(lc3_encoder_t *encoder, void *input)
{
    int samples_num = LC3_SAMPLES_PER_FRAME(encoder);

    if(8 == encoder->sample_bits)
    {
        int8_t *sample = (int8_t *)input;
        for(int i = 0; i < samples_num; i++)
        {
            encoder->buf_in[i] = (int32_t)sample[i];
        }
    }
    else if(16 == encoder->sample_bits)
    {
        int16_t *sample = (int16_t *)input;
        for(int i = 0; i < samples_num; i++)
        {
            encoder->buf_in[i] = (int32_t)sample[i];
        }
    }
    else if(24 == encoder->sample_bits)
    {
        uint8_t *data = (uint8_t *)input;
        int32_t sample;
        for(int i = 0; i < samples_num; i++)
        {
            sample = data[3 * i + 0] << 8;
            sample |= data[3 * i + 1] << 16;
            sample |= data[3 * i + 2] << 24;
            encoder->buf_in[i] = sample / 256;
        }
    }
    else if(32 == encoder->sample_bits)
    {
        (void)memcpy(encoder->buf_in, input, samples_num * sizeof(int32_t));
    }
    else
    {
        return LC3_CODEC_ERR;
    }

    return 0;
}

static int output_precessing(lc3_decoder_t *decoder, void *output)
{
    int samples_num = LC3_SAMPLES_PER_FRAME(decoder);

    if(8 == decoder->sample_bits)
    {
        int8_t *sample = (int8_t *)output;
        for(int i = 0; i < samples_num; i++)
        {
            sample[i] = (int8_t)decoder->buf_out[i];
        }
    }
    else if(16 == decoder->sample_bits)
    {
        int16_t *sample = (int16_t *)output;
        for(int i = 0; i < samples_num; i++)
        {
            sample[i] = (int16_t)decoder->buf_out[i];
        }
    }
    else if(24 == decoder->sample_bits)
    {
        uint8_t *data = (uint8_t *)output;
        int32_t sample;
        for(int i = 0; i < samples_num; i++)
        {
            sample = decoder->buf_out[i] * 256;
            data[3 * i + 0] = (uint8_t)((sample & 0x0000ff00) >> 8);
            data[3 * i + 1] = (uint8_t)((sample & 0x00ff0000) >> 16);
            data[3 * i + 2] = (uint8_t)((sample & 0xff000000) >> 24);
        }
    }
    else if(32 == decoder->sample_bits)
    {
        (void)memcpy(output, decoder->buf_out, samples_num * sizeof(int32_t));
    }
    else
    {
        return LC3_CODEC_ERR;
    }

    return 0;
}
#endif

int lc3_encoder_init(lc3_encoder_t *encoder, int sample_rate, int duration_us, int target_bytes, int sample_bits)
{
#if defined(LC3_HIFI4) && (LC3_HIFI4 != 0)
    struct lc3_msg_header *msg = (struct lc3_msg_header *)lc3_msg;
    struct lc3_msg_res_headr *msg_res = (struct lc3_msg_res_headr *)lc3_msg_res;

    int res;
    int instans;

    msg->type = 0x01;
    msg->len  = 10;
    memcpy(&msg->data[0], &sample_rate, sizeof(uint32_t));
    memcpy(&msg->data[4], &duration_us, sizeof(uint32_t));
    msg->data[8] = (uint8_t)target_bytes;
    msg->data[9] = (uint8_t)sample_bits;

    BOARD_DSP_IPC_Send(lc3_msg, LC3_MSG_HEADER_SIZE);
    if(msg->len)
    {
        BOARD_DSP_IPC_Send(&lc3_msg[LC3_MSG_HEADER_SIZE], msg->len);
    }

    BOARD_DSP_IPC_Recv(lc3_msg_res, LC3_MSG_HEADER_SIZE);
    if(msg_res->len)
    {
        BOARD_DSP_IPC_Recv(&lc3_msg_res[LC3_MSG_HEADER_SIZE], msg_res->len);
    }

    res = (int)msg_res->res;
    if(res)
    {
        return LC3_CODEC_ERR;
    }

    instans = (int)msg_res->data[0];
    encoder->instans = instans;

#else
    INT32 ret;

    if(!encoder)
    {
        return LC3_CODEC_ERR;
    }

#if defined(LC3_DSP) && (LC3_DSP == 0)
    ret = lc3_ndsp_enc_set_config_params(&encoder->lc3_cfg, sample_rate, 1, duration_us / 100, sample_bits);
    ret = lc3_enc_init_ch_cntx(&encoder->lc3_enc, &encoder->lc3_cfg, target_bytes);
#else
    INT32 bytes_count[1];
    INT32 *pcm_bufs_in[1];
    UINT8 *enc_bufs_out[1];

    bytes_count[0] = target_bytes;
    pcm_bufs_in[0] = encoder->buf_in;
    enc_bufs_out[0] = encoder->buf_out;

    ret = LC3_encoder_create
        (
            &encoder->lc3_ctx,
            sample_rate,
            sample_bits,
            1,
            duration_us / 100,
            bytes_count,
            encoder->core_buff,
            encoder->work_buff,
            pcm_bufs_in,
            enc_bufs_out
        );
#endif

    if(ret != LC3_ENCODER_SUCCESS)
    {
        return LC3_CODEC_ERR;
    }

#endif
    encoder->sample_rate = sample_rate;
    encoder->duration_us = duration_us;
    encoder->enc_bytes   = target_bytes;
    encoder->sample_bits = sample_bits;

    return 0;
}

int lc3_encoder(lc3_encoder_t *encoder, void *input, uint8_t *output)
{
#if defined(LC3_HIFI4) && (LC3_HIFI4 != 0)
    struct lc3_msg_header *msg = (struct lc3_msg_header *)lc3_msg;
    struct lc3_msg_res_headr *msg_res = (struct lc3_msg_res_headr *)lc3_msg_res;

    int res;
    int instans;

    msg->type = 0x03;
    msg->len  = 1 + LC3_SAMPLES_BYTES_PER_FRAME(encoder);
    msg->data[0] = (uint8_t)encoder->instans;
    memcpy(&msg->data[1], input, LC3_SAMPLES_BYTES_PER_FRAME(encoder));

    BOARD_DSP_IPC_Send(lc3_msg, LC3_MSG_HEADER_SIZE);
    if(msg->len)
    {
        BOARD_DSP_IPC_Send(&lc3_msg[LC3_MSG_HEADER_SIZE], msg->len);
    }

    BOARD_DSP_IPC_Recv(lc3_msg_res, LC3_MSG_HEADER_SIZE);
    if(msg_res->len)
    {
        BOARD_DSP_IPC_Recv(&lc3_msg_res[LC3_MSG_HEADER_SIZE], msg_res->len);
    }

    res = (int)msg_res->res;
    if(res)
    {
        return LC3_CODEC_ERR;
    }

    instans = (int)msg_res->data[0];

    memcpy(output, &msg_res->data[1], encoder->enc_bytes);

#else
    INT32 ret;

    if(!encoder)
    {
        return LC3_CODEC_ERR;
    }

#if defined(LC3_DSP) && (LC3_DSP == 0)
#else
    ret = input_precessing(encoder, input);
    if(ret != 0)
    {
        return LC3_CODEC_ERR;
    }
#endif

#if defined(LC3_DSP) && (LC3_DSP == 0)
    ret = LC3_encode_a_frame(&encoder->lc3_enc, input, output, encoder->work_buff);
#else
    ret = LC3_encoder_process(&encoder->lc3_ctx);
#endif
    if(ret == 0)
    {
        return LC3_CODEC_ERR;
    }

#if defined(LC3_DSP) && (LC3_DSP == 0)
#else
    (void)memcpy(output, encoder->buf_out, encoder->enc_bytes);
#endif
#endif
    return 0;
}

int lc3_encoder_deinit(lc3_encoder_t *encoder)
{
#if defined(LC3_HIFI4) && (LC3_HIFI4 != 0)
    struct lc3_msg_header *msg = (struct lc3_msg_header *)lc3_msg;
    struct lc3_msg_res_headr *msg_res = (struct lc3_msg_res_headr *)lc3_msg_res;

    int res;
    int instans;

    msg->type = 0x02;
    msg->len  = 1;
    msg->data[0] = (uint8_t)encoder->instans;

    BOARD_DSP_IPC_Send(lc3_msg, LC3_MSG_HEADER_SIZE);
    if(msg->len)
    {
        BOARD_DSP_IPC_Send(&lc3_msg[LC3_MSG_HEADER_SIZE], msg->len);
    }

    BOARD_DSP_IPC_Recv(lc3_msg_res, LC3_MSG_HEADER_SIZE);
    if(msg_res->len)
    {
        BOARD_DSP_IPC_Recv(&lc3_msg_res[LC3_MSG_HEADER_SIZE], msg_res->len);
    }

    res = (int)msg_res->res;
    if(res)
    {
        return LC3_CODEC_ERR;
    }

#else
    if(!encoder)
    {
        return LC3_CODEC_ERR;
    }

#if defined(LC3_DSP) && (LC3_DSP == 0)
#else
    LC3_encoder_delete(&encoder->lc3_ctx);
#endif
#endif

    (void)memset(encoder, 0, sizeof(lc3_encoder_t));

    return 0;
}

int lc3_decoder_init(lc3_decoder_t *decoder, int sample_rate, int duration_us, int input_bytes, int sample_bits)
{
#if defined(LC3_HIFI4) && (LC3_HIFI4 != 0)
    struct lc3_msg_header *msg = (struct lc3_msg_header *)lc3_msg;
    struct lc3_msg_res_headr *msg_res = (struct lc3_msg_res_headr *)lc3_msg_res;

    int res;
    int instans;

    msg->type = 0x04;
    msg->len  = 10;
    memcpy(&msg->data[0], &sample_rate, sizeof(uint32_t));
    memcpy(&msg->data[4], &duration_us, sizeof(uint32_t));
    msg->data[8] = (uint8_t)input_bytes;
    msg->data[9] = (uint8_t)sample_bits;

    BOARD_DSP_IPC_Send(lc3_msg, LC3_MSG_HEADER_SIZE);
    if(msg->len)
    {
        BOARD_DSP_IPC_Send(&lc3_msg[LC3_MSG_HEADER_SIZE], msg->len);
    }

    BOARD_DSP_IPC_Recv(lc3_msg_res, LC3_MSG_HEADER_SIZE);
    if(msg_res->len)
    {
        BOARD_DSP_IPC_Recv(&lc3_msg_res[LC3_MSG_HEADER_SIZE], msg_res->len);
    }

    res = (int)msg_res->res;
    if(res)
    {
        return LC3_CODEC_ERR;
    }

    instans = (int)msg_res->data[0];
    decoder->instans = instans;
#else
    INT32 ret;

    if(!decoder)
    {
        return LC3_CODEC_ERR;
    }

#if defined(LC3_DSP) && (LC3_DSP == 0)
    ret = lc3_ndsp_dec_set_config_params(&decoder->lc3_cfg, sample_rate, 1, 0, duration_us / 100, sample_bits);
    ret = lc3_dec_init_ch_cntx(&decoder->lc3_dec, &decoder->lc3_cfg, input_bytes);
#else
    UINT8 *enc_bufs_in[1];
    INT32 *dec_bufs_out[1];

    enc_bufs_in[0] = decoder->buf_in;
    dec_bufs_out[0] = decoder->buf_out;

    ret = LC3_decoder_create
		(
			&decoder->lc3_ctx,
			sample_rate,
			sample_bits,
			1,
			duration_us / 100,
			0,
			decoder->core_buff,
			decoder->work_buff,
			enc_bufs_in,
			dec_bufs_out
		);
#endif

    if(ret != LC3_ENCODER_SUCCESS)
    {
        return LC3_CODEC_ERR;
    }
#endif

    decoder->sample_rate = sample_rate;
    decoder->duration_us = duration_us;
    decoder->enc_bytes   = input_bytes;
    decoder->sample_bits = sample_bits;

    return 0;
}

int lc3_decoder(lc3_decoder_t *decoder, uint8_t *input, int frame_flag, void *output)
{
#if defined(LC3_HIFI4) && (LC3_HIFI4 != 0)
    struct lc3_msg_header *msg = (struct lc3_msg_header *)lc3_msg;
    struct lc3_msg_res_headr *msg_res = (struct lc3_msg_res_headr *)lc3_msg_res;

    int res;
    int instans;

    msg->type = 0x06;
    msg->len  = 2 + LC3_ENC_BYTES_PER_FRAME(decoder);
    msg->data[0] = (uint8_t)decoder->instans;
    msg->data[1] = (uint8_t)frame_flag;
    memcpy(&msg->data[2], input, LC3_ENC_BYTES_PER_FRAME(decoder));

    BOARD_DSP_IPC_Send(lc3_msg, LC3_MSG_HEADER_SIZE);
    if(msg->len)
    {
        BOARD_DSP_IPC_Send(&lc3_msg[LC3_MSG_HEADER_SIZE], msg->len);
    }

    BOARD_DSP_IPC_Recv(lc3_msg_res, LC3_MSG_HEADER_SIZE);
    if(msg_res->len)
    {
        BOARD_DSP_IPC_Recv(&lc3_msg_res[LC3_MSG_HEADER_SIZE], msg_res->len);
    }

    res = (int)msg_res->res;
    if(res)
    {
        return LC3_CODEC_ERR;
    }

    instans = (int)msg_res->data[0];

    memcpy(output, &msg_res->data[1], LC3_SAMPLES_BYTES_PER_FRAME(decoder));
#else
    INT32 ret;

    if(!decoder)
    {
        return LC3_CODEC_ERR;
    }

#if defined(LC3_DSP) && (LC3_DSP == 0)
#else
    (void)memcpy(decoder->buf_in, input, decoder->enc_bytes);
#endif

#if defined(LC3_DSP) && (LC3_DSP == 0)
    ret = LC3_decode_a_frame(&decoder->lc3_dec, input, output, frame_flag, decoder->work_buff);
#else
    INT32 flg_bfi[1];
    INT32 byte_count[1];

    flg_bfi[0] = frame_flag;
    byte_count[0] = decoder->enc_bytes;

    ret = LC3_decoder_process(&decoder->lc3_ctx, flg_bfi, byte_count);
#endif
    if((ret != LC3_DECODER_SUCCESS) && (frame_flag == LC3_FRAME_FLAG_GOOD))
    {
        return LC3_CODEC_ERR;
    }

#if defined(LC3_DSP) && (LC3_DSP == 0)
#else
    ret = output_precessing(decoder, output);
    if(ret != 0)
    {
        return LC3_CODEC_ERR;
    }
#endif
#endif
    return 0;
}

int lc3_decoder_deinit(lc3_decoder_t *decoder)
{
#if defined(LC3_HIFI4) && (LC3_HIFI4 != 0)
    struct lc3_msg_header *msg = (struct lc3_msg_header *)lc3_msg;
    struct lc3_msg_res_headr *msg_res = (struct lc3_msg_res_headr *)lc3_msg_res;

    int res;
    int instans;

    msg->type = 0x05;
    msg->len  = 1;
    msg->data[0] = (uint8_t)decoder->instans;

    BOARD_DSP_IPC_Send(lc3_msg, LC3_MSG_HEADER_SIZE);
    if(msg->len)
    {
        BOARD_DSP_IPC_Send(&lc3_msg[LC3_MSG_HEADER_SIZE], msg->len);
    }

    BOARD_DSP_IPC_Recv(lc3_msg_res, LC3_MSG_HEADER_SIZE);
    if(msg_res->len)
    {
        BOARD_DSP_IPC_Recv(&lc3_msg_res[LC3_MSG_HEADER_SIZE], msg_res->len);
    }

    res = (int)msg_res->res;
    if(res)
    {
        return LC3_CODEC_ERR;
    }
#else
    if(!decoder)
    {
        return LC3_CODEC_ERR;
    }

#if defined(LC3_DSP) && (LC3_DSP == 0)
#else
    LC3_decoder_delete(&decoder->lc3_ctx);
#endif

    (void)memset(decoder, 0, sizeof(lc3_decoder_t));
#endif
    return 0;
}