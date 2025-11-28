#include <algorithm>
#include <iostream>
#include <ranges>

int main() {

  int entered_number;
  std::cout << "Please enter a number to check its divisibility: ";
  std::cin >> entered_number;

  auto lam = [&entered_number](int &&arg) {
    auto range = std::views::iota(1, entered_number);
    return std::count_if(range.begin(), range.end() + 1,
                         [arg](int i) { return arg % i == 0; });
  };

  std::cout << entered_number << " has " << lam(std::move(entered_number))
            << " divisers\n";

  return 0;
}
