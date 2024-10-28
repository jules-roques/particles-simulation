/**
 * @file config.hpp
 * @author jules roques (jules.roques@grenoble-inp.org)
 * @brief Contains constants definitions for preprocessing of the code.
 * 
 * This header file defines several preprocessor directives that control various
 * aspects of the program's behavior, including debugging, output formats, and 
 * progress information.
 * 
 * @version 0.1
 * @date 2024-04-12
 */


#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

/**
 * @def SHOW_PROGRESS_INFOS
 * @brief Enables the display of progress information during the program's execution.
 * 
 * When defined, this directive allows the program to output information about its
 * progress, which can be useful for monitoring and debugging long-running tasks.
 */
#define SHOW_PROGRESS_INFOS

/**
 * @def NDEBUG
 * @brief Disables all assertions (xassert) to potentially improve performance.
 * 
 * When defined, this directive deactivates all xassert statements in the code,
 * which can help improve performance by avoiding the overhead of these checks.
 * Uncomment this line to enable this behavior.
 */
#define NDEBUG

/**
 * @def PNG_OUTPUT
 * @brief Enables the generation of output in PNG format.
 * 
 * When defined, this directive allows the program to write its output in PNG format.
 * This is useful for generating visual representations of the simulation data.
 */
#define PNG_OUTPUT

/**
 * @def XML_OUTPUT
 * @brief Enables the generation of output in XML format.
 * 
 * When defined, this directive allows the program to write its output in XML format.
 * This can be useful for exporting data in a structured and widely-used format for
 * further processing or analysis. Uncomment this line to enable this behavior.
 */
// #define XML_OUTPUT

#endif // _CONFIG_HPP_
