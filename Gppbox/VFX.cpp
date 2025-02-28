#include "VFX.h"
#include "Game.hpp"
VFX::VFX(sf::Vector2f pos, sf::Vector2f off, float duration, EaseType ease, float size, VFXType type, Entity* p) : mSize(size), vType(type), parent(p)
{
	offSet = off;
	sptr = new sf::CircleShape(0.f);
	sptr->setPosition(pos + off);
	tween = Tween<float>().From(0).To(size).For(duration).Ease(ease).OnCompleted([this]()
		{
			isDestroy = true;
		});
	if(vType==VFXType::Explosion)
	{
		sptr->setFillColor(sf::Color::Red);
		t2 = Tween<float>().From(0).To(255).For(duration).Ease(ease);
		t3 = Tween<float>().From(255).To(0).For(duration).Ease(EaseType::InCubic);
	}
	else if(vType == VFXType::Muzzle)
	{
		sptr->setFillColor(sf::Color(0xbd3939));
		t2 = Tween<float>().From(0).To(220).For(duration).Ease(EaseType::InCubic);
		t3 = Tween<float>().From(255).To(0).For(duration).Ease(EaseType::InCubic);
		t4 = Tween<float>().From(12).To(20).For(duration/2.f).Ease(EaseType::InCubic).OnCompleted([this, duration]()
			{
				t4.Reset().From(20).To(12).For(duration / 2.5f).Ease(EaseType::InCubic);
			});
	}
	
}

void VFX::Update(double dt)
{
	
	float size = tween.Update(dt);
	float g = t2.Update(dt);

	if(parent != nullptr)
	{
		sptr->setPosition(parent->getPosPixelf() + offSet);
	}

	if(vType == VFXType::Muzzle)
	{
		*Game::instance->bloomWidth = t4.Update(dt);
	}

	sptr->setFillColor(sf::Color(255, g, g, t3.Update(dt)));
	sptr->setOrigin(size, size);
	sptr->setRadius(size);
}

void VFX::Draw(sf::RenderWindow* win)
{
	if (sptr)
		win->draw(*sptr);
}

VFX::~VFX()
{
	delete sptr;
}