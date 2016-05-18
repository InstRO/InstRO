# Testing

The test suite comprises several test binaries which are invoked on a set of input files.
In order to run the test suite for a single build, it is sufficient to call `make check` in the toplevel build directory.

As of today, we expect four tests to fail for the Rose implementation (as that are mainly problems within Rose itself):
```
|--- ConstructHierarchySelectionTest	templContainer.cpp
|--- ConstructHierarchySelectionTest	controlConstructs.cpp
|--- ConstructHierarchySelectionTest	whiletest.cpp
|--- ConstructElevatorTest	templContainer.cpp
```


In general, the tests are split up into two scripts and organized as follows:
- **Apply tests** are the tests run on a single invocation of `make check`.
- **Compile tests** are the more time consuming but stronger tests, as they build InstRO and run `make check` for every build.
- **input** holds all input data for the test suite - each test in turn has its own sub directory for the configuration input data


## Compile tests

The `CompileRunner.py` script is used to configure and build all possible variants using the specified parameters automatically.
It requires paths to Boost and one of [Rose, Clang] and optionally RapidJSON.

The script builds all sensible combinations possible with the provided arguments and invokes `make check` for each configuration.
The `--jobs N` option may be used to execute the builds in parallel and is equivalent to the `-j N` option of make. `N` defaults to 20.

## Apply tests

Invoke the tests using `make check` in the toplevel directory.

The different apply-tests are orchestrated by the Python script `ApplyRunner.py`, which takes care of invoking the TestInstrumentor-binaries on all input files.
In order to add a TestInstrumentor the binary name needs to be added to the `testPrograms` list in the runner script.

At the moment (18 Mai 2016) 5 tests should fail for the ROSE implementation and 17 for the Clang implementation. Furthermore, the tests for the Clang implementation require static libraries due to linking issues with shared libraries.

### Selection tests

Selection tests are carried out with different TestInstrumentors, each taking care of a specific selection configuration.

For each TestInstrumentor in the test directory there is a configuration directory in `test/input`.
A configuration directory contains a file listing all test-case source files which are passed to the respective instrumentor (`targets.lst`) as well as files describing the expected selection for each test-case (`*.in`).
All test-case source files reside in the `test/input` directory and are referenced by a `targets.lst` file simply by name.
The specification allows for multiple adapters which are identified using labels.
Inside the .in file this is reflected with labels of the form `+labelname`.
Every line following a label will be used as an expected element for the `TestAdapter` instance identified by `+labelname` unless the line is left blank.
So a valid .in file for some file main.cpp with `int main(...){...}` in line one, would look like:
~~~
+CTFunctionSelector
main.cpp:1:1--Function-main
~~~

And a valid `targets.lst` file which references solely the `main.cpp` test-case source file would look like:
~~~
main
~~~

### Instrumentation tests
The DefaultInstrumentationAdapterTest instruments and compiles the input source.
It does the linking and the runner takes care of invoking the instrumented program.
In order to test for correct nesting of the instrumented constructs it uses the `InstRO_rtsupport` library, which maintains a stack, pushing and poping the encountered identifiers.
If identifiers do not match, this is considered an error and the program aborts.

As we want to test the actual instrumentation, we do not check again for the selection being accurate in these tests.
Thus, there are no testcase.in files as with the selection tests.
