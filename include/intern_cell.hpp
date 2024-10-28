/**
 * @file InternCell.hpp
 * @author jules roques (jules.roques@grenoble-inp.org)
 * @brief Cells inside the bounds of the universe
 * @version 0.1
 * @date 2024-05-20
 */

#ifndef _INTERN_CELLS_HPP_
#define _INTERN_CELLS_HPP_


#include "cell.hpp"
#include "xassert.hpp"

class GriddedUniverse;

/**
 * @brief Limits Cells are contains copies of the cellule
 *        of same coordinates, with a position offset
 */
class InternCell : public Cell {
private:
    /* Store in list (doubly linked list) because
      particles of the universe will be deleted
      and added when travelling through cells.
      We need constant time complexity
      for deletion and insertion. */
   std::list<Particle*> _particles;

   void applyForceOnParticle(Particle* p) const override;

public:
    InternCell(size_t dimension, GriddedUniverse* universe, std::vector<int> coordinates);

    const std::list<Particle*> getParticles() const  { return _particles; }

    /**
     * @brief apply forces between particles in the cell
     *        (adds to existing forces)
     */
    void computeInternInterractions();

    /**
     * @brief Adds a cell to the neighbours
     *        Dimensions must match
     * @param p pointer to particle
     */
    void addParticle(Particle* p);

    void applyForceOnNeighbours()const override;
    void clearParticles()              override;
};

#endif // _INTERN_CELLS_HPP_