/**
 * @file   jpeg_bmp.h
 * @author Late Lee 
 * @date   2012-7-1 12:47:56
 * @brief  
 *
 */

#ifndef _JPEG_BMP_H
#define _JPEG_BMP_H

#ifdef __cplusplus
extern "C" {
#endif

// ²âÊÔº¯Êý£ºJPEG-->BMP
int jpg_to_bmp(const char* jpg_file, const char* bmp_file);
int jpg_to_bmp1(const char* jpg_file, const char* bmp_file);
int jpg_to_bmp2(const char* jpg_file, const char* bmp_file);

// ²âÊÔº¯Êý£ºBMP-->JPEG
int bmp_to_jpg(const char* bmp_file, const char* jpg_file);

int bmp_to_jpg1(const char* bmp_file, const char* jpg_file);

#ifdef __cplusplus
};
#endif

#endif /* _JPEG_BMP_H */