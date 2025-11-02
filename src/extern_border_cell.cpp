#include "extern_border_cell.hpp"

#include "gridded_universe.hpp"
#include "intern_cell.hpp"

/* ----------------------------- private ----------------------------- */

void ExternBorderCell::applyForceOnParticle(Particle* p) const {
  for (const Particle& cellParticle : _particles) {
    cellParticle.applyInteractionForcesOn(*p, getUniverse()->getInteractions());
  }
}

/* ----------------------------- public ----------------------------- */

void ExternBorderCell::copyParticles() {
  for (const Particle* p : _copyCell->getParticles()) {
    Vector newPosition = p->getPosition();
    newPosition += _offset;
    _particles.emplace_back(newPosition, p->getSpeed(), p->getMass(),
                            p->getName());
  }
}

void ExternBorderCell::clearParticles() { _particles.clear(); }

void ExternBorderCell::applyForceOnNeighbours() const {
  for (InternCell* neighbour : getNeighbours()) {
    for (Particle* p : neighbour->getParticles()) {
      applyForceOnParticle(p);
    }
  }
}