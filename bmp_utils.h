/**
 * @file   bmp_utils.h
 * @author Late Lee 
 * @date   2012-7-2 13:21:53
 * @brief  
 *          BMP��ع��ߺ�����Ŀǰֻ���24λͼƬ����
 *
 *        1����VS003��GCC�±������ͨ����
 *        2�������BMPͼƬ������ƫɫ����б�����⡣
 *        3��BMPͼ��ÿ��������Ҫ4�ֽڶ��룬��һ�����ݲ���4�ı�������0����
 *           ��������ⷽ��������2��������
 *           width_byte��ʵ�ʵ�RGBÿһ�е��ֽ���
 *           stride_byte��4�ֽڶ����ÿһ�е��ֽ���(�Ѷ���ʱ�������)
 *           ����ʱ�����⿪��һ��������4�ֽڶ���Ļ�������ÿ����һ������(width_byte)��
 *           ����stride_byte���ֽڣ�������4�ֽڶ������һ�С�
 *           ��ȡʱ��ֻ��width_byte����������ÿ����󲹵�0��
 *        4��ͼ��������ȡ�뱣��BMPʱ�������ݵ�����:
 *           ��ȡʱ���������������������ϴ�ŵ�������
 *           ����ʱ���������������Ͽ�����������
 *        5��ƫɫ��BMP����ΪBGR����RGB���ݵ�G��B����λ�ü��ɡ�
 *        6����б����ȡBMPʱ��δ���������0��
 *
 *       �ʼǣ�
            BMPͼƬ�ṹ�����е�1����2����ռ54�ֽڣ����ɫͼû�е�������
              _______________________________
             |        BITMAPFILEHEADER       |
             |_______________________________|
             |        BITMAPINFOHEADER       |
             |_______________________________|
             |          n * RGBQUAD          |
             |_______________________________|
             |          image  data          |
             |_______________________________|

 *
        ����2ɫλͼ����1λ��ʾ�����ص���ɫ(һ��0��ʾ�ڣ�1��ʾ��)��һ���ֽڿ��Ա�ʾ8�����ء���ɫ�壺2*4=8
        ����16ɫλͼ����4λ��ʾһ�����ص���ɫ����һ���ֽڿ��Ա�ʾ2�����ء���ɫ�壺16*4=64
        ����256ɫλͼ��һ���ֽڱ�ʾ1�����ء���ɫ�壺256*4=1024
        �������ɫͼ�������ֽڱ�ʾ1�����ء��޵�ɫ��

 *      ��ɫBMPͼ����ɫ��ռ8�ֽڣ���ͷ��ռ��54+8=62�ֽڣ�����Ϊ�����ֽڣ�
        ע��ÿ���ֽ���Ҫ4�ֽڶ��룬
        ������16*16���ص�ɫλͼ��һ��ռ16/8 = 2�ֽڣ���Ҫ��2�ֽڡ�
        ʵ�������ֽڣ�16*16/2 = 32�ֽڣ������ֽڣ�2*16 = 32����64�ֽ�
        ͷ����62�ֽڣ��ʸ�ͼƬ�ܴ�СΪ64+62=126�ֽ�
 */

#ifndef _BMP_UTILS_H
#define _BMP_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
#include <Windows.h>
#else
typedef unsigned char   BYTE;
typedef unsigned short  WORD;
typedef unsigned long   DWORD;
typedef long            LONG;

#pragma pack(push)
// 2�ֽڶ��룬��14
#pragma pack(2)
typedef struct tagBITMAPFILEHEADER {
    WORD    bfType;             // �ļ�����, 0x4d42
    DWORD   bfSize;             // �ļ��ܴ�С
    WORD    bfReserved1;
    WORD    bfReserved2;
    DWORD   bfOffBits;          // ʵ��λͼ����ƫ��
} BITMAPFILEHEADER; //__attribute__ ((packed));

