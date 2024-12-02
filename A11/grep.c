/*----------------------------------------------
 * Author: Idan Hussain
 * Date: November 30 2024
 * This program uses N threads to search for a keyword in a set of files
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

// Mutex initialization
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct
{
  int thread_id;
  int num_files;
  char **files;
  char *keyword;
  int lines_found;
} thread_data_t;

void *search_files(void *arg)
{
  thread_data_t *data = (thread_data_t *)arg;
  data->lines_found = 0;

  for (int i = 0; i < data->num_files; i++)
  {
    FILE *file = fopen(data->files[i], "r");
    if (file == NULL)
    {
      perror("Error opening file");
      continue;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file))
    {
      if (strstr(line, data->keyword))
      {
        pthread_mutex_lock(&mutex);
        printf("%d) %s:%s", data->thread_id, data->files[i], line);
        pthread_mutex_unlock(&mutex);
        data->lines_found++;
      }
    }
    fclose(file);
  }

  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  if (argc < 4)
  {
    printf("Usage: %s <num_threads> <keyword> $(find <directory> -name \"file extension\")\n", argv[0]);
    printf("Example: %s 3 public $(find code -name \"*.h\")\n", argv[0]);
    return EXIT_FAILURE;
  }

  int num_threads = atoi(argv[1]);
  char *keyword = argv[2];
  int num_files = argc - 3;

  if (num_threads > num_files)
  {
    num_threads = num_files;
  }

  // Allocating memory
  pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
  thread_data_t *thread_data = malloc(num_threads * sizeof(thread_data_t));

  if (threads == NULL || thread_data == NULL)
  {
    fprintf(stderr, "Memory allocation failed.\n");
    return EXIT_FAILURE;
  }

  // Files per thread
  int files_per_thread = num_files / num_threads;
  int remaning_files = num_files % num_threads;

  // Begin time elapsed
  clock_t start_time = clock();

  printf("Searching %d files for keyword: %s\n", num_files, keyword);

  // Create threads and assign files
  int file_index = 0;
  for (int i = 0; i < num_threads; i++)
  {
    thread_data[i].thread_id = i;
    thread_data[i].keyword = keyword;

    int files_to_assign = files_per_thread + (i < remaning_files ? 1 : 0);
    thread_data[i].num_files = files_to_assign;
    thread_data[i].files = &argv[file_index + 3];

    printf("Thread %d searching %d files (%d to %d)\n", i, files_to_assign, file_index + 1, file_index + files_to_assign);

    file_index += files_to_assign;
    pthread_create(&threads[i], NULL, search_files, (void *)&thread_data[i]);
  }

  // Join threads
  int total_lines_found = 0;
  for (int i = 0; i < num_threads; i++)
  {
    pthread_join(threads[i], NULL);
    total_lines_found += thread_data[i].lines_found;
    printf("Thread %d found %d lines containing keyword: %s\n", i, thread_data[i].lines_found, keyword);
  }

  // End time elapsed
  clock_t end_time = clock();
  double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

  printf("\nElapsed time: %.6f seconds\n", elapsed_time);

  // Free
  free(threads);
  free(thread_data
  );

  return EXIT_SUCCESS;
}