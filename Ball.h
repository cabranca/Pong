#pragma once
#include "Agent.h"
#include "GameManager.h"

constexpr float BALL_WIDTH = 20.0f;
class Player;

class Ball : public Agent
{
public:
	Ball(GameManager& manager, int textureId, int screenWidth, int screenHeight);
	~Ball();

	void update(float deltaTime) override;
	void updateBall(float deltaTime, Player* players[]);
	
	
	void reset();

	void draw(CabrankEngine::SpriteBatch& spriteBatch);

private:
	bool isGoal();
	bool tresspassLeftSide();
	bool tresspassRightSide();
	bool collideWithTopBottom();
	bool collideWithPlayer(Player* player);
	bool inCollisionRange(float yBallPos, float yPlayerPos);
	bool collide(float xBallPos, float xPlayerPos);
	bool collideWithTop(float yPos);
	bool collideWithBottom(float yPos);
	float calculateNewYDirection(Player* player);

	glm::vec2 _direction;
	GameManager& _manager;
	bool _goalIncoming = false; //Es true cuando ya sobrepasó la linea de player pero esta por encima o por debajo
};

