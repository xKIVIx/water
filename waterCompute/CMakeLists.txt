cmake_minimum_required(VERSION 3.9)

set(PACKAGE_CL_FILES
	src/cl/edgeCompute.cl
	src/cl/prepareData.cl)

set(LIBRES
	${OpenCL_LIBRARIES}
    SettingsManager)

add_library(WaterCompute STATIC
            src/CWaterCompute.cpp
			src/CWaterOpenCL.cpp
			src/CKernel.cpp
			src/CMemObject.cpp)

target_include_directories(WaterCompute PRIVATE
						   ${OpenCL_INCLUDE_DIRS}
						   ${waterCompute_INCLUDE_DIRS}
						   ${settingManager_INCLUDE_DIRS})

target_link_libraries(WaterCompute ${LIBRES})

file(WRITE ${CMAKE_BINARY_DIR}/program.cl "")

foreach(PACKAGE_CL_FILE ${PACKAGE_CL_FILES})
	file(READ ${PACKAGE_CL_FILE} CONTENTS)
	file(APPEND ${CMAKE_BINARY_DIR}/program.cl "${CONTENTS}")
endforeach()
configure_file(
	${CMAKE_BINARY_DIR}/program.cl
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/program.cl
	COPYONLY)

if(UTest)
    add_subdirectory(src/unitTests)
endif()