#include "game.h"

// The primary class should inherit from Game class

//=============================================================================
// Constructor
//=============================================================================
Game::Game() {
	input = new Input();        // initialize keyboard input immediately
	// additional initialization is handled in later call to input->initialize()
	paused = false;             // game is not paused
	graphics = NULL;
	initialized = false;
}

//=============================================================================
// Destructor
//=============================================================================
Game::~Game() {
	deleteAll();                // free all reserved memory
	ShowCursor(true);			// show cursor
}

//=============================================================================
// Window message handler
//=============================================================================
LRESULT Game::messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (initialized) {
		switch (msg) {
		case WM_DESTROY:
			PostQuitMessage(0);					// tell Windows to kill this program
			return 0;
		case WM_KEYDOWN: case WM_SYSKEYDOWN:    // key down
			input->keyDown(wParam);
			return 0;
		case WM_KEYUP: case WM_SYSKEYUP:        // key up
			input->keyUp(wParam);
			return 0;
		case WM_CHAR:                           // character entered
			input->keyIn(wParam);
			return 0;
		case WM_MOUSEMOVE:                      // mouse moved
			input->mouseIn(lParam);
			return 0;
		case WM_INPUT:                          // raw mouse data in
			input->mouseRawIn(lParam);
			return 0;
		case WM_LBUTTONDOWN:                    // left mouse button down
			input->setMouseLButton(true);
			input->mouseIn(lParam);             // mouse position
			return 0;
		case WM_LBUTTONUP:                      // left mouse button up
			input->setMouseLButton(false);
			input->mouseIn(lParam);             // mouse position
			return 0;
		case WM_MBUTTONDOWN:                    // middle mouse button down
			input->setMouseMButton(true);
			input->mouseIn(lParam);             // mouse position
			return 0;
		case WM_MBUTTONUP:                      // middle mouse button up
			input->setMouseMButton(false);
			input->mouseIn(lParam);             // mouse position
			return 0;
		case WM_RBUTTONDOWN:                    // right mouse button down
			input->setMouseRButton(true);
			input->mouseIn(lParam);             // mouse position
			return 0;
		case WM_RBUTTONUP:                      // right mouse button up
			input->setMouseRButton(false);
			input->mouseIn(lParam);             // mouse position
			return 0;
		case WM_XBUTTONDOWN:					// mouse X button down/up
		case WM_XBUTTONUP:
			input->setMouseXButton(wParam);
			input->mouseIn(lParam);             // mouse position
			return 0;
		case WM_DEVICECHANGE:                   // check for controller insert
			input->checkControllers();
			return 0;
		}
	}
	return DefWindowProc(hwnd, msg, wParam, lParam); // let Windows handle it
}

//=============================================================================
// Initializes the game
// throws GameError on error
//=============================================================================
void Game::initialize(HWND hw) {
	hwnd = hw;                                  // save window handle

	// initialize graphics
	graphics = new Graphics();
	// throws GameError
	graphics->initialize(hwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN);

	// initialize input, do not capture mouse
	input->initialize(hwnd, false);             // throws GameError

	// attempt to set up high resolution timer
	if (QueryPerformanceFrequency(&timerFreq) == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing high resolution timer"));

	QueryPerformanceCounter(&timeStart);        // get starting time

	initialized = true;
}

//=============================================================================
// Render game items
//=============================================================================
void Game::renderGame() {
	//start rendering
	if (SUCCEEDED(graphics->beginScene())) {
		// render is a pure virtual function that must be provided in the
		// inheriting class.
		render();               // call render in derived class

		//stop rendering
		graphics->endScene();
	}

	handleLostGraphicsDevice();

	//display the back buffer on the screen
	graphics->showBackbuffer();
}

//=============================================================================
// Handle lost graphics device
//=============================================================================
void Game::handleLostGraphicsDevice() {
	// test for and handle lost device
	hr = graphics->getDeviceState();
	if (FAILED(hr)) {				// if graphics device is not in a valid state
		// if the device is lost and not available for reset
		if (hr == D3DERR_DEVICELOST) {
			Sleep(100);             // yield cpu time (100 mili-seconds)
			return;
		}
		// the device was lost but is now available for reset
		else if (hr == D3DERR_DEVICENOTRESET) {
			releaseAll();
			hr = graphics->reset(); // attempt to reset graphics device
			if (FAILED(hr))         // if reset failed
				return;
			resetAll();
		}
		else
			return;					// other device error
	}
}

//=============================================================================
// Call repeatedly by the main message loop in WinMain
//=============================================================================
void Game::run(HWND hwnd) {
	if (graphics == NULL)            // if graphics not initialized
		return;

	// calculate elapsed time of last frame, save in frameTime
	QueryPerformanceCounter(&timeEnd);
	frameTime = (float)(timeEnd.QuadPart - timeStart.QuadPart) / (float)timerFreq.QuadPart;

	// Power saving code, requires winmm.lib
	// if not enough time has elapsed for desired frame rate
	if (frameTime < MIN_FRAME_TIME) {
		sleepTime = (DWORD)((MIN_FRAME_TIME - frameTime) * 1000);
		timeBeginPeriod(1);         // Request 1mS resolution for windows timer
		Sleep(sleepTime);           // release cpu for sleepTime
		timeEndPeriod(1);           // End 1mS timer resolution
		return;
	}

	if (frameTime > 0.0)
		fps = (fps * 0.99f) + (0.01f / frameTime);  // average fps

	if (frameTime > MAX_FRAME_TIME) // if frame rate is very slow
		frameTime = MAX_FRAME_TIME; // limit maximum frameTime

	timeStart = timeEnd;

	// update(), ai(), and collisions() are pure virtual functions.
	// These functions must be provided in the class that inherits from Game.
	if (!paused) {
		update();                   // update all game items
		ai();                       // artificial intelligence
		collisions();               // handle collisions
		input->vibrateControllers(frameTime); // handle controller vibration
	}

	renderGame();                   // draw all game items
	input->readControllers();       // read state of controllers

	// Clear input
	// Call this after all key checks are done
	input->clear(inputNS::KEYS_PRESSED);
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void Game::releaseAll() {}

//=============================================================================
// Recreate all surfaces and reset all entities.
//=============================================================================
void Game::resetAll() {}

//=============================================================================
// Delete all reserved memory
//=============================================================================
void Game::deleteAll() {
	releaseAll();			// call onLostDevice() for every graphics item
	SAFE_DELETE(graphics);
	SAFE_DELETE(input);
	initialized = false;
}
