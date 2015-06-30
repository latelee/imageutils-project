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

int jpg_to_bmp1(const char* jpg_file, const char* bmp_file)
{
    int width, height;
    FILE* fp = NULL;
    unsigned char* jpeg_buffer = NULL;
    unsigned int jpeg_size = 0;
    size_t ret = 0;
    
    unsigned char* rgb_buffer = NULL;
    int rgb_size = 512*512*3;
    
    fp = fopen(jpg_file, "rb");
    if (fp == NULL)
    {
        printf("open file %s failed.\n", jpg_file);
        return -1;
    }
    
    fseek(fp, 0L, SEEK_END);
    jpeg_size = ftell(fp);
    rewind(fp);

    jpeg_buffer = (unsigned char *)malloc(sizeof(char) * jpeg_size);
    if (jpeg_buffer == NULL)
    {
        return -1;
    }
    ret = fread(jpeg_buffer, 1, jpeg_size, fp);
    if (ret != jpeg_size)
    {
        return -1;
    }
    rgb_buffer = (unsigned char *)malloc(sizeof(char) * rgb_size);
    jpeg2rgb(jpeg_buffer, jpeg_size, rgb_buffer, &rgb_size, &width, &height);
    
    swap_rgb(rgb_buffer, rgb_size);

    write_bmp_file(bmp_file, rgb_buffer, width, height);

    return 0;
}

int jpg_to_bmp2(const char* jpg_file, const char* bmp_file)
{
    int width, height, components;
    FILE* fp = NULL;
    unsigned char* jpeg_buffer = NULL;
    unsigned int jpeg_size = 0;
    size_t ret = 0;
    
    unsigned char* rgb_buffer = NULL;
    int rgb_size = 0;
    
    fp = fopen(jpg_file, "rb");
    if (fp == NULL)
    {
        printf("open file %s failed.\n", jpg_file);
        return -1;
    }
    
    fseek(fp, 0L, SEEK_END);
    jpeg_size = ftell(fp);
    rewind(fp);

    jpeg_buffer = (unsigned char *)malloc(sizeof(char) * jpeg_size);
    if (jpeg_buffer == NULL)
    {
        return -1;
    }
    ret = fread(jpeg_buffer, 1, jpeg_size, fp);
    if (ret != jpeg_size)
    {
        return -1;
    }
    
    jpeg_header(jpeg_buffer, jpeg_size, &width, &height, &components);
    
    rgb_size = width * height * components;
    rgb_buffer = (unsigned char *)malloc(sizeof(char) * rgb_size);

    printf("read jpeg header %d %d %d total: %d\n", width, height, components, rgb_size);
    
    jpeg2rgb1(jpeg_buffer, jpeg_size, rgb_buffer, &rgb_size);
    
    swap_rgb(rgb_buffer, rgb_size);

    write_bmp_file(bmp_file, rgb_buffer, width, height);

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

int bmp_to_jpg1(const char* bmp_file, const char* jpg_file)
{
    FILE* fp = NULL;
    unsigned char* buffer = NULL;
    int size;
    int width,height;
    unsigned char* jpg_buffer = NULL;
    unsigned long  jpg_size = 0;

    read_bmp_file(bmp_file, &buffer, &size, &width, &height);
    swap_rgb(buffer, size);
    printf("size: %d, width: %d height: %d\n", size, width, height);

    //write_jpeg_file(jpg_file, buffer, width, height, 50);

    rgb2jpeg(buffer, width, height, 100, &jpg_buffer, &jpg_size);
    
    printf("got jpeg size: %d\n", (int)jpg_size);

    fp = fopen(jpg_file, "wb");
    if (fp == NULL)
    {
        printf("open file %s failed.\n", jpg_file);
        return -1;
    }
    fwrite(jpg_buffer, 1, jpg_size, fp);

    free(buffer);
    free(jpg_buffer);

    return 0;
}