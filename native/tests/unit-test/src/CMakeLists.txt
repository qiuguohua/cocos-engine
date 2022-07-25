
include(${CMAKE_CURRENT_LIST_DIR}/../../../cmake/predefine.cmake)
set(BINARY ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)

set(TestDir "tests/unit-test/src")
cocos_source_files(MODULE FILESYSTEM
    ${TestDir}/cocos/platform/filesystem/filepath_test.cpp
    ${TestDir}/cocos/platform/filesystem/filesystem_test.cpp
    ${TestDir}/cocos/platform/filesystem/local_filesystem.cpp)

if(WIN32)
cocos_source_files(MODULE FILESYSTEM
    ${TestDir}/cocos/platform/filesystem/win_filesystem_test.cpp)
endif()

source_group(TREE ${CWD}/${TestDir} PREFIX "Source Files" FILES ${FILESYSTEM_SOURCE_LIST})

list(APPEND SOURCES "${FILESYSTEM_SOURCE_LIST}")

add_executable(${BINARY} ${SOURCES})

add_test(NAME ${BINARY} COMMAND ${BINARY})

target_link_libraries(${BINARY} PUBLIC gtest ${ENGINE_NAME})
target_include_directories(${BINARY} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../..)

