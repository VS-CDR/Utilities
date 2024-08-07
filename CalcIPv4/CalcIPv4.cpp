#include <array>
#include <algorithm>
#include <charconv>
#include <iostream>
#include "../fastmath.hpp"

class InputExc : private std::exception {
 public:
  [[nodiscard]] const char* what() const noexcept override {
    return "Wrong Input";
  }
};

constexpr int kNumOfQuadrants = 4;
constexpr int kMaxValue = 256;

int Transfer(int byte, int radix) {
  int res = 0;
  for (int k = 1; byte != 0; k *= 10, byte /= radix) {
    res += (byte % radix) * k;
  }
  return res;
}

int FormOut(int out) {
  int radix = 0;
  for (int tmp = out; tmp > 0; tmp /= 10) {
    ++radix;
  }
  for (int i = radix; i < 8; ++i) {
    std::cout << 0;
  }
  return out;
}

void Print(const int* byte) {
  for (int i = 0; i < kNumOfQuadrants; ++i) {
    std::cout << byte[i];
    if (i != 3) {
      std::cout << '.';
    }
  }
}

void Info(const int* byte) {
  std::cout << "In BIN(2): ";
  for (int i = 0; i < kNumOfQuadrants; ++i) {
    std::cout << FormOut(Transfer(byte[i], 2));
    if (i != 3) {
      std::cout << '.';
    }
  }
  std::cout.unsetf(std::ios::dec);
  std::cout.setf(std::ios::hex);
  std::cout << '\n' << "In HEX(16): ";
  Print(byte);
  std::cout.unsetf(std::ios::hex);
  std::cout.setf(std::ios::oct);
  std::cout << '\n' << "In OCT(8): ";
  Print(byte);
  std::cout.unsetf(std::ios::oct);
  std::cout.setf(std::ios::dec);
  std::cout << std::endl;
}

void PrintCroppedAddress(int begin, int end, const std::array<int, 32>& arr);
void CIDR(const int* byte, int cidr) {
  std::array<int, 32> ip{};

  for (int i = 0; i < kNumOfQuadrants; ++i) {
    for (int j = i * 8; j < (i + 1) * 8; ++j) {
      int octet = Transfer(byte[i], 2) % FastPow(10, 8 - (j % 8));
      int div = FastPow(10, 8 - 1 - (j % 8));
      ip[j] = octet / div;
    }
  }

  std::cout << '\n' << "Net ID = ";
  PrintCroppedAddress(0, cidr, ip);
  std::cout << '\n' << "Host ID = ";
  PrintCroppedAddress(cidr, 32, ip);
}

void PrintCroppedAddress(int begin, int end, const std::array<int, 32>& arr) {
  for (int i = begin; i < end; ++i) {
    if (i % 8 == 0 && i != 0) {
      std::cout << '.';
    }
    std::cout << arr[i];
  }
}

void ClassOfIP(const int* byte) {
  constexpr std::string_view kCommonAddressInfo("!Common address!");
  constexpr std::string_view kUniqueAddressInfo("Unique address");
  if (byte[0] < 128) {
    std::cout << (byte[0] == 10 ? kCommonAddressInfo : kUniqueAddressInfo)
              << '\n' << "A class:"
              << '\n' << "Net ID = "
              << FormOut(Transfer(byte[0], 2))
              << '\n' << "Host ID = ";
    for (int i = 1; i < kNumOfQuadrants; ++i) {
      std::cout << FormOut(Transfer(byte[i], 2));
      if (i != kNumOfQuadrants - 1) { std::cout << '.'; }
    }
  } else if (byte[0] < 192) {
    std::cout << (byte[0] == 172 && byte[1] >= 16 && byte[1] <= 31
                  ? kCommonAddressInfo : kUniqueAddressInfo)
              << '\n' << "B class:"
              << '\n' << "Net ID = " << FormOut(Transfer(byte[0], 2))
              << '.' << FormOut(Transfer(byte[1], 2))
              << '\n' << "Host ID = " << FormOut(Transfer(byte[2], 2))
              << '.' << FormOut(Transfer(byte[3], 2));
  } else if (byte[0] < 224) {
    std::cout << (byte[0] == 192 && byte[1] == 168 ? kCommonAddressInfo : kUniqueAddressInfo)
              << '\n' << "C class:"
              << '\n' << "Net ID = ";
    for (int i = 0; i < kNumOfQuadrants - 1; ++i) {
      std::cout << FormOut(Transfer(byte[i], 2));
      if (i != 2) { std::cout << '.'; }
    }
    std::cout << '\n' << "Host ID = " << FormOut(Transfer(byte[3], 2));
  } else {
    std::cout << "D/E class";
  }
}

void InputAndProcessData(std::array<int, kNumOfQuadrants>& byte) {
  std::cout << "Input ip address: ";
  std::string input;
  std::cin >> input;

  auto begin_quadrant = input.begin();
  auto end_quadrant = std::ranges::find(input, '.');

  for (int i = 0; i < kNumOfQuadrants; ++i) {
    std::from_chars(begin_quadrant.base(), end_quadrant.base(), byte[i]);
    begin_quadrant = std::min(end_quadrant + 1, input.end());
    end_quadrant = std::find(begin_quadrant, input.end(), '.');
    if (byte[i] < 0 || byte[i] >= kMaxValue) {
      throw InputExc();
    }
  }
}

void ChooseOptions(const std::array<int, kNumOfQuadrants>& byte);

int main() {
  std::ios::sync_with_stdio(false);
  std::array<int, kNumOfQuadrants> byte{};
  InputAndProcessData(byte);
  Info(byte.data());
  ChooseOptions(byte);
}

void ChooseOptions(const std::array<int, kNumOfQuadrants>& byte) {
  std::cout << "CIDR y/n : ";
  char cmd(' ');
  while (cmd != 'y' && cmd != 'n') {
    std::cin >> cmd;
    if (cmd == 'y') {
      std::cout << "Input CIDR: /";
      int cidr;
      std::cin >> cidr;
      CIDR(byte.data(), cidr);
    } else if (cmd == 'n') {
      ClassOfIP(byte.data());
    } else {
      std::cout << "Invalid input! Choose between (y/n): ";
    }
  }
}
