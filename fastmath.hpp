#pragma once

int FastPow(int a, int n) {
  int res = 1;
  while (n > 0) {
    if (n % 2 != 0) {
      res *= a;
    }
    a *= a;
    n >>= 1;
  }
  return res;
}