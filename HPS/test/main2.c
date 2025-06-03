#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>


#define PRINT_ERROR(a, args...) printf("ERROR %s() %s Line %d: " a "\n", __FUNCTION__, __FILE__, __LINE__, ##args);

#if RAND_MAX == 32767
#define rand32() ((rand()%lt%lt16) + (rand()%lt%lt1) + (rand()&1))
#else
#define rand32() rand()
#endif

typedef struct {
	uint32_t samples;
	int16_t *data;
} sound_t;

sound_t song;

bool LoadWav(const char *filename, sound_t *sound);

#define SAMPLING_RATE 44100
#define CHUNK_SIZE 2000
int16_t chunks[2][CHUNK_SIZE] = {0};
bool chunk_swap = false;
int16_t *to;
bool quit = false;

int main(int argc, char **argv) {
	char* path = argv[0];
	if(!LoadWav("../songs/replay.wav", &song)) {
		PRINT_ERROR("Failed to load replay.wav");
		return -1;
	}
	static uint32_t sound_position = 0;
	for(int j = 0; j < CHUNK_SIZE; ++j) {
		if(sound_position < song.samples) {
			int16_t value = song.data[sound_position++];
			printf("Value: %d \n", value);
		}
		else {
			quit = true;
			*(to++) = 0;
		}
	}
	printf("About to exit");
	return 0;
}

// Loads ONLY 16-bit 1-channel PCM .WAV files. Allocates sound->data and fills with the pcm data. Fills sound->samples with the number of ELEMENTS in sound->data. EG for 2-bytes per sample single channel, sound->samples = HALF of the number of bytes in sound->data.
bool LoadWav(const char *filename, sound_t *sound) {
	bool return_value = true;
	FILE *file;
	char magic[4];
	int32_t filesize;
	int32_t format_length;		// 16
	int16_t format_type;		// 1 = PCM
	int16_t num_channels;		// 1
	int32_t sample_rate;		// 44100
	int32_t bytes_per_second;	// sample_rate * num_channels * bits_per_sample / 8
	int16_t block_align;		// num_channels * bits_per_sample / 8
	int16_t bits_per_sample;	// 16
	int32_t data_size;

	file = fopen(filename, "rb");
	if(file == NULL) {
		PRINT_ERROR("%s: Failed to open file", filename);
		return false;
	}
    // size_t fread( void *buffer, size_t size, size_t count, FILE *stream
	fread(magic, 1, 4, file);
	if(magic[0] != 'R' || magic[1] != 'I' || magic[2] != 'F' || magic[3] != 'F') {
		PRINT_ERROR("%s First 4 bytes should be \"RIFF\", are \"%4s\"", filename, magic);
		return_value = false;
		goto CLOSE_FILE;
	}

	fread(&filesize, 4, 1, file);

	fread(magic, 1, 4, file);
	if(magic[0] != 'W' || magic[1] != 'A' || magic[2] != 'V' || magic[3] != 'E') {
		PRINT_ERROR("%s 4 bytes should be \"WAVE\", are \"%4s\"", filename, magic);
		return_value = false;
		goto CLOSE_FILE;
	}

	fread(magic, 1, 4, file);
	if(magic[0] != 'f' || magic[1] != 'm' || magic[2] != 't' || magic[3] != ' ') {
		PRINT_ERROR("%s 4 bytes should be \"fmt/0\", are \"%4s\"", filename, magic);
		return_value = false;
		goto CLOSE_FILE;
	}

	fread(&format_length, 4, 1, file);
	fread(&format_type, 2, 1, file);
	if(format_type != 1) {
		PRINT_ERROR("%s format type should be 1, is %d", filename, format_type);
		return_value = false;
		goto CLOSE_FILE;
	}

	fread(&num_channels, 2, 1, file);
	if(num_channels != 1) {
		PRINT_ERROR("%s Number of channels should be 1, is %d", filename, num_channels);
		return_value = false;
		goto CLOSE_FILE;
	}

	fread(&sample_rate, 4, 1, file);
	if(sample_rate != 44100) {
		PRINT_ERROR("%s Sample rate should be 44100, is %d", filename, sample_rate);
		return_value = false;
		goto CLOSE_FILE;
	}

	fread(&bytes_per_second, 4, 1, file);
	fread(&block_align, 2, 1, file);
	fread(&bits_per_sample, 2, 1, file);
	if(bits_per_sample != 16) {
		PRINT_ERROR("%s bits per sample should be 16, is %d", filename, bits_per_sample);
		return_value = false;
		goto CLOSE_FILE;
	}

	fread(magic, 1, 4, file);
	if(magic[0] != 'd' || magic[1] != 'a' || magic[2] != 't' || magic[3] != 'a') {
		PRINT_ERROR("%s 4 bytes should be \"data\", are \"%4s\"", filename, magic);
		return_value = false;
		goto CLOSE_FILE;
	}

	fread(&data_size, 4, 1, file);

	sound->data = malloc(data_size);
	if(sound->data == NULL) {
		PRINT_ERROR("%s Failed to allocate %d bytes for data", filename, data_size);
		return_value = false;
		goto CLOSE_FILE;
	}

	if(fread(sound->data, 1, data_size, file) != data_size) {
		PRINT_ERROR("%s Failed to read data bytes", filename);
		return_value = false;
		free(sound->data);
		goto CLOSE_FILE;
	}

	sound->samples = data_size / 2;

	CLOSE_FILE:
	fclose(file);

	return return_value;
}