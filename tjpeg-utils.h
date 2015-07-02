/**
 * @file   tjpeg-utils.h
 * @author Late Lee 
 * @date   2015-07-01 22:17:56
 * @brief  
 *         本文件接口函数只考虑RGB 24bit情况，即3分量，RGB32未考虑
 */

#ifndef _TJPEG_UTILS_H
#define _TJPEG_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

int tjpeg_header(unsigned char* jpeg_buffer, int jpeg_size, int* width, int* height, int* subsample, int* colorspace);

int tjpeg2rgb(unsigned char* jpeg_buffer, int jpeg_size, unsigned char* rgb_buffer, int* size);

int trgb2jpeg(unsigned char* rgb_buffer, int width, int height, int quality, unsigned char** jpeg_buffer, unsigned long* jpeg_size);

#ifdef __cplusplus
};
#endif

#endif /* _TJPEG_UTILS_H */