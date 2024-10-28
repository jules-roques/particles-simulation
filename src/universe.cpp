#include <iostream>
#include <string>
#include <universe.hpp>
#include <particle.hpp>
#include <cmath>
#include <vector.hpp>
#include <vector>
#include <xassert.hpp>
#include <cell.hpp>
#include <config.hpp>
#include <fstream>
#include <iomanip>
#include <sstream>
#ifdef SHOW_PROGRESS_INFOS
#include <progressbar.hpp>
#endif


/* ---------------------------------------- intern ---------------------------------------- */

void writeData(std::ofstream& dataFile, const std::list<Particle>& particles) {

    // Iterate through the particles in each time step
    for (const Particle& p : particles) {
        // Get the position data of the particle
        std::vector<double> data = p.getPosition().getData();
        // Write the position data to the file
        for (double value : data) {
            dataFile << value << " ";
        }
        dataFile << p.getForce().norm();
        dataFile << std::endl; // Add newline after writing each position vector
    }
    dataFile << std::endl << std::endl;  // Two new lines to separate groups
}

void writeDataVTK(std::ofstream& dataFile, const std::list<Particle>& particles, const size_t dimmension) {

    // Header
    dataFile << "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"LittleEndian\">" << std::endl;
    dataFile << "<UnstructuredGrid>" << std::endl;
    dataFile << "<Piece NumberOfPoints=\"" << particles.size() << "\" NumberOfCells=\"0\">" << std::endl;
    dataFile << "<Points>" << std::endl;

    // Particles
    // Position
    dataFile << "<DataArray type=\"Float32\" name=\"Position\" NumberOfComponents=\"" << dimmension << "\" format=\"ascii\">" << std::endl;
    for (const Particle& p : particles) {
        std::vector<double> data = p.getPosition().getData();
        for (double value : data) {
            dataFile << value << " ";
        }
    }
    dataFile << std::endl;
    dataFile << "</DataArray>" << std::endl;

    // Speed
    dataFile << "<DataArray type=\"Float32\" name=\"Velocity\" NumberOfComponents=\"" << dimmension << "\" format=\"ascii\">" << std::endl;
    for (const Particle& p : particles) {
        std::vector<double> data = p.getSpeed().getData();
        for (double value : data) {
            dataFile << value << " ";
        }
    }
    dataFile << std::endl;
    dataFile << "</DataArray>" << std::endl;

    // Mass
    dataFile << "<DataArray  type=\"Float32\" name=\"Masse\" format=\"ascii\">" << std::endl;
    for (const Particle& p : particles) {
        dataFile << p.getMass() << " ";
    }
    dataFile << std::endl;
    dataFile << "</DataArray>" << std::endl;

    dataFile << "</Points>" << std::endl;

    // Cells
    dataFile << "<Cells>" << std::endl;
    dataFile << "<DataArray  type=\"Int32\" name=\"connectivity\" format=\"ascii\">" << std::endl;
    dataFile << "</DataArray>" << std::endl;
    dataFile << "<DataArray  type=\"Int32\" name=\"offsets\" format=\"ascii\">" << std::endl;
    dataFile << "</DataArray>" << std::endl;
    dataFile << "<DataArray  type=\"Int8\" name=\"types\" format=\"ascii\">" << std::endl;
    dataFile << "</DataArray>" << std::endl;
    dataFile << "</Cells>" << std::endl;

    // Footer
    dataFile << "</Piece>" << std::endl;
    dataFile << "</UnstructuredGrid>" << std::endl;
    dataFile << "</VTKFile>" << std::endl;

}

/* ---------------------------------------- private ---------------------------------------- */

void Universe::
updateForces() {

    // Set all forces to 0
    setForcesToZero();

    // External forces
    applyExternalForces();

    // Forces from interactions
    applyInteractionForces();
}

void Universe::
applyInteractionForces() {

    for(const Interaction& interaction : _interactions) {
        // Calculates efficiently forces (for an ungridded universe)
        for (auto it_i = _particles.begin(); it_i != std::prev(_particles.end()); ++it_i) {
            for (auto it_j = std::next(it_i); it_j != _particles.end(); ++it_j) {
                interaction(*it_i, *it_j);
                interaction(*it_j, *it_i);
            }
        }
    }    
}

void Universe::
updatesExtremumValues() {
    for (const Particle& p : _particles) {
        _maxPosition = max(_maxPosition, p.getPosition());
        _minPosition = min(_minPosition, p.getPosition());
        double pforce = p.getForce().norm();
        if (pforce > _maxForce) {
            _maxForce = pforce;
        }
    }
}


double Universe::
currentCineticEnergy() const {
    double sum = 0;
    for (const Particle& p : _particles) {
        double speedNorm = p.getSpeed().norm();
        sum += p.getMass() * speedNorm * speedNorm;
    }
    xassert(!std::isnan(sum), "Cinetic energy calculated is not a number (nan).");
    return sum / 2;
}

