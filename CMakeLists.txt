cmake_minimum_required(VERSION 3.10.0)

project(test)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

add_definitions("-Wall -g")

include_directories(${PROJECT_SOURCE_DIR}/include)

add_definitions(-DDEBUG)

file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c ${CMAKE_CURRENT_SOURCE_DIR}/src/cjson/*.c ${CMAKE_CURRENT_SOURCE_DIR}/src/log/*.c ${CMAKE_CURRENT_SOURCE_DIR}/src/request/*.c ${CMAKE_CURRENT_SOURCE_DIR}/src/response/*.c ${CMAKE_CURRENT_SOURCE_DIR}/src/WebSocket/*.c)


link_libraries(pthread)
link_libraries(crypto)

#运行与打包
if (YES)

message("run coding")
add_executable(app ${SRC} main.c)

else()
add_library(http SHARED ${SRC})
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/v2/)
endif()

