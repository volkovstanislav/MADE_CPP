#!/bin/bash

set -e

g++ -std=c++17 -I./ my_test.cpp -o my_list_test
./my_list_test

echo All tests passed!
