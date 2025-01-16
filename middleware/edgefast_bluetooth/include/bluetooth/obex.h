/** @file
 *  @brief Bluetooth OBEX handling.
 */

/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_BLUETOOTH_OBEX_H_
#define ZEPHYR_INCLUDE_BLUETOOTH_OBEX_H_

/* GOEP version */
#define BT_GOEP_VERSION_1_1 (0x0101U)
#define BT_GOEP_VERSION_2_0 (0x0200U)

/* OBEX Header */
#define BT_OBEX_HDR_COUNT                   (0xC0U)
#define BT_OBEX_HDR_NAME                    (0x01U)
#define BT_OBEX_HDR_TYPE                    (0x42U)
#define BT_OBEX_HDR_LENGTH                  (0xC3U)
#define BT_OBEX_HDR_TIME_ISO                (0x44U)
#define BT_OBEX_HDR_TIME_4BYTE              (0xC4U)
#define BT_OBEX_HDR_DESCRIPTION             (0x05U)
#define BT_OBEX_HDR_TARGET                  (0x46U)
#define BT_OBEX_HDR_HTTP                    (0x47U)
#define BT_OBEX_HDR_BODY                    (0x48U)
#define BT_OBEX_HDR_END_OF_BODY             (0x49U)
#define BT_OBEX_HDR_WHO                     (0x4AU)
#define BT_OBEX_HDR_CONNECTION_ID           (0xCBU)
#define BT_OBEX_HDR_APP_PARAM               (0x4CU)
#define BT_OBEX_HDR_AUTH_CHALLENGE          (0x4DU)
#define BT_OBEX_HDR_AUTH_RESPONSE           (0x4EU)
#define BT_OBEX_HDR_CREATOR_ID              (0xCFU)
#define BT_OBEX_HDR_WAN_UUID                (0x50U)
#define BT_OBEX_HDR_OBJECT_CLASS            (0x51U)
#define BT_OBEX_HDR_SESSION_PARAM           (0x52U)
#define BT_OBEX_HDR_SESSION_SEQ_NUM         (0x93U)
#define BT_OBEX_HDR_ACTION_ID               (0x94U)
#define BT_OBEX_HDR_DEST_NAME               (0x15U)
#define BT_OBEX_HDR_PERMISSION              (0xD6U)
#define BT_OBEX_HDR_SRM                     (0x97U)
#define BT_OBEX_HDR_SRMP                    (0x98U)

