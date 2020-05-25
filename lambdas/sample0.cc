#include <vector>
#include <functional>
#include <iostream>

class Foo {
private:
  int toAdd;
public:
  Foo(int dx) : toAdd {dx} {}
  int operator()(int x) const { return x+toAdd; }
};

std::function<int(int)> makeAdder(int dx) {
  return [dx](int x) { return x+dx; };
}

std::function<void()> makePrinter(int x) {
  return [x]() { std::cout << x << "\n"; };
}
  
int main() {

  std::vector<int> v {0,1,4,9,16,25,36,49,64};
  
  std::function<bool(int)> isEven = [](int i)->bool { return (i % 2) == 0; };
  std::function<int(int)> successor = [](int x)->int { return x+1; };
  std::function<int(int)> add10 = makeAdder(10);
  std::function<void()> print5 = makePrinter(5);
  std::function<void(int)> printv = [&v](int index)->void { std::cout << v[index] << "\n"; };
  std::function<void(int)> sqrv = [&v](int index)->void { v[index] *= v[index]; };

  std::cout << "isEven(5) = " << isEven(5) << std::endl;
  std::cout << "successor(5) = " << successor(5) << std::endl;
  std::cout << "add10(5) = " << add10(5) << std::endl;
  std::cout << "print5(): " << std::endl;
  print5();

  std::cout << "printv(2): " << std::endl;
  printv(2);
  std::cout << "printv(3): " << std::endl;
  printv(3);
  std::cout << "printv(4): " << std::endl;
  printv(4);

  std::cout << "sqrv(4). " << std::endl;
  sqrv(4);
  std::cout << "printv(4): " << std::endl;
  printv(4);
  
  int sum = 10000;
  std::for_each(v.begin(),v.end(),[&sum](int e)mutable ->void { sum +=e; } );
  std::cout << "foreach(...i...,sum += v[i]). " << std::endl;
  std::cout << "sum = " << sum << std::endl;

  std::cout << "(Foo {10})(5) = " << (Foo {10})(5) << std::endl;

}



