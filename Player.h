#pragma once
#include <CabrankEngine/InputManager.h>
#include "Agent.h"

constexpr auto PLAYER_WIDTH = 15.0f; //Por qué se puso auto?
constexpr auto PLAYER_HEIGHT = 120.0f;

enum Direction { UP, DOWN, NONE };

class Player : public Agent
{
public:
	Player(const glm::vec2& position, int textureId, 
		   int screenWidth, int screenHeight, CabrankEngine::InputManager& inputManager);
	~Player();

	void update(float deltaTime, unsigned int upKey, unsigned int downKey);
	virtual void update(float deltaTime) = 0;
	void draw(CabrankEngine::SpriteBatch& spriteBatch) override;

private:
	bool collideWithTopBottom() override;

	CabrankEngine::InputManager& _inputManager;
	Direction _direction = Direction::NONE;

};

