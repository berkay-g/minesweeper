#pragma once

#include <utility>

struct Vec2i
{
    int x, y;

    Vec2i(int _x = 0, int _y = 0) : x(_x), y(_y) {}

    Vec2i operator+(Vec2i const &other)
    {
        return Vec2i(x + other.x, y + other.y);
    }

    bool operator==(Vec2i const &other)
    {
        return (x == other.x) && (y == other.y);
    }

    bool operator!=(Vec2i const &other)
    {
        return (x != other.x) && (y != other.y);
    }
};

struct Vec3i
{
    int x, y, z;

    Vec3i(int _x = 0, int _y = 0, int _z = 0) : x(_x), y(_y), z(_z) {}

    Vec3i operator+(Vec3i const &other)
    {
        return Vec3i(x + other.x, y + other.y, z + other.z);
    }

    bool operator==(Vec3i const &other)
    {
        return (x == other.x) && (y == other.y) && (z == other.z);
    }

    bool operator!=(Vec3i const &other)
    {
        return (x != other.x) && (y != other.y) && (z != other.z);
    }
};

struct Vec2f
{
    float x, y;

    Vec2f(float _x = 0.f, float _y = 0.f) : x(_x), y(_y) {}

    Vec2f operator+(Vec2f const &other)
    {
        return Vec2f(x + other.x, y + other.y);
    }

    bool operator==(Vec2f const &other)
    {
        return (x == other.x) && (y == other.y);
    }

    bool operator!=(Vec2f const &other)
    {
        return (x != other.x) && (y != other.y);
    }
};

struct Vec3f
{
    float x, y, z;

    Vec3f(float _x = 0.f, float _y = 0.f, float _z = 0.f) : x(_x), y(_y), z(_z) {}

    Vec3f operator+(Vec3f const &other)
    {
        return Vec3f(x + other.x, y + other.y, z + other.z);
    }

    bool operator==(Vec3f const &other)
    {
        return (x == other.x) && (y == other.y) && (z == other.z);
    }

    bool operator!=(Vec3f const &other)
    {
        return (x != other.x) && (y != other.y) && (z == other.z);
    }
};

constexpr int get1DIndex(int i, int j, int num_columns)
{
    return i * num_columns + j;
}

constexpr std::pair<int, int> get2DIndex(int index, int num_columns)
{
    int i = index / num_columns;
    int j = index % num_columns;
    return {i, j};
}