cmake_minimum_required(VERSION 3.9)

add_library(SettingsManager STATIC
            src/CSettingsManager.cpp)

target_include_directories(SettingsManager PRIVATE
						   ${settingManager_INCLUDE_DIRS})

if(UTest)
    add_subdirectory(src/unitTests)
endif()