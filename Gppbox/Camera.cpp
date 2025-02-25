#include "Camera.h"
#include "C.hpp"
#include "Utils.h"
#include <imgui.h>

Camera::Camera(){}

void Camera::SetFollowTarget(Entity* target, sf::Vector2f offset, sf::Vector2f dOffset)
{
	this->target = target;
	this->offset = offset;
	this->dOffset = dOffset;
}

void Camera::UpdateCamera(sf::RenderWindow* win)
{
	if (target == nullptr)
		return;

	auto v = win->getDefaultView();
	

	if(target->dv.x > 0.1 || target->dv.x < -0.1)
	{
		bool sign = target->dv.x > 0;
		tempOffset.x += (sign ? dOffset.x : -dOffset.x) * sCam;
		tempOffset.x = fminf(fmaxf(tempOffset.x, -dOffset.x), dOffset.x);
		float t = map(tempOffset.x, -dOffset.x, dOffset.x, 0, 1);
		x = lerp(-dOffset.x, dOffset.x,t);
	}

	v.setCenter(target->getPosPixelf() + offset + sf::Vector2f{x,0.f});
	win->setView(v);
}

bool Camera::im()
{
	using namespace ImGui;
	
	sf::Vector2f cO = offset + sf::Vector2f{ x,0.f };
	Value("Current Offset", cO.x);
	SameLine();
	Value(",", cO.y);

	bool chg = DragFloat("Speed Camera", &sCam, 0.001f);
	chg | DragFloat2("Displacement Offset", &dOffset.x, 1.f);
	
	return chg;
}