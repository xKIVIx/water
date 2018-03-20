cmake_minimum_required(VERSION 3.9)

enable_testing()

set(TESTS
    CNet.cpp
    main.cpp)

set(LIBRES
    CNet
    gtest_main)

add_executable(serverTest 
               ${TESTS})


target_link_libraries(serverTest 
                      ${LIBRES})


add_test(NAME test  COMMAND serverTest)