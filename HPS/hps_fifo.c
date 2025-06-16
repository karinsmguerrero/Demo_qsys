#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "lib/HPS.h"

#define HW_REGS_BASE          0xFF200000
#define HW_REGS_SPAN          0x00200000
#define FIFO_ADDR             (HW_REGS_BASE + FIFO_BASE)

volatile uint32_t *fifo_ptr;

int main() {
    int fd;
    void *virtual_base;

    fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd == -1) {
        perror("Error abriendo /dev/mem");
        return EXIT_FAILURE;
    }

    virtual_base = mmap(NULL, HW_REGS_SPAN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, HW_REGS_BASE);
    if (virtual_base == MAP_FAILED) {
        perror("Error en mmap");
        close(fd);
        return EXIT_FAILURE;
    }

    fifo_ptr = (uint32_t *)((char *)virtual_base + FIFO_BASE);

    // Escribir datos al FIFO
    for (int i = 0; i < 10; i++) {
        fifo_ptr[0] = 0xA0000000 | i;  // Escribe datos distintos
        printf("Dato escrito: 0x%08X\n", 0xA0000000 | i);
    }

    munmap(virtual_base, HW_REGS_SPAN);
    close(fd);

    return EXIT_SUCCESS;
}
