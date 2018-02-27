cmake_minimum_required(VERSION 3.11)

enable_testing()

set(TESTS 
    main.cpp)

set(LIBRES
    CNet
    gtest_main)

if(${OS} STREQUAL "linux")
    message("ggg")
    set(TESTS
        ${TESTS}
        CNetLinux.cpp)
else()
endif()


add_executable(serverTest 
               ${TESTS})


target_link_libraries(serverTest 
                      ${LIBRES})


add_test(NAME test  COMMAND serverTest)