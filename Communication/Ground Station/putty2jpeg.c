#include "stdio.h"
#include "stdlib.h"

void putty2jpeg(char *, char *);

// take the log name and convert to image
void putty2jpeg(char *filename, char *jpeg_name){
	FILE *log, *image;
	long filelen;
	long i;
	char *buffer;
	
	log = fopen(filename, "r");
	image = fopen(jpeg_name, "w");

	// read log into buffer
	fseek(log, 0, SEEK_END);          
	filelen = ftell(log);            
	rewind(log);    
	buffer = (char *)malloc((filelen+1)*sizeof(char)); // Enough memory for file + \0
	fread(buffer, filelen, 1, log);
	fclose(log);

    // header has 80 bytes of junk to remove
	for(i = 79; i < filelen; i++){		
		fprintf(image, "%c", buffer[i]);
	}
	
	fclose(image);

	return;
}
