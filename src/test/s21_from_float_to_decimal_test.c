#include <check.h>

#include "../s21_decimal.h"

START_TEST(test_denormalized_float_conversion) {
    s21_decimal result;
    float input = 1.40129846e-45f; 
    int status = s21_from_float_to_decimal(input, &result);
    ck_assert_int_eq(status, 0); 
 
    ck_assert_int_eq(result.bits[0], 0); 
    ck_assert_int_eq(result.bits[1], 0); 
    ck_assert_int_eq(result.bits[2], 0); 
    ck_assert_int_eq(result.bits[3], 0); 
}
END_TEST


START_TEST(test_zero_input_conversion) {
    s21_decimal result;
    float input = 0.0f; 
    int status = s21_from_float_to_decimal(input, &result);
    ck_assert_int_eq(status, 0); 
    ck_assert_int_eq(result.bits[0], 0); 
    ck_assert_int_eq(result.bits[1], 0); 
    ck_assert_int_eq(result.bits[2], 0); 
    ck_assert_int_eq(result.bits[3], 0); 
}
END_TEST


START_TEST(test_exponent_greater_than_95) {
    s21_decimal result;
    float input = 1e30; 

    int status = s21_from_float_to_decimal(input, &result);
    ck_assert_int_eq(status, 1);  
}
END_TEST

START_TEST(test_scale_for_small_float) {
    s21_decimal result;
    float input = 1e-16; 

    int status = s21_from_float_to_decimal(input, &result);
    ck_assert_int_eq(status, 0); 
    
   
    printf("Small float conversion: bits[0] = %d, bits[3] = %d\n", result.bits[0], result.bits[3]);
    
    
    ck_assert_int_eq(result.bits[0], 1); 
    ck_assert_int_eq(result.bits[1], 0); 
    ck_assert_int_eq(result.bits[2], 0);
    ck_assert_int_eq(result.bits[3], 16 << 16);
}
END_TEST


START_TEST(test_s21_from_float_to_decimal_1) {
  s21_decimal result;
  float input, output;

  input = -6789.0123f;
  ck_assert_int_eq(s21_from_float_to_decimal(input, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(result, &output), 0);
  ck_assert_float_eq(output, input);

  input = 0.9876543f;
  ck_assert_int_eq(s21_from_float_to_decimal(input, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(result, &output), 0);
  ck_assert_float_eq(output, input);

  input = 1.2345e+10f;
  ck_assert_int_eq(s21_from_float_to_decimal(input, &result), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(result, &output), 0);
  ck_assert_float_eq(output, input);

  input = NAN;
  ck_assert_int_eq(s21_from_float_to_decimal(input, &result), 1);

  input = INFINITY;
  ck_assert_int_eq(s21_from_float_to_decimal(input, &result), 1);

  input = -INFINITY;
  ck_assert_int_eq(s21_from_float_to_decimal(input, &result), 1);
}
END_TEST


START_TEST(test_s21_from_float_to_decimal) {
  float input = -12345.6789f;
  s21_decimal result;

  ck_assert_int_eq(s21_from_float_to_decimal(input, &result), 0);  
  float output;
  ck_assert_int_eq(s21_from_decimal_to_float(result, &output),
                   0);                
  ck_assert_float_eq(output, input);  

  input = 1.0f;
  ck_assert_int_eq(s21_from_float_to_decimal(input, &result), 0); 
  ck_assert_int_eq(s21_from_decimal_to_float(result, &output),
                   0);                
  ck_assert_float_eq(output, input);  

  input = 12345.6789f;
  ck_assert_int_eq(s21_from_float_to_decimal(input, &result), 0); 
  ck_assert_int_eq(s21_from_decimal_to_float(result, &output),
                   0);          
  ck_assert_float_eq(output, input);  

  input = 1e+25f;
  ck_assert_int_eq(s21_from_float_to_decimal(input, &result), 0);  
  ck_assert_int_eq(s21_from_decimal_to_float(result, &output),
                   0);  
  ck_assert_float_eq(output, input);
 
END_TEST
}

START_TEST(test_s21_from_float_to_decimal_positive) {
  float src = 123.456;
  s21_decimal dst;
  int error = s21_from_float_to_decimal(src, &dst);
  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(dst.bits[0], 123456);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq(dst.bits[3], 0x00030000);
}
END_TEST

START_TEST(test_s21_from_float_to_decimal_too_large) {
  float src = 1e30;
  s21_decimal dst;
  int error = s21_from_float_to_decimal(src, &dst);
  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(test_s21_from_float_to_decimal_rounding) {
  float value = 12345.67890123f;
  s21_decimal result;
  ck_assert_int_eq(s21_from_float_to_decimal(value, &result), 0);
  ck_assert_int_eq(result.bits[0], 12345679);
}
END_TEST

START_TEST(test_s21_from_float_to_decimal_negative_with_fraction) {
  float value = -0.00001234f;
  s21_decimal result;
  ck_assert_int_eq(s21_from_float_to_decimal(value, &result), 0);
  ck_assert_int_eq(result.bits[0], 1234);
  ck_assert_int_eq(result.bits[3] & 0x80000000, 0x80000000);
}
END_TEST

START_TEST(test_s21_from_float_to_decimal_exact_decimal) {
  float value = 0.125f;
  s21_decimal result;
  ck_assert_int_eq(s21_from_float_to_decimal(value, &result), 0);
  ck_assert_int_eq(result.bits[0], 125);
  ck_assert_int_eq(result.bits[3], 0x00030000); // -0
}
END_TEST

START_TEST(test_s21_from_float_to_decimal_very_small_fraction) {
  float value = 0.0000001f;
  s21_decimal result;
  ck_assert_int_eq(s21_from_float_to_decimal(value, &result), 0);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(result.bits[3], 0x00070000);
}
END_TEST



Suite *from_float_to_decimal_suite(void) {
  Suite *suite = suite_create("s21_from_float_to_decimal");
  TCase *test_case = tcase_create("case_from_float_to_decimal");


  tcase_add_test(test_case, test_exponent_greater_than_95);
    tcase_add_test(test_case, test_scale_for_small_float);

  tcase_add_test(test_case, test_s21_from_float_to_decimal_1);
  tcase_add_test(test_case, test_s21_from_float_to_decimal);
  tcase_add_test(test_case, test_s21_from_float_to_decimal_positive);
  tcase_add_test(test_case, test_s21_from_float_to_decimal_too_large);
  tcase_add_test(test_case,
                 test_s21_from_float_to_decimal_negative_with_fraction);
  tcase_add_test(test_case, test_s21_from_float_to_decimal_exact_decimal);
  tcase_add_test(test_case, test_s21_from_float_to_decimal_very_small_fraction);
  tcase_add_test(test_case, test_s21_from_float_to_decimal_very_small_fraction);
  tcase_add_test(test_case, test_s21_from_float_to_decimal_rounding);

  suite_add_tcase(suite, test_case);
  return suite;
}

int main() {
  Suite *suite = from_float_to_decimal_suite();
  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int failures = srunner_ntests_failed(runner);
  srunner_free(runner);
  printf("\033[41m-========= FAILED: %d ==========-\033[0m\n", failures);
  return failures == 0 ? 0 : 1;
}