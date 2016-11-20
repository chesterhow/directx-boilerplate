#include "image.h"

//=============================================================================
// Constructor
//=============================================================================
Image::Image() {
	initialized = false;				// set true when successfully initialized
	spriteData.width = 2;
	spriteData.height = 2;
	spriteData.x = 0.0;
	spriteData.y = 0.0;
	spriteData.scale = 1.0;
	spriteData.angle = 0.0;
	spriteData.rect.left = 0;			// used to select one frame from multi-frame image
	spriteData.rect.top = 0;
	spriteData.rect.right = spriteData.width;
	spriteData.rect.bottom = spriteData.height;
	spriteData.texture = NULL;			// the sprite texture (picture)
	spriteData.flipHorizontal = false;
	spriteData.flipVertical = false;
	cols = 1;
	textureManager = NULL;
	startFrame = 0;
	endFrame = 0;
	currentFrame = 0;
	frameDelay = 1.0;					// default to 1 second per frame of animation
	animTimer = 0.0;
	visible = true;						// the image is visible
	loop = true;						// loop frames
	animComplete = false;
	graphics = NULL;					// link to graphics system
	colorFilter = graphicsNS::WHITE;	// WHITE for no change
}

//=============================================================================
// Destructor
//=============================================================================
Image::~Image() {}

bool Image::initialize(Graphics *g, int width, int height, int ncols, TextureManager *textureM) {
	try {
		graphics = g;                               // the graphics object
		textureManager = textureM;                  // pointer to texture object

		spriteData.texture = textureManager->getTexture();
		if (width == 0)
			width = textureManager->getWidth();     // use full width of texture
		spriteData.width = width;
		if (height == 0)
			height = textureManager->getHeight();   // use full height of texture
		spriteData.height = height;
		cols = ncols;
		if (cols == 0)
			cols = 1;                               // if 0 cols use 1

		// configure spriteData.rect to draw currentFrame
		spriteData.rect.left = (currentFrame % cols) * spriteData.width;
		// right edge + 1
		spriteData.rect.right = spriteData.rect.left + spriteData.width;
		spriteData.rect.top = (currentFrame / cols) * spriteData.height;
		// bottom edge + 1
		spriteData.rect.bottom = spriteData.rect.top + spriteData.height;
	}
	catch (...) { return false; }
	initialized = true;
	return true;
}

void Image::draw(COLOR_ARGB color) {
	if (!visible || graphics == NULL)
		return;
	// get fresh texture in case onReset() was called
	spriteData.texture = textureManager->getTexture();
	if (color == graphicsNS::FILTER)                    // if draw with filter
		graphics->drawSprite(spriteData, colorFilter);	// use colorFilter
	else
		graphics->drawSprite(spriteData, color);        // use color as filter
}

void Image::draw(SpriteData sd, COLOR_ARGB color) {
	if (!visible || graphics == NULL)
		return;
	sd.rect = spriteData.rect;						// use this Images rect to select texture
	sd.texture = textureManager->getTexture();		// get fresh texture incase onReset() was called

	if (color == graphicsNS::FILTER)				// if draw with filter
		graphics->drawSprite(sd, colorFilter);		// use colorFilter
	else
		graphics->drawSprite(sd, color);			// use color as filter
}

void Image::update(float frameTime) {
	if (endFrame - startFrame > 0) {				// if animated sprite
		animTimer += frameTime;						// total elapsed time
		if (animTimer > frameDelay) {
			animTimer -= frameDelay;
			currentFrame++;
			if (currentFrame < startFrame || currentFrame > endFrame) {
				if (loop == true)					// if looping animation
					currentFrame = startFrame;
				else {								// not looping animation
					currentFrame = endFrame;
					animComplete = true;
				}
			}
			setRect();								// set spriteData.rect
		}
	}
}

void Image::setCurrentFrame(int c) {
	if (c >= 0) {
		currentFrame = c;
		animComplete = false;
		setRect();										// set spriteData.rect
	}
}

inline void Image::setRect() {
	// configure spriteData.rect to draw currentFrame
	spriteData.rect.left = (currentFrame % cols) * spriteData.width;
	// right edge + 1
	spriteData.rect.right = spriteData.rect.left + spriteData.width;
	spriteData.rect.top = (currentFrame / cols) * spriteData.height;
	// bottom edge + 1
	spriteData.rect.bottom = spriteData.rect.top + spriteData.height;
}
