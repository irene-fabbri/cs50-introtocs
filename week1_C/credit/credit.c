#include <cs50.h>
#include <math.h>
#include <stdio.h>

int checksum(long);
bool isVisa(long, int);
bool isMasterCard(long, int);
bool isAmex(long, int);

int main(void)
{
    long int credit_num = get_long("Number: ");
    int num_digits = checksum(credit_num);
    if (num_digits > 0)
    {
        if (isAmex(credit_num, num_digits))
        {
            printf("AMEX\n");
        }
        else if (isMasterCard(credit_num, num_digits))
        {
            printf("MASTERCARD\n");
        }
        else if (isVisa(credit_num, num_digits))
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}

int checksum(long credit_number)
{
    long even = 0;
    long odd = 0;
    int num_digits = 0;
    int curr_digit;

    long power = 1;
    long remaining_digits = credit_number;
    long checked_digits = 0;
    while (checked_digits != credit_number && num_digits < 17)
    {
        num_digits++;
        curr_digit = remaining_digits % 10;
        checked_digits += curr_digit * power;
        remaining_digits = remaining_digits / 10;
        power = power * 10;

        if (num_digits % 2 == 0)
        {
            int num = 2 * curr_digit;
            even += num / 10 + num % 10;
        }
        else
        {
            odd += curr_digit;
        }
    }

    if ((even + odd) % 10 == 0 && num_digits > 0)
    {
        return num_digits;
    }
    else
    {
        return -1;
    }
}

bool isMasterCard(long credit_num, int num_digits)
{
    if (num_digits == 16)
    {
        int first_digits = credit_num / pow(10, 14);
        if (first_digits == 51 || first_digits == 52 || first_digits == 53 || first_digits == 54 ||
            first_digits == 55)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

bool isAmex(long credit_num, int num_digits)
{
    if (num_digits == 15)
    {
        int first_digits = credit_num / pow(10, 13);
        if (first_digits == 34 || first_digits == 37)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}
bool isVisa(long credit_num, int num_digits)
{
    if (num_digits == 13 || num_digits == 16)
    {
        int first_digit = credit_num / pow(10, num_digits - 1);
        if (first_digit == 4)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    return false;
}
