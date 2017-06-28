#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include "bmp_utils.h"
//#include "debug.h"

// ע��ֻ���24λͼƬ
int analyse_bmp_file(const char* bmp_file)
{
#if 0
    FILE* fp;
    BITMAPFILEHEADER bmpHeader;
    BITMAPINFOHEADER bmpInfo;
    int rgb_size1 = 0;
    int rgb_size2 = 0;
    int width = 0;
    int height = 0;
    int padding = 0;
    int stride_byte = 0;
    int color_num = 0;
    int paltette_len = 0;

    char* palette = NULL;

    fp = fopen(bmp_file, "rb");
    if (fp == NULL)
    {
        printf("open file %s failed.\n", bmp_file);
        return -1;
    }

    fread(&bmpHeader, 1, sizeof(BITMAPFILEHEADER), fp);
    fread(&bmpInfo, 1, sizeof(BITMAPINFOHEADER), fp);

    if (bmpHeader.bfType != (('M' << 8) | 'B'))
    {
        printf("Sorry, not bmp picture.\n");
        return -1;
    }

    width = bmpInfo.biWidth;
    height = (int)fabs((double)bmpInfo.biHeight);

    switch(bmpInfo.biBitCount) 
    {
    case 1:
        color_num = 2;
        break;
    case 4:
        color_num = 16;
        break;
    case 8:
        color_num = 256;
        break;
    case 24:
    default:
        color_num = 0;
        break;
    }

    stride_byte = ALIGN(width*bmpInfo.biBitCount/8, 4);
    padding = stride_byte - width*bmpInfo.biBitCount/8;
    paltette_len = color_num * sizeof(RGBQUAD);

    rgb_size1 = bmpHeader.bfSize - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER) - paltette_len;
    rgb_size2 = stride_byte*height;
    // ��ӡ�ṹ����ÿ����Ա
    printf("file name: %s\n", bmp_file);
    printf("file type: %c%c %x\n", (bmpHeader.bfType)>>8, (bmpHeader.bfType)&0xff, bmpHeader.bfType);
    printf("file size: %d(B) = %0.2f(KB) = %0.2f(MB)\n", bmpHeader.bfSize, (float)bmpHeader.bfSize/1024.00, (float)bmpHeader.bfSize/1024.00/1024.00);
    printf("offset of image data: %d\n", bmpHeader.bfOffBits);
    //////////////////////////////////

    printf("biSize: %d\n", bmpInfo.biSize);
    printf("width: %d\n", bmpInfo.biWidth);
    printf("height: %d\n", bmpInfo.biHeight);
    printf("Plane: %d\n", bmpInfo.biPlanes);
    printf("BitCount: %d\n", bmpInfo.biBitCount);
    printf("biCompression: %d\n", bmpInfo.biCompression);
    printf("biSizeImage: %d\n", bmpInfo.biSizeImage);
    printf("XPelsPerMeter: %d\n", bmpInfo.biXPelsPerMeter);
    printf("YPelsPerMeter: %d\n", bmpInfo.biYPelsPerMeter);
    printf("biClrUsed: %d\n", bmpInfo.biClrUsed);
    printf("biClrImportant: %d\n", bmpInfo.biClrImportant);

    printf("width*3: %d stride byte: %d padding: %d\n", width*3, stride_byte, padding);

    printf("rgb buffer size: %d %d\n", rgb_size1,rgb_size2);

    if (color_num != 0)
    {
        palette = (char *)malloc(paltette_len * sizeof(char));
        fread(palette, paltette_len, 1, fp);
        printf("palette:\n");
        //dump(palette, paltette_len);
    }
#endif
    return 0;
}

