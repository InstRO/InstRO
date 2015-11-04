## Modules

The following Passes are currently planned [ ] and implemented [x] in InstRO.

#### Selectors
- [ ] AggregationStatementSelector
- [x] BooleanCompoundSelector 
- [ ] CallpathSelector
- [x] ConstructClassSelector
- [x] ConstructElevator
- [ ] IdentifierMatcherSelector
- [x] ProgramEntrySelector

#### Adapters
- [ ] DefaultInstrumentationAdapter

#### Rose only Implementations
- [x] FunctionWrapper (transformer)
- [x] UniqueCallpathTransformer
- [x] ConstructPrinter (Adapter)
- [x] RoseStrategyBasedAdapter

## Libraries and Versions

Internally we use the following version of compilers and libraries for building InstRO and ROSE.

### To build InstRO
- GCC 4.8.5
- Boost 1.57.0
- automake 1.14
- autoconf 2.69
- libtool 2.4
- RapidJSON (github master head)

### Additionally to build ROSE
- graphviz 2.38
- java 1.8
- doxygen 1.8.8
- ghostscript 9.16

### Building with Clang
The support to build with Clang will soon be available


## Style guidelines

### Filenames

Filenemas are generally mixed case. That means a file starts with an uppercase letter and then uses cammel case.
For example PassFactory.h

Files should be named like the class it contains. Generally one should have one class per file, unless there is a very good reason for not to follow this guideline.


### Code style

We use an indentation with only tab characters. With that users can decide to display source w.r.t. their personal preference (e.g. 2 vs. 4 spaces per indentation).

Namespaces are not indented. Accordingly a class definition starts at column 0.

### Naming

Although we use namespaces to separate disstinct entities, we still reflect this in a classes name.
For example would a Pass which is specialized to work with Rose be named RosePass and put into the Rose namespace - LLVMPass accordingly.

## JSON Configuration

InstRO is able to parse a configuration of connected passes from a JSON file using the `ConfigurationLoader` in the `InstRO::Utility` namespace if it was built with the rapidjson library. The JSON file must contain a global array of objects where each object represents a pass. Each pass object must specify the type name of the pass (without Rose or Clang specific prefixes) under the *type* key und a unique identifier using the *id* key. The *inputs* key may be used by a pass to specify multiple input passes via an array of pass identifiers. Passes may need additional arguments like a list of strings or a `ConstructTraitType`. For convenience, the *ConfigInstrumentor* is shipped as a simple application which reads in the JSON file specified by the environment variable *INSTRO_CONFIG* and runs the resulting setup.

The following JSON is an example which marks occurences of *printf* and *exit* and feeds them to a `ConstructRaisingElevator` which outputs their file scope to a `ConstructPrinter`:
```
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
```
