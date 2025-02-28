#pragma once
#include "SFML/System.hpp"

inline float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

template <typename T>
inline sf::Vector2<T> lerp(sf::Vector2<T> a, sf::Vector2<T> b, float f)
{
    return{lerp(a.x, b.x,f), lerp(a.y, b.y, f)};
}

inline float map(float s, float a1, float a2, float b1, float b2)
{
    return b1 + (s - a1) * (b2 - b1) / (a2 - a1);
}

inline float easeOutCubic(float x)
{
    return 1 - pow(1 - x, 3);
}

inline float easeInCubic(float x)
{
    return x*x*x;
}

template <typename T>
inline int dotProduct(sf::Vector2<T> vect_A, sf::Vector2<T>  vect_B)
{
    return vect_A.x * vect_B.x + vect_A.y * vect_B.y;
}

template <typename T>
inline float sqrtMagnitude(sf::Vector2<T> vec) {
    return vec.x * vec.x + vec.y * vec.y;
}