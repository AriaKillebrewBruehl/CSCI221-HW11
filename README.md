# Homework 11: lambdas and cicruits in C++

*Part 1 due: Monday, May 11th, 3pm*  
*Part 2 due: Thursday, May 14th, 3pm*  

This homework comes in two parts:

* `lambdas`: a series of exercises of varying difficulty that
allow you to practice using the notation for lambdas in C++.
It comes with several samples that you can work from to come
up with your solutions.

* `logicSim`: a single program assignment where you are to finish the
code of a partially implemented program that simulates circuits that
don't have feedback. A few pieces of code need to be completed for it
to work.

---

# Part 1: Lambdas

Here I ask you to practice writing functions using the `functional` component
of the C++ Standard Template Library, particularly its definition of a "lambda"
construct. In C++ you can express an anonymous function with the expression:

    [captures](parameters) -> return-type { body }
   
In the above, the `body` and the `parameters` look as they might in a
normal function definition. For example, this function computes the
integer whose digits are the two given integers:

    [](unsigned int tens, unsigned int ones) -> unsigned int { return tens*10+ones; }

Note that the `return-type` is just the type that would normally sit
at the start of a normal function definition. The `captures` is a list
of variables within the same scope of the lambda expression that are
used within the `body`. They can either be "captured" *by value* (i.e.
by *copy*), in which case only the variable name is listed, or *by
reference*, in which case the variable name is listed with an `&`
symbol at its front. As an example, consider the following:

    std::vector<int> v {10,3,1,19};
    int i = 3;
    std::function<bool(int)> f = [&v,i](int j) -> bool {return v[i]==v[j];};
    i--;
    std::cout << f(3) << std::endl;
    
This code will output 1 (for `true`) because `f` checks whether the value of `v` at
the index of its argument is the same as the value `v[3]`. This is because
`i` is passed by value, and so `v[i]` means `v[3]` at the time that `f` is
expressed. If the third line was instead:

    std::function<bool(int)> f = [&v,&i](int j) -> bool {return v[i]==v[j];};
    
then the output `f(3)` would instead be 0 (for `false`) because `f`
would be checking `v[2]` on the left side of the `==`. We would need
to call `f(2)` instead to get the result `true`.

Note the `std::function` type of the lambda in the declaration of `f` above.
In general, if a lambda takes a series of arguments of type `T1`, `T2`, up to
`Tk` and returns something of type `T`, then its type is 

    std::function<T(T1,T2,...,Tk)>
    
It is also possible for a lambda to "mutate" the state of variables
captured by value in its capture list by including the keyword
`mutable` before the arrow to the return type. For example,

    int sum = 0;
    std::function<int(int)> accum = 
      [sum](int v) mutable -> void {sum += v; return sum;});     
    std::cout << accum(7) << std::endl;
    std::cout << accum(30) << std::endl;
    std::cout << accum(500) << std::endl;

This will output three lines with `7`, `37`, and `537`. This is because,
though `sum` is copied, the `sum` variable for `accum` is allowed to
be changed because of the `mutable` keyword. The variable `sum` outside
the lambda stays at value `0`, but the one created by the capture changes. 

Here is another example of capture by reference:

    std::vector<int> v {10,3,1,19};
    int sum = 0;
    std::for_each(v.begin(),v.end(),[&sum](int e)->void{sum += e;});
    std::cout << sum << std::endl;

The third line above feeds a lambda to the `algorithm` package's
`for_each` construct. This calls our lambda on each element value `e`
of the vector `v`.  Since `sum` is captured by reference, the `+=`
operation bumps the value of `sum` by the value of each element of
`v`. This results in the output of `33` by the code.

This example highlights the common use of lambda expressions: they are a
convenient way of working with the `algorithms` component of the C++ STL.
Incidentally, the lambda expression isn't entirely necessary for using this
library. For example, we could have instead developed this class

    class Summer {
    private:
        int& _sum;
    public:
        Summer(int& s) : _sum{s} {}
        void operator()(int e) { _sum += e; } 
    };

