#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../include/image.h"

// Characters to print
#define VALUE_CHARS " .-=+*x#$&X@"
#define N_VALUES (sizeof(VALUE_CHARS) - 1) // Exclude null

// Color ANSI codes
#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"


double* get_max(double* a, double* b, double* c) {
    if ((*a >= *b) && (*a >= *c)) {
        return a;
    } else if (*b >= *c) {
        return b;
    } else {
        return c;
    }
}

double* get_min(double* a, double* b, double* c) {
        if ((*a <= *b) && (*a <= *c)) {
        return a;
    } else if (*b <= *c) {
        return b;
    } else {
        return c;
    }
}


void print_image(image_t* image) {
    for (size_t y = 0; y < image->height; y++) {
        for (size_t x = 0; x < image->width; x++) {
            double* pixel = get_pixel(image, x, y);
            double grayscale;

            if (image->channels <= 2) {
                // Grayscale
                grayscale = pixel[0];
            } else {
                // RGB
                double red = pixel[0];
                double green = pixel[1];
                double blue = pixel[2];
                
                double* max = get_max(&red, &green, &blue);
                double value = *max;
                double chroma = value - *get_min(&red, &green, &blue);
                
                double saturation;
                if (fabs(value) < 1e-4) {
                    saturation = 0.0;
                } else {
                    saturation = chroma / value;
                }

                if (saturation < 0.25) {
                    printf(WHT);
                } else {
                    double hue;
                    if (max == &red) {
                        hue = 60.0 * fmod((green - blue) / chroma, 6.0);
                        if (hue < 0.0) hue += 360.0;
                    } else if (max == &green) {
                        hue = 60.0 * (2.0 + (blue - red)/chroma);
                    } else {
                        hue = 60.0 * (4.0 + (red - green)/chroma);
                    }

                    if (hue >= 30.0 && hue < 90.0) {
                        printf(YEL);
                    } else if (hue >= 90.0 && hue < 150.0) {
                        printf(GRN);
                    } else if (hue >= 150.0 && hue < 210.0) {
                        printf(CYN);
                    } else if (hue >= 210.0 && hue < 270.0) {
                        printf(BLU);
                    } else if (hue >= 270.0 && hue < 330.0) {
                        printf(MAG);
                    } else {
                        printf(RED);
                    }
                }

                grayscale = value * value;

                // grayscale = red + green + blue + chroma - 2 * value; // median

                // double value_3 = value * value * value;
                // double value_4 = value_3 * value;
                // double value_5 = value_4 * value;
                // grayscale = 6.0 * value_5 - 15.0 * value_4 + 10.0 * value_3;
            }

            size_t rounded_grayscale = grayscale * N_VALUES;
            putchar(VALUE_CHARS[rounded_grayscale]);
        }

        printf("\n");
    }
}

