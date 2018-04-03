/*
 * OS_Function.c
 *
 *  Created on: Mar 21, 2018
 *      Author: Hsuan-Wei Lo
 */

#include "fsl_device_registers.h"
#include "OS.h"
#include "../Sources/main.h"

#define V_REF 3.3

void MOSFET_ON_Cam(){
	PORTC_PCR3 |= PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
    GPIOC_PDDR = 0x01 << 3; //PTC3
    GPIOC_PSOR = 0x01 << 3;//turn ON Camera via MOSFET switch
}	

void MOSFET_OFF_Cam(){
	GPIOC_PCOR = 0x01 << 3;//turn OFF Camera via MOSFET switch
}	

void MOSFET_ON_RF(){
	PORTC_PCR6 |= PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
    GPIOC_PDDR = 0x01 << 6; //PTC6
    GPIOC_PSOR = 0x01 << 6;//turn ON RF via MOSFET switch
}	

void MOSFET_OFF_RF(){
	GPIOC_PCOR = 0x01 << 6;//turn OFF RF via MOSFET switch
}	

void disable_modules() {
	// turn off port clocks, NOT and then masks to the register
	SIM_SCGC5 &= ~SIM_SCGC5_PORTA_MASK;
	SIM_SCGC5 &= ~SIM_SCGC5_PORTB_MASK;
	SIM_SCGC5 &= ~SIM_SCGC5_PORTC_MASK;
	SIM_SCGC5 &= ~SIM_SCGC5_PORTD_MASK;
	SIM_SCGC5 &= ~SIM_SCGC5_PORTE_MASK;

	// turn off module clocks, NOT and then mask to the register
	SIM_SCGC4 &= ~SIM_SCGC4_I2C0_MASK;
	SIM_SCGC6 &= ~SIM_SCGC6_SPI1_MASK;
	SIM_SCGC6 &= ~SIM_SCGC6_SPI0_MASK;
	SIM_SCGC6 &= ~SIM_SCGC6_FTM0_MASK;
	SIM_SCGC6 &= ~SIM_SCGC6_FTM1_MASK;
}

void LPTMR0_enable(int miliseconds) {
	//call this function when low power is needed. This function will activate the LPTMR and its interrupt. The
	// board will go to sleep for "miliseconds" and then will wake up using the lptmr interrupt

	disable_modules();

	SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK; //enable system clock
	//Reset LPTMR module
	LPTMR0_CSR = 0x00;
	LPTMR0_PSR = 0x00;
	LPTMR0_CMR = 0x00;

	// Enable LPT Interrupt in NVIC
	NVIC_EnableIRQ(LPTMR0_IRQn);

	SIM_SOPT1 |= 0xC0000;  // setting 1khz internal clock to LPTMR
	LPTMR0_CMR = LPTMR_CMR_COMPARE(miliseconds);  //Set compare value
	LPTMR0_PSR |= 0x45; //Prescaler Register, Prescale disabled, 1 count = 1 milisecond
	LPTMR0_CSR = LPTMR_CSR_TIE_MASK;  //Enable interrupt
	LPTMR0_CSR |= LPTMR_CSR_TEN_MASK; //Turn on LPTMR and start counting

	// Enable Low Power Mode
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk; //Enable Sleepdeep instead of sleep
	SMC_PMPROT |= 0x20; //allow VLPS to be activated

	SMC_STOPCTRL &= 0x40; // Enabling PSTOP1

	SMC_PMCTRL |= 0x2; // Enter VLPS
	SMC_PMCTRL; // dummy read
	__WFI();   // Wait for interrupt //this line starts VLPS
}

void LPTMR0_IRQHandler(void) {

	LPTMR0_CSR |= LPTMR_CSR_TCF_MASK; // set the TCF flag to 0
	LPTMR0_CSR &= 0xFE; //disable the counter
	master_init(); //
}

void ADC0_Init() {

	//Pick Port ADC0_DP0 for the testing

	//Pick Port PTB0 ADC0_SE8/ADC1_SE8 ALT0 pg219

	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK; //0x08000000
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB_PCR0 |= PORT_PCR_MUX(0) | PORT_PCR_DSE_MASK;

	ADC0_SC1A = 0b0001000;
	//ADC0_SC1A = 0b0001111; //Mask the ADCH selection ADC0_SE14

	ADC0_CFG1 |= 0b00011100; // Set the Mode in 16 as it to have 10-bit conversion, with long sample time
							 // Set the ADIV (clock Divide Select) ratio is 1
	ADC0_CFG2 |= 0x00;
	ADC0_SC2 |= 0x00;// ADTRG software triiger, DMA disabled, Voltage Refereence (REFSEL) default voltage reference =1.207
					 // ACREN Range function disabled
	ADC0_SC3 |= 0x00;

}

float ADC0_Convert() {

	ADC0_SC1A &= ADC_SC1_ADCH(8); // Mask the ADCH selection
	while (1) {
		if ((ADC0_SC1A & 0x80) == 0x80) {
			break;
		}
	}

	// return the value read from the ADC
	return ADC0_RA;

}

