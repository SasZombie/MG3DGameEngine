/// @brief
/// Not really sure what are we supposed
/// To do here? Wrapper around glm?
/// Ngl not gonna use anything I write here
/// Since glm is way more optimized with SIMD and stuff
#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wshadow"

#include <glm/glm.hpp>

#pragma GCC diagnostic pop
#include <algorithm>

namespace sas
{
    namespace math
    {
        struct Vec2
        {
            float x, y;
            Vec2() : x(0), y(0) {}
            Vec2(float x, float y) : x(x), y(y) {}

            Vec2 operator+(const Vec2 &other) const
            {
                return {x + other.x, y + other.y};
            }
            Vec2 operator-(const Vec2 &other) const
            {
                return {x - other.x, y - other.y};
            }

            Vec2 operator*(float scalar) const
            {
                return {x * scalar, y * scalar};
            }

            Vec2 operator/(float scalar) const
            {
                return {x / scalar, y / scalar};
            }

            float length() const
            {
                return std::sqrt(x * x + y * y);
            }

            Vec2 normalized() const
            {
                return {*this / length()};
            }
        };

        struct Vec3
        {
            float x, y, z;
            Vec3() : x(0), y(0), z(0) {}
            Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

            Vec3 operator+(const Vec3 &other) const
            {
                return {x + other.x, y + other.y, z + other.z};
            }
            Vec3 operator-(const Vec3 &other) const
            {
                return {x - other.x, y - other.y, z - other.z};
            }

            Vec3 operator*(float scalar) const
            {
                return {x * scalar, y * scalar, z * scalar};
            }

            Vec3 operator/(float scalar) const
            {
                return {x / scalar, y / scalar, z / scalar};
            }

            float length() const
            {
                return std::sqrt(x * x + y * y + z * z);
            }

            Vec3 normalized() const
            {
                return {*this / length()};
            }
        };

        struct Vec4
        {
            float x, y, z, w;
            Vec4() : x(0), y(0), z(0), w(0) {}
            Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

            Vec4 operator+(const Vec4 &other) const
            {
                return {x + other.x, y + other.y, z + other.z, w + other.w};
            }
            Vec4 operator-(const Vec4 &other) const
            {
                return {x - other.x, y - other.y, z - other.z, w - other.w};
            }

            Vec4 operator*(float scalar) const
            {
                return {x * scalar, y * scalar, z * scalar, w * scalar};
            }

            Vec4 operator/(float scalar) const
            {
                return {x / scalar, y / scalar, z / scalar, w / scalar};
            }

            float length() const
            {
                return std::sqrt(x * x + y * y + z * z + w * w);
            }

            Vec4 normalized() const
            {
                return {*this / length()};
            }
        };

        // I really dont wanna do all that
        Vec3 crossProduct(const Vec3 &vec1, const Vec3 &vec2) noexcept
        {
            glm::vec3 crosProd = glm::cross(glm::vec3{vec1.x, vec1.y, vec1.z}, glm::vec3{vec2.x, vec2.y, vec2.z});
            return {crosProd.x, crosProd.y, crosProd.z};
        }

        // Limits to my custom vectors
        template <typename T>
        concept customVector = std::same_as<T, Vec2> || std::same_as<T, Vec3> || std::same_as<T, Vec4>;
        // Ngl I am kinda impressed
        // That I can actually do this
        // Pretty awesome feature
        template <customVector T>
        float dotProduct(const T &vec1, const T &vec2) noexcept
        {
            float result = vec1.x * vec2.x + vec1.y * vec2.y;
            if constexpr (requires { vec1.z; })
            {
                result = result + vec1.z * vec2.z;

                if constexpr (requires { vec1.w; })
                {
                    result = result + vec1.w * vec2.w;
                }
            }

            return result;
        }

        template <customVector T>
        float distance(const T &vec1, const T &vec2) noexcept
        {
            float result = (vec2.x - vec1.x) * (vec2.x - vec1.x) + (vec2.y - vec1.y) * (vec2.y * vec1.y);
            if constexpr (requires { vec1.z; })
            {
                result = result + (vec2.z - vec1.z) * (vec2.z - vec1.z);

                if constexpr (requires { vec1.w; })
                {
                    result = result + (vec2.w - vec1.w) * (vec2.w - vec1.w);
                }
            }

            return std::sqrt(result);
        }

        template <customVector T>
        T projection(const T &vec1, const T &vec2) noexcept
        {
            float scalar = dotProduct(vec1, vec2) / dotProduct(vec2, vec2);

            return vec2 * scalar;
        }

        template <customVector T>
        T lerp(const T &vec1, const T &vec2, float t) noexcept
        {
            return vec1 * (1 - t) + vec2 * t;
        }

        template <customVector T>
        T min(const T &vec1, const T &vec2) noexcept
        {
            T result;
            result.x = std::min(vec1.x, vec2.x);
            result.y = std::min(vec1.y, vec2.y);

            if constexpr (requires { vec1.z; })
            {
                result.z = std::min(vec1.z, vec2.z);

                if constexpr (requires { vec1.w; })
                {
                    result.w = std::min(vec1.w, vec2.w);
                }
            }

            return result;
        }

        template <customVector T>
        T max(const T &vec1, const T &vec2) noexcept
        {
            T result;
            result.x = std::max(vec1.x, vec2.x);
            result.y = std::max(vec1.y, vec2.y);

            if constexpr (requires { vec1.z; })
            {
                result.z = std::max(vec1.z, vec2.z);

                if constexpr (requires { vec1.w; })
                {
                    result.w = std::max(vec1.w, vec2.w);
                }
            }

            return result;
        }


        template <customVector T>
        T clamp(const T &vec1, const T& min, const T& max) noexcept
        {
            T result;
            result.x = std::clamp(vec1.x, min.x, max.x);
            result.y = std::clamp(vec1.y, min.y, max.y);

            if constexpr (requires { vec1.z; })
            {
                result.z = std::clamp(vec1.z, min.y, max.y);

                if constexpr (requires { vec1.w; })
                {
                    result.w = std::clamp(vec1.w, min.y, max.y);
                }
            }

            return result;
        }



    } // namespace math

} // namespace sas

// 3. Matrix Operations

// For Mat3/Mat4:

// Identity matrix: identity()

// Matrix multiplication: * operator

// Transpose: transpose(mat)

// Inverse: inverse(mat)

// Determinant: determinant(mat)

// Creation helpers:

// Translation matrix

// Rotation matrix (Euler angles or axis-angle)

// Scale matrix

// Perspective projection matrix

// Orthographic projection matrix

// LookAt matrix (view matrix)

// Example:

// Mat4 mat = Mat4::translate({0, 1, 0}) * Mat4::rotate(angle, axis) * Mat4::scale({1,2,1});

// 4. Quaternions

// Construction from axis-angle

// Construction from Euler angles

// Multiplication (q1 * q2)

// Rotation of vectors: rotatedVec = q * v

// Normalization & inverse

// SLERP (spherical linear interpolation)

// 5. Utilities

// Constants: PI, TAU, EPSILON

// Conversion functions: degrees ↔ radians

// Random vectors (optional)

// Basic geometric utilities:

// reflect(v, n) → reflection over a normal

// refract(v, n, eta) → refraction

// angle_between(v1, v2)