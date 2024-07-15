#pragma once

template <typename T>
T FastPow(T a, T n) {
  T res = 1;
  while (n > 0) {
    if (n % 2 != 0) {
      res *= a;
    }
    a *= a;
    n /= 2;
  }
  return res;
}
