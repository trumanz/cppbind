cmake_minimum_required(VERSION 3.10.2)
set(CMAKE_VERBOSE_MAKEFILE ON)



# Enable common compiler warnings
#if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
#elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
#endif()


# Treat all warnings as errors
#if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
#elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
#endif()



project(cppbind_test)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)

find_package(GTest CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)

set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -g -O0  -fno-inline")
#set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} --coverage -g -O0  -fno-inline")
set(CMAKE_CXX_STANDARD 14)

function(add_source_files dir regex_name dst)
    file(GLOB_RECURSE current_files ${dir}/${regex_name})
    set(${dst} ${current_files} ${${dst}} PARENT_SCOPE)
    if(MSVC)
        source_group(TREE ${dir} FILES ${current_files})
    endif()
endfunction()

#message("x " ${CMAKE_CURRENT_SOURCE_DIR})

add_source_files(${CMAKE_CURRENT_SOURCE_DIR}/test/src *.cpp  CPP_FILES)


add_executable(cppbind_test ${CPP_FILES})
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_include_directories(cppbind_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_link_libraries(cppbind_test PRIVATE
        GTest::gmock GTest::gtest
        GTest::gmock_main GTest::gtest_main
        Boost::filesystem)


if(APPLE)
    message(STATUS "Compile on Apple")

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(JSONCPP REQUIRED jsoncpp)
    include_directories(${JSONCPP_INCLUDE_DIRS})
    link_directories(${JSONCPP_LIBRARY_DIRS})
    message(STATUS "JSONCPP_INCLUDE_DIRS: ${JSONCPP_INCLUDE_DIRS}")
    message(STATUS "JSONCPP_LIBRARY_DIRS: ${JSONCPP_LIBRARY_DIRS}")
    message(STATUS "JSONCPP_LIBRARIES: ${JSONCPP_LIBRARIES}")
    target_link_libraries(cppbind_test PRIVATE /opt/homebrew/lib/libjsoncpp.dylib)
else()
    find_package(jsoncpp CONFIG REQUIRED)
    target_link_libraries(cppbind_test PRIVATE jsoncpp_static)

endif()


# Custom target to generate preprocessed files for all .cpp files
foreach(file ${CPP_FILES})
    get_filename_component(file_name ${file} NAME_WE)
    add_custom_target(preprocess_${file_name}
            COMMAND ${CMAKE_CXX_COMPILER} -E ${file} -o ${CMAKE_CURRENT_BINARY_DIR}/${file_name}_preprocessed.cpp
            DEPENDS cppbind_test
            COMMENT "Generating preprocessed file ${file_name}_preprocessed.cpp"
    )
endforeach()

# create directory for gcov coverage reports
add_custom_target(coverage_dir ALL
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/coverage)

# run test executable to generate .gcno and .gcda files
add_custom_target(run_test ALL
        COMMAND $<TARGET_FILE:cppbind_test>
        DEPENDS cppbind_test coverage_dir
        )

# run gcov to generate report
add_custom_target(compute_coverage ALL
        COMMAND gcovr --sonarqube coverage.xml  --exclude-throw-branches -r ${CMAKE_CURRENT_SOURCE_DIR}/
        DEPENDS run_test
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/coverage
        )


