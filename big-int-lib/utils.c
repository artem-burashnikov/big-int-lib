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
  }

  if ((new_len == 1) && (*ap->digits == 0)) {
    ap->sign = zero;
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

bigint_t* bigint_cpy(const bigint_t* ap) {
  bigint_t* res;

  res = malloc(sizeof(bigint_t));

  if (!ap || !res) {
    return NULL;
  }

  res->digits = calloc(ap->len, sizeof(char));

  if (!res->digits) {
    bifree(res);
    return NULL;
  }

  res->sign = ap->sign;
  res->len = ap->len;
  assert(res->len == ap->len);
  memcpy(res->digits, ap->digits, ap->len * sizeof(char));

  return res;
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

int iabs(int x) { return (x < 0) ? -x : x; }

int eu_mod(const int x, const int y) {
  int r;
  assert(y != 0);
  r = x % y;
  if (r < 0) {
    r += iabs(y);
  }
  return r;
}

int eu_div(const int x, const int y) {
  int q, r;
  assert(y != 0);
  r = eu_mod(x, y);
  q = (x - r) / y;
  return q;
}

sign_t rev_sign(sign_t sgn) {
  sign_t ret = -1 * sgn;
  return ret;
}

void add_one(bigint_t *ap) {
  size_t i, n;
  int carry, sum;

  n = ap->len;

  bigint_add_padding(ap, 1);

  carry = 0;
  for (i = 0; i < n; ++i) {
    if (i == 0) {
      sum = ap->digits[i] + carry + 1;
    } else {
      sum = ap->digits[i] + carry;
    }
    carry = sum / BASE;
    sum %= BASE;
    ap->digits[i] = sum;
  }
  assert(ap->len == (n+1));
  ap->digits[n] = carry;
  bigint_normalize(ap);
  return;
}