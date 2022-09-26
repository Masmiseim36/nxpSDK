
/**
 *  \file appl_pan.c
 *
 *  Source File for PAN Command Line Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_pan.h"
#include "appl_utils.h"

#include "pan_pl.h"
#include "BT_fops.h"

#ifdef PAN
/* ----------------------------------------- Exported Global Variables */


/* ----------------------------------------- Exported Global Variables */
#if 0
/* DHCP Discover : (Ethernet Type : 0x0800) https ://www.cloudshark.org/captures/0009d5398f37 */
static UCHAR appl_dhcp_discover[] = {
    0x45, 0x00, 0x02, 0x5c, 0x00, 0x05, 0x00, 0x00, 0xff, 0x11, 0xb9, 0x8c, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x44, 0x00, 0x43, 0x02, 0x48, 0x04, 0x43,
    0x01, 0x01, 0x06, 0x00, 0x00, 0x00, 0x15, 0x5c, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcc, 0x00, 0x0a, 0xc4,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x82, 0x53, 0x63,
    0x35, 0x01, 0x01, 0x39, 0x02, 0x04, 0x80, 0x3d, 0x1b, 0x00, 0x63, 0x69, 0x73, 0x63, 0x6f, 0x2d,
    0x63, 0x63, 0x30, 0x30, 0x2e, 0x30, 0x61, 0x63, 0x34, 0x2e, 0x30, 0x30, 0x30, 0x30, 0x2d, 0x46,
    0x61, 0x30, 0x2f, 0x30, 0x0c, 0x02, 0x52, 0x30, 0x37, 0x08, 0x01, 0x06, 0x0f, 0x2c, 0x03, 0x21,
    0x96, 0x2b, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* DHCP Offer : (Ethernet Type : 0x0800) */
static UCHAR appl_dhcp_offer[] = {
    0x45, 0x00, 0x01, 0x48, 0x00, 0x02, 0x00, 0x00, 0xff, 0x11, 0xf9, 0xf9, 0xc0, 0xa8, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x43, 0x00, 0x44, 0x01, 0x34, 0xc1, 0xb6,
    0x02, 0x01, 0x06, 0x00, 0x00, 0x00,
    0x15, 0x5c, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xa8, 0x00, 0x03, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcc, 0x00, 0x0a, 0xc4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x82, 0x53, 0x63, 0x35, 0x01, 0x02, 0x36, 0x04, 0xc0,
    0xa8, 0x00, 0x01, 0x33, 0x04, 0x00, 0x00, 0x00, 0x3c, 0x3a, 0x04, 0x00, 0x00, 0x00, 0x1e, 0x3b,
    0x04, 0x00, 0x00, 0x00, 0x34, 0x01, 0x04, 0xff, 0xff, 0xff, 0x00, 0x03, 0x04, 0xc0, 0xa8, 0x00,
    0x01, 0x06, 0x08, 0xc0, 0xa8, 0x00, 0x01, 0xc0, 0xa8, 0x01, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* DHCP Request : (Ethernet Type : 0x0800) */
static UCHAR appl_dhcp_request[] = {
    0x45, 0x00, 0x02, 0x5c, 0x00, 0x06, 0x00, 0x00, 0xff, 0x11, 0xb9, 0x8b, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x44, 0x00, 0x43, 0x02, 0x48, 0xe5, 0xa4,
    0x01, 0x01, 0x06, 0x00, 0x00, 0x00,
    0x15, 0x5c, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcc, 0x00, 0x0a, 0xc4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x82, 0x53, 0x63, 0x35, 0x01, 0x03, 0x39, 0x02, 0x04,
    0x80, 0x3d, 0x1b, 0x00, 0x63, 0x69, 0x73, 0x63, 0x6f, 0x2d, 0x63, 0x63, 0x30, 0x30, 0x2e, 0x30,
    0x61, 0x63, 0x34, 0x2e, 0x30, 0x30, 0x30, 0x30, 0x2d, 0x46, 0x61, 0x30, 0x2f, 0x30, 0x36, 0x04,
    0xc0, 0xa8, 0x00, 0x01, 0x32, 0x04, 0xc0, 0xa8, 0x00, 0x03, 0x33, 0x04, 0x00, 0x00, 0x00, 0x3c,
    0x0c, 0x02, 0x52, 0x30, 0x37, 0x08, 0x01, 0x06, 0x0f, 0x2c, 0x03, 0x21, 0x96, 0x2b, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* DHCP ACK : (Ethernet Type : 0x0800) */
static UCHAR appl_dhcp_ack[] = {
    0x45, 0x00, 0x01, 0x48, 0x00, 0x03, 0x00, 0x00, 0xff, 0x11, 0xf9, 0xf8, 0xc0, 0xa8, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x43, 0x00, 0x44, 0x01, 0x34, 0x8c, 0x58,
    0x02, 0x01, 0x06, 0x00, 0x00, 0x00,
    0x15, 0x5c, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xa8, 0x00, 0x03, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcc, 0x00, 0x0a, 0xc4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x82, 0x53, 0x63, 0x35, 0x01, 0x05, 0x36, 0x04, 0xc0,
    0xa8, 0x00, 0x01, 0x33, 0x04, 0x00, 0x00, 0x00, 0x3c, 0x3a, 0x04, 0x00, 0x00, 0x00, 0x1e, 0x3b,
    0x04, 0x00, 0x00, 0x00, 0x34, 0x0c, 0x02, 0x52, 0x30, 0x01, 0x04, 0xff, 0xff, 0xff, 0x00, 0x03,
    0x04, 0xc0, 0xa8, 0x00, 0x01, 0x06, 0x08, 0xc0, 0xa8, 0x00, 0x01, 0xc0, 0xa8, 0x01, 0x01, 0xff,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* ARP Request : (Ethernet Type : 0x0806) https://www.cloudshark.org/captures/1e62881aa75b */
static UCHAR appl_arp_request[] = {
    0x00, 0x01, 0x08, 0x00, 0x06, 0x04, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x02,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* ARP Response : (Ethernet Type : 0x0806) */
static UCHAR appl_arp_response[] = {
    0x00, 0x01, 0x08, 0x00, 0x06, 0x04, 0x00, 0x02, 0xc4, 0x02, 0x32, 0x6b, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x02, 0xc4, 0x01, 0x32, 0x58, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* ARP Reply / Gratuitous : (Ethernet Type : 0x0806) https://www.cloudshark.org/captures/90bca9fa3bfe */
static UCHAR appl_arp_gratuitous_reply[] = {
    0x00, 0x01, 0x08, 0x00, 0x06, 0x04, 0x00, 0x02, 0x00, 0x00, 0x0c, 0x07, 0xac, 0x01, 0x0a, 0x00, 0x00, 0x06, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0a, 0x00, 0x00, 0x06,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* Neighbour Solicitation Request : (Ethernet Type : 0x86DD) https://www.cloudshark.org/captures/ca639fdea9f7 */
static UCHAR appl_ndns_request[] = {
    0x60, 0x00, 0x00, 0x00, 0x00, 0x20, 0x3a, 0xff, 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x27, 0xff, 0xfe, 0xd4, 0x10, 0xbb, 0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0xff, 0xfe, 0x8f, 0x95, 0x87, 0x00, 0xa9, 0x57, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x80,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x27, 0xff, 0xfe, 0xfe, 0x8f, 0x95, 0x01, 0x01,
    0x08, 0x00, 0x27, 0xd4, 0x10, 0xbb
};

/* DNS Query : (Ethernet Type : 0x0800) https://www.cloudshark.org/captures/13833cdd14ba */
static UCHAR appl_dns_query[] = {
    0x45, 0x00, 0x00, 0x38, 0x66, 0xbd, 0x00, 0x00, 0x80, 0x11, 0x02, 0x0c, 0xc0, 0xa8, 0x01, 0x34, 0x08, 0x08, 0x08, 0x08,
    0xd5, 0x39, 0x00, 0x35, 0x00, 0x24, 0x44, 0x8f,
    0x00, 0x03, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x67, 0x6f, 0x6f, 0x67, 0x6c, 0x65, 0x03, 0x63, 0x6f, 0x6d, 0x00, 0x00, 0x01, 0x00, 0x01
};

/* DNS Response : (Ethernet Type : 0x0800) */
static UCHAR appl_dns_response[] = {
    0x45, 0x08, 0x00, 0xe8, 0xb2, 0xef, 0x00, 0x00, 0x37, 0x11, 0xfe, 0x21, 0x08, 0x08, 0x08, 0x08, 0xc0, 0xa8, 0x01, 0x34,
    0x00, 0x35, 0xd5, 0x39, 0x00, 0xd4, 0x28, 0xa2,
    0x00, 0x03, 0x81, 0x80, 0x00, 0x01,
    0x00, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x06, 0x67, 0x6f, 0x6f, 0x67, 0x6c, 0x65, 0x03, 0x63, 0x6f,
    0x6d, 0x00, 0x00, 0x01, 0x00, 0x01, 0xc0, 0x0c, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04,
    0x00, 0x04, 0x4a, 0x7d, 0xec, 0x23, 0xc0, 0x0c, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04,
    0x00, 0x04, 0x4a, 0x7d, 0xec, 0x25, 0xc0, 0x0c, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04,
    0x00, 0x04, 0x4a, 0x7d, 0xec, 0x27, 0xc0, 0x0c, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04,
    0x00, 0x04, 0x4a, 0x7d, 0xec, 0x20, 0xc0, 0x0c, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04,
    0x00, 0x04, 0x4a, 0x7d, 0xec, 0x28, 0xc0, 0x0c, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04,
    0x00, 0x04, 0x4a, 0x7d, 0xec, 0x21, 0xc0, 0x0c, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04,
    0x00, 0x04, 0x4a, 0x7d, 0xec, 0x29, 0xc0, 0x0c, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04,
    0x00, 0x04, 0x4a, 0x7d, 0xec, 0x22, 0xc0, 0x0c, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04,
    0x00, 0x04, 0x4a, 0x7d, 0xec, 0x24, 0xc0, 0x0c, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04,
    0x00, 0x04, 0x4a, 0x7d, 0xec, 0x2e, 0xc0, 0x0c, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04,
    0x00, 0x04, 0x4a, 0x7d, 0xec, 0x26
};

/* LLMNR Query : (Ethernet Type : 0x0800) https://www.cloudshark.org/captures/66ae2b86a9bf?filter=frame%20and%20eth%20and%20ip%20and%20udp */
static UCHAR appl_llmnr_query[] = {
    0x45, 0x00, 0x00, 0x3d, 0x13, 0x6c, 0x00, 0x00, 0x01, 0x11, 0x1c, 0x07, 0xa9, 0xfe, 0xff, 0x42, 0xe0, 0x00, 0x00, 0xfc,
    0xf7, 0x05, 0x14, 0xeb, 0x00, 0x29, 0x8a, 0x78,
    0x7d, 0xcc, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x63, 0x68, 0x6d,
    0x75, 0x74, 0x68, 0x75, 0x2d, 0x77, 0x37, 0x2d, 0x74, 0x73, 0x74, 0x33, 0x00, 0x00, 0x01, 0x00,
    0x01
};

/* LLMNR Response : (Ethernet Type : 0x0800) */
static UCHAR appl_llmnr_response[] = {
    0x45, 0x00, 0x00, 0x5c, 0x01, 0x2b, 0x00, 0x00, 0x80, 0x11, 0x81, 0xc4, 0xa9, 0xfe, 0x64, 0x62, 0xa9, 0xfe, 0xff, 0x42,
    0x14, 0xeb, 0xf7, 0x05, 0x00, 0x48, 0x84, 0xce,
    0x7d, 0xcc, 0x80, 0x00, 0x00, 0x01,
    0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x63, 0x68, 0x6d, 0x75, 0x74, 0x68, 0x75, 0x2d, 0x77,
    0x37, 0x2d, 0x74, 0x73, 0x74, 0x33, 0x00, 0x00, 0x01, 0x00, 0x01, 0x0f, 0x63, 0x68, 0x6d, 0x75,
    0x74, 0x68, 0x75, 0x2d, 0x77, 0x37, 0x2d, 0x74, 0x73, 0x74, 0x33, 0x00, 0x00, 0x01, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x1e, 0x00, 0x04, 0xa9, 0xfe, 0x64, 0x62
};


/* LLMNR Query IPv6: (Ethernet Type : 0x86DD) https://www.cloudshark.org/captures/a59f35d38471 */
static UCHAR appl_llmnr_query_ipv6[] = {
    0x60, 0x00, 0x00, 0x00, 0x00, 0x29, 0x11, 0x01, 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x9c, 0x09, 0xb4, 0x16, 0x07, 0x68, 0xff, 0x42, 0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x03, 0xcd, 0x1b, 0x14, 0xeb, 0x00, 0x29, 0x56, 0x3f,
    0xd0, 0xc8, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x63, 0x68, 0x6d,
    0x75, 0x74, 0x68, 0x75, 0x2d, 0x77, 0x37, 0x2d, 0x74, 0x65, 0x73, 0x74, 0x00,
    0x00, 0xff, 0x00, 0x01
};

/* HTTP Request: (Ethernet Type : 0x0800) https://www.cloudshark.org/captures/3ab30ee32c91 */
static UCHAR appl_http_request[] = {
    0x45, 0x00, 0x00, 0xba, 0xcb, 0x5d, 0x40, 0x00, 0x40, 0x06, 0x28, 0x64, 0xc0, 0xa8, 0x01, 0x8c, 0xae, 0x8f, 0xd5, 0xb8,
    0xe1, 0x4e, 0x00, 0x50, 0x8e, 0x50, 0x19, 0x02, 0xc7, 0x52, 0x9d, 0x89, 0x80, 0x18, 0x00, 0x2e, 0x47, 0x29, 0x00, 0x00, 0x01, 0x01, 0x08, 0x0a, 0x00, 0x21, 0xd2, 0x5f, 0x31, 0xc7, 0xba, 0x48,
    0x47, 0x45, 0x54, 0x20, 0x2f, 0x69, 0x6d, 0x61, 0x67, 0x65, 0x73, 0x2f, 0x6c, 0x61,
    0x79, 0x6f, 0x75, 0x74, 0x2f, 0x6c, 0x6f, 0x67, 0x6f, 0x2e, 0x70, 0x6e, 0x67, 0x20, 0x48, 0x54,
    0x54, 0x50, 0x2f, 0x31, 0x2e, 0x30, 0x0d, 0x0a, 0x55, 0x73, 0x65, 0x72, 0x2d, 0x41, 0x67, 0x65,
    0x6e, 0x74, 0x3a, 0x20, 0x57, 0x67, 0x65, 0x74, 0x2f, 0x31, 0x2e, 0x31, 0x32, 0x20, 0x28, 0x6c,
    0x69, 0x6e, 0x75, 0x78, 0x2d, 0x67, 0x6e, 0x75, 0x29, 0x0d, 0x0a, 0x41, 0x63, 0x63, 0x65, 0x70,
    0x74, 0x3a, 0x20, 0x2a, 0x2f, 0x2a, 0x0d, 0x0a, 0x48, 0x6f, 0x73, 0x74, 0x3a, 0x20, 0x70, 0x61,
    0x63, 0x6b, 0x65, 0x74, 0x6c, 0x69, 0x66, 0x65, 0x2e, 0x6e, 0x65, 0x74, 0x0d, 0x0a, 0x43, 0x6f,
    0x6e, 0x6e, 0x65, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x3a, 0x20, 0x4b, 0x65, 0x65, 0x70, 0x2d, 0x41,
    0x6c, 0x69, 0x76, 0x65, 0x0d, 0x0a, 0x0d, 0x0a
};

/* HTTP Response: (Ethernet Type : 0x0800) https://www.cloudshark.org/captures/3ab30ee32c91 */
static UCHAR appl_http_response[] = {
    0x45, 0x00, 0x01, 0x79, 0x27, 0xed, 0x40, 0x00, 0xfb, 0x06, 0x10, 0x15, 0xae, 0x8f, 0xd5, 0xb8, 0xc0, 0xa8, 0x01, 0x8c,
    0x00, 0x50, 0xe1, 0x4e, 0xc7, 0x52, 0xf2, 0x61, 0x8e, 0x50, 0x19, 0x88, 0x80, 0x18, 0x00, 0x6c, 0x7d, 0x05, 0x00, 0x00, 0x01, 0x01, 0x08, 0x0a, 0x31, 0xc7, 0xba, 0x6e, 0x00, 0x21, 0xd2, 0x69,
    0x6b, 0x7a, 0xc5, 0xdd, 0xfe, 0x17, 0x00, 0x9a, 0xa1, 0x80, 0xae, 0xcb, 0xee, 0x0b,
    0x8c, 0x15, 0x92, 0x78, 0x2c, 0x97, 0xee, 0x00, 0x20, 0xde, 0x28, 0xa2, 0x88, 0x22, 0x8a, 0x28,
    0xa2, 0x7c, 0x7c, 0x51, 0xf7, 0x8c, 0xad, 0xa4, 0xfc, 0x2c, 0x62, 0x80, 0xaa, 0x6b, 0x4c, 0x5b,
    0xe7, 0xf0, 0x53, 0x1f, 0x2c, 0x5d, 0xeb, 0x2f, 0xd7, 0x37, 0xd7, 0x0e, 0x5f, 0xb3, 0x15, 0x01,
    0x9d, 0xf5, 0x31, 0x00, 0x4d, 0xa6, 0x95, 0x29, 0x5a, 0x2d, 0x7f, 0x80, 0xef, 0xbf, 0xd4, 0x7e,
    0xbe, 0x4a, 0x2a, 0xde, 0x09, 0xa2, 0x88, 0x22, 0x8a, 0x28, 0xa2, 0xfc, 0x2b, 0x44, 0x3b, 0x60,
    0xa5, 0x99, 0xaa, 0x5b, 0xde, 0x65, 0x54, 0x35, 0x83, 0x56, 0x99, 0x34, 0x5d, 0xdd, 0xc4, 0xcd,
    0x66, 0x56, 0x5f, 0xac, 0xcb, 0xfe, 0xdf, 0x6e, 0xc6, 0x2e, 0x9d, 0xee, 0x9b, 0x4d, 0x9d, 0x74,
    0xdf, 0x6f, 0x99, 0x6f, 0xfb, 0xf3, 0x8e, 0xcd, 0x08, 0xe8, 0x0b, 0x08, 0xe8, 0x97, 0xc6, 0x01,
    0x7d, 0xb6, 0x60, 0x40, 0xe7, 0xb7, 0xd4, 0xe7, 0xd0, 0x55, 0xa0, 0xe9, 0xbf, 0xe2, 0x9e, 0xba,
    0x57, 0xfc, 0x41, 0x8c, 0xe2, 0x43, 0xd4, 0x3d, 0x62, 0xc7, 0x68, 0x87, 0xac, 0xaa, 0x22, 0xde,
    0x05, 0xa2, 0x88, 0x22, 0x8a, 0x28, 0xa2, 0x88, 0x62, 0x44, 0x94, 0x9d, 0x96, 0xe7, 0xf8, 0xdf,
    0x6e, 0xce, 0x5e, 0x4b, 0xbb, 0xe9, 0xbb, 0x6d, 0xec, 0xe6, 0xee, 0x2b, 0xe9, 0xb0, 0xf8, 0x60,
    0x45, 0x47, 0xef, 0xc3, 0x9d, 0x1c, 0x03, 0x8f, 0x7c, 0x8f, 0x80, 0x0e, 0x42, 0x40, 0x6f, 0xc3,
    0x08, 0xfa, 0xb0, 0x73, 0xf4, 0xc9, 0xbf, 0x9d, 0xc3, 0x8e, 0xdf, 0x71, 0x0a, 0x3a, 0x92, 0xe2,
    0xe8, 0x73, 0x38, 0x13, 0x01, 0x9d, 0x86, 0x80, 0x7e, 0x64, 0x3f, 0x6b, 0xef, 0x0d, 0xdb, 0xdf,
    0x77, 0x9d, 0xb5, 0xfd, 0x65, 0xc7, 0x61, 0xdd, 0xf7, 0x5b, 0x57, 0xe9, 0x26, 0x6c, 0x9e, 0x83,
    0xe0, 0x1f, 0x61, 0x3d, 0x66, 0x5d, 0x5d, 0xed, 0xb0, 0xd5, 0x65, 0x34, 0x43, 0x56, 0x39, 0x59,
    0x8d, 0x5a, 0xa3, 0x34, 0x12, 0xc9, 0x8b, 0x17, 0x42, 0x14, 0x51, 0x44, 0x11, 0x45, 0x94, 0xf7,
    0x2a, 0xff, 0x03, 0x4e, 0xb9, 0x6b, 0x4c, 0x97, 0xbd, 0xff, 0x42, 0x00, 0x00, 0x00, 0x00, 0x49,
    0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
};

/* IPv4 Ping Request: (Ethernet Type : 0x0800) https://www.cloudshark.org/captures/1a48275478d5 */
static UCHAR appl_ping_request[] = {
    0x45, 0x00, 0x00, 0x64, 0x00, 0x0f, 0x00, 0x00, 0xff, 0x01, 0xa7, 0x87, 0x0a, 0x00, 0x00, 0x01, 0x0a, 0x00, 0x00, 0x02,
    0x08, 0x00, 0x03, 0x5c, 0x00, 0x03, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x2f, 0x7a, 0xbc, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd,
    0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd,
    0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd,
    0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd,
    0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd
};

/* IPv4 Ping Reply: (Ethernet Type : 0x0800) https://www.cloudshark.org/captures/1a48275478d5 */
static UCHAR appl_ping_reply[] = {
    0x45, 0x00, 0x00, 0x64, 0x00, 0x0f, 0x00, 0x00, 0xff, 0x01, 0xa7, 0x87, 0x0a, 0x00, 0x00, 0x02, 0x0a, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x0b, 0x5c, 0x00, 0x03, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x2f, 0x7a, 0xbc, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd,
    0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd,
    0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd,
    0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd,
    0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd, 0xab, 0xcd
};
#endif /* 0 */

/* ----------------------------------------- Static Global Variables */
static const char pan_options[] = "\n\
========= PAN Menu ================= \n\
    0.  Exit.\n\
    1.  Refresh.\n\
 \n\
    2.  Show PAN Handles.\n\
 \n\
    3.  PAN Init.\n\
    4.  PAN Shutdown.\n\
    5.  PAN Registration.\n\
    6.  PAN Set Policy.\n\
 \n\
    8.  ACL Connect.\n\
    9.  ACL Disconnect. \n\
 \n\
    10. PAN Accept.\n\
    11. PAN Accept Cancel. \n\
    12. PAN Connect.\n\
    13. PAN Setup.\n\
    14. Set PAN Setup Response.\n\
    15. PAN Filter.\n\
    16. PAN Disconnect.\n\
 \n\
    20. PAN Write. \n\
    21. PAN Write from File \n\
 \n\
    30. Enable/Disable Header Compression. \n\
 \n\
    50. Enable DHCP. \n\
 \n\
Your PAN Option ? \0";


/* PAN Connection Handles */
static APPL_PAN_HANDLE appl_pan_handle [PAN_MAX_ENTITIES];

/** HCI ACL Connection List */
static HCI_CONNECTION_LIST appl_pan_acl_list [BT_MAX_REMOTE_DEVICES];

/* To Reply to PAN_SETUP_IND */
static UINT16 appl_pan_setup_ind_reply = PAN_SETUP_SUCCESSFUL;

/* Local BD_ADDR */
UCHAR appl_pan_local_bd_addr [BT_BD_ADDR_SIZE];

/* PAN Services activated. */
UCHAR appl_pan_services = PAN_SERVICE_NONE;


/* Static Echo Request Packet */
DECL_STATIC DECL_CONST UCHAR appl_echo_req[] =
{
    0x45U, 0x00U, 0x00U, 0x3CU, 0x7AU, 0xA2U, 0x00U, 0x00U, 0x80U, 0x01U, 0x00U, 0x00U, 0xC0U, 0xA8U, 0xA7U, 0x98U,
    0xC0U, 0xA8U, 0xA8U, 0x64U, 0x08U, 0x00U, 0x4DU, 0x71U, 0x00U, 0x01U, 0x00U, 0x0AU, 0x61U, 0x62U, 0x63U, 0x64U,
    0x65U, 0x66U, 0x67U, 0x68U, 0x69U, 0x6AU, 0x6BU, 0x6CU, 0x6DU, 0x6EU, 0x6FU, 0x70U, 0x71U, 0x72U, 0x73U, 0x74U,
    0x75U, 0x76U, 0x77U, 0x78U, 0x79U, 0x7AU, 0x7BU, 0x7CU, 0x7DU, 0x7EU, 0x7FU, 0x80U
};

static void pan_pan_read(UCHAR *edata, UINT16 edatalen);
static UCHAR pan_bd_addr[BT_BD_ADDR_SIZE];
static UCHAR * panc_bd_addr;

static UCHAR pan_local_service;
static UCHAR appl_have_dhcp;

/* ----------------------------------------- Functions */

void main_pan_operations (void)
{
    int indx, choice, menu_choice;
    API_RESULT retval;
    static UCHAR first_time = 0x0U;
    UINT16 hci_handle;

#if 0
    FILE *fp;

    fp = fopen("dhcp_discover.dat", "wb");
    fwrite(appl_dhcp_discover, sizeof(appl_dhcp_discover), 1, fp);
    fclose(fp);
    fp = fopen("dhcp_offer.dat", "wb");
    fwrite(appl_dhcp_offer, sizeof(appl_dhcp_offer), 1, fp);
    fclose(fp);
    fp = fopen("dhcp_request.dat", "wb");
    fwrite(appl_dhcp_request, sizeof(appl_dhcp_request), 1, fp);
    fclose(fp);
    fp = fopen("dhcp_ack.dat", "wb");
    fwrite(appl_dhcp_ack, sizeof(appl_dhcp_ack), 1, fp);
    fclose(fp);
    fp = fopen("arp_request.dat", "wb");
    fwrite(appl_arp_request, sizeof(appl_arp_request), 1, fp);
    fclose(fp);
    fp = fopen("arp_response.dat", "wb");
    fwrite(appl_arp_response, sizeof(appl_arp_response), 1, fp);
    fclose(fp);
    fp = fopen("arp_gratuitous_reply.dat", "wb");
    fwrite(appl_arp_gratuitous_reply, sizeof(appl_arp_gratuitous_reply), 1, fp);
    fclose(fp);
    fp = fopen("ndns_request.dat", "wb");
    fwrite(appl_ndns_request, sizeof(appl_ndns_request), 1, fp);
    fclose(fp);
    fp = fopen("dns_query.dat", "wb");
    fwrite(appl_dns_query, sizeof(appl_dns_query), 1, fp);
    fclose(fp);
    fp = fopen("dns_response.dat", "wb");
    fwrite(appl_dns_response, sizeof(appl_dns_response), 1, fp);
    fclose(fp);
    fp = fopen("llmnr_query.dat", "wb");
    fwrite(appl_llmnr_query, sizeof(appl_llmnr_query), 1, fp);
    fclose(fp);
    fp = fopen("llmnr_response.dat", "wb");
    fwrite(appl_llmnr_response, sizeof(appl_llmnr_response), 1, fp);
    fclose(fp);
    fp = fopen("llmnr_query_ipv6.dat", "wb");
    fwrite(appl_llmnr_query_ipv6, sizeof(appl_llmnr_query_ipv6), 1, fp);
    fclose(fp);
    fp = fopen("http_request.dat", "wb");
    fwrite(appl_http_request, sizeof(appl_http_request), 1, fp);
    fclose(fp);
    fp = fopen("http_response.dat", "wb");
    fwrite(appl_http_response, sizeof(appl_http_response), 1, fp);
    fclose(fp);
    fp = fopen("ping_request.dat", "wb");
    fwrite(appl_ping_request, sizeof(appl_ping_request), 1, fp);
    fclose(fp);
    fp = fopen("ping_reply.dat", "wb");
    fwrite(appl_ping_reply, sizeof(appl_ping_reply), 1, fp);
    fclose(fp);

    fp = fopen("eth_payload_60_0x5a0.dat", "wb");

    for (indx = 0; indx <= 0x3b; indx++)
    {
        fwrite(&indx, 1, 1, fp);
    }
    for (indx = 0; indx <= 0xff; indx++)
    {
        fwrite(&indx, 1, 1, fp);
    }
    for (indx = 0; indx <= 0xff; indx++)
    {
        fwrite(&indx, 1, 1, fp);
    }
    for (indx = 0; indx <= 0xff; indx++)
    {
        fwrite(&indx, 1, 1, fp);
    }
    for (indx = 0; indx <= 0xff; indx++)
    {
        fwrite(&indx, 1, 1, fp);
    }
    for (indx = 0; indx <= 0xff; indx++)
    {
        fwrite(&indx, 1, 1, fp);
    }
    for (indx = 0; indx <= 0x9f; indx++)
    {
        fwrite(&indx, 1, 1, fp);
    }
    fclose(fp);
#endif /* 0 */

    if (0x0U == first_time)
    {
        first_time = 0x1U;

        for (indx = 0U; indx < PAN_MAX_ENTITIES; indx ++)
        {
            appl_pan_handle[indx].allocated = 0x0U;
            appl_pan_handle[indx].local_service = PAN_SERVICE_NONE;
            appl_pan_handle[indx].remote_service = PAN_SERVICE_NONE;
            BT_mem_set(appl_pan_handle[indx].bd_addr, 0x0, BT_BD_ADDR_SIZE);
        }

        (BT_IGNORE_RETURN_VALUE) BT_hci_get_local_bd_addr (appl_pan_local_bd_addr);
    }

    BT_LOOP_FOREVER()
    {
        printf("%s", pan_options); fflush(stdout);
        scanf("%d", &choice);
        menu_choice = choice;

        switch (choice)
        {
        case 0:
            break; /* return; */

        case 1:
        break;

        case 2:
            appl_pan_display_handles();
            break;

        case 3:
            LOG_DEBUG("Initializing PAN ... "); fflush (stdout);
            retval = BT_pan_init();
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("FAILED. Reason = 0x%04X\n", retval);
            }
            else
            {
                LOG_DEBUG("SUCCEEDED\n");
            }

            break;

        case 4:
            LOG_DEBUG("Shutting down PAN ... "); fflush (stdout);
            retval = BT_pan_shutdown();
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("FAILED. Reason = 0x%04X\n", retval);
            }
            else
            {
                LOG_DEBUG("SUCCEEDED\n");
            }

            break;

        case 5:
            appl_pan_registration();
        break;

        case 6:
            appl_pan_set_policy();
            break;

        case 8:
            /*
             * First Application should establish ACL Connection with
             * the peer Device
             */
            LOG_DEBUG("Enter PAN peer BD Address: ");

            /* Read the BD_ADDR of Remote Device */
            appl_get_bd_addr(pan_bd_addr);

            retval = BT_hci_create_connection
                     (
                         pan_bd_addr,
                         LMP_ACL_DH5,
                         0x0U, 0x0U, 0x0U, 0x1U
                     );
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("HCI Connect Attempt FAILED !! Error Code = 0x%04X\n",
                        retval);
            }

            break;

        case 9:
            retval = BT_hci_get_acl_connection_handle
                     (
                         pan_bd_addr,
                         &hci_handle
                     );

            if (API_SUCCESS == retval)
            {
                LOG_DEBUG("Disconnecting ACL Connection 0x%04X\n",
                hci_handle);
            }
            else
            {
                LOG_DEBUG ("ACL Connection for PAN not found\n");
                LOG_DEBUG("Enter ACL Handle\n");
                scanf("%x",&choice);
                hci_handle = (UINT16)choice;
            }

            retval = BT_hci_disconnect
                     (
                         hci_handle,
                         0x13U
                     );

            LOG_DEBUG("> API RETVAL ACL disconnect : 0x%04X\n", retval);

            break;

        case 10:
            appl_pan_accept();
            break;

        case 11:
            appl_pan_accept_cancel();
            break;

        case 12:
            appl_pan_connect();
        break;

        case 13:
            appl_pan_setup();
            break;

        case 14:
            LOG_DEBUG("[0] Set Setup IND Reply - Reject\n");
            LOG_DEBUG("[1] Set Setup IND Reply - Accept\n");
            LOG_DEBUG("Your Choice = "); fflush (stdout);
            scanf("%d", &choice);

            if (0U == choice)
            {
                appl_pan_setup_ind_reply = PAN_SETUP_NOT_ALLOWED;
            }
            else if (1U == choice)
            {
                appl_pan_setup_ind_reply = PAN_SETUP_SUCCESSFUL;
            }
            else
            {
                LOG_DEBUG("*** Invalid Choice. Try again\n");
            }

            break;

        case 15:
            appl_pan_filter();
        break;

        case 16:
            appl_pan_disconnect();
        break;

        case 20:
            appl_pan_write_data();
        break;

        case 21:
            appl_pan_write_data_from_file();
            break;

        case 30:
            appl_pan_hdr_comression();
        break;

        case 50:
            LOG_DEBUG ("Enable/Disable DHCP (1/0): ");
            scanf("%d", &choice);
            appl_have_dhcp = (UCHAR)choice;
            break;

        default:
            LOG_DEBUG("Unrecognised User Option for PAN Operations\n");
        break;
        }

        if (0 == menu_choice)
        {
            /* return */
            break;
        }
    }

    return;
}


void appl_pan_registration ( void )
{
    int choice;
    API_RESULT retval;
    PAN_CALLBACKS pan_cb;
    BT_DEVICE_ADDR bd_addr;

    pan_cb.pan_event_ind = appl_pan_event_ind;
    pan_cb.pan_read_ind = appl_pan_read_ind;

    LOG_DEBUG("Registering to PAN ... "); fflush (stdout);
    retval = BT_pan_register (&pan_cb);
    if (API_SUCCESS != retval)
    {
        LOG_DEBUG("FAILED !! Reason = 0x%04x.\n", retval);
    }
    else
    {
        LOG_DEBUG("SUCCEEDED !\n");

        /* Get Local Service */
        LOG_DEBUG("Options for Local Service: [1] PANU [2] NAP [3] GN\n");
        LOG_DEBUG("Your Choice = "); fflush(stdout);
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            pan_local_service = PAN_SERVICE_PANU;
            break;
        case 2:
            pan_local_service = PAN_SERVICE_NAP;
            break;
        case 3:
            pan_local_service = PAN_SERVICE_GN;
            break;
        default:
            /* Error Case */
            pan_local_service = (UCHAR) choice;
            break;
        }

        bd_addr.type = 0x00U;
        (BT_IGNORE_RETURN_VALUE) BT_hci_get_local_bd_addr(bd_addr.addr);

        pan_init_pl
        (
            &bd_addr,
            pan_pan_read,
            (PAN_SERVICE_NAP == pan_local_service)
        );
    }

    return;
}


void appl_pan_set_policy ( void )
{
    int indx, choice;
    API_RESULT retval;
    UCHAR policy;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    indx = 0;

    /* Choose Handle */
    appl_pan_get_handle (&indx);
    if (indx < 0)
    {
        LOG_DEBUG("Invalid PAN Handle\n");
        /* return; */
    }
    else
    {
        LOG_DEBUG("Enter Policy (Hex) = "); fflush(stdin);
        scanf("%x", &choice);
        policy = (UCHAR)choice;

        /* PAN Set Policy */
        LOG_DEBUG("Starting PAN Set Policy ... "); fflush(stdout);
        retval = BT_pan_set_policy(appl_pan_handle[indx].bd_addr, policy);

        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("FAILED !! Reason = 0x%04X\n", retval);
            /* return; */
        }
        else
        {
            LOG_DEBUG("SUCCEEDED !!\n");
        }
    }

    return;
}


void appl_pan_accept ( void )
{
    int service;
    API_RESULT retval;

    LOG_DEBUG("Configure Local PAN Service:\n");
    LOG_DEBUG(" 1. Start PANU Service\n");
    LOG_DEBUG(" 2. Start NAP Service\n");
    LOG_DEBUG(" 3. Start GN Service\n");
    LOG_DEBUG("Your Choice = "); fflush(stdout);
    scanf("%d", &service);

    /* Init */
    retval = API_SUCCESS;

    switch (service)
    {
    case 1:
#ifdef PAN_ROLE_PANU
        LOG_DEBUG("Configuring as PANU\n");

        appl_pan_services |= PAN_SERVICE_PANU;

        /* Register with SDP database */
        (BT_IGNORE_RETURN_VALUE) appl_pan_update_sdp_record (DB_RECORD_PANU, 0x01U);
#else
        LOG_DEBUG ("PAN_ROLE_PANU is not defined.\n");
#endif /* PAN_ROLE_PANU */

        break;

    case 2:
#ifdef PAN_ROLE_NAP
        LOG_DEBUG("Configuring as NAP\n");

        appl_pan_services |= PAN_SERVICE_NAP;

        /* Register with SDP database */
        (BT_IGNORE_RETURN_VALUE) appl_pan_update_sdp_record (DB_RECORD_NAP, 0x01U);
#else
        LOG_DEBUG ("PAN_ROLE_NAP is not defined.\n");
#endif /* PAN_ROLE_NAP */

        break;

    case 3:
#ifdef PAN_ROLE_GN
        LOG_DEBUG("Configuring as GN\n");

        appl_pan_services |= PAN_SERVICE_GN;

        /* Register with SDP database */
        (BT_IGNORE_RETURN_VALUE) appl_pan_update_sdp_record (DB_RECORD_GN, 0x01U);
#else
        LOG_DEBUG ("PAN_ROLE_GN is not defined.\n");
#endif /* PAN_ROLE_GN */

        break;

    default:
        LOG_DEBUG("Invalid Service Type: %d\n", service);
        retval = API_FAILURE; /* return; */
    }

    if (API_SUCCESS == retval)
    {
        /* PAN Accept */
        LOG_DEBUG("Starting PAN Accept ... "); fflush (stdout);
        retval = BT_pan_accept (appl_pan_services);

        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("FAILED !! Reason = 0x%04X\n", retval);
            /* return; */
        }
        else
        {
            LOG_DEBUG("SUCCEEDED !!\n");
        }
    }

    return;
}

void appl_pan_accept_cancel ( void )
{
    int service;
    API_RESULT retval;

    LOG_DEBUG("Configure Local PAN Service:\n");
    LOG_DEBUG(" 1. Stop PANU Service\n");
    LOG_DEBUG(" 2. Stop NAP Service\n");
    LOG_DEBUG(" 3. Stop GN Service\n");
    LOG_DEBUG("Your Choice = "); fflush(stdout);
    scanf("%d", &service);

    /* Init */
    retval = API_SUCCESS;

    switch ( service )
    {
    case 1:
#ifdef PAN_ROLE_PANU
        LOG_DEBUG("Shutting down PANU service\n");

        appl_pan_services &= ~PAN_SERVICE_PANU;

        /* Unregister from SDP database */
        (BT_IGNORE_RETURN_VALUE) appl_pan_update_sdp_record (DB_RECORD_PANU, 0x00U);
#else
        LOG_DEBUG ("PAN_ROLE_PANU is not defined.\n");
#endif /* PAN_ROLE_PANU */

        break;

    case 2:
#ifdef PAN_ROLE_NAP
        LOG_DEBUG("Shutting down NAP service\n");

        appl_pan_services &= ~PAN_SERVICE_NAP;

        /* Unregister from SDP database */
        (BT_IGNORE_RETURN_VALUE) appl_pan_update_sdp_record (DB_RECORD_NAP, 0x00U);
#else
        LOG_DEBUG ("PAN_ROLE_NAP is not defined.\n");
#endif /* PAN_ROLE_NAP */

        break;

    case 3:
#ifdef PAN_ROLE_GN
        LOG_DEBUG("Shutting down GN service\n");

        appl_pan_services &= ~PAN_SERVICE_GN;

        /* Unregister from SDP database */
        (BT_IGNORE_RETURN_VALUE) appl_pan_update_sdp_record (DB_RECORD_GN, 0x00U);
        #else
        LOG_DEBUG ("PAN_ROLE_GN is not defined.\n");
#endif /* PAN_ROLE_GN */

        break;

    default:
        LOG_DEBUG("Invalid Service Type: %d\n", service);
        retval = API_FAILURE; /* return; */
    }

    if (API_SUCCESS == retval)
    {
        /* PAN Accept Cancel */
        LOG_DEBUG("Starting PAN Accept Cancel... "); fflush(stdout);
        retval = BT_pan_accept(appl_pan_services);

        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("FAILED !! Reason = 0x%04X\n", retval);
            /* return; */
        }
        else
        {
            LOG_DEBUG("SUCCEEDED !!\n");
        }
    }

    return;
}


