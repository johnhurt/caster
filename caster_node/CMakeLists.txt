cmake_minimum_required(VERSION 3.3)
project(caster_node)

set(TEST_PROJECT_NAME ${PROJECT_NAME}_test)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

set(BOOST_COMPONENTS
        program_options
        date_time
        filesystem
        system
)


find_package(Boost 1.57.0 COMPONENTS ${BOOST_COMPONENTS} REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_LIST_DIR}/src)

file(GLOB MAIN_CPP "src/main.cpp")
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

file(GLOB_RECURSE TEST_SOURCE_FILES "src/*.cpp" "test/*.cpp")

list(REMOVE_ITEM TEST_SOURCE_FILES "${MAIN_CPP}")

add_executable(${TEST_PROJECT_NAME} ${TEST_SOURCE_FILES})
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_dependencies(${PROJECT_NAME} ${TEST_PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

find_package(Boost 1.57.0 COMPONENTS ${BOOST_COMPONENTS}
        unit_test_framework REQUIRED)

target_link_libraries(${TEST_PROJECT_NAME} ${Boost_LIBRARIES})

include(CTest)

enable_testing()

add_test(NAME "caster_test" COMMAND ${TEST_PROJECT_NAME})