void Universe::
updatePaces(double timeStep) {
    
    // Update the speeds
    for (Particle& p : _particles) {
        Vector deltaSpeed = p.getForce();
        deltaSpeed += p.getOldForce();
        deltaSpeed *= 0.5 * timeStep / p.getMass();
        p.addToSpeed(deltaSpeed);
    }

    // Readapt speed if cinetic energy is too high
    double cineticEnergy = currentCineticEnergy();
    if (cineticEnergy > _cineticEnergyLimit) {
        double betaFactor = std::sqrt(_cineticEnergyLimit / cineticEnergy);
        for (Particle& p : _particles) {
            p.multiplySpeed(betaFactor);
        }
    }
}

void Universe::
setForcesToZero() {
    for (Particle &p : _particles) {
        p.setForceToZero();
    }
}

/* ---------------------------------------- protected ---------------------------------------- */

std::pair<Vector, Vector> Universe::
getBounds() const {
    return std::pair<Vector, Vector> (_minPosition, _maxPosition);
}

void Universe::
updatePositions(double timeStep) {
    for (Particle& p : _particles) {
        Vector deltaPosition = p.getForce();
        deltaPosition *= 0.5 * timeStep / p.getMass();
        deltaPosition += p.getSpeed();
        deltaPosition *= timeStep;
        p.addToPosition(deltaPosition);
    }
}

void Universe::
applyExternalForces() {
    for (const ExternalForce& force : _forces) {
        for (Particle& p : _particles) {
            force.applyOn(p);
        }
    }
}


/* ---------------------------------------- public ---------------------------------------- */

Universe::
Universe(size_t dimension) : _minPosition(dimension), _maxPosition(dimension) {
    xassert(dimension > 0 && dimension <= 3, "Universe dimension must be 1, 2 or 3.");
    _dimension = dimension;
}

std::ostream& operator<<(std::ostream &strm, const Universe &univers) {
    return strm << "Universe in dimension " << univers._dimension
                << " with "<< univers.getNbParticles() << " active particles ";
}


void Universe::
addParticle(Vector pos, Vector speed,
            double mass, std::string name) {
    xassert(pos.getDimension() == speed.getDimension() &&
            pos.getDimension() == getDimension(),
            "Position and speed dimensions must match with universe dimension.");
    _particles.emplace_back(pos, speed, mass, name);
}

void Universe::
addParticle(std::initializer_list<double> posCoords,
            std::initializer_list<double> speedCoords,
            double mass, std::string name) {
    addParticle(Vector(posCoords), Vector(speedCoords), mass, name);
}

void Universe::
addParticle(Vector pos, Vector speed, double mass) {
    std::string name = "Particle " + std::to_string(getNbParticles());

    _particles.emplace_back(pos, speed, mass, name);
}

void Universe::
addParticle(std::initializer_list<double> posCoords,
            std::initializer_list<double> speedCoords,
            double mass) {
    addParticle(Vector(posCoords), Vector(speedCoords), mass);
}

void Universe::
addInteraction(std::function<void(const Particle& source, Particle& target)> interactionFunction) {
    _interactions.emplace_back(Interaction(interactionFunction));
}

void Universe::
addExternalForce(std::function<void(Particle& target)> forceFunction) {
    _forces.emplace_back(ExternalForce(forceFunction));
}

void Universe::
simulateStormerVerlet(double timeStep, double finalTime) {
    double currentTime = 0;

#ifdef PNG_OUTPUT
    // Open file for writing data
    std::ofstream dataFile(_pastParticlesFileName); // Write in binary
    if (!dataFile) {
        std::cerr << "Error opening file for writing." << std::endl;
        abort();
    }
#endif

    updateForces();

#ifdef SHOW_PROGRESS_INFOS
    size_t nbIterations = static_cast<size_t>(finalTime / timeStep);
    std::cerr << "Stormer Verlet simulation (" << _particles.size() << " particles, " << nbIterations << " iterations)" << std::endl;
    Progressbar bar (nbIterations);
#endif

#ifdef XML_OUTPUT
    // Creates folder to store future generated VTK files
    std::stringstream ss;
    ss << "rm -rf VTKFiles; mkdir VTKFiles";
    if (system(ss.str().c_str()) != 0) {
        std::cerr << "Error while creating folder VTKFiles" << std::endl;
        abort();
    }
#endif

    while (currentTime < finalTime) {

#ifdef XML_OUTPUT
        // Defines the name of the VTK file
        int index = static_cast<int>(currentTime / timeStep);
        std::ostringstream filename;
        filename << "VTKFiles/particles_" << std::setfill('0') << std::setw(5) << index << ".vtu";

        // Open VTK file for each time step
        std::ofstream dataFile(filename.str());
        if (!dataFile) {
            std::cerr << "Error opening file for writing." << std::endl;
            abort();
        }
#endif

        // Updates extremum values
        updatesExtremumValues();

        // Copies particles into the history
#ifdef XML_OUTPUT
        writeDataVTK(dataFile, _particles, _dimension);
        dataFile.close();
#endif

#ifdef PNG_OUTPUT
        writeData(dataFile, _particles);
#endif

        // Updates positions
        updatePositions(timeStep);

        // Register old forces
        for(Particle& p : _particles) { 
            p.setOldForce(p.getForce());
        }

        updateForces();
        updatePaces(timeStep);

        // Updates time
        currentTime += timeStep;
        _nbPastStates++;

#ifdef SHOW_PROGRESS_INFOS
        bar.update();
#endif

    }

#ifdef PNG_OUTPUT
    dataFile.close();
#endif

}



