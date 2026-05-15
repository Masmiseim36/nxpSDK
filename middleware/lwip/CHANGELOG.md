# NXP lwIP Fork Changelog

This file documents the changes and additions made in the NXP fork of lwIP.
For the list of changes in the upstream project version which this fork is based on, see [CHANGELOG](CHANGELOG).

Source code included in this SDK is currently based on development version 2.2.1 taken from 3rd party lwIP GIT repository.
The webpage https://git.savannah.nongnu.org/cgit/lwip.git allows to browse that repository and also contains URLs for its cloning.

The development versions (X.Y.Z.dev) do not refer to a single source code snapshots.
To avoid ambiguity, change log below contains SHA-1 hashes of GIT commits used when importing the upstream code into the SDK.

KSDK refers to Kinetis SDK, the predecessor of MCUXpresso SDK.

## 2.2.1_rev8 (newest)
### New features:
- Applied patch #10465: Overall altcp_tls_mbedtls fixes and enhancements (https://savannah.nongnu.org/patch/?10465):
  - Copy received TCP flags
  - Free client's TLS config on dealloc
  - Fix abort management. Fix to return ERR_ABRT on altcp_abort(). Add check to altcp_mbedtls_abort().
  - Free SSL cache and session tickets on tls_free_config()
  - Mark freed members to prevent double free.
  - Correctly close and free LISTEN connections
  - Use handshake steps rather than a single blocking call
  - Add mbedtls return values.
  - free rx_app on dealloc
  - correct ALTCP_MBEDTLS_PLATFORM_ALLOC configuration
  - Manage write apiflags
  - Port to mbedtls v3
  - Fix the return value of altcp_mbdtls_sndbuf when the underlying tcp sndbuf is consumed by ssl expansion bytes.
- Switched from mbedTLS 2.x to mbedTLS 3.x:
  - Updated Kconfig dependencies.
  - Updated altcp_tls_mbedtls layer to use PSA crypto random number generator abstraction instead of ctr_drbg, which would be initialized twice.
  - Updated NXP web server (src/apps/httpsrv).
### Bug fixes:
- Fixed sys_sem_new in FreeRTOS porting layer:
  - If the initial count argument was 1, the newly created semaphore was not signalled from the start.
  - The initial count should be just 0 or 1 according to lwIP documentation, but the implementation allowed
    it to be more than 1 (counting semaphore). Fixed to allow only binary semaphore behavior.
  - `src/apps/httpsrv` updated to use FreeRTOS API directly where it cannot use sys_sem_t as a counting
    semaphore anymore.

## 2.2.1_rev7
### New features:
- Operating system abstraction layer: the default implementation of the sys_msleep function (using a temporary
  allocated semaphore) is overwritten by a new implementation in the adaptation layer (using vTaskDelay on FreeRTOS).
### Bug fixes:
- ENET QoS adaptation layer: initialize the CSR (control and status register) clock field in the driver configuration.
  This fixes the bug with the number of clock ticks for one-microsecond reference timer being set incorrectly.
- Ethernet adaptation layers: Updated logic in ethernetif_pbuf_free_safe to allow the thread to sleep between
  attempts to schedule pbuf_free on tcpip_thread. Previously, when ETH_RX_TASK_PRIO was higher than tcpip_thread
  priority and scheduling failed due to resource constraints, ethernetif_pbuf_free_safe would loop indefinitely,
  preventing tcpip_thread from freeing those resources. Changes include:
  - Added thread sleep between scheduling attempts to prevent infinite loops
  - Removed looping on schedule failure in interrupt context (now asserts instead)
  - Added direct pbuf_free calls when LWIP_ALLOW_MEM_FREE_FROM_OTHER_CONTEXT is enabled

