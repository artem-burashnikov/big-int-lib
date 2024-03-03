#include <stdint.h>

#include "bigint.h"
#include "utils.h"

bigint_t *bigint_mod(const bigint_t *ap, const bigint_t *bp) {
  bigint_t *r, *q, *qb;

  r = q = qb = NULL;

  if (!ap || !bp) {
    return r;
  }

  q = bigint_div(ap, bp);
  qb = bigint_mul(bp, q);
  r = bigint_sub(ap, qb);

  bifree(q);
  bifree(qb);

  return r;
}