#include "bigint.h"

#include <stdlib.h>
#include <string.h>

BigInt *bigint_init() {
  BigInt *x = malloc(sizeof(BigInt));

  return x;
}

int bifree(BigInt *x) {
  if ((x != NULL) && (x->digits != NULL)) {
    free(x->digits);
    free(x);
  } else if (x != NULL) {
    free(x);
  }

  return 0;
}

BigInt *bigint_from_size(size_t length) {
  BigInt *x = bigint_init();

  if ((length == 0) || (x == NULL)) {
    return NULL;
  }

  x->length = length;
  x->sign = positive;

  x->digits = malloc(sizeof(char) * length);
  if (x->digits != NULL) {
    return x;
  } else {
    free(x);
    return NULL;
  }
}

/* Assume base 10. */
BigInt *str_to_bigint(char *str) {
  BigInt *x;
  size_t len, i, digits_cnt;
  Sign sign;
  char c;

  if (str == NULL) {
    return NULL;
  }

  len = strlen(str);
  digits_cnt = len;

  /* Empry input string. */
  if (len == 0) {
    return NULL;
  }

  if ((len == 1) && (str[0] == '0')) {
    x = bigint_from_size(len);

    if (x == NULL) {
      return NULL;
    }

    x->sign = positive;
    x->digits[0] = '0' - '0';
    return x;
  }

  /* String only contains a sign. */
  if ((len == 1) && ((str[0] == '-') || (str[0] == '+'))) {
    return NULL;
  }

  /* If the first character in a string is a sign,
     the number of digits is less than the string's length. */
  if ((len > 1) && (str[0] == '-')) {
    digits_cnt--;
    sign = negative;
  } else if (len > 1) {
    sign = positive;
  }

  i = sign;
  /* Ignore leading zeroes. */
  while ((c = str[i++]) == '0') {
    --digits_cnt;
  }

  x = bigint_from_size(digits_cnt);

  if (x == NULL) {
    return NULL;
  }

  x->sign = sign;

  for (i = 0; i < digits_cnt; ++i) {
    c = str[len - 1 - i];

    /* Only numeric strings are allowed. */
    if ((c >= '0') && (c <= '9')) {
      x->digits[i] = c - '0';
    } else {
      bifree(x);
      return NULL;
    }
  }

  return x;
}

char *bigint_to_str(const BigInt *x) {
  char *str;
  size_t str_len, i;

  if (x == NULL) {
    return NULL;
  }

  /* +1 for terminator character '\0' */
  str_len = x->length + x->sign + 1;
  str = calloc(str_len, sizeof(char));

  if (str == NULL) {
    return NULL;
  }

  if (x->sign == negative) {
    str[0] = '-';
  }

  for (i = 0; i < x->length; ++i) {
    str[x->length + x->sign - 1 - i] = x->digits[i] + '0';
  }

  str[str_len - 1] = '\0';

  return str;
}