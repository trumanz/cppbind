cmake_minimum_required(VERSION 3.10.2)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(cppbind_test)

function(add_source_files dir regex_name dst)
    file(GLOB_RECURSE current_files ${dir}/${regex_name})
    set(${dst} ${current_files} ${${dst}} PARENT_SCOPE)
    if(MSVC)
        source_group(TREE ${dir} FILES ${current_files})
    endif()
endfunction()

#message("x " ${CMAKE_CURRENT_SOURCE_DIR})

add_source_files(${CMAKE_CURRENT_SOURCE_DIR}/test/src *.cpp  CPP_FILES)


set(CMAKE_CXX_STANDARD 14)

add_executable(cppbind_test ${CPP_FILES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)


find_package(GTest CONFIG REQUIRED)
target_link_libraries(cppbind_test PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)
find_package(jsoncpp CONFIG REQUIRED)
target_link_libraries(cppbind_test PRIVATE jsoncpp_object jsoncpp_static)
