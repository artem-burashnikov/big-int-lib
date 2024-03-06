#include <stdint.h>

#include "bigint.h"
#include "utils.h"

static void helper_sub(const bigint_t *ap, const bigint_t *bp, bigint_t *res) {
  int8_t carry, w;

  carry = 0;
  for (size_t i = 0; i < ap->len; ++i) {
    w = ap->digits[i] + carry;
    if (i < bp->len) {
      w -= bp->digits[i];
    }
    res->digits[i] = eu_mod(w, BASE);
    carry = eu_div(w, BASE);
  }

  return;
}

static void helper_sum(const bigint_t *ap, const bigint_t *bp, bigint_t *res) {
  int8_t carry, w;

  carry = 0;
  for (size_t i = 0; i < ap->len; ++i) {
    w = ap->digits[i] + carry;
    if (i < bp->len) {
      w += bp->digits[i];
    }
    res->digits[i] = w % BASE;
    carry = w / BASE;
  }
  res->digits[ap->len] = carry;

  return;
}

bigint_t *bigint_sum(const bigint_t *ap, const bigint_t *bp) {
  bigint_t *res;

  if (!ap || !bp) {
    return NULL;
  }

  if (bigint_cmp_abs(ap, bp) == -1) {
    return bigint_sum(bp, ap);
  }

  res = bigint_from_size(ap->len + 1);

  if (res == NULL) {
    return NULL;
  }

  res->sign = ap->sign;
  if (ap->sign != bp->sign) {
    helper_sub(ap, bp, res);
  } else {
    helper_sum(ap, bp, res);
  }

  bigint_normalize(res);
  return res;
}