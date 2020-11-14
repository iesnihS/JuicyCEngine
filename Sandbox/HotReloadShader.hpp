#pragma once

#include "SFML/Graphics.hpp"
#include <string>
#include <functional>


using namespace std;
class HotReloadShader {
public:
	string vertPath;
	string fragPath;

	string vertSrc;
	string fragSrc;

	time_t mtimeVert = 0;
	time_t mtimeFrag = 0;
	bool	inError = false;

	std::function<void(void)> onUpdate;

	HotReloadShader( string vertPath, string fragPath) {
		this->vertPath = vertPath;
		this->fragPath = fragPath;
	}

	float	time = 0.0;
	string	getFileContent(const std::string & path);
	void	update(double dt);

	sf::Shader sh;
};