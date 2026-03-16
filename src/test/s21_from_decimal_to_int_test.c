#include <check.h>
#include <limits.h>

#include "../s21_decimal.h"

START_TEST(test_s21_from_decimal_to_int_positive) {
  s21_decimal src = {.bits = {123456, 0, 0, 0}};
  int dst;
  int error = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(dst, 123456);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int_negative) {
  s21_decimal src = {.bits = {123456, 0, 0, 0x80000000}};
  int dst;
  int error = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(dst, -123456);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int_overflow) {
  s21_decimal src = {.bits = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}};
  int dst;
  int error = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int_underflow) {
  s21_decimal src = {.bits = {0x80000000, 0x00000000, 0x00000000, 0x80000000}};
  int dst;
  int error = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int_zero) {
  s21_decimal src = {.bits = {0, 0, 0, 0}};
  int dst;
  int error = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(dst, 0);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int_positive_with_fraction) {
  s21_decimal src = {.bits = {123456, 0, 0, 0x00020000}};
  int dst;
  int error = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(dst, 1234);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int_negative_with_fraction) {
  s21_decimal src = {.bits = {123456, 0, 0, 0x80020000}};
  int dst;
  int error = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(dst, -1234);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int_max_int) {
  s21_decimal src = {.bits = {INT_MAX, 0, 0, 0}};
  int dst;
  int error = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(dst, INT_MAX);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int_positive_overflow) {
  s21_decimal src = {.bits = {INT_MAX, 1, 0, 0}};
  int dst;
  int error = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int_negative_overflow) {
  s21_decimal src = {.bits = {INT_MAX, 1, 0, 0x80000000}};
  int dst;
  int error = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int_no_scale) {
  s21_decimal src = {.bits = {987654321, 0, 0, 0x00000000}};
  int dst;
  int error = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(dst, 987654321);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int_negative_no_scale) {
  s21_decimal src = {.bits = {987654321, 0, 0, 0x80000000}};
  int dst;
  int error = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(dst, -987654321);
}
END_TEST
START_TEST(test_s21_from_decimal_to_int_null_dst) {
  s21_decimal src;
  int *dst = NULL;
  int result = s21_from_decimal_to_int(src, dst);
  ck_assert_int_eq(result, 1);
}
END_TEST

Suite *from_decimal_to_int_suite(void) {
  Suite *suite = suite_create("s21_from_decimal_to_int");
  TCase *test_case = tcase_create("case_from_decimal_to_int");

  tcase_add_test(test_case, test_s21_from_decimal_to_int_null_dst);
  tcase_add_test(test_case, test_s21_from_decimal_to_int_positive);
  tcase_add_test(test_case, test_s21_from_decimal_to_int_negative);
  tcase_add_test(test_case, test_s21_from_decimal_to_int_overflow);
  tcase_add_test(test_case, test_s21_from_decimal_to_int_underflow);
  tcase_add_test(test_case, test_s21_from_decimal_to_int_zero);
  tcase_add_test(test_case,
                 test_s21_from_decimal_to_int_positive_with_fraction);
  tcase_add_test(test_case,
                 test_s21_from_decimal_to_int_negative_with_fraction);
  tcase_add_test(test_case, test_s21_from_decimal_to_int_max_int);
  tcase_add_test(test_case, test_s21_from_decimal_to_int_positive_overflow);
  tcase_add_test(test_case, test_s21_from_decimal_to_int_negative_overflow);
  tcase_add_test(test_case, test_s21_from_decimal_to_int_no_scale);
  tcase_add_test(test_case, test_s21_from_decimal_to_int_negative_no_scale);

  suite_add_tcase(suite, test_case);
  return suite;
}

int main() {
  Suite *suite = from_decimal_to_int_suite();
  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int failures = srunner_ntests_failed(runner);
  srunner_free(runner);
  printf("\033[41m-========= FAILED: %d ==========-\033[0m\n", failures);
  return failures == 0 ? 0 : 1;
}