#include <string.h>

#include <config.hpp>
#include <cstdlib>
#include <progressbar.hpp>
#include <visual_generator.hpp>
#include <xassert.hpp>

/* ---------------------------------- intern ----------------------------------
 */

/**
 * @brief Write the range for one axe in the script
 * @param scriptFile
 * @param axName
 * @param min
 * @param max
 */
void setAxRange(std::ofstream& scriptFile, std::string axName, double min,
                double max) {
  xassert(axName == "x" || axName == "y" || axName == "z",
          "axName must be \"x\", \"y\", or \"z\".");
  xassert(min <= max, "min must be lower or equals to max.");
  if (min == max) {
    min--;
    max++;
  }
  scriptFile << "set " << axName << "range [" << min << " : " << max << "]"
             << std::endl;
}

/* ---------------------------------- private ----------------------------------
 */

bool VisualGenerator::colorPaletteCanBeUsed() const {
  return _universe->getMaxForce() > 0;
}

void VisualGenerator::writePhotoData() const {
  std::ofstream dataFile(_photoDataFileName);  // Write in binary
  if (!dataFile) {
    throw std::runtime_error("Error opening file for writing: " +
                             _photoDataFileName);
  }

  // Write binary data in the file
  for (const Particle& p : _universe->_particles) {
    const std::vector<double>& vect = p.getPosition().getData();
    dataFile.write(reinterpret_cast<const char*>(vect.data()),
                   vect.size() * sizeof(*vect.data()));
  }

  dataFile.close();
}

VisualGenerator::VisualGenerator(const Universe* universe)
    : _universe(universe) {}

void VisualGenerator::writePhotoScript() const {
  std::ofstream scriptFile(_videoScriptName);
  if (!scriptFile) {
    throw std::runtime_error("Error opening file for writing: " +
                             _videoScriptName);
  }

#ifdef SHOW_PROGRESS_INFOS
  scriptFile << "print \"Generating photo '" << _photoImageName << "'\""
             << std::endl;
#endif

  // Disable plot legend
  scriptFile << "unset key" << std::endl;

  // Set image sizes
  writeImageSizes(scriptFile);

  scriptFile << "set output '" << _photoImageName << "'" << std::endl;
  writePhotoPlotCommand(scriptFile);

  scriptFile.close();
}

void VisualGenerator::writeVideoScript(size_t numberFrames) const {
  std::ofstream scriptFile(_videoScriptName);
  if (!scriptFile) {
    throw std::runtime_error("Error opening file for writing: " +
                             _videoScriptName);
  }

  size_t nbPastStates = _universe->_nbPastStates;

  // Disable plot legend
  scriptFile << "unset key" << std::endl;

  // Set image sizes
  writeImageSizes(scriptFile);

  // Creates folder to store future generated images
  std::stringstream ss;
  ss << "rm -rf " << _videoFolderName << "; mkdir " << _videoFolderName;
  if (system(ss.str().c_str()) != 0) {
    throw std::runtime_error("Error while creating folder: " +
                             _videoFolderName);
  }

  // for the color palette if there is different forces
  if (colorPaletteCanBeUsed()) {
    scriptFile
        << "set cbrange [0:" << _universe->getMaxForce() << "]" << std::endl
        << "set cblab 'Force applied (Newton)' offset -2,0" << std::endl
        << "set cbtics offset -1.2,0"
        << std::endl;  //<< "set logscale cb" << std::endl;  // echelle log
  }

  // Sets axes ranges
  setAxesRanges(scriptFile);

  // Writes for loop for images generation
  size_t step = nbPastStates / numberFrames;
  scriptFile << "n = 1" << std::endl
             << "do for [i=0 : " << numberFrames * step - 1 << " : " << step
             << "] {" << std::endl
             << "    set output sprintf('" << _videoFolderName
             << "/img%03.0f.png',n)" << std::endl
             << std::endl;
  writeVideoPlotCommand(scriptFile);

/* Here, we write directly in the gnuplot script a loading bar
   because otherwise we cannot access progress informations
   using the header file "progressbar.hpp".*/
#ifdef SHOW_PROGRESS_INFOS
  scriptFile << std::endl
             << "    # Print progress bar" << std::endl
             << "    progress = int(50.0 * n / " << numberFrames << ")"
             << std::endl
             << "    bar = \"\"" << std::endl
             << "    do for [i=1:progress] { bar = bar . \"#\" }" << std::endl
             << "    do for [i=progress+1:50] { bar = bar . \" \" }"
             << std::endl
             << "    shell_command = sprintf(\"echo -n \\\"\\rGenerating "
             << numberFrames << " images in '" << _videoFolderName
             << "' [%s] %i%\\\"\", bar, int(2 * progress))" << std::endl
             << "    system(shell_command)" << std::endl
             << std::endl;
#endif

  scriptFile << "    n=n+1" << std::endl
             << "}" << std::endl
             << "system('echo')" << std::endl;

  scriptFile.close();
}

