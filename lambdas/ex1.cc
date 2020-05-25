// Aria Killebrew Bruehl HW11, exercise 1
#include <iostream>
#include <functional>

int main() {
  {
    // takes int, returns its square
    std::function<int(int)> sq = [](int i)->int {return i *= i; };
    std::cout << sq(9) << std::endl;
    std::cout << sq(3) << std::endl;
  }
}
