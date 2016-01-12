## Directory structure

<b>core</b> contains the core files of InstRO
I guess we still need to decide <b>what</b> are core files....

<b>tooling</b> contains the compiler abstraction interface of the tooling layer.
This abstraction can be used to develop compiler agnostic tools, which operate solely on top of the InstRO abstraction.

<b>pass</b> contains generic platform independent passes (if applicable and feasible).
These passes are built on top of the tooling interface layer.

<b>rose</b> contains ROSE specific implementation

<b>clang</b> contains the Clang specific implementation

<b>llvm</b> contains the LLVM specific implementation

<b>utility</b>: place to put helpers for memory management or retrieving environment variables etc.

<b>misc</b> what?