/* OBEX Response Codes */
#define BT_OBEX_RSP_CONTINUE                (0x90U) /* Continue */
#define BT_OBEX_RSP_SUCCESS                 (0xA0U) /* OK, Success */
#define BT_OBEX_RSP_CREATED                 (0xA1U) /* Created */
#define BT_OBEX_RSP_ACCEPTED                (0xA2U) /* Accepted */
#define BT_OBEX_RSP_NON_AUTH                (0xA3U) /* Non-Authoritative Infomation */
#define BT_OBEX_RSP_NO_CONTENT              (0xA4U) /* No content */
#define BT_OBEX_RSP_RESET_CONTENT           (0xA5U) /* Reset Content */
#define BT_OBEX_RSP_PARTIAL_CONTENT         (0xA6U) /* Partial Content */
#define BT_OBEX_RSP_MULTI_CHOICE            (0xB0U) /* Multiple Choices */
#define BT_OBEX_RSP_MOVED_PERM              (0xB1U) /* Moved Permanently */
#define BT_OBEX_RSP_MOVED_TEMP              (0xB2U) /* Moved Temporarily */
#define BT_OBEX_RSP_SEE_OTHER               (0xB3U) /* See Other */
#define BT_OBEX_RSP_NOT_MODIFIED            (0xB4U) /* Not Modified */
#define BT_OBEX_RSP_USE_PROXY               (0xB5U) /* Use Proxy */
#define BT_OBEX_RSP_BAD_REQ                 (0xC0U) /* Bad Request - server couldn’t understand request */
#define BT_OBEX_RSP_UNAUTH                  (0xC1U) /* Unauthorized */
#define BT_OBEX_RSP_PAYMENT_REQD            (0xC2U) /* Payment Required */
#define BT_OBEX_RSP_FORBIDDEN               (0xC3U) /* Forbidden - operation is understood but refused */
#define BT_OBEX_RSP_NOT_FOUND               (0xC4U) /* Not Found */
#define BT_OBEX_RSP_METHOD_NA               (0xC5U) /* Method not allowed */
#define BT_OBEX_RSP_NOT_ACCEPTABLE          (0xC6U) /* Not Acceptable */
#define BT_OBEX_RSP_PROXY_AUTH_REQD         (0xC7U) /* Proxy Authentication required */
#define BT_OBEX_RSP_REQ_TIMEOUT             (0xC8U) /* Request Time Out */
#define BT_OBEX_RSP_CONFLICT                (0xC9U) /* Conflict */
#define BT_OBEX_RSP_GONE                    (0xCAU) /* Gone */
#define BT_OBEX_RSP_LENGTH_REQD             (0xCBU) /* Length Required */
#define BT_OBEX_RSP_PRECOND_FAILED          (0xCCU) /* Precondition Failed */
#define BT_OBEX_RSP_REQ_ENTITY_LARGE        (0xCDU) /* Requested entity too large */
#define BT_OBEX_RSP_REQ_URL_LARGE           (0xCEU) /* Requested URL too large */
#define BT_OBEX_RSP_UNSUPP_MEDIA            (0xCFU) /* Unsupported media type */
#define BT_OBEX_RSP_INT_SERVER_ERR          (0xD0U) /* Internal Server Error */
#define BT_OBEX_RSP_NOT_IMPLEMENTED         (0xD1U) /* Not Implemented */
#define BT_OBEX_RSP_BAD_GATEWAY             (0xD2U) /* Bad Gateway */
#define BT_OBEX_RSP_SERVICE_UNAVBL          (0xD3U) /* Service Unavailable */
#define BT_OBEX_RSP_GATEWAY_TIMEOUT         (0xD4U) /* Gateway Timeout */
#define BT_OBEX_RSP_HTTP_VER_NOT_SUPPORTED  (0xD5U) /* HTTP version not supported */
#define BT_OBEX_RSP_DATABASE_FULL           (0xE0U) /* Database Full */
#define BT_OBEX_RSP_DATABASE_LOCKED         (0xE1U) /* Database Locked */

struct bt_obex_tag_bytes
{
    uint8_t id;
    uint8_t length;
    uint8_t value[0];
} __packed;

struct bt_obex_hdr_u8
{
    uint8_t hi;
    uint8_t hv;
} __packed;

struct bt_obex_hdr_u32
{
    uint8_t hi;
    uint32_t hv;
} __packed;

struct bt_obex_hdr_bytes
{
    uint8_t hi;
    uint16_t length;
    uint8_t hv[0];
} __packed;

/** @brief OBEX request flags. */
enum bt_obex_req_flags
{
    BT_OBEX_REQ_START     = 0x01, /* packet is a start of packet without final bit.
                                     There are packets(contiune or end) that will be sent later. */
    BT_OBEX_REQ_CONTINUE  = 0x02, /* packet is a continuation of packet */
    BT_OBEX_REQ_END       = 0x04, /* packet is a end of packet but not start of packet.
                                     There are packets(start or contiune) that have been sent before. */
    BT_OBEX_REQ_UNSEG = BT_OBEX_REQ_START |
                        BT_OBEX_REQ_END, /* unsegmented packet is a start of packet with final bit */
};

static inline void bt_obex_add_hdr(struct net_buf *buf, uint8_t hi, uint8_t *value, uint16_t length)
{
    net_buf_add_u8(buf, hi);
    net_buf_add_be16(buf, length + sizeof(struct bt_obex_hdr_bytes));
    net_buf_add_mem(buf, value, length);
}

static inline int bt_obex_get_hdr(struct net_buf *buf, uint8_t hi, uint8_t **value, uint16_t *length)
{
    struct bt_obex_hdr_bytes hdr;
    uint16_t buf_length;
    uint8_t *buf_data;

    if ((buf == NULL) || (value == NULL) || (length == NULL))
    {
        return -EINVAL;
    }
    *length = 0;

    buf_length = buf->len;
    buf_data   = buf->data;

    while (buf_length > 0U)
    {
        hdr.hi = ((struct bt_obex_hdr_bytes *)buf_data)->hi;

        if ((hdr.hi >> 6) == 0x3U) /* 4 byte */
        {
            hdr.length = 4 + sizeof(struct bt_obex_hdr_bytes);
        }
        else if ((hdr.hi >> 6) == 0x2U) /* 1 byte */
        {
            hdr.length = 1 + sizeof(struct bt_obex_hdr_bytes);
        }
        else /* unicode or byte sequence */
        {
            hdr.length = sys_cpu_to_be16(((struct bt_obex_hdr_bytes *)buf_data)->length);
        }

        if (hdr.hi == hi)
        {
            *value = &((struct bt_obex_hdr_bytes *)buf_data)->hv[0];
            *length = hdr.length - sizeof(struct bt_obex_hdr_bytes);
            return 0;
        }
        if (buf_length < hdr.length)
        {
            return -EINVAL;
        }
        buf_length -= hdr.length;
        buf_data   += hdr.length;
    }

    return -EAGAIN;
}

