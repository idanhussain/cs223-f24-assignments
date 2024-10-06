/*----------------------------------------------
 * Author: Idan Hussain
 * Date: October 4th, 2024
 * Tests read_ppm.c
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main() {
	int w, h;
	struct ppm_pixel** pixels = read_ppm_2d("feep-raw.ppm", &w, &h);

	if (pixels) {
		printf("Width: %d, Height: %d\n", w, h);
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				printf("(%d, %d, %d) ", pixels[i][j].red, pixels[i][j].green, pixels[i][j].blue);
			}
			printf("\n");
		}

		// Free
		for (int i = 0; i < h; i++) {
			free(pixels[i]);
		}
		free(pixels);
	}

	return 0;
}

