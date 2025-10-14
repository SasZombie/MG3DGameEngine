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
#include <glm/gtc/type_ptr.hpp>

#pragma GCC diagnostic pop
#include <algorithm>
#include <iostream>

namespace sas
{
    namespace math
    {
        struct Vec2
        {
            float x, y;
            Vec2() : x(0), y(0) {}
            Vec2(float nx, float ny) : x(nx), y(ny) {}

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
            Vec3(float nx, float ny, float nz) : x(nx), y(ny), z(nz) {}

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
            Vec4(float nx, float ny, float nz, float nw) : x(nx), y(ny), z(nz), w(nw) {}

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
        struct Quaterion;

        // I really dont wanna do all that
        Vec3 crossProduct(const Vec3 &vec1, const Vec3 &vec2) noexcept
        {
            glm::vec3 crosProd = glm::cross(glm::vec3{vec1.x, vec1.y, vec1.z}, glm::vec3{vec2.x, vec2.y, vec2.z});
            return {crosProd.x, crosProd.y, crosProd.z};
        }

        // Limits to my custom vectors
        template <typename T>
        concept customVector = std::same_as<T, Vec2> || std::same_as<T, Vec3> || std::same_as<T, Vec4> || std::same_as<T, Quaterion>;
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
        T clamp(const T &vec1, const T &min, const T &max) noexcept
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

        struct Mat3
        {
            float data[3][3]{};

            Mat3 identity() noexcept
            {
                Mat3 result;

                result.data[0][0] = 1;
                result.data[1][1] = 1;
                result.data[2][2] = 1;

                return result;
            }

            // Not doing all that
            Mat3 operator*(const Mat3 &other) const noexcept
            {
                glm::mat3 m1 = glm::make_mat3(&data[0][0]);
                glm::mat3 m2 = glm::make_mat3(&other.data[0][0]);

                glm::mat3 product = m1 * m2;

                Mat3 result;
                const float *ptr = glm::value_ptr(product);
                for (int col = 0; col < 3; ++col)
                {
                    for (int row = 0; row < 3; ++row)
                    {
                        result.data[row][col] = ptr[col * 3 + row];
                    }
                }

                return result;
            }

            Mat3 transpose() noexcept
            {
                Mat3 result;
                for (int i = 0; i < 3; ++i)
                {
                    for (int j = 0; j < 3; ++j)
                    {
                        result.data[i][j] = data[j][i];
                    }
                }

                return result;
            }

            // Diabolical, calculate adjugate + inverse + check det != 0
            Mat3 inverse() noexcept
            {
                glm::mat3 m1 = glm::make_mat3(&data[0][0]);

                if (glm::determinant(m1) == 0.0f)
                {
                    std::cerr << "Determinant is 0\n";
                    return {};
                }

                glm::mat3 product = glm::inverse(m1);

                Mat3 result;
                const float *ptr = glm::value_ptr(product);
                for (int col = 0; col < 3; ++col)
                {
                    for (int row = 0; row < 3; ++row)
                    {
                        result.data[row][col] = ptr[col * 3 + row];
                    }
                }

                return result;
            }
        };

        struct Mat4
        {
            float data[4][4]{};

            Mat4 identity() noexcept
            {
                Mat4 result;

                result.data[0][0] = 1;
                result.data[1][1] = 1;
                result.data[2][2] = 1;
                result.data[3][3] = 1;

                return result;
            }

            Mat4 operator*(const Mat4 &other) const noexcept
            {
                glm::mat4 m1 = glm::make_mat4(&data[0][0]);
                glm::mat4 m2 = glm::make_mat4(&other.data[0][0]);

                glm::mat4 product = m1 * m2;

                Mat4 result;
                const float *ptr = glm::value_ptr(product);
                for (int col = 0; col < 4; ++col)
                {
                    for (int row = 0; row < 4; ++row)
                    {
                        result.data[row][col] = ptr[col * 4 + row];
                    }
                }

                return result;
            }

