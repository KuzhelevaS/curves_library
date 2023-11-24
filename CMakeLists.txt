cmake_minimum_required(VERSION 3.11)

set(PROJECT_NAME geometric)

project(${PROJECT_NAME} CXX)
set(CMAKE_CXX_STANDARD 17)

if (MSVC)
	add_compile_options(/W4)
else()
	add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

if (WIN32)
  # Make visible dll for exe. Must be before add_subdirectory.
  set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
  set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
endif()

add_subdirectory(curves_lib curves_lib_build)

add_executable(${PROJECT_NAME} main.cpp)

if(CMAKE_SYSTEM_NAME MATCHES "^MINGW")
	set(SYSTEM_LIBS -lstdc++)
else()
	set(SYSTEM_LIBS)
endif()

if (UNIX)
	set(LTBB -ltbb)
else()
	set(LTBB)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/curves_lib")
target_link_libraries(${PROJECT_NAME} CurvesLib ${SYSTEM_LIBS} ${LTBB})

