#include "s21_decimal.h"


int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    *result = (s21_decimal){0};
    s21_big_decimal big_value1 = {0}, big_value2 = {0}, big_result = {0};
    from_decimal_to_big(value_1, &big_value1);
    from_decimal_to_big(value_2, &big_value2);

    
    if (big_value1.scale != big_value2.scale) {
        normalization(&big_value1, &big_value2);
    }
    if (big_value1.sign == big_value2.sign) {
        // Одинаковые знаки - выполняем сложение
        big_result = add_for_big(big_value1, big_value2);
    } else {
        // Разные знаки - выполняем вычитание
        int is_greater = big_is_greater_abs(big_value1, big_value2);
        if (big_is_equal_abs(big_value1, big_value2)) {
            big_result = (s21_big_decimal){0};  // Если числа равны по модулю, результат - 0
        } else {
            big_result = is_greater ? sub_for_big(big_value1, big_value2) : sub_for_big(big_value2, big_value1);
        }
    }
    
    return from_big_to_decimal(big_result, result);;
}

s21_big_decimal add_for_big(s21_big_decimal decimal1, s21_big_decimal decimal2) {
    s21_big_decimal result = {0};
    int carry = 0;
    int total_bits = 32 * 6;

    for (int index = 0; index < total_bits; index++) {
        int n = index / 32;
        int i = index % 32;

        int bit_value1 = get_bit_by_position(decimal1.bits[n], i);
        int bit_value2 = get_bit_by_position(decimal2.bits[n], i);

        if (add_bits(bit_value1, bit_value2, &carry)) {
            result.bits[n] = set_bit_by_position(result.bits[n], i);
        }
    }

    result.sign = decimal1.sign;
    result.scale = decimal1.scale;

    return result;
}

int add_bits(int bit_1, int bit_2, int *carry) {
    int bits_summ = bit_1 + bit_2 + *carry;
    
    *carry = (bits_summ >= 2) ? 1 : 0;
    
    return bits_summ % 2;
}