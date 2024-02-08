/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 */

#include "test_dhcp6.h"

#include "lwip/netif.h"
#include "lwip/dhcp6.h"
#include "lwip/nd6.h"
#include "lwip/prot/dhcp6.h"
#include "lwip/etharp.h"
#include "netif/ethernet.h"
#include "lwip/dns.h"
#include "lwip/apps/sntp.h"

static const uint16_t OPT_NOT_FOUND = 0xffffU;
static const uint16_t IP6_WRONG_ZONE = 0xfaU;

enum pkt_opts {
    OPT_CLI_ID = 1,
    OPT_SRV_ID = 1<<1,
    OPT_IA_PD  = 1<<2,
    OPT_IA_NA  = 1<<3,
    OPT_DNS_SRV = 1<<4,
    OPT_DOMAIN_SRCH_LST = 1<<5,
    OPT_SNTP_SRV = 1<<6,
    OPT_IA_PD_0  = 1<<7,
    OPT_IA_NA_0  = 1<<8
};

enum dhcp_msg_type {
    DHCP_MSG_ADVERTISE = 2,
    DHCP_MSG_REPLY = 7
};

static const unsigned pkt_ip_len_pos = 18;
static const unsigned pkt_udp_len_pos = 58;

static uint8_t pkt_headers[] = {
        /* 00 */0x00,0x23,0xc1,0xde,0xd0,0x0d, /*dst mac*/
        /* 06 */0x08,0x00,0x27,0xd4,0x10,0xbb, /*src mac*/
        /* 12 */0x86,0xdd, /*type ipv6*/
        /* 14 */0x60,0x00,0x00,0x00,
        /* 18 */0x00,0x5d, /* IPv6 payload length */
        /* 20 */0x11,0x40,
        /* 22 */0xfe,0x80,0x00,0x00, 0x00,0x00,0x00,0x00, 0x0a,0x00,0x27,0xff, 0xfe,0xd4,0x10,0xbb, /*src ip*/
        /* 38 */0xfe,0x80,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x23, 0xc1,0xde,0xd0,0x0d, /*dst ip*/
        /* 54 */0x02,0x23, 0x02,0x22, /* src, dst ports*/
        /* 58 */0x00,0x5d, /* UDP packet length including UDP header */
        /* 60 *//*0xa1,0x3d,*/ 0x00,0x00, /* UDP chksum 0 = no checksum */
};

static uint8_t pkt_dhcp_advertise[] = {
        0x02
};

static uint8_t pkt_dhcp_reply[] = {
        0x07
};

static uint8_t pkt_xid[] = { /* Transaction id */
        0x00, 0x00, 0x00
};

static uint8_t pkt_server_id[] = {
        0x00, 0x02, 0x00, 0x0e, 0x00, 0x01, 0x00, 0x01, 0x1c, 0x38, 0x25, 0xe8, 0x08, 0x00, 0x27, 0xd4, 0x10,
        0xbb
};

#if LWIP_IPV6_DHCP6_STATEFUL
static const uint8_t pkt_client_id_custom_duid[] = {
        0x00, 0x04, 0xac, 0x26, 0x6e, 0xf2, 0xfd, 0x37,
        0xa9, 0x85, 0x61, 0x0c, 0x57, 0x05, 0x92, 0x59,
        0x1a, 0x4c
};
#endif



static unsigned pkt_client_id_len = 0;
static uint8_t pkt_client_id[128];

static const unsigned pkt_iaid_pos = 4;

static uint8_t pkt_ia_pd[] = {
        0x00,0x19,0x00,0x29,
        0x27,0xfe,0x8f,0x95, /*IAID*/
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

        /*IA Prefix:*/
        0x00,0x1a,0x00,0x19,
        0x00,0x00,0x11,0x94, /* Preferred time  4500*/
        0x00,0x00,0x1c,0x20, /* Valid time 7200 */
        0x40, /*Prefix length 64*/
        0x20,0x01, 0x00,0x00, 0x00,0x00, 0xfe,0x00, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* 2001:0000:0000:fe00::*/
};

static uint8_t pkt_ia_pd_0[] = {
        0x00,0x19,0x00,0x29,
        0x27,0xfe,0x8f,0x95, /*IAID*/
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

        /*IA Prefix:*/
        0x00,0x1a,0x00,0x19,
        0x00,0x00,0x00,0x00, /* Preferred time 0*/
        0x00,0x00,0x00,0x00, /* Valid time 0 == request refused*/
        0x40, /*Prefix length 64*/
        0x20,0x01, 0x00,0x00, 0x00,0x00, 0xfe,0x00, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* 2001:0000:0000:fe00::*/
};

static uint8_t pkt_ia_na[] = {
        0x00,0x03,0x00,0x28,
        0x1d,0x00,0xfc,0xea, /* IAID */
        0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, /* T1, T2 */

        /*IA address:*/
        0x00,0x05, 0x00,0x18,
        0x20,0x01, 0x0d,0xba, 0x01,0xA0, 0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8, 0xA9,0xA0,
        /* 2001:0dba:01a0:a1a2:a3a4:a5a6:a7a8:a9a0 */

        0x00,0x00,0x01,0x77, /* Preferred time 375s */
        0x00,0x00,0x02,0x58  /* Valid time 600 */
};

static uint8_t pkt_ia_na_0[] = {
        0x00,0x03,0x00,0x28,
        0x1d,0x00,0xfc,0xea, /* IAID */
        0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, /* T1, T2 */

        /*IA address:*/
        0x00,0x05, 0x00,0x18,
        0x20,0x01, 0x0d,0xba, 0x01,0xA0, 0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8, 0xA9,0xA0,
        /* 2001:0dba:01a0:a1a2:a3a4:a5a6:a7a8:a9a0 */

        0x00,0x00,0x00,0x00, /* Preferred time 0s */
        0x00,0x00,0x00,0x00  /* Valid time 0 == request refused */
};

static uint8_t pkt_dns_srv[] = {
        0x00, 0x17, 0x00, 0x10, 0x20, 0x01, 0x0d, 0xba,
        0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x01
};

static uint8_t pkt_domain_srch_lst[] = {
        0x00, 0x18, 0x00, 0x0d,
        0x05,
        0x63, 0x69, 0x73, 0x63, 0x6f, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* cisco.local. */
        0x00
};

static uint8_t pkt_sntp_srv[] = {
        0x00, 0x1F, 0x00, 0x20,
        0x20, 0x01, 0x0d, 0xba, 0x01, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
        0x20, 0x01, 0x0d, 0xba, 0x01, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03
};

extern u32_t lwip_sys_now;

static struct netif net_test;

static int allow_override_client_id = 0;

static uint32_t elapsed_ms = 0;

static void tick_500ms_lwip(void)
{
  lwip_sys_now += 500;
  if (lwip_sys_now % 1000 == 0) {
    nd6_tmr();  /* call every 1s */
  }
  dhcp6_tmr(); /* call every 500ms */

}

void dhcp6_oro_hook(const struct netif * netif, u16_t type, uint8_t state, const struct pbuf * p, uint16_t offset, uint16_t opt_len);
void dhcp6_oro_hook(const struct netif * netif, u16_t type, uint8_t state, const struct pbuf * p, uint16_t offset, uint16_t opt_len) {
  (void)state;

  opt_len += 4; /* Add header size */

  fail_if(netif != &net_test);

  fail_if(type != 24, "dhcp6_oro_hook: expected option type 24 got %u", type);

  fail_if(opt_len != sizeof(pkt_domain_srch_lst), "dhcp6_oro_hook: opt_len (%u) != (%u) sizeof(pkt_domain_srch_lst)",
          opt_len, (uint16_t)sizeof(pkt_domain_srch_lst));

  fail_if(pbuf_memcmp(p, offset-4, pkt_domain_srch_lst, opt_len));
}

static err_t lwip_tx_func(struct netif *netif, struct pbuf *p);
static err_t output_ip6_chk_no_send(struct netif *netif, struct pbuf *p, const ip6_addr_t *ipaddr);
static void reset_client_id(void);

