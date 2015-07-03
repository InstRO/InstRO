#!/usr/bin/env bash
#
# This is a runner script to assemble all the necessary steps
# to use clang on an input file to create a llvm file
# After that the llvm optimizer is invoked loading the instrumentation
# pass and generating byte code, which it self then gets
# compiled to assembly and use gcc to finally produce an object
#
# Link step needs to be carried out manually for now!

echo "Running toolchain on input file: $1"

INPUT=$1
# get rid of the path
FILE=${INPUT##*/}
# get rid of the file extension
FILE_NO_EXT=${FILE%.*}
# prepare the filename we want to output to
OUTPUT_BC_FILE=$FILE_NO_EXT.bc

echo "Cutted input is $FILE"
echo $FILE_NO_EXT

#Invocation
# TODO change this line to account for your actual path settings.
/home/j_lehr/opt/clang/install/bin/opt --disable-inlining --debug-pass=Structure -load /home/j_lehr/all_repos/instro-v5/repo/lib/cashesPass.so -cgb-sel -cgb-instro -time-passes -o $OUTPUT_BC_FILE $INPUT

#/home/j_lehr/opt/clang/install/bin/opt --disable-opt --disable-inlining --debug-pass=Structure -load /home/j_lehr/all_repos/instro-v5/repo/lib/cashesPass.so -fdef-sel -fdef-instro -time-passes -o $OUTPUT_BC_FILE $INPUT

# Translation from LLVM to Assembler
llc $OUTPUT_BC_FILE -o ./obj/$FILE_NO_EXT.s

# Lets assemble the .s file into an object file using GCC
gcc -c ./obj/$FILE_NO_EXT.s -o ./obj/$FILE_NO_EXT.o
