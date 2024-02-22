#include "bigint.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

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
    if (x != NULL)
    {
        free(x->digits);
        free(x);
    }

    return 0;
}

int bigint_add_padding(BigInt *x, size_t new_length)
{
    char *tmp;
    size_t old_length = x->length;
    size_t i;

    if (x == NULL)
    {
        return 1;
    }

    if ((new_length - old_length) <= 0)
    {
        return 1;
    }

    tmp = realloc(x->digits, sizeof(char) * new_length);

    if (tmp == NULL)
    {
        return 1;
    }

    x->digits = tmp;
    x->length = new_length;

    for (i = old_length; i < new_length; ++i)
    {
        x->digits[i] = 0;
    }

    return 0;
}

int bigint_normalize(BigInt *x)
{
    char *tmp;
    size_t old_length, new_length, i;

    if (x == NULL)
    {
        return 1;
    }

    old_length = new_length = x->length;
    i = new_length - 1;

    while ((x->digits[i]) == 0)
    {
        --i;
    }

    new_length = i + 1;

    if (new_length != old_length)
    {
        tmp = realloc(x->digits, sizeof(char) * new_length);

        if (tmp == NULL)
        {
            return 1;
        }

        x->digits = tmp;
        x->length = new_length;
    }

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

    if (str == NULL)
    {
        return NULL;
    }

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

    /* If the first character in a string is a sign,
       the number of digits is less than the string's length. */
    if ((len > 1) && (str[0] == '-'))
    {
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

char *bigint_to_str(const BigInt *x)
{
    char *str;
    size_t str_len, num_digits, i, j;

    if (x == NULL)
    {
        return NULL;
    }

    str_len = x->length;
    num_digits = x->length;
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

int bigint_cmp(const BigInt *x, const BigInt *y) /* Needs NULL-check? */
{
    size_t len;

    if ((x->length) != (y->length))
    {
        return 1;
    }
    else if ((x->is_negative) != (y->is_negative))
    {
        return 1;
    }
    else
    {
        /* Both lengthes are identical at this point. */
        len = x->length;
        for (size_t i = 0; i < len; ++i)
        {
            if ((x->digits[i]) != (y->digits[i]))
            {
                return 1;
            }
        }
    }

    return 0;
}

int bigint_max(const BigInt *x, const BigInt *y) /* Needs NULL-check? */
{
    int ret;
    size_t i, len;

    if (bigint_cmp(x, y) == 0)
    {
        return 0;
    }

    /* max(-x, y) = 1 */
    if ((x->is_negative) && (!(y->is_negative)))
    {
        ret = 1;
    }
    /* max(x, -y) = 0 */
    else if ((!(x->is_negative)) && (y->is_negative))
    {
        ret = 0;
    }
    else if (x->length < y->length)
    {
        ret = 1;
    }
    else if (x->length == y->length)
    {
        len = x->length;
        i = len - 1;
        while (x->digits[i] == y->digits[i])
        {
            --i;
        }

        if (x->digits[i] < y->digits[i])
        {
            ret = 1;
        }
    }
    else
    {
        ret = 0;
    }

    return ret;
}

BigInt *bigint_sum(const BigInt *x, const BigInt *y)
{
    BigInt *res;
    bool sign;
    char sum, carry;
    size_t i, j, max_length, min_length;

    if ((x == NULL) || (y == NULL))
    {
        return NULL;
    }

    sign = false;

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

    if (x->length >= y->length)
    {
        max_length = x->length;
        min_length = y->length;
    }
    else
    {
        max_length = y->length;
        min_length = x->length;
    }

    /* +1 for possible carry. */
    res = bigint_from_size(max_length + 1);

    if (res == NULL)
    {
        return NULL;
    }

    res->is_negative = sign;

    carry = 0;
    for (i = 0; i < min_length; ++i)
    {
        sum = x->digits[i] + y->digits[i] + carry;
        res->digits[i] = sum % 10;
        carry = sum / 10;
    }

    for (j = min_length; j < max_length; ++j)
    {
        if (max_length == x->length)
        {
            sum = x->digits[j] + carry;
        }
        else
        {
            sum = y->digits[j] + carry;
        }

        res->digits[j] = sum % 10;
        carry = sum / 10;
    }

    if (carry != 0)
    {
        res->digits[res->length - 1] = carry;
    }
    else
    {
        res->digits[res->length - 1] = 0;
    }

    if (bigint_normalize(res) == 0)
    {
        return res;
    }
    else
    {
        return NULL;
    }
}

BigInt *bigint_subtract(const BigInt *x, const BigInt *y)
{
    return NULL;
}