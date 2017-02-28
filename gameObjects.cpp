#include "headers.hpp"
#include "varStore.hpp"

gameObjects::gameObjects(){
	this->isFalling = false;
	this->rotateAngleFalling = 0.0f;
	this->leftAxis = vec3(0.0f, 0.0f, 1.0f);
	this->rightAxis = vec3(0.0f, 0.0f, -1.0f);
	this->upAxis = vec3(-1.0f, 0.0f, 0.0f);
	this->downAxis = vec3(1.0f, 0.0f, 0.0f);
	
}

void gameObjects::setMVP(GLint programId, mat4 modelMatrix, mat4 viewMatrix, mat4 projMatrix){
	GLint  modelLoc = glGetUniformLocation(programId, "model");
	GLint viewLoc = glGetUniformLocation(programId, "view");
  	GLint projLoc = glGetUniformLocation(programId, "projection");

  	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(modelMatrix));
  	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(viewMatrix));
  	glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projMatrix));
}

void gameObjects::displayBlock(){
	vector<pieceBlock>::iterator it;
	for(it = Blocks.begin(); it!=Blocks.end(); it++){
		
		/*mat4 modelMatrix;
		modelMatrix = scale(modelMatrix, vec3(1.0f, 1.0f, 1.0f));
		modelMatrix = translate(modelMatrix, it->position);*/

		glBindTexture(GL_TEXTURE_2D, it->texture);
		glBindVertexArray(it->VAO);
		//cout<<Angle<<" <- Angle\n";
		if(Angle>0.1f && !this->isFalling){		// & not falling
			this->rotation(this->dx, this->dy, this->dz, 0.1f);
		}
		else if(Angle<=0.1f && Angle>0 && !this->isFalling){
			this->rotation(this->dx, this->dy, this->dz, Angle);
		} 

		if(this->isFalling){
			if(this->rotateAngleFalling > 0.1f){
				this->fallRotate(this->dx, this->dz, this->transPos, this->axis, 0.1f);
			}
			else if(this->rotateAngleFalling<=0.1f && this->rotateAngleFalling > 0){
				this->fallRotate(this->dx, this->dz, this->transPos, this->axis, this->rotateAngleFalling);
			}
			
			else{
				//girata reh
				
				if(this->downFall < 20.0f){
					this->downFall += 0.1f;
					this->keepFalling();
				}
				else{
					wc->close();
				}
			}
		}

		mat4 view = vp->getView(); GLuint programId = wc->getProgramId();
		mat4 projection = vp->getProjection();
		this->setMVP(programId, it->modelMatrix, view, projection);
		glDrawElements(GL_TRIANGLES, it->Indices.size(), GL_UNSIGNED_INT, (void*)0);
		glBindVertexArray(0);
      	glBindBuffer(GL_ARRAY_BUFFER, 0);
     	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void gameObjects::rotateTileUtil(int x, int z){
	int targetX, targetZ;
	vector<switchTile>::iterator it;
	for(it = switchTiles.begin(); it != switchTiles.end(); it++){
		if(it->x == x && it->z == z){
			targetX = it->targetX; targetZ = it->targetZ;
			break;
		}
	}

	vector<Block>::iterator _it;
	for(_it = Tiles.begin(); _it != Tiles.end(); _it++){
		if(_it->z == targetX && _it->x == targetZ){
			mat4 trans, Rotate, final, rotateY;
			vec3 reduceY = vec3(0.0f, 0.2f, 0.0f);
			rotateY = translate(reduceY);
			trans = translate(vec3((GLfloat)(targetX)*1.0f + 1.0f, 0.0f, (GLfloat)(targetZ)*1.0f));
			if(_it->isAvailable){
				Rotate = rotate(acos(0.0f), -this->rightAxis);
				_it->isAvailable = false;
				this->layout[targetZ][targetX] = 0;
			}
			else{
				Rotate = rotate(acos(0.0f), -this->leftAxis);
				_it->isAvailable = true;	
				this->layout[targetZ][targetX] = 1;
			}
			final = rotateY * trans * Rotate * glm::inverse(trans) * glm::inverse(rotateY);
			_it->modelMatrix = final * _it->modelMatrix;
		}
	}

}

void gameObjects::rotateTile(){
	vector<switchTile>::iterator it;
	vector<pieceBlock>::iterator _it;
	for(it = switchTiles.begin(); it != switchTiles.end(); it++){
		for(_it = Blocks.begin(); _it != Blocks.end(); _it++){
			//cout<<"itX: "<< it->x << "itLeftX: " << _it->left.x << "itZ: "<< it->z << "itUpZ: " << _it->up.z <<"\n";
			if(abs(_it->left.x - _it->right.x) == 1 && abs(_it->up.z - _it->down.z) == 1){
				if(it->z == _it->left.x && it->x == _it->up.z){
					//rotate
					this->rotateTileUtil(it->x, it->z);
				}
			}
			else{
				if(abs(_it->left.x - _it->right.x) == 1){
					if((it->z == _it->left.x && it->x == _it->up.z) || (it->z == _it->left.x && it->x == _it->up.z + 1) ){
						// rotate
						this->rotateTileUtil(it->x, it->z);		
					}
				}
				else{
					if(abs(_it->up.z - _it->down.z) == 1){
						if((it->z == _it->left.x && it->x == _it->up.z) || (it->z == _it->left.z + 1  && it->x == _it->up.z)){
							// rotate;
							this->rotateTileUtil(it->x, it->z);
						}
					}
				}
			}
		}
	}
}

void gameObjects::rotationController(int Dx, int Dy, int Dz, GLfloat ROTATE){
	while(!(Angle<=0.001f)){
		this->rotation(Dx, Dy, Dz, ROTATE);
	}
}

void gameObjects::keepFalling(){
	vector<pieceBlock>::iterator it;
	for(it = Blocks.begin(); it != Blocks.end(); it++){
		mat4 D = translate(vec3(0.0f, -0.1f, 0.0f));
		it->modelMatrix = D * it->modelMatrix;
	}
}

void gameObjects::fallCheck(int Dx, int Dz){
	vector<pieceBlock>::iterator it;
	for(it = Blocks.begin(); it != Blocks.end(); it++){
		if(Dx != 0){
			if(Dx == 1){
				if(abs(it->left.x - it->right.x) == 1.0f){
					// standing
					int z = (int)it->left.x, x = (int)it->up.z;
					//cout<<"X : "<<x<<"\tZ: "<<z<<"layout[x][z] = "<<this->layout[x][z]<<"\n";
					if(this->layout[x][z] == 0){
						//seedhe gira do;
						if(abs(it->up.z - it->down.z) == 1 || ((abs(it->up.z - it->down.z)==2.0f && this->layout[x+1][z] == 0))){
							this->isFalling = true;
							this->keepFalling();
						}
						else if(this->layout[x][z] == 0 && this->layout[x+1][z]){
							this->dx = 0; this->dy = 0; this->dz = 1;
							this->isFalling = true;
							this->rotateAngleFalling = acos(0.0f);
							this->transPos = it->down + vec3(0.0f, 0.0f, -1.0f);
							this->axis = it->upAxis;
						}
					}
					else if(this->layout[x][z]!=0 && this->layout[x+1][z] == 0 && (abs(it->up.z - it->down.z) != 1)){
						this->dx = 0; this->dy = 0; this->dz = -1;
						this->isFalling = true;
						this->rotateAngleFalling = acos(0.0f);
						this->transPos = it->down + vec3(0.0f, 0.0f, -1.0f);
						this->axis = it->downAxis;
					}


					if(this->layout[x][z] == 2 && abs(it->up.z - it->down.z) == 1){
						this->layout[x][z] = 0;
						this->isFalling = true;
						this->keepFalling();
					}
					if(x == this->goalZ && z == this->goalX && abs(it->up.z - it->down.z) == 1){
						isWinner = true;
						Score += 1000;
					}
				}
				else{
					int z = (int)it->left.x, x = (int)it->up.z;
					//cout<<"X : "<<x<<"\tZ: "<<z<<"layout[x][z] = "<<this->layout[x][z]<<"\n";
					if(this->layout[x][z] == 0 && this->layout[x][z+1] == 0){
						// rotate about left axis; and make it Fall
						this->dx = Dx; this->dy = 0; this->dz = Dz;
						this->isFalling = true;
						this->rotateAngleFalling = acos(0.0f);
						this->transPos = it->left;
						this->axis = it->rightAxis;
					}
					else if(this->layout[x][z] != 0 && this->layout[x][z+1] == 0){
						this->dx = Dx; this->dy = 0; this->dz = Dz;
						this->isFalling = true;
						//rotate about 'left+ 1' axis; and make it fall
						this->rotateAngleFalling = acos(0.0f);
						this->transPos = it->left + vec3(1.0, 0.0f, 0.0f);
						this->axis = it->rightAxis;	
					}
				}
			}
			else{
				
				if(abs(it->left.x - it->right.x) == 1.0f){
					// standing
					int z = (int)it->right.x - 1, x = (int)it->up.z;
					//cout<<"X : "<<x<<"\tZ: "<<z<<"layout[x][z] = "<<this->layout[x][z]<<"*********\n";
					if(z <0 || this->layout[x][z] == 0){
						//seedhe gira do;
						if(z<0 || abs(it->up.z - it->down.z) == 1 || ((abs(it->up.z - it->down.z)==2.0f && this->layout[x+1][z] == 0))){
							this->isFalling = true;
							this->keepFalling();
						}
						else if(this->layout[x][z] == 0 && this->layout[x+1][z] != 0){
							this->dx = 0; this->dy = 0; this->dz = 1;
							this->isFalling = true;
							this->rotateAngleFalling = acos(0.0f);
							this->transPos = it->down + vec3(0.0f, 0.0f, -1.0f);
							this->axis = it->upAxis;
						}
					}
					else if(this->layout[x][z]!=0 && this->layout[x+1][z] == 0 && (abs(it->up.z - it->down.z) != 1)){
						this->dx = 0; this->dy = 0; this->dz = -1;
						this->isFalling = true;
						this->rotateAngleFalling = acos(0.0f);
						this->transPos = it->down + vec3(0.0f, 0.0f, -1.0f);
						this->axis = it->downAxis;
					}

					if(this->layout[x][z] == 2){
						if(abs(it->up.z - it->down.z) == 1){
							this->layout[x][z] = 0;
							this->isFalling = true;
							this->keepFalling();
						}
					}
					if(x == this->goalZ && z == this->goalX && abs(it->up.z - it->down.z) == 1 ){
						isWinner = true;
						Score += 1000;
					}
				}
				else{
					int z = (int)it->right.x - 1, x = (int)it->up.z;
					//cout<<"X : "<<x<<"\tZ: "<<z<<"layout[x][z] = "<<this->layout[x][z]<<"\t^^^^\n";
					if(z == 0 || (this->layout[x][z] != 0 && this->layout[x][z-1] == 0)){
						this->dx = Dx; this->dy = 0; this->dz = Dz;
						this->isFalling = true;
						this->rotateAngleFalling = acos(0.0f);
						this->transPos = it->right + vec3(-1.0f, 0.0f, 0.0f);
						this->axis = it->leftAxis;
					}
					else if(z<0 || (this->layout[x][z] == 0 && this->layout[x][z-1] == 0)){
						this->dx = Dx; this->dy = 0; this->dz = Dz;
						this->isFalling = true;
						this->rotateAngleFalling = acos(0.0f);
						this->transPos = it->right;
						this->axis = it->leftAxis;
					}
				}
			}
		}
		else{
			if(Dz == 1){
				if(abs(it->up.z - it->down.z) == 1.0f){
					int z = (int)it->left.x, x = (int)it->up.z;
					if(this->layout[x][z] == 0){
						if(abs(it->left.x - it->right.x) == 1 || ((abs(it->left.x - it->right.x)==2 && this->layout[x][z+1] == 0))){
							this->isFalling = true;
							this->keepFalling();
						}
						else if(this->layout[x][z] == 0 && this->layout[x][z+1] != 0){
							this->dx = 1; this->dy = 0; this->dz = 0;
							this->isFalling = true;
							this->rotateAngleFalling = acos(0.0f);
							this->transPos = it->left + vec3(1.0f, 0.0f, 0.0f);
							this->axis = it->rightAxis;
						}
					}
					else if(this->layout[x][z] == 0 && this->layout[x][z+1] != 0 && (abs(it->left.x - it->right.x) != 1)){
						this->dx = -1; this->dy = 0; this->dz = 0;
						this->isFalling = true;
						this->rotateAngleFalling = acos(0.0f);
						this->transPos = it->left + vec3(1.0f, 0.0f, 0.0f);
						this->axis = it->leftAxis;
					}


					if(this->layout[x][z] == 2 && abs(it->left.x - it->right.x) == 1){
						this->layout[x][z] = 0;
						this->isFalling = true;
						this->keepFalling();
					}
					if(x == this->goalZ && z == this->goalX && abs(it->left.x - it->right.x) == 1){
						isWinner = true;
						Score += 1000;
					}
				}
				else{
					int z = (int)it->left.x, x = (int)it->down.z-1;
					//cout<<"X : "<<x<<"\tZ: "<<z<<"layout[x][z] = "<<this->layout[x][z]<<"\n";
					if(x == 0 || (this->layout[x][z] != 0 && this->layout[x-1][z] == 0)){
						this->dx = Dx; this->dy = 0; this->dz = Dz;
						this->isFalling = true;
						this->rotateAngleFalling = acos(0.0f);
						this->transPos = it->down + vec3(0.0f, 0.0f, -1.0f);
						this->axis = it->upAxis;
					}
					else if(x<0 || (this->layout[x][z] == 0 && this->layout[x-1][z] == 0)){
						this->dx = Dx; this->dy = 0; this->dz = Dz;
						this->isFalling = true;
						this->rotateAngleFalling = acos(0.0f);
						this->transPos = it->down;
						this->axis = it->upAxis;
					}
				}
			}
			else if(Dz == -1){
				if(abs(it->up.z - it->down.z) == 1.0f){
					int z = (int)it->left.x, x = (int)it->up.z;
					if(this->layout[x][z] == 0){
						if(abs(it->left.x - it->right.x) == 1 || ((abs(it->left.x - it->right.x)==2 && this->layout[x][z+1] == 0))){
							this->isFalling = true;
							this->keepFalling();
						}
						else if(this->layout[x][z] == 0 && this->layout[x][z+1] != 0){
							this->dx = 1; this->dy = 0; this->dz = 0;
							this->isFalling = true;
							this->rotateAngleFalling = acos(0.0f);
							this->transPos = it->left + vec3(1.0f, 0.0f, 0.0f);
							this->axis = it->rightAxis;
						}
					}
					else if(this->layout[x][z] == 0 && this->layout[x][z+1] != 0 && abs(it->left.x - it->right.x) != 1){
						this->dx = -1; this->dy = 0; this->dz = 0;
						this->isFalling = true;
						this->rotateAngleFalling = acos(0.0f);
						this->transPos = it->left + vec3(1.0f, 0.0f, 0.0f);
						this->axis = it->leftAxis;
					}

					if(this->layout[x][z] == 2 && abs(it->left.x - it->right.x) == 1){
						this->layout[x][z] = 0;
						this->isFalling = true;
						this->keepFalling();
					}
					if(x == this->goalZ && z == this->goalX && abs(it->left.x - it->right.x) == 1){
						isWinner = true;
						Score += 1000;
					}
				}
				else{
					int z = (int)it->left.x, x = (int)it->up.z;
					if(this->layout[x][z] == 0 && this->layout[x+1][z] == 0){
						this->dx = Dx; this->dy = 0; this->dz = Dz;
						this->isFalling = true;
						this->rotateAngleFalling = acos(0.0f);
						this->transPos = it->up;
						this->axis = it->downAxis;
					}
					else if(this->layout[x][z] != 0 && this->layout[x+1][z] == 0){
						this->dx = Dx; this->dy = 0; this->dz = Dz;
						this->isFalling = true;
						this->rotateAngleFalling = acos(0.0f);
						this->transPos = it->up + vec3(0.0f, 0.0f, 1.0f);
						this->axis = it->downAxis;
					}
				}
			}
		}
	}
}

void gameObjects::fallRotate(int Dx, int Dz, vec3 transPos, vec3 axis, GLfloat ROTATE){
	this->rotateAngleFalling -= ROTATE;
	vector<pieceBlock>::iterator it;
	for(it = Blocks.begin(); it != Blocks.end(); it++){	
		
		mat4 trans, Rotate, final, rotateY;
		vec3 reduceY = vec3(0.0f, 0.2f, 0.0f);
		rotateY = translate(reduceY);
		trans = translate(transPos);
		Rotate = rotate(ROTATE, axis);
		final = rotateY * trans * Rotate * glm::inverse(trans) * glm::inverse(rotateY);
		it->modelMatrix  = final * it->modelMatrix;
		if(Angle<= 0.001f){
			if(Dx == 1){
				swap(it->length, it->height);
				it->left = it->right;
				it->right = vec3(it->length, 0.0f, 0.0f) + it->left;
			}
			else if(Dx == -1){
				swap(it->length, it->height);
				it->right = it->left;
				it->left = vec3(-it->length, 0.0f, 0.0f) + it->right;
			}
			else if(Dz == 1){
				swap(it->breadth, it->height);
				it->down = it->up;
				it->up = vec3(0.0f, 0.0f, -it->breadth) + it->down;
			}
			else if(Dz == -1){
				swap(it->breadth, it->height);
				it->up = it->down;
				it->down = vec3(0.0f, 0.0f, it->breadth) + it->up;
			}
		}
	}
}

void gameObjects::rotation(int Dx, int Dy, int Dz, GLfloat ROTATE){
	Angle -= ROTATE;	
	this->dx = Dx; this->dy = Dy; this->dz = Dz;
	//rate = 0.5f;
	// extra
	vector<pieceBlock>::iterator it;
	for(it = Blocks.begin(); it != Blocks.end(); it++){
		if(Dx != 0){
			mat4 trans, Rotate, final, rotateY;
			vec3 reduceY = vec3(0.0f, 0.2f, 0.0f);
			rotateY = translate(reduceY);
			if(Dx == 1){
				trans = translate(it->right);
				Rotate = rotate(ROTATE, it->rightAxis);
				final = rotateY * trans * Rotate * glm::inverse(trans) * glm::inverse(rotateY);
				it->modelMatrix  = final * it->modelMatrix;
				if(Angle<=0.001f){
					swap(it->length, it->height);
					blockLeft = it->left = it->right;
					blockRight = it->right = vec3(it->length, 0.0f, 0.0f) + it->left;
				}
				//cout<<"Right: ("<<it->right.x<<", "<<it->right.y<<", "<<it->right.z<<")\n";
				//cout<<"Left: ("<<it->left.x<<", "<<it->left.y<<", "<<it->left.z<<")\n";
				/*cout<<"Angle: "<<Angle<<"\trotating right\n";
				cout<<"-----LENGTH = "<<it->length<<"\tBREADTH = "<<it->breadth<<"\n";
				cout<<"-----HEIGHT = "<<it->height<<"\n";*/
			}
			else if(Dx == -1){
				trans = translate(it->left);
				Rotate = rotate(ROTATE, it->leftAxis);
				final = rotateY * trans * Rotate * glm::inverse(trans) * glm::inverse(rotateY);
				it->modelMatrix  = final * it->modelMatrix;
				if(Angle<=0.001f){
					swap(it->length, it->height);
					blockRight = it->right = it->left;
					blockLeft = it->left = vec3(-it->length, 0.0f, 0.0f) + it->right;
				}
				//cout<<"Right: ("<<it->right.x<<", "<<it->right.y<<", "<<it->right.z<<")\n";
				//cout<<"Left: ("<<it->left.x<<", "<<it->left.y<<", "<<it->left.z<<")\n";
				/*cout<<"Angle: "<<Angle<<"\trotating left\n";
				cout<<"-----LENGTH = "<<it->length<<"\tBREADTH = "<<it->breadth<<"\n";
				cout<<"-----HEIGHT = "<<it->height<<"\n";*/
			}
		}
		else if(Dz != 0){
			mat4 trans, Rotate, final, rotateY;
			vec3 reduceY = vec3(0.0f, 0.2f, 0.0f);
			rotateY = translate(reduceY);
			if(Dz == 1){
				trans = translate(it->up);
				Rotate = rotate(ROTATE, it->upAxis);
				final = rotateY * trans * Rotate * glm::inverse(trans) * glm::inverse(rotateY);
				it->modelMatrix = final * it->modelMatrix;
				if(Angle<=0.001f){
					swap(it->breadth, it->height);
					blockDown = it->down = it->up;
					blockUp = it->up = vec3(0.0f, 0.0f, -it->breadth) + it->down;
				}
				/*cout<<"UP: ("<<it->up.x<<", "<<it->up.y<<", "<<it->up.z<<")\n";
				cout<<"DOWN: ("<<it->down.x<<", "<<it->down.y<<", "<<it->down.z<<")\n";
				cout<<"-----LENGTH = "<<it->length<<"\tBREADTH = "<<it->breadth<<"\n";
				cout<<"-----HEIGHT = "<<it->height<<"\n";*/
			}
			else if(Dz == -1){
				trans = translate(it->down);
				Rotate = rotate(ROTATE, it->downAxis);
				final = rotateY * trans * Rotate * glm::inverse(trans) * glm::inverse(rotateY);
				it->modelMatrix = final * it->modelMatrix;
				if(Angle<=0.001f){
					swap(it->breadth, it->height);
					blockUp = it->up = it->down;
					blockDown = it->down = vec3(0.0f, 0.0f, it->breadth) + it->up;
				}	
				/*cout<<"UP: ("<<it->up.x<<", "<<it->up.y<<", "<<it->up.z<<")\n";
				cout<<"DOWN: ("<<it->down.x<<", "<<it->down.y<<", "<<it->down.z<<")\n";
				cout<<"-----LENGTH = "<<it->length<<"\tBREADTH = "<<it->breadth<<"\n";
				cout<<"-----HEIGHT = "<<it->height<<"\n";*/
			}
		}
	}
	//Angle = 0.0f;
	if(Angle == 0.0f){
		blockDx = Dx, blockDz = Dz;
		if(isBlockView){
			vp->setBlockView();
		}
		if(isFollowView){
			vp->setFollowView();
		}
		this->fallCheck(Dx, Dz);
	}
	if(Angle == 0.0f){
		this->rotateTile();
	}

}

void gameObjects::rotateUpdater(){
	vector<pieceBlock>::iterator it;
	for(it = Blocks.begin(); it != Blocks.end(); it++){
		if(dx != 0){
			mat4 trans, Rotate, final;
			if(dx == 1){
				trans = translate(it->right);
				Rotate = rotate(rate, it->rightAxis);
				final = trans * Rotate * glm::inverse(trans);
			}
			else if(dx == -1){
				trans = translate(it->left);
				Rotate = rotate(rate, it->leftAxis);
				final = trans * Rotate * glm::inverse(trans);
			}
		}
		else if(dz != 0){

		}
	}
}

void gameObjects::initBlock(){
	pieceBlock newBlock;
	glGenVertexArrays(1, &newBlock.VAO);
	glBindVertexArray(newBlock.VAO);

	this->buildBlock(newBlock.Vertices, newBlock.Indices);
	
	// Setting Axis
	newBlock.leftAxis = vec3(0.0f, 0.0f, 1.0f);
	newBlock.rightAxis = vec3(0.0f, 0.0f, -1.0f);
	newBlock.upAxis = vec3(-1.0f, 0.0f, 0.0f);
	newBlock.downAxis = vec3(1.0f, 0.0f, 0.0f);

	// Setting Coordinates
	blockRight = newBlock.right = vec3(4.0f, 0.0f, 0.0f);
	blockLeft =  newBlock.left = vec3(3.0f, 0.0f, 0.0f);
	blockUp = newBlock.up = vec3(0.0f, 0.0f, 1.0f);
	blockDown = newBlock.down = vec3(0.0f, 0.0f, 2.0f);

	newBlock.length = 1.0f, newBlock.breadth = 1.0f, newBlock.height = 2.0f;

	newBlock.position = vec3(3.0f, 0.0f, 1.0f);
	
	newBlock.modelMatrix = scale(newBlock.modelMatrix, vec3(1.0f, 1.0f, 1.0f));
	newBlock.modelMatrix = translate(newBlock.modelMatrix, newBlock.position);

	glGenBuffers(1, &newBlock.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, newBlock.VBO);
	glBufferData(GL_ARRAY_BUFFER, newBlock.Vertices.size()*sizeof(GLfloat), newBlock.Vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &newBlock.EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newBlock.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, newBlock.Indices.size()*sizeof(GLuint), newBlock.Indices.data(), GL_STATIC_DRAW);

	glGenTextures(1, &newBlock.texture);
	glBindTexture(GL_TEXTURE_2D, newBlock.texture);

	unsigned char* image = SOIL_load_image("Wood.jpg", &newBlock.texWidth, &newBlock.texHeight, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newBlock.texWidth, newBlock.texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)0);
  	glEnableVertexAttribArray(0);
  	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
  	glEnableVertexAttribArray(1);
  	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(6*sizeof(GLfloat)));
  	glEnableVertexAttribArray(2);

  	glBindVertexArray(0);
  	glBindBuffer(GL_ARRAY_BUFFER, 0);
  	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  	this->Blocks.push_back(newBlock);
}

