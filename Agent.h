#pragma once
#include <CabrankEngine/SpriteBatch.h>

class Agent
{
public:
	Agent(const glm::vec2& position, int textureId, int screenWidth, int screenHeight);
	~Agent();

	virtual void update(float deltaTime) = 0; //Para corregir los fps
	virtual void draw(CabrankEngine::SpriteBatch& spriteBatch) = 0;

	glm::ivec2 getPosition() const { return _position; }

protected:
	virtual bool collideWithTopBottom() = 0;

	glm::vec2 _position;
	float _speed = 0.0f;
	CabrankEngine::ColorRGBA8 _color = CabrankEngine::ColorRGBA8(255, 255, 255, 255);
	int _textureId;
	int _screenWidth; //Un poco raro guardar estos datos, mejor pasarlos en update
	int _screenHeight;
};

