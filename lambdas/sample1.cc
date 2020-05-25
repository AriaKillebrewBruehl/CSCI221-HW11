#include <iostream>
#include <vector>
#include <functional>

int main() {
  {
    std::vector<int> v {10,3,1,19};
    int i = 3;
    std::function<bool(int)> f = [&v,i](int j)->bool {return v[i]==v[j];};
    i--;
    std::cout << f(2) << std::endl;
    std::cout << f(3) << std::endl;
  }
  {
    std::vector<int> v {10,3,1,19};
    int i = 3;
    std::function<bool(int)> f = [&v,&i](int j)->bool {return v[i]==v[j];};
    i--;
    std::cout << f(2) << std::endl;
    std::cout << f(3) << std::endl;
  }

}
