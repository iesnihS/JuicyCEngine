#include "C.hpp"
#include "Game.hpp"
#include <imgui.h>
#include "HotReloadShader.hpp"
#include "eastl_config.hpp"

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

void* __cdecl operator new[](size_t size, const char* name, int flags, unsigned debugFlags, const char* file, int line)
{
	return new uint8_t[size];
}

void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line) {
	return _aligned_malloc(size,alignment);
}

static int cols = 1280 / C::GRID_SIZE;
static int lastLine = 720 / C::GRID_SIZE - 1;

Game::Game(sf::RenderWindow * win) {
	this->win = win;
	bg = sf::RectangleShape(Vector2f(win->getSize().x, win->getSize().y));

	bool isOk = tex.loadFromFile("res/bg_stars.png");
	if (!isOk) {
		printf("ERR : LOAD FAILED\n");
	}
	bg.setTexture(&tex);
	bg.setSize(sf::Vector2f(1280, 720));

	bgShader = new HotReloadShader("res/bg.vert", "res/bg.frag");
	
	for (int i = 0; i < 1280 / C::GRID_SIZE; ++i) 
		walls.push_back( Vector2i(i, lastLine) );

	walls.push_back(Vector2i(0, lastLine-1));
	walls.push_back(Vector2i(0, lastLine-2));
	walls.push_back(Vector2i(0, lastLine-3));

	walls.push_back(Vector2i(cols-1, lastLine - 1));
	walls.push_back(Vector2i(cols-1, lastLine - 2));
	walls.push_back(Vector2i(cols-1, lastLine - 3));

	walls.push_back(Vector2i(cols >>2, lastLine - 2));
	walls.push_back(Vector2i(cols >>2, lastLine - 3));
	walls.push_back(Vector2i(cols >>2, lastLine - 4));
	walls.push_back(Vector2i((cols >> 2) + 1, lastLine - 4));
	cacheWalls();
}

void Game::cacheWalls()
{
	wallSprites.clear();
	for (Vector2i & w : walls) {
		sf::RectangleShape rect(Vector2f(16,16));
		rect.setPosition(w.x * C::GRID_SIZE, w.y * C::GRID_SIZE);
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

	float lateralSpeed = 8.0;
	float maxSpeed = 40.0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {

	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {

	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {

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

static bool s_RectVAFlood = false;
static sf::VertexArray va;

static bool s_RectShapeFlood = false;
static eastl::vector<sf::RectangleShape> rects;

void Game::update(double dt) {
	pollInput(dt);

	g_time += dt;
	if (bgShader) bgShader->update(dt);

	beforeParts.update(dt);

	afterParts.update(dt);

	g_tickTimer -= dt;
	if (g_tickTimer <= 0.0) {
		onFileTick();
		g_tickTimer = 0.1;
	}


	if (ImGui::CollapsingHeader("Flood Rect - Shapes Method")) {
		int start;
		ImGui::PushID(&start);
		ImGui::Indent();
		if (!s_RectShapeFlood && ImGui::Button("Start")) {
			s_RectShapeFlood = true;
		}

		if (s_RectShapeFlood) {
			ImGui::LabelText("NbShapes", "%d", rects.size());
			if (dt < (1.0 / 60)) {
				for (int i = 0; i < 1000; ++i) {
					sf::RectangleShape r(Vector2f(13, 13));
					r.rotate(Dice::angleDeg());
					r.setPosition(Dice::randF() * 1280, Dice::randF() * 600);
					r.setFillColor(Lib::makeFromHSV(Dice::angleDeg(), 0.9, 0.9));
					rects.push_back(r);
				}
			}
			if (ImGui::Button("Reset")) {
				rects.clear();
				s_RectShapeFlood = false;
			}
		}
		ImGui::Unindent();
		ImGui::PopID();
	}

	if (ImGui::CollapsingHeader("Flood Rect - Vertex Array Method")) {
		int start;
		ImGui::PushID(&start);
		ImGui::Indent();
		if (!s_RectVAFlood && ImGui::Button("Start")) {
			s_RectVAFlood = true;
		}

		if (s_RectVAFlood) {
			ImGui::LabelText("Nb VA Rects", "%d", rects.size());
			if (dt < (1.0 / 60)) {
				for (int i = 0; i < 1000; ++i) {
					sf::RectangleShape r(Vector2f(13, 13));
					r.rotate(Dice::angleDeg());
					r.setPosition(Dice::randF() * 1280, Dice::randF() * 600);
					r.setFillColor(Lib::makeFromHSV(Dice::angleDeg(), 0.9, 0.9));
					//va.push_back(r);

					va.setPrimitiveType(sf::PrimitiveType::Quads);

					auto trs = r.getTransform();
					sf::Vertex v0 = sf::Vertex(trs * r.getPoint(0), r.getFillColor());
					sf::Vertex v1 = sf::Vertex(trs * r.getPoint(1), r.getFillColor());
					sf::Vertex v2 = sf::Vertex(trs * r.getPoint(2), r.getFillColor());
					sf::Vertex v3 = sf::Vertex(trs * r.getPoint(3), r.getFillColor());
					//va.append( )
				}
			}
			if (ImGui::Button("Reset")) {
				va.clear();
				s_RectVAFlood = false;
			}
		}
		ImGui::Unindent();
		ImGui::PopID();
	}
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

	if(s_RectShapeFlood)
		for (sf::RectangleShape& r : rects) {
			win.draw(r);
		}

	afterParts.draw(win);
}


void Game::onSpacePressed() {
	
}

void Game::onFileTick() {
	
}

bool Game::isWall(int cx, int cy)
{
	for (Vector2i & w : walls) {
		if (w.x == cx && w.y == cy)
			return true;
	}
	return false;
}

void Game::im()
{

}

