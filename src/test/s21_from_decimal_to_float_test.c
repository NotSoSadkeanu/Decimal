#include <check.h>

#include "../s21_decimal.h"

START_TEST(test_s21_from_decimal_to_float_positive) {
  s21_decimal src = {.bits = {123456, 0, 0, 0x00030000}};
  float dst;
  int error = s21_from_decimal_to_float(src, &dst);
  ck_assert_int_eq(error, 0);
  ck_assert_float_eq(dst, 123.456);
}
END_TEST

START_TEST(test_s21_from_decimal_to_float_negative) {
  s21_decimal src = {.bits = {123456, 0, 0, 0x80030000}};
  float dst;
  int error = s21_from_decimal_to_float(src, &dst);
  ck_assert_int_eq(error, 0);
  ck_assert_float_eq(dst, -123.456);
}
END_TEST

START_TEST(test_s21_from_decimal_to_float_zero) {
  s21_decimal src = {.bits = {0, 0, 0, 0}};
  float dst;
  int error = s21_from_decimal_to_float(src, &dst);
  ck_assert_int_eq(error, 0);
  ck_assert_float_eq(dst, 0.0);
}
END_TEST

START_TEST(test_s21_from_decimal_to_float_small_fraction) {
  s21_decimal src = {.bits = {1, 0, 0, 0x001E0000}};
  float dst;
  int error = s21_from_decimal_to_float(src, &dst);
  ck_assert_int_eq(error, 0);
  ck_assert(dst < 1e-28);
}
END_TEST

START_TEST(test_s21_from_decimal_to_float_rounding) {
  s21_decimal src = {
      .bits = {123456789, 0, 0, 0x00060000}}; // -0
  float dst;
  int error = s21_from_decimal_to_float(src, &dst);
  ck_assert_int_eq(error, 0);
  ck_assert_float_eq(dst, 123.45679);
}
END_TEST
 // -0

START_TEST(test_s21_from_decimal_to_float_no_scale) {
  s21_decimal src = {.bits = {123456, 0, 0, 0}};
  float dst;
  int error = s21_from_decimal_to_float(src, &dst);
  ck_assert_int_eq(error, 0);
  ck_assert_float_eq(dst, 123456);
}
END_TEST

START_TEST(test_s21_from_decimal_to_float_negative_with_fraction) {
  s21_decimal src = {
      .bits = {123456789, 0, 0, 0x80060000}};
  float dst;
  int error = s21_from_decimal_to_float(src, &dst);
  ck_assert_int_eq(error, 0);
  ck_assert_float_eq(dst, -123.45679);
}
END_TEST


Suite *from_float_to_decimal_suite(void) {
  Suite *suite = suite_create("s21_from_decimal_to_float");
  TCase *test_case = tcase_create("case_from_decimal_to_float");

  tcase_add_test(test_case, test_s21_from_decimal_to_float_positive);
  tcase_add_test(test_case, test_s21_from_decimal_to_float_negative);
  tcase_add_test(test_case, test_s21_from_decimal_to_float_zero);
  tcase_add_test(test_case, test_s21_from_decimal_to_float_small_fraction);
  tcase_add_test(test_case, test_s21_from_decimal_to_float_rounding);
  tcase_add_test(test_case, test_s21_from_decimal_to_float_no_scale);
  tcase_add_test(test_case,
                 test_s21_from_decimal_to_float_negative_with_fraction);

  suite_add_tcase(suite, test_case);
  return suite;
}

int main() {
  Suite *suite = from_float_to_decimal_suite();
  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int failures = srunner_ntests_failed(runner);
  srunner_free(runner);
  printf("\033[41m-========= FAILED: %d ==========-\033[0m\n", failures);
  return failures == 0 ? 0 : 1;
}