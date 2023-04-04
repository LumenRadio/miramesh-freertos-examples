# MiraMesh on FreeRTOS examples

Current supported hardware is nrf52840, tested on nrf52840-dk.

## Install

Download and unpack mira to `src/vendor/libmira`. Link should be provided from when
getting your license.

Download and unpack nrf5-sdk, and place in `src/vendor/nrf5-sdk`. nrf5-sdk can be
[downloaded from Nordic Semiconductor](https://www.nordicsemi.com/Products/Development-software/nrf5-sdk).
Preferred version for MiraOS. Current supported version is 17.1.0. Note, the
original directory has a name similar to `nRF5_SDK_17.1.0_ddde560` and has to be
renamed.

Some dependencies are referred to as git submodules. Install them using:
`git submodule update --init --recursive`

Those are:
- [tinyprintf](https://github.com/cjlano/tinyprintf) by [CJlano](https://github.com/cjlano)
- [FreeRTOS-Kernel](https://github.com/FreeRTOS/FreeRTOS-Kernel)

## Compilation

All examples are built by running `make` in the top level directory

The build artifacts will be outputted as `examplename-nrf52840.hex` in top level
directory.

All intermediate files will be placed in `build/`

To build a specific example, for example `uart_tx`, type `make uart_tx-nrf52840.hex`

For networking, there are three examples:
 - `net_rx` - Acts as root node. run one per network
 - `net_tx` - A mesh node, that sends packets to `net_rx`
 - `net_tx_with_ble` - Same as `net_tx`, but is running concurrent BLE

## Flashing

First, add a license to the device on address `0xff000` of length `0x1000`,
according to MiraOS documentation:

https://docs.lumenrad.io/miraos/2.7.2/description/licensing/licensing_tool.html#signing-a-connected-device

...or by flashing the license as a hex file if available.

Second, flash the softdevice to the device:
```
nrfjprog -f nrf52 --program src/vendor/nrf5-sdk/components/softdevice/s140/hex/s140_nrf52_7.2.0_softdevice.hex --sectorerase --verify
```

Then flash the application, for example:
```
nrfjprog -f nrf52 --program uart_tx-nrf52840.hex --sectorerase --verify
```

## Examples

Examples can be found as directories under `src/`, except `src/lib/` and
`src/vendor/, which is common files used by all examples

Makefile specifying content of the builds can be found in `make/targets/`

### UART TX

`src/uart_tx/` is a bare minimum example to show the boilerplate of FreeRTOS and
MiraMesh integration, running no network

### Network Sender / Network Receiver

`src/network_sender/` and `src/network_receiver/` shows a pair firmwares where
network receiver runs as root and can receieve packets from multiple network
senders.

### BLE example

`src/ble_example/` shows how to integrate with concurrent BLE using nordics
softdevice.

## Version compatibility

Tested on agains Mira version 2.7.2, but should work with earlier versions too.

## Timers

FreeRTOS needs a tick timer for task switching. Currently there are three modes
available:
- SysTick
- RTC
- MiraMesh

Beside that, FreeRTOS can run in a mode called "Tickless", which can disable the
ticks when no tasks are active, and compensate upon next wakeup. Tickless is
necessary to reduce power consumption for running battery powered. Tickless
however adds complexity during development, and may be 

### SysTick
Uses the SysTick handler in the ARM core. No nRF peripherals will be used for
this mode.

### RTC
Uses the nRF RTC peripheral directly, to provide a steady callback for ticks.
This mode takes up one RTC peripheral. To use this mode, it is required to make
sure that peripherals are available.

### MiraMesh timer
Uses MiraMesh internal timers to provide a ticksource. The timer needs to be
running anyway when using MiraMesh. Internally uses an nRF RTC, but is then
shared with MiraMesh. Note that miramesh_init() has to be called before
starting FreeRTOS scheduler. Also note that mira_net_init() doesn't have to be
called for timer to run.

## Memory allocation

MiraMesh allocates memory upon startup, including when calling `mira_net_init()`
and other init functions. It however shouldn't allocate memory during runtime.

Same can apply to FreeRTOS. So using the same memory manager is suitable.

For that reason, MiraMesh memory management is therefore remapped to use
FreeRTOS, using `mira_mem_set_alloc_callback()` to replace the internal MiraMesh
allocation functions.