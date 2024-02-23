#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "../big-int-lib/bigint.h"
#include "minunit.h"

MU_TEST(length_tests) {
  size_t size_0 = 0;
  size_t size_1000 = 1000;
  size_t size_uint_max = UINT_MAX;

  BigInt *a = bigint_from_size(size_0);
  BigInt *b = bigint_from_size(size_1000);
  BigInt *c = bigint_from_size(size_uint_max);

  mu_check(a == NULL);
  mu_check(b->length == 1000);
  mu_check(c->length == size_uint_max);

  bifree(a);
  bifree(b);
  bifree(c);
}

MU_TEST(str_tests) {
  BigInt *b_empty = str_to_bigint("");
  mu_check(b_empty == NULL);

  BigInt *b_neg_sgn = str_to_bigint("-");
  mu_check(b_neg_sgn == NULL);

  BigInt *b_pos_sgn = str_to_bigint("+");
  mu_check(b_pos_sgn == NULL);

  BigInt *b_alpha = str_to_bigint("12ab4ba25");
  mu_check(b_alpha == NULL);

  char *ptr_empty = bigint_to_str(b_empty);
  mu_check(ptr_empty == NULL);

  char *ptr_neg_sgn = bigint_to_str(b_neg_sgn);
  mu_check(ptr_neg_sgn == NULL);

  char *ptr_pos_sgn = bigint_to_str(b_pos_sgn);
  mu_check(ptr_pos_sgn == NULL);

  char *ptr_alpha = bigint_to_str(b_alpha);
  mu_check(ptr_alpha == NULL);

  BigInt *b_neg_ld_z = str_to_bigint("-012");
  char *ptr_neg_leading_zeroes = bigint_to_str(b_neg_ld_z);
  mu_check(b_neg_ld_z->length == 2);
  mu_check(b_neg_ld_z->sign == negative);
  mu_assert_string_eq("-12", ptr_neg_leading_zeroes);

  BigInt *b_pos_ld_z = str_to_bigint("00136");
  char *ptr_pos_leading_zeroes = bigint_to_str(b_pos_ld_z);
  mu_check(b_pos_ld_z->length == 3);
  mu_check(b_pos_ld_z->sign == positive);
  mu_assert_string_eq("136", ptr_pos_leading_zeroes);

  BigInt *b_zero = str_to_bigint("0");
  char *ptr_zero = bigint_to_str(b_zero);
  mu_check(b_zero->length == 1);
  mu_check(b_zero->sign == positive);
  mu_assert_string_eq("0", ptr_zero);

  BigInt *b_neg = str_to_bigint("-22558877");
  char *ptr_neg = bigint_to_str(b_neg);
  mu_check(b_neg->length == 8);
  mu_check(b_neg->sign == negative);
  mu_assert_string_eq("-22558877", ptr_neg);

  BigInt *b_pos = str_to_bigint("44221100");
  char *ptr_pos = bigint_to_str(b_pos);
  mu_check(b_pos->length == 8);
  mu_check(b_pos->sign == positive);
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
  BigInt *a = str_to_bigint("112");
  BigInt *b = str_to_bigint("999");
  BigInt *neg_b = str_to_bigint("-999");
  BigInt *zero = str_to_bigint("0");
  BigInt *one = str_to_bigint("1");
  BigInt *neg_one = str_to_bigint("-1");

  BigInt *a_plus_b = bigint_sum(a, b);
  char *str_a_plus_b = bigint_to_str(a_plus_b);
  mu_check(a_plus_b->length == 4);
  mu_check(a_plus_b->sign == positive);
  mu_assert_string_eq("1111", str_a_plus_b);

  BigInt *b_plus_a = bigint_sum(b, a);
  char *str_b_plus_a = bigint_to_str(b_plus_a);
  mu_check(b_plus_a->length == 4);
  mu_check(b_plus_a->sign == positive);
  mu_assert_string_eq("1111", str_b_plus_a);

  BigInt *a_plus_b_plus_a_1 = bigint_sum(a_plus_b, a);
  char *str_a_plus_b_plus_a_1 = bigint_to_str(a_plus_b_plus_a_1);
  mu_check(a_plus_b_plus_a_1->length == 4);
  mu_check(a_plus_b_plus_a_1->sign == positive);
  mu_assert_string_eq("1223", str_a_plus_b_plus_a_1);

  BigInt *a_plus_b_plus_a_2 = bigint_sum(a, b_plus_a);
  char *str_a_plus_b_plus_a_2 = bigint_to_str(a_plus_b_plus_a_2);
  mu_check(a_plus_b_plus_a_2->length == 4);
  mu_check(a_plus_b_plus_a_2->sign == positive);
  mu_assert_string_eq("1223", str_a_plus_b_plus_a_2);

  /* Commutativity. */
  mu_check(bigint_cmp(a_plus_b, b_plus_a) == 0);

  /* Associativity. */
  mu_check(bigint_cmp(a_plus_b_plus_a_1, a_plus_b_plus_a_2) == 0);

  /* 0 is neutral. */
  BigInt *one_plus_zero = bigint_sum(one, zero);
  mu_check(bigint_cmp(one_plus_zero, one) == 0);

  BigInt *b_neg_b = bigint_sum(b, neg_b);
  mu_check(bigint_cmp(zero, b_neg_b) == 0);

  BigInt *neg_one_plus_zero = bigint_sum(neg_one, zero);
  mu_check(bigint_cmp(neg_one_plus_zero, neg_one) == 0);

  /* Invariant a + 1 - 1 = a */
  BigInt *a_plus_one = bigint_sum(a, one);
  BigInt *inv_a = bigint_sum(a_plus_one, neg_one);
  mu_check(bigint_cmp(a, inv_a) == 0);

  BigInt *zero_minus_one = bigint_sum(zero, neg_one);
  mu_check(bigint_cmp(zero_minus_one, neg_one) == 0);

  BigInt *zero_minus_one_plus_one = bigint_sum(zero_minus_one, one);
  mu_check(bigint_cmp(zero_minus_one_plus_one, zero) == 0);

  BigInt *inv_zero = bigint_sum(zero, zero);
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