/* Convolution Testing With Premade Data
 * Author: Alex Colpitts
 * Date: November 15, 2017
 */

#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "inttypes.h"

// use Barker Code length 5 where 1 = 1 and -1 = 0 00000
#define PN 0b11101
#define PN_LENGTH 5

//#define PN 0b110
//#define PN_LENGTH 3

// convolve a PN sequences against data, returns 
int64_t data_conv(uint8_t data){
	uint8_t i;
	int64_t tx_data = 0;
	int shift;
			
	for(i = 0; i < sizeof(data)*8; i++){ // number of bytes * bits/byte = number of bits in data
				
		if((data & (0x80 >> i)) >= 1){ // if the data bit is greater or equal to 1...  
			tx_data = tx_data | PN;
		}	
		
		tx_data = tx_data << PN_LENGTH; // shift to make room for next part encoded bit
	}
	
	// shift all the way to end to allow error correcting bits to be added at the end
	shift = sizeof(tx_data)*8 - PN_LENGTH*sizeof(data)*8;
	printf("Shift: %d\n", shift);
	
	return tx_data;
};

int main(){
	uint8_t data = 0b11110000; // assume camera takes 8 bit pixels x3 for RGB
	int64_t temp;
		
	temp = data_conv(data);

	return 0;
}
