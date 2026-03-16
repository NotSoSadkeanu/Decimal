#include <check.h>
#include <limits.h>

#include "../s21_decimal.h"



START_TEST(test_s21_div_with_rem_less_than_five) {
    s21_decimal value_1 = {{20, 0, 0, 0}}; 
    s21_decimal value_2 = {{4, 0, 0, 0}};  
    s21_decimal result;

    int error_code = s21_div(value_1, value_2, &result);

    ck_assert_int_eq(error_code, 0);

    s21_decimal expected_result = {{5, 0, 0, 0}}; 
    ck_assert_mem_eq(&result, &expected_result, sizeof(s21_decimal));
}
END_TEST


START_TEST(test_s21_div_rounding_up) {
    s21_decimal value_1 = {{10, 0, 0, 0}};  
    s21_decimal value_2 = {{3, 0, 0, 0}};   
    s21_decimal result;


    int status = s21_div(value_1, value_2, &result);

    ck_assert_int_eq(status, 0);

    s21_decimal expected_result = {{4, 0, 0, 0}};
    ck_assert_mem_eq(&result, &expected_result, sizeof(s21_decimal));
}
END_TEST


START_TEST(test_s21_div_division_by_zero) {
    s21_decimal value_1 = {{1, 0, 0, 0}};
    s21_decimal value_2 = {{0, 0, 0, 0}};
    s21_decimal result;

    int status = s21_div(value_1, value_2, &result);
    ck_assert_int_eq(status, 3);
}
END_TEST

START_TEST(test_s21_div_both_negative) {
    s21_decimal value_1 = {{1, 0, 0, 0}};  
    s21_decimal value_2 = {{2, 0, 0, 0}};  
    set_sign(&value_1, 1);  
    set_sign(&value_2, 1);  
    s21_decimal result;

    int status = s21_div(value_1, value_2, &result);
    ck_assert_int_eq(status, 0);
    ck_assert_int_eq(get_sign(result), 0); 
}
END_TEST

START_TEST(test_s21_div_reset_sign_when_both_negative) {
    s21_decimal value_1 = {{10, 0, 0, 0}}; 
    s21_decimal value_2 = {{5, 0, 0, 0}};  
    set_sign(&value_1, 1); 
    set_sign(&value_2, 1); 
    s21_decimal result;

    int status = s21_div(value_1, value_2, &result);

    ck_assert_int_eq(status, 0);
    ck_assert_int_eq(get_sign(result), 0); 
}
END_TEST


START_TEST(test_s21_div_underflow) {
    s21_decimal a = {{1, 0, 0, 0}};  
    s21_decimal b = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};  
    s21_decimal result;

    ck_assert_int_eq(s21_div(a, b, &result), 0);  
    ck_assert_int_eq(result.bits[0], 0);         
    ck_assert_int_eq(result.bits[1], 0);
    ck_assert_int_eq(result.bits[2], 0);
}
END_TEST


START_TEST(test_s21_div_edge_cases) {

    s21_decimal a = {{INT_MAX, 0, 0, 0}};   
    s21_decimal b = {{1, 0, 0, 0}};    
    a.bits[3] |= (1 << 31);                

    s21_decimal result;
    ck_assert_int_eq(s21_div(a, b, &result), 0); 
    ck_assert_int_eq(result.bits[0], INT_MAX);   
    ck_assert_int_eq(result.bits[3], a.bits[3]); 
}
END_TEST

START_TEST(test_s21_div) {
    s21_decimal a = {{10, 0, 0, 0}};  
    s21_decimal b = {{2, 0, 0, 0}};  
    s21_decimal result;

    ck_assert_int_eq(s21_div(a, b, &result), 0);  
    ck_assert_int_eq(result.bits[0], 5);         

    b.bits[0] = 0;  
    ck_assert_int_eq(s21_div(a, b, &result), 3);

    b.bits[0] = 2;                     
    b.bits[3] |= (1 << 31);          
    ck_assert_int_eq(s21_div(a, b, &result), 0);  
    ck_assert_int_eq(result.bits[0], 5);         
    ck_assert_int_eq(result.bits[3], b.bits[3]);  

    a.bits[0] = 0;  
    ck_assert_int_eq(s21_div(a, b, &result), 0);  
    ck_assert_int_eq(result.bits[0], 0);          

    s21_decimal c = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
    s21_decimal d = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
    ck_assert_int_eq(s21_div(c, d, &result), 0); 
    ck_assert_int_eq(result.bits[0], 1);          
    result = (s21_decimal){0};      

    d.bits[0] = 1;  
    d.bits[1] = 0;
    d.bits[2] = 0;
    d.bits[3] = 0;
    ck_assert_int_eq(s21_div(c, d, &result), 0); 
    ck_assert_int_eq(result.bits[0], (int)UINT_MAX);  
    ck_assert_int_eq(result.bits[1], (int)UINT_MAX);  
    ck_assert_int_eq(result.bits[2], (int)UINT_MAX); 

}
END_TEST


Suite *div_suite(void) {
  Suite *suite = suite_create("s21_div");
  TCase *test_case = tcase_create("case_div");


tcase_add_test(test_case, test_s21_div_with_rem_less_than_five);


tcase_add_test(test_case, test_s21_div_division_by_zero);
tcase_add_test(test_case, test_s21_div_both_negative);
tcase_add_test(test_case, test_s21_div_reset_sign_when_both_negative);

    tcase_add_test(test_case, test_s21_div);
    tcase_add_test(test_case, test_s21_div_underflow);
    tcase_add_test(test_case, test_s21_div_edge_cases);

   
 suite_add_tcase(suite, test_case);
  return suite;
}
int main() {
  Suite *suite = div_suite();
  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int failures = srunner_ntests_failed(runner);
  srunner_free(runner);
  printf("\033[41m-========= FAILED: %d ==========-\033[0m\n", failures);
  return failures == 0 ? 0 : 1;
}

