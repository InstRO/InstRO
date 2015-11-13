### Test

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

To test for valid selection according to the instrumentation configuration, we apply a test instrumentor to the collection of test-case codes.
All test-case files reside in the test/input directory.
Each one is split into two files: one source file and one specification of the awaited Construct Sets.
The convention is to name source and specification file equally, with the specification having a suffix of .in.
The specification allows for multiple adapters, which are identified using labels.
Inside the .in file this is reflected with labels of the form `+labelname`.
Every line following a label will be used as an expected element for the `TestAdapter` instance identified by `+labelname`.
So a valid .in file for some file main.cpp with `int main(...){...}` in line one, would look like
```
+Selector1
main.cpp:1--Function-main
```
