// 测试代码，具体图片文件名称自行定义
#include <stdio.h>
#include "bmp_utils.h"
#include "jpeg_bmp.h"

int main(int argc, char* argv[])
{

    char src_pic[32] = {0};
    char jpg_pic[32] = {0};
    char bmp_pic[32] = {0};
    int width = 0;

    width = 1920;

    sprintf(src_pic, "test_%d.bmp", width);
    sprintf(jpg_pic, "test_%d.jpg", width);
    sprintf(bmp_pic, "test_a_%d.bmp", width);

    #if 0
    bmp_to_jpg1(src_pic, jpg_pic);
    sprintf(jpg_pic, "test_%d_t.jpg", width);
    bmp_to_jpg2(src_pic, jpg_pic);
    #endif
    //jpg_to_bmp1(jpg_pic, bmp_pic);

    jpg_to_bmp2(jpg_pic, bmp_pic);
    jpg_to_bmp3(jpg_pic, bmp_pic);
    
    
    return 0;
}