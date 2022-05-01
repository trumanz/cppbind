#include <stdio.h>
#include "gtest/gtest.h"
#include <filesystem>

int main(int argc, char *argv[]){
    std::cout << "Current Path:" << std::filesystem::current_path()  << std::endl;

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
