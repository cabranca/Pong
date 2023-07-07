#pragma once
#include "Player.h"

class Player1 : public Player
{
public:
	Player1(const glm::vec2& position, int textureId,
		int screenWidth, int screenHeight, CabrankEngine::InputManager& inputManager);
	
	~Player1();

	void update(float deltaTime) override; //Para corregir los fps
};

