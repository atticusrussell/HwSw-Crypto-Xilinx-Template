#include "stdio.h"
#include "xgpio.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "platform.h"
#include "xtime_l.h"

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
  int count = 0;
  XTime time;
  unsigned int cycles, cycles1, cycles2, cycles3;

  // Load and start the timer
  XTime_SetTime(0);

  int whole, fract;
  float xx;

  if (statusAll) {
    while (1) {
      XTime_SetTime(0);
      printf("--------------------------------------------------------------------------------------------------------------\r\n");
      XTime_GetTime(&time);
      cycles1 = time;
      printf("Cycles loop 1 - %10d\r\n", cycles1);
      SW_check = XGpio_DiscreteRead(&GpioSW, 1);
      printf("\nDIP switches status %x\r\n\n", SW_check);
      XGpio_DiscreteWrite(&GpioLED, 1, count);
      for (i = 0; i < 99999; i++)
        ;
      XTime_GetTime(&time);
      cycles2 = time;
      printf("Cycles loop 2 - %10d\r\n", cycles2);
      for (i = 0; i < 99999; i++)
        ;
      count++;
      XTime_GetTime(&time);
      cycles3 = time;
      printf("Cycles loop 3 - %10d\r\n", cycles3);
      cycles = cycles1 + cycles2 + cycles3;
      printf("--------------------------------------------------------------------------------------------------------------\r\n");
      printf("Profiling\r\n");
      xx = (float)cycles1 / cycles;
      whole = xx;
      fract = (xx - whole) * 1000;
      printf("Start - cycles 1 - %d.%3d\r\n", whole, fract);
      xx = (float)cycles2 / cycles;
      whole = xx;
      fract = (xx - whole) * 1000;
      printf("Start - cycles 2 - %d.%3d\r\n", whole, fract);
      xx = (float)cycles3 / cycles;
      whole = xx;
      fract = (xx - whole) * 1000;
      printf("Start - cycles 3 - %d.%3d\r\n", whole, fract);
      printf("--------------------------------------------------------------------------------------------------------------\r\n");
      for (i = 0; i < 99999999; i++)
        ;
    }
  }

  printf("-- Exiting main() --\r\n");
  return 0;
}
