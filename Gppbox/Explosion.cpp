#include "Explosion.h"

Explosion::Explosion(sf::Vector2f pos, float duration, EaseType ease, float size) : mSize(size)
{
	sptr = new sf::CircleShape(0.f);
	sptr->setPosition(pos);
	sptr->setFillColor(sf::Color::White);
	tween = Tween<float>().From(0).To(size).For(duration).Ease(ease).OnCompleted([this]()
		{
			isDestroy = true;
		});


}

void Explosion::Update(double dt)
{
	
	float size = tween.Update(dt);
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