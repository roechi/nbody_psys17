//
// Created by Richard Remus on 04.05.17.
//

/**
 * This is the main test runner.
 */

#include "gtest/gtest.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


