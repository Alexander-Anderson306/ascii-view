#include <stdio.h>
#include <stdlib.h>
#include "../include/image.h"
#include "../include/print_image.h"

#define DEFAULT_MAX_WIDTH 64
#define DEFAULT_MAX_HEIGHT 48


int main(int argc, char* argv[]) {
    // Parse arguments
    if (argc == 1) {
        printf(
            "Usage: %s [path/to/image] [max width (default %d)] [max height (default %d)]\n",
            argv[0], DEFAULT_MAX_WIDTH, DEFAULT_MAX_HEIGHT
        );
        return 1;
    }

    char* file_path = argv[1];
    size_t max_width = DEFAULT_MAX_WIDTH, max_height = DEFAULT_MAX_HEIGHT;
    if (argc >= 3) max_width = (size_t) atoi(argv[2]);
    if (argc >= 4) max_height = (size_t) atoi(argv[3]);

    // Load image
    image_t original = load_image(file_path);

    double character_ratio = 2.0;
    image_t resized = make_resized(&original, max_width, max_height, character_ratio);
    
    double Gx[] = {-1., 0., 1., -2., 0., 2., -1., 0., 1};
    double* convolution = calloc(resized.width * resized.height * resized.channels, sizeof(*convolution));
    get_convolution(&resized, Gx, convolution);

    for (size_t c = 0; c < resized.channels; c++) {
        for (size_t y = 0; y < resized.height; y++) {
            for (size_t x = 0; x < resized.width; x++) {
                size_t index = c + resized.channels * (x + resized.width * y);
                if (convolution[index] > 0.5)
                    printf("+");
                else if (convolution[index] < -0.5)
                    printf("-");
                else
                    printf(" ");
            }
            printf("\n");
        }
    }

    print_image(&resized);

    return 0;
}