## 2.2.1_rev6
### New features:
- Support for on-demand timers for some of the protocols. Reused from Espressif open source lwIP port.
  LwIP default protocol timer handlers are invoked periodically, even if there is nothing to do.
  This is not suitable for low-power applications entering into a sleep mode. The added feature
  calculates the time when the timer callback should be invoked, instead of waking-up periodically
  to only check if there is anything to do or not.
  The feature is turned off by default and can be enabled by the following options:
  - LWIP_IP4_REASSEMBLY_TIMERS_ONDEMAND
  - LWIP_IP6_REASSEMBLY_TIMERS_ONDEMAND
  - LWIP_DNS_TIMERS_ONDEMAND
  - LWIP_DHCP_FINE_TIMERS_ONDEMAND
  - LWIP_IGMP_TIMERS_ONDEMAND
  - LWIP_MLD6_TIMERS_ONDEMAND

  Note that other protocols don't have on-demand timers implemented, so for example if there are
  open TCP connections, its protocol timers will be still invoked periodically.
- NETC adaptation layer: Possibility to use switch port, see NETC_USE_SWT and related options.
- New option ETH_USE_GPIO_ADAPTER which can be used on platforms without GPIO adapter to bypass the code using it.
- New option ETH_ENET_QOS_MII_MODE to override default MII mode for ENET QoS.

## 2.2.1_rev5
### New features:
- Iperf now tries to place buffers into fast memory.

## 2.2.1_rev4
### New features:
- Enabled build of `ALTCP_TLS`:
  - This enables option to use `ALTCP` with mbedTLS for transparent TLS.
  - Added new `MCUX_COMPONENT_middleware.lwip.apps.altcp_tls` component to Kconfig.
  - Updated CMake to compile the source files if the component is enabled.
  - Added `ALTCP_MBEDTLS` configuration settings to Kconfig.
- Added support for NETC VSI pre-init.
- Removed unused files with LGPL license or unknown origin.
### Bug fixes:
- `src/apps/altcp_tls`: fixed compilation when `MBEDTLS_HAVE_TIME` is not defined.
- `src/core/ipv6/mld6.c`: fixed the setting of MLDv6 general report delay.
  Especially when the maximum response delay was smaller than `MLD6_TMR_INTERVAL`,
  it could be assigned to a value out of the requested range, or end up not sending
  the report at all.
- Fixed ethernet transmission issue on USB (not handling chained pbufs).

