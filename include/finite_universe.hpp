/**
 * @file finite_universe.hpp
 * @author jules roques (jules.roques@grenoble-inp.org)
 * @brief A universe with limits
 * @version 0.1
 * @date 2024-04-13
 */

#ifndef _FINITE_UNIVERSE_HPP_
#define _FINITE_UNIVERSE_HPP_

#include "universe.hpp"

/* Enum type to define how
   particles react when overpassing
   bounds of universe */
enum OOBBehavior { PERIODIC, REFLEXION, ABSORPTION };

/**
 * @brief Universe with bounds
 */
class FiniteUniverse : public Universe {
 private:
  // Bounds of the universe
  Vector _lowerBound;
  Vector _upperBound;

  /* How particles will react passing bounds (oob = out of bouds) */
  OOBBehavior _oobbehavior = ABSORPTION;  // ABSORPTION by default

  /* Repulsing force from the walls if needed */
  ExternalForce _wallsForce;
  bool _applyWallsForce = false;

  /**
   * @brief Applies repulsing force from walls
   */
  void applyWallsForces();

  /**
   * @brief Applies the force implied by the foreign
   *        neighbours on the particles.
   *        Foreign neighbours are cells that are close
   *        for a PERIODIC universe point of view,
   *        but with foreign coordinates.
   */
  virtual void applyForeignNeighboursForces();

 protected:
  // Getters
  const Vector& getLowerBound() const { return _lowerBound; }
  const Vector& getUpperBound() const { return _upperBound; }
  std::pair<Vector, Vector> getBounds() const override;

  /**
   * @brief Removes particles outside of the bounds
   *        of the finite universe
   */
  void removeOutOfBoundsParticles();

  /**
   * @brief Reflect particles outside of the bounds
   *        of the finite universe
   */
  void reflectOutOfBoundsParticles();

  /**
   * @brief Teleport particles outside of the bounds
   *        of the finite universe to the other side
   */
  void teleportOutOfBoundsParticles();

  /**
   * @brief Handle out of bounds particles
   */
  void handleOutOfBoundsParticles();

  OOBBehavior getoobbehavior() const { return _oobbehavior; }

  void setApplyWallsForces(bool apply) { _applyWallsForce = apply; }

  /**
   * @brief Applies external forces on particles in the universe.
   *        Linear complexity.
   *        Deal with limits forces.
   */
  void applyExternalForces() override;

  /**
   * @brief Applies forces between particles in the universe.
   *        Quadratic complexity.
   */
  void applyInteractionForces() override;

  /**
   * @brief Applies forces between particles in the universe,
   *        but not limit interractions (like for PERIODIC)
   */
  virtual void applyInternInterractionsForces();

  /**
   * @brief Deal with limits forces, for exple
   *        when the universe is PERIODIC.
   */
  void applyLimitinterractionForces();

 public:
  FiniteUniverse(Vector lowerBound, Vector upperBound);

  /* So the following function addParticle don't hide
     The Universe::addParticle function with different signature */
  using Universe::addParticle;
  /**
   * @brief Adds a particle to the gridded universe
   * @param p the particle to add
   */
  virtual void addParticle(Vector posCoords, Vector speedCoords, double mass,
                           std::string name) override;

  /**
   * @brief Set the Limit Behavior of particles.
   *        REFLEXION makes particle stay in,
   *        ABSORPTION delete particles,
   *        PERIODIC teleports particles to the other side.
   * @param lb
   */
  void setOOBBehavior(OOBBehavior lb);

  /**
   * @brief Says if particle is in the bounds of the universe
   * @param p
   * @return true if particle in the bounds of the universe
   * @return false if not
   */
  bool isInBounds(const Particle& p);

  /**
   * @brief Updates particles positions
   *        in Stormer Verlet algorithm.
   *        Handles cases about
   *        being outside of the finite universe.
   */
  virtual void updatePositions(double timeStep) override;

  /**
   * @brief Activate reflexion mode with a repulsing force
   *        from the walls
   * @param epsilon
   * @param sigma
   */
  void activateReflexionWithForces(double epsilon, double sigma);

  /**
   * @brief Overrides the << operator
   * @param strm
   * @param universe
   * @return std::ostream&
   */
  friend std::ostream& operator<<(std::ostream& strm, FiniteUniverse universe);
};

#endif  // _FINITE_UNIVERSE_HPP_