static inline void bt_obex_add_app_param(struct net_buf *buf, uint8_t tag_id, uint8_t *value, uint8_t length)
{
    struct bt_obex_hdr_bytes *hdr;
    uint16_t total_len;

    hdr = (struct bt_obex_hdr_bytes *)buf->data;
    if (buf->len == 0U)
    {
        net_buf_add(buf, sizeof(struct bt_obex_hdr_bytes));
        hdr->hi = BT_OBEX_HDR_APP_PARAM;
        hdr->length = sys_cpu_to_be16(sizeof(struct bt_obex_hdr_bytes));
    }
    total_len = sys_be16_to_cpu(hdr->length);
    total_len += length + sizeof(struct bt_obex_tag_bytes);  /* Tag ID(1-byte), Length(1-byte) */
    hdr->length = sys_cpu_to_be16(total_len);

    net_buf_add_u8(buf, tag_id);
    net_buf_add_u8(buf, length);
    net_buf_add_mem(buf, value, length);
}

#define BT_OBEX_ADD_APP_PARAM_U8(buf, tag_id, val) do { \
            uint8_t data = val; \
            bt_obex_add_app_param(buf, tag_id, &data, sizeof(data)); \
        } while(0);

#define BT_OBEX_ADD_APP_PARAM_U16(buf, tag_id, val) do { \
            uint8_t data[2]; \
            sys_put_be16(val, data);  \
            bt_obex_add_app_param(buf, tag_id, data, sizeof(data)); \
        } while(0);

#define BT_OBEX_ADD_APP_PARAM_U32(buf, tag_id, val) do { \
            uint8_t data[4]; \
            sys_put_be32(val, data);  \
            bt_obex_add_app_param(buf, tag_id, data, sizeof(data)); \
        } while(0);

#define BT_OBEX_ADD_APP_PARAM_U64(buf, tag_id, val) do { \
            uint8_t data[8]; \
            sys_put_be64(val, data);  \
            bt_obex_add_app_param(buf, tag_id, data, sizeof(data)); \
        } while(0);

static inline int bt_obex_get_body(struct net_buf *buf, uint8_t **body, uint16_t *length)
{
    int err;

    err = bt_obex_get_hdr(buf, BT_OBEX_HDR_BODY, body, length);
    if (err == 0)
    {
        return err;
    }

    return bt_obex_get_hdr(buf, BT_OBEX_HDR_END_OF_BODY, body, length);
}

static inline void bt_obex_app_param_parse(struct net_buf *buf,
            bool (*func)(struct bt_data *data, void *user_data),
            void *user_data)
{
    uint16_t hdr_length;
    uint8_t *hdr_value;

    if ((buf == NULL) || (func == NULL))
    {
        return;
    }

    if (bt_obex_get_hdr(buf, BT_OBEX_HDR_APP_PARAM, &hdr_value, &hdr_length) < 0)
    {
        return;
    }
    while (hdr_length)
    {
        struct bt_data data;

        data.type = ((struct bt_obex_tag_bytes *)hdr_value)->id;
        data.data_len = ((struct bt_obex_tag_bytes *)hdr_value)->length;
        data.data = &((struct bt_obex_tag_bytes *)hdr_value)->value[0];

        if (!func(&data, user_data)) {
            return;
        }

        data.data_len += sizeof(struct bt_obex_tag_bytes);
        if (hdr_length < data.data_len)
        {
            return;
        }
        hdr_length -= data.data_len;
        hdr_value  += data.data_len;
    }
}

#endif /* ZEPHYR_INCLUDE_BLUETOOTH_OBEX_H_ */
