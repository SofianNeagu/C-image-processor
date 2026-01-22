//
// Created by Sofian on 17/01/2026.
//

#include "Imageprocessing.h"
#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <errno.h>
#include <math.h>
Image ProcessImage(const char* filename)
{
 FILE *pb=fopen(filename,"rb");
 if (pb==NULL)
 {
  printf("Error opening file");
  exit(1);
 }
 Image MyImage;
 fread(&MyImage.header,sizeof(BMPheader),1,pb);
 fread(&MyImage.infoheader,sizeof(BMPinfoheader),1,pb);
 if (MyImage.header.type!=19778)
 {
  printf("File is not a bitmap");
  exit(2);
 }
 int width=MyImage.infoheader.width,height=MyImage.infoheader.height;
 int pixelcount=width*height;
 MyImage.pixels=malloc(pixelcount*sizeof(Pixel));
 if (MyImage.pixels==NULL)
 {
  printf("Error allocating memory");
  exit(3);
 }
 fseek(pb,MyImage.header.offset,SEEK_SET);
fread(MyImage.pixels,sizeof(Pixel),pixelcount,pb);
 fclose(pb);
 return MyImage;
}
void FreeImage(const Image *MyImage)
{
 free(MyImage->pixels);
}
void SaveImage(const char* filename,const Image* MyImage)
{ int check=_mkdir("OutputImages");
 if (check==-1 && errno!=EEXIST) //errno!=EEXIST verifies existence of directory
 {
  printf("Error creating output directory");
  exit(4);
 } //This is the ex Outputdirectory function
 char pathname[250];
 sprintf(pathname,"OutputImages/%s",filename);
 FILE *po=fopen(pathname,"wb");
 if (po==NULL)
 {
  printf("Error opening file");
  exit(5);
 }
 int pixelcount=MyImage->infoheader.width*MyImage->infoheader.height;
 fwrite(&MyImage->header,sizeof(BMPheader),1,po);
 fwrite(&MyImage->infoheader,sizeof(BMPinfoheader),1,po);
 fwrite(MyImage->pixels,sizeof(Pixel),pixelcount,po);
 fclose(po);
}
Image Grayscale(const Image* MyImage)
{ Image GrayscaleImage;
 GrayscaleImage.header=MyImage->header;
 GrayscaleImage.infoheader=MyImage->infoheader;
 int pixelcount=MyImage->infoheader.width*MyImage->infoheader.height;
 GrayscaleImage.pixels=malloc(pixelcount*sizeof(Pixel));
 if (GrayscaleImage.pixels==NULL)
 {
  printf("Error allocating memory");
  exit(6);
 }
 for (int i=0;i<pixelcount;i++)
 {
  unsigned char gray=(unsigned char)(0.114 * MyImage->pixels[i].b + 0.587* MyImage->pixels[i].g + 0.299 * MyImage->pixels[i].r) ;
  GrayscaleImage.pixels[i].b=gray;
  GrayscaleImage.pixels[i].g=gray;
  GrayscaleImage.pixels[i].r=gray;
 }
 return GrayscaleImage;
}
Image Invertedimage(const Image* MyImage)
{ Image InvImage;
 InvImage.header=MyImage->header;
 InvImage.infoheader=MyImage->infoheader;
 int pixelcount=MyImage->infoheader.width*MyImage->infoheader.height;
 InvImage.pixels=malloc(pixelcount*sizeof(Pixel));
 if (InvImage.pixels==NULL)
 {
  printf("Error allocating memory");
  exit(6);
 }
 for (int i=0;i<pixelcount;i++)
 {
  InvImage.pixels[i].b=255-MyImage->pixels[i].b;
  InvImage.pixels[i].g=255-MyImage->pixels[i].g;
  InvImage.pixels[i].r=255-MyImage->pixels[i].r;
 }
 return InvImage;
}
Image Rotateimage(const Image* MyImage)
{
 Image RotatedImage;
 RotatedImage.header=MyImage->header;
 RotatedImage.infoheader=MyImage->infoheader;
 int t=RotatedImage.infoheader.height;
 RotatedImage.infoheader.height=RotatedImage.infoheader.width;
 RotatedImage.infoheader.width=t;
 int myheight=MyImage->infoheader.height;
 int mywidth=MyImage->infoheader.width;
 int pixelcount=myheight*mywidth;
 RotatedImage.pixels=malloc(pixelcount*sizeof(Pixel));
 if (RotatedImage.pixels==NULL)
 {
  printf("Error allocating memory");
  exit(7);
 }
 for (int i=0;i<myheight;i++)
 {
  for (int j=0;j<mywidth;j++)
  { int originalposition=(i*mywidth)+j;
    int newposition=(j*RotatedImage.infoheader.width)+(myheight-i-1);
   RotatedImage.pixels[newposition]=MyImage->pixels[originalposition];
  }
 }
 return RotatedImage;
}
Image Blurimage(const Image* MyImage, int blurfactor)
{
 Image BlurImage;
 BlurImage.header=MyImage->header;
 BlurImage.infoheader=MyImage->infoheader;
 int width=BlurImage.infoheader.width;
 int height=BlurImage.infoheader.height;
 int pixelcount=width*height;
 BlurImage.pixels=malloc(pixelcount*sizeof(Pixel));
 if (BlurImage.pixels==NULL)
 {
  printf("Error allocating memory");
  exit(8);
 }
 for (int i=0;i<height;i++)
 {
  for (int j=0;j<width;j++)
  { int bneighbors=0,gneighbors=0,rneighbors=0,count=0;
   int position=(i*width)+j;
   for (int di=-blurfactor;di<=blurfactor;di++) //By messing around with these values we can change the blur's intensity!
   {for (int dj=-blurfactor;dj<=blurfactor;dj++)
   {
    int neighbory=i+di;
    int neighborx=j+dj;
    if (neighbory>=0 && neighbory < height && neighborx>=0 && neighborx<width)
    {
     int neighborposition=(neighbory*width)+neighborx;
     bneighbors+=MyImage->pixels[neighborposition].b;
     gneighbors+=MyImage->pixels[neighborposition].g;
     rneighbors+=MyImage->pixels[neighborposition].r;
     count++;
    }
   }

   }
   BlurImage.pixels[position].b = (unsigned char)(bneighbors / count);
   // we want to cast the division, telling the compiler to not go to an int which will be too big of a size , thus returning a TV-static like photo
   BlurImage.pixels[position].g=(unsigned char)(gneighbors/count);
   BlurImage.pixels[position].r=(unsigned char)(rneighbors/count);
  }
 }
 return BlurImage;
}
Image Sobeledges(const Image* MyImage)
{ Image SobelImage=Grayscale(MyImage);
 Image GrayImage=Grayscale(MyImage);// switch to grayscale in order to make the kernel passing easier(we have to pass only one value, not 3!)
  int SobelH[3][3]={
   {-1,0,1},
   {-2,0,2},
   {-1,0,1}
  };
 int SobelV[3][3]={
  {-1,-2,-1},
  {0,0,0},
  {1,2,1}
 };
 int width=SobelImage.infoheader.width;
 int height=SobelImage.infoheader.height;
for (int i=1;i<height-1;i++)
{for (int j=1;j<width-1;j++)
{ double Gx=0,Gy=0;
  int ogposition=(i*width)+j;
  for (int di=-1;di<=1;di++)
  {for (int dj=-1;dj<=1;dj++)
  {
   int neighborY=i+di;
   int neighborX=j+dj;
   int position=(neighborY*width)+neighborX;
   int grayvalue=GrayImage.pixels[position].b;
   Gx+=SobelH[di+1][dj+1]*grayvalue;
   Gy+=SobelV[di+1][dj+1]*grayvalue;
  }
  }
 double G=sqrt((Gx*Gx)+(Gy*Gy));
 if (G>255)
 {
  G=255;
 }
 SobelImage.pixels[ogposition].b=(unsigned char)G;
 SobelImage.pixels[ogposition].g=(unsigned char)G;
 SobelImage.pixels[ogposition].r=(unsigned char)G;
}
}
 FreeImage(&GrayImage);
 return SobelImage;
}