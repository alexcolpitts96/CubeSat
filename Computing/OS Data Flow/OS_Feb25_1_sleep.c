//#include "fsl_device_registers.h"
#include "MK22F51212.h"
#include <stdio.h>
#include <stdlib.h>

/*
ECE 4040 Senior Design 2017-2018
Project: OS data flow Rev 1.0
Author: Hsuan-Wei Lo 3476309
Created Feb 14, 2018

Updated Feb 25,2018
Sleep Flag Fixed
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
int Tracker=0;

float batraw=0;
float bat=0;

bat = 4;
ADC0_Init();

while(1){
init_sys_module();
//-------------------------ADC_Convert----------------------
batraw=ADC0_Convert();//Print the Read ADC value in the num
//bat=(batraw*3.3f)/(1024.0f);
//----------------------------------------------------------

//Test case


if (Tracker){
    check_bat_flag=0;
}
else check_bat_flag=1;


//-------------------------------------------------
            //Check battery state 1

	while(check_bat_flag==1){

        int BatS1=1;
        Tracker++;//1


        if(!check_bat(bat)){

                sleep_flag=1;
                BatS1=0;
                check_bat_flag=reset;
        }

        if (BatS1){
        		image_iden_flag=1;
                check_bat_flag=reset;
        }

	}

//Sleep Mode Condition
	while(sleep_flag==1){

		//Tracker is to keep in track which state has been entered from 1 to 6 state
		if (Tracker == 1){
			check_bat_flag = 1;
			sleep_flag=reset;  // State 1
		}

//bat=3;
		//Check the battery once it take up from sleep
		if(check_bat(bat)){
				if (Tracker == 2){
					image_iden_flag = 1; // State 2
					Tracker--;//To keep the tracker consistent since it will increment twice
							  // once it return to the currently state
					sleep_flag=reset;//exit the sleep flag loop
				}

				else if (Tracker == 3){
					image_cap_flag = 1; // State 3
					Tracker--;
					sleep_flag=reset;
				}

				else if (Tracker == 4){
					prep_Tx_flag = 0; 	// State 4
					Tracker--;
					sleep_flag=reset;
				}

				else if (Tracker == 5){
					listen_flag =1;  	// State 5
					Tracker--;
					sleep_flag=reset;
				}

				else if (Tracker == 6){
					tx_flag = 1;     	// State 6
					Tracker--;
					sleep_flag=reset;
				}


		else {Tracker=0; sleep_flag=reset; }//if bat still low return to the state 1
		}


	}


//--------------------------------------------------
	//image identification state 2

	while(image_iden_flag==1){
        int imageI=0;//State 2 Condition for image identification
        int BatS2=1;// State 2 condition for tumbling



        Tracker++;//2
			if(!check_bat(bat)){

                //printf("\n No GO STATE 2  \n");
                sleep_flag=1;
                BatS2=0;
                image_iden_flag=reset;

			}

			while((tumb>2)&&BatS2){
               if (count2==2){
                    tumb=1;//force to continue
                    imageI=1;
                        }
                //delay();
               // printf("\n Tumbling too fast \n");
                count2++;
                }

            imageI=1;//No Tumbling has occurred

            if (imageI&&BatS2){
            //printf("\n Checking cycle excced, exit the tumbling check! \n");
            //delay();
			image_cap_flag=1;
			//printf("\n ready for capture in State 2\n");
			//Tracker=0;//Restart the Main program again

			image_iden_flag=reset;
                }

            }

//--------------------------------------------------
        //Capture Image state 3
	while(image_cap_flag==1){
    int BatS3=1;// State 2 condition for tumbling
        //bat=1;


        Tracker++;//3

			if(!check_bat(bat)){

                //printf("\n No GO STATE 3  \n");
                sleep_flag=1;
                BatS3=0;
                image_cap_flag=reset;

                }
            if(BatS3){
            prep_Tx_flag =1;

			//GPIOC_PSOR = 0x01 << 1;//turn on tx
			//printf("\n Image capture in State 3\n");
			//GPIOC_PCOR=0x01 << 1;//turn off tx

			image_cap_flag=reset;
                }

        }


//--------------------------------------------------
        //Transmit Preparation state 4
while(prep_Tx_flag==1){
    int BatS4=1;// State 2 condition for tumbling
        //bat=1;
        Tracker++;//4


        if(!check_bat(bat)){

                //printf("\n No GO STATE 4 \n");
                sleep_flag=1;
                BatS4=0;
                prep_Tx_flag=reset;

                }
            if(BatS4){
            listen_flag=1;
            //printf("\n Image ready for Transmit in State 4 \n");

            prep_Tx_flag=reset;

                }
        }

//--------------------------------------------------
            //Listen state 5
	while(listen_flag ==1){
        int BatS5=1;// State 2 condition for tumbling

        //bat=1;
        Tracker++;//5

			if(!check_bat(bat)){

                //printf("\n No GO STATE 5  \n");
                sleep_flag=1;
                BatS5=0;
                listen_flag=reset;

                }
            if(BatS5){
            tx_flag=1;
            //printf("\n Waiting for packet request in state 5\n");

            listen_flag=reset;
                }
        }

//--------------------------------------------------
        //Transmit state 6
	while(tx_flag==1){
        int BatS6=1;// State 2 condition for tumbling
        request=0;//requestion from ground station
        //bat=1;
        Tracker++;//6

  //Packet request check condition

        /*
        count3++;
        if (count3==2) request=1;
*/
                if(!check_bat(bat)){

                    sleep_flag=1;
                    BatS6=0;
                    tx_flag=reset;

                    }

                if(BatS6){

                Tracker=0;//reset the tracker to start again once all 6 state has finished
                tx_flag=reset;
                    }
                else {listen_flag=1;tx_flag=reset;}
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

		if (bat<3){
        //printf("\n -----------Low Battery Detected----------- \n");
        return 0;
	}
}


void delay(){
int c,d;
 for (c=1; c<50;c++)
    for(d=1;d<=50;d++)
 {}
 }
