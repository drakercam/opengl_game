#ifndef GL_MATH_H
#define GL_MATH_H

#include <cmath>

#define PI 3.14159265358979323846f
#define MAT3_SIZE 9
#define MAT4_SIZE 16

struct vec2 {

    float x, y;

    vec2() {
        this->x = 0;
        this->y = 0;
    }

    vec2(float x, float y) {
        this->x = x;
        this->y = y;
    }

    vec2 operator+(vec2& other) {
        return {this->x + other.x, this->y + other.y};
    }

    vec2 operator-(vec2& other) {
        return {this->x - other.x, this->y - other.y};
    }

    vec2 operator*(float value) {
        return {this->x * value, this->y * value};
    }

    static vec2 add(vec2& a, vec2& b) {
        return a + b;
    }

    static vec2 sub(vec2& a, vec2& b) {
        return a - b;
    }

    static vec2 scale(vec2& v, float s) {
        return v * s;
    }

    static float length(vec2& v) {
        return sqrtf(v.x*v.x + v.y*v.y);
    }

    static float dot(vec2& v1, vec2& v2) {
        return v1.x*v2.x + v1.y*v2.y;
    }

    static vec2 normalize(vec2& v) {

        float length = vec2::length(v);
        if (length == 0.0f) {
            return {0.0f, 0.0f};
        }

        return { v.x / length, v.y / length  };
    }

    static float distance(vec2& v1, vec2& v2) {

        return sqrtf(((v1.x-v2.x)*(v1.x-v2.x)) + ((v1.y-v2.y)*(v1.y-v2.y)));
    }

    static float distanceSq(vec2& v1, vec2& v2) {

        return ((v1.x-v2.x)*(v1.x-v2.x)) + ((v1.y-v2.y)*(v1.y-v2.y));
    }

    static vec2 lerp(vec2& v1, vec2& v2, float t) {

        vec2 diff = v2 - v1;
        vec2 scaled = diff * t;
        vec2 result = v1 + scaled;
        return result;
    }
};

struct vec3 {

    float x, y, z;

    vec3() {
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }

    vec3(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    vec3(const vec3& v) {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
    }

    vec3 operator+(const vec3& other) const {
        return vec3(this->x + other.x, this->y + other.y, this->z + other.z);
    }

    vec3 operator-(const vec3& other) const {
        return vec3(this->x - other.x, this->y - other.y, this->z - other.z);
    }

    vec3 operator*(const float value) const {
        return vec3(this->x * value, this->y * value, this->z * value);
    }

    vec3& operator+=(const vec3& other) {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;

        return *this;
    }

    vec3& operator-=(const vec3& other) {
        this->x -= other.x;
        this->y -= other.y;
        this->z -= other.z;

        return *this;
    }

    static vec3 add(vec3& a, vec3& b) {
        return a + b;
    }

    static vec3 sub(vec3& a, vec3& b) {
        return a - b;
    }

    static vec3 scale(vec3& v, float s) {
        return v * s;
    }

    static float length(vec3& v) {
        return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
    }

    static float dot(vec3& v1, vec3& v2) {
        return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
    }

    static vec3 normalize(vec3 v) {

        float length = vec3::length(v);
        if (length == 0.0f) {
            return {0.0f, 0.0f, 0.0f};
        }

        return { v.x / length, v.y / length, v.z / length };
    }

    static float distance(vec3& v1, vec3& v2) {

        return sqrtf(((v1.x-v2.x)*(v1.x-v2.x)) + ((v1.y-v2.y)*(v1.y-v2.y)) + ((v1.z-v2.z)*(v1.z-v2.z)));
    }

    static float distanceSq(vec3& v1, vec3& v2) {

        return ((v1.x-v2.x)*(v1.x-v2.x)) + ((v1.y-v2.y)*(v1.y-v2.y)) + ((v1.z-v2.z)*(v1.z-v2.z));
    }

    static vec3 lerp(vec3& v1, vec3& v2, float t) {

        vec3 diff = v2 - v1;
        vec3 scaled = diff * t;
        vec3 result = v1 + scaled;
        return result;
    }

    static vec3 cross(const vec3& vec1, const vec3& vec2)
    {
        vec3 result{};

        result.x = vec1.y * vec2.z - vec1.z * vec2.y;
        result.y = vec1.z * vec2.x - vec1.x * vec2.z;
        result.z = vec1.x * vec2.y - vec1.y * vec2.x;

        return result;
    }
};

struct vec4 {

    float x, y, z, w;

    vec4() {
        this->x = 0;
        this->y = 0;
        this->z = 0;
        this->w = 1.0f;
    }

    vec4(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = 1.0f;
    }

    vec4(float x, float y, float z, float w) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    vec4 operator+(vec4& other) {
        return vec4(this->x + other.x, this->y + other.y, this->z + other.z);
    }

    vec4 operator-(vec4& other) {
        return vec4(this->x - other.x, this->y - other.y, this->z - other.z);
    }

    vec4 operator*(float value) {
        return vec4(this->x * value, this->y * value, this->z * value);
    }

