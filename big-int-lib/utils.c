#include <stdlib.h>

#include "bigint.h"

int bigint_add_padding(BigInt *x, size_t new_length) {
  char *tmp;
  size_t old_length;
  size_t i;

  if (x == NULL) {
    return 1;
  }

  old_length = x->length;

  if ((new_length - old_length) <= 0) {
    return 1;
  }

  tmp = realloc(x->digits, sizeof(char) * new_length);

  if (tmp == NULL) {
    return 1;
  }

  x->digits = tmp;
  x->length = new_length;

  for (i = old_length; i < new_length; ++i) {
    x->digits[i] = 0;
  }

  return 0;
}

int bigint_normalize(BigInt *x) {
  char *tmp;
  size_t old_length, new_length, i;

  if (x == NULL) {
    return 1;
  }

  old_length = new_length = x->length;
  i = new_length - 1;

  while ((x->digits[i--]) == 0) {
    ;
  }

  new_length = i + 1;

  if (new_length != old_length) {
    tmp = realloc(x->digits, sizeof(char) * new_length);

    if (tmp == NULL) {
      return 1;
    }

    x->digits = tmp;
    x->length = new_length;
  }

  return 0;
}

int sgn(const BigInt *x) {
  if (x->sign) {
    return -1;
  } else if ((!x->sign) && (x->length == 1) && (x->digits[0] == 0)) {
    return 0;
  } else {
    return 1;
  }
}

int bigint_cmp(const BigInt *x, const BigInt *y) /* Needs NULL-check? */
{
  size_t i;

  if ((x->length) != (y->length)) {
    return 1;
  } else if ((x->sign) != (y->sign)) {
    return 1;
  } else {
    /* Both lengthes are identical at this point. */
    for (i = 0; i < x->length; ++i) {
      if ((x->digits[i]) != (y->digits[i])) {
        return 1;
      }
    }
  }

  return 0;
}

int bigint_max_abs(const BigInt *x, const BigInt *y) /* Needs NULL-check? */
{
  int ret;
  size_t i;

  if ((x->length) < (y->length)) {
    ret = 1;
  } else if ((x->length) > (y->length)) {
    ret = 0;
  } else {
    for (i = 0; i < x->length; ++i) {
      if ((x->digits[x->length - 1 - i]) < (y->digits[y->length - 1 - i])) {
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