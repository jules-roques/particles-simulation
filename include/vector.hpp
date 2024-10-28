/**
 * @file vector.hpp
 * @author jules roques (jules.roques@grenoble-inp.org)
 * @brief File for vector calculations
 * @version 0.1
 * @date 2024-04-08
 */

#ifndef _VECTOR_HPP_
#define _VECTOR_HPP_

#include <vector>
#include <ostream>

/**
 * @brief Vector class
 */
class Vector {
private:
    size_t _dimension;
    std::vector<double> _data;

public:
    // Constructors
    Vector(size_t size) : _dimension(size), _data(size, 0) {}
    Vector() : Vector(0) {}
    Vector(std::initializer_list<double> values) : _dimension(values.size()), _data(values) {}

    // Getters
    double&                    operator[](size_t index)       { return _data[index]; }
    double                     operator[](size_t index) const { return _data[index]; }
    const std::vector<double>& getData()                const { return _data; }
    size_t                     getDimension()           const { return _dimension; }

    // Calculations
    bool                 operator==(const Vector &other) const;
    bool                 operator!=(const Vector &other) const;
    void                 operator+=(const Vector &other);
    void                 operator-=(const Vector &other);
    void                 operator*=(double scalar);

    /**
     * @brief Tell for each dimension if the coords of the calling vector
     *        is greater than the other vector given
     * @param other 
     * @return true if all coords are greater
     * @return false if not
     */
    bool areAllCoordsGreater(const Vector &other) const;

    /**
     * @brief Tell if a vector is in a certain cubic space
     * @param lowerBound 
     * @param upperBound 
     * @return true if the vector is in bounds given
     * @return false if not
     */
    bool isInBounds(const Vector &lowerBound, const Vector &upperBound) const;

    /**
     * @brief Returns vector with min on each coordinates
     * @param v1 
     * @param v2 
     * @return Vector 
     */
    friend Vector min(Vector v1, Vector v2);

    /**
     * @brief Returns vector with max on each coordinates
     * @param v1 
     * @param v2 
     * @return Vector 
     */
    friend Vector max(Vector v1, Vector v2);

    /**
     * @brief give the norm of the vector
     *        i.e. square root of sum of squares
     * @return double 
     */
    double norm() const;

    /**
     * @brief Overrides the << operator
     * @param strm 
     * @param v 
     * @return std::ostream& 
     */
    friend std::ostream& operator<<(std::ostream &strm, const Vector &v);
};

#endif // _VECTOR_HPP_