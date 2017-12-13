#include <algorithm>
#include "gtest/gtest.h"

extern "C" {
    #include "../src/fa.h"
}

/*
 *
 */

TEST(fa_structure, fa_create_test_1)
{
    fa *fa = (struct fa*) malloc(sizeof fa);
    fa_create(fa,2,5);

    EXPECT_NE(fa, nullptr);
    EXPECT_EQ(fa->alpha_count, 2);
    EXPECT_EQ(fa->state_count, 5);
}
