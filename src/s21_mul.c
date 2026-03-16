#include "s21_decimal.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  *result = (s21_decimal){0};
  s21_big_decimal decimal1 = {0}, decimal2 = {0}, big_result = {0};
  from_decimal_to_big(value_1, &decimal1);
  from_decimal_to_big(value_2, &decimal2);
  int sign_value_1 = decimal1.sign, sign_value_2 = decimal2.sign;

  big_result = mul_for_big(decimal1, decimal2);
  if (sign_value_1 || sign_value_2) {
    big_result.sign = 1;
  }
  if (sign_value_1 && sign_value_2) {
    big_result.sign = 0;
  }
  return from_big_to_decimal(big_result, result);
}


s21_big_decimal mul_for_big(s21_big_decimal decimal1, s21_big_decimal decimal2) {
    s21_big_decimal result = {0};

    // Обработка знака
    if (decimal1.sign || decimal2.sign) {
        result.sign = 1;
        decimal1.sign = 0;
        decimal2.sign = 0;
    }

    s21_big_decimal memo = {0}; // Промежуточная переменная
    int shift = 0;              // Переменная для сдвига
    int bit_value1 = 0, bit_value2 = 0;
    int total_bits = 6 * 32;     // Всего бит в одном числе (6 * 32 = 192)

    // Внешний цикл по битам первого числа
    for (int index1 = 0; index1 < total_bits; index1++) {
        int n1 = index1 / 32;
        int i1 = index1 % 32;

        bit_value1 = get_bit_by_position(decimal1.bits[n1], i1); // Проверяем бит первого числа

        // Внутренний цикл по битам второго числа
        for (int index2 = 0; index2 < total_bits; index2++) {
            int n2 = index2 / 32;
            int i2 = index2 % 32;

            bit_value2 = get_bit_by_position(decimal2.bits[n2], i2); // Проверяем бит второго числа

            // Если оба бита равны 1, устанавливаем бит во временной переменной
            if (bit_value1 * bit_value2 == 1) {
                memo.bits[n2] = set_bit_by_position(memo.bits[n2], i2);
            }
        }

        // Сдвигаем временное число и добавляем его к результату
        big_shift(&memo, shift);
        result = add_for_big(result, memo);

        // Сброс временной переменной для следующего шага
        memo = (s21_big_decimal){0};

        // Увеличиваем сдвиг
        shift++;
    }

    // Устанавливаем масштаб результата
    result.scale = decimal1.scale + decimal2.scale;

    return result;
}

void mul_for_scale(s21_big_decimal *decimal, s21_big_decimal multr, int scale) {
    for (int i = 0; i < (int)(scale - decimal->scale); i++) {
        *decimal = mul_for_big(*decimal, multr);
    }
    decimal->scale = scale;
}

