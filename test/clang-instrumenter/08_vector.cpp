// RUN: %instrument %s | FileCheck %s

#include <vector>

// CHECK-NOT: Instrumentation failed

int len(std::vector<int>& v) {
	return v.size();
}
