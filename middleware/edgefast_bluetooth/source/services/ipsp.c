/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stddef.h>
#include <errno/errno.h>
#include <toolchain.h>
#include <porting.h>
#include <bluetooth/gatt.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/services/ipsp.h>
#include <BT_assigned_numbers.h>


/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define IPSP_MTU     1280U
#define INIT_CREDITS 1024U

NET_BUF_POOL_DEFINE(data_pool, 1, IPSP_MTU, USER_DATA_MIN, NULL);

static int l2cap_rx(struct bt_l2cap_chan *chan, struct net_buf *buf);
static struct net_buf *alloc_buf_cb(struct bt_l2cap_chan *chan);
static int ipsp_accept(struct bt_conn *conn, struct bt_l2cap_chan **ppl2cap_chan);


/** IPSP GATT Service Declaration */
static BT_GATT_SERVICE_DEFINE(ipss_svc,
	BT_GATT_PRIMARY_SERVICE(BT_UUID_IPSS)
);

/** Server structure */
static struct bt_l2cap_server ipsp_l2cap = {
    .psm = IPSP_LE_PSM,
    .sec_level = BT_SECURITY_L1,
    .accept = ipsp_accept
};


/** Hold both Client and Server L2CAP channel information */
static struct bt_l2cap_le_chan l2cap_chan;

/** L2CAP channel callbacks  */
static const struct bt_l2cap_chan_ops l2cap_ops = {
    .alloc_buf = alloc_buf_cb,
    .recv = l2cap_rx,
    .connected = NULL,
    .disconnected = NULL,
};

/** RX data callback */
static ipsp_rx_cb_t mpf_rx_cb = NULL;

static struct net_buf *alloc_buf_cb(struct bt_l2cap_chan *chan)
{
	return net_buf_alloc(&data_pool, osaWaitForever_c);
}

static int l2cap_rx(struct bt_l2cap_chan *chan, struct net_buf *buf)
{
    if (mpf_rx_cb)
    {
        mpf_rx_cb(buf);
    }

    /* Mark that we completed processing data */
    return 0;
}

static int ipsp_accept(struct bt_conn *conn, struct bt_l2cap_chan **ppl2cap_chan)
{
    l2cap_chan.chan.ops = &l2cap_ops;
    l2cap_chan.rx.mtu = IPSP_MTU;
    l2cap_chan.rx.init_credits = INIT_CREDITS;
    *ppl2cap_chan = &l2cap_chan.chan;
    return 0;
}


/*******************************************************************************
 * API
 ******************************************************************************/
int ipsp_init(ipsp_rx_cb_t pf_rx_cb)
{
    mpf_rx_cb = pf_rx_cb;
    
    return 0;
}

int ipsp_connect(struct bt_conn *conn)
{
    int result = -1;
    if (l2cap_chan.state == BT_L2CAP_DISCONNECTED)
    {
        l2cap_chan.chan.ops = &l2cap_ops;
        l2cap_chan.rx.mtu = IPSP_MTU;
        l2cap_chan.rx.init_credits = INIT_CREDITS;

        result = bt_l2cap_chan_connect(conn, &l2cap_chan.chan, IPSP_LE_PSM);
    }
    return result;
}

int ipsp_listen(void)
{
    int err = 0;

    err = bt_l2cap_server_register(&ipsp_l2cap);
    if (err < 0) {
        /* BT_ERR("IPSS registration failed %d", err); */
    }
    return err;
}

int ipsp_send(struct net_buf *buf)
{
    return bt_l2cap_chan_send(&l2cap_chan.chan, buf);
}
