#ifndef EXTRAS_H
#define EXTRAS_H

/* Plugins and extra features outside the normal scope of
 * the PhobGCC can be found here. Uncomment each #define
 * to include the features they provide in a build then
 * set the configuration slot it uses (if it needs configuration)
 *
 */
//------------------------------------------------------
//#define EXTRAS_ESS
//------------------------------------------------------

#ifdef EXTRAS_ESS
#include "ess.h"
#endif

typedef bool(*ExtrasToggleFn)(IntOrFloat config[]);//Used for toggling extras
typedef void(*ExtrasConfigFn)(IntOrFloat config[], Cardinals dpad);//Used for configuring extras with the Dpad

struct ExtrasFunctions{
	ExtrasToggleFn toggleFn;
	ExtrasConfigFn configFn;
};

#include "structsAndEnums.h"


static ExtrasFunctions extrasFunctions[EXTRAS_SIZE] = {NULL};

void extrasConfigAssign(ExtrasSlot slot, ExtrasToggleFn toggleFn, ExtrasConfigFn configFn){
	switch(slot){
		case EXTRAS_UP:
			debug_println("Extra: Setting configuration to Up...");
			break;
		case EXTRAS_DOWN:
			debug_println("Extra: Setting configuration to Down...");
			break;
		case EXTRAS_LEFT:
			debug_println("Extra: Setting configuration to Left...");
			break;
		case EXTRAS_RIGHT:
			debug_println("Extra: Setting configuration to Right...");
			break;
		case EXTRAS_UNSET:
			debug_println("Extra: Configuration slot not set, feature will be inaccessible.");
			return;
		default:
			debug_println("Extra: Invalid configuration slot requested, feature will be inaccessible.");
			return;
	}
	ExtrasFunctions &fns = extrasFunctions[slot];
	if(fns.toggleFn || fns.configFn) {
		debug_println("Extra: Warning! Configuration slot was already in use, previous feature will be inaccessible.");
	}
	fns.toggleFn = toggleFn;
	fns.configFn = configFn;
}

void extrasInit() {
	/*----------------------------------------------------------
	 * Configuration slots for extras, the available options are:
	 *	- EXTRAS_UP
	 *	- EXTRAS_DOWN
	 *	- EXTRAS_LEFT
	 *	- EXTRAS_RIGHT
	 * Make sure to set the slot from EXTRAS_UNSET to one of the
	 * above for the extras below if applicable!
	 */
#ifdef EXTRAS_ESS
	//-----------------------------------------
	ess::extrasEssConfigSlot = EXTRAS_UNSET;
	//-----------------------------------------
	debug_println("Extra: Enabling ESS functionality...");
	extrasConfigAssign(ess::extrasEssConfigSlot, ess::toggle, NULL);
#endif

}


int extraHelperSide(int x, int y, int x1,int y1, int x2, int y2) {
	return ((y2 - y1)*(x - x1) + (-x2 + x1)*(y - y1));
}

bool extraHelperPointInTriangle(int x, int y, int x1,int y1, int x2, int y2, int x3, int y3) {
	return ((extraHelperSide(x1, y1, x2, y2, x, y) >= 0) && (extraHelperSide(x2, y2, x3, y3, x, y) >= 0) && (extraHelperSide(x3, y3, x1, y1, x, y) >= 0)); 
}


void extrasDigitizeCStick(Buttons &btn) {
	uint8_t x = btn.Cx;
	uint8_t y = btn.Cy;

	uint8_t xCenter = 128;
	uint8_t yCenter = 128;


	//Coordinates of the triangles we use to map coordinates to their cardinals
	//Ideally these should be configurable in the setting but i can't be bothered right now
	uint8_t xTopLeft = 8;
	uint8_t yTopLeft = 255;

	uint8_t xTopRight = 255;
	uint8_t yTopRight = 255;

	uint8_t xBottomLeft = 8;
	uint8_t yBottomLeft = 0;

	uint8_t xBottomRight = 255;
	uint8_t yBottomRight = 0;
	
	//deadzone check
	if(x > 150 || y > 150) {
		//In order to avoid potential edge cases with the dot product, we expand the triangle for left and right by one coordinate to make sure that every value gets mapped
		//left
		if(extraHelperPointInTriangle(x,y,xCenter,yCenter,xBottomLeft+1,yBottomLeft,xTopLeft+1,yTopLeft)) {
			x = 48;
			y = 128;
		}
		//right
		if(extraHelperPointInTriangle(x,y,xCenter,yCenter,xBottomRight-1,yBottomRight,xTopRight-1,yTopRight)) {
			x = 208;
			y = 128;
		}
		//up
		if(extraHelperPointInTriangle(x,y,xCenter,yCenter,xTopLeft,yTopLeft,xTopRight,yTopRight)) {
			x = 128;
			y = 208;
		}
		//down
		if(extraHelperPointInTriangle(x,y,xCenter,yCenter,xBottomLeft,yBottomLeft,xBottomRight,yBottomRight)) {
			x = 128;
			y = 48;
		}
		btn.Cx = x;
		btn.Cy = y;
	}


}



#endif //EXTRAS_H
