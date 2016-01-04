
./core contains the core files of InstRO
./tooling contains the compiler abstraction interface of the tooling layer
./rose contains the implementation for ROSE
./pass contains generic platform independent passes (if applicable and feasible)
./extra contains additional stuff we wish to ship, e.g. support for reading specific files.
		We should however keep the main extra platform agnostic, i.e. no rose or clang related stuff. 
		That should be located in ./clang/extra or ./rose/extra
