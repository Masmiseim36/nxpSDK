/*
 * Copyright lwIP authors
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
 * This file is part of the lwIP TCP/IP stack.
 */

#include "lwip_check.h"

#include "ip4/test_ip4.h"
#include "ip6/test_ip6.h"
#include "udp/test_udp.h"
#include "tcp/test_tcp.h"
#include "tcp/test_tcp_oos.h"
#include "tcp/test_tcp_state.h"
#include "core/test_def.h"
#include "core/test_dns.h"
#include "core/test_mem.h"
#include "core/test_netif.h"
#include "core/test_pbuf.h"
#include "core/test_timers.h"
#include "etharp/test_etharp.h"
#include "dhcp/test_dhcp.h"
#include "dhcp6/test_dhcp6.h"
#include "mdns/test_mdns.h"
#include "mqtt/test_mqtt.h"
#include "api/test_sockets.h"
#include "ppp/test_pppos.h"

#include "lwip/init.h"
#if !NO_SYS
#include "lwip/tcpip.h"
#endif

/* This function is used for LWIP_RAND by some ports... */
unsigned int
lwip_port_rand(void)
{
  return (unsigned int)rand();
}

Suite* create_suite(const char* name, testfunc *tests, size_t num_tests, SFun setup, SFun teardown)
{
  size_t i;
  Suite *s = suite_create(name);

  for(i = 0; i < num_tests; i++) {
    TCase *tc_core = tcase_create(name);
    if ((setup != NULL) || (teardown != NULL)) {
      tcase_add_checked_fixture(tc_core, setup, teardown);
    }
    tcase_add_named_test(tc_core, tests[i]);
    suite_add_tcase(s, tc_core);
  }
  return s;
}

void lwip_check_ensure_no_alloc(unsigned int skip)
{
  int i;
  unsigned int mask;

  if (!(skip & SKIP_HEAP)) {
    fail_unless(lwip_stats.mem.used == 0,
      "mem heap still has %d bytes allocated", lwip_stats.mem.used);
  }
  for (i = 0, mask = 1; i < MEMP_MAX; i++, mask <<= 1) {
    if (!(skip & mask)) {
#if defined(LWIP_DEBUG) || LWIP_STATS_DISPLAY
      fail_unless(lwip_stats.memp[i]->used == 0,
        "memp pool '%s' still has %d entries allocated",
        lwip_stats.memp[i]->name, lwip_stats.memp[i]->used);
#else
      fail_unless(lwip_stats.memp[i]->used == 0,
        "memp pool %d still has %d entries allocated",
        i, lwip_stats.memp[i]->used);
#endif
    }
  }
}

#ifdef LWIP_UNITTESTS_LIB
int lwip_unittests_run(void)
#else
int main(void)
#endif
{
  int number_failed;
  SRunner *sr;
  size_t i;
  suite_getter_fn* suites[] = {
    ip4_suite,
    ip6_suite,
    udp_suite,
    tcp_suite,
    tcp_oos_suite,
    tcp_state_suite,
    def_suite,
    dns_suite,
    mem_suite,
    netif_suite,
    pbuf_suite,
    timers_suite,
    etharp_suite,
    dhcp_suite,
    dhcp6_suite,
    mdns_suite,
    mqtt_suite,
    sockets_suite
#if PPP_SUPPORT && PPPOS_SUPPORT
    , pppos_suite
#endif /* PPP_SUPPORT && PPPOS_SUPPORT */
  };
  size_t num = sizeof(suites)/sizeof(void*);
  LWIP_ASSERT("No suites defined", num > 0);

#if NO_SYS
  lwip_init();
#else
  tcpip_init(NULL, NULL);
#endif

  sr = srunner_create((suites[0])());
  srunner_set_xml(sr, "lwip_unittests.xml");
  for(i = 1; i < num; i++) {
    srunner_add_suite(sr, ((suite_getter_fn*)suites[i])());
  }

#ifdef LWIP_UNITTESTS_NOFORK
  srunner_set_fork_status(sr, CK_NOFORK);
#endif
#ifdef LWIP_UNITTESTS_FORK
  srunner_set_fork_status(sr, CK_FORK);
#endif

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
