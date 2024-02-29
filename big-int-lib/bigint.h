#ifndef __BIGINT_H__
#define __BIGINT_H__

#include <stdlib.h>

#define BASE 10

typedef enum { neg = -1, zero = 0, pos = 1 } sign_t;

typedef struct {
  sign_t sign;
  size_t len;
  char* digits;
} bigint_t;

/* Free an allocated memory. */
void bifree(bigint_t* ap);

/* Return a pointer to a new BigInt structure of the specified length. */
bigint_t* bigint_from_size(const size_t len);

/* Convert from string base 10 to BigInt. */
bigint_t* bigint_from_str(const char* str);

/* Convert from BigInt to string base 10. */
char* bigint_to_str(const bigint_t* ap);

/* Convert from integer to BigInt. */
bigint_t* bigint_from_int(int n);

int bigint_to_int(const bigint_t* ap);

bigint_t* bigint_sum_pos(const bigint_t* ap, const bigint_t* bp);

/* Return the sum of two big integers. */
bigint_t* bigint_sum(const bigint_t* ap, const bigint_t* bp);

/* Return the difference of two big integers. */
bigint_t* bigint_sub_pos(const bigint_t* ap, const bigint_t* bp);

/* Return the difference of two big integers. */
bigint_t* bigint_sub(const bigint_t* ap, const bigint_t* bp);

/* Return the product of two big integers. */
bigint_t* bigint_mul(const bigint_t* ap, const bigint_t* bp);

/* Multiply a big integer by a 0 <= d < 10. */
bigint_t* bigint_mul_dec(const bigint_t* ap, const unsigned char d);

/* Perform an integer division big integer and a 0 < d < 10. */
bigint_t* bigint_div_dec(const bigint_t* ap, const unsigned char d);

/* Perform an integer division of two big integers. */
bigint_t* bigint_div_mod(const bigint_t* ap, const bigint_t* bp);

/* Perform a modulo division of two big integers. */
bigint_t* bigint_mod(const bigint_t* ap, const bigint_t* bp);

#endif /* __BIGINT_H__ */