mkdir build
cd build
cmake .. -DUTest=1 -DOS=linux -DCMAKE_BUILD_TYPE=Debug
cmake --build .
