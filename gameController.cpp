#include "headers.hpp"
#include "varStore.hpp"

gameController::gameController(){
	this->go.initTiles();
	this->go.initBlock();
}

void gameController::displayAll(){
	if(isHelicopterView){
		vp->zoomInHelicopter();
		if(isRotating){
			vp->setHelicopterView();
		}
	}
	this->go.displayTiles();
	this->go.displayBlock();
	// if Winner, then display winning screen
}

void gameController::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
	//newTime = glfwGetTime();
	//if(newTime - oldTime >= 0.5f){
		//oldTime = newTime;
		if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
			wc->close();
		}
		else if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS && Angle<=0.0f){
			Angle = acos(0.0f);
			gc->go.rotation(1, 0, 0, 0.1f);
		}
		else if(key == GLFW_KEY_LEFT && action == GLFW_PRESS && Angle<=0.0f){
			Angle = acos(0.0f);
			gc->go.rotation(-1, 0, 0, 0.1f);
		}
		else if(key == GLFW_KEY_UP && action == GLFW_PRESS && Angle<=0.0f){
			Angle = acos(0.0f);
			gc->go.rotation(0, 0, 1,  0.1f);
		}
		else if(key == GLFW_KEY_DOWN && action == GLFW_PRESS && Angle<=0.0f){
			Angle = acos(0.0f);
			gc->go.rotation(0, 0, -1,  0.1f);
		}
		else if(key == GLFW_KEY_T && action == GLFW_PRESS){
			isBlockView = false;
			isFollowView = false;
			isHelicopterView = false;
			vp->setTopView();
		}
		else if(key == GLFW_KEY_O && action == GLFW_PRESS){
			isBlockView = false;
			isFollowView = false;
			isHelicopterView = false;
			vp->setTowerView();
		}
		else if(key == GLFW_KEY_B && action == GLFW_PRESS){
			isBlockView = true;
			isFollowView = false;
			isHelicopterView = false;
			vp->setBlockView();
		}
		else if(key == GLFW_KEY_N && action == GLFW_PRESS){
			isBlockView = false;
			isFollowView = false;
			isHelicopterView = false;
			vp->setNormalView();
		}
		else if(key == GLFW_KEY_F && action == GLFW_PRESS){
			isFollowView = true;
			isBlockView = false;
			isHelicopterView = false;
			vp->setFollowView();
		}
		else if(key == GLFW_KEY_H && action == GLFW_PRESS){
			isFollowView = false;
			isBlockView = false;
			isHelicopterView = true;
			vp->setHelicopterView();
		}
		/*
		else if(key == GLFW_KEY_A && action == GLFW_PRESS && isHelicopterView){
			vp->rotateHelicopterCW();
			vp->setHelicopterView();
		}
		else if(key == GLFW_KEY_D && action == GLFW_PRESS && isHelicopterView){
			vp->rotateHelicopterACW();
			vp->setHelicopterView();
		}
		else if(key == GLFW_KEY_W && action == GLFW_PRESS && isHelicopterView){
			vp->zoomInHelicopter();
			vp->setHelicopterView();
		}
		else if(key == GLFW_KEY_S && action == GLFW_PRESS && isHelicopterView){
			vp->zoomOutHelicopter();
			vp->setHelicopterView();
		}*/
		//}
}

void gameController::mouse_moveCallback(GLFWwindow* window, double xpos, double ypos){

}

void gameController::mouse_buttonCallback(GLFWwindow* window, int button, int action, int mods){
	switch(button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			if(isHelicopterView){
				if(action == GLFW_PRESS){
					glfwGetCursorPos(window, &xCursor, &yCursor);
					isRotating = true;
				}
				if(action == GLFW_RELEASE){
					isRotating = false;
				}
			}
			break;
		default:
			break;
	}
}

void gameController::mouse_scrollCallback(GLFWwindow* window, double xoffset, double yoffset){
	zoomFactor = zoomFactor + yoffset;
}
