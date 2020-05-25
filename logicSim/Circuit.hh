#ifndef _CIRCUIT_HH
#define _CIRCUIT_HH

#include <fstream>
#include <vector>
#include <map>
#include <memory>
#include "Gate.hh"

class Circuit {
private:
  std::vector<std::shared_ptr<Input>> inputs;
  std::vector<std::shared_ptr<Output>> outputs;
  std::map<std::string,std::shared_ptr<Gate>> components;

  void _readFrom(std::ifstream&);

public:
  Circuit(std::string);
  
  int inputSize() const;
  int outputSize() const;

  void setInputsFrom(const std::vector<bool>&);
  void readOutputsInto(std::vector<bool>&) const;

  void reset();
  void evaluate();
  void heading();
  void outputLine(std::vector<bool>, std::vector<bool>);
  void outputTruthTable();
};

#endif
