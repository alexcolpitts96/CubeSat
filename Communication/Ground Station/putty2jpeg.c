#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

#define IMAGE_SIZE 8590

int main(){
	FILE *log, *image;
	long filelen;
	long i;
	char *buffer;
	
	log = fopen("2018-03-04-120029.log", "r");
	image = fopen("test.jpeg", "w");

	// read log into buffer
	fseek(log, 0, SEEK_END);          
	filelen = ftell(log);            
	rewind(log);    
	buffer = (char *)malloc((filelen+1)*sizeof(char)); // Enough memory for file + \0
	fread(buffer, filelen, 1, log);
	fclose(log);
	
	printf("Image Size: %ld\n", filelen);
	printf("Removing %ld char", filelen - IMAGE_SIZE);
	
	for(i = filelen-IMAGE_SIZE; i < filelen; i++){
		fprintf(image, "%c", buffer[i]);
	}
	
	fclose(image);
	return 0;	
}