int read_bmp_file(const char* bmp_file, unsigned char** rgb_buffer,
                  int* size, int* width, int* height)
{
    int ret = 0;
    FILE* fp = NULL;
    BITMAPFILEHEADER bmpHeader;
    BITMAPINFOHEADER bmpInfo;
    int tmp_width = 0;
    int tmp_height = 0;
    int rgb_size = 0;
    int stride_byte = 0; // ÿ��ռ���ֽ���(4�ֽڶ���)
    int width_byte = 0;  // ÿ��������Ч�ֽ���
    int padding = 0;    // ��Ҫ������ֽ���
    unsigned char* tmp_buf = 0;
    int color_num = 0;
    int palette_len = 0;
    int i = 0;

    fp = fopen(bmp_file, "rb");
    if (fp == NULL)
    {
        printf("open file %s failed.\n", bmp_file);
        return -1;
    }

    ret = fread(&bmpHeader, 1, sizeof(BITMAPFILEHEADER), fp);
    if (ret != sizeof(BITMAPFILEHEADER))
    {
        printf("read BITMAPFILEHEADER failed.\n");
        ret = -1;
        goto end;
    }

    ret = fread(&bmpInfo, 1, sizeof(BITMAPINFOHEADER), fp);
    if (ret != sizeof(BITMAPINFOHEADER))
    {
        printf("read BITMAPINFOHEADER failed read: %d %d.\n", ret, sizeof(BITMAPINFOHEADER));
        ret = -1;
        goto end;
    }

    if (bmpHeader.bfType != (('M' << 8) | 'B'))
    {
        printf("Sorry, not bmp picture.\n");
        ret = -1;
        goto end;
    }
    tmp_width = bmpInfo.biWidth;
    tmp_height = (int)fabs((double)bmpInfo.biHeight);   // Ԥ����Ϊ���������

    // ����RGB���ݴ�С
    rgb_size = tmp_width * tmp_height * bmpInfo.biBitCount/8;

    *width = tmp_width;
    *height = tmp_height;
    *size = rgb_size;
    /**
     * ÿ��ռ���ֽ���������ʽ�����ͬ
     * stride_byte = (width * bmpInfo.biBitCount/8+3)/4*4;
     */
    stride_byte = ALIGN(tmp_width*bmpInfo.biBitCount/8, 4);
    width_byte = tmp_width * bmpInfo.biBitCount/8;

    /**
     * �����ֽڣ�����ʽ�����ͬ
     * padding = (4 - width * 3 % 4) % 4;
     * ʵ��δʹ��
     */
    padding = stride_byte - width_byte;

    // �жϵ�ɫ��
    switch(bmpInfo.biBitCount) 
    {
    case 1:
        color_num = 2;
        break;
    case 4:
        color_num = 16;
        break;
    case 8:
        color_num = 256;
        break;
    case 24:
    default:
        color_num = 0;
        break;
    }
    // todo����ȡ��ɫ��
    palette_len = color_num * sizeof (RGBQUAD);

    // ����ƫ������ʵ��ƫ�����Ƚϣ��粻�ȣ���ɫ������
    if (bmpHeader.bfOffBits != sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + palette_len)
    {
        ret = -1;
        goto end;
    }

    printf("debug--:\nfile size: %d rgb size: %d %d stride byte: %d res: %dx%d padding: %d BitCount: %d\n", 
        (int)bmpHeader.bfSize, rgb_size, stride_byte*tmp_height, stride_byte, (int)bmpInfo.biWidth, (int)bmpInfo.biHeight, padding, bmpInfo.biBitCount);

    if (color_num != 0)
    {
        // ����ͼ�����ݴ�
        fseek(fp, palette_len, SEEK_CUR);
    }

    // ������ʵ��ڴ�
    *rgb_buffer = (unsigned char *)malloc(sizeof(char) * rgb_size);
    if (*rgb_buffer == NULL)
    {
        ret = -1;
        goto end;
    }
    
    // ���ߣ�����
    if (bmpInfo.biHeight > 0)
    {
        // ����ȡ�����ݵ��Ŵ�ŵ�������(��BMPͼ���һ�����ݷŵ����������һ�У��ȵ�)��
        // ����ͼ����������ģ�����ͼ���ǵ�����
        tmp_buf = *rgb_buffer + rgb_size;
        for (i = 0; i < tmp_height; i++)
        {
            tmp_buf -= width_byte;
            ret = fread(tmp_buf, 1, width_byte, fp);
            if (ret != width_byte)
            {
                free(*rgb_buffer);
                ret = -1;
                goto end;
            }
            fseek(fp, padding, SEEK_CUR);
        }
    }
    else // ���ߣ�˳���
    {
        unsigned char* tmp_buf = *rgb_buffer;
        for (i = 0; i < tmp_height; i++)
        {
            ret = fread(tmp_buf, 1, width_byte, fp);
            if (ret != width_byte)
            {
                free(*rgb_buffer);
                ret = -1;
                goto end;
            }
            fseek(fp, padding, SEEK_CUR);
            tmp_buf += width_byte;
        }
    }

end:
    fclose(fp);
    return ret;
}

