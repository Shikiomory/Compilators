cmake_minimum_required(VERSION 3.10)
project(LexParser)

set(CMAKE_CXX_STANDARD 17)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/error
    ${PROJECT_SOURCE_DIR}/frequent
    ${PROJECT_SOURCE_DIR}/Scann
    ${PROJECT_SOURCE_DIR}/TextDriver
)

file(GLOB SOURCES
    "${PROJECT_SOURCE_DIR}/*.cpp"
    "${PROJECT_SOURCE_DIR}/error/*.cpp"
    "${PROJECT_SOURCE_DIR}/frequent/*.cpp"
    "${PROJECT_SOURCE_DIR}/Scann/*.cpp"
    "${PROJECT_SOURCE_DIR}/TextDriver/*.cpp"
)

add_executable(lexparser ${SOURCES})
