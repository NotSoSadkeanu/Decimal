#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <stdint.h>
#include <stdbool.h>

// Структуры данных

// Структура для хранения битового представления числа (s21_decimal)
typedef struct {
    int bits[4];  // bits[0]: младшие 32 бита, bits[1]: средние 32 бита, bits[2]: старшие 32 бита
                  // bits[3]: знак и коэффициент масштабирования
} s21_decimal;

// Структура для хранения большого битового представления (s21_big_decimal)
typedef struct {
    int bits[6];  // bits[0]: младшие 32 бита, ... , bits[5]: старшие 32 бита
    int sign;     // знак числа (0 - положительное, 1 - отрицательное)
    int scale;    // степень для масштабирования числа (экспонента)
} s21_big_decimal;

// ФУНКЦИИ НЕОБХОДИМЫЕ ПО ЗАДАНИЮ ИМЕЮТ НАЗВАНИЯ s21_, ОСТАЛЬНЫЕ БЕЗ ПРИСТАВКИ!

// Вспомогательные функции работы с битами

// Устанавливает определенный бит в s21_decimal
void set_bit(s21_decimal *decimal, int position, unsigned bit);

// Возвращает значение определенного бита в s21_decimal
int get_bit(s21_decimal decimal, int position);

// Возвращает бит по заданной позиции из int
int get_bit_by_position(const int value, const int position);

// Устанавливает бит по заданной позиции в int
int set_bit_by_position(const int value, const int position);

// Устанавливает определенный бит в s21_big_decimal
void set_big_bit(s21_big_decimal *decimal, int position, unsigned bit);

// Возвращает бит по заданной позиции в s21_big_decimal
int get_big_bit(s21_big_decimal decimal, int position);

// Устанавливает знак числа в s21_decimal (0 - положительное, 1 - отрицательное)
void set_sign(s21_decimal *decimal, int bit);

// Возвращает знак числа в s21_decimal
int get_sign(s21_decimal decimal);

// Устанавливает коэффициент масштабирования (экспоненту) в s21_decimal
void set_scale(s21_decimal *decimal, int scale);

// Возвращает коэффициент масштабирования (экспоненту) в s21_decimal
int get_scale(s21_decimal decimal);

// Функции нормализации и работы с масштабом

// Приводит мантиссы двух чисел s21_big_decimal к одинаковой степени
void normalization(s21_big_decimal *decimal1, s21_big_decimal *decimal2);

// Умножает мантиссу числа на множитель для нормализации
void mul_for_scale(s21_big_decimal *decimal, s21_big_decimal multr, int scale);

// Выполняет двоичный сдвиг влево на заданное количество бит в s21_big_decimal
void big_shift(s21_big_decimal *decimal, int shift);

// Проверяет, содержит ли s21_big_decimal все нулевые биты
int check_big(s21_big_decimal value);

// Сравнивает числа, 0 если равны, 1 если первое меньше?
int less_num(s21_decimal num1, s21_decimal num2, int res);

// Сравнение битов?
int comp_bit(int bit1, int bit2);

// Сравнение экспоненты?
int comp_exp(s21_decimal num1, s21_decimal num2);

//
s21_decimal upper_10(int pow, s21_decimal *val);

//
void bin_conv(s21_decimal *val, int res);

//
void s21_left_shift(s21_decimal *num, int shift);

//
void on_bit_set_1(int *val, int BitNumber);

//
void on_bit_set_0(int *val, int BitNumber);

//
int s21_logic_add_bit(s21_decimal val_1, s21_decimal val_2,
                      s21_decimal *result);


// Функции конвертации

// Конвертирует целое число в s21_decimal
int s21_from_int_to_decimal(int num, s21_decimal *dec);

// Конвертирует s21_decimal в s21_big_decimal
void from_decimal_to_big(s21_decimal decimal, s21_big_decimal *big_decimal);

// Конвертирует s21_big_decimal обратно в s21_decimal
int from_big_to_decimal(s21_big_decimal big_decimal, s21_decimal *decimal);

// Конвертирует s21_decimal в целое число
int s21_from_decimal_to_int(s21_decimal src, int *dst);
//
int s21_from_float_to_decimal(float src, s21_decimal *dst);
//
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// Конвертируем s21_decimal в float
int s21_from_decimal_to_float(s21_decimal src, float *dst);


// Функции сравнения

//
int s21_is_less(s21_decimal num1, s21_decimal num2);

