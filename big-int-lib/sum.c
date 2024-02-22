#include "bigint.h"

BigInt *bigint_sum(const BigInt *x, const BigInt *y) {
  BigInt *res;
  size_t max_length;
  size_t i;
  int sgn_x, sgn_y;
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

  sgn_x = sgn(x);

  sgn_y = sgn(y);

  carry = 0;

  for (i = 0; i <= max_length; ++i) {
    res->digits[i] = 0;

    if ((i < x->length) && (i < y->length)) {
      sum = sgn_x * (x->digits[i]) + sgn_y * (y->digits[i]) + carry;
    } else if ((i < x->length) && (i >= y->length)) {
      sum = sgn_x * (x->digits[i]) + carry;
    } else if ((i >= x->length) && (i < y->length)) {
      sum = sgn_y * (y->digits[i]) + carry;
    } else {
      sum = carry;
    }

    if (!x->sign && !y->sign) {
      if (sum >= 10) {
        sum = sum % 10;
        carry = 1;
      } else if ((sum > 0) && (sum < 10)) {
        sum = sum % 10;
        carry = 0;
      }
    }
    if (!x->sign && y->sign) {
      if (sum < 0) {
        sum = (sum + 10);
        carry = -1;
      } else if (sum >= 0) {
        carry = 0;
      }
    }
    if (x->sign && y->sign) {
      if (sum <= -10) {
        sum = (-sum) % 10;
        carry = -1;
      } else if ((sum > -10) && (sum <= 0)) {
        sum = (-sum) % 10;
        carry = 0;
      }
    } else if (x->sign && !y->sign) {
      if (sum > 0) {
        sum = (-sum + 10) % 10;
        carry = -1;
      } else if (sum == 0) {
        carry = 0;
      } else if (sum < 0) {
        sum = (-sum) % 10;
        carry = 0;
      }
    }
    res->digits[i] = sum;
  }

  if (bigint_normalize(res) == 0) {
    return res;
  } else {
    return NULL;
  }
}