/*----------------------------------------------
 * Author: Idan Hussain 
 * Date: October 11 2024
 * This program reads in a single 64-bit unsigned integer and outputs it as an 8x8 1bpp sprite
 ---------------------------------------------*/
#include <stdio.h>

int main() {
	unsigned long img;
	scanf(" %lx", &img);
	printf("Image (unsigned long): %lx\n", img);

	for (int i = 0; i < 64; i++) {
		// Mask
		unsigned long mask = 0x1ul << (63 - i);

		if (img & mask) {
			printf(" @ ");
		} else {
			printf("   ");
		}

		if ((i + 1) % 8 == 0) {
			printf("\n");
		}
	}

	return 0;
}
