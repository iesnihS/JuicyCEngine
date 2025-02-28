#include "Explosion.h"

Explosion::Explosion(sf::Vector2f pos, float duration, EaseType ease, float size) : mSize(size)
{
	sptr = new sf::CircleShape(0.f);
	sptr->setPosition(pos);
	sptr->setFillColor(sf::Color::Red);
	tween = Tween<float>().From(0).To(size).For(duration).Ease(ease).OnCompleted([this]()
		{
			isDestroy = true;
		});

	gt = Tween<float>().From(0).To(255).For(duration).Ease(ease);
	at = Tween<float>().From(255).To(0).For(duration).Ease(EaseType::InCubic);
}

void Explosion::Update(double dt)
{
	
	float size = tween.Update(dt);
	float g = gt.Update(dt);


	sptr->setFillColor(sf::Color(255, g, g, at.Update(dt)));
	sptr->setOrigin(size, size);
	sptr->setRadius(size);
}

void Explosion::Draw(sf::RenderWindow* win)
{
	if (sptr)
		win->draw(*sptr);
}

Explosion::~Explosion()
{
	delete sptr;
}