#include <xassert.hpp>
#include <cmath>
#include <vector>
#include <algorithm>
#include <sstream>

#include "gridded_universe.hpp"
#include "progressbar.hpp"
#include "intern_cell.hpp"
#include "extern_border_cell.hpp"


/* ------------------------------- intern (for checking asserts) ------------------------------- */

bool isExternCoord(const std::vector<int>& coords, const std::vector<int>& dimensions) {
    for (size_t i = 0; i < dimensions.size(); i++) {
        if (coords[i] == -1 || coords[i] == dimensions[i]) {
            return true;
        }
    }
    return false;
}

bool isInternCoord(const std::vector<int>& coords, const std::vector<int>& dimensions) {
    for (size_t i = 0; i < dimensions.size(); i++) {
        if (coords[i] < 0 || coords[i] >= dimensions[i]) {
            return false;
        }
    }
    return true;
}

/* ------------------------------- private ------------------------------- */

void GriddedUniverse::
createExternBorderCell(std::vector<int>& coords, InternCell *copyCell) {
    xassert(coords.size() == _dimensions.size(), "Dimensions must match.");
    xassert(isExternCoord(coords, _dimensions), "Coords should be an external coord.");

    Vector offset(_dimensions.size());
    Vector universeSizes = getUpperBound();
    universeSizes -= getLowerBound();
    for (size_t i = 0; i < _dimensions.size(); i++) {
        if (coords[i] == -1) {
            offset[i] -= universeSizes[i];
        } else if (coords[i] == _dimensions[i]) {
            offset[i] += universeSizes[i];
        }
    }

    xassert(offset != Vector(_dimensions.size()), std::stringstream() << "offset shouldn't be zero. offset = " << offset);
    _externBorderCells.emplace_back(getDimension(), this, coords, offset, copyCell);
}

void GriddedUniverse::
createInternCellsRecursive(std::vector<int>& coordinates, int depth) {
    if (depth == -1) {
        xassert(isInternCoord(coordinates, _dimensions), "Must have intern coordinates.");
        _internCells.emplace_back(InternCell(getDimension(), this, coordinates));
        return;
    }

    // Loop through the current dimension
    for (int i = 0; i < _dimensions[depth]; ++i) {
        coordinates[depth] = i;
        createInternCellsRecursive(coordinates, depth - 1);
    }
}


void GriddedUniverse::createExternCellsRecursive(std::vector<int>& coordinates, int depth) {
    if (depth == -1) {
        bool isBorder = false;
        for (size_t i = 0; i < _dimensions.size(); ++i) {
            if (coordinates[i] == -1 || coordinates[i] == _dimensions[i]) {
                isBorder = true;
                break;
            }
        }

        if (isBorder) {

            xassert(isExternCoord(coordinates, _dimensions), "Not extern coord.");
            std::vector<int> copyCellCoords(coordinates.size());
            for (size_t i = 0; i < coordinates.size(); i++) {

                if (coordinates[i] == _dimensions[i]) {
                    copyCellCoords[i] = 0;
                } else if (coordinates[i] == -1) {
                    copyCellCoords[i] = _dimensions[i] - 1;
                } else {
                    copyCellCoords[i] = coordinates[i];
                }
            }

            xassert(isInternCoord(copyCellCoords, _dimensions), "Must have intern coordinates.");
            InternCell *copyCell = &_internCells.at(internCellIndex(copyCellCoords));
            createExternBorderCell(coordinates, copyCell);
        }
        return;
    }

    // Extend the range to include negative and beyond bounds
    for (int i = -1; i <= _dimensions[depth]; ++i) {
        coordinates[depth] = i;
        createExternCellsRecursive(coordinates, depth - 1);
    }
}