// 40
typedef struct tagBITMAPINFOHEADER{
    DWORD      biSize;          // ���ṹ�峤��
    LONG       biWidth;         // ��(��λ����)
    LONG       biHeight;        // ��(��λ����) ������ʾͼ���ǵ�����
    WORD       biPlanes;        // Ϊ1
    WORD       biBitCount;      // ����ռ��λ�� 1(2^1=2�ڰ׶�ɫ)�� 4(2^4=16ɫ)��8(2^8=256ɫ)��24(���ɫ)��32
    DWORD      biCompression;   // ѹ�����ͣ���ѹ����BI_RGB(0)
    DWORD      biSizeImage;     // λͼ���ݴ�С������ǲ�ѹ�����ͣ�����Ϊ0
    LONG       biXPelsPerMeter; // ˮƽ�ֱ���,��λ��ÿ�׵����ظ���
    LONG       biYPelsPerMeter; // ��ֱ�ֱ���
    DWORD      biClrUsed;       // λͼʵ��ʹ�õ���ɫ���е���ɫ��
    DWORD      biClrImportant;  // λͼ��ʾ��������Ҫ����ɫ��
} BITMAPINFOHEADER; //__attribute__ ((aligned(2)));

typedef struct tagRGBQUAD {
    BYTE    rgbBlue;
    BYTE    rgbGreen;
    BYTE    rgbRed;
    BYTE    rgbReserved;
} RGBQUAD;

typedef struct tagBITMAPINFO{
    BITMAPINFOHEADER    bmiHeader;
    RGBQUAD             bmiColors[1];
} BITMAPINFO;   // __attribute__ ((aligned(2)));

#pragma pack(pop)

#endif

#undef  ALIGN
#define ALIGN(x, n) (((x)+(n)-1)&~((n)-1))

/**
 * RGB����R��B˳��
 * 
 * @param[IN]  rgb_buffer RGB������
 * @param[IN]  len        ��������С
 * 
 * @return none
 *
 * @note
 *        ���������ݿ�����RGB��Ҳ������BGR���ú���ֻ�ǽ�B��G���л���
 */
void swap_rgb(unsigned char* rgb_buffer, int len);

/**
 * ����BMP�ļ�ͷ��
 * 
 * @param[IN]  bmp_file  BMPͼƬ�ļ�����
 * 
 * @return 
 *         0:  �ɹ�
 *         -1: �ļ������ڻ���BMP�ļ�
 */
int analyse_bmp_file(const char* bmp_file);

/**
 * ��ȡBMPͼƬ�ļ�
 * 
 * @param[IN]   bmp_file    BMPͼƬ�ļ�����
 * 
 * @param[OUT]  rgb_buffer RGB����(ʵ��ΪBGR)
 * @param[OUT]  size       RGB���ݴ�С
 * @param[OUT]  width      ͼƬ��
 * @param[OUT]  height     ͼƬ��
 *
 * @return 
 *         0���ɹ�
 *         -1����ȡ�ļ�ʧ�ܣ�����BMP�ļ����������ڴ�ʧ��
 * @note
 *         rgb_bufferΪ����ָ�룬�ڴ��ɸú������䣬��Ҫ�����ͷ�
 *         rgb_buffer��������˳��ΪBGR����ˣ�����ʱ������Ҫת����RGB˳��
 */
int read_bmp_file(const char* bmp_file, unsigned char** rgb_buffer,
                  int* size, int* width, int* height);

int read_bmp_file_1(const char* bmp_file, unsigned char** rgb_buffer, int* rgb_size,
                    unsigned char** palette_buf, int* palette_len,
                    int* width, int* height);
/**
 * ����BMP�ļ�
 * 
 * @param[IN]  bmp_file   BMPͼƬ�ļ�����
 * 
 * @param[IN]  rgb_buffer RGB����(ʵ��ΪBGR)
 * @param[IN]  width      ͼƬ��
 * @param[IN]  height     ͼƬ��
 *
 * @return 
 *         0���ɹ�
 *         -1�����ļ�ʧ��
 * @note
 *         BMPͼƬ��ɫ����ʵ��ΪBGR����ˣ���Ҫ���Ƚ�rgb_buffer��������˳��ת����BGR��
 */
int write_bmp_file(const char* bmp_file, unsigned char* rgb_buffer, int width, int height);

int write_bmp_file_1(const char* bmp_file, unsigned char* rgb_buffer,
                     unsigned char* palette_buf, int* palette_len,
                     int width, int height);
#ifdef __cplusplus
};
#endif

#endif /* _BMP_UTILS_H */