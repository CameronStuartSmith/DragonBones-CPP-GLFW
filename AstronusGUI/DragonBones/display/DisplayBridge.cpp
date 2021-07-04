#include "DisplayBridge.h"
#include "../textures/DisplayObjectContainer.h"
#include "../textures/Texture.h"
#include "../utils/TransformUtil.h"

namespace DragonBones {

		bool DisplayBridge::getVisible() {
			if(_display)
				return _display->visible;
			return false;
		}

		void DisplayBridge::setVisible(bool value){
			if(_display)
			{
				_display->visible = value;
			}
		}

		Object* DisplayBridge::getDisplay(){
			return _display;
		}

		void DisplayBridge::setDisplay(Object* value){
			if(value && _display) {
				if(checkIfClass<Texture>(_display) && checkIfClass<Texture>(value))
				{
				Texture* from = (Texture*) _display;
				Texture* to = (Texture*) value;
				if (from->image == to->image)
				{
					//printf("WHAT! 6 \n");
					if(from == _imageBackup)
					{
						from->image = _textureBackup;
						from->setPivotX(_pivotXBackup);
						from->setPivotY(_pivotYBackup);
						//from.readjustSize();
					}
					return;
				}
				//printf("WHAT! 5 \n");
				from->image = to->image;
				from->setPivotX(to->getPivotX());
				from->setPivotY(to->getPivotY());
				//from.readjustSize();
				return;
				}
			}
			
			if (_display == value)
			{
				//printf("WHAT! 4 \n");
				return;
			}
			
			int index = -1;
			DisplayObjectContainer* parent = 0;
			if (_display)
			{
				//printf("WHAT! 3 \n");
				DisplayObjectContainer* parent = _display->parent;
				if (parent)
				{
					index = _display->parent->getChildIndex(_display);
				}
				removeDisplay();
			}
			else if(checkIfClass<Texture>(value) && !_imageBackup)
			{
				//printf("WHAT! 2 \n");
				_imageBackup = (Texture*) value;
				_textureBackup = _imageBackup->image;
				_pivotXBackup = _imageBackup->getPivotX();
				_pivotYBackup = _imageBackup->getPivotY();
			}
			//printf("WHAT! \n");
			_display = (DisplayObject*)value;
			addDisplay(parent, index);
		}

		DisplayBridge::DisplayBridge() {
			_display = 0;
			_imageBackup = 0;
		}

		DisplayBridge::~DisplayBridge(){

		}

		void DisplayBridge::updateTransform(glm::mat4 matrix, DBTransform* transform){
			float pivotX = _display->getPivotX();
			float pivotY = _display->getPivotY();

			//TranformUtils::printMatrix(matrix);
			if(pivotX != 0 || pivotY != 0) {
				matrix[3][0] -= matrix[0][0] * pivotX + matrix[1][0] * pivotY;
				matrix[3][1] -= matrix[0][1] * pivotX + matrix[1][1] * pivotY;
			}
			//printf("Hit here \n");
			//TranformUtils::printMatrix(matrix);
			_display->setTransformationMatrix(matrix);
			//TranformUtils::printMatrix(_display->getTransformationMatrix());
		}

		void DisplayBridge::updateColor(float aOffset, float rOffset, float gOffset, float bOffset, 
			float aMultiplier, float rMultiplier, float gMultiplier, float bMultiplier){
				ColorTransform* color = new ColorTransform();
				color->setOffset(rOffset, gOffset,bOffset,aOffset);
				color->setMultiplier(rMultiplier, gMultiplier, bMultiplier, aMultiplier);
				if(checkIfClass<Texture>(_display))
				{
					Texture* texture = (Texture*) _display;
					texture->color = color;
				}
		}

		void DisplayBridge::addDisplay(DisplayObjectContainer* container, int index){
			if (container && _display)
			{
				if (index < 0)
				{
					container->addChild(_display);
				}
				else
				{
					if(index > container->getNumberOfChildren())
						container->addChildAt(_display, container->getNumberOfChildren());
					else
						container->addChildAt(_display, index);
				}
			}
		}
	
		void DisplayBridge::removeDisplay(){
			if(_display)
			{
				if(checkIfClass<DisplayObject>(_display))
				{
					DisplayObject* ob = (DisplayObject*)_display;
					if (ob && ob->parent)
					{
						ob->parent->removeChild(ob);
					}
				}
			}
		}
}
