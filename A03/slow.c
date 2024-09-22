/*----------------------------------------------
 * Author: Idan Hussain
 * Date: 18th September 2024
 * This program takes in a phrase and converts it to Ent speech.
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
	char buffer[32];
	int pauseLength;
	int phraseLength;	
	char* slowPhrase;
	int index = 0;

	printf("Pause length: ");
	scanf("%d", &pauseLength);
	printf("Text: ");
	scanf(" %[^\n]", buffer);

	phraseLength = strlen(buffer);

	slowPhrase = malloc((phraseLength * (pauseLength + 1) + 1) * sizeof(char)); // +1 to account for the character along with the pause length, +1 for the null character
	
	for (int i = 0; i < phraseLength; i++) {
		slowPhrase[index] = buffer[i];
		index++;
		for (int j = 0; j < pauseLength; j++) {
			slowPhrase[index] = '.';
			index++;
		}
	}

	slowPhrase[index] = '\0';

	printf("%s\n", slowPhrase);
	free(slowPhrase);

  return 0;
}
