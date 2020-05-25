// Aria Killebrew Bruehl HW11, exercise 2
#include <iostream>
#include <vector>
#include <functional>
#include <math.h>

int main() {
  std::vector<int> digits = {1, 0, 1, 1, 1};
  int base = 2;
  // captures base, takes  vector, returns int from interpretation of bits in vector
  std::function<void()> f = [digits, base]()->void {
      int sum = 0;
      for (int i = 0; i < digits.size(); i++) {
          sum += digits[i] * pow(base, digits.size() - (i + 1));
      }
      std::cout << sum << std::endl;
  };
  f();
}
