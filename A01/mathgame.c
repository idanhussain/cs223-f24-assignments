/***************************************************
 * mathgame.c
 * Author: Idan Hussain 
 * Implements a math game
 */

#include <stdio.h>
#include <stdlib.h>

int main() {
	printf("Welcome to Math Game!\nHow many rounds do you want to play? ");
	
	int rounds = 0;
	scanf("%d", &rounds);
	int score = 0;

	for (int i = 0; i < rounds; i++) {
		int operand1 = (rand() % 9) + 1;
		int operand2 = (rand() % 9) + 1;

		printf("%d + %d = ", operand1, operand2);
		
		int response = 0;
		scanf("%d", &response);

		int correctAnswer = operand1 + operand2;
		
		if (response == correctAnswer) {
			printf("Correct!\n");
			score++;
		} else printf("Incorrect :(\n");
	}

	printf("You answered %d/%d correctly.", score, rounds);
  return 0;
}
