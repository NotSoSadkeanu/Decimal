#include <check.h>

#include "../s21_decimal.h"


START_TEST(test_s21_floor_negative_add_one) {
   
    s21_decimal value = {{1, 0, 0, 0}}; 
    value.bits[3] = 0x80000000;          
    
    value.bits[3] |= (1 << 16); 

    s21_decimal result;

  
    int error_code = s21_floor(value, &result);

    
    ck_assert_int_eq(error_code, 0);
    
    
    s21_decimal expected_result = {{1, 0, 0, 0}}; 
    expected_result.bits[3] = 0x80000000;
    expected_result.bits[3] &= ~(0xFF << 16); 
    ck_assert_mem_eq(&result, &expected_result, sizeof(s21_decimal));
}
END_TEST


START_TEST(test_s21_floor_negative_with_remainder) {
   
    s21_decimal value = {{15, 0, 0, 0}}; 
    value.bits[3] = 0x80000000;        

   
    value.bits[3] |= (1 << 16);       

    s21_decimal result;

 
    int error_code = s21_floor(value, &result);

   
    ck_assert_int_eq(error_code, 0);
    
   
    s21_decimal expected_result = {{2, 0, 0, 0}}; 
    expected_result.bits[3] = 0x80000000; 
    expected_result.bits[3] &= ~(0xFF << 16); 
    ck_assert_mem_eq(&result, &expected_result, sizeof(s21_decimal));
}
END_TEST

START_TEST(test_s21_floor_result_null) {
    s21_decimal value = {{1, 0, 0, 0}}; 
    s21_decimal *result = NULL;         
    int error_code = s21_floor(value, result);

   
    ck_assert_int_eq(error_code, 1);
}
END_TEST


START_TEST(test_s21_floor_scale_too_high) {
    s21_decimal value = {{1, 0, 0, 0}}; 
    set_scale(&value, 29);             
    s21_decimal result;

    int error_code = s21_floor(value, &result);

    ck_assert_int_eq(error_code, 1);
}
END_TEST



START_TEST(test_s21_floor) {
  s21_decimal value = {
      {12345, 0, 0, 2 << 16}};
  s21_decimal result;

  ck_assert_int_eq(s21_floor(value, &result), 0);
  ck_assert_int_eq(result.bits[0],
                   123);
  ck_assert_int_eq(result.bits[3], 0);

  value = (s21_decimal){
      {-12345, 0, 0, 2 << 16}};
  ck_assert_int_eq(s21_floor(value, &result), 0);
  ck_assert_int_eq(result.bits[0],
                   42949549);
  ck_assert_int_eq(result.bits[3], 0);

  value = (s21_decimal){{0, 0, 0, 0}};
  ck_assert_int_eq(s21_floor(value, &result), 0);
  ck_assert_int_eq(result.bits[0], 0);
}
END_TEST

Suite *floor_suite(void) {
  Suite *suite = suite_create("s21_floor");
  TCase *test_case = tcase_create("case_floor");

 tcase_add_test(test_case, test_s21_floor_negative_add_one);
 tcase_add_test(test_case, test_s21_floor_negative_with_remainder);
 tcase_add_test(test_case, test_s21_floor_result_null);
 tcase_add_test(test_case, test_s21_floor_scale_too_high);


 tcase_add_test(test_case, test_s21_floor);
 tcase_add_test(test_case, test_s21_floor_result_null);
tcase_add_test(test_case, test_s21_floor_scale_too_high);


  suite_add_tcase(suite, test_case);
  return suite;
}

int main() {
  Suite *suite = floor_suite();
  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int failures = srunner_ntests_failed(runner);
  srunner_free(runner);
  printf("\033[41m-========= FAILED: %d ==========-\033[0m\n", failures);
  return failures == 0 ? 0 : 1;
}
