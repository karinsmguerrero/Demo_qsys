#include <stdio.h>
#include "altera_up_avalon_audio.h"
#include "altera_up_avalon_audio_and_video_config.h"

#define AUDIO_BASE   0x4000
#define LED_BASE     0x4040
#define TIMER_BASE   0x4020

#define BUF_SIZE     1000
#define SAMPLE_RATE  48000
#define FREQUENCY    333
#define AMPLITUDE    35000

unsigned int data_buffer[BUF_SIZE];
volatile int buffer_index = 0;

// Bandera para indicar que el beep está activo
volatile int playing = 1;

// Función que genera una muestra
void beep() {
    volatile int* audio_ptr = (int*) AUDIO_BASE;
    int fifospace = *(audio_ptr + 1);

    if (playing && (fifospace & 0x00FF0000)) {
        *(audio_ptr + 2) = data_buffer[buffer_index]; // canal izquierdo
        *(audio_ptr + 3) = data_buffer[buffer_index]; // canal derecho
        buffer_index = (buffer_index + 1) % BUF_SIZE;
    }
}


void timer_isr_handler(void* context) {
    volatile int* timer_status = (int*) TIMER_BASE;
    *timer_status = 0x0;  // limpiar interrupción
    beep();
}

int main() {
    volatile int* leds_ptr         = (int*) LED_BASE;
    volatile int* timer_base_ptr   = (int*) TIMER_BASE;
    volatile int* timer_ctrl_ptr   = timer_base_ptr + 1;
    volatile int* timer_period_ptr = timer_base_ptr + 2;

    printf("Hello from Nios II!\n");
    *leds_ptr = 0xF;

    // onda sawtooth
    int period = SAMPLE_RATE / FREQUENCY;
    for (int i = 0; i < BUF_SIZE; i++) {
        data_buffer[i] = (int)((i % period) * 2 * AMPLITUDE / period) - AMPLITUDE;
    }


    *timer_base_ptr = 0x1;                  // reset timer status
    *timer_period_ptr = 50000 - 1;          // 50 MHz / 1000 = 1 ms
    alt_irq_register(2, NULL, timer_isr_handler); // irq 2 para timer
    *timer_ctrl_ptr = 0x7;                  // start, continuous, interrupt enable

    // Bucle principal solo actualiza LEDs
    while (1) {
        *leds_ptr = buffer_index;
    }

    return 0;
}
