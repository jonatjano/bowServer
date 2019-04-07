cmake_minimum_required(VERSION 3.6)
set(PROJECT_NAME cppWSTest)
project(${PROJECT_NAME})

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
    include_directories(/usr/include/x86_64-linux-gnu/)
endif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
include_directories(/usr/local/include/)
link_directories(/usr/local/lib)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
#set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
#set(CMAKE_EXE_LINK_DYNAMIC_C_FLAGS)       # remove -Wl,-Bdynamic
#set(CMAKE_EXE_LINK_DYNAMIC_CXX_FLAGS)
set(CMAKE_SHARED_LIBRARY_C_FLAGS)         # remove -fPIC
set(CMAKE_SHARED_LIBRARY_CXX_FLAGS)
#set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)    # remove -rdynamic
#set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)

find_library(uWS STATIC)
find_library(jsoncpp STATIC)
find_library(pthread STATIC)
find_library(GreenLibS STATIC)
find_library(stdc++fs STATIC)
set(SOURCE_FILES
        src/main.cpp
        )

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_SEARCH_START_STATIC 1)
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_SEARCH_END_STATIC 1)
#target_link_libraries(${PROJECT_NAME} libGreenLibS.a libjsoncpp.a libpthread.a libuWS.a libcurl.a libssl.a libstdc++fs.a libcrypto.a libz.a libdl.a libidn.a libpthread_nonshared.a libpthread.a libc_nonshared.a libc.a)
target_link_libraries(${PROJECT_NAME} jsoncpp pthread uWS curl stdc++fs ssl z)