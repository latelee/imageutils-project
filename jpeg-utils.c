#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>

// jpeg��ͷ�ļ�����ŵ�stdio.h����
#include "libjpeg/include/jpeglib.h"
#include "libjpeg/include/jerror.h"

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
// ע����rt��
#if 0
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    unsigned int time = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
    return time;
#endif
}
#endif

struct my_error_mgr {
  struct jpeg_error_mgr pub;
  jmp_buf setjmp_buffer;
};

typedef struct my_error_mgr * my_error_ptr;

void my_error_exit (j_common_ptr cinfo)
{
    my_error_ptr myerr = (my_error_ptr) cinfo->err;

    (*cinfo->err->output_message) (cinfo);

    longjmp(myerr->setjmp_buffer, 1);
}

// ��ȡJPGͼƬ���ݣ�����ѹ���ڴ��У�*rgb_buffer��Ҫ�����ͷ�
int read_jpeg_file(const char* jpeg_file, unsigned char** rgb_buffer, int* size, int* width, int* height)
{
    struct jpeg_decompress_struct cinfo;
    struct my_error_mgr jerr;
    FILE* fp;

    JSAMPARRAY buffer;
    int row_stride = 0;
    unsigned char* tmp_buffer = NULL;
    int rgb_size;

    fp = fopen(jpeg_file, "rb");
    if (fp == NULL)
    {
        printf("open file %s failed.\n", jpeg_file);
        return -1;
    }

    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;

    if (setjmp(jerr.setjmp_buffer))
    {
        jpeg_destroy_decompress(&cinfo);
        fclose(fp);
        return -1;
    }

    jpeg_create_decompress(&cinfo);

    jpeg_stdio_src(&cinfo, fp);

    jpeg_read_header(&cinfo, TRUE);

    //cinfo.out_color_space = JCS_RGB; //JCS_YCbCr;  // ���������ʽ

    jpeg_start_decompress(&cinfo);

    row_stride = cinfo.output_width * cinfo.output_components;
    *width = cinfo.output_width;
    *height = cinfo.output_height;

    rgb_size = row_stride * cinfo.output_height; // �ܴ�С
    *size = rgb_size;

    buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
    
    *rgb_buffer = (unsigned char *)malloc(sizeof(char) * rgb_size);    // �������ڴ�
    
    printf("debug--:\nrgb_size: %d, size: %d w: %d h: %d row_stride: %d \n", rgb_size,
                cinfo.image_width*cinfo.image_height*3,
                cinfo.image_width, 
                cinfo.image_height,
                row_stride);
    tmp_buffer = *rgb_buffer;
    while (cinfo.output_scanline < cinfo.output_height) // ��ѹÿһ��
    {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        // ���Ƶ��ڴ�
        memcpy(tmp_buffer, buffer[0], row_stride);
        tmp_buffer += row_stride;
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    fclose(fp);

    return 0;
}

int write_jpeg_file(const char* jpeg_file, unsigned char* rgb_buffer, int width, int height, int quality)
{
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    int row_stride = 0;
    FILE* fp = NULL;
    JSAMPROW row_pointer[1];

    cinfo.err = jpeg_std_error(&jerr);

    jpeg_create_compress(&cinfo);
    fp = fopen(jpeg_file, "wb");
    if (fp == NULL)
    {
        printf("open file %s failed.\n", jpeg_file);
        return -1;
    }
    jpeg_stdio_dest(&cinfo, fp);
    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, 1);  // todo 1 == true
    jpeg_start_compress(&cinfo, TRUE);
    row_stride = width * cinfo.input_components;

    while (cinfo.next_scanline < cinfo.image_height)
    {
        row_pointer[0] = &rgb_buffer[cinfo.next_scanline * row_stride];
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    fclose(fp);

    return 0;
}

//////////////////////////////////////////////////////////

int jpeg2rgb(unsigned char* jpeg_buffer, int jpeg_size, unsigned char* rgb_buffer, int* size, int* width, int* height)
{
    struct jpeg_decompress_struct cinfo;
    struct my_error_mgr jerr;

    JSAMPARRAY buffer;
    int row_stride = 0;
    unsigned char* tmp_buffer = NULL;
    int rgb_size;

    if (jpeg_buffer == NULL)
    {
        printf("no jpeg buffer here.\n");
        return -1;
    }
    if (rgb_buffer == NULL)
    {
        printf("you need to alloc rgb buffer.\n");
        return -1;
    }
    
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;

    if (setjmp(jerr.setjmp_buffer))
    {
        jpeg_destroy_decompress(&cinfo);
        return -1;
    }

    jpeg_create_decompress(&cinfo);

    jpeg_mem_src(&cinfo, jpeg_buffer, jpeg_size);

    jpeg_read_header(&cinfo, TRUE);

    //cinfo.out_color_space = JCS_EXT_BGR;//JCS_RGB; //JCS_YCbCr;  // ���������ʽ��Ĭ����JCS_RGB

    jpeg_start_decompress(&cinfo);

    row_stride = cinfo.output_width * cinfo.output_components;
    *width = cinfo.output_width;
    *height = cinfo.output_height;

    rgb_size = row_stride * cinfo.output_height; // �ܴ�С
    if (*size < rgb_size)
    {
        printf("rgb buffer to small, we need %d but has only: %d\n", rgb_size, *size);
    }
    
    *size = rgb_size;

    buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    printf("debug--:\nrgb_size: %d, size: %d w: %d h: %d row_stride: %d \n", rgb_size,
                cinfo.image_width*cinfo.image_height*3,
                cinfo.image_width, 
                cinfo.image_height,
                row_stride);
    tmp_buffer = rgb_buffer;
    while (cinfo.output_scanline < cinfo.output_height) // ��ѹÿһ��
    {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        // ���Ƶ��ڴ�
        memcpy(tmp_buffer, buffer[0], row_stride);
        tmp_buffer += row_stride;
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    return 0;
}

int jpeg_header(unsigned char* jpeg_buffer, int jpeg_size, int* width, int* height, int* components)
{
    struct jpeg_decompress_struct cinfo;
    struct my_error_mgr jerr;

    //int row_stride = 0;

    if (jpeg_buffer == NULL)
    {
        printf("no jpeg buffer here.\n");
        return -1;
    }

    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;

    if (setjmp(jerr.setjmp_buffer))
    {
        jpeg_destroy_decompress(&cinfo);
        return -1;
    }

    jpeg_create_decompress(&cinfo);

    jpeg_mem_src(&cinfo, jpeg_buffer, jpeg_size);

    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);  // �����ѹ�����ܵõ���ȷֵ

    //row_stride = cinfo.output_width * cinfo.output_components;
    *width = cinfo.output_width;
    *height = cinfo.output_height;
    *components = cinfo.output_components;
    
    return 0;
}

int jpeg2rgb1(unsigned char* jpeg_buffer, int jpeg_size, unsigned char* rgb_buffer, int* size)
{
    struct jpeg_decompress_struct cinfo;
    struct my_error_mgr jerr;

    JSAMPARRAY buffer;
    int row_stride = 0;
    unsigned char* tmp_buffer = NULL;
    int rgb_size;

    if (jpeg_buffer == NULL)
    {
        printf("no jpeg buffer here.\n");
        return -1;
    }
    if (rgb_buffer == NULL)
    {
        printf("you need to alloc rgb buffer.\n");
        return -1;
    }
    
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;

    if (setjmp(jerr.setjmp_buffer))
    {
        jpeg_destroy_decompress(&cinfo);
        return -1;
    }

    jpeg_create_decompress(&cinfo);

    jpeg_mem_src(&cinfo, jpeg_buffer, jpeg_size);

    jpeg_read_header(&cinfo, TRUE);

    //cinfo.out_color_space = JCS_EXT_BGR;//JCS_RGB; //JCS_YCbCr;  // ���������ʽ��Ĭ����JCS_RGB

    jpeg_start_decompress(&cinfo);

    row_stride = cinfo.output_width * cinfo.output_components;

    rgb_size = row_stride * cinfo.output_height; // �ܴ�С
    if (*size < rgb_size)
    {
        printf("rgb buffer to small, we need %d but has only: %d\n", rgb_size, *size);
    }
    
    *size = rgb_size;

    buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    printf("debug--:\nrgb_size: %d, size: %d w: %d h: %d row_stride: %d \n", rgb_size,
                cinfo.image_width*cinfo.image_height*3,
                cinfo.image_width, 
                cinfo.image_height,
                row_stride);
    tmp_buffer = rgb_buffer;
    while (cinfo.output_scanline < cinfo.output_height) // ��ѹÿһ��
    {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        // ���Ƶ��ڴ�
        memcpy(tmp_buffer, buffer[0], row_stride);
        tmp_buffer += row_stride;
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    return 0;
}

int rgb2jpeg(unsigned char* rgb_buffer, int width, int height, int quality, unsigned char** jpeg_buffer, unsigned long* jpeg_size)
{
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    int row_stride = 0;
    JSAMPROW row_pointer[1];

    if (jpeg_buffer == NULL)
    {
        printf("you need a pointer for jpeg buffer.\n");
        return -1;
    }
    
    cinfo.err = jpeg_std_error(&jerr);

    jpeg_create_compress(&cinfo);

    jpeg_mem_dest(&cinfo, jpeg_buffer, jpeg_size);

    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, 1);  // todo 1 == true
    jpeg_start_compress(&cinfo, TRUE);
    row_stride = width * cinfo.input_components;

    while (cinfo.next_scanline < cinfo.image_height)
    {
        row_pointer[0] = &rgb_buffer[cinfo.next_scanline * row_stride];
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);

    return 0;
}
