#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#define HW_REGS_BASE (0xff200000)
#define HW_REGS_SPAN (0x00200000)
#define HW_REGS_MASK (HW_REGS_SPAN - 1)
#define LED_PIO_BASE 0x4040

int main(void)
{
    volatile unsigned int *h2p_lw_led_addr = NULL;
    void *virtual_base;
    int fd;

    // Open /dev/mem
    if ((fd = open("/dev/mem", (O_RDWR | O_SYNC))) == -1)
    {
        printf("ERROR: could not open \"/dev/mem\"...\n");
        return (1);
    }

    // get virtual addr that maps to physical
    virtual_base = mmap(NULL, HW_REGS_SPAN, (PROT_READ | PROT_WRITE),
                        MAP_SHARED, fd, HW_REGS_BASE);
    if (virtual_base == MAP_FAILED)
    {
        printf("ERROR: mmap() failed...\n");
        close(fd);
        return (1);
    }

    // Get the address that maps to the LEDs
    h2p_lw_led_addr = (unsigned int *)(virtual_base + ((LED_PIO_BASE) & (HW_REGS_MASK)));

    // Add 1 to the PIO register
    *h2p_lw_led_addr = *h2p_lw_led_addr + 1;

    if (munmap(virtual_base, HW_REGS_SPAN) != 0)
    {
        printf("ERROR: munmap() failed...\n");
        close(fd);
        return (1);
    }
    close(fd);
    return 0;
}
