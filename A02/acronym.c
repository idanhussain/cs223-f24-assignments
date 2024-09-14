/***************************************************
 * acronym.c
 * Author: Idan Hussain 
 * Implements a program that turns a phrase into an acronym
 */


#include <stdio.h>

int main() {
	char phrase[1024];
	char acronym[512];
	int acronymIndex = 0;

	printf("Enter a phrase: ");
	scanf("%[^\n]%*c", phrase);

	if ((int) phrase[0] >= 65 && (int) phrase[0] <= 90 ) {
		acronym[acronymIndex] = phrase[0];
		acronymIndex++;
	}

	for (int i = 0; phrase[i] != '\0'; i++) {

		if ((int) phrase[i] >= 65 && (int) phrase[i] <= 90 && phrase[i - 1] == ' ') {
			acronym[acronymIndex] = phrase[i];
			acronymIndex++;
		}

	}

	acronym[acronymIndex] = '\0';

	printf("Your acronym is %s\n", acronym);

	return 0;

}

