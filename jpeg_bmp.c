#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
//#include <math.h>

#include <sys/time.h>

#include "jpeg-utils.h"
#include "tjpeg-utils.h"

#include "bmp_utils.h"


#if 01
unsigned int get_tick_count()
{
#ifdef WIN32
    return GetTickCount();
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
#endif
// ×¢£ºÐèrt¿â
#if 0
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    unsigned int time = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
    return time;
#endif
}
#endif

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
    unsigned int read_size = 0;
    int ret = 0;
    
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
        ret = -1;
        goto end;
    }
    read_size = fread(jpeg_buffer, 1, jpeg_size, fp);
    if (read_size != jpeg_size)
    {
        ret = -1;
        goto end;
    }
    rgb_buffer = (unsigned char *)malloc(sizeof(char) * rgb_size);
    jpeg2rgb(jpeg_buffer, jpeg_size, rgb_buffer, &rgb_size, &width, &height);
    
    swap_rgb(rgb_buffer, rgb_size);

    write_bmp_file(bmp_file, rgb_buffer, width, height);

end:
    if (jpeg_buffer != NULL)
    {
        free(jpeg_buffer);
        jpeg_buffer = NULL;
    }
    fclose(fp);
    return ret;
}

int jpg_to_bmp2(const char* jpg_file, const char* bmp_file)
{
    int width, height, components;
    FILE* fp = NULL;
    unsigned char* jpeg_buffer = NULL;
    unsigned int jpeg_size = 0;
    unsigned int read_size = 0;
    int ret = 0;
    
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
        ret = -1;
        goto end;
    }
    read_size = fread(jpeg_buffer, 1, jpeg_size, fp);
    if (read_size != jpeg_size)
    {
        ret = -1;
        goto end;
    }
    
    jpeg_header(jpeg_buffer, jpeg_size, &width, &height, &components);
    
    rgb_size = width * height * components;
    rgb_buffer = (unsigned char *)malloc(sizeof(char) * rgb_size);

    printf("read jpeg header %d %d %d total: %d\n", width, height, components, rgb_size);

    jpeg2rgb1(jpeg_buffer, jpeg_size, rgb_buffer, &rgb_size);
    
    swap_rgb(rgb_buffer, rgb_size);

    write_bmp_file(bmp_file, rgb_buffer, width, height);

end:
    if (jpeg_buffer != NULL)
    {
        free(jpeg_buffer);
        jpeg_buffer = NULL;
    }
    fclose(fp);
    return ret;
}

int jpg_to_bmp3(const char* jpg_file, const char* bmp_file)
{
    int width, height, subsample, colorspace;
    FILE* fp = NULL;
    unsigned char* jpeg_buffer = NULL;
    unsigned int jpeg_size = 0;
    unsigned int read_size = 0;
    int ret = 0;
    
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
        ret = -1;
        goto end;
    }
    read_size = fread(jpeg_buffer, 1, jpeg_size, fp);
    if (read_size != jpeg_size)
    {
        ret = -1;
        goto end;
    }
    
    tjpeg_header(jpeg_buffer, jpeg_size, &width, &height, &subsample, &colorspace);
    
    rgb_size = width * height * 3;
    rgb_buffer = (unsigned char *)malloc(sizeof(char) * rgb_size);

    printf("read jpeg header %d %d %d %d total: %d\n", width, height, subsample, colorspace, rgb_size);
    
    tjpeg2rgb(jpeg_buffer, jpeg_size, rgb_buffer, &rgb_size);
    
    swap_rgb(rgb_buffer, rgb_size);

    write_bmp_file(bmp_file, rgb_buffer, width, height);

end:
    if (jpeg_buffer != NULL)
    {
        free(jpeg_buffer);
        jpeg_buffer = NULL;
    }
    fclose(fp);
    return ret;
}

