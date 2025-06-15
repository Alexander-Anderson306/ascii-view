#include <stdlib.h>

typedef struct {
    char* file_path;
    size_t max_width;
    size_t max_height;
    double character_ratio;
    double edge_threshold;
} args_t;

args_t parse_args(int argc, char* argv[]);
