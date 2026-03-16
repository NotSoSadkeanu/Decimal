#include "s21_decimal.h"


void set_bit(s21_decimal *decimal, int position, unsigned bit) {
  int mask = 1 << (position % 32);  // сдвигаем 1 влево на н позицию и получаем
                                    // маску, например, 0001000
  if (bit)
    decimal->bits[position / 32] |=
        mask;  // если бит не равен 0, то побитово прибавляем маску
  else
    decimal->bits[position / 32] &=
        ~mask;  // если равен 0, то побитово умножаем на побитовое отрицание
                // маски, например, 1110111
}

int get_bit(s21_decimal decimal, int position) {
  int mask = 1 << (position % 32);  // сдвигаем 1 влево на н позицию и получаем
                                    // маску, например, 0001000
  return !!(decimal.bits[position / 32] &
            mask);  // побитово умножаем ев маску и преобразуем в 0 или 1
}

int get_bit_by_position(const int value, const int position) {
    return ((value & (1U << position)) != 0);
}

int set_bit_by_position(const int value, const int position) {
    return (value | (1U << position));
}

int get_scale(s21_decimal decimal) {
  return (decimal.bits[3] & 0x00FF0000) >>
         16;  // 4ый бит побитово умножаем на 11111111 00000000 00000000
              // 00000000 и сдвигаем вправо на 16 позиций
}

/*Функция для записи бит в большое dec число по указанной позиции*/
// void set_big_bit(s21_big_decimal *decimal, int position, unsigned bit) {
//   int mask = 1 << (position % 32);
//   if (bit)
//     decimal->bits[position / 32] |= mask;
//   else
//     decimal->bits[position / 32] &= ~mask;
// }

/*Функция для получения бит в большое dec число по указанной позиции*/
int get_big_bit(s21_big_decimal decimal, int position) {
  int mask = 1 << (position % 32);
  return !!(decimal.bits[position / 32] & mask);
}



void set_scale(s21_decimal *decimal, int scale) {
  decimal->bits[3] =
      ((decimal->bits[3] & 0x80000000) |
       (scale << 16));  // 4ый бит побитово умножаем на 10000000 00000000
                        // 00000000 00000000
                        //  и побитово прибавляем степень предварительно
                        //  смещенную влево на 16 позиций
}


int get_sign(s21_decimal decimal) {
  return get_bit(decimal, 127);
}  // получаем 4ый бит 32 позицию 1 (отрицательное число) или 0
   // (положительное)


void set_sign(s21_decimal *decimal, int bit) {
  set_bit(decimal, 127,
              bit);  // записываем в 4ый бит 32 позицию 1 (отрицательное число)
                     // или 0 (положительное)
}

int check_big(s21_big_decimal value) {
  int result = 0;
  if (value.bits[5] == 0 && value.bits[4] == 0 && value.bits[3] == 0) {
    result = 1;
  }
  return result;
}


void big_shift(s21_big_decimal *decimal, int shift) {
    if (shift <= 0) {
        return;
    }

    for (int i = 0; i < shift; i++) {  // Цикл для каждого сдвига на 1 бит
        int now_bit = 0, prev_bit = 0;

        // Цикл по всем блокам (по 32 бита)
        for (int j = 0; j < 6; j++) { 
            // Запоминаем старший бит текущего блока для переноса в следующий
            now_bit = (decimal->bits[j] >> 31) & 1;

            // Сдвигаем текущий блок влево на 1 бит
            decimal->bits[j] <<= 1;

            // Если в предыдущем блоке был старший бит равен 1, переносим его в младший бит текущего блока
            if (prev_bit) {
                decimal->bits[j] |= 1;  // Устанавливаем младший бит
            }

            // Сохраняем старший бит текущего блока для следующей итерации
            prev_bit = now_bit;
        }
    }
}


void normalization(s21_big_decimal *decimal1, s21_big_decimal *decimal2) {
    s21_big_decimal multr = {0};
    multr.bits[0] = 10;

    int larger_scale = (decimal1->scale > decimal2->scale) ? 
                                                decimal1->scale : decimal2->scale;

    mul_for_scale(decimal1, multr, larger_scale);
    mul_for_scale(decimal2, multr, larger_scale);
}


int less_num(s21_decimal num1, s21_decimal num2, int res) {
  if (num1.bits[3] == num2.bits[3]) {
    if (num1.bits[2] == num2.bits[2]) {
      if (num1.bits[1] == num2.bits[1]) {
        if (num1.bits[0] == num2.bits[0]) {
          res = 0;
        } else
          res = comp_bit(num1.bits[0], num2.bits[0]);
      } else
        res = comp_bit(num1.bits[1], num2.bits[1]);
    } else
      res = comp_bit(num1.bits[2], num2.bits[2]);
  } else
    res = comp_exp(num1, num2);
  return res;
}

int comp_bit(int bit1, int bit2) {
  int res = 0;
  if (bit1 < bit2) {
    res = 1;
  }
  return res;
}

int comp_exp(s21_decimal num1, s21_decimal num2) {
  int exp1 = get_scale(num1);
  int exp2 = get_scale(num2);

  if (exp1 > exp2) {
    num2 = upper_10(exp1, &num2);
  } else if (exp2 > exp1) {
    num1 = upper_10(exp2, &num1);
  }
  return comp_bit(num1.bits[2], num2.bits[2]);
}



