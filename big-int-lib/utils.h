#ifndef __UTILS_H__
#define __UTILS_H__

#include "bigint.h"

int bigint_cmp(const bigint_t* ap, const bigint_t* bp);

/* Return -1 if |x| < |y|, 0 if |x| == |y|, 1 if |x| > |y|. */
int bigint_cmp_abs(const bigint_t* ap, const bigint_t* bp);

/* Remove leading zeroes.
   Return 0 if the memory reallocation was successful, 1 if something went
   wrong. */
int bigint_normalize(bigint_t* ap);

/* Pad right with t zeroes. Return 0 if successfull. */
int bigint_add_padding(bigint_t* ap, size_t t);

/* Shift rigth to t positions. Return 0 if successfull. */
int bigint_rshift(bigint_t* ap, size_t t);

/* Initialize a new bigint struct which copies the ap. */
bigint_t* bigint_cpy(const bigint_t* ap);

/* Initialize a new bigint struct which points to the same char array as ap. */
bigint_t* bigint_mirror(const bigint_t* ap);

int iabs(int x);

int eu_mod(const int x, const int y);

int eu_div(const int x, const int y);

sign_t rev_sign(sign_t sgn);

#endif /* __UTILS_H__ */