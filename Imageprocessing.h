//
// Created by Sofian on 17/01/2026.
//
#ifndef IMAGEPROJECT_IMAGEPROCESSING_H
#define IMAGEPROJECT_IMAGEPROCESSING_H
typedef struct
{unsigned char b;
    unsigned char g;
    unsigned char r;
}__attribute__((packed)) Pixel;
//C tends to go for a struct size that is divisible by 4(padding), so we have to use packed in order to maintain the 3 byte size of the struct, ensuring proper reading.
typedef struct
{unsigned short type;
    unsigned int size;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int offset;
}__attribute__((packed)) BMPheader;
//read line 12.(Actual size 14)
typedef struct
{ unsigned int size;
    int width,height;
    unsigned short planes;
    unsigned short bits;
    unsigned int compression;
    unsigned int imagesize;
    int xresolution,yresolution;
    unsigned int ncolors;
    unsigned int importantcolors;
}__attribute__((packed)) BMPinfoheader;
//read line 12.(Here packed wasn't necessary since the struct has a size of 40(a multiple of 4), but helpful for learning)
typedef struct
{BMPinfoheader infoheader;
    BMPheader header;
    Pixel* pixels;
}Image;
Image ProcessImage(const char* filename);
void FreeImage(const Image* image);
void SaveImage(const char* filename,const Image* MyImage);
Image Grayscale(const Image* MyImage);
Image Invertedimage(const Image* MyImage);
Image Rotateimage(const Image* MyImage);
Image Blurimage(const Image* MyImage, int blurfactor);
Image Sobeledges(const Image* MyImage);
#endif //IMAGEPROJECT_IMAGEPROCESSING_H
