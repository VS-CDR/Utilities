#include <iostream>
#include <array>
#include "fastmath.hpp"

int Transfer(int byte, int radix) {
  int res = 0;
  int k = 1;
  while (byte != 0) {
    res += (byte % radix) * k;
    k *= 10;
    byte /= radix;
  }
  return res;
}

int FormOut(int out) {
  int radix = 0;
  int tmp = out;
  while (tmp > 0) {
    tmp /= 10;
    ++radix;
  }
  for (int i = radix; i < 8; ++i) {
    std::cout << 0;
  }
  return out;
}

void Info(const int* byte) {
  std::cout << "In BIN(2): ";
  for (int i = 0; i < 4; ++i) {
    std::cout << FormOut(Transfer(byte[i], 2));
    if (i != 3) {
      std::cout << ".";
    }
  }
  std::cout.unsetf(std::ios::dec);
  std::cout.setf(std::ios::hex);
  std::cout << "\nIn HEX(16): ";
  for (int i = 0; i < 4; ++i) {
    std::cout << byte[i];
    if (i != 3) {
      std::cout << ".";
    }
  }
  std::cout.unsetf(std::ios::hex);
  std::cout.setf(std::ios::oct);
  std::cout << '\n' << "In OCT(8): ";
  for (int i = 0; i < 4; ++i) {
    std::cout << byte[i];
    if (i != 3) {
      std::cout << ".";
    }
  }
  std::cout.unsetf(std::ios::oct);
  std::cout.setf(std::ios::dec);
}

void CIDR(const int* byte) {
  int cidr;
  std::array<int, 32> ip{};
  std::cin >> cidr;
  for (int i = 0; i < 4; ++i) {
    for (int j = i * 8; j < 8 * (i + 1); ++j) {
      int octet = Transfer(byte[i], 2) % FastPow(10, 8 - (j % 8));
      int div = FastPow(10, 7 - (j % 8));
      ip[j] = octet / div;
    }
  }
  std::cout << '\n' << "Net ID = ";
  for (int i = 0; i < cidr; ++i) {
    if (i % 8 == 0 && i != 0) { std::cout << '.'; }
    std::cout << ip[i];
  }
  std::cout << "\nHost ID = ";
  for (int i = cidr; i < 32; ++i) {
    if (i % 8 == 0) { std::cout << '.'; }
    std::cout << ip[i];
  }
  std::cout << "\n";
}

void ClassOfIP(const int* byte) {
  if (byte[0] < 128) {
    if (byte[0] == 10 && byte[0] <= 126 && byte[1] < 256 && byte[2] < 256
        && byte[3] < 256) {
      std::cout << "\n!Common adress!\n";
    } else { std::cout << "\nUnic adress\n"; }
    std::cout << "\nA class:\n\tNet ID = " << FormOut(Transfer(byte[0], 2)) << "\n";
    std::cout << "\tHost ID = ";
    for (int i = 1; i < 4; ++i) {
      std::cout << FormOut(Transfer(byte[i], 2));
      if (i != 3) { std::cout << "."; }
    }
  } else if (byte[0] < 192) {
    if (byte[0] == 172 && byte[1] < 32 && byte[1] > 15 && byte[2] < 256
        && byte[3] < 256) {
      std::cout << "\n!Common adress!\n";
    } else {
      std::cout << "\nUnic adress\n";
    }
    std::cout << "\nB class:\n\tNet ID = " << FormOut(Transfer(byte[0], 2)) << "."
         << FormOut(Transfer(byte[1], 2)) << "\n";
    std::cout << "\tHost ID = " << FormOut(Transfer(byte[2], 2)) << "."
         << FormOut(Transfer(byte[3], 2));
  } else if (byte[0] < 224) {
    if (byte[0] == 192 && byte[1] == 168 && byte[2] < 256 && byte[3] < 256) {
      std::cout << "\n!Common adress!\n";
    } else {
      std::cout << "\nUnic adress\n";
    }
    std::cout << "\nC class:\n\tNet ID = ";
    for (int i = 0; i < 3; ++i) {
      std::cout << FormOut(Transfer(byte[i], 2));
      if (i != 2) { std::cout << "."; }
    }
    std::cout << "\n\tHost ID = " << FormOut(Transfer(byte[3], 2));
  } else {
    std::cout << "\tD/E class";
  }
  std::cout << "\n";
}

int main() {
  std::ios::sync_with_stdio(false);
  std::array<int, 4> byte;
  char dot;
  char cmd = '0';
  for (int i = 0; i < 4; ++i) {
    std::cin >> byte[i];
    if (i != 3) {
      std::cin >> dot;
    }
  }
  Info(byte.data());
  std::cout << std::endl;
  std::cin >> cmd;
  if (cmd == '/') {
    CIDR(byte.data());
  } else {
    ClassOfIP(byte.data());
  }
  std::cin.get();
  std::cin.get();
}