#include "stdio.h"
#include "xgpio.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "platform.h"
#include "xscutimer.h"

#define TIMER_LOAD_VALUE 0xFFFFFFFF

/* Xilinx version of printf - smaller footprint */
#define printf xil_printf

/* GPIO device that LEDs are connected to */
#define GPIO_DEVICE_ID_LED XPAR_AXI_GPIO_0_DEVICE_ID

/* GPIO port for LEDs */
#define LED_CHANNEL 1

/* GPIO device that SWITCHES are connected to */
#define GPIO_DEVICE_ID_SW XPAR_AXI_GPIO_1_DEVICE_ID

/* GPIO port for SWITCHES */
#define SW_CHANNEL 1

int main(void) {
    printf("-- Entering main() --\r\n");

    XGpio GpioLED;
    XGpio GpioSW;
    int status, statusAll = 1;
    int count = 0;
    XScuTimer Timer;
    XScuTimer_Config *TMRConfigPtr;
    unsigned int cycles, cycles1, cycles2, cycles3;

    // Timer initialization
    TMRConfigPtr = XScuTimer_LookupConfig(XPAR_XSCUTIMER_0_DEVICE_ID);
    XScuTimer_CfgInitialize(&Timer, TMRConfigPtr, TMRConfigPtr->BaseAddr);
    XScuTimer_SelfTest(&Timer);

    // Load the timer
    XScuTimer_LoadTimer(&Timer, TIMER_LOAD_VALUE);

    // Start the timer
    XScuTimer_Start(&Timer);

    status = XGpio_Initialize(&GpioLED, GPIO_DEVICE_ID_LED);
    if (status != XST_SUCCESS) {
        statusAll = 0;
        printf("LEDs initialization failure!\r\n");
    } else {
        XGpio_SetDataDirection(&GpioLED, LED_CHANNEL, 0x00);
        printf("LEDs initialization success!\r\n");
    }

    status = XGpio_Initialize(&GpioSW, GPIO_DEVICE_ID_SW);
    if (status != XST_SUCCESS) {
        statusAll = 0;
        printf("Switches initialization failure!\r\n");
    } else {
        XGpio_SetDataDirection(&GpioSW, SW_CHANNEL, 0xff);
        printf("SWITCHES initialization success!\r\n");
    }

    int i, SW_check;
    int whole, fract;
    float xx;

    if (statusAll) {
		while (1) {
			XScuTimer_LoadTimer(&Timer, TIMER_LOAD_VALUE);
			XScuTimer_Start(&Timer);
			printf(" --------------------------------------------------------------------------------------------------------------\r\n");
			cycles1 = TIMER_LOAD_VALUE - XScuTimer_GetCounterValue(&Timer);
			printf("Cycles loop 1 - %10d \r\n", cycles1);
			SW_check = XGpio_DiscreteRead(&GpioSW, 1);
			printf("\nDIP switches status %x \r\n\n", SW_check);
			XGpio_DiscreteWrite(&GpioLED, 1, count);
			for (i = 0; i < 9999999; i++)
				;
			cycles2 = TIMER_LOAD_VALUE - XScuTimer_GetCounterValue(&Timer);
			printf("Cycles loop 2 - %10d\r\n", cycles2);
			for (i = 0; i < 9999999; i++)
				;
			count++;
			cycles3 = TIMER_LOAD_VALUE - XScuTimer_GetCounterValue(&Timer);
			printf("Cycles loop 3 - %10d \r\n", cycles3);
			cycles = cycles1 + cycles2 + cycles3;
			printf(" --------------------------------------------------------------------------------------------------------------\r\n");
			printf(" Profiling \r\n");
			xx = (float)cycles1 / cycles;
			whole = xx;
			fract = (xx - whole) * 1000;
			printf(" Start - cycles 1 - %d.%3d\r\n", whole, fract);
			xx = (float)cycles2 / cycles;
			whole = xx;
			fract = (xx - whole) * 1000;
			printf(" Start - cycles 2 - %d.%3d\r\n", whole, fract);
			xx = (float)cycles3 / cycles;
			whole = xx;
			fract = (xx - whole) * 1000;
			printf(" Start - cycles 3 - %d.%3d\r\n", whole, fract);
			printf(" --------------------------------------------------------------------------------------------------------------\r\n");
			for (i = 0; i < 99999999; i++)
				;
		}
	}
	printf(" -- exiting main() -- \r\n");
	return 0;
}
