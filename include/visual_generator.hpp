/**
 * @file images_generation.hpp
 * @author jules roques (jules.roques@grenoble-inp.org)
 * @brief Les cellules sont les cases élémentaires formant
 *        un maillage de l'univers
 * @version 0.1
 * @date 2024-04-08
 */

#ifndef _VISUAL_GENERATOR_HPP_
#define _VISUAL_GENERATOR_HPP_

#include <config.hpp>
#include <fstream>
#include <universe.hpp>

/* To define the way
   we plot the points */
enum PointType { NO_SYMBOL, PLUS, CROSS, STAR, BOX, BOX_F, CIRCLE, CIRCLE_F };

/**
 * @brief Provides methods to generate
 *        visualisation of an universe
 */
class VisualGenerator {
 private:
  // Universe we want to visualize
  const Universe* _universe;

  // Private fields with default values
  PointType _pointType = PLUS;
  double _pointSize = 1;
  size_t _imageWidth = 1200;
  size_t _imageHeigth = 800;

  // Files related to photo generation
  static inline std::string _photoScriptName = "photo.gnu";
  static inline std::string _photoDataFileName = "photo.bin";
  static inline std::string _photoImageName = "photo.png";

  // Files related to video generation (data is already written in Stormer
  // Verlet function)
  static inline std::string _videoScriptName = "video.gnu";
  static inline std::string _videoFolderName = "video";

  /**
   * @brief Says if wa can use a color palette to represent the universe
   * @return true
   * @return false
   */
  bool colorPaletteCanBeUsed() const;

  /**
   * @brief Writes in the photo data file
   *        content of universe current state
   */
  void writePhotoData() const;

  /**
   * @brief Writes the script to generate a video
   *        of universe past states
   */
  void writeVideoScript(size_t numberFrames) const;

  /**
   * @brief Writes the script to generate a photo
   *        of universe current state
   */
  void writePhotoScript() const;

  /**
   * @brief Writes the images sizes setting command in the script
   * @param scriptFile
   */
  void writeImageSizes(std::ofstream& scriptFile) const;

  /**
   * @brief Writes the axes ranges setting command in the script
   * @param scriptFile
   */
  void setAxesRanges(std::ofstream& scriptFile) const;

  /**
   * @brief Writes the plot command in the video script
   * @param scriptFile
   */
  void writeVideoPlotCommand(std::ofstream& scriptFile) const;

  /**
   * @brief Writes the plot command in the photo script
   * @param scriptFile
   */
  void writePhotoPlotCommand(std::ofstream& scriptFile) const;

  /**
   * @brief for a given n, return the string "1:2: ... :n"
   * @param n
   * @return std::string
   */
  std::string dotsRange(size_t n) const;

 public:
  VisualGenerator(const Universe* universe);

  /**
   * @brief Set the sizes for images that will be created
   * @param width
   * @param height
   */
  void setImageSizes(size_t width, size_t height);

  /**
   * @brief Set the size of the points for the plots
   * @param pointSize
   */
  void setPointSize(double pointSize);

  /**
   * @brief Set the shape of the points plotted
   *        Accepts
   *        NO_SYMBOL, PLUS, CROSS, STAR,
   *        BOX, BOX_F, CIRCLE, CIRCLE_F
   * @param pointType
   */
  void setPointType(PointType pointType) { _pointType = pointType; };

  /**
   * @brief Generates a photo of the current state of the universe
   */
  void generatePhoto() const;

  /**
   * @brief Generates a video (multiple images) of the past states
   *        the universe has been into.
   * @param numberFrames number of images to generate
   *                     (must be smaller than past states)
   */
  void generateVideo(size_t numberFrames) const;
};

#endif  // _VISUAL_GENERATOR_HPP_