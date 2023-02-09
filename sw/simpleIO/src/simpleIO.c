#include "stdio.h"
#include "xgpio.h"
#include "xparameters.h"
#include "xil_printf.h"
#include "sleep.h"
#include "platform.h"

/*Xilinx version of printf - smaller footprint */
#define printf xil_printf

/* GPIO device that LEDs are connected to */
#define GPIO_DEVICE_ID_LED XPAR_AXI_GPIO_0_DEVICE_ID
/* GPIO port for LEDs */
#define LED_CHANNEL 1

/* GPIO device that SWITCHES are connected to */
#define GPIO_DEVICE_ID_SW XPAR_AXI_GPIO_1_DEVICE_ID
/* GPIO port for SWITCHES */
#define SW_CHANNEL 1

int main(){
	init_platform();

	printf("-- Entering main() -- \r\n");
	XGpio GpioLED;
	XGpio GpioSW;

	int SW_check;
	XGpio_Initialize(&GpioLED, GPIO_DEVICE_ID_LED);
	XGpio_SetDataDirection(&GpioLED, LED_CHANNEL, 0x00);

	XGpio_Initialize(&GpioSW, GPIO_DEVICE_ID_SW);
	XGpio_SetDataDirection(&GpioSW, SW_CHANNEL, 0xff);

	while(1){
		SW_check = XGpio_DiscreteRead(&GpioSW, 1);
		printf("DIP switches status %x \r\n", SW_check);
		XGpio_DiscreteWrite(&GpioLED, 1, SW_check);

		sleep(1);

		if(SW_check == 0x0f)break;
	}
	printf(" -- exiting main() -- \r\n");
	cleanup_platform();
	return 0;

}
