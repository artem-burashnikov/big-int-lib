#include <assert.h>
#include <stdlib.h>
#include <string.h>

#ifndef __BIGINT_H__
#define __BIGINT_H__

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
int bifree(bigint_t *ap);

/* Remove leading zeroes.
   Return 0 if the memory reallocation was successful, 1 if something went
   wrong. */
int bigint_normalize(bigint_t *ap);

/* Return a pointer to a new BigInt structure of the specified length. */
bigint_t *bigint_from_size(size_t len);

/* Convert from string base 10 to BigInt. */
bigint_t *bigint_from_str(char *str);

/* Convert from BigInt to string base 10. */
char *bigint_to_str(const bigint_t *ap);

/* Compare two big integers.
   Return 0 if x = y, 1 otherwise. */
int bigint_cmp(const bigint_t *ap, const bigint_t *bp);

/* Return 0 if |x| >= |y|, 1 otherwise. */
int bigint_max_abs(const bigint_t *ap, const bigint_t *bp);

/* Return 1 if x > 0, return -1 if x < 0, return 0 if x = 0. */
int sgn(const bigint_t *ap);

/* Return the sum of two big integers. */
bigint_t *bigint_sum(const bigint_t *ap, const bigint_t *bp);

/* Return the difference of two big integers. */
bigint_t *bigint_sub(const bigint_t *ap, const bigint_t *bp);

/* Return the product of two big integers. */
bigint_t *bigint_mul(const bigint_t *ap, const bigint_t *bp);

/* Perform an integer division of two big integers. */
bigint_t *bigint_div(const bigint_t *ap, const bigint_t *bp);

/* Perform a modulo division of two big integers. */
bigint_t *bigint_mod(const bigint_t *ap, const bigint_t *bp);

#endif /* __BIGINT_H__ */