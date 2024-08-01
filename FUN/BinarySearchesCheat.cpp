#include <numeric>
#include <vector>

ssize_t lower_bound(std::vector<int> arr, int val) {
  ssize_t l = -1, r = std::ssize(arr);
  while (l + 1 < r) {
    auto mid = std::midpoint(l, r);
    (arr[mid] < val ? l : r) = mid;
  }
  return r;
}

ssize_t upper_bound(std::vector<int> arr, int val) {
  ssize_t l = -1, r = std::ssize(arr);
  while (l + 1 < r) {
    auto mid = std::midpoint(l, r);
    (arr[mid] <= val ? l : r) = mid;
  }
  return r;
}
