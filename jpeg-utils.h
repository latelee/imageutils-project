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
 * 利用libjpeg读取JPEG图片文件
 * 
 * @param[IN]   jpeg_file    JPEG图片文件名称
 * 
 * @param[OUT]  rgb_buffer JPEG图片RGB数据
 * @param[OUT]  size       RGB数据大小
 * @param[OUT]  width      图片宽
 * @param[OUT]  height     图片高
 *
 * @return 
 *         0：成功
 *         -1：读取文件失败，或申请内存失败
 * @note
 *         rgb_buffer为二级指针，内存由该函数分配，需要自行释放
 */
int read_jpeg_file(const char* jpeg_file, unsigned char** rgb_buffer, int* size, int* width, int* height);

// 解jpeg
int jpeg2rgb(unsigned char* jpeg_buffer, int jpeg_size, int width, int height, unsigned char* rgb_buffer, int rgb_size);

/**
 * 利用libjpeg转换为JPEG图片并保存
 * 
 * @param[IN]  jpeg_fle    JPEG图片文件名称
 * 
 * @param[IN]  rgb_buffer JPEG图片RGB数据
 * @param[IN]  width      图片宽
 * @param[IN]  height     图片高
 * @param[IN]  quality    图片质量
 *
 * @return 
 *         0：成功
 *         -1：打开文件失败
 * @note
 *         图片质量未实际测试
 */
int write_jpeg_file(const char* jpeg_file, unsigned char* rgb_buffer, int width, int height, int quality);

// 压jpeg
int rgb2jpeg(unsigned char* rgb_buffer, int width, int height,  unsigned char* jpeg_buffer, int jpeg_size);

#ifdef __cplusplus
};
#endif

#endif /* _JPEG_UTILS_H */