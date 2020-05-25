// Aria Killebrew Bruehl, HW11 ex 3 (with help from drop in tutors!)
#include <iostream>
#include <vector>
#include <functional>

int main() {
  // squaring function
  std::function<int(int)> sq = [](int i)->int {return i *= i; };
  // add 10 function
  std::function<int(int)> add10 = [](int i) ->int {return i += 10;};
  // raise to power of three function
  std::function<int(int)> pow3 = [](int i) ->int {return i *= i * i;};
  // apply_twice function takes function, applies it twice and return integer
  auto apply_twice = [](std::function<int(int)>& f, int x)->int {
     return f(f(x));
  };
  // tests
  std::cout << apply_twice(sq, 3) << std::endl;
  std::cout << apply_twice(add10, 5) << std::endl;
  std::cout << apply_twice(pow3, 2) << std::endl;
}
