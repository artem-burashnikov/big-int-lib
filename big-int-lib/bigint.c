#include "bigint.h"
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

BigInt *bigint_init()
{
    BigInt *x = malloc(sizeof(BigInt));

    if (x == NULL)
    {
        return NULL;
    }
    else
    {
        return x;
    }
}

int bifree(BigInt *x)
{
    free(x->digits);
    free(x);
    return 0;
}

BigInt *bigint_from_size(size_t length)
{
    BigInt *x = bigint_init();

    if (x == NULL)
    {
        return NULL;
    }

    x->length = length;
    x->is_negative = false;
    x->digits = malloc(sizeof(char) * length);

    if (x->digits != NULL)
    {
        return x;
    }
    else
    {
        free(x);
        return NULL;
    }
}

/* Assume base 10 */
BigInt *str_to_bigint(char *str)
{
    BigInt *x;
    /* Length of the input string. */
    size_t len;
    /* Actual significant digits count. */
    size_t digits_cnt;
    bool sign;
    size_t i;
    char c;

    len = strlen(str);
    digits_cnt = len;

    /* Empry input string. */
    if (len == 0)
    {
        return NULL;
    }

    /* String only contains a sign. */
    if ((len == 1) && ((str[0] == '-') || (str[0] == '+')))
    {
        return NULL;
    }

    if ((len > 1) && (str[0] == '-'))
    {
        /* If the first character in a string is a sign,
           the number of digits is less than the string's length. */
        digits_cnt--;
        sign = true;
    }
    else
    {
        sign = false;
    }

    x = bigint_from_size(digits_cnt);
    x->is_negative = sign;

    for (i = 0; i < digits_cnt; ++i)
    {
        c = str[len - 1 - i];

        if ((c >= '0') && (c <= '9'))
        {
            x->digits[i] = c - '0';
        }
        else
        {
            bifree(x);
            return NULL;
        }
    }

    return x;
}

char *bigint_to_str(BigInt *x)
{
    char *str;
    size_t str_len = x->length;
    size_t num_digits = x->length;
    size_t i, j;
    i = j = 0;

    if (x->is_negative)
    {
        ++str_len;
    }

    str = malloc((sizeof(char)) * str_len + 1);

    if (str == NULL)
    {
        return NULL;
    }

    if (x->is_negative)
    {
        str[0] = '-';
        ++i;
    }

    while (j < num_digits)
    {
        str[i] = x->digits[num_digits - 1 - j];
        i++;
        j++;
    }

    str[i] = '\0';

    return str;
}

int bigint_cmp(BigInt *x, BigInt *y)
{
    size_t len;
    int ret = 0;

    if ((x->length) != (y->length))
    {
        ret = 1;
    }
    else if ((x->is_negative) != (y->is_negative))
    {
        ret = 1;
    }
    else
    {
        /* Both length values are identical at this point. */
        len = x->length;
        for (size_t i = 0; i < len; ++i)
        {
            if ((x->digits[i]) != (y->digits[i]))
            {
                ret = 1;
                break;
            }
        }
    }

    return ret;
}

#if 0
BigInt *bigint_add(BigInt *x, BigInt *y)
{
    BigInt *res;
    bool sign = false;
    size_t i, j;

    /* x + (-y) = x - y */
    if ((!(x->is_negative)) && (y->is_negative))
    {
        return bigint_subtract(x, y);
    }

    /* (-x) + y = y - x */
    if ((x->is_negative) && (!(y->is_negative)))
    {
        return bigint_subtract(y, x);
    }

    /* (-x) + (-y) = - (x + y) */
    if ((x->is_negative) && (y->is_negative))
    {
        sign = true;
    }

    res = bigint_init();

    if (res == NULL)
    {
        return NULL;
    }

    res->is_negative = sign;

    return NULL;
}

BigInt *bigint_subtract(BigInt *x, BigInt *y)
{
    return NULL;
}
#endif