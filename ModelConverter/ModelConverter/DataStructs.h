#pragma once
#include <vector>

using std::vector;

struct vec2
{
    float x, y;

    vec2(float x = 0, float y = 0) : x(x), y(y) { }

    vec2 operator-(const vec2& v)
    {
        return vec2(x - v.x, y - v.y);
    }

};

struct vec3
{
    float x, y, z;

    vec3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) { }

    vec3 operator-(const vec3& v)
    {
        return vec3(x - v.x, y - v.y, z - v.z);
    }

    vec3 operator*(const float& f)
    {
        // Need this.x, etc. ???
        return vec3(x * f, y * f, z * f);
    }
};

struct oldVertex
{
    vec3 pos;
    vec2 uv;
    vec3 normal;
};

struct newVertex
{
    vec3 pos;
    vec2 uv;
    vec3 normal;
    vec3 binormal;
    vec3 tangent;
};

struct oldFace
{
    vector<vector<unsigned int>> indices; // 3 (maybe 4) sets of 3 indices
};

struct newFace
{
    vector<vector<unsigned int>> indices; // 2 additional spots for binormal and tangent
};