void GriddedUniverse::
cellsCreation() {

    // Computes how many cells we need for each dimension
    Vector universeSizes = getUpperBound();
    universeSizes -= getLowerBound();
    for (size_t i = 0; i < getDimension(); i++) {
        _dimensions.push_back(
            static_cast<size_t>(std::ceil(universeSizes[i] / _cellSide))
        );
    }

    // Initial coords to 0
    std::vector<int> coords(_dimensions.size(), 0);

    // Creates intern cells
    createInternCellsRecursive(coords, _dimensions.size() - 1);

    // Creates extern cells
    createExternCellsRecursive(coords, _dimensions.size() - 1);
}


inline bool GriddedUniverse::
areNeighbours(std::vector<int> coord1, std::vector<int> coord2) {
    xassert(coord1.size() == coord2.size(), "Coords dimensions must match");
    for (size_t d = 0; d < coord1.size(); d++) {
        if (abs(coord1[d] - coord2[d]) > 1) {
            return false;
        }
    }
    return true;
}

void GriddedUniverse::
setCellsNeighbours() {
    xassert(_internCells.size() > 0, "There is no cell to add neighbour to.");

#ifdef SHOW_PROGRESS_INFOS
    std::cerr << "Assigning cells neighbours ";
    Progressbar bar (_internCells.size() + _externBorderCells.size());
#endif

    for (size_t i = 0; i < _internCells.size(); i++) {
        setOneInernCellNeighbours(_internCells[i]);

#ifdef SHOW_PROGRESS_INFOS
        bar.update();
#endif
    }

    for (size_t i = 0; i < _externBorderCells.size(); i++) {
        setOneExternCellNeighbours(_externBorderCells[i]);

#ifdef SHOW_PROGRESS_INFOS
        bar.update();
#endif
    }
}


void GriddedUniverse::
setOneExternCellNeighbours(ExternBorderCell& borderCell) {
    // Get the coordinates of the current cell
    const std::vector<int> coordinates = borderCell.getCoordinates();

    // Set neighboring cells
    for (InternCell& potentialNeighbour : _internCells) {

        // Get coordinates of potential neighbour
        const std::vector<int>& neighborCoordinates = potentialNeighbour.getCoordinates();        

        // Add to cell's neighbours if is neighbour
        if (areNeighbours(coordinates, neighborCoordinates)) {
            borderCell.addNeighbour(&potentialNeighbour);
        }
    }
        
    xassert(borderCell.getNbNeighbours() > 0 &&
            borderCell.getNbNeighbours() < getDimension() * getDimension(),
            "ExternCell don't have a correct number of neighbours.");
}

void GriddedUniverse::
setOneInernCellNeighbours(InternCell& internCell) {
    
    // Get the coordinates of the current cell
    const std::vector<int> coordinates = internCell.getCoordinates();

    // Set neighboring cells
    for (InternCell& potentialNeighbour : _internCells) {

        // Skip current cell
        if (&internCell == &potentialNeighbour) {
            continue;
        }

        // Get coordinates of potential neighbour
        const std::vector<int>& neighborCoordinates = potentialNeighbour.getCoordinates();        

        // Add to cell's neighbours if is neighbour
        if (areNeighbours(coordinates, neighborCoordinates)) {
            internCell.addNeighbour(&potentialNeighbour);
        }
    }
}

size_t GriddedUniverse::
internCellIndex(std::vector<int> coordinates) {
    xassert(coordinates.size() == getDimension(),
            "Coordinates an gridded universe dimensions must match.");

    size_t index = 0;
    size_t multiplier = 1;
    for (size_t i = 0; i < _dimensions.size(); i++) {
        index += coordinates[i] * multiplier;
        multiplier *= _dimensions[i];
    }
    xassert(index < _internCells.size(),
            std::stringstream() <<
            "index out of bounds, maximum is " << _internCells.size() <<
            ", got " << index << ".");
    return index;
}

/* Calculates the coordinates of the cell which constains
   the positions vector.
   For value in each vector dimension:
        - if the value is not at the upper bound, we divide the distance
          to the lower bound by the cellSide and we have the index,
        - if we do the same when the value is at upper bound, we will
          get an out of bounds index. So we decrease the index by 1. */
