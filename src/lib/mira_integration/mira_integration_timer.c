#include "miramesh.h"
#include "FreeRTOS.h"
#include "log.h"
#include "nrf.h"
#include "nrf_sdh.h"
#include <stdbool.h>

static volatile miracore_timer_time_t current_tick_time;
static volatile miracore_timer_time_t previous_tick_time;

/* default systick handler in FreeRTOS */
void xPortSysTickHandler(
    void);

static void tick_callback(
    miracore_timer_time_t now,
    void *storage)
{
    current_tick_time = now;
    __DSB();
    // Trigger SysTick Exception:
    SCB->ICSR = SCB_ICSR_PENDSTSET_Msk;
    __SEV();
}

void SysTick_Handler(
    void)
{
    miracore_timer_time_t cyclesPerTick =
        miramesh_timer_time_add_us(0, 1000000 / configTICK_RATE_HZ);
    int32_t diff;
#if (configUSE_TICKLESS_IDLE == 1)
    diff = (current_tick_time - previous_tick_time) / cyclesPerTick;
    if (diff < 1) {
        diff = 1;
    } else if (diff > 1) {
        /* Correct the tick count when processor sleeps skipping a
         * few scheduler ticks.
         *
         * Increment by (diff - 1) as xPortSysTickHandler() call at the end
         * would increment tick by 1 */
        vTaskStepTick ((TickType_t) diff - 1);
    }
#else
    /* If not tickless, we need to catch up to keep track of tick count */
    diff = 1;
#endif
    previous_tick_time += diff * cyclesPerTick;

    miramesh_timer_schedule(
        previous_tick_time + cyclesPerTick, /* Schedule for next tick */
        tick_callback,
        NULL);

    xPortSysTickHandler();
}

/**
 * Override default SysTick initialization function, to
 * use miramesh internal timers instead.
 *
 * This will allow the use of an RTC that is shared with
 * Mira
 */
void vPortSetupTimerInterrupt(
    void)
{
    /* Turn off SysTick */
    SysTick->CTRL = 0;
    /* Set exception priority */
    NVIC_SetPriority(SysTick_IRQn, configKERNEL_INTERRUPT_PRIORITY);
    /* Initialize startup timers */
    previous_tick_time = miramesh_timer_get_time_now();
    current_tick_time = previous_tick_time;
    /* Trigger SysTick exception */
    SCB->ICSR = SCB_ICSR_PENDSTSET_Msk;

    /* Wakeup routine depends on WFE waking up from IRQ */
    /* also needed for ERRATA 220 fix for 52832 */
    SCB->SCR |= SCB_SCR_SEVONPEND_Msk;
}

#if (configUSE_TICKLESS_IDLE == 1)

/**
 * Wrapper for WFE instruction, with fixes for nordic erratas
 *
 * To be called with interrupts disabled
 */
static void wait_for_event(
    void)
{
    /* ERRATA 87 fix */
    if ((SCB->CPACR & ((1UL << 20) | (1UL << 22)))
        == ((1UL << 20) | (1UL << 22))) {
        asm (
            "vmrs r0, fpscr      \n"
            "bic  r0, r0, 0x9f   \n"
            "vmsr fpscr, r0      \n"
            "vmrs r0, fpscr      \n"
            : : : "r0"
        );
        NVIC_ClearPendingIRQ(FPU_IRQn);
    }

#ifdef NRF52832_XXAA
    /* ERRATA 75 fix for 52832 */
    uint32_t enabled_regions = NRF_MWU->REGIONENCLR;
    NRF_MWU->REGIONENCLR = enabled_regions;
#endif

    __WFE();

#ifdef NRF52832_XXAA
    /* ERRATA 220 fix for 52832-rev1 */
    __NOP();
    __NOP();
    __NOP();
    __NOP();

    /* ERRATA 75 fix for 52832 */
    NRF_MWU->REGIONENSET = enabled_regions;
#endif
}

void vPortSuppressTicksAndSleep(
    TickType_t xExpectedIdleTime)
{
    const miracore_timer_time_t cyclesPerTick =
        miramesh_timer_time_add_us(0, 1000000 / configTICK_RATE_HZ);

    /* Make sure the SysTick reload value does not overflow the counter. */
    if (xExpectedIdleTime >= ((1 << 30) / cyclesPerTick) ) {
        xExpectedIdleTime = ((1 << 30) / cyclesPerTick) - 1;
    }

    miracore_timer_time_t enterTime = previous_tick_time;

    miramesh_timer_schedule(enterTime + xExpectedIdleTime * cyclesPerTick,
        tick_callback,
        NULL);

    /* Block all the interrupts globally */
#if SOFTDEVICE_PRESENT
    uint8_t prev_prio = 0;
    uint32_t err_code = sd_nvic_critical_region_enter(&prev_prio);
    APP_ERROR_CHECK(err_code);
#else
    __disable_irq();
#endif

    if (enterTime != previous_tick_time) {
        miramesh_timer_schedule(previous_tick_time + cyclesPerTick,
            tick_callback,
            NULL);
    } else if (eTaskConfirmSleepModeStatus() != eAbortSleep) {
        /* Sleep until something happens.  configPRE_SLEEP_PROCESSING() can
         * set its parameter to 0 to indicate that its implementation contains
         * its own wait for interrupt or wait for event instruction, and so wfi
         * should not be executed again.  However, the original expected idle
         * time variable must remain unmodified, so a copy is taken. */
        TickType_t xModifiableIdleTime = xExpectedIdleTime;
        configPRE_SLEEP_PROCESSING(xModifiableIdleTime);
        if (xModifiableIdleTime > 0) {
            /* There are issues with using sd_app_evt_wait and current
             * consumption. Therefore do WFE manually */
            do {
                wait_for_event();
            } while (0 == (NVIC->ISPR[0] | NVIC->ISPR[1]));
        }
        configPOST_SLEEP_PROCESSING(xExpectedIdleTime);

        /* Correct the system ticks */
        {
            TickType_t diff = 0;
            miracore_timer_time_t exitTime = miramesh_timer_get_time_now();
            diff = (exitTime - enterTime) / cyclesPerTick;

#if 0
            if ((configUSE_TICKLESS_IDLE_SIMPLE_DEBUG)
                && (diff > xExpectedIdleTime)) {
                diff = xExpectedIdleTime;
            }
#endif
            if (diff < xExpectedIdleTime) {
                miramesh_timer_schedule(previous_tick_time + cyclesPerTick
                    * (diff + 1),
                    tick_callback,
                    NULL);
            }
        }
    }
#if SOFTDEVICE_PRESENT
    err_code = sd_nvic_critical_region_exit(prev_prio);
    APP_ERROR_CHECK(err_code);
#else
    __enable_irq();
#endif
}
#endif