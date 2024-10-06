/*----------------------------------------------
 * Author: Idan Hussain 
 * Date: October 4th, 2024
 * This program generates a crossword from two words given by the user
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init2D(char **arr, int rows, int cols) {
	int i, j;

	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {
		arr[i][j] = '.';
		}
	}
}

int commonLetter(char *word1, char *word2, int *index1, int *index2) {
	for (int i = 0; word1[i] != '\0'; i++) {
		for (int j = 0; word2[j] != '\0'; j++) {
			if (word1[i] == word2[j]) {
				*index1 = i;
				*index2 = j;
				return 1;
			}
		}
	}
	return 0;
}

void printCrossword(char **crossword, int rows, int cols) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			printf("%c ", crossword[i][j]);
		}
		printf("\n");
	}
}

int main(int argc, char* argv[]) {

	if (argc < 3) {
		printf("Usage: %s <word1> <word2>\n", argv[0]);
		exit(0);
	}

	char *word1 = argv[1]; // Horizontal word (cols)
	char *word2 = argv[2]; // Vertical word (rows)
	int index1, index2;

	if (!commonLetter(word1, word2, &index1, &index2)) {
		printf("No common letter!\n");
		return 0;
	}

	int len1 = strlen(word1);
	int len2 = strlen(word2);

	// Allocating memory
	int rows = len2;
	int cols = len1;

	char **crossword = malloc(sizeof(char *) * rows);
	for (int i = 0; i < rows; i++) {
		crossword[i] = malloc(sizeof(char) * cols);
	}

	init2D(crossword, rows, cols);

	// Filling the 2D array
	for (int i = 0; i < len1; i++) {
		crossword[index2][i] = word1[i]; // word1 is horizontal
	}
	for (int i = 0; i < len2; i++) {
		crossword[i][index1] = word2[i]; // word2 is vertical
	}

	printCrossword(crossword, rows, cols);

	// Free
	for (int i = 0; i < rows; i++) {
		free(crossword[i]);
	}
	free(crossword);

	return 0;
}
