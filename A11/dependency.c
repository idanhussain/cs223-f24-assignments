/*----------------------------------------------
 * Author: Idan Hussain
 * Date: November 30 2024
 * This program uses N threads to build a binary search tree of a given set of files
 ---------------------------------------------*/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "tree.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Declaring root
extern struct tree_node *root;

// Function to parse #include statements
void parse_file_dependencies(const char *filename)
{
  FILE *file = fopen(filename, "r");
  if (!file)
  {
    perror("Error opening file");
    return;
  }

  char line[1024];
  char *dependencies[1024];
  int dep_count = 0;

  int has_dependencies = 0;

  while (fgets(line, sizeof(line), file))
  {
    if (strstr(line, "#include"))
    {
      char dep_filename[1024];

      if (sscanf(line, "#include \"%[^\"]\"", dep_filename) == 1 || // Handle " "
          sscanf(line, "#include <%[^\"]>", dep_filename) == 1)     // Handle < >
      {
        // Remove " or > from  filenames
        dep_filename[strcspn(dep_filename, ">")] = 0;
        dep_filename[strcspn(dep_filename, "\"")] = 0;

        // Check if dependency has already been printed
        int found = 0;
        for (int i = 0; i < dep_count; i++)
        {
          if (strcmp(dependencies[i], dep_filename) == 0)
          {
            found = 1;
            break;
          }
        }
        if (!found)
        {
          dependencies[dep_count++] = strdup(dep_filename); // Store a duplicate of the dependency
          has_dependencies = 1;
        }
      }
    }
  }

  if (has_dependencies)
  {
    pthread_mutex_lock(&mutex);
    printf("%s has the following dependencies:\n", filename);
    for (int i = 0; i < dep_count; i++)
    {
      printf("  %s\n", dependencies[i]);
      free(dependencies[i]); // Free strdup
    }
    pthread_mutex_unlock(&mutex);
  }

  fclose(file);
}

// Function to process files
void *process_files(void *arg)
{
  char **files = (char **)arg;
  for (int i = 0; files[i] != NULL; i++)
  {
    parse_file_dependencies(files[i]);
  }

  free(files);
  return NULL;
}

int main(int argc, char *argv[])
{
  struct tree_node *root = NULL;

  if (argc < 4)
  {
    printf("Usage: %s <num_threads> $(find <directory> -name \"file extension\")\n", argv[0]);
    return EXIT_FAILURE;
  }

  int num_threads = atoi(argv[1]);
  int num_files = argc - 3;

  // Allocating memory
  pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
  if (threads == NULL)
  {
    printf("Memory allocation failed.\n");
    return EXIT_FAILURE;
  }

  // Begin time elapsed
  clock_t start_time = clock();
  printf("Processing %d files\n", num_files);

  int files_per_thread = num_files / num_threads;
  int extra_files = num_files % num_threads;

  // Create threads and assign files
  int file_index = 3;
  for (int i = 0; i < num_threads; i++)
  {
    int files_to_assign = files_per_thread + (i < extra_files ? 1 : 0);
    char **thread_files = malloc((files_to_assign + 1) * sizeof(char *));
    for (int j = 0; j < files_to_assign; j++)
    {
      thread_files[j] = argv[file_index + j];

      pthread_mutex_lock(&mutex);
      printf("Thread %d assigning file: %s\n", i, thread_files[j]);
      pthread_mutex_unlock(&mutex);
    }
    thread_files[files_to_assign] = NULL;

    // Tree insertion
    for (int j = 0; thread_files[j] != NULL; j++)
    {
      pthread_mutex_lock(&mutex);
      root = insert(thread_files[j], root);
      pthread_mutex_unlock(&mutex);
    }

    pthread_create(&threads[i], NULL, process_files, (void *)thread_files);
    file_index += files_to_assign;
  }

  for (int i = 0; i < num_threads; i++)
  {
    pthread_join(threads[i], NULL);
  }

  // End time elapsed
  clock_t end_time = clock();
  double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
  printf("Elapsed time is %.6f\n", elapsed_time);

  // Command prompt
  char command[1024];
  while (1)
  {
    printf("$ ");
    fgets(command, sizeof(command), stdin);
    command[strcspn(command, "\n")] = 0; // Removing newline character

    if (strcmp(command, "quit") == 0)
    {
      break;
    }
    else if (strcmp(command, "list") == 0)
    {
      pthread_mutex_lock(&mutex);
      if (root == NULL)
      {
        printf("The tree is empty.\n");
      }
      else
      {
        printSorted(root);
      }
      pthread_mutex_unlock(&mutex);
    }
    else
    {
      struct tree_node *node = find(command, root);
      if (node)
      {
        parse_file_dependencies(command);
      }
      else
      {
        printf("%s not found\n", command);
      }
    }
  }

  // Free
  free(threads);

  // Clear tree
  clear(root);

  return EXIT_SUCCESS;
}