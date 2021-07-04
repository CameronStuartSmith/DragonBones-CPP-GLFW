#include "XMLDataParser.h"
#include "../utils/DBDataUtil.h"

namespace DragonBones {
	using namespace rapidxml;
	using std::string;
	using std::vector;

	TextureAtlasData* XMLDataParser::parseTextureAtlasData(std::string filename, float scale) {
		file<> xmlFile(filename.c_str()); // Default template is char
		xml_document<> doc;
		doc.parse<0>(xmlFile.data());
		xml_node<> *textureAtlasNode = doc.first_node();
		xml_attribute<> *textureAtlasAttribute = textureAtlasNode->first_attribute();
		string name = textureAtlasAttribute->value();
		textureAtlasAttribute = textureAtlasAttribute->next_attribute("width");
		int width = textureAtlasAttribute->valueAsInt();
		textureAtlasAttribute = textureAtlasAttribute->next_attribute("height");
		int height = textureAtlasAttribute->valueAsInt();
		TextureAtlasData* textureAtlas = new TextureAtlasData(name, width, height);
		xml_node<> * subTexture = textureAtlasNode->first_node();
		while(subTexture) {
			xml_attribute<> *a = subTexture->first_attribute("name");
			string name = a->value();
			a = a->next_attribute("width");
			int width = a->valueAsInt()/scale;
			a = a->next_attribute("height");
			int height = a->valueAsInt()/scale;
			a = a->next_attribute("x");
			int x = a->valueAsInt()/scale;
			a = a->next_attribute("y");
			int y = a->valueAsInt()/scale;
			SubTextureData* data = new SubTextureData(name,x,y,width,height);
			textureAtlas->addSubTextureData(data);
			//printf("%s has X:%i Y:%i W:%i H:%i \n", data->name.c_str(), data->x, data->y, data->width, data->height);
			subTexture = subTexture->next_sibling();
		}
		
		//printf("Size of it is %i \n", textureAtlas->getSubTextureDataList().size());
		//printf("Name: %s Width: %i Height: %i \n", name.c_str(), width, height);
		return textureAtlas;
	}

	SkeletonData* XMLDataParser::parseSkeletonData(std::string filename) {
		SkeletonData* skeletonData = new SkeletonData();
		file<> xmlFile(filename.c_str());
		xml_document<> doc;
		doc.parse<0>(xmlFile.data());

		xml_node<>* dragonBones = doc.first_node(); 
		xml_attribute<>* dragonBoneA = dragonBones->first_attribute("name");
		std::string name = dragonBoneA->value(); 

		dragonBoneA = dragonBoneA->next_attribute("frameRate");
		int framerate = dragonBoneA->valueAsInt(); 

		dragonBoneA = dragonBoneA->next_attribute("version");
		string version = dragonBoneA->value();

		printf("%s FrameRate:%i Version:%s \n", name.c_str(), framerate, version.c_str());

		if(version == "1.5" || version == "2.0" || version =="2.1" || version =="2.1.1" || version =="2.1.2" || version =="2.2" ) {
			//Old version. Havent implemented parser yet
			printf("Tis is an old version! Upgrade \n");
			return 0;
		} else if(version != "2.3") {
			printf("Not a valid version for DragonBones \n");
		}

		skeletonData->name = name;
		
		xml_node<>* armature = dragonBones->first_node("armature");
		while(armature) {
			ArmatureData* armatureData = parseArmatureData(armature, skeletonData, framerate);
			skeletonData->addArmatureData(armatureData);
			armature = armature->next_sibling("armature");
		}
		return skeletonData;
	}

