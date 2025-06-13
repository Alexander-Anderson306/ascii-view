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

const char* get_color_code(const hsv_t* hsv) {
    if (hsv->saturation < 0.25) {
        return WHT;
    }
    
    if (hsv->hue >= 30.0 && hsv->hue < 90.0) {
        return YEL;
    } else if (hsv->hue >= 90.0 && hsv->hue < 150.0) {
        return GRN;
    } else if (hsv->hue >= 150.0 && hsv->hue < 210.0) {
        return CYN;
    } else if (hsv->hue >= 210.0 && hsv->hue < 270.0) {
        return BLU;
    } else if (hsv->hue >= 270.0 && hsv->hue < 330.0) {
        return MAG;
    } else {
        return RED;
    }
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

void print_image(image_t* image) {
    for (size_t y = 0; y < image->height; y++) {
        for (size_t x = 0; x < image->width; x++) {
            double* pixel = get_pixel(image, x, y);
            double grayscale;
            
            if (image->channels <= 2) {
                // Grayscale image
                grayscale = pixel[0];
            } else {
                // RGB image
                hsv_t hsv = rgb_to_hsv(pixel[0], pixel[1], pixel[2]);
                
                const char* color = get_color_code(&hsv);
                printf("%s", color);
                
                grayscale = calculate_grayscale_from_hsv(&hsv);
            }
            
            char ascii_char = get_ascii_char(grayscale);
            putchar(ascii_char);
        }
        printf("\n");
    }
}