API_RESULT appl_pan_update_sdp_record
           (
               /* IN */  UCHAR    service_type,
               /* IN */  UCHAR    flag
           )
{
    UINT32 record_handle;
    API_RESULT retval;

    /* Get Record Handle from SDP Database */
    retval = BT_dbase_get_record_handle (service_type, 0U, &record_handle);

    if (API_SUCCESS == retval)
    {
        /* Activate/Inactivate Record Handle */
        retval = (0x1U == flag)
                 ? BT_dbase_activate_record (record_handle)
                 : BT_dbase_inactivate_record (record_handle);
    }

    return retval;
}


void appl_pan_connect ( void )
{
    int indx;
    UINT16 acl_indx;
    API_RESULT retval;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    indx = 0;
    acl_indx = 0U;

    /* Choose Handle */
    appl_pan_get_handle (&indx);
    if (indx < 0)
    {
        LOG_DEBUG("Invalid PAN Handle\n");
        /* return; */
    }
    else
    {
        /* Allocate Handle */
        appl_pan_handle[indx].allocated = 0x1U;

        /* Get Remote BD_ADDR */
        retval = appl_pan_choose_acl_connection(&acl_indx);
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("Invalid ACL Handle\n");
            /* return; */
        }
        else
        {
            /* Copy BD_ADDR */
            BT_COPY_BD_ADDR
            (
                appl_pan_handle[indx].bd_addr,
                appl_pan_acl_list[acl_indx].bd_addr
            );

            /* Initiate PAN Connect */
            LOG_DEBUG("Starting PAN Connect ... "); fflush(stdout);
            retval = BT_pan_connect(appl_pan_handle[indx].bd_addr);

            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("FAILED !! Reason = 0x%04X\n", retval);
                /* return; */
            }
            else
            {
                LOG_DEBUG("SUCCEEDED !!\n");
                LOG_DEBUG("Wait for PAN_CONNECT_CNF\n");
            }
        }
    }

    return;
}


