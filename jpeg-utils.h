/**
 * @file   jpeg-utils.h
 * @author Late Lee 
 * @date   2015-6-28 22:21:56
 * @brief  
 *
 */

#ifndef _JPEG_UTILS_H
#define _JPEG_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * ����libjpeg��ȡJPEGͼƬ�ļ�
 * 
 * @param[IN]   jpeg_file    JPEGͼƬ�ļ�����
 * 
 * @param[OUT]  rgb_buffer JPEGͼƬRGB����
 * @param[OUT]  size       RGB���ݴ�С
 * @param[OUT]  width      ͼƬ��
 * @param[OUT]  height     ͼƬ��
 *
 * @return 
 *         0���ɹ�
 *         -1����ȡ�ļ�ʧ�ܣ��������ڴ�ʧ��
 * @note
 *         rgb_bufferΪ����ָ�룬�ڴ��ɸú������䣬��Ҫ�����ͷ�
 */
int read_jpeg_file(const char* jpeg_file, unsigned char** rgb_buffer, int* size, int* width, int* height);

// ��jpeg
int jpeg2rgb(unsigned char* jpeg_buffer, int jpeg_size, int width, int height, unsigned char* rgb_buffer, int rgb_size);

/**
 * ����libjpegת��ΪJPEGͼƬ������
 * 
 * @param[IN]  jpeg_fle    JPEGͼƬ�ļ�����
 * 
 * @param[IN]  rgb_buffer JPEGͼƬRGB����
 * @param[IN]  width      ͼƬ��
 * @param[IN]  height     ͼƬ��
 * @param[IN]  quality    ͼƬ����
 *
 * @return 
 *         0���ɹ�
 *         -1�����ļ�ʧ��
 * @note
 *         ͼƬ����δʵ�ʲ���
 */
int write_jpeg_file(const char* jpeg_file, unsigned char* rgb_buffer, int width, int height, int quality);

// ѹjpeg
int rgb2jpeg(unsigned char* rgb_buffer, int width, int height,  unsigned char* jpeg_buffer, int jpeg_size);

#ifdef __cplusplus
};
#endif

#endif /* _JPEG_UTILS_H */