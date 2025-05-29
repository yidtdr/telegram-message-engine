cmake_minimum_required(VERSION 3.15)

project(telegram_engine)

include(FetchContent)

FetchContent_Declare(
  gumbo
  GIT_REPOSITORY https://github.com/google/gumbo-parser.git
  GIT_TAG v0.10.1
)
FetchContent_MakeAvailable(gumbo)

FetchContent_Declare(
	pybind11
	GIT_REPOSITORY https://github.com/pybind/pybind11.git
	GIT_TAG v2.13.6
)
FetchContent_MakeAvailable(pybind11)

find_package(Python REQUIRED COMPONENTS Interpreter Development)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(telegram_engine 
	src/messages/messagedb.cpp
	src/messages/messagequery.cpp
	src/core/parsenode/node.cpp
	src/parsing/message_extractor.cpp
	src/main.cpp
)

target_include_directories(telegram_engine PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(telegram_engine
	gumbo
	pybind11::embed 
	Python::Python
)
