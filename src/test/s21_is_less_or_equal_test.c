#include <check.h>

#include "../s21_decimal.h"

START_TEST(test_s21_is_less_or_equal) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{10, 0, 0, 0}};

  ck_assert_int_eq(s21_is_less_or_equal(a, b), 1);

  s21_decimal zero = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_is_less_or_equal(zero, zero), 1);
  ck_assert_int_eq(s21_is_less_or_equal(a, zero), 0);

  b.bits[3] |= (1 << 31);
  ck_assert_int_eq(s21_is_less_or_equal(a, b),
                   0);

  a.bits[3] |= (1 << 31);
  ck_assert_int_eq(s21_is_less_or_equal(a, b),
                   0);

  a.bits[0] = b.bits[0];
  ck_assert_int_eq(s21_is_less_or_equal(a, b), 1);

  s21_decimal large_value = {{INT_MAX, 0, 0, 0}};
  s21_decimal small_value = {{1, 0, 0, 0}};

  ck_assert_int_eq(s21_is_less_or_equal(small_value, large_value),
                   1);
  ck_assert_int_eq(s21_is_less_or_equal(large_value, small_value),
                   0);

  s21_decimal min_value = {{INT_MIN, 0, 0, (1 << 31)}};
  ck_assert_int_eq(s21_is_less_or_equal(min_value, large_value),
                   1);
  ck_assert_int_eq(s21_is_less_or_equal(large_value, min_value),
                   0);
}
END_TEST

Suite *is_less_or_equal_suite(void) {
  Suite *suite = suite_create("s21_is_less_or_equal");
  TCase *test_case = tcase_create("case_is_less_or_equal");

  tcase_add_test(test_case, test_s21_is_less_or_equal);

  suite_add_tcase(suite, test_case);
  return suite;
}

int main() {
  Suite *suite = is_less_or_equal_suite();
  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int failures = srunner_ntests_failed(runner);
  srunner_free(runner);
  printf("\033[41m-========= FAILED: %d ==========-\033[0m\n", failures);
  return failures == 0 ? 0 : 1;
}