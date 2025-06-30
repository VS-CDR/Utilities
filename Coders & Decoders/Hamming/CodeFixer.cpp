//#include <windows.h>
#include <algorithm>
#include <string>
#include <iostream>
#include <vector>
#include "../../fastmath.hpp"

const int kBlockSize = 15;

int FindMistake(std::string_view input) {
  int mistake = 0;
  for (int i = 0; i < kBlockSize; ++i) {
    if (input[i] == '1') {
      mistake ^= i + 1;
    }
  }
  return mistake;
}

void OutputFixed(std::string_view input, std::string& res) {
  int bit = 1;
  std::string info;
  for (int i = 0; i < kBlockSize; ++i) {
    if (i + 1 == bit) {
      bit <<= 1;
    } else {
      info += input[i];
    }
  }
  std::cout << info << " ";
  res += info;
}

void Fix(int mistake, std::string& input) {
  if (mistake != 0) {
    input[mistake - 1] ^= '0' ^ '1';
    if (FindMistake(input) != 0) {
      std::cout << "Invalid code";
    }
  }
}

void SplitInParts(std::vector<std::string>& parts, std::string_view input) {
  for (auto [BlockBegin, BlockInd] = std::pair<std::size_t, int>(0, 0);
       BlockBegin < input.length();
       BlockBegin += kBlockSize) {
    parts[BlockInd++] = std::string(input, BlockBegin++, kBlockSize);
  }
}

void PrepareCode(std::vector<std::string>& parts, std::string& input) {
  std::cout << "Are there any separating spaces: y/n?" << '\n';
  char choose;
  std::cin >> choose;
  if (choose == 'n') {
    for (size_t i = kBlockSize; i < input.length(); i += 16) {
      input.insert(i, " ");
    }
  }

  SplitInParts(parts, input);
}

void InputCode(std::string& input) {
  std::cout << "Input your code: ";
  std::getline(std::cin, input);
}

void OutputDecoded(std::string_view res);

void ChooseOptions(std::string& res) {
  std::cout << '\n' << "Split in eights(8): y/n?" << '\n';
  char choose;
  std::cin >> choose;
  if (choose == 'y') {
    for (std::size_t i = 8; i < res.length(); i += 9) {
      res.insert(i, " ");
    }
    std::cout << "Output the result in file: y/n?" << '\n';
    std::cin >> choose;
    if (choose == 'y') {
      freopen("res.txt", "w", stdout);
    }
  }
}

std::string ProcessCode(std::vector<std::string>& parts) {
  std::string res;
  for (auto& str : parts) {
    int mistake = FindMistake(str);
    if (std::ranges::all_of(str, [](char c) { return c == '0' || c == '1'; })) {
      Fix(mistake, str);
      OutputFixed(str, res);
    } else {
      std::cout << "Invalid code";
    }
  }
  return res;
}

int main() {
  std::ios::sync_with_stdio(false);
//  SetConsoleCP(1251); SetConsoleOutputCP(1251);
  std::string input;
  InputCode(input);
  std::vector<std::string> parts(input.length() / kBlockSize);
  PrepareCode(parts, input);
  std::string res = ProcessCode(parts);
  ChooseOptions(res);
  OutputDecoded(res);
}

void OutputDecoded(std::string_view res) {
  for (std::size_t i = 0; i < res.length(); ++i) {
    int pow = 7;
    int code = 0;
    while (res[i] != ' ' && i < res.length()) {
      code += FastPow(2, pow--) * (res[i++] - '0');
    }
    std::cout << static_cast<unsigned char>(code);
  }
}
