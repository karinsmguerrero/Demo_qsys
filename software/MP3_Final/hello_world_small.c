#include <stdio.h>
#include "system.h"
#include "altera_up_avalon_audio.h"
#include "altera_up_avalon_audio_and_video_config.h"

#define AUDIO_BASE   0x4000
#define LED_BASE     0x4040
#define TIMER_BASE   0x4020
#define BUTTON_BASE  0x4010
#define BUF_SIZE     100
#define SAMPLE_RATE  44100
#define FREQUENCY    333
#define AMPLITUDE    35000
#define MUSIC_FIFO_BASE 0x00010000

unsigned int data_buffer[BUF_SIZE];




volatile int buffer_index = 0;

// Bandera para indicar que el beep est� activo
volatile int playing = 1;

// Estados de m�sica y control
volatile int music_playing = 0;     // 0 = pausa, 1 = reproduciendo
volatile int change_music = 0;    // 0 = siguiente, 1 = anterior

//                                                    __________________________________________________
//___________________________________________________/ajuste de beep() para que lea el fifo

void beep() {
    volatile int* audio_ptr = (int*) AUDIO_BASE;
    volatile int* music_fifo_ptr = (int*) MUSIC_FIFO_BASE;


    int fifospace = *(audio_ptr + 1);

    if (playing && (fifospace & 0x00FF0000)) {

    	int sample = *music_fifo_ptr;  // Leer muestra de FIFO

        *(audio_ptr + 2) = sample; // canal izquierdo
        *(audio_ptr + 3) = sample; // canal derecho
        //buffer_index = (buffer_index + 1) % BUF_SIZE;
    }
}


void timer_isr_handler(void* context) {
    volatile int* timer_status = (int*) TIMER_BASE;
    *timer_status = 0x0;  // limpiar interrupci�n
    beep();
}

void mp3_state(int state){
	if(state == 2){
		change_music = 1;
		printf("Canción Anterior\n");
	}else{
		change_music = 0;
		printf("Canción Siguiente\n");
	}
}

void repro(){
	if(music_playing == 1 ){
		music_playing = 0;
		printf("música en pausa\n");

	}else{
		music_playing = 1;
		printf("reproduciendo música\n");
	}


}


int main() {
    volatile int* leds_ptr         = (int*) LED_BASE;
    volatile int* timer_base_ptr   = (int*) TIMER_BASE;
    volatile int* timer_ctrl_ptr   = timer_base_ptr + 1;
    volatile int* timer_period_ptr = timer_base_ptr + 2;
    volatile int* button_ptr = (int*) BUTTON_BASE;

    int prev_state = *button_ptr;  // Leer estado inicial (liberado)

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
        int current_state = *button_ptr;
        for (int i = 0; i < 3; i++) {
            // Rising edge: antes presionado (0), ahora liberado (1)
            int was_pressed  = !(prev_state & (1 << i));
            int now_released =  (current_state & (1 << i));

            if (was_pressed && now_released) {
                switch (i) {
                    case 0:
                        mp3_state(0);  // Bot�n 0: Siguiente
                        break;
                    case 1:
                        printf("Boton pausa/reproducir\n");
                        repro();       // Bot�n 1: Pausar/Reproducir
                        break;
                    case 2:
                        mp3_state(2);  // Bot�n 2: Anterior
                        break;
                }
            }
        }

        prev_state = current_state;
    }

    return 0;
}