static const char* msg_type_to_str(unsigned type)
{
  static char buf[32];

  switch(type) {
    case 1:   return "1 (SOLICIT)";
    case 2:   return "2 (ADVERTISE)";
    case 3:   return "3 (REQUEST)";
    case 4:   return "4 (CONFIRM)";
    case 5:   return "5 (RENEW)";
    case 6:   return "6 (REBIND)";
    case 7:   return "7 (REPLY)";
    case 8:   return "8 (RELEASE)";
    case 9:   return "9 (DECLINE)";
    case 10:  return "10 (RECONFIGURE)";
    case 11:  return "11 (INFOREQUEST)";
    case 12:  return "12 (RELAYFORW)";
    case 13:  return "13 (RELAYREPL)";
    default:
      snprintf(buf, sizeof(buf), "%u (unknown)", type);
      return buf;
  }
}

static const char* addr_state_to_str(uint8_t state)
{
  if (ip6_addr_isinvalid(state)) {
    return "invalid";
  } else if (ip6_addr_ispreferred(state)) {
    return "valid preferred";
  } else if (ip6_addr_isdeprecated(state)) {
    return "valid deprecated";
  } else if (ip6_addr_istentative(state)) {
    return "tentative";
  } else if (ip6_addr_isduplicated(state)) {
    return "duplicated";
  } else {
    static char buf[32];
    snprintf(buf, sizeof(buf), "unhandled state 0x%x", state);
    return buf;
  }
}

static void reset_dns_servers(void)
{
  int i;

  ip_addr_t wrong_addr = ip_addr_any;
  wrong_addr.u_addr.ip6.zone = IP6_WRONG_ZONE;

  for(i=0; i<LWIP_DHCP6_MAX_DNS_SERVERS; i++) {
    dns_setserver(i, &wrong_addr);
  }
}

static void reset_sntp_servers(void)
{
  int i;

  ip_addr_t wrong_addr = ip_addr_any;
  wrong_addr.u_addr.ip6.zone = IP6_WRONG_ZONE;

  sntp_servermode_dhcp(1);

  for(i=0; i<SNTP_MAX_SERVERS; i++) {
    sntp_setserver(i, &wrong_addr);
  }
}



static err_t testif_init(struct netif *netif)
{
  netif->name[0] = 'c';
  netif->name[1] = 'h';
  netif->output = etharp_output;
  netif->linkoutput = lwip_tx_func;
  netif->output_ip6 = output_ip6_chk_no_send;
  netif->mtu = 1500;
  netif->hwaddr_len = 6;
  netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;

  netif->hwaddr[0] = 0x00;
  netif->hwaddr[1] = 0x23;
  netif->hwaddr[2] = 0xC1;
  netif->hwaddr[3] = 0xDE;
  netif->hwaddr[4] = 0xD0;
  netif->hwaddr[5] = 0x0D;

  return ERR_OK;
}

static void dhcp6_setup(void)
{
  /*lwip_check_ensure_no_alloc(SKIP_POOL(MEMP_SYS_TIMEOUT));*/

  reset_client_id();
  allow_override_client_id = 0;
  lwip_sys_now = 0;

  netif_add(&net_test, NULL, NULL, NULL, &net_test, testif_init, ethernet_input);
  netif_set_link_up(&net_test);
  netif_set_up(&net_test);

  netif_create_ip6_linklocal_address(&net_test, 0);
  netif_ip6_addr_set_state(&net_test, 0, IP6_ADDR_PREFERRED);

  net_test.output_ip6 = &output_ip6_chk_no_send;
}

static void dhcp6_teardown(void)
{
  /*lwip_check_ensure_no_alloc(SKIP_POOL(MEMP_SYS_TIMEOUT));*/
  netif_remove(&net_test);
}

static err_t lwip_tx_func(struct netif *netif, struct pbuf *p)
{
  fail_unless(netif == &net_test);
  fail_unless(p == NULL);

  return ERR_OK;
}

static void send_pkt(struct netif *netif, enum dhcp_msg_type type, int opts)
{
  struct pbuf *p, *q;
  static uint8_t buf[512];
  uint8_t *p_buf = buf;
  size_t len = 0;
  uint16_t udp_pkt_with_its_hdr_len;

  memset(buf, 1, sizeof(buf));
  memcpy((void*)(&buf[len]), pkt_headers, sizeof(pkt_headers));
  len += sizeof(pkt_headers);

  switch (type) {
    case DHCP_MSG_ADVERTISE:
      memcpy((void *) (&buf[len]), pkt_dhcp_advertise, sizeof(pkt_dhcp_advertise));
      len += sizeof(pkt_dhcp_advertise);
      break;
    case DHCP_MSG_REPLY:
      memcpy((void *) (&buf[len]), pkt_dhcp_reply, sizeof(pkt_dhcp_reply));
      len += sizeof(pkt_dhcp_reply);
      break;
    default:
      fail("Unhandled DHCP msg type");
  }

  memcpy((void*)(&buf[len]), pkt_xid, sizeof(pkt_xid));
  len += sizeof(pkt_xid);


  if (opts & OPT_CLI_ID) {
    fail_unless(pkt_client_id_len != 0, "Append of client id to msg requested but no client id received so far");
    memcpy((void*)(&buf[len]), pkt_client_id, pkt_client_id_len);
    fail_if(pkt_client_id[3] != pkt_client_id_len - 4);
    len += pkt_client_id_len;
  }

  if (opts & OPT_SRV_ID) {
    memcpy((void*)(&buf[len]), pkt_server_id, sizeof(pkt_server_id));
    fail_if(pkt_server_id[3] != sizeof(pkt_server_id)-4);
    len += sizeof(pkt_server_id);
  }

  if (opts & OPT_IA_NA) {
    memcpy((void*)(&buf[len]), pkt_ia_na, sizeof(pkt_ia_na));
    fail_if(pkt_ia_na[3] != sizeof(pkt_ia_na)-4);
    len += sizeof(pkt_ia_na);
  }

  if (opts & OPT_IA_NA_0) {
    memcpy((void*)(&buf[len]), pkt_ia_na_0, sizeof(pkt_ia_na_0));
    fail_if(pkt_ia_na_0[3] != sizeof(pkt_ia_na_0)-4);
    len += sizeof(pkt_ia_na_0);
  }

  if (opts & OPT_IA_PD) {
    memcpy((void*)(&buf[len]), pkt_ia_pd, sizeof(pkt_ia_pd));
    fail_if(pkt_ia_pd[3] != sizeof(pkt_ia_pd)-4);
    len += sizeof(pkt_ia_pd);
  }

  if (opts & OPT_IA_PD_0) {
    memcpy((void*)(&buf[len]), pkt_ia_pd_0, sizeof(pkt_ia_pd_0));
    fail_if(pkt_ia_pd_0[3] != sizeof(pkt_ia_pd_0)-4);
    len += sizeof(pkt_ia_pd_0);
  }

  if (opts & OPT_DNS_SRV) {
    memcpy((void*)(&buf[len]), pkt_dns_srv, sizeof(pkt_dns_srv));
    fail_if(pkt_dns_srv[3] != sizeof(pkt_dns_srv)-4);
    len += sizeof(pkt_dns_srv);
  }

  if (opts & OPT_DOMAIN_SRCH_LST) {
    memcpy((void*)(&buf[len]), pkt_domain_srch_lst, sizeof(pkt_domain_srch_lst));
    fail_if(pkt_domain_srch_lst[3] != sizeof(pkt_domain_srch_lst)-4);
    len += sizeof(pkt_domain_srch_lst);
  }

  if (opts & OPT_SNTP_SRV) {
    memcpy((void*)(&buf[len]), pkt_sntp_srv, sizeof(pkt_sntp_srv));
    fail_if(pkt_sntp_srv[3] != sizeof(pkt_sntp_srv)-4);
    len += sizeof(pkt_sntp_srv);
  }

  /* Adjust lengths in UDP and IPv6 header */
  udp_pkt_with_its_hdr_len = len - 54;
  buf[pkt_ip_len_pos  ] = udp_pkt_with_its_hdr_len << 8;
  buf[pkt_ip_len_pos+1] = udp_pkt_with_its_hdr_len & 0xffU;
  buf[pkt_udp_len_pos  ] = udp_pkt_with_its_hdr_len << 8;
  buf[pkt_udp_len_pos+1] = udp_pkt_with_its_hdr_len & 0xffU;

  p = pbuf_alloc(PBUF_RAW, (u16_t)len, PBUF_RAM);


  fail_unless(p != NULL);
  for(q = p; q != NULL; q = q->next) {
    /* copy to pbuf chain */
    memset(q->payload, 0xf0, q->len);
    memcpy(q->payload, p_buf, q->len);
    p_buf += q->len;
  }
  netif->input(p, netif);
}

