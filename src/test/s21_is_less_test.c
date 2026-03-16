#include <check.h>

#include "../s21_decimal.h"

START_TEST(test_s21_is_less_num1_positive_num2_negative) {
    s21_decimal num1 = {{0, 0, 0, 0}};  
    s21_decimal num2 = {{0, 0, 0, 1 << 31}};  

    int result = s21_is_less(num1, num2);  

    ck_assert_int_eq(result, 0);  
}
END_TEST


START_TEST(test_s21_big_is_less_num1_greater_negative) {
   
    s21_big_decimal big_val_1 = {{1, 0, 0, 0}};  
    s21_big_decimal big_val_2 = {{0, 0, 0, 0}}; 

    int sign = 1; 

    int result = s21_big_is_less(&big_val_1, &big_val_2, sign); 

    ck_assert_int_eq(result, 1); 
}
END_TEST



START_TEST(test_less_num_bits1_not_equal) {
    s21_decimal num1 = {{0, 2, 1, 0}};  
    s21_decimal num2 = {{0, 3, 1, 0}}; 

    int res = -1;
    res = less_num(num1, num2, res);  
    ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_comp_exp_num1_greater_exp) {
    s21_decimal num1 = {{1, 0, 0, 0}};  
    s21_decimal num2 = {{1, 0, 0, 0}}; 
    set_scale(&num1, 5); 
    set_scale(&num2, 3); 

    int res = comp_exp(num1, num2);  

    ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_comp_exp_num2_greater_exp) {
    s21_decimal num1 = {{1, 0, 0, 0}};
    s21_decimal num2 = {{1, 0, 0, 0}};
    set_scale(&num1, 3);  
    set_scale(&num2, 5);  
    int res = comp_exp(num1, num2);  
    ck_assert_int_eq(res, 0);  
}
END_TEST


START_TEST(test_upper_10_increases_scale) {
    s21_decimal num = {{1, 0, 0, 0}};  
    int scale = 3;  
    s21_decimal result = upper_10(scale, &num);
    ck_assert_int_eq(get_scale(result), scale);
}
END_TEST

START_TEST(test_upper_10_triggers_multi_ten) {
    s21_decimal num = {{1, 0, 0, 0}}; 
    int scale = 5;  
    upper_10(scale, &num);  
    ck_assert_int_eq(num.bits[0], 100000);  
}
END_TEST

