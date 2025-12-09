#include "bmp.h"

void grayscale(int height, int width, RGBTRIPLE image[height][width]);
void sepia(int height, int width, RGBTRIPLE image[height][width]);
void reflect(int height, int width, RGBTRIPLE image[height][width]);
void blur(int height, int width, RGBTRIPLE image[height][width]);

void silhouette(int height, int width, RGBTRIPLE image[height][width])
{
}

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{

    int red = pixel.rgbtRed;
    int green = pixel.rgbtGreen;
    int blue = pixel.rgbtBlue;

    int average;
    for (int x = 0; x < width; x++){
        for (int y = 0; y < height; y++){
            RGBTRIPLE pixel = image[y][x];
            average = round((red + green + blue) / 3.0);
            pixel.rgbtBlue = average;
            pixel.rgbtGreen = average;
            pixel.rgbtRed = average;
            //image[y][x] = pixel;
        }

    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    int red, green, blue;
    int sepiaRed, sepiaGreen, sepiaBlue;

    for (int x = 0; x < width; x++){
        for (int y = 0; y < height; y++){
            RGBTRIPLE pixel = image[y][x];

            red = pixel.rgbtRed;
            green = pixel.rgbtGreen;
            blue = pixel.rgbtBlue;

            sepiaRed = .393 * red + .769 * green + .189 * blue;
            sepiaGreen = .349 * red + .686 * green + .168 * blue;
            sepiaBlue = .272 * red + .534 * green + .131 * blue;

            sepiaRed = round(clamp(sepiaRed,0,255));
            sepiaGreen = round(clamp(sepiaGreen,0,255));
            sepiaBlue = round(clamp(sepiaBlue,0,255));

            image[y][x].rgbtRed = sepiaRed;
            image[y][x].rgbtGreen = sepiaGreen;
            image[y][x].rgbtBlue = sepiaBlue;
        }

    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{

    for (int x = 0; x < width; x++){
        for (int y = 0; y < height; y++){
            RGBTRIPLE pixel = image[y][x];
            image[y][x] = image[y][width - x - 1];
            image[y][width - x - 1] = pixel;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //The new value of each pixel would be the average of the values of all of the pixels that are within 1 row and column of the original pixel (forming a 3x3 box)
    RGBTRIPLE copy[height][width];
    for (int x = 0; x < width; x++){
        for (int y = 0; y < height; y++){
            copy[y][x] = image[y][x];
            for (int i = -3;  i < 3; i++){
                copy[y+i][x+i] = image[y][x];
            }
            
        }
    }
}
