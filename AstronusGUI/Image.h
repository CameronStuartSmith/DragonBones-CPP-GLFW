#ifndef IMAGE_H
#define IMAGE_H

#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include "Shader.h"
#include <stdio.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <map>
#include <string>

struct ImageData {
	GLuint texture;
	int width;
	int height;
	int channels;
};

class Image {
private:
	static std::map<std::string, ImageData> _imagePool;
	GLuint loadImage(std::string filename, int &width, int &height, int &channels);

	GLuint programID, textureID;
	
	//float x, y;
	GLuint vbo_quad_vertices, vbo_quad_texcoords;
	GLuint ibo_quad_elements;
	GLint attribute_coord2d, attribute_texcoord;
	GLint uniform_mvp, uniform_mytexture;
	void load(float x, float y, float width, float height, float imageWidth, float imageHeight);
public:
	Image(const char* filename);
	Image(const char* filename, float x, float y, float subWidth, float subHeight);
	void render(glm::mat4 mvp);
	float scale;
	int width, height;
};

#endif