#pragma once

namespace Matrixes
{
    struct Matrix4
    {
        float data[16];
    };
    struct Matrix3x3
    {
        float r00, r01, r02;
        float r10, r11, r12;
        float r20, r21, r22;
    };
}

namespace Vectors
{
    struct Vector2
    {
        float x;
        float y;

        float Magnitude() const
        {
            return std::sqrt(x * x + y * y);
        }

        float Distance(const Vector2& b) const
        {
            float dx = b.x - x;
            float dy = b.y - y;
            return std::sqrt(dx * dx + dy * dy);
        }
    };

    struct Vector3 {
        float x, y, z;

        inline Vector3 operator+(const Vector3& other) const {
            return { x + other.x, y + other.y, z + other.z };
        }
        inline Vector3 operator-(const Vector3& other) const {
            return { x - other.x, y - other.y, z - other.z };
        }
        inline Vector3 operator*(float scalar) const {
            return { x * scalar, y * scalar, z * scalar };
        }

        float Magnitude() const
        {
            return std::sqrt(x * x + y * y + z * z);
        }

        Vector3 Normalize() const
        {
            float len = Magnitude();
            if (len == 0) return { 0, 0, 0 };
            return { x / len, y / len, z / len };
        }

        Vector3 cross(const Vector3& b) const
        {
            return {
                y * b.z - z * b.y,
                z * b.x - x * b.z,
                x * b.y - y * b.x
            };
        }

        float Distance(const Vector3& b) const
        {
            float dx = b.x - x;
            float dy = b.y - y;
            float dz= b.z - z;
            return std::sqrt(dx * dx + dy * dy + dz * dz);
        }
    };

    struct Vector4
    {
        float w;
        float x;
        float y;
        float z;

        static float Dot(const Vector4& a, const Vector4& b)
        {
            return a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;
        }

        void Normalize()
        {
            float mag = std::sqrt(w * w + x * x + y * y + z * z);
            if (mag > 0)
            {
                w /= mag;
                x /= mag;
                y /= mag;
                z /= mag;
            }
        }

        static Vector4 FromMatrix(const Matrixes::Matrix3x3& m)
        {
            Vector4 q;
            float trace = m.r00 + m.r11 + m.r22;
            if (trace > 0)
            {
                float s = std::sqrt(trace + 1.0f) * 2.0f;
                q.w = 0.25f * s;
                q.x = (m.r21 - m.r12) / s;
                q.y = (m.r02 - m.r20) / s;
                q.z = (m.r10 - m.r01) / s;
            }
            else if ((m.r00 > m.r11) && (m.r00 > m.r22))
            {
                float s = std::sqrt(1.0f + m.r00 - m.r11 - m.r22) * 2.0f;
                q.w = (m.r21 - m.r12) / s;
                q.x = 0.25f * s;
                q.y = (m.r01 + m.r10) / s;
                q.z = (m.r02 + m.r20) / s;
            }
            else if (m.r11 > m.r22)
            {
                float s = std::sqrt(1.0f + m.r11 - m.r00 - m.r22) * 2.0f;
                q.w = (m.r02 - m.r20) / s;
                q.x = (m.r01 + m.r10) / s;
                q.y = 0.25f * s;
                q.z = (m.r12 + m.r21) / s;
            }
            else
            {
                float s = std::sqrt(1.0f + m.r22 - m.r00 - m.r11) * 2.0f;
                q.w = (m.r10 - m.r01) / s;
                q.x = (m.r02 + m.r20) / s;
                q.y = (m.r12 + m.r21) / s;
                q.z = 0.25f * s;
            }
            q.Normalize();
            return q;
        }

        Matrixes::Matrix3x3 ToMatrix()
        {
            Matrixes::Matrix3x3 m;

            float xx = x * x;
            float yy = y * y;
            float zz = z * z;
            float xy = x * y;
            float xz = x * z;
            float yz = y * z;
            float wx = w * x;
            float wy = w * y;
            float wz = w * z;

            m.r00 = 1.0f - 2.0f * (yy + zz);
            m.r01 = 2.0f * (xy - wz);
            m.r02 = 2.0f * (xz + wy);

            m.r10 = 2.0f * (xy + wz);
            m.r11 = 1.0f - 2.0f * (xx + zz);
            m.r12 = 2.0f * (yz - wx);

            m.r20 = 2.0f * (xz - wy);
            m.r21 = 2.0f * (yz + wx);
            m.r22 = 1.0f - 2.0f * (xx + yy);

            return m;
        }

