#include "bigint.h"
#include "utils.h"

bigint_t *bigint_sub(const bigint_t *ap, const bigint_t *bp) {
  bigint_t *v, *res;

  v = bigint_mirror(bp);

  if (!ap || !bp || !v) {
    return NULL;
  }

  v->sign = -bp->sign;

  res = bigint_sum(ap, v);

  free(v);

  return res;
}
