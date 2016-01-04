# Testing

The test-suite is work in progress.

- <b>Compile tests</b> are the more time consuming but stronger tests, as they build InstRO and run `make check` for every build.
- <b>Apply tests</b> in turn are meant as the tests run on a single invocation of `make check`.

## Compile tests

The `CompileRunner.py` script is used to configure and build all possible variants using the specified parameters automatically.
It requires paths to Boost and optionally ROSE, Clang and RapidJSON.

The script builds all sensible combinations possible with the provided arguments and invokes `make check` for each configuration.
Right now it uses eight compile jobs and four when running `make check`.

## Apply tests

Invoke the tests using `make check` in the toplevel directory.

The different apply-tests are orchestrated by the Python script `ApplyRunner.py`, which takes care of invoking the TestInstrumentor-binaries on all input files specified.
In order to add a TestInstrumentor, the binary name needs to be added to the `testPrograms` list in the runner script.

Right now, only selection testing is supported.

### Selection tests

Selection tests are carried out with different TestInstrumentors, each taking care of a specific selection configuration.

For each TestInstrumentor in the test directory there is a configuration directory in `test/input`.
Within this directory a file specifying which test-case source files the respective instrumentor should be applied to (`targets.lst`) as well as the expected-items config file (`*.in`).
All test-case source files reside in the `test/input` directory and are reference from the `targets.lst` file simply by name.
The specification allows for multiple adapters, which are identified using labels.
Inside the .in file this is reflected with labels of the form `+labelname`.
Every line following a label will be used as an expected element for the `TestAdapter` instance identified by `+labelname` unless the line is left blank.
So a valid .in file for some file main.cpp with `int main(...){...}` in line one, would look like
~~~
+CTFunctionSelector
main.cpp:1:1--Function-main
~~~
And a valid `targets.lst` file which references solely the `main.cpp` test-case source file would look like
~~~
main
~~~
