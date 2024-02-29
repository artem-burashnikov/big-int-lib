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

MU_TEST(sum) {
  bigint_t *a = bigint_from_int(112);
  bigint_t *neg_a = bigint_from_int(-112);
  bigint_t *b = bigint_from_int(999);
  bigint_t *neg_b = bigint_from_int(-999);
  bigint_t *zero = bigint_from_int(0);
  bigint_t *one = bigint_from_int(1);

  bigint_t *apb = bigint_sum(a, b);
  mu_check(apb->len == 4);
  mu_check(apb->sign == pos);
  mu_assert_int_eq(1111, bigint_to_int(apb));

  bigint_t *bpa = bigint_sum(b, a);
  mu_check(bpa->len == 4);
  mu_check(bpa->sign == pos);
  mu_assert_int_eq(1111, bigint_to_int(bpa));

  bigint_t *apbpa1 = bigint_sum(apb, a);
  mu_check(apbpa1->len == 4);
  mu_check(apbpa1->sign == pos);
  mu_assert_int_eq(1223, bigint_to_int(apbpa1));

  bigint_t *apbpa2 = bigint_sum(a, bpa);
  mu_check(apbpa2->len == 4);
  mu_check(apbpa2->sign == pos);
  mu_assert_int_eq(1223, bigint_to_int(apbpa2));

  bigint_t *nbpa = bigint_sum(neg_b, a);
  mu_check(nbpa->len == 3);
  mu_check(nbpa->sign == neg);
  mu_assert_int_eq(-887, bigint_to_int(nbpa));

  bigint_t *napb = bigint_sum(neg_a, b);
  mu_check(napb->len == 3);
  mu_check(napb->sign == pos);
  mu_assert_int_eq(887, bigint_to_int(napb));

  bigint_t *apnb = bigint_sum(a, neg_b);
  mu_check(apnb->len == 3);
  mu_check(apnb->sign == neg);
  mu_assert_int_eq(-887, bigint_to_int(apnb));

  bigint_t *bpna = bigint_sum(b, neg_a);
  mu_check(bpna->len == 3);
  mu_check(bpna->sign == pos);
  mu_assert_int_eq(887, bigint_to_int(bpna));

  bigint_t *nbpna = bigint_sum(neg_b, neg_a);
  mu_check(nbpna->len == 4);
  mu_check(nbpna->sign == neg);
  mu_assert_int_eq(-1111, bigint_to_int(nbpna));

  /* Commutativity. */
  mu_check(!bigint_cmp(apb, bpa));

  /* Associativity. */
  mu_check(!bigint_cmp(apbpa1, apbpa2));

  /* 0 is neutral. */
  bigint_t *opz = bigint_sum(one, zero);
  mu_check(!bigint_cmp(opz, one));

  bifree(a);
  bifree(neg_a);
  bifree(b);
  bifree(neg_b);
  bifree(zero);
  bifree(one);
  bifree(apb);
  bifree(bpa);
  bifree(apbpa1);
  bifree(apbpa2);
  bifree(opz);
  bifree(nbpa);
  bifree(napb);
  bifree(apnb);
  bifree(bpna);
  bifree(nbpna);
}

MU_TEST(sub) {
  bigint_t *a = bigint_from_int(112);
  bigint_t *b = bigint_from_int(999);
  bigint_t *neg_b = bigint_from_int(-999);
  bigint_t *c = bigint_from_int(123);
  bigint_t *neg_c = bigint_from_int(-123);
  bigint_t *d = bigint_from_int(95);
  bigint_t *e = bigint_from_int(234);
  bigint_t *zero = bigint_from_int(0);
  bigint_t *one = bigint_from_int(1);
  bigint_t *neg_one = bigint_from_int(-1);

  bigint_t *asb = bigint_sub(a, b);
  mu_check(asb->len == 3);
  mu_check(asb->sign == neg);
  mu_assert_int_eq(-887, bigint_to_int(asb));

  bigint_t *bsa = bigint_sub(b, a);
  mu_check(bsa->len == 3);
  mu_check(bsa->sign == pos);
  mu_assert_int_eq(887, bigint_to_int(bsa));

  bigint_t *bsnegc = bigint_sub(b, neg_c);
  mu_check(bsnegc->len == 4);
  mu_check(bsnegc->sign == pos);
  mu_assert_int_eq(1122, bigint_to_int(bsnegc));

  bigint_t *csnegb = bigint_sub(c, neg_b);
  mu_check(csnegb->len == 4);
  mu_check(csnegb->sign == pos);
  mu_assert_int_eq(1122, bigint_to_int(csnegb));

  bigint_t *negbsc = bigint_sub(neg_b, c);
  mu_check(negbsc->len == 4);
  mu_check(negbsc->sign == neg);
  mu_assert_int_eq(-1122, bigint_to_int(negbsc));

  bigint_t *negcsb = bigint_sub(neg_c, b);
  mu_check(negcsb->len == 4);
  mu_check(negcsb->sign == neg);
  mu_assert_int_eq(-1122, bigint_to_int(negcsb));

  bigint_t *negbsnegc = bigint_sub(neg_b, neg_c);
  mu_check(negbsnegc->len == 3);
  mu_check(negbsnegc->sign == neg);
  mu_assert_int_eq(-876, bigint_to_int(negbsnegc));

  bigint_t *negcsnegb = bigint_sub(neg_c, neg_b);
  mu_check(negcsnegb->len == 3);
  mu_check(negcsnegb->sign == pos);
  mu_assert_int_eq(876, bigint_to_int(negcsnegb));

  bigint_t *bsb = bigint_sub(b, b);
  mu_check(bsb->len == 1);
  mu_check(bsb->sign == 0);
  mu_check(!bigint_cmp(zero, bsb));

  bigint_t *negbsnegb = bigint_sub(neg_b, neg_b);
  mu_check(!bigint_cmp(zero, negbsnegb));

  /* Invariant a + 1 - 1 = a */
  bigint_t *apa = bigint_sum(a, one);
  bigint_t *inv_a = bigint_sub(apa, one);
  mu_check(!bigint_cmp(a, inv_a));

  bigint_t *zso = bigint_sub(zero, one);
  mu_check(zso->len == 1);
  mu_check(zso->sign == neg);
  mu_check(!bigint_cmp(zso, neg_one));

  bigint_t *zsopo = bigint_sum(zso, one);
  mu_check(zsopo->len == 1);
  mu_check(zsopo->sign == 0);
  mu_check(!bigint_cmp(zsopo, zero));

  /* 0 is neutral. */
  bigint_t *zsz = bigint_sub(zero, zero);
  mu_check(zsz->len == 1);
  mu_check(zsz->sign == 0);
  mu_check(bigint_cmp(zsz, zero) == 0);

  bigint_t *csd = bigint_sub(c, d);
  mu_check(csd->len == 2);
  mu_check(csd->sign == pos);
  mu_assert_int_eq(28, bigint_to_int(csd));

  bifree(a);
  bifree(b);
  bifree(neg_b);
  bifree(c);
  bifree(neg_c);
  bifree(d);
  bifree(e);
  bifree(zero);
  bifree(one);
  bifree(neg_one);

  bifree(asb);
  bifree(bsa);
  bifree(bsnegc);
  bifree(csnegb);
  bifree(negbsc);
  bifree(negcsb);
  bifree(negbsnegc);
  bifree(negcsnegb);
  bifree(bsb);
  bifree(negbsnegb);

  bifree(apa);
  bifree(inv_a);
  bifree(zso);
  bifree(zsopo);
  bifree(zsz);
  bifree(csd);
}

