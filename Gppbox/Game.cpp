
#include <imgui.h>
#include <array>
#include <vector>
#include "Entity.hpp"
#include "C.hpp"
#include "Game.hpp"
#include "Tween.h"
#include "HotReloadShader.hpp"


static int cols = C::RES_X / C::CELL_SIZE;
static int lastLine = C::RES_Y / C::CELL_SIZE - 1;
Game* Game::instance = 0;

Game::Game(sf::RenderWindow * win) {

	instance = this;
	this->win = win;

	cam = new Camera();

	bg = sf::RectangleShape(Vector2f((float)win->getSize().x, (float)win->getSize().y));

	bool isOk = tex.loadFromFile("res/bg_stars.png");
	if (!isOk) {
		printf("ERR : LOAD FAILED\n");
	}
	bg.setTexture(&tex);
	bg.setSize(sf::Vector2f(C::RES_X, C::RES_Y));

	bgShader = new HotReloadShader("res/bg.vert", "res/bg.frag");
	
	for (int i = 0; i < C::RES_X / C::CELL_SIZE; ++i)
		walls.push_back( Vector2i(i, lastLine) );

	walls.push_back(Vector2i(cols >> 2, lastLine-2));
	walls.push_back(Vector2i((cols >> 2) - 1, lastLine - 2));

	
	cacheWalls();
	initMainChar();

	cam->SetFollowTarget(ents[0], { 0, -300.f }, {250.f, 0.f });

}

void Game::initMainChar()
{
	sf::RectangleShape* sprite = new sf::RectangleShape({ C::CELL_SIZE, C::CELL_SIZE * 2 });
	sprite->setFillColor(sf::Color::Cyan);
	sprite->setOutlineColor(sf::Color::Red);
	sprite->setOutlineThickness(2);
	sprite->setOrigin({ C::CELL_SIZE * 0.5f, C::CELL_SIZE * 2 });
	Entity* e = new Entity(sprite, EntityType::Player, 5, 40);

	e->size = C::CELL_SIZE;
	e->ry = 0.99f;
	e->cx = 3;
	e->cy = 54;
	e->rx = 0.5f;
	e->syncPos();
	

	ents.push_back(e);
}

void Game::initEnnemy(int cx, int cy)
{
	sf::RectangleShape* sprite = new sf::RectangleShape({ C::CELL_SIZE, C::CELL_SIZE * 2 });
	sprite->setFillColor(sf::Color::Red);
	sprite->setOrigin({ C::CELL_SIZE * 0.5f, C::CELL_SIZE * 2 });
	Entity* e = new Entity(sprite, EntityType::Enemy, 2, 30);

	e->size = C::CELL_SIZE;
	e->ry = 0.99f;
	e->cx = cx;
	e->cy = cy;
	e->rx = 0.5f;
	e->dv = { 1,0 };
	e->syncPos();


	ents.push_back(e);
}

void Game::cacheWalls()
{
	wallSprites.clear();
	for (Vector2i & w : walls) {
		sf::RectangleShape rect(Vector2f(C::CELL_SIZE, C::CELL_SIZE));
		rect.setPosition((float)w.x * C::CELL_SIZE, (float)w.y * C::CELL_SIZE);
		rect.setFillColor(sf::Color(0x07ff07ff));
		wallSprites.push_back(rect);
	}
}

void Game::processInput(sf::Event ev) {
	if (ev.type == sf::Event::Closed) {
		win->close();
		closing = true;
		return;
	}
	if (ev.type == sf::Event::KeyReleased) {
		
	
	}
}


static double g_time = 0.0;
static double g_tickTimer = 0.0;


