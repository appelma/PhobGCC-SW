int extraHelperSide(int x, int y, int x1,int y1, int x2, int y2) {
	return ((y2 - y1)*(x - x1) + (-x2 + x1)*(y - y1));
}

bool extraHelperPointInTriangle(int x, int y, int x1,int y1, int x2, int y2, int x3, int y3) {
	return ((extraHelperSide(x1, y1, x2, y2, x, y) >= 0) && (extraHelperSide(x2, y2, x3, y3, x, y) >= 0) && (extraHelperSide(x3, y3, x1, y1, x, y) >= 0)); 
}


void extrasDigitizeCStick(Buttons &btn) {
	uint8_t x = btn.Cx;
	uint8_t y = btn.Cy;

	uint8_t xCenter = 127;
	uint8_t yCenter = 127;


	//Coordinates of the triangles we use to map coordinates to their cardinals
	//Ideally these should be configurable in the setting but i can't be bothered right now
	uint8_t xTopLeft = 0;
	uint8_t yTopLeft = 255;

	uint8_t xTopRight = 255;
	uint8_t yTopRight = 255;

	uint8_t xBottomLeft = 0;
	uint8_t yBottomLeft = 0;

	uint8_t xBottomRight = 255;
	uint8_t yBottomRight = 0;
	
	//deadzone check
	if(x > 150 || y > 150 || x < 106 || y < 106) {
		//left
		if(extraHelperPointInTriangle(x,y,xCenter,yCenter,xBottomLeft,yBottomLeft,xTopLeft,yTopLeft)) {
			x = 47;
			y = 127;
		}
		//right
		if(extraHelperPointInTriangle(x,y,xCenter,yCenter,xTopRight,yTopRight,xBottomRight,yBottomRight)) {
			x = 207;
			y = 127;
		}
		//up
		if(extraHelperPointInTriangle(x,y,xCenter,yCenter,xTopLeft,yTopLeft,xTopRight,yTopRight)) {
			x = 127;
			y = 207;
		}
		//down
		if(extraHelperPointInTriangle(x,y,xCenter,yCenter,xBottomRight,yBottomRight,xBottomLeft,yBottomLeft)) {
			x = 127;
			y = 47;
		}
		btn.Cx = x;
		btn.Cy = y;
	}


}

