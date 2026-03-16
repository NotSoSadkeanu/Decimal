#include <check.h>
#include <limits.h>

#include "../s21_decimal.h"

START_TEST(test_s21_from_int_to_decimal_null_pointer) {
    int src = 123;
    s21_decimal *dst = NULL;

    int result = s21_from_int_to_decimal(src, dst);

    ck_assert_int_eq(result, 1);
}
END_TEST


START_TEST(test_s21_from_int_to_decimal_1) {
  s21_decimal result;

  int input = 67890;
  ck_assert_int_eq(s21_from_int_to_decimal(input, &result), 0);
  ck_assert_int_eq(result.bits[0], input);

  input = INT_MAX;
  ck_assert_int_eq(s21_from_int_to_decimal(input, &result), 0);
  ck_assert_int_eq(result.bits[0], input);

  input = INT_MIN;
  ck_assert_int_eq(s21_from_int_to_decimal(input, &result), 0);
  ck_assert_int_eq(result.bits[0], -input);

  input = -98765;
  ck_assert_int_eq(s21_from_int_to_decimal(input, &result), 0);
  ck_assert_int_eq(result.bits[0], -input);

  input = 0;
  ck_assert_int_eq(s21_from_int_to_decimal(input, &result), 0);
  ck_assert_int_eq(result.bits[0], 0);
}
END_TEST

START_TEST(test_s21_from_int_to_decimal_positive) {
  int src = 123456;
  s21_decimal dst;
  int error = s21_from_int_to_decimal(src, &dst);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(dst.bits[0], 123456);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(test_s21_from_int_to_decimal_overflow) {
  int src = 0x7FFFFFFF;
  s21_decimal dst;
  int error = s21_from_int_to_decimal(src, &dst);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(dst.bits[0], 0x7FFFFFFF);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(test_s21_from_int_to_decimal_zero) {
  int src = 0;
  s21_decimal dst;
  int error = s21_from_int_to_decimal(src, &dst);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(dst.bits[0], 0);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(test_s21_from_int_to_decimal_positive_one) {
  int src = 1;
  s21_decimal dst;
  int error = s21_from_int_to_decimal(src, &dst);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(dst.bits[0], 1);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(test_s21_from_int_to_decimal_max_positive) {
  int src = INT_MAX;
  s21_decimal dst;
  int error = s21_from_int_to_decimal(src, &dst);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(dst.bits[0], INT_MAX);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(test_s21_from_int_to_decimal_max_int) {
  int src = INT_MAX;
  s21_decimal dst;
  int error = s21_from_int_to_decimal(src, &dst);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(dst.bits[0], INT_MAX);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(test_s21_from_int_to_decimal_int_max) {
  int src = INT_MAX;
  s21_decimal dst;
  int error = s21_from_int_to_decimal(src, &dst);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(dst.bits[0], 0x7FFFFFFF);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

Suite *from_int_to_decimal_suite(void) {
  Suite *suite = suite_create("s21_from_int_to_decimal");
  TCase *test_case = tcase_create("case_from_int_to_decimal");

  tcase_add_test(test_case, test_s21_from_int_to_decimal_null_pointer);
  tcase_add_test(test_case, test_s21_from_int_to_decimal_1);
  tcase_add_test(test_case, test_s21_from_int_to_decimal_positive);
  tcase_add_test(test_case, test_s21_from_int_to_decimal_overflow);
  tcase_add_test(test_case, test_s21_from_int_to_decimal_zero);
  tcase_add_test(test_case, test_s21_from_int_to_decimal_positive_one);
  tcase_add_test(test_case, test_s21_from_int_to_decimal_max_positive);
  tcase_add_test(test_case, test_s21_from_int_to_decimal_int_max);
  tcase_add_test(test_case, test_s21_from_int_to_decimal_max_int);

  suite_add_tcase(suite, test_case);
  return suite;
}

int main() {
  Suite *suite = from_int_to_decimal_suite();
  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int failures = srunner_ntests_failed(runner);
  srunner_free(runner);
  printf("\033[41m-========= FAILED: %d ==========-\033[0m\n", failures);
  return failures == 0 ? 0 : 1;
}