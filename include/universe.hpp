/**
 * @file Universe.h
 * @author Zeph
 * @brief Définition de la classe univers
 * @version 0.1
 * @date 2024-04-07
 */

#ifndef _UNIVERSE_HPP_
#define _UNIVERSE_HPP_

#include <vector>
#include <functional>

#include "particle.hpp"
#include "vector.hpp"
#include "cell.hpp"
#include "interraction.hpp"
#include "external_force.hpp"

/**
 * @class Universe
 * @brief Represents an universe containing particles.
 *        Can be in dimension 1, 2, or 3.
 */
class Universe {

private:
   size_t _dimension; // Dimension of the universe

   /* Store in list (doubly linked list) because
      particles of the universe will be deleted
      if they quit the GriddedUniverse domain.
      So we need constant time complexity
      for deletion. Plus we will never try to reach
      only one particle in the list, we will always 
      iterate through all of them. */
   std::list<Particle> _particles;

   /* past particles in the universe */
   size_t _nbPastStates = 0;
   const char* _pastParticlesFileName = "pastParticles.txt";

   /* list of interactions between particles.
      For exemple can contain gravitational interraction
      and Lennard Jones interraction. */
   std::list<Interaction> _interactions;

   /* list of forces applied on any particle
      (not an interaction).
      For exemple the gravition field. */
   std::list<ExternalForce> _forces;

   // Extremum values that particles had been into
   Vector _minPosition;
   Vector _maxPosition;
   double _maxForce;

   /* Cinetic energy limit for the system, c.f. TP6
      Avoid speed divergence of particles */
   double _cineticEnergyLimit = 100000;
<<<<<<< HEAD
=======

   /**
    * @brief Applies forces between particles in the universe.
    *        Quadratic complexity.
    */
   virtual void applyInteractionForces();
>>>>>>> 22b10cc0c87503308962e9d792e920a26a3dc432

   /**
    * @brief Set all forces applied on particles
    *        to zero in the universe;
    */
   void setForcesToZero();

   /**
    * @brief Calculates the current cinetic energy of the system.
    *        Half the sum of the particles masses times their speed norm squared.
    *        O(n) complexity.
    * @return double 
    */
   double currentCineticEnergy() const;

   /**
    * @brief updates extremum values such as positions and force
    */
   void updatesExtremumValues();

   /**
    * @brief Updates particles paces
    *        in Stormer Verlet algorithm
    */
   void updatePaces(double timeStep);

   /* Friend class access private fields for
      universe visualisation */
   friend class VisualGenerator;

protected:

   /**
    * @brief Get list of particles reference
    * @return std::list<Particle>& 
    */
   std::list<Particle>& getParticles() { return _particles; }

   /**
    * @brief Get the bounds of the universe,
    *        i.e. the min and max vectors of all past particles
    *        (not present particles) 
    * @return const std::pair<Vector, Vector>& 
    */
   virtual std::pair<Vector, Vector> getBounds() const;

   /**
    * @brief Get the Last Particle Pointer
    *        Useful in the method addParticle
    *        of GriddedUniverse
    * @return Particle* 
    */
   Particle* getLastAddedParticlePointer() { return &_particles.back(); }

   /**
    * @brief Updates particles positions
    *        in Stormer Verlet algorithm
    */
   virtual void updatePositions(double timeStep);

   /**
    * @brief Updates forces applied on particles in the universe.
    *        (Forces from interactions and external forces)
    */
   virtual void updateForces();

   /**
    * @brief Applies external forces on particles in the universe.
    *        Linear complexity.
    */
   virtual void applyExternalForces();

   /**
    * @brief Applies forces between particles in the universe.
    *        Quadratic complexity.
    */
   virtual void applyInteractionForces();

public:
   /**
    * @brief Constructor of class Universe
    * @param dimension Universe dimension (1D, 2D or 3D)
    */
   Universe(size_t dimension);

   /**
    * @brief Get the number of states the universe has been in
    *        in the past (not including present).
    * @return size_t 
    */
   size_t getNbPastStates() const { return _nbPastStates; }

   /**
    * @brief Get the universe interactions
    * @return const std::list<Interaction>& 
    */
   const std::list<Interaction>& getInteractions() const { return _interactions; }

   /**
    * @brief Get the dimension of the universe
    * @return size_t 
    */
   size_t getDimension() const { return _dimension; }

   /**
    * @brief Get the maximum force any particle has never felt
    * @return double 
    */
   double getMaxForce() const { return _maxForce; }

   /**
    * @brief Adds a particle into the universe
    * @param dimension 
    * @param pos 
    * @param speed 
    * @param mass 
    * @param name 
    */
   virtual void addParticle(Vector pos,
                           Vector speed,
                           double mass, std::string name);
   
   /**
    * @brief Adds a particle into the universe
    * @param dimension 
    * @param posCoords
    * @param speedCoords
    * @param mass 
    * @param name 
    */
   void addParticle(std::initializer_list<double> posCoords,
                  std::initializer_list<double> speedCoords,
                  double mass, std::string name);

   /**
    * @brief Adds a particle into the universe.
    *        Give it automaticly a name.
    * @param pos 
    * @param speed 
    * @param mass 
    */
   void addParticle(Vector pos,
                  Vector speed,
                  double mass);

   /**
    * @brief Adds a particle into the universe.
    *        Give it automaticly a name.
    * @param posCoords
    * @param speedCoords
    * @param mass 
    */
   void addParticle(std::initializer_list<double> posCoords,
                  std::initializer_list<double> speedCoords,
                  double mass);
   

   /**
    * @brief Adds interaction between two particles
    *        in the universe.
    *        The force function must compute the force
    *        applied by the 1rst argument particle on the 2nd.
    * @param interactionFunction
    */
   void addInteraction(std::function<void(const Particle&, Particle&)> interactionFunction);

   /**
    * @brief Adds force on particles in the universe.
    * @param forceFunction
    */
   void addExternalForce(std::function<void(Particle&)> forceFunction);
   
   /**
    * @brief Renvoie le nombre de particules dans l'univers
    * @return Le nombre de particules dans l'univers
    */
   int getNbParticles() const { return _particles.size(); }

   /**
    * @brief Simulates the movement of particles in the universe using the Störmer-Verlet method
    * @param timeStep
    * @param finalTime End time of the simulation
    */
   virtual void simulateStormerVerlet(double timeStep, double finalTime);

   /**
    * @brief Surcharge de l'opérateur de flux de sortie pour afficher les informations sur l'univers
    * @param strm Le flux de sortie
    * @param univers L'univers à afficher
    * @return std::ostream&, le flux de sortie
    */
   friend std::ostream& operator<<(std::ostream &strm, const Universe &univers);

   /**
    * @brief Set the total Cinetic Energy Limit for the universe.
    *        Avoids speed divergence.
    * @param cel 
    */
   void setCineticEnergyLimit(double cineticEnergyLimit) { _cineticEnergyLimit = cineticEnergyLimit; }
};

#endif // _UNIVERSE_HPP_
   