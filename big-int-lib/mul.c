#include "bigint.h"

bigint_t *bigint_mul(const bigint_t *ap, const bigint_t *bp) {
  bigint_t *res;
  size_t i, j;
  int max_num, sum, carry;

  if ((ap == NULL) || (bp == NULL) || (ap->len == 0) || (bp->len == 0)) {
    return NULL;
  }

  max_num = bigint_max_abs(ap, bp);

  if (max_num == 1) {
    return bigint_mul(bp, ap);
  }

  assert(ap->len >= bp->len);

  res = bigint_init();

  if (res == NULL) {
    return NULL;
  }

  res->len = ap->len + bp->len;

  res->digits = calloc(ap->len + bp->len, sizeof(char));

  assert(res->len == ap->len + bp->len);
  assert((sizeof(res->digits[0]) * (ap->len + bp->len)) == (ap->len + bp->len) * (sizeof(char)));

  if (res->digits == NULL) {
    bifree(res);
    return NULL;
  }

  res->sign = ap->sign ^ bp->sign;

  for (j = 0; j < bp->len; ++j) {
    if (bp->digits[j] == 0) {
      res->digits[j + ap->len] = 0;
      continue;
    }

    for (i = 0, carry = 0; i < ap->len; ++i) {
      sum = (ap->digits[i]) * (bp->digits[j]) + res->digits[i + j] + carry;
      res->digits[i + j] = sum % BASE;
      carry = sum / BASE;
      assert((carry >= 0) && (carry < BASE));
    }

    res->digits[j + ap->len] = carry;
  }

  bigint_normalize(res);

  return res;
}