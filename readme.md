# InstRO README

InstRO - a framework for building tailored compiler instrumentation.

## Modules

The following Passes are currently planned [ ] and implemented [x] in InstRO.

#### Selectors
- [ ] AggregationStatementSelector
- [x] BooleanCompoundSelector 
- [x] CallpathSelector
- [x] ConstructClassSelector
- [x] ConstructElevator
- [x] IdentifierMatcherSelector
- [x] ProgramEntrySelector

#### Adapters
- [x] DefaultInstrumentationAdapter
- [x] ConstructPrinterAdapter

#### Rose only Implementations
- [x] RoseFunctionWrapper (transformer)
- [x] RoseUniqueCallpathTransformer
- [x] RoseStrategyBasedAdapter


## Building InstRO
### Dependencies
We use the following version of compilers and libraries for building InstRO and ROSE.
- GCC 4.8.5
- Boost 1.57.0
- automake 1.14
- autoconf 2.69
- libtool 2.4
- [https://github.com/miloyip/rapidjson](https://github.com/miloyip/rapidjson) (HEAD)
- Rose (preferably obtained from our fork [InstRO/InstRO-ROSE](https://github.com/InstRO/InstRO-ROSE))
- Python 2.7.10

### Build Steps
In order to build InstRO, one first builds and installs Rose. We recommend using the InstRO/rose fork, as we add functionalityt which may not be merged back into the rose main repository. Secondly, one builds and installs Boost.
After these prerequisites are fulfilled one runs the 'make-config' script located in the top level directory, which builds the configure file. 'configure' with providing the Boost (--with-boost) and the Rose (--with-rose) paths. InstRO comes with an examples directory, which includes show-cases. In order to build these examples, invoke configure with additional --enable-examples. Finally, run 'make'.

### Testing InstRO
Run 'make check' in the top level build directory. More about how we test InstRO can be found in the test directory.

### Building with Clang
Support for building the InstRO framework with the Clang compiler infrastructure is work in progress.


## JSON Configuration

InstRO is able to parse a configuration of connected passes from a JSON file using the `ConfigurationLoader` in the `InstRO::Utility` namespace if it was built with the rapidjson library. The JSON file must contain a global array of objects where each object represents a pass. Each pass object must specify the type name of the pass (without Rose or Clang specific prefixes) under the *type* key und a unique identifier using the *id* key. The *inputs* key may be used by a pass to specify multiple input passes via an array of pass identifiers. Passes may need additional arguments like a list of strings or a `ConstructTraitType`. For convenience, the *ConfigInstrumentor* is shipped as a simple application which reads in the JSON file specified by the environment variable *INSTRO_CONFIG* and runs the resulting setup.

The following JSON is an example which marks occurences of *printf* and *exit* and feeds them to a `ConstructRaisingElevator` which outputs their file scope to a `ConstructPrinter`:
~~~
[
  {
    "type":"IdentifyerSelector",
    "id":"identifyer",
    "args":[
      "printf",
      "exit"
    ]
  },
  {
    "type":"ConstructPrinter",
    "id":"printer",
    "inputs":[
      "elevator"
    ]
  },
  {
    "type":"ConstructRaisingElevator",
    "id":"elevator",
    "inputs":[
      "identifyer"
    ],
    "level":"fileScope"
  }
]
~~~


## Style guidelines

### Filenames

Filenames are generally mixed case. That means a file starts with an uppercase letter and then uses cammel case.
For example PassFactory.h

Files should be named like the class it contains. Generally one should have one class per file, unless there is a very good reason for not to follow this guideline.


### Code style

We use an indentation with only tab characters. With that users can decide to display source w.r.t. their personal preference (e.g. 2 vs. 4 spaces per indentation).

Namespaces are not indented. Accordingly a class definition starts at column 0.

We use include guards which correspond to the directory structure that lead to the respective file. If the file is located at lib/include/instro/pass/foo/Foo.h then its respective include guard is INSTRO_PASS_FOO_FOO_H.

### Naming

Although we use namespaces to separate disstinct entities, we still reflect this in a classes name.
For example would a Pass which is specialized to work with Rose be named RosePass and put into the Rose namespace - LLVMPass accordingly.

