// Collections
#include <deque>
#include <list>
#include <set>
#include <vector>

// Personnal headers
#include <config.hpp>
#include <forces.hpp>
#include <gridded_universe.hpp>
#include <particle.hpp>
#include <progressbar.hpp>
#include <universe.hpp>
#include <vector.hpp>
#include <visual_generator.hpp>

// Other
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
  // Problem constants
  double L1 = 250;            // Length of the universe
  double L2 = 180;            // Height of the universe
  double epsilon = 5;         // Lennard-Jones potential parameter
  double sigma = 1;           // Lennard-Jones potential parameter
  double m = 1;               // Mass of particles
  Vector red_speed({0, 0});   // Speed of red particles
  Vector blue_speed({0, 0});  // Speed of blue particles
  double G = 12;              // Gravitational constant

  double r_cut = 2.5 * sigma;                  // Cut-off radius
  double spaceStep = pow(2, 1.0 / 6) / sigma;  // Space step for particles

  // Universe creation (size L1xL2, cells of side r_cut)
  Vector lowerBound = Vector({0, 0});
  Vector upperBound = Vector({L1, L2});
  GriddedUniverse universeGrid(lowerBound, upperBound, r_cut);

  // Adds interaction of particles in universe
  universeGrid.addInteraction(
      [epsilon, sigma](const Particle& source, Particle& target) {
        lennardJonesInteraction(source, target, epsilon, sigma);
      });

  // Adds gravitation force
  universeGrid.addExternalForce(
      [G](Particle& target) { gravitationalForce(target, G); });

  size_t blue_width = 160;
  size_t blue_height = 40;
  size_t red_width = 20;
  size_t red_height = 20;

  // // Adds blue rectangle particles
  // Vector bottomLeftCorner({20,1});
  // for (size_t i = 0; i < blue_width; i++) {
  //     for (size_t j = 0; j < blue_height; j++) {
  //         Vector position = bottomLeftCorner;
  //         position += Vector({i * spaceStep, j * spaceStep});
  //         universeGrid.addParticle(position, blue_speed, m);
  //     }
  // }

  // Adds red rectangle particles
  bottomLeftCorner = {100, 60};
  for (size_t i = 0; i < red_width; i++) {
    for (size_t j = 0; j < red_height; j++) {
      Vector position = bottomLeftCorner;
      position += Vector({i * spaceStep, j * spaceStep});
      universeGrid.addParticle(position, red_speed, m);
    }
  }

  universeGrid.setOOBBehavior(ABSORPTION);

  // Simulates evolution
  double timeStep = 0.001;  // 0.00005;
  double finalTime = 19.5;
  universeGrid.simulateStormerVerlet(timeStep, finalTime);

#ifdef PNG_OUTPUT
  // Generate images
  VisualGenerator vg(&universeGrid);
  vg.setImageSizes(static_cast<size_t>(6 * L1), static_cast<size_t>(6 * L2));
  vg.setPointSize(1);
  vg.setPointType(CIRCLE_F);
  vg.generateVideo(200);
#endif

  return EXIT_SUCCESS;
}
