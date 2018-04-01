cmake_minimum_required(VERSION 3.9)

enable_testing()

set(TESTS
	mainTests.cpp
    main.cpp)

set(LIBRES
	CSettingsManager
    gtest_main)

add_executable(settingManagerTest 
               ${TESTS})


target_link_libraries(settingManagerTest 
                      ${LIBRES})

target_include_directories(settingManagerTest PRIVATE
						   ${GTEST_INCLUDE_DIR}
						   ../../include)


add_test(NAME testSM  COMMAND settingManagerTest)