#include "s21_decimal.h"


int s21_is_less(s21_decimal num1, s21_decimal num2) {
  int res = -1;
  int sign1 = get_sign(num1);
  int sign2 = get_sign(num2);

  if (sign1 == sign2 && sign1 == 0) {
    res = less_num(num1, num2, res);
  } else if (sign1 == 1 && sign2 == 0) {
    res = 1;
  } else if (sign1 == 0 && sign2 == 1) {
    res = 0;
  } else if (sign1 == sign2 && sign1 == 1) {
    if (num1.bits[3] == num2.bits[3]) {
      if (num1.bits[2] == num2.bits[2]) {
        if (num1.bits[1] == num2.bits[1]) {
          if (num1.bits[0] == num2.bits[0]) {
            res = 0;
          } else
            res = !less_num(num1, num2, res);
        } else
          res = !less_num(num1, num2, res);
      } else
        res = !less_num(num1, num2, res);
    } else
      res = !less_num(num1, num2, res);
  }
  return res;
}

int s21_is_less_or_equal(s21_decimal num1, s21_decimal num2) {
  int res = 0;
  if (s21_is_less(num1, num2) || s21_is_equal(num1, num2)) {
    res = 1;
  }
  return res;
}

int s21_is_greater(s21_decimal num1, s21_decimal num2) {
  return !s21_is_less(num1, num2) && !s21_is_equal(num1, num2);
}

int s21_is_greater_or_equal(s21_decimal num1, s21_decimal num2) {
  int res = 0;
  if (s21_is_greater(num1, num2) || s21_is_equal(num1, num2)) {
    res = 1;
  }
  return res;
}

int s21_is_equal(s21_decimal num1, s21_decimal num2){

    int res = 0;
    int sign1 = get_sign(num1);
    int sign2 = get_sign(num2);

    if(sign1 != sign2){
        res = 0;
    }else{
        comp_exp(num1, num2);
        if((num1.bits[0] == num2.bits[0]) && (num1.bits[1] == num2.bits[1]) &&
           (num1.bits[2] == num2.bits[2]) && (num1.bits[3] == num2.bits[3]))
        res = 1;
    }
    return res;
}

int s21_is_not_equal(s21_decimal num1, s21_decimal num2) {
  return !s21_is_equal(num1, num2);
}


int big_is_equal_abs(s21_big_decimal decimal1, s21_big_decimal decimal2) {
    int result = 1;

    for (int i = 0; i < 6; i++) {
        if (decimal1.bits[i] != decimal2.bits[i]) {
            result = 0;
            break;
        }
    }

    return result;
}

int big_is_greater_abs(s21_big_decimal decimal1, s21_big_decimal decimal2) {
    int total_bits = 6 * 32;
    int result = 0;

    for (int index = total_bits - 1; index >= 0; index--) {
        int n = index / 32;
        int i = index % 32;

        int bit1 = get_bit_by_position(decimal1.bits[n], i);
        int bit2 = get_bit_by_position(decimal2.bits[n], i);

        if (bit1 > bit2) {
            result = 1;
            break;
        } else if (bit1 < bit2) {
            result = 0;
            break;
        }
    }
    
    return result;
}

/*
Вариант, когда arg_1 >= arg_2?
*/
int s21_big_is_greater_or_equal(s21_big_decimal *val_1, s21_big_decimal *val_2,
                                int sign) {
  return !s21_big_is_less(val_1, val_2, sign);
}

/*
Функции сравнивает 2 больших dec числа
Вариант, когда arg_1 > arg_2?
Знак по умолчанию равен = 0
*/
int s21_big_is_less(s21_big_decimal *big_val_1, s21_big_decimal *big_val_2,
                    int sign) {
  int is_less = 0;
  s21_big_decimal xor = {0};
  for (int i = 7; i >= 0; i--) {
    xor.bits[i] = big_val_1->bits[i] ^ big_val_2->bits[i];
  }
  int j = 256, bit = 0;
  while (bit != 1 && j != 0) {
    j--;
    bit = get_big_bit(xor, j);
  }
  if (bit && get_big_bit(*big_val_1, j) < get_big_bit(*big_val_2, j) &&
      !sign)
    is_less += 1;
  if (bit && get_big_bit(*big_val_1, j) > get_big_bit(*big_val_2, j) &&
      sign)
    is_less += 1;

  return is_less;
}