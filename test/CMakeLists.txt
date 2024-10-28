# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Add source files from the src directory
set(
    SRC_SOURCES
    ../src/vector.cpp
    ../src/particle.cpp
)

# Add all test files in the test directory
file(GLOB TEST_SOURCES "*.cpp")

# Ajoutez les exécutables de test
add_executable(
    test
    ${TEST_SOURCES}
    ${SRC_SOURCES}
)

# Link test executable against gtest & your code
target_link_libraries(
    test
    ${GTEST_LIBRARIES}
)

enable_testing()

# Découvrir et exécuter automatiquement les tests
include(GoogleTest)
gtest_discover_tests(test)
