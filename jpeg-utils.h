/**
 * @file   jpeg-utils.h
 * @author Late Lee 
 * @date   2015-6-28 22:21:56
 * @brief  
 *         ���ļ��ӿں���ֻ����RGB 24bit�������3������RGB32δ����
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

/**
 * ����libjpegת��ΪJPEGͼƬ������
 * 
 * @param[IN]  jpeg_fle    JPEGͼƬ�ļ�����
 * @param[IN]  rgb_buffer JPEGͼƬRGB����
 * @param[IN]  width      ͼƬ��
 * @param[IN]  height     ͼƬ��
 * @param[IN]  quality    ͼƬ����
 *
 * @return 
 *         0���ɹ�
 *         -1�����ļ�ʧ��
 * @note
 *         ͼƬ�����ɿ�����Ч��
 */
int write_jpeg_file(const char* jpeg_file, unsigned char* rgb_buffer, int width, int height, int quality);

/**
 * ����libjpeg����������JPEGת����RGB ��ѹJPEG
 * 
 * @param[IN]  jpeg_buffer  JPEGͼƬ������
 * @param[IN]  jpeg_size    JPEGͼƬ��������С
 * @param[IN] rgb_buffer    RGB������
 * @param[IN/OUT] size      RGB��������С
 * @param[OUT] width        ͼƬ��
 * @param[OUT] height       ͼƬ��
 *
 * @return 
 *         0���ɹ�
 *         -1�����ļ�ʧ��
 * @note
 *         jpeg��rgb�ռ��ɵ��������룬sizeΪ�����������������Ϊrgb�ռ��С��ת��Ϊrgbʵ�ʴ�С
 */
int jpeg2rgb(unsigned char* jpeg_buffer, int jpeg_size, unsigned char* rgb_buffer, int* size, int* width, int* height);

/**
 * ����libjpeg����������RGBת����JPEG ѹ��ΪJPEG
 * 
 * @param[IN]  rgb_buffer JPEGͼƬRGB����
 * @param[IN]  width       ͼƬ��
 * @param[IN]  height      ͼƬ��
 * @param[IN]  quality     ͼƬ����
 * @param[OUT] jpeg_buffer JPEG������ָ��
 * @param[OUT] jpeg_size   JPEG��������С
 *
 * @return 
 *         0���ɹ�
 *         -1�����ļ�ʧ��
 * @note
 *         jpeg_bufferΪ����ָ�룬�������������ռ䣬��libjpeg���룬��������Ҫ�����ͷ�
 */
int rgb2jpeg(unsigned char* rgb_buffer, int width, int height, int quality, unsigned char** jpeg_buffer, unsigned long* jpeg_size);

#ifdef __cplusplus
};
#endif

#endif /* _JPEG_UTILS_H */