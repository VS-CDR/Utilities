long lower_bound(std::vector<int> arr, int val) {
  long l = -1, r = std::ssize(arr);
  while (l + 1 < r) {
    auto mid = std::midpoint(l, r);
    (arr[mid] < val ? l : r) = mid;
  }
  return r;
}

long upper_bound(std::vector<int> arr, int val) {
  long l = -1, r = std::ssize(arr);
  while (l + 1 < r) {
    auto mid = std::midpoint(l, r);
    (arr[mid] <= val ? l : r) = mid;
  }
  return r;
}
