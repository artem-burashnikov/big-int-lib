#include <limits.h>
#include <stdlib.h>

#include "../big-int-lib/bigint.h"
#include "../big-int-lib/utils.h"
#include "minunit.h"

static bigint_t *factorial(const unsigned int n) {
  bigint_t *resp, *tmp, *factor;
  unsigned int p;

  resp = bigint_from_int(1);

  if ((n == 0) || (n == 1)) {
    return resp;
  }

  for (p = 2; p <= n; ++p) {
    factor = bigint_from_int(p);
    tmp = bigint_mul(resp, factor);
    bifree(factor);
    bifree(resp);
    resp = tmp;
  }

  return resp;
}

static bigint_t *gcd(const bigint_t *ap, const bigint_t *bp) {
  bigint_t *u, *v, *r;
  int8_t cmp;

  u = v = r = NULL;

  if (!ap || !bp) {
    return r;
  }

  u = bigint_cpy(ap);
  v = bigint_cpy(bp);

  if ((ap->sign == zero) || (bp->sign == zero)) {
    return bigint_from_int(0);
  }

  if ((ap->sign != pos) && (bp->sign != pos)) {
    u->sign = pos;
    v->sign = pos;
    r = gcd(u, v);
    bifree(u);
    bifree(v);
    return r;
  }

  cmp = bigint_cmp_abs(ap, bp);

  if (cmp == -1) {
    return gcd(bp, ap);
  }

  r = bigint_mod(u, v);
  while (r->sign != 0) {
    bifree(u);
    u = bigint_mirror(v);
    free(v);
    v = bigint_mirror(r);
    free(r);
    r = bigint_mod(u, v);
  }

  bifree(r);
  bifree(u);
  return v;
}

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

MU_TEST(nfactorial) {
  bigint_t *a = factorial(1000);
  bigint_t *b = bigint_from_str(
      "402387260077093773543702433923003985719374864210714632543799910429938512"
      "398629020592044208486969404800479988610197196058631666872994808558901323"
      "829669944590997424504087073759918823627727188732519779505950995276120874"
      "975462497043601418278094646496291056393887437886487337119181045825783647"
      "849977012476632889835955735432513185323958463075557409114262417474349347"
      "553428646576611667797396668820291207379143853719588249808126867838374559"
      "731746136085379534524221586593201928090878297308431392844403281231558611"
      "036976801357304216168747609675871348312025478589320767169132448426236131"
      "412508780208000261683151027341827977704784635868170164365024153691398281"
      "264810213092761244896359928705114964975419909342221566832572080821333186"
      "116811553615836546984046708975602900950537616475847728421889679646244945"
      "160765353408198901385442487984959953319101723355556602139450399736280750"
      "137837615307127761926849034352625200015888535147331611702103968175921510"
      "907788019393178114194545257223865541461062892187960223838971476088506276"
      "862967146674697562911234082439208160153780889893964518263243671616762179"
      "168909779911903754031274622289988005195444414282012187361745992642956581"
      "746628302955570299024324153181617210465832036786906117260158783520751516"
      "284225540265170483304226143974286933061690897968482590125458327168226458"
      "066526769958652682272807075781391858178889652208164348344825993266043367"
      "660176999612831860788386150279465955131156552036093988180612138558600301"
      "435694527224206344631797460594682573103790084024432438465657245014402821"
      "885252470935190620929023136493273497565513958720559654228749774011413346"
      "962715422845862377387538230483865688976461927383814900140767310446640259"
      "899490222221765904339901886018566526485061799702356193897017860040811889"
      "729918311021171229845901641921068884387121855646124960798722908519296819"
      "372388642614839657382291123125024186649353143970137428531926649875337218"
      "940694281434118520158014123344828015051399694290153483077644569099073152"
      "433278288269864602789864321139083506217095002597389863554277196742822248"
      "757586765752344220207573630569498825087968928162753848863396909959826280"
      "956121450994871701244516461260379029309120889086942028510640182154399457"
      "156805941872748998094254742173582401063677404595741785160829230135358081"
      "840096996372524230560855903700624271243416909004153690105933983835777939"
      "410970027753472000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000");
  mu_check(!bigint_cmp(a, b));

  bifree(a);
  bifree(b);
}

MU_TEST(divdecpos) {
  bigint_t *a = bigint_from_str("12345678998");
  bigint_t *b = bigint_from_int(5);
  bigint_t *c = bigint_from_str("123456789985211");
  bigint_t *d = bigint_from_int(7);

  bigint_t *res1 = bigint_div(a, b);
  char *s1 = bigint_to_str(res1);
  mu_assert_string_eq("2469135799", s1);

  bigint_t *res2 = bigint_div(c, d);
  char *s2 = bigint_to_str(res2);
  mu_assert_string_eq("17636684283601", s2);

  bigint_t *res3 = bigint_div(d, b);
  mu_assert_int_eq(1, bigint_to_int(res3));

  bigint_t *res4 = bigint_div(b, d);
  mu_assert_int_eq(0, bigint_to_int(res4));

  bifree(a);
  bifree(b);
  bifree(c);
  bifree(d);
  bifree(res1);
  bifree(res2);
  bifree(res3);
  bifree(res4);
  free(s1);
  free(s2);
}

