#include "Entity.hpp"
#include "C.hpp"

Entity::Entity()
{
	
}

void Entity::Init()
{
	sf::CircleShape circle = sf::CircleShape(50);
	circle.setFillColor(sf::Color(100, 250, 250));
	this->sprite = &circle;
}

void Entity::Draw(sf::RenderWindow* win)
{
	sf::Shape* sprite = this->sprite;

	win->draw(*sprite);
}