/**
 * @file   jpeg-utils.h
 * @author Late Lee 
 * @date   2015-6-28 22:21:56
 * @brief  
 *         本文件接口函数只考虑RGB 24bit情况，即3分量，RGB32未考虑
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

/**
 * 利用libjpeg转换为JPEG图片并保存
 * 
 * @param[IN]  jpeg_fle    JPEG图片文件名称
 * @param[IN]  rgb_buffer JPEG图片RGB数据
 * @param[IN]  width      图片宽
 * @param[IN]  height     图片高
 * @param[IN]  quality    图片质量
 *
 * @return 
 *         0：成功
 *         -1：打开文件失败
 * @note
 *         图片质量可看到有效果
 */
int write_jpeg_file(const char* jpeg_file, unsigned char* rgb_buffer, int width, int height, int quality);

/**
 * 利用libjpeg将缓冲区的JPEG转换成RGB 解压JPEG
 * 
 * @param[IN]  jpeg_buffer  JPEG图片缓冲区
 * @param[IN]  jpeg_size    JPEG图片缓冲区大小
 * @param[IN] rgb_buffer    RGB缓冲区
 * @param[IN/OUT] size      RGB缓冲区大小
 * @param[OUT] width        图片宽
 * @param[OUT] height       图片高
 *
 * @return 
 *         0：成功
 *         -1：打开文件失败
 * @note
 *         jpeg、rgb空间由调用者申请，size为输入输出参数，传入为rgb空间大小，转出为rgb实际大小
 */
int jpeg2rgb(unsigned char* jpeg_buffer, int jpeg_size, unsigned char* rgb_buffer, int* size, int* width, int* height);

/**
 * 利用libjpeg将缓冲区的RGB转换成JPEG 压缩为JPEG
 * 
 * @param[IN]  rgb_buffer JPEG图片RGB数据
 * @param[IN]  width       图片宽
 * @param[IN]  height      图片高
 * @param[IN]  quality     图片质量
 * @param[OUT] jpeg_buffer JPEG缓冲区指针
 * @param[OUT] jpeg_size   JPEG缓冲区大小
 *
 * @return 
 *         0：成功
 *         -1：打开文件失败
 * @note
 *         jpeg_buffer为二级指针，无须调用者申请空间，由libjpeg申请，但调用者要自行释放
 */
int rgb2jpeg(unsigned char* rgb_buffer, int width, int height, int quality, unsigned char** jpeg_buffer, unsigned long* jpeg_size);

#ifdef __cplusplus
};
#endif

#endif /* _JPEG_UTILS_H */