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
    
    print_image(&resized);

    return 0;
}