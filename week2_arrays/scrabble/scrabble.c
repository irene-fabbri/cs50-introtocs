#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int get_points(string word);
void find_winner(int score[]);

const int N = 2;
const int values[26] = {1, 3, 3, 2,  1, 4, 2, 4, 1, 8, 5, 1, 3,
                        1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int main(void)
{
    string input[N];
    int points[N];

    for (int i = 0; i < N; i++)
    {
        input[i] = get_string("Player %i: ", i + 1);
        points[i] = get_points(input[i]);
    }

    find_winner(points);
}

int get_points(string word)
{
    int points = 0;
    for (int i = 0, length = strlen(word); i < length; i++)
    {
        if (isalpha(word[i]))
        {
            int letter_index = toupper(word[i]) - 'A';
            points += values[letter_index];
        }
    }
    return points;
}

void find_winner(int score[])
{
    int winner = -1;

    if (score[0] > score[1])
    {
        winner = 1;
    }
    else if (score[0] < score[1])
    {
        winner = 2;
    }
    else
    {
        printf("Tie!\n");
        return;
    }

    printf("Player %i wins!\n", winner);
    return;
}
