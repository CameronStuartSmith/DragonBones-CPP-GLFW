#ifndef CONSTVALUES_H
#define CONSTVALUES_H

#include <string>
#include <math.h>

namespace DragonBones  {
	namespace Names {
	const double M_PI = atan(1.0)*4;
	float ANGLE_TO_RADIAN = M_PI / 180;

		const char* DRAGON_BONES = "dragonBones";
		const char* ARMATURE = "armature";
		const char* SKIN = "skin";
		const char* BONE = "bone";
		const char* SLOT = "slot";
		const char* DISPLAY = "display";
		const char* ANIMATION = "animation";
		const char* TIMELINE = "timeline";
		const char* FRAME = "frame";
		const char* TRANSFORM = "transform";
		const char* COLOR_TRANSFORM = "colorTransform";
		
		const char* TEXTURE_ATLAS = "TextureAtlas";
		const char* SUB_TEXTURE = "SubTexture";
		
		const char* A_VERSION = "version";
		const char* A_FRAME_RATE = "frameRate";
		const char* A_NAME = "name";
	const char* A_PARENT = "parent";
	const char* A_TYPE = "type";
	const char* A_FADE_IN_TIME = "fadeInTime";
	const char* A_DURATION = "duration";
	const char* A_SCALE = "scale";
	const char* A_OFFSET = "offset";
	const char* A_LOOP = "loop";
	const char* A_EVENT = "event";
	const char* A_SOUND = "sound";
	const char* A_ACTION = "action";
	const char* A_HIDE = "hide";
	const char* A_TWEEN_EASING = "tweenEasing";
	const char* A_TWEEN_ROTATE = "tweenRotate";
	const char* A_DISPLAY_INDEX = "displayIndex";
	const char* A_Z_ORDER = "z";
	const char* A_WIDTH = "width";
	const char* A_HEIGHT = "height";
	const char* A_X = "x";
	const char* A_Y = "y";
	const char* A_SKEW_X = "skX";
	const char* A_SKEW_Y = "skY";
	const char* A_SCALE_X = "scX";
	const char* A_SCALE_Y = "scY";
	const char* A_PIVOT_X = "pX";
	const char* A_PIVOT_Y = "pY";
	const char* A_ALPHA_OFFSET = "aO";
	const char* A_RED_OFFSET = "rO";
	const char* A_GREEN_OFFSET = "gO";
	const char* A_BLUE_OFFSET = "bO";
	const char* A_ALPHA_MULTIPLIER = "aM";
	const char* A_RED_MULTIPLIER = "rM";
	const char* A_GREEN_MULTIPLIER = "gM";
	const char* A_BLUE_MULTIPLIER = "bM";
	};
};


#endif