#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifndef __BIGINT_H__
#define __BIGINT_H__

typedef struct
{
    bool is_negative;
    size_t length;
    char *digits;
} BigInt;

/* Allocates the memory for the BigInt structure of the specified length. */
BigInt alloc_bigint(size_t length);

/* Converts from string base 10 to BigInt. */
BigInt strtobi(char *str);

/* Converts from BigInt to string base 10. */
char *bitostr(BigInt x);

/* Compares two big integers.
   Return 0 if identical, 1 if different. */
int bicmp(BigInt *x, BigInt *y);

/* Return sum of two big integers. */
BigInt biadd(BigInt *x, BigInt *y);

/* Return difference of two big integers. */
BigInt bisubtract(BigInt *x, BigInt *y);

#endif /* BIGINT_H_ */