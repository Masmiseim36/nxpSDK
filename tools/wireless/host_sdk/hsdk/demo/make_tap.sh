#!/bin/bash

# IMPORTANT: Make sure SERIAL_TAP is set on 1 in Thread_KW_Tun.c and
#            THR_SERIAL_TUN_ENABLE_ND_HOST (former SERIAL_TAP_ROUTER)
#            is set on TRUE in app_serial_tun.h

# Delete existing tunnel
ip link del dev threadtap0
# Create a new TAP interface for Thread interaction.
ip -6 tuntap add mode tap threadtap0
# Assign it an IPv6 address.
ip -6 addr add fd01::2 dev threadtap0
# Add route to the default address of Serial TAP embedded interface.
ip -6 route add fd01::1 dev threadtap0
# Add route to the default Unique Local /64 Prefix via threadtap0. Add next hop to avoid NS.
# Using this ULA space when no DHCPv6-PD router is available on-link.
ip -6 route add fd01:0000:0000:3ead::/64 via fe80::260:37ff:fe00:fa5c dev threadtap0
# The interface is ready.
ip link set threadtap0 up

# Enable IPv6 routing on host.
sysctl -w net.ipv6.conf.all.forwarding=1

# How to bridge the TAP interface with the uplink interface in OpenWrt
# brctl addif br-lan threadtap0
# brctl show
# /etc/init.d/odhcpd restart

# How to bridge the TAP interface with the uplink interface in Linux
# brctl addbr br0
# brctl addif br0 ethX
# brctl addif br0 threadtap0
# ifconfig br0 up
