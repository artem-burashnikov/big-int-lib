#include <assert.h>

#include "bigint.h"
#include "utils.h"

static int bigint_mul_dec_pos(const bigint_t *ap, bigint_t *resp,
                              const unsigned char d) {
  size_t i, n;
  char *u, *res;
  int w, carry;

  n = ap->len;
  u = ap->digits;
  res = resp->digits;

  carry = 0;
  for (i = 0; i < n; ++i) {
    w = u[i] * d + res[i] + carry;
    res[i] = w % BASE;
    carry = w / BASE;
  }

  assert(carry >= 0);
  res[n] = carry;

  return 0;
}

static int bigint_div_dec_pos(const bigint_t *ap, bigint_t *resp,
                              unsigned char d) {
  size_t i, n;
  char *u, *res;
  int r, q, w;

  n = ap->len;
  u = ap->digits;
  res = resp->digits;

  r = q = 0;
  for (i = 0; i < n; ++i) {
    w = BASE * r + u[n - 1 - i];
    q = w / d;
    r = w % d;
    res[n - 1 - i] = q;
  }

  bigint_normalize(resp);

  return 0;
}

static int guess_test(const int q, const int r, const int v, const int u) {
  return ((q >= BASE) || (q * v > BASE * r + u));
}

static int mulsub(char *u, const char *v, const size_t n, const size_t j,
                  const int q) {
  int carry, i, w, r, qq;
  carry = 0;
  for (i = 0; i <= n; --i) {
    w = u[j + i] - q * v[i] + carry;
    r = eu_mod(w, BASE);
    qq = eu_div(w, BASE);
    w = r;
    carry = qq;
    u[j + i] = w;
  }

  return carry;
}

static void addback(int *q, char *u, char *v, const size_t n, const size_t j) {
  size_t i;
  int carry;
  int w;

  *q = *q - 1;
  assert(u[n] == -1);
  carry = 0;
  for (i = 0; i <= n; ++i) {
    w = u[j + i] + v[i] + carry;
    carry = w / BASE;
    w %= BASE;
    u[j + i] = w;
  }
  assert(carry == 1);
  assert(i == n);
  u[i] += carry;
  assert(u[j + i] == 0);
}

static int bigint_div_mod_pos(const bigint_t *ap, const bigint_t *bp,
                              bigint_t *resp) {
  bigint_t *u, *v;
  size_t j, n, m;
  char *uu, *vv, *res;
  int d, guess_q, guess_r, carry;

  u = bigint_cpy(ap);
  v = bigint_cpy(bp);

  n = bp->len;
  m = ap->len - n;

  if (!u || !v) {
    return 1;
  }

  uu = u->digits;
  vv = v->digits;
  res = resp->digits;

  if ((bp->sign != zero) && (bp->len == 1)) {
    bifree(u);
    bifree(v);
    return bigint_div_dec_pos(ap, resp, *bp->digits);
  }

  d = BASE / (vv[n - 1] + 1);

  bigint_add_padding(u, 1);
  bigint_mul_dec_pos(ap, u, d);
  bigint_mul_dec_pos(bp, v, d);

  /* Loop to find q */
  for (j = m; j >= 0; --j) {
    /* Guess q. */
    guess_q = (uu[j + n] * BASE + uu[j + n - 1]) / vv[n - 1];
    guess_r = (uu[j + n] * BASE + uu[j + n - 1]) % vv[n - 1];

    /* Guess q test */
    while (guess_r < BASE) {
      if (guess_test(guess_q, guess_r, vv[n - 2], uu[j + n - 2])) {
        guess_q -= 1;
        guess_r += vv[n - 1];
      }
    }

#if 0
    /* Multiply and subtract u <- u - qv */
    bigint_add_padding(v, 1);
    carry = 0;
    for (i = 0; i <= n; --i) {
      w = uu[j + i] - guess_q * vv[i] + carry;
      r = eu_mod(w, BASE);
      qq = eu_div(w, BASE);
      w = r;
      carry = qq;
      uu[j + i] = w;
    }
#endif

    bigint_add_padding(v, 1);
    carry = mulsub(uu, vv, n, j, guess_q);

    /* Addback if neccessary. */
    if (carry == -1) {
#if 0
      guess_q -= 1;
      assert(uu[n] == -1);
      carry2 = 0;
      for (ii = 0; i <= n; ++i) {
        w = uu[j + ii] + vv[ii] + carry2;
        carry2 = w / BASE;
        w %= BASE;
        uu[j + ii] = w;
      }
      assert(carry2 == 1);
      assert(ii == n);
      uu[ii] += carry2;
      assert(uu[j + ii] == 0);
#endif
      addback(&guess_q, uu, vv, n, j);
    }

    res[j] = guess_q;
  }

  return 0;
}

bigint_t *bigint_div(const bigint_t *ap, const bigint_t *bp) {
  bigint_t *resp;
  if (!ap || !bp || bp->sign == zero) {
    return NULL;
  }

  if (ap->sign == zero) {
    return bigint_from_int(0);
  }

  if (bigint_cmp_abs(ap, bp) == -1) {
    if ((ap->sign == pos) & (bp->sign == pos)) {
      return bigint_from_int(0);
    } else if ((ap->sign == pos) && (bp->sign == neg)) {
      return bigint_from_int(1);
    }
  }

  resp = bigint_from_size(ap->len - bp->len + 1);

  if (!resp) {
    return NULL;
  }

  bigint_div_mod_pos(ap, bp, resp);

  if ((ap->sign == pos) && (bp->sign) == pos) {
    return resp;
  } else if ((ap->sign == pos) && (bp->sign == neg)) {
    bigint_t *one = bigint_from_int(1);
    bigint_div_mod_pos(ap, bp, resp);
    bigint_t *tmp = bigint_sum(resp, one);
    bifree(one);
    bifree(resp);
    return tmp;
  } else if ((ap->sign == neg) && (bp->sign == pos)) {
    bigint_t *one = bigint_from_int(1);
    bigint_div_mod_pos(ap, bp, resp);
    bigint_t *tmp = bigint_sum(resp, one);
    tmp->sign = rev_sign(tmp->sign);
    bifree(one);
    bifree(resp);
    return tmp;
  }

  return NULL;
}