#include <imgui.h>
#include "Entity.hpp"
#include "C.hpp"
#include "Game.hpp"

Entity::Entity(Shape* shape, EntityType t, float s, float jumpForce) : sptr(shape), eType(t), speed(s), jF(jumpForce)
{
	if (t == EntityType::Player)
		showIg = true;
	currentST = shootRate;
}

void Entity::update(double dt)
{
	double rate = 1.0 / dt;
	double dfr = 60.0 / rate;

	
	dx += dv.x * speed * (dt == 0 ? 0 : 1);

	dy += gravity * dt;

	dx = dx * pow(frx, dfr);
	dy = dy * pow(fry, dfr);

	rx += dx * dt;
	ry += dy * dt;

	ManagePhysic(dt);
	ShootBullet(dt);
	syncPos();

	if (eType != EntityType::Bullet) return;

	lifeTime -= dt;
	if (lifeTime <= 0) isDestroy = true;
}

void Entity::ManagePhysic(double dt)
{
	Game& g = *Game::instance;
	if(eType == EntityType::Player || eType==EntityType::Enemy)
	{
		vector<Entity*> touchedVec = g.hasCollisionEntity(this);
		for(Entity* touched : touchedVec)
		{
			if (touched->eType == EntityType::Bullet && touched->parent != this)
			{
				touched->Destroy();
				Destroy();
			}
			else if (touched->eType == EntityType::Player)
			{
				touched->Reset();
			}	
		}
		
		if (rx + 0.5f > 1.0f)
		{
			bool side1 = g.hasCollision(cx + rx + 0.5f, cy + ry - 0.5f);
			bool side2 = g.hasCollision(cx + rx + 0.5f, cy + ry - 1.5f);
			bool floor1 = g.hasCollision(cx + rx + 0.5f, cy + ry + .5f);

			if (((eType == EntityType::Enemy && floor1) || eType == EntityType::Player)&& !side1 && !side2)
			{
				rx--;
				cx++;
			}
			else {
				rx -= dx * dt;
				dx = 0;
				if (eType == EntityType::Enemy)
					dv.x = -1;
			}

		}
		if (rx - 0.5 < 0) {
			bool side1 = g.hasCollision(cx + rx - 0.5, cy + ry - 0.5f);
			bool side2 = g.hasCollision(cx + rx - 0.5f, cy + ry - 1.5f);
			bool floor1 = g.hasCollision(cx + rx - 0.5f, cy + ry + .5f);
			if (((eType == EntityType::Enemy && floor1) || eType == EntityType::Player) && !side1 && !side2)
			{
				rx++;
				cx--;
			}
			else {
				rx -= dx * dt;
				dx = 0;
				if (eType == EntityType::Enemy)
					dv.x = 1;
			}
		}
		
		


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
				if (g.hasCollision(cx + rx, cy + ry - 2))
				{
					ry -= dy * dt;
					dy = 0;
				}
				while (ry < 0)
				{
					ry++;
					cy--;
				}
			}
		}
		else if (!g.hasCollision(cx + rx + 0.5f, cy + ry + 0.1f) && !g.hasCollision(cx + rx - 0.5f, cy + ry + 0.1f))
			setJumping(true);

	}
	else if(eType == EntityType::Bullet)
	{
		float ratio = (size / (float)C::CELL_SIZE);
		if (rx + ratio > 1.0f)
		{
			if (!g.hasCollision(cx + rx + ratio, cy + ry - 0.5f) && !g.hasCollision(cx + rx + ratio, cy + ry - 1.5f)) {
				rx--;
				cx++;
			}
			else {
				dx = 0;
				Destroy();
			}
		}
		if (rx - ratio < 0)
		{
			if (!g.hasCollision(cx + rx - ratio, cy + ry - 0.5f) && !g.hasCollision(cx + rx - ratio, cy + ry - 1.5f)) {
				rx--;
				cx++;
			}
			else {
				dx = 0;
				Destroy();
			}
		}
	}

}

void Entity::Destroy()
{
	if(eType == EntityType::Enemy)
	{
		CreateExplosion();
		Game::instance->SleepDT(0.02f);
	}
	isDestroy = true;
}

void Entity::Reset()
{
	cx = 3;
	cy = 54;
	rx = 0.5f;
	ry = 0.99f;

	dx = dy = 0;
	setJumping(false);
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
	PushID(this);

	Value("Entity Type", (int) eType);
	if (eType == EntityType::Bullet)
		Value("Life Time", lifeTime);
	if(Button(showIg ? "Hidden" : "Show"))
	{
		showIg = !showIg;
	}
	if (!showIg)
	{
		PopID();
		return chg;
	}
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

	Value("Time before shooting", currentST);

	if (Button("Reset")) {
		Reset();
	}
	PopID();
	return chg || chgCoo;
}

sf::Vector2i Entity::getPosPixel()
{
	int px = (cx + rx) * C::CELL_SIZE;
	int py = (cy + ry) * C::CELL_SIZE;
	return  { px,py };
}

sf::Vector2i Entity::getSPosPixel()
{
	sf::RenderWindow* win = Game::instance->win;
	return getPosPixel() -  Vector2i(win->getView().getCenter() - win->getView().getSize() / 2.f);
}

sf::Vector2f Entity::getPosPixelf()
{
	float px = (cx + rx) * C::CELL_SIZE;
	float py = (cy + ry) * C::CELL_SIZE;
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

void Entity::ShootBullet(double dt)
{
	if (currentST > 0)
	{
		currentST -= dt;
		return;
	}
	if (bBuffer <= 0) return;
	
	Game& g = *Game::instance;

	sf::RectangleShape* sprite = new sf::RectangleShape({6, 6});
	sprite->setFillColor(sf::Color::Magenta);
	sprite->setOrigin({ 6 * 0.5f, 6 * 0.5});
	Entity* e = new Entity(sprite, EntityType::Bullet, 50);
	
	bool mDir =  signbit((getSPosPixel() - g.GetSMousePosition()).x);

	dx += mDir ? -kb : kb;

	e->lifeTime = 3.f;
	e->size = 6;
	e->ry = ry;
	e->cx = cx;
	e->cy = cy - 1;
	e->rx = rx;
	e->frx = 1;
	e->dx = e->speed * (mDir ? 1 : -1);
	e->syncPos();
	e->parent = this;
	g.ents.push_back(e);
	
	if(eType == EntityType::Player)
		g.cam->AddScreenShake();

	bBuffer--;
	currentST = shootRate;
}

void Entity::AddBulletBuffer()
{
	if(bBuffer < maxSizeBB)
		bBuffer++;
}
void Entity::CreateExplosion()
{
	
	Explosion* e = new Explosion(getPosPixelf() + Vector2f{0,(float)-size}, 0.3f, EaseType::OutCubic, rand()%20 +5);
	Game::instance->fVFX.push_back(e);
}

Entity::~Entity()
{
	delete sptr;
}
