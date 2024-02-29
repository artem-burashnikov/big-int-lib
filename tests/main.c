#include <limits.h>
#include <stdlib.h>

#include "../big-int-lib/bigint.h"
#include "../big-int-lib/utils.h"
#include "minunit.h"

MU_TEST(from_size) {
  size_t size_0 = 0;
  size_t size_1000 = 1000;
  size_t size_BIG = 10000000;

  bigint_t *a = bigint_from_size(size_0);
  bigint_t *b = bigint_from_size(size_1000);
  bigint_t *c = bigint_from_size(size_BIG);

  mu_check(a == NULL);

  mu_check(b->len == 1000);
  mu_check(c->len = size_BIG);

  bifree(b);
  bifree(c);
}

MU_TEST(from_str) {
  bigint_t *b_empty = bigint_from_str("");
  mu_check(b_empty == NULL);

  bigint_t *b_alpha = bigint_from_str("12ab4ba25");
  mu_check(b_alpha == NULL);

  bigint_t *b_neg_ld_z = bigint_from_str("-012");
  char *ptr_neg_leading_zeroes = bigint_to_str(b_neg_ld_z);
  mu_check(b_neg_ld_z->len == 2);
  mu_check(b_neg_ld_z->sign == neg);
  mu_assert_string_eq("-12", ptr_neg_leading_zeroes);
  bifree(b_neg_ld_z);
  free(ptr_neg_leading_zeroes);

  bigint_t *b_pos_ld_z = bigint_from_str("00136");
  char *ptr_pos_leading_zeroes = bigint_to_str(b_pos_ld_z);
  mu_check(b_pos_ld_z->len == 3);
  mu_check(b_pos_ld_z->sign == pos);
  mu_assert_string_eq("136", ptr_pos_leading_zeroes);
  bifree(b_pos_ld_z);
  free(ptr_pos_leading_zeroes);

  bigint_t *b_zero = bigint_from_str("0");
  char *ptr_zero = bigint_to_str(b_zero);
  mu_check(b_zero->len == 1);
  mu_check(b_zero->sign == zero);
  mu_assert_string_eq("0", ptr_zero);
  bifree(b_zero);
  free(ptr_zero);

  bigint_t *b_many_zero = bigint_from_str("000000");
  char *ptr_zero_many = bigint_to_str(b_many_zero);
  mu_check(b_many_zero->len == 1);
  mu_check(b_many_zero->sign == zero);
  mu_assert_string_eq("0", ptr_zero_many);
  bifree(b_many_zero);
  free(ptr_zero_many);

  bigint_t *b_neg = bigint_from_str("-22558877");
  char *ptr_neg = bigint_to_str(b_neg);
  mu_check(b_neg->len == 8);
  mu_check(b_neg->sign == neg);
  mu_assert_string_eq("-22558877", ptr_neg);
  bifree(b_neg);
  free(ptr_neg);

  bigint_t *b_pos = bigint_from_str("44221100");
  char *ptr_pos = bigint_to_str(b_pos);
  mu_check(b_pos->len == 8);
  mu_check(b_pos->sign == pos);
  mu_assert_string_eq("44221100", ptr_pos);
  bifree(b_pos);
  free(ptr_pos);
}

MU_TEST(from_int) {
  bigint_t *a = bigint_from_int(0);
  bigint_t *b = bigint_from_int(10);
  bigint_t *c = bigint_from_int(-10);

  mu_assert_int_eq(0, a->digits[0]);
  mu_assert_int_eq(10, (b->digits[1]) * 10);
  mu_assert_int_eq(-10, (c->digits[1]) * (-10));

  bifree(a);
  bifree(b);
  bifree(c);
}

MU_TEST(padding) {
  bigint_t *a = bigint_from_int(1256);
  bigint_t *b = bigint_from_int(1);
  bigint_add_padding(a, 10);
  bigint_add_padding(b, 0);
  char *str_a = bigint_to_str(a);
  char *str_b = bigint_to_str(b);
  mu_assert_string_eq("00000000001256", str_a);
  mu_assert_string_eq("1", str_b);

  bifree(a);
  bifree(b);
  free(str_a);
  free(str_b);
}

