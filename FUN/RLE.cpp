template<typename T>
long inplaceRLE(std::vector<T>& str) {
  long ins_pos = 0;
  for (int i = 0; i < std::ssize(str); ++i) {
    auto ch = str[i];
    int cnt = 1;
    for (; i + 1 < std::ssize(str) && ch == str[i + 1]; ++i) {
      ++cnt;
    }
    if (cnt > 1) {
      auto count = std::to_string(cnt);
      std::ranges::copy(count, str.begin() + ins_pos);
      str[ins_pos + std::ssize(count)] = ch;
      ins_pos += std::ssize(count);
    } else {
      str[ins_pos] = ch;
    }
    ++ins_pos;
  }
  return ins_pos;
}

std::string RLE(std::string_view str) {
  std::string res;
  for (int i = 0; i < std::ssize(str); ++i) {
    auto ch = str[i];
    int cnt = 1;
    for (; i + 1 < std::ssize(str) && ch == str[i + 1]; ++i) {
      ++cnt;
    }
    res += ch;
    res += std::to_string(cnt);
  }
  return res;
}