MU_TEST(divdecneg) {
  bigint_t *a = bigint_from_int(12);
  bigint_t *b = bigint_from_int(-5);
  bigint_t *d = bigint_from_int(-7);
  bigint_t *e = bigint_from_int(3);

  bigint_t *res1 = bigint_div(a, b);
  mu_assert_int_eq(-2, bigint_to_int(res1));

  bigint_t *res2 = bigint_div(d, e);
  mu_assert_int_eq(-3, bigint_to_int(res2));

  bigint_t *res3 = bigint_div(b, e);
  mu_assert_int_eq(-2, bigint_to_int(res3));

  bifree(a);
  bifree(b);
  bifree(d);
  bifree(e);
  bifree(res1);
  bifree(res2);
  bifree(res3);
}

MU_TEST(bidiv) {
  bigint_t *w = bigint_from_int(300);
  bigint_t *a = bigint_from_int(52621);
  bigint_t *b = bigint_from_int(455);
  bigint_t *neg_a = bigint_from_int(-52621);
  bigint_t *neg_b = bigint_from_int(-455);

  /* 4100/588 enables addback step. */
  bigint_t *c = bigint_from_int(4100);
  bigint_t *d = bigint_from_int(588);

  bigint_t *e = bigint_from_str("99899999001");
  bigint_t *f = bigint_from_str("99999999");

  bigint_t *res1 = bigint_div(a, b);
  mu_assert_int_eq(115, bigint_to_int(res1));

  bigint_t *res2 = bigint_div(c, d);
  mu_assert_int_eq(6, bigint_to_int(res2));

  bigint_t *res3 = bigint_div(e, f);
  mu_assert_int_eq(999, bigint_to_int(res3));

  bigint_t *res4 = bigint_div(b, a);
  mu_assert_int_eq(0, bigint_to_int(res4));

  bigint_t *res5 = bigint_div(a, neg_b);
  mu_assert_int_eq(-115, bigint_to_int(res5));

  bigint_t *res6 = bigint_div(w, neg_b);
  mu_assert_int_eq(0, bigint_to_int(res6));

  bigint_t *res7 = bigint_div(neg_a, b);
  mu_assert_int_eq(-116, bigint_to_int(res7));

  bigint_t *res8 = bigint_div(neg_b, a);
  mu_assert_int_eq(-1, bigint_to_int(res8));

  bigint_t *res9 = bigint_div(neg_a, neg_b);
  mu_assert_int_eq(116, bigint_to_int(res9));

  bigint_t *res10 = bigint_div(neg_b, neg_a);
  mu_assert_int_eq(1, bigint_to_int(res10));

  bifree(w);
  bifree(a);
  bifree(b);
  bifree(c);
  bifree(d);
  bifree(e);
  bifree(f);
  bifree(neg_a);
  bifree(neg_b);
  bifree(res1);
  bifree(res2);
  bifree(res3);
  bifree(res4);
  bifree(res5);
  bifree(res6);
  bifree(res7);
  bifree(res8);
  bifree(res9);
  bifree(res10);
}

MU_TEST(bigint_gcd) {
  bigint_t *a = bigint_from_str("14");
  bigint_t *b = bigint_from_str("12");
  bigint_t *c = bigint_from_str("125212666666662124203582305152351360");
  bigint_t *d = bigint_from_str("4205023020002352");

  bigint_t *res1 = gcd(a, b);
  mu_assert_int_eq(2, bigint_to_int(res1));

  bigint_t *res2 = gcd(c, d);
  mu_assert_int_eq(16, bigint_to_int(res2));

  bifree(a);
  bifree(b);
  bifree(c);
  bifree(d);
  bifree(res1);
  bifree(res2);
}

MU_TEST(bigint_cmp_test) {
  bigint_t *a = bigint_from_int(1);
  bigint_t *b = bigint_from_int(0);
  bigint_t *c = bigint_from_int(-2);

  mu_check(bigint_cmp(a, b) == 1);
  mu_check(bigint_cmp(a, c) == 1);
  mu_check(bigint_cmp(b, a) == -1);
  mu_check(bigint_cmp(b, c) == 1);
  mu_check(bigint_cmp(c, a) == -1);
  mu_check(bigint_cmp(c, b) == -1);
  mu_check(bigint_cmp(c, c) == 0);

  bifree(a);
  bifree(b);
  bifree(c);
}

int main() {
  MU_RUN_TEST(from_size);
  MU_RUN_TEST(from_str);
  MU_RUN_TEST(from_int);
  MU_RUN_TEST(padding);
  MU_RUN_TEST(sum);
  MU_RUN_TEST(sub);
  MU_RUN_TEST(mul);
  MU_RUN_TEST(nfactorial);
  MU_RUN_TEST(divdecpos);
  MU_RUN_TEST(divdecneg);
  MU_RUN_TEST(bidiv);
  MU_RUN_TEST(bigint_gcd);
  MU_RUN_TEST(bigint_cmp_test);
  MU_REPORT();
  return MU_EXIT_CODE;
}
