#ifndef __UTILS_H__
#define __UTILS_H__

#include "bigint.h"

/* Return a pointer to a new BigInt structure of the specified length. */
bigint_t* bigint_from_size(const size_t len);

/* Return -1 if |x| < |y|, 0 if |x| == |y|, 1 if |x| > |y|. */
int8_t bigint_cmp_abs(const bigint_t* ap, const bigint_t* bp);

/* Remove leading zeroes. */
void bigint_normalize(bigint_t* ap);

/* Pad right with t zeroes. */
void bigint_add_padding(bigint_t* ap, uint8_t t);

/* Initialize a new bigint struct which points to the same char array as ap. */
bigint_t* bigint_mirror(const bigint_t* ap);

/* Perform euclidian modulo division. */
int32_t eu_mod(const int32_t x, const int32_t y);

/* Perform euclidian integer division. */
int32_t eu_div(const int32_t x, const int32_t y);

#endif /* __UTILS_H__ */