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

	Game& g = *Game::instance;
	if (rx+0.5f > 1.0f) 
	{
		if (!g.hasCollision(cx + rx + 0.5f, cy + ry-0.5f) && !g.hasCollision(cx + rx + 0.5f, cy + ry - 1.5f)) {
			rx--;
			cx++;
		}
		else {
			rx -= dx * dt;
			dx = 0;
		}

	}
	if (rx -0.5 < 0) {
		if (!g.hasCollision(cx + rx - 0.5, cy + ry - 0.5f) && !g.hasCollision(cx + rx - 0.5f, cy + ry - 1.5f)) {
			rx++;
			cx--;
		}
		else {
			rx -= dx * dt;
			dx = 0;
		}
	}
	
	printf("Collision au dessus : %s\n", g.hasCollision(cx + rx, cy + ry - 2) ? "true" : "false");

	if (jumping) {
		if ((dy > 0)) {
			if (g.hasCollision(cx + rx + 0.5f, cy + ry) || g.hasCollision(cx + rx - 0.5f, cy + ry)) {
				setJumping(false);
				ry = 0.99f;
				dy = 0;
			}
			else {
				if (ry > 1) {
					ry--;
					cy++;
				}
			}
		}

		if (dy < 0) {
			while (ry < 0) {
				ry++;
				cy--;
			}
		}
	}
	else if(!g.hasCollision(cx + rx + 0.5f, cy + ry + 0.1f) && !g.hasCollision(cx + rx - 0.5f, cy + ry + 0.1f))
		setJumping(true);

	syncPos();
}

void Entity::setCooGrid(float coox, float cooy)
{
	cx = (int)coox;
	rx = coox - cx;

	cy = (int)cooy;
	ry = cooy - cy;
}
void Entity::setCooPixel(int px, int py)
{
	cx = px / C::CELL_SIZE;
	cy = py / C::CELL_SIZE;

	rx = (px - (cx * C::CELL_SIZE)) / (float)C::CELL_SIZE;
	ry = (py - (cy * C::CELL_SIZE)) / (float)C::CELL_SIZE;

	syncPos();
}

void Entity::syncPos()
{
	sf::Vector2f pos = { (cx + rx) * C::CELL_SIZE,(cy + ry) * C::CELL_SIZE };
	sptr->setPosition(pos);
}

void Entity::draw(sf::RenderWindow& win)
{
	if (sptr)
		win.draw(*sptr);
}

bool Entity::im()
{
	using namespace ImGui;

	bool chg = false;

	Value("jumping", jumping);
	Value("cx", cx);
	Value("cy", cy);

	Value("rx", rx);
	Value("ry", ry);

	sf::Vector2i pix = getPosPixel();
	chg |= DragInt2("pix x/pix y", &pix.x, 1.0f, -2000, 2000);
	if (chg)
		setCooPixel(pix.x, pix.y);

	chg |= DragInt2("cx/cy", &cx, 1.0f, -2000, 2000);

	sf::Vector2f coo = { cx + rx, cy + ry };
	bool chgCoo = DragFloat2("coo grid x/y", &coo.x, 1.0f, -2000, 2000);
	if (chgCoo)
		setCooGrid(coo.x, coo.y);

	chg |= DragFloat2("dx/dy", &dx, 0.01f, -20, 20);
	chg |= DragFloat2("frx/fry", &frx, 0.001f, 0, 1);
	chg |= DragFloat("gravity", &gravity, 0.001f, -2, 2);

	if (Button("Reset")) {
		cx = 3;
		cy = 54;
		rx = 0.5f;
		ry = 0.99f;

		dx = dy = 0;
		setJumping(false);
	}
	return chg || chgCoo;
}

sf::Vector2i Entity::getPosPixel()
{
	int px = (cx + rx) * C::CELL_SIZE;
	int py = (cy + ry) * C::CELL_SIZE;
	return  { px,py };
}

void Entity::setJumping(bool onOff) {
	if (jumping && onOff)
		return;

	if (onOff) {
		gravity = 80;
		jumping = true;
	}
	else {
		gravity = 0;
		jumping = false;
	}
}
