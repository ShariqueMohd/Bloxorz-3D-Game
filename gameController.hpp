#include "windowController.hpp"
#include "gameObjects.hpp"
#include "headers.hpp"
#include "VP.hpp"

#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

class gameController{
	
public:
	//gameObjects *go_;
	gameObjects go;

	gameController();	//initialise the block 
	
	void displayAll();

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void mouse_moveCallback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_buttonCallback(GLFWwindow* window, int button, int action, int mods);
	static void mouse_scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		
};

#endif