#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <ranges>
#include <utility>
#include <vector>

int Transfer(int byte, int radix) {
  int res = 0;
  for (int k = 1; byte != 0; k <<= 1, byte /= radix) {
    res += (byte % radix) * k;
  }
  return res;
}

void Decompress(std::vector<std::string>& dec, int arg) {
  for (auto i = std::ssize(dec) - 1; i >= 0; --i, arg ^= 1) {
    auto cnt = std::stoi(std::exchange(dec[i], std::string{}));
    std::fill_n(std::back_inserter(dec[i]), cnt, '0' + arg);
  }
}

void DeleteNull(std::string &str) {
  int cnt = 0;
  for (int i = 0; i < std::ssize(str) && str[i] != '\0'; ++i) {
    if (str[i] == '0') {
      ++cnt;
      str.erase(i--, 1);
    } else {
      if (cnt != 0) {
        i += std::exchange(cnt, 0);
      }
      str.insert(++i, " ");
    }
  }
}

decltype(auto) InDEC(std::string_view str) {
  std::vector<std::string> res;
  for (auto i = std::ssize(str) - 1; i >= 0; --i) {
    if (str[i] == ' ') {
      int combo = 0;
      int pow = 0;
      for (auto j = i - 1; j >= 0 && str[j] != ' '; --j) {
        combo += static_cast<int>(str[j] - '0') * (1 << pow++);
      }
      res.emplace_back(std::to_string(combo));
    }
  }
  return res;
}

void DecodeAndOtput(const std::vector<std::string>& dec) {
  auto joined = dec | std::views::join;
  std::string res(joined.begin(), joined.end());

  std::string symb;
  auto is_full = std::ssize(res) % 8;
  if (is_full != 0) {
    auto cnt_nan = is_full - 1;
    for (auto j = std::ssize(res) - 1; cnt_nan >= 0; --j, --cnt_nan) {
      symb += res[j];
    }
    std::cout << static_cast<char>(
      Transfer(std::stoi(std::exchange(symb, std::string{})), 10)
    );
  }
  for (auto j = std::ssize(res) - is_full - 1; j >= 0; --j) {
    symb += res[j];
    if (symb.size() == 8) {
      std::cout << static_cast<char>(
        Transfer(std::stoi(std::exchange(symb, std::string{})), 10)
      );
    }
  }
  std::cout << std::endl;
}

void InputCode(std::string& code) {
  std::cout << "Input your code: ";
  std::getline(std::cin, code);
}

void RemoveSpaces(std::string &str);

int FirstBit(std::string& code) {
  int first_bit = code[0] == '0' ? 0 : 1;
  code.erase(0, 1);
  return first_bit;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::string code;
  InputCode(code);
  int first_bit = FirstBit(code);
  RemoveSpaces(code);
  DeleteNull(code);
  auto dec = InDEC(code);
  Decompress(dec, first_bit);
  DecodeAndOtput(dec);
}

void RemoveSpaces(std::string& str) {
  int ins_pos = 0;
  for (auto ch : str) {
    if (ch != ' ') {
      str[ins_pos++] = ch;
    }
  }
  str.resize(ins_pos);
}
