#pragma once
#include <CabrankEngine/AudioEngine.h>
#include <CabrankEngine/Window.h>
#include <CabrankEngine/SpriteBatch.h>
#include <CabrankEngine/InputManager.h>
#include <CabrankEngine/GLSLProgram.h>
#include <CabrankEngine/Camera2D.h>

#include "GameManager.h"
#include "Player.h"
#include "Ball.h"

enum GameState { PLAY, EXIT };

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();
	void initSystems();
	void initShaders();
	void initLevel();
	void gameLoop();
	void processInput();
	void drawGame();

private:
	GameState _gameState;
	CabrankEngine::GLSLProgram _textureProgram; //The shader program
	CabrankEngine::AudioEngine _audioEngine;
	CabrankEngine::Window _window;
	CabrankEngine::SpriteBatch _agentBatch;
	CabrankEngine::InputManager _inputManager;
	CabrankEngine::Camera2D _camera;
	CabrankEngine::SoundEffect _collideEffect;

	GameManager _manager;
	Player* _players[2];
	Ball* _ball;

	int _screenWidth = 1024;
	int _screenHeight = 768;
	float _fps;
};

