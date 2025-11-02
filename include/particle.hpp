/**
 * @file Particle.hpp
 * @author jules roques (jules.roques@grenoble-inp.org)
 * @brief
 * @version 0.1
 * @date 2024-04-07
 */

#ifndef _PARTICLE_HPP_
#define _PARTICLE_HPP_

#include <list>
#include <string>

#include "external_force.hpp"
#include "interraction.hpp"
#include "vector.hpp"

class Particle {
 private:
  size_t _dimension;
  Vector _position;
  Vector _speed;
  Vector _force;
  Vector _oldForce;
  double _mass;
  std::string _name;
  int _id;
  static int
      _particleCount;  // Static variable to count number or created particles

 public:
  /**
   * @brief Construct a new Particle object
   * @param dimension dimension of the particle
   * @param pos
   * @param speed
   * @param mass
   * @param name
   */
  Particle(Vector pos, Vector speed, double mass, std::string name);

  // Getters
  static int getParticleCount() { return _particleCount; }
  size_t getDimension() const { return _dimension; }
  const Vector& getPosition() const { return _position; }
  const Vector& getSpeed() const { return _speed; }
  const Vector& getForce() const { return _force; }
  const Vector& getOldForce() const { return _oldForce; }
  double getMass() const { return _mass; }
  const std::string& getName() const { return _name; }

  // Setters
  void setPosCoord(size_t coord, double value);
  void setPosition(const Vector& pos) { _position = pos; }
  void setSpeed(const Vector& speed) { _speed = speed; }
  void setForce(const Vector& force) { _force = force; }
  void setOldForce(const Vector& oldForce) { _oldForce = oldForce; }

  /**
   * @brief Multiply the speed by a scalar
   * @param scalar
   */
  void multiplySpeed(double scalar) { _speed *= scalar; }

  /**
   * @brief Adds to the i th force coordinate the value given
   * @param i
   * @param value
   */
  void addToForceCoord(size_t i, double value);

  /**
   * @brief Adds to existing force the vector given
   * @param vect
   */
  void addToForce(const Vector& vect);

  /**
   * @brief Adds to existing position the vector given
   * @param vect
   */
  void addToPosition(const Vector& vect);

  /**
   * @brief Adds to existing speed the vector given
   * @param vect
   */
  void addToSpeed(const Vector& vect);

  /**
   * @brief Set the force of particle to zero
   */
  void setForceToZero();

  /**
   * @brief Overrides the << operator
   * @param strm
   * @param p
   * @return std::ostream&
   */
  friend std::ostream& operator<<(std::ostream& strm, const Particle& p);

  /**
   * @brief Calculates distance from this particle to the other
   *
   * @param other
   * @return double
   */
  double distanceTo(const Particle& other) const;

  /**
   * @brief invert the speed coordinate i
   * @param i
   */
  void invertSpeed(size_t i);

  /**
   * @brief Apply to a particle the extarnel forces
   *        (adds to existing force)
   * @param extForces
   */
  void applyExternalForces(const std::list<ExternalForce>& extForces);

  /**
   * @brief Apply to a particle the force
   *        implied by calling particle
   *        (adds to existing force)
   * @param other the particle to apply force on
   */
  void applyInteractionForcesOn(
      Particle& other, const std::list<Interaction>& interactions) const;
};

#endif  // _PARTICLE_HPP_