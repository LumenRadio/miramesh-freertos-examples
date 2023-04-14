FreeRTOS port for nrf52 with SoftDevice
=======================================

Based upon ARM_CM4F port from v10.5.1 of FreeRTOS

Some weaks are done to work with softdevice

prvPortStartFirstTask
---------------------

Resets MSP to initial value. However, original reads from main interrupt vector,
which is the interrupt vector of MBR pointing to stack start at 0x20000400.

It needs to look in application interrupt vector instead.

uxCriticalNesting
-----------------

Is set to a magical value by default, which made all calls to
`portENTER_CRITICAL` and `portEXIT_CRITICAL` to not reset the `basepri`
register, which masks `svc` instruction. That means `basepri` is set after all
memory allocations upon startup, including `xTaskCreate`

Given that calls to softdevice, including those during setup, is done using
`svc`, this becomes an issue.

There are two solutions:
1. Make sure BASEPRI is reset after each critical section, including during
   startup, by setting `uxCriticalNesting` to `0` by default
2. Set `configMAX_SYSCALL_INTERRUPT_PRIORITY` to `_PRIO_APP_LOW_MID`, to put
   softdevice calls above the FreeRTOS masking threshold.

Given that fast high priority interrupts, such as UART interrupt, needs access
to queues and stream buffers, option 1 provieds best option for quick response.