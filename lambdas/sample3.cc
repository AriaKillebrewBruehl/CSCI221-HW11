#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>


class Summer {
private: 
  int& _sum;
public:
  Summer(int& s) : _sum{s} {}
  void operator()(int e) { _sum += e; }
};

int main() {

  std::vector<int> v {10,3,1,19};
  int sum = 0;
  std::for_each(v.begin(),v.end(),Summer {sum});
  std::cout << sum << std::endl;

}
