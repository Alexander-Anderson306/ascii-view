#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#include "../include/image.h"


image_t load_image(const char* file_path) {
    int width, height, channels;
    unsigned char* raw_data = stbi_load(file_path, &width, &height, &channels, 0);

    if (!raw_data) {
        fprintf(stderr, "Error: Failed to load image '%s': %s!\n", file_path, stbi_failure_reason());
        return (image_t) {0}; // Return empty image on failure
    }

    // Convert to [0., 1.]
    size_t total_size = (size_t) width * height * channels;
    double* data = calloc(total_size, sizeof(*data));
    if (!data) {
        fprintf(stderr, "Error: Failed to allocate memory for image data!\n");
        stbi_image_free(raw_data);
        return (image_t) {0}; // Return empty image on failure
    }

    for (size_t i = 0; i < total_size; i++) {
        data[i] = raw_data[i] / 255.0;
    }

    stbi_image_free(raw_data);

    return (image_t) {
        .width = (size_t) width,
        .height = (size_t) height,
        .channels = (size_t) channels,
        .data = data
    };
}


// Don't need this function but adding it in case...
void free_image(image_t* image) {
    if (image && image->data) {
        free(image->data);
        image->data = NULL;
        image->width = image->height = image->channels = 0;
    }
}


// Gets pointer to pixel data at index (x, y)
double* get_pixel(image_t* image, size_t x, size_t y) {
    return &image->data[(y * image->width + x) * image->channels];
}


// Sets pixel channel values to those of new_pixel
void set_pixel(image_t* image, size_t x, size_t y, const double* new_pixel) {
    double* pixel = get_pixel(image, x, y);
    for (size_t c = 0; c < image->channels; c++) {
        pixel[c] = new_pixel[c];
    }
}


// Gets average pixel value in rectangular region; writes to `average`
void get_average(image_t* image, double* average, size_t x1, size_t x2, size_t y1, size_t y2) {
    // Set average to zero
    for (size_t c = 0; c < image->channels; c++) {
        average[c] = 0.0;
    }
    
    // Get total
    for (size_t x = x1; x < x2; x++) {
        for (size_t y = y1; y < y2; y++) {
            double* pixel = get_pixel(image, x, y);
            for (size_t c = 0; c < image->channels; c++) {
                average[c] += pixel[c];
            }
        }
    }

    // Divide by number of pixels in region
    double n_pixels = (double) (x2 - x1) * (y2 - y1);
    for (size_t c = 0; c < image->channels; c++) {
        average[c] /= n_pixels;
    }
}


image_t make_resized(image_t* original, size_t max_width, size_t max_height) {
    size_t width, height;
    size_t channels = original->channels;

    // Note: Dividing heights by 2 for approximate terminal font aspect ratio
    size_t proposed_height = (original->height * max_width) / (2 * original->width);
    if (proposed_height <= max_height) {
        width = max_width, height = proposed_height;
    } else {
        width = (2 * original->width * max_height) / (original->height);
        height = max_height;
    }

    double* data = calloc(width * height * channels, sizeof(*data));
    if (!data) {
        fprintf(stderr, "Error: Failed to allocate memory for resized image!\n");
        return (image_t) {0};
    }

    // i, j are coordinates in resized image
    for (size_t i = 0; i < width; i++) {
        size_t x1 = (i * original->width) / (width);
        size_t x2 = ((i + 1) * original->width) / (width);
        for (size_t j = 0; j < height; j++) {
            size_t y1 = (j * original->height) / (height);
            size_t y2 = ((j + 1) * original->height) / (height);

            get_average(original, &data[(i + j * width) * channels], x1, x2, y1, y2);
        }
    }

    return (image_t) {
        .width = width,
        .height = height,
        .channels = channels,
        .data = data
    };
}