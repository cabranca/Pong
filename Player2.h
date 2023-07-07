#pragma once
#include "Player.h"

class Player2 : public Player
{
public:
	Player2(const glm::vec2& position, int textureId,
		int screenWidth, int screenHeight, CabrankEngine::InputManager& inputManager);

	~Player2();

	void update(float deltaTime) override; //Para corregir los fps
};