void gameObjects::buildBlock(vector<GLfloat> &Vertices, vector<GLuint> &Indices){

	int x = 1, z = 0;
	for(int i=0;i <4;i++){
		if(i&1){
			z ^= 1;
		}
		else{
			x ^= 1;
		}
		Vertices.push_back((GLfloat)(x)); Vertices.push_back(0.2f); Vertices.push_back((GLfloat)(z)); 
		Vertices.push_back((GLfloat)(x)); Vertices.push_back(0.0f); Vertices.push_back((GLfloat)(z));
		Vertices.push_back((GLfloat)(x)); Vertices.push_back((GLfloat)(z));
	}

	x = 1, z = 0;
	for(int i=0;i <4;i++){
		if(i&1){
			z ^= 1;
		}
		else{
			x ^= 1;
		}
		Vertices.push_back((GLfloat)(x)); Vertices.push_back(2.2f); Vertices.push_back((GLfloat)(z)); 
		//Vertices.push_back((GLfloat)(x)); Vertices.push_back(0.2f); Vertices.push_back((GLfloat)(z));

		Vertices.push_back((GLfloat)(x)); Vertices.push_back(0.0f); Vertices.push_back((GLfloat)(z));
		Vertices.push_back((GLfloat)(x)); Vertices.push_back((GLfloat)(z));
	}

	
	Indices.push_back(0);	Indices.push_back(1);	Indices.push_back(2);
	Indices.push_back(0);	Indices.push_back(3);	Indices.push_back(2);

	Indices.push_back(4);	Indices.push_back(5);	Indices.push_back(6);
	Indices.push_back(4);	Indices.push_back(7);	Indices.push_back(6);

	Indices.push_back(0);	Indices.push_back(1);	Indices.push_back(5);
	Indices.push_back(0);	Indices.push_back(4);	Indices.push_back(5);

	Indices.push_back(3);	Indices.push_back(2);	Indices.push_back(6);
	Indices.push_back(3);	Indices.push_back(7);	Indices.push_back(6);

	Indices.push_back(1);	Indices.push_back(2);	Indices.push_back(6);
	Indices.push_back(1);	Indices.push_back(5);	Indices.push_back(6);

	Indices.push_back(0);	Indices.push_back(3);	Indices.push_back(7);
	Indices.push_back(0);	Indices.push_back(4);	Indices.push_back(7);	
}

