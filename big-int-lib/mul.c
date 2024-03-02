#include <assert.h>

#include "bigint.h"
#include "utils.h"

static sign_t get_sign(sign_t sgnx, sign_t sgny) {
  sign_t res;

  res = 0;

  if ((sgnx == zero) || (sgny == zero)) {
    res = zero;
  } else if (sgnx == sgny) {
    res = pos;
  } else {
    res = neg;
  }

  return res;
}

bigint_t *bigint_mul(const bigint_t *ap, const bigint_t *bp) {
  bigint_t *resp;
  size_t i, j, m, n;
  int w, carry;
  char *u, *v, *res;

  if (!ap || !bp) {
    return NULL;
  }

  resp = bigint_from_size(ap->len + bp->len);

  resp->sign = get_sign(ap->sign, bp->sign);

  n = ap->len;
  m = bp->len;
  u = ap->digits;
  v = bp->digits;
  res = resp->digits;

  for (j = 0; j < m; ++j) {
    carry = 0;
    for (i = 0; i < n; ++i) {
      w = (u[i]) * (v[j]) + res[i + j] + carry;
      res[i + j] = w % BASE;
      carry = w / BASE;
    }
    res[j + n] = carry;
  }

  bigint_normalize(resp);

  return resp;
}