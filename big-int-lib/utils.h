#ifndef __UTILS_H__
#define __UTILS_H__

#include "bigint.h"

/* Compare two big integers.
   Return 0 if x = y, 1 otherwise. */
int bigint_cmp(const bigint_t *ap, const bigint_t *bp);

/* Return 0 if |x| >= |y|, 1 otherwise. */
int bigint_max_abs(const bigint_t *ap, const bigint_t *bp);

/* Return 1 if a is 0, else 0. */
int eq_zero(const bigint_t *ap);

/* Remove leading zeroes.
   Return 0 if the memory reallocation was successful, 1 if something went
   wrong. */
int bigint_normalize(bigint_t *ap);

/* Pad right with zeroes. Return 0 if successfull. */
int bigint_add_padding(bigint_t *ap, size_t new_len);

#endif /* __UTILS_H__ */