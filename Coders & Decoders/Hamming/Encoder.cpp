#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "../../fastmath.hpp"

void AddExtraNulls(std::string& s) {
  while (s.length() % 11) {
    s.push_back('0');
  }
}

void RemoveUnusedSymbols(std::string& s) {
  for (std::size_t i = 0; i < s.length(); ++i) {
    if (!isdigit(s[i])) {
      int cnt = 1;
      while (!isdigit(s[++i])) {
        ++cnt;
      }
      s.erase(i - cnt, cnt);
    }
  }
}

void AddControlBits(std::string& s) {
  for (std::size_t iter = 1; iter < s.length(); iter*=2) {
    s.insert(s.begin() + iter - 1, 'c');
  }
}

void CalcControlBits(std::string& s) {
  for (int step = 0; step <= 3; ++step) {
    int c = FastPow(2, step);
    int cnt = 0;
    for (std::size_t i = c + 1; i <= s.length(); ++i) {
      if (i & c) {
        cnt += s[i - 1] - '0';
      }
    }

    s[static_cast<std::basic_string<char, std::char_traits<char>, std::allocator<char>>::size_type>(c) - 1] = static_cast<char>(48 + cnt % 2);
  }
}

std::vector<std::string> ProcessSegments(std::vector<std::string> codes) {
  std::ranges::for_each(codes, CalcControlBits);
  return codes;
}

std::vector<std::string> SplitInSegments(std::string_view s) {
  std::vector<std::string> segments;
  for (std::size_t i = 0; i < s.length(); i += 11) {
    segments.emplace_back(s.substr(i, 11));
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

int main(){
  std::ios::sync_with_stdio(false);
  std::string code;

  std::cout << "Input your code: ";
  getline(std::cin, code);

  RemoveUnusedSymbols(code);
  AddExtraNulls(code);

  std::vector<std::string> codes = ProcessSegments(SplitInSegments(code));

  code = VectorToStr(codes);
  std::cout <<"Your code: "<< code;
}
