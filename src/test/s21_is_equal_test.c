#include <check.h>

#include "../s21_decimal.h"

START_TEST(test_s21_is_equal_positive_equal) {
  s21_decimal value1 = {
      .bits = {0x0000000A, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal value2 = {
      .bits = {0x0000000A, 0x00000000, 0x00000000, 0x00000000}};
  int result = s21_is_equal(value1, value2);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_s21_is_equal_positive_not_equal) {
  s21_decimal value1 = {
      .bits = {0x0000000A, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal value2 = {
      .bits = {0x0000000B, 0x00000000, 0x00000000, 0x00000000}};
  int result = s21_is_equal(value1, value2);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_s21_is_equal_negative_equal) {
  s21_decimal value1 = {
      .bits = {0x0000000A, 0x00000000, 0x00000000, 0x80000000}};
  s21_decimal value2 = {
      .bits = {0x0000000A, 0x00000000, 0x00000000, 0x80000000}};
  int result = s21_is_equal(value1, value2);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_s21_is_equal_negative_not_equal) {
  s21_decimal value1 = {
      .bits = {0x0000000A, 0x00000000, 0x00000000, 0x80000000}};
  s21_decimal value2 = {
      .bits = {0x0000000B, 0x00000000, 0x00000000, 0x80000000}};
  int result = s21_is_equal(value1, value2);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_s21_is_equal_positive_negative) {
  s21_decimal value1 = {
      .bits = {0x0000000A, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal value2 = {
      .bits = {0x0000000A, 0x00000000, 0x00000000, 0x80000000}};
  int result = s21_is_equal(value1, value2);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_s21_is_equal_negative_positive) {
  s21_decimal value1 = {
      .bits = {0x0000000A, 0x00000000, 0x00000000, 0x80000000}};
  s21_decimal value2 = {
      .bits = {0x0000000A, 0x00000000, 0x00000000, 0x00000000}};
  int result = s21_is_equal(value1, value2);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_s21_is_equal_positive_max_value) {
  s21_decimal value1 = {
      .bits = {0x7FFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}};
  s21_decimal value2 = {
      .bits = {0x7FFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}};
  int result = s21_is_equal(value1, value2);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_s21_is_equal_negative_min_value) {
  s21_decimal value1 = {
      .bits = {0x80000000, 0x00000000, 0x00000000, 0x80000000}};
  s21_decimal value2 = {
      .bits = {0x80000000, 0x00000000, 0x00000000, 0x80000000}};
  int result = s21_is_equal(value1, value2);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_s21_is_equal_positive_fraction) {
  s21_decimal value1 = {.bits = {0x0000000A, 0x00000000, 0x00000000,
                                 0x00000001}};
  s21_decimal value2 = {.bits = {0x0000000A, 0x00000000, 0x00000000,
                                 0x00000001}};
  int result = s21_is_equal(value1, value2);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_s21_is_equal_negative_fraction) {
  s21_decimal value1 = {
      .bits = {0x0000000A, 0x00000000, 0x00000000,
               0x80000001}};
  s21_decimal value2 = {
      .bits = {0x0000000A, 0x00000000, 0x00000000,
               0x80000001}};
  int result = s21_is_equal(value1, value2);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_s21_is_equal_positive_zero) {
  s21_decimal value1 = {
      .bits = {0x00000000, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal value2 = {
      .bits = {0x00000000, 0x00000000, 0x00000000, 0x00000000}};
  int result = s21_is_equal(value1, value2);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_s21_is_equal_negative_zero) {
  s21_decimal value1 = {
      .bits = {0x00000000, 0x00000000, 0x00000000, 0x80000000}};
  s21_decimal value2 = {
      .bits = {0x00000000, 0x00000000, 0x00000000, 0x80000000}};
  int result = s21_is_equal(value1, value2);
  ck_assert_int_eq(result, 1);
}
END_TEST

Suite *is_equal_suite(void) {
  Suite *suite = suite_create("s21_is_equal");
  TCase *test_case = tcase_create("case_is_equal");

  tcase_add_test(test_case, test_s21_is_equal_positive_equal);
  tcase_add_test(test_case, test_s21_is_equal_positive_not_equal);
  tcase_add_test(test_case, test_s21_is_equal_negative_equal);
  tcase_add_test(test_case, test_s21_is_equal_negative_not_equal);
  tcase_add_test(test_case, test_s21_is_equal_positive_negative);
  tcase_add_test(test_case, test_s21_is_equal_negative_positive);
  tcase_add_test(test_case, test_s21_is_equal_positive_max_value);
  tcase_add_test(test_case, test_s21_is_equal_negative_min_value);
  tcase_add_test(test_case, test_s21_is_equal_positive_fraction);
  tcase_add_test(test_case, test_s21_is_equal_negative_fraction);
  tcase_add_test(test_case, test_s21_is_equal_positive_zero);
  tcase_add_test(test_case, test_s21_is_equal_negative_zero);

  suite_add_tcase(suite, test_case);
  return suite;
}

int main() {
  Suite *suite = is_equal_suite();
  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int failures = srunner_ntests_failed(runner);
  srunner_free(runner);
  printf("\033[41m-========= FAILED: %d ==========-\033[0m\n", failures);
  return failures == 0 ? 0 : 1;
}