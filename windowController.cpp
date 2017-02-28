#include "headers.hpp"
#include "varStore.hpp"

void windowController::initWindow(){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}

windowController::windowController(string windowName, int width, int height){
	this->windowName = windowName;
	this->width = width;
	this->height = height;
	
	initWindow();
	this->window = glfwCreateWindow(width, height, windowName.c_str(), NULL, NULL);

	if(window == nullptr){
   	 	std::cout << "Failed to create GLFW window" << std::endl;
    	glfwTerminate();
  	}	
}

void windowController::setWindowContext(){
	glfwMakeContextCurrent(this->window);
	glewExperimental = GL_TRUE;
  	if(glewInit() != GLEW_OK){
    	std::cout << "Failed to initialize GLEW" << std::endl;
  		return;
  	}
}

void windowController::setViewPort(){
	int width, height;
	glfwGetFramebufferSize(this->window, &width, &height);

	glViewport(0, 0, width, height);
}

bool windowController::isWindowClosed(){
	return glfwWindowShouldClose(this->window);
}

void windowController::close(){
	glfwSetWindowShouldClose(this->window, GL_TRUE);
}

GLuint windowController::getProgramId(){
	return this->programId;
}
void windowController::setProgramId(GLuint programId){
	this->programId = programId;
}

GLFWwindow* windowController::getWindow(){
	return this->window;
}

void windowController::keyCallback(GLFWkeyfun cbfunc){
	glfwSetKeyCallback(this->window, cbfunc);
}	

void windowController::mouseCallback(GLFWcursorposfun cursorCallbackFn, GLFWmousebuttonfun mouseCallbackFn, GLFWscrollfun scrollCallbackFn){
	glfwSetCursorPosCallback(this->window, cursorCallbackFn);
	glfwSetMouseButtonCallback(this->window, mouseCallbackFn);
	glfwSetScrollCallback(this->window, scrollCallbackFn);
}