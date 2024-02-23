#include <stdbool.h>
#include <stddef.h>

#ifndef __BIGINT_H__
#define __BIGINT_H__

typedef enum { positive = 0, negative = 1 } Sign;

typedef struct {
  Sign sign;
  size_t length;
  char *digits;
} BigInt;

/* Initialize a new BigInt structure. */
BigInt *bigint_init(void);

/* Free an allocated memory. */
int bifree(BigInt *x);

/* Remove leading zeroes.
   Return 0 if the memory reallocation was successful, 1 if something went
   wrong. */
int bigint_normalize(BigInt *x);

/* Return a pointer to a new BigInt structure of the specified length. */
BigInt *bigint_from_size(size_t length);

/* Convert from string base 10 to BigInt. */
BigInt *str_to_bigint(char *str);

/* Convert from BigInt to string base 10. */
char *bigint_to_str(const BigInt *x);

/* Compare two big integers.
   Return 0 if x = y, 1 otherwise. */
int bigint_cmp(const BigInt *x, const BigInt *y);

/* Return 0 if |x| >= |y|, 1 otherwise. */
int bigint_max_abs(const BigInt *x, const BigInt *y);

/* Return 1 if x > 0, return -1 if x < 0, return 0 if x = 0. */
int sgn(const BigInt *x);

/* Return the sum of two big integers. */
BigInt *bigint_sum(const BigInt *x, const BigInt *y);

/* Return the difference of two big integers. */
BigInt *bigint_subtract(const BigInt *x, const BigInt *y);

/* Return the product of two big integers. */
BigInt *bigint_mul(const BigInt *x, const BigInt *y);

/* Perform an integer division of two big integers. */
BigInt *bigint_div(const BigInt *x, const BigInt *y);

/* Perform a modulo division of two big integers. */
BigInt *bigint_mod(const BigInt *x, const BigInt *y);

#endif /* BIGINT_H_ */