/*----------------------------------------------
 * Author: Idan Hussain
 * Date: October 11 2024
 * This program writes a ppm file
 ---------------------------------------------*/
#include "write_ppm.h"
#include <stdio.h>
#include <string.h>

// implement *one* (but do not remove the other one)!

void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {
	FILE* fp = fopen(filename, "wb");
	if (!fp) {
		printf("Error: Unable to open file %s\n", filename);
		return;
	}

	// Header
	fprintf(fp, "P6\n");
	fprintf(fp, "# Created by Idan version 2.0\n");
	fprintf(fp, "%d %d\n", w, h);
	fprintf(fp, "255\n");

	//  Pixel data
	fwrite(pixels, sizeof(struct ppm_pixel), w * h, fp);

	fclose(fp);
}

void write_ppm_2d(const char* filename, struct ppm_pixel** pixels, int w, int h) {

}
