#include "bigint.h"

#include <assert.h>
#include <ctype.h>
#include <string.h>

#include "utils.h"

static size_t get_num_digits(const int n) {
  size_t len;
  int x;

  for (len = 0, x = n; x > 0; x /= 10) {
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

  if ((ap == NULL) || (tmp == NULL) || (!len)) {
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
  size_t i, sign, len, digits_cnt;
  char c;

  if (!str || ((len = strlen(str)) == 0)) {
    return NULL;
  }

  digits_cnt = len;

  /* If the first character in a string is a sign,
     the number of digits is less than the string's length. */
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
  for (i = (sign == neg); (digits_cnt > 0) && ((c = str[i++]) == '0');
       --digits_cnt) {
    ;
  }

  if (digits_cnt == 0) {
    ap = bigint_from_size(1);
    ap->sign = zero;
    *ap->digits = 0;
    return ap;
  }

  ap = bigint_from_size(digits_cnt);

  if (!ap) {
    return NULL;
  }

  ap->sign = sign;

  for (i = 0; i < digits_cnt; ++i) {
    c = str[len - 1 - i];
    if (isdigit(c)) {
      ap->digits[i] = c - '0';
    } else {
      bifree(ap);
      return NULL;
    }
  }

  return ap;
}

static bigint_t *bigint_from_digits(const size_t n, const size_t digits_cnt) {
  bigint_t *retp;
  size_t i, x;
  char d;

  retp = bigint_from_size(digits_cnt);

  if (!retp) {
    return NULL;
  }

  x = n;
  if (x == 0) {
    retp->digits[0] = 0;
    assert(retp->sign == zero);
  } else {
    for (i = 0; x > 0; i++, x /= 10) {
      d = x % 10;
      retp->digits[i] = d;
    }
    retp->sign = pos;
  }

  return retp;
}

static bigint_t *bigint_zero(void) { return bigint_from_digits(0, 1); }

static bigint_t *bigint_from_uint(const size_t n) {
  bigint_t *retp;
  size_t digits_cnt;

  if (n == 0) {
    return bigint_zero();
  }

  digits_cnt = get_num_digits(n);

  assert(digits_cnt != 0);

  retp = bigint_from_digits(n, digits_cnt);

  return retp;
}

bigint_t *bigint_from_int(int n) {
  bigint_t *retp;
  if (n == 0) {
    retp = bigint_zero();
  } else if (n > 0) {
    retp = bigint_from_uint(n);
  } else {
    int x = -n;
    retp = bigint_from_uint(x);
    if (!retp) {
      return NULL;
    }
    retp->sign = neg;
  }
  return retp;
}

int bigint_to_int(const bigint_t *ap) {
  int i, p, res;

  if (!ap) {
    return NULL;
  }

  if (ap->sign != zero) {
    for (p = 1, res = 0, i = 0; i < ap->len; ++i) {
      res += p * ap->digits[i];
      p *= 10;
    }
  }

  if (ap->sign == zero) {
    res = 0;
  }

  if (ap->sign == neg) {
    res = -res;
  }

  return res;
}

char *bigint_to_str(const bigint_t *ap) {
  char *str;
  int sign;
  size_t str_len, i;

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

  for (i = 0; i < ap->len; ++i) {
    str[ap->len + sign - 1 - i] = ap->digits[i] + '0';
  }

  str[str_len] = '\0';

  return str;
}