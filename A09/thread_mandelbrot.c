/*----------------------------------------------
 * Author: Idan Hussain
 * Date: November 9 2024
 * This program computes the Mandelbrot set using four threads
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include "read_ppm.h"
#include "write_ppm.h"
#define MAX_COLORS 1000

struct ThreadData
{
  struct ppm_pixel *pixels;
  int size;
  int startRow, endRow;
  int startCol, endCol;
  float xMin, xMax, yMin, yMax;
  int maxIterations;
  struct ppm_pixel *palette;
};

void *computeMandelbrot(void *arg)
{
  struct ThreadData *data = (struct ThreadData *)arg;

  for (int row = data->startRow; row < data->endRow; row++)
  {
    for (int col = data->startCol; col < data->endCol; col++)
    {
      float xFrac = (float)col / data->size;
      float yFrac = (float)row / data->size;
      float x0 = data->xMin + xFrac * (data->xMax - data->xMin);
      float y0 = data->yMin + yFrac * (data->yMax - data->yMin);

      float x = 0, y = 0;
      int iter = 0;
      while (iter < data->maxIterations && x * x + y * y < 4)
      {
        float xTemp = x * x - y * y + x0;
        y = 2 * x * y + y0;
        x = xTemp;
        iter++;
      }

      int index = row * data->size + col;
      if (iter < data->maxIterations)
      {
        data->pixels[index] = data->palette[iter % MAX_COLORS]; // to wrap back around the start of palette
      }
      else
      {
        data->pixels[index].red = 0;
        data->pixels[index].green = 0;
        data->pixels[index].blue = 0;
      }
    }
  }
  printf("Thread %lu) finished\n", pthread_self());
  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  int size = 480;
  float xMin = -2.0, xMax = 0.47;
  float yMin = -1.12, yMax = 1.12;
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
      xMin = atof(optarg);
      break;
    case 'r':
      xMax = atof(optarg);
      break;
    case 't':
      yMax = atof(optarg);
      break;
    case 'b':
      yMin = atof(optarg);
      break;
    case '?':
      printf("usage: %s -s <size> -l <xmin> -r <xmax> "
             "-b <ymin> -t <ymax> -p <numProcesses>\n",
             argv[0]);
      return 1;
    }
  }

  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xMin, xMax);
  printf("  Y range = [%.4f,%.4f]\n", yMin, yMax);

  // Allocating memory
  struct ppm_pixel *pixels = malloc(size * size * sizeof(struct ppm_pixel));
  if (!pixels)
  {
    printf("Error: Memory allocation failed :(\n");
    return 1;
  }

  // Using a color palette
  struct ppm_pixel palette[MAX_COLORS];

  srand(time(0));

  int baseRed = rand() % 255;
  int baseGreen = rand() % 255;
  int baseBlue = rand() % 255;

  for (int i = 0; i < MAX_COLORS; i++)
  {
    palette[i].red = (baseRed + rand() % 100 - 50) % 255;
    palette[i].green = (baseGreen + rand() % 100 - 50) % 255;
    palette[i].blue = (baseBlue + rand() % 100 - 50) % 255;
  }

  pthread_t threads[4];
  struct ThreadData threadArgs[4];

  struct timeval tStart, tEnd;
  gettimeofday(&tStart, NULL);

  for (int i = 0; i < numProcesses; i++)
  {
    threadArgs[i].pixels = pixels;
    threadArgs[i].size = size;
    threadArgs[i].xMin = xMin;
    threadArgs[i].xMax = xMax;
    threadArgs[i].yMin = yMin;
    threadArgs[i].yMax = yMax;
    threadArgs[i].maxIterations = maxIterations;
    threadArgs[i].palette = palette;

    // Quadrant boundaries
    threadArgs[i].startRow = (i / 2) * (size / 2);
    threadArgs[i].endRow = threadArgs[i].startRow + (size / 2);
    threadArgs[i].startCol = (i % 2) * (size / 2);
    threadArgs[i].endCol = threadArgs[i].startCol + (size / 2);

    printf("Thread %d) sub-image block: cols (%d, %d) to rows (%d, %d)\n",
           i, threadArgs[i].startCol, threadArgs[i].endCol,
           threadArgs[i].startRow, threadArgs[i].endRow);

    pthread_create(&threads[i], NULL, computeMandelbrot, &threadArgs[i]);
  }

  for (int i = 0; i < numProcesses; i++)
  {
    pthread_join(threads[i], NULL);
  }

  gettimeofday(&tEnd, NULL);
  double timer = tEnd.tv_sec - tStart.tv_sec + (tEnd.tv_usec - tStart.tv_usec) / 1.e6;
  printf("Computed mandelbrot set (%dx%d) in %g seconds\n", size, size, timer);

  // Writing the image
  char fileName[64];
  snprintf(fileName, sizeof(fileName), "mandelbrot-%d-%ld.ppm", size, time(0));
  write_ppm(fileName, pixels, size, size);

  free(pixels);
  return 0;
}
