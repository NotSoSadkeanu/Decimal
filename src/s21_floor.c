#include "s21_decimal.h"


int s21_floor(s21_decimal value, s21_decimal *result) {
  int error_code = 0;
  int scale = get_scale(value);
  if (result == NULL || scale > 28 || !is_last_bits_clear(&value)) {
    error_code = 1;
  } else if (scale > 0) {
    set_dec_to_zero(result);
    s21_decimal tmp = value, dec_remainder = {0}, res = {0}, tmp2 = {0};
    set_scale(&tmp, 0);
    set_sign(&tmp, 0);
    res = tmp;
    tmp2 = tmp;
    for (int i = 0; i < scale; i++) {
      s21_div_ten(res, &tmp);
      res = tmp;
    }
    for (int i = 0; i < scale; i++) {
      s21_multi_ten(&tmp);
    }
    s21_sub(tmp2, tmp, &dec_remainder);
    if (s21_decimal_not_empty(&dec_remainder)) {
      float f = 0;
      s21_from_decimal_to_float(dec_remainder, &f);
      for (int i = 0; i < scale - 1; i++) {
        f /= 10;
      }
      if (get_sign(value)) {
        s21_decimal add = {{1, 0, 0, 0}};
        s21_add(res, add, &res);
      }
    }
    *result = res;
    set_sign(result, get_sign(value));
  } else {
    *result = value;
  }
  return error_code;
}

