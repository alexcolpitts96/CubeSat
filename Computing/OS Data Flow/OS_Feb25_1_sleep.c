//#include "fsl_device_registers.h"
#include "MK22F51212.h"
#include <stdio.h>
#include <stdlib.h>

/*
ECE 4040 Senior Design 2017-2018
Project: OS data flow Rev 1.0
Author: Hsuan-Wei Lo 3476309
Created Feb 14, 2018

Updated Feb 26,2018
Sleep Flag Fixed rev. 1
Coutinuos ADC reading on the battery/GPIO on and off switch for cam and trasmiter
*/

void init_sys_module();
void sleep_now();
int check_bat(int bat);
int check_bat_image(int bat);
int check_image();
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
int tumb = 1;// State 2 Condition for tumbling
int count2=0;
int count3=0;

float batraw=0;
float bat=0;

bat = 4;//Test value 
ADC0_Init();

while(1){
init_sys_module();
//-------------------------ADC_Convert----------------------
batraw=ADC0_Convert();//Print the Read ADC value in the num
//bat=(batraw*3.3f)/(1024.0f);
//----------------------------------------------------------

//Test case
//-------------------------------------------------
//Check battery state 1
			
        if(!check_bat(bat)){
			sleep_flag=1;
        }
		else 
			image_iden_flag=1;//If battery level is good, go to state 2

	}
//----------------------------------------------------
//Sleep Mode Condition
	while(sleep_flag==1){
		//1. Put the System into sleep
		//2. Set a delay block function
		//3. Wakes up after the duration
//bat=3;
		//Check the battery once it take up from sleep
		if(check_bat(bat)){
				sleep_flag=reset;
				}
		//If the battery still low put into sleep again
	}


//--------------------------------------------------
	//image identification state 2

	while(image_iden_flag==1&&!sleep_flag){
        int imageI=0;//State 2 Condition for image identification
        int BatS2=1;// State 2 condition for tumbling


			//Camera Task for identification:
			while((tumb>2)&&BatS2){
               if (count2==2){
                    tumb=1;//force to continue
                    imageI=1;
                        }
				//delay();
				//Tumbling too fast
				count2++;
                }
				//Set the next flag ready
				if (imageI){
					image_cap_flag=1;
                }
			
			//-------------------------------
			if(!check_bat(bat)){
                sleep_flag=1;
                image_iden_flag=reset;
			else 
				image_iden_flag=reset;//exit the state go to next state

			}

        }

//--------------------------------------------------
        //Capture Image state 3
	while(image_cap_flag==1&&!sleep_flag){
        //bat=1;
			
			//Capture the camera Task:
			
			//GPIOC_PSOR = 0x01 << 1;//turn on Camera via MOSFET switch
			//Capture the image 
			//GPIOC_PCOR=0x01 << 1;//turn off the Camera via MOSFET switch
			
			 prep_Tx_flag =1;//Set the Next flag ready 
			 
			if(!check_bat(bat)){
                sleep_flag=1;
                image_cap_flag=reset;
                }
			else 
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
		  
        if(!check_bat(bat)){
               sleep_flag=1;
               prep_Tx_flag=reset;
		}
		else 
			prep_Tx_flag=reset;//exit the state go to next state
  
                }
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
				
			
			if(!check_bat(bat)){
                //printf("\n No GO STATE 5  \n");
                sleep_flag=1;
                listen_flag=reset;
                }
			else
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
        if(!check_bat(bat)){
            sleep_flag=1;
            tx_flag=reset;
            }
		else 
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




int check_bat(int bat){
	
	//Set the All SIM Moudule to default or turn off
	
	//Check the Battery level without the load.
	if (bat<3){
		
	//Reinitialized All the SIM module clock back
	return 0;
	}
	//Reinitialized All the SIM module clock back
}


void delay(){
int c,d;
 for (c=1; c<50;c++)
    for(d=1;d<=50;d++)
 {}
 }
