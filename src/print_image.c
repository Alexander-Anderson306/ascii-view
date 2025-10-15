#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../include/image.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Characters to print
#define VALUE_CHARS " .-=+*x#$&X@"
#define N_VALUES (sizeof(VALUE_CHARS) - 1) // Exclude null

// Color ANSI codes
#define RESET "\x1b[0m"


typedef struct {
    double hue;
    double saturation;
    double value;
} hsv_t;


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


hsv_t rgb_to_hsv(double red, double green, double blue) {
    hsv_t hsv;
    
    double* max = get_max(&red, &green, &blue);
    double* min = get_min(&red, &green, &blue);
    
    hsv.value = *max;
    double chroma = hsv.value - *min;
    
    // Calculate saturation
    if (fabs(hsv.value) < 1e-4) {
        hsv.saturation = 0.0;
    } else {
        hsv.saturation = chroma / hsv.value;
    }
    
    // Calculate hue
    if (chroma < 1e-4) {
        hsv.hue = 0.0;
    } else if (max == &red) {
        hsv.hue = 60.0 * fmod((green - blue) / chroma, 6.0);
        if (hsv.hue < 0.0) hsv.hue += 360.0;
    } else if (max == &green) {
        hsv.hue = 60.0 * (2.0 + (blue - red) / chroma);
    } else {
        hsv.hue = 60.0 * (4.0 + (red - green) / chroma);
    }
    
    return hsv;
}


char* get_color_code(const hsv_t* hsv) {
    // This function is no longer used with truecolor
    return NULL;
}


double calculate_grayscale_from_hsv(const hsv_t* hsv) {
    // Use value * value for increased contrast
    return hsv->value * hsv->value;
}


char get_ascii_char(double grayscale) {
    size_t index = (size_t) (grayscale * N_VALUES);

    // Clamp
    if (index >= N_VALUES) {
        index = N_VALUES - 1;
    }

    return VALUE_CHARS[index];
}


char get_sobel_angle_char(double sobel_angle) {
    if ((22.5 <= sobel_angle && sobel_angle <= 67.5) || (-157.5 <= sobel_angle && sobel_angle <= -112.5))
        return '\\';
    else if ((67.5 <= sobel_angle && sobel_angle <= 112.5) || (-112.5 <= sobel_angle && sobel_angle <= -67.5))
        return '_';
    else if ((112.5 <= sobel_angle && sobel_angle <= 157.5) || (-67.5 <= sobel_angle && sobel_angle <= -22.5))
        return '/';
    else
        return '|';
}


void print_image(image_t* image, double edge_threshold) {
    image_t grayscale = make_grayscale(image);
    double* sobel_x = calloc(grayscale.width * grayscale.height, sizeof(*sobel_x));
    double* sobel_y = calloc(grayscale.width * grayscale.height, sizeof(*sobel_y));
    if (!sobel_x || !sobel_y)
        fprintf(stderr, "Error: Failed to allocate memory for edge detection!\n");

    if (edge_threshold < 4.0)
        get_sobel(&grayscale, sobel_x, sobel_y);

    for (size_t y = 0; y < image->height; y++) {
        for (size_t x = 0; x < image->width; x++) {
            double* pixel = get_pixel(image, x, y);

            size_t index = y * image->width + x;
            double sx = sobel_x[index];
            double sy = sobel_y[index];

            double square_sobel_magnitude = sx * sx + sy * sy;
            double sobel_angle = atan2(sy, sx) * 180. / M_PI;

            char ascii_char;
            
            double grayscale;
            int r = 255, g = 255, b = 255; // Default white for grayscale
            
            if (image->channels <= 2) {
                // Grayscale image
                grayscale = pixel[0];
                r = g = b = (int)(pixel[0] * 255);
            } else {
                // RGB image
                hsv_t hsv = rgb_to_hsv(pixel[0], pixel[1], pixel[2]);
                
                grayscale = calculate_grayscale_from_hsv(&hsv);
                
                // Get actual RGB values from the image
                r = (int)(pixel[0] * 255);
                g = (int)(pixel[1] * 255);
                b = (int)(pixel[2] * 255);
            }

            ascii_char = get_ascii_char(grayscale);

            // If edge
            if (square_sobel_magnitude >= edge_threshold * edge_threshold)
                ascii_char = get_sobel_angle_char(sobel_angle);
            
            // Use 24-bit truecolor ANSI escape code
            printf("\x1b[38;2;%d;%d;%dm%c", r, g, b, ascii_char);
        }
        printf("\n");
    }

    printf("%s", RESET);
    
    free(sobel_x);
    free(sobel_y);
    free_image(&grayscale);
}
