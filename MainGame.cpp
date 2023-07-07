#include "MainGame.h"
#include "CabrankEngine/CabrankEngine.h"
#include <CabrankEngine/Timing.h>
#include <iostream>
#include "Player1.h"
#include "Player2.h"
#include <CabrankEngine/ResourceManager.h>

MainGame::MainGame() {

}

MainGame::~MainGame() {
	delete[] _players;
	delete _ball;
	_ball = nullptr;
	//_players = nullptr NO ME DEJA
}

void MainGame::run() {
	initSystems();

	_collideEffect = _audioEngine.loadSoundEffect("Audio/hit.ogg");
	//music.play(-1);

	initLevel();
	gameLoop();
}

void MainGame::initSystems() {
	CabrankEngine::init();
	_audioEngine.init();

	_window.create("Pong", _screenWidth, _screenHeight, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 255.0f);
	initShaders();
	_agentBatch.init();

	//Set up camera
	_camera.init(_screenWidth, _screenHeight);
}

void MainGame::initShaders() {
	//Compile our Color shader
	_textureProgram.compileShaders("Shaders/textureShading.vert.txt", "Shaders/textureShading.frag.txt");
	_textureProgram.addAttribute("vertexPosition");
	_textureProgram.addAttribute("vertexColor");
	_textureProgram.addAttribute("vertexUV");
	_textureProgram.linkShaders();
}

void MainGame::initLevel() {
	static int playerTextureId = CabrankEngine::ResourceManager::getTexture("Textures/bar.png").id;

	auto player1StartPos = glm::vec2(-_screenWidth / 2 + 2 * PLAYER_WIDTH, -PLAYER_HEIGHT / 2.0f);
	auto player2StartPos = glm::vec2(_screenWidth / 2 - 3 * PLAYER_WIDTH, -PLAYER_HEIGHT / 2.0f);

	_players[0] = new Player1(player1StartPos, playerTextureId, _screenWidth, _screenHeight, _inputManager);
	_players[1] = new Player2(player2StartPos, playerTextureId, _screenWidth, _screenHeight, _inputManager);

	int ballTextureId = CabrankEngine::ResourceManager::getTexture("Textures/ball.png").id;
	_ball = new Ball(_manager, ballTextureId, _screenWidth, _screenHeight);
}

void MainGame::gameLoop() {
	CabrankEngine::FPSLimiter fpsLimiter;
	fpsLimiter.setMaxFPS(60.0f);

	//const float CAMERA_SCALE = 1.0f / 2.0f;
	//_camera.setScale(CAMERA_SCALE);

	const int MAX_PHYSICS_STEPS = 6;
	const float DESIRED_FPS = 60.0f;
	const float MS_PER_SECOND = 1000;
	const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS;
	const float MAX_DELTA_TIME = 1.0f;
	float previousTicks = SDL_GetTicks();

	int resetCountdown = 20;

	while (_gameState == GameState::PLAY) {
		fpsLimiter.begin();

		float newTicks = SDL_GetTicks();
		float frameTime = newTicks - previousTicks;
		previousTicks = newTicks;
		float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

		_inputManager.update(); //Esto es si quiero usar pressedKey
		processInput();

		int i = 0;
		if (resetCountdown == 0) {
			resetCountdown = 20;
			_ball->reset();
		}

		if (_manager.isGoal() && resetCountdown != 0) {
			resetCountdown--;
			_manager.reset();
		}

		while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS) {
			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
			_players[0]->update(deltaTime);
			_players[1]->update(deltaTime);
			_ball->updateBall(deltaTime, _players);
			totalDeltaTime -= deltaTime;
			i++;
		}

		//Activar cuando tenga un efecto mejor
		/*if (_manager.getCollide()) {
			_collideEffect.play(1);
			_manager.setCollide(false);
		}*/
		

		drawGame();

		_fps = fpsLimiter.end();
		std::cout << _fps << std::endl;
	}
}

void MainGame::processInput() {
	SDL_Event evnt;
	//Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_KEYDOWN:
			_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		}
	}
}

void MainGame::drawGame() {
	//Set the base depth to 1.0
	glClearDepth(1.0);
	//Clear the ColorRGBA8 and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Bind shaders
	_textureProgram.use();

	//Draw code goes here
	glActiveTexture(GL_TEXTURE0);

	//Make sure the shader uses texture 0
	GLint textureUniform = _textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	//Grab the camera matrix
	glm::mat4 projectionMatrix = _camera.getCameraMatrix();
	GLint pUniform = _textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	//draw bars
	_agentBatch.begin();
	_players[0]->draw(_agentBatch);
	_players[1]->draw(_agentBatch);
	_ball->draw(_agentBatch);
	_agentBatch.end();
	_agentBatch.renderBatch();

	//Unbind shaders
	_textureProgram.unuse();

	//Swap our buffer and draw everything to the screen!
	_window.swapBuffer();
}