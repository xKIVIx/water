cmake_minimum_required(VERSION 3.9)

enable_testing()

set(TESTS
	CWaterOpenCL.cpp
    main.cpp)

set(LIBRES
	WaterCompute
    gtest_main)

add_executable(waterComputeTest 
               ${TESTS})


target_link_libraries(waterComputeTest 
                      ${LIBRES})

target_include_directories(waterComputeTest PRIVATE
						   ${GTEST_INCLUDE_DIR}
						   ${waterCompute_INCLUDE_DIRS})


add_test(NAME testWCT  COMMAND waterComputeTest)