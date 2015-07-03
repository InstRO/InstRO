#include "instro/example/ExampleInstrumentor.h"
namespace InstRO {
void ExampleInstrumentor::init() {}
void ExampleInstrumentor::finalize() {}

void ExampleInstrumentor::apply() { passManager->execute(); }
}