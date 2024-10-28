/**
 * @file vector_tests.cpp
 * @author Zephirin Faure (zephirin.faure@grenoble-inp.org)
 * @brief Unit tests for the Vector class.
 * 
 * This file contains a suite of unit tests for the Vector class, which tests 
 * vector addition, subtraction, scalar multiplication, equality, norm calculation, 
 * and min/max functions.
 * 
 * @version 1.0
 * @date 2024-04-19
 */

#include <gtest/gtest.h>
#include <math.h>
#include <sstream>
#include <vector.hpp>

/**
 * @brief Test vector addition.
 * 
 * This test checks that the vector addition operator correctly
 * adds two vectors element-wise.
 */
TEST(VectorTest, Addition) {
    Vector v1({4.0, 5.0, 6.0});
    Vector v2({1.0, 2.0, 3.0});
    Vector expected({5.0, 7.0, 9.0});
    v1 += v2;

    EXPECT_EQ(v1, expected);
}

/**
 * @brief Test vector subtraction.
 * 
 * This test checks that the vector subtraction operator correctly
 * subtracts one vector from another element-wise.
 */
TEST(VectorTest, Subtraction) {
    Vector v1({4.0, 5.0, 6.0});
    Vector v2({1.0, 2.0, 3.0});
    Vector expected({3.0, 3.0, 3.0});
    v1 -= v2;

    EXPECT_EQ(v1, expected);
}

/**
 * @brief Test vector scalar multiplication.
 * 
 * This test checks that the scalar multiplication operator correctly
 * multiplies each element of the vector by the scalar.
 */
TEST(VectorTest, MultScalar) {
    Vector v({1.0, 2.0, 3.0});
    double scalar = 2.0;
    Vector expected({2.0, 4.0, 6.0});
    v *= scalar;

    EXPECT_EQ(v, expected);
}

/**
 * @brief Test vector equality operator.
 * 
 * This test checks that the equality operator correctly
 * determines if two vectors are equal element-wise.
 */
TEST(VectorTest, Equality) {
    Vector v1({1.0, 2.0, 3.0});
    Vector v2({1.0, 2.0, 3.0});

    EXPECT_EQ(v1, v2);
}

/**
 * @brief Test norm calculation.
 * 
 * This test checks that the norm function correctly
 * calculates the Euclidean norm of the vector.
 */
TEST(VectorTest, Norm) {
    Vector v({1.0, 2.0, 2.0});
    double expectedNorm = std::sqrt(1.0*1.0 + 2.0*2.0 + 2.0*2.0);

    EXPECT_EQ(v.norm(), expectedNorm);
}

/**
 * @brief Test min function.
 * 
 * This test checks that the min function correctly
 * returns a vector consisting of the element-wise minimum values.
 */
TEST(VectorTest, MinFunction) {
    Vector v1({1.0, 2.0, 3.0});
    Vector v2({0.0, 3.0, -1.0});
    Vector expected({0.0, 2.0, -1.0});

    EXPECT_EQ(min(v1, v2), expected);
}

/**
 * @brief Test max function.
 * 
 * This test checks that the max function correctly
 * returns a vector consisting of the element-wise maximum values.
 */
TEST(VectorTest, MaxFunction) {
    Vector v1({1.0, 2.0, 3.0});
    Vector v2({0.0, 3.0, -1.0});
    Vector expected({1.0, 3.0, 3.0});

    EXPECT_EQ(max(v1, v2), expected);
}

/**
 * @brief Test output stream operator.
 * 
 * This test checks that the output stream operator correctly
 * prints the vector in the expected format.
 */
TEST(VectorTest, OutputOperator) {
    Vector v({1.0, 2.0, 3.0});
    std::ostringstream oss;
    oss << v;

    std::string expectedOutput = "(1, 2, 3)";
    EXPECT_EQ(oss.str(), expectedOutput);
}
