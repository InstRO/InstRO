# Directory structure

**instro/core** contains the core files of InstRO

**instro/tooling** contains the compiler abstraction interface of the tooling layer.
This abstraction can be used to develop compiler agnostic tools which operate solely on top of the InstRO abstraction.

**instro/pass** contains generic platform independent passes (if applicable and feasible).
These passes are built on top of the tooling interface layer.

**instro/rose** contains the ROSE specific implementation

**instro/clang** contains the Clang specific implementation

**instro/utility** contains the JSON configuration as well as helpers for memory management or retrieving environment variables, etc.
