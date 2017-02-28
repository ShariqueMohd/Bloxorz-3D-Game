#include "headers.hpp"
#include "varStore.hpp"

VP::VP(){
	this->projection = perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
	this->view = lookAt(vec3(0.0f, 10.0f, 20.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	this->helicopterRadius = 20.0f;
	this->helicopterTheta = 0.0f;
	this->helicopterHeight = 10.0f;
	this->helicopterPosition = vec3(10.0f, 15.0f, 0.0f);
	this->helicopterCentre = vec3(6.0f, 0.0f, 4.0f); 
	this->prevZoomFactor = 0.0f;
	glEnable(GL_DEPTH_TEST);
}

void VP::setNormalView(){
	this->view = lookAt(vec3(0.0f, 10.0f, 20.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
}

void VP::setTopView(){
	this->view = lookAt(vec3(4.0f, 20.0f, 6.0f), vec3(4.0f, 0.0f, 6.0f), vec3(0.0f, 0.0f, -1.0f));
}

void VP::setTowerView(){
	vec3 C = glm::cross(vec3(5.0f, 20.0f, 4.0f), vec3(5.0f, 20.0f, -4.0f));
	this->view = lookAt(vec3(5.0f, 20.0f, 4.0f), vec3(0.0f, 0.0f, 0.0f), C);
}

void VP::setBlockView(){
	vec3 pos, target;
	if(blockDx == 1)
		pos = vec3(blockRight.x, (abs(blockLeft.x - blockRight.x) ==1 && abs(blockUp.z - blockDown.z) == 1)?2.0f:1.0f, (blockUp.z+blockDown.z)/2.0);
	else if(blockDx == -1)
		pos = vec3(blockLeft.x, (abs(blockLeft.x - blockRight.x) ==1 && abs(blockUp.z - blockDown.z) == 1)?2.0f:1.0f, (blockUp.z+blockDown.z)/2.0);
	else if(blockDz == 1)
		pos = vec3((blockLeft.x+blockRight.x)/2.0, (abs(blockLeft.x - blockRight.x) ==1 && abs(blockUp.z - blockDown.z) == 1)?2.0f:1.0f, blockUp.z);
	else if(blockDz == -1)
		pos = vec3((blockLeft.x+blockRight.x)/2.0, (abs(blockLeft.x - blockRight.x) ==1 && abs(blockUp.z - blockDown.z) == 1)?2.0f:1.0f, blockDown.z);
	
	if(blockDx == 1){
		target = vec3(blockRight.x+2.0f, 0.0f, (blockUp.z+blockDown.z)/2.0);
	}
	else if(blockDx == -1){
		target = vec3(blockLeft.x-2.0f, 0.0f, (blockUp.z+blockDown.z)/2.0);
	}
	else if(blockDz == 1){
		target = vec3((blockLeft.x+blockRight.x)/2.0, 0.0f, blockUp.z-2.0f);
	}
	else if(blockDz == -1){
		target = vec3((blockLeft.x+blockRight.x)/2.0, 0.0f, blockDown.z+2.0f);	
	}

	this->view = lookAt(pos, target, vec3(0.0f, 1.0f, 0.0f));
}

void VP::setFollowView(){
	vec3 pos, target;
	if(blockDx == 1)
		pos = vec3(blockRight.x-5.0f, ((abs(blockLeft.x - blockRight.x) ==1 && abs(blockUp.z - blockDown.z) == 1)?2.0f:1.0f) + 3.0f, (blockUp.z+blockDown.z)/2.0);
	else if(blockDx == -1)
		pos = vec3(blockLeft.x+5.0f, ((abs(blockLeft.x - blockRight.x) ==1 && abs(blockUp.z - blockDown.z) == 1)?2.0f:1.0f) + 3.0f, (blockUp.z+blockDown.z)/2.0);
	else if(blockDz == 1)
		pos = vec3((blockLeft.x+blockRight.x)/2.0, ((abs(blockLeft.x - blockRight.x) ==1 && abs(blockUp.z - blockDown.z) == 1)?2.0f:1.0f) + 3.0f, blockUp.z + 5.0f);
	else if(blockDz == -1)
		pos = vec3((blockLeft.x+blockRight.x)/2.0, ((abs(blockLeft.x - blockRight.x) ==1 && abs(blockUp.z - blockDown.z) == 1)?2.0f:1.0f) + 3.0f, blockDown.z - 5.0f);
	
	if(blockDx == 1){
		target = vec3(blockRight.x+2.0f, 0.0f, (blockUp.z+blockDown.z)/2.0);
	}
	else if(blockDx == -1){
		target = vec3(blockLeft.x-2.0f, 0.0f, (blockUp.z+blockDown.z)/2.0);
	}
	else if(blockDz == 1){
		target = vec3((blockLeft.x+blockRight.x)/2.0, 0.0f, blockUp.z-2.0f);
	}
	else if(blockDz == -1){
		target = vec3((blockLeft.x+blockRight.x)/2.0, 0.0f, blockDown.z+2.0f);	
	}

	this->view = lookAt(pos, target, vec3(0.0f, 1.0f, 0.0f));	
}

void VP::setHelicopterView(){
	GLfloat camX = this->helicopterRadius*cos(this->helicopterTheta);
	GLfloat camZ = this->helicopterRadius*sin(this->helicopterTheta);
	// set default locations
	mat4 modelMatrix;
	double xCurr, yCurr;
	glfwGetCursorPos(wc->getWindow(), &xCurr, &yCurr);
	prevRotatingAngle = xCurr - xCursor;
	glfwGetCursorPos(wc->getWindow(), &xCursor, &yCursor);
	if(prevRotatingAngle > 0.0f){
		modelMatrix = translate(this->helicopterCentre) * rotate(prevRotatingAngle, vec3(0.0f, -1.0f, 0.0f)) * glm::inverse(translate(this->helicopterCentre));
		this->helicopterPosition = vec3(modelMatrix * vec4(this->helicopterPosition, 1.0f));
	}
	else if(prevRotatingAngle < 0.0f){
		modelMatrix = translate(this->helicopterCentre) * rotate(prevRotatingAngle, vec3(0.0f, 1.0f, 0.0f)) * glm::inverse(translate(this->helicopterCentre));
		this->helicopterPosition = vec3(modelMatrix * vec4(this->helicopterPosition, 1.0f));		
	}
	this->helicopterTheta = 0.0f;
	this->view = lookAt(this->helicopterPosition, vec3(4.0f, 0.0f, 6.0f), vec3(0.0f, 1.0f, 0.0f));
}

void VP::rotateHelicopterCW(){
	this->helicopterTheta +=  0.1f;
}

void VP::rotateHelicopterACW(){
	this->helicopterTheta -= 0.1f;
}

void VP::zoomInHelicopter(){
	vec3 effective = this->helicopterPosition - this->helicopterCentre;
	GLfloat h = this->helicopterHeight;
	double diff = this->prevZoomFactor - zoomFactor;
	this->prevZoomFactor = zoomFactor;
	this->helicopterHeight -= diff;
	effective.x = ((h-diff)/h) * effective.x; effective.y = ((h-diff)/h) * effective.y; effective.z = ((h-diff)/h) * effective.z;
	this->helicopterPosition = effective + this->helicopterCentre;
	this->view = lookAt(this->helicopterPosition, vec3(4.0f, 0.0f, 6.0f), vec3(0.0f, 1.0f, 0.0f));
}

void VP::zoomOutHelicopter(){
	vec3 effective = this->helicopterPosition - this->helicopterCentre;
	GLfloat h = this->helicopterHeight;
	this->helicopterHeight += 0.2f;
	effective.x = ((h+0.2)/h) * effective.x; effective.y = ((h+0.2)/h) * effective.y; effective.z = ((h+0.2)/h) * effective.z;
	this->helicopterPosition = effective + this->helicopterCentre;
	this->view = lookAt(this->helicopterPosition, vec3(4.0f, 0.0f, 6.0f), vec3(0.0f, 1.0f, 0.0f));
}

void VP::setView(mat4 view){
	this->view = view;
}

mat4 VP::getView(){
	return this->view;
}

void VP::setProjection(mat4 projection){
	this->projection = projection;
}

mat4 VP::getProjection(){
	return this->projection;
}