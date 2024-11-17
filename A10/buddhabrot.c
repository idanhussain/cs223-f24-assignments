/*----------------------------------------------
 * Author: Idan Hussain
 * Date: November 14 2024
 * This program outputs a PPM image of the Buddhabrot using multiple threads.
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include "read_ppm.h"
#include "write_ppm.h"

struct buddhabrot_params
{
  int start_row;
  int end_row;
  int start_col;
  int end_col;
  int size;
  float xmin;
  float xmax;
  float ymin;
  float ymax;
  int maxIterations;
  int **counts;
  struct ppm_pixel *pixels;
  pthread_mutex_t *mutex;
  pthread_barrier_t *barrier;
};

// Step 1 function
int in_mandelbrot(float x0, float y0, int maxIterations)
{
  float x = 0, y = 0;
  int iter = 0;
  while (iter < maxIterations && (x * x + y * y) < 4.0)
  {
    float xtmp = x * x - y * y + x0;
    y = 2 * x * y + y0;
    x = xtmp;
    iter++;
  }
  return iter >= maxIterations;
}

// Computing buddhabrot funciton
void *compute_buddhabrot(void *arg)
{
  struct buddhabrot_params *params = (struct buddhabrot_params *)arg;

  int size = params->size;
  int maxIterations = params->maxIterations;
  float xmin = params->xmin, xmax = params->xmax;
  float ymin = params->ymin, ymax = params->ymax;
  int **counts = params->counts;

  printf("Thread %ld) sub-image block: cols (%d, %d) to rows (%d, %d)\n",
         pthread_self(), params->start_col, params->end_col, params->start_row, params->end_row);

  // Step 1 and 2
  for (int row = params->start_row; row < params->end_row; row++)
  {
    for (int col = params->start_col; col < params->end_col; col++)
    {
      float xfrac = (float)row / size;
      float yfrac = (float)col / size;
      float x0 = xmin + xfrac * (xmax - xmin);
      float y0 = ymin + yfrac * (ymax - ymin);

      if (!in_mandelbrot(x0, y0, maxIterations))
      {
        float x = 0, y = 0;
        while ((x * x + y * y) < 4.0)
        {
          float xtmp = x * x - y * y + x0;
          y = 2 * x * y + y0;
          x = xtmp;

          int yrow = (int)round(size * (y - ymin) / (ymax - ymin));
          int xcol = (int)round(size * (x - xmin) / (xmax - xmin));

          if (yrow >= 0 && yrow < size && xcol >= 0 && xcol < size)
          {
            pthread_mutex_lock(params->mutex);
            counts[yrow][xcol]++;
            pthread_mutex_unlock(params->mutex);
          }
        }
      }
    }
  }

  // Barrier wait before color values are computed
  pthread_barrier_wait(params->barrier);

  // Step 3
  // Finding max count to scale values for color mapping
  int max_count = 0;
  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++)
    {
      if (counts[i][j] > max_count)
      {
        max_count = counts[i][j];
      }
    }
  }

  float gamma = 0.681;
  float factor = 1.0 / gamma;
  for (int row = params->start_row; row < params->end_row; row++)
  {
    for (int col = params->start_col; col < params->end_col; col++)
    {
      float value = 0;

      if (counts[row][col] > 0)
      {
        value = log(counts[row][col]) / log(max_count);
        value = pow(value, factor);
      }
      params->pixels[row * size + col].red = (unsigned char)(value * 255);
      params->pixels[row * size + col].green = (unsigned char)(value * 255);
      params->pixels[row * size + col].blue = (unsigned char)(value * 255);
    }
  }

  printf("Thread %ld) finished\n", pthread_self());
  return NULL;
}

int main(int argc, char *argv[])
{
  int size = 480;
  float xmin = -2.0, xmax = 0.47, ymin = -1.12, ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1)
  {
    switch (opt)
    {
    case 's':
      size = atoi(optarg);
      break;
    case 'l':
      xmin = atof(optarg);
      break;
    case 'r':
      xmax = atof(optarg);
      break;
    case 't':
      ymax = atof(optarg);
      break;
    case 'b':
      ymin = atof(optarg);
      break;
    case 'p':
      numProcesses = atoi(optarg);
      break;
    case '?':
      printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax> -p <numProcesses>\n", argv[0]);
      return 1;
    }
  }

  printf("Generating buddhabrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f, %.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f, %.4f]\n", ymin, ymax);

  struct ppm_pixel *pixels = malloc(size * size * sizeof(struct ppm_pixel));
  if (!pixels)
  {
    printf("Error: Memory allocation failed\n");
    return 1;
  }

  int **counts = malloc(size * sizeof(int *));
  for (int i = 0; i < size; i++)
  {
    counts[i] = malloc(size * sizeof(int));
    if (!counts[i])
    {
      fprintf(stderr, "Memory allocation failed for counts\n");
      return 1;
    }
    memset(counts[i], 0, size * sizeof(int)); // Initializing allocated memory to 0 everytime
  }

  pthread_t *threads = malloc(numProcesses * sizeof(pthread_t));
  struct buddhabrot_params *params = malloc(numProcesses * sizeof(struct buddhabrot_params));
  if (!threads || !params)
  {
    fprintf(stderr, "Memory allocation failed\n");
    return 1;
  }

  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_barrier_t barrier;
  pthread_barrier_init(&barrier, NULL, numProcesses);

  struct timeval tstart, tend;
  gettimeofday(&tstart, NULL);

  int rows_per_thread = size / 2;
  int cols_per_thread = size / 2;

  for (int i = 0; i < numProcesses; i++)
  {
    params[i].start_row = (i / 2) * rows_per_thread;
    params[i].end_row = ((i / 2) + 1) * rows_per_thread;
    params[i].start_col = (i % 2) * cols_per_thread;
    params[i].end_col = ((i % 2) + 1) * cols_per_thread;
    params[i].size = size;
    params[i].xmin = xmin;
    params[i].xmax = xmax;
    params[i].ymin = ymin;
    params[i].ymax = ymax;
    params[i].maxIterations = maxIterations;
    params[i].counts = counts;
    params[i].pixels = pixels;
    params[i].mutex = &mutex;
    params[i].barrier = &barrier;
    pthread_create(&threads[i], NULL, compute_buddhabrot, (void *)&params[i]);
  }

  for (int i = 0; i < numProcesses; i++)
  {
    pthread_join(threads[i], NULL);
  }

  pthread_barrier_destroy(&barrier);

  gettimeofday(&tend, NULL);
  double timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec) / 1.e6;
  printf("Computed buddhabrot set (%dx%d) in %g seconds\n", size, size, timer);

  char filename[64];
  snprintf(filename, sizeof(filename), "buddhabrot-%d-%ld.ppm", size, time(0));
  printf("Writing file: %s\n", filename);
  write_ppm(filename, pixels, size, size);

  // Free
  free(threads);
  free(params);

  for (int i = 0; i < size; i++)
  {
    free(counts[i]);
  }
  free(counts);
  free(pixels);
}