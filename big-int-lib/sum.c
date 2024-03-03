#include <assert.h>
#include <stdint.h>

#include "bigint.h"
#include "utils.h"

static void helper_sub(const bigint_t *ap, const bigint_t *bp, bigint_t *res) {
  int8_t carry, w, r, q;
  size_t i;

  carry = 0;
  for (i = 0; i < ap->len; ++i) {
    w = ap->digits[i] + carry;
    if (i < bp->len) {
      w -= bp->digits[i];
    }
    r = eu_mod(w, BASE);
    q = eu_div(w, BASE);
    carry = q;
    res->digits[i] = r;
  }
  assert(carry == 0);

  return;
}

static void helper_sum(const bigint_t *ap, const bigint_t *bp, bigint_t *res) {
  int8_t carry, w;
  size_t i;

  carry = 0;
  for (i = 0; i < ap->len; ++i) {
    w = ap->digits[i] + carry;
    if (i < bp->len) {
      w += bp->digits[i];
    }
    carry = w / BASE;
    w %= BASE;
    res->digits[i] = w;
  }
  assert(carry >= 0);
  assert(i == ap->len);
  res->digits[i] = carry;

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

  assert(ap->len >= bp->len);

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