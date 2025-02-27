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

void Camera::UpdateCamera(double dt,sf::RenderWindow* win)
{
	if (target == nullptr)
		return;

	auto v = win->getDefaultView();
	
	if (cxTween != nullptr)
		scOffset.x = cxTween->Update(dt);
	if (cyTween != nullptr)
		scOffset.y = cyTween->Update(dt);

	if(target->dv.x > 0.1 || target->dv.x < -0.1)
	{
		bool sign = target->dv.x > 0;
		tempOffset.x += (sign ? dOffset.x : -dOffset.x) * sCam;
		tempOffset.x = fminf(fmaxf(tempOffset.x, -dOffset.x), dOffset.x);
		float t = map(tempOffset.x, -dOffset.x, dOffset.x, 0, 1);
		x = lerp(-dOffset.x, dOffset.x,t);
	}

	v.setCenter(target->getPosPixelf() + offset + sf::Vector2f{x,0.f} + scOffset);
	win->setView(v);
}

bool Camera::im()
{
	using namespace ImGui;
	
	sf::Vector2f cO = offset + sf::Vector2f{ x,0.f };
	Value("Current Offset", cO.x);
	SameLine();
	Value(",", cO.y);
	if(Button("Screen Shake"))
	{
		AddScreenShake();
	}

	bool chg = DragFloat("Speed Camera", &sCam, 0.001f);
	chg | DragFloat2("Displacement Offset", &dOffset.x, 1.f);
	chg | DragInt("Screen Shake Power", &scPower, 1.f);
	
	return chg;
}

void Camera::AddScreenShake()
{
	int rx = rand() % (scPower * 2) - scPower;
	if(cxTween ==nullptr)
	{
		cxTween = std::make_unique<Tween<float>>();
		cxTween->From(0).To(rx).For(0.2f).Ease(EaseType::InOutBounce).OnCompleted([this, rx]()
			{
				cxTween->Reset().From(rx).To(0).For(0.05f).Ease(EaseType::InOutBounce);
			});
	}else
	{
		cxTween->Reset().From(0).To(rx).For(0.2f).Ease(EaseType::InOutBounce).OnCompleted([this, rx]()
			{
				cxTween->Reset().From(rx).To(0).For(0.05f).Ease(EaseType::InOutBounce);
			});
	}
	
	int ry = rand() % (scPower * 2) - scPower;
	if (cyTween == nullptr)
	{
		cyTween = std::make_unique<Tween<float>>();
		cyTween->From(0).To(rx).For(0.5f).Ease(EaseType::InOutBounce).OnCompleted([this, rx]()
			{
				cyTween->Reset().From(rx).To(0).For(0.2f).Ease(EaseType::InOutBounce);
			});
	}
	else
	{
		cyTween->Reset().From(0).To(rx).For(0.5f).Ease(EaseType::InOutBounce).OnCompleted([this, rx]()
			{
				cyTween->Reset().From(rx).To(0).For(0.2f).Ease(EaseType::InOutBounce);
			});
	}
}
