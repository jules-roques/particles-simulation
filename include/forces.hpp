/**
 * @file forces.hpp
 * @author jules roques (jules.roques@grenoble-inp.org)
 * @brief File containing different forces functions
 * @version 0.1
 * @date 2024-04-12
 */

#ifndef _FORCES_HPP_
#define _FORCES_HPP_

#include <vector.hpp>
#include <particle.hpp>
#include <cmath>
#include <xassert.hpp>

/**
 * @brief Computes the gravitational force applied by
 *        source on target particle.
 * @param source 
 * @param target 
 * @return Vector 
 */
void gravitationalInteraction(const Particle &source, Particle& target);

/**
 * @brief Computes the Lennard Jones force applied by
 *        source on target particle.
 * @param source 
 * @param target 
 * @param epsilon 
 * @param sigma 
 * @return Vector 
 */
void lennardJonesInteraction(const Particle& source, Particle& target,
                           double epsilon, double sigma);

/**
 * @brief Adds the gravitational force applied on a particle to the existing force.
 *        The gravitational field is applied on the last dimension ot the particle.
 * @param source 
 * @param target 
 */
void gravitationalForce(Particle& target, double G);


/**
 * @brief Applies on the particle the strength of the walls (c.f. TP6)
 * @param target 
 * @param lowerBound 
 * @param upperBound 
 */
void wallsForce(Particle& target, Vector lowerBound, Vector upperBound, double epsilon, double sigma);



#endif // _FORCES_HPP_