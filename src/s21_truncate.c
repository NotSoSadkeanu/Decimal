#include "s21_decimal.h"

/*Возвращает целые цифры указанного Decimal числа; любые дробные цифры
 * отбрасываются, включая конечные нули.*/
int s21_truncate(s21_decimal value, s21_decimal *result) {
  int error_code = 0;
  int scale = get_scale(value);
  if (result == NULL || scale > 28 || !is_last_bits_clear(&value)) {
    error_code = 1;
  } else if (scale > 0) {
    set_dec_to_zero(result);
    s21_decimal tmp = value, res;
    set_scale(&tmp, 0);
    set_sign(&tmp, 0);
    res = tmp;
    for (int i = 0; i < scale; i++) {
      s21_div_ten(res, &tmp);
      res = tmp;
    }
    *result = res;
    set_sign(result, get_sign(value));
  } else {
    *result = value;
  }
  return error_code;
}

