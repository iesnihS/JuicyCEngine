#pragma once
#include "SFML/System.hpp"

float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

template <typename T>
sf::Vector2<T> lerp(sf::Vector2<T> a, sf::Vector2<T> b, float f)
{
    return{lerp(a.x, b.x,f), lerp(a.y, b.y, f)};
}

float map(float s, float a1, float a2, float b1, float b2)
{
    return b1 + (s - a1) * (b2 - b1) / (a2 - a1);
}

float easeOutCubic(float x)
{
    return 1 - pow(1 - x, 3);
}

float easeInCubic(float x)
{
    return x*x*x;
}