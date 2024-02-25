#include "bigint.h"

bigint_t *bigint_mul(const bigint_t *ap, const bigint_t *bp) {
  bigint_t *res;
  size_t i, j;
  int max_num, sum, carry;

  if ((ap == NULL) || (bp == NULL) || (ap->len == 0) || (bp->len == 0)) {
    return NULL;
  }

  max_num = bigint_max_abs(ap, bp);

  if (max_num == 1) {
    return bigint_mul(bp, ap);
  }

  assert(ap->len >= bp->len);

  res = bigint_init();

  if (res == NULL) {
    return NULL;
  }

  res->len = ap->len + bp->len;

  res->digits = calloc(res->len, sizeof(char));

  if (res->digits == NULL) {
    bifree(res);
    return NULL;
  }

  res->sign = ap->sign ^ bp->sign;

  for (j = 0; j < bp->len; ++j) {
    for (i = 0, carry = 0; i < ap->len; ++i) {
      sum = (ap->digits[i]) * (bp->digits[j]) + res->digits[i + j] + carry;
      res->digits[i + j] = sum % BASE;
      carry = sum / BASE;
    }

    res->digits[j + ap->len] = carry;
  }

  bigint_normalize(res);

  return res;
}

bigint_t *bigint_mul_dec(const bigint_t *ap, const unsigned char d) {
  bigint_t *res;
  size_t i;
  int sum, carry;

  if ((ap == NULL) || (ap->len == 0)) {
    return NULL;
  }

  if (d > 10) {
    return NULL;
  }

  if ((d == 0) || (eq_zero(ap))) {
    return bigint_from_str("0");
  }

  if (d == 1) {
    res = bigint_from_size(ap->len);

    if (res == NULL) {
      return NULL;
    }

    res->sign = ap->sign;

    res->digits = memcpy(res->digits, ap->digits, ap->len);

    return res;
  }

  res = bigint_init();

  if (res == NULL) {
    return NULL;
  }

  res->len = ap->len + 1;

  res->digits = calloc(res->len, sizeof(char));

  if (res->digits == NULL) {
    bifree(res);
    return NULL;
  }

  res->sign = ap->sign;

  for (i = 0, carry = 0; i < ap->len; ++i) {
    sum = ap->digits[i] * d + res->digits[i] + carry;
    res->digits[i] = sum % BASE;
    carry = sum / BASE;
  }

  res->digits[ap->len] = carry;

  bigint_normalize(res);

  return res;
}