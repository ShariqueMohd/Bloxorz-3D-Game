#include "headers.hpp"

#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

typedef struct block Block;
typedef struct pBlock pieceBlock;
typedef struct sTile switchTile;

struct block{
	GLuint VAO, VBO, EBO, texture;
	int texType;
	vec3 position;
	int x, z;		// based on index of layout
	mat4 modelMatrix;	// changed
	bool isAvailable;
	int texWidth, texHeight;
	vector<GLfloat> Vertices;
	vector<GLuint> Indices;
};

struct pBlock{
	GLuint VAO, VBO,EBO, texture;
	vec3 position;
	int texWidth, texHeight;
	mat4 modelMatrix;
	vec3 rightAxis, leftAxis, upAxis, downAxis;
	vec3 right, left, up, down; 
	GLfloat length, breadth, height; 
	vector<GLfloat> Vertices;
	vector<GLuint> Indices;
};

struct sTile{
	int x, z;
	int targetX, targetZ;
};

class gameObjects{
	
public:
	int dx, dy, dz;
	bool isFalling;
	GLfloat rotateAngleFalling;
	vec3 transPos, axis;
	GLfloat downFall = 0.0f;
	vec3 rightAxis, leftAxis, upAxis, downAxis;
	
	int goalX, goalZ;

	gameObjects();
	void setMVP(GLint programId, mat4 modelMatrix, mat4 viewMatrix, mat4 projMatrix);

	vector<pieceBlock> Blocks;	
	void initBlock();
	void initBlock(vec3 position, int Tex);
	void buildBlock(vector<GLfloat> &Vertices, vector<GLuint> &Indices);
	void displayBlock();

	void rotation(int Dx, int Dy, int Dz, GLfloat ROTATE);
	void rotationController(int Dx, int Dy, int Dz, GLfloat ROTATE);
	void rotateUpdater();

	void fallCheck(int Dx, int Dz);
	void fallRotate(int Dx, int Dz, vec3 transPos, vec3 axis, GLfloat ROTATE);
	void keepFalling();

	int layout[20][20];
	vector<Block> Tiles;
	void initLayout();
	void initTiles();
	void buildTileBlock(vector<GLfloat> &Vertices, vector<GLuint> &Indices);
	void displayTiles();

	vector<switchTile> switchTiles;
	void rotateTile();
	void rotateTileUtil(int x, int z);
};

#endif