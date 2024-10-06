/*----------------------------------------------
 * Author: Idan Hussian
 * Date: October 4th, 2024
 * This program writes a PPM file
 ---------------------------------------------*/
#include "write_ppm.h"
#include <stdio.h>
#include <string.h>

// implement *one* (but do not remove the other one)!

void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {

}

void write_ppm_2d(const char* filename, struct ppm_pixel** pixels, int w, int h) {
	FILE* fp = fopen(filename, "wb");
	if (!fp) {
		printf("Error: Unable to open file %s\n", filename);
		return;
	}

	// Header
	fprintf(fp, "P6\n");
	fprintf(fp, "# Created by Idan version 1.0\n");
	fprintf(fp, "%d %d\n", w, h);
	fprintf(fp, "255\n");

	// Pixel data
	for (int i = 0; i < h; i++) {
		fwrite(pixels[i], sizeof(struct ppm_pixel), w, fp);
	}

	fclose(fp);
}
