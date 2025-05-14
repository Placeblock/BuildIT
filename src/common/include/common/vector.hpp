//
// Created by felix on 12.05.25.
//

#ifndef VECTOR_HPP
#define VECTOR_HPP

namespace Math {

template <typename Scalar>
struct Vector {
    Scalar x, y;

    explicit Vector(Scalar x = 0, Scalar y = 0) : x(x), y(y) {
    }

    Vector& operator+=(const Vector& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector& operator*=(const Scalar& scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    friend Vector operator+(Vector lhs, const Vector& rhs) {
        lhs += rhs;
        return lhs;
    }

    friend Vector operator*(Vector lhs, const Scalar& rhs) {
        lhs *= rhs;
        return lhs;
    }

    bool operator==(const Vector& other) const {
        return x == other.x && y == other.y;
    }
};

}

#endif //VECTOR_HPP
