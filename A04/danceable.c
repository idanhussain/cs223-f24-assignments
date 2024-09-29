/*----------------------------------------------
 * Author: Idan Hussain 
 * Date: September 27, 2024
 * This program allows users to easily access the most danceable songs in songlist.csv
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

struct node {
	Song song;
	struct node* next;
};

void printSongs(struct node *head) {
	struct node *current = head;
	int index = 0;
	while (current != NULL) {
		int minutes = current->song.duration_ms / 60000;
		int seconds = (current->song.duration_ms % 60000) / 1000;
		printf("%d) %-30s artist: %-25s duration: %d:%02d D: %.3f E: %.3f T: %.3f V: %.3f\n", 
			index, current->song.title, current->song.artist, minutes, seconds, 
			current->song.danceability, current->song.energy, current->song.tempo, current->song.valence);
		current = current->next;
		index++;
	}
}

void insertFront(Song song, struct node** head) {
	struct node* n = malloc(sizeof(struct node));
	if (n == NULL) {
		printf("ERROR: Out of space!\n");
		exit(1);
	}
	n->song = song;
	n->next = *head;
	*head = n;	
}

void freeList(struct node* head) {
	struct node* temp;
	while (head != NULL) {
		temp = head;
		head = head->next;
		free(temp);
	}
}

Song* danceable(struct node** head) {
	if (*head == NULL) return NULL;

	struct node* current = *head;
	struct node* mostDanceable = *head;
	struct node* prev = NULL;
	struct node* prevMostDanceable = NULL;

	while (current != NULL) {
		if (current->song.danceability >= mostDanceable->song.danceability) {
			mostDanceable = current;
			prevMostDanceable = prev;
		}
		prev = current;
		current = current->next;
	}

	// Removing
	if (mostDanceable == *head) {
		*head = mostDanceable->next;
	} else {
		prevMostDanceable->next = mostDanceable->next;
	}
	
	return &mostDanceable->song;
}

int countSongs(struct node* head) {
	int count = 0;
	struct node* current = head;

	while (current != NULL) {
		count++;
		current = current->next;
	}

	return count;
}


int main() {
	struct node* head = NULL;

	FILE *file = fopen("songlist.csv", "r");
	if (file == NULL) {
		printf("Error: unable to open file %s\n", "songlist.csv");
		exit(1);
	}

	char buffer[256];
	fgets(buffer, sizeof(buffer), file); // Header line

	while (fgets(buffer, sizeof(buffer), file)) {
		Song song;
		char *token = strtok(buffer, ",");
		strcpy(song.title, token);

		token = strtok(NULL, ",");
		strcpy(song.artist, token);

		token = strtok(NULL, ",");
		song.duration_ms = atoi(token);

		token = strtok(NULL, ",");
		song.danceability = atof(token);

		token = strtok(NULL, ",");
		song.energy = atof(token);

		token = strtok(NULL, ",");
		song.tempo = atof(token);

		token = strtok(NULL, ",");
		song.valence = atof(token);

		insertFront(song, &head);
	}

	fclose(file);

	printSongs(head);
	printf("\nDataset contains %d songs\n", countSongs(head));    

	char input;
	while (countSongs(head) > 0) {
		printf("\n=======================\n");
		printf("Press 'd' to show the most danceable song (any other key to quit): ");
		scanf(" %c", &input);

		if (input == 'd') {
			Song* mostDanceableSong = danceable(&head);

			if (mostDanceableSong != NULL) {
				printf("\n---------------------------------------- Most danceable ------------------------------------\n");
				int minutes = mostDanceableSong->duration_ms / 60000;
				int seconds = (mostDanceableSong->duration_ms % 60000) / 1000;
				printf("%-30s artist: %-25s duration: %d:%02d D: %.3f E: %.3f T: %.3f V: %.3f\n",
				mostDanceableSong->title, mostDanceableSong->artist, minutes, seconds,
				mostDanceableSong->danceability, mostDanceableSong->energy, mostDanceableSong->tempo, mostDanceableSong->valence);
				printf("--------------------------------------------------------------------------------------------\n");

				printSongs(head);
				printf("\nDataset contains %d songs\n", countSongs(head));
				free(mostDanceableSong);
			}
		} 
		 else {
			break;
		}
	}


	freeList(head);
	free(head);

	return 0;
}