void Game::pollInput(double dt) {

	auto& io = ImGui::GetIO();
	if (dt == 0 || io.WantCaptureMouse || io.WantCaptureKeyboard || canBuild)
	{
		if (ents.size()) {
			auto mainChar = ents[0];
			if (mainChar)
				mainChar->dv.x = 0;
		}
		return;
	}

	if (ents.size()) {
		auto mainChar = ents[0];
		if (mainChar) {
			mainChar->dv = { 0,0 };
		}
	}

	float lateralSpeed = 8.0;
	float maxSpeed = 40.0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
		if (ents.size()) {
			auto mainChar = ents[0];
			if (mainChar) 
				mainChar->dv.x =-1;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		if (ents.size()) {
			auto mainChar = ents[0];
			if (mainChar) 
				mainChar->dv.x = 1;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
		if (ents.size()) {
			auto mainChar = ents[0];
			if (mainChar && !mainChar->jumping) {
				mainChar->dy -= mainChar->jF;
				mainChar->setJumping(true);
			}
		}
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		if (ents.size()) {
			auto mainChar = ents[0];
			if (mainChar) {
				mainChar->AddBulletBuffer();
			}
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T)) {

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
		if (!wasPressed) {
			onSpacePressed();
			wasPressed = true;
		}
	}
	else {
		wasPressed = false;
	}

}

static sf::VertexArray va;
static RenderStates vaRs;
static std::vector<sf::RectangleShape> rects;

int blendModeIndex(sf::BlendMode bm) {
	if (bm == sf::BlendAlpha) return 0;
	if (bm == sf::BlendAdd) return 1;
	if (bm == sf::BlendNone) return 2;
	if (bm == sf::BlendMultiply) return 3;
	return 4;
};

void Game::update(double dt) {
	dt *= dtModifier;
	
	pollInput(dt);

	g_time += dt;

	for(int  i = ents.size()-1; i >= 0; i--)
	{
		Entity* e = ents[i];
		e->update(dt);

		if (!e->isDestroy) continue;
		ents.erase(ents.begin() + i);
		delete e;
	}

	for (int i = fVFX.size() - 1; i >= 0; i--)
	{
		Explosion* e = fVFX[i];
		e->Update(dt);

		if (!e->isDestroy) continue;
		fVFX.erase(fVFX.begin() + i);
		delete e;
	}

	if (bgShader) bgShader->update(dt);

	beforeParts.update(dt);
	afterParts.update(dt);

	cam->UpdateCamera(dt, win);
}

 void Game::draw(sf::RenderWindow & win) {
	if (closing) return;

	sf::RenderStates states = sf::RenderStates::Default;
	sf::Shader * sh = &bgShader->sh;
	states.blendMode = sf::BlendAdd;
	states.shader = sh;
	states.texture = &tex;
	sh->setUniform("texture", tex);
	//sh->setUniform("time", g_time);
	win.draw(bg, states);

	beforeParts.draw(win);

	for (sf::RectangleShape & r : wallSprites)
		win.draw(r);

	/*for (sf::RectangleShape& r : rects) 
		win.draw(r);*/

	for(auto& e : ents)
	{
		e->draw(win);
	}
	for (auto& vfx : fVFX)
	{
		vfx->Draw(&win);
	}

	afterParts.draw(win);
}

void Game::onSpacePressed() {
	
}
bool Game::hasCollision(float gridx, float gridy)
{
	for (auto& w : walls)
		if ((w.x == floor(gridx)) && (w.y == floor(gridy)))
			return true;

	return false;
}

vector<Entity*> Game::hasCollisionEntity(Entity*current)
{
	vector<Entity*> touched;
	for(auto& e : ents)
	{
		if (e != current && e->sptr->getGlobalBounds().intersects(current->sptr->getGlobalBounds()))
			touched.push_back(e);
	}
	return touched;
}

void Game::DrawGrid(bool canDraw)
{
	if (!canDraw) return;

	std::vector<sf::Vertex> lines;
	Color color = Color(255, 255, 255, 255/3);
	for(uint32_t i = 0; i < cols; i++)
	{
		lines.push_back({ sf::Vector2f(i*C::CELL_SIZE,0),color });
		lines.push_back({ sf::Vector2f(i * C::CELL_SIZE,	C::RES_Y),color });
	}
	for (uint32_t i = 0; i < lastLine + 2; i++)
	{
		lines.push_back({ sf::Vector2f(0,i * C::CELL_SIZE),color });
		lines.push_back({ sf::Vector2f(C::RES_X,	i * C::CELL_SIZE),color });
	}
	win->draw(lines.data(), lines.size(), sf::PrimitiveType::Lines);
}
void Game::DrawBuildIndicator(bool canDraw)
{
	if (!canDraw) return;

	sf::Vector2i pos = GetWMousePosition();
	sf::RectangleShape rect;
	if(cbm == BuildMode::Ennemy)
	{
		rect = sf::RectangleShape(Vector2f(C::CELL_SIZE, C::CELL_SIZE * 2));
		rect.setOrigin({ 0, C::CELL_SIZE });
	}
	else
		rect = sf::RectangleShape(Vector2f(C::CELL_SIZE, C::CELL_SIZE));
	
	rect.setPosition(floor(pos.x/ (float)C::CELL_SIZE)*C::CELL_SIZE, floor(pos.y / (float)C::CELL_SIZE) * C::CELL_SIZE);
	rect.setFillColor(Color(255, 255, 255, 255 / 3));
	win->draw(rect);
}
sf::Vector2i Game::GetWMousePosition()
{
	return sf::Mouse::getPosition(*win) + Vector2i(win->getView().getCenter() - win->getView().getSize() / 2.f);
}
sf::Vector2i Game::GetSMousePosition()
{
	return sf::Mouse::getPosition(*win);
}

bool Game::isWall(int cx, int cy)
{
	for (Vector2i & w : walls) {
		if (w.x == cx && w.y == cy)
			return true;
	}
	return false;
}

void Game::DrawDebug()
{
	DrawGrid(canDrawGrid);
	DrawBuildIndicator(canBuild);
}

void Game::UpdateBuild()
{
	auto& io = ImGui::GetIO();
	if (!canBuild || io.WantCaptureMouse || io.WantCaptureKeyboard)
		return;
	else if(Mouse::isButtonPressed(Mouse::Left) && cbm == BuildMode::Block)
	{
		sf::Vector2i pos = GetWMousePosition();
		auto newPos = Vector2i(floor(pos.x / 16.f), floor(pos.y / 16.f));
		for(uint32_t i = 0; i < walls.size(); i++ )
		{
			if (walls[i].x == newPos.x && walls[i].y == newPos.y)
				return;
		}
		walls.push_back(newPos);
		cacheWalls();
	}
	else if (Mouse::isButtonPressed(Mouse::Left) && cbm == BuildMode::Ennemy)
	{
		sf::Vector2i pos = GetWMousePosition();
		auto newPos = Vector2i(floor(pos.x / 16.f), floor(pos.y / 16.f));
		for (uint32_t i = 0; i < ents.size(); i++)
		{
			if (ents[i]->cx == newPos.x && ents[i]->cy == newPos.y)
				return;
		}
		initEnnemy(newPos.x, newPos.y);
	}
	else if (Mouse::isButtonPressed(Mouse::Right))
	{
		sf::Vector2i pos = GetWMousePosition();
		auto newPos = Vector2i(floor(pos.x / 16.f), floor(pos.y / 16.f));
		if(cbm == BuildMode::Block)
		{
			for (uint32_t i = 0; i < walls.size(); i++)
			{
				if (walls[i].x == newPos.x && walls[i].y == newPos.y)
				{
					walls.erase(walls.begin() + i);
					break;
				}
			}
			cacheWalls();
		}else if(cbm == BuildMode::Ennemy)
		{
			for (uint32_t i = 0; i < ents.size(); i++)
			{
				Entity* e = ents[i];
				if (e->eType == EntityType::Enemy && e->cx == newPos.x && (e->cy == newPos.y || e->cy == newPos.y + 1))
				{
					
					ents.erase(ents.begin() + i);
					delete e;
				}
			}
		}
	}
}

void Game::im()
{
	using namespace ImGui;


	if (ImGui::CollapsingHeader("Game", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
	{

		if (TreeNodeEx("General", 0)) {
			Checkbox("Can Draw Grid", &canDrawGrid);
			DragFloat("Delta Time Modifier", &dtModifier, 0.1f, 0.f, 5.f);
			if (Button("Stop Time"))
				dtModifier = 0;
			ImGui::SameLine();
			if (Button("Start Time"))
				dtModifier = 1.f;
			TreePop();
		}
		if (TreeNodeEx("Walls", 0)) {
			for (auto& w : walls) {
				Value("x", w.x);
				Value("y", w.y);
			}
			TreePop();

		}
		if (TreeNodeEx("Entities")) {
			for (auto e : ents)
			{
				e->im();
			}
			TreePop();
		}
		if (TreeNodeEx("Camera", 0)) {
			cam->im();
			TreePop();
		}
		
	}
	if (ImGui::CollapsingHeader("Level Design"))
	{
		if(Checkbox("Can Build", &canBuild))
		{
			dtModifier = canBuild ? 0 : 1;
		}

		int i= (int)cbm;
		const char* entityTypeNames[] = { "None", "Block", "Ennemy"};
		if(ImGui::Combo("Build Mode", &i, entityTypeNames, 3))
		{
			cbm = (BuildMode) i;
		}
	}
}
Game::~Game()
{
	delete cam;
}

