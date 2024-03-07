#include "utils.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "bigint.h"

void bigint_add_padding(bigint_t* ap, const uint8_t t) {
  if (!ap) {
    return;
  }

  ap->digits = realloc(ap->digits, sizeof(char) * (ap->len + t));
  memset(ap->digits + ap->len, 0, t);

  ap->len += t;

  return;
}

void bigint_normalize(bigint_t* ap) {
  char* tmp;
  size_t old_len, new_len;

  old_len = new_len = ap->len;

  while ((new_len > 1) && (ap->digits[new_len - 1] == 0)) {
    --new_len;
  }

  if (new_len != old_len) {
    tmp = realloc(ap->digits, sizeof(char) * new_len);
    if (!tmp) {
      return;
    }
    ap->digits = tmp;
    ap->len = new_len;
  }

  if ((new_len == 1) && (*ap->digits == 0)) {
    ap->sign = zero;
  }

  return;
}

int8_t bigint_cmp_abs(const bigint_t* ap, const bigint_t* bp) {
  int8_t ret;

  ret = 0;
  if ((ap->len) < (bp->len)) {
    ret = -1;
  } else if ((ap->len) > (bp->len)) {
    ret = 1;
  } else {
    for (size_t i = 0; i < ap->len; ++i) {
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

bigint_t* bigint_mirror(const bigint_t* ap) {
  bigint_t* res;

  res = malloc(sizeof(bigint_t));

  if (!ap || !res) {
    return NULL;
  }

  res->digits = ap->digits;
  res->sign = ap->sign;
  res->len = ap->len;

  return res;
}

static int32_t iabs(int32_t x) { return (x < 0) ? -x : x; }

int32_t eu_mod(const int32_t x, const int32_t y) {
  int32_t r;
  r = x % y;
  if (r < 0) {
    r += iabs(y);
  }
  return r;
}

int32_t eu_div(const int32_t x, const int32_t y) {
  int32_t q, r;
  r = eu_mod(x, y);
  q = (x - r) / y;
  return q;
}
