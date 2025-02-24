#pragma once

#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

#include "sys.hpp"

#include "Particle.hpp"
#include "ParticleMan.hpp"
#include "Camera.h"


using namespace sf;
class Entity;
class HotReloadShader;
class Game {
private :
	void DrawGrid(bool canDraw);
	void DrawBuildIndicator(bool canDraw);
	sf::Vector2i GetMousePosition();
	Camera cam;
public:
	
	sf::RenderWindow*				win = nullptr;

	sf::RectangleShape				bg;
	HotReloadShader *				bgShader = nullptr;

	sf::Texture						tex;

	bool canDrawGrid = true;
	bool canBuild = false;
	float dtModifier = 1;
	bool							closing = false;
	
	std::vector<sf::Vector2i>		walls;
	std::vector<sf::RectangleShape> wallSprites;

	std::vector<Entity*> ents;

	ParticleMan beforeParts;
	ParticleMan afterParts;

	static Game* instance;

	Game(sf::RenderWindow * win);

	void initMainChar();

	void cacheWalls();

	void processInput(sf::Event ev);
	bool wasPressed = false;
	void pollInput(double dt);
	void onSpacePressed();
	
	bool hasCollision(float gridx, float gridy);

	void update(double dt);

	void draw(sf::RenderWindow& win);

	bool isWall(int cx, int cy);
	void im();

	void UpdateBuild();
	void DrawDebug();
};