## 2.2.1_rev3
### New features:
- Added optional ability to generate lwIP options header file from Kconfig. The feature
  has been introduced to align with the changes in MCUXpresso SDK. It can be enabled
  when using Kconfig and lwIP is used directly from repository, for example when using
  `west build` command to generate or build projects. It cannot be used when working
  with SDK packages, but since lwIP examples (in `lwip_examples` folder) have been switched
  to this approach in repository, you can find pregenerated `lwipopts_gen.h` files in SDK
  packages (note that if you want to add/change some of the pregenerated settings, do it
  in `lwipopts_gen.h`, not `lwipopts.h`).

  The optional feature is disabled by default and can be enabled by setting
  `CONFIG_MCUX_PRJSEG_middleware.lwip.generated_config=y` in `prj.conf`. When enabled,
  lwIP options are configured using the Kconfig system, usually written in `prj.conf`
  file(s) or `.config` file with `CONFIG_` prefix. Kconfig system uses this information
  to generate `lwipopts_gen.h` during build phase. It is included from `lwipopts.h`
  (from `template/kconfig`). The `lwipopts.h` file is not intended to be edited, it does
  preprocessing of the values from `lwipopts_gen.h`.
  
  The list of options, its types, default values and description is in the file
  named `Kconfig`, under the `menu "Options (lwipopts_gen.h)"`.
  
  Note that there are some differences from using manually edited `lwipopts.h`:
  
    - Use `CONFIG_` prefix in Kconfig, so for example `CONFIG_LWIP_IPV6` instead of
      `LWIP_IPV6`. In the generated header, the prefix is removed.
  
    - For boolean options, use `y` or `n` instead of `0` and `1` in Kconfig.
      For example `CONFIG_LWIP_IPV6=y` instead of `#define LWIP_IPV6 1`.
  
    - Default values for some integer options are using formulas referencing other
      options. For example, `TCP_WND` default is `(4 * TCP_MSS)`. Kconfig does not
      support this, so this option has string type instead of int. The consequence
      is that you must write quotes in Kconfig for such settings, even if entering
      integer value, for example `CONFIG_TCP_WND="5840"`.
      There are more such options, please pay attention to the type definition in
      Kconfig and any Kconfig warnings in the output of processing.
  
    - Default values for some options are referencing other options, without using
      formulas. For example `TCP_TTL` defaults to `IP_DEFAULT_TTL`. But since Kconfig
      translates defaults to values, if you change `IP_DEFAULT_TTL` later, you may
      need to override the `TCP_TTL` to the same value as well (if you still want
      it the same as `IP_DEFAULT_TTL`).
  
    - Options which are function macros, taking no parameters, have a single
      space between the braces. For example the following written in manually
      edited `lwipopts.h`:

      `#define LWIP_ASSERT_CORE_LOCKED() sys_check_core_locking()`

      would be like this in Kconfig instead:

      `CONFIG_LWIP_ASSERT_CORE_LOCKED( )="sys_check_core_locking()"`
  
    - Some functions macros are used in the lwIP code if they are defined.
      Because Kconfig generates string options with empty value, it would result
      in a compilation error. Therefore, such options have `_CUSTOM` suffix in Kconfig
      and the final option is hidden, without a prompt, generated only when the one
      with the `_CUSTOM` suffix does not have empty string value. Example is
      `LOCK_TCPIP_CORE` option, where it could be written as
      `CONFIG_LOCK_TCPIP_CORE_CUSTOM( )="sys_lock_tcpip_core()"` instead of
      `CONFIG_LOCK_TCPIP_CORE( )="sys_lock_tcpip_core()"`.
  
    - Function signatures cannot be written in the generated `lwipopts_gen.h`.
      But it can be written in `CONFIG_LWIP_OPT_H_CODE_LINE_N` options, where
      `N` is a line number. It will appear in `template/kconfig/lwipopts.h` before
      `lwipopts_gen.h` is included. That way the signatures will be visible for
      the code which uses the function macros. For example the options to bring
      some function signatures from NXP porting layer could be entered like this
      for FreeRTOS projects:

      ```
      CONFIG_LWIP_OPT_H_CODE_LINE_1="void sys_lock_tcpip_core(void);"
      CONFIG_LWIP_OPT_H_CODE_LINE_2="void sys_unlock_tcpip_core(void);"
      CONFIG_LWIP_OPT_H_CODE_LINE_3="void sys_check_core_locking(void);"
      CONFIG_LWIP_OPT_H_CODE_LINE_4="void sys_mark_tcpip_thread(void);"
      ```

      or for bare-metal projects:

      `CONFIG_LWIP_OPT_H_CODE_LINE_1="void sys_check_core_locking(void);"`

      Note that the "line numbers" must be used from 1 and numbers cannot be skipped.
      So it is not possible to comment or delete `CONFIG_LWIP_OPT_H_CODE_LINE_2` and
      continue with `CONFIG_LWIP_OPT_H_CODE_LINE_3`, all must be renumbered.
      The function signatures must use only basic C data types, no data types or
      typedefs from stdint.h or lwIP etc. There might be a cast used in the function
      macro option if needed.
  
    - Some boolean options, which are just defined without value in C code, must still
      be written with a value. Example is `LWIP_DEBUG`, it would be written as
      `CONFIG_LWIP_DEBUG=y` or `CONFIG_LWIP_DEBUG=n`. Other such options are
      `LWIP_PROVIDE_ERRNO` and `LWIP_ENET_FLEXIBLE_CONFIGURATION`.
  
    - Note that Kconfig does not generate boolean values if they are set to "n".
      The file `template/kconfig/lwipopts.h` defines the non-generated options to 0,
      so that lwIP does not define them again to 1 (if it has such default value
      in its code).
  
    - When using Kconfig, do not define `LWIP_RAND`. It's function macro is defined
      in the porting layer in `cc.h`. See `LWIP_DEFINE_LWIP_RAND_IN_CC_H` there and in
      Kconfig.
  
    - All options from `opt.h` and some of the applications and porting layer options
      have been enabled Kconfig. If you find something missing, such macro can be
      added to a project command line or manually edited in `lwipopts_gen.h` (if it is
      used and will not be regenerated).
  
    - Formatting macros like `X8_F`, `U16_F`, etc. are not present in Kconfig system
      and does not need to be put into `lwipopts.h`, see below.
  
  - Default values for formatting macros like `X8_F`, `U16_F`, etc. are defined in `cc.h`,
    so it does not need to be entered in `lwipopts.h`.

