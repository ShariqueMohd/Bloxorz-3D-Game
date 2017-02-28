#include "windowController.hpp"
#include "gameController.hpp"
#include "VP.hpp"
#include "gameObjects.hpp"

extern windowController *wc;
extern gameController *gc;
extern VP *vp;
extern GLfloat Angle, rate, oldTime, newTime;
extern int dx, dy, dz;
extern int blockDx, blockDz;
extern vec3 blockLeft, blockRight, blockUp, blockDown;
extern double zoomFactor;
extern double xCursor, yCursor;

extern bool isBlockView, isFollowView, isHelicopterView;
extern bool isRotating;
extern bool isWinner;
extern int Score;
extern GLfloat prevRotatingAngle;