#ifndef _CONSTANTS_H
#define _CONSTANTS_H
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

//-----------------------------------------------
// Useful macros
//-----------------------------------------------
// Safely delete pointer referenced item
#define SAFE_DELETE(ptr) { if (ptr) { delete (ptr); (ptr)=NULL; } }
// Safely release pointer referenced item
#define SAFE_RELEASE(ptr) { if (ptr) { (ptr)->Release(); (ptr)=NULL; } }
// Safely delete pointer referenced array
#define SAFE_DELETE_ARRAY(ptr) { if (ptr) { delete [](ptr); (ptr)=NULL; } }
// Safely call onLostDevice
#define SAFE_ON_LOST_DEVICE(ptr) { if (ptr) { (ptr)->onLostDevice(); } }
// Safely call onResetDevice
#define SAFE_ON_RESET_DEVICE(ptr) { if (ptr) { (ptr)->onResetDevice(); } }
// Color which should be transparent
#define TRANSCOLOR SETCOLOR_ARGB(0,255,0,255)

//-----------------------------------------------
// Constants
//-----------------------------------------------
// Window
const char CLASS_NAME[] = "Boilerplate";
const char GAME_TITLE[] = "Boilerplate";
const bool FULLSCREEN = false;
const UINT GAME_WIDTH = 960;
const UINT GAME_HEIGHT = 600;

// Game
const double PI = 3.14159265;
const float FRAME_RATE = 200.0f;
const float MIN_FRAME_RATE = 10.0f;
const float MIN_FRAME_TIME = 1.0f / FRAME_RATE;
const float MAX_FRAME_TIME = 1.0f / MIN_FRAME_RATE;

// Key Mappings
const UCHAR ESC_KEY = VK_ESCAPE;
const UCHAR ALT_KEY = VK_MENU;
const UCHAR ENTER_KEY = VK_RETURN;
const UCHAR LEFT_KEY = VK_LEFT;
const UCHAR RIGHT_KEY = VK_RIGHT;
const UCHAR UP_KEY = VK_UP;
const UCHAR DOWN_KEY = VK_DOWN;

// Sprites
const char BACKGROUND_IMAGE[] = "sprites\\background.png";
const char SHIP_IMAGE[] = "sprites\\ship.png";

const float BACKGROUND_SCALE = 0.5f;
const int SHIP_START_FRAME = 0;					// starting frame of ship animation
const int SHIP_END_FRAME = 3;					// last frame of ship animation
const float SHIP_ANIMATION_DELAY = 0.2f;		// time between frames of ship animation
const int SHIP_HEIGHT = 32;						// height of ship image
const int SHIP_WIDTH = 32;						// width of ship image
const int SHIP_COLS = 2;						// ship texture has 2 columns
const float ROTATION_RATE = 180.0f;				// degrees per second
const float SCALE_RATE = 0.2f;					// % change per second
const int SHIP_SPEED = 100.0f;					// pixels per seconds
const float SHIP_SCALE = 1.5f;					// starting ship scale

#endif
