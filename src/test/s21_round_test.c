#include <check.h>

#include "../s21_decimal.h"

START_TEST(test_s21_round) {
  s21_decimal value = {
      {12345, 0, 0, 2 << 16}};
  s21_decimal result;

  ck_assert_int_eq(s21_round(value, &result), 0);
  ck_assert_int_eq(result.bits[0],
                   123);

  value =
      (s21_decimal){{6789, 0, 0, 2 << 16}};
  ck_assert_int_eq(s21_round(value, &result), 0);
  ck_assert_int_eq(result.bits[0],
                   68);
  value =
      (s21_decimal){{-6789, 0, 0, 2 << 16}};
  ck_assert_int_eq(s21_round(value, &result), 0);
  ck_assert_int_eq(
      result.bits[0],
      42949605);

  value = (s21_decimal){{0, 0, 0, 0}};
  ck_assert_int_eq(s21_round(value, &result), 0);
  ck_assert_int_eq(result.bits[0], 0);
}
END_TEST


START_TEST(test_s21_round_null_result) {
    s21_decimal value = {{1, 0, 0, 0}};
    s21_decimal *result = NULL;

    int error_code = s21_round(value, result);

    ck_assert_int_eq(error_code, 1);
}
END_TEST


START_TEST(test_s21_round_scale_greater_than_28) {
    s21_decimal value = {{1, 0, 0, 0}};
    s21_decimal result;

    value.bits[3] = (29 << 16);

    int error_code = s21_round(value, &result);

    ck_assert_int_eq(error_code, 1);
}
END_TEST


START_TEST(test_s21_round_round_half_even) {
    s21_decimal value = {{0, 0, 0, 0}};
    s21_decimal result;

    value.bits[0] = 5;
    set_scale(&value, 1);

    int error_code = s21_round(value, &result);

    ck_assert_int_eq(error_code, 0);

    s21_decimal expected_result = {{0, 0, 0, 0}};
    ck_assert_mem_eq(&result, &expected_result, sizeof(s21_decimal));
}
END_TEST
Suite *round_suite(void) {
  Suite *suite = suite_create("s21_round");
  TCase *test_case = tcase_create("case_round");

  tcase_add_test(test_case, test_s21_round);

  tcase_add_test(test_case, test_s21_round_null_result);
  tcase_add_test(test_case, test_s21_round_scale_greater_than_28);
  tcase_add_test(test_case, test_s21_round_round_half_even);

  suite_add_tcase(suite, test_case);

  return suite;
}

int main() {
  Suite *suite = round_suite();
  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int failures = srunner_ntests_failed(runner);
  srunner_free(runner);
  printf("\033[41m-========= FAILED: %d ==========-\033[0m\n", failures);
  return failures == 0 ? 0 : 1;
}