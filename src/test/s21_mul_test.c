#include <check.h>
#include <limits.h>

#include "../s21_decimal.h"


START_TEST(test_s21_mul_one) {
  s21_decimal a = {{0x7fffffff, 0, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal result;

  ck_assert_int_eq(s21_mul(a, b, &result), 0);
  ck_assert_int_eq(result.bits[0], 0x7fffffff);
}
END_TEST

START_TEST(test_s21_mul_normal) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{3, 0, 0, 0}};
  s21_decimal result;

  int status = s21_mul(a, b, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], 15);
}
END_TEST

START_TEST(test_s21_mul_rounding) {
  s21_decimal a = {{1000000, 0, 0, 0}};
  s21_decimal b = {{333, 0, 0, 0}};
  s21_decimal result;

  int status = s21_mul(a, b, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], 333000000);
}
END_TEST

START_TEST(test_s21_mul_negative) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{3, 0, 0, 0}};
  b.bits[3] |= (1 << 31);
  s21_decimal result;

  int status = s21_mul(a, b, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], 15);
  ck_assert_int_eq((result.bits[3] >> 31) & 1, 1);
}
END_TEST

START_TEST(test_s21_mul_by_zero) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  s21_decimal result;

  int status = s21_mul(a, b, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], 0);
}
END_TEST

START_TEST(test_s21_mul_large_numbers) {
  s21_decimal a = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  s21_decimal b = {{UINT_MAX, 0, 0, 0}};
  s21_decimal result;

  int status = s21_mul(a, b, &result);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_s21_mul_min_int) {
  s21_decimal a = {{(unsigned int)INT_MIN, 0, 0, 0}};
  s21_decimal b = {{2, 0, 0, 0}};
  s21_decimal result;

  int status = s21_mul(a, b, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], (unsigned int)(INT_MIN * 2U));
}
END_TEST

START_TEST(test_s21_mul_one_negative) {
    s21_decimal value_1 = {{2, 0, 0, 0}};
    s21_decimal value_2 = {{3, 0, 0, 0}};
    s21_decimal result;

    set_sign(&value_2, 1); 

   
    int error_code = s21_mul(value_1, value_2, &result);

    ck_assert_int_eq(error_code, 0);

    s21_decimal expected_result = {{6, 0, 0, 2147483648}}; 
    ck_assert_mem_eq(&result, &expected_result, sizeof(s21_decimal));
}
END_TEST

START_TEST(test_s21_mul_both_negative) {
    s21_decimal value_1 = {{2, 0, 0, 0}};
    s21_decimal value_2 = {{3, 0, 0, 0}};
    s21_decimal result;

    set_sign(&value_1, 1); 
    set_sign(&value_2, 1); 

    int error_code = s21_mul(value_1, value_2, &result);

    ck_assert_int_eq(error_code, 0);

    s21_decimal expected_result = {{6, 0, 0, 0}}; 
    ck_assert_mem_eq(&result, &expected_result, sizeof(s21_decimal));
}
END_TEST

Suite *mul_suite(void) {
  Suite *suite = suite_create("s21_mul");
  TCase *test_case = tcase_create("case_mul");

  tcase_add_test(test_case, test_s21_mul_one);
  tcase_add_test(test_case, test_s21_mul_normal);
  tcase_add_test(test_case, test_s21_mul_rounding);
  tcase_add_test(test_case, test_s21_mul_negative);
  tcase_add_test(test_case, test_s21_mul_by_zero);
  tcase_add_test(test_case, test_s21_mul_large_numbers);
  tcase_add_test(test_case, test_s21_mul_min_int);

  tcase_add_test(test_case, test_s21_mul_one_negative);
  tcase_add_test(test_case, test_s21_mul_both_negative);


  suite_add_tcase(suite, test_case);
  return suite;
}

int main() {
  Suite *suite = mul_suite();
  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int failures = srunner_ntests_failed(runner);
  srunner_free(runner);
  printf("\033[41m-========= FAILED: %d ==========-\033[0m\n", failures);
  return failures == 0 ? 0 : 1;
}
