#ifndef _GATE_HH
#define _GATE_HH

#include <memory>
#include <vector>
#include <string>

class Gate {
protected:

  std::string _name;
  std::vector<std::weak_ptr<Gate>> _outputs;
  std::vector<std::shared_ptr<Gate>> _inputs;

  bool _outputValue;
  bool _outputIsReady;
  int _numberOfInputsReady;
  virtual void _signalReady();

public:
  Gate(std::string);
  std::string name() const;
  virtual void addInput(std::shared_ptr<Gate>);
  virtual void addOutput(std::shared_ptr<Gate>);

  virtual void reset();
  virtual bool outputValue() const;
  virtual bool outputIsReady() const;
  virtual bool allInputsReady() const;

  virtual void evaluate() = 0;
};

class And : public Gate {
public:
  And(std::string);
  void evaluate();
};

class Or : public Gate {
public:
  Or(std::string);
  void evaluate();
};

class Not : public Gate {
public:
  Not(std::string);
  void evaluate();
};

class Input : public Gate {
private:
  bool _setting;
public:
  Input(std::string);
  std::string name();
  void setValue(bool);
  void evaluate();
};
  
class Output : public Gate {
public:
  Output(std::string);
  std::string name ();
  void evaluate();
};

#endif
