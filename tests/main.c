#include <limits.h>

#include "../big-int-lib/utils.h"
#include "minunit.h"

bigint_t *factorial(unsigned int n) {
  bigint_t *prod, *tmp, *cnt;
  unsigned int length;
  char *str;

  prod = bigint_from_str("1");
  for (unsigned int i = 1; i <= n; ++i) {
    length = snprintf(NULL, 0, "%d", i);
    str = malloc(length + 1);
    snprintf(str, length + 1, "%d", i);

    cnt = bigint_from_str(str);

    free(str);

    tmp = bigint_mul(prod, cnt);
    bifree(prod);
    bifree(cnt);

    prod = bigint_init();
    prod->len = tmp->len;
    prod->sign = pos;
    prod->digits = tmp->digits;
    free(tmp);
  }

  return prod;
}

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

MU_TEST(sub_tests) {
  bigint_t *a = bigint_from_str("123");
  bigint_t *b = bigint_from_str("95");
  bigint_t *c = bigint_from_str("234");
  bigint_t *neg_b = bigint_from_str("-95");
  bigint_t *zero = bigint_from_str("0");
  bigint_t *one = bigint_from_str("1");
  bigint_t *neg_one = bigint_from_str("-1");

  bigint_t *a_sub_b = bigint_sub(a, b);
  char *str_a_sub_b = bigint_to_str(a_sub_b);
  mu_check(a_sub_b->len == 2);
  mu_check(a_sub_b->sign == pos);
  mu_assert_string_eq("28", str_a_sub_b);

  bigint_t *b_sub_a = bigint_sub(b, a);
  char *str_b_sub_a = bigint_to_str(b_sub_a);
  mu_check(b_sub_a->len == 2);
  mu_check(b_sub_a->sign == neg);
  mu_assert_string_eq("-28", str_b_sub_a);

  bigint_t *a_sub_c = bigint_sub(a, c);
  char *str_a_sub_c = bigint_to_str(a_sub_c);
  mu_assert_string_eq("-111", str_a_sub_c);

  /* 0 is neutral. */
  bigint_t *zero_sub_zero = bigint_sum(zero, zero);
  mu_check(bigint_cmp(zero_sub_zero, zero) == 0);

  /* x - (-x) = x + x <> 0 */
  bigint_t *b_sub_neg_b = bigint_sub(b, neg_b);
  mu_check(bigint_cmp(zero, b_sub_neg_b) == 1);

  bifree(zero);
  bifree(one);
  bifree(neg_one);

  bifree(a);
  bifree(b);
  bifree(neg_b);

  bifree(a_sub_b);
  free(str_a_sub_b);

  bifree(b_sub_a);
  free(str_b_sub_a);

  bifree(zero_sub_zero);

  bifree(b_sub_neg_b);

  bifree(c);
  bifree(a_sub_c);
  free(str_a_sub_c);
}

MU_TEST(mul_tests) {
  bigint_t *a = bigint_from_str("245");
  bigint_t *b = bigint_from_str("103");
  bigint_t *c = bigint_from_str("32");
  bigint_t *neg_b = bigint_from_str("-103");
  bigint_t *zero = bigint_from_str("0");
  bigint_t *one = bigint_from_str("1");
  bigint_t *neg_one = bigint_from_str("-1");

  bigint_t *a_mul_b = bigint_mul(a, b);
  char *str_a_mul_b = bigint_to_str(a_mul_b);
  mu_check(a_mul_b->len == 5);
  mu_check(a_mul_b->sign == pos);
  mu_assert_string_eq("25235", str_a_mul_b);

  bigint_t *b_mul_c = bigint_mul(b, c);
  char *str_b_mul_c = bigint_to_str(b_mul_c);
  mu_check(b_mul_c->len == 4);
  mu_check(b_mul_c->sign == pos);
  mu_assert_string_eq("3296", str_b_mul_c);

  bigint_t *b_mul_a = bigint_mul(b, a);
  char *str_b_mul_a = bigint_to_str(b_mul_a);
  mu_check(b_mul_a->len == 5);
  mu_check(b_mul_a->sign == pos);
  mu_assert_string_eq("25235", str_b_mul_a);

  bigint_t *a_mul_b_mul_c1 = bigint_mul(a_mul_b, c);
  char *str_a_mul_b_mul_c1 = bigint_to_str(a_mul_b_mul_c1);
  mu_check(a_mul_b_mul_c1->len == 6);
  mu_check(a_mul_b_mul_c1->sign == pos);
  mu_assert_string_eq("807520", str_a_mul_b_mul_c1);

  bigint_t *a_mul_b_mul_c2 = bigint_mul(a, b_mul_c);
  char *str_a_mul_b_mul_c2 = bigint_to_str(a_mul_b_mul_c2);
  mu_check(a_mul_b_mul_c2->len == 6);
  mu_check(a_mul_b_mul_c2->sign == pos);
  mu_assert_string_eq("807520", str_a_mul_b_mul_c2);

  bigint_t *one_mul_neg_one = bigint_mul(one, neg_one);
  mu_check(one_mul_neg_one->len = 1);
  mu_check(one_mul_neg_one->digits[0] == 1);
  mu_check(one_mul_neg_one->sign == neg);

  bigint_t *neg_one_mul_neg_one = bigint_mul(neg_one, neg_one);
  mu_check(neg_one_mul_neg_one->len = 1);
  mu_check(neg_one_mul_neg_one->digits[0] == 1);
  mu_check(neg_one_mul_neg_one->sign == pos);

  /* Commutativity. */
  mu_check(bigint_cmp(a_mul_b, b_mul_a) == 0);

  /* Associativity. */
  mu_check(bigint_cmp(a_mul_b_mul_c1, a_mul_b_mul_c2) == 0);

  /* 1 is neutral. */
  bigint_t *a_mul_one = bigint_mul(a, one);
  mu_check(bigint_cmp(a, a_mul_one) == 0);

  bigint_t *one_mul_a = bigint_mul(one, a);
  mu_check(bigint_cmp(one_mul_a, a) == 0);

  bifree(a);
  bifree(b);
  bifree(neg_b);
  bifree(c);
  bifree(one);
  bifree(neg_one);
  bifree(zero);

  bifree(a_mul_b);
  free(str_a_mul_b);

  bifree(b_mul_c);
  free(str_b_mul_c);

  bifree(b_mul_a);
  free(str_b_mul_a);

  bifree(a_mul_b_mul_c1);
  free(str_a_mul_b_mul_c1);

  bifree(a_mul_b_mul_c2);
  free(str_a_mul_b_mul_c2);

  bifree(one_mul_neg_one);
  bifree(neg_one_mul_neg_one);

  bifree(a_mul_one);
  bifree(one_mul_a);
}

