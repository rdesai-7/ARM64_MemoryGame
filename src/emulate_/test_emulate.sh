#!/bin/bash

#To run this file, first run "chmod +x test_emulate.sh" in the terminal
#Then run "./test_emulate.sh" in the terminal

#Script to run all emulator tests
echo "Creating emulate executable file"
make clean
make 
rm ../../armv8_testsuite/solution/emulate
mv emulate ../../armv8_testsuite/solution/
cd ../../armv8_testsuite
echo "Running emulator tests"
source ./.venv/bin/activate
make clean
./run -E -pf