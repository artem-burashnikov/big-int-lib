#include "bigint.h"
#include "utils.h"

bigint_t *bigint_sum(const bigint_t *ptr_x, const bigint_t *ptr_y) {
  bigint_t *res;
  size_t i, max_len;
  int max_num, sum, carry;

  if ((ptr_x == NULL) || (ptr_y == NULL) || (ptr_x->len == 0) ||
      (ptr_y->len == 0)) {
    return NULL;
  }

  max_num = bigint_max_abs(ptr_x, ptr_y);

  if (max_num != 0) {
    return bigint_sum(ptr_y, ptr_x);
  }

  assert(ptr_x->len >= ptr_y->len);

  max_len = ptr_x->len;

  /* +1 for possbile carry. */
  res = bigint_from_size(max_len + 1);

  if (res == NULL) {
    return NULL;
  }

  res->sign = ptr_x->sign;

  assert(ptr_x->len >= ptr_y->len);

  for (i = 0, carry = 0; i < max_len; ++i) {
    sum = ptr_x->digits[i] + carry;

    /* Bascically a subtraction if signs are different. */
    if (ptr_x->sign != ptr_y->sign) {
      if (i < ptr_y->len) {
        sum = sum - ptr_y->digits[i];
        carry = 0;
        if (sum < 0) {
          carry = -1;
          sum += BASE;
        }
      } else {
        carry = 0;
      }
      /* Actual addition if signs are equal. */
    } else if (ptr_x->sign == ptr_y->sign) {
      if (i < ptr_y->len) {
        carry = (sum + ptr_y->digits[i]) / BASE;
        sum = (sum + ptr_y->digits[i]) % BASE;
      }
    }
    res->digits[i] = sum;
  }

  assert(i == max_len);
  assert(carry >= 0);

  res->digits[i] = carry;

  bigint_normalize(res);

  if (res == NULL) {
    return NULL;
  }

  if ((res->len == 1) && (res->digits[0] == 0)) {
    res->sign = pos;
  }

  return res;
}