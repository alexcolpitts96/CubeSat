#include "stdio.h"
#include "image.h"
#include "stdlib.h"
#include "stdint.h"

#define IMAGE_SIZE 8590

int main(){
	FILE *fp, *jp;
	long filelen;
	long i;
	char *buffer;
	
	fp = fopen("image_output.txt", "w");
	jp = fopen("test.jpeg", "r");

	// read in jpeg byte by byte
	fseek(jp, 0, SEEK_END);          
	filelen = ftell(jp);            
	rewind(jp);    
	buffer = (char *)malloc((filelen+1)*sizeof(char)); // Enough memory for file + \0
	fread(buffer, filelen, 1, jp);
	fclose(jp);
	
	// print array to text file
	for(i = 0; i < filelen; i++){
		fprintf(fp, "%c", buffer[i]);
	}
	
	fclose(fp);
	
	return 0;	
}
