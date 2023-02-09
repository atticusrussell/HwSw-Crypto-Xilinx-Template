#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"

#include "xparameters.h"
#include "xmbox.h"
#include "sleep.h"

int main()
{
    init_platform();

    printf("Hello World from core %d\n\r", XPAR_CPU_ID);

    XMbox_Config *cfg = XMbox_LookupConfig(XPAR_CPU_ID);
    XMbox mbox;

    XMbox_CfgInitialize(&mbox, cfg, cfg->BaseAddress);

    printf("Core %d writing to Mailbox\n\r", XPAR_CPU_ID);
    XMbox_WriteMBox(cfg->BaseAddress, 0xBBCCDDEE);

    sleep(5);

    while(XMbox_IsEmpty(&mbox));

    printf("Core %d received %08X\n\r", XPAR_CPU_ID, XMbox_ReadMBox(cfg->BaseAddress));

    return 0;
}
