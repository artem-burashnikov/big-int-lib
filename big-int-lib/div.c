#include "utils.h"

bigint_t *bigint_div_dec(const bigint_t *ptr_x, unsigned char d) {
  bigint_t *res;
  size_t i;
  int r, q, w;

  if ((d >= 10) || (d == 0)) {
    return NULL;
  }

  if ((ptr_x == NULL) || (ptr_x->len == 0)) {
    return NULL;
  }

  if (eq_zero(ptr_x)) {
    return bigint_from_str("0");
  }

  res = bigint_init();

  if (res == NULL) {
    return NULL;
  }

  res->len = ptr_x->len;

  res->digits = calloc(res->len, sizeof(char));

  if (res->digits == NULL) {
    bifree(res);
    return NULL;
  }

  res->sign = ptr_x->sign;

  for (r = q = i = 0; i < ptr_x->len; ++i) {
    w = BASE * r + ptr_x->digits[ptr_x->len - 1 - i];
    q = w / d;
    r = w % d;
    res->digits[ptr_x->len - 1 - i] = q;
  }

  bigint_normalize(res);

  return res;
}