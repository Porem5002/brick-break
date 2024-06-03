#pragma once

#include <array>
#include <cmath>

#include <SDL2/SDL.h>

struct Vector2
{
    float x;
    float y;

    Vector2(float x, float y) : x(x), y(y) { }

    float sqr_magnitude() const
    {
        return x * x + y * y;
    }

    float magnitude() const
    {
        return std::sqrt(sqr_magnitude());
    }

    Vector2 normalized() const
    {
        float mag = magnitude();

        if(mag == 0) return Vector2(0, 0);

        return *this / mag;
    }

    float angle() const
    {
        return std::atan2(y, x);
    }

    Vector2 operator +(Vector2 other) const
    {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 operator -(Vector2 other) const
    {
        return Vector2(x - other.x, y - other.y);
    }

    Vector2 operator *(float scalar) const
    {
        return Vector2(x * scalar, y * scalar);
    }

    Vector2 operator /(float scalar) const
    {
        return Vector2(x / scalar, y / scalar);
    }

    void operator +=(Vector2 other)
    {
        this->x += other.x;
        this->y += other.y;
    }

    void operator -=(Vector2 other)
    {
        this->x -= other.x;
        this->y -= other.y;
    }

    void operator *=(float scalar)
    {
        this->x *= scalar;
        this->y *= scalar;
    }

    void operator /=(float scalar)
    {
        this->x /= scalar;
        this->y /= scalar;
    }
};

struct Rectangle
{
    Vector2 position;
    float width;
    float height;

    Rectangle(float x, float y, float width, float height)
        : position(Vector2(x, y)), width(width), height(height) { }

    Rectangle(Vector2 center, float width, float height)
        : position(center), width(width), height(height) { }

    bool colliding_with(Rectangle other) const
    {
        Vector2 diff = position - other.position;
        bool cond_x = std::abs(diff.x) < (width + other.width)/2;
        bool cond_y = std::abs(diff.y) < (height + other.height)/2;
        return cond_x && cond_y;
    }

    explicit operator SDL_FRect() const
    {
        float x = position.x - width / 2;
        float y = position.y - height / 2;
        SDL_FRect rect = { x, y, width, height };
        return rect;
    }
};