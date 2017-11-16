/* Convolution Testing With Premade Data
 * Author: Alex Colpitts
 * Date: November 15, 2017
 */

#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "inttypes.h"
#include "time.h"

///*
// use Binary Barker Code length 5
#define PN 0b11101
#define PN_LENGTH 5
//*/

/*
// use Binary Barker Code length 11
#define PN 0b11100010010
#define PN_LENGTH 11
*/

#define TX_LENGTH_MASK 0x8000000000000000 // mask to check if data has been shifted all the way over

// image constants
#define IMAGE_WIDTH 640
#define IMAGE_HEIGHT 480
#define IMAGE_BYTE_PER_PIXEL 3 // RGB image 
#define IMAGE_BYTES (IMAGE_WIDTH * IMAGE_HEIGHT * IMAGE_BYTE_PER_PIXEL) //92160

// integer masks
#define UINT8_MASK_7 0x80
#define UINT8_MASK_6 0x40
#define UINT8_MASK_5 0x20
#define UINT8_MASK_4 0x10
#define UINT8_MASK_3 0x08
#define UINT8_MASK_2 0x04
#define UINT8_MASK_1 0x02
#define UINT8_MASK_0 0x01

typedef struct TX_STRUCT{
	uint8_t block[92160];
} TX;

// creates 640x480 RGB image simulation 
uint8_t *image_creation(){ //return pointer with pseudo image associated
	int i;
		 
	uint8_t *p = malloc(sizeof(uint8_t)*IMAGE_BYTES);
	
	for(i = 0; i < IMAGE_BYTES; i++){
		p[i] = 0xAA;
	}
	
	return p;
};

// convolve a PN sequences against data, returns expanded data  
uint64_t data_conv(uint8_t data){
	uint8_t i;
	uint64_t tx_data = 0;
			
	for(i = 0; i < sizeof(data)*8; i++){ // number of bytes * bits/byte = number of bits in data
				
		if((data & (0x80 >> i)) >= 1){ // if the data bit is greater or equal to 1...  
			tx_data = tx_data | PN;
		}	
		
		tx_data = tx_data << PN_LENGTH; // shift to make room for next part encoded bit
	}
	
	// shift all the way to end to allow error correcting bits to be added at the end
	
	while(!(tx_data & TX_LENGTH_MASK)){ // shift until 1 is the MSB
		tx_data = tx_data << 1;
	}
	
	return tx_data; // return the data block
};

// convolve the image with the PN sequence
uint64_t *image_preprocess(uint8_t *p){
	uint64_t *q = malloc(sizeof(uint64_t)*IMAGE_BYTES);
	int i;
	
	for(i = 0; i < IMAGE_BYTES; i++){
		q[i] = data_conv(p[i]);		
	}
	
	return q;
}

int main(){
	uint8_t *p;
	uint64_t *q;
	
	// for convolve timing	
	clock_t start, end;
	double cpu_time_used;
			
	// image creation testing
	p = image_creation(); // receive image data
	start = clock();
	q = image_preprocess(p); // convolve image with PN sequence
	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("Time to convolve: %lf", cpu_time_used); // taking roughly 55 ms to complete the convolution
	
	free(p);
	free(q);
	return 0;
};
