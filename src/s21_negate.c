#include "s21_decimal.h"

/*Возвращает результат умножения указанного Decimal на -1.*/
int s21_negate(s21_decimal value, s21_decimal *result) {
  int error_code = 0;
  if (result == NULL || get_scale(value) > 28 ||
      !is_last_bits_clear(&value)) {
    error_code = 1;
  } else {
    *result = value;
    set_sign(result, !get_sign(value));
  }
  return error_code;
}

