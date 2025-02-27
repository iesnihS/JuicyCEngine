#pragma once

#include <SFML/Graphics.hpp>
#include <queue>

enum class EntityType 
{
	None,
	Player,
	Bullet,
	Enemy
};

class Entity
{
	private:
		int bBuffer;
		int maxSizeBB = 1; //bb == Bullet Buffer
		float shootRate = 0.1f; //seconds
		float currentST = 0; //current time shot in seconds
		std::queue<Entity*> bullets;
		bool showIg = false;
		float lifeTime = 3;
		
	public :
		EntityType eType = EntityType::Enemy;
		sf::Shape* sptr = 0; //nullptr

		float kb = 1.6f;
		float speed = 5;
		int size = 5;

		//Cell coord
		int cx = 0;
		int cy = 0;

		//Cellratio
		float rx = 0.5f;
		float ry = 0.0f;

		//Deplacement
		float dx = 0.f; 
		float dy = 0.f;

		float gravity = 0.f;

		//Friction
		float frx = 0.88f;
		float fry = 1.0f;

		sf::Vector2f dv = sf::Vector2f::Vector2(0, 0); //Desire Velocity

		bool jumping = false;
		bool isDestroy = false;

		Entity(sf::Shape* shape, EntityType t, float speed);

		void update(double deltaTime);

		//Convert Pixel coord to Cell coord
		void setCooPixel(int px, int py);
		void setCooGrid(float coox, float cooy);
		void syncPos();
		void draw(sf::RenderWindow& win);
		bool im();

		void setJumping(bool onOff);
		void ManagePhysic(double dt);
		void AddBulletBuffer();

		sf::Vector2i getPosPixel();
		sf::Vector2i getSPosPixel(); //ScreenPosPixel
		sf::Vector2f getPosPixelf();

		void ShootBullet(double dt);
		void Destroy();
};

