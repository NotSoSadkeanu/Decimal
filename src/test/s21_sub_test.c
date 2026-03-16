#include <check.h>

#include "../s21_decimal.h"

START_TEST(test_s21_sub_positive) {
  s21_decimal value_1 = {.bits = {67890, 0, 0, 0}};
  s21_decimal value_2 = {.bits = {12345, 0, 0, 0}};
  s21_decimal result;
  int error = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], 55545);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

START_TEST(test_s21_sub_negative) {
  s21_decimal value_1 = {.bits = {-12345, 0, 0, 0}};
  s21_decimal value_2 = {.bits = {-67890, 0, 0, 0}};
  s21_decimal result;
  int error = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], 55545);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

START_TEST(test_s21_sub_positive_result) {
  s21_decimal value_1 = {
      .bits = {0x0000000A, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal value_2 = {
      .bits = {0x00000005, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal result = {0};
  int error_code = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(error_code, 0);
}
END_TEST

START_TEST(test_s21_sub_negative_result) {
  s21_decimal value_1 = {
      .bits = {0x00000005, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal value_2 = {
      .bits = {0x0000000A, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal result = {0};
  int error_code = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(error_code, 0);
}
END_TEST

START_TEST(test_s21_sub_large_values) {
  s21_decimal value_1 = {
      .bits = {0x7FFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}};
  s21_decimal value_2 = {
      .bits = {0x00000001, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal result = {0};
  int error_code = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(error_code, 0);
}
END_TEST

START_TEST(test_s21_sub_small_values) {
  s21_decimal value_1 = {
      .bits = {0x00000001, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal value_2 = {
      .bits = {0x00000001, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal result = {0};
  int error_code = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(error_code, 0);
  ck_assert_int_eq(result.bits[0], 0);
}
END_TEST

START_TEST(test_s21_sub_same_values) {
  s21_decimal value_1 = {
      .bits = {0x0000000A, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal value_2 = {
      .bits = {0x0000000A, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal result = {0};
  int error_code = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(error_code, 0);
}
END_TEST

START_TEST(test_s21_sub_min_values) {
  s21_decimal value_1 = {
      .bits = {0x80000000, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal value_2 = {
      .bits = {0x80000000, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal result = {0};
  int error_code = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(error_code, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

START_TEST(test_s21_sub_zero_values) {
  s21_decimal value_1 = {
      .bits = {0x00000000, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal value_2 = {
      .bits = {0x00000000, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal result = {0};
  int error_code = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(error_code, 0);
  ck_assert_int_eq(result.bits[0], 0);
}
END_TEST

START_TEST(test_s21_sub_large_positive_and_small_negative) {
  s21_decimal value_1 = {
      .bits = {0x7FFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}};
  s21_decimal value_2 = {
      .bits = {0x80000001, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal result = {0};
  int error_code = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(error_code, 0);
}
END_TEST

START_TEST(test_s21_sub_large_negative_and_small_positive) {
  s21_decimal value_1 = {
      .bits = {0x80000001, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal value_2 = {
      .bits = {0x7FFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}};
  s21_decimal result = {0};
  int error_code = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(error_code, 0);
}
END_TEST

START_TEST(test_s21_sub_positive_fraction) {
  s21_decimal value1 = {.bits = {0x0000000F, 0x00000000, 0x00000000,
                                 0x00000003}};
  s21_decimal value2 = {.bits = {0x00000007, 0x00000000, 0x00000000,
                                 0x00000004}};
  s21_decimal result = {0};
  int error_code = s21_sub(value1, value2, &result);
  ck_assert_int_eq(error_code, 0);
  ck_assert_int_eq(result.bits[0], 0x00000008);
  ck_assert_int_eq(result.bits[3], 0x00000000);
}
END_TEST

START_TEST(test_s21_sub_positive_fraction_negative) {
  s21_decimal value1 = {.bits = {0x0000000F, 0x00000000, 0x00000000,
                                 0x00000003}};
  s21_decimal value2 = {.bits = {0x00000007, 0x00000000, 0x00000000,
                                 0x80000004}};
  s21_decimal result = {0};
  int error_code = s21_sub(value1, value2, &result);
  ck_assert_int_eq(error_code, 0);
  ck_assert_int_eq(result.bits[0], 0x00000016);
  ck_assert_int_eq(result.bits[3], 0x00000000);
}
END_TEST

Suite *sub_suite(void) {
  Suite *suite = suite_create("s21_sub");
  TCase *test_case = tcase_create("case_sub");

  tcase_add_test(test_case, test_s21_sub_positive);
  tcase_add_test(test_case, test_s21_sub_negative);
  tcase_add_test(test_case, test_s21_sub_positive_result);
  tcase_add_test(test_case, test_s21_sub_negative_result);
  tcase_add_test(test_case, test_s21_sub_large_values);
  tcase_add_test(test_case, test_s21_sub_small_values);
  tcase_add_test(test_case, test_s21_sub_same_values);
  tcase_add_test(test_case, test_s21_sub_min_values);
  tcase_add_test(test_case, test_s21_sub_zero_values);
  tcase_add_test(test_case, test_s21_sub_large_positive_and_small_negative);
  tcase_add_test(test_case, test_s21_sub_large_negative_and_small_positive);
  tcase_add_test(test_case, test_s21_sub_positive_fraction);
  tcase_add_test(test_case, test_s21_sub_positive_fraction_negative);

  suite_add_tcase(suite, test_case);
  return suite;
}

int main() {
  Suite *suite = sub_suite();
  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int failures = srunner_ntests_failed(runner);
  srunner_free(runner);
  printf("\033[41m-========= FAILED: %d ==========-\033[0m\n", failures);
  return failures == 0 ? 0 : 1;
}
