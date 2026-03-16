#include <check.h>

#include "../s21_decimal.h"

START_TEST(test_s21_is_not_equal_positive) {
  s21_decimal value1 = {{123456789, 0, 0, 0}};
  s21_decimal value2 = {{987654321, 0, 0, 0}};
  int result = s21_is_not_equal(value1, value2);
  ck_assert_int_eq(result, 1);
}

START_TEST(test_s21_is_not_equal_negative) {
  s21_decimal value1 = {{123456789, 0, 0, 0x80000000}};
  s21_decimal value2 = {{987654321, 0, 0, 0x80000000}};
  int result = s21_is_not_equal(value1, value2);
  ck_assert_int_eq(result, 1);
}

START_TEST(test_s21_is_not_equal_fractional) {
  s21_decimal value1 = {{123456789, 987654321, 0, 0}};
  s21_decimal value2 = {{123456789, 987654320, 0, 0}};
  int result = s21_is_not_equal(value1, value2);
  ck_assert_int_eq(result, 1);
}

START_TEST(test_s21_is_not_equal_equal) {
  s21_decimal value1 = {{123456789, 0, 0, 0}};
  s21_decimal value2 = {{123456789, 0, 0, 0}};
  int result = s21_is_not_equal(value1, value2);
  ck_assert_int_eq(result, 0);
}

START_TEST(test_s21_is_not_equal_min) {
  s21_decimal value1 = {{0, 0, 0, 0}};
  s21_decimal value2 = {{0, 0, 0, 0x80000000}};
  int result = s21_is_not_equal(value1, value2);
  ck_assert_int_eq(result, 1);
}

START_TEST(test_s21_is_not_equal_max) {
  s21_decimal value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0, 0}};
  s21_decimal value2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0, 0x80000000}};
  int result = s21_is_not_equal(value1, value2);
  ck_assert_int_eq(result, 1);
}

START_TEST(test_s21_is_not_equal_positive_zero) {
  s21_decimal value1 = {{0, 0, 0, 0}};
  s21_decimal value2 = {{0, 0, 0, 0}};
  int result = s21_is_not_equal(value1, value2);
  ck_assert_int_eq(result, 0);
}

START_TEST(test_s21_is_not_equal_negative_zero) {
  s21_decimal value1 = {{0, 0, 0, 0x80000000}};
  s21_decimal value2 = {{0, 0, 0, 0x80000000}};
  int result = s21_is_not_equal(value1, value2);
  ck_assert_int_eq(result, 0);
}

START_TEST(test_s21_is_not_equal_positive_fractional_zero) {
  s21_decimal value1 = {{0, 1, 0, 0}};
  s21_decimal value2 = {{0, 0, 0, 0}};
  int result = s21_is_not_equal(value1, value2);
  ck_assert_int_eq(result, 1);
}

START_TEST(test_s21_is_not_equal_negative_fractional_zero) {
  s21_decimal value1 = {{0, 1, 0, 0x80000000}};
  s21_decimal value2 = {{0, 0, 0, 0x80000000}};
  int result = s21_is_not_equal(value1, value2);
  ck_assert_int_eq(result, 1);
}

START_TEST(test_s21_is_not_equal_positive_max) {
  s21_decimal value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0, 0}};
  s21_decimal value2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0, 0}};
  int result = s21_is_not_equal(value1, value2);
  ck_assert_int_eq(result, 0);
}

START_TEST(test_s21_is_not_equal_negative_max) {
  s21_decimal value1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0, 0x80000000}};
  s21_decimal value2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0, 0x80000000}};
  int result = s21_is_not_equal(value1, value2);
  ck_assert_int_eq(result, 0);
}

START_TEST(test_s21_is_not_equal_positive_min) {
  s21_decimal value1 = {{0, 0, 0, 0}};
  s21_decimal value2 = {{0, 0, 0, 0}};
  int result = s21_is_not_equal(value1, value2);
  ck_assert_int_eq(result, 0);
}

START_TEST(test_s21_is_not_equal_negative_min) {
  s21_decimal value1 = {{0, 0, 0, 0x80000000}};
  s21_decimal value2 = {{0, 0, 0, 0x80000000}};
  int result = s21_is_not_equal(value1, value2);
  ck_assert_int_eq(result, 0);
}

Suite *is_not_equal_suite(void) {
  Suite *suite = suite_create("s21_is_not_equal");
  TCase *test_case = tcase_create("case_is_not_equal");

  tcase_add_test(test_case, test_s21_is_not_equal_positive);
  tcase_add_test(test_case, test_s21_is_not_equal_negative);
  tcase_add_test(test_case, test_s21_is_not_equal_fractional);
  tcase_add_test(test_case, test_s21_is_not_equal_equal);
  tcase_add_test(test_case, test_s21_is_not_equal_min);
  tcase_add_test(test_case, test_s21_is_not_equal_max);
  tcase_add_test(test_case, test_s21_is_not_equal_positive_zero);
  tcase_add_test(test_case, test_s21_is_not_equal_negative_zero);
  tcase_add_test(test_case, test_s21_is_not_equal_positive_fractional_zero);
  tcase_add_test(test_case, test_s21_is_not_equal_negative_fractional_zero);
  tcase_add_test(test_case, test_s21_is_not_equal_positive_max);
  tcase_add_test(test_case, test_s21_is_not_equal_negative_max);
  tcase_add_test(test_case, test_s21_is_not_equal_positive_min);
  tcase_add_test(test_case, test_s21_is_not_equal_negative_min);

  suite_add_tcase(suite, test_case);
  return suite;
}

int main() {
  Suite *suite = is_not_equal_suite();
  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int failures = srunner_ntests_failed(runner);
  srunner_free(runner);
  printf("\033[41m-========= FAILED: %d ==========-\033[0m\n", failures);
  return failures == 0 ? 0 : 1;
}