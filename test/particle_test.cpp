/**
 * @file particle_tests.cpp
 * @author Zephirin Faure (zephirin.faure@grenoble-inp.org)
 * @brief Unit tests for the Particle class.
 * 
 * This file contains a suite of unit tests for the Particle class, which tests 
 * the constructor, various member functions, and operator overloads.
 * 
 * @version 1.0
 * @date 2024-04-19
 */

#include <gtest/gtest.h>
#include <particle.hpp>
#include <vector.hpp>
#include <interraction.hpp>
#include <math.h>

/**
 * @brief Test particle setup for all tests.
 */
Vector pos({1.0, 2.0, 3.0});
Vector speed({0.1, 0.2, 0.3});
double mass = 2.5;
std::string name = "TestParticle";
Particle p(pos, speed, mass, name);

/**
 * @brief Test the constructor and output operator.
 * 
 * This test checks that the Particle constructor initializes
 * the particle correctly and that the output operator prints
 * the expected output.
 */
TEST(ParticleTest, ConstructorAndOutputOperator) {
    std::ostringstream oss;
    oss << p;

    std::string expectedOutput =
        "Particle 0\n"
        "    name = TestParticle\n"
        "    mass = 2.5\n"
        "    position = (1, 2, 3)\n"
        "    speed    = (0.1, 0.2, 0.3)\n"
        "    force = (0, 0, 0)";

    EXPECT_EQ(oss.str(), expectedOutput);
}

/**
 * @brief Test the distanceTo function.
 * 
 * This test checks that the distanceTo function calculates
 * the correct distance between two particles.
 */
TEST(ParticleTest, DistanceTo) {
    Vector pos2({4.0, 5.0, 6.0});
    Vector speed2({0.0, 0.0, 0.0});
    Particle p2(pos2, speed2, 1.0, "");

    double expectedDistance = std::sqrt(27.0);

    EXPECT_EQ(p.distanceTo(p2), expectedDistance);
}

/**
 * @brief Test the invertSpeed function.
 * 
 * This test checks that the invertSpeed function correctly
 * inverts the speed in the specified dimension.
 */
TEST(ParticleTest, InvertSpeed) {
    // Invert speed in the first dimension
    p.invertSpeed(0);

    // Expected speed after inversion
    Vector expectedSpeed({-0.1, 0.2, 0.3});

    EXPECT_EQ(p.getSpeed(), expectedSpeed);
}

/**
 * @brief Test the setPosCoord function.
 * 
 * This test checks that the setPosCoord function correctly
 * sets the position coordinate at the specified index.
 */
TEST(ParticleTest, SetPosCoord) {
    // Set position coordinate at index 1 to 5.0
    p.setPosCoord(1, 5.0);

    // Expected position after setting coordinate
    Vector expectedPos({1.0, 5.0, 3.0});

    EXPECT_EQ(p.getPosition(), expectedPos);
}

/**
 * @brief Test the addToForceCoord function.
 * 
 * This test checks that the addToForceCoord function correctly
 * adds the specified value to the force coordinate at the specified index.
 */
TEST(ParticleTest, AddToForceCoord) {
    // Add value 2.0 to force coordinate at index 0
    p.addToForceCoord(0, 2.0);

    // Expected force after adding value
    Vector expectedForce({2.0, 0.0, 0.0});

    EXPECT_EQ(p.getForce(), expectedForce);
}

/**
 * @brief Test the addToForce function.
 * 
 * This test checks that the addToForce function correctly
 * adds the specified vector to the particle's force.
 */
TEST(ParticleTest, AddToForce) {
    // Add vector to particle's force
    p.addToForce({0.0, 3.0, 4.0});

    // Expected force after adding vector
    Vector expectedForce({2.0, 3.0, 4.0});

    EXPECT_EQ(p.getForce(), expectedForce);
}

/**
 * @brief Test the addToPosition function.
 * 
 * This test checks that the addToPosition function correctly
 * adds the specified vector to the particle's position.
 */
TEST(ParticleTest, AddToPosition) {
    // Add vector to particle's position
    p.addToPosition({2.0, 0.0, 4.0});

    // Expected position after adding vector
    Vector expectedPosition({3.0, 5.0, 7.0});

    EXPECT_EQ(p.getPosition(), expectedPosition);
}

/**
 * @brief Test the addToSpeed function.
 * 
 * This test checks that the addToSpeed function correctly
 * adds the specified vector to the particle's speed.
 */
TEST(ParticleTest, AddToSpeed) {
    // Add vector to particle's speed
    p.addToSpeed({0.7, 0.5, 0.5});

    // Expected speed after adding vector
    Vector expectedSpeed({0.6, 0.7, 0.8});

    EXPECT_EQ(p.getSpeed(), expectedSpeed);
}

/**
 * @brief Test the setForceToZero function.
 * 
 * This test checks that the setForceToZero function correctly
 * resets the particle's force to zero.
 */
TEST(ParticleTest, SetForceToZero) {
    // Reset force to zero
    p.setForceToZero();

    // Expected force after resetting
    Vector expectedForce({0.0, 0.0, 0.0});

    EXPECT_EQ(p.getForce(), expectedForce);
}
