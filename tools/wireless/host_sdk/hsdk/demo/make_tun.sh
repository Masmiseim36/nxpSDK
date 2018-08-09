#!/bin/bash

# Delete existing tunnel
ip link del dev threadtun0
# Create a new TUN interface for Thread interaction.
ip -6 tuntap add mode tun threadtun0
# Assign it a global IPv6 address.
ip -6 addr add fd01::2 dev threadtun0
# Add route to default address of Serial TUN embedded interface.
ip -6 route add fd01::1 dev threadtun0
# Add route to Unique Local /64 Prefix via threadtun0.
ip -6 route add fd01:0000:0000:3ead::/64 dev threadtun0
# The interface is ready.
ip link set threadtun0 up

# Enable IPv6 routing on host.
sysctl -w net.ipv6.conf.all.forwarding=1
