#pragma once

#include "Tween.h"
#include <SFML/Graphics.hpp>
#include "Entity.hpp"
enum class VFXType
{
	Explosion,
	Muzzle
};
class VFX
{
public:
	VFXType vType = VFXType::Explosion;
	bool isDestroy = false;
	Entity* parent;
	sf::Vector2f offSet;
	float mSize;
	Tween<float> tween;
	Tween<float> t1;
	Tween<float> t2;
	Tween<float> t3;
	Tween<float> t4;
	sf::CircleShape* sptr;
	VFX(sf::Vector2f pos, sf::Vector2f off, float duration, EaseType ease, float size, VFXType type, Entity* parent = nullptr);
	void Update(double dt);
	void Draw(sf::RenderWindow* win);
	~VFX();
};

