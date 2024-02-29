#include <assert.h>

#include "bigint.h"
#include "utils.h"

bigint_t *bigint_sum_pos(const bigint_t *ap, const bigint_t *bp) {
  bigint_t *res;
  size_t i, mlen;
  int sum, carry;

  if (!ap || !bp) {
    return NULL;
  }

  assert(((ap->sign == pos) && (bp->sign == pos)) || (ap->sign == zero) ||
         (bp->sign == zero));

  if (bigint_cmp_abs(ap, bp) == -1) {
    return bigint_sum_pos(bp, ap);
  }

  assert(ap->len >= bp->len);
  mlen = ap->len;

  /* +1 for possible carry. */
  res = bigint_from_size(mlen + 1);

  if (!res) {
    return NULL;
  }

  carry = 0;
  for (i = 0; i < mlen; ++i) {
    sum = ap->digits[i] + carry;
    if (i < bp->len) {
      carry = (sum + bp->digits[i]) / BASE;
      sum = (sum + bp->digits[i]) % BASE;
    }
    res->digits[i] = sum;
  }
  assert((carry == 0) || (carry == 1));
  assert(i == mlen);
  res->digits[mlen] = carry;

  res->sign = pos;

  if (bigint_normalize(res)) {
    bifree(res);
    return NULL;
  }

  return res;
}

bigint_t *bigint_sum(const bigint_t *ap, const bigint_t *bp) { return NULL; }