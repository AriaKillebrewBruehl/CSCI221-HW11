// Aria Killebrew Bruehl, HW11 part 2 (with help from the lovely drop in tutors!)
#include "Gate.hh"
#include "Circuit.hh"
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include <algorithm>
#include <math.h>
#include <string>

//
// class Circuit
//
// A circuit is a collection of circuit components, each an instance
// of a subclass of Gate. It is constructed by providing a file name,
// one of a text file that contains the description of its components,
// along with how they should be wired together.
//
// A few sample circuit files are included in this folder.
//
//    xor.lsc: a two input XOR circuit
//
//    mux4.lsc: a 6 input circuit that behaves like a 4:1 MUX
//
//    inc.lsc: next state logic for a 3-bit cyclic counter
//
//    dec2_4: a 2:4 decoder circuit
//
// The Circuit components are defined in Gate.cc and Gate.hh.
//
// There are Input components. These are ones that can be set to
// true/false with the method setValue. Then there are And, Or, and
// Not gates. These take their input(s) from other conponents, and
// compute an output value based on their logical behavior. Finally,
// there are Output components, which simply reflect the value of
// their one input.
//
// There are four major methods for computing with a circuit.
//
//    reset: called before we want to compute the outputs
//           of a circuit from its inputs
//
//    setInputsFrom: called before we start that computation.
//           It sets each of the Input components to true/false
//           based on a vector<bool> with inputSize() elements.
//
//    evaluate: called to compute the outputs of a circuit from
//           its set inputs.
//
//    readOutputsInto: called with a vector<bool>. The truth
//           values of each of the outputs are read into this
//           vector.
//
// To compute with a Circuit `c`, follow these four steps.
//
//     1. Call `c.reset()`.
//     2. Call `c.setInputsFrom(ibv)` with some vector `ibv`.
//        Make sure its length is `c.inputSize()`.  This gets
//        the inputs ready.
//     3. Call `c.evaluate()`. This should simulate the 
//        circuit and ultimately set the outputs.
//     4. Call `c.readOutputsInto(obv)` with some vector `obv`.
//        Make sure its length is `c.outputSize()`. This reads
//        the outputs into that vector.
//
// The method `evaluate` is not written. That is part of the
// assignment.
//
// One test of `evaluate` is the method `outputTruthTable`.  This code
// should generate a series of text lines that give the behavior of
// the circuit with all possible inputs. It should run steps 1-4 on
// each input possible vector, and output the result.
// 
// See the `.out` files in this folder for the expected output
// of the truth table of each of the sample circuits.
//
// The method `outputTruthTable` is not written. That is part of the
// assignment.
//


// constructor
//
// Builds a Circuit object by processing an input text file.
//
Circuit::Circuit(std::string filename) :
  inputs {},
  outputs {},
  components {} 
{
  std::ifstream fs;

  // Open the file.
  //
  fs.open(filename);

  if (fs) {
    // Read it if no errors occurred.
    //
    std::cerr << "Reading '" << filename << "'...\n";
    _readFrom(fs);
  }

  // Close it.
  //
  fs.close();
}

// _readFrom
//
// Does the bulk of the work of the constructor.
//
void Circuit::_readFrom(std::ifstream& fs) {

  // Read the number of gate components.
  int numGates;
  fs >> numGates;

  // Read each gate component entry, add it to the circuit.
  //
  // This involves several steps:
  //  1. create a shared_ptr to the new gate.
  //  2. If input/output add it to the appropriate vector.
  //  3. Add it to a name->gate map so that we can wire it up
  //     based on the compoent's name.
  //
  for (int gi=0; gi<numGates; gi++) {
    std::string gateType;
    std::string gateName;

    // Read the gate's type.
    // I.e. one of "and", "or", "not", "input", or "output".
    fs >> gateType;

    // Read the gate's identifying name (for wiring later).
    fs >> gateName;

    std::cerr << "Adding " << gateType << " named '" << gateName << "'.\n";

    // Make a new gate; add it to the data structures.
    //
    std::shared_ptr<Gate> gate;
    if (gateType == "and") {
      gate = std::shared_ptr<And> { new And {gateName} };
    } else if (gateType == "or") {
      gate = std::shared_ptr<Or> {new Or {gateName} };
    } else if (gateType == "not") {
      gate = std::shared_ptr<Not> {new Not {gateName} };
    } else if (gateType == "input") {
      // If an input, also add it to the input list.
      std::shared_ptr<Input> igate = std::shared_ptr<Input> {new Input {gateName} };
      gate = igate;
      inputs.push_back(igate);
    } else if (gateType == "output") {
      // If an output, also add it to the output list.
      std::shared_ptr<Output> ogate = std::shared_ptr<Output> {new Output {gateName} };
      gate = ogate;
      outputs.push_back(ogate);
    }
    // Add it to the circuit's name->gate map so that we can wire things.
    components.insert(std::pair<std::string,std::shared_ptr<Gate>> {gateName,gate});
  }

  // Read each wiring line, linking up circuit components.
  //
  int numWires;
  fs >> numWires;
  
  for (int wi=0; wi<numWires; wi++) {
    std::string wire_tag, from, to;
    fs >> wire_tag; // read "wire"
    fs >> from;
    fs >> to;
    std::cerr << "Wire from '" << from << "' to '" << to << "'.\n";
    std::shared_ptr<Gate> from_g = components[from];
    std::shared_ptr<Gate> to_g = components[to];
    from_g->addOutput(to_g);
    to_g->addInput(from_g);
  }
}