    static vec4 add(vec4& a, vec4& b) {
        return a + b;
    }

    static vec4 sub(vec4& a, vec4& b) {
        return a - b;
    }

    static vec4 scale(vec4& v, float s) {
        return v * s;
    }

    static float length(vec4& v) {
        return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w);
    }

    static float dot(vec4& v1, vec4& v2) {
        return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w;
    }

    static vec4 normalize(vec4& v) {

        float length = vec4::length(v);
        if (length == 0.0f) {
            return {0.0f, 0.0f, 0.0f};
        }

        return { v.x / length, v.y / length, v.z / length };
    }

    static float distance(vec4& v1, vec4& v2) {

        return sqrtf(((v1.x-v2.x)*(v1.x-v2.x)) + ((v1.y-v2.y)*(v1.y-v2.y)) + ((v1.z-v2.z)*(v1.z-v2.z)));
    }

    static float distanceSq(vec4& v1, vec4& v2) {

        return ((v1.x-v2.x)*(v1.x-v2.x)) + ((v1.y-v2.y)*(v1.y-v2.y)) + ((v1.z-v2.z)*(v1.z-v2.z));
    }

    static vec4 lerp(vec4& v1, vec4& v2, float t) {

        vec4 diff = v2 - v1;
        vec4 scaled = diff * t;
        vec4 result = v1 + scaled;
        return result;
    }
};

struct ops {

    static float degreesToRadians(float degrees) {

        return (degrees * PI / 180.0f);
    }

    static float radiansToDegrees(float radians) {

        return (radians * 180.0f / PI);
    }

    static float clamp(float x, float min, float max) {

        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    static vec2 vec2Clamp(vec2& v, vec2& min, vec2& max) {

        return { ops::clamp(v.x, min.x, max.x), ops::clamp(v.y, min.y, max.y) };
    }

    static vec3 vec3Clamp(vec3& v, vec3& min, vec3& max) {

        return { ops::clamp(v.x, min.x, max.x), ops::clamp(v.y, min.y, max.y), ops::clamp(v.z, min.z, max.z) };
    }
};

struct mat3 {
    float m[MAT3_SIZE];     // column major

    static mat3 identity() {
        return {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
        };
    }

    static vec3 multiplyWithvec3(mat3& m, vec3& v) {
        return {
            m.m[0]*v.x + m.m[3]*v.y + m.m[6]*v.z,
            m.m[1]*v.x + m.m[4]*v.y + m.m[7]*v.z,
            m.m[2]*v.x + m.m[5]*v.y + m.m[8]*v.z
        };
    }

    static mat3 multiplyWithmat3(mat3& a, mat3& b) {

        mat3 result{0};

        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 3; ++col) {
                result.m[col*3 + row] =
                a.m[0*3 + row] * b.m[col*3 + 0] +
                a.m[1*3 + row] * b.m[col*3 + 1] +
                a.m[2*3 + row] * b.m[col*3 + 2];
            }
        }

        return result;
    }

    static mat3 rotate(float theta, vec3& axis) {

        vec3 n = vec3::normalize(axis);

        float c = cosf(theta);
        float s = sinf(theta);
        float ic = 1.0f - c;

        return {
            {c + n.x*n.x*ic,      n.x*n.y*ic - n.z*s,  n.x*n.z*ic + n.y*s,
                n.y*n.x*ic + n.z*s,  c + n.y*n.y*ic,      n.y*n.z*ic - n.x*s,
                n.z*n.x*ic - n.y*s,  n.z*n.y*ic + n.x*s,  c + n.z*n.z*ic}
        };
    }
};

struct mat4 {
    float m[MAT4_SIZE];    // column major