### Bug fixes:
- `src/apps/mdns`: `mdns_resp_remove_netif` stops all timeouts related to the netif now.
  This prevents the timeout handlers to send answers on netifs which already have mDNS
  responder disabled and prevents access to deallocated structures. It also prevents
  excessive number of timeouts being scheduled when mDNS responder is disabled and
  reenabled on netif quickly.

## 2.2.1_rev2
### Bug fixes:
- `src/apps/lwiperf`: Fixed calculation of the number of UDP datagrams to transmit.
  When the desired rate was high and datagram size small, the ideal delay between
  each transmitted datagram could be smaller than 1 microsecond. The value was
  truncated to zero and resulted in zero-division and/or no UDP transmitted at all.
- `port/arch/cc.h`: code which checks for the presence of Newlib standard C library
  adds import of a standard header first so the `__NEWLIB__` macro is always available
  when Newlib is used. This prevents `LWIP_TIMEVAL_PRIVATE` from having different values
  in one program.

## 2.2.1_rev1
### New features:
- Ported lwIP 2.2.1.dev (2024-02-19, branch: master, SHA-1: d0efd9ef7ba08e54b46b1060e2b4629a4907391b) to MCUXpresso SDK.
- Added `ETH_MAX_RX_PKTS_AT_ONCE` macro. See [port/README.md](port/README.md) for details.
- In `port/netc_ethernetif.c`, added `NETC_VSI_NUM_USED` macro to support using VSI. A thread of SI
  message handling will be started to handle VSI-PSI messages.
### Bug fixes:
- Added the missing implementation for `IP_FORWARD_ALLOW_TX_ON_RX_NETIF` option in the function ip6_forward.
  Therefore, IPv6 packets could be sent back out on the netif where they were originally received from.
- NETC adaptation layer: Do not call `xEventGroupSetBits` from ISR.
- Ethernet adaptation layers: Default value of priority of the receive task (`ETH_RX_TASK_PRIO`) is set lower
  than the priority of the FreeRTOS daemon task (timer task).

## 2.2.0_rev11
### New features:
- NETC adaptation layer: Possible to disable IPv4/TCP/UDP checksum validation done in HW.
- EtherCAT EoE(Ethernet over EtherCAT) driver is added to lwip.
### Bug fixes:
- `src/apps/httpsrv/httpsrv_supp.c`: Fixed performing of the HTTP server task priority limitation.

## 2.2.0_rev10
### New features:
- Ported lwIP 2.2.0 (2023-09-25, branch: master, SHA-1: 0a0452b2c39bdd91e252aef045c115f88f6ca773, tag: STABLE-2_2_0_RELEASE) to MCUXpresso SDK.
- Enabled hardware-accelerated CRC computation and verification (MAC, IPv4, TCP, UDP, ICMPv4, ICMPv6) for ENET Kinetis, ENET QoS and ENET LPC.
- Enabled link state detection based on PHY interrupts. 
  The `ETH_LINK_POLLING_INTERVAL_MS` macro controls this - setting it to 0 and specifying `ethernetif_config_t->phyIntGpio` enables it, setting it to a value greater
  than zero enables polling instead. Supported only under an RTOS (`NO_SYS == 0`). By default, the link state is polled.
