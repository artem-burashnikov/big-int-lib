#include <assert.h>
#include <stdlib.h>

#include "bigint.h"

BigInt *bigint_sum(const BigInt *x, const BigInt *y) {
  BigInt *res;
  size_t max_length;
  size_t i;
  int max_num;
  int sum, carry;

  if ((x == NULL) || (y == NULL)) {
    return NULL;
  }

  max_num = bigint_max_abs(x, y);

  if (max_num == 1) {
    return bigint_sum(y, x);
  }

  /* At this point |x| >= |y|. */
  max_length = x->length;

  res = bigint_from_size(max_length + 1);

  if (res == NULL) {
    return NULL;
  }

  res->sign = x->sign;

  carry = 0;

  assert(x->length >= y->length);

  for (i = 0; i < max_length; ++i) {
    sum = x->digits[i] + carry;
    carry = 0;

    if (x->sign != y->sign) {
      if (i < y->length) {
        sum = sum - y->digits[i];
      }
      if (sum < 0) {
        sum += 10;
        carry = -1;
      }
    } else if (x->sign == y->sign) {
      if (i < y->length) {
        sum = sum + y->digits[i];
      }
      if (sum >= 10) {
        sum -= 10;
        carry = 1;
      }
    }

    res->digits[i] = sum;
  }

  assert(i == max_length);
  assert(carry >= 0);

  res->digits[i] = carry;

  bigint_normalize(res);

  if (res == NULL) {
    return NULL;
  }

  if ((res->length == 1) && (res->digits[0] == ('0' - '0'))) {
    free(res->digits);
    res->digits = calloc(1, sizeof(char));

    if (res->digits == NULL) {
      return NULL;
    }
    
    res->digits[0] = '0' - '0';
    res->sign = positive;

  }

  return res;
}