//
int s21_is_less_or_equal(s21_decimal num1, s21_decimal num2);

//
int s21_is_greater(s21_decimal num1, s21_decimal num2);

//
int s21_is_greater_or_equal(s21_decimal num1, s21_decimal num2);

//
int s21_is_equal(s21_decimal num1, s21_decimal num2);

//
int s21_is_not_equal(s21_decimal num1, s21_decimal num2);

// Проверяет равенство двух чисел типа s21_big_decimal по модулю
int big_is_equal_abs(s21_big_decimal decimal1, s21_big_decimal decimal2);

// Проверяет, является ли первое число больше второго по модулю
int big_is_greater_abs(s21_big_decimal decimal1, s21_big_decimal decimal2);


// Арифметические функции

// Складывает два числа типа s21_decimal
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// Складывает два числа типа s21_big_decimal
s21_big_decimal add_for_big(s21_big_decimal decimal1, s21_big_decimal decimal2);

// Вычитает два числа типа s21_decimal
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// Вычитает два бита с учетом заема
int bits_sub(int bit1, int bit2, int *borrow);

// Умножает два числа типа s21_big_decimal
s21_big_decimal mul_for_big(s21_big_decimal decimal1, s21_big_decimal decimal2);

// Вычитает одно число типа s21_big_decimal из другого
s21_big_decimal sub_for_big(s21_big_decimal decimal1, s21_big_decimal decimal2);

// Делит два числа типа s21_big_decimal, возвращая частное и остаток
s21_big_decimal div_with_remainder(s21_big_decimal decimal1, 
                                   s21_big_decimal decimal2, 
                                   s21_big_decimal *remainder);

// Делит два числа типа s21_big_decimal без учета остатка
s21_big_decimal div_without_remainder(s21_big_decimal decimal1, 
                                      s21_big_decimal decimal2);

// Сложение двух битов с учетом переноса
int add_bits(int bit_1, int bit_2, int *carry);

// Делит два числа типа s21_decimal
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// Умножает два числа типа s21_decimal
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

//оставляем

// // Округляет указанное Decimal число до ближайшего целого числа
int s21_floor(s21_decimal value, s21_decimal *result);
int is_last_bits_clear(const s21_decimal *value);
void set_dec_to_zero(s21_decimal *dst);
int s21_div_ten(s21_decimal value_1, s21_decimal *result);
void s21_multi_ten(s21_decimal *value);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_decimal_not_empty(s21_decimal *value);
int s21_big_is_greater_or_equal(s21_big_decimal *val_1, s21_big_decimal *val_2,
                                int sign);
int s21_big_is_less(s21_big_decimal *big_val_1, s21_big_decimal *big_val_2,
                    int sign);
void s21_get_remainder(const s21_decimal *res, s21_decimal *tmp,
                       s21_decimal *dec_remainder);
void s21_decimal_add_one(s21_decimal *src);
int s21_round(s21_decimal value, s21_decimal* result);
int s21_negate(s21_decimal value, s21_decimal *result);

//DELETEDS
// int get_sign(s21_decimal *num); - реализация Александра
// int ten_conv(s21_decimal val);
// bool test_bit(unsigned int val, int BitNumber);
// void mult_10(s21_decimal val, s21_decimal *res);
// void s21_sum(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
// void s21_left_shift(s21_decimal *decimal, int shift);
// int s21_decimal_not_empty(s21_decimal *value);
// void big_decimal_to_num(s21_big_decimal num_big_1, s21_decimal *num_1);
// void s21_big_sum(s21_big_decimal value_1, s21_big_decimal value_2, s21_big_decimal *result);
// void s21_big_set_bit(s21_big_decimal *decimal, int position, int bit);
// void s21_big_sub(s21_big_decimal value_1, s21_big_decimal value_2, s21_big_decimal *result);
// int s21_big_get_bit(s21_big_decimal decimal, int position);
// void s21_big_left_shift(s21_big_decimal *decimal, int shift);
// void num_to_big_decimal(s21_big_decimal *num_big_1, s21_decimal num_1);
// int s21_from_decimal_to_float(s21_decimal src, float *dst);
// int s21_get_bit(s21_decimal decimal, int position);
// int s21_floor(s21_decimal value, s21_decimal *result);
// void abs_add(s21_decimal decimal1, s21_decimal decimal2, s21_decimal *result);


#endif // S21_DECIMAL_H