//
// reset
// 
// Prepares each gate so that the circuit can be evaluated.
//
void Circuit::reset() {
  for (auto name_gate : components) {
    name_gate.second->reset();
  }
}

//
// inputSize
//
// Returns the number of inputs into the circuit.
//
int Circuit::inputSize() const {
  return inputs.size();
}

//
// outputSize
//
// Returns the number of outputs from a circuit.
//
int Circuit::outputSize() const {
  return outputs.size();
}

//
// setInputsFrom
//
// Sets each of the Input components according to a boolean
// vector. The order of the vector corresponds to the order of the
// circuit's inputs (the same order that they occur in the file).
//
void Circuit::setInputsFrom(const std::vector<bool>& vals) {
  for (int i = 0; i < inputSize(); i++) {
    inputs[i]->setValue(vals[i]);
  }
}
 
//
// readInputsInto
//
// Reads each of the Outputs components to set the contents of a given
// boolean vector. The order of the vector corresponds to the order of
// the circuit's oututs (the same order that they occur in the file).
//
void Circuit::readOutputsInto(std::vector<bool>& vals) const {
  for (int i = 0; i < outputSize(); i++) {
    vals[i] = outputs[i]->outputValue();
  }
}

//
// evaluate
//
// Traverses the circuit's components in such a way that the outputs
// get computed based on the values set for each of the inputs.
//
// WRITE THIS CODE!
//
void Circuit::evaluate() {
  // create vector of gates
  std::vector<std::shared_ptr<Gate>> v;
  for( std::map<std::string, std::shared_ptr<Gate>>::iterator it = components.begin(); it != components.end(); ++it ) {
        v.push_back( it->second );
  }
  // while there are still gates to evaluate
  while (!v.empty()) {
    for (int i = 0; i < v.size(); i++) {
      // pick a gate that is ready to be evaluated
      if (v[i]->allInputsReady()) {
        v[i]->evaluate();
        // remove evaluated gate from vector
        v.erase(v.begin() + i);
      }
    }
  }
}

//
// outputTruthTable
//
// Outputs the full series of rows for the truth table of this
// circuit by repeatedly calling reset/setInputsFrom/evaluate
// and ouputting the results read by readOutputsInto.
//
//
// WRITE THIS CODE!
//
// heading()  funct, outputs heading of truth table 
void Circuit::heading() {
  for (std::shared_ptr<Input> i : inputs) {
    std::cout << " " << i->name() << " |";
  }
  for (std::shared_ptr<Output> o : outputs) {
    std::cout << "| " << o->name() << " ";
  }
  std::cout<<std::endl;
  for (std::shared_ptr<Input> i : inputs) {
    // make filler string
    std::string f;
    for (int j = 0; j < i->name().length(); j++ ){
      f += "-";
    }
    std::cout << "-" << f << "-+";
  }
  for (std::shared_ptr<Output> o : outputs) {
    // make filler string
    std::string f;
    for (int j = 0; j < o->name().length(); j++ ){
      f += "-";
    }
    std::cout << "+-" << f << "-";
  }
  std::cout<<std::endl;
}
// getVector(index, numInputs) funct returns vecotr of inputs for index in circuit w NumInputs
std::vector<bool> getVector(int index, int numInputs) {
  std::vector<bool> values; 
  for(int i = 0; i <numInputs; i++){
    // create a "mask" int of 1 bit shiffted left by (numInputs-i-1)
    const int mask = 1 << (numInputs - i - 1);
    // compare index & mask to get input value at index 
    values.push_back(index & mask);
  }
  return values;
}
// outputLine(ibv, obv) funct uses ibv and obv to fill in table
void Circuit::outputLine(std::vector<bool> ibv, std::vector<bool> obv){
  // output each input with separators 
  for ( int i = 0; i < ibv.size(); i++) {
    // make filler string
    std::string f;
    for (int j = 0; j < inputs[i]->name().length() - 1; j++ ){
      f += " ";
    }
    std::cout << " " << f << ibv[i] << " |";
  }
  // output each output with separators
  for (int i = 0; i < obv.size(); i++) {
    // make filler string
    std::string f;
    for (int j = 0; j < outputs[i]->name().length() - 1; j++ ){
      f += " ";
    }
    std::cout << "| " << f << obv[i] << " ";
  }
  
  std::cout<<std::endl;
}
void Circuit::outputTruthTable() {
  heading();
  // loop through process of resetting, creating input vector, evaluating, and making outputs 
  for (int i = 0; i < pow(2, inputs.size()); i++) {
    reset();
    std::vector<bool> ibv;
    ibv.resize(inputSize());
    ibv = getVector(i, inputSize());
    setInputsFrom(ibv);
    evaluate();
    std::vector<bool> obv;
    obv.resize(outputSize());
    readOutputsInto(obv);
    outputLine(ibv, obv);
  }
}