//#include <windows.h>
#include <string>
#include <iostream>
#include <vector>
#include "../../fastmath.hpp"

int FindMistake(std::string& input) {
  int mistake = 0;
  for (int i = 0; i < 15; ++i) {
    if (input[i] == '1') {
      mistake ^= i + 1;
    }
  }
  return mistake;
}

bool CheckTheDigits(std::string& input) {
  for (int i = 0; i < 15; ++i) {
    if (input[i] != '0' && input[i] != '1') {
      return false;
    }
  }
  return true;
}

void OutRes(int b, std::string& input, std::string& res) {
  std::string info;
  for (int i = 0; i < 15; ++i) {
    if (i + 1 == b) { b <<= 1; }
    else { info += input[i]; }
  }
  std::cout << info << " ";
  res += info;
}

void Fix(int mistake, std::string& input) {
  if (mistake) {
    input[mistake - 1] ^= '0' ^ '1';
    if (FindMistake(input)) {
      std::cout << "Invalid code";
    }
  }
}

void SplitInParts(std::vector<std::string>& parts, std::string& input) {
  for (std::pair<std::size_t, int> iters(0, 0); iters.first < input.length();
       iters.first += 15) {
    std::string tmp(input, iters.first, 15);
    parts[iters.second++] = tmp;
    ++iters.first;
  }
}

void PrepareCode(std::vector<std::string>& parts, std::string& input) {
  std::cout << "Are there any separating spaces: y/n?" << '\n';
  char c;
  std::cin >> c;
  if (c == 'n') {
    for (size_t i = 15; i < input.length(); i += 16) {
      input.insert(i, " ");
    }
  }

  SplitInParts(parts, input);
}

void InputCode(std::string& input) {
  std::getline(std::cin, input);
}

int main() {
  std::ios::sync_with_stdio(false);
//  SetConsoleCP(1251); SetConsoleOutputCP(1251);

  std::cout << "Input your code: ";

  int mistake = 0;
  std::string input;
  std::string res;
  InputCode(input);
  std::vector<std::string> parts(input.length() / 15);

  PrepareCode(parts, input);
  for (auto it = parts.begin(); it < parts.end(); ++it) {
    mistake = FindMistake(*it);
    if (CheckTheDigits(*it)) {
      Fix(mistake, *it);
      OutRes(1, *it, res);
    } else {
      std::cout << "Invalid code";
    }
  }

  std::cout << "\nSplit in eights(8): y/n?\n";
  char c;
  std::cin >> c;

  if (c == 'y') {
    for (std::size_t i = 8; i < res.length(); i += 9) {
      res.insert(i, " ");
    }
    std::cout << "Output the result in file: y/n?\n";
    std::cin >> c;
    if (c == 'y') {
      freopen("res.txt", "w", stdout);
    }
  }

  for (std::size_t i = 0; i < res.length(); ++i) {
    int pow = 7;
    int code = 0;
    while (res[i] != ' ' && i < res.length()) {
      code += FastPow(2, pow--) * static_cast<int>(res[i++] - 48);
    }
    std::cout << static_cast<unsigned char>(code);
  }
}
