template<typename T>  //index version
long InplaceRLE(std::vector<T>& str) {
  long ins_pos = 0;
  for (int i = 0; i < std::ssize(str); ++i, ++ins_pos) {
    int cnt = 1;
    for (; i + 1 < std::ssize(str) && str[i] == str[i + 1]; ++i) { ++cnt; }
    if (cnt > 1) {
      auto count = std::to_string(cnt);
      std::ranges::copy(count, str.begin() + ins_pos);
      ins_pos += std::ssize(count);
    }
    str[ins_pos] = str[i];
  }
  return ins_pos;
}

template<typename T>  //iterators version
auto InplaceRLE(std::vector<T>& str) {
  auto ins_pos = str.begin();
  for (auto it = str.begin(); it != str.end(); ++it, ++ins_pos) {
    int cnt = 1;
    for (; it + 1 != str.end() && *it == *(it + 1); ++it) { ++cnt; }
    if (cnt > 1) {
      auto count = std::to_string(cnt);
      std::ranges::copy(count, ins_pos);
      ins_pos += std::ssize(count);
    }
    *ins_pos = *it;
  }
  return ins_pos;
}

std::string RLE(std::string_view str) {  //index version
  std::string res;
  for (int i = 0; i < std::ssize(str); ++i) {
    res += str[i];
    int cnt = 1;
    for (auto ch = str[i]; i + 1 < std::ssize(str) && ch == str[i + 1]; ++i) {
      ++cnt;
    }
    res += std::to_string(cnt);
  }
  return res;
}

std::string RLE(std::string_view str) {  //iterators version
  std::string res;
  for (const auto *it = str.begin(); it != str.end(); ++it) {
    res += *it;
    int cnt = 1;
    for (auto ch = *it; it + 1 != str.end() && ch == *(it + 1); ++it) {
      ++cnt;
    }
    res += std::to_string(cnt);
  }
  return res;
}