void appl_pan_setup ( void )
{
    int indx, choice;
    API_RESULT retval;
    UCHAR remote_service;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    indx = 0;

    /* Choose Handle */
    appl_pan_get_handle (&indx);
    if (indx < 0)
    {
        LOG_DEBUG("Invalid PAN Handle\n");
        /* return; */
    }
    else
    {
        /* Get Remote Service */
        LOG_DEBUG("Options for Remote Service: [1] PANU [2] NAP [3] GN\n");
        LOG_DEBUG("Your Choice = "); fflush(stdout);
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            remote_service = PAN_SERVICE_PANU;
            break;
        case 2:
            remote_service = PAN_SERVICE_NAP;
            break;
        case 3:
            remote_service = PAN_SERVICE_GN;
            break;
        default:
            /* Error Case */
            remote_service = (UCHAR)choice;
            break;
        }

        /* Initiate PAN Setup */
        LOG_DEBUG("Starting PAN Setup ... "); fflush(stdout);
        retval = BT_pan_setup
        (appl_pan_handle[indx].bd_addr, pan_local_service, remote_service);

        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("FAILED !! Reason = 0x%04X\n", retval);
            /* return; */
        }
        else
        {
            LOG_DEBUG("SUCCEEDED !!\n");
            LOG_DEBUG("Wait for PAN_SETUP_CNF\n");
        }
    }

    return;
}


