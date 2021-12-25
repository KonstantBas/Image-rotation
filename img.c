#include "include/img.h"
#include "include/rotation.h"

size_t calculate_padding(size_t w) { return w % 4 == 0 ? 0 : 4 - w % 4; }

bool read_bitmap(FILE* fin, struct img_image* i) {
	if (!fin || !i) return false;
	
	read_header(fin, &i->header);
	size_t imgSize = sizeof(struct img_pixel) * i->header.biWidth * i->header.biHeight;

	i->pixels_data = malloc(imgSize);

	fseek(fin, i->header.bfOffbits, SEEK_SET);


    uint32_t height = i -> header.biHeight;
    uint32_t width = i->header.biWidth;


	for (size_t y = 0; y < height; y += 1)
	{

        size_t pixel_size = sizeof(struct img_pixel);



		fread(&i->pixels_data[y * height], pixel_size, width, fin);

		uint8_t _padding;
		fread(&_padding, sizeof(_padding), calculate_padding(width), fin);
	}

	return true;
}


bool write_bitmap(FILE* fout, struct img_image* i) {
	if (!fout || !i) return false;
	if (!write_header(fout, &i->header)) return false;

    uint32_t height = i -> header.biHeight;
    uint32_t width = i->header.biWidth;

	for (size_t y = 0; y < height; y += 1) {
		fwrite((i->pixels_data + y*width), sizeof(struct img_pixel), width, fout);

		uint8_t _padding = 0x0;
		fwrite(&_padding, 1, calculate_padding(width), fout);
	}


	return true;
}
