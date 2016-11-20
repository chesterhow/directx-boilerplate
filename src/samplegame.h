#ifndef _SAMPLEGAME_H
#define _SAMPLEGAME_H
#define _WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "image.h"

class SampleGame : public Game {
private:
	// Game items
	TextureManager backgroundTexture;
	TextureManager shipTexture;
	Image		   background;
	Image		   ship;

public:
	// Constructor
	SampleGame();

	// Destructor
	virtual ~SampleGame();

	// Initialize the game
	void initialize(HWND hwnd);
	void update();      // must override pure virtual from Game
	void ai();          // "
	void collisions();  // "
	void render();      // "
	void releaseAll();
	void resetAll();
};

#endif
