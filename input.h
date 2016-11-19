#ifndef _INPUT_H
#define _INPUT_H
#define WIN32_LEAN_AND_MEAN

class Input;

#include <Windows.h>
#include <windowsx.h>
#include <string>
#include <XInput.h>
#include "constants.h"
#include "gameError.h"

// High-definition mouse
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC      ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE     ((USHORT) 0x02)
#endif

namespace inputNS {
	// size of key arrays
	const int KEYS_ARRAY_LEN = 256;

	// what values for clear(), bit flag
	const UCHAR KEYS_DOWN = 1;
	const UCHAR KEYS_PRESSED = 2;
	const UCHAR MOUSE = 4;
	const UCHAR TEXT_IN = 8;
	const UCHAR KEYS_MOUSE_TEXT = KEYS_DOWN + KEYS_PRESSED + MOUSE + TEXT_IN;
}

const DWORD GAMEPAD_THUMBSTICK_DEADZONE = (DWORD)(0.20f * 0X7FFF);  // default to 20% of range as deadzone
const DWORD GAMEPAD_TRIGGER_DEADZONE = 30;							// trigger range 0-255
const DWORD MAX_CONTROLLERS = 4;									// Maximum number of controllers supported by XInput

// Bit corresponding to gamepad button in state.Gamepad.wButtons
const DWORD GAMEPAD_DPAD_UP = 0x0001;
const DWORD GAMEPAD_DPAD_DOWN = 0x0002;
const DWORD GAMEPAD_DPAD_LEFT = 0x0004;
const DWORD GAMEPAD_DPAD_RIGHT = 0x0008;
const DWORD GAMEPAD_START_BUTTON = 0x0010;
const DWORD GAMEPAD_BACK_BUTTON = 0x0020;
const DWORD GAMEPAD_LEFT_THUMB = 0x0040;
const DWORD GAMEPAD_RIGHT_THUMB = 0x0080;
const DWORD GAMEPAD_LEFT_SHOULDER = 0x0100;
const DWORD GAMEPAD_RIGHT_SHOULDER = 0x0200;
const DWORD GAMEPAD_A = 0x1000;
const DWORD GAMEPAD_B = 0x2000;
const DWORD GAMEPAD_X = 0x4000;
const DWORD GAMEPAD_Y = 0x8000;

struct ControllerState {
	XINPUT_STATE        state;
	XINPUT_VIBRATION    vibration;
	float               vibrateTimeLeft;    // mSec
	float               vibrateTimeRight;   // mSec
	bool                connected;
};

class Input {
private:
	bool keysDown[inputNS::KEYS_ARRAY_LEN];			// true if specified key is down
	bool keysPressed[inputNS::KEYS_ARRAY_LEN];		// true if specified key was pressed
	std::string textIn;								// user entered text
	char charIn;									// last character entered
	bool newLine;									// true on start of new line
	int  mouseX, mouseY;							// mouse screen coordinates
	int  mouseRawX, mouseRawY;						// high-definition mouse data
	RAWINPUTDEVICE Rid[1];							// for high-definition mouse
	bool mouseCaptured;								// true if mouse captured
	bool mouseLButton;								// true if left mouse button down
	bool mouseMButton;								// true if middle mouse button down
	bool mouseRButton;								// true if right mouse button down
	bool mouseX1Button;								// true if X1 mouse button down
	bool mouseX2Button;								// true if X2 mouse button down
	ControllerState controllers[MAX_CONTROLLERS];   // state of controllers

public:
	// Constructor
	Input();

	// Destructor
	virtual ~Input();

	// Initialize mouse and controller input.
	// Throws GameError
	// Pre: hwnd = window handle
	//      capture = true to capture mouse.
	void initialize(HWND hwnd, bool capture);

	// Save key down state
	void keyDown(WPARAM);

	// Save key up state
	void keyUp(WPARAM);

	// Save the char just entered in textIn string
	void keyIn(WPARAM);

	// Returns true if the specified VIRTUAL KEY is down, otherwise false.
	bool isKeyDown(UCHAR vkey) const;

	// Return true if the specified VIRTUAL KEY has been pressed in the most recent frame.
	// Key presses are erased at the end of each frame.
	bool wasKeyPressed(UCHAR vkey) const;

	// Return true if any key was pressed in the most recent frame.
	// Key presses are erased at the end of each frame.
	bool anyKeyPressed() const;

	// Clear the specified key press
	void clearKeyPress(UCHAR vkey);

	// Clear specified input buffers where what is any combination of
	// KEYS_DOWN, KEYS_PRESSED, MOUSE, TEXT_IN or KEYS_MOUSE_TEXT.
	// Use OR '|' operator to combine parmeters.
	void clear(UCHAR what);

	// Clears key, mouse and text input data
	void clearAll() { clear(inputNS::KEYS_MOUSE_TEXT); }

	// Clear text input buffer
	void clearTextIn() { textIn.clear(); }

	// Return text input as a string
	std::string getTextIn() { return textIn; }

	// Return last character entered
	char getCharIn() { return charIn; }

	// Reads mouse screen position into mouseX, mouseY
	void mouseIn(LPARAM);

	// Reads raw mouse data into mouseRawX, mouseRawY
	// This routine is compatible with a high-definition mouse
	void mouseRawIn(LPARAM);

	// Save state of mouse button
	void setMouseLButton(bool b) { mouseLButton = b; }

	// Save state of mouse button
	void setMouseMButton(bool b) { mouseMButton = b; }

