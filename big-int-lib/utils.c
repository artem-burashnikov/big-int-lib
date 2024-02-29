#include "utils.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "bigint.h"

int bigint_add_padding(bigint_t* ap, const size_t t) {
  char* tmp;
  size_t old_len, new_len;
  size_t i;

  if (!ap) {
    return 1;
  }

  old_len = ap->len;
  new_len = old_len + t;

  tmp = realloc(ap->digits, sizeof(char) * new_len);

  if (!tmp) {
    return 1;
  }

  ap->digits = tmp;
  ap->len = new_len;

  for (i = old_len; i < new_len; ++i) {
    ap->digits[i] = 0;
  }

  return 0;
}

int bigint_normalize(bigint_t* ap) {
  char* tmp;
  size_t old_len, new_len, i;

  if (!ap) {
    return 1;
  }

  old_len = new_len = ap->len;

  while ((new_len > 1) && ((ap->digits[new_len - 1]) == 0)) {
    --new_len;
  }

  if (new_len != old_len) {
    for (i = old_len - 1; i >= new_len; --i) {
      assert(ap->digits[i] == 0);
    }

    tmp = realloc(ap->digits, sizeof(char) * new_len);

    if (!tmp) {
      return 1;
    }

    ap->digits = tmp;
    ap->len = new_len;

    if ((new_len == 1) && (*tmp == 0)) {
      ap->sign = zero;
    }
  }

  return 0;
}

int bigint_cmp_abs(const bigint_t* ap, const bigint_t* bp) {
  int ret;
  size_t i;

  ret = 0;
  if ((ap->len) < (bp->len)) {
    ret = -1;
  } else if ((ap->len) > (bp->len)) {
    ret = 1;
  } else {
    for (i = 0; i < ap->len; ++i) {
      if ((ap->digits[ap->len - 1 - i]) < (bp->digits[bp->len - 1 - i])) {
        ret = -1;
        break;
      } else if ((ap->digits[ap->len - 1 - i]) >
                 (bp->digits[bp->len - 1 - i])) {
        ret = 1;
        break;
      }
    }
  }
  return ret;
}

int bigint_cmp(const bigint_t* ap, const bigint_t* bp) {
  size_t i;
  int ret, sign, abs_cmp;

  ret = 0;
  abs_cmp = bigint_cmp_abs(ap, bp);
  sign = ap->sign == bp->sign;

  if (sign) {
    ret = (abs_cmp == 0) ? (0) : (-abs_cmp);
  } else if ((ap->sign == neg) && (bp->sign == pos)) {
    ret = -1;
  } else if ((ap->sign == pos) && (bp->sign == neg)) {
    ret = 1;
  }

  return ret;
}

int bigint_rshift(bigint_t* ap, size_t t) {
  char* tmp;

  if (!ap) {
    return 1;
  }

  if (t == 0) {
    return 0;
  }

  tmp = calloc(ap->len + t, sizeof(char));

  if (!tmp) {
    return 1;
  }

  memcpy(tmp + t, ap->digits, ap->len * sizeof(char));
  free(ap->digits);
  ap->digits = tmp;
  ap->len = ap->len + t;

  return 0;
}