void ADC1_Init() {
	//Pick Port ADC0_DP3 for the testing
	//PTB1/ADC0_SE9/ADC1_SE9
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB_PCR1 |= PORT_PCR_MUX(0) | PORT_PCR_DSE_MASK;

	SIM_SCGC6 |= SIM_SCGC6_ADC1_MASK;		//0x08000000
	ADC1_SC1A = 0b0001001; //Mask the ADCH selection

	ADC1_CFG1 |= 0b00011100; // Set the Mode in 16 as it to have 10-bit conversion, with long sample time
							 // Set the ADIV (clock Divide Select) ratio is 1
	ADC1_CFG2 |= 0x00;
	ADC1_SC2 |= 0x00; // ADTRG software trigger, DMA disabled, Voltage Refereence (REFSEL) default voltage reference =1.207
					  // ACREN Range function disabled
	ADC1_SC3 |= 0x00;
}

float ADC1_Convert() {

	ADC1_SC1A &= ADC_SC1_ADCH(9); //Mask the ADCH selection
	while (1) {
		if ((ADC1_SC1A & 0x80) == 0x80) {
			break;
		}
	}
	return ADC1_RA; //Return the value read from the ADC register

}

uint8_t check_bat() {
	float batraw = 0;
	float bat = 0;

	//uint8_t sleep_flag;

	// turn off all modules except for ADC0
	disable_modules();
	ADC0_Init();

	// read in ADC value and convert to voltage
	batraw = ADC0_Convert();
	bat = (batraw * V_REF) / (65536.0f);

	// if the system is in sunlight
	if (check_solar()) {

		// sleep if the battery voltage is too low
		if (bat < 2.6)
			//sleep_flag = 1;
			return 1;

		// stay awake
		else {
			//sleep_flag = 0;
			master_init(); // init modules when exiting check
			return 0;

			//GPIOC_PCOR = 0x01 << 1;
		}
	}

	// no sunlight
	else {
		// sleep if the battery voltage is too low
		if (bat < 2.5) {
			//sleep_flag = 1;
			return 1;

		// stay awake
		} else {
			//sleep_flag = 0;
			master_init(); // init the modules when exiting check
			return 0;

			//GPIOC_PCOR = 0x01 << 1;
		}
	}
}

uint8_t check_solar() {
	float solraw = 0;
	float sol = 0;

	// turn off all modules except ADC1
	disable_modules();
	ADC1_Init();

	// read ADC and convert to voltage
	solraw = ADC1_Convert();
	sol = (solraw * 3.3f) / (65536.0f);

	if (sol > 2.4) {
		return 1;
		//LED1();
	} else {
		return 0;
		//GPIOC_PCOR = 0x01 << 2;
	}

}

//----------------------------LED------------------------------
void LED() {
	//PTA1 Red LED
	//PTD5 Blue LED
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
	PORTC_PCR1 |= PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	GPIOC_PDDR = 0x01 << 1; //PTC1
	GPIOC_PSOR = 0x01 << 1; //turn on Camera via MOSFET switch
}

void LED1() {
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
	PORTC_PCR2 |= PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	GPIOC_PDDR = 0x01 << 2; //PTC2
	GPIOC_PSOR = 0x01 << 2; //turn on Camera via MOSFET switch

}

//--------------------------WatchDog Timer-------------------------------

void WDT_int(void) {
	// Status and Control Register High
	WDOG_STCTRLH |= WDOG_STCTRLH_WDOGEN_MASK; //enable WDOG
	WDOG_STCTRLH |= WDOG_STCTRLH_DBGEN_MASK;  //WDOG enable in CPU debug mode
	WDOG_STCTRLH |= WDOG_STCTRLH_TESTWDOG_MASK; // Put the WDOG in the quick test mode for small amount value

	//Time-Out Value Register
	//WDOG_TOVALH = 0x0000;
	//WDOG_TOVALL |= 0x5;
	WDOG_TOVALL = WDOG_TOVALL_TOVALLOW(1000000);

	WDOG_TMROUTH = 0x0;
	WDOG_TMROUTL = 0x0;

	WDOG_PRESC |= WDOG_PRESC_PRESCVAL(0);	// set our frequency 1 kHz -> 1 mSec

}

void Refresh_Dog(void) {
	WDOG_REFRESH = 0xA602;
	WDOG_REFRESH = 0xB480;
}

void Unleash_Dog(void) {
	WDOG_UNLOCK = 0xC520;
	WDOG_UNLOCK = 0xD928;
}

//----------------------------Miscellaneous-------------------------------

void Wait32() {
	for (int i = 0; i < 15000; i++) {
		__asm__("nop");
	}
	return;
}

void delay() {
	int c, d;
	for (c = 1; c < 50; c++)
		for (d = 1; d <= 50; d++) {
		}
}

