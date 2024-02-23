#include <assert.h>
#include <stdlib.h>

#include "bigint.h"

int bigint_normalize(BigInt *x) {
  char *tmp;
  size_t old_length, new_length, i;

  if (x == NULL) {
    return 1;
  }

  old_length = new_length = x->length;

  while ((new_length > 0) && ((x->digits[new_length - 1]) == ('0' - '0'))) {
    --new_length;
  }

  if (new_length == 0) {
    x->length = 1;
    free(x->digits);
    x->digits = calloc(1, sizeof(char));

    if (x->digits == NULL) {
      return 1;
    }
    
    x->digits[0] = '0' - '0';

    return 0;
  }

  assert(new_length > 0);

  if (new_length != old_length) {
    for (i = old_length - 1; i >= new_length; --i) {
      assert(x->digits[i] == ('0' - '0'));
    }

    tmp = realloc(x->digits, sizeof(char) * new_length);

    if (tmp == NULL) {
      return 1;
    }

    x->digits = tmp;
    x->length = new_length;
  }

  return 0;
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