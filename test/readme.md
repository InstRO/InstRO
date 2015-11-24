# Testing

We do black-box testing

- compile InstRO
- apply to a target
- compile target
- run target

## Compile tests

The Runner.py script is used to configure and build all possible variants using the specified parameters automatically.
It requires paths to Boost and optionally ROSE, Clang and RapidJSON.
Please see Runner's included help for more details.

## Apply tests

Invoke the tests using `make check` in the toplevel directory.

For each TestInstrumentor in the test directory there is a configuration directory in `test/input`.
Within this directory a file specifying which test-case source files the respective instrumentor should be applied to (`targets.lst`) as well as the expected-items config file (`*.in`).
All test-case source files reside in the `test/input` directory and are reference from the `targets.lst` file simply by name.
The specification allows for multiple adapters, which are identified using labels.
Inside the .in file this is reflected with labels of the form `+labelname`.
Every line following a label will be used as an expected element for the `TestAdapter` instance identified by `+labelname`.
So a valid .in file for some file main.cpp with `int main(...){...}` in line one, would look like
~~~
+CTFunctionSelector
main.cpp:1--Function-main
~~~
And a valid `targets.lst` file which references solely the `main.cpp` test-case source file would look like
~~~
main
~~~
