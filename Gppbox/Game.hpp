#pragma once

#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

#include "sys.hpp"

#include "Particle.hpp"
#include "ParticleMan.hpp"
#include "Camera.h"
#include "Explosion.h"


using namespace sf;
class Entity;
class HotReloadShader;

enum class BuildMode
{
	None,
	Block,
	Ennemy
};

class Game {
private :
	void DrawGrid(bool canDraw);
	void DrawBuildIndicator(bool canDraw);
	BuildMode cbm = BuildMode::Block;
	float cSleep = 0; //current Sleep
	
public:
	Camera* cam;
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
	std::vector<Explosion*> fVFX; //Front

	ParticleMan beforeParts;
	ParticleMan afterParts;

	static Game* instance;

	Game(sf::RenderWindow * win);
	~Game();

	void initMainChar(int cx, int cy, float rx, float ry);
	void initEnnemy(int cx, int cy);

	void cacheWalls();

	void processInput(sf::Event ev);
	bool wasPressed = false;
	void pollInput(double dt);
	void onSpacePressed();
	
	bool hasCollision(float gridx, float gridy);
	vector<Entity*> hasCollisionEntity(Entity* current);
	void update(double dt);
	
	void draw(sf::RenderWindow& win);

	bool isWall(int cx, int cy);
	void im();

	void SleepDT(double sleep);
	void UpdateBuild();
	void DrawDebug();
	void LevelToFile(ofstream& of, bool pl = false); //save player
	void FileToLevel(ifstream& ifs, bool pl = false); //load player
	void ClearEnts(bool pl = false);//clear player
	sf::Vector2i GetWMousePosition(); //World
	sf::Vector2i GetSMousePosition(); //Screen
};