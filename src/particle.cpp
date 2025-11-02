#include <math.h>

#include <interraction.hpp>
#include <iostream>
#include <particle.hpp>
#include <string>
#include <vector.hpp>
#include <xassert.hpp>

/* ------------------------------- private ------------------------------- */

/* ------------------------------- public ------------------------------- */

// Initialize static variable outside the class
int Particle::_particleCount = 0;

Particle::Particle(Vector pos, Vector speed, double mass, std::string name) {
  xassert(pos.getDimension() == speed.getDimension(),
          "Position and speed dimensions must match.");

  _dimension = pos.getDimension();
  _position = pos;
  _speed = speed;
  _force = Vector(_dimension);
  _oldForce = Vector(_dimension);
  _mass = mass;
  _name = name;
  _id = _particleCount;
  _particleCount++;  // Incrémente le compteur à chaque création d'instance
}

std::ostream& operator<<(std::ostream& strm, const Particle& p) {
  return strm << "Particle " << p._id << std::endl
              << "    name = " << p._name << std::endl
              << "    mass = " << p._mass << std::endl
              << "    position = " << p._position << std::endl
              << "    speed    = " << p._speed << std::endl
              << "    force = " << p._force;
}

double Particle::distanceTo(const Particle& other) const {
  Vector diff = other._position;
  diff -= _position;
  return diff.norm();
}

void Particle::applyExternalForces(const std::list<ExternalForce>& extForces) {
  for (const ExternalForce& force : extForces) {
    force.applyOn(*this);
  }
}

void Particle::applyInteractionForcesOn(
    Particle& other, const std::list<Interaction>& interactions) const {
  xassert(this != &other,
          "Force calculation must be applied on two different particles.");

  for (const Interaction& interaction : interactions) {
    interaction(*this, other);
  }
}

void Particle::invertSpeed(size_t i) {
  xassert(i < _dimension, "i coordinate do not exist for this particle.");
  _speed[i] *= -1;
}

void Particle::setPosCoord(size_t coord, double value) {
  xassert(coord < _dimension, "coord is too high.");
  _position[coord] = value;
}

void Particle::addToForceCoord(size_t coord, double value) {
  xassert(coord < _dimension, "coord is too high.");
  _force[coord] += value;
}

void Particle::addToForce(const Vector& vect) {
  xassert(vect.getDimension() == _dimension,
          "Vector and particle dimension must match.");
  _force += vect;
}

void Particle::addToPosition(const Vector& vect) {
  xassert(vect.getDimension() == _dimension,
          "Vector and particle dimension must match.");
  _position += vect;
}

void Particle::addToSpeed(const Vector& vect) {
  xassert(vect.getDimension() == _dimension,
          "Vector and particle dimension must match.");
  _speed += vect;
}

void Particle::setForceToZero() {
  for (size_t i = 0; i < _dimension; i++) {
    _force[i] = 0;
  }
}