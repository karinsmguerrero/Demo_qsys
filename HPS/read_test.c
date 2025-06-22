#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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

#define FIFO_SONG_OUT_BASE 0x00010000
#define FIFO_SONG_OUT_SPAN 4

#define FIFO_SONG_WRITE (*(FIFO_SONG_write_ptr))
#define FIFO_SONG_READ (*(FIFO_SONG_read_ptr))

#define FIFO_SONG_FULL ((*(FIFO_SONG_status_ptr + 1)) & 1)
#define FIFO_SONG_EMPTY ((*(FIFO_SONG_status_ptr + 1)) & 2)
#define FIFO_SONG_FILL_LEVEL (*FIFO_SONG_status_ptr)

// #define READ_FIFO_FULL		  ((*(FIFO_read_status_ptr+1)) & 1 )
// #define READ_FIFO_EMPTY	      ((*(FIFO_read_status_ptr+1)) & 2 )

#define WAIT {}
#define FIFO_WRITE_BLOCK(a)    \
    {                          \
        while (FIFO_SONG_FULL) \
        {                      \
            WAIT               \
        };                     \
        FIFO_SONG_WRITE = a;   \
    }

#define ALTERA_AVALON_FIFO_STATUS_ALL (0x3F)

volatile uint32_t *fifo_in_ptr;
volatile uint32_t *fifo_out_ptr;
volatile uint32_t *fifo_status_ptr;

// the light weight buss base
void *h2f_lw_virtual_base;

volatile unsigned int *FIFO_SONG_status_ptr = NULL;
volatile unsigned int *FIFO_SONG_write_ptr = NULL;
volatile unsigned int *FIFO_SONG_read_ptr = NULL;

int main()
{
    int fd;
    // void *virtual_base;

    fd = open("/dev/mem", O_RDWR | O_SYNC);
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

    fifo_in_ptr = (uint32_t *)((char *)h2f_lw_virtual_base + ((FIFO_SONG_IN_BASE) & (HW_REGS_MASK)));
    fifo_out_ptr = (uint32_t *)((char *)h2f_lw_virtual_base + ((FIFO_SONG_OUT_BASE) & (HW_REGS_MASK)));
    fifo_status_ptr = (uint32_t *)((char *)h2f_lw_virtual_base + ((FIFO_SONG_CSRREGS_BASE) & (HW_REGS_MASK)));
    // the two status registers
    FIFO_SONG_status_ptr = (uint32_t *)((char *)h2f_lw_virtual_base + ((FIFO_SONG_CSRREGS_BASE) & (HW_REGS_MASK)));
    FIFO_SONG_write_ptr = (uint32_t *)((char *)h2f_lw_virtual_base + ((FIFO_SONG_IN_BASE) & (HW_REGS_MASK)));
    FIFO_SONG_read_ptr = (uint32_t *)((char *)h2f_lw_virtual_base + ((FIFO_SONG_IN_BASE) & (HW_REGS_MASK)));

    /* printf("=====================\n\r");
     printf("fill levels before block write\n\r");
     printf("write=%d read=%d\n\r", WRITE_FIFO_FILL_LEVEL, READ_FIFO_FILL_LEVEL);

         // send array to FIFO and read block
     for (int i=0; i<10; i++){
         // wait for a slot
         // do the FIFO write
         FIFO_WRITE_BLOCK( 0xA0000000 | i);  // Escribe datos distintos);
     }

     printf("fill levels before block read\n\r");
     printf("write=%d read=%d\n\r", WRITE_FIFO_FILL_LEVEL, READ_FIFO_FILL_LEVEL);*/

    // Escribir datos al FIFO
    for (int i = 0; i < 300; i++)
    {
        if(!FIFO_SONG_FULL)
            FIFO_SONG_write_ptr[0] = 0xA0000000 | i; // Escribe datos distintos
        else
            printf("Skipping write due to full fifo \n");
        // printf("Dato escrito: 0x%08X\n", 0xA0000000 | i);
    }

    printf("----------------------------------------------- \n");

    /*int retdata[10];
    int i=0;
    int N = 9;
        while (!READ_FIFO_EMPTY) {
            retdata[i] = FIFO_READ;
            if (i>N) i=N;
            // print array from FIFO read port
            printf("return=%d %d %d\n\r", retdata[i], WRITE_FIFO_FILL_LEVEL, READ_FIFO_FILL_LEVEL) ;
            i++;
        }*/
    uint32_t status = fifo_status_ptr[1]; // Suponiendo que el registro de estado está en offset 4 bytes
    printf("Antes del read\n");
    printf("Status: %d \n", (status & ALTERA_AVALON_FIFO_STATUS_ALL));
    printf("Status fill: %d \n", FIFO_SONG_FULL);

    int value;
    for (int i = 0; i < 256; i++)
    {
        value = FIFO_SONG_READ;
        // printf("Read value: 0x%08X \n", value);
    }
    printf("Last value read: 0x%08X \n", value);
    printf("Despues del read\n");
    printf("Status: %d \n", (status & ALTERA_AVALON_FIFO_STATUS_ALL));
    printf("Status fill: %d \n", FIFO_SONG_FULL);

    munmap(h2f_lw_virtual_base, HW_REGS_SPAN);
    close(fd);

    return EXIT_SUCCESS;
}