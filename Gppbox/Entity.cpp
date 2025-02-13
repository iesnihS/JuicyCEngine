#include <imgui.h>
#include "Entity.hpp"
#include "C.hpp"
#include "Game.hpp"

Entity::Entity(Shape* shape) : sptr(shape)
{
	
}

void Entity::update(double dt)
{
	double rate = 1.0 / dt;
	double dfr = 60.0 / rate;

	dy += gravity * dt;

	dx = dx * pow(frx, dfr);
	dy = dy * pow(fry, dfr);

	rx += dx * dt;
	ry += dy * dt;

	Game* g = Game::instance;

	if(rx > 1.0f)
	{
		
	}
}

void Entity::setCooGrid(float coox, float cooy)
{
}

void Entity::syncPos()
{
}
