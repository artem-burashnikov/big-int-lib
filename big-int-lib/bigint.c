#include "bigint.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

BigInt alloc_bigint(size_t length)
{
    BigInt bint;

    bint.length = length;
    bint.is_negative = false;
    bint.digits = malloc(sizeof(char) * length);

    if (bint.digits != NULL)
    {
        return bint;
    }
    else
    {
        printf("A problem occured while trying to allocate the memory.\n");
        exit(-1);
    }
}

/* Assume base 10 */
BigInt strtobi(char *str)
{
    /* Length of the input string. */
    size_t len;
    /* Actual significant digits count. */
    size_t digits_cnt;
    bool sign;
    char c;

    len = strlen(str);
    digits_cnt = len;

    /* The first character might be a sign. */
    if (len == 0)
    {
        printf("Incorrect strtobi input: string was empty\n");
        exit(-1);
    }

    if ((len == 1) && str[0] == '-')
    {
        printf("Incorrect strtobi input: string only contains a sign.\n");
        exit(-1);
    }

    if ((len > 1) && (str[0] == '-'))
    {
        digits_cnt--;
        sign = true;
    }
    else
    {
        sign = false;
    }

    BigInt bint = alloc_bigint(digits_cnt);
    bint.is_negative = sign;

    for (size_t i = 0; i < digits_cnt; ++i)
    {
        c = str[len - 1 - i];

        if ((c >= '0') && (c <= '9'))
        {
            bint.digits[i] = c - '0';
        }
        else
        {
            free(bint.digits);
            printf("Incorrect strtobi input: string contains non-numeric characters.\n");
            exit(-1);
        }
    }

    return bint;
}

char *bitostr(BigInt n)
{
    char *str;
    size_t str_len = n.length;
    size_t num_digits = n.length;
    size_t i, j;
    i = j = 0;

    if (n.is_negative)
    {
        ++str_len;
    }

    str = malloc((sizeof(char)) * str_len + 1);

    if (n.is_negative)
    {
        str[0] = '-';
        ++i;
    }

    while (j < num_digits)
    {
        str[i] = n.digits[num_digits - 1 - j];
        i++;
        j++;
    }

    str[i] = '\0';

    return str;
}