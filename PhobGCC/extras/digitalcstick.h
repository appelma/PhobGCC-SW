#ifndef EXTRAS_DIGITALCSTICK_H
#define EXTRAS_DIGITALCSTICK_H

namespace dcs {

	ExtrasSlot extrasDCSConfigSlot = EXTRAS_UNSET;

	enum DCSSettings{
		DCS_SETTING_ENABLE,
		DCS_SETTING_TOP,
		DCS_SETTING_BOTTOM,
		DCS_SETTING_UNUSED
	};

	enum DCSSettingEnable{
		DCS_DISABLED,
		DCS_ENABLED
	};
	
	//since we only get 4 ints of space we have to pack the settings of two sides together
	//int32_t  i32 = (int32_t)( (uint32_t)s1<<16 | (uint32_t)s2 );
	
	//Config values, each of these adjusts the mapping by shifting the horizontal coordinate of the diagonal line that seperates each input.
	//positive values shift them away from the center (increase upair/dair area, positive values move them towards it (increase fair/bair area)
	int8_t topLeftAdjust = 0;
	int8_t topRightAdjust = 0;
	int8_t bottomLeftAdjust = 0;
	int8_t bottomRightAdjust = 0;
	



	int extraHelperSide(int x, int y, int x1,int y1, int x2, int y2) {
		return ((y2 - y1)*(x - x1) + (-x2 + x1)*(y - y1));
	}	

	bool extraHelperPointInTriangle(int x, int y, int x1,int y1, int x2, int y2, int x3, int y3) {
		return ((extraHelperSide(x1, y1, x2, y2, x, y) >= 0) && (extraHelperSide(x2, y2, x3, y3, x, y) >= 0) && (extraHelperSide(x3, y3, x1, y1, x, y) >= 0)); 
	}


	void extrasDigitizeCStick(Buttons &btn) {
		if (config[DCS_SETTING_ENABLE].intValue != DCS_ENABLED) {
			return; //do nothing, keep cstick normal
		}
		
		uint8_t x = btn.Cx;
		uint8_t y = btn.Cy;
	
		uint8_t xCenter = 127;
		uint8_t yCenter = 127;
	
		
		//Coordinates that define the triangles we use to map coordinates to their cardinals, adjustedable by the settings
		//NOTE: Probably should be adjusted, potentially unmapped coordinates could be prevented by increasing the size of the triangles outward. Default config should be fine as is though
		int16_t xTopLeft = 0 + topLeftAdjust;
		int16_t yTopLeft = 255;
	
		int16_t xTopRight = 255 - topRightAdjust;
		int16_t yTopRight = 255;
	
		int16_t xBottomLeft = 0 + bottomLeftAdjust;
		int16_t yBottomLeft = 0;
	
		int16_t xBottomRight = 255 - bottomRightAdjust;
		int16_t yBottomRight = 0;
		
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
	
	bool toggle(IntOrFloat config[]) {
		int& enabled = config[DCS_SETTING_ENABLE].intValue;
		if (enabled != DCS_DISABLED){
			enabled = DCS_DISABLED;
		} else {
			enabled = DCS_ENABLED;
		}
		setExtrasSettingInt(extrasDCSConfigSlot, DCS_SETTING_ENABLE, enabled);
		return (enabled == DCS_ENABLED);
	}
	
	//Used for configuring extras with the Dpad
	 void dcsconfig (IntOrFloat config[], Cardinals dpad) {
	 	//TODO
	 }

} 

#endif //EXTRAS_DIGITALCSTICK_H