void gameObjects::initBlock(vec3 position, int Tex){
	Block newBlock;
	glGenVertexArrays(1, &newBlock.VAO);
	glBindVertexArray(newBlock.VAO);

	this->buildTileBlock(newBlock.Vertices, newBlock.Indices);

	newBlock.position = position;
	newBlock.texType = Tex;

	newBlock.modelMatrix = scale(newBlock.modelMatrix, vec3(1.0f, 1.0f, 1.0f));
	newBlock.modelMatrix = translate(newBlock.modelMatrix, position);

	newBlock.x = (int)position.z; newBlock.z = (int)position.x;	
	newBlock.isAvailable = true;

	glGenBuffers(1, &newBlock.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, newBlock.VBO);
	glBufferData(GL_ARRAY_BUFFER, newBlock.Vertices.size()*sizeof(GLfloat), newBlock.Vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &newBlock.EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newBlock.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, newBlock.Indices.size()*sizeof(GLuint), newBlock.Indices.data(), GL_STATIC_DRAW);

	glGenTextures(1, &newBlock.texture);
	glBindTexture(GL_TEXTURE_2D, newBlock.texture);

	if(Tex == 1){
		unsigned char* image = SOIL_load_image("Tile2.jpg", &newBlock.texWidth, &newBlock.texHeight, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newBlock.texWidth, newBlock.texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else if(Tex == 2){
		unsigned char* image = SOIL_load_image("orangeTile1.jpg", &newBlock.texWidth, &newBlock.texHeight, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newBlock.texWidth, newBlock.texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);	
	}
	else if(Tex == 3){
		unsigned char* image = SOIL_load_image("switchTile.jpg", &newBlock.texWidth, &newBlock.texHeight, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newBlock.texWidth, newBlock.texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);	
	}		

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)0);
  	glEnableVertexAttribArray(0);
  	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
  	glEnableVertexAttribArray(1);
  	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(6*sizeof(GLfloat)));
  	glEnableVertexAttribArray(2);

  	glBindVertexArray(0);
  	glBindBuffer(GL_ARRAY_BUFFER, 0);
  	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  	this->Tiles.push_back(newBlock);
}

