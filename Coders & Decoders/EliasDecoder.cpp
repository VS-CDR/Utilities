//#include <windows.h>
#include <algorithm>
#include <vector>
#include <string>
#include <iostream>

int Transfer(int byte, int radix) {
  int res = 0;
  int k = 1;
  while (byte != 0) {
    res += (byte % radix) * k;
    k <<= 1;
    byte /= radix;
  }
  return res;
}

void Decompress(std::vector<std::string>& dec, int arg) {
  for (int i = static_cast<int>(dec.size()) - 1; i >= 0; --i) {
    arg %= 2;
    int cnt = stoi(dec[i]);
    dec[i].clear();
    for (int j = 0; j < cnt; ++j) {
      dec[i].push_back(static_cast<char>(arg + 48));
    }
    ++arg;
  }
}

void DeleteNull(std::string& str) {
  int cnt = 0;
  for (std::size_t i = 0; i < str.size() && str[i] != '\0'; ++i) {
    if (str[i] == '0') {
      ++cnt;
      str.erase(i--, 1);
    } else if (cnt) {
      i = i + cnt + 1;
      str.insert(i, " ");
      cnt = 0;
    } else {
      str.insert(++i, " ");
    }
  }
}

std::vector<std::string> InDEC(std::string_view str) {
  std::vector<std::string> res;
  int combo;
  int p;
  for (int i = static_cast<int>(str.size() - 1); i >= 0; --i) {
    if (str[i] == ' ') {
      combo = 0;
      p = 0;
      for (int j = i - 1; j >= 0 && str[j] != ' '; --j) {
        combo += (static_cast<int>(str[j]) - '0') * (1 << p++);
      }
      res.push_back(std::to_string(combo));
    }
  }
  return res;
}

void DecodeAndOtput(const std::vector<std::string>& dec) {
  std::string res;
  std::string symb;
  std::ranges::for_each(dec, [&res](const auto& elem) { res += elem; });
  int isfull = static_cast<int>(res.size()) % 8;
  if (isfull) {
    int cntnon = isfull;
    for (std::size_t j = res.size() - 1; --cntnon >= 0; --j) {
      symb += res[j];
    }
    auto symbol = static_cast<unsigned char>(Transfer(stoi(symb), 10));
    symb.clear();
    std::cout << symbol;
  }
  for (int j = static_cast<int>(res.size()) - isfull - 1; j >= 0; --j) {
    symb += res[j];
    if (symb.size() == 8) {
      auto symbol = static_cast<unsigned char>(Transfer(stoi(symb), 10));
      symb.clear();
      std::cout << symbol;
    }
  }
  std::cout << std::endl;
}

void InputCode(std::string& code) {
  std::cout << "Input your code: ";
  std::getline(std::cin, code);
}

void RemoveSpaces(std::string& str);

int FirstBit(std::string& code) {
  int first_bit = code[0] == '0' ? 0 : 1;
  code.erase(0, 1);
  return first_bit;
}

int main() {
  std::ios::sync_with_stdio(false);
//  SetConsoleCP(1251); SetConsoleOutputCP(1251);
  std::string code;
  InputCode(code);
  int first_bit = FirstBit(code);
  RemoveSpaces(code);
  DeleteNull(code);
  std::vector<std::string> dec = InDEC(code);
  Decompress(dec, first_bit);
  DecodeAndOtput(dec);
}

void RemoveSpaces(std::string& str) {
  for (std::size_t i = 0; i < str.size(); ++i) {
    if (str[i] == ' ') {
      str.erase(i, 1);
    }
  }
}
