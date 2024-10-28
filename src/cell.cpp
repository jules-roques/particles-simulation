#include "cell.hpp"
#include "xassert.hpp"
#include "gridded_universe.hpp"
#include "intern_cell.hpp"

/* ----------------------------- private ----------------------------- */



/* ----------------------------- protected ----------------------------- */

/* ----------------------------- public ----------------------------- */

Cell::Cell(size_t dimension, GriddedUniverse* universe, std::vector<int> coordinates) :
         _dimension(dimension), _universe(universe), _coordinates(coordinates) {
    xassert(dimension == coordinates.size(), "Dimensions must match.");
}

void Cell::
addNeighbour(InternCell* neighbour) {
    xassert(_dimension == neighbour->_dimension,
            "Cell and neighbour dimension must be the same.");
    _neighbours.push_back(neighbour);
}