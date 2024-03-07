#include <stdint.h>

#include "bigint.h"
#include "utils.h"

void add_one(bigint_t *ap) {
  size_t n;
  int8_t carry, w;

  n = ap->len;

  bigint_add_padding(ap, 1);

  carry = 0;
  for (size_t i = 0; i < n; ++i) {
    if (i == 0) {
      w = ap->digits[i] + carry + 1;
    } else {
      w = ap->digits[i] + carry;
    }
    ap->digits[i] = w % BASE;
    carry = w / BASE;
  }
  ap->digits[n] = carry;
  bigint_normalize(ap);
  return;
}

/* Perform a multiplication by a single decimal. */
static void bigint_mul_dec_pos(const bigint_t *ap, bigint_t *resp, uint8_t d) {
  uint8_t w, carry;

  carry = 0;
  for (size_t i = 0; i < ap->len; ++i) {
    w = ap->digits[i] * d + carry;
    resp->digits[i] = w % BASE;
    carry = w / BASE;
  }
  resp->digits[ap->len] = carry;
  return;
}

/* Perform a division by a single decimal. */
static void bigint_div_dec_pos(const bigint_t *ap, bigint_t *resp, uint8_t d) {
  uint8_t w, r;

  r = 0;
  for (size_t i = 0; i < ap->len; ++i) {
    w = BASE * r + ap->digits[ap->len - 1 - i];
    resp->digits[ap->len - 1 - i] = w / d;
    r = w % d;
  }

  bigint_normalize(resp);
  return;
}

/* Sort out almost all incorrect quesses of digit of quotient. */
static int8_t guess_test(uint8_t q, uint8_t r, uint8_t v, uint8_t u) {
  return ((q >= BASE) || (q * v > BASE * r + u));
}

static int8_t mulsub(bigint_t *u, bigint_t *v, size_t n, size_t j, uint8_t q) {
  int8_t mul, sum_carry, sub, sub_carry;

  sum_carry = sub_carry = 0;
  for (size_t i = 0; i <= n; ++i) {
    mul = q * v->digits[i] + sum_carry;
    sub = u->digits[j + i] - (mul % BASE) + sub_carry;
    u->digits[j + i] = eu_mod(sub, BASE);
    sum_carry = mul / BASE;
    sub_carry = eu_div(sub, BASE);
  }

  return sub_carry;
}

/* This function is called when the guessed q was 1 off. */
static void addback(bigint_t *u, bigint_t *v, size_t n, size_t j) {
  int8_t w, carry;

  carry = 0;
  for (size_t i = 0; i <= n; ++i) {
    w = u->digits[j + i] + v->digits[i] + carry;
    u->digits[j + i] = w % BASE;
    carry = w / BASE;
  }
  u->digits[n + j] = 0;

  return;
}

/* Normalize u and v when performing u/v division (u/v == (ud)/(vd))
   so that it is possible to guess q with high precision. */
static void alg_normalize(const bigint_t *ap, const bigint_t *bp, bigint_t *u,
                          bigint_t *v, uint8_t d) {
  bigint_add_padding(u, 1);
  bigint_add_padding(v, 1);
  bigint_mul_dec_pos(ap, u, d);
  bigint_mul_dec_pos(bp, v, d);
  bigint_normalize(v);

  return;
}

static uint8_t guessq(char *u, char *v, size_t j, size_t n) {
  uint8_t q, r;
  /* Guess q, r. */
  q = (u[j + n] * BASE + u[j + n - 1]) / v[n - 1];
  r = (u[j + n] * BASE + u[j + n - 1]) % v[n - 1];

  /* These two steps eliminate all guesses that are too big. */
  if (guess_test(q, r, v[n - 2], u[j + n - 2])) {
    q -= 1;
    r += v[n - 1];

    if (r < BASE) {
      if (guess_test(q, r, v[n - 2], u[j + n - 2])) {
        q -= 1;
        r += v[n - 1];
      }
    }
  }
  return q;
}

/* Schoolbook division which utilizes Knuth's Algorithm D section 4.3.1. */
static bigint_t *bigint_div_mod_pos(const bigint_t *ap, const bigint_t *bp) {
  bigint_t *resp, *u, *v;
  size_t n, m;
  uint8_t d, q;
  int8_t carry;

  resp = bigint_from_size(ap->len - bp->len + 1);
  resp->sign = pos;

  if (!resp) {
    return NULL;
  }

  if ((bp->sign != zero) && (bp->len == 1)) {
    bigint_div_dec_pos(ap, resp, *bp->digits);
    return resp;
  }

  u = bigint_from_size(ap->len);
  v = bigint_from_size(bp->len);

  n = v->len;
  m = u->len - n;

  /* Normalization factor for easy q guessing. */
  d = BASE / (bp->digits[n - 1] + 1);
  alg_normalize(ap, bp, u, v, d);

  /* Loop to find the resulting quotient of size no bigger than m + 1.
     j should be set to m, but to avoid the resulting oveflow from going
     below zero, it is instead set to m + 1. */
  for (size_t j = m + 1; j > 0; --j) {
    q = guessq(u->digits, v->digits, (j - 1), n);

    bigint_add_padding(v, 1);
    carry = mulsub(u, v, n, (j - 1), q);

    /* Addback if neccessary. */
    if (carry == -1) {
      q -= 1;
      addback(u, v, n, (j - 1));
    }

    resp->digits[j - 1] = q;
    bigint_normalize(v);
  }

  bifree(u);
  bifree(v);

  bigint_normalize(resp);
  return resp;
}

bigint_t *bigint_div(const bigint_t *ap, const bigint_t *bp) {
  bigint_t *resp;
  int8_t cmp;

  if (!ap || !bp || (bp->sign == zero)) {
    return NULL;
  }

  cmp = bigint_cmp_abs(ap, bp);
  if ((cmp == -1) && (ap->sign != neg)) {
    return bigint_from_int(0);
  } else if ((cmp == -1) && (bp->sign == pos)) {
    return bigint_from_int(-1);
  } else if ((cmp == -1) && (bp->sign == neg)) {
    return bigint_from_int(1);
  }
  resp = bigint_div_mod_pos(ap, bp);

  if (ap->sign == neg) {
    add_one(resp);
  }

  if (resp->sign != zero) {
    resp->sign = ap->sign * bp->sign;
  }

  return resp;
}