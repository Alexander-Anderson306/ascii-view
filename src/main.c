#include <stdio.h>
#include <stdlib.h>
#include "../include/image.h"
#include "../include/print_image.h"
#include "../include/argparse.h"


int main(int argc, char* argv[]) {
    args_t args = parse_args(argc, argv);

    // Load image
    if (args.file_path == NULL)
        return 1;
    
    image_t original = load_image(args.file_path);
    image_t resized = make_resized(&original, args.max_width, args.max_height, args.character_ratio);
    
    print_image(&resized, args.edge_threshold);

    return 0;
}