std::vector<int> GriddedUniverse::
correspondingCellCoordonates(const Vector& pos) {

    xassert(pos.isInBounds(getLowerBound(), getUpperBound()),
            std::stringstream()
            << "Position has to be inside the bounds of the gridded universe. "
            << "Bounds: " << getLowerBound() << ", " << getUpperBound() << ". "
            << "Position: " << pos << ".");

    std::vector<int> coordinates;
    for (size_t i = 0; i < getDimension(); ++i) {
        int coord = static_cast<int>((pos[i] - getLowerBound()[i]) / _cellSide);
        if (pos[i] == getUpperBound()[i]) coord--;
        xassert(coord < _dimensions[i],
                std::stringstream()
                << "Particle corresponding cell coordinate calculated on dimension " << i << " is too big. "
                << "On this dimension, maximum cell coordinate is " << _dimensions[i] - 1 << ", found " << coord << ".");
        coordinates.push_back(coord);
    }
    return coordinates;
}

void GriddedUniverse::
putInCorrespondingCell(Particle* p) {

    // Computes coordinates of the cell containing the particle
    std::vector<int> coordinates = correspondingCellCoordonates(p->getPosition());

    // Get the index of the cell
    size_t index = internCellIndex(coordinates);
    xassert(index < _internCells.size(),
            std::stringstream() 
            << "Cell index calculated is out of bounds. "
            << "Index is " << index 
            << " while there is " << _internCells.size() << " cells.");

    // Adds the particle in the corresponding cell
    _internCells[index].addParticle(p);
}

void GriddedUniverse::
applyInternInterractionsForces() {
    for (InternCell &cell : _internCells) {
        cell.applyForceOnNeighbours();
        cell.computeInternInterractions();
    }
}

void GriddedUniverse::
applyForeignNeighboursForces() {
    for (const ExternBorderCell& cell : _externBorderCells) {
        cell.applyForceOnNeighbours();
    }
}


void GriddedUniverse::
clearCells() {
    for (Cell &cell : _internCells) {
        cell.clearParticles();
    }

    if (getoobbehavior() == PERIODIC) {
        for (Cell &cell : _externBorderCells) {
            cell.clearParticles();
        }
    }
}

void GriddedUniverse::
fillCells() {
    for (Particle& p : getParticles()) {
        putInCorrespondingCell(&p);
    }

    if (getoobbehavior() == PERIODIC) {
        for (ExternBorderCell& cell : _externBorderCells) {
            cell.copyParticles();
        }
    }
}


void GriddedUniverse::
updatePositions(double timeStep) {

    // Updates positions and deal with out of bounds particles (linear complexity)
    FiniteUniverse::updatePositions(timeStep);

    // Clear cells from particle pointers (linear complexity)
    clearCells();

    // Fill cells with particles pointers (linear)
    fillCells();
}

/* ------------------------------- public ------------------------------- */

GriddedUniverse::
GriddedUniverse(Vector lowerBound, Vector upperBound, double cellSide) :
                FiniteUniverse(lowerBound, upperBound), _cellSide(cellSide) {
    cellsCreation();
    setCellsNeighbours();
}

std::ostream& operator<<(std::ostream& strm, GriddedUniverse universe) {
    strm << "GriddedUniverse" << std::endl
         << "   dimension: " << universe.getDimension() << std::endl
         << "   lower bound: " << universe.getLowerBound() << std::endl
         << "   upper bound: " << universe.getUpperBound() << std::endl
         << "   cell side: " << universe._cellSide << std::endl
         << "   number of cell on each dimension: ";
    
    for (size_t nb : universe._dimensions) {
        strm << nb << " ";
    }

    strm << std::endl
         << "   number of active particles: " << universe.getNbParticles();    
    return strm;     
}


void GriddedUniverse::
simulateStormerVerlet(double timeStep, double finalTime) {

    clearCells();
    fillCells();
    FiniteUniverse::simulateStormerVerlet(timeStep, finalTime);
}