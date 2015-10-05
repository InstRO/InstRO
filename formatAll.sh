#!/bin/sh
find . -regex '.*\.\(h\|cpp\)' -exec clang-format -i {} \;
