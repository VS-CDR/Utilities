/* Interesting story
 * Данную задачу мне дали на финальном собеседовании в Яндекс, но есть нюанс.
 * Интервьюер ждал, что я напишу ее проще, из-за чего не стал дожидаться пока я допишу это решение.
 * Кратко условие: есть поле судоку 9x9 и нужно проверить каждый квадрат на то, что символы в нем уникальны.
*/

#include <array>
#include <iostream>
#include <vector>

bool CheckSquare(const std::vector<std::vector<int>>& field,
                 int start_row, int start_col) {
  std::array<int, 10> cnt{};
  for (int i = start_row; i < start_row + 3; ++i) {
    for (int j = start_col; j < start_col + 3; ++j) {
      if (++cnt[field[i][j]] > 1) {
        return false;
      }
    }
  }
  return true;
}

bool CheckSudokuField(const std::vector<std::vector<int>>& field) {
  int start_row = 0;
  int start_col = 0;

  while (start_row + 3 <= 9) {

    if (!CheckSquare(field, start_row, start_col)) {
      return false;
    }

    start_col += 3;

    if (start_col == 9) {
      start_row += 3;
      start_col = 0;
    }
  }

  return true;
}

int main(int argc, char* argv[]) {
  std::vector<std::vector<int>> field = {
      {1, 2, 3,   4, 5, 6,   7, 8, 9},
      {7, 8, 9,   1, 2, 3,   1, 3, 2},
      {4, 5, 6,   7, 8, 9,   4, 5, 6},

      {1, 2, 3,   4, 5, 6,   7, 8, 9},
      {7, 8, 9,   1, 2, 3,   1, 3, 2},
      {4, 5, 6,   7, 8, 9,   4, 5, 6},

      {1, 2, 3,   4, 5, 6,   7, 8, 9},
      {7, 8, 9,   1, 2, 3,   1, 3, 2},
      {4, 5, 6,   7, 8, 9,   4, 5, 6},
  };

  std::cout << (CheckSudokuField(field) ? "Ok" : "Invalid");
}