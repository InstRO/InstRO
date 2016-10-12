# InstRO
A framework for building tailored compiler instrumentation.

## Modules

The following Passes are currently planned [ ] and implemented [x] in InstRO.

#### Selectors
- [x] BooleanCompoundSelector 
- [x] CallpathSelector
- [x] ConstructTraitSelector
- [x] ConstructElevator
- [x] IdentifierMatcherSelector
- [x] ProgramEntrySelector

#### Adapters
- [x] DefaultInstrumentationAdapter
- [x] ConstructPrinterAdapter

#### Rose only Implementations
- [x] RoseFunctionWrapper (Transformer)
- [x] RoseUniqueCallpathTransformer
- [x] RoseStrategyBasedAdapter


## Building InstRO
### Dependencies
We use the following version of compilers and libraries for building InstRO and ROSE.
- ROSE (obtained from our fork [InstRO/InstRO-ROSE](https://github.com/InstRO/InstRO-ROSE))
- GCC 4.8.5
- Boost 1.57.0
- automake 1.14
- autoconf 2.69
- libtool 2.4
- [https://github.com/miloyip/rapidjson](https://github.com/miloyip/rapidjson) (optional, only for the JSON configuration feature)
- Python 2.7.10 (required for testing)

### Build Steps
Install the required dependencies. We highly recommend to use our [InstRO/InstRO-ROSE](https://github.com/InstRO/InstRO-ROSE) fork as we added functionality and fixes that are not merged back to the original repository (yet).

```bash
$ ./make-config
>> generates the configure script
$ ./configure --with-boost=/PATH/TO/BOOST/BASE --with-rose=/PATH/TO/ROSE/BASE \
  [--with-rapidjson=/PATH/TO/RAPIDJSON/BASE] [--enable-examples] [--prefix=path/to/where/install][--with-scorep=yes]
>> generates the Makefiles
$ make
```
Optionally you can build and run the tests (requires Python) and install InstRO.
```bash
$ make check
>> builds and runs the test suite
$ make install
>> installs the binaries to the given prefix
```

InstRO comes with an examples directory, which includes some show-cases. In order to build these examples invoke configure with the additional `--enable-examples` flag.
Building with examples enabled gives you a show-case target `example-run`.
```bash
$ make example-run
>> Applies an example InstRO instrumentation tool to a sample target application
```

The JSON configuration feature requires the optional flag `--with-rapidjson`.

InstRO is able to instrument with a specialized Score-P adapter.
If you are planning to use this functionality it is recommended to also enable the tests using `--with-scorep=yes`.
InstRO assumes that scorep can be found in your `PATH` variable.

### Documentation
Documentation may be generated from the source code and all readme files via [Doxygen](http://doxygen.org). Navigate into the `docs` subdirectory and type `make`:

```
cd docs/
make
```

### Testing InstRO
Run `make check` in the top level build directory. For more information please consult the README in the test directory.

### Building with Clang
Support for building the InstRO framework with the Clang compiler infrastructure is currently work in progress.

#### Dependencies
- CMake >= 3.4.3
- Clang 3.9
- Boost 1.60

#### Build steps
At least in our environment the following steps are sufficient. However, as noted building with Clang is work in progress.

```
mkdir build && cd build
cmake ../repo
make
```

## Using InstRO
InstRO includes a few examples which demonstrate some of its components.
These examples can be found in either `$(builddir)/examples` or, if InstRO has been installed, in `$(installdir)/bin`.
When installed, invoking an example is as easy as changing into the `bin` directory and invoking an example translator on a source file.
```
$ ./RoseInstrumentor myInputFile.cpp
```
The above command will create a simple `a.out` file, just like GCC would do.
Running the transformed program may require adding `$(installdir)/lib` to the `LD_LIBRARY_PATH` environment variable of the system.

### Using InstRO with a measurement library
InstRO has two implementations of the InstRO Measurement Interface.
The default library (libimi) does not provide any measurement functionality and is implemented using empty function bodies.
In addition, a test support library is provided, which does generate console output of the construct identifier for enter/exits.

### Command line interface
In order to change the library implementation that InstRO should link the transformed program with, the following command line arguments are accepted:

`--instro-library-path` tells the translator which directory it should add to the search list in order to find the desired library.

`--instro-library-name` tells the translator which library it should link. Note: Expects solely the library name without the preceeding lib or succeeding .so.

`--instro-include` tells the translator where to look for headers which need to be included to declare inserted function signatures. This is only sensible to change if an adapter does not use the InstRO Measurement Interface functions.

Thus, an invocation of a translator in the install directory, which uses the provided support library, is as follows:
```
$ ./RoseInstrumentor --instro-library-path=../lib --instro-library-name=InstRO_rtsupport myTarget.cpp
```
Please note that the `$(installdir)/include` directory is used as the default value of the `--instro-include` flag.

### JSON Configuration

InstRO is able to parse a configuration of connected passes from a JSON file using the `ConfigurationLoader` in the `InstRO::Utility` namespace if it was built with the rapidjson library. The JSON file must contain a global array of objects where each object represents a pass. Each pass object must specify the type name of the pass (without Rose or Clang specific prefixes) under the *type* key and a unique identifier using the *id* key. The *inputs* key may be used by a pass to specify multiple input passes via an array of pass identifiers. Passes may need additional arguments like a list of strings or a `ConstructTraitType`. For convenience, the *ConfigInstrumentor* is shipped as an example application which reads in the JSON file specified by the environment variable *INSTRO_CONFIG* and runs the resulting setup.

The following JSON is an example which marks occurences of `printf` and `exit` and feeds them to a `ConstructRaisingElevator` which outputs their file scope to a `ConstructPrinterAdapter`:
~~~
[
  {
    "type":"IdentifierMatcherSelector",
    "id":"identifier",
    "args":[
      "::printf",
      "::exit"
    ]
  },
  {
    "type":"ConstructPrinterAdapter",
    "id":"printer",
    "inputs":[
      "elevator"
    ]
  },
  {
    "type":"ConstructRaisingElevator",
    "id":"elevator",
    "inputs":[
      "identifier"
    ],
    "level":"FileScope"
  }
]
~~~


## Style guidelines

### Filenames

Filenames are generally mixed case: A file starts with an uppercase letter and then uses cammel case, e.g, PassFactory.h.

A file should be named like the class it contains. In general, we pursue a one-class-per-file policy, unless there is a very good reason for not following this guideline.

### Code style

We use an indentation with only tab characters, so that users may decide to display the source code w.r.t. their personal preference (e.g. 2 vs. 4 spaces per indentation).

Namespaces are not indented. Therefore, a class definition starts at column 0.

Include guards are to correspond to the directory structure leading to the respective file. For example, a header file located at lib/include/instro/pass/foo/Foo.h has the include guard INSTRO_PASS_FOO_FOO_H.

A *clang-format* configuration file is included in the top level of the repository. The configuration assumes a tab width of two spaces.

### Naming

Although we utilize namespaces to separate distinct entities, we still reflect this in the name of a class.
For instance, a Pass which is specialized to work with ROSE would be named RosePass and put into the Rose namespace.

