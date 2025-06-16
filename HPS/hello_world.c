/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */

#include <stdio.h>

#include "altera_up_avalon_audio.h"
#include "altera_up_avalon_audio_and_video_config.h"

#define AUDIO_BASE 0x21010
#define AUDIO_CTL 0x21000
#define LED_BASE   0x21020
#define BUF_SIZE  20000
// app constants
#define SAMPLE_RATE 48000
#define FREQUENCY 333
#define AMPLITUDE 350000




int main(){
	volatile int* leds_ptr = (int*) LED_BASE;
	volatile int* audio_ptr = (int*) AUDIO_BASE;
	volatile int* ctl_ptr = (int*) AUDIO_CTL;

	printf("Hello from Nios II!\n");
	*leds_ptr = 0xf;

	//Let's build a big array with a discrete saw-tooth wave
	unsigned int data_buffer[BUF_SIZE];
	int period = SAMPLE_RATE / FREQUENCY;
	for (int i = 0; i < BUF_SIZE; i++) {
		data_buffer[i] = (int)((i % period) * 2 * AMPLITUDE / period) - AMPLITUDE;
	}


	//Continuously play the buffer
	int buffer_index = 0;
	while(1) {
		//check that we have space
		int fifospace = *(audio_ptr + 1);
		while (fifospace & 0x00FF0000) {
					// write data point to fifos
					*(audio_ptr + 2) = data_buffer[buffer_index];
					*(audio_ptr + 3) = data_buffer[buffer_index];
					//update index in the big source array
					buffer_index = (buffer_index + 1)% BUF_SIZE;
					//get available space
					fifospace = *(audio_ptr + 1);
				}
		//fifospace = *(audio_ptr + 1);
	}


	return 0;
}