START_TEST(test_s21_is_less_positive_less) {
  s21_decimal value1 = {
      .bits = {0x00000005, 0x00000000, 0x00000000, 0x00000000}}; // 5
  s21_decimal value2 = {
      .bits = {0x0000000A, 0x00000000, 0x00000000, 0x00000000}}; // 10
  int result = s21_is_less(value1, value2);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_s21_is_less_positive_equal) {
  s21_decimal value1 = {
      .bits = {0x0000000F, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal value2 = {
      .bits = {0x0000000F, 0x00000000, 0x00000000, 0x00000000}};
  int result = s21_is_less(value1, value2);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_s21_is_less_positive_greater) {
  s21_decimal value1 = {
      .bits = {0x00000014, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal value2 = {
      .bits = {0x0000000A, 0x00000000, 0x00000000, 0x00000000}};
  int result = s21_is_less(value1, value2);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_s21_is_less_negative_less) {
  s21_decimal value1 = {
      .bits = {0x00000005, 0x00000000, 0x00000000, 0x80000000}};
  s21_decimal value2 = {
      .bits = {0x0000000A, 0x00000000, 0x00000000, 0x80000000}};
  int result = s21_is_less(value1, value2);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_s21_is_less_negative_equal) {
  s21_decimal value1 = {
      .bits = {0x0000000F, 0x00000000, 0x00000000, 0x80000000}};
  s21_decimal value2 = {
      .bits = {0x0000000F, 0x00000000, 0x00000000, 0x80000000}};
  int result = s21_is_less(value1, value2);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_s21_is_less_negative_greater) {
  s21_decimal value1 = {
      .bits = {0x00000014, 0x00000000, 0x00000000, 0x80000000}};
  s21_decimal value2 = {
      .bits = {0x0000000A, 0x00000000, 0x00000000, 0x80000000}};
  int result = s21_is_less(value1, value2);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_s21_is_less_positive_negative) {
  s21_decimal value1 = {
      .bits = {0x0000000A, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal value2 = {
      .bits = {0x00000005, 0x00000000, 0x00000000, 0x80000000}};
  int result = s21_is_less(value1, value2);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_s21_is_less_negative_positive) {
  s21_decimal value1 = {
      .bits = {0x0000000A, 0x00000000, 0x00000000, 0x80000000}};
  s21_decimal value2 = {
      .bits = {0x00000014, 0x00000000, 0x00000000, 0x00000000}};
  int result = s21_is_less(value1, value2);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_s21_is_less_positive_decimal_part) {
  s21_decimal value1 = {
      .bits = {0x0000000A, 0x0000000B, 0x0000000C, 0x00000000}};
  s21_decimal value2 = {
      .bits = {0x0000000D, 0x0000000E, 0x0000000F, 0x00000000}};
  int result = s21_is_less(value1, value2);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_s21_is_less_negative_decimal_part) {
  s21_decimal value1 = {
      .bits = {0x0000000A, 0x0000000B, 0x0000000C, 0x80000000}};
  s21_decimal value2 = {
      .bits = {0x0000000D, 0x0000000E, 0x0000000F, 0x80000000}};
  int result = s21_is_less(value1, value2);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_s21_is_less_positive_decimal_part_fraction) {
  s21_decimal value1 = {.bits = {0x0000000A, 0x0000000B, 0x0000000C,
                                 0x00000001}};
  s21_decimal value2 = {
      .bits = {0x0000000D, 0x0000000E, 0x0000000F, 0x00000000}};
  int result = s21_is_less(value1, value2);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_s21_is_less_negative_decimal_part_fraction) {
  s21_decimal value1 = {
      .bits = {0x0000000A, 0x0000000B, 0x0000000C,
               0x80000001}};
  s21_decimal value2 = {
      .bits = {0x0000000D, 0x0000000E, 0x0000000F, 0x80000000}};
  int result = s21_is_less(value1, value2);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_s21_is_less) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{10, 0, 0, 0}};

  ck_assert_int_eq(s21_is_less(a, b), 1);  // a < b

  a.bits[0] = b.bits[0];
  ck_assert_int_eq(s21_is_less(a, b), 0);

  s21_decimal zero = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_is_less(zero, zero), 0);
  ck_assert_int_eq(s21_is_less(a, zero), 0);

  s21_decimal large_value = {{INT_MAX, 0, 0, 0}};
  s21_decimal small_value = {{1, 0, 0, 0}};

  ck_assert_int_eq(s21_is_less(small_value, large_value), 1);
  ck_assert_int_eq(s21_is_less(large_value, small_value), 0);

  s21_decimal min_value = {{INT_MIN, 0, 0, (1 << 31)}};
  ck_assert_int_eq(s21_is_less(min_value, large_value),
                   1);
  ck_assert_int_eq(s21_is_less(large_value, min_value),
                   0);

  a.bits[3] |= (1 << 31);
  b.bits[3] |= (1 << 31);
  ck_assert_int_eq(
      s21_is_less(a, b),
      (a.bits[0] < b.bits[0]) ? 1 : 0);
}
END_TEST

Suite *is_less_suite(void) {
  Suite *suite = suite_create("s21_is_less");
  TCase *test_case = tcase_create("case_is_less");



   tcase_add_test(test_case, test_s21_is_less_num1_positive_num2_negative);
    tcase_add_test(test_case, test_s21_big_is_less_num1_greater_negative);

    tcase_add_test(test_case, test_less_num_bits1_not_equal);
    tcase_add_test(test_case, test_comp_exp_num1_greater_exp);
    tcase_add_test(test_case, test_comp_exp_num2_greater_exp);
    tcase_add_test(test_case, test_upper_10_increases_scale);
    tcase_add_test(test_case, test_upper_10_triggers_multi_ten);

  tcase_add_test(test_case, test_s21_is_less);
  tcase_add_test(test_case, test_s21_is_less_positive_less);
  tcase_add_test(test_case, test_s21_is_less_positive_equal);
  tcase_add_test(test_case, test_s21_is_less_positive_greater);
  tcase_add_test(test_case, test_s21_is_less_negative_less);
  tcase_add_test(test_case, test_s21_is_less_negative_equal);
  tcase_add_test(test_case, test_s21_is_less_negative_greater);
  tcase_add_test(test_case, test_s21_is_less_positive_negative);
  tcase_add_test(test_case, test_s21_is_less_negative_positive);
  tcase_add_test(test_case, test_s21_is_less_positive_decimal_part);
  tcase_add_test(test_case, test_s21_is_less_negative_decimal_part);
  tcase_add_test(test_case, test_s21_is_less_positive_decimal_part_fraction);
  tcase_add_test(test_case, test_s21_is_less_negative_decimal_part_fraction);

  suite_add_tcase(suite, test_case);
  return suite;
}

int main() {
  Suite *suite = is_less_suite();
  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int failures = srunner_ntests_failed(runner);
  srunner_free(runner);
  printf("\033[41m-========= FAILED: %d ==========-\033[0m\n", failures);
  return failures == 0 ? 0 : 1;
}