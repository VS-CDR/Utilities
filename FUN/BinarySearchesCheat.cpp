#include <compare>
#include <numeric>
#include <vector>

template <std::three_way_comparable T>
ssize_t lower_bound(std::vector<T> arr, T val) {
  ssize_t l = -1, r = std::ssize(arr);
  while (l + 1 < r) {
    auto mid = std::midpoint(l, r);
    (arr[mid] < val ? l : r) = mid;
  }
  return r;
}

template <std::three_way_comparable T>
ssize_t upper_bound(std::vector<T> arr, T val) {
  ssize_t l = -1, r = std::ssize(arr);
  while (l + 1 < r) {
    auto mid = std::midpoint(l, r);
    (arr[mid] <= val ? l : r) = mid;
  }
  return r;
}
