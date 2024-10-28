/**
 * @file SolarSystemTP2.cpp
 * @author jules roques (jules.roques@grenoble-inp.org)
 * @brief Calculates the evolution of a solar system
 *        and creates images for visualization using a gnuplot script
 * @version 0.1
 * @date 2024-04-07
 */

// Collections
#include <set>
#include <list>
#include <deque>
#include <vector>

// Personnal headers
#include <particle.hpp>
#include <universe.hpp>
#include <forces.hpp>
#include <visual_generator.hpp>

// Other
#include <chrono>
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <cmath>


int main(int argc, char** argv) {

    // Creates the universe
    Universe univ(3);

    // Adds particles
    univ.addParticle({0, 0, 1},     {0, 0, 0},      1,       "Soleil");
    univ.addParticle({0, 1, 0},     {-1, 0, 0},     3.0e-6,  "Terre");
    univ.addParticle({0, 5.36, 0},  {-0.425, 0, 0}, 9.55e-4, "Jupiter");
    univ.addParticle({34.75, 0, 0}, {0, 0.0296, 0}, 1e-14,   "Haley");

    // Adds interaction between them
    univ.addInteraction(gravitationalInteraction);
    
    double timeStep = 0.1;//0.015;
    double finalTime = 468.5;

    // Simulation calculation
    std::cerr << "System evolution calculation ";
    univ.simulateStormerVerlet(timeStep, finalTime);

    // generate video
    VisualGenerator vg(&univ);
    vg.setPointSize(2);
    vg.setPointType(CIRCLE_F);
    vg.generateVideo(50);
    
    return EXIT_SUCCESS;
}