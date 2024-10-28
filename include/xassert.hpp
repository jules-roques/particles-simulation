/**
 * @file xassert.hpp
 * @author jules roques (jules.roques@grenoble-inp.org)
 * @brief To define an assert macro
 * @version 0.1
 * @date 2024-04-09
 */

#ifndef _XASSERT_HPP_
#define _XASSERT_HPP_

#include <iostream>
#include <sstream>
#include <config.hpp>

inline void _xassert(const char* expr_str, bool expr,
              const char* file, int line, const char* function, std::stringstream&& msg) {
    if (!expr)
    {
        std::cerr << file << ":" << line << " in " << function << "():" << std::endl
                  << "Assertion failed: (" << expr_str << ")" << std::endl
                  << msg.str() << std::endl;
        abort();
    }
}

inline void _xassert(const char* expr_str, bool expr,
              const char* file, int line, const char* function, std::string&& msg) {
    if (!expr)
    {
        std::cerr << file << ":" << line << " in " << function << "():" << std::endl
                  << "Assertion failed: (" << expr_str << ")" << std::endl
                  << msg << std::endl;
        abort();
    }
}

#ifndef NDEBUG
#   define xassert(expr, msg) _xassert(#expr, expr, __FILE__, __LINE__, __func__, msg)
#else
#   define xassert(expr, msg)  // Nothing if no debug
#endif

#endif // _XASSERT_HPP_