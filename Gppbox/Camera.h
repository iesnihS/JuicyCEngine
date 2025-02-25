#pragma once

#include "Entity.hpp"
#include "SFML/Window.hpp"

class Camera
{
	private:
		Entity* target = nullptr;
		sf::Vector2f offset;
		sf::Vector2f dOffset; //displacement Offset
		sf::Vector2f tempOffset = sf::Vector2f::Vector2(0.f, 0.f); //tempory Offset
		float x = 0; //x displacement current Offset
	public :
		float sCam = 0.02f; //Speed Cam;
		Camera();
		void SetFollowTarget(Entity* target, sf::Vector2f offset = sf::Vector2f::Vector2(0.f, 0.f), sf::Vector2f dOffset = sf::Vector2f::Vector2(0.f, 0.f));
		void UpdateCamera(sf::RenderWindow *win);
		bool im();
};

