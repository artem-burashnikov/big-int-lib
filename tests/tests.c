#include <limits.h>
#include <stdlib.h>

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
  BigInt *b_neg_sgn = str_to_bigint("-");
  BigInt *b_pos_sgn = str_to_bigint("+");
  BigInt *b_alpha = str_to_bigint("12ab4ba25");
  BigInt *b_zero = str_to_bigint("0");
  BigInt *b_neg_ld_z = str_to_bigint("-012");
  BigInt *b_pos_ld_z = str_to_bigint("00136");
  BigInt *b_neg = str_to_bigint("-22558877");
  BigInt *b_pos = str_to_bigint("44221100");

  char *ptr_empty = bigint_to_str(b_empty);
  char *ptr_neg_sgn = bigint_to_str(b_neg_sgn);
  char *ptr_pos_sgn = bigint_to_str(b_pos_sgn);
  char *ptr_alpha = bigint_to_str(b_alpha);
  char *ptr_zero = bigint_to_str(b_zero);
  char *ptr_neg_leading_zeroes = bigint_to_str(b_neg_ld_z);
  char *ptr_pos_leading_zeroes = bigint_to_str(b_pos_ld_z);
  char *ptr_neg = bigint_to_str(b_neg);
  char *ptr_pos = bigint_to_str(b_pos);

  mu_check(b_empty == NULL);
  mu_check(ptr_empty == NULL);
  mu_check(b_neg_sgn == NULL);
  mu_check(ptr_neg_sgn == NULL);
  mu_check(b_pos_sgn == NULL);
  mu_check(ptr_pos_sgn == NULL);
  mu_check(b_alpha == NULL);
  mu_check(ptr_alpha == NULL);

  mu_check(b_neg_ld_z->length == 2);
  mu_check(b_neg_ld_z->sign == negative);
  mu_assert_string_eq("-12", ptr_neg_leading_zeroes);

  mu_check(b_pos_ld_z->length == 3);
  mu_check(b_pos_ld_z->sign == positive);
  mu_assert_string_eq("136", ptr_pos_leading_zeroes);

  mu_check(b_zero->length == 1);
  mu_check(b_zero->sign == positive);
  mu_assert_string_eq("0", ptr_zero);

  mu_check(b_neg->length == 8);
  mu_check(b_neg->sign == negative);
  mu_assert_string_eq("-22558877", ptr_neg);

  mu_check(b_pos->length == 8);
  mu_check(b_pos->sign == positive);
  mu_assert_string_eq("44221100", ptr_pos);

  bifree(b_neg_ld_z);
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

int main(int argc, char *argv[]) {
  MU_RUN_TEST(length_tests);
  MU_RUN_TEST(str_tests);
  MU_REPORT();
  return MU_EXIT_CODE;
}