void appl_pan_filter ( void )
{
    void * filter;
    PAN_FILTER_NW_TYPE * nw_type_filter;
    PAN_FILTER_MC_ADDR * mc_addr_filter;

    int indx, choice, i;
    API_RESULT retval;

    UINT16 num_range;
    UCHAR local, type;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    indx = 0;

    /* Choose Handle */
    appl_pan_get_handle (&indx);
    if (indx < 0)
    {
        LOG_DEBUG("Invalid PAN Handle\n");
        /* return; */
    }
    else
    {
        LOG_DEBUG("Enter Filter Type: [1] Local [2] Remote ? "); fflush (stdout);
        scanf("%d", &choice);
        local = (UCHAR) choice;

        LOG_DEBUG("Enter Filter Type: [1] NW Type [2] MC ADDR ? "); fflush (stdout);
        scanf("%d", &choice);
        type = (UCHAR) choice;

        LOG_DEBUG("Enter Number of Filter Range = "); fflush (stdout);
        scanf("%d", &choice);
        num_range = (UINT16) choice;

        if (num_range > 0U)
        {
            switch (type)
            {
            case PAN_FILTER_TYPE_NW_TYPE:
                filter = BT_alloc_mem (sizeof (PAN_FILTER_NW_TYPE) * num_range);

                if (NULL == filter)
                {
                    LOG_DEBUG("*** Memory Allocation FAILED for NW Type Filter\n");
                    /* return; */
                }
                else
                {
                    nw_type_filter = filter;
                    for (i = 0U; i < num_range; i++)
                    {
                        LOG_DEBUG("\tFilter %d: Start = ", i); fflush(stdout);
                        scanf("%x", &choice);
                        nw_type_filter[i].start = (UINT16)choice;

                        LOG_DEBUG("\tFilter %d: End   = ", i); fflush(stdout);
                        scanf("%x", &choice);
                        nw_type_filter[i].end = (UINT16)choice;
                    }
                }

                break;

            case PAN_FILTER_TYPE_MC_ADDR:
                filter = BT_alloc_mem (sizeof (PAN_FILTER_MC_ADDR) * num_range);

                if (NULL == filter)
                {
                    LOG_DEBUG("*** Memory Allocation FAILED for MC Addr Filter\n");
                    /* return; */
                }
                else
                {
                    mc_addr_filter = filter;
                    for (i = 0U; i < num_range; i++)
                    {
                        LOG_DEBUG("\tFilter %d: Start = ", i); fflush(stdout);
                        appl_get_bd_addr(mc_addr_filter[i].start);

                        LOG_DEBUG("\tFilter %d: End   = ", i); fflush(stdout);
                        appl_get_bd_addr(mc_addr_filter[i].end);
                    }
                }

                break;

            default:
                filter = NULL;
                break;
            }
        }
        else
        {
            filter = NULL;
        }

        /* Initiate PAN Setup */
        LOG_DEBUG("Starting PAN Filter ... "); fflush (stdout);
        retval = BT_pan_filter
                 (
                     appl_pan_handle[indx].bd_addr,
                     local,
                     type,
                     num_range,
                     filter
                 );

        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("FAILED !! Reason = 0x%04X\n", retval);
        }
        else
        {
            LOG_DEBUG("SUCCEEDED !!\n");
            LOG_DEBUG("Wait for PAN_FILTER_CNF\n");
            LOG_DEBUG("Filter Data Pointer = %p\n", filter);
        }
    }

    return;
}


