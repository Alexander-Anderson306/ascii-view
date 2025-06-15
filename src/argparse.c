#include <stdio.h>
#include <string.h>
#include "../include/argparse.h"

#define DEFAULT_MAX_WIDTH 64
#define DEFAULT_MAX_HEIGHT 48
#define DEFAULT_CHARACTER_RATIO 2.0
#define DEFAULT_EDGE_THRESHOLD 4.0


void print_help(char* exec_alias) {
    printf("USAGE:\n");
    printf("\t%s <path/to/image> [OPTIONS]\n\n", exec_alias);

    printf("ARGUMENTS:\n");
    printf("\t<path/to/image>\t\tPath to image file\n\n");

    printf("OPTIONS:\n");
    printf("\t-mw <width>\t\tMaximum width in characters (default: %d)\n", DEFAULT_MAX_WIDTH);
    printf("\t-mh <height>\t\tMaximum height in characters (default: %d)\n", DEFAULT_MAX_HEIGHT);
    printf("\t-et <threshold>\t\tEdge detection threshold, range: 0 - 4 (default: %.1f, disabled)\n", DEFAULT_EDGE_THRESHOLD);
    printf("\t-cr <ratio>\t\tHeight-to-width ratio for characters (default: %.1f)\n", DEFAULT_CHARACTER_RATIO);
}


args_t parse_args(int argc, char* argv[]) {
    args_t args = {
        .file_path = NULL,
        .max_width = DEFAULT_MAX_WIDTH,
        .max_height = DEFAULT_MAX_HEIGHT,
        .character_ratio = DEFAULT_CHARACTER_RATIO,
        .edge_threshold = DEFAULT_EDGE_THRESHOLD,
    };

    if (argc == 1) {
        print_help(argv[0]);
        return args;
    }

    // Get file path
    if (!strcmp(argv[1], "-h")) {
        print_help(argv[0]);
        return args;
    } else {
        args.file_path = argv[1];
    }

    // Get optional parameters
    for (size_t i = 2; i < (size_t) argc; i++) {
        if (!strcmp(argv[i], "-mw"))
            args.max_width = (size_t) atoi(argv[++i]);
        else if (!strcmp(argv[i], "-mh"))
            args.max_height = (size_t) atoi(argv[++i]);
        else if (!strcmp(argv[i], "-et"))
            args.edge_threshold = atof(argv[++i]);
        else if (!strcmp(argv[i], "-cr"))
            args.character_ratio = atof(argv[++i]);
    }

    return args;
}
