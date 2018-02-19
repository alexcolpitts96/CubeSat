
//#include "fsl_device_registers.h"
#include "MK22F51212.h"

//static int i = 0;

int main(void)
{

		SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
		PORTD_PCR5 |= PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
//Turn on blue LED test

		while(1) {
			GPIOD_PDDR = 0x01 << 5;
			GPIOD_PSOR = 0x01 << 5;
			GPIOD_PCOR = 0x01 << 5;
		}

	    return 0;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////