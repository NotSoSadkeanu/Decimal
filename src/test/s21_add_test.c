#include <check.h>

#include "../s21_decimal.h"

START_TEST(test_s21_add_positive) {
  s21_decimal value_1 = {{12345, 0, 0, 0}};
  s21_decimal value_2 = {{67890, 0, 0, 0}};
  s21_decimal result;
  ck_assert_int_eq(s21_add(value_1, value_2, &result), 0);
  ck_assert_int_eq(result.bits[0], 80235);
}
END_TEST

START_TEST(test_s21_add_with_zero) {
  s21_decimal value_1 = {{12345, 0, 0, 0}};
  s21_decimal value_2 = {{0, 0, 0, 0}};
  s21_decimal result;
  ck_assert_int_eq(s21_add(value_1, value_2, &result), 0);
  ck_assert_int_eq(result.bits[0], 12345);
}
END_TEST

START_TEST(test_s21_add_overflow) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{1, 0, 0, 0}};
  s21_decimal result;
  ck_assert_int_eq(s21_add(value_1, value_2, &result), 1);
}
END_TEST

START_TEST(test_s21_add_positive_overflow) {
  s21_decimal value_1 = {
      .bits = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}};
  s21_decimal value_2 = {.bits = {1, 0, 0, 0}};
  s21_decimal result;
  int error = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(test_s21_add_negative_overflow) {
  s21_decimal value_1 = {
      .bits = {0x80000000, 0x00000000, 0x00000000, 0x80000000}};
  s21_decimal value_2 = {
      .bits = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal result;
  int error = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(error, 2);
}
END_TEST

START_TEST(test_s21_add_large_numbers) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0}};
  s21_decimal value_2 = {{0x00000001, 0x00000000, 0x00000000, 0}};
  s21_decimal result;
  ck_assert_int_eq(s21_add(value_1, value_2, &result), 0);
  ck_assert_int_eq(result.bits[0], 0x00000000);
  ck_assert_int_eq(result.bits[1], 0x00000000);
  ck_assert_int_eq(result.bits[2], 0x00000001);
}
END_TEST

START_TEST(test_s21_add_decimal_with_scale) {
  s21_decimal value_1 = {{500000, 0, 0, 0x00050000}};
  s21_decimal value_2 = {{500000, 0, 0, 0x00050000}};
  s21_decimal result;
  ck_assert_int_eq(s21_add(value_1, value_2, &result), 0);
  ck_assert_int_eq(result.bits[0], 1000000);
  ck_assert_int_eq(result.bits[3], 0x00050000);
}
END_TEST

START_TEST(test_s21_add_with_zero_high_bits) {
  s21_decimal value_1 = {{12345, 0, 0, 0}};
  s21_decimal value_2 = {{0, 0, 0, 0}};
  s21_decimal result;
  ck_assert_int_eq(s21_add(value_1, value_2, &result), 0);
  ck_assert_int_eq(result.bits[0], 12345);
}
END_TEST

START_TEST(test_s21_add_different_signs) {
    s21_decimal value_1 = {{12345, 0, 0, 0}};
    s21_decimal value_2 = {{12345, 0, 0, 0x80000000}};
    s21_decimal result;
    ck_assert_int_eq(s21_add(value_1, value_2, &result), 0);
    ck_assert_int_eq(result.bits[0], 0);
}
END_TEST

START_TEST(test_s21_add_opposite_values) {
    s21_decimal value_1 = {{12345678, 0, 0, 0}};
    s21_decimal value_2 = {{12345678, 0, 0, 0x80000000}};
    s21_decimal result;
    ck_assert_int_eq(s21_add(value_1, value_2, &result), 0);
    ck_assert_int_eq(result.bits[0], 0);
}
END_TEST

