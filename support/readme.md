## InstRO Measurement Interface

This interface is used by the default InstRO adapter to insert calls to, when instrumenting source code.
It can be used either directly or used as a wrapper for state-of-the-art measurement libraries, which typically implement the GNU profiling interface.


### Function level instrumentation

The InstRO measurement interface resembles the GNU profiling interface with its entry/exit functions.
A difference is that the function is identified using a compile-time identifier.
The identifier is passed to the measurement function and an external id-file can be queried for its corresponding name.


### Additional construct instrumentation

In addition to function level instrumentation the InstRO interface allows for instrumentation of all construct levels.
Thus, it provides a fine grain level of detail possibility for instrumentation.
Each construct has a corresponding start/end pair of functions which are inserted by the default adapter.
