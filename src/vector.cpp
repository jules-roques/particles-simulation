#include <vector.hpp>
#include <cmath>
#include <xassert.hpp>

/* --------------------------- private --------------------------- */

/* --------------------------- public --------------------------- */

void Vector::
operator*=(double scalar) {
    for (size_t i = 0; i < _dimension; ++i)
        _data[i] *= scalar;
}

void Vector::
operator+=(const Vector &other) {
    for (size_t i = 0; i < _dimension; ++i)
        _data[i] += other._data[i];
}

void Vector::
operator-=(const Vector &other) {
    for (size_t i = 0; i < _dimension; ++i)
        _data[i] -= other._data[i];
}

bool Vector::
operator==(const Vector &other) const {
    xassert(_dimension == other._dimension, "Vectors dimensions must match.");
    for (size_t i = 0; i < _dimension; i++) {
        if (_data[i] != other._data[i]) return false;
    }
    return true;
}

bool Vector::
operator!=(const Vector &other) const {
    return !operator==(other);
}

bool Vector::
areAllCoordsGreater(const Vector &other) const {
    xassert(_dimension == other._dimension, "Vectors dimensions must match.");
    for (size_t i = 0; i < _dimension; i++) {
        if (_data[i] <= other._data[i]) return false;
    }
    return true;
}

bool Vector::
isInBounds(const Vector &lowerBound, const Vector &upperBound) const {
    xassert(lowerBound._dimension == upperBound._dimension, "Bounds dimensions must match.");
    xassert(_dimension == lowerBound._dimension, "Vector dimension must match with bounds.");
    xassert(upperBound.areAllCoordsGreater(lowerBound),
            "upperBound coordinates must be greater than lowerBound coordinates.");

    for (size_t i = 0; i < _dimension; i++)
        if (_data[i] < lowerBound._data[i] || _data[i] > upperBound._data[i]) return false;
    return true;
}

double Vector::
norm() const {
    double sum = 0;
    for (size_t i = 0; i < _dimension; ++i)
        sum += _data[i] * _data[i];
    return std::sqrt(sum);
}

Vector min(Vector v1, Vector v2) {
    xassert(v1.getDimension() == v2.getDimension(),
            "Vectors must be of same dimension.");
    size_t dim = v1.getDimension();
    Vector result = v1;
    for (size_t i = 0; i < dim; i++) {
        if (v2._data[i] < result._data[i])
            result._data[i] = v2._data[i];
    }
    return result;
}

Vector max(Vector v1, Vector v2) {
    xassert(v1.getDimension() == v2.getDimension(),
            "Vectors must be of same dimension.");
    size_t dim = v1.getDimension();
    Vector result = v1;
    for (size_t i = 0; i < dim; i++) {
        if (v2._data[i] > result._data[i])
            result._data[i] = v2._data[i];
    }
    return result;
}


std::ostream& operator<<(std::ostream &strm, const Vector &v) {
    strm << "(";
    for (size_t i = 0; i < v._dimension - 1; i++)
        strm << v[i] << ", ";
    strm << v[v._dimension - 1] << ")";
    return strm;
}