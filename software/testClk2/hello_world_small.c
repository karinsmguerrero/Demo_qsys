/* 
 * "Small Hello World" example. 
 * 
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example 
 * designs. It requires a STDOUT  device in your system's hardware. 
 *
 * The purpose of this example is to demonstrate the smallest possible Hello 
 * World application, using the Nios II HAL library.  The memory footprint
 * of this hosted application is ~332 bytes by default using the standard 
 * reference design.  For a more fully featured Hello World application
 * example, see the example titled "Hello World".
 *
 * The memory footprint of this example has been reduced by making the
 * following changes to the normal "Hello World" example.
 * Check in the Nios II Software Developers Manual for a more complete 
 * description.
 * 
 * In the SW Application project (small_hello_world):
 *
 *  - In the C/C++ Build page
 * 
 *    - Set the Optimization Level to -Os
 * 
 * In System Library project (small_hello_world_syslib):
 *  - In the C/C++ Build page
 * 
 *    - Set the Optimization Level to -Os
 * 
 *    - Define the preprocessor option ALT_NO_INSTRUCTION_EMULATION 
 *      This removes software exception handling, which means that you cannot 
 *      run code compiled for Nios II cpu with a hardware multiplier on a core 
 *      without a the multiply unit. Check the Nios II Software Developers 
 *      Manual for more details.
 *
 *  - In the System Library page:
 *    - Set Periodic system timer and Timestamp timer to none
 *      This prevents the automatic inclusion of the timer driver.
 *
 *    - Set Max file descriptors to 4
 *      This reduces the size of the file handle pool.
 *
 *    - Check Main function does not exit
 *    - Uncheck Clean exit (flush buffers)
 *      This removes the unneeded call to exit when main returns, since it
 *      won't.
 *
 *    - Check Don't use C++
 *      This builds without the C++ support code.
 *
 *    - Check Small C library
 *      This uses a reduced functionality C library, which lacks  
 *      support for buffering, file IO, floating point and getch(), etc. 
 *      Check the Nios II Software Developers Manual for a complete list.
 *
 *    - Check Reduced device drivers
 *      This uses reduced functionality drivers if they're available. For the
 *      standard design this means you get polled UART and JTAG UART drivers,
 *      no support for the LCD driver and you lose the ability to program 
 *      CFI compliant flash devices.
 *
 *    - Check Access device drivers directly
 *      This bypasses the device file system to access device drivers directly.
 *      This eliminates the space required for the device file system services.
 *      It also provides a HAL version of libc services that access the drivers
 *      directly, further reducing space. Only a limited number of libc
 *      functions are available in this configuration.
 *
 *    - Use ALT versions of stdio routines:
 *
 *           Function                  Description
 *        ===============  =====================================
 *        alt_printf       Only supports %s, %x, and %c ( < 1 Kbyte)
 *        alt_putstr       Smaller overhead than puts with direct drivers
 *                         Note this function doesn't add a newline.
 *        alt_putchar      Smaller overhead than putchar with direct drivers
 *        alt_getchar      Smaller overhead than getchar with direct drivers
 *
 */

#include "sys/alt_stdio.h"
#include "alt_types.h"
#include "sys/alt_irq.h"


#define LEDS_BASE    0x4000
#define BUTTON_BASE  0x4010
#define TIMER_BASE   0x4020

unsigned int elapsed_ms;

void timer_ir_handler (void * context);

int main()
{ 
	volatile unsigned int * leds_ptr = (unsigned int *) LEDS_BASE;
	volatile unsigned int * button_ptr = (unsigned int *) BUTTON_BASE;
	volatile unsigned int * timer_status_ptr = (unsigned int *) TIMER_BASE; //offset 0
	volatile unsigned int * timer_ctr_ptr = timer_status_ptr + 1; //offset 1
	volatile unsigned int * timer_snapl_ptr = timer_status_ptr + 4; //offset 4

	alt_putstr("Hello from Nios II!\n");
	if (*timer_status_ptr != 0) {
		alt_printf("ERROR: status is not 0 -> %x\n", *timer_status_ptr);
		return 0;
	}
	alt_ic_isr_register(0x0,0x2, timer_ir_handler, 0x0, 0x0);

	alt_putstr("Turning on the timer\n");
	*timer_ctr_ptr = 0x7;
	while (*timer_status_ptr != 0x2);
	alt_putstr("Timer is running\n");
	elapsed_ms = 0;
	/* Event loop never exits. */
	  /* Event loop never exits. */
	  while (1) {
			  *leds_ptr = elapsed_ms;
			  if ((*leds_ptr) >= 1<< 10)
				return 0;
		  }
	  return 0;
}

	void timer_ir_handler (void * context) {
		  //Clear the interrupt so that we can keep counting
		  volatile int* timer_status_ptr = (int *) TIMER_BASE;
		  *timer_status_ptr = 0x0;
		  alt_printf("interrupt handler called.  clear status is %x\n", *timer_status_ptr);
		  elapsed_ms += 1;
	}
