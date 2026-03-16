#include <check.h>

#include "../s21_decimal.h"

START_TEST(test_s21_negate_error_null_result) {
  s21_decimal value;
  int result = s21_negate(value, NULL);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_s21_negate_error_scale_too_large) {
  s21_decimal value;
  set_scale(&value, 29);
  s21_decimal result;
  int res = s21_negate(value, &result);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_negate_positive) {
  s21_decimal value = {{123456789, 0, 0, 0}};
  s21_decimal result;
  s21_negate(value, &result);
  s21_decimal expected = {{123456789, 0, 0, 0x80000000}};
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(result.bits[1], expected.bits[1]);
  ck_assert_int_eq(result.bits[2], expected.bits[2]);
  ck_assert_int_eq(result.bits[3], expected.bits[3]);
}

START_TEST(test_s21_negate_negative) {
  s21_decimal value = {{123456789, 0, 0, 0x80000000}};
  s21_decimal result;
  s21_negate(value, &result);
  s21_decimal expected = {{123456789, 0, 0, 0}};
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(result.bits[1], expected.bits[1]);
  ck_assert_int_eq(result.bits[2], expected.bits[2]);
  ck_assert_int_eq(result.bits[3], expected.bits[3]);
}

START_TEST(test_s21_negate_fractional) {
  s21_decimal value = {{123456789, 987654321, 0, 0}};
  s21_decimal result;
  s21_negate(value, &result);
  s21_decimal expected = {{123456789, 987654321, 0, 0x80000000}};
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(result.bits[1], expected.bits[1]);
  ck_assert_int_eq(result.bits[2], expected.bits[2]);
  ck_assert_int_eq(result.bits[3], expected.bits[3]);
}

START_TEST(test_s21_negate_min) {
  s21_decimal value = {{0, 0, 0, 0}};
  s21_decimal result;
  s21_negate(value, &result);
  s21_decimal expected = {{0, 0, 0, 0x80000000}};
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(result.bits[1], expected.bits[1]);
  ck_assert_int_eq(result.bits[2], expected.bits[2]);
  ck_assert_int_eq(result.bits[3], expected.bits[3]);
}

START_TEST(test_s21_negate_max) {
  s21_decimal value = {{0xFFFFFFFF, 0xFFFFFFFF, 0, 0}};
  s21_decimal result;
  s21_negate(value, &result);
  s21_decimal expected = {{0xFFFFFFFF, 0xFFFFFFFF, 0, 0x80000000}};
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(result.bits[1], expected.bits[1]);
  ck_assert_int_eq(result.bits[2], expected.bits[2]);
  ck_assert_int_eq(result.bits[3], expected.bits[3]);
}

START_TEST(test_s21_negate_zero) {
  s21_decimal value = {{0, 0, 0, 0}};
  s21_decimal result;
  s21_negate(value, &result);
  s21_decimal expected = {{0, 0, 0, 0x80000000}};
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(result.bits[1], expected.bits[1]);
  ck_assert_int_eq(result.bits[2], expected.bits[2]);
  ck_assert_int_eq(result.bits[3], expected.bits[3]);
}



Suite *negate_suite(void) {
  Suite *suite = suite_create("s21_negate");
  TCase *test_case = tcase_create("case_negate");

  tcase_add_test(test_case, test_s21_negate_error_null_result);
  tcase_add_test(test_case, test_s21_negate_error_scale_too_large);
  tcase_add_test(test_case, test_s21_negate_positive);
  tcase_add_test(test_case, test_s21_negate_negative);
  tcase_add_test(test_case, test_s21_negate_fractional);
  tcase_add_test(test_case, test_s21_negate_min);
  tcase_add_test(test_case, test_s21_negate_max);
  tcase_add_test(test_case, test_s21_negate_zero);

  suite_add_tcase(suite, test_case);
  return suite;
}

int main() {
  Suite *suite = negate_suite();
  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int failures = srunner_ntests_failed(runner);
  srunner_free(runner);
  printf("\033[41m-========= FAILED: %d ==========-\033[0m\n", failures);
  return failures == 0 ? 0 : 1;
}