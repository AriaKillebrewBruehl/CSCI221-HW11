#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include "Gate.hh"

Gate::Gate(std::string s) :
  _name {s},
  _outputs {},
  _inputs {} 
{
  reset();
}
And::And(std::string s) : Gate {s} { }
Or::Or(std::string s) : Gate {s} { }
Not::Not(std::string s) : Gate {s} { }
Input::Input(std::string s) : Gate {s} { }
Output::Output(std::string s) : Gate {s} { }
      
void Gate::reset() {
  _outputIsReady = false;
  _numberOfInputsReady = 0;
}

void Gate::_signalReady() {
  _outputIsReady = true;
  for (auto o:_outputs) {
    std::shared_ptr<Gate> sp = o.lock();
    sp->_numberOfInputsReady++;
  }
}

std::string Gate::name() const {
  return _name;
}

bool Gate::outputValue() const {
  return _outputValue;
}

bool Gate::outputIsReady() const {
  return _outputIsReady;
}

bool Gate::allInputsReady() const {
  return (_numberOfInputsReady == _inputs.size());
}

void Gate::addInput(std::shared_ptr<Gate> other) {
  _inputs.push_back(other);
}

void Gate::addOutput(std::shared_ptr<Gate> other) {
  _outputs.push_back(other);
}
    
void And::evaluate() {
  _outputValue = true;
  for (auto i:_inputs) {
    if (!i->outputValue()) {
      _outputValue = false;
      break;
    }
  }
  _signalReady();
}
  
void Or::evaluate() {
  _outputValue = false;
  for (auto i:_inputs) {
    if (i->outputValue()) {
      _outputValue = true;
    }
  }
  _signalReady();
}

void Not::evaluate() {
  _outputValue = !_inputs[0]->outputValue();
  _outputIsReady = true;
  _signalReady();
}

std::string Input::name() {
  return _name;
}
void Input::setValue(bool v) {
  _setting = v;
}

std::string Output::name() {
  return _name;
}
void Input::evaluate() {
  _outputValue = _setting;
  _signalReady();
}

void Output::evaluate() {
  _outputValue = _inputs[0]->outputValue();
  _signalReady();
}