            Mat4 transpose() noexcept
            {
                Mat4 result;
                for (int i = 0; i < 4; ++i)
                {
                    for (int j = 0; j < 4; ++j)
                    {
                        result.data[i][j] = data[j][i];
                    }
                }

                return result;
            }

            Mat4 inverse() noexcept
            {
                glm::mat4 m1 = glm::make_mat4(&data[0][0]);

                if (glm::determinant(m1) == 0.0f)
                {
                    std::cerr << "Determinant is 0\n";
                    return {};
                }

                glm::mat4 product = glm::inverse(m1);

                Mat4 result;
                const float *ptr = glm::value_ptr(product);
                for (int col = 0; col < 4; ++col)
                {
                    for (int row = 0; row < 4; ++row)
                    {
                        result.data[row][col] = ptr[col * 4 + row];
                    }
                }

                return result;
            }
        };

        // I have to admit
        // I have no idea what this is
        // Or used for, but I shall try
        // To implement the formulas off the internet
        struct Quaterion : public Vec4
        {
            using Vec4::operator*;

            Quaterion() noexcept = default;

            Quaterion(const Vec3 &vec) noexcept
                :   Vec4(vec.x, vec.y, vec.z, 0)
            {
            }

            Quaterion(const Vec4 &vec) noexcept
                : Vec4(vec)
            {
            }

            Quaterion angleAxis(float angle) noexcept
            {

                Vec4 norm = this->normalized();
                Quaterion result;

                result.w = std::cos(norm.w / 2);
                result.x = norm.x * std::sin(norm.w / 2);
                result.y = norm.y * std::sin(norm.w / 2);
                result.z = norm.z * std::sin(norm.w / 2);

                return result;
            }

            // This assumes Radians
            Quaterion eulerAngles(const Vec3 &angles) noexcept
            {
                Quaterion result;
                // Roll
                float cr = std::cos(angles.x / 2);
                float sr = std::sin(angles.x / 2);

                // Pitch
                float cp = std::cos(angles.y / 2);
                float sp = std::sin(angles.y / 2);

                // Yaw
                float cy = std::sin(angles.z / 2);
                float sy = std::sin(angles.z / 2);

                result.w = cr * cp * cy + sr * sp * sy;
                result.x = sr * cp * cy - cr * sp * sy;
                result.y = cr * sp * cy + sr * cp * sy;
                result.z = cr * cp * sy - sr * sp * cy;

                return result;
            }

            Quaterion operator*(const Quaterion &other) const noexcept
            {
                Quaterion result;

                result.w = other.w * w - other.x * x - other.y * y - other.z * z;
                result.x = other.w * x + other.x * w + other.y * z - other.z * y;
                result.y = other.w * y - other.x * z + other.y * w + other.z * x;
                result.z = other.w * z + other.x * y - other.y * x + other.z * w;

                return result;
            }

            Quaterion conjugate() const noexcept
            {
                Quaterion result;
                result.w = w;
                result.x = -1 * x;
                result.y = -1 * y;
                result.z = -1 * z;

                return result;
            }

            float sqMagnitude() const noexcept
            {
                return w * w + x * x + y * y + z * z;
            }

            Quaterion inverse() const noexcept
            {
                return conjugate() / sqMagnitude();
            }

            Quaterion rotate(const Vec3 &roation) const noexcept
            {
                return *this * Quaterion{roation} * inverse();
            }
        };

        Quaterion slerp(const Quaterion &q1, const Quaterion& q2, float t) noexcept
        {
            if(t > 1 || t < 0)
            {
                std::cerr << "T is not between [0, 1]\n";
                return {};
            }

            float theta = dotProduct(q1, q2);
            float sinTheta = std::sin(theta);

            return (q1 * (std::sin((1 - theta) * theta)) / sinTheta) + (q2 * (std::sin(theta * t) / sinTheta));
        }

    } // namespace math

} // namespace sas