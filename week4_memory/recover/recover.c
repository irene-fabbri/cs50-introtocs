#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const int BOX_SIZE = 512;

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./recover infile\n");
        return 1;
    }

    // Open card file
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("Could not open %s.\n", argv[1]);
        return 1;
    }

    // Create buffer to store a chunk of data
    uint8_t buffer[BOX_SIZE];
    int jpg_count = 0;
    char filename[8];
    bool isReading = false;
    FILE *img;

    while (fread(&buffer, sizeof(uint8_t), BOX_SIZE, card) == 512)
    {
        // If I find a jpg signature
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // If it's the fist file
            if (!isReading)
            {
                isReading = true;
            }
            else
            {
                fclose(img);
            }
            // create new file
            sprintf(filename, "%03i.jpg", jpg_count);
            img = fopen(filename, "w");
            if (img == NULL)
            {
                fclose(card);
                printf("Could not create %s.\n", filename);
                return 2;
            }
            // write sequence on file
            fwrite(&buffer, sizeof(uint8_t), BOX_SIZE, img);
            jpg_count++;
        }
        else if (isReading)
        {
            fwrite(&buffer, sizeof(uint8_t), BOX_SIZE, img);
        }
    }

    fclose(img);
    fclose(card);
    return 0;
}
