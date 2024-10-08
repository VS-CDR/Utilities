// A simple attempt to implement AES — rijndael with key size 128 bits and data block with size 16 bytes
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "../fastmath.hpp"

using ms = std::vector<std::vector<std::string>>;
using uc = unsigned char;

const uc SBOX[]{
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b,
    0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf,
    0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1,
    0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2,
    0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3,
    0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39,
    0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f,
    0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21,
    0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d,
    0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14,
    0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62,
    0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea,
    0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f,
    0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9,
    0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9,
    0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f,
    0xb0, 0x54, 0xbb, 0x16
};

void RotWord(std::string& key) {
  uc c = key[0];
  for (int i = 0; i < 3; ++i) { key[i] = key[i + 1]; }
  key[3] = c;
}

void SubByte(std::string& key) {
  for (int j = 0; j < 4; ++j) {
    key[j] = SBOX[static_cast<int>(static_cast<uc>(key[j]))];
  }
}

void XorWithRcon(std::string& key, int i) {
  int Rcon = FastPow(2, (i / 3) - 1);
  key[0] ^= Rcon;
}

void DropData(std::string& key, std::string& xor_col) {
  key.clear();
  xor_col.clear();
}

std::string To16(char arg) {
  std::string res;
  int p = 1;
  int n = static_cast<int>(static_cast<uc>(arg));
  while (p * 16 <= n) { p <<= 4; }
  do {
    int d = n / p;
    n %= p;
    p >>= 4;
    if (d > 9) {
      switch (d) {
        case 10:res += "A";
          break;
        case 11:res += "B";
          break;
        case 12:res += "C";
          break;
        case 13:res += "D";
          break;
        case 14:res += "E";
          break;
        case 15:res += "F";
          break;
      }
    } else { res += std::to_string(d); }
  } while (p);
  return res;
}

uc To10(std::string& arg) {
  int res = 0;
  switch (arg[1]) {
    case 'A': res += 10;
      break;
    case 'B': res += 11;
      break;
    case 'C': res += 12;
      break;
    case 'D': res += 13;
      break;
    case 'E': res += 14;
      break;
    case 'F': res += 15;
      break;
    default:res += static_cast<int>(arg[1] - 48);
  }
  switch (arg[0]) {
    case 'A': res += 10 * 16;
      break;
    case 'B': res += 11 * 16;
      break;
    case 'C': res += 12 * 16;
      break;
    case 'D': res += 13 * 16;
      break;
    case 'E': res += 14 * 16;
      break;
    case 'F': res += 15 * 16;
      break;
    default:res += static_cast<int>(arg[0] - 48) * 16;
  }
  return static_cast<uc>(res);
}

void MakeAStr(std::string& key, ms& table, int i) {
  std::string radix16 = "ABCDEF";
  for (int j = 0; j < 4; ++j) {
    if (std::ranges::find_first_of(table[j][i], radix16) == table[j][i].end()) {
      key += static_cast<uc>(To10(table[j][i]));
    } else {
      key += To10(table[j][i]);
    }
  }
}

void MakeAXorcol(std::string& xor_col, ms& table, int i) {
  xor_col.clear();
  std::string radix16 = "ABCDEF";
  for (int j = 0; j < 4; ++j) {
    if (std::ranges::find_first_of(table[j][i - 3], radix16) == table[j][i - 3].end()) {
      xor_col += static_cast<uc>(To10(table[j][i - 3]));
    } else {
      xor_col += To10(table[j][i - 3]);
    }
  }
}

int main() {
  std::ios::sync_with_stdio(false);
  ms table(4, std::vector<std::string>(4));
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      std::cin >> table[i][j];
    }
  }

  std::string key;
  std::string xor_col;
  bool step = true;
  for (int i = 3; step; ) {
    DropData(key, xor_col);
    MakeAStr(key, table, i);
    RotWord(key);
    SubByte(key);
    XorWithRcon(key, i);
    for (int cnt = 0; cnt < 4; ++cnt) {
      MakeAXorcol(xor_col, table, ++i);
      for (int j = 0; j < 4; ++j) {
        key[j] ^= xor_col[j];
        table[j].push_back(To16(key[j]));
      }
    }

    std::cout << "Create a new key: y/n?\n";
    char c;
    std::cin >> c;
    step = c == 'y';
  }
}
