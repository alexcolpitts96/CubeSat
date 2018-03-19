/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// standard libraries
#include "fsl_device_registers.h"
#include "core_cm4.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"

// custom libraries
#include "../RFM69/RFM69_driver.h"
#include "../UART1/UART1_driver.h"
#include "../FTM/FTM_driver.h"
#include "../UART0/UART0_driver.h"
#include "../SPI0/SPI0_driver.h"
#include "../GPIO/gpio.h" // included in RFM69 driver
#include "../Comms/Comms.h"
#include "../Camera/camera.h"
#include "../I2C/i2c.h"

void delay();
void deep_sleep();
void Wakeup_pin_init();
void master_init();
void LPTMR0_init();
void LPTMR0_Interrupt_Delay(int seconds);
void Wait32();
void enter_VLPS(void);
void deep_sleep_int();

int main() {
	master_init();
	LPTMR0_Interrupt_Delay(20); //Wait "n" seconds before wake up

	//This block is just to confirm the timer is working via putty terminal
	/////////////////////////////////////////////////////////////////////////////////

	//int TCF_Check=LPTMR0_CSR;
	uint8_t status=0;

	// print out current timer count
		while(1){


			LPTMR0_CNR=1; //you must write a value to counter for syncing
			//enter_VLPS();
			deep_sleep();
			status=SMC_PMSTAT;
			/*
			int Time_Passed=LPTMR0_CNR; //retrieving count value
			// Printing current count value
			char buf[200];
			sprintf(buf, "\033[2J\033[1;1H count: %d ",(Time_Passed));
					putty_putstr(buf);

			// Printing the value for the TCF flag when the interupt should be sent
			char buf1[100];
			sprintf(buf1, "\n\r Reset And Interrupt value %d ",(LPTMR0_CMR));
			putty_putstr(buf1);

			// Printing When TCF flag is triggered
			int TCF_Check2=LPTMR0_CSR;
			if(TCF_Check2!=TCF_Check){  //check TCF
						putty_putstr("\n\r TCF triggered (interrupt sent) \n\r Note: The counter still counts until you disable it");
					}

			*/

			status=SMC_PMSTAT;
			if((LPTMR0_CSR&0x80)==0x80){
				//LPTMR0_CSR|=0x40;
				LPTMR0_CSR|=0x0;
			}
	//Wait32(); // Makes the Putty terminal readable
	/////////////////////////////////////////////////////////////////////////////



		}
		return 0;
}

void master_init() {
	//UART0_Init();
	//UART1_putty_init();
	LPTMR0_init();
	deep_sleep_int();
	//SPI0_Init(16);
	//RFM69_DIO0_Init();
	//RFM69_Init(); // must always be after the SPI interface has been enabled
	//FTM0_init();
	//FTM1_init();
	//init_I2C();
	//SPI1_Init(16);
	//camera_init();
}

void LPTMR0_init() {
	// Initialize the LPTMR
	SIM_SCGC5 |=SIM_SCGC5_LPTMR_MASK; // Sim clock
	LPTMR0_CSR&=0xE; // disable timer LPTMR0_Interrupt_Delay(int seconds)as first step, This needs to be done or the counter will resume from its last value
	SIM_SOPT1|=0xC0000;  // setting 1khz internal clock to LPTMR
	LPTMR0_CSR&=0x00; // clear CSR
	LPTMR0_CSR|=0x00;  //Status Register, Set to timer Mode, Reset on TCF flag
	LPTMR0_PSR|=0x41;  //Prescaler Register, prescale=1/1024 so 1 count =~1 second
	return;
}

void LPTMR0_Interrupt_Delay(int seconds) {
	// call this before entering low power mode. This will wakeup the core after a delay of "seconds"
	// via an interrupt. "Seconds" is how long you want to wait before wakeup in seconds (obviously)
	// The timer starts immediately after this function is called
	//Note: Unless the timer is disabled, the interrupt will activated after a period of "seconds".
	// Not sure if this will affect anything.
	LPTMR0_CMR&=0x0000; // reset compare value to 0
	LPTMR0_CMR|=seconds; //compare register, value in seconds that the counter counts to before it resets and sends an interrupt
	LPTMR0_CSR|=0x40;  //Enable TIE (timer interrupt enable as last init step)
	LPTMR0_CSR|=0x1; // enable timer as last step
	return;
}




// This wait function makes it easier to step through for debugging
void Wait32() {
	for(int i = 0;i<7500;i++){
				Pause();
			}
	return;
}

void deep_sleep(){

    //SCB->SCR |=0b00110;
	SCB->SCR|= SCB_SCR_SLEEPDEEP_Msk;
	__WFI();


}

void deep_sleep_int(){


    //SMC_PMPROT =0b00100000;
    SMC_PMPROT |= SMC_PMPROT_AVLP_MASK;//allow very low power mode

    SMC_PMCTRL |= 0b01000000; //desired low power mode

    SMC_STOPCTRL |= 0b01100000; //partial stop with both system and bus clock disabled pg 357
}

void Wakeup_pin_init(){
	SIM_SCGC5  |= SIM_SCGC5_PORTB_MASK;
	PORTB_PCR0 |= PORT_PCR_ISF_MASK;
	PORTB_PCR0 |= PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	PORTB_PCR0 |= PORT_PCR_IRQC(12);


}

void enter_VLPS(void)
{
  SMC_PMPROT = SMC_PMPROT_AVLP_MASK;
  /* Set the STOPM field to 0b011 for LLS mode  */
  SMC_PMCTRL &= ~SMC_PMCTRL_STOPM_MASK;
    SMC_PMCTRL &= ~SMC_PMCTRL_RUNM(0X3);
  SMC_PMCTRL |=  SMC_PMCTRL_STOPM(0x2);
  /*wait for write to complete to SMC before stopping core */

  /* Now execute the stop instruction to go into VLLS */
  //stop();
    SCB->SCR |= 0x4u;

    /* WFI instruction will start entry into STOP mode */
    //asm("WFI");
    __WFI();

}

void delay(){
int c,d;
 for (c=1; c<50;c++)
    for(d=1;d<=50;d++)
 {}
 }
