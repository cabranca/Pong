#include <SDL/SDL.h>
#include <random>
#include <glm/gtx/rotate_vector.hpp>
#include <CabrankEngine/MathHelpers.h>
#include "Ball.h"
#include <Pong/Player.h>

Ball::Ball(GameManager& manager, int textureId, int screenWidth, int screenHeight) :
	Agent(glm::vec2(0.0f, 0.0f), textureId, screenWidth, screenHeight),
	_direction(glm::vec2(0.0f, 0.0f)), _manager(manager) {
	reset();
}

Ball::~Ball(){}

void Ball::reset() {
	//Vuelvo a posicion y velocidad original
	_position = glm::vec2(0.0f, 0.0f);
	_speed = 5.0f;
	_goalIncoming = false;

	//genero randomizer
	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

	//get random direction
	_direction = glm::vec2(glm::sign(randDir(randomEngine)), randDir(randomEngine));
}

void Ball::updateBall(float deltaTime, Player* players[]) {
	if (isGoal())
		_direction.x > 0.0f ? _manager.p1Scores() : _manager.p2Scores();

	//Antes chequeo si esta fuera de rango para pegar con el borde superior o inferior
	for (int i = 0; i < 2; i++) {
		if (collideWithPlayer(players[i])) {
			if (_goalIncoming)
				_direction.y = -_direction.y;
			else {
				_direction.x = -_direction.x;
				_direction.y = calculateNewYDirection(players[i]);
				_direction = glm::normalize(_direction);
				_manager.setCollide(true);
			}

			_speed += 1.0f;
		}
	}
	
	if (collideWithTopBottom())
		_direction.y = -_direction.y;

	update(deltaTime);
}

bool Ball::isGoal() {
	//Chequeo si fue gol
	auto ballCenterPos = glm::vec2(_position.x + BALL_WIDTH / 2.0f, _position.y + BALL_WIDTH / 2);
	if (_direction.x < 0.0f)
		return tresspassLeftSide();
	else
		return tresspassRightSide();
}

bool Ball::tresspassLeftSide() {
	return _position.x <= -_screenWidth / 2.0f;
}

bool Ball::tresspassRightSide() {
	return _position.x + BALL_WIDTH >= _screenWidth / 2.0f;
}

bool Ball::collideWithPlayer(Player* player) {
	//Descarto que sea imposible la colision
	if (player->getPosition().x > _position.x && _direction.x < 0.0f)
		return false;
	if (player->getPosition().x < _position.x && _direction.x > 0.0f)
		return false;

	auto ballCenterPos = glm::vec2(_position.x + BALL_WIDTH / 2.0f, _position.y + BALL_WIDTH / 2.0f);
	auto playerPos = player->getPosition();
	auto playerCenterPos = glm::vec2(playerPos.x + PLAYER_WIDTH / 2.0f, playerPos.y + PLAYER_HEIGHT / 2.0f);

	//La distancia entre centros tiene que ser menor a la suma de los anchos divido 2
	//SOLO PARA GEOMETRIA RECTANGULAR
	if (!collide(ballCenterPos.x, playerCenterPos.x))
		return false;
	
	//Veo si estoy en rango de colision
	if (!inCollisionRange(ballCenterPos.y, playerCenterPos.y)) {
		_goalIncoming = true;
		return false;
	}
	return true;
}

bool Ball::inCollisionRange(float yBallPos, float yPlayerPos) {
	return abs(yBallPos - yPlayerPos) <= (PLAYER_HEIGHT + BALL_WIDTH) / 2.0f;
}

bool Ball::collide(float xBallPos, float xPlayerPos) {
	return abs(xBallPos - xPlayerPos) <= (PLAYER_WIDTH + BALL_WIDTH) / 2.0f;
}

bool Ball::collideWithTopBottom() {
	//Chequeo rebote con paredes
	if (_direction.y < 0.0f)
		return collideWithBottom(_position.y);
	else
		return collideWithTop(_position.y + BALL_WIDTH);
}

bool Ball::collideWithBottom(float yPos) {
	return yPos <= -_screenHeight / 2.0f;
}

bool Ball::collideWithTop(float yPos) {
	return yPos >= _screenHeight / 2.0f;
}

float Ball::calculateNewYDirection(Player* player) {
	auto ballCenterPos = glm::vec2(_position.x + BALL_WIDTH / 2.0f, _position.y + BALL_WIDTH / 2.0f);
	auto playerPos = player->getPosition();
	auto playerCenterPos = glm::vec2(playerPos.x + PLAYER_WIDTH / 2.0f, playerPos.y + PLAYER_HEIGHT / 2.0f);

	float delta = ballCenterPos.y - playerCenterPos.y;
	return 2.0f * delta / (BALL_WIDTH + PLAYER_HEIGHT);
}

void Ball::update(float deltaTime) {
	_position += _direction * _speed * deltaTime;
}

void Ball::draw(CabrankEngine::SpriteBatch& spriteBatch) {
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	glm::vec4 destRect;
	destRect.x = _position.x;
	destRect.y = _position.y;
	destRect.z = BALL_WIDTH;
	destRect.w = BALL_WIDTH;


	spriteBatch.draw(destRect, uvRect, _textureId, 0.0f, _color);
}