# MiraMesh with FreeRTOS

An example of using MiraMesh with FreeRTOS.
This is a combination of Nordic Semiconductor's SDK's blinky_freertos example,
MiraOS' blink_sync_client and network_sender example.

Nordic Semiconductor's nRF52 SDK 17.0.2 and Mira, version 2.6.0 or later is needed
to build the example.

## How to build, flash and run

The example runs on Nordic's nRF52832 and nRF52840 development boards.

Connect two of them to the computer. Both cards need to have valid
Mira-licenses flashed to them.

Start one and run:
```sh
make GNU_INSTALL_ROOT=$(dirname $(which arm-none-eabi-gcc))/ flash-with-sd
```

For nRF52840 instead run:
```sh
make GNU_INSTALL_ROOT=$(dirname $(which arm-none-eabi-gcc))/ TARGET=nrf52840 flash-with-sd
```

The command might also need SDK_ROOT=/path/to/nordics/sdk and MIRA_LIB=path/to/libmira

This flashes the card with this example and SoftDevice.
Use target `flash` to just flash this example.

Start another card running the network_receiver example (either the MiraOS version or the FreeRTOS version).

Two LEDs will shortly start to blink. A third LED will start
to blink once the node has joined the mesh-network. The network_receiver
node will then start to receive messages from this node and print
them to its serial port.


## Limitations

* The FreeRTOS configuration isn't optimised in any way.
* The UART output busywaits for each character to be written.

## Overview

The integration parameters are configured in `config/MiraMeshConfig.h`
and the code is in the `../nrf_common/miramesh_integration` directory.

`network_sender_task.c` is the example application using MiraMesh, `main.c` is almost
unchanged from the blinky_freertos example.

`main` starts the MiraMesh integration and the MiraMesh using application.
The MiraMesh application waits until the integration is ready, then
starts MiraMesh and starts using it.
