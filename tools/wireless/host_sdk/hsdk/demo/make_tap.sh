#!/bin/bash

# IMPORTANT: Use Thread_Shell executable or make sure SERIAL_TAP is set to 1 in 
#            Thread_KW_Tun.c. SERIAL_TUN_IF and BR_ROUTER_MODE or BR_HOST_MODE must 
#            be enabled in config.h of host control project.

# Delete existing tunnel
ip link del dev threadtap0
# Create a new TAP interface for Thread interaction.
ip -6 tuntap add mode tap threadtap0
# The interface is ready.
ip link set threadtap0 up

# Deprecated - used for old TUN mode, TAP mode uses ND Router advertisements to 
# learn route to Thread Nwk and to configure ULA address.
# Assign it an IPv6 address.
#ip -6 addr add fd01::2 dev threadtap0
# Add route to the default address of Serial TAP embedded interface.
#ip -6 route add fd01::1 dev threadtap0
# Add route to the default Unique Local /64 Prefix via threadtap0. Add next hop to avoid NS.
# Using this ULA space when no DHCPv6-PD router is available on-link.
#ip -6 route add fd01:0000:0000:3ead::/64 via fe80::260:37ff:fe00:fa5c dev threadtap0

# Enable IPv6 routing on host and make it accept RA from Thread Border router. 
# This is needed when Thread BR is used in BR_ROUTER_MODE.
sysctl -w net.ipv6.conf.all.forwarding=1
sysctl -w net.ipv6.conf.threadtap0.accept_ra=2
sysctl -w net.ipv6.conf.threadtap0.accept_ra_rt_info_max_plen=64 

# Bridging is used in BR_HOST_MODE. Configuration information will come 
# from the external bridged interface.

# How to bridge the TAP interface with the uplink interface in OpenWrt
# brctl addif br-lan threadtap0
# brctl show
# /etc/init.d/odhcpd restart

# How to bridge the TAP interface with the uplink interface in Linux
# brctl addbr br0
# brctl addif br0 ethX
# brctl addif br0 threadtap0
# ifconfig br0 up
