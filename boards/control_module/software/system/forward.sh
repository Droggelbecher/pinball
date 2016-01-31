#!/bin/bash

sysctl net.ipv4.ip_forward=1

ip addr add 10.0.0.1/24 broadcast 10.0.0.255 dev eth0


iptables -t nat -A POSTROUTING -o wlan0 -j MASQUERADE
iptables -A FORWARD -m conntrack --ctstate RELATED,ESTABLISHED -j ACCEPT
iptables -A FORWARD -i eth0 -o wlan0 -j ACCEPT
