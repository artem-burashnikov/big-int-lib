#ifndef __BIGINT_H__
#define __BIGINT_H__

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BASE 10

typedef enum { pos = 0, neg = 1 } sign_t;

typedef struct {
  sign_t sign;
  size_t len;
  char *digits;
} bigint_t;

/* Initialize a new BigInt structure. */
bigint_t *bigint_init(void);

/* Free an allocated memory. */
void bifree(bigint_t *ptr_x);

/* Return a pointer to a new BigInt structure of the specified length. */
bigint_t *bigint_from_size(size_t len);

/* Convert from string base 10 to BigInt. */
bigint_t *bigint_from_str(char *str);

/* Convert from BigInt to string base 10. */
char *bigint_to_str(const bigint_t *ptr_x);

/* Return the sum of two big integers. */
bigint_t *bigint_sum(const bigint_t *ptr_x, const bigint_t *ptr_y);

/* Return the difference of two big integers. */
bigint_t *bigint_sub(const bigint_t *ptr_x, const bigint_t *ptr_y);

/* Return the product of two big integers. */
bigint_t *bigint_mul(const bigint_t *ptr_x, const bigint_t *ptr_y);

/* Multiply a big integer by a 0 <= d < 10. */
bigint_t *bigint_mul_dec(const bigint_t *ptr_x, const unsigned char d);

/* Perform an integer division big integer and a 0 < d < 10. */
bigint_t *bigint_div_dec(const bigint_t *ptr_x, const unsigned char d);

/* Perform an integer division of two big integers. */
bigint_t *bigint_div_mod(const bigint_t *ptr_x, const bigint_t *ptr_y);

/* Perform a modulo division of two big integers. */
bigint_t *bigint_mod(const bigint_t *ptr_x, const bigint_t *ptr_y);

#endif /* __BIGINT_H__ */