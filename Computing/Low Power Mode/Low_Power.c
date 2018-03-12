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

#include "fsl_device_registers.h"
#include <stdint.h>
#include "core_cm4.h"
//#define SCB_SCR_SLEEPDEEP_Msk

void sleep ();
void Wakeup_pin_init();
void deep_sleep();

void enter_LLS();
void enter_VLPS();
void VLPR();


int main()
{
	int count=0;
	int status=0;


	//Wakeup_pin_init();// GPIO interrupt
	status=SMC_PMSTAT;
	VLPR();
	status=SMC_PMSTAT;

	while(1){
		status=SMC_PMSTAT;// check the system in which power mode 1=RUN,
		count++; //make it do stuff
	}
return 0;
}




void Wakeup_pin_init(){
	SIM_SCGC5  |= SIM_SCGC5_PORTB_MASK;
	PORTB_PCR0 |= PORT_PCR_ISF_MASK;
	PORTB_PCR0 |= PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	PORTB_PCR0 |= PORT_PCR_IRQC(12);


}

void deep_sleep(){


    //SMC_PMPROT =0b00100000;
    SMC_PMPROT |= SMC_PMPROT_AVLP_MASK;//allow very low power mode

    SMC_PMCTRL |= 0b01000000; //desired low power mode

    SMC_STOPCTRL |= 0b01100000; //partial stop with both system and bus clock disabled pg 357


    //SCB->SCR|= SCB_SCR_SLEEPDEEP_Msk;
    SCB->SCR |=0b00110;//  Set/Clear ARM SLEEPDEEP bit for STOP/WAIT.

    __WFI();// Wait for interrupt

    //SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    //SCR|= 0b00110;
    //SMC_PMCTRL|=0x2;
    //SMC_PMCTRL|=SMC_PMCTRL_STOPM(2);


}

void VLPR(){


    //SMC_PMPROT =0b00100000;
    SMC_PMPROT |= SMC_PMPROT_AVLP_MASK;//allow very low power mode

    SMC_PMCTRL |= 0b00000010; //desired low power mode

    //SMC_STOPCTRL |= 0b00000000;//test first 3 MSB
                               //partial stop with both system and bus clock disabled pg 357

    SCB->SCR |=0b10110;

    __WFI();
}


//Example
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
//Example
void enter_LLS(void)
{
  SMC_PMPROT = SMC_PMPROT_ALLS_MASK;
  /* Set the STOPM field to 0b011 for LLS mode  */
  SMC_PMCTRL &= ~SMC_PMCTRL_STOPM_MASK;
    SMC_PMCTRL &= ~SMC_PMCTRL_RUNM(0X3);
  SMC_PMCTRL |=  SMC_PMCTRL_STOPM(0x3);
  /*wait for write to complete to SMC before stopping core */

  /* Now execute the stop instruction to go into VLLS */
  //stop();
    SCB->SCR |= 0x4u;

    /* WFI instruction will start entry into STOP mode */
    //asm("WFI");
    __WFI();

}
/***************************************************************/


/*
Grave Yard


Wakeup_pin_init();
    	SMC_init();
    	   SCB->SCR |=0b00110;
    	__DSB(); // Ensure effect of last store takes effect
    	__WFI(); // Enter sleep mode
    	status=SMC_PMSTAT;// Power Mode Status register


    	//__ASM volatile ("wfi");//Wait for interrupt


//--------------------------------------------------------


void RTC_wakeup(){

	RTC_TSR = 0;
	RTC_IER |= RTC_IER_TAIE_MASK;// Enable alarm interrupt
		while(1){
			if ((RTC_SR&0x4)==0x4){
				break;
			}
		}

}

	//SCB_SCR |= SCB_SCR_SLEEPDEEP_Msk;
	//SMC_PMCTRL &= ~SMC_PMCTRL_STOPM_MASK;
	//SMC_PMCTRL |= SMC_PMCTRL_STOPM(0x2);
    //__ASM volatile ("wfi");//Wait for interrupt

    // PTB 0 set as wake up pin LLWU_P5
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB_PCR0 |= PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	LLWU_PE2 |= LLWU_PE2_WUPE5_MASK; // set the external input pin enable with any change detection
	//LLWU_PE2_WUPE5(x);

	//LLWU_ME |= LLWU_ME_WUME5_MASK; //LLWU_P5 used internal module flag as wake up source
    LLWU_F1 |= LLWU_F1_WUF5_MASK;
 */


/***************************************************************/
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