int write_bmp_file(const char* bmp_file, unsigned char* rgb_buffer, int width, int height)
{
#define BPP 24  // Ŀǰֻ����24ɫλͼ

    BITMAPFILEHEADER bmpHeader;
    BITMAPINFOHEADER bmpInfo;
    FILE* fp = NULL;
    int offset = 0;
    int stride_byte = 0;    // ÿ��ռ���ֽ���(4�ֽڶ���)
    int width_byte = 0;     // ÿ��������Ч�ֽ���
    int rgb_size = 0;
    int padding = 0;
    unsigned char* tmp_buf = NULL;
    int i = 0;
    int ret = 0;

    fp = fopen(bmp_file, "wb");
    if (fp == NULL)
    {
        printf("open %s failed\n", bmp_file);
        return -1;
    }

    offset = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);  //54�ֽ�
    // 4�ֽڶ��� ((width * 24 + 31) / 32) * 4
    // ���Ѿ����룬��rowStride��ʵ�ʿ�һ�£��粻����rowStride��ȿ��һЩ
    // stride_byte = ((width * 24 + 31) >> 5) << 2;
    stride_byte = ALIGN(width*BPP/8, 4);
    width_byte = width*BPP/8;
    rgb_size = stride_byte * (int)fabs((double)height);  // �ѿ��Ƕ��� ���ҷ�ֹ���ָ���

    bmpHeader.bfType = ('M' << 8) | 'B';
    bmpHeader.bfSize = offset + rgb_size;    // BMP�ļ��ܴ�С
    bmpHeader.bfReserved1 = 0;
    bmpHeader.bfReserved2 = 0;
    bmpHeader.bfOffBits = offset;

    bmpInfo.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.biWidth  = width;
    bmpInfo.biHeight = height;
    bmpInfo.biPlanes = 1;
    bmpInfo.biBitCount = BPP;
    bmpInfo.biCompression = 0;
    bmpInfo.biSizeImage = rgb_size;
    bmpInfo.biXPelsPerMeter = 0;
    bmpInfo.biYPelsPerMeter = 0;
    bmpInfo.biClrUsed = 0;
    bmpInfo.biClrImportant = 0;

    // ��Ҫ����ֽڣ�BMPҪ��ÿһ�����ݱ���4�ֽڶ��룬������0����
    //padding = (4 - width * 3 % 4) % 4;
    // ʵ��δʹ�õ�
    padding = stride_byte - width_byte;

    printf("debug--:\nwidth: %d height: %d padding: %d rgb_size: %d, stride_byte: %d\n",
                width, height, padding, rgb_size, stride_byte);
    tmp_buf = (unsigned char *)malloc(sizeof(char) * rgb_size);
    if (tmp_buf == NULL)
    {
        ret = -1;
        goto end;
    }
    memset(tmp_buf, '\0', sizeof(char) * rgb_size);
    
    
    // TOCHECK:��Ȩ��˵��ȷ�ϡ�����
    if (height > 0)
    {
        // ���ſ�����������
        for (i = 0; i < height; i++)
        {
            // ÿһ�е�ʵ������Ϊwidth * 3(R��G��B)
            memcpy(tmp_buf + i * stride_byte, rgb_buffer + (height - i - 1) * width_byte, width_byte);
        }
    }
    else
    {
        for (i = 0; i < -height; i++)
        {
            memcpy(tmp_buf + i * stride_byte, rgb_buffer + i * width_byte, width_byte);
            //memcpy(tmp_buf + i * stride_byte, rgb_buffer + (-height - i - 1) * width_byte, width_byte);
        }
    }

    fwrite(&bmpHeader, 1, sizeof(BITMAPFILEHEADER), fp);
    fwrite(&bmpInfo, 1, sizeof(BITMAPINFOHEADER), fp);
    fwrite(tmp_buf, 1, rgb_size, fp);

end:
    if (tmp_buf != NULL)
    {
        free(tmp_buf);
    }
    fclose(fp);

    return ret;
}

// rgb --> bgr or
// bgr --> rgb
void swap_rgb(unsigned char* rgb_buffer, int len)
{
    int i = 0;
    for (i = 0; i < len; i += 3)
    {
        unsigned char tmp;
        tmp = rgb_buffer[i];
        rgb_buffer[i] = rgb_buffer[i + 2];
        rgb_buffer[i + 1] = rgb_buffer[i + 1];
        rgb_buffer[i + 2] = tmp;
    }
}