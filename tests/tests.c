#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "../big-int-lib/bigint.h"
#include "minunit.h"

MU_TEST(length_tests) {
  size_t size_0 = 0;
  size_t size_1000 = 1000;
  size_t size_uint_max = UINT_MAX;

  bigint_t *a = bigint_from_size(size_0);
  bigint_t *b = bigint_from_size(size_1000);
  bigint_t *c = bigint_from_size(size_uint_max);

  mu_check(a == NULL);
  mu_check(b->len == 1000);
  mu_check(c->len == size_uint_max);

  bifree(a);
  bifree(b);
  bifree(c);
}

MU_TEST(str_tests) {
  bigint_t *b_empty = bigint_from_str("");
  mu_check(b_empty == NULL);

  bigint_t *b_neg_sgn = bigint_from_str("-");
  mu_check(b_neg_sgn == NULL);

  bigint_t *b_pos_sgn = bigint_from_str("+");
  mu_check(b_pos_sgn == NULL);

  bigint_t *b_alpha = bigint_from_str("12ab4ba25");
  mu_check(b_alpha == NULL);

  char *ptr_empty = bigint_to_str(b_empty);
  mu_check(ptr_empty == NULL);

  char *ptr_neg_sgn = bigint_to_str(b_neg_sgn);
  mu_check(ptr_neg_sgn == NULL);

  char *ptr_pos_sgn = bigint_to_str(b_pos_sgn);
  mu_check(ptr_pos_sgn == NULL);

  char *ptr_alpha = bigint_to_str(b_alpha);
  mu_check(ptr_alpha == NULL);

  bigint_t *b_neg_ld_z = bigint_from_str("-012");
  char *ptr_neg_leading_zeroes = bigint_to_str(b_neg_ld_z);
  mu_check(b_neg_ld_z->len == 2);
  mu_check(b_neg_ld_z->sign == neg);
  mu_assert_string_eq("-12", ptr_neg_leading_zeroes);

  bigint_t *b_pos_ld_z = bigint_from_str("00136");
  char *ptr_pos_leading_zeroes = bigint_to_str(b_pos_ld_z);
  mu_check(b_pos_ld_z->len == 3);
  mu_check(b_pos_ld_z->sign == pos);
  mu_assert_string_eq("136", ptr_pos_leading_zeroes);

  bigint_t *b_zero = bigint_from_str("0");
  char *ptr_zero = bigint_to_str(b_zero);
  mu_check(b_zero->len == 1);
  mu_check(b_zero->sign == pos);
  mu_assert_string_eq("0", ptr_zero);

  bigint_t *b_neg = bigint_from_str("-22558877");
  char *ptr_neg = bigint_to_str(b_neg);
  mu_check(b_neg->len == 8);
  mu_check(b_neg->sign == neg);
  mu_assert_string_eq("-22558877", ptr_neg);

  bigint_t *b_pos = bigint_from_str("44221100");
  char *ptr_pos = bigint_to_str(b_pos);
  mu_check(b_pos->len == 8);
  mu_check(b_pos->sign == pos);
  mu_assert_string_eq("44221100", ptr_pos);

  free(ptr_neg_leading_zeroes);
  bifree(b_pos_ld_z);
  free(ptr_pos_leading_zeroes);
  bifree(b_zero);
  free(ptr_zero);
  bifree(b_neg);
  free(ptr_neg);
  bifree(b_pos);
  free(ptr_pos);
}

MU_TEST(sum_tests) {
  bigint_t *a = bigint_from_str("112");
  bigint_t *b = bigint_from_str("999");
  bigint_t *neg_b = bigint_from_str("-999");
  bigint_t *zero = bigint_from_str("0");
  bigint_t *one = bigint_from_str("1");
  bigint_t *neg_one = bigint_from_str("-1");

  bigint_t *a_plus_b = bigint_sum(a, b);
  char *str_a_plus_b = bigint_to_str(a_plus_b);
  mu_check(a_plus_b->len == 4);
  mu_check(a_plus_b->sign == pos);
  mu_assert_string_eq("1111", str_a_plus_b);

  bigint_t *b_plus_a = bigint_sum(b, a);
  char *str_b_plus_a = bigint_to_str(b_plus_a);
  mu_check(b_plus_a->len == 4);
  mu_check(b_plus_a->sign == pos);
  mu_assert_string_eq("1111", str_b_plus_a);

  bigint_t *a_plus_b_plus_a_1 = bigint_sum(a_plus_b, a);
  char *str_a_plus_b_plus_a_1 = bigint_to_str(a_plus_b_plus_a_1);
  mu_check(a_plus_b_plus_a_1->len == 4);
  mu_check(a_plus_b_plus_a_1->sign == pos);
  mu_assert_string_eq("1223", str_a_plus_b_plus_a_1);

  bigint_t *a_plus_b_plus_a_2 = bigint_sum(a, b_plus_a);
  char *str_a_plus_b_plus_a_2 = bigint_to_str(a_plus_b_plus_a_2);
  mu_check(a_plus_b_plus_a_2->len == 4);
  mu_check(a_plus_b_plus_a_2->sign == pos);
  mu_assert_string_eq("1223", str_a_plus_b_plus_a_2);

  /* Commutativity. */
  mu_check(bigint_cmp(a_plus_b, b_plus_a) == 0);

  /* Associativity. */
  mu_check(bigint_cmp(a_plus_b_plus_a_1, a_plus_b_plus_a_2) == 0);

  /* 0 is neutral. */
  bigint_t *one_plus_zero = bigint_sum(one, zero);
  mu_check(bigint_cmp(one_plus_zero, one) == 0);

  bigint_t *b_neg_b = bigint_sum(b, neg_b);
  mu_check(bigint_cmp(zero, b_neg_b) == 0);

  bigint_t *neg_one_plus_zero = bigint_sum(neg_one, zero);
  mu_check(bigint_cmp(neg_one_plus_zero, neg_one) == 0);

  /* Invariant a + 1 - 1 = a */
  bigint_t *a_plus_one = bigint_sum(a, one);
  bigint_t *inv_a = bigint_sum(a_plus_one, neg_one);
  mu_check(bigint_cmp(a, inv_a) == 0);

  bigint_t *zero_minus_one = bigint_sum(zero, neg_one);
  mu_check(bigint_cmp(zero_minus_one, neg_one) == 0);

  bigint_t *zero_minus_one_plus_one = bigint_sum(zero_minus_one, one);
  mu_check(bigint_cmp(zero_minus_one_plus_one, zero) == 0);

  bigint_t *inv_zero = bigint_sum(zero, zero);
  mu_check(bigint_cmp(inv_zero, zero) == 0);

  bifree(a);
  bifree(b);
  bifree(a_plus_b);
  bifree(b_plus_a);
  bifree(a_plus_b_plus_a_1);
  bifree(a_plus_b_plus_a_2);
  bifree(zero);
  bifree(one);
  bifree(neg_one);
  bifree(a_plus_one);
  bifree(inv_a);
  free(str_a_plus_b);
  free(str_b_plus_a);
  free(str_a_plus_b_plus_a_1);
  free(str_a_plus_b_plus_a_2);
  bifree(neg_b);
  bifree(zero_minus_one);
  bifree(zero_minus_one_plus_one);
  bifree(inv_zero);
}

int main(int argc, char *argv[]) {
  MU_RUN_TEST(length_tests);
  MU_RUN_TEST(str_tests);
  MU_RUN_TEST(sum_tests);
  MU_REPORT();
  return MU_EXIT_CODE;
}