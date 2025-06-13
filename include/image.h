#ifndef MY_IMAGE_LIB
#define MY_IMAGE_LIB

typedef struct {
    size_t width;
    size_t height;
    size_t channels;
    double* data;
} image_t;

image_t load_image(const char* file_path);

image_t make_resized(image_t* original, size_t max_width, size_t max_height, double character_ratio);

double* get_pixel(image_t* image, size_t x, size_t y);
void set_pixel(image_t* image, size_t x, size_t y, const double* new_pixel);

// void print_image(image_t* image, bool dark_mode);
#endif