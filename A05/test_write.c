/*----------------------------------------------
 * Author: Idan Hussain 
 * Date: October 4th, 2024
 * Tests write_ppm.c
 ---------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {
	int w, h;
	struct ppm_pixel** pixels = read_ppm_2d("feep-raw.ppm", &w, &h);

	write_ppm_2d("test.ppm", pixels, w, h);

	struct ppm_pixel** new_pixels = read_ppm_2d("test.ppm", &w, &h);
	if (new_pixels) {
		printf("Testing file feep-raw.ppm: %d %d\n", w, h);
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				printf("(%d, %d, %d) ", new_pixels[i][j].red, new_pixels[i][j].green, new_pixels[i][j].blue);
			}
			printf("\n");
		}
		// Free new pixel
		for (int i = 0; i < h; i++) {
			free(new_pixels[i]);
		}
		free(new_pixels);
	} else {
		printf("Error reading test.ppm.\n");
	}
	
	// Free original array
	for (int i = 0; i < h; i++) {
		free(pixels[i]);
	}
	free(pixels);
	
	return 0;
}
