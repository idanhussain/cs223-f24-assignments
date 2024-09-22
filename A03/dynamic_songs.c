/*----------------------------------------------
 * Author: Idan Hussain 
 * Date: September 20th 2024
 * This program reads songs from a text file and prints them as a list.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char title[128];
	char artist[128];
	int duration_ms;
	float danceability;
	float energy;
	float tempo;
	float valence;
} Song;

void printSongs(Song *songs, int numSongs) {
	printf("\nWelcome to Incredible Idan's Instant Hits.\n");
	for (int i = 0; i < numSongs; i++) {
		int minutes = songs[i].duration_ms / 60000;
		int seconds = (songs[i].duration_ms % 60000) / 1000;
		printf("%d) %-30s artist: %-25s duration: %d:%02d D: %.3f E: %.3f T: %.3f V: %.3f\n", 
			i, songs[i].title, songs[i].artist, minutes, seconds, 
			songs[i].danceability, songs[i].energy, songs[i].tempo, songs[i].valence);
	}
}

int main() {

	FILE *file = fopen("songlist.csv", "r");
	if (file == NULL) {
		printf("Error: unable to open file %s\n", "songlist.csv");
		exit(1);
	} 

	char buffer[256];
	fgets(buffer, sizeof(buffer), file); // First line
	int numSongs = atoi(strtok(buffer, ","));

	fgets(buffer, sizeof(buffer), file); // Header line

	Song *songs = malloc(numSongs * sizeof(Song));

	int i = 0;
	while (fgets(buffer, sizeof(buffer), file) && i < numSongs) {
		char *token = strtok(buffer, ",");
		strcpy(songs[i].title, token);
		token = strtok(NULL, ",");
		strcpy(songs[i].artist, token);
		token = strtok(NULL, ",");
		songs[i].duration_ms = atoi(token);
		token = strtok(NULL, ",");
		songs[i].danceability = atof(token);
		token = strtok(NULL, ",");
		songs[i].energy = atof(token);
		token = strtok(NULL, ",");
		songs[i].tempo = atof(token);
		token = strtok(NULL, ",");
		songs[i].valence = atof(token);
		i++;
	}

	printSongs(songs, numSongs);

	fclose(file);
	free(songs);
	
	return 0;
}
