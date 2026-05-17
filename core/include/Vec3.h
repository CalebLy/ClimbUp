#pragma once

#include <cmath>
#include <stdexcept>

struct Vec3
{
    double x;
    double y;
    double z;

    // =========================
    // Basic arithmetic
    Vec3 operator+(const Vec3& other) const
    {
        return {x + other.x, y + other.y, z + other.z};
    }

    Vec3 operator-(const Vec3& other) const
    {
        return {x - other.x, y - other.y, z - other.z};
    }

    Vec3 operator*(double scalar) const
    {
        return {x * scalar, y * scalar, z * scalar};
    }

    Vec3 operator/(double scalar) const
    {
        if (scalar == 0.0)
        {
            throw std::invalid_argument("Cannot divide Vec3 by zero.");
        }

        return {x / scalar, y / scalar, z / scalar};
    }

    // =========================
    // Compound assignment
    Vec3& operator+=(const Vec3& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;

        return *this;
    }

    Vec3& operator-=(const Vec3& other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;

        return *this;
    }

    Vec3& operator*=(double scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;

        return *this;
    }

    // =========================
    // Magnitude
    double length() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    // Use this to avoid sqrt when you only need relative distance
    double lengthSquared() const
    {
        return x * x + y * y + z * z;
    }

    // =========================
    // Vector operations
    double dot(const Vec3& other) const
    {
        return x * other.x + y * other.y + z * other.z;
    }

    Vec3 normalized() const
    {
        double len = length();

        if (len == 0.0)
        {
            throw std::invalid_argument("Cannot normalize zero vector.");
        }

        return *this / len;
    }

    double distanceTo(const Vec3& other) const
    {
        return (*this - other).length();
    }
};