    static void identity(mat4& transform) {
        transform = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
        };
    }

    static void multiply(mat4& transform, const mat4& m2) {

        mat4 res{};

        for (int row = 0; row < 4; ++row)
        {
            for (int col = 0; col < 4; ++col)
            {
                for (int k = 0; k < 4; ++k)
                {
                    res.m[col*4 + row] += transform.m[k*4+row] * m2.m[col*4+k];
                }
            }
        }

        transform = res;
    }

    static void translate(mat4& transform, vec3 t) {

        mat4 translation;
        translation = {
            {1,0,0,0,
                0,1,0,0,
                0,0,1,0,
                t.x,t.y,t.z,1}
        };
        mat4::multiply(transform, translation);
    }

    static void scale(mat4& transform, vec3 s) {

        mat4 scaled;
        scaled = {
            {s.x,0,0,0,
                0,s.y,0,0,
                0,0,s.z,0,
                0,0,0,1}
        };
        mat4::multiply(transform, scaled);
    }

    static void rotate(mat4& transform, float theta, vec3 axis) {

        mat4 rotation;
        vec3 nA = vec3::normalize(axis);
        float c = cosf(theta);
        float s = sinf(theta);
        float ic = (1-c);

        rotation = {
            {c+(nA.x*nA.x)*ic,      nA.y*nA.x*ic+nA.z*s,    nA.z*nA.x*ic-nA.y*s,    0,  // Col 1
                nA.x*nA.y*ic-nA.z*s,   c+(nA.y*nA.y)*ic,       nA.y*nA.z*ic+nA.x*s,    0,  // Col 2
                nA.x*nA.z*ic+nA.y*s,   nA.y*nA.z*ic-nA.x*s,    c+(nA.z*nA.z)*ic,       0,  // Col 3
                0,                     0,                      0,                      1}  // Col 4
        };
        mat4::multiply(transform, rotation);
    }

    static void rotateX(mat4& transform, float theta) {

        mat4 rotation;
        float c = cosf(theta);
        float s = sinf(theta);

        rotation = {
            {1, 0, 0, 0,
                0, c, -s, 0,
                0, s, c, 0,
                0, 0, 0, 1}
        };
        mat4::multiply(transform, rotation);
    }

    static void rotateY(mat4& transform, float theta) {

        mat4 rotation;
        float c = cosf(theta);
        float s = sinf(theta);

        rotation = {
            {c, 0, s, 0,
                0, 1, 0, 0,
                -s, 0, c, 0,
                0, 0, 0, 1}
        };
        mat4::multiply(transform, rotation);
    }

    static void rotateZ(mat4& transform, float theta) {

        mat4 rotation;
        float c = cosf(theta);
        float s = sinf(theta);

        rotation = {
            {c, -s, 0, 0,
                s, c, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1}
        };
        mat4::multiply(transform, rotation);
    }

    static void getProjection(mat4& result, float fov, float aspect, float nearPlane, float farPlane) {

        mat4::identity(result);

        float tanHalfFOV = tanf(fov * 0.5f);
        float zRange = farPlane - nearPlane;

        result.m[0]  = 1.0f / (tanHalfFOV * aspect);
        result.m[5]  = 1.0f / tanHalfFOV;
        result.m[10] = -(farPlane + nearPlane) / zRange;
        result.m[11] = -1.0f;
        result.m[14] = -2.0f * farPlane * nearPlane / zRange;
        result.m[15] = 0.0f;
    }

    static void getOrthographic(mat4& result, float l, float r, float b, float t, float near, float far) {

        mat4::identity(result);

        result.m[0]  = 2.0f / (r - l);
        result.m[5]  = 2.0f / (t - b);
        result.m[10] = -2.0f / (far - near);
        result.m[12] = -((r + l) / (r - l));
        result.m[13] = -((t + b) / (t - b));
        result.m[14] = -((far + near) / (far - near));
    }

    static void lookAt(mat4& transform, vec3 eye, vec3 target, vec3 up) {

        vec3 forward = vec3::normalize(target - eye);
        vec3 right   = vec3::normalize(vec3::cross(forward, up));
        vec3 upAxis  = vec3::cross(right, forward);

        transform = {
            {
                right.x,    upAxis.x,   -forward.x,    0.0f,
                right.y,    upAxis.y,   -forward.y,    0.0f,
                right.z,    upAxis.z,   -forward.z,    0.0f,

                -vec3::dot(right, eye),
                -vec3::dot(upAxis, eye),
                vec3::dot(forward, eye),
                1.0f
            }
        };
    }
};

struct quat {

};

struct ray {
    vec3 origin;
    vec3 direction;

    static vec3 pointAt(const ray& r, float t) {
        return r.origin + r.direction * t;
    }

    static bool sphereIntersect(ray& r, const vec3& sphereCenter, float sphereRadius, float& hitDistance) {
        vec3 originCenter = r.origin - sphereCenter;

        // 1.0 because the ray direction is normalized
        float a = 1.0f;

        // projecting the vector from sphere center to ray origin onto the ray direction
        // tells us how far along the ray direction the sphere center lies, tells us if sphere
        // is behind or infront of the ray
        float b = vec3::dot(originCenter, r.direction);

        // if c < 0, it tells us that the ray starts inside the sphere
        float c = vec3::dot(originCenter, originCenter) - sphereRadius*sphereRadius;

        // the discriminant tells us if the ray gets close enough to the sphere
        // < 0 means it misses, = 0 ray grazes the sphere, > 0 means ray enters and exits the sphere
        float discriminant = b * b - c;

        // ray missed, no collision
        if (discriminant < 0)
        {
            return false;
        }

        // the two solutions to the quadratic equation
        // t1 is where the ray enters, t2 is where the ray exits
        float t1 = (-b - sqrt(discriminant));
        float t2 = (-b + sqrt(discriminant));

        // we want to track the closest valid hit
        float tMin = std::numeric_limits<float>::infinity();

        // we choose the nearest positive intersection
        if (t1 > 0 && t1 < tMin) tMin = t1;
        if (t2 > 0 && t2 < tMin) tMin = t2;

        // check if we actually hit something
        if (tMin < std::numeric_limits<float>::infinity())
        {
            hitDistance = tMin;
            return true;    // return true if we hit
        }

        return false;   // return false if we did not
    }
};

#endif
