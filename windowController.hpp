#include "headers.hpp"

#ifndef WINDOWCONTROLLER_H
#define WINDOWCONTROLLER_H

class windowController{
	
	GLuint programId;
	GLFWwindow* window;
	string windowName;
	int width, height;

public:
	windowController(string windowName, int width, int height);
	
	GLFWwindow* getWindow();

	GLuint getProgramId();
	void setProgramId(GLuint programId);

	void initWindow();
	void setWindowContext();

	void setViewPort();
	
	void keyCallback(GLFWkeyfun cbfunc);
	void mouseCallback(GLFWcursorposfun cursorCallbackFn, GLFWmousebuttonfun mouseCallbackFn, GLFWscrollfun scrollCallbackFunc);

	bool isWindowClosed();
	void close();
};

#endif