START_TEST(test_s21_add_with_scale_normalization) {
    s21_decimal value_1 = {{500000, 0, 0, 0x00030000}};
    s21_decimal value_2 = {{2500, 0, 0, 0x00020000}};
    s21_decimal result;
    ck_assert_int_eq(s21_add(value_1, value_2, &result), 0);
    ck_assert_int_eq(result.bits[0], 525000);
    ck_assert_int_eq(result.bits[3], 0x00030000);
}
END_TEST

START_TEST(test_s21_add_large_numbers_overflow) {
    s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
    s21_decimal value_2 = {{1, 0, 0, 0}};
    s21_decimal result;
    ck_assert_int_eq(s21_add(value_1, value_2, &result), 1);
}
END_TEST

START_TEST(test_s21_add_with_zero_values) {
    s21_decimal a = {{0, 0, 0, 0}};
    s21_decimal b = {{12345, 0, 0, 0}};
    s21_decimal result;

    ck_assert_int_eq(s21_add(a, b, &result), 0);
    ck_assert_int_eq(result.bits[0], 12345);

    a.bits[0] = 0;
    b.bits[0] = 12345;
    b.bits[3] |= (1 << 31);
    ck_assert_int_eq(s21_add(a, b, &result), 0);
    ck_assert_int_eq(result.bits[0], 12345);
    ck_assert_int_eq(result.bits[3], (1 << 31));
}
END_TEST

START_TEST(test_s21_add_max_values_overflow) {
    s21_decimal a = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
    s21_decimal b = {{1, 0, 0, 0}};
    s21_decimal result;

    ck_assert_int_eq(s21_add(a, b, &result), 1);
}
END_TEST

START_TEST(test_s21_add_opposite_values_zero) {
    s21_decimal a = {{12345, 0, 0, 0}};
    s21_decimal b = {{12345, 0, 0, 0x80000000}};
    s21_decimal result;

    ck_assert_int_eq(s21_add(a, b, &result), 0);
    ck_assert_int_eq(result.bits[0], 0);
}
END_TEST

START_TEST(test_s21_add_with_scale) {
    s21_decimal a = {{1000, 0, 0, 0x00030000}};
    s21_decimal b = {{2000, 0, 0, 0x00030000}};
    s21_decimal result;

    ck_assert_int_eq(s21_add(a, b, &result), 0);
    ck_assert_int_eq(result.bits[0], 3000);
    ck_assert_int_eq(result.bits[3], 0x00030000);
}
END_TEST

Suite *add_suite(void) {
  Suite *suite = suite_create("s21_add");
  TCase *test_case = tcase_create("case_add");

  tcase_add_test(test_case, test_s21_add_with_scale);
  tcase_add_test(test_case, test_s21_add_opposite_values_zero);
  tcase_add_test(test_case, test_s21_add_max_values_overflow);
  tcase_add_test(test_case, test_s21_add_with_zero_values);
  tcase_add_test(test_case, test_s21_add_with_scale_normalization);
  tcase_add_test(test_case, test_s21_add_large_numbers_overflow);
  tcase_add_test(test_case, test_s21_add_different_signs);
  tcase_add_test(test_case, test_s21_add_opposite_values);
  tcase_add_test(test_case, test_s21_add_positive);
  tcase_add_test(test_case, test_s21_add_with_zero);
  tcase_add_test(test_case, test_s21_add_overflow);
  tcase_add_test(test_case, test_s21_add_positive_overflow);
  tcase_add_test(test_case, test_s21_add_negative_overflow);
  tcase_add_test(test_case, test_s21_add_large_numbers);
  tcase_add_test(test_case, test_s21_add_decimal_with_scale);
  tcase_add_test(test_case, test_s21_add_with_zero_high_bits);

  suite_add_tcase(suite, test_case);
  return suite;
}

int main() {
  Suite *suite = add_suite();
  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int failures = srunner_ntests_failed(runner);
  srunner_free(runner);
  printf("\033[41m-========= FAILED: %d ==========-\033[0m\n", failures);
  return failures == 0 ? 0 : 1;
}