- ND6: Implemented RFC 4191 type C host, which means default router list (learned from Router Advertisement messages)
  has been replaced with routing table, which contains default route records for each router and also routes learned
  from received Route Information Options. Changes partially based on [patch 10114](https://savannah.nongnu.org/patch/?10114).
  The option `LWIP_ND6_NUM_ROUTERS` has been removed, and the new option `LWIP_ND6_NUM_ROUTES` has been added
  to configure the size of the routing table.
- IPv6: Implemented a new hook - `LWIP_HOOK_IP6_CANFORWARD`. This hook can be used, for example, for multicast 
  forwarding between netifs. Defining this hook enables multicast traffic forwarding, thus the hook is also invoked
  for multicast traffic.
- MLD6: Multicast Listener Discovery v1 replaced by v2 (RFC 3810) but without support of source specific multicast.
- `port/enet_ethernetif_kinetis.c`: Added check to generate/validate ICMPv6 checksum in SW as the Kinetis ENET peripheral does not do it.
- Added disabling of Rx interrupt when the port is out of Rx buffers. See [port/README.md](port/README.md) for more details.
### Bug fixes:
- `src/apps/lwiperf`: Fixed access to invalid data when UDP report is to be sent from a timer but abort has been called before.
- `src/apps/lwiperf`: Fixed deallocation of TCP server started by client (in reverse or dual modes) which failed to connect.
- `port/netc_ethernetif.c`: Fixed cache control enablement macro (`FSL_SDK_ENABLE_DRIVER_CACHE_CONTROL > FSL_ETH_ENABLE_CACHE_CONTROL`).
- `port/sys_arch.c`: The function `sys_assert` does not call `portENTER_CRITICAL` when called from an interrupt.
- `src/core/ipv4/ip4.c`: Fixed checksum reset condition.
- ND6:
  - Lladdr length is now taken from netif->hwaddr_len so ND6 works properly regardless of `NETIF_MAX_HWADDR_LEN`.
  - Added check of sufficient length of lladdr options from incoming messages.
- `src/apps/httpsrv/httpsrv.c`: Fixed hangup in `HTTPSRV_release` if caller's task has higher priority than server task.
- `port/arch/cc.h`: LWIP_PLATFORM_DIAG is defined (and can be overridden) independently of the `LWIP_DEBUG` setting.
  Removed printing extra newline symbols from LWIP_PLATFORM_DIAG.
- `src/apps/lwiperf`: The "end of test" UDP datagram is resent more often.
  This increases the probability of the server to receive it and end the test when datagrams are getting lost.
- Added [port/README.md](port/README.md) describing possible settings and helper functions in the port layer.

## 2.2.0_rev9
### New features:
- Ported lwIP 2.2.0.dev (2023-01-03, branch: master, SHA-1: 3fe8d2fc43a9b69f7ed28c63d44a7744f9c0def9) to MCUXpresso SDK.
- Applied patch to allow sending IPv6 router advertisement. Improved to allow selection of interface and router life time 
  and to allow sending route information options.
- `src/apps/lwiperf`: Support for reverse test (client receives, server sends). Requires iperf version 2.1.0 or newer.
### Bug fixes:
- `src/apps/httpsrv`: Fixed operation with `LWIP_IPV6` enabled. Server can be also accessed using both IPv4 and IPv6 at the same
  time if compiled with both `LWIP_IPV4=1` and `LWIP_IPV6=1`.
  Note the type of the field struct httpsrv_param_struct.address has changed from struct sockaddr to struct sockaddr_storage.

## 2.2.0_rev8
### New features:
- `src/apps/lwiperf`: Added new parameter "buffer_len" to functions `lwiperf_start_tcp_client()` and `lwiperf_start_udp_client()`
  to configure TCP/UDP packet size.
- `src/apps/lwiperf`: Added new parameter "tos" to functions `lwiperf_start_tcp_client()` to configure TCP packet priority.
- NETC adaptation layer: Not forcing the RX/TX buffers placement in non-cacheable memory.
  Requires the symbol FSL_ETH_ENABLE_CACHE_CONTROL to be defined on project level if the memory region,
  where the buffers are placed by a linker, has cache enabled.
### Bug fixes:
- `src/apps/httpsrv`: Added missing includes.
- `src/apps/lwiperf`: Fixed TCP client to send settings at the beginning of each 128 KB block like the PC iperf 2.0.x application does.
- `src/apps/lwiperf`: Fixed validation of TCP received data (with LWIPERF_CHECK_RX_DATA enabled, works with iperf 2.0.x).
- `src/apps/lwiperf`: Fixed lwiperf_list_remove() to clear references to the removed item.
- `src/apps/lwiperf`: Program does not assert when buffer cannot be cloned in UDP test, only "can't clone buffer" message is printed.

## 2.2.0_rev7
### New features:
- Ported lwIP 2.2.0.dev (2022-05-09, branch: master, SHA-1: 239918ccc173cb2c2a62f41a40fd893f57faf1d6) to MCUXpresso SDK.
- Added function `ethernetif_probe_link()` which reads actual link, speed and duplex settings from phy and passes them to driver.
  Stack could be set to call this function periodically by setting `ETH_LINK_POLLING_INTERVAL_MS` to value higher than zero.
- Added helper functions `ethernetif_wait_linkup()` and `ethernetif_wait_ipv4_valid()` to allow blocking of RTOS task or 
  bare metal application until link is up or IPv4 address becomes valid.
- Added NETC adaptation layer.
- Processing of rx packets under RTOS moved from ISR to a separate task to improve system reaction times.
  Switch back to old behavior can be done by setting `ETH_DO_RX_IN_SEPARATE_TASK` macro to 0.
### Bug fixes:
- `port`: Fixed copying of pbuf contents. Previous code was using an incorrect end condition and could result
  in the overrun of the destination buffer if more packets were on the queue.
- `port`: Delegating `pbuf_free` calls to `tcpip_thread` via `pbuf_free_callback` where possible (RTOS),
  ensuring `pbuf_free` is not called from interrupt context when `LWIP_ALLOW_MEM_FREE_FROM_OTHER_CONTEXT` is not set (bare metal).
- `port/enet_ethernetif_qos.c` - Fixed `ENET_RXBD_NUM` which was used instead of `ENET_TXBD_NUM`.
- `port/enet_ethernetif_qos.c` - Fixed buffer alignment to be at least 64.
- `src/apps/lwiperf`: Fixed IPv6 TCP TX throughput lower than IPv4 by modifying maximum segment size
  to avoid sending two segments instead of one.
- `src/apps/lwiperf`: Out-of-order datagrams in UDP RX server mode are counted to the throughput.
- `src/apps/httpsrv`: Implemented receive timeouts on sockets.
- `src/apps/httpsrv`: Don't assert on HTTP session task creation failure.
- `src/apps/httpsrv`: Fixed build with IPv6 enabled.
- `src/apps/httpsrv`: Updated endianess macros required for websocket SHA generation.
- `src/apps/httpsrv`: Added missing includes.

## 2.2.0_rev6
### New features:
- Ported lwIP 2.2.0.dev (2022-03-25, branch: master, SHA-1: 124dc0a64ef5d7c14a27e3115e5888df6559cb72) to MCUXpresso SDK.
- Implemented leaving of multicast groups on ENET and ENET QOS.

## 2.2.0_rev5
### New features:
- Ported lwIP 2.2.0.dev (2021-05-11, branch: master, SHA-1: 7ec4e9be304e7f8953740f10b2c810a292e89449) to MCUXpresso SDK.
- LPC ENET adaptation layer allocates more buffers for frame reception now.
  Previously the number of receive buffers was determined by `ENET_RXBD_NUM`, which defaults to 5.
  It is determined by `ENET_RXBUFF_NUM` now, which is `2 * ENET_RXBD_NUM` by default.
  Increase was needed because the actual version of LPC ENET driver always hold `ENET_RXBD_NUM` number of buffers
  and few additional buffers are needed for passing zero-copy frame data to lwIP.
  If this takes too much memory in your application, you can counteract by decreasing `PBUF_POOL_SIZE`,
  since `PBUF_POOL` is used only for transmission when LPC ENET, Kinetis ENET or ENET QOS is used.

## 2.2.0_rev4
### New features:
- Ported lwIP 2.2.0.dev (2021-03-05, branch: master, SHA-1: 0056522cc974d2be2005c324f37187b5b3695765) to KSDK 2.0.0.
- `LWIP_DHCP_DOES_ACD_CHECK` option default changed to 0 (disabled):
  - Although the ACD check makes getting IP address from DHCP more robust, it added several seconds delay at startup of all applications which use DHCP.
  - This feature was not present in earlier versions of lwIP.
- ENET QOS adaptation layer - implemented zero-copy on receive.
- Kinetis ENET and ENET QOS adaptation layers allocate more buffers for frame reception now.
  Previously the number of receive buffers was determined by `ENET_RXBD_NUM`, which defaults to 5.
  It is determined by `ENET_RXBUFF_NUM` now, which is `2 * ENET_RXBD_NUM` by default.
  Increase was needed because the actual version of Kinetis ENET and ENET QOS drivers always hold `ENET_RXBD_NUM` number of buffers
  and few additional buffers are needed for passing zero-copy frame data to lwIP.
  If this takes too much memory in your application, you can counteract by decreasing `PBUF_POOL_SIZE`,
  since `PBUF_POOL` is used only for transmission when Kinetis ENET or ENET QOS is used.
- Removed `ethernetif_config_t.non_dma_memory` field which was required to configure memory ranges unusable by ENET DMA on LPC devices.
  The setting has been replaced by `BOARD_ENET_NON_DMA_MEMORY_ARRAY` macro.

## 2.2.0_rev3
### New features:
- Ported lwIP 2.2.0.dev (2020-07-07, branch: master, SHA-1: c385f31076b27efb8ee37f00cb5568783a58f299) to KSDK 2.0.0.

## 2.2.0_rev2
### New features:
- Kinetis ENET adaptation layer - implemented zero-copy on receive.
- lwiperf - counter of transferred bytes extended from 32 to 64 bit
### Bug fixes:
- Fixed restarting Auto IP from DHCP.

## 2.2.0_rev1
### New features:
- Ported lwIP 2.2.0.dev (2019-12-12, branch: master, SHA-1: 555812dcec38c9a2ef1ef9b31816291549fbf9f8) to KSDK 2.0.0.
- Implemented `LWIP_ASSERT_CORE_LOCKED` related functions in `sys_arch.c`. It can be enabled in `lwipopts.h`:
  ```c
  #define LWIP_ASSERT_CORE_LOCKED() sys_check_core_locking()
  #define LWIP_MARK_TCPIP_THREAD() sys_mark_tcpip_thread() // if NO_SYS == 0
  #define LOCK_TCPIP_CORE() sys_lock_tcpip_core() // if NO_SYS == 0 and LWIP_TCPIP_CORE_LOCKING == 1
  #define UNLOCK_TCPIP_CORE() sys_unlock_tcpip_core() // if NO_SYS == 0 and LWIP_TCPIP_CORE_LOCKING == 1
  ```

## 2.1.2_rev5
### New features:
- Implemented `TCP_USER_TIMEOUT` socket option.
- Implemented `SIOCOUTQ` ioctl.

## 2.1.2_rev4
### New features:
- Ported lwIP 2.1.3.dev (2019-02-27, branch: STABLE-2_1_x, SHA-1: 1bb6e7f52de1cd86be0eed31e348431edc2cd01e) to KSDK 2.0.0.
- Updated sys_thread_new implementation and comment.
- Kinetis ENET adaptation layer - reading frames into a pbuf chain is conditionally compiled only when a single pbuf from pool
  cannot hold maximum frame size (`PBUF_POOL_BUFSIZE` >= maximum frame size).
  Avoiding this code also reduces stack size requirements by about 1.5 kilobytes.
### Bug fixes:
- Fixes in `ethernetif_linkoutput()` in `enet_ethernetif_lpc.c`:
  - Removed access to possibly freed pbuf.
  - Call `pbuf_free()` when transmit buffers not available.
  - When copying pbuf chain, updating the number of necessary transmit buffers to wait for, which can be often smaller in the copy.
- When CGI script is reading POST data by chunks, the loop in `httpsrv_read()` may cause blocking in receive function waiting for more data at the end of the stream
  - `HTTPSRV_cgi_read()` - added limiting of the last chunk length according to content length to avoid undesired blocking
- Applied `AUTOIP` [patch 9847](https://savannah.nongnu.org/patch/?9847) - with modification to support multiple network interfaces.
- Fixed buffer overflow in `httpsrv` when application provided CGI script does not handle the whole content of POST request
  - Removed `LwipMibCompiler` contrib application as it contained LGPL licensed files in `SharpSnmpLib`.

## 2.1.2_rev3
### New features:
- lwiperf updated with UDP client/server support from the [patch 9751](https://savannah.nongnu.org/patch/?9751)

## 2.1.2_rev2
### Bug fixes:
- Fixed `lwiperf_abort()` in `lwiperf.c` to correctly close connections and free resources

## 2.1.2_rev1
### New features:
- Ported lwIP 2.1.2 (2018-11-22, SHA-1: 159e31b689577dbf69cf0683bbaffbd71fa5ee10) to KSDK 2.0.0.
- Ported lwIP-contrib 2.1.0 (2018-09-24, SHA-1: 35b011d4cf4c4b480f8859c456587a884ec9d287) to KSDK 2.0.0.

## 2.0.3_rev1
### New features:
- Ported lwIP 2.0.3 (2017-09-15, SHA-1: 92f23d6ca0971a32f2085b9480e738d34174417b) to KSDK 2.0.0.

## 2.0.2_rev1
### New features:
- Ported lwIP 2.0.2 (2017-03-13, SHA-1: c0862d60746e2d1ceae69af4c6f24e469570ecef) to KSDK 2.0.0.
      
## 2.0.0_rev3
### New features:
- Ported lwIP 2.0.0 (2016-11-10, SHA-1: 216bf89491815029aa15463a18744afa04df58fe) to KSDK 2.0.0.
      
## 2.0.0_rev2
### New features:
- Ported lwIP 2.0.0 RC2 (2016-08-08, SHA-1: b1dfd00f9233d124514a36a8c8606990016f2ad4) to KSDK 2.0.0.
      
## 2.0.0_rev1
### New features:
- Ported lwIP 2.0.0 RC0 (2016-05-26) to KSDK 2.0.0. 
- Changed lwIP bare-metal examples to use poll-driven approach instead of interrupt-driven one.
      
## 1.4.1_rev2
### New features:
- Enabled critical sections in lwIP.
### Bug fixes:
- Fixed default lwIP packet-buffer size to be able to accept a maximum size frame from the ENET driver.
- Fixed possible drop of multi-frame packets during transmission.
 
## 1.4.1_rev1
### New features:
- Ported lwIP 1.4.1 to KSDK 2.0.0.
