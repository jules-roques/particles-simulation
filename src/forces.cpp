#include "forces.hpp"

#include <cmath>
#include <particle.hpp>
#include <vector.hpp>
#include <xassert.hpp>

void gravitationalInteraction(const Particle& source, Particle& target) {
  xassert(&source != &target,
          "Cannot compute force if particles given are the same.");
  double r = source.distanceTo(target);
  Vector force = source.getPosition();
  force -= target.getPosition();
  force *= (source.getMass() * target.getMass() / std::pow(r, 3));
  target.addToForce(force);
}

void lennardJonesInteraction(const Particle& source, Particle& target,
                             double epsilon, double sigma) {
  xassert(&source != &target,
          "Cannot compute force if particles given are the same.");
  double r = source.distanceTo(target);
  double power_6_term = pow(sigma / r, 6);
  Vector force = source.getPosition();  // Copy
  force -= target.getPosition();
  force *= 24 * epsilon * 1 / pow(r, 2) * power_6_term * (1 - 2 * power_6_term);
  target.addToForce(force);
}

void gravitationalForce(Particle& target, double G) {
  double weigth = -target.getMass() * G;
  size_t coord = target.getDimension() - 1;
  target.addToForceCoord(coord, weigth);
}

static inline double FWall(double r, double epsilon, double sigma) {
  double power_6_term = pow(sigma / r / 2, 6);
  return 24 * epsilon / 2 / r * power_6_term * (1 - 2 * power_6_term);
}

void wallsForce(Particle& target, Vector lowerBound, Vector upperBound,
                double epsilon, double sigma) {
  xassert(target.getDimension() == lowerBound.getDimension() &&
              target.getDimension() == upperBound.getDimension(),
          "Particle and bounds must have the same dimension.");
  xassert(target.getPosition().isInBounds(lowerBound, upperBound),
          "particle must be inside the walls.");

  double r_cut = sigma * pow(2, 1.0 / 6);
  size_t dim = target.getDimension();
  const Vector& pos = target.getPosition();
  for (size_t i = 0; i < dim; i++) {
    double r = pos[i] - lowerBound[i];
    if (r < r_cut) {
      target.addToForceCoord(i, -FWall(r, epsilon, sigma));
    }
    r = upperBound[i] - pos[i];
    if (r < r_cut) {
      target.addToForceCoord(i, FWall(r, epsilon, sigma));
    }
  }
}