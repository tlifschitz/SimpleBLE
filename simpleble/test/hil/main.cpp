
#include <gtest/gtest.h>
#include "environment.hpp"

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    ::testing::AddGlobalTestEnvironment(&SimpleBLEHilTest::getInstance());

    auto test_result = RUN_ALL_TESTS();
    return test_result;
}
