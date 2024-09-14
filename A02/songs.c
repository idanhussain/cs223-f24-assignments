/***************************************************
 * song.c
 * Author: Idan Hussain 
 * Implements a program that fills an array with three or more songs and then allows the user to edit the data in the array
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
        char title[32];
        char artist[32];
        int minutes;
        int seconds;
        float danceability;
} Song;

void displaySongs(Song songs[]) {
	printf("\n=======================\n");
	
	for (int i = 0; i < 4; i++) {
		printf("%d) %-20s artist: %-20s duration: %d:%02d  danceability: %.2f\n",
				i, songs[i].title, songs[i].artist, songs[i].minutes, songs[i].seconds, songs[i].danceability);
	}
	
	printf("=======================\n\n");
}

int getSongId() {
	int id;
	do {
		printf("Enter a song id to edit [0, 1, 2, 3]: ");
		scanf("%d", &id);
		if (id < 0 || id > 3) {
			printf("\n%d is an invalid id. ", id);
		}
	} while (id < 0 || id > 3);

	return id;
}

void editSong(Song *song) {
	char attribute[32];
	printf("\nWhich attribute of %s would you like to edit? [artist, title, duration, danceability]: ", song->title);
	scanf(" %[^\n]%*c", attribute);

	if (strcmp(attribute, "artist") == 0) {
		char newArtist[32];
		printf("Enter a new artist: ");
		scanf(" %[^\n]%*c", newArtist);
		strcpy(song->artist, newArtist);
	} else if (strcmp(attribute, "title") == 0) {
		char newTitle[32];
		printf("Enter a new title: ");
		scanf(" %[^\n]%*c", newTitle);
		strcpy(song->title, newTitle);
	} else if (strcmp(attribute, "duration") == 0) {
		int newMinutes, newSeconds;
		printf("Enter new duration (minutes): ");
		scanf(" %d%*c", &newMinutes);
		printf("Enter new duration (seconds): ");
		scanf(" %d%*c", &newSeconds);
		song->minutes = newMinutes;
		song->seconds = newSeconds;  
	} else if (strcmp(attribute, "danceability") == 0) {
		float newDanceability;
		printf("Enter new danceability: ");
		scanf(" %f%*c", &newDanceability);
		song->danceability = newDanceability;
	} else {
		printf("\nInvalid attribute.\n\n");
		exit(0);
	}
}

int main() {
	Song songs[4] = {
	{"Pixeleen", "Steely Dan", 4, 1, 0.60},
	{"New Light", "John Mayer", 3, 37, 0.70},
	{"Blue", "Joni Mitchell", 3, 6, 0.20},
	{"White Flag", "Dido", 4, 1, 0.50}
	};

	printf("\nWelcome to Idan's Song List.\n");

	displaySongs(songs);

	int songId = getSongId();
	Song* selectedSong = &songs[songId];

	editSong(selectedSong);
	displaySongs(songs);

	return 0;
}
