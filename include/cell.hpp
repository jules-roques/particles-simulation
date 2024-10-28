/**
 * @file cell.hpp
 * @author jules roques (jules.roques@grenoble-inp.org)
 * @brief Les cellules sont les cases élémentaires formant
 *        un maillage de l'univers
 * @version 0.1
 * @date 2024-04-08
 */

#ifndef _CELL_HPP_
#define _CELL_HPP_

#include <list>

#include "vector.hpp"
#include "particle.hpp"

class ExternBorderCell;
class InternCell;
class Universe;
class GriddedUniverse;

/**
 * @brief Cells are elementary bricks
 *        forming a grid of the universe
 */
class Cell {
private:
   size_t _dimension;
   GriddedUniverse* _universe; // Containing universe

   /* Coordinates of the cell in its universe.
      Can ve negative or out of gridded universe _dimensions
      for _limitsCells. */
   std::vector<int> _coordinates;

   /* Store in list (doubly linked list) because
      we will never try to get one neighbour alone,
      we will always iterate through all of them.
      But we will never insert in the middle or delete,
      so we could have use vector. */
   std::list<InternCell*> _neighbours;

   /**
    * @brief Apply to a particle the force
    *        implied by particles in the cell
    *        (adds to existing force)
    * @param p the particle to apply forces on
    */
   virtual void applyForceOnParticle(Particle* p) const = 0;

protected:
   Cell(size_t dimension, GriddedUniverse* universe, std::vector<int> coordinates);

   // Getters
   std::list<InternCell*> getNeighbours() const { return _neighbours; }
   GriddedUniverse*       getUniverse()   const { return _universe;   }
   size_t                 getDimension()        { return _dimension;  }

public:
   // Public getters
   const std::vector<int>& getCoordinates() const { return _coordinates; }

   /**
    * @brief Adds a cell to the neighbours
    * @param neighbour pointer to cell
    */
   void addNeighbour(InternCell* neighbour);

   /**
     * @brief applies forces on neighours
     *        particles (adds to existing forces)
     */
   virtual void applyForceOnNeighbours() const = 0;

   /**
    * @brief Delete particles
    */
   virtual void clearParticles() = 0;

   size_t getNbNeighbours() const { return _neighbours.size(); }

};



#endif // _CELL_HPP_