        static Vector4 Slerp(const Vector4& a, const Vector4& b, float t)
        {
            t = std::clamp(t, 0.0f, 1.0f);

            float cosTheta = Dot(a, b);

            Vector4 end = b;
            if (cosTheta < 0.0f)
            {
                end.w = -b.w;
                end.x = -b.x;
                end.y = -b.y;
                end.z = -b.z;
                cosTheta = -cosTheta;
            }

            if (cosTheta > 0.9995f)
            {
                Vector4 result = {
                    a.w + t * (end.w - a.w),
                    a.x + t * (end.x - a.x),
                    a.y + t * (end.y - a.y),
                    a.z + t * (end.z - a.z)
                };
                result.Normalize();
                return result;
            }
            else
            {
                float angle = std::acos(cosTheta);
                float sinAngle = std::sqrt(1.0f - cosTheta * cosTheta);
                float invSin = 1.0f / sinAngle;

                float factorA = std::sin((1 - t) * angle) * invSin;
                float factorB = std::sin(t * angle) * invSin;

                Vector4 result = {
                    factorA * a.w + factorB * end.w,
                    factorA * a.x + factorB * end.x,
                    factorA * a.y + factorB * end.y,
                    factorA * a.z + factorB * end.z
                };
                return result;
            }
        }
    };
}

struct sCFrame
{
    float r00, r01, r02;
    float r10, r11, r12;
    float r20, r21, r22;
    float x, y, z;

    inline Vectors::Vector3 Position() const { return { x, y, z }; }
    inline Vectors::Vector3 GetRightVector() const { return { r00, r10, r20 }; }
    inline Vectors::Vector3 GetUpVector() const { return { r01, r11, r21 }; }
    inline Vectors::Vector3 GetLookVector() const { return { -r02, -r12, -r22 }; }

    sCFrame operator+(const sCFrame& other) const
    {
        return {
            r00 + other.r00, r01 + other.r01, r02 + other.r02,
            r10 + other.r10, r11 + other.r11, r12 + other.r12,
            r20 + other.r20, r21 + other.r21, r22 + other.r22,
            x + other.x, y + other.y, z + other.z
        };
    }

    sCFrame operator-(const sCFrame& other) const
    {
        return {
            r00 - other.r00, r01 - other.r01, r02 - other.r02,
            r10 - other.r10, r11 - other.r11, r12 - other.r12,
            r20 - other.r20, r21 - other.r21, r22 - other.r22,
            x - other.x, y - other.y, z - other.z
        };
    }

    sCFrame operator*(float scalar) const
    {
        return {
            r00 * scalar, r01 * scalar, r02 * scalar,
            r10 * scalar, r11 * scalar, r12 * scalar,
            r20 * scalar, r21 * scalar, r22 * scalar,
            x * scalar, y * scalar, z * scalar
        };
    }

    sCFrame operator/(float scalar) const
    {
        return {
            r00 / scalar, r01 / scalar, r02 / scalar,
            r10 / scalar, r11 / scalar, r12 / scalar,
            r20 / scalar, r21 / scalar, r22 / scalar,
            x / scalar, y / scalar, z / scalar
        };
    }
};

inline sCFrame LookAt(const Vectors::Vector3& pos, const Vectors::Vector3& target, const Vectors::Vector3& up = { 0.f, 1.f, 0.f })
{
    Vectors::Vector3 forward = (target - pos).Normalize();
    Vectors::Vector3 right = forward.cross(up).Normalize();
    Vectors::Vector3 realUp = right.cross(forward);

    sCFrame result;

    result.r00 = right.x;
    result.r01 = realUp.x;
    result.r02 = forward.x;

    result.r10 = right.y;
    result.r11 = realUp.y;
    result.r12 = forward.y;

    result.r20 = right.z;
    result.r21 = realUp.z;
    result.r22 = forward.z;

    result.x = pos.x;
    result.y = pos.y;
    result.z = pos.z;

    return result;
}

// more about that ^
// https://create.roblox.com/docs/reference/engine/datatypes/CFrame#lookAt