#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define HW_REGS_BASE          0xFF200000
#define HW_REGS_SPAN          0x80000
#define HW_REGS_MASK 		  (HW_REGS_SPAN - 1)

// main bus; FIFO write address
#define FIFO_BASE            0x00000000
#define FIFO_SPAN            4

/// lw_bus; FIFO status address
#define FIFO_CSRREGS_BASE     0x00000020
#define FIFO_CSRREGS_SPAN     32

#define FIFO_OUT_BASE        0x00010000
#define FIFO_OUT_SPAN        4

#define FIFO_WRITE		     (*(FIFO_write_ptr))
#define FIFO_READ            (*(FIFO_read_ptr))

#define WRITE_FIFO_FULL		  ((*(FIFO_write_status_ptr+1))& 1 )
#define WRITE_FIFO_EMPTY	  ((*(FIFO_write_status_ptr+1))& 2 )

#define WRITE_FIFO_FILL_LEVEL (*FIFO_write_status_ptr)
#define READ_FIFO_FILL_LEVEL  (*FIFO_read_status_ptr)

#define READ_FIFO_FULL		  ((*(FIFO_read_status_ptr+1)) & 1 )
#define READ_FIFO_EMPTY	      ((*(FIFO_read_status_ptr+1)) & 2 )

#define WAIT {}
#define FIFO_WRITE_BLOCK(a)	  {while (WRITE_FIFO_FULL){WAIT};FIFO_WRITE=a;}

#define ALTERA_AVALON_FIFO_STATUS_ALL  (0x3F)

volatile uint32_t *fifo_in_ptr;
volatile uint32_t *fifo_out_ptr;
volatile uint32_t *fifo_status_ptr;

// the light weight buss base
void *h2p_lw_virtual_base;
// HPS_to_FPGA FIFO status address = 0
volatile unsigned int * FIFO_write_status_ptr = NULL ;
volatile unsigned int * FIFO_read_status_ptr = NULL ;

// HPS_to_FPGA FIFO write address
// main bus address 0x0000_0000
void *h2p_virtual_base;
volatile unsigned int * FIFO_write_ptr = NULL ;
volatile unsigned int * FIFO_read_ptr = NULL ;

int main() {
    int fd;
    //void *virtual_base;

    fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd == -1) {
        perror("Error abriendo /dev/mem");
        return EXIT_FAILURE;
    }

    h2p_lw_virtual_base = mmap(NULL, HW_REGS_SPAN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, HW_REGS_BASE);
    if (h2p_lw_virtual_base == MAP_FAILED) {
        perror("Error en mmap LW BASE");
        close(fd);
        return EXIT_FAILURE;
    }

    fifo_in_ptr = (uint32_t *)((char *)h2p_lw_virtual_base + FIFO_IN_BASE);
    fifo_out_ptr = (uint32_t *)((char *)h2p_lw_virtual_base + FIFO_OUT_BASE);
    fifo_status_ptr = (uint32_t *)((char *)h2p_lw_virtual_base + FIFO_CSRREGS_BASE);
    // the two status registers
	FIFO_write_status_ptr = (unsigned int *)(h2p_lw_virtual_base);
	// From Qsys, second FIFO is 0x20
	FIFO_read_status_ptr = (unsigned int *)(h2p_lw_virtual_base + FIFO_CSRREGS_SPAN);


    // FIFO write addr
	h2p_virtual_base = mmap( NULL, FIFO_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, FIFO_IN_BASE);

	if( h2p_virtual_base == MAP_FAILED ) {
		perror("Error en mmap FIFO BASE");
        close(fd);
        return EXIT_FAILURE;
	}
    // Get the address that maps to the FIFO read/write ports
	FIFO_write_ptr =(unsigned int *)(h2p_virtual_base);
	FIFO_read_ptr = (unsigned int *)(h2p_virtual_base + FIFO_SPAN);

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
    for (int i = 0; i < 255; i++) {
        fifo_in_ptr[0] = 0xA0000000 | i;  // Escribe datos distintos
        printf("Dato escrito: 0x%08X\n", 0xA0000000 | i);
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
    uint32_t status = fifo_status_ptr[1];  // Suponiendo que el registro de estado está en offset 4 bytes
    printf("Antes del read\n");
    printf("Status: %d \n", status);
    printf("Status fill: %d \n", ((*(fifo_status_ptr+1))& 1 ));

    int value;
    for(int i = 0; i < 100; i++){
    value = *fifo_out_ptr;
    printf("Read value: %d \n", value);
    }

    printf("Despues del read\n");
    printf("Status: %d \n", status);
    printf("Status fill: %d \n", ((*(fifo_status_ptr+1))& 1 ));

    munmap(h2p_lw_virtual_base, FIFO_CSRREGS_SPAN);
    munmap(h2p_virtual_base, FIFO_SPAN);
    close(fd);

    return EXIT_SUCCESS;
}