# CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
cmake_minimum_required (VERSION 2.8.11)
project (server_zmq)

include_directories (${CMAKE_CURRENT_SOURCE_DIR})
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../include)

set(CMAKE_INCLUDE_PATH $ENV{CPLUS_INCLUDE_PATH} $ENV{C_INCLUDE_PATH})
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_COMPILER clang)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "-Wall -g")
set(CMAKE_LD_FLAGS "-lc++")

file(GLOB SRV_CC_FILES ${CMAKE_CURRENT_SOURCE_DIR} *server.cc)
file(GLOB CLI_CC_FILES ${CMAKE_CURRENT_SOURCE_DIR} *client.cc)

add_executable(dummy_server ${SRV_CC_FILES})
add_executable(dummy_client ${CLI_CC_FILES})

target_link_libraries(dummy_server zmq stdc++ pthread)
target_link_libraries(dummy_client zmq stdc++ pthread)

