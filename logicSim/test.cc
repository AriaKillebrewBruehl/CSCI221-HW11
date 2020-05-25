#include <iostream>
#include <vector>

std::vector<bool> getVector(int index, int numInputs) {
  std::vector<bool> values;
  for(int i = 0; i <numInputs; i++){
    const int mask = 1 << (numInputs - i - 1);
    values.push_back(index & mask);
  }
  return values;
  // give a line numbers and  numInputs, then gives back specific set of bools 
  // start at 1 if line num is in second half add 1 else add zero keep dup until get correct numinputs 
}

int main(void) {

    std::vector<bool> v = getVector(3, 4);
    for (bool i : v) {
        std::cout << i << " ";
    }
    std::cout <<std::endl;
    return 0;
}