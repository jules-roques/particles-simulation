#include "intern_cell.hpp"

#include "extern_border_cell.hpp"
#include "gridded_universe.hpp"

/* ----------------------------- private ----------------------------- */

void InternCell::applyForceOnParticle(Particle* p) const {
  for (const Particle* cellParticle : _particles) {
    cellParticle->applyInteractionForcesOn(*p,
                                           getUniverse()->getInteractions());
  }
}

/* ----------------------------- public ----------------------------- */

InternCell::InternCell(size_t dimension, GriddedUniverse* universe,
                       std::vector<int> coordinates)
    : Cell(dimension, universe, coordinates) {
  xassert(coordinates.size() == universe->getDimensions().size(),
          "Dimensions must match.");
  for (size_t i = 0; i < coordinates.size(); i++) {
    xassert(
        coordinates[i] >= 0 && coordinates[i] < universe->getDimensions()[i],
        "Coordinates should not have those values.");
  }
}

void InternCell::clearParticles() { _particles.clear(); }

void InternCell::computeInternInterractions() {
  for (Particle* p : _particles) {
    // Forces applied by other particles in the cell
    for (Particle* cellParticle : _particles) {
      if (cellParticle != p) {
        cellParticle->applyInteractionForcesOn(
            *p, getUniverse()->getInteractions());
      }
    }
  }
}

void InternCell::addParticle(Particle* p) {
  xassert(getDimension() == p->getDimension(),
          "Particle and cell must have the same dimension.");
  _particles.push_back(p);
}

void InternCell::applyForceOnNeighbours() const {
  for (InternCell* neighbour : getNeighbours()) {
    for (Particle* p : neighbour->getParticles()) {
      applyForceOnParticle(p);
    }
  }
}