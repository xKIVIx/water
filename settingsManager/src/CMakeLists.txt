cmake_minimum_required(VERSION 3.9)

add_library(CSettingsManager STATIC
            CSettingsManager.cpp)

target_include_directories(CSettingsManager PRIVATE
						   ../include)

if(UTest)
    add_subdirectory(unitTests)
endif()