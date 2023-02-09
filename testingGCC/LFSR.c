#include <stdio.h>
#include <stdint.h>

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
		lfsr <<= 1;                         	/* Shift register left */

		if (msb) {                          	/* If MSB is high */
			lfsr ^= taps;                 		/* xor tap bits of LFSR*/
		}
        ++loops;

    } while (lfsr != initial_state && lfsr != 0 && loops <= cycleLimit);

	printf("%d 	 %d \r\n", loops, lfsr); // print last result also

	/* output result to the console */
	if (loops < cycleLimit) {
		printf("Reached inital LFSR value after %d cycles. \r\n", loops); 
	} else if (loops < 0){
		printf("Stopped because LFSR went to zero. \r\n");
	} else {
		printf("Stopped after %d cycles without reaching initial LFSR value to prevent infinite execution.\r\n", loops);
	}
	printf("\r\n");


	return 0;
}
