//#include "fsl_device_registers.h"
#include "MK22F51212.h"
#include <stdio.h>
#include <stdlib.h>

/*
ECE 4040 Senior Design 2017-2018
Project: OS data flow Rev 1.0
Author: Hsuan-Wei Lo 3476309
Created Feb 14, 2018

Updated Feb 28,2018
Sleep Flag Fixed rev. 2
Continue on ADC reading on the battery/GPIO on and off switch for camera and transmitter
/+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++/
log:
Feb 28 Combine ADC reading with check bat into one function
/+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++/
*/

void init_sys_module();
void sleep_now();
void check_bat();


void ADC0_Init ();
float ADC0_Convert();

//Global Flag Register
int check_bat_flag  = 0;// State 1
int image_iden_flag = 0;// State 2
int image_cap_flag  = 0;// State 3
int prep_Tx_flag = 0; 	// State 4
int listen_flag = 0;  	// State 5
int tx_flag = 0;     	// State 6

//Global Sleep Flag Condition
int sleep_flag=0;//Check battery state




int main(){


int reset=0;
int request=0;

int count2=0;
int count3=0;

ADC0_Init();

check_bat_flag=1;//Start the first state in the while 1 loop

//-------------------------------------------------
		//Check battery state 1

while(check_bat_flag==1){
         	//1. Put the System into sleep
        	//2. Set a delay block function
        	//3. Wakes up after the duration

        check_bat();
        if (!sleep_flag){
        	image_iden_flag=1;check_bat_flag=reset;}//If battery level is good, go to state 2
}
//-------------------------------------------------
while(1){
init_sys_module();

//Test case

//----------------------------------------------------
//Sleep Mode Condition
	while(sleep_flag==1){
		//1. Put the System into sleep
		//2. Set a delay block function
		//3. Wakes up after the duration
//bat=3;
		//Check the battery once it take up from sleep
		check_bat();

		//If the battery still low put into sleep again
	}


//--------------------------------------------------
	//image identification state 2

	while(image_iden_flag==1&&!sleep_flag){
		int imageI = 1;

				//Set the next flag ready
				if (imageI){
					image_cap_flag=1;
                }

			//-------------------------------
			check_bat();
			image_iden_flag=reset;//exit the state go to next state

			}



//--------------------------------------------------
        //Capture Image state 3
	while((image_cap_flag==1)&&!sleep_flag){
        //bat=1;

			//Capture the camera Task:

			//GPIOC_PSOR = 0x01 << 1;//turn on Camera via MOSFET switch
			//Capture the image
			//GPIOC_PCOR=0x01 << 1;//turn off the Camera via MOSFET switch

			 prep_Tx_flag=1;//Set the Next flag ready

			 check_bat();
			 image_cap_flag=reset;//exit the state go to next state

        }


//--------------------------------------------------
        //Transmit Preparation state 4

	while(prep_Tx_flag==1&&!sleep_flag){
        //bat=1;

		//Transmit Preparation:
			//Setup the Transmition in SPI module ready
			//Check the image size or location in the flash memory

		  listen_flag=1;//Set the next flag ready

		  check_bat();
		  prep_Tx_flag=reset;//exit the state go to next state

                }

//--------------------------------------------------
            //Listen state 5
	while(listen_flag ==1&&!sleep_flag){

    //bat=1;
			//Listen Mode:
				//Listen to the ground station for packet request
				//If (packet_request == 1){
				  tx_flag=1;//If ground station is heard set the next flag ready
				// }


				check_bat();
				listen_flag=reset;

        }

//--------------------------------------------------
        //Transmit state 6
	while(tx_flag==1&&!sleep_flag){
        request=0;//requestion from ground station
        //bat=1;
		 /*
		//Packet request check condition

        count3++;
        if (count3==2) request=1;
		*/

		//Transmit mode:
			//While the ground station still heard, then transmit the image
			//If (packet_request==1){
				//Transmit the image to the ground station function
				//Clear the image

			// }

			//else {listen_flag=1;tx_flag=reset;}// go back to the listen mode again if ground station lost communication


        	image_iden_flag=1;
        	check_bat();
        	tx_flag=reset;//Start again from the beginning

			}

//---------------------------End Of Line ----------------------------------
		}//While(1) Function

	}//Main Function



void init_sys_module(){

	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK; // Enable port A and B
	//SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;

	PORTC_PCR1 |= PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; // Use PTC1 as GPIO
	PORTC_PCR2 |= PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; // Use PTC2 as GPIO


	GPIOC_PDDR = 0x01 << 1;

}


void ADC0_Init (){

	//Pick Port ADC0_DP0 for the testing

	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;//0x08000000

	ADC0_CFG1 |= 0b00101000;// Set the Mode in 10 as it to have 10-bit conversion
							// Set the ADIV (clock Divide Select) ratio is 1
	ADC0_CFG2 |= 0x00;
	ADC0_SC2 |= 0x00;// ADTRG software triiger, DMA disabled, Voltage Refereence (REFSEL) default voltage reference =1.207
					  // ACREN Range function disabled
	ADC0_SC3 |= 0x00;
	ADC0_SC1A = 0b0000000; //Mask the ADCH selection

}

float ADC0_Convert() {

	ADC0_SC1A &= ADC_SC1_ADCH(0); //Mask the ADCH selection
	while(1) {
		if ((ADC0_SC1A&0x80)==0x80) {
			break;
		}
	}
	return ADC0_RA;//Return the value read from the ADC register

	}




void check_bat(){
	float batraw=0;
	float bat=0;

	//Set the All SIM Moudule to default or turn off expect ADC module

	//-------------------------ADC_Convert----------------------
	batraw=ADC0_Convert();//Print the Read ADC value in the num
	bat=(batraw*3.3f)/(1024.0f);
	//----------------------------------------------------------
	//Check the Battery level without the load.
	if (bat<3)
		sleep_flag=1;
	else
		sleep_flag=0;
	//Reinitialized All the SIM module clock back
}


void delay(){
int c,d;
 for (c=1; c<50;c++)
    for(d=1;d<=50;d++)
 {}
 }
