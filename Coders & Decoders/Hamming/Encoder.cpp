#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "../../fastmath.hpp"

void AddExtraNulls(std::string& str) {
  const int kInfoBits = 11;
  while (str.length() % kInfoBits != 0) {
    str.push_back('0');
  }
}

void RemoveUnusedSymbols(std::string& str) {
  for (std::size_t i = 0; i < str.length(); ++i) {
    if (!isdigit(str[i])) {
      int cnt = 1;
      while (!isdigit(str[++i])) {
        ++cnt;
      }
      str.erase(i - cnt, cnt);
    }
  }
}

void AddControlBits(std::string& str) {
  for (std::size_t iter = 1; iter < str.length(); iter *= 2) {
    str.insert(str.begin() + iter - 1, 'c');
  }
}

void CalcControlBits(std::string& str) {
  const int kProtectBits = 4;
  for (int step = 0; step < kProtectBits; ++step) {
    int control_bit = FastPow(2, step);
    int cnt = 0;

    for (std::size_t i = control_bit + 1; i <= str.length(); ++i) {
      if (i & control_bit) {
        cnt += str[i - 1] - '0';
      }
    }

    str[static_cast<std::size_t>(control_bit - 1)] =
        static_cast<char>('0' + cnt % 2);
  }
}

std::vector<std::string> ProcessSegments(std::vector<std::string> codes) {
  std::ranges::for_each(codes, CalcControlBits);
  return codes;
}

std::vector<std::string> SplitInSegments(std::string_view str) {
  const int kInfoBits = 11;
  std::vector<std::string> segments;
  for (std::size_t i = 0; i < str.length(); i += kInfoBits) {
    segments.emplace_back(str.substr(i, kInfoBits));
    AddControlBits(segments.back());
  }
  return segments;
}

std::string VectorToStr(const std::vector<std::string>& codes) {
  std::string code;
  for (const auto& elem : codes) {
    code += elem;
  }
  return code;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::string code;

  std::cout << "Input your code: ";
  getline(std::cin, code);

  RemoveUnusedSymbols(code);
  AddExtraNulls(code);

  std::vector<std::string> codes = ProcessSegments(SplitInSegments(code));

  code = VectorToStr(codes);
  std::cout << "Your code: " << code;
}
