#include "bigint.h"

bigint_t *bigint_subtract(const bigint_t *ap, const bigint_t *bp) {
  bigint_t *tmp, *res;

  if ((ap == NULL) || (bp == NULL)) {
    return NULL;
  }

  tmp = bigint_init();

  if (tmp == NULL) {
    return NULL;
  }

  tmp->sign = !(bp->sign);
  tmp->len = bp->len;
  tmp->digits = bp->digits;

  res = bigint_sum(ap, tmp);

  bifree(tmp);

  return res;
}