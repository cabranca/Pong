#include "Agent.h"
#include <CabrankEngine/ResourceManager.h>

Agent::Agent(const glm::vec2& position, int textureId, int screenWidth, int screenHeight) :
	_position(position), _textureId(textureId),
	_screenWidth(screenWidth), _screenHeight(screenHeight)
{}

Agent::~Agent() {}