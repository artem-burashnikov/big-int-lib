#include <stdint.h>

#include "bigint.h"
#include "utils.h"

bigint_t *bigint_mul(const bigint_t *ap, const bigint_t *bp) {
  bigint_t *resp;
  size_t m, n;
  int8_t w, carry;
  char *u, *v, *res;

  if (!ap || !bp) {
    return NULL;
  }

  resp = bigint_from_size(ap->len + bp->len);

  resp->sign = ap->sign * bp->sign;

  n = ap->len;
  m = bp->len;
  u = ap->digits;
  v = bp->digits;
  res = resp->digits;

  for (size_t j = 0; j < m; ++j) {
    carry = 0;
    for (size_t i = 0; i < n; ++i) {
      w = (u[i]) * (v[j]) + res[i + j] + carry;
      res[i + j] = w % BASE;
      carry = w / BASE;
    }
    res[j + n] = carry;
  }

  bigint_normalize(resp);

  return resp;
}
