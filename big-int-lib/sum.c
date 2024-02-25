#include "bigint.h"

bigint_t *bigint_sum(const bigint_t *ap, const bigint_t *bp) {
  bigint_t *res;
  size_t i, max_len;
  int max_num, sum, carry;

  if ((ap == NULL) || (bp == NULL) || (ap->len == 0) || (bp->len == 0)) {
    return NULL;
  }

  max_num = bigint_max_abs(ap, bp);

  if (max_num == 1) {
    return bigint_sum(bp, ap);
  }

  assert(ap->len >= bp->len);

  max_len = ap->len;

  /* +1 for possbile carry. */
  res = bigint_from_size(max_len + 1);

  if (res == NULL) {
    return NULL;
  }

  res->sign = ap->sign;

  assert(ap->len >= bp->len);

  for (i = 0, carry = 0; i < max_len; ++i) {
    sum = ap->digits[i] + carry;

    /* Bascically a subtraction if signs are different. */
    if (ap->sign != bp->sign) {
      if (i < bp->len) {
        sum = sum - bp->digits[i];
        carry = 0;
        if (sum < 0) {
          carry = -1;
          sum += BASE;
        }
      } else {
        carry = 0;
      }
      /* Actual addition if signs are equal. */
    } else if (ap->sign == bp->sign) {
      if (i < bp->len) {
        carry = (sum + bp->digits[i]) / BASE;
        sum = (sum + bp->digits[i]) % BASE;
      }
    }
    res->digits[i] = sum;
  }

  assert(i == max_len);
  assert(carry >= 0);

  res->digits[i] = carry;

  bigint_normalize(res);

  if (res == NULL) {
    return NULL;
  }

  if ((res->len == 1) && (res->digits[0] == 0)) {
    res->sign = pos;
  }

  return res;
}