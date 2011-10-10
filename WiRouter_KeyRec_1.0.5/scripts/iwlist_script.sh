#!/bin/bash
#
# This script uses iwlist to scan and test
# the networks.
#
# Thanks to Mancausoft for the regular expression.


if [ $# -lt 1 ]; then
    echo -ne "\nUsage: ${0} wireless_interface (example: wlan0)\n\n"
    exit
fi

if [ ! -e "/usr/local/bin/wirouterkeyrec" ]; then
    echo -ne "\nError: WiRouter KeyRec is not installed!\n\n"
    exit
fi

if [[ $EUID -ne 0 ]]; then
  echo -ne "\nError: You must be a root user in order to use the wireless inferface.\n\n"
  exit
fi

#iwlist $1 scan | grep -e ESSID -e Address | sed 'N
#s/\n//;s/.*Address\: \([0-9:A-F]*\).*"\(.*\)"/\2\/\1/' > .wpa_ssid_list
iwlist $1 scan | grep SSID | sed 's/.*"\(.*\)".*/\1/' > .wpa_ssid_list
wirouterkeyrec -f .wpa_ssid_list
rm .wpa_ssid_list

