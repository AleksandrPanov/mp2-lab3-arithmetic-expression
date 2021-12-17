cmake_minimum_required(VERSION 3.5)

project(ArithmeticExpression LANGUAGES CXX)

if(NOT CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

include_directories("include/")

FILE(GLOB SRCS
	"*.h"
        "*.cpp"
	"src/*.cpp"
	"include/*.h"
)

add_executable(${PROJECT_NAME} "src/main.cpp" ${SRCS})