static int16_t find_udp_hdr(const struct pbuf *p)
{
  const uint16_t ipv6_hdr_len = 40;
  const uint8_t * const d = (const uint8_t *) p->payload;
  uint8_t next_hdr;
  fail_if(p->tot_len < ipv6_hdr_len, "Packet length %u < IPv6 header length %u", p->tot_len, ipv6_hdr_len);

  next_hdr = d[6];

  if (next_hdr == 17) { /* Next header is UDP 17 */
    return ipv6_hdr_len;
  } else {
    /* Ignore ICMPv6 packets. (nd6 module sends them because we call its timer) */
    fail_unless(next_hdr == 58, "Got packet with next header %u which is not UDP(17) nor ICMPv6(58)", next_hdr);
    return -1;
  }
}

static uint16_t parse_udp_hdr(const struct pbuf *p, uint16_t pos, uint16_t * src_port, uint16_t * dst_port)
{
  const uint8_t * const d = (const uint8_t *) p->payload;

  fail_unless(pos+6 <= p->len, "Packet too small");

  *src_port =  d[pos++]<<8;
  *src_port += d[pos++];

  *dst_port =  d[pos++]<<8;
  *dst_port += d[pos++];

  return pos + 4;
}

static uint16_t parse_dhcp_hdr(const struct pbuf *p, uint16_t pos, uint8_t * msg_type, uint32_t * transaction_id)
{
  const uint8_t * d = (const uint8_t *) p->payload;

  uint8_t dd[100];
  memcpy(dd, d, 100);

  fail_unless(pos+4 <= p->len, "Packet too small");

  *msg_type = d[pos++];

  *transaction_id =  d[pos++]<<16;
  *transaction_id += d[pos++]<<8;
  *transaction_id += d[pos++];

  return pos;
}

static uint16_t parse_ia_id(const struct pbuf *p, uint16_t pos, uint32_t * ia_id)
{
  const uint8_t * const d = (const uint8_t *) p->payload;
  fail_if(pos == OPT_NOT_FOUND);
  fail_unless(pos+12 <= p->len, "Packet too small");

  *ia_id =  d[pos++] << 24;
  *ia_id += d[pos++] << 16;
  *ia_id += d[pos++] <<  8;
  *ia_id += d[pos++];

  return pos + 8; /* Skip T1, T2 point to the begining of nested option */
}

/* Sets flags (enum pkt_opts) with found options */
static uint16_t parse_oro(const struct pbuf *p, uint16_t pos, uint16_t opt_len, unsigned * const found_opts)
{
  const uint8_t * const d = (const uint8_t *) p->payload;
  const unsigned num_opts = opt_len / 2; /* Each opt has 2 bytes */
  unsigned i;
  fail_unless(pos + opt_len <= p->len, "Packet too small");

  *found_opts = 0U;

  for(i=0; i<num_opts; i++) {

    uint16_t opt = d[pos++] << 8;
    opt += d[pos++];

    switch (opt) {
      case DHCP6_OPTION_DNS_SERVERS /* 23 */:
        *found_opts |= OPT_DNS_SRV;
        break;

      case DHCP6_OPTION_DOMAIN_LIST /* 24 */:
        *found_opts |= OPT_DOMAIN_SRCH_LST;
        break;

      case DHCP6_OPTION_SNTP_SERVERS /* 31 */:
        *found_opts |= OPT_SNTP_SRV;
        break;

      default:
        break;
    }
  }
  return pos;
}

/* Returns elapsed time in ms */
static uint32_t parse_elapsed_ms(const struct pbuf *p, uint16_t pos, uint16_t opt_len )
{
  const uint8_t * const d = (const uint8_t *) p->payload;
  uint16_t t;
  fail_if(opt_len != 2, "Elapsed option len is %u instead of 2", opt_len);
  fail_unless(pos + opt_len <= p->len, "Packet too small");

  t  = d[pos++] << 8;
  t += d[pos++];

  return t*10;
}

static uint16_t find_opt(const struct pbuf *p, uint16_t pos, uint16_t opt_type, uint16_t * opt_len)
{
  const uint8_t * d = (const uint8_t *) p->payload;
  uint16_t found_type;
  int prev_found_type = -1;
  unsigned prev_found_type_pos = 0;

  fail_unless(pos+4 <= p->len, "Packet too small");

  while((pos+4) <= p->len) {
    found_type = d[pos++] << 8;
    found_type += d[pos++];

    *opt_len = d[pos++] << 8;
    *opt_len += d[pos++];

    fail_unless(found_type < 150, "Found option type > 150 (%u) at %u byte. (Previous opt was %d at %u byte)",
                found_type, pos-4, prev_found_type, prev_found_type_pos);

    fail_unless(*opt_len <= (p->len - pos),
                "Found option %u at %u byte with length %u exceeding remaining packet length %u. "
                "(Previous opt was %d at %u byte)",
                found_type, pos-4, *opt_len, p->len - pos,
                prev_found_type, prev_found_type_pos);

    if (opt_type == found_type) {
      return pos;
    }

    prev_found_type = found_type;
    prev_found_type_pos = pos - 4;
    pos += *opt_len;
  }

  return OPT_NOT_FOUND;
}

static void store_xid(uint32_t xid)
{
  pkt_xid[0] = (xid>>16) & 0xffU;
  pkt_xid[1] = (xid>> 8) & 0xffU;
  pkt_xid[2] = (xid    ) & 0xffU;
}

static void store_client_id(const struct pbuf *p, uint16_t pos, size_t len)
{
  const uint8_t * d = (const uint8_t *) p->payload;
  fail_if(pos < 4);
  pos -= 4; /* Copy whole opt including header */

  fail_unless(len <= sizeof(pkt_client_id)-4);
  len += 4;

  if (!allow_override_client_id) {
    fail_unless(pkt_client_id_len == 0, "Trying to override client id stored form previous run");
  }

  memcpy(pkt_client_id, &d[pos], len);
  pkt_client_id_len = len;
}

static void reset_client_id(void)
{
  pkt_client_id_len = 0;
}

static void store_ia_na_id(uint32_t ia_id)
{
  pkt_ia_na[pkt_iaid_pos + 0] = (ia_id>>24) & 0xffU;
  pkt_ia_na[pkt_iaid_pos + 1] = (ia_id>>16) & 0xffU;
  pkt_ia_na[pkt_iaid_pos + 2] = (ia_id>> 8) & 0xffU;
  pkt_ia_na[pkt_iaid_pos + 3] = (ia_id    ) & 0xffU;
}

#if LWIP_IPV6_DHCP6_PD
static void store_ia_pd_id(uint32_t ia_id)
{
  pkt_ia_pd[pkt_iaid_pos + 0] = (ia_id>>24) & 0xffU;
  pkt_ia_pd[pkt_iaid_pos + 1] = (ia_id>>16) & 0xffU;
  pkt_ia_pd[pkt_iaid_pos + 2] = (ia_id>> 8) & 0xffU;
  pkt_ia_pd[pkt_iaid_pos + 3] = (ia_id    ) & 0xffU;
}
#endif