void appl_pan_disconnect ( void )
{
    int indx;
    API_RESULT retval;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    indx = 0;

    /* Choose Handle */
    appl_pan_get_handle (&indx);
    if (indx < 0)
    {
        LOG_DEBUG("Invalid PAN Handle\n");
        /* return; */
    }
    else
    {
        /* Initiate PAN Disconnect */
        LOG_DEBUG("Starting PAN Disconnect ... "); fflush(stdout);
        retval = BT_pan_disconnect(appl_pan_handle[indx].bd_addr);

        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("FAILED !! Reason = 0x%04X\n", retval);
        }
        else
        {
            LOG_DEBUG("SUCCEEDED !!\n");
            LOG_DEBUG("Wait for PAN_DISCONNECT_CNF\n");
        }
    }

    return;
}

/* Write data over PAN */
void appl_pan_write_data ( void )
{
    UCHAR * eth_payload;
    int read_val, i;
    UINT16 indx;
    API_RESULT retval;
    UINT16 eth_payload_len, offset;
    UINT16 protocol, protocol_802_1q;
    UCHAR eth_bd [BT_BD_ADDR_SIZE];
    UCHAR eth_header [PAN_ETH_HEADER_SIZE];

    /* Init */
    retval = API_SUCCESS;

    /* Choose Handle */
    retval = appl_pan_choose_acl_connection (&indx);
    if (API_SUCCESS != retval)
    {
        LOG_DEBUG("Invalid ACL Handle\n");
        /* return; */
    }
    else
    {
        LOG_DEBUG("Choose PAN Packet Type:\n");
        LOG_DEBUG("  [0] General Ethernet\n");
        LOG_DEBUG("  [1] Compressed Ethernet Source Only\n");
        LOG_DEBUG("  [2] Compressed Ethernet Dest Only\n");
        LOG_DEBUG("  [3] Compressed Ethernet\n");
        LOG_DEBUG("Your Choice = "); fflush(stdout);
        scanf("%d", &read_val);

        if (0U == read_val)
        {
            /* Get Ethernet Destination Address */
            LOG_DEBUG("Enter Ethernet Dst Address = "); fflush(stdout);
            appl_get_bd_addr (eth_header);

            /* Get Ethernet Source Address */
            LOG_DEBUG("Enter Ethernet Src Address = "); fflush(stdout);
            appl_get_bd_addr (&eth_header[6U]);
        }
        else if (1U == read_val)
        {
            /* Set Ethernet Destination Address */
            pan_convert_address (eth_bd, appl_pan_handle[indx].bd_addr);
            BT_mem_copy (eth_header, eth_bd, BT_BD_ADDR_SIZE);

            /* Get Ethernet Source Address */
            LOG_DEBUG("Enter Ethernet Src Address = "); fflush(stdout);
            appl_get_bd_addr (&eth_header[BT_BD_ADDR_SIZE]);
        }
        else if (2U == read_val)
        {
            /* Get Ethernet Destination Address */
            LOG_DEBUG("Enter Ethernet Dst Address = "); fflush(stdout);
            appl_get_bd_addr (eth_header);

            /* Set Ethernet Source Address */
            pan_convert_address (eth_bd, appl_pan_local_bd_addr);
            BT_mem_copy (&eth_header[BT_BD_ADDR_SIZE], eth_bd, BT_BD_ADDR_SIZE);
        }
        else if (3U == read_val)
        {
            /* Set Ethernet Destination Address */
            pan_convert_address (eth_bd, appl_pan_handle[indx].bd_addr);
            BT_mem_copy (eth_header, eth_bd, BT_BD_ADDR_SIZE);

            /* Set Ethernet Source Address */
            pan_convert_address (eth_bd, appl_pan_local_bd_addr);
            BT_mem_copy (&eth_header[BT_BD_ADDR_SIZE], eth_bd, BT_BD_ADDR_SIZE);
        }
        else
        {
            LOG_DEBUG("*** Invalid Choice - %d. Try again.\n", read_val);
            retval = API_FAILURE; /* return; */
        }

        if (API_SUCCESS == retval)
        {
            LOG_DEBUG("Send Echo Request: Yes/No (1/0) = "); fflush(stdout);
            scanf("%x", &read_val);

            if (0U == read_val)
            {
                LOG_DEBUG("Enter Ethernet Protocol (in Hex) = "); fflush(stdout);
                scanf("%x", &read_val);
                protocol = (UINT16)read_val;

                eth_header[12U] = (UCHAR)(protocol >> 8U);
                eth_header[13U] = (UCHAR)protocol;

                protocol_802_1q = 0x0U;
                if (PAN_ETH_PROTO_TYPE_802_1Q == protocol)
                {
                    LOG_DEBUG("Enter 802.1Q Ethernet Protocol (in Hex) = "); fflush(stdout);
                    scanf("%x", &read_val);
                    protocol_802_1q = (UINT16)read_val;
                }

                LOG_DEBUG("Enter Ethernet Payload Length = "); fflush(stdout);
                scanf("%d", &read_val);
                eth_payload_len = (UINT16)read_val;

                if (PAN_ETH_PROTO_TYPE_802_1Q == protocol)
                {
                    eth_payload_len += 4U;
                }

                eth_payload = (UCHAR *)BT_alloc_mem(eth_payload_len);
                if (NULL == eth_payload)
                {
                    LOG_DEBUG("*** FAILED to Allocate Memory for Ethernet Payload\n");
                    /* return; */
                }
                else
                {
                    offset = 0U;
                    if (PAN_ETH_PROTO_TYPE_802_1Q == protocol)
                    {
                        offset = 4U;
                        eth_payload[0U] = 0x0U;
                        eth_payload[1U] = 0x0U;
                        eth_payload[2U] = (UCHAR)(protocol_802_1q >> 8U);
                        eth_payload[3U] = (UCHAR)protocol_802_1q;
                    }

                    for (i = offset; i < eth_payload_len; i++)
                    {
                        eth_payload[i] = (UCHAR)i;
                    }
                }
            }
            else
            {
                /* IPv4 */
                protocol = 0x0800U;

                eth_header[12U] = (UCHAR)(protocol >> 8U);
                eth_header[13U] = (UCHAR)protocol;

                /* Use static Ping Packet */
                eth_payload_len = sizeof(appl_echo_req);

                eth_payload = BT_alloc_mem(eth_payload_len);

                if (NULL == eth_payload)
                {
                    LOG_DEBUG("*** FAILED to Allocate Memory for Ethernet Payload\n");
                    /* return; */
                }
                else
                {
                    /* Copy */
                    BT_mem_copy(eth_payload, appl_echo_req, eth_payload_len);
                }
            }

            /* Write to BNEP */
            LOG_DEBUG("Writing Data to PAN ... "); fflush(stdout);
            retval = BT_pan_write_panu
                     (
                         appl_pan_handle[indx].bd_addr,
                         eth_header,
                         NULL,
                         0U,
                         eth_payload,
                         eth_payload_len
                     );

            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("FAILED !! Reason = 0x%04X\n", retval);
            }
            else
            {
                LOG_DEBUG("SUCCEEDED !!\n");
            }

            /* Free Allocated Memory */
            if (NULL != eth_payload)
            {
                BT_free_mem(eth_payload);
            }
        }
    }

    return;
}