int jpg_to_bmp4(const char* jpg_file, const char* bmp_file)
{
    int width, height, subsample, colorspace;
    FILE* fp = NULL;
    unsigned char* jpeg_buffer = NULL;
    unsigned int jpeg_size = 0;
    unsigned int read_size = 0;
    int ret = 0;
    
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
        ret = -1;
        goto end;
    }
    read_size = fread(jpeg_buffer, 1, jpeg_size, fp);
    if (read_size != jpeg_size)
    {
        ret = -1;
        goto end;
    }
    
    tjpeg_header(jpeg_buffer, jpeg_size, &width, &height, &subsample, &colorspace);

    printf("read jpeg header %d %d %d %d total: %d\n", width, height, subsample, colorspace, rgb_size);
    
    tjpeg2rgb_1(jpeg_buffer, jpeg_size, &rgb_buffer, &rgb_size);
    
    swap_rgb(rgb_buffer, rgb_size);

    write_bmp_file(bmp_file, rgb_buffer, width, height);

end:
    if (jpeg_buffer != NULL)
    {
        free(jpeg_buffer);
        jpeg_buffer = NULL;
    }
    fclose(fp);
    return ret;
}


int bmp_to_jpg(const char* bmp_file, const char* jpg_file)
{
    unsigned char* buffer = NULL;
    int size;
    int width,height;

    read_bmp_file(bmp_file, &buffer, &size, &width, &height);
    swap_rgb(buffer, size);
    printf("size: %d, width: %d height: %d\n", size, width, height);
    int a = get_tick_count();
    write_jpeg_file(jpg_file, buffer, width, height, 100);
    int b = get_tick_count();
    printf("write jpeg file. time: %d\n", b-a);

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
    int a = get_tick_count();
    rgb2jpeg(buffer, width, height, 100, &jpg_buffer, &jpg_size);
    int b = get_tick_count();
    
    printf("got jpeg size: %d time: %d\n", (int)jpg_size, b-a);

    fp = fopen(jpg_file, "wb");
    if (fp == NULL)
    {
        printf("open file %s failed.\n", jpg_file);
        return -1;
    }
    fwrite(jpg_buffer, 1, jpg_size, fp);

    free(buffer);
    free(jpg_buffer);

    fclose(fp);
    return 0;
}

int bmp_to_jpg2(const char* bmp_file, const char* jpg_file)
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
    int a = get_tick_count();
    trgb2jpeg(buffer, width, height, 100, &jpg_buffer, &jpg_size);
    
    int b = get_tick_count();
    
    printf("got jpeg size: %d time: %d\n", (int)jpg_size, b-a);

    fp = fopen(jpg_file, "wb");
    if (fp == NULL)
    {
        printf("open file %s failed.\n", jpg_file);
        return -1;
    }
    fwrite(jpg_buffer, 1, jpg_size, fp);

    free(buffer);
    free(jpg_buffer);

    fclose(fp);
    return 0;
}

/////////////////////////////////////////////////////////////

int jpg_to_yuv(const char* jpg_file, const char* yuv_file)
{
    FILE* fp = NULL;
    unsigned char* jpeg_buffer = NULL;
    unsigned int jpeg_size = 0;
    size_t ret = 0;
    int yuv_type = 0;
    
    unsigned char* yuv_buffer = NULL;
    int yuv_size = 0;
    
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
        fclose(fp);
        return -1;
    }
    ret = fread(jpeg_buffer, 1, jpeg_size, fp);
    if (ret != jpeg_size)
    {
        fclose(fp);
        free(jpeg_buffer);
        return -1;
    }
    
    int a = get_tick_count();

    tjpeg2yuv(jpeg_buffer, jpeg_size, &yuv_buffer, &yuv_size, &yuv_type);
        
    int b = get_tick_count();

    printf("yuv size: %d subsample: %d time: %d\n", yuv_size, yuv_type,  b-a);

    fclose(fp);
    
    fp = fopen(yuv_file, "wb");
    if (fp == NULL)
    {
        printf("open file %s failed.\n", yuv_file);
        return -1;
    }
    fwrite(yuv_buffer, 1, yuv_size, fp);

    free(yuv_buffer);
    fclose(fp);
    return 0;
}

