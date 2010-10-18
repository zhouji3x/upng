#include <stdio.h>
#include <malloc.h>

#include "upng.h"

#define HI(w) (((w) >> 8) & 0xFF)
#define LO(w) ((w) & 0xFF)

int main(int argc, char** argv) {
	FILE* fh;
	upng_error error;
	upng_t* upng;
	unsigned width, height;
	unsigned x, y;

	if (argc <= 2) {
		return 0;
	}

	upng = upng_new();
	error = upng_decode_file(upng, argv[1]);
	if (error != UPNG_EOK) {
		printf("error: %u %u\n", upng_get_error(upng), upng_get_error_line(upng));
		return 0;
	}

	width = upng_get_width(upng);
	height = upng_get_height(upng);

	printf("size:	%ux%ux%u (%u)\n", width, height, upng_get_bpp(upng), upng_get_size(upng));
	printf("format:	%u\n", upng_get_format(upng));

	fh = fopen(argv[2], "wb");
	fprintf(fh, "%c%c%c", 0, 0, 2);
	fprintf(fh, "%c%c%c%c%c", 0, 0, 0, 0, 0);
	fprintf(fh, "%c%c%c%c%c%c%c%c%c%c", 0, 0, 0, 0, LO(width), HI(width), LO(height), HI(height), 32, 8);

	for (y = 0; y != height; ++y) {
		for (x = 0; x != width; ++x) {
			putc(upng_get_buffer(upng)[(height - y - 1) * width * 4 + x * 4 + 0], fh);
			putc(upng_get_buffer(upng)[(height - y - 1) * width * 4 + x * 4 + 1], fh);
			putc(upng_get_buffer(upng)[(height - y - 1) * width * 4 + x * 4 + 2], fh);
			putc(upng_get_buffer(upng)[(height - y - 1) * width * 4 + x * 4 + 3], fh);
		}
	}

	fclose(fh);

	upng_free(upng);
	return 0;
}