/* Write data over PAN */
void appl_pan_write_data_from_file(void)
{
    UCHAR * eth_payload;
    int read_val;
    UINT16 indx, actual;
    API_RESULT retval;
    UINT16 eth_payload_len, offset;
    UINT16 protocol, protocol_802_1q;
    UCHAR eth_bd[BT_BD_ADDR_SIZE];
    UCHAR eth_header[PAN_ETH_HEADER_SIZE];

    PAN_EXT_HEADER extn[APPL_PAN_MAX_EXTN_HDRS];
    UCHAR extn_count, i;

    BT_fops_file_handle fd;
    CHAR filename[128U];
    CHAR fileobject[256U];

    UCHAR flag_send_data;

    /* Init */
    retval = API_SUCCESS;

    /* Choose Handle */
    retval = appl_pan_choose_acl_connection(&indx);
    if (retval != API_SUCCESS)
    {
        LOG_DEBUG("Invalid ACL Handle \n");
        /* return; */
    }
    else
    {
        LOG_DEBUG("Choose PAN Packet Type:\n");
        LOG_DEBUG("  [0] General Ethernet\n");
        LOG_DEBUG("  [1] Compressed Ethernet Source Only\n");
        LOG_DEBUG("  [2] Compressed Ethernet Dest Only\n");
        LOG_DEBUG("  [3] Compressed Ethernet\n");
        LOG_DEBUG("Your Choice = "); fflush(stdout);
        scanf("%d", &read_val);

        if (0U == read_val)
        {
            /* Get Ethernet Destination Address */
            LOG_DEBUG("Enter Ethernet Dst Address = "); fflush(stdout);
            appl_get_bd_addr(eth_header);

            /* Get Ethernet Source Address */
            LOG_DEBUG("Enter Ethernet Src Address = "); fflush(stdout);
            appl_get_bd_addr(&eth_header[6U]);
        }
        else if (1U == read_val)
        {
            /* Set Ethernet Destination Address */
            pan_convert_address(eth_bd, appl_pan_handle[indx].bd_addr);
            BT_mem_copy(eth_header, eth_bd, BT_BD_ADDR_SIZE);

            /* Get Ethernet Source Address */
            LOG_DEBUG("Enter Ethernet Src Address = "); fflush(stdout);
            appl_get_bd_addr(&eth_header[BT_BD_ADDR_SIZE]);
        }
        else if (2U == read_val)
        {
            /* Get Ethernet Destination Address */
            LOG_DEBUG("Enter Ethernet Dst Address = "); fflush(stdout);
            appl_get_bd_addr(eth_header);

            /* Set Ethernet Source Address */
            pan_convert_address(eth_bd, appl_pan_local_bd_addr);
            BT_mem_copy(&eth_header[BT_BD_ADDR_SIZE], eth_bd, BT_BD_ADDR_SIZE);
        }
        else if (3U == read_val)
        {
            /* Set Ethernet Destination Address */
            pan_convert_address(eth_bd, appl_pan_handle[indx].bd_addr);
            BT_mem_copy(eth_header, eth_bd, BT_BD_ADDR_SIZE);

            /* Set Ethernet Source Address */
            pan_convert_address(eth_bd, appl_pan_local_bd_addr);
            BT_mem_copy(&eth_header[BT_BD_ADDR_SIZE], eth_bd, BT_BD_ADDR_SIZE);
        }
        else
        {
            LOG_DEBUG("*** Invalid Choice - %d. Try again.\n", read_val);
            retval = API_FAILURE; /* return; */
        }

        if (API_SUCCESS == retval)
        {
            LOG_DEBUG("Enter Ethernet Protocol (in Hex) = "); fflush(stdout);
            scanf("%x", &read_val);
            protocol = (UINT16)read_val;

            eth_header[12U] = (UCHAR)(protocol >> 8U);
            eth_header[13U] = (UCHAR)protocol;

            protocol_802_1q = 0x0U;
            if (PAN_ETH_PROTO_TYPE_802_1Q == protocol)
            {
                LOG_DEBUG("Enter 802.1Q Ethernet Protocol (in Hex) = "); fflush(stdout);
                scanf("%x", &read_val);
                protocol_802_1q = (UINT16)read_val;
            }

            LOG_DEBUG("Have Extension? (1/0): ");
            scanf("%d", &read_val);

            if (0U != read_val)
            {
                LOG_DEBUG("Enter Number of headers (Max %d): ", APPL_PAN_MAX_EXTN_HDRS);
                scanf("%d", &read_val);
                extn_count = (UCHAR)read_val;

                for (i = 0U; i < extn_count; i++)
                {
                    LOG_DEBUG("Enter Extn %d type (Hex): ", i);
                    scanf("%x", &read_val);
                    extn[i].type = (UCHAR)read_val;

                    LOG_DEBUG("Enter Extn %d length: ", i);
                    scanf("%d", &read_val);
                    extn[i].length = (UCHAR)read_val;

                    extn[i].value = BT_alloc_mem(extn[i].length);

                    if (0x00U == extn[i].type)
                    {
                        extn[i].value[0U] = 0x55U;
                        BT_mem_set((extn[i].value + 1U), 0xFFU, (extn[i].length - 1U));
                    }
                    else if (0x55U == extn[i].type)
                    {
                        for (read_val = 0U; read_val < extn[i].length; read_val++)
                        {
                            extn[i].value[read_val] = (UCHAR)read_val;
                        }
                    }
                    else if (0x7FU == extn[i].type)
                    {
                        for (read_val = extn[i].length; read_val > 0U; read_val--)
                        {
                            extn[i].value[extn[i].length - read_val] = (UCHAR)read_val;
                        }
                    }
                    else
                    {
                        /* MISRA C-2012 Rule 15.7 */
                    }
                }
            }
            else
            {
                extn_count = 0U;
            }

            LOG_DEBUG("Enter Payload File Name: ");
            scanf("%s", filename);

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_vfops_create_object_name
            (
                (UCHAR *)PAN_ROOT_FOLDER_BASE,
                (UCHAR *)filename,
                (UCHAR *)fileobject
            );

            LOG_DEBUG("Getting Payload from file..."); fflush(stdout);

            /* Set send data flag as true, by default */
            flag_send_data = BT_TRUE;

            retval = BT_fops_file_open ((UCHAR *)fileobject, (UCHAR *)"rb", &fd);
            if ((API_SUCCESS != retval) || (NULL == fd))
            {
                LOG_DEBUG("Failed to open file\n");

                if ((PAN_ETH_PROTO_TYPE_802_1Q == protocol) &&
                    (0U != protocol_802_1q))
                {
                    eth_payload_len = 4U;
                    eth_payload = (UCHAR *)BT_alloc_mem(eth_payload_len);

                    eth_payload[0U] = 0x0U;
                    eth_payload[1U] = 0x0U;
                    eth_payload[2U] = (UCHAR)(protocol_802_1q >> 8U);
                    eth_payload[3U] = (UCHAR)protocol_802_1q;
                }
                else
                {
                    eth_payload_len = 0U;
                    eth_payload = NULL;
                }
            }
            else
            {
                /* Get payload size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size (fd, (UINT32 *)&read_val);
                eth_payload_len = (UINT16)read_val;

                if (PAN_ETH_PROTO_TYPE_802_1Q == protocol)
                {
                    eth_payload_len += 4U;
                }

                eth_payload = (UCHAR *)BT_alloc_mem(eth_payload_len);
                if (NULL == eth_payload)
                {
                    LOG_DEBUG("*** FAILED to Allocate Memory for Ethernet Payload\n");

                    /* Set send data flag as false */
                    flag_send_data = BT_FALSE;
                }
                else
                {
                    offset = 0U;
                    if (PAN_ETH_PROTO_TYPE_802_1Q == protocol)
                    {
                        offset = 4U;
                        eth_payload[0U] = 0x0U;
                        eth_payload[1U] = 0x0U;
                        eth_payload[2U] = (UCHAR)(protocol_802_1q >> 8U);
                        eth_payload[3U] = (UCHAR)protocol_802_1q;
                    }

                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_read(&eth_payload[offset], (eth_payload_len - offset), fd, (UINT16 *)&actual);
                }

                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(fd);
            }

            /* Write to PAN/BNEP */
            if (BT_FALSE != flag_send_data)
            {
                LOG_DEBUG("Writing Data to PAN ... "); fflush(stdout);
                retval = BT_pan_write_panu
                         (
                             appl_pan_handle[indx].bd_addr,
                             eth_header,
                             extn,
                             extn_count,
                             eth_payload,
                             eth_payload_len
                         );

                if (retval != API_SUCCESS)
                {
                    LOG_DEBUG("FAILED !! Reason = 0x%04X\n", retval);
                }
                else
                {
                    LOG_DEBUG("SUCCEEDED !!\n");
                }
            }

            /* Free Allocated Memory */
            for (i = 0U; i < extn_count; i++)
            {
                BT_free_mem(extn[i].value);
            }

            if (NULL != eth_payload)
            {
                BT_free_mem(eth_payload);
            }
        }
    }

    return;
}

