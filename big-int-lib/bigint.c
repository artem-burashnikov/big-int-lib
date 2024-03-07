#include "bigint.h"

#include <ctype.h>
#include <stdint.h>
#include <string.h>

#include "utils.h"

static size_t get_num_digits(const uint64_t n) {
  size_t len;
  uint64_t x;

  for (len = 0, x = n; x > 0; x /= BASE) {
    len += 1;
  }
  return len;
}

void bifree(bigint_t *ap) {
  if (ap) {
    free(ap->digits);
    free(ap);
  }
  return;
}

bigint_t *bigint_from_size(const size_t len) {
  bigint_t *ap = malloc(sizeof(bigint_t));
  char *tmp = calloc(len, sizeof(char));

  if (!ap || !tmp || !len) {
    free(tmp);
    free(ap);
    return NULL;
  }

  ap->len = len;
  ap->sign = zero;
  ap->digits = tmp;
  return ap;
}

bigint_t *bigint_from_str(const char *str) {
  bigint_t *ap;
  int8_t sign;
  size_t len, digits_cnt;

  if (!str || ((len = strlen(str)) == 0)) {
    return NULL;
  }

  digits_cnt = len;

  /* Pick a leading sign if any. */
  if ((*str == '-')) {
    --digits_cnt;
    sign = neg;
  } else {
    sign = pos;
    if (*str == '+') {
      --digits_cnt;
    }
  }

  /* Ignore leading zeroes. */
  while ((digits_cnt > 0) && (str[len - digits_cnt] == '0')) {
    --digits_cnt;
  }

  /* Single zero is a zero element. */
  if (digits_cnt == 0) {
    ap = bigint_from_size(1);
    *ap->digits = 0;
    return ap;
  }

  ap = bigint_from_size(digits_cnt);

  if (!ap) {
    return NULL;
  }

  ap->sign = sign;

  for (size_t i = 0; i < digits_cnt; ++i) {
    char c = str[len - 1 - i];
    if (isdigit(c)) {
      ap->digits[i] = c - '0';
    } else {
      bifree(ap);
      return NULL;
    }
  }

  return ap;
}

static bigint_t *bigint_from_digits(const uint64_t n, const size_t digits_cnt) {
  bigint_t *retp;

  retp = bigint_from_size(digits_cnt);

  if (!retp || !retp->digits) {
    return NULL;
  }

  if (n == 0) {
    *retp->digits = 0;
  } else {
    for (size_t i = 0, x = n; x > 0; i++, x /= BASE) {
      retp->digits[i] = x % BASE;
    }
    retp->sign = pos;
  }

  return retp;
}

static bigint_t *bigint_zero(void) { return bigint_from_digits(0, 1); }

static bigint_t *bigint_from_uint(const uint64_t n) {
  bigint_t *retp;
  size_t digits_cnt;

  if (n == 0) {
    return bigint_zero();
  }

  digits_cnt = get_num_digits(n);

  retp = bigint_from_digits(n, digits_cnt);

  return retp;
}

bigint_t *bigint_from_int(int32_t n) {
  bigint_t *retp;
  if (n == 0) {
    retp = bigint_zero();
  } else if (n > 0) {
    retp = bigint_from_uint(n);
  } else {
    retp = bigint_from_uint(-(uint64_t)n);
    if (!retp) {
      return NULL;
    }
    retp->sign = neg;
  }
  return retp;
}

int32_t bigint_to_int(const bigint_t *ap) {
  int32_t i, p, res;

  if (ap->sign == zero) {
    return 0;
  }

  for (p = 1, res = 0, i = 0; i < ap->len; ++i) {
    res += p * ap->digits[i];
    p *= BASE;
  }

  if (ap->sign == neg) {
    res = -res;
  }

  return res;
}

char *bigint_to_str(const bigint_t *ap) {
  char *str;
  int8_t sign;
  size_t str_len;

  if (!ap) {
    return NULL;
  }

  sign = (ap->sign == neg);

  str_len = ap->len + sign;

  /* +1 for str term '\0' */
  str = calloc(str_len + 1, sizeof(char));

  if (!str) {
    return NULL;
  }

  if (sign) {
    *str = '-';
  }

  for (size_t i = 0; i < ap->len; ++i) {
    str[ap->len + sign - 1 - i] = ap->digits[i] + '0';
  }

  str[str_len] = '\0';

  return str;
}

int8_t bigint_cmp(const bigint_t *ap, const bigint_t *bp) {
  int8_t ret, sign, abs_cmp;

  ret = 0;
  abs_cmp = bigint_cmp_abs(ap, bp);
  sign = ap->sign == bp->sign;

  if (sign) {
    ret = (abs_cmp == 0) ? (0) : (-abs_cmp);
  } else if (ap->sign < bp->sign) {
    ret = -1;
  } else {
    ret = 1;
  }

  return ret;
}

bigint_t *bigint_cpy(const bigint_t *ap) {
  bigint_t *res;

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
  memcpy(res->digits, ap->digits, ap->len * sizeof(char));

  return res;
}