s21_decimal upper_10(int pow, s21_decimal *val) {
  int lower = get_scale(*val);
  bin_conv(val, pow);
  for (int i = 0; i < (pow - lower); i++) {
    s21_multi_ten(val);
  }
  return (*val);
}

void bin_conv(s21_decimal *val, int res) {
  int pow = 16;
  for (int i = 20; i > 15; i--, pow /= 2) {
    if (res >= pow) {
      res -= pow;
      on_bit_set_1(&val->bits[3], i);
    } else
      on_bit_set_0(&val->bits[3], i);
  }
}




void on_bit_set_1(int *val, int BitNumber) { *val |= 1U << BitNumber; }

void on_bit_set_0(int *val, int BitNumber) {
  *val &= ~(1U << BitNumber);
}


// //моё 
// int s21_logic_add_bit(s21_decimal val_1, s21_decimal val_2,
//                       s21_decimal *result) {
//   int prev = 0;
//   int res;
//   int err = 0;
//   for (int j = 0; j < 3; j++) {
//     for (int i = 0; i < 32; i++) {
//       int next = 0;
//       int a = get_bit_by_position(val_1.bits[j], i);
//       int b = get_bit_by_position(val_2.bits[j], i);
//       if (a == 0) {
//         if (b == 0 && prev == 0) {
//           res = 0;
//           next = 0;
//         } else if (b == 0 || prev == 0) {
//           res = 1;
//           next = 0;
//         } else {
//           res = 0;
//           next = 1;
//         }
//       } else {
//         if (b == 0 && prev == 0) {
//           res = 1;
//           next = 0;
//         } else if (b == 0 || prev == 0) {
//           res = 0;
//           next = 1;
//         } else {
//           res = 1;
//           next = 1;
//         }
//       }
//       prev = next;

//       if (res) {
//         on_bit_set_1(&(result->bits[j]), i);
//       } else {
//         on_bit_set_0(&(result->bits[j]), i);
//       }
//     }
//   }
//   return err;
// }

int is_last_bits_clear(const s21_decimal *value) {
  int count = 0;
  for (int i = 96; i < 112; i++) count += get_bit(*value, i);
  for (int i = 120; i < 127; i++) count += get_bit(*value, i);
  return count == 0 ? 1 : 0;
}

void set_dec_to_zero(s21_decimal *dst) {
  for (int i = 0; i < 4; i++) {
    dst->bits[i] = 0;
  }
}

/*умножение decimal на 10*/
void s21_multi_ten(s21_decimal *value) {
  s21_decimal num = *value;     // 00000010 = 2
  s21_left_shift(&num, 3);      // 00010000 = 16
  s21_left_shift(value, 1);     // 00000100 = 4
  s21_add(num, *value, value);  // побитово складываем
}


// void s21_sum(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
//   int tmp = 0;
//   for (int i = 0; i < 96; i++) {
//     int bits = get_bit(value_1, i) + get_bit(value_2, i) + tmp;
//     set_bit(result, i, bits % 2);
//     tmp = bits / 2;
//   }
// }

void s21_left_shift(s21_decimal *decimal, int shift) {
  unsigned tmp = 0;
  for (int i = 0; i < 3; i++) {
    unsigned temp = decimal->bits[i];
    decimal->bits[i] <<= shift;  // побитовый сдвиг влево на шифт
    decimal->bits[i] |= tmp;  // побитово прибавляем тмп
    tmp = temp >>
          (32 -
           shift);  // присваиваем тмп результат сдвига темп вправо на (32-шифт)
  }
}

int s21_div_ten(s21_decimal value_1, s21_decimal *result) {
  s21_decimal result_res = *result;
  
  // Инициализируем массив bits с 6 элементами и добавляем sign и scale
  s21_big_decimal big_1 = {0}, big_2 = {{10, 0, 0, 0, 0, 0}, 0, 0}, big_3 = {0};
  
  from_decimal_to_big(value_1, &big_1);
  set_dec_to_zero(&result_res);
  
  while (s21_big_is_greater_or_equal(&big_1, &big_2, 0)) {
    s21_big_decimal big_4 = big_2;
    int tmp = 0;
    
    // Инициализируем num_3 с правильным количеством элементов
    s21_big_decimal num_3 = {{1, 0, 0, 0, 0, 0}, 0, 0};
    
    while (s21_big_is_greater_or_equal(&big_1, &big_4, 0)) {
      big_shift(&big_4, 1);
      tmp++;
    }
    
    big_4 = big_2;
    
    for (int i = 0; i < tmp - 1; i++) {
      big_shift(&big_4, 1);
      big_shift(&num_3, 1);
    }
    
    big_1= sub_for_big(big_1, big_4);
    big_3 = add_for_big(big_3, num_3);
  }
  
  from_big_to_decimal(big_3, result);
  
  return 0;
}


/*если decimal не пустота взвращаем 1, иначе 0*/
int s21_decimal_not_empty(s21_decimal *value) {
  return !!value->bits[0] + !!value->bits[1] + !!value->bits[2];
}

void s21_decimal_add_one(s21_decimal *src) {
  s21_decimal one = {{1, 0, 0, 0}};
  s21_add(*src, one, src);
}

void s21_get_remainder(const s21_decimal *res, s21_decimal *tmp,
                       s21_decimal *dec_remainder) {
  s21_div_ten(*res, tmp);
  s21_decimal tmp2 = *tmp;
  s21_multi_ten(&tmp2);
  s21_sub(*res, tmp2, dec_remainder);
}
