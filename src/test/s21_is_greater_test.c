#include <check.h>

#include "../s21_decimal.h"

START_TEST(test_s21_is_greater) {
  s21_decimal a = {{10, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};

  ck_assert_int_eq(s21_is_greater(a, b), 1);

  a.bits[0] = b.bits[0];
  ck_assert_int_eq(s21_is_greater(a, b), 0);

  s21_decimal zero = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_is_greater(a, zero), 1);
  ck_assert_int_eq(s21_is_greater(zero, a), 0);

  s21_decimal large_value = {{INT_MAX, 0, 0, 0}};
  s21_decimal small_value = {{1, 0, 0, 0}};

  ck_assert_int_eq(s21_is_greater(large_value, small_value), 1);
  ck_assert_int_eq(s21_is_greater(small_value, large_value),
                   0);

  s21_decimal min_value = {{INT_MIN, 0, 0, (1 << 31)}};
  ck_assert_int_eq(s21_is_greater(large_value, min_value),
                   1);
  ck_assert_int_eq(s21_is_greater(min_value, large_value),
                   0);
}
END_TEST

Suite *is_greater_suite(void) {
  Suite *suite = suite_create("s21_is_greater");
  TCase *test_case = tcase_create("case_is_greater");

  tcase_add_test(test_case, test_s21_is_greater);

  suite_add_tcase(suite, test_case);
  return suite;
}

int main() {
  Suite *suite = is_greater_suite();
  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int failures = srunner_ntests_failed(runner);
  srunner_free(runner);
  printf("\033[41m-========= FAILED: %d ==========-\033[0m\n", failures);
  return failures == 0 ? 0 : 1;
}