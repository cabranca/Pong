#include "Player2.h"
#include <SDL/SDL_keycode.h>

Player2::Player2(const glm::vec2& position, int textureId,
	int screenWidth, int screenHeight, CabrankEngine::InputManager& inputManager) :
	Player(position, textureId, screenWidth, screenHeight, inputManager) {
}

Player2::~Player2() {
}

void Player2::update(float deltaTime) {
	Player::update(deltaTime, SDLK_UP, SDLK_DOWN);
}