static err_t output_ip6_chk_no_send(struct netif *netif, struct pbuf *p, const ip6_addr_t *ipaddr)
{
  uint16_t src_port = 0;
  uint16_t dst_port = 0;
  uint8_t dhcp6_msg_type = 255;
  uint32_t transaction_id = 0;

  int16_t pos = find_udp_hdr(p);
  if (pos<0) {
    return ERR_OK; /* Not interested in non UDP packets */
  }

  LWIP_UNUSED_ARG(ipaddr);

  fail_unless(netif == &net_test);
  fail_unless(p != NULL);

  pos = parse_udp_hdr(p, pos, &src_port, &dst_port);
  fail_unless( src_port == 546, "Unexpected msg sent from src port %u (instead of 546)", src_port);
  fail_unless( dst_port == 547, "Unexpected msg sent to dst port %u (instead of 547)", dst_port );

  pos = parse_dhcp_hdr(p, pos, &dhcp6_msg_type, &transaction_id);
  fail("Unexpected message of type %s sent from DHCP client", msg_type_to_str(dhcp6_msg_type));

  return ERR_OK;
}

static unsigned pkt_solicit_cnt;
static err_t output_ip6_chk_solicit(struct netif *netif, struct pbuf *p, const ip6_addr_t *ipaddr)
{
  uint16_t src_port = 0;
  uint16_t dst_port = 0;
  uint8_t dhcp6_msg_type = 255;
  uint32_t transaction_id = 0;

  uint16_t opt_cli_id_pos;
  uint16_t opt_cli_id_len;
  uint16_t opt_elapsed_pos;
  uint16_t opt_elapsed_len;
  uint16_t opt_iapd_pos;
  uint16_t opt_iapd_len;
#if LWIP_IPV6_DHCP6_PD
  uint32_t opt_iapd_iaid;
#endif
  uint16_t opt_iana_pos;
  uint16_t opt_iana_len;
  uint32_t opt_iana_iaid;
  uint16_t opt_oro_pos;
  uint16_t opt_oro_len;
  unsigned found_oro_requests;

  int16_t pos = find_udp_hdr(p);
  if (pos<0) {
    return ERR_OK; /* Not interested in non UDP packets */
  }

  fail_unless(netif == &net_test);
  fail_unless(p != NULL);
  LWIP_UNUSED_ARG(ipaddr);

  pos = parse_udp_hdr(p, pos, &src_port, &dst_port);
  fail_unless( src_port == 546 );
  fail_unless( dst_port == 547 );

  pos = parse_dhcp_hdr(p, pos, &dhcp6_msg_type, &transaction_id);
  fail_unless( dhcp6_msg_type == 1, "Got msg of type %s from DHCP client instead of %s",
               msg_type_to_str(dhcp6_msg_type), msg_type_to_str(1));

  opt_cli_id_pos = find_opt(p, pos, 1 /*client identifier*/, &opt_cli_id_len);
  fail_unless(opt_cli_id_pos != OPT_NOT_FOUND);

  opt_elapsed_pos = find_opt(p, pos, 8 /*elapsed time*/, &opt_elapsed_len);
  fail_unless(opt_elapsed_pos != OPT_NOT_FOUND);
  fail_unless(opt_elapsed_len == 2);
  elapsed_ms = parse_elapsed_ms(p, opt_elapsed_pos, opt_elapsed_len);

  opt_iapd_pos = find_opt(p, pos, 25 /*IA PD*/, &opt_iapd_len);
#if LWIP_IPV6_DHCP6_PD
  fail_unless(opt_iapd_pos != OPT_NOT_FOUND);
  fail_unless(opt_iapd_len == 12);
#else
  fail_unless(opt_iapd_pos == OPT_NOT_FOUND);
#endif

  opt_iana_pos = find_opt(p, pos, 3 /*IA NA*/, &opt_iana_len);
  fail_unless(opt_iana_pos != OPT_NOT_FOUND);
  fail_unless(opt_iana_len == 12);

  opt_oro_pos = find_opt(p, pos, 6 /*ORO option request*/, &opt_oro_len);
  fail_unless(opt_oro_pos != OPT_NOT_FOUND);
  parse_oro(p, opt_oro_pos, opt_oro_len, &found_oro_requests);
#if LWIP_DHCP6_MAX_DNS_SERVERS
  fail_unless(found_oro_requests & OPT_DNS_SRV);
#else
  fail_if(found_oro_requests & OPT_DNS_SRV,
          "LWIP_DHCP6_MAX_DNS_SERVERS is 0 but DNS server option found in solicitation");
#endif
#if LWIP_DHCP6_GET_NTP_SRV
  fail_unless(found_oro_requests & OPT_SNTP_SRV);
#endif


  store_xid(transaction_id);
  store_client_id(p, opt_cli_id_pos, opt_cli_id_len);

#if LWIP_IPV6_DHCP6_PD
  parse_ia_id(p, opt_iapd_pos, &opt_iapd_iaid);
  store_ia_pd_id(opt_iapd_iaid);
#endif

  parse_ia_id(p, opt_iana_pos, &opt_iana_iaid);
  store_ia_na_id(opt_iana_iaid);

  pkt_solicit_cnt++;

  return ERR_OK;
}

static unsigned pkt_request_cnt;
static err_t output_ip6_chk_request(struct netif *netif, struct pbuf *p, const ip6_addr_t *ipaddr)
{
  uint16_t src_port = 0;
  uint16_t dst_port = 0;
  uint8_t dhcp6_msg_type = 255;
  uint32_t transaction_id = 0;

  uint16_t opt_elapsed_pos;
  uint16_t opt_elapsed_len;
  uint16_t opt_iapd_pos;
  uint16_t opt_iapd_len;
  uint16_t opt_iana_pos;
  uint16_t opt_iana_len;
  uint16_t opt_server_id_pos;
  uint16_t opt_server_id_len;
  uint16_t opt_client_id_pos;
  uint16_t opt_client_id_len;
  uint16_t opt_oro_pos;
  uint16_t opt_oro_len;
  unsigned found_oro_requests;

  int16_t pos = find_udp_hdr(p);
  if (pos<0) {
    return ERR_OK; /* Not interested in non UDP packets */
  }

  fail_unless(netif == &net_test);
  fail_unless(p != NULL);
  LWIP_UNUSED_ARG(ipaddr);

  pos = parse_udp_hdr(p, pos, &src_port, &dst_port);
  fail_unless( src_port == 546 );
  fail_unless( dst_port == 547 );

  pos = parse_dhcp_hdr(p, pos, &dhcp6_msg_type, &transaction_id);
  fail_unless( dhcp6_msg_type == 3, "Got msg of type %s from DHCP client instead of %s",
               msg_type_to_str(dhcp6_msg_type), msg_type_to_str(3));

  opt_client_id_pos = find_opt(p, pos, 1 /*Client ID*/, &opt_client_id_len);
  fail_unless(opt_client_id_pos != OPT_NOT_FOUND);

  opt_server_id_pos = find_opt(p, pos, 2 /*Server ID*/, &opt_server_id_len);
  fail_unless(opt_server_id_pos != OPT_NOT_FOUND);
  fail_unless(opt_server_id_len == sizeof(pkt_server_id)-4);

  opt_iana_pos = find_opt(p, pos, 3 /*IA NA*/, &opt_iana_len);
  fail_unless(opt_iana_pos != OPT_NOT_FOUND);
  fail_unless(opt_iana_len == 40);

  opt_elapsed_pos = find_opt(p, pos, 8 /*Elapsed time*/, &opt_elapsed_len);
  fail_unless(opt_elapsed_pos != OPT_NOT_FOUND);
  fail_unless(opt_elapsed_len == 2);
  elapsed_ms = parse_elapsed_ms(p, opt_elapsed_pos, opt_elapsed_len);

  opt_iapd_pos = find_opt(p, pos, 25 /*IA PD*/, &opt_iapd_len);
#if LWIP_IPV6_DHCP6_PD
  fail_unless(opt_iapd_pos != OPT_NOT_FOUND);
  fail_unless(opt_iapd_len == 41); /* Must include prefix */
#else
  fail_unless(opt_iapd_pos == OPT_NOT_FOUND);
#endif

  opt_oro_pos = find_opt(p, pos, 6 /*ORO option request*/, &opt_oro_len);
  fail_unless(opt_oro_pos != OPT_NOT_FOUND);
  parse_oro(p, opt_oro_pos, opt_oro_len, &found_oro_requests);
#if LWIP_DHCP6_MAX_DNS_SERVERS
  fail_unless(found_oro_requests & OPT_DNS_SRV);
#else
  fail_if(found_oro_requests & OPT_DNS_SRV,
          "LWIP_DHCP6_MAX_DNS_SERVERS is 0 but DNS server option found in request");
#endif

#if LWIP_DHCP6_GET_NTP_SRV
  fail_unless(found_oro_requests & OPT_SNTP_SRV);
#endif

  store_xid(transaction_id);

  pkt_request_cnt++;
  return ERR_OK;
}

