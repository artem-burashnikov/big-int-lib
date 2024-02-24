#include "bigint.h"

bigint_t *bigint_sum(const bigint_t *ap, const bigint_t *bp) {
  bigint_t *res;
  size_t i, max_len;
  int max_num, sum, carry;

  if ((ap == NULL) || (bp == NULL)) {
    return NULL;
  }

  max_num = bigint_max_abs(ap, bp);

  if (max_num == 1) {
    return bigint_sum(bp, ap);
  }

  /* At this point |x| >= |y|. */
  max_len = ap->len;

  /* +1 for possbile carry. */
  res = bigint_from_size(max_len + 1);

  if (res == NULL) {
    return NULL;
  }

  res->sign = ap->sign;

  carry = 0;

  assert(ap->len >= bp->len);

  for (i = 0; i < max_len; ++i) {
    sum = ap->digits[i] + carry;
    carry = 0;

    if (ap->sign != bp->sign) {
      if (i < bp->len) {
        sum = sum - bp->digits[i];
      }
      if (sum < 0) {
        sum += 10;
        carry = -1;
      }
    } else if (ap->sign == bp->sign) {
      if (i < bp->len) {
        sum = sum + bp->digits[i];
      }
      if (sum >= 10) {
        sum -= 10;
        carry = 1;
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

  if ((res->len == 1) && (res->digits[0] == ('0' - '0'))) {
    res->sign = pos;
  }

  return res;
}