#include <SDL/SDL.h>
#include "Player.h"

Player::Player(const glm::vec2& position, int textureId, 
	           int screenWidth, int screenHeight, CabrankEngine::InputManager& inputManager) :
			   Agent(position, textureId, screenWidth, screenHeight), _inputManager(inputManager){
	_speed = 4.5f;
}

Player::~Player() {}

void Player::update(float deltaTime, unsigned int upKey, unsigned int downKey) {
	//Seteo la direccion antes de fijarme si hay colision
	if (_inputManager.isKeyDown(upKey))
		_direction = UP;
	else if (_inputManager.isKeyDown(downKey))
		_direction = DOWN;
	else
		_direction = NONE;

	//Si hay colision, no hago update
	if (collideWithTopBottom())
		return;

	//Me muevo hacia donde diga la direccion, pero no puedo pasarme de los limites
	switch (_direction) {
	case UP:
		if (_position.y + PLAYER_HEIGHT >= _screenHeight / 2.0)
			_position.y = (float)_screenHeight / 2.0f - PLAYER_HEIGHT;
		else
			_position.y += _speed * deltaTime;
		break;
	case DOWN:
		if (_position.y <= -_screenHeight / 2.0)
			_position.y = (float) - _screenHeight / 2.0;
		else
			_position.y -= _speed * deltaTime;
		break;
	}
}

bool Player::collideWithTopBottom() {
	//Dependiendo de la direccion me fijo si choco contra algun borde
	switch (_direction) {
	case UP:
		return _position.y + PLAYER_HEIGHT / 2 == _screenHeight;
	case DOWN:
		return _position.y - PLAYER_HEIGHT / 2 == 0;
	case NONE:
		return false;
	}
}

void Player::draw(CabrankEngine::SpriteBatch& spriteBatch) {
	//static int textureID = CabrankEngine::ResourceManager::getTexture("Textures/ball.png").id;
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	glm::vec4 destRect;
	destRect.x = _position.x;
	destRect.y = _position.y;
	destRect.z = PLAYER_WIDTH;
	destRect.w = PLAYER_HEIGHT;


	spriteBatch.draw(destRect, uvRect, _textureId, 0.0f, _color);
}