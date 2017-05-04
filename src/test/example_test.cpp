//
// Created by Richard Remus on 04.05.17.
//

#include "example_test.h"
#include "gtest/gtest.h"

TEST(ExampleSuite, ExampleSuite_ShouldBeEqual_Test) {
    ASSERT_EQ(1, 1);
}

void throwSomeThing(int something)
{
    throw something;
}

TEST(ExampleSuite, ExampleSuite_SecondExample_Test) {
    ASSERT_ANY_THROW(throwSomeThing(1));
}

