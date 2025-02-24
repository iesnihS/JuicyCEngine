#include "Camera.h"
#include "C.hpp"

Camera::Camera(){}

void Camera::SetFollowTarget(Entity* target, sf::Vector2f offset)
{
	this->target = target;
	this->offset = offset;
}

void Camera::UpdateCamera(sf::RenderWindow* win)
{
	auto v = win->getDefaultView();
	v.setCenter(target->getPosPixelf() + offset);
	win->setView(v);
}