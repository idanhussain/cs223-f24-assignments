/*----------------------------------------------
 * Author: Idan Hussain
 * Date: October 11 2024
 * This program reads in a PPM file and then outputs any message that might be stored in the least significant bits of each color.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("usage: decode <file.ppm>\n");
		return 0;
	}

	int width, height;
	struct ppm_pixel* pixels = read_ppm(argv[1], &width, &height);
	if (!pixels) {
		printf("Error: Unable to read the PPM file\n");
		return 1;
	}

	// Image dimensions and max message size
	int max_chars = (width * height * 3) / 8;
	printf("Reading %s with width %d and height %d\n", argv[1], width, height);
	printf("Max number of characters in the image: %d\n", max_chars);

	unsigned char current_char = 0;
	int bit_count = 0;

	for (int i = 0; i < width * height * 3; i++) {
		unsigned char bit = ((unsigned char*)pixels)[i] & 1;

		current_char |= (bit << (7 - bit_count));
		bit_count++;

		if (bit_count == 8) {
			if (current_char == '\0') {
				break;
			}
			printf("%c", current_char);
			// Reset values
			current_char = 0;
			bit_count = 0;
		}
	}

	printf("\n");

	// Free
	free(pixels);

	return 0;
}
