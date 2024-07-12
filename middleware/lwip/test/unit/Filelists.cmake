#
# Copyright lwIP authors
# Copyright 2023 NXP
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
# 3. The name of the author may not be used to endorse or promote products
#    derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
# SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
# OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
# IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
# OF SUCH DAMAGE.
#
# This file is part of the lwIP TCP/IP stack.

# This file is indended to be included in end-user CMakeLists.txt
# include(/path/to/Filelists.cmake)
# It assumes the variable LWIP_DIR is defined pointing to the
# root path of lwIP sources.
#
# This file is NOT designed (on purpose) to be used as cmake
# subdir via add_subdirectory()
# The intention is to provide greater flexibility to users to
# create their own targets using the *_SRCS variables.

if(NOT ${CMAKE_VERSION} VERSION_LESS "3.10.0")
    include_guard(GLOBAL)
endif()

set(LWIP_TESTDIR ${LWIP_DIR}/test/unit)
set(LWIP_TESTFILES
	${LWIP_TESTDIR}/lwip_unittests.c
	${LWIP_TESTDIR}/api/test_sockets.c
	${LWIP_TESTDIR}/arch/sys_arch.c
	${LWIP_TESTDIR}/core/test_def.c
	${LWIP_TESTDIR}/core/test_dns.c
	${LWIP_TESTDIR}/core/test_mem.c
	${LWIP_TESTDIR}/core/test_netif.c
	${LWIP_TESTDIR}/core/test_pbuf.c
	${LWIP_TESTDIR}/core/test_timers.c
	${LWIP_TESTDIR}/dhcp/test_dhcp.c
	${LWIP_TESTDIR}/dhcp6/test_dhcp6.c
	${LWIP_TESTDIR}/etharp/test_etharp.c
	${LWIP_TESTDIR}/ip4/test_ip4.c
	${LWIP_TESTDIR}/ip6/test_ip6.c
	${LWIP_TESTDIR}/mdns/test_mdns.c
	${LWIP_TESTDIR}/mqtt/test_mqtt.c
	${LWIP_TESTDIR}/tcp/tcp_helper.c
	${LWIP_TESTDIR}/tcp/test_tcp_oos.c
	${LWIP_TESTDIR}/tcp/test_tcp_state.c
	${LWIP_TESTDIR}/tcp/test_tcp.c
	${LWIP_TESTDIR}/udp/test_udp.c
	${LWIP_TESTDIR}/ppp/test_pppos.c
)