	ArmatureData* XMLDataParser::parseArmatureData(rapidxml::xml_node<>* armatureNode, SkeletonData* skeletonData, int framerate) {
		ArmatureData* armatureData = new ArmatureData();
		armatureData->name = armatureNode->first_attribute("name")->value();
		printf("ARMATURE: %s \n", armatureData->name.c_str());
		xml_node<>* child = armatureNode->first_node();
		std::string childName = child->name();
		bool notDone = true;
		int bn = 0;
		while(notDone) {
			while(childName == "bone") {
				//printf("Bone: %s \n", child->first_attribute()->value());
				BoneData* boneData = parseBoneData(child);
				if(boneData)
					armatureData->addBoneData(boneData);
				child = child->next_sibling();
				childName = child->name();
				bn++;
			}
			//printf("Number is %i \n", armatureData->_boneDataList.size());

			while(childName == "skin") {
				//printf("Skin \n");
				SkinData* skinData = parseSkinData(child, skeletonData);
				if(skinData)
					armatureData->addSkinData(skinData);
				child = child->next_sibling();
				childName = child->name();
			}

			DBDataUtil::transformArmatureData(armatureData);
			armatureData->sortBoneDataList();

			//printf("Number is %i \n", armatureData->_boneDataList.size());

			while(childName == "animation") {
				//printf("Animation: %s\n", child->first_attribute()->value());
				AnimationData* animationData = parseAnimationData(child, armatureData, framerate);
				if(animationData)
					armatureData->addAnimationData(animationData);
				child = child->next_sibling();
				if(child)
					childName = child->name();
				else
					childName = "";
			}
			notDone = false;
		}
		return armatureData;
	}

	BoneData* XMLDataParser::parseBoneData(xml_node<>* boneNode) {
		BoneData* boneData = new BoneData();
		xml_attribute<>* boneNodeA = boneNode->first_attribute("name");
		boneData->name = boneNodeA->value();
		xml_attribute<>* parentA = boneNodeA->next_attribute("parent");
		if(parentA) {
			boneData->parent = parentA->value();
		}
		//printf("%s with parent:%s \n", boneData->name.c_str(), boneData->parent.c_str());
		xml_node<>* boneTransform = boneNode->first_node();
		if(boneTransform)
			parseTransform(boneTransform, boneData->global, boneData->pivot);
		boneData->transform->copy(boneData->global);
		return boneData;
	}

	SkinData* XMLDataParser::parseSkinData(xml_node<>* skinNode, SkeletonData* skeletonData) {
		SkinData* skinData = new SkinData();
		skinData->name = skinNode->first_attribute("name")->value();
		xml_node<>* slotNode = skinNode->first_node();
		while(slotNode) {
			SlotData* slotData = parseSlotData(slotNode, skeletonData);
			skinData->addSlotData(slotData);
			slotNode = slotNode->next_sibling();
		}
		return skinData;
	}

	SlotData* XMLDataParser::parseSlotData(xml_node<>* slotNode, SkeletonData* skeletonData) {
		SlotData* slotData = new SlotData();
		xml_attribute<>* slotAttribute = slotNode->first_attribute("name");
		slotData->name = slotAttribute->value();
		slotAttribute = slotAttribute->next_attribute("parent");
		if(slotAttribute->value())
			slotData->parent = slotAttribute->value();
		slotAttribute = slotAttribute->next_attribute("z");
		slotData->zOrder = slotAttribute->valueAsInt();
		//printf("Name:%s parent:%s z:%i \n", slotData->name.c_str(), slotData->parent.c_str(), slotData->zOrder);

		xml_node<>* displayNode = slotNode->first_node();
		while(displayNode) {
			DisplayData* displayData = parseDisplayData(displayNode, skeletonData);
			slotData->addDisplayData(displayData);
			displayNode = displayNode->next_sibling();
		}

		return slotData;
	}

	DisplayData* XMLDataParser::parseDisplayData(xml_node<>* displayNode, SkeletonData* skeletonData) {
		DisplayData* displayData = new DisplayData();
		xml_attribute<>* displayAttribute = displayNode->first_attribute("name");
		displayData->name = displayAttribute->value();
		displayAttribute = displayAttribute->next_attribute("type");
		displayData->type = displayAttribute->value();

		displayData->pivot = skeletonData->addSubTexturePivot(0,0,displayData->name);

		parseTransform(displayNode->first_node("transform"), displayData->transform, displayData->pivot);
		//printf("Name:%s, x:%f, y:%f, skewX:%f, skewY:%f, pivotX:%f, pivotY:%f \n",
		///	displayData->name.c_str(), displayData->transform->x,displayData->transform->y,
		//	displayData->transform->skewX, displayData->transform->skewY,
		//	displayData->pivot->x, displayData->pivot->y);
		return displayData;
	}

