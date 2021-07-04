#include "DisplayObject.h"
#include "DisplayObjectContainer.h"

namespace DragonBones {
	DisplayObject::DisplayObject() : EventDispatcher(){
		name = "";
		visible = true;
		transformationMatrix = glm::mat4(1.0f);
		parent = 0;
		scaleX = 1;
		scaleY = 1;
	}

	DisplayObject::~DisplayObject(){
	}

	glm::mat4 DisplayObject::getTransformationMatrix(){
		return transformationMatrix;
	}

	void DisplayObject::setTransformationMatrix(glm::mat4 value){
		transformationMatrix = value;
	}

	void DisplayObject::setPivotX(float value){
		pivotX = value;
	}

	void DisplayObject::setPivotY(float value){
		pivotY = value;
	}

	float DisplayObject::getPivotX(){
		return pivotX;
	}

	float DisplayObject::getPivotY(){
		return pivotY;
	}

	void DisplayObject::removeFromParent(){
		if(parent)
			this->parent->removeChild(this);
	}

	void DisplayObject::setX(float value) {
		x = value;
		transformationMatrix[3][0] = x;
	}

	void DisplayObject::setY(float value) {
		y = value;
		transformationMatrix[3][1] = y;
	}

	float DisplayObject::getX() {
		return transformationMatrix[3][0];
	}

	float DisplayObject::getY() {
		return transformationMatrix[3][1];
	}

	void DisplayObject::setScaleX(float value) {
		this->scaleX = value;
		transformationMatrix[0][0] =  scaleX * cos(0); //a global->scaleX * cos(global->skewY);
		transformationMatrix[0][1] =  scaleX * sin(0); //b global->scaleX * sin(global->skewY);
		transformationMatrix[1][0] = -scaleY * sin(0); //c -global->scaleY * sin(global->skewX);
		transformationMatrix[1][1] =  scaleY * cos(0); //d global->scaleY * cos(global->skewX);
	}
}