which overloads the "call" operator. It has an "`int` reference" attribute.
With this class definition, we could have instead written

    std::for_each(v.begin(),v.end(),Summer {sum});
    
to obtain that same output of `sum` of `33`.

This class definition suggest the work done by C++ behind the scenes 
to develop every anonymous function object expressed, semantically speaking.

### Part 1 Exercises

For each of these, write testing code in a `main` inside a C++ source
file named `lambdas/exN.cc`, where `N` is the exercise number. The
code can be similar to the `lambdas/sampleN.cc` files I've included in
this repo.

**Note:** The last two of these are somewhat tricky, especially in getting 
the type information correct. They are BONUS exercises.

**Exercise 1.**  
Write and test the use of a lambda that computes the square of its integer
parameter. It shouldn't capture any variables. Show it working on several 
different parameter inputs. For example, when given `9`, it should return
`81`.

**Exercise 2.**   
Write a lambda that captures a local variable `int base` and a local
variable that's a vector `digits` of integers. That vector should have elements
that are only `0`s and `1`s. It should take no parameters.

When called, it should output to `std::cout` a line with a single
integer to the console. That value should be the integer formed from
that integer sequence interpreted as digits in that base.

For example, if the base is 2 and the digits vector was initialized with
the sequence

    {1,0,1,1,1}
    
that lambda should output `23` because, interpreting those as bits from
most- to least-significant, the lambda would calculate the number whose
binary sequence is `10111`.  (23 is just 16 + 4 + 2 + 1).

If the base were changed to 10, then calling that same lambda object should 
output `10111`. If we then called `pop_back` on the digits vector, then that
same lambda object should then output `1011`. If, after that, we changed the
base back to 2, then it would ouput `11`. (11 is just 8+2+1).

Note that the type of this lambda is `std::function<void()>`. It takes no
parameters and returns no result. It outputs to `std::cout` instead.

**BONUS: Exercise 3.**  
Write a lambda that takes a parameter `f` that is a function of type `int(int)`,
and a second parameter `int x`. Its capture list should be empty. When that 
lambda object is called, it should apply `f` twice to that given `x` and return
that result of type `int`.

Test it with the squaring function you wrote in Exercise 1. For example, 
if you wrote the squaring lambda by giving it the name `sqr`, and then wrote
this lambda giving it the name `apply_twice`, then

    std::cout << apply_twice(sqr,3) << std::endl;
    
should lead to output of `81` because the square of the square of 3 is 81.

You can also demonstrate it by writing an "add 10" function lambda. In that case,

    std::cout << apply_twice(add10,3) << std::endl;
    
should output `23`.

**BONUS Exercise 4.**  
Write a lambda that takes a parameter `f` that is a function of type `int(int)`.
It should take no other parameters. It should instead *give back a function* of
type `int(int)`. In other words, the body of the lambda should return a lambda!

Name this lambda object `twice`, and demonstrate it by creating several functions.
For example, you could redo the work of testing Exercise 3 and try out lines
like the following:

    std::function<int(int)> quad = twice(sqr);
    std::function<int(int)> add20 = twice(add10);

and then test these functions on several integer parameters.

**Hint:** You may have to pass arguments by reference, and you may have to capture
variables by reference. Think it through!

---

# Part 2: A Logic Simulator

The `logicSim` folder contains a nearly implemented combinatorial
circuit simulator named `logicSim`. It can be compiled with the
command:

    g++ -o logicSim -std=c++17 Gate.cc Circuit.cc logicSim.cc
    
When run with a specified filename, it reads a text file that
specifies the components and wiring of a circuit. When your complete
code is written, it should output the truth table for that
circuit. The folder `logicSim/circuits` contains several sample
circuit files

• `xor.lsc`: a two input XOR circuit  
• `mux4.lsc`: a 6 input circuit that behaves like a 4:1 MUX  
• `inc.lsc`: the next state logic for a 3-bit cyclic counter  
• `dec2_4.lsc`: a 2:4 decoder circuit  

For example, running the command

    ./logicSim circuits/xor.lsc