MU_TEST(mul_dec_tests) {
  bigint_t *a = bigint_from_str("245");
  unsigned char d = 8;
  bigint_t *res = bigint_mul_dec(a, d);

  mu_check(bigint_cmp(res, bigint_from_str("1960")) == 0);

  bifree(a);
  bifree(res);
}

MU_TEST(factorial_test) {
  bigint_t *a;
  a = bigint_from_str(
      "402387260077093773543702433923003985719374864210714632"
      "543799910429938512"
      "398629020592044208486969404800479988610197196058631666"
      "872994808558901323"
      "829669944590997424504087073759918823627727188732519779"
      "505950995276120874"
      "975462497043601418278094646496291056393887437886487337"
      "119181045825783647"
      "849977012476632889835955735432513185323958463075557409"
      "114262417474349347"
      "553428646576611667797396668820291207379143853719588249"
      "808126867838374559"
      "731746136085379534524221586593201928090878297308431392"
      "844403281231558611"
      "036976801357304216168747609675871348312025478589320767"
      "169132448426236131"
      "412508780208000261683151027341827977704784635868170164"
      "365024153691398281"
      "264810213092761244896359928705114964975419909342221566"
      "832572080821333186"
      "116811553615836546984046708975602900950537616475847728"
      "421889679646244945"
      "160765353408198901385442487984959953319101723355556602"
      "139450399736280750"
      "137837615307127761926849034352625200015888535147331611"
      "702103968175921510"
      "907788019393178114194545257223865541461062892187960223"
      "838971476088506276"
      "862967146674697562911234082439208160153780889893964518"
      "263243671616762179"
      "168909779911903754031274622289988005195444414282012187"
      "361745992642956581"
      "746628302955570299024324153181617210465832036786906117"
      "260158783520751516"
      "284225540265170483304226143974286933061690897968482590"
      "125458327168226458"
      "066526769958652682272807075781391858178889652208164348"
      "344825993266043367"
      "660176999612831860788386150279465955131156552036093988"
      "180612138558600301"
      "435694527224206344631797460594682573103790084024432438"
      "465657245014402821"
      "885252470935190620929023136493273497565513958720559654"
      "228749774011413346"
      "962715422845862377387538230483865688976461927383814900"
      "140767310446640259"
      "899490222221765904339901886018566526485061799702356193"
      "897017860040811889"
      "729918311021171229845901641921068884387121855646124960"
      "798722908519296819"
      "372388642614839657382291123125024186649353143970137428"
      "531926649875337218"
      "940694281434118520158014123344828015051399694290153483"
      "077644569099073152"
      "433278288269864602789864321139083506217095002597389863"
      "554277196742822248"
      "757586765752344220207573630569498825087968928162753848"
      "863396909959826280"
      "956121450994871701244516461260379029309120889086942028"
      "510640182154399457"
      "156805941872748998094254742173582401063677404595741785"
      "160829230135358081"
      "840096996372524230560855903700624271243416909004153690"
      "105933983835777939"
      "410970027753472000000000000000000000000000000000000000"
      "000000000000000000"
      "000000000000000000000000000000000000000000000000000000"
      "000000000000000000"
      "000000000000000000000000000000000000000000000000000000"
      "000000000000000000"
      "000000000000000000000000000000000000000000000000");

  bigint_t *b = factorial(1000);

  mu_check(bigint_cmp(a, b) == 0);

  bifree(a);
  bifree(b);
}

int main(int argc, char *argv[]) {
  MU_RUN_TEST(length_tests);
  MU_RUN_TEST(str_tests);
  MU_RUN_TEST(sum_tests);
  MU_RUN_TEST(sub_tests);
  MU_RUN_TEST(mul_tests);
  MU_RUN_TEST(mul_dec_tests);
  MU_RUN_TEST(factorial_test);
  MU_REPORT();
  return MU_EXIT_CODE;
}