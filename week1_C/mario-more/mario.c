#include <cs50.h>
#include <stdio.h>

void print_row(int, int);
void print_blanks(int);
void print_bricks(int);

int main(void)
{
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    for (int i = 0; i < height; i++)
    {
        print_row(height, i + 1);
    }
}

void print_row(int height, int width)
{
    print_blanks(height - width);
    print_bricks(width);
    printf("  ");
    print_bricks(width);
    printf("\n");
}

void print_bricks(int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("#");
    }
}

void print_blanks(int n)
{
    for (int i = 0; i < n; i++)
    {
        printf(" ");
    }
}
