#include "bigint.h"

bigint_t *bigint_div_dec(const bigint_t *ap, unsigned char d) {
  bigint_t *res;
  size_t i;
  int r, q, w;

  if ((d >= 10) || (d == 0)) {
    return NULL;
  }

  if ((ap == NULL) || (ap->len == 0)) {
    return NULL;
  }

  if (eq_zero(ap)) {
    return bigint_from_str("0");
  }

  res = bigint_init();

  if (res == NULL) {
    return NULL;
  }

  res->len = ap->len;

  res->digits = calloc(res->len, sizeof(char));

  if (res->digits == NULL) {
    bifree(res);
    return NULL;
  }

  res->sign = ap->sign;

  r = q = 0;

  for (i = 0; i < ap->len; ++i) {
    w = BASE * r + ap->digits[ap->len - 1 - i];
    q = w / d;
    r = w % d;
    res->digits[ap->len - 1 - i] = q;
  }

  bigint_normalize(res);

  return res;
}

bigint_t *bigint_div_mod(const bigint_t *ap, const bigint_t *bp) {
  bigint_t *tmp, *u, *v, *res, *qv, *factor;
  long int i, j, k;
  size_t n, m;
  char q, w, r, d;
  char *factor_str;

  if ((ap == NULL) || (bp == NULL)) {
    return NULL;
  }

  if ((ap->len == 0) || (bp->len == 0)) {
    return NULL;
  }

  if (eq_zero(bp)) {
    return NULL;
  }

  if (eq_zero(ap)) {
    return bigint_from_str("0");
  }

  if (bigint_max_abs(ap, bp)) {
    return bigint_from_str("0");
  }

  assert(ap->len >= bp->len);

  if (bp->len == 1) {
    return (bigint_div_dec(ap, bp->digits[0]));
  }

  res = bigint_init();

  if (res == NULL) {
    return NULL;
  }

  res->len = ap->len;

  res->digits = calloc(res->len, sizeof(char));

  if (res->digits == NULL) {
    bifree(res);
    return NULL;
  }

  res->sign = ap->sign ^ bp->sign;

  d = BASE / (bp->digits[bp->len - 1] + 1);

  m = ap->len - bp->len;

  u = bigint_mul_dec(ap, d);
  v = bigint_mul_dec(bp, d);

  if((u == NULL) && (v == NULL)) {
    bifree(res);
    return NULL;
  } else if ((u == NULL) && (v != NULL)) {
    bifree(res);
    bifree(v);
    return NULL;
  } else if ((u != NULL) && (v == NULL)) {
    bifree(u);
    bifree(res);
    return NULL;
  }

  assert(u->len > v->len);

  u->sign = v->sign = pos;

  n = v->len;

  if (u->len == ap->len) {
    bigint_add_padding(u, u->len + 1);
  }
  
  assert(u->len == n + m + 1);

  for (j = m; j >= 0; --j) {
    w = (BASE * u->digits[j + n] + u->digits[j + n - 1]);
    q = w / v->digits[n - 1];
    r = w % v->digits[n - 1];

    if ((q >= BASE) || (q * v->digits[n - 2] > BASE * r + u->digits[j + n - 2])) {
      q -= 1;
      r += v->digits[n - 1];
    }

    while (r < BASE) {
      if ((q >= BASE) || (q * v->digits[n - 2] > BASE * r + u->digits[j + n - 2])) {
        q -= 1;
        r += v->digits[n - 1];
      } else {
        break;
      }
    }

    assert(q < BASE);

    tmp = bigint_from_size(n + m + 1);

    if (tmp == NULL) {
      return NULL;
    }

    for (i = n + m; i >= 0; --i) {
      if (i > j + n ) {
        tmp->digits[i] = 0;
      } else {
        tmp->digits[i] = u->digits[i]; 
      }
    }
#if 0
    /* WIP */
    size_t p = 1;
    for (k = 0; k < m-1; ++k) {
      p *= 10;
    }

    p = q * p;
    factor_str = str_from_int(p);
    factor = bigint_from_str(factor_str);
    qv = bigint_mul(v, factor);
    bifree(factor);
    free(factor_str);

    if (qv == NULL) {
      bifree(u);
      bifree(tmp);
      bifree(v);
      bifree(res);
      return NULL;
    }

    if (bigint_add_padding(qv, qv->len + 1)) {
      bifree(u);
      bifree(tmp);
      bifree(v);
      bifree(res);
      bifree(qv);
      return NULL;
    }

    bifree(u);

    u = bigint_sub(tmp, qv);

    bifree(tmp);

    bifree(qv);

    assert(u->sign != neg);

    res->digits[j] = q;
    /* WIP */
#endif
  }

  bifree(u);
  bifree(v);

  bigint_normalize(res);
  
  return res;

}