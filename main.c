#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <errno.h>
#include <malloc.h>

#include "include/err.h"
#include "include/img.h"





int main(int argc, char** argv)
{

	struct img_image image = { 0 };

	if (argc == 1) {
	    errno = NO_FILENAME;
		perror("Specify file name");
		exit(NO_FILENAME);
	}

	FILE* bmp_file = fopen(argv[1], "rb");
	if (!bmp_file || !read_bitmap(bmp_file, &image))
	{
		perror("File failure");
		exit(EXIT_FAILURE);
	}


	fclose(bmp_file);


	struct img_image rotated = {  0 };
	rotated.header =image.header;
	rotated.header.biWidth  =image.header.biHeight;
	rotated.header.biHeight = rotated.header.biWidth;

	rotated.pixels_data = malloc(sizeof(struct img_pixel) * rotated.header.biWidth * rotated.header.biHeight);

    for (size_t i = 0; i < rotated.header.biWidth * rotated.header.biHeight; i += 1)
		rotated.pixels_data[i] =image.pixels_data[i];

	FILE* bmp_rotated = fopen("rot.bmp", "wb");
	fwrite(&image.header, sizeof(struct bitmap_header), 1, bmp_rotated);
    for (size_t i = 0; i < image.header.biHeight; i += 1){
		fwrite( &image.pixels_data[i *image.header.biWidth], sizeof(struct img_pixel), image.header.biWidth, bmp_rotated );

		uint8_t _p = 0x0;
		fwrite(&_p, 1, calculate_padding(image.header.biWidth), bmp_rotated);
	}
	fclose(bmp_rotated);

	free(image.pixels_data);
	free(rotated.pixels_data);

#if 0
	
	fseek(bmp_file, header.bfOffbits, SEEK_SET);

	struct img_image img = { 0 };
	puts("READING................................");
	if (!read_bitmap(bmp_file, &our_img, header)) {
		fprintf(stderr, "Error reading bitmap\n");
		exit(EXIT_FAILURE);
	}

	uint8_t byte;
	while(fread(&byte, sizeof(byte), 1, bmp_file)) printf("EXTRA BYTE:\t0x%"PRIx8"\n", byte);
	
	printf(
		"Read image: %"PRIu32"x%"PRIu32", %zu pixels\n",
		img.width,
		img.height,
		sizeof(img.pixels_data)
	);

	fclose(bmp_file);


	FILE* h = fopen("header.bin", "wb");
	write_header(h, &header);
	fclose(h);

	FILE* rotated = fopen("rot.bmp", "wb");

	size_t img_size = sizeof(struct img_pixel) * header.biWidth * header.biHeight;
	struct img_pixel* rpx = malloc(img_size);

	rotate_pixels_32(img.pixels_data, rpx, header.biWidth, header.biHeight);
	

	uint32_t buf_width = header.biWidth;
	header.biWidth = header.biHeight;
	header.biHeight = buf_width;
	fwrite(&header, sizeof(header), 1, rotated);

	
	for (size_t i = 0; i < header.biHeight; i += 1) {
	    fwrite((rpx + i*header.biWidth),
            sizeof(struct img_pixel),
                    header.biWidth, rotated);
		uint8_t padding = 0;
		fwrite(&padding,
            1,
            calculate_padding(header.biHeight), rotated);
	}


	fclose(rotated);
	free(rpx);
	free(img.pixels_data);
#endif

	puts("\n");
	return 0;
}
