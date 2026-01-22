#include "Imageprocessing.h"
#include <stdio.h>
int main(void)
{  char filename[100];
    printf("Please enter the name of your BMP file(e.g., photo.bmp): ");
    scanf("%s",filename);
    Image YourImage=ProcessImage(filename);
    int option;
    printf(
        "Welcome to my image processing program! Your options are:\n1.Image rotation \n2.Make a grayscale copy \n3.Make an inverted colors copy \n4.Blurred copy of your image \n5.Sobel filtered image copy");
    scanf("%d",&option);
    switch (option)
    {
    case 1:
        {
            Image rotated=Rotateimage(&YourImage);
            SaveImage("RotatedImage.bmp",&rotated);
            FreeImage(&rotated);
            break;

        }
    case 2:
        {
            Image gray=Grayscale(&YourImage);
            SaveImage("GrayImage.bmp",&gray);
            FreeImage(&gray);
            break;
        }
    case 3:
        {
            Image inverse=Invertedimage(&YourImage);
            SaveImage("InverseImage.bmp",&inverse);
            FreeImage(&inverse);
            break;
        }
    case 4:
        {
            int bf;
            printf("Please provide a blurring factor (between 0-15),please note that the bigger the blurring factor you choose the more time it will take for the blurred image to be processed ");
            scanf("%d",&bf);
            Image blurred=Blurimage(&YourImage,bf);
            SaveImage("Blurredimage.bmp",&blurred);
            FreeImage(&blurred);
            break;
        }
    case 5:
        {
            Image sobel=Sobeledges(&YourImage);
            SaveImage("SobelImage.bmp",&sobel);
            FreeImage(&sobel);
            break;
        }
    default:
        {
            printf("Please enter a valid option in the menu");
            break;
        }
    }
    FreeImage(&YourImage);
    return 0;
}