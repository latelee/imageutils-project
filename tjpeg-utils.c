#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

#include "jpeg-utils.h"

// jpeg库头文件必须放到stdio.h后面
#include "libjpeg/include/turbojpeg.h"


int tjpeg_header(unsigned char* jpeg_buffer, int jpeg_size, int* width, int* height, int* subsample, int* colorspace)
{
    tjhandle handle = NULL;

    handle = tjInitDecompress();
    tjDecompressHeader3(handle, jpeg_buffer, jpeg_size, width, height, subsample, colorspace);

    tjDestroy(handle);

    return 0;
}

int tjpeg2rgb(unsigned char* jpeg_buffer, int jpeg_size, unsigned char* rgb_buffer, int* size)
{
    tjhandle handle = NULL;
    int width, height, subsample, colorspace;
    int flags = 0;
    int pixelfmt = TJPF_RGB;

    handle = tjInitDecompress();
    tjDecompressHeader3(handle, jpeg_buffer, jpeg_size, &width, &height, &subsample, &colorspace);

    flags |= 0;
    tjDecompress2(handle, jpeg_buffer, jpeg_size, rgb_buffer, width, 0,
            height, pixelfmt, flags);

    tjDestroy(handle);

    return 0;
}

int trgb2jpeg(unsigned char* rgb_buffer, int width, int height, int quality, unsigned char** jpeg_buffer, unsigned long* jpeg_size)
{
    tjhandle handle = NULL;
    //unsigned long size=0;
    int flags = 0;
    int subsamp = TJSAMP_422;
    int pixelfmt = TJPF_RGB;

    handle=tjInitCompress();
    //size=tjBufSize(width, height, subsamp);
    tjCompress2(handle, rgb_buffer, width, 0, height, pixelfmt, jpeg_buffer, jpeg_size, subsamp,
            quality, flags);

    tjDestroy(handle);

    return 0;
}

