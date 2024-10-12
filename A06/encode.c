/*----------------------------------------------
 * Author: Idan Hussain 
 * Date: October 11 2024
 * This program reads in a PPM file and asks the user for a message to embed within it
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("usage: encode <file.ppm>\n");
		return 0;
	}

	int width, height;
	struct ppm_pixel* pixels = read_ppm(argv[1], &width, &height);
	if (!pixels) {
		printf("Error: Unable to read the PPM file\n");
		return 1;
	}

	// Maximum characters
	int max_chars = (width * height * 3) / 8;
	printf("Reading %s with width %d and height %d\n", argv[1], width, height);
	printf("Max number of characters in the image: %d\n", max_chars);

	// User encoded meessage
	char message[256];
	printf("Enter a phrase: ");
	fgets(message, sizeof(message), stdin);

	// Check if message fits in image
	if (strlen(message) > max_chars) {
		printf("Error: The message must be %d characters long.\n", max_chars);
		free(pixels);
		return 1;
	}

	int char_index = 0;
	int bit_index = 0;
	for (int i = 0; i < width * height; i++) {
		if (char_index > strlen(message)) break;

		pixels[i].red &= 0xFE;
		pixels[i].green &= 0xFE;
		pixels[i].blue &= 0xFE;

		// Red
		pixels[i].red |= (message[char_index] >> (7 - bit_index)) & 1;
		if (++bit_index == 8) {
			bit_index = 0;
			char_index++;
		}
		if (char_index >= strlen(message)) break;

		// Green		
		pixels[i].green |= (message[char_index] >> (7 - bit_index)) & 1;
		if (++bit_index == 8) {
			bit_index = 0;
			char_index++;
		}
		if (char_index >= strlen(message)) break;

		// Blue
		pixels[i].blue |= (message[char_index] >> (7 - bit_index)) & 1;
		if (++bit_index == 8) {
			bit_index = 0;
			char_index++;
		}
	}

	char new_filename[256];
	char *dot = strrchr(argv[1], '.');
	if (dot) {
		snprintf(new_filename, sizeof(new_filename), "%.*s-encoded.ppm", (int)(dot - argv[1]), argv[1]);
	} else {
		// If there is no dot, append -encoded to the original name
		snprintf(new_filename, sizeof(new_filename), "%s-encoded.ppm", argv[1]);
	}

	// Write the new file
	printf("Writing file %s\n", new_filename);
	write_ppm(new_filename, pixels, width, height);

	// Free
	free(pixels);

	return 0;
}

