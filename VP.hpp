#include "headers.hpp"

#ifndef MVP_H
#define MVP_H

class VP{
	mat4 view;
	mat4 projection;
	vec3 pos; // camera pos;
	GLfloat helicopterRadius; 
	GLfloat helicopterTheta;
	GLfloat helicopterHeight;
	vec3 helicopterPosition;
	vec3 helicopterCentre;
	double prevZoomFactor;

public:
	VP();

	void setView(mat4 view);
	mat4 getView();

	void setTopView();
	void setTowerView();
	void setBlockView();
	void setNormalView();
	void setFollowView();

	void setHelicopterView();
	void rotateHelicopterCW();
	void rotateHelicopterACW();
	void zoomInHelicopter();
	void zoomOutHelicopter();

	void setProjection(mat4 projection);
	mat4 getProjection();
};

#endif