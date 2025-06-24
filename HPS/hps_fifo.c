#include "lib/HPS_FIFO.h"

int main(int argc, char **argv) 
{

	if (argc < 2) {
		PRINT_ERROR("Usage: %s <audio_file.wav>\n", argv[0]);
		return -1;
	}

	int res = init_system();
	if(res){
		printf("Could not initialize system \n");
		return -1;
	}

	const char* audio_path = argv[1];

	if (!loadWav(audio_path, &song)) {
		PRINT_ERROR("Failed to load %s", audio_path);
		return -1;
	}
	else {
		printf("Sucessfully loaded song \n");
	}

	sendMetadata();
	sendWavSamples();


	return EXIT_SUCCESS;
}

int init_system(){

	int fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd == -1)
    {
        perror("Error opening /dev/mem");
        return EXIT_FAILURE;
    }

    h2f_lw_virtual_base = mmap(NULL, HW_REGS_SPAN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, HW_REGS_BASE);
    if (h2f_lw_virtual_base == MAP_FAILED)
    {
        perror("Error in mmap");
        close(fd);
        return EXIT_FAILURE;
    }

    FIFO_SONG_status_ptr = (uint32_t *)((char *)h2f_lw_virtual_base + ((FIFO_SONG_CSRREGS_BASE) & (HW_REGS_MASK)));
    FIFO_SONG_write_ptr = (uint32_t *)((char *)h2f_lw_virtual_base + ((FIFO_SONG_IN_BASE) & (HW_REGS_MASK)));
    FIFO_SONG_read_ptr = (uint32_t *)((char *)h2f_lw_virtual_base + ((FIFO_SONG_IN_BASE) & (HW_REGS_MASK)));

	printf("Sucessfully initialized system \n");
	return 0;
}
// Loads ONLY 16-bit 1-channel PCM .WAV files. Allocates sound->data and fills with the pcm data. 
//Fills sound->samples with the number of ELEMENTS in sound->data. 
//EG for 2-bytes per sample single channel, sound->samples = HALF of the number of bytes in sound->data.
bool loadWav(const char *filename, sound_t *sound)
{
	bool return_value = true;
	FILE *file;
	char magic[4];
	int32_t filesize;
	int32_t format_length;	  // 16
	int16_t format_type;	  // 1 = PCM
	int16_t num_channels;	  // 1
	int32_t sample_rate;	  // 44100
	int32_t bytes_per_second; // sample_rate * num_channels * bits_per_sample / 8
	int16_t block_align;	  // num_channels * bits_per_sample / 8
	int16_t bits_per_sample;  // 16
	int32_t data_size;

	file = fopen(filename, "rb");
	if (file == NULL)
	{
		PRINT_ERROR("%s: Failed to open file", filename);
		return false;
	}
	// size_t fread( void *buffer, size_t size, size_t count, FILE *stream
	fread(magic, 1, 4, file);
	if (magic[0] != 'R' || magic[1] != 'I' || magic[2] != 'F' || magic[3] != 'F')
	{
		PRINT_ERROR("%s First 4 bytes should be \"RIFF\", are \"%4s\"", filename, magic);
		return_value = false;
		goto CLOSE_FILE;
	}

	fread(&filesize, 4, 1, file);

	fread(magic, 1, 4, file);
	if (magic[0] != 'W' || magic[1] != 'A' || magic[2] != 'V' || magic[3] != 'E')
	{
		PRINT_ERROR("%s 4 bytes should be \"WAVE\", are \"%4s\"", filename, magic);
		return_value = false;
		goto CLOSE_FILE;
	}

	fread(magic, 1, 4, file);
	if (magic[0] != 'f' || magic[1] != 'm' || magic[2] != 't' || magic[3] != ' ')
	{
		PRINT_ERROR("%s 4 bytes should be \"fmt/0\", are \"%4s\"", filename, magic);
		return_value = false;
		goto CLOSE_FILE;
	}

	fread(&format_length, 4, 1, file);
	fread(&format_type, 2, 1, file);
	if (format_type != 1)
	{
		PRINT_ERROR("%s format type should be 1, is %d", filename, format_type);
		return_value = false;
		goto CLOSE_FILE;
	}

	fread(&num_channels, 2, 1, file);
	if (num_channels != 1)
	{
		PRINT_ERROR("%s Number of channels should be 1, is %d", filename, num_channels);
		return_value = false;
		goto CLOSE_FILE;
	}

	fread(&sample_rate, 4, 1, file);
	if (sample_rate != 44100 && sample_rate != 48000)
	{
		PRINT_ERROR("%s Sample rate should be 44100 or 48000, is %d", filename, sample_rate);
		return_value = false;
		goto CLOSE_FILE;
	}

	fread(&bytes_per_second, 4, 1, file);
	fread(&block_align, 2, 1, file);
	fread(&bits_per_sample, 2, 1, file);
	if (bits_per_sample != 16)
	{
		PRINT_ERROR("%s bits per sample should be 16, is %d", filename, bits_per_sample);
		return_value = false;
		goto CLOSE_FILE;
	}

	fread(magic, 1, 4, file);
	if (magic[0] != 'd' || magic[1] != 'a' || magic[2] != 't' || magic[3] != 'a')
	{
		PRINT_ERROR("%s 4 bytes should be \"data\", are \"%4s\"", filename, magic);
		return_value = false;
		goto CLOSE_FILE;
	}

	fread(&data_size, 4, 1, file);

	sound->data = malloc(data_size);
	if (sound->data == NULL)
	{
		PRINT_ERROR("%s Failed to allocate %d bytes for data", filename, data_size);
		return_value = false;
		goto CLOSE_FILE;
	}

	if (fread(sound->data, 1, data_size, file) != data_size)
	{
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

void sendWavSamples() {
    for (uint32_t i = 0; i < song.samples; ++i) {
        FIFO_WRITE_BLOCK(song.data[i]);
    }
}

void readWavMetadata(const char *filename, wav_metadata_t *metadata) {
	FILE *file = fopen(filename, "rb");
	if (!file) {
		PRINT_ERROR("Cannot open file for metadata: %s", filename);
		return;
	}

	fseek(file, 12, SEEK_SET); // Skip RIFF header

	char chunk_id[5] = {0};
	uint32_t chunk_size;

	while (fread(chunk_id, 1, 4, file) == 4) {
		fread(&chunk_size, 4, 1, file);

		if (strncmp(chunk_id, "LIST", 4) == 0) {
			long list_end = ftell(file) + chunk_size;

			char list_type[5] = {0};
			fread(list_type, 1, 4, file);
			if (strncmp(list_type, "INFO", 4) != 0) {
				fseek(file, list_end, SEEK_SET);
				continue;
			}

			while (ftell(file) < list_end) {
				char info_id[5] = {0};
				uint32_t info_size = 0;

				if (fread(info_id, 1, 4, file) != 4) break;
				if (fread(&info_size, 4, 1, file) != 1) break;

				char buffer[MAX_METADATA_LEN] = {0};
				size_t read_len = (info_size < MAX_METADATA_LEN - 1) ? info_size : MAX_METADATA_LEN - 1;
				fread(buffer, 1, read_len, file);
				buffer[read_len] = '\0';

				if (strncmp(info_id, "IART", 4) == 0) {
					strncpy(metadata->artist, buffer, MAX_METADATA_LEN);
				} else if (strncmp(info_id, "INAM", 4) == 0) {
					strncpy(metadata->title, buffer, MAX_METADATA_LEN);
				} else if (strncmp(info_id, "IPRD", 4) == 0) {
					strncpy(metadata->album, buffer, MAX_METADATA_LEN);
				} else if (strncmp(info_id, "ICMT", 4) == 0) {
					strncpy(metadata->comment, buffer, MAX_METADATA_LEN);
				}

				// Word alignment
				if (info_size % 2 != 0)
					fseek(file, 1, SEEK_CUR);
			}
		} else {
			fseek(file, chunk_size, SEEK_CUR);
		}
	}

	fclose(file);
}

void sendMetadata(char* audio_path){
	wav_metadata_t metadata = {{0}};
	readWavMetadata(audio_path, &metadata);

	char data[32] ="";
	data = "Artist: ";
	strcat(data, metadata.artist);
	FIFO_WRITE_BLOCK(data);

	data = "Title: ";
    strcat(data, metadata.title);
	FIFO_WRITE_BLOCK(data);

	data = "Album: ";
    strcat(data, metadata.album);
	FIFO_WRITE_BLOCK(data);
}
