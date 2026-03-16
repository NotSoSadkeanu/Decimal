#include "s21_decimal.h"


int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    *result = (s21_decimal){0};
    value_2.bits[3] = (value_2.bits[3] ^ (1U << 31));
    return s21_add(value_1, value_2, result);
}

s21_big_decimal sub_for_big(s21_big_decimal decimal1, s21_big_decimal decimal2) {
    s21_big_decimal result = {0};
    int borrow = 0;
    int bit1, bit2;

    for (int index = 0; index < 32 * 6; index++) {
        int n = index / 32;
        int i = index % 32;

        bit1 = get_bit_by_position(decimal1.bits[n], i);
        bit2 = get_bit_by_position(decimal2.bits[n], i);

        if (bits_sub(bit1, bit2, &borrow)) {
            result.bits[n] = set_bit_by_position(result.bits[n], i);
        }
    }
    result.scale = decimal1.scale;
    result.sign = decimal1.sign;

    return result;
}

int bits_sub(int bit1, int bit2, int *borrow) {
    int result = bit1 - bit2 - *borrow;

    if (result < 0) {
        result += 2;
        *borrow = 1;
    } else {
        *borrow = 0;
    }
    
    return result;
}
