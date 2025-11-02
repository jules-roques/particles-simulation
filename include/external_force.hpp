/**
 * @file force.hpp
 * @author jules roques (jules.roques@grenoble-inp.org)
 * @brief to define a force applied on any particle
 * @version 0.1
 * @date 2024-04-22
 */

#ifndef _FORCE_HPP_
#define _FORCE_HPP_

#include <functional>

#include "vector.hpp"
class Particle;

/**
 * @brief Stores a function that rules a force applied on particles
 */
class ExternalForce {
 private:
  /* Function containing the force */
  std::function<void(Particle&)> _forceFunction;

 public:
  ExternalForce() {}

  ExternalForce(std::function<void(Particle&)> forceFunction)
      : _forceFunction(forceFunction) {}

  void setForceFunction(std::function<void(Particle&)> forceFunction) {
    _forceFunction = forceFunction;
  }

  /**
   * @brief Computes and add the force applied on the given particle
   * @param target
   * @return Vector
   */
  void applyOn(Particle& target) const { _forceFunction(target); }
};

#endif  // _FORCE_HPP_