void VisualGenerator::writeVideoPlotCommand(std::ofstream& scriptFile) const {
  if (_universe->getDimension() == 3) {
    scriptFile << "    splot ";
  } else {
    scriptFile << "    plot ";
  }

  scriptFile << "'" << _universe->_pastParticlesFileName << "'"
             << " index i using ";
  if (colorPaletteCanBeUsed()) {
    scriptFile << dotsRange(_universe->_dimension + 1);  // + 1 for color column
  } else
    scriptFile << dotsRange(_universe->_dimension);

  scriptFile << " with points"
             << " pointtype " << _pointType << " pointsize " << _pointSize;

  if (colorPaletteCanBeUsed())
    scriptFile << " palette";  // For color palette usage

  scriptFile << std::endl;
}

void VisualGenerator::writePhotoPlotCommand(std::ofstream& outFile) const {
  if (_universe->getDimension() == 3) {
    outFile << "splot ";
  } else {
    outFile << "plot ";
  }

  outFile << "'" << _photoDataFileName << "'"
          << " binary format='%double'"
          << " using " << dotsRange(_universe->_dimension) << " with points"
          << " pointtype " << _pointType << " pointsize " << _pointSize
          << std::endl;
}

void VisualGenerator::writeImageSizes(std::ofstream& scriptFile) const {
  scriptFile << "set terminal pngcairo size " << _imageWidth << ','
             << _imageHeigth << std::endl;
}

void VisualGenerator::setAxesRanges(std::ofstream& scriptFile) const {
  // Gets the bounds of the universe
  std::pair<Vector, Vector> bounds = _universe->getBounds();
  Vector lowerBound = bounds.first;
  Vector upperBound = bounds.second;
  xassert(upperBound.areAllCoordsGreater(lowerBound),
          "greaterBound must be greater than lowerBound on each dimension.");

  std::array<std::string, 3> axNames = {"x", "y", "z"};
  for (size_t i = 0; i < _universe->_dimension; i++) {
    setAxRange(scriptFile, axNames[i], lowerBound[i], upperBound[i]);
  }
}

std::string VisualGenerator::dotsRange(size_t n) const {
  xassert(n > 0, "n must be > 0");
  std::stringstream ss;
  ss << "1";
  for (size_t i = 2; i <= n; i++) {
    ss << ':' << i;
  }
  return ss.str();
}

/* ---------------------------------- public ----------------------------------
 */

void VisualGenerator::setPointSize(double pointSize) {
  xassert(pointSize > 0, "pointSize must be greater than 0.");
  _pointSize = pointSize;
}

void VisualGenerator::setImageSizes(size_t width, size_t height) {
  _imageWidth = width;
  _imageHeigth = height;
}

/* Generates an image representing a photo of the current state of the universe.
   Writes the data of the current state in a file.
   Writes the gnuplot script to generate image.
   Run script to generate image. */
void VisualGenerator::generatePhoto() const {
  // Write data
#ifdef SHOW_PROGRESS_INFOS
  std::cerr << "Writing data in '" << _photoDataFileName << "'" << std::endl;
#endif
  writePhotoData();

  // Write script to generate images
#ifdef SHOW_PROGRESS_INFOS
  std::cerr << "Writing script '" << _videoScriptName << "'" << std::endl;
#endif
  writePhotoScript();

  // Run script (progress informations in the script)
  std::stringstream ss;
  ss << "gnuplot " << _videoScriptName;
  if (system(ss.str().c_str()) != 0) {
    throw std::runtime_error("Error while executing gnuplot script: " +
                             _videoScriptName);
  }
}

/* Generates multiple images representing a video
   of the past states of the universe.
   Writes the gnuplot script to generate images.
   Run script using data stored previously in the
   Stormer Verlet method execution */
void VisualGenerator::generateVideo(size_t numberFrames) const {
  size_t nbPastStates = _universe->_nbPastStates;
  xassert(nbPastStates >= numberFrames,
          "nbFrames must be lower than the number of states the universe has "
          "been into.");

#ifdef SHOW_PROGRESS_INFOS
  std::cerr << "Data writed in '" << _universe->_pastParticlesFileName << "'"
            << std::endl;
#endif

  // Write script to generate images
#ifdef SHOW_PROGRESS_INFOS
  std::cerr << "Writing script '" << _videoScriptName << "'" << std::endl;
#endif
  writeVideoScript(numberFrames);

  // Run script and check for warnings
  std::stringstream ss;
  ss << "gnuplot " << _videoScriptName << " 2> gnuplot_errors.txt";
  if (system(ss.str().c_str()) != 0) {
    throw std::runtime_error("Error while executing gnuplot script: " +
                             _videoScriptName);
  }

  // Check if the error file has content (indicating warnings)
  std::ifstream errorFile("gnuplot_errors.txt");
  if (errorFile.is_open()) {
    errorFile.seekg(0, std::ios::end);    // Move file pointer to end
    size_t fileSize = errorFile.tellg();  // Get file size
    errorFile.close();

    if (fileSize > 0)
      std::cerr
          << "Warning: There were warnings while executing the gnuplot script '"
          << _videoScriptName << "'. Check 'gnuplot_errors.txt'." << std::endl;
    else
      std::remove("gnuplot_errors.txt");

  } else {
    throw std::runtime_error(
        "Error: Unable to open gnuplot error file: gnuplot_errors.txt");
  }
}