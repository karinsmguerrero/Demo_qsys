#include <stdio.h>
#include <system.h>
#include <altera_up_avalon_video_character_buffer_with_dma.h>

int main() {
    printf("Starting Character Buffer Debug...\n");

    alt_up_char_buffer_dev *char_buf = alt_up_char_buffer_open_dev(CHAR_BUFFER_AVALON_CHAR_BUFFER_SLAVE_NAME);

    if (char_buf == NULL) {
        printf("Error: Could not open character buffer device\n");
        return -1;
    }

    printf("Character buffer opened successfully!\n");

    // Initialize the character buffer
    alt_up_char_buffer_init(char_buf);
    printf("Character buffer initialized!\n");

    for(int y = 0; y < 10; y++) {
            for(int x = 0; x < 20; x++) {
                char ch = '0' + (x + y) % 10;
                alt_up_char_buffer_draw(char_buf, ch, x, y);
            }
        }

    return 0;
}
