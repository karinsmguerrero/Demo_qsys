#ifndef HPS_FIFO_H
#define HPS_FIFO_H

#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>

#define HW_REGS_BASE 0x00000000
#define HW_REGS_SPAN 0x00200000

#define HPS_FPGA_LW_BASE 0xff200000
#define HPS_FPGA_LW_SPAN 0x00001000

#define FIFO_BASE 0x10000
#define FIFO_SPAN 4
#define FIFO_END 0x10003


#define PRINT_ERROR(a, args...) printf("ERROR %s() %s Line %d: " a "\n", __FUNCTION__, __FILE__, __LINE__, ##args);
#define MAX_METADATA_LEN 128
#define SAMPLING_RATE 44100
#define CHUNK_SIZE 256

typedef struct
{
	uint32_t samples;
	int16_t *data;
} sound_t;

sound_t song;

int16_t chunks[2][CHUNK_SIZE] = {{0}};
bool chunk_swap = false;
int16_t *to;
bool quit = false;
volatile uint32_t *fifo_ptr;

typedef struct {
	char artist[MAX_METADATA_LEN];
	char title[MAX_METADATA_LEN];
	char album[MAX_METADATA_LEN];
	char comment[MAX_METADATA_LEN];
} wav_metadata_t;

void ReadWavMetadata(const char *filename, wav_metadata_t *metadata);
bool LoadWav(const char *filename, sound_t *sound);
void send_metada(char* audio_path);


#endif