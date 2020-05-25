// Aria Killebrew Bruehl, HW11 ex 4 (with help from drop in tutors :) )
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
  // twice takes function and returns new function which applys input function twice 
  const auto twice = [](std::function<int(int)>& f) {
    return [f](int x)->int {
        return f(f(x));
    };
  };
// testing twice 
 std::function<int(int)> quad = twice(sq);
 std::cout << quad(4) << std::endl;
 std::function<int(int)> add20 = twice(add10);
 std::cout << add20(9) << std::endl;
 std::function<int(int)> pow9 = twice(pow3);
 std::cout << pow9(3) << std::endl;
}
