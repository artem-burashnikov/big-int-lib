#include "utils.h"

#include "bigint.h"

int eq_zero(const bigint_t *ptr_x) {
  if (ptr_x == NULL) {
    return 0;
  }

  if (ptr_x->digits == NULL) {
    return 0;
  }

  if ((ptr_x->len == 1) && (ptr_x->digits[0] == 0)) {
    return 1;
  } else {
    return 0;
  }
}

int bigint_add_padding(bigint_t *ptr_x, size_t new_len) {
  char *tmp;
  size_t old_len;
  size_t i;

  old_len = ptr_x->len;

  if ((new_len - old_len) <= 0) {
    return 1;
  }

  tmp = realloc(ptr_x->digits, sizeof(char) * new_len);

  if (tmp == NULL) {
    return 1;
  }

  ptr_x->digits = tmp;
  ptr_x->len = new_len;

  for (i = old_len; i < new_len; ++i) {
    ptr_x->digits[i] = 0;
  }

  return 0;
}

int bigint_normalize(bigint_t *ptr_x) {
  char *tmp;
  size_t old_len, new_len, i;

  if (ptr_x == NULL) {
    return 1;
  }

  old_len = new_len = ptr_x->len;

  while ((new_len - 1 > 0) && ((ptr_x->digits[new_len - 1]) == 0)) {
    --new_len;
  }

  assert(new_len > 0);

  if (new_len != old_len) {
    for (i = old_len - 1; i >= new_len; --i) {
      assert(ptr_x->digits[i] == 0);
    }

    tmp = realloc(ptr_x->digits, sizeof(char) * new_len);

    if (tmp == NULL) {
      return 1;
    }

    ptr_x->digits = tmp;
    ptr_x->len = new_len;
  }

  return 0;
}

int bigint_cmp(const bigint_t *ptr_x, const bigint_t *ptr_y) {
  size_t i;

  if ((ptr_x->len) != (ptr_y->len)) {
    return 1;
  } else if ((ptr_x->sign) != (ptr_y->sign)) {
    return 1;
  } else {
    /* Both lengthes are identical at this point. */
    for (i = 0; i < ptr_x->len; ++i) {
      if ((ptr_x->digits[i]) != (ptr_y->digits[i])) {
        return 1;
      }
    }
  }

  return 0;
}

int bigint_max_abs(const bigint_t *ptr_x, const bigint_t *ptr_y) {
  int ret;
  size_t i;

  if ((ptr_x->len) < (ptr_y->len)) {
    ret = 1;
  } else if ((ptr_x->len) > (ptr_y->len)) {
    ret = 0;
  } else {
    assert(ptr_x->len == ptr_y->len);
    for (i = 0; i < ptr_x->len; ++i) {
      if ((ptr_x->digits[ptr_x->len - 1 - i]) <
          (ptr_y->digits[ptr_y->len - 1 - i])) {
        ret = 1;
        break;
      } else {
        ret = 0;
        break;
      }
    }
  }
  return ret;
}

int bigint_shift_rigt(bigint_t *ap, size_t t) {
  char *tmp;

  if (t <= 0) {
    return 0;
  }

  tmp = calloc(ap->len + t, sizeof(char));
  memcpy(tmp + t, ap->digits, ap->len);
  free(ap->digits);
  ap->digits = tmp;
  ap->len = ap->len + t;

  return 0;
}