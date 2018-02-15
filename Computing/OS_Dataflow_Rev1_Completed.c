#include <stdio.h>
#include <stdlib.h>

/*
ECE 4040 Senior Design 2017-2018
Project: OS data flow Rev 1.0
Author: Hsuan-Wei Lo 3476309
Updated Feb 14, 2017
*/

void init_sys_module();
void sleep_now();
int check_bat(int bat);
int check_bat_image(int bat);
int check_image();

//Global Flag Register
int check_bat_flag  = 0;// State 1
int image_iden_flag = 0;// State 2
int image_cap_flag  = 0;// State 3
int prep_Tx_flag = 0; // State 4
int listen_flag = 0;  // State 5
int tx_flag = 0;      // State 6

//Global Sleep Flag Condition
int sleep_flag1=0;//Check battery state
int sleep_flag2=0;//Image identification state
int sleep_flag3=0;//Capture image state
int sleep_flag4=0;//Transmit preparation state
int sleep_flag5=0;//Listen flag
int sleep_flag6=0;//Transmit flag

//Global Check Battery flag Condition
//int check_bat_flag1= 0;
int check_bat_flag2= 0;
int check_bat_flag3= 0;
int check_bat_flag4= 0;
int check_bat_flag5= 0;
int check_bat_flag6= 0;


