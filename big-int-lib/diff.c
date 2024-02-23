#include <stdlib.h>

#include "bigint.h"

BigInt *bigint_subtract(const BigInt *x, const BigInt *y) {
  BigInt *tmp, *res;

  if ((x == NULL) || (y == NULL)) {
    return NULL;
  }

  tmp = bigint_init();

  if (tmp == NULL) {
    return NULL;
  }

  tmp->sign = !(y->sign);
  tmp->length = y->length;
  tmp->digits = y->digits;

  res = bigint_sum(x, tmp);

  free(tmp);

  return res;
}