will read that file, add all the components to a circuit, wire them together,
and then attempt to print its truth table to the console. The same folder
contains sample output of my solution run on each of these circuit files.
For example the file `circuits/xor.out` is the result of my solution
running on `circuits/xor.lsc`.

The code, as it stands, defines a `main` in `LogicSim.cc`. That code builds an
instance of class `Circuit` from that input file, as defined in the files 
`Circuit.cc` and `Circuit.hh`. A circuit is just made up of a collection of
`Gate` objects, a combination of `Input`, `And`, `Or`, `Not`, and `Output`
objects, linked together, from the circuit's outputs, through the logic 
gates, back to the circuit's inputs, using `shared_ptr<Gate>` links.
There are also symmetric `weak_ptr<Gate>` links that go from the circuit's
inputs, through the logic gates, to the circuit's outputs. The components
are all defined in `Gate.cc` and `Gate.hh`.

The sample files do not specify circuits with "cycles", that is, no gate's
output eventually feeds its way back into itself as input. They are all 
*combinatorial* circuits because there areno cycles.

### Part 2 Assignment

There is more to tell you about the implementation, but first I'd like to
summarize the assignment. **You are to complete two methods of the circuit
class.**

• `Circuit::evaluate`: This should work through the components of a circuit,
setting the outputs based on the logic components' behavior and connectivity,
from some set of input settings.

• `Circuit::outputTruthTable`: This should cycle through all the possible
input settings, evaluate the circuit for each, outputting each as a row of
a well-formatted truth table.

A lot of code is written for you. The bulk of the your assignment is actually
to read through the existing code, understand it, and use it to write the code
for these two methods in `Circuit.cc`.

### Some notes on the `circuits` code

**Gates**  
There are five different component types, each a subclass of `class Gate`. They
share the following methods and attributes:

• `name()`: a string identifying the component from the file  
• `reset()`: gets the component ready to be evaluated  
• `evaluate()`: the rule for computing the output of the component from its input(s).  
• `_inputs`: wired links to other components that feed into this component  
• `_outputs`: wired links to other components that are fed this component's output  
• `_outputValue`: this is the value computed by `evaluate` and is read by the method `outputValue`.  
• `_outputReady`: indicates whether this component has had its `evaluate` method called yet, since `reset`.  This
is used by the `outputIsReady()` method.  
• `_numberOfInputsReady`: a count of how many of this component's input components have had their `evaluate` method called. This
is used by the `allInputsAreReady()` method.  
• `_signalReady()`: method that is called at the end of `evaluate`, alerting subsequent components  

Here are the differences amongst the component types:

`And`: outputs `true` only if all of its input components are feeding
`true`. Can have 0 or more inputs fed to it.

`Or`: outputs `true` only if at least of its input components is
feeding `true`. Can have 0 or more inputs fed to it.

`Not`: should only have one fed input. Inverts that to produce its
output.

`Input`: should have no fed inputs. Instead, has a method `setValue`
to set its value to `true` or `false`.

`Output`: should feed its output nowhere and should have only one fed
input.

For all of them, `evaluate` computes the component's rule for
producing its output, sets its `_outputValue`, and then signals that
evaluation's completion by decrementing `_numberOfInputsReady` for
each of the components that are fed by it.

**Computing with a Circuit**  
You are asked to write `Circuit::evaluate`, the code for evaluating
all the components of a circuit.  Before describing that method's
algorithm below, first it's best to understand the steps incolved in
computing with a circuit.

To compute with a Circuit `c`, code is expected to follow these four steps:

1. Call `c.reset()`. This "clears" all the state of each of the
circuit's components, specifically by having them indicate that their
outputs aren't "ready", and thus that none of their fed inputs are
ready either.

2. Call `c.setInputsFrom(ibv)` with some vector `ibv` of type
`vector<bool>`. It sets each of the `Input` components of `c` to
`true` or `false` based on a `vector<bool>` with `inputSize()`
elements.  Make sure that `ibv.size()` is the same as
`c.inputSize()`. This call gets the inputs ready for the next step.

