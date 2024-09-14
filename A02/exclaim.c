/***************************************************
 * exclaim.c
 * Author: Idan Hussain 
 * Implements a program that turns a word into an exclamation
 */


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
	char exclamations[4] = {'@', '!', '#', '*'};
	char buff[32];
	char finalWord[32];
	
	srand(time(NULL));

        printf("Enter a word: ");	
	scanf("%31s", buff); 

	int i;
	for (i = 0; buff[i] != '\0'; i++) {
		int ascii = buff[i];

		if (ascii >= 97 && ascii <= 122) {
			int randomIndex = (rand() % 4);
		       	char exclamation = exclamations[randomIndex];	
			finalWord[i] = exclamation;
		} else {
			finalWord[i] = buff[i];
		}
	}      

	finalWord[i] = '\0';

	printf("OMG! %s\n", finalWord);
	
	return 0;

}
