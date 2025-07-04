#include <algorithm>
#include <cctype>
#include <cmath>
#include <concepts>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include "../../fastmath.hpp"

int Transfer(int arg, int& k, int radix) {
  int res = 0;
  if (arg < 0) {
    arg += 256;
  }
  for (k = 1; arg > 0; arg /= radix, k *= 10) {
    res += (arg % radix) * k;
  }
  return res;
}

void DecodeTable(std::string_view str, std::vector<int>& code) {
  for (std::size_t i = 0; i < str.length(); ++i) {
    if (isalpha(str[i]) != 0) {
      if (str[i] > 95) {
        code[i] = str[i] - 71;
      } else {
        code[i] = str[i] - 65;
      }
    } else if (isdigit(str[i]) != 0) {
      code[i] = str[i] + 4;
    } else if (str[i] == '=') {
      code[i] = 0;
    } else if (str[i] == '+') {
      code[i] = 62;
    } else {
      code[i] = 63;
    }
  }
}

void TransferToASCII(std::vector<int> &code, const std::vector<int>& bin,
                     std::size_t radix) {
  for (std::size_t i = 0; i < code.size(); ++i) {
    for (std::size_t j = i * radix; j < (i + 1) * radix; ++j) {
      for (int bin_pow = 1 << (radix - 1); bin_pow > 0; bin_pow /= 2, ++j) {
        code[i] += bin_pow * bin[j];
      }
    }
  }
}

void TransferToBase64(std::size_t end, std::vector<int>& code,
                      const std::vector<int>& bin, std::size_t r,
                      std::string& res) {
  for (std::size_t i = 0; i <= end / r; ++i) {
    for (std::size_t j = i * r; j < (i + 1) * r && j < end; ++j) {
      for (int bin_p = 1 << (r - 1); bin_p > 0 && j < end; bin_p /= 2, ++j) {
        code[i] += bin_p * bin[j];
      }
      if (code[i] < 26) {
        res += static_cast<char>(code[i] + 65);
      } else if (code[i] < 52) {
        res += static_cast<char>(code[i] + 71);
      } else if (code[i] < 62) {
        res += static_cast<char>(code[i] - 4);
      } else if (code[i] == 62) {
        res += static_cast<char>(43);
      } else {
        res += static_cast<char>(47);
      }
    }
  }
}

void SplitIntoDigits(int& dec_power, int n, std::vector<int>& bin,
                     std::size_t& ind) {
  for (; dec_power > 0; n %= dec_power, dec_power /= 10, ++ind) {
    bin[ind] = n / dec_power;
  }
}

void TransferToBin(std::string_view str, std::vector<int>& code,
                   std::vector<int>& bin, int arg) {
  int dec_power = 1;
  for (std::size_t i = 0; i < str.length(); ++i) {
    code[i] = Transfer(code[i], dec_power, 2);
    dec_power /= 10;
    for (std::size_t j = i * arg; j < (i + 1) * arg; ++j) {
      if (dec_power < FastPow(10, arg)) {
        if (dec_power == 0 && (isalpha(str[i]) != 0)) {
          for (std::size_t lim = j; j < lim + arg; ++j) {
            bin[j] = 0;
          }
        } else {
          int cnt = 0;
          for (int copy_p(dec_power); copy_p > 0; copy_p /= 10) {
            ++cnt;
          }
          for (int r = 0; r < arg - cnt; ++r, ++j) {
            bin[j] = 0;
          }
          SplitIntoDigits(dec_power, code[i], bin, j);
        }
      } else if (dec_power > 0) {
        SplitIntoDigits(dec_power, code[i], bin, j);
      }
    }
  }
}

void AddExtraSymbolsInTheEnd(std::size_t end, std::string& res) {
  if (end % 6 != 0) {
    for (std::size_t i = 1; i < 6 - (end % 6); i <<= 1) {
      res += '=';
    }
  }
}

void OutputResult(std::string& input, std::string_view res,
                  const std::string &filename = "output.txt");
int main() {
  std::ios::sync_with_stdio(false);

  std::string input;
  std::cout << "encode or decode (en/de): ";
  while (input != "en" && input != "de") {
    getline(std::cin, input);
  }

  std::string str;
  std::cout << "Input your string: ";
  getline(std::cin, str);

  std::string res;
  if (input == "de") {
    while (str.back() == '=') {
      str.pop_back();
    }
    std::vector code(str.length(), 0);
    std::vector bin(str.length() * 6, 0);

    DecodeTable(str, code);
    TransferToBin(str, code, bin, 6);
    code.assign(str.length() * 3 / 4, 0); // 6/8 = 3/4

    TransferToASCII(code, bin, 8);
    std::copy(code.begin(), code.end(), std::back_inserter(res));
  } else if (input == "en") {
    std::vector<int> code(str.begin(), str.end());
    std::vector<int> bin(str.length() * 8);

    TransferToBin(str, code, bin, 8);
    auto end = str.length() * 8;
    code.assign((end / 6) + 1, 0);

    TransferToBase64(end, code, bin, 6, res);
    AddExtraSymbolsInTheEnd(end, res);
  }

  OutputResult(input, res);
  std::cout << "You could close this app." << std::endl;
}

void OutputResult(std::string &input, std::string_view res,
                  const std::string &filename) {
  constexpr std::string_view kOutputChoose =
      "Where to output the result: file or console (input f or c)?\n"
      "It's recommended to use a file because console may not have enough buff "
      "size to display";
  std::cout << kOutputChoose << std::endl;

  while (input != "f" && input != "c") {
    std::getline(std::cin, input);
    if (input == "f") {
      std::fstream fout(filename, std::ios::out);
      fout << res;
    } else if (input == "c") {
      std::cout << res << std::endl;
    } else {
      std::cout << "Invalid option! Choose (f/c): ";
    }
  }
}