void gameObjects::buildTileBlock(vector<GLfloat> &Vertices, vector<GLuint> &Indices){

	int x = 1, z = 0;
	for(int i=0;i <4;i++){
		if(i&1){
			z ^= 1;
		}
		else{
			x ^= 1;
		}
		Vertices.push_back((GLfloat)(x)); Vertices.push_back(0.0f); Vertices.push_back((GLfloat)(z)); 
		Vertices.push_back((GLfloat)(x)); Vertices.push_back(0.0f); Vertices.push_back((GLfloat)(z));
		Vertices.push_back((GLfloat)(x)); Vertices.push_back((GLfloat)(z));
	}

	x = 1, z = 0;
	for(int i=0;i <4;i++){
		if(i&1){
			z ^= 1;
		}
		else{
			x ^= 1;
		}
		//Vertices.push_back((GLfloat)(x)); Vertices.push_back(1.0f); Vertices.push_back((GLfloat)(z)); 
		Vertices.push_back((GLfloat)(x)); Vertices.push_back(0.2f); Vertices.push_back((GLfloat)(z));

		Vertices.push_back((GLfloat)(x)); Vertices.push_back(0.0f); Vertices.push_back((GLfloat)(z));
		Vertices.push_back((GLfloat)(x)); Vertices.push_back((GLfloat)(z));
	}

	
	Indices.push_back(0);	Indices.push_back(1);	Indices.push_back(2);
	Indices.push_back(0);	Indices.push_back(3);	Indices.push_back(2);

	Indices.push_back(4);	Indices.push_back(5);	Indices.push_back(6);
	Indices.push_back(4);	Indices.push_back(7);	Indices.push_back(6);

	Indices.push_back(0);	Indices.push_back(1);	Indices.push_back(5);
	Indices.push_back(0);	Indices.push_back(4);	Indices.push_back(5);

	Indices.push_back(3);	Indices.push_back(2);	Indices.push_back(6);
	Indices.push_back(3);	Indices.push_back(7);	Indices.push_back(6);

	Indices.push_back(1);	Indices.push_back(2);	Indices.push_back(6);
	Indices.push_back(1);	Indices.push_back(5);	Indices.push_back(6);

	Indices.push_back(0);	Indices.push_back(3);	Indices.push_back(7);
	Indices.push_back(0);	Indices.push_back(4);	Indices.push_back(7);
}

