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

//main bus; LW r/w address
#define HW_REGS_BASE 0xFF200000
#define HW_REGS_SPAN 0x80000
#define HW_REGS_MASK (HW_REGS_SPAN - 1)

// lw_bus; FIFO write address
#define FIFO_SONG_IN_BASE 0x00000000
#define FIFO_SONG_IN_SPAN 4

/// lw_bus; FIFO status address
#define FIFO_SONG_CSRREGS_BASE 0x00000020
#define FIFO_SONG_CSRREGS_SPAN 32

#define FIFO_SONG_OUT_BASE 0x00000010
#define FIFO_SONG_OUT_SPAN 4

#define FIFO_SONG_WRITE (*(FIFO_SONG_write_ptr))
#define FIFO_SONG_READ (*(FIFO_SONG_read_ptr))

#define FIFO_SONG_FULL ((*(FIFO_SONG_status_ptr + 1)) & 1)
#define FIFO_SONG_EMPTY ((*(FIFO_SONG_status_ptr + 1)) & 2)
#define FIFO_SONG_FILL_LEVEL (*FIFO_SONG_status_ptr)

#define ALTERA_AVALON_FIFO_STATUS_ALL (0x3F)

//lightweight bridge pointer
void *h2f_lw_virtual_base;

volatile unsigned int *FIFO_SONG_status_ptr = NULL;
volatile unsigned int *FIFO_SONG_write_ptr = NULL;
volatile unsigned int *FIFO_SONG_read_ptr = NULL;

#define WAIT {}
#define FIFO_WRITE_BLOCK(a)    \
    {                          \
        while (FIFO_SONG_FULL) \
        {                      \
            WAIT               \
        };                     \
        FIFO_SONG_WRITE = a;   \
    }


#define PRINT_ERROR(a, args...) printf("ERROR %s() %s Line %d: " a "\n", __FUNCTION__, __FILE__, __LINE__, ##args);
#define MAX_METADATA_LEN 128
#define SAMPLING_RATE 44100
#define CHUNK_SIZE 255

typedef struct
{
	uint32_t samples;
	int16_t *data;
} sound_t;

sound_t song;

int16_t chunks[2][CHUNK_SIZE] = {{0}};

typedef struct {
	char artist[MAX_METADATA_LEN];
	char title[MAX_METADATA_LEN];
	char album[MAX_METADATA_LEN];
	char comment[MAX_METADATA_LEN];
} wav_metadata_t;

void readWavMetadata(const char *filename, wav_metadata_t *metadata);
int init_system();
bool loadWav(const char *filename, sound_t *sound);
void sendMetadata(char* audio_path);
void sendWavSamples();

#endif