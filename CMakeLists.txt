cmake_minimum_required(VERSION 3.0.0)
project(SikoraGenerator VERSION 0.1.0)
set (CMAKE_CXX_STANDARD 20)
include(CTest)
enable_testing()

include_directories(
        ${PROJECT_SOURCE_DIR}/headers
        ${PROJECT_SOURCE_DIR}/cppSources
)
file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/headers/*.h"
        "${PROJECT_SOURCE_DIR}/headers/*.hpp"
        "${PROJECT_SOURCE_DIR}/cppSources/*.cpp"
        "${PROJECT_SOURCE_DIR}/cppSources/*.c"
)
add_executable(SikoraGenerator ${all_SRCS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
