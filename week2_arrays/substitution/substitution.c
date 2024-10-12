#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

bool isValid(string key);
string encrypt(string plaintext, string key);

int main(int argc, string argv[])
{
    if (argc != 2 || !isValid(argv[1]))
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    // get message to encrypt and encrypt it using key=argv[1]
    string plaintext = get_string("plaintext:\t");
    string ciphertext = encrypt(plaintext, argv[1]);
    printf("ciphertext:\t%s\n", ciphertext);
}

bool isValid(string key)
{
    int used_letters[26] = {0};
    // check if key has a valid length
    if (strlen(key) != 26)
    {
        return false;
    }
    else
    {
        for (int i = 0; i < 26; i++)
        {
            char this_letter = key[i];
            // check if key is alphabetical
            if (!isalpha(this_letter))
            {
                return false;
            }
            else
            {
                // check if every letter appears once
                int index = toupper(key[i]) - 'A';
                if (used_letters[index] == 0)
                {
                    used_letters[index]++;
                }
                else
                {
                    return false;
                }
            }
        }
    }
    return true;
}

string encrypt(string plaintext, string key)
{

    string encrypted = plaintext;

    for (int i = 0, length = strlen(plaintext); i < length; i++)
    {
        char this_char = plaintext[i];
        // if this char is alphabetic, encrypt it using key
        if (isalpha(this_char))
        {
            int index = toupper(this_char) - 'A';
            int this_shift = toupper(key[index]) - toupper(this_char);
            encrypted[i] += this_shift;
        }
    }
    return encrypted;
}
