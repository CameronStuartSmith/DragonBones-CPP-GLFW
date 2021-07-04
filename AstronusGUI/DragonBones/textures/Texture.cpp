#include "Texture.h"
#include "../../Image.h"
#include "../utils/TransformUtil.h"
#include "DisplayObjectContainer.h"

namespace DragonBones {
	Texture::Texture(Image* img) : DisplayObject(){
		image = img;
		visible = true;
	}

	void Texture::render() {
		if(visible) {
			if(image) {

				if(parent)
				{
					image->render(this->parent->rec*getTransformationMatrix());
				} else {

					image->render(getTransformationMatrix());

				}

			}
		}
	}

}