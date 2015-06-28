#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <math.h>

#include "jpeg-utils.h"
#include "bmp_utils.h"

int jpg_to_bmp(const char* jpg_file, const char* bmp_file)
{
    unsigned char* buffer = NULL;
    int size;
    int width, height;

    read_jpeg_file(jpg_file, &buffer, &size, &width, &height);

    swap_rgb(buffer, size);

    write_bmp_file(bmp_file, buffer, width, height);

    free(buffer);

    return 0;
}

int bmp_to_jpg(const char* bmp_file, const char* jpg_file)
{
    unsigned char* buffer = NULL;
    int size;
    int width,height;

    read_bmp_file(bmp_file, &buffer, &size, &width, &height);
    swap_rgb(buffer, size);
    printf("size: %d, width: %d height: %d\n", size, width, height);
    write_jpeg_file(jpg_file, buffer, width, height, 50);

    free(buffer);

    return 0;
}