	AnimationData* XMLDataParser::parseAnimationData(xml_node<>* animationNode, ArmatureData* armatureData, int framerate) {
		AnimationData* animationData = new AnimationData();
		xml_attribute<>* animationAttribute = animationNode->first_attribute("name");
		animationData->name = animationAttribute->value();
		animationAttribute = animationAttribute->next_attribute("fadeInTime");
		animationData->setFadeInTime(animationAttribute->valueAsFloat());
		animationAttribute = animationAttribute->next_attribute("duration");
		animationData->setDuration(animationAttribute->valueAsFloat()/framerate);
		animationAttribute = animationAttribute->next_attribute("scale");
		animationData->setScale(animationAttribute->valueAsFloat());
		animationAttribute = animationAttribute->next_attribute("loop");
		animationData->loop = animationAttribute->valueAsInt();
		animationAttribute = animationAttribute->next_attribute("tweenEasing");
		animationData->tweenEasing = animationAttribute->valueAsFloat();
		animationData->framerate = framerate;
		//printf("Name:%s fadeInTime:%f duration:%f scale:%f loop:%i tweenEasing:%f \n \n",
		//	animationData->name.c_str(), animationData->getFadeInTime(),
		//	animationData->getDuration(), animationData->getScale(),
		//	animationData->loop, animationData->tweenEasing);

		parseTimeline(animationNode, animationData, "parseMainFrame", framerate);

		TransformTimeline* timeline;
		std::string timelineName;
		xml_node<>* timelineXML = animationNode->first_node("timeline");
		while(timelineXML) {
			timeline = parseTransformTimeLine(timelineXML, animationData->getDuration(), framerate);
			timelineName = timelineXML->first_attribute("name")->value();
			animationData->addTimeLine(timeline, timelineName);
			timelineXML = timelineXML->next_sibling("timeline");
		}

		DBDataUtil::addHideTimeline(animationData, armatureData);
		DBDataUtil::transformAnimationData(animationData, armatureData);

		//printf("%s with bone size of %i \n", animationData->name.c_str(), armatureData->_boneDataList.size());

		return animationData;
	}

	void XMLDataParser::parseTimeline(xml_node<>* timelineNode, Timeline* timeline, std::string frametype, int framerate) {
		int position = 0;
		Frame* frame = 0;
		xml_node<>* subNode = timelineNode->first_node("frame");
		while(subNode){
			if(frametype == "parseMainFrame")
				frame = parseMainFrame(subNode, framerate);
			else if(frametype == "parseTransformFrame")
				frame = parseTransformFrame(subNode, framerate);
			if(frame) {
				frame->position = position;
				timeline->addFrame(frame);
				position += frame->duration;
			}
			subNode = subNode->next_sibling("frame");
		}

		if(frame) {
			frame->duration = timeline->getDuration() - frame->position;
			//printf("%s \n", frame->action.c_str());
		}

	}

	TransformTimeline* XMLDataParser::parseTransformTimeLine(xml_node<>* timelineNode, float duration, int framerate) {
		TransformTimeline* timeline = new TransformTimeline();
		timeline->setDuration(duration);

		parseTimeline(timelineNode, timeline, "parseTransformFrame", framerate);
		
		xml_attribute<>* timelineAttribute = timelineNode->first_attribute("scale");
		timeline->setScale(timelineAttribute->valueAsFloat());
		timelineAttribute = timelineAttribute->next_attribute("offset");
		timeline->setOffset(timelineAttribute->valueAsFloat());

		return timeline;
	}
/**
**Added this to parseMainFrame and parseTransformFrame for performance reasons
	void XMLDataParser::parseFrame(xml_node<>* frameNode, Frame* frame, int framerate) {
		xml_attribute<>* frameAttr = frameNode->first_attribute();
		while(frameAttr) {
			std::string name = frameAttr->name();
			if(name=="duration")
				frame->duration = frameAttr->valueAsFloat() / framerate;
			else if(name=="action")
				frame->action = frameAttr->value();
			else if(name=="event")
				frame->event = frameAttr->value();
			else if(name=="sound")
				frame->sound = frameAttr->value();
			frameAttr=frameAttr->next_attribute();
		}
	}**/

	Frame* XMLDataParser::parseMainFrame(xml_node<>* frameNode, int framerate) {
		Frame* frame = new Frame();
		//parseFrame(frameNode, frame, framerate);
		xml_attribute<>* frameAttr = frameNode->first_attribute();
		while(frameAttr) {
			std::string name = frameAttr->name();
			if(name=="duration")
				frame->duration = frameAttr->valueAsFloat() / framerate;
			else if(name=="action")
				frame->action = frameAttr->value();
			else if(name=="event")
				frame->event = frameAttr->value();
			else if(name=="sound")
				frame->sound = frameAttr->value();
			frameAttr=frameAttr->next_attribute();
		}
		return frame;
	}

