#include <check.h>

#include "../s21_decimal.h"

START_TEST(truncate_0) {
  s21_decimal val = {{7, 7, 7, 0}};
  s21_decimal res = {0};
  ck_assert_int_eq(0, s21_truncate(val, &res));
  float fres = 0;
  s21_from_decimal_to_float(res, &fres);
  float need = 129127208515966861312.0;
  ck_assert_float_eq(need, fres);
}
END_TEST

START_TEST(truncate_1) {
  s21_decimal val = {{2, 0, 0, ~(UINT_MAX / 2)}};
  s21_decimal res = {0};
  ck_assert_int_eq(0, s21_truncate(val, &res));
  float fres = 0;
  s21_from_decimal_to_float(res, &fres);
  float need = -2.0;
  ck_assert_float_eq(need, fres);
}
END_TEST

START_TEST(truncate_2) {
  s21_decimal val = {{2, 0, 0, ~(UINT_MAX / 2)}};
  s21_decimal res = {0};
  ck_assert_int_eq(0, s21_truncate(val, &res));
  float fres = 0;
  s21_from_decimal_to_float(res, &fres);
  float need = -2.0;
  ck_assert_float_eq(need, fres);
}
END_TEST


START_TEST(test_s21_truncate_null_result) {
    s21_decimal value = {{1, 0, 0, 0}}; 
    s21_decimal *result = NULL; 

    
    int error_code = s21_truncate(value, result);

    ck_assert_int_eq(error_code, 1);
}
END_TEST

START_TEST(test_s21_truncate_scale_greater_than_28) {
    s21_decimal value = {{1, 0, 0, 0}}; 
    s21_decimal result;


    value.bits[3] = (29 << 16); 

    int error_code = s21_truncate(value, &result);

    
    ck_assert_int_eq(error_code, 1);
}
END_TEST


Suite *truncate_suite(void) {
  Suite *suite = suite_create("s21_truncate");
  TCase *test_case = tcase_create("case_truncate");

  tcase_add_test(test_case, truncate_0);
  tcase_add_test(test_case, truncate_1);
  tcase_add_test(test_case, truncate_2);
  tcase_add_test(test_case, test_s21_truncate_null_result);
  tcase_add_test(test_case, test_s21_truncate_scale_greater_than_28);


  suite_add_tcase(suite, test_case);
  return suite;
}


int main() {
  Suite *suite = truncate_suite();
  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int failures = srunner_ntests_failed(runner);
  srunner_free(runner);
  printf("\033[41m-========= FAILED: %d ==========-\033[0m\n", failures);
  return failures == 0 ? 0 : 1;
}