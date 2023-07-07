#include "Player1.h"
#include <SDL/SDL_keycode.h>

Player1::Player1(const glm::vec2& position, int textureId,
	int screenWidth, int screenHeight, CabrankEngine::InputManager& inputManager) : 
	Player(position, textureId, screenWidth, screenHeight, inputManager) {
}

Player1::~Player1() {
}

void Player1::update(float deltaTime) {
	Player::update(deltaTime, SDLK_w, SDLK_s);
}