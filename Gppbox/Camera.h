#pragma once

#include "Entity.hpp"
#include "SFML/Window.hpp"

class Camera
{
	private:
		Entity* target;
		sf::Vector2f offset;
	public :
		Camera();
		void SetFollowTarget(Entity* target, sf::Vector2f offset = sf::Vector2f::Vector2(0.f, 0.f));
		void UpdateCamera(sf::RenderWindow *win);
};

