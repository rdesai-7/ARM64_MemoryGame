#!/bin/bash

#To run this file, first run "chmod +x test_assemble.sh" in the terminal
#Then run "./test_assemble.sh" in the terminal

#Script to run all assembler tests
echo "Creating assemble executable file"
make clean
make 
rm ../../armv8_testsuite/solution/assemble
mv assemble ../../armv8_testsuite/solution/
cd ../../armv8_testsuite
source ./.venv/bin/activate
make clean
echo "Running assembler tests"
./run -A -pf
