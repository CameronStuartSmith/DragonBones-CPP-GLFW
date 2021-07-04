#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Image.h"
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <windows.h>
#include "DragonBones\DragonBones.h"
#include "DragonBones\events\FrameEvent.h"

using namespace DragonBones;
void initGLFW();
GLFWwindow* window;
float zoom;
DragonBones::Armature* armature;
bool _isJumping = false;
int _moveDir = 0;
float _speedX = 0;
bool _isLeftDown = false;
bool _isRightDown = false;
float _speedY = 0;
DBFactory* factory;
int weaponLevel = 0;
std::vector<std::string> weaponType;
int currentWeaponType = 0;

void updateMovement()
{
	if (_isJumping) 
	{
		return;
	}

	if (_moveDir == 0) 
	{
		_speedX = 0;
		armature->getAnimation()->gotoAndPlay("stand");
		printf("HERE! \n");
	}
	else 
	{
		_speedX = _moveDir * 4;
		armature->getAnimation()->gotoAndPlay("run");
		armature->getDisplay()->setScaleX( _moveDir);
	}
}

void move(int dir) 
{
	if (_isLeftDown && _isRightDown) 
	{
	}
	else if (_isLeftDown)
	{
		dir = -1;
	}
	else if (_isRightDown)
	{
		dir = 1;
	}
	else 
	{
		//dir = 0;
	}
	
	if (_moveDir == dir) 
	{
		return;
	}
	_moveDir = dir;
	updateMovement();
}

void updateSpeed() 
{
	if (_isJumping) 
	{
		_speedY += 0.6;
	}

	if (_speedX != 0) 
	{
		//_armatureDisplay.x += _speedX;
		armature->getDisplay()->setX(armature->getDisplay()->getX() + _speedX);
		if (armature->getDisplay()->getX() < 0) 
		{
			armature->getDisplay()->setX(0);
		}
		else if (armature->getDisplay()->getX() > 800) 
		{
			armature->getDisplay()->setX(800);
			//_armatureDisplay.x = 800;
		}
	}


	if (_speedY != 0) 
	{
		//_armatureDisplay.rotation = _speedY * 0.02 * _armatureDisplay.scaleX;
		armature->getDisplay()->setY(armature->getDisplay()->getY() + _speedY);
		//_armatureDisplay-> += _speedY;
		if (armature->getDisplay()->getY() > 400) 
		{
			armature->getDisplay()->setY(400);
			//_armatureDisplay.y = 400;
			_isJumping = false;
			_speedY = 0;
			//_armatureDisplay.rotation = 0;
			updateMovement();
		}
	}
}

void jump()
{
	if (_isJumping) 
	{
		return;
	}
	_speedY = -15;
	_isJumping = true;
	armature->getAnimation()->gotoAndPlay("jump");
}

void test() {
	printf("It works! \n");
}

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_F1 && action == GLFW_REPEAT) {
		printf("Hey there \n");
	}
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		zoom -= 0.1f;
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		zoom += 0.1f;
	}

	if(key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		//_isLeftDown = true;
		move(-1);
	}
	if(key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
	{
		_speedX = 0;
		//_isLeftDown = false;
		move(0);
	}

	if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		//_isRightDown = true;
		move(1);
	}
	
	if(key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
	{
		_speedX = 0;
		//_isRightDown = false;
		move(0);
	}

	if(key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		jump();
	}

	if(key == '5' && action == GLFW_PRESS)
	{
		DragonBones::Bone* _arm = armature->getBone("armOutside");
		//armature->getBones()[0];
		//for(DragonBones::Bone* bone : armature->getBones())
		//{
		//	printf("%s \n", bone->name.c_str());
		//}
		std::stringstream ss;
		ss << "attack_" << "pike" << "_1";
		_arm->getChildArmature()->getAnimation()->gotoAndPlay(ss.str());
		//armature->getAnimation()->gotoAndPlay("stand");
	}

	if(key == '6' && action == GLFW_PRESS)
	{
		std::stringstream ss;
		weaponLevel++;
		if(weaponLevel > 3)
		{
			weaponLevel = 1;
			currentWeaponType++;
			if(currentWeaponType > 2)
				currentWeaponType=0;
		}
	
		Bone* weapon = armature->getBone("armOutside")->getChildArmature()->getBone("weapon");
		//weapon->setDisplay( factory->getTextureDisplay((Texture)"knightFolder/" + weaponName + "_" + (weaponLevel + 1)));
		
		ss << "knightFolder/" << "pike" << "_" << weaponLevel;
		weapon->setDisplay((Texture*)factory->getTextureDisplay(ss.str()));

		//ss.clear();

		//ss << "ready_" << weaponType[currentWeaponType];

		//armature->getBone("armOutside")->getChildArmature()->getAnimation()->gotoAndPlay(ss.str());
	}

}

void FrameEventHandeler(Event* e)
{
	std::stringstream ss;
	ss << "ready_" << "pike";
	armature->getBone("armOutside")->getChildArmature()->getAnimation()->gotoAndPlay(ss.str());
}

int main(void)
{
	initGLFW();
	using namespace DragonBones;
	weaponType.push_back("sword");
	weaponType.push_back("pike");
	weaponType.push_back("axe");
	DisplayObjectContainer* scene = new DisplayObjectContainer();
	scene->setTransformationMatrix(glm::ortho( 0.0f, 800.f*1, 600.f*1, 0.0f, -1.f, 1.f));

	Texture* bigLogo = new Texture( new Image("../data/images/big-logo.png") );
	Texture* small_logo = new Texture( new Image("../data/images/logo.png") );
	Texture* poweredby = new Texture( new Image("../data/images/poweredbydb.png") );

	poweredby->setX(800 - poweredby->image->width);
	small_logo->setY(600 - small_logo->image->height);
	small_logo->setX(800/2 - small_logo->image->width/2);

	scene->addChild(bigLogo);
	scene->addChild(small_logo);
	scene->addChild(poweredby);



	TextureAtlas* textureAtlas = new TextureAtlas("../data/DragonBones/KnightJSON/texture.png",
		JSONParser::parseTextureAtlas("../data/DragonBones/KnightJSON/texture.json"));

	SkeletonData* skeleton = JSONParser::parseSkeletonData("../data/DragonBones/KnightJSON/skeleton.json");
	factory = new DBFactory();
	factory->addSkeletonData(skeleton);
	factory->addTextureAtlas(textureAtlas);
	armature = factory->buildArmature("knight");

	armature->getDisplay()->setX(400);
	armature->getDisplay()->setY(400);
	armature->getAnimation()->play();

	WorldClock->add(armature);

	armature->getBone("armOutside")->getChildArmature()->addEventListener(FrameEvent::ANIMATION_FRAME_EVENT, FrameEventHandeler);


	scene->addChild(armature->getDisplay());


	float gray = 103.0f/255.0f;
	zoom = 1.0f;
    while (!glfwWindowShouldClose(window))
    {
       // float ratio;
       // int width, height;

       // glfwGetFramebufferSize(window, &width, &height);
       // ratio = width / (float) height;

		glClearColor(gray, gray, gray, 1);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		WorldClock->advanceTime(-1);
		updateSpeed();
		scene->render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void initGLFW()
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_SAMPLES,4);
	
    window = glfwCreateWindow(800, 600, "DragonBones OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

	glfwMakeContextCurrent(window);

	GLenum glew_status = glewInit();

	if (GLEW_OK != glew_status) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
	}

    glfwSetKeyCallback(window, key_callback);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}