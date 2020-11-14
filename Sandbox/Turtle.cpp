#include "Turtle.hpp"

void Turtle::update(double dt){
	life += dt;
	if (cmdBuffer.size()) {
		if (life > 0.8) {
			play(cmdBuffer[0]);
			cmdBuffer.erase(cmdBuffer.begin());
			life = 0;
		}
	}

	angle = (targetAngle - angle) * 0.15 + angle;
	Vector2f position = shape.getPosition();
	position.x = (targetPosition.x - position.x) * 0.2 + position.x;
	position.y = (targetPosition.y - position.y) * 0.2 + position.y;


	_setPosition(position);
	direction.setRotation(angle);
}

void Turtle::reset()
{
	cmdBuffer.clear();
	isDrawing = false;
	targetAngle = angle = 0;
	shape.setPosition(400, 400);
	targetPosition = shape.getPosition();
	direction.setRotation(angle);
	steps.clear();
}

void Turtle::play(Cmd exe) {
	switch(exe)
	{
		case Advance:		advance(100);			break;
		case Backward:		backward(100);			break;
		case TurnLeft45:	turnProgressive(45);		break;
		case TurnRight45:	turnProgressive(-45);		break;
		case DrawOn:		drawOn();				break;
		case DrawOff:		drawOff();				break;
		default:
			break;
	}
}

void Turtle::play(std::vector<Cmd> execute) {
	for (Cmd & cmd : execute) 
		play(cmd);
}

void Turtle::playProgressive(std::vector<Cmd> & execute) {
	cmdBuffer = execute;
}

void Turtle::_setPosition(sf::Vector2f pos) {
	sf::Vector2f opos = pos;
	shape.setPosition(pos);
	pos.x += 32;//on peut ecrire dans pos, il est passé par copie, donc la translation ne remontera pas dans game
	pos.y += 32;
	direction.setPosition(pos);
	if (isDrawing) {
		sf::CircleShape c(8);
		c.setFillColor(sf::Color(0, 255, 255));
		c.setPosition(pos);
		c.setOrigin(4, 4);
		steps.push_back(c);
	}
}