static unsigned pkt_release_cnt;
static err_t output_ip6_chk_release(struct netif *netif, struct pbuf *p, const ip6_addr_t *ipaddr)
{
  uint16_t src_port = 0;
  uint16_t dst_port = 0;
  uint8_t dhcp6_msg_type = 255;
  uint32_t transaction_id = 0;

  uint16_t opt_elapsed_pos;
  uint16_t opt_elapsed_len;
  uint16_t opt_iapd_pos;
  uint16_t opt_iapd_len;
  uint16_t opt_iana_pos;
  uint16_t opt_iana_len;
  uint16_t opt_server_id_pos;
  uint16_t opt_server_id_len;
  uint16_t opt_client_id_pos;
  uint16_t opt_client_id_len;

  int16_t pos = find_udp_hdr(p);
  if (pos<0) {
    return ERR_OK; /* Not interested in non UDP packets */
  }

  fail_unless(netif == &net_test);
  fail_unless(p != NULL);
  LWIP_UNUSED_ARG(ipaddr);

  pos = parse_udp_hdr(p, pos, &src_port, &dst_port);
  fail_unless( src_port == 546 );
  fail_unless( dst_port == 547 );

  pos = parse_dhcp_hdr(p, pos, &dhcp6_msg_type, &transaction_id);
  fail_unless( dhcp6_msg_type == 8, "Got msg of type %s from DHCP client instead of %s",
               msg_type_to_str(dhcp6_msg_type), msg_type_to_str(8));

  opt_client_id_pos = find_opt(p, pos, 1 /*Client ID*/, &opt_client_id_len);
  fail_unless(opt_client_id_pos != OPT_NOT_FOUND);

  opt_server_id_pos = find_opt(p, pos, 2 /*Server ID*/, &opt_server_id_len);
  fail_unless(opt_server_id_pos != OPT_NOT_FOUND);
  fail_unless(opt_server_id_len == sizeof(pkt_server_id)-4);

  opt_iana_pos = find_opt(p, pos, 3 /*IA NA*/, &opt_iana_len);
  fail_unless(opt_iana_pos != OPT_NOT_FOUND);
  fail_unless(opt_iana_len == 40);

  opt_elapsed_pos = find_opt(p, pos, 8 /*Elapsed time*/, &opt_elapsed_len);
  fail_unless(opt_elapsed_pos != OPT_NOT_FOUND);
  fail_unless(opt_elapsed_len == 2);
  elapsed_ms = parse_elapsed_ms(p, opt_elapsed_pos, opt_elapsed_len);

  opt_iapd_pos = find_opt(p, pos, 25 /*IA PD*/, &opt_iapd_len);
#if LWIP_IPV6_DHCP6_PD
  fail_unless(opt_iapd_pos != OPT_NOT_FOUND);
  fail_unless(opt_iapd_len == 41); /* Must include prefix */
#else
  fail_unless(opt_iapd_pos == OPT_NOT_FOUND);
#endif

  store_xid(transaction_id);

  pkt_release_cnt++;

  return ERR_OK;
}

static unsigned pkt_renew_cnt;
static err_t output_ip6_chk_renew(struct netif *netif, struct pbuf *p, const ip6_addr_t *ipaddr)
{
  uint16_t src_port = 0;
  uint16_t dst_port = 0;
  uint8_t dhcp6_msg_type = 255;
  uint32_t transaction_id = 0;

  uint16_t opt_elapsed_pos;
  uint16_t opt_elapsed_len;
  uint16_t opt_iapd_pos;
  uint16_t opt_iapd_len;
  uint16_t opt_iana_pos;
  uint16_t opt_iana_len;
  uint16_t opt_server_id_pos;
  uint16_t opt_server_id_len;
  uint16_t opt_client_id_pos;
  uint16_t opt_client_id_len;
  uint16_t opt_oro_pos;
  uint16_t opt_oro_len;
  unsigned found_oro_requests;

  int16_t pos = find_udp_hdr(p);
  if (pos<0) {
    return ERR_OK; /* Not interested in non UDP packets */
  }

  fail_unless(netif == &net_test);
  fail_unless(p != NULL);
  LWIP_UNUSED_ARG(ipaddr);

  pos = parse_udp_hdr(p, pos, &src_port, &dst_port);
  fail_unless( src_port == 546 );
  fail_unless( dst_port == 547 );

  pos = parse_dhcp_hdr(p, pos, &dhcp6_msg_type, &transaction_id);
  fail_unless( dhcp6_msg_type == 5, "Got msg of type %s from DHCP client instead of %s",
               msg_type_to_str(dhcp6_msg_type), msg_type_to_str(5));

  opt_client_id_pos = find_opt(p, pos, 1 /*Client ID*/, &opt_client_id_len);
  fail_unless(opt_client_id_pos != OPT_NOT_FOUND);

  opt_server_id_pos = find_opt(p, pos, 2 /*Server ID*/, &opt_server_id_len);
  fail_unless(opt_server_id_pos != OPT_NOT_FOUND);
  fail_unless(opt_server_id_len == sizeof(pkt_server_id)-4);

  opt_iana_pos = find_opt(p, pos, 3 /*IA NA*/, &opt_iana_len);
  fail_unless(opt_iana_pos != OPT_NOT_FOUND);
  fail_unless(opt_iana_len == 40);

  opt_elapsed_pos = find_opt(p, pos, 8 /*Elapsed time*/, &opt_elapsed_len);
  fail_unless(opt_elapsed_pos != OPT_NOT_FOUND);
  fail_unless(opt_elapsed_len == 2);
  elapsed_ms = parse_elapsed_ms(p, opt_elapsed_pos, opt_elapsed_len);

  opt_iapd_pos = find_opt(p, pos, 25 /*IA PD*/, &opt_iapd_len);
#if LWIP_IPV6_DHCP6_PD
  fail_unless(opt_iapd_pos != OPT_NOT_FOUND);
  fail_unless(opt_iapd_len == 41); /* Must include prefix */
#else
  fail_unless(opt_iapd_pos == OPT_NOT_FOUND);
#endif

  opt_oro_pos = find_opt(p, pos, 6 /*ORO option request*/, &opt_oro_len);
  fail_unless(opt_oro_pos != OPT_NOT_FOUND);
  parse_oro(p, opt_oro_pos, opt_oro_len, &found_oro_requests);
#if LWIP_DHCP6_MAX_DNS_SERVERS
  fail_unless(found_oro_requests & OPT_DNS_SRV);
#else
  fail_if(found_oro_requests & OPT_DNS_SRV,
          "LWIP_DHCP6_MAX_DNS_SERVERS is 0 but DNS server option found in renew request");
#endif
#if LWIP_DHCP6_GET_NTP_SRV
  fail_unless(found_oro_requests & OPT_SNTP_SRV);
#endif

  store_xid(transaction_id);

  pkt_renew_cnt++;

  return ERR_OK;
}