int main(){


int reset=0;
int bat  = 0;//Battery condition for all state
int tumb = 1;// State 2 Condition for tumbling
int count2=0;
int count3=0;
int request=1;
int Tracker=0;
while(1){


printf("\n +++++++++++++++++++++++++++New_Start+++++++++++++++++++++++++++++  \n");

if (Tracker){
    check_bat_flag=0;
}
else check_bat_flag=1;


//-------------------------------------------------
            //Check battery state 1

	while(check_bat_flag==1){

        int BatS1=1;
        Tracker++;//1

        printf("\n Enter current battery level for state 1: ");
        scanf("%d", &bat);

        if(!check_bat(bat)){
                printf("\n No GO STATE 1  \n");
                sleep_flag1=1;
                BatS1=0;
                check_bat_flag=reset;
        }

        if (BatS1){
                image_iden_flag=1;
                printf("\n Battery is good in state 1 \n");
                check_bat_flag=reset;
        }

	}


	while(sleep_flag1==1){
		printf("\n Battery is Low on state 1, Entering Sleep Mode \n");
		printf("\n +Waiting battery to charge+ \n");
		delay();
		printf("\n Wake Up from Sleep Mode State 1 \n");
        Tracker=0;//Tracker Importance
		sleep_flag1=reset;
	}



//--------------------------------------------------
	//image identification state 2

	while(image_iden_flag==1){
        int imageI=0;//State 2 Condition for image identification
        int BatS2=1;// State 2 condition for tumbling

        printf("\n Enter current battery level for state 2: ");
        scanf("%d", &bat);
        printf("\n Enter current tumbling condition for state 2: ");
        scanf("%d", &tumb);
        //printf("\n <<<<<<<<<<< Check Point State 2 >>>>>>>>>>> \n");

        // bat=1;
        //tumb=4;

        Tracker++;//2
			if(!check_bat(bat)){

                printf("\n No GO STATE 2  \n");
                sleep_flag2=1;
                BatS2=0;
                image_iden_flag=reset;

			}

			while((tumb>2)&&BatS2){
               if (count2==2){
                    tumb=1;//force to continue
                    imageI=1;
                        }
                delay();
                printf("\n Tumbling too fast \n");
                count2++;
                }

            imageI=1;//No Tumbling has occurred

            if (imageI&&BatS2){
            printf("\n Checking cycle excced, exit the tumbling check! \n");
            //delay();
			image_cap_flag=1;
			printf("\n ready for capture in State 2\n");

			//Tracker=0;//Restart the Main program again

			image_iden_flag=reset;
                }

            }

	while(sleep_flag2==1){
		printf("\n Battery is Low on state 2, Entering Sleep Mode \n");
		printf("\n +Waiting battery to charge+ \n");
		delay();
		printf("\n Wake Up from Sleep Mode State 2 \n");

        printf("\n Enter current battery level for state 2 [wake up]: ");
        scanf("%d", &bat);

		//bat=4;
		//Return to the State 1
		if(!check_bat(bat)){
			//check_bat_flag=reset;
			sleep_flag2=reset;
			Tracker=0;//Tracker Importance
		}
		else {image_iden_flag=1;
            sleep_flag2=reset;}

	}


//--------------------------------------------------
        //Capture Image state 3
	while(image_cap_flag==1){
    int BatS3=1;// State 2 condition for tumbling
        //bat=1;

        printf("\n Enter current battery level for state 3: ");
        scanf("%d", &bat);

        Tracker++;//3

			if(!check_bat(bat)){

                printf("\n No GO STATE 3  \n");
                sleep_flag3=1;
                BatS3=0;
                image_cap_flag=reset;

                }
            if(BatS3){
            prep_Tx_flag =1;
            printf("\n Image capture in State 3\n");
            //Tracker=0;//Restart the Main program again
            image_cap_flag=reset;
                }
        }

	while(sleep_flag3==1){
		printf("\n Battery is Low on state 3, Entering Sleep Mode \n");
		printf("\n +Waiting battery to charge+ \n");
		delay();
		printf("\n Wake Up from Sleep Mode State 3 \n");

        printf("\n Enter current battery level for state 2 [wake up]: ");
        scanf("%d", &bat);
		//bat=4;
		if(check_bat(bat)){
			image_cap_flag=1;//Reset State 2
			sleep_flag3=reset;
		}
		else {Tracker=0;sleep_flag3=reset;}
	}



//--------------------------------------------------
        //Transmit Preparation state 4
while(prep_Tx_flag==1){
    int BatS4=1;// State 2 condition for tumbling
        //bat=1;
        Tracker++;//4

        printf("\n Enter current battery level for state 4: ");
        scanf("%d", &bat);

        //printf("\n <<<<<<<<<<< Check Point State 4 >>>>>>>>>>> \n");
        if(!check_bat(bat)){

                printf("\n No GO STATE 4 \n");
                sleep_flag4=1;
                BatS4=0;
                prep_Tx_flag=reset;

                }
            if(BatS4){
            listen_flag=1;
            printf("\n Image ready for Transmit in State 4 \n");
            //Tracker=0;
            prep_Tx_flag=reset;

                }
        }

    while(sleep_flag4==1){
		printf("\n Battery is Low on state 4, Entering Sleep Mode \n");
		printf("\n +Waiting battery to charge+ \n");
		delay();
		printf("\n Wake Up from Sleep Mode State 4 \n");

        printf("\n Enter current battery level for state 4 [wake up]: ");
        scanf("%d", &bat);

		//bat=4;
		if(check_bat(bat)){
			prep_Tx_flag=1;//Go back to state 4
			sleep_flag4=reset;
		}
		else {Tracker=0;sleep_flag4=reset;}
	}

//--------------------------------------------------
            //Listen state 5
	while(listen_flag ==1){
        int BatS5=1;// State 2 condition for tumbling
        //bat=1;
        Tracker++;//5

        printf("\n Enter current battery level for state 5: ");
        scanf("%d", &bat);

			if(!check_bat(bat)){

                printf("\n No GO STATE 5  \n");
                sleep_flag5=1;
                BatS5=0;
                listen_flag=reset;

                }
            if(BatS5){
            tx_flag=1;
            printf("\n Waiting for packet request in state 5\n");
            //Tracker=0;
            listen_flag=reset;
                }
        }
    while(sleep_flag5==1){
		printf("\n Battery is Low on state 5, Entering Sleep Mode \n");
		printf("\n +Waiting battery to charge+ \n");
		delay();
		printf("\n Wake Up from Sleep Mode State 5 \n");

		//bat=4;
		if(check_bat(bat)){
			listen_flag=1;//Go back to state 4
			sleep_flag5=reset;
		}
		else {Tracker=0;sleep_flag5=reset;}
	}

//--------------------------------------------------
        //Transmit state 6
	while(tx_flag==1){
        int BatS6=1;// State 2 condition for tumbling
        request=0;//requestion from ground station
        //bat=1;
        Tracker++;//6

        printf("\n Enter current battery level for state 6: ");
        scanf("%d", &bat);
        printf("\n Enter condition for request for state 6: ");
        scanf("%d", &request);
        // printf("\n Packet Recived in state 6 \n");


        count3++;
        if (count3==2) request=1;

                if(!check_bat(bat)){
                    printf("\n No GO STATE 6  \n");
                    sleep_flag6=1;
                    BatS6=0;
                    tx_flag=reset;

                    }

                if(BatS6&&request){
                printf("\n Packet trasmitting in state 6 \n");
                delay();
                printf("\n Transmitation is completed in state 6 \n");
                printf("\n >>>>>>>>>> Clear the captatured Image in the SD card <<<<<<<<<< \n");
                Tracker=0;
                tx_flag=reset;
                    }
                else {listen_flag=1;tx_flag=reset;}
        }

    while(sleep_flag6==1){
		printf("\n Battery is Low on state 6, Entering Sleep Mode \n");
		printf("\n +Waiting battery to charge+ \n");
		delay();
		printf("\n Wake Up from Sleep Mode State 6 \n");

        printf("\n Enter current battery level for state 6 [wake up]: ");
        scanf("%d", &bat);

		//bat=4;
		if(check_bat(bat)){
			tx_flag=1;//Go back to state 4
			sleep_flag6=reset;
		}
		else {Tracker=0;sleep_flag6=reset;}
    }

printf("\n ---------------------------End_of_Line---------------------------  \n");
delay();


//---------------------------End Of Line ----------------------------------
	}//While(1) Function

}//Main Function







int check_bat(int bat){

		if (bat<3){
        printf("\n -----------Low Battery Detected----------- \n");
        return 0;
	}
}


void delay(){
int c,d;
 for (c=1; c<50000;c++)
    for(d=1;d<=50000;d++)
 {}
 }
