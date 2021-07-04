#ifndef TEXTUREATLAS_H
#define TEXTUREATLAS_H

#include <string>
#include <map>

class Image;

namespace DragonBones {
	class TextureAtlasData;
	class TextureAtlas {
	private:
		TextureAtlasData* _subTextureData;
		float _scale;
		std::string _name;
		std::string _imagepath;
	public:
		std::map<std::string, Image*> _subTextureDic;
		std::string getName();
		TextureAtlas(std::string imagePath, TextureAtlasData* data);
		~TextureAtlas();
		Image* getTexture(std::string name);
		void parseData(TextureAtlasData* data);
	};
}

#endif