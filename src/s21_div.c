#include "s21_decimal.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    if (value_2.bits[0] == 0 && value_2.bits[1] == 0 && value_2.bits[2] == 0) {
        return 3;  // Деление на ноль
    }
    
    *result = (s21_decimal){0};
    
    s21_big_decimal big_decimal1 = {0}, big_decimal2 = {0}, big_result = {0};
    
    from_decimal_to_big(value_1, &big_decimal1);
    from_decimal_to_big(value_2, &big_decimal2);
    
    normalization(&big_decimal1, &big_decimal2);
    
    int sign_decimal1 = big_decimal1.sign;
    int sign_decimal2 = big_decimal2.sign;
    
    big_result = div_without_remainder(big_decimal1, big_decimal2);
    
    // Устанавливаем знак результата
    if (sign_decimal1 || sign_decimal2) {
        big_result.sign = 1;
    }
    if (sign_decimal1 && sign_decimal2) {
        big_result.sign = 0;
    }
    
    return from_big_to_decimal(big_result, result);
}

s21_big_decimal div_with_remainder(s21_big_decimal decimal1, 
                                   s21_big_decimal decimal2, 
                                   s21_big_decimal *remainder) {
    decimal1.sign = 0;
    decimal2.sign = 0;
    
    // Инициализация переменных
    s21_big_decimal result = {0};
    s21_big_decimal dividend = decimal1;
    s21_big_decimal rem = {0};
    int total_bits = 6 * 32; // Количество бит для обработки (192 бита)
    int bit = 0;
    
    // Основной цикл обработки каждого бита делимого
    for (int i = 0; i < total_bits; i++) {
        // Получаем старший бит делимого перед сдвигом
        bit = get_bit_by_position(dividend.bits[5], 31);
        
        // Сдвигаем делимое и остаток
        big_shift(&dividend, 1);
        big_shift(&rem, 1);
        big_shift(&result, 1);
        
        // Переносим старший бит в остаток
        if (bit) {
            rem.bits[0] = set_bit_by_position(rem.bits[0], 0);
        }

        // Проверяем, достаточно ли остатка для вычитания делителя
        if (big_is_greater_abs(rem, decimal2) || big_is_equal_abs(rem, decimal2)) {
            // Если остаток больше или равен делителю, устанавливаем бит в результат
            result.bits[0] = set_bit_by_position(result.bits[0], 0);
            // Вычитаем делитель из остатка
            rem = sub_for_big(rem, decimal2);
        }    
    }
    
    // Возвращаем остаток через указатель
    *remainder = rem;
    
    // Возвращаем результат (частное)
    return result;
}

s21_big_decimal div_without_remainder(s21_big_decimal decimal1, 
                                      s21_big_decimal decimal2) {
    s21_big_decimal result = {0};
    s21_big_decimal fractional_part = {0};
    s21_big_decimal rem = {0};
    s21_big_decimal ten = {0};
    ten.bits[0] = 10;
    int scale = 0;
    
    // Вычисляем целую часть и остаток
    result = div_with_remainder(decimal1, decimal2, &rem);
    
    // Цикл для вычисления дробной части
    while (!big_is_equal_abs(rem, (s21_big_decimal){0}) && scale < 28) {
        scale++;
        
        // Увеличиваем остаток для получения следующей цифры дробной части
        rem = mul_for_big(rem, ten);
        
        // Получаем дробную часть
        s21_big_decimal temp = div_with_remainder(rem, decimal2, &rem);
        temp.scale = scale;
        
        // Добавляем дробную часть к результату
        fractional_part = add_for_big(fractional_part, temp);
    }
    
    // Суммируем целую и дробную часть
    normalization(&result, &fractional_part);
    result = add_for_big(result, fractional_part);

    // Округление результата на основе остатка
    rem = mul_for_big(rem, ten);
    div_with_remainder(rem, decimal2, &rem); // Используем остаток для округления

    // Если остаток больше 5, округляем вверх
    if (rem.bits[0] > 5 || (rem.bits[0] == 5 && get_bit_by_position(result.bits[0], 0))) {
        result.bits[0] += 1; // Увеличиваем результат на 1
    }

    return result;
}