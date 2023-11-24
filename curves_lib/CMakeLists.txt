cmake_minimum_required(VERSION 3.11)

project(CurvesLib CXX)
set(CMAKE_CXX_STANDARD 17)

FILE(GLOB CURVES_SRC *.cpp *.h)

add_library(CurvesLib SHARED ${CURVES_SRC})

if(CMAKE_SYSTEM_NAME MATCHES "^MINGW")
	set(SYSTEM_LIBS -lstdc++)
else()
	set(SYSTEM_LIBS)
endif()

target_link_libraries(CurvesLib ${SYSTEM_LIBS})

# Define param for macros section in curves.h
target_compile_definitions(CurvesLib PRIVATE CURVES_DLL)
