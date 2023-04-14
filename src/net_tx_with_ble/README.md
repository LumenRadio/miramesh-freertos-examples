Network sender with BLE
=======================

Shows how to set up a mesh node.

This example sends packets to root on UDP port 456 and prints them out.

Intended to be matched with Network receiver / net_rx in this repository, or the
network_receiver example for MiraOS shipped with Mira distribution.

It also provides concurrent BLE connectivity, integrated with an example
originally from Nordic.

Since BLE uses many drivers from nrf5-sdk, there are more includes in to be
found in `make/targets/net_tx_with_ble-nrf52840.mk`. It also requires an
implementation of `app_timer` that integrates with FreeRTOS and MiraMesh, which
can be found in `app_timer_freertos.c` in this directory, that overrides the
original `app_timer`.

This is intended to be installed on a nrf52840-dk development kit

Compilation and flashing
------------------------

See README.md in top level directory of the repository
