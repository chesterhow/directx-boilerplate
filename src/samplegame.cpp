#include "samplegame.h"
#include <string>

//=============================================================================
// Constructor
//=============================================================================
SampleGame::SampleGame() {}

//=============================================================================
// Destructor
//=============================================================================
SampleGame::~SampleGame() {
	releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void SampleGame::initialize(HWND hwnd) {
	Game::initialize(hwnd); // throws GameError

	// background texture
	if (!backgroundTexture.initialize(graphics, BACKGROUND_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing background texture"));

	// ship texture
	if (!shipTexture.initialize(graphics, SHIP_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship texture"));

	// background
	if (!background.initialize(graphics, 0, 0, 0, &backgroundTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing background"));

	// ship
	if (!ship.initialize(graphics, SHIP_WIDTH, SHIP_HEIGHT, SHIP_COLS, &shipTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship"));

	background.setScale(BACKGROUND_SCALE);

	ship.setX(GAME_WIDTH / 2);
	ship.setY(GAME_HEIGHT / 2);
	ship.setFrames(SHIP_START_FRAME, SHIP_END_FRAME);	// animation frames
	ship.setCurrentFrame(SHIP_START_FRAME);				// starting frame
	ship.setFrameDelay(SHIP_ANIMATION_DELAY);
	ship.setDegrees(45.0f);								// angle of ship

	return;
}

//=============================================================================
// Update all game items
//=============================================================================
void SampleGame::update() {
	if (input->isKeyDown(RIGHT_KEY)) {
		ship.setX(ship.getX() + frameTime * SHIP_SPEED);
		if (ship.getX() > GAME_WIDTH)						// if off screen right
			ship.setX((float)-ship.getWidth());				// position off screen left
	}
	if (input->isKeyDown(LEFT_KEY)) {
		ship.setX(ship.getX() - frameTime * SHIP_SPEED);
		if (ship.getX() < -ship.getWidth())					// if off screen left
			ship.setX((float)GAME_WIDTH);					// position off screen right
	}
	if (input->isKeyDown(UP_KEY)) {
		ship.setY(ship.getY() - frameTime * SHIP_SPEED);
		if (ship.getY() < -ship.getHeight())				// if off screen top
			ship.setY((float)GAME_HEIGHT);					// position off screen bottom
	}
	if (input->isKeyDown(DOWN_KEY)) {
		ship.setY(ship.getY() + frameTime * SHIP_SPEED);
		if (ship.getY() > GAME_HEIGHT)						// if off screen bottom
			ship.setY((float)-ship.getHeight());			// position off screen
	}

	ship.update(frameTime);									// animate ship
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void SampleGame::ai() {}

//=============================================================================
// Handle collisions
//=============================================================================
void SampleGame::collisions() {}

//=============================================================================
// Render game items
//=============================================================================
void SampleGame::render() {
	graphics->spriteBegin();                // begin drawing sprites

	background.draw();                      // add the background to the scene
	ship.draw();							// add the ship to the scene

	graphics->spriteEnd();                  // end drawing sprites
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void SampleGame::releaseAll() {
	backgroundTexture.onLostDevice();
	shipTexture.onLostDevice();

	Game::releaseAll();
	return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void SampleGame::resetAll() {
	backgroundTexture.onResetDevice();
	shipTexture.onLostDevice();

	Game::resetAll();
	return;
}