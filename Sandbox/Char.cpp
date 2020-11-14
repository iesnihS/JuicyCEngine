#include "Char.hpp"
#include "Game.hpp"

void Char::setCellPosition(int cx, int cy){
	rx = 0.5;
	ry = 0.99;
	this->cx = cx;
	this->cy = cy;
}

void Char::setPosition(int rpx, int rpy) {
	cx = rpx / GRID_SIZE;
	rx = rpx - (cx * GRID_SIZE);

	cy = rpy / GRID_SIZE;
	ry = rpy - (cy * GRID_SIZE);
}

void Char::update(double dt) {

	if (state == Jumping) {
		speedY += gravityY;
	}

	if (speedY > maxFallSpeedY) {
		//speedY = maxFallSpeedY;
	}

	rx += speedX * dt;
	ry += speedY * dt;

	while (rx > 1) {
		if (isWallHit(cx + 1, cy)) {
			rx = 0.95;
		}
		else {
			rx--;
			cx++;
		}
	}

	while (rx < 0) {
		if (isWallHit(cx - 1, cy)) {
			rx = 0.05;
		}
		else {
			rx++;
			cx--;
		}
	}

	if (state == Jumping) {
		if (speedY > 0)
		while (ry < 0) {
			ry++;
			cy--;
		}

		if(speedY > 0)
		while (  ry > 0.99) {
			if (isWallHit(cx, cy+1)) {
				ry = 0.99;
				speedY = 0.0;
				speedX *= 0.5;
				state = Running;
				break;
			}
			else {
				ry--;
				cy++;
			}
		}
	}

	if (state == Running) {
		if (!isWallHit(cx, cy + 1)) {
			state = Jumping;
		}
	}

	speedX *= 0.87;

	spr.setPosition(getPositionPixel());
}

bool Char::isWallHit(int cx, int cy)
{
	return game->isWall(cx,cy);
}
