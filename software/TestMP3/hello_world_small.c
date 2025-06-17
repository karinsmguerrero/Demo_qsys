#include "sys/alt_stdio.h"
#include "alt_types.h"
#include "sys/alt_irq.h"
#include <stdint.h>

#define LEDS_BASE    0x4040
#define BUTTON_BASE  0x4010
#define TIMER_BASE   0x4020
#define AUDIO_0_BASE 0x4000
#define AUDIO_AND_VIDEO_CONFIG_0_BASE 0x0000

unsigned int elapsed_ms;

volatile int beep_pos = 0;
volatile int beep_running = 0;

void timer_ir_handler(void *context);

// Nueva función para emitir una muestra a la vez
void beep_step() {

	volatile uint32_t *audio_status = (uint32_t *)(AUDIO_0_BASE + 1); // STATUS en offset 8
	volatile int32_t *audio_left  = (int32_t *) (AUDIO_0_BASE + 2);
	volatile int32_t *audio_right = (int32_t *) (AUDIO_0_BASE + 3);

    if (!beep_running || beep_pos >= 240000) {
        beep_running = 0; // Termina beep
        alt_putstr("Beep finished\n");
        return;
    }

    int32_t sample = (beep_pos % 100 < 50) ? 0x600000 : -0x600000;

    int fifo_space = (*audio_status >> 16) & 0xFF;

    // Imprimir cada 1000 muestras para no saturar salida
    if (beep_pos % 1000 == 0) {
        alt_printf("beep_pos=%d, fifo_space=%d\n", beep_pos, fifo_space);
    }

    if (fifo_space > 0) {
        *audio_left = sample;
        *audio_right = sample;
        beep_pos++;
    }
}

int main()
{
    volatile unsigned int * leds_ptr = (unsigned int *) LEDS_BASE;
    volatile unsigned int * timer_status_ptr = (unsigned int *) TIMER_BASE;
    volatile unsigned int * timer_ctr_ptr = timer_status_ptr + 1;
    volatile unsigned int * timer_cmp_ptr = timer_status_ptr + 2;

    volatile unsigned int * audio_config_ptr = (unsigned int *) AUDIO_AND_VIDEO_CONFIG_0_BASE;
    *audio_config_ptr = 0x1;  // Configura codec

    alt_putstr("Hello from Nios II!\n");
    beep_step();

    *timer_status_ptr = 0x1;  // reset timer

    if (*timer_status_ptr & 0x1) {
        alt_printf("ERROR: interrupción pendiente -> %x\n", *timer_status_ptr);
        return 0;
    }

    *timer_cmp_ptr = 50000 - 1;
    alt_irq_register(0x2, 0x0, timer_ir_handler);

    alt_putstr("Turning on the timer\n");
    *timer_ctr_ptr = 0x7;  // start timer

    while (*timer_status_ptr != 0x2);
    alt_putstr("Timer is running\n");

    elapsed_ms = 0;

    beep_running = 1;  // iniciar beep
    beep_pos = 0;

    while (1) {
                // generar beep sin bloqueo
        *leds_ptr = elapsed_ms;

        if ((*leds_ptr) >= 1 << 10)
            break;
    }

    return 0;
}

void timer_ir_handler(void * context) {
    volatile int* timer_status_ptr = (int *) TIMER_BASE;
    *timer_status_ptr = 0x0;  // limpiar interrupción
    alt_printf("interrupt handler called.  clear status is %x\n", *timer_status_ptr);
    elapsed_ms += 1;

}
