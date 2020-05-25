#include "Circuit.hh"
#include <iostream>

//
// logicSim.cc
//
// A circuit simulator for combinatorial circuits specified by
// circuit text files.
//
// Compile with:
//        g++ -o logicSim -std=c++17 Gate.cc Circuit.cc logicSim.cc
//
// Usage: 
//        ./logicSim circuitfile.lsc
//
// Reads the circuit file, outputs its truth table.
//
int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "Error! Need to supply the name of a circuit file.\n";
    std::cout << "usage: " << argv[0] << " [circuit file name]\n";
    return -1;
  }
  Circuit c {argv[1]};
  c.outputTruthTable();
}
