//#include <windows.h> for windows
#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int Transfer(int arg, int& k, int radix) {
  int res = 0;
  if (arg < 0) {
    arg += 256;
  }
  k = 1;
  while (arg != 0) {
    res += (arg % radix) * k;
    k *= 10;
    arg /= radix;
  }
  return res;
}

void DecodeTable(const std::string& str, std::vector<int>& code, bool decode = true) {
  if (decode) {
    for (unsigned i = 0; i < str.length(); ++i) {
      if (isalpha(str[i])) {
        if (str[i] > 95) {
          code[i] = str[i] - 71;
        } else {
          code[i] = str[i] - 65;
        }
      } else if (isdigit(str[i])) {
        code[i] = str[i] + 4;
      } else if (str[i] == '=') {
        code[i] = 0;
      } else if (str[i] == '+') {
        code[i] = 62;
      } else {
        code[i] = 63;
      }
    }
  } else {
    for (std::size_t i = 0; i < str.length(); ++i) {
      code[i] = str[i];
    }
  }
}

void SplitIntoDigits(int& p, int n, std::vector<int>& bin, int& i) {
  while (p) {
    bin[i++] = n / p;
    n %= p;
    p /= 10;
  }
}

void TransferToAscii(int end,
                     std::vector<int>& code,
                     std::vector<int>& bin,
                     int r) {
  for (int i = 0; i < end / r; ++i) {
    code[i] = 0;
    for (int j = i * r; j < i * r + r; ++j) {
      int p = 1 << (r - 1);
      while (p) {
        code[i] = code[i] + (p * bin[j++]);
        p >>= 1;
      }
    }
  }
}

void TransferToBase64(int& end,
                      std::vector<int>& code,
                      std::vector<int>& bin,
                      int r,
                      std::string& res) {
  for (int i = 0; i <= end / r; ++i) {
    code[i] = 0;
    for (int j = i * r; j < i * r + r && j < end; ++j) {
      int p = 1 << (r - 1);
      while (p && j < end) {
        code[i] = code[i] + (p * bin[j++]);
        p >>= 1;
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

int TransferToBin(std::string& str,
                  std::vector<int>& code,
                  std::vector<int>& bin,
                  int arg) {
  int p = 1;
  for (int i = 0; i < static_cast<int>(str.length()); ++i) {
    code[i] = Transfer(code[i], p, 2);
    p /= 10;
    for (int j = i * arg; j < arg + i * arg; ++j) {
      if (p < pow(10, arg)) {
        if (!p && isalpha(str[i])) {
          int lim = j;
          while (j < lim + arg) {
            bin[j++] = 0;
          }
        } else {
          int copy_p = p;
          int cnt = 0;
          while (copy_p) {
            copy_p /= 10;
            cnt++;
          }
          for (int r = 0; r < arg - cnt; ++r) {
            bin[j++] = 0;
          }
          SplitIntoDigits(p, code[i], bin, j);
        }
      } else if (p) {
        SplitIntoDigits(p, code[i], bin, j);
      }
    }
  }
  return (static_cast<int>(str.length()) * arg);
}

void Format(std::string& str) {
  for (std::size_t i = 0; i < str.length(); ++i) {
    if (str[i] == '.') {
      str.insert(str.begin() + i + 1, '\n');
    }
  }
}

int main() {
// for Windows
//  SetConsoleCP(1251);
//  SetConsoleOutputCP(1251);
  std::ios::sync_with_stdio(false);

  int end;
  std::string str;
  std::string output_choose =
      "Where to output the result: file or console (press f or c)?\nIt's recommended to use a file because console may not have enough buffer size to display all text";

  std::cout << "encode or decode?\n";
  getline(std::cin, str);
  std::cout << "Input your string\n";

  if (str == "de") {
    getline(std::cin, str);
    std::vector<int> code(str.length());
    std::vector<int> bin(str.length() * 6);

    DecodeTable(str, code);
    end = TransferToBin(str, code, bin, 6);
    TransferToAscii(end, code, bin, 8);

    std::cout << output_choose << "\n";

    getline(std::cin, str);
    if (str == "f") {
      std::fstream fout("decryption.txt", std::ios::out);
      str.clear();
      for (int i = 0; i < end >> 3; ++i) {
        str += static_cast<char>(code[i]);
      }
      Format(str);
      fout << str;
      fout.close();
    } else if (str == "c") {
      str.clear();
      for (int i = 0; i < end >> 3; ++i) {
        str += static_cast<char>(code[i]);
      }
      Format(str);
      std::cout << str << "\n";
    }
  } else if (str == "en") {
    getline(std::cin, str);
    std::vector<int> code(str.length() << 1, 0);
    std::vector<int> bin(str.length() << 3);

    DecodeTable(str, code, false);
    end = TransferToBin(str, code, bin, 8);

    std::string res;
    TransferToBase64(end, code, bin, 6, res);

    if (end % 6) {
      for (int i = 1; i < 6 - (end % 6); i <<= 1) {
        res += "=";
      }
    }

    std::cout << output_choose << "\n";
    getline(std::cin, str);

    if (str == "f") {
      std::fstream fout("decryption.txt", std::ios::out);
      fout << res;
      fout.close();
    } else if (str == "c") {
      std::cout << res << "\n";
    }
  }
  std::cout << "It's done!" << '\n' << "You could close this app" << "\n";
  std::cin.get();
  std::cin.get();
}