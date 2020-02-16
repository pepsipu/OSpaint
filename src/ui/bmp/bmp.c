#include <ui/primitives.c>

// oh god this is so fragile i don't even understand how it works
// made via trial and error
#define bmp_pixel(k, i) (uint8_t) *(pixels + (img->width * 3 *img->height) - (img->height - k + (i * img->width * 3)))

struct file_header {
    uint16_t signature; // "BM", to signify that it's a bitmap file
    uint32_t file_size;
    uint32_t reserved; // must be 0
    uint32_t pixel_data;
} __attribute__((packed));

struct image_header {
    uint32_t header_size; // size of the image_header
    uint32_t width; // width in pixels
    uint32_t height; // height in pixels
    uint16_t planes; // image planes, always 1 for a bmp
    uint16_t bpp; // bits per pixel
    uint16_t compression; // compression
    uint32_t image_size;
    uint32_t horiz_pref; // preferred resolution, ignored
    uint32_t vert_pref; // preferred resolution ignored
    uint32_t color_map_entries;
    uint32_t important_colors; // ignored, xinOS can display the colors needed without sacrificing some
};

void draw_bmp(uint8_t *bmp, uint16_t x, uint16_t y) {
    struct file_header *file = bmp;
    struct image_header *img = bmp + sizeof(struct file_header);
    uint8_t *pixels = bmp + file->pixel_data;
    for (uint32_t i = img->height; i > 0; i--) {
        for (uint32_t k = 0; k < img->width * 3; k += 3) {
            uint32_t color;
            *(uint8_t *) &color = bmp_pixel(k, i);
            *((uint8_t *) &color + 1) = bmp_pixel(k + 1, i);
            *((uint8_t *) &color + 2) = bmp_pixel(k + 2, i);
            // why divide k by 4? good question! it works! don't touch it!
            plot_pixel(x + k/4, y + i, color);
        }
    }
}