MU_TEST(mul) {
  bigint_t *a = bigint_from_int(245);
  bigint_t *b = bigint_from_int(103);
  bigint_t *c = bigint_from_int(32);
  bigint_t *neg_b = bigint_from_int(-103);
  bigint_t *zero = bigint_from_int(0);
  bigint_t *one = bigint_from_int(1);
  bigint_t *neg_one = bigint_from_int(-1);

  bigint_t *amb = bigint_mul(a, b);
  mu_check(amb->len == 5);
  mu_check(amb->sign == pos);
  mu_assert_int_eq(25235, bigint_to_int(amb));

  bigint_t *bmc = bigint_mul(b, c);
  mu_check(bmc->len == 4);
  mu_check(bmc->sign == pos);
  mu_assert_int_eq(3296, bigint_to_int(bmc));

  bigint_t *bma = bigint_mul(b, a);
  mu_check(bma->len == 5);
  mu_check(bma->sign == pos);
  mu_assert_int_eq(25235, bigint_to_int(bma));

  bigint_t *ambmc1 = bigint_mul(amb, c);
  mu_check(ambmc1->len == 6);
  mu_check(ambmc1->sign == pos);
  mu_assert_int_eq(807520, bigint_to_int(ambmc1));

  bigint_t *ambmc2 = bigint_mul(a, bmc);
  mu_check(ambmc2->len == 6);
  mu_check(ambmc2->sign == pos);
  mu_assert_int_eq(807520, bigint_to_int(ambmc2));

  bigint_t *omno = bigint_mul(one, neg_one);
  mu_check(omno->len = 1);
  mu_check(*omno->digits == 1);
  mu_check(omno->sign == neg);

  bigint_t *nomno = bigint_mul(neg_one, neg_one);
  mu_check(nomno->len = 1);
  mu_check(*nomno->digits == 1);
  mu_check(nomno->sign == pos);

  /* Commutativity. */
  mu_check(!bigint_cmp(amb, bma));

  /* Associativity. */
  mu_check(!bigint_cmp(ambmc1, ambmc2));

  /* 1 is neutral. */
  bigint_t *amo = bigint_mul(a, one);
  mu_check(!bigint_cmp(a, amo));

  bigint_t *oma = bigint_mul(one, a);
  mu_check(!bigint_cmp(oma, a));

  bigint_t *zma = bigint_mul(zero, a);
  mu_check(!bigint_cmp(zero, zma));

  bifree(a);
  bifree(b);
  bifree(neg_b);
  bifree(c);
  bifree(one);
  bifree(neg_one);
  bifree(zero);

  bifree(amb);
  bifree(bmc);
  bifree(bma);
  bifree(ambmc1);
  bifree(ambmc2);
  bifree(omno);
  bifree(nomno);
  bifree(amo);
  bifree(oma);
  bifree(zma);
}

int main() {
  MU_RUN_TEST(from_size);
  MU_RUN_TEST(from_str);
  MU_RUN_TEST(from_int);
  MU_RUN_TEST(padding);
  MU_RUN_TEST(rshift);
  MU_RUN_TEST(sum);
  MU_RUN_TEST(sub);
  MU_RUN_TEST(mul);
  MU_REPORT();
  return MU_EXIT_CODE;
}