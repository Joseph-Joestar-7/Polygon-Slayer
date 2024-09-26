#pragma once

#include "glm/glm.hpp"
#include <SFML/Graphics.hpp>

class CTransform {
public:
	glm::vec2 position = { 0.0f,0.0f };
	glm::vec2 velocity = { 0.0f,0.0f };
	float angle = 0.0f;

	CTransform(const glm::vec2& p,const glm::vec2& v,float a)
		:position(p), velocity(v),angle(a)
	{}
};

class CShape {
public:
	sf::CircleShape circle;

	CShape(float radius, int points, const sf::Color& fill,
		const sf::Color& outline, float thickness)
		: circle(radius, points)
	{
		circle.setFillColor(fill);
		circle.setOutlineColor(outline);
		circle.setOutlineThickness(thickness);
		circle.setOrigin(radius, radius);
	}
};

class CCollision {
public:
	float collisionRadius = 0;

	CCollision(float r)
		:collisionRadius(r)
	{}
};

class CScore {
public:
	int score = 0;

	CScore(int s):score(s){}
};

class CLifespan {
public:
	int remaining = 0;
	int total = 0;

	CLifespan(int total):
		remaining(total),total(total){}
};

class CInput {
public:
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool shoot = false;

	CInput()
	{}
};