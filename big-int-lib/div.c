#include "bigint.h"
#include "utils.h"

#if 0
static int helper_div_cmp(bigint_t *ap, bigint_t *bp, size_t pos) {
  bigint_t *u, *v;
  size_t i;

  u = bigint_from_size(bp->len);
  v = bigint_from_size(bp->len);
  u->digits = calloc(u->len, sizeof(char));
  v->digits = calloc(v->len, sizeof(char));

  for (i = 0; i < bp->len; ++i) {
    u->digits[bp->len - i - 1] = ap->digits[pos - i];
    v->digits[bp->len - i - 1] = bp->digits[bp->len - i - 1];
  }

  int res = (bigint_max_abs(u, v) == 0);

  bifree(u);
  bifree(v);

  return res;
}
#endif

bigint_t* bigint_div_dec(const bigint_t* ap, unsigned char d) {
  bigint_t* res;
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

#if 0
bigint_t *bigint_div_mod(const bigint_t *ap, const bigint_t *bp) {
  bigint_t *res, *tmp_u, *tmp_v, *tmp;
  size_t n, m;
  int q;

  res = malloc(sizeof(bigint_t));

  if ((res == NULL) || (ap == NULL) || (bp == NULL)) {
    return NULL;
  }

  if (eq_zero(bp)) {
    return NULL;
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

  n = ap->len;
  m = bp->len;

  /* TODO */

  bigint_normalize(res);

  return res;
}
#endif