	TransformFrame* XMLDataParser::parseTransformFrame(xml_node<>* frameNode, int framerate) {
		TransformFrame* frame = new TransformFrame();
		//parseFrame(frameNode, frame, framerate);

		xml_attribute<>* frameAttr = frameNode->first_attribute();
		while(frameAttr) {
			std::string name = frameAttr->name();
				if(name == "z")
					frame->zOrder = frameAttr->valueAsInt();
				else if(name == "displayIndex")
					frame->displayIndex = frameAttr->valueAsInt();
				else if(name == "hide")
					frame->visible = (frameAttr->valueAsInt() != 1);
				else if(name == "tweenEasing")
					frame->tweenEasing = frameAttr->valueAsFloat();
				else if(name == "tweenRotate")
					frame->tweenRotate = frameAttr->valueAsFloat();
				else if(name=="duration")
					frame->duration = frameAttr->valueAsFloat() / framerate;
				else if(name=="action")
					frame->action = frameAttr->value();
				else if(name=="event")
					frame->event = frameAttr->value();
				else if(name=="sound")
					frame->sound = frameAttr->value();
			frameAttr=frameAttr->next_attribute();
		}


		parseTransform(frameNode->first_node("transform"), frame->global, frame->pivot);

		frame->transform->copy(frame->global);

		xml_node<>* colorNode = frameNode->first_node("colorTransform");
		if(colorNode) {
			frame->color = new ColorTransform();
			xml_attribute<>* colorAttribute = colorNode->first_attribute("aO");
			frame->color->setAlphaOffset(colorAttribute->valueAsFloat());
			colorAttribute = colorAttribute->next_attribute("rO");
			frame->color->setRedOffset(colorAttribute->valueAsFloat());
			colorAttribute = colorAttribute->next_attribute("gO");
			frame->color->setGreenOffset(colorAttribute->valueAsFloat());
			colorAttribute = colorAttribute->next_attribute("bO");
			frame->color->setBlueOffset(colorAttribute->valueAsFloat());
			colorAttribute = colorAttribute->next_attribute("aM");
			frame->color->setAlphaMultiplier(colorAttribute->valueAsFloat() * 0.01);
			colorAttribute = colorAttribute->next_attribute("rM");
			frame->color->setRedMultiplier(colorAttribute->valueAsFloat() * 0.01);
			colorAttribute = colorAttribute->next_attribute("gM");
			frame->color->setGreenMultiplier(colorAttribute->valueAsFloat() * 0.01);
			colorAttribute = colorAttribute->next_attribute("bM");
			frame->color->setBlueMultiplier(colorAttribute->valueAsFloat() * 0.01);
		}

		return frame;
	}

	void XMLDataParser::parseTransform(xml_node<>* transformNode, DBTransform* transform, Vector2D* pivot) {
		if(transformNode)
		{
			if(transform)
			{
				xml_attribute<> *transformAttribute = transformNode->first_attribute("x");
				transform->x = transformAttribute->valueAsFloat();
				transformAttribute = transformAttribute->next_attribute("y");
				transform->y = transformAttribute->valueAsFloat();
				transformAttribute = transformAttribute->next_attribute("skX");
				transform->skewX = transformAttribute->valueAsFloat() * atan(1.0)*4 / 180;
				transformAttribute = transformAttribute->next_attribute("skY");
				transform->skewY = transformAttribute->valueAsFloat() * atan(1.0)*4 / 180;
				transformAttribute = transformAttribute->next_attribute("scX");
				transform->scaleX = transformAttribute->valueAsFloat();
				transformAttribute = transformAttribute->next_attribute("scY");
				transform->scaleY = transformAttribute->valueAsFloat();
				if(pivot) {
					transformAttribute = transformAttribute->next_attribute("pX");
					if(transformAttribute) {
						pivot->x = transformAttribute->valueAsFloat();
					transformAttribute = transformAttribute->next_attribute("pY");
					if(transformAttribute)
						pivot->y = transformAttribute->valueAsFloat();
					}
				}
				//printf("%f, %f, %f, %f, %f, %f, %f, %f \n", transform->x, transform->y, transform->skewX, transform->skewY, transform->scaleX, transform->scaleY, pivot.x, pivot.y);
			}
		}
	}
}