int yuv_to_jpg(const char* yuv_file, const char* jpg_file, int yuv_type)
{
    FILE* fp = NULL;
    unsigned char* yuv_buffer = NULL;
    int yuv_size = 0;
    
    unsigned char* jpeg_buffer = NULL;
    int jpeg_size = 0;

    size_t ret = 0;
    //int yuv_type = 1;
    
    fp = fopen(yuv_file, "rb");
    if (fp == NULL)
    {
        printf("open file %s failed.\n", yuv_file);
        return -1;
    }
    
    fseek(fp, 0L, SEEK_END);
    yuv_size = ftell(fp);
    rewind(fp);

    yuv_buffer = (unsigned char *)malloc(sizeof(char) * yuv_size);
    if (yuv_buffer == NULL)
    {
        fclose(fp);
        return -1;
    }
    ret = fread(yuv_buffer, 1, yuv_size, fp);
    if (ret != yuv_size)
    {
        fclose(fp);
        free(yuv_buffer);
        return -1;
    }
    
    printf("yuv size: %d\n", yuv_size);
    int a = get_tick_count();

    tyuv2jpeg(yuv_buffer, yuv_size, 1920, 1080, yuv_type, &jpeg_buffer, (unsigned long*)&jpeg_size, 100);
        
    int b = get_tick_count();

    printf("jpeg size: %d time: %d\n", jpeg_size,  b-a);

    fclose(fp);
    
    fp = fopen(jpg_file, "wb");
    if (fp == NULL)
    {
        printf("open file %s failed.\n", jpg_file);
        return -1;
    }
    fwrite(jpeg_buffer, 1, jpeg_size, fp);

    fclose(fp);
    free(jpeg_buffer);
    return 0;
}

int bmp_to_yuv(const char* bmp_file, const char* yuv_file)
{
    FILE* fp = NULL;
    unsigned char* buffer = NULL;
    int size;
    int width,height;
    unsigned char* yuv_buffer = NULL;
    int  yuv_size = 0;

    read_bmp_file(bmp_file, &buffer, &size, &width, &height);
    swap_rgb(buffer, size);
    printf("size: %d, width: %d height: %d\n", size, width, height);

    int a = get_tick_count();

    trgb2yuv(buffer, width, height, &yuv_buffer, &yuv_size, 2);
    
    int b = get_tick_count();
    
    printf("got rgb size: %d time: %d\n", (int)yuv_size, b-a);

    fp = fopen(yuv_file, "wb");
    if (fp == NULL)
    {
        printf("open file %s failed.\n", yuv_file);
        return -1;
    }
    fwrite(yuv_buffer, 1, yuv_size, fp);

    fclose(fp);
    free(buffer);
    free(yuv_buffer);

    return 0;
}

int yuv_to_bmp(const char* yuv_file, const char* bmp_file, int yuv_type)
{
    FILE* fp = NULL;
    unsigned char* yuv_buffer = NULL;
    int yuv_size = 0;
    
    unsigned char* rgb_buffer = NULL;
    int rgb_size = 0;

    size_t ret = 0;
    int width = 1920;
    int height = 1080;
    
    //int yuv_type = 2;
    
    fp = fopen(yuv_file, "rb");
    if (fp == NULL)
    {
        printf("open file %s failed.\n", yuv_file);
        return -1;
    }
    
    fseek(fp, 0L, SEEK_END);
    yuv_size = ftell(fp);
    rewind(fp);

    yuv_buffer = (unsigned char *)malloc(sizeof(char) * yuv_size);
    if (yuv_buffer == NULL)
    {
        fclose(fp);
        return -1;
    }
    ret = fread(yuv_buffer, 1, yuv_size, fp);
    if (ret != yuv_size)
    {
        fclose(fp);
        free(yuv_buffer);
        return -1;
    }
    
    printf("yuv size: %d\n", yuv_size);
    int a = get_tick_count();

    tyuv2rgb(yuv_buffer, yuv_size, width, height, yuv_type, &rgb_buffer, &rgb_size);
        
    int b = get_tick_count();

    printf("rgb size: %d time: %d\n", rgb_size,  b-a);

    swap_rgb(rgb_buffer, rgb_size);

    write_bmp_file(bmp_file, rgb_buffer, width, height);

    fclose(fp);
    free(rgb_buffer);
    free(yuv_buffer);
    return 0;
}
