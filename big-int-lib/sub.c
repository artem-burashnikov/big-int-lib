#include "utils.h"

bigint_t *bigint_sub(const bigint_t *ptr_x, const bigint_t *ptr_y) {
  bigint_t *tmp, *res;

  tmp = bigint_init();

  if ((ptr_x == NULL) || (ptr_y == NULL) || (tmp == NULL)) {
    return NULL;
  }

  tmp->sign = !ptr_y->sign;
  tmp->len = ptr_y->len;
  tmp->digits = ptr_y->digits;

  res = bigint_sum(ptr_x, tmp);

  free(tmp);

  return res;
}