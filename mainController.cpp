#include "headers.hpp"
#include "varStore.hpp"

windowController *wc;
gameController *gc;
VP *vp;
GLfloat Angle, rate, newTime, oldTime;
int dx, dy, dz;
int blockDx, blockDz;
vec3 blockLeft, blockRight, blockUp, blockDown;

double zoomFactor;
double xCursor, yCursor;
bool isBlockView = false, isFollowView = false, isHelicopterView = false;
bool isWinner = false;
bool isRotating = false;
int Score = 0;
GLfloat prevRotatingAngle = 0.0f;

int main(){
	
	wc = new windowController("Blockerz", 800, 600);
	wc->setWindowContext();
	wc->keyCallback(gameController::key_callback);
	wc->mouseCallback(gameController::mouse_moveCallback, gameController::mouse_buttonCallback, gameController::mouse_scrollCallback);

	Shader ourShader("shader.vs", "shader.frag");
	wc->setProgramId(ourShader.Program);
	wc->setViewPort();

	vp = new VP();
	gc = new gameController();

	oldTime = 0.0f;
	blockDx = 1; blockDz = 0;
	while(!wc->isWindowClosed()){
		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    	ourShader.Use();
    	gc->displayAll();

    	glfwSwapBuffers(wc->getWindow());
    	
	}
	if(isWinner){
    	cout<<"You Win!!!!\n";
    	cout<<Score<<"\n";
    }

}