mkdir build
cd build
cmake .. -DUTest=1 -DOS=linux
cmake --build .
../debug/serverTest