/* Enable/disable header compression */
void appl_pan_hdr_comression(void)
{
    int read_val;
    UCHAR option;

    /* Choose Enable/Disable */
    LOG_DEBUG("Choose header compression Enable/Disable (1/0):\n");
    LOG_DEBUG("Your Choice = "); fflush(stdout);
    scanf("%d", &read_val);

    if (0U == read_val)
    {
        option = (UCHAR)BT_FALSE;
    }
    else
    {
        option = (UCHAR)BT_TRUE;
    }

    /* Set header compression state */
    BT_pan_set_compression_state(option);

    return;
}

/* Callback Handler for PAN_EVENT_IND_CB */
API_RESULT appl_pan_event_ind
           (
               UCHAR      event_type,
               UCHAR *    bd_addr,
               UINT16     event_result,
               void *     event_data,
               UINT16     event_data_len
           )
{
    int indx;
    PAN_SETUP * pan_setup;
    PAN_FILTER * pan_filter;
    API_RESULT retval;

    BT_IGNORE_UNUSED_PARAM(event_data_len);

    switch (event_type)
    {
    case PAN_CONNECT_IND:
        LOG_DEBUG("Received PAN_CONNECT_IND\n");
        LOG_DEBUG("    BD_ADDR: %02X:%02X:%02X:%02X:%02X:%02X\n",
        bd_addr[0U], bd_addr[1U], bd_addr[2U], bd_addr[3U], bd_addr[4U], bd_addr[5U]);
        LOG_DEBUG("    Event Result = 0x%04X\n", event_result);

        /* Allocate PAN Handle */
        retval = appl_pan_alloc_bd_addr (&indx);
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("*** FAILED to Allocate PAN Entity for BD_ADDR\n");
            break;
        }
        else
        {
            /* Save BD_ADDR */
            BT_COPY_BD_ADDR (appl_pan_handle[indx].bd_addr, bd_addr);
        }

        break;

    case PAN_CONNECT_CNF:
        LOG_DEBUG("Received PAN_CONNECT_CNF\n");
        LOG_DEBUG("    BD_ADDR: %02X:%02X:%02X:%02X:%02X:%02X\n",
        bd_addr[0U], bd_addr[1U], bd_addr[2U], bd_addr[3U], bd_addr[4U], bd_addr[5U]);
        LOG_DEBUG("    Event Result = 0x%04X\n", event_result);

        /* Search PAN Handle */
        retval = appl_pan_search_bd_addr (bd_addr, &indx);
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("*** FAILED to Get PAN Entity for BD_ADDR\n");
            break;
        }

        break;

    case PAN_SETUP_IND:
        LOG_DEBUG("Received PAN_SETUP_IND\n");
        LOG_DEBUG("    BD_ADDR: %02X:%02X:%02X:%02X:%02X:%02X\n",
        bd_addr[0U], bd_addr[1U], bd_addr[2U], bd_addr[3U], bd_addr[4U], bd_addr[5U]);
        LOG_DEBUG("    Event Result = 0x%04X\n", event_result);

        /* Get PAN_SETUP */
        pan_setup = (PAN_SETUP *) event_data;

        LOG_DEBUG("    Setup: Local = 0x%02X, Remote = 0x%02X, Flag 0x%02X\n",
        pan_setup->local_service, pan_setup->remote_service,
        pan_setup->flag);

        /* Search PAN Handle */
        retval = appl_pan_search_bd_addr (bd_addr, &indx);
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("*** FAILED to Get PAN Entity for BD_ADDR\n");
            break;
        }

        LOG_DEBUG("    Current: Local = 0x%02X, Remote = 0x%02X\n",
        appl_pan_handle[indx].local_service,
        appl_pan_handle[indx].remote_service);

        /* Is Response Required ? */
        if (0x01U == pan_setup->flag)
        {
            /* Update Local & Remote Service */
            if (PAN_SETUP_SUCCESSFUL == appl_pan_setup_ind_reply)
            {
                appl_pan_handle[indx].local_service =
                                       pan_setup->local_service;
                appl_pan_handle[indx].remote_service =
                                       pan_setup->remote_service;
            }

            /* Send PAN Setup RSP */
            LOG_DEBUG("Starting PAN Setup ... "); fflush (stdout);
            retval = BT_pan_setup_rsp
                     (
                         appl_pan_handle[indx].bd_addr,
                         appl_pan_setup_ind_reply
                     );

            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("FAILED !! Reason = 0x%04X\n", retval);
                break;
            }
            else
            {
                LOG_DEBUG("SUCCEEDED !!\n");
            }
        }
        else
        {
            appl_pan_handle[indx].local_service = pan_setup->local_service;
            appl_pan_handle[indx].remote_service = pan_setup->remote_service;
        }

        /* Bring up NAP Network Interface */
        if (PAN_SERVICE_NAP == appl_pan_handle[indx].local_service)
        {
            /* TODO: Configure and UP the network interface */
        }

        /* Start the platform interface */
        panc_bd_addr = appl_pan_handle[indx].bd_addr;
        pan_start_pl(appl_have_dhcp);

        break;

    case PAN_SETUP_CNF:
        LOG_DEBUG("Received PAN_SETUP_CNF\n");
        LOG_DEBUG("    BD_ADDR: %02X:%02X:%02X:%02X:%02X:%02X\n",
        bd_addr[0U], bd_addr[1U], bd_addr[2U], bd_addr[3U], bd_addr[4U], bd_addr[5U]);
        LOG_DEBUG("    Event Result = 0x%04X\n", event_result);

        /* Get PAN_SETUP */
        pan_setup = (PAN_SETUP *) event_data;

        LOG_DEBUG("    Setup: Local = 0x%02X, Remote = 0x%02X, Flag 0x%02X\n",
        pan_setup->local_service, pan_setup->remote_service,
        pan_setup->flag);

        /* Search PAN Handle */
        retval = appl_pan_search_bd_addr (bd_addr, &indx);
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("*** FAILED to Get PAN Entity for BD_ADDR\n");
            break;
        }

        LOG_DEBUG("    Current: Local = 0x%02X, Remote = 0x%02X\n",
        appl_pan_handle[indx].local_service,
        appl_pan_handle[indx].remote_service);

        /* Is Setup Successful ? */
        if (API_SUCCESS == event_result)
        {
            appl_pan_handle[indx].local_service = pan_setup->local_service;
            appl_pan_handle[indx].remote_service = pan_setup->remote_service;

            /* Start the platform interface */
            panc_bd_addr = appl_pan_handle[indx].bd_addr;
            pan_start_pl(appl_have_dhcp);
        }

        break;

    case PAN_DISCONNECT_IND:
        LOG_DEBUG("Received PAN_DISCONNECT_IND\n");
        LOG_DEBUG("    BD_ADDR: %02X:%02X:%02X:%02X:%02X:%02X\n",
        bd_addr[0U], bd_addr[1U], bd_addr[2U], bd_addr[3U], bd_addr[4U], bd_addr[5U]);
        LOG_DEBUG("    Event Result = 0x%04X\n", event_result);

        /* Search PAN Handle */
        retval = appl_pan_search_bd_addr (bd_addr, &indx);
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("*** FAILED to Get PAN Entity for BD_ADDR\n");
            break;
        }

        BT_mem_set(appl_pan_handle[indx].bd_addr, 0x0, BT_BD_ADDR_SIZE);
        appl_pan_handle[indx].local_service = PAN_SERVICE_NONE;
        appl_pan_handle[indx].remote_service = PAN_SERVICE_NONE;
        appl_pan_handle[indx].allocated = 0x0U;

        /* Stop the platform interface */
        pan_stop_pl();

        break;

    case PAN_DISCONNECT_CNF:
        LOG_DEBUG("Received PAN_DISCONNECT_CNF\n");
        LOG_DEBUG("    BD_ADDR: %02X:%02X:%02X:%02X:%02X:%02X\n",
        bd_addr[0U], bd_addr[1U], bd_addr[2U], bd_addr[3U], bd_addr[4U], bd_addr[5U]);
        LOG_DEBUG("    Event Result = 0x%04X\n", event_result);

        /* Search PAN Handle */
        retval = appl_pan_search_bd_addr (bd_addr, &indx);
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("*** FAILED to Get PAN Entity for BD_ADDR\n");
            break;
        }

        BT_mem_set(appl_pan_handle[indx].bd_addr, 0x0, BT_BD_ADDR_SIZE);
        appl_pan_handle[indx].local_service = PAN_SERVICE_NONE;
        appl_pan_handle[indx].remote_service = PAN_SERVICE_NONE;
        appl_pan_handle[indx].allocated = 0x0U;

        /* Stop the platform interface */
        pan_stop_pl();

        break;

    case PAN_FILTER_IND:
        LOG_DEBUG("Received PAN_FILTER_IND\n");
        LOG_DEBUG("    BD_ADDR: %02X:%02X:%02X:%02X:%02X:%02X\n",
        bd_addr[0U], bd_addr[1U], bd_addr[2U], bd_addr[3U], bd_addr[4U], bd_addr[5U]);
        LOG_DEBUG("    Event Result = 0x%04X\n", event_result);

        /* Get PAN_FILTER */
        pan_filter = (PAN_FILTER *) event_data;

        LOG_DEBUG("    Filter Type = 0x%02X\n", pan_filter->type);
        LOG_DEBUG("    Number of Filter Range = %d\n", pan_filter->num_range);
        if (pan_filter->num_range > 0U)
        {
            if (PAN_FILTER_TYPE_NW_TYPE == pan_filter->type)
            {
                LOG_DEBUG("    Filter (NW Type) Data: "); fflush (stdout);
                appl_dump_bytes
                (
                    (UCHAR *) pan_filter->filter,
                    pan_filter->num_range * 4U
                );
            }
            else if (PAN_FILTER_TYPE_MC_ADDR == pan_filter->type)
            {
                LOG_DEBUG("    Filter (MC Addr) Data: "); fflush (stdout);
                appl_dump_bytes
                (
                    (UCHAR *) pan_filter->filter,
                    pan_filter->num_range * 12U
                );
            }
            else
            {
                LOG_DEBUG("*** Unknown Filter Type - 0x%02X\n",
                pan_filter->type);
            }
        }

        break;

    case PAN_FILTER_CNF:
        LOG_DEBUG("Received PAN_FILTER_CNF\n");
        LOG_DEBUG("    BD_ADDR: %02X:%02X:%02X:%02X:%02X:%02X\n",
        bd_addr[0U], bd_addr[1U], bd_addr[2U], bd_addr[3U], bd_addr[4U], bd_addr[5U]);
        LOG_DEBUG("    Event Result = 0x%04X\n", event_result);

        /* Get PAN_FILTER */
        pan_filter = (PAN_FILTER *) event_data;

        LOG_DEBUG("    Filter Type = 0x%02X\n", pan_filter->type);
        LOG_DEBUG("    Number of Filter Range = %d\n", pan_filter->num_range);
        if (pan_filter->num_range > 0U)
        {
            LOG_DEBUG("    Filter Data: %p\n", pan_filter->filter);
            if (NULL != pan_filter->filter)
            {
                BT_free_mem (pan_filter->filter);
            }
            else
            {
                LOG_DEBUG("*** Expected Filter Data Pointer. NOT Returned\n");
            }
        }
        else
        {
            if (NULL != pan_filter->filter)
            {
                LOG_DEBUG("*** NOT Expected Filter Data Pointer. Num Range 0!\n");
            }
        }

        break;

    case PAN_WRITE_CNF:
#if 0
        LOG_DEBUG("Received PAN_WRITE_CNF\n");
        LOG_DEBUG("    BD_ADDR: %02X:%02X:%02X:%02X:%02X:%02X\n",
        bd_addr[0], bd_addr[1], bd_addr[2], bd_addr[3], bd_addr[4], bd_addr[5]);
        LOG_DEBUG("    Event Result = 0x%04X\n", event_result);
#endif /* 0 */

        break;

    default:
        LOG_DEBUG("Received ???? Unknwon PAN Event IND: 0x%02X\n",
        event_type);

        /* return API_SUCCESS; */
    }

    return API_SUCCESS;
}


API_RESULT appl_pan_read_ind
           (
               UCHAR      local_service,
               UCHAR *    bd_addr,
               UCHAR *    eth_header,
               UCHAR *    eth_payload,
               UINT16     eth_payload_len
           )
{
    UINT16 eth_protocol, offset;

#if 1
    LOG_DEBUG("\n");
    LOG_DEBUG("Received PAN_READ_IND. Local Service 0x%02X\n", local_service);

    LOG_DEBUG("    BD_ADDR: %02X:%02X:%02X:%02X:%02X:%02X\n",
    bd_addr[0U], bd_addr[1U], bd_addr[2U], bd_addr[3U], bd_addr[4U], bd_addr[5U]);

    LOG_DEBUG("    Eth Dst Addr: %02X:%02X:%02X:%02X:%02X:%02X\n",
    eth_header[0U], eth_header[1U], eth_header[2U],
    eth_header[3U], eth_header[4U], eth_header[5U]);

    LOG_DEBUG("    Eth Src Addr: %02X:%02X:%02X:%02X:%02X:%02X\n",
    eth_header[6U], eth_header[7U], eth_header[8U],
    eth_header[9U], eth_header[10U], eth_header[11U]);

    BT_UNPACK_BE_2_BYTE(&eth_protocol, &eth_header[12U]);

    LOG_DEBUG("    Eth Protocol: %02X %02X (0x%04X, %d)\n",
    eth_header[12U], eth_header[13U], eth_protocol, eth_protocol);

    LOG_DEBUG("    Eth Header: ");
    appl_dump_bytes(eth_header, 14U);

    LOG_DEBUG("    Payload Len : %d Octets\n", eth_payload_len);
    appl_dump_bytes (eth_payload, eth_payload_len);
#else
    BT_IGNORE_UNUSED_PARAM(local_service);
    BT_IGNORE_UNUSED_PARAM(bd_addr);
#endif /* 0 */

    eth_protocol  = eth_header[12U];
    eth_protocol  = eth_protocol << 8U;
    eth_protocol |= eth_header[13U];

    offset = 0U;
    if (BNEP_ETH_PROTO_TYPE_802_1Q == eth_protocol)
    {
        offset = 4U;

        eth_protocol  = eth_payload[2U];
        eth_protocol  = eth_protocol << 8U;
        eth_protocol |= eth_payload[3U];

        LOG_DEBUG("    Eth 802.1Q Protocol: %02X %02X (0x%04X, %d)\n",
        eth_payload[2U], eth_payload[3U], eth_protocol, eth_protocol);
    }

    /* Write to platform interface */
    pan_write_pl(eth_header, eth_payload, eth_payload_len);

    BT_IGNORE_UNUSED_PARAM(offset);

    return API_SUCCESS;
}

static void pan_pan_read(UCHAR *edata, UINT16 edatalen)
{
    UCHAR * eth_header;
    UCHAR * eth_payload;
    UINT16 eth_payloadlen;

    eth_header = (UCHAR *)edata;
    eth_payload = (&((UCHAR *)edata)[14U]);
    eth_payloadlen = (UINT16)(edatalen - 14U);

    (BT_IGNORE_RETURN_VALUE) BT_pan_write_panu
    (
        panc_bd_addr,
        eth_header,
        NULL, 0U,
        eth_payload,
        eth_payloadlen
    );
}

API_RESULT appl_pan_alloc_bd_addr (int * indx)
{
    int count;
    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;

    for (count = 0U; count < PAN_MAX_ENTITIES; count ++)
    {
        if (0x0U == appl_pan_handle[count].allocated)
        {
            appl_pan_handle[count].allocated = 0x1U;
            *indx = count;
            break; /* return API_SUCCESS; */
        }
    }

    if (PAN_MAX_ENTITIES == count)
    {
        retval = API_FAILURE;
    }

    return retval;
}


API_RESULT appl_pan_search_bd_addr (UCHAR * bd_addr, int * indx)
{
    int count;
    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;

    for (count = 0U; count < PAN_MAX_ENTITIES; count ++)
    {
        if ((0x1U == appl_pan_handle[count].allocated) &&
            (0 == BT_mem_cmp(appl_pan_handle[count].bd_addr, bd_addr, 6U)))
        {
            *indx = count;
            break; /* return API_SUCCESS; */
        }
    }

    if (PAN_MAX_ENTITIES == count)
    {
        retval = API_FAILURE;
    }

    return retval;
}


/* To Display & Choose a PAN Connection Handle */
void appl_pan_get_handle ( int * indx )
{
    /* Display Available PAN Handles */
    appl_pan_display_handles();

    BT_LOOP_FOREVER()
    {
        LOG_DEBUG("Choose PAN Handle Index = "); fflush(stdout);
        scanf("%d", indx);
        if (((*indx) >= 0) && ((*indx) < PAN_MAX_ENTITIES))
        {
            break;
        }
    }

    return;
}


/* To Display PAN Connection Handles */
void appl_pan_display_handles ( void )
{
    int indx;

    LOG_DEBUG("\n");
    LOG_DEBUG("Index  Remote BD_ADDR     Local  Remote\n");
    LOG_DEBUG("-----  -----------------  -----  ------\n");

    for (indx = 0U; indx < PAN_MAX_ENTITIES; indx ++)
    {
        if (0x0U == appl_pan_handle[indx].allocated)
        {
            LOG_DEBUG("%3d    NOT ALLOCATED\n", indx);
            continue;
        }

        LOG_DEBUG("%3d    ", indx);
        LOG_DEBUG("%02X:%02X:%02X:%02X:%02X:%02X  ",
        appl_pan_handle[indx].bd_addr[0U], appl_pan_handle[indx].bd_addr[1U],
        appl_pan_handle[indx].bd_addr[2U], appl_pan_handle[indx].bd_addr[3U],
        appl_pan_handle[indx].bd_addr[4U], appl_pan_handle[indx].bd_addr[5U]);
        switch (appl_pan_handle[indx].local_service)
        {
        case PAN_SERVICE_NONE:
            LOG_DEBUG("N/A    ");
            break;
        case PAN_SERVICE_PANU:
            LOG_DEBUG("PANU   ");
            break;
        case PAN_SERVICE_NAP:
            LOG_DEBUG("NAP    ");
            break;
        case PAN_SERVICE_GN:
            LOG_DEBUG("GN     ");
            break;
        default:
            LOG_DEBUG("?????  ");
            break;
        }

        switch (appl_pan_handle[indx].remote_service)
        {
        case PAN_SERVICE_NONE:
            LOG_DEBUG("N/A   ");
            break;
        case PAN_SERVICE_PANU:
            LOG_DEBUG("PANU  ");
            break;
        case PAN_SERVICE_NAP:
            LOG_DEBUG("NAP   ");
            break;
        case PAN_SERVICE_GN:
            LOG_DEBUG("GN    ");
            break;
        default:
            LOG_DEBUG("??????");
            break;
        }
        LOG_DEBUG("\n");
    }

    LOG_DEBUG("\n");
}


API_RESULT appl_pan_choose_acl_connection (UINT16 * indx)
{
    int choice;
    API_RESULT retval;
    UCHAR i, num_list;

    /* Init */
    retval = API_SUCCESS;

    LOG_DEBUG("Getting Connection Details from HCI ... "); fflush(stdout);
    retval = BT_hci_get_connection_details
             (appl_pan_acl_list, BT_MAX_REMOTE_DEVICES, &num_list);

    if (API_SUCCESS != retval)
    {
         LOG_DEBUG("FAILED !! Error Code = 0x%04X\n", retval);
         retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
        LOG_DEBUG("Succeeded.\n");

        if (0U == num_list)
        {
            retval = API_FAILURE; /* return API_FAILURE; */
        }
        else
        {
            LOG_DEBUG("\n");
            LOG_DEBUG("==========================================================\n");
            LOG_DEBUG("Active ACL Connections\n");
            LOG_DEBUG("----------------------------------------------------------\n");
            for (i = 0U; i < num_list; i++)
            {
                LOG_DEBUG("  [%2d]  ", i);
                LOG_DEBUG("BD_ADDR %02X:%02X:%02X:%02X:%02X:%02X  ",
                    appl_pan_acl_list[i].bd_addr[0U], appl_pan_acl_list[i].bd_addr[1U],
                    appl_pan_acl_list[i].bd_addr[2U], appl_pan_acl_list[i].bd_addr[3U],
                    appl_pan_acl_list[i].bd_addr[4U], appl_pan_acl_list[i].bd_addr[5U]);
                LOG_DEBUG("ACL Handle = 0x%04X\n", appl_pan_acl_list[i].acl_handle);
            }
            LOG_DEBUG("==========================================================\n");
            LOG_DEBUG("\n");
        }
    }

    if (API_SUCCESS == retval)
    {
        LOG_DEBUG("Choose ACL Connection Index = "); fflush(stdout);
        scanf("%d", &choice);
        if ((UINT16)choice >= BT_MAX_REMOTE_DEVICES)
        {
            retval = API_FAILURE; /* return API_FAILURE; */
        }
        else
        {
            *indx = (UINT16)choice;
        }
    }

    return retval;
}

#endif /* PAN */

