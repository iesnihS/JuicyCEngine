#pragma once

#include "Tween.h"
#include <SFML/Graphics.hpp>
class Explosion
{
public:
	bool isDestroy = false;
	float mSize;
	Tween<float> tween;
	sf::CircleShape* sptr;
	Explosion(sf::Vector2f pos, float duration, EaseType ease, float size);
	void Update(double dt);
	void Draw(sf::RenderWindow* win);
	~Explosion();
};

