#ifndef XMLDATAPARSER_H
#define XMLDATAPARSER_H

#include <RapidXML\rapidxml.hpp>
#include <RapidXML\rapidxml_utils.hpp>
#include <sstream>
#include "SkeletonData.h"
#include "../textures/TextureAtlasData.h"
#include "TransformFrame.h"


namespace DragonBones {
	using namespace rapidxml;
	class XMLDataParser {
	private:
		ArmatureData* parseArmatureData(rapidxml::xml_node<>* armatureNode, SkeletonData* skeletonData, int framerate);
		BoneData* parseBoneData(xml_node<>* boneNode);
		SkinData* parseSkinData(xml_node<>* skinNode, SkeletonData* skeletonData);
		SlotData* parseSlotData(xml_node<>* slotNode, SkeletonData* skeletonData);
		DisplayData* parseDisplayData(xml_node<>* displayNode, SkeletonData* skeletonData);
		AnimationData* parseAnimationData(xml_node<>* animationNode, ArmatureData* armatureData, int framerate);
		void parseTimeline(xml_node<>* timelineNode, Timeline* timeline, std::string frametype, int framerate);
		TransformTimeline* parseTransformTimeLine(xml_node<>* timelineNode, float duration, int framerate);
		void parseFrame(xml_node<>* frameNode, Frame* frame, int framerate);
		Frame* parseMainFrame(xml_node<>* frameNode, int framerate);
		TransformFrame* parseTransformFrame(xml_node<>* frameNode, int framerate);
		void parseTransform(xml_node<>* transformNode, DBTransform* transform, Vector2D *pivot);
	public:
		XMLDataParser(){}
		~XMLDataParser(){}

		TextureAtlasData* parseTextureAtlasData(std::string filename, float scale);
		SkeletonData* parseSkeletonData(std::string filename);
	
	};
};


#endif