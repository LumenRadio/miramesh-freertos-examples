# MiraMesh with FreeRTOS

An example of using MiraMesh with FreeRTOS.
This is a combination of Nordic Semiconductor's SDK's blinky_freertos example,
MiraOS' blink_sync_server and network_receiver example.

Nordic Semiconductor's nRF52 SDK 15.2.0 and Mira, version 2.4.0 or later is needed
to build the example.

## How to build, flash and run

The example runs on Nordic's nRF52832 and nRF52840 development boards.

Connect two of them to the computer.

Start one and run:
```sh
make GNU_INSTALL_ROOT=$(dirname $(which arm-none-eabi-gcc))/ flash-with-sd
```

For nRF52840 instead run:
```sh
make GNU_INSTALL_ROOT=$(dirname $(which arm-none-eabi-gcc))/ TARGET=nrf52840 flash-with-sd
```

The command might also need SDK_ROOT=/path/to/nordics/sdk and MIRA_LIB=path/to/libmira

This compiles and flashes one of the cards with this example and SoftDevice.
Use target `flash` to flash this example without overwriting the SoftDevice.

Two LEDs will shortly start to blink. A third LED will start
to blink once the node has configured the mesh-network.
The network_receiver node will print received messages.


## Limitations

* The FreeRTOS configuration isn't optimised in any way.
* The UART output busywaits for each character to be written.

## Overview

Mira version 2.4.0 and 2.4.1 doesn't use the VFP ABI, that is,
it doesn't use the hardware floating point ABI. Nordics' FreeRTOS port
in the SDK requires VFP, so `port.c` and `port_cmsis.c` have been modified
to also support running without VFP.


The integration parameters are configured in `config/MiraMeshConfig.h`
and the code is in the `../nrf_common/miramesh_integration` directory.

`network_receiver_task.c` is the example application using MiraMesh, `main.c` is almost
unchanged from the blinky_freertos example, except for calls to the
other files.

`main` starts the MiraMesh integration and the MiraMesh using application.
The MiraMesh application waits until the integration is ready, then
starts MiraMesh and starts using it.