MU_TEST(rshift) {
  bigint_t *a = bigint_from_int(1256);
  bigint_t *b = bigint_from_int(0);
  bigint_t *c = bigint_from_int(3);
  bigint_rshift(a, 1);
  bigint_rshift(b, 10);
  bigint_rshift(c, 0);
  char *str_a = bigint_to_str(a);
  char *str_b = bigint_to_str(b);
  char *str_c = bigint_to_str(c);
  mu_assert_string_eq("12560", str_a);
  mu_assert_string_eq("00000000000", str_b);
  mu_assert_string_eq("3", str_c);

  bifree(a);
  bifree(b);
  bifree(c);
  free(str_a);
  free(str_b);
  free(str_c);
}

MU_TEST(pos_sum) {
  bigint_t *a = bigint_from_int(112);
  bigint_t *b = bigint_from_int(999);
  bigint_t *zero = bigint_from_int(0);
  bigint_t *one = bigint_from_int(1);

  bigint_t *apb = bigint_sum_pos(a, b);
  mu_check(apb->len == 4);
  mu_check(apb->sign == pos);
  mu_assert_int_eq(1111, bigint_to_int(apb));

  bigint_t *bpa = bigint_sum_pos(b, a);
  mu_check(bpa->len == 4);
  mu_check(bpa->sign == pos);
  mu_assert_int_eq(1111, bigint_to_int(bpa));

  bigint_t *apbpa1 = bigint_sum_pos(apb, a);
  mu_check(apbpa1->len == 4);
  mu_check(apbpa1->sign == pos);
  mu_assert_int_eq(1223, bigint_to_int(apbpa1));

  bigint_t *apbpa2 = bigint_sum_pos(a, bpa);
  mu_check(apbpa2->len == 4);
  mu_check(apbpa2->sign == pos);
  mu_assert_int_eq(1223, bigint_to_int(apbpa2));

  /* Commutativity. */
  mu_check(!bigint_cmp(apb, bpa));

  /* Associativity. */
  mu_check(!bigint_cmp(apbpa1, apbpa2));

  /* 0 is neutral. */
  bigint_t *opz = bigint_sum_pos(one, zero);
  mu_check(!bigint_cmp(opz, one));

  bifree(a);
  bifree(b);
  bifree(zero);
  bifree(one);
  bifree(apb);
  bifree(bpa);
  bifree(apbpa1);
  bifree(apbpa2);
  bifree(opz);
}

MU_TEST(sub_pos) {
  bigint_t *a = bigint_from_int(112);
  bigint_t *b = bigint_from_int(999);
  bigint_t *c = bigint_from_int(123);
  bigint_t *d = bigint_from_int(95);
  bigint_t *e = bigint_from_int(234);
  bigint_t *zero = bigint_from_int(0);
  bigint_t *one = bigint_from_int(1);
  bigint_t *neg_one = bigint_from_int(-1);

  bigint_t *bsb = bigint_sub_pos(b, b);
  mu_check(!bigint_cmp(zero, bsb));

  /* Invariant a + 1 - 1 = a */
  bigint_t *apa = bigint_sum_pos(a, one);
  bigint_t *inv_a = bigint_sub_pos(apa, one);
  mu_check(!bigint_cmp(a, inv_a));

  bigint_t *zso = bigint_sub_pos(zero, one);
  mu_check(!bigint_cmp(zso, neg_one));

  bigint_t *zsopo = bigint_sum_pos(zso, one);
  mu_check(!bigint_cmp(zsopo, zero));

  /* 0 is neutral. */
  bigint_t *zsz = bigint_sub_pos(zero, zero);
  mu_check(bigint_cmp(zsz, zero) == 0);

  bigint_t *csd = bigint_sub(c, d);
  mu_check(csd->len == 2);
  mu_check(csd->sign == pos);
  mu_assert_int_eq(28, bigint_to_int(csd));

  bifree(a);
  bifree(b);
  bifree(c);
  bifree(d);
  bifree(e);
  bifree(zero);
  bifree(one);
  bifree(neg_one);
  bifree(bsb);
  bifree(apa);
  bifree(inv_a);
  bifree(zso);
  bifree(zsopo);
  bifree(zsz);
  bifree(csd);
}

int main() {
  MU_RUN_TEST(from_size);
  MU_RUN_TEST(from_str);
  MU_RUN_TEST(from_int);
  MU_RUN_TEST(padding);
  MU_RUN_TEST(rshift);
  MU_RUN_TEST(pos_sum);
  MU_REPORT();
  return MU_EXIT_CODE;
}