#include "bigint.h"

bigint_t *bigint_init() {
  bigint_t *ap = malloc(sizeof(bigint_t));

  return ap;
}

int bifree(bigint_t *ap) {
  if ((ap != NULL) && (ap->digits != NULL)) {
    free(ap->digits);
    free(ap);
  } else if (ap != NULL) {
    free(ap);
  }

  return 0;
}

bigint_t *bigint_from_size(size_t len) {
  bigint_t *ap = bigint_init();

  if ((len == 0) || (ap == NULL)) {
    return NULL;
  }

  ap->len = len;
  ap->sign = pos;

  ap->digits = calloc(len, sizeof(char));
  if (ap->digits != NULL) {
    return ap;
  } else {
    free(ap);
    return NULL;
  }
}

/* Assume base 10. */
bigint_t *bigint_from_str(char *str) {
  bigint_t *ap;
  size_t i, sign, len, digits_cnt;
  char c;

  if ((str == NULL) || ((len = strlen(str)) == 0)) {
    return NULL;
  }

  assert(len > 0);

  digits_cnt = len;

  /* String only contains a sign. */
  if ((len == 1) && ((str[0] == '-') || ((len == 1) && (str[0] == '+')))) {
    return NULL;
  }

  /* If the first character in a string is a sign,
     the number of digits is less than the string's length. */
  if ((str[0] == '-')) {
    digits_cnt--;
    sign = neg;
  } else {
    sign = pos;
  }

  i = sign;
  /* Ignore leading zeroes. A single zero left is still a significant digit. */
  while ((digits_cnt > 1) && ((c = str[i++]) == '0')) {
    --digits_cnt;
  }

  ap = bigint_from_size(digits_cnt);

  if (ap == NULL) {
    return NULL;
  }

  ap->sign = sign;

  for (i = 0; i < digits_cnt; ++i) {
    c = str[len - 1 - i];
    if ((c >= '0') && (c <= '9')) {
      ap->digits[i] = c - '0';
    } else {
      bifree(ap);
      return NULL;
    }
  }

  return ap;
}

char *bigint_to_str(const bigint_t *ap) {
  char *str;
  size_t str_len, i;

  if (ap == NULL) {
    return NULL;
  }

  /* +1 for str term '\0' */
  str_len = ap->len + ap->sign + 1;
  str = calloc(str_len, sizeof(char));

  if (str == NULL) {
    return NULL;
  }

  if (ap->sign == neg) {
    str[0] = '-';
  }

  for (i = 0; i < ap->len; ++i) {
    str[ap->len + ap->sign - 1 - i] = ap->digits[i] + '0';
  }

  str[str_len - 1] = '\0';

  return str;
}