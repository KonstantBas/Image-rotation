#include "img.h"


void rotate_pixels_32(struct img_pixel* from, struct img_pixel* to, uint32_t w, uint32_t h) {
    for (uint32_t y = 0; y < h; y += 1) {
        for (uint32_t x = 0; x < w; x += 1) {
            to[(w*h - w) + y - (x*w)] = from[y*w + x];
        }
    }
}
void rotate_pixels_64(struct img_pixel from[], struct img_pixel to[], uint32_t w, uint32_t h) {
    int64_t x = 0;
    for (int64_t i = 0; i < w; i += 1) {
        for(int64_t j = h-1; j >= 0; j -= 1) {
            *( to + i*h + (x++)) = *(from + j*w + i);
        }
        x = 0;
    }
}

void rotate_image(struct img_image* orig, struct img_image* to) {
    to->pixels_data = malloc(sizeof(struct img_pixel) * orig->header.biWidth * orig->header.biHeight);
    rotate_pixels_64(orig->pixels_data,to->pixels_data,orig->header.biWidth,orig->header.biHeight);
}

#endif