	// Save state of mouse button
	void setMouseRButton(bool b) { mouseRButton = b; }

	// Save state of mouse button
	void setMouseXButton(WPARAM wParam) {
		mouseX1Button = (wParam & MK_XBUTTON1) ? true : false;
		mouseX2Button = (wParam & MK_XBUTTON2) ? true : false;
	}

	// Return mouse X position
	int  getMouseX() const { return mouseX; }

	// Return mouse Y position
	int  getMouseY() const { return mouseY; }

	// Return raw mouse X movement. Left is <0, Right is >0
	// Compatible with high-definition mouse.
	int  getMouseRawX() const { return mouseRawX; }

	// Return raw mouse Y movement. Up is <0, Down is >0
	// Compatible with high-definition mouse.
	int  getMouseRawY() const { return mouseRawY; }

	// Return state of left mouse button.
	bool getMouseLButton() const { return mouseLButton; }

	// Return state of middle mouse button.
	bool getMouseMButton() const { return mouseMButton; }

	// Return state of right mouse button.
	bool getMouseRButton() const { return mouseRButton; }

	// Return state of X1 mouse button.
	bool getMouseX1Button() const { return mouseX1Button; }

	// Return state of X2 mouse button.
	bool getMouseX2Button() const { return mouseX2Button; }

	// Update connection status of game controllers.
	void checkControllers();

	// Save input from connected game controllers.
	void readControllers();

	// Return state of specified game controller.
	const ControllerState* getControllerState(UINT n) {
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return &controllers[n];
	}

	// Return state of controller n buttons.
	const WORD getGamepadButtons(UINT n) {
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return controllers[n].state.Gamepad.wButtons;
	}

	// Return state of controller n D-pad Up
	bool getGamepadDPadUp(UINT n) {
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_UP) != 0);
	}

	// Return state of controller n D-pad Down.
	bool getGamepadDPadDown(UINT n) {
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_DOWN) != 0);
	}

	// Return state of controller n D-pad Left.
	bool getGamepadDPadLeft(UINT n) {
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_LEFT) != 0);
	}

	// Return state of controller n D-pad Right.
	bool getGamepadDPadRight(UINT n) {
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_RIGHT) != 0);
	}

	// Return state of controller n Start button.
	bool getGamepadStart(UINT n) {
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_START_BUTTON) != 0);
	}

	// Return state of controller n Back button.
	bool getGamepadBack(UINT n) {
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_BACK_BUTTON) != 0);
	}

	// Return state of controller n Left Thumb button.
	bool getGamepadLeftThumb(UINT n) {
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_LEFT_THUMB) != 0);
	}

	// Return state of controller n Right Thumb button.
	bool getGamepadRightThumb(UINT n) {
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_RIGHT_THUMB) != 0);
	}

	// Return state of controller n Left Shoulder button.
	bool getGamepadLeftShoulder(UINT n) {
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_LEFT_SHOULDER) != 0);
	}

	// Return state of controller n Right Shoulder button.
	bool getGamepadRightShoulder(UINT n) {
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_RIGHT_SHOULDER) != 0);
	}

	// Return state of controller n A button.
	bool getGamepadA(UINT n) {
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_A) != 0);
	}

	// Return state of controller n B button.
	bool getGamepadB(UINT n) {
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_B) != 0);
	}

	// Return state of controller n X button.
	bool getGamepadX(UINT n) {
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_X) != 0);
	}

	// Return state of controller n Y button.
	bool getGamepadY(UINT n) {
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return bool((controllers[n].state.Gamepad.wButtons&GAMEPAD_Y) != 0);
	}

	// Return value of controller n Left Trigger.
	BYTE getGamepadLeftTrigger(UINT n) {
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return (controllers[n].state.Gamepad.bLeftTrigger);
	}

	// Return value of controller n Right Trigger.
	BYTE getGamepadRightTrigger(UINT n) {
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return (controllers[n].state.Gamepad.bRightTrigger);
	}

	// Return value of controller n Left Thumbstick X.
	SHORT getGamepadThumbLX(UINT n) {
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return (controllers[n].state.Gamepad.sThumbLX);
	}

	// Return value of controller n Left Thumbstick Y.
	SHORT getGamepadThumbLY(UINT n) {
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return (controllers[n].state.Gamepad.sThumbLY);
	}

	// Return value of controller n Right Thumbstick X.
	SHORT getGamepadThumbRX(UINT n) {
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return (controllers[n].state.Gamepad.sThumbRX);
	}

	// Return value of controller n Right Thumbstick Y.
	SHORT getGamepadThumbRY(UINT n) {
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return (controllers[n].state.Gamepad.sThumbRY);
	}

	// Vibrate controller n left motor.
	// Left is low frequency vibration.
	// speed 0=off, 65536=100 percent
	// sec is time to vibrate in seconds
	void gamePadVibrateLeft(UINT n, WORD speed, float sec) {
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		controllers[n].vibration.wLeftMotorSpeed = speed;
		controllers[n].vibrateTimeLeft = sec;
	}

	// Vibrate controller n right motor.
	// Right is high frequency vibration.
	// speed 0=off, 65536=100 percent
	// sec is time to vibrate in seconds
	void gamePadVibrateRight(UINT n, WORD speed, float sec) {
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		controllers[n].vibration.wRightMotorSpeed = speed;
		controllers[n].vibrateTimeRight = sec;
	}

	// Vibrates the connected controllers for the desired time.
	void vibrateControllers(float frameTime);
};

#endif