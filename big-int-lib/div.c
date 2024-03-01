#include <assert.h>
#include <stdint.h>

#include "bigint.h"
#include "utils.h"

/* Perform a multiplication by a single decimal. */
static int bigint_mul_dec_pos(const bigint_t *ap, bigint_t *resp, uint8_t d) {
  size_t i;
  uint8_t w, carry;

  carry = 0;
  for (i = 0; i < ap->len; ++i) {
    w = ap->digits[i] * d + carry;
    resp->digits[i] = w % BASE;
    carry = w / BASE;
  }
  resp->digits[ap->len] = carry;

  return 0;
}

/* Perform a division by a single decimal. */
static int bigint_div_dec_pos(const bigint_t *ap, bigint_t *resp, uint8_t d) {
  size_t i;
  uint8_t r, q, sum;

  r = q = 0;
  for (i = 0; i < ap->len; ++i) {
    sum = BASE * r + ap->digits[ap->len - 1 - i];
    q = sum / d;
    r = sum % d;
    resp->digits[ap->len - 1 - i] = q;
  }

  bigint_normalize(resp);

  return 0;
}

/* Sort out almost all incorrect quesses. */
static int guess_test(uint8_t q, uint8_t r, uint8_t v, uint8_t u) {
  return ((q >= BASE) || (q * v > BASE * r + u));
}

static int mulsub(bigint_t *u, bigint_t *v, size_t n, size_t j, uint8_t q) {
  uint8_t mul, sum_carry;
  int8_t sub, sub_carry;
  size_t i;

  sum_carry = 0;
  sub_carry = 0;
  for (i = 0; i <= n; ++i) {
    mul = q * v->digits[i] + sum_carry;
    sub = u->digits[j + i] - (mul % 10) + sub_carry;
    sum_carry = mul / 10;
    sub_carry = eu_div(sub, BASE);
    sub = eu_mod(sub, BASE);
    u->digits[j + i] = sub;
  }

  return sub_carry;
}

/* This function is called when the guessed q is 1 off. */
static void addback(bigint_t *u, bigint_t *v, size_t n, size_t j) {
  size_t i;
  int8_t w, carry;

  carry = 0;
  for (i = 0; i <= n; ++i) {
    w = u->digits[j + i] + v->digits[i] + carry;
    carry = w / BASE;
    w %= BASE;
    u->digits[j + i] = w;
  }
  assert(carry == 1);
  assert(i == n + 1);
  assert(u->len >= (n + j));
  u->digits[n + j] = 0;
}

/* Normalize u and v when performing u/v division
   so that it is possible to guess q almost accurately. */
static void alg_normalize(bigint_t *ap, bigint_t *bp, bigint_t *u, bigint_t *v,
                          uint8_t d) {
  bigint_add_padding(u, 1);
  bigint_add_padding(v, 1);
  bigint_mul_dec_pos(ap, u, d);
  bigint_mul_dec_pos(bp, v, d);
  bigint_normalize(v);
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

/* Schoolbook division which utilizes math to guess the digits of quotient.
   The remainder is calculated for free. */
static void bigint_div_mod_pos(const bigint_t *ap, const bigint_t *bp,
                              bigint_t *resp) {
  bigint_t *u, *v;
  size_t j, n, m;
  uint8_t d, q;
  int8_t carry;

  if ((bp->sign != zero) && (bp->len == 1)) {
  return bigint_div_dec_pos(ap, resp, *bp->digits);
  }

  u = bigint_from_size(ap->len);
  v = bigint_from_size(bp->len);

  n = v->len;
  m = u->len - n;
  u->sign = v->sign = pos;

  /* Normalization factor for easy q guessing. */
  d = BASE / (bp->digits[n - 1] + 1);
  alg_normalize(ap, bp, u, v, d);

  /* Loop to find the resulting quotient of size no bigger than m + 1. 
     j should be set to m, but to avoid the resulting oveflow from going
     below zero, it is instead set to m + 1. */
  for (j = m + 1; j > 0; --j) {
    
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

  bigint_normalize(resp);
  bifree(u);
  bifree(v);
  return;
}

bigint_t *bigint_div(const bigint_t *ap, const bigint_t *bp) {
  bigint_t *resp;

  if (!ap || !bp || (bp->sign == zero)) {
    return NULL;
  }

  if (ap->sign == zero) {
    return bigint_from_int(0);
  }

  if ((bigint_cmp_abs(ap, bp) == -1) && (ap->sign == pos)) {
    return bigint_from_int(0);
  }

  resp = bigint_from_size(ap->len - bp->len + 1);
  resp->sign = pos;

  if (!resp) {
    return NULL;
  }

  bigint_div_mod_pos(ap, bp, resp);

  if (ap->sign == neg) {
    add_one(resp);
  }

  resp->sign = ap->sign * bp->sign;

  return resp;
}