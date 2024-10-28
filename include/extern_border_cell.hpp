/**
 * @file extern_border_cell.hpp
 * @author jules roques (jules.roques@grenoble-inp.org)
 * @brief Cells for limit behavior of a PERIODIC universe
 * @version 0.1
 * @date 2024-05-20
 */

#ifndef _EXTERN_BORDER_CELL_HPP_
#define _EXTERN_BORDER_CELL_HPP_


#include "cell.hpp"
#include "xassert.hpp"

class InternCell;
class GriddedUniverse;

/**
 * @brief Limits Cells contains copies of particles
 *        of a foreign cell, neighbour for a PERIDIC
 *        point of view.
 *        An offset is applied on the particles positions.
 */
class ExternBorderCell : public Cell {
private:

    /* Corresponding cell to copy */
    InternCell *_copyCell;

    /* Position offset applied to the particles */
    Vector _offset;

    /* Copies of particles with an offset */
    std::list<Particle> _particles;

    /* Foreign neighbours are cells that are close
       for a PERIODIC universe point of view,
       but with foreign coordinates. */
    std::list<InternCell*> _neighbours;

    void applyForceOnParticle(Particle* p) const override;

public:
    ExternBorderCell(size_t dimension, GriddedUniverse* universe, std::vector<int> coordinates,
              Vector offset, InternCell *copyCell)
                : Cell(dimension, universe, coordinates),
                 _copyCell(copyCell),
                 _offset(offset) {
        xassert(coordinates.size() == offset.getDimension(), "Dimensions must match.");
    }

    /**
     * @brief Copies particles from the _copyCell
     */
    void copyParticles();


    void applyForceOnNeighbours() const override;
    void clearParticles()               override;
};

#endif // _EXTERN_BORDER_CELL_HPP_