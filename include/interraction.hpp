/**
 * @file interraction.hpp
 * @author jules roques (jules.roques@grenoble-inp.org)
 * @brief to define an interraction between two particles
 * @version 0.1
 * @date 2024-04-12
 */

#ifndef _INTERRACTION_HPP_
#define _INTERRACTION_HPP_

#include <functional>

class Particle;

/**
 * @brief Store a function that rules an interraction between particles
 */
class Interaction {
 private:
  /* function containing the interaction */
  std::function<void(const Particle&, Particle&)> _interactionFunction;

 public:
  Interaction(
      std::function<void(const Particle&, Particle&)> interactionFunction)
      : _interactionFunction(interactionFunction) {}

  /**
   * @brief Computes and applies the force implied by source on target
   * @param source the particle that applies force
   * @param target the particle that receives
   * @return Vector
   */
  void operator()(const Particle& source, Particle& target) const {
    _interactionFunction(source, target);
  }
};

#endif  // _INTERRACTION_HPP_