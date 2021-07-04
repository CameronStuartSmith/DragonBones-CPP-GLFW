#ifndef TEXTURE_H
#define TEXTURE_H

#include "DisplayObject.h"
#include "../graphics/ColorTransform.h"

class Image;

namespace DragonBones {
	class Texture : public DisplayObject {
	public:
		Image* image;

		Texture(Image* img);
		void render();
		ColorTransform* color;
	};
}

#endif