#include "CPOINT2D.h"

CPOINT2D::CPOINT2D() {
	mX = 0;
	mY = 0;
}

CPOINT2D::CPOINT2D(const CPOINT2D& other) {
	mX = other.mX;
	mY = other.mY;
}

CPOINT2D& CPOINT2D::operator=(const CPOINT2D& other) {
	if (this != &other) {
		mX = other.mX;
		mY = other.mY;
	}
	return *this;
}

void CPOINT2D::setX(int x) {
	mX = x;
}

void CPOINT2D::setY(int y) {
	mY = y;
}

void CPOINT2D::setXY(int x, int y) {
	setX(x);
	setY(y);
}

void CPOINT2D::moveX(int deltaX) {
	mX += deltaX;
}
void CPOINT2D::moveY(int deltaY) {
	mY += deltaY;
}
void CPOINT2D::moveXY(int deltaX, int deltaY) {
	moveX(deltaX);
	moveY(deltaY);
}

int CPOINT2D::getX() const {
	return mX;
}
int CPOINT2D::getY() const {
	return mY;
}
