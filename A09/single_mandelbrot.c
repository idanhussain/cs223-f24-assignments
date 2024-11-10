/*----------------------------------------------
 * Author: Idan Hussain 
 * Date: November 9 2024
 * This program computes the Mandelbrot set
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char *argv[])
{
    int size = 480;
    float xmin = -2.0;
    float xmax = 0.47;
    float ymin = -1.12;
    float ymax = 1.12;
    int maxIterations = 1000;

    int opt;
    while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1)
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
        case '?':
            printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]);
            return 1;
        }
    }

    printf("Generating mandelbrot with size %dx%d\n", size, size);
    printf("  X range = [%.4f, %.4f]\n", xmin, xmax);
    printf("  Y range = [%.4f, %.4f]\n", ymin, ymax);

    // Allocating memory
    struct ppm_pixel *pixels = malloc(size * size * sizeof(struct ppm_pixel));
    if (!pixels)
    {
        printf("Error: Memory allocation failed\n");
        return 1;
    }

    // getting time
    struct timeval tstart, tend;
    gettimeofday(&tstart, NULL);

    // Generate Mandelbrot
    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            float xfrac = (float)col / size;
            float yfrac = (float)row / size;
            float x0 = xmin + xfrac * (xmax - xmin);
            float y0 = ymin + yfrac * (ymax - ymin);

            float x = 0;
            float y = 0;
            int iter = 0;
            while (iter < maxIterations && x * x + y * y < 4)
            {
                float xtemp = x * x - y * y + x0;
                y = 2 * x * y + y0;
                x = xtemp;
                iter++;
            }

            // Set pixel color
            if (iter < maxIterations)
            {
                srand(time(NULL));
                int redMultiplier = (rand() % 100);
                int greenMultiplier = (rand() % 100);
                int blueMultiplier = (rand() % 100);

                pixels[row * size + col].red = (iter * redMultiplier) % 255;
                pixels[row * size + col].green = (iter * greenMultiplier) % 255;
                pixels[row * size + col].blue = (iter * blueMultiplier) % 255;
            }
            else
            {
                pixels[row * size + col].red = 0;
                pixels[row * size + col].green = 0;
                pixels[row * size + col].blue = 0;
            }
        }
    }

    gettimeofday(&tend, NULL);
    double timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec) / 1.e6;
    printf("Computed mandelbrot set (%dx%d) in %g seconds\n", size, size, timer);

    srand(time(0));

    // Writing the image
    char filename[64];
    snprintf(filename, sizeof(filename), "mandelbrot-%d-%ld.ppm", size, time(0));
    write_ppm(filename, pixels, size, size);

    free(pixels);
    return 0;
}