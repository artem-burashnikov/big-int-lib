#include "utils.h"

bigint_t *bigint_mul_dec(const bigint_t *ptr_x, const unsigned char d) {
  bigint_t *res;
  size_t i;
  int sum, carry;

  if ((ptr_x == NULL) || (ptr_x->len == 0)) {
    return NULL;
  }

  if (d > 10) {
    return NULL;
  }

  if ((d == 0) || (eq_zero(ptr_x))) {
    return bigint_from_str("0");
  }

  if (d == 1) {
    res = bigint_from_size(ptr_x->len);

    if (res == NULL) {
      return NULL;
    }

    res->sign = ptr_x->sign;

    res->digits = memcpy(res->digits, ptr_x->digits, ptr_x->len * sizeof(char));

    return res;
  }

  res = bigint_init();

  if (res == NULL) {
    return NULL;
  }

  /* +1 for potential carry. */
  res->len = ptr_x->len + 1;

  res->digits = calloc(res->len, sizeof(char));

  if (res->digits == NULL) {
    bifree(res);
    return NULL;
  }

  res->sign = ptr_x->sign;

  for (carry = i = 0; i < ptr_x->len; ++i) {
    sum = ptr_x->digits[i] * d + res->digits[i] + carry;
    res->digits[i] = sum % BASE;
    carry = sum / BASE;
  }

  res->digits[ptr_x->len] = carry;

  bigint_normalize(res);

  return res;
}

bigint_t *bigint_mul(const bigint_t *ptr_x, const bigint_t *ptr_y) {
  bigint_t *res;
  size_t i, j;
  int max_num, sum, carry;

  res = bigint_init();

  if ((res == NULL) || (ptr_x == NULL) || (ptr_y == NULL) ||
      (ptr_x->len == 0) || (ptr_y->len == 0)) {
    return NULL;
  }

  max_num = bigint_max_abs(ptr_x, ptr_y);

  if (max_num != 0) {
    return bigint_mul(ptr_y, ptr_x);
  }

  assert(ptr_x->len >= ptr_y->len);

  res->len = ptr_x->len + ptr_y->len;

  res->digits = calloc(res->len, sizeof(char));

  if (res->digits == NULL) {
    bifree(res);
    return NULL;
  }

  res->sign = ptr_x->sign ^ ptr_y->sign;

  for (j = 0; j < ptr_y->len; ++j) {
    for (carry = i = 0; i < ptr_x->len; ++i) {
      sum =
          (ptr_x->digits[i]) * (ptr_y->digits[j]) + res->digits[i + j] + carry;
      res->digits[i + j] = sum % BASE;
      carry = sum / BASE;
    }

    res->digits[j + ptr_x->len] = carry;
  }

  bigint_normalize(res);

  return res;
}