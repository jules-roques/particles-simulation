#include <finite_universe.hpp>
#include <xassert.hpp>
#include <cmath>
#include "forces.hpp"

/* ------------------------------- private ------------------------------- */

void FiniteUniverse::
applyWallsForces() {
    for (Particle& p : getParticles()) {
        _wallsForce.applyOn(p);
    }
}

void FiniteUniverse::
applyForeignNeighboursForces() {
    throw std::runtime_error("Not yet implemented.");
}

void FiniteUniverse::
applyExternalForces() {

    Universe::applyExternalForces();

    if (_applyWallsForce) {
        applyWallsForces();
    }    
}

void FiniteUniverse::
applyInteractionForces() {
    applyInternInterractionsForces();
    applyLimitinterractionForces();
}


void FiniteUniverse::
applyInternInterractionsForces() {
    Universe::applyInteractionForces();
}


void FiniteUniverse::
applyLimitinterractionForces() {
    if (_oobbehavior == PERIODIC) {
        applyForeignNeighboursForces();
    }
}

/* ------------------------------- protected ------------------------------- */

std::pair<Vector, Vector> FiniteUniverse::
getBounds() const  {
    return std::pair<Vector, Vector>(_lowerBound, _upperBound);
}

void FiniteUniverse::
removeOutOfBoundsParticles() {
    std::list<Particle>& particles = getParticles();
    auto it = particles.begin();
    while (it != particles.end()) {
        if (isInBounds(*it)) {
            it++;
        } else {
            it = particles.erase(it);
        }
    }
}

void FiniteUniverse::
reflectOutOfBoundsParticles() {
    size_t dim = getDimension();
    for (Particle& p : getParticles()) {
        for (size_t i = 0; i < dim; i++) {
            double coordValue = p.getPosition()[i];
            while (coordValue < _lowerBound[i] || coordValue > _upperBound[i]) {
                double boundValue = (coordValue < _lowerBound[i]) ? _lowerBound[i] : _upperBound[i];
                p.invertSpeed(i);
                coordValue = 2*boundValue - coordValue;
                p.setPosCoord(i, coordValue);
            }
        }
        xassert(isInBounds(p), "particle should not be out of bounds anymore.");
    }
}

void FiniteUniverse::
teleportOutOfBoundsParticles() {
    size_t dim = getDimension();
    for (Particle& p : getParticles()) {
        for (size_t i = 0; i < dim; i++) {
            double coordValue = p.getPosition()[i];
            if (coordValue < _lowerBound[i] || coordValue > _upperBound[i]) {
                double divisor = _upperBound[i] - _lowerBound[i];
                coordValue = std::fmod(std::fmod(coordValue - _lowerBound[i], divisor) + divisor, divisor);
                p.setPosCoord(i, coordValue);
            }
        }
        xassert(isInBounds(p), "particle should not be out of bounds anymore.");
    }
}

void FiniteUniverse::
handleOutOfBoundsParticles() {
    switch (_oobbehavior)
    {
        case ABSORPTION:
            removeOutOfBoundsParticles();
            break;
        
        case REFLEXION:
            reflectOutOfBoundsParticles();
            break;
        
        case PERIODIC:
            teleportOutOfBoundsParticles();
            break;

        default:
            throw std::runtime_error("Unexpected out-of-bounds behavior");
    }
}

/* ------------------------------- public ------------------------------- */

FiniteUniverse::
FiniteUniverse(Vector lowerBound, Vector upperBound)
        : Universe(lowerBound.getDimension()) {
    xassert(lowerBound.getDimension() == upperBound.getDimension(),
            "Bounds dimensions must match.");
    xassert(upperBound.areAllCoordsGreater(lowerBound),
            "upperBound coordinates must be strictly greater than lowerBound ones.");

    _lowerBound = lowerBound;
    _upperBound = upperBound;
}

void FiniteUniverse::
updatePositions(double timeStep) {

    // Updates positions
    Universe::updatePositions(timeStep);

    // Deal with out of bounds particles
    handleOutOfBoundsParticles();
}

std::ostream& operator<<(std::ostream& strm, FiniteUniverse universe) {
    strm << "FiniteUniverse" << std::endl
         << "   dimension: " << universe.getDimension() << std::endl
         << "   lower bound: " << universe._lowerBound << std::endl
         << "   upper bound: " << universe._upperBound << std::endl
         << "   number of active particles: " << universe.getNbParticles();
    return strm;     
}

void FiniteUniverse::
addParticle(Vector pos, Vector speed,
            double mass, std::string name) {
    xassert(pos.isInBounds(_lowerBound, _upperBound),
            "Particucle added must be inside the bounds of the finite universe.");
    Universe::addParticle(pos, speed, mass, name);
}

bool FiniteUniverse::
isInBounds(const Particle& p) {
    xassert(getDimension() == p.getDimension(),
            "Particle and universe must have same dimension.");
    Vector position = p.getPosition();
    return position.isInBounds(_lowerBound, _upperBound);
}

void FiniteUniverse::
setOOBBehavior(OOBBehavior lb) {
    _oobbehavior = lb;
    _applyWallsForce = false;
}

void FiniteUniverse::
activateReflexionWithForces(double epsilon, double sigma) {
    _oobbehavior = REFLEXION;
    _applyWallsForce = true;
    _wallsForce.setForceFunction(
        [this, epsilon, sigma](Particle& target) { wallsForce(target, _lowerBound, _upperBound, epsilon, sigma); }
    );
}
