#!/bin/bash

set -e

all_eths() {
  ip link | sed -rne 's/.*: (eth[0-9]+).*/\1/p'
}

active_eth() {
  ip a | awk '
      BEGIN {
        eth=""
        addr=""
      }
      /.*eth[0-9]+/ {
        eth=gensub(":","","",$2)
        if (/LOWER_UP/) {
          print eth
          exit 0
        }
      }
      $1 == "inet6" && $4 == "link" {
        print eth
      }'
}


# set link up on all eths
all_eths | xargs -n1 -i{} ip link set up dev {}

# wait a second
sleep 1

# start dhcp on an active one
active=$(active_eth)
echo $active
dhclient $active
