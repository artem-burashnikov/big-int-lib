#include "bigint.h"

int bigint_normalize(bigint_t *ap) {
  char *tmp;
  size_t old_len, new_len, i;

  if (ap == NULL) {
    return 1;
  }

  old_len = new_len = ap->len;

  while ((new_len > 0) && ((ap->digits[new_len - 1]) == ('0' - '0'))) {
    --new_len;
  }

  if (new_len == 0) {
    ap->len = 1;
    free(ap->digits);
    ap->digits = calloc(1, sizeof(char));

    if (ap->digits == NULL) {
      return 1;
    }

    ap->digits[0] = '0' - '0';

    return 0;
  }

  assert(new_len > 0);

  if (new_len != old_len) {
    for (i = old_len - 1; i >= new_len; --i) {
      assert(ap->digits[i] == ('0' - '0'));
    }

    tmp = realloc(ap->digits, sizeof(char) * new_len);

    if (tmp == NULL) {
      return 1;
    }

    ap->digits = tmp;
    ap->len = new_len;
  }

  return 0;
}

int bigint_cmp(const bigint_t *ap, const bigint_t *bp) /* Needs NULL-check? */
{
  size_t i;

  if ((ap->len) != (bp->len)) {
    return 1;
  } else if ((ap->sign) != (bp->sign)) {
    return 1;
  } else {
    /* Both lengthes are identical at this point. */
    for (i = 0; i < ap->len; ++i) {
      if ((ap->digits[i]) != (bp->digits[i])) {
        return 1;
      }
    }
  }

  return 0;
}

int bigint_max_abs(const bigint_t *ap, const bigint_t *bp) /* Needs NULL-check? */
{
  int ret;
  size_t i;

  if ((ap->len) < (bp->len)) {
    ret = 1;
  } else if ((ap->len) > (bp->len)) {
    ret = 0;
  } else {
    for (i = 0; i < ap->len; ++i) {
      if ((ap->digits[ap->len - 1 - i]) < (bp->digits[bp->len - 1 - i])) {
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