static unsigned pkt_inforeq_cnt;
static err_t output_ip6_chk_inforeq(struct netif *netif, struct pbuf *p, const ip6_addr_t *ipaddr)
{
  uint16_t src_port = 0;
  uint16_t dst_port = 0;
  uint8_t dhcp6_msg_type = 255;
  uint32_t transaction_id = 0;

  uint16_t opt_oro_pos;
  uint16_t opt_oro_len;
  unsigned found_oro_requests;


  int16_t pos = find_udp_hdr(p);
  if (pos<0) {
    return ERR_OK; /* Not interested in non UDP packets */
  }

  fail_unless(netif == &net_test);
  fail_unless(p != NULL);
  LWIP_UNUSED_ARG(ipaddr);

  pos = parse_udp_hdr(p, pos, &src_port, &dst_port);
  fail_unless( src_port == 546 );
  fail_unless( dst_port == 547 );

  pos = parse_dhcp_hdr(p, pos, &dhcp6_msg_type, &transaction_id);
  fail_unless( dhcp6_msg_type == 11, "Got msg of type %s from DHCP client instead of %s",
               msg_type_to_str(dhcp6_msg_type), msg_type_to_str(11));


  opt_oro_pos = find_opt(p, pos, 6 /*ORO option request*/, &opt_oro_len);
  fail_unless(opt_oro_pos != OPT_NOT_FOUND);
  parse_oro(p, opt_oro_pos, opt_oro_len, &found_oro_requests);
#if LWIP_DHCP6_MAX_DNS_SERVERS
  fail_unless(found_oro_requests & OPT_DNS_SRV);
#endif
#if LWIP_DHCP6_MAX_DNS_SERVERS
  fail_unless(found_oro_requests & OPT_SNTP_SRV);
#endif

  store_xid(transaction_id);
  pkt_inforeq_cnt++;
  return ERR_OK;
}

#if LWIP_IPV6_DHCP6_PD
static const struct dhcp6_delegated_prefix * prefix;
static u8_t prefix_valid;

static void prefix_cb(struct netif* netif, const struct dhcp6_delegated_prefix * _prefix, u8_t valid) {
  (void) netif;
  prefix = _prefix;
  prefix_valid = valid;
}
#endif

/*
 * Test basic happy flow DHCP session.
 * Validate that xid is checked.
 */
START_TEST(test_dhcp6_stateful)
{
  int i;
  err_t ret;
  /*u32_t xid;*/
  LWIP_UNUSED_ARG(_i);



  ret = dhcp6_enable(&net_test);
  fail_unless(ret == ERR_OK);

  /* Test one more time */
  ret = dhcp6_enable(&net_test);
  fail_unless(ret == ERR_ALREADY);

  /* Got RA with M bit set */
  dhcp6_nd6_ra_trigger(&net_test, 1, 0);

  pkt_solicit_cnt = 0;
  net_test.output_ip6 = &output_ip6_chk_solicit;

  tick_500ms_lwip();

  fail_unless(pkt_solicit_cnt>0);

  net_test.output_ip6 = &output_ip6_chk_no_send;

  /* Prepare for request to be sent by client and send advertisement */
  net_test.output_ip6 = output_ip6_chk_request;
  send_pkt(&net_test, DHCP_MSG_ADVERTISE, OPT_SRV_ID | OPT_CLI_ID | OPT_IA_PD | OPT_IA_NA | OPT_DNS_SRV | OPT_SNTP_SRV | OPT_DOMAIN_SRCH_LST);


  net_test.output_ip6 = &output_ip6_chk_no_send;
#if LWIP_IPV6_DHCP6_PD
  prefix_valid = 0;
  memset(&prefix, 0xa5, sizeof(prefix));
  dhcp6_register_pd_callback(&net_test, &prefix_cb);
#endif
  reset_dns_servers();
  reset_sntp_servers();

  /* send reply */
  send_pkt(&net_test, DHCP_MSG_REPLY, OPT_SRV_ID | OPT_CLI_ID | OPT_IA_PD | OPT_IA_NA | OPT_DNS_SRV | OPT_SNTP_SRV | OPT_DOMAIN_SRCH_LST);

  net_test.output_ip6 = &output_ip6_chk_no_send;

#if LWIP_IPV6_DHCP6_PD
  fail_if(prefix_valid == 0);
  fail_unless(prefix->prefix_len == 64);

  fail_unless(prefix->prefix_pref == 4500);
  fail_unless(prefix->prefix_valid == 7200);
#endif

  fail_unless(ip6_addr_istentative(netif_ip6_addr_state(&net_test, 1)),
              "addr in state %s", addr_state_to_str(netif_ip6_addr_state(&net_test, 1)));
  fail_if(ip6_addr_isany(netif_ip6_addr(&net_test, 1)));
  fail_unless(netif_ip6_addr_pref_life(&net_test, 1) == 375);
  fail_unless(netif_ip6_addr_valid_life(&net_test, 1) == 600);

#if LWIP_DHCP6_MAX_DNS_SERVERS
  fail_if(ip6_addr_isany(&dns_getserver(0)->u_addr.ip6));
  fail_if(dns_getserver(0)->u_addr.ip6.zone == IP6_WRONG_ZONE);
#else
  fail_unless(ip6_addr_isany(&dns_getserver(0)->u_addr.ip6),
          "LWIP_DHCP6_MAX_DNS_SERVERS is 0 but DNS server 0 was changed by dhcp client");
#endif

#if LWIP_DHCP6_GET_NTP_SRV
  fail_if(ip6_addr_isany(&sntp_getserver(0)->u_addr.ip6));
  fail_if(sntp_getserver(0)->u_addr.ip6.zone == IP6_WRONG_ZONE);
#else
  fail_unless(ip6_addr_isany(&sntp_getserver(0)->u_addr.ip6),
              "DHCP6 client sets SNTP server but LWIP_DHCP6_GET_NTP_SRV is disabled.");
#endif

  /* Wait 5s */
  for(i=0; i<5*2; i++) {
    tick_500ms_lwip();
  }

#if LWIP_IPV6_DHCP6_PD
  /* Times should be 5s shorter now */
  fail_unless(prefix->prefix_pref == (4500-5));
  fail_unless(prefix->prefix_valid == (7200-5));
#endif

  /* Wait 370s to let expire pref time of address */
  pkt_renew_cnt = 0;
  net_test.output_ip6 = &output_ip6_chk_renew;
  reset_dns_servers();
  reset_sntp_servers();

  fail_unless(ip6_addr_ispreferred(netif_ip6_addr_state(&net_test, 1)),
              "addr in state %s  but should be preferred (pref time = %u)",
              addr_state_to_str(netif_ip6_addr_state(&net_test, 1)), netif_ip6_addr_pref_life(&net_test, 1));

  for(i=0; i<370*2; i++) {
    tick_500ms_lwip();
  }
  fail_unless(pkt_renew_cnt > 0);

  fail_unless(ip6_addr_isdeprecated(netif_ip6_addr_state(&net_test, 1)),
              "addr in state %s, but should be deprecated (pref time = %u)",
              addr_state_to_str(netif_ip6_addr_state(&net_test, 1)), netif_ip6_addr_pref_life(&net_test, 1));

  /* reply to renew */
  send_pkt(&net_test, DHCP_MSG_REPLY, OPT_SRV_ID | OPT_CLI_ID | OPT_IA_PD | OPT_IA_NA | OPT_DNS_SRV | OPT_SNTP_SRV | OPT_DOMAIN_SRCH_LST);

  fail_unless(ip6_addr_ispreferred(netif_ip6_addr_state(&net_test, 1)),
              "addr in state %s, but should be preferred (pref time = %u)",
              addr_state_to_str(netif_ip6_addr_state(&net_test, 1)), netif_ip6_addr_pref_life(&net_test, 1));
  fail_if(ip6_addr_isany(netif_ip6_addr(&net_test, 1)));
  fail_unless(netif_ip6_addr_pref_life(&net_test, 1) == 375);
  fail_unless(netif_ip6_addr_valid_life(&net_test, 1) == 600);

#if LWIP_DHCP6_MAX_DNS_SERVERS
  fail_if(ip6_addr_isany(&dns_getserver(0)->u_addr.ip6));
  fail_if(dns_getserver(0)->u_addr.ip6.zone == IP6_WRONG_ZONE);
#else
  fail_unless(ip6_addr_isany(&dns_getserver(0)->u_addr.ip6),
              "LWIP_DHCP6_MAX_DNS_SERVERS is 0 but DNS server 0 was changed by dhcp client");
#endif

#if LWIP_DHCP6_GET_NTP_SRV
  fail_if(ip6_addr_isany(&sntp_getserver(0)->u_addr.ip6));
  fail_if(sntp_getserver(0)->u_addr.ip6.zone == IP6_WRONG_ZONE);
#else
  fail_unless(ip6_addr_isany(&sntp_getserver(0)->u_addr.ip6),
              "DHCP6 client sets SNTP server but LWIP_DHCP6_GET_NTP_SRV is disabled.");
#endif

  pkt_release_cnt = 0;
  net_test.output_ip6 = &output_ip6_chk_release;

  /* Clean up */
  dhcp6_disable(&net_test); /* wait somehow for disabling? we should get reply from server*/
  fail_unless(pkt_release_cnt > 0);
  send_pkt(&net_test, DHCP_MSG_REPLY, OPT_SRV_ID | OPT_CLI_ID);
  fail_if(netif_get_client_data(&net_test, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP6) != NULL);
}
END_TEST

