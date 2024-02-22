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

/* Initialize a new BigInt structure. */
BigInt *bigint_init(void);

/* Free an allocated memory. */
int bifree(BigInt *x);

/* Pad to right with zeroes.
   Return 0 if the memory reallocation was successful, 1 if something went wrong. */
int bigint_add_padding(BigInt *x, size_t new_length);

/* Remove leading zeroes.
   Return 0 if the memory reallocation was successful, 1 if something went wrong. */
int bigint_normalize(BigInt *x);

/* Return a pointer to a new BigInt structure of the specified length. */
BigInt *bigint_from_size(size_t length);

/* Convert from string base 10 to BigInt. */
BigInt *str_to_bigint(char *str);

/* Convert from BigInt to string base 10. */
char *bigint_to_str(BigInt *x);

/* Compare two big integers.
   Return 0 if identical, 1 if different. */
int bigint_cmp(BigInt *x, BigInt *y);

/* Return the sum of two big integers. */
BigInt *bigint_sum(BigInt *x, BigInt *y);

/* Return the difference of two big integers. */
BigInt *bigint_subtract(BigInt *x, BigInt *y);

/* Return the product of two big integers. */
BigInt *bigint_mul(BigInt *x, BigInt *y);

/* Perform an integer division of two big integers. */
BigInt *bigint_div(BigInt *x, BigInt *y);

/* Perform a modulo division of two big integers. */
BigInt *bigint_mod(BigInt *x, BigInt *y);

#endif /* BIGINT_H_ */