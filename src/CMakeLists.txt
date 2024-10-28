# # Get all .cpp files in the src directory
# file(GLOB SOURCES "*.cpp")

# # Create executable from all source files
# add_executable(main ${SOURCES})

### or ###

add_executable(
    main
    main.cpp
    particle.cpp
    universe.cpp
    finite_universe.cpp
    gridded_universe.cpp
    vector.cpp
    cell.cpp
    visual_generator.cpp
)