3. Call `c.evaluate()`. This should simulate the circuit and
ultimately set the outputs. **You need to write this code.**

4. Call `c.readOutputsInto(obv)` with some vector `obv` of type
`vector<bool>`. Make sure its length is `c.outputSize()`. This reads
the outputs of the circuit into that vector.

Your code for `c.outputTruthTable` should repeatedly perform steps 1
through 4, once for each different set of boolean inputs.

**The algorithm for Circuit::evaluate()**  
There are several ways you can write your code for `evaluate`. All of
the ways should, in essence, traverse the "network" of wired
components, determining outputs of the circuit based on the inputs to
the circuit.

Here is a standard description of one such evaluation scheme: keep
track of which components have all their fed inputs "ready", and
haven't yet been evaluated. Pick one such gate `g` that is ready in
this way, and call `g.evaluate()` on it. This, presumably, will make
other components ready.  Keep repeating this until all the components
have been evaluated. Note that this process can be started by choosing
any of the `Input` components since they have no fed inputs. Because
their `_inputs` vector has size 0, then `allInputsAreReady` will be
true for them.

Other methods can involve performing a search through the circuit,
either by working forward from the inputs, evaluating those, then
evaluating the components they feed into, etc, until the circuit's
output values have all been determined.

Another possibility is to make this search a recursive one. You start
with some output, look at what wire feed into it, and figure out
the output of each such gate. To do that, you can recursively figure
out the output of each of the gates that produce a signal of some
wire feeding into it. Continuing this way, you reach te inputs,
terminating that particular recursive call of the backward
search immediately. **NOTE**: if you *do* choose this backwards
recursive method, you must write it in such a way that each
backwards-reachable component of the circuit is evaluated
*exactly once*.

**The truth table**  
Please have your `Circuit::outputTruthTable` exactly mimic the sample
output in the `samples` folder. For example, `xor.out` gives the
output I expect for the `xor.lsc` file.

For my own solution, I relied heavily on `vector`, `algorithm`, and
several other C++ STL components. I recommend that you do the same.
This part of the homework is your chance to play a bit with the
standard template library.

### BONUS BONUS BONUS

Some of you might be disappointed that the design of this project's
code is not as robust as the LogiSim tool we used to simulate
circuits. It does not handle circuits where there is a cyclic
path, that is, circuits where there is feedback. This means,
for example, that you cannot simulate a flip-flop, and other
sequential circuits.

It's a bit trickier to handle feedback. First off, for such circuits
to have any meaning, you would want to introduce a *delay* for each
component in the circuit. You also need to add a third "floating"
value to each wire (one that's neither `true` nor `false`), and
determine how each gate behaves when some/all of its inputs are this
floating value. 

Then, evaluation becomes a live simulation where wires carry different
values at different times. And that simulation is driven by *events*.
In the event where a component *g*'s output changes, then that changes
one of the inputs to some component *h* connected to it by a wire.
Say that change happens at time *t* and that the component *h* has 
a delay of *d*. Then the output of *h* needs to be re-evaluated at
time *t+d*. And that change gets propagated to any components that
*h* is connected to. And so on.

So we see that, in the event that there is a change to the output 
of component *g* at time *t*, that leads to the event of a change
to the output of *h* at time *t+d*.

A standard way to manage all this is with a *priority queue*,
something provided by the C++ STL.  That queue stores events. Each
event is stamped by the time it needs to occur in the simulation. They
get removed from the queue in order of increasing timestamp. (The
timestamp gives the "priority" of the event's exit from the
queue---earlier timestamps have greater priority.)

If an event on the queue corresponds to a change in an input fed
into *g* at time *t*, and it has delay of *d*, and it feeds
into *k*  components *h1*, *h2*,etc. Then a re-evaluation of *g*
needs to happen at time *t+d*, and that lead to re-evaluation
events of *h1* and *h2* at times *t+d+d1* and *t+d+d2*.

Anyway, this is a big BONUS exercise, but you are welcome to
give this a try instead.  Rather than output a truth table,
you instead want to produce a live feed of circuit changes.













            
            
    




    
