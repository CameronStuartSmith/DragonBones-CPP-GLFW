#ifndef DISPLAYBRIDGE_H
#define DISPLAYBRIDGE_H

#include "../objects/DBTransform.h"
#include "../graphics/ColorTransform.h"
#include <glm/glm.hpp>
#include "../as3/Object.h"

class Image;

namespace DragonBones {
	class Texture;
	class DisplayObject;
	class DisplayObjectContainer;
	class DisplayBridge {
	private:
		DisplayObject* _display;
		Texture* _imageBackup;
		Image* _textureBackup;
		float _pivotXBackup;
		float _pivotYBackup;
	public:
		bool getVisible();
		void setVisible(bool value);

		Object* getDisplay();
		void setDisplay(Object* value);

		DisplayBridge();
		~DisplayBridge();

		void updateTransform(glm::mat4 matrix, DBTransform* transform);

		void updateColor(
			float aOffset, 
			float rOffset, 
			float gOffset, 
			float bOffset, 
			float aMultiplier, 
			float rMultiplier, 
			float gMultiplier, 
			float bMultiplier);

		//Adds the original display object to another display object.
		void addDisplay(DisplayObjectContainer* container, int index=-1);

		//remove the original display object from its parent.
		void removeDisplay();
	};
}


#endif