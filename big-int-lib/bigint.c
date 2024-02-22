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

  if (x == NULL) {
    return NULL;
  }

  x->length = length;
  x->sign = false;

  x->digits = malloc(sizeof(char) * length);
  if (x->digits != NULL) {
    return x;
  } else {
    free(x);
    return NULL;
  }
}

/* Assume base 10 */
BigInt *str_to_bigint(char *str) {
  BigInt *x;
  /* Length of the input string. */
  size_t len;
  /* Actual significant digits count. */
  size_t digits_cnt;
  bool sign;
  size_t i;
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

  /* String only contains a sign. */
  if ((len == 1) && ((str[0] == '-') || (str[0] == '+'))) {
    return NULL;
  }

  /* If the first character in a string is a sign,
     the number of digits is less than the string's length. */
  if ((len > 1) && (str[0] == '-')) {
    digits_cnt--;
    sign = true;
  } else if (len > 1) {
    sign = false;
  }

  x = bigint_from_size(digits_cnt);

  if (x == NULL) {
    return NULL;
  }

  x->sign = sign;

  for (i = 0; i < digits_cnt; ++i) {
    c = str[len - 1 - i];

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
  size_t str_len, num_digits, i, j;

  if (x == NULL) {
    return NULL;
  }

  str_len = x->length;
  num_digits = x->length;
  i = j = 0;

  if (x->sign) {
    ++str_len;
  }

  str = calloc(str_len + 1, sizeof(char));

  if (str == NULL) {
    return NULL;
  }

  if (x->sign) {
    str[0] = '-';
  }

  for (i = 0; i < x->length; ++i) {
    str[i] = x->digits[num_digits - 1 - i] + '0';
  }

  str[i] = '\0';

  return str;
}