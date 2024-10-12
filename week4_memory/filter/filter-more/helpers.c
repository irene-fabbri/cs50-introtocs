#include "helpers.h"
#include <math.h>
#include <stdio.h>

void swap(RGBTRIPLE *left_pixel, RGBTRIPLE *right_pixel);

// Convert image to grayscale

void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int avg = (int) round(
                (image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.);

            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

void swap(RGBTRIPLE *left_pixel, RGBTRIPLE *right_pixel)
{
    RGBTRIPLE tmp = *left_pixel;
    *left_pixel = *right_pixel;
    *right_pixel = tmp;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int half_width = width / 2;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < half_width; j++)
        {
            // Find index of the specular pixel
            int k = width - j - 1;
            swap(&image[i][j], &image[i][k]);
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        int start_row = i > 0 ? i - 1 : i;
        int stop_row = i < height - 1 ? i + 1 : i;

        for (int j = 0; j < width; j++)
        {
            int start_column = j > 0 ? j - 1 : j;
            int stop_column = j < width - 1 ? j + 1 : j;

            int count = 0;
            double avg_red = 0.;
            double avg_green = 0.;
            double avg_blue = 0.;

            for (int l = start_row; l <= stop_row; l++)
            {
                for (int m = start_column; m <= stop_column; m++)
                {
                    avg_red += copy[l][m].rgbtRed;
                    avg_green += copy[l][m].rgbtGreen;
                    avg_blue += copy[l][m].rgbtBlue;
                    count++;
                }
            }

            image[i][j].rgbtRed = (int) round(avg_red / count);
            image[i][j].rgbtGreen = (int) round(avg_green / count);
            image[i][j].rgbtBlue = (int) round(avg_blue / count);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    const int N = 3;
    int Gx[N][N] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[N][N] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    RGBTRIPLE black;
    black.rgbtRed = 0;
    black.rgbtGreen = 0;
    black.rgbtBlue = 0;

    RGBTRIPLE copy[height + 2][width + 2];
    // Create a black border
    for (int i = 0; i < height + 2; i++)
    {
        copy[i][0] = black;
    }
    for (int i = 0; i < height + 2; i++)
    {
        copy[i][width + 1] = black;
    }

    for (int j = 1; j < width + 1; j++)
    {
        copy[0][j] = black;
    }
    for (int j = 1; j < width + 1; j++)
    {
        copy[height + 1][j] = black;
    }

    // Create a copy of image
    for (int i = 1; i < height + 1; i++)
    {
        for (int j = 1; j < width + 1; j++)
        {
            copy[i][j] = image[i - 1][j - 1];
        }
    }

    // Implement the edge filter
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            int gx_red = 0;
            int gx_green = 0;
            int gx_blue = 0;

            int gy_red = 0;
            int gy_green = 0;
            int gy_blue = 0;

            for (int dx = 0; dx < 3; dx++)
            {
                for (int dy = 0; dy < 3; dy++)
                {
                    gx_red += copy[i + dx][j + dy].rgbtRed * Gx[dx][dy];
                    gx_green += copy[i + dx][j + dy].rgbtGreen * Gx[dx][dy];
                    gx_blue += copy[i + dx][j + dy].rgbtBlue * Gx[dx][dy];

                    gy_red += copy[i + dx][j + dy].rgbtRed * Gy[dx][dy];
                    gy_green += copy[i + dx][j + dy].rgbtGreen * Gy[dx][dy];
                    gy_blue += copy[i + dx][j + dy].rgbtBlue * Gy[dx][dy];
                }
            }

            double g_red = sqrt(gx_red * gx_red + gy_red * gy_red);
            double g_green = sqrt(gx_green * gx_green + gy_green * gy_green);
            double g_blue = sqrt(gx_blue * gx_blue + gy_blue * gy_blue);

            image[i][j].rgbtRed = (g_red > 255) ? 255 : (BYTE) round(g_red);
            image[i][j].rgbtGreen = (g_green > 255) ? 255 : (BYTE) round(g_green);
            image[i][j].rgbtBlue = (g_blue > 255) ? 255 : (BYTE) round(g_blue);
        }
    }
    return;
}
