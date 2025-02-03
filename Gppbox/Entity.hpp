#pragma once

#include <SFML/Graphics.hpp>
class Entity
{
public :
	sf::Shape* shape = 0; //nullptr

	//Cell coord
	int cx = 0;
	int cy = 0;

	float rx = 0.5f;
	float ry = 0.5f;

	//Deplacement
	float dx = 0.f; 
	float dy = 0.f;

	float gravity = 0.f;

	//Friction
	float frx = 0.88f;
	float fry = 1.0f;

	bool jumping = false;

	Entity(sf::Shape* shape);
	void update(double deltaTime);
	void setCooPixel(int px, int py);
	void setCooGrid(float coox, float cooy);
	void syncPos();
	void draw(sf::RenderWindow& win);
	void im();

	void setJumping(bool onOff);

	sf::Vector2i getPosPixel;
};

