#include <assert.h>

#include "bigint.h"
#include "utils.h"

static int iabs(int x) { return (x < 0) ? -x : x; }

static int eu_mod(const int x, const int y) {
  int r;
  assert(y != 0);
  r = x % y;
  if (r < 0) {
    r += iabs(y);
  }
  return r;
}

static int eu_div(const int x, const int y) {
  int q, r;
  assert(y != 0);
  r = eu_mod(x, y);
  q = (x - r) / y;
  return q;
}

bigint_t* bigint_sub_pos(const bigint_t* ap, const bigint_t* bp) {
  bigint_t* res;
  size_t i, mlen;
  int diff, carry;

  if (!ap || !bp) {
    return NULL;
  }

  assert((ap->sign == pos) && (bp->sign == pos) || (ap->sign == zero) ||
         (bp->sign == zero));

  assert(ap->len >= bp->len);
  mlen = ap->len;

  /* +1 for possible carry. */
  res = bigint_from_size(mlen);

  if (!res) {
    return NULL;
  }

  carry = 0;
  for (i = 0; i < mlen; ++i) {
    diff = ap->digits[i] + carry;
    if (i < bp->len) {
      diff -= bp->digits[i];
    }
    diff = eu_mod(diff, BASE);
    carry = eu_div(diff, BASE);
  }

  assert((carry == 0));

  if (bigint_normalize(res)) {
    bifree(res);
    return NULL;
  }

  return res;
}

bigint_t* bigint_sub(const bigint_t* ptr_x, const bigint_t* ptr_y) {
  return NULL;
}