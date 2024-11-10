#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
	FILE* fp = fopen(filename, "rb");
	if (!fp) {
		printf("Error: Unable to open file %s\n", filename);
		return NULL;
	}

	char buffer[256];

	// Read magic number
	if (fgets(buffer, sizeof(buffer), fp) == NULL) {
		printf("Error reading magic number\n");
		fclose(fp);
		return NULL;
	}

	// Read comment
	if (fgets(buffer, sizeof(buffer), fp) == NULL) {
		printf("Error reading comment\n");
		fclose(fp);
		return NULL;
	}

	// Read width, height, and max color
	if (fgets(buffer, sizeof(buffer), fp) == NULL) {
		printf("Error reading height/width\n");
		fclose(fp);
		return NULL;
	}

	// Parse width and height
	sscanf(buffer, "%d %d", w, h);

	// Read max color
	if (fgets(buffer, sizeof(buffer), fp) == NULL) {
		printf("Error reading max color\n");
		fclose(fp);
		return NULL;
	}

	int max_color;
	sscanf(buffer, "%d", &max_color);

	// Memory Allocation
	struct ppm_pixel* pixels = malloc((*w) * (*h) * sizeof(struct ppm_pixel));

	// Read pixel data
	fread(pixels, sizeof(struct ppm_pixel), (*w) * (*h), fp);

	fclose(fp);
	return pixels;
}

struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {
	return NULL;
}
