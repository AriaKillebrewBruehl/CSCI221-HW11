#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>

int main() {

  std::vector<int> v {10,3,1,19};
  int sum = 0;
  std::for_each(v.begin(),v.end(),[&sum](int e)mutable ->void { sum +=e; } );
  std::cout << sum << std::endl;

}
