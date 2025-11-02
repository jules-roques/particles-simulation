/**
 * @file GriddedUniverse.hpp
 * @author jules roques (jules.roques@grenoble-inp.org)
 * @brief A gridded universe is a finite universe, separated into cells.
 * @version 0.1
 * @date 2024-04-10
 */

#ifndef _GRIDDED_UNIVERSE_HPP_
#define _GRIDDED_UNIVERSE_HPP_

#include <vector>

#include "extern_border_cell.hpp"
#include "finite_universe.hpp"
#include "intern_cell.hpp"
#include "vector.hpp"

/**
 * @brief A GriddedUniverse is a finite universe, separated into cells.
 *        Extends Universe.
 */
class GriddedUniverse : public FiniteUniverse {
 private:
  double _cellSide;

  /* Cells are parts of the finite universe.
     Store them in a vector because once initialised,
     we will not delete or instert one cell  inside.
     But we want to have a rapid access to get the cell
     by index. */
  std::vector<InternCell> _internCells;

  /* For PERIODIC universe, cells containg copies of particles
     at the other side of the universe. They have as neighbours
     cells on which they must have an impmlact on the other side. */
  std::vector<ExternBorderCell> _externBorderCells;

  /* Number of cell in each dimension, not including limit cells */
  std::vector<int> _dimensions;

  /**
   * @brief Create an extern border cell
   */
  void createExternBorderCell(std::vector<int>& coords, InternCell* copyCell);

  /**
   * @brief Creates recursively intern cells
   * @param dimensions
   * @param indices
   * @param initialCoords
   * @param depth
   */
  void createInternCellsRecursive(std::vector<int>& coordinates, int depth);

  /**
   * @brief Creates recursively extern border cells
   * @param dimensions
   * @param indices
   * @param initialCoords
   * @param depth
   */
  void createExternCellsRecursive(std::vector<int>& coordinates, int depth);

  /**
   * @brief Creates as many cells as needed for the universe bounds
   *        to be INSIDE the grid of cells.
   *        If the universe is of sizes (1.5, 3) and the cell side
   *        of 1, we will have a 2x3 grid of cells.
   *        Creates also the contouring ExternBorderCells.
   */
  void cellsCreation();

  /**
   * @brief Give the cell coordinates
   *        containing the position given
   * @param pos
   * @return std::vector<size_t>
   */
  std::vector<int> correspondingCellCoordonates(const Vector& pos);

  /**
   * @brief Give the index of an intern cell by its coordinates
   * @param coordinates
   * @return size_t
   */
  size_t internCellIndex(std::vector<int> coordinates);

  /**
   * @brief Tells if cells of corresponding coordinates are neighbours
   * @return true
   * @return false
   */
  inline bool areNeighbours(std::vector<int> coord1, std::vector<int> coord2);

  /**
   * @brief Set neighbours for each cell
   */
  void setCellsNeighbours();

  /**
   * @brief Set neighbours of a cell
   * @param index index of the cell
   */
  void setOneInernCellNeighbours(InternCell& internCell);

  /**
   * @brief Set neighbours of a cell
   * @param index index of the cell
   */
  void setOneExternCellNeighbours(ExternBorderCell& borderCell);

  /**
   * @brief Updates particles positions
   *        in Stormer Verlet algorithm.
   *        Handles change of cell and
   *        being outside of the finite universe.
   */
  void updatePositions(double timeStep) override;

  /**
   * @brief Puts the patricle given in the good cell
   *        in the universe
   * @param p
   */
  void putInCorrespondingCell(Particle* p);

  /**
   * @brief delete all particle pointers
   *        stored in cells
   */
  void clearCells();

  /**
   * @brief Fill cells with particles in the universe
   */
  void fillCells();

  /**
   * @brief Applies the force implied by the foreign
   *        neighbours on the particles.
   *        Foreign neighbours are cells that are close
   *        for a PERIODIC universe point of view,
   *        but with foreign coordinates.
   */
  void applyForeignNeighboursForces() override;

  /**
   * @brief Applies forces between particles in the universe,
   *        but not limit interractions (like for PERIODIC)
   */
  void applyInternInterractionsForces() override;

 public:
  /**
   * @brief Create a GriddedUniverse.
   *        Improve efficiency while computing interractions between particles.
   *        If a particle quits the domain during the simulation,
   *        it is deleted from the gridded universe.
   * @param cellSide distance with which we can neglect
   *                  interractions between two particles
   * @param lowerBound one extreme corner of the area of the universe
   * @param upperBound the other extreme corner, must have greater coordinates
   */
  GriddedUniverse(Vector lowerBound, Vector upperBound, double cellSide);

  const std::vector<int>& getDimensions() const { return _dimensions; }

  friend std::ostream& operator<<(std::ostream& strm, GriddedUniverse universe);

  /**
   * @brief Simulates the movement of particles in the universe using the
   * Störmer-Verlet method Before simulation, fill cells with particles
   * @param timeStep
   * @param finalTime End time of the simulation
   */
  void simulateStormerVerlet(double timeStep, double finalTime) override;
};

#endif  // _GRIDDED_UNIVERSE_HPP_