/*
 * Test basic happy flow DHCP session.
 * Validate that xid is checked.
 */
START_TEST(test_dhcp6_stateless)
{
  err_t ret;
  /*u32_t xid;*/
  LWIP_UNUSED_ARG(_i);

  ret = dhcp6_enable(&net_test);
  fail_unless(ret == ERR_OK);

  /* Test one more time */
  ret = dhcp6_enable(&net_test);
  fail_unless(ret == ERR_ALREADY);

  reset_dns_servers();
  reset_sntp_servers();

  /* Got RA with M bit NOT set, prepare for request to be sent by client */
  pkt_inforeq_cnt = 0;
  net_test.output_ip6 = &output_ip6_chk_inforeq;
  dhcp6_nd6_ra_trigger(&net_test, 0, 1);

  fail_unless(pkt_inforeq_cnt > 0);

  /* Send reply */
  send_pkt(&net_test, DHCP_MSG_REPLY, OPT_SRV_ID | OPT_DNS_SRV | OPT_SNTP_SRV | OPT_DOMAIN_SRCH_LST);

#if LWIP_DHCP6_MAX_DNS_SERVERS
  fail_if(ip6_addr_isany(&dns_getserver(0)->u_addr.ip6));
  fail_if(dns_getserver(0)->u_addr.ip6.zone == IP6_WRONG_ZONE);
#endif

#if LWIP_DHCP6_MAX_DNS_SERVERS
  fail_if(ip6_addr_isany(&sntp_getserver(0)->u_addr.ip6));
  fail_if(sntp_getserver(0)->u_addr.ip6.zone == IP6_WRONG_ZONE);
#else
  fail_unless(ip6_addr_isany(&sntp_getserver(0)->u_addr.ip6),
              "DHCP6 client sets SNTP server but LWIP_DHCP6_MAX_DNS_SERVERS is disabled.");
#endif

  /* Client should set nothing on disable */
  net_test.output_ip6 = &output_ip6_chk_no_send;

  /* Clean up */
  dhcp6_disable(&net_test);
  fail_if(netif_get_client_data(&net_test, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP6) != NULL);

}
END_TEST


/*
 * 1) Send two solicitations
 * 2) Get advertise
 * 3) Send request until MRC expires
 * 4) Check if starts sending solicitations again
 */
START_TEST(test_dhcp6_stateful_solicit_req_resend)
{
  int i;
  err_t ret;
  LWIP_UNUSED_ARG(_i);

  ret = dhcp6_enable(&net_test);
  fail_unless(ret == ERR_OK);

  /* Got RA with M bit set */
  dhcp6_nd6_ra_trigger(&net_test, 1, 0);

  pkt_solicit_cnt = 0;
  net_test.output_ip6 = &output_ip6_chk_solicit;

  tick_500ms_lwip();

  fail_unless(pkt_solicit_cnt == 1);
  fail_if(elapsed_ms != 0, "Non-zero value of elapsed time (%u ms) in first solicit message.", elapsed_ms);

  reset_client_id();

  for (i=0; i<2*2; i++) tick_500ms_lwip(); /* wait 2s*/
  fail_unless(pkt_solicit_cnt == 2);
  fail_if(elapsed_ms != 2000, "wrong elapsed time (%u ms != 2000)  in solicit message after 2s.", elapsed_ms);


  /* Prepare for request to be sent by client, send advertise and wait until it reaches REQ_MAX_RC and then goes back to sending solicitations. */
  pkt_request_cnt = 0;
  net_test.output_ip6 = output_ip6_chk_request;
  send_pkt(&net_test, DHCP_MSG_ADVERTISE, OPT_SRV_ID | OPT_CLI_ID | OPT_IA_PD | OPT_IA_NA | OPT_DNS_SRV | OPT_SNTP_SRV | OPT_DOMAIN_SRCH_LST);

  for (i=0; i<180*2; i++) tick_500ms_lwip(); /* wait 180s to get maximum (10) requests */
  fail_unless(pkt_request_cnt == 10);

  /* prepare to get solicitation again */
  pkt_solicit_cnt = 0;
  reset_client_id();
  net_test.output_ip6 = &output_ip6_chk_solicit;

  /* Move in time */
  tick_500ms_lwip();
  tick_500ms_lwip();
  fail_unless(pkt_solicit_cnt == 1);

  /* prepare to get request */
  pkt_request_cnt = 0;
  net_test.output_ip6 = &output_ip6_chk_request;

  send_pkt(&net_test, DHCP_MSG_ADVERTISE, OPT_SRV_ID | OPT_CLI_ID | OPT_IA_PD | OPT_IA_NA | OPT_DNS_SRV | OPT_SNTP_SRV | OPT_DOMAIN_SRCH_LST);

  fail_unless(pkt_request_cnt == 1);

  net_test.output_ip6 = &output_ip6_chk_no_send;

  /* send reply */
  send_pkt(&net_test, DHCP_MSG_REPLY, OPT_SRV_ID | OPT_CLI_ID | OPT_IA_PD | OPT_IA_NA | OPT_DNS_SRV | OPT_SNTP_SRV | OPT_DOMAIN_SRCH_LST);

  /* Now we should be bound */
  fail_unless(ip6_addr_istentative(netif_ip6_addr_state(&net_test, 1)),
              "addr in state %s", addr_state_to_str(netif_ip6_addr_state(&net_test, 1)));
  fail_if(ip6_addr_isany(netif_ip6_addr(&net_test, 1)));
  fail_unless(netif_ip6_addr_pref_life(&net_test, 1) == 375);

  /* Wait 374s */
  for(i=0; i<374*2; i++) tick_500ms_lwip();

  /* prepare to get renew */
  pkt_renew_cnt = 0;
  net_test.output_ip6 = &output_ip6_chk_renew;

  /* Wait 1s to get first renew request */
  for(i=0; i<1*2; i++) tick_500ms_lwip();

  fail_unless(pkt_renew_cnt == 1);

  fail_unless(ip6_addr_isdeprecated(netif_ip6_addr_state(&net_test, 1)),
              "addr in state %s  but should be deprecated (pref time = %u) (valid time = %u)",
              addr_state_to_str(netif_ip6_addr_state(&net_test, 1)),
              netif_ip6_addr_pref_life(&net_test, 1), netif_ip6_addr_valid_life(&net_test, 1));

  /* Now wait for remaining valid life (224s) for timeout in state renewing */

  for(i=0; i<224*2; i++) {
    tick_500ms_lwip();
  }
  fail_unless(pkt_renew_cnt = 5);

  /* Our IP address is not valid anymore and because this implementation doesn't hold list of other DHCP servers
   * check if client will send solicitations again */

  /* prepare to get solicitation again */
  pkt_solicit_cnt = 0;
  reset_client_id();
  allow_override_client_id = 1;
  net_test.output_ip6 = &output_ip6_chk_solicit;

  /* Move in time */
  tick_500ms_lwip();
  tick_500ms_lwip();
  fail_unless(pkt_solicit_cnt > 0);

  fail_unless(ip6_addr_isinvalid(netif_ip6_addr_state(&net_test, 1)),
              "addr in state %s, but should be invalid (valid time = %u)",
              addr_state_to_str(netif_ip6_addr_state(&net_test, 1)), netif_ip6_addr_valid_life(&net_test, 1));

  /* Clean up */
  net_test.output_ip6 = &output_ip6_chk_no_send; /* In soliciting state we shouldn't send anything on disable */
  dhcp6_disable(&net_test);
  send_pkt(&net_test, DHCP_MSG_REPLY, OPT_SRV_ID | OPT_CLI_ID);
  fail_if(netif_get_client_data(&net_test, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP6) != NULL);
}
END_TEST

/*
 * 1) get addr and prefix
 * 2) try to renew
 * 3) get addr and prefix with 0 valid times
 * 4) Check if starts sending solicitations again
 */
START_TEST(test_dhcp6_stateful_refuse)
{
  int i;
  err_t ret;
  LWIP_UNUSED_ARG(_i);

  ret = dhcp6_enable(&net_test);
  fail_unless(ret == ERR_OK);

#if LWIP_IPV6_DHCP6_PD
  prefix_valid = 0;
  memset(&prefix, 0xa5, sizeof(prefix));
  dhcp6_register_pd_callback(&net_test, &prefix_cb);
#endif

  /* Got RA with M and O bits set */
  dhcp6_nd6_ra_trigger(&net_test, 1, 1);

  pkt_solicit_cnt = 0;
  net_test.output_ip6 = &output_ip6_chk_solicit;

  tick_500ms_lwip();

  fail_unless(pkt_solicit_cnt == 1);

  /* Prepare for request to be sent by client, send advertise */
  pkt_request_cnt = 0;
  net_test.output_ip6 = output_ip6_chk_request;
  send_pkt(&net_test, DHCP_MSG_ADVERTISE, OPT_SRV_ID | OPT_CLI_ID | OPT_IA_PD | OPT_IA_NA );

  net_test.output_ip6 = &output_ip6_chk_no_send;

  /* send reply */

  send_pkt(&net_test, DHCP_MSG_REPLY, OPT_SRV_ID | OPT_CLI_ID | OPT_IA_PD | OPT_IA_NA );

  /* Now we should be bound */
  fail_unless(ip6_addr_istentative(netif_ip6_addr_state(&net_test, 1)),
              "addr in state %s", addr_state_to_str(netif_ip6_addr_state(&net_test, 1)));
  fail_if(ip6_addr_isany(netif_ip6_addr(&net_test, 1)));
  fail_unless(netif_ip6_addr_pref_life(&net_test, 1) == 375);

  /* Wait 374s */
  for(i=0; i<374*2; i++) tick_500ms_lwip();

  /* prepare to get renew */
  pkt_renew_cnt = 0;
  net_test.output_ip6 = &output_ip6_chk_renew;

  /* Wait 1s to get first renew request */
  for(i=0; i<1*2; i++) tick_500ms_lwip();

  fail_unless(pkt_renew_cnt == 1);

  fail_unless(ip6_addr_isdeprecated(netif_ip6_addr_state(&net_test, 1)),
              "addr in state %s  but should be deprecated (pref time = %u) (valid time = %u)",
              addr_state_to_str(netif_ip6_addr_state(&net_test, 1)),
              netif_ip6_addr_pref_life(&net_test, 1), netif_ip6_addr_valid_life(&net_test, 1));

  /* Prepare for solicitation and send reply refusing address */
  pkt_solicit_cnt = 0;
  allow_override_client_id = 1;
#if LWIP_IPV6_DHCP6_PD
  prefix_valid = 1;
#endif
  net_test.output_ip6 = &output_ip6_chk_solicit;

  send_pkt(&net_test, DHCP_MSG_REPLY, OPT_SRV_ID | OPT_CLI_ID | OPT_IA_PD_0 | OPT_IA_NA_0 );

  fail_unless(pkt_solicit_cnt == 1);
  allow_override_client_id = 0;
  fail_unless(ip6_addr_isinvalid(netif_ip6_addr_state(&net_test, 1)),
              "addr in state %s, but should be invalid (valid time = %u)",
              addr_state_to_str(netif_ip6_addr_state(&net_test, 1)), netif_ip6_addr_valid_life(&net_test, 1));
#if LWIP_IPV6_DHCP6_PD
  fail_if(prefix_valid, "Prefix callback not called or called with prefix set as valid");
#endif

  /* Clean up */
  net_test.output_ip6 = &output_ip6_chk_no_send; /* In soliciting state we shouldn't send anything on disable */
  dhcp6_disable(&net_test);
  send_pkt(&net_test, DHCP_MSG_REPLY, OPT_SRV_ID | OPT_CLI_ID);
  fail_if(netif_get_client_data(&net_test, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP6) != NULL);
}
END_TEST

/*
 * 1) get addr and prefix
 * 2) try to renew
 * 3) get addr and prefix with 0 valid times
 * 4) Check if starts sending solicitations again
 */
START_TEST(test_dhcp6_stateful_custom_client_id)
{
#if LWIP_IPV6_DHCP6_STATEFUL
  err_t ret;
  LWIP_UNUSED_ARG(_i);

  ret = dhcp6_enable(&net_test);
  fail_unless(ret == ERR_OK);

  dhcp6_set_client_id_duid(&net_test, pkt_client_id_custom_duid, sizeof(pkt_client_id_custom_duid));

  /* Got RA with M and O bits set */
  dhcp6_nd6_ra_trigger(&net_test, 1, 1);

  pkt_solicit_cnt = 0;
  net_test.output_ip6 = &output_ip6_chk_solicit;

  tick_500ms_lwip();

  fail_unless(pkt_solicit_cnt == 1);
  fail_unless(pkt_client_id_len == sizeof(pkt_client_id_custom_duid) + 4);
  fail_if(memcmp(pkt_client_id_custom_duid, &pkt_client_id[4], sizeof(pkt_client_id_custom_duid)) != 0);

  /* Clean up */
  net_test.output_ip6 = &output_ip6_chk_no_send; /* In soliciting state we shouldn't send anything on disable */
  dhcp6_disable(&net_test);
  send_pkt(&net_test, DHCP_MSG_REPLY, OPT_SRV_ID | OPT_CLI_ID);
  fail_if(netif_get_client_data(&net_test, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP6) != NULL);
#endif /* #if LWIP_IPV6_DHCP6_STATEFUL */
}
END_TEST

/** Create the suite including all tests for this module */
Suite *
dhcp6_suite(void)
{
  testfunc tests[] = {
#if LWIP_IPV6_DHCP6_STATEFUL
    TESTFUNC(test_dhcp6_stateful),
    TESTFUNC(test_dhcp6_stateful_solicit_req_resend),
    TESTFUNC(test_dhcp6_stateful_refuse),
    TESTFUNC(test_dhcp6_stateful_custom_client_id),
#endif
#if !LWIP_IPV6_DHCP6_PD
    TESTFUNC(test_dhcp6_stateless),
#endif
  };

  /* suppress unused function warnings: */
#if !LWIP_IPV6_DHCP6_STATEFUL
  (void) &test_dhcp6_stateful;
  (void) &test_dhcp6_stateful_solicit_req_resend;
  (void) &test_dhcp6_stateful_refuse;
  (void) &test_dhcp6_stateful_custom_client_id;
#elif LWIP_IPV6_DHCP6_PD
  (void) &test_dhcp6_stateless; /* suppress unused function warning */
#endif


  return create_suite("DHCP6", tests, sizeof(tests)/sizeof(testfunc), dhcp6_setup, dhcp6_teardown);
}
