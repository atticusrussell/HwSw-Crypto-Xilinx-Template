/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include <stdint.h>
#include "platform.h"
#include "xil_printf.h"


/**
 * initial values of the 8 LFSR bits
 * 0 1 0 0 1 1 0 1
 * can be interpreted as either 77 (0x4d) or 178 (0xB2)
 * due to direction of arrows on page we're gonna say 0xB2
 * 
 * taps
 * y -> xor(y) ->  x1 -> xor(y) -> x2 -> x3 -> x4 -> x5 -> xor(y) -> x6 -> x7 -> y
 * 0b01000111
*/
int main() {
    init_platform();
    
	printf("starting LFSR program \r\n");

    uint8_t initial_state = 0xb2; //0b01001101;
	uint8_t taps = 0b01000111;	// 4 taps because of wrap around MSB->LSB
	

	printf("Cycle	LFSR \r\n");
	printf("-----   ---- \r\n");

	int cycleLimit = 500; // number of times to loop before giving up to prevent infinite exec

	uint8_t lfsr = initial_state;
    unsigned loops = 0;

    do {
		printf("%d\t %d \r\n", loops, lfsr);    /* print each value */
		unsigned msb = lfsr & 0b10000000;   	/* Get MSB */
		lfsr <<= 1;                         	/* Shift left */

		if (msb) {                          	
			lfsr ^= taps;                 		/* xor tap bits of LFSR*/
		}
        ++loops;

    } while (lfsr != initial_state && lfsr != 0 && loops <= cycleLimit);

	printf("%d 	 %d \r\n", loops, lfsr); // print last result also

	/* output result to the console */
	if (loops < cycleLimit) {
		printf("Reached initial LFSR value after %d cycles. \r\n", loops);
	} else if (loops < 0){
		printf("Stopped because LFSR went to zero. \r\n");
	} else {
		printf("Stopped after %d cycles without reaching initial LFSR value to prevent infinite execution.\r\n", loops);
	}
	printf("\r\n");

    cleanup_platform();
    return 0;
}


