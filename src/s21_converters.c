#include "s21_decimal.h"

double float_div10(double num, int count);
void from_long_to_decimal(long int src, s21_decimal *dst);
int get_float_exp(float src);
int get_float_scale(float src);
int has_overflow(s21_decimal *src);

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int code = 0;

  if (!dst) {
    code = 1;
  } else {
    *dst = 0;
    s21_decimal truncated_decimal;
    set_dec_to_zero(&truncated_decimal);
    s21_truncate(src, &truncated_decimal);

    if (has_overflow(&truncated_decimal)) {
      code = 1;
    } else {
      *dst = truncated_decimal.bits[0];
      if (get_sign(src)) {
        *dst *= -1;
      }
    }
  }

  return code;
}

int has_overflow(s21_decimal *src) {
  int result = 0;
  for (int i = 1; i < 3; i++) {
    if (src->bits[i] != 0) {
      result = 1;
      i = 3;
    }
  }
  if (src->bits[0] < 0) result = 1;
  return result;
}

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
    int error = 0;

    if (dst == NULL) {
        error = 1;
    } else {
        memset(dst, 0, sizeof(s21_decimal));

        if (src < 0) {
            set_sign(dst, 1);
            src = -src;
        } else {
            set_sign(dst, 0);
        }

        dst->bits[0] = (unsigned int)(src); 
    }
    return error;
}

void from_decimal_to_big(s21_decimal decimal,
                                 s21_big_decimal *big_decimal) {
  for (int i = 0; i < 3; i++) {
    big_decimal->bits[i] = decimal.bits[i];
  }
  big_decimal->sign = get_sign(decimal);
  big_decimal->scale = get_scale(decimal);
}


int from_big_to_decimal(s21_big_decimal big_decimal, s21_decimal *decimal) {
    int func_return = 0;
    s21_big_decimal remainder = {0};
    s21_big_decimal ten;
    ten.bits[0] = 10;
    s21_big_decimal one;
    one.bits[0] = 1;
    int temp_value = big_decimal.scale;
    int sign = big_decimal.sign;

    while (!check_big(big_decimal) && big_decimal.scale > 0) {
        big_decimal = div_with_remainder(big_decimal, ten, &remainder);
        big_decimal.scale = --temp_value;

        if (remainder.bits[0] > 5 || (remainder.bits[0] == 5 && get_bit_by_position(big_decimal.bits[0], 0) == 1)) {
            big_decimal = (big_decimal.sign) ? sub_for_big(big_decimal, one) : add_for_big(big_decimal, one);
        }
    }

    if (check_big(big_decimal)) {
        for (int i = 0; i < 3; i++) {
            decimal->bits[i] = big_decimal.bits[i];
        }
        decimal->bits[3] = temp_value << 16;
        if (sign) {
            decimal->bits[3] = set_bit_by_position(decimal->bits[3], 31);
        }
    } else {
        func_return = (sign == 0) ? 1 : 2;
    }

    return func_return;
}

double float_div10(double num, int count) {
  double result = num;
  for (int i = 0; i < count; i++) result /= 10;
  return result;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int result;
  result = 0;
  *dst = 0.0;
  int scale = get_scale(src);
  double dst1 = (src.bits[0] + src.bits[1] * (4294967295 + 1) +
                 src.bits[2] * pow(4294967295 + 1, 2));
  dst1 = float_div10(dst1, scale);
  *dst = (float)dst1;
  if (get_sign(src) == 1) *dst *= -1;
  return result;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int result;
  if (!isinf(src) && !isnan(src) &&
      fabs(src) < 79228162514264337593543950335.0f && fabs(src) > 1e-28) {
    result = 0;
    set_dec_to_zero(dst);
    int sign = 0;
    if (src < 0) {
      sign = 1;
      src *= -1;
    }
    if (src == 0) {
    } else {
      int exp = get_float_exp(src);
      if (exp > 95)
        result = 1;
      else {
        long int scale = 0;
        float tmp = src;
        s21_decimal tmp_decimal;
        if (src < 1e-15) {
          src *= pow(10, 15);
          scale = 15;
        }
        if (src >= pow(2, 64)) {
          tmp /= pow(4294967295 + 1, 2);
          from_long_to_decimal(round(tmp), &tmp_decimal);
          dst->bits[2] = tmp_decimal.bits[0];
          src -= tmp_decimal.bits[0] * pow(4294967295 + 1, 2);
          tmp = src;
        }
        if (tmp >= pow(2, 32)) {
          tmp /= 4294967295 + 1;
          from_long_to_decimal(round(tmp), &tmp_decimal);
          dst->bits[1] = tmp_decimal.bits[0];
          src = src - tmp_decimal.bits[0] * (4294967295 + 1);
          tmp = src;
        }
        scale += get_float_scale(src);
        from_long_to_decimal(round(tmp * pow(10, scale)), &tmp_decimal);
        dst->bits[0] = tmp_decimal.bits[0];
        if (sign) set_sign(dst, 1);
        set_scale(dst, scale);
      }
    }
  } else {
    result = 1;
  }
  return result;
}

void from_long_to_decimal(long int src, s21_decimal *dst) {
  set_dec_to_zero(dst);
  dst->bits[0] = src;
}

int get_float_exp(float src) {
  unsigned int float_bits = *((unsigned int *)&src);
  int exp = ((2139095040 & float_bits) >> 23) - 127;
  if (exp == -127) {
    exp = 0;
  }
  return exp;
}

int get_float_scale(float src) {
  int scale = 0;
  long int tmp = (long int)src;
  while (src - ((float)tmp / (long int)pow(10, scale)) != 0) {
    scale++;
    tmp = src * (long int)pow(10, scale);
  }
  return scale;
}