void gameObjects::initLayout(){
	memset(this->layout, 0, sizeof(this->layout));
	this->layout[0][2] = 3; this->layout[0][3] = this->layout[0][4] = this->layout[0][5] = this->layout[0][6] = 1;
	this->layout[1][2] = this->layout[1][3] = this->layout[1][4] = this->layout[1][5] = this->layout[1][6] = 1;
	this->layout[2][2] = this->layout[2][3] = this->layout[2][4] = this->layout[2][5] = this->layout[2][6] = 1;
	this->layout[3][6] = 1; this->layout[3][7] = this->layout[3][8] = 2;
	this->layout[4][7] = this->layout[4][8] = 2;	
	this->layout[5][0] = this->layout[5][1] = this->layout[5][2] = this->layout[5][3] = 1; this->layout[5][7] = this->layout[5][8] = 1;
	this->layout[6][0] = this->layout[6][2] = this->layout[6][3] = 1; this->layout[6][7] = this->layout[6][8] = 1;
	this->layout[7][0] = this->layout[7][1] = this->layout[7][2] = this->layout[7][3] = 1; this->layout[7][7] = this->layout[7][8] = 1;
	this->layout[8][2] = this->layout[8][3] = 1; this->layout[8][7] = this->layout[8][8] = 2;	
	this->layout[9][2] = this->layout[9][3] = 2; this->layout[9][6] = 1; this->layout[9][7] = this->layout[9][8] = 2;
	this->layout[10][0] = this->layout[10][1] = this->layout[10][2] = this->layout[10][3] = 2; this->layout[10][4] = this->layout[10][5] = this->layout[10][6] = 1;
	this->layout[11][0] = this->layout[11][1] = this->layout[11][2] = this->layout[11][3] = 2; this->layout[11][4] = this->layout[11][5] = this->layout[11][6] = 1;
	this->layout[12][0] = 2;  this->layout[12][1] = 1; this->layout[12][2] = this->layout[12][3] = 2;
	this->layout[13][0] = this->layout[13][1] = this->layout[13][2] = this->layout[13][3] = 2;
	this->goalX = 1, this->goalZ = 6;


	switchTile newSwitchTile;
	newSwitchTile.x = 0; newSwitchTile.z = 2; newSwitchTile.targetX = 6; newSwitchTile.targetZ = 3;
	switchTiles.push_back(newSwitchTile);
}

void gameObjects::initTiles(){
	this->initLayout();
	for(int x=0; x<20; x++){
		for(int z=0; z<20; z++){
			if(this->layout[x][z]){
				vec3 position = vec3((GLfloat)(z)*1.0f, 0.0f, (GLfloat)(x)*1.0f);
				initBlock(position, this->layout[x][z]);
			}
		}	
	}
}

void gameObjects::displayTiles(){
	vector<Block>::iterator it;
	for(it = Tiles.begin(); it!=Tiles.end(); it++){
		//mat4 modelMatrix;
		//modelMatrix = scale(modelMatrix, vec3(1.0f, 1.0f, 1.0f));
		//modelMatrix = translate(modelMatrix, it->position);
		int x = (int)it->position.z, z = (int)it->position.x;
		if(this->layout[x][z] == 0 && it->texType == 2){
			glBindTexture(GL_TEXTURE_2D, it->texture);
			glBindVertexArray(it->VAO);

			mat4 view = vp->getView(); GLuint programId = wc->getProgramId();
			mat4 projection = vp->getProjection();
			it->modelMatrix = translate(it->modelMatrix, vec3(0.0f, -0.2f, 0.0f));
			this->setMVP(programId, it->modelMatrix, view, projection);
			glDrawElements(GL_TRIANGLES, it->Indices.size(), GL_UNSIGNED_INT, (void*)0);
			glBindVertexArray(0);
	      	glBindBuffer(GL_ARRAY_BUFFER, 0);
	     	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
     	}
     	else{
			glBindTexture(GL_TEXTURE_2D, it->texture);
			glBindVertexArray(it->VAO);

			mat4 view = vp->getView(); GLuint programId = wc->getProgramId();
			mat4 projection = vp->getProjection();
			this->setMVP(programId, it->modelMatrix, view, projection);
			glDrawElements(GL_TRIANGLES, it->Indices.size(), GL_UNSIGNED_INT, (void*)0);
			glBindVertexArray(0);
	      	glBindBuffer(GL_ARRAY_